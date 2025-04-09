#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProjListA::Init(VOID* pCA)
{

if (DEBUG + DEBUG_PROJECTILE_A >= 1)
	Init_LogFile();
		
	pCAccess = pCA;
	
	//allocate memory for list anchors
	pPRL_First = new cProjectileA;
	pPRL_Last = new cProjectileA;

	//link them together
	pPRL_First->pPrev = NULL;
	pPRL_First->pNext = (VOID*)pPRL_Last;
	pPRL_Last->pPrev = (VOID*)pPRL_First;
	pPRL_Last->pNext = NULL;

	//no units have been assigned yet
	iID_Last_Assigned = -1;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProjectileA::Init(VOID* pCA)
{

	pCAccess = pCA;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProjectileA::RegisterHitOnUnit(cUnitA* pUnit, double ddDamage, double contactpoint_abs_x, double contactpoint_abs_y)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	PROJTYPEINFO* pInfo = &pCA->pPTypeTable->ProjTypes[iType];


		//////////////////////////////////////////////////////////////////////////////
		//flag the unit that it has been struck by a projectile
		pUnit->L6_LastFlagged = pCA->pTimer->iCurrTick;
		//if the unit is moving (external command CQ2TYPE_MOVE_B), set it to L6_DISTRACT
		BOOL bInitiateL6 = FALSE;
		if (pUnit->wExternalOrderIssued == CQ2TYPE_MOVE_B)
		if (pUnit->bExternalOrderCompleted == FALSE)
		if (pUnit->bMode_L6 != UMODE_L6_DISTRACTED)
		{
			//if the unit is within five squares of its target
		    if (pUnit->Calc_Distance(pUnit->Pos_sqcurr.x, pUnit->Pos_sqcurr.y, pUnit->Pos_sqtarget.x, pUnit->Pos_sqtarget.y) < 5)
			{
				bInitiateL6 = TRUE;
			}
			//if L2 mode is IDLE
			if (pUnit->bMode_L2 == UMODE_L2_IDLE)
				bInitiateL6 = TRUE;
		}


		if (bInitiateL6 == TRUE)
		{
			if (pUnit->pGuardInfo != NULL)
			{
				delete pUnit->pGuardInfo;
				pUnit->pGuardInfo = NULL;
			} 
			pUnit->pGuardInfo = new UNITGUARDINFO;
			pUnit->pGuardInfo->Pos_guard.index = Pos_square.index; 
			pUnit->pGuardInfo->Pos_guard.Calc_xy(pCA->pGameWorld->iWidth);
			pUnit->L6_sqOriginalTarget.index = pUnit->Pos_sqtarget.index;
			pUnit->L6_sqOriginalTarget.Calc_xy(pCA->pGameWorld->iWidth);
			pUnit->bMode_L6 = UMODE_L6_DISTRACTED;
		}
		////////////////////////////////////////////////////////////

		//reduce the unit's HP
		double fHPOld = pUnit->fHP;
		pUnit->fHP -= (ddDamage - pUnit->fArmor); //temper the damage done by the armor on the unit


		if ( ((fHPOld*3) > pUnit->fHPMax) && ((pUnit->fHP*3) < pUnit->fHPMax))
		if (pUnit->iControl == pCA->pGameOptions->iControl)
		{
				//set acknowledgement
				int iRet = pCA->pAckTable->Get_Ack(FALSE, pUnit->iType, ACK_TYPE_STATUS_UNIT_ALERT, pUnit->iPersonality, rand() % 3);
				pCA->pCommQueue->Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, pUnit->iID, 0, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
		}



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cProjectileA::Run_Cycle()
{
	int retval = 1; //nominally return "i have not been destroyed"
	SQCOORDSET Sqcoord; //sq coordinates of the projectile
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cUnitA*	pUnit; //unit that might be in the square the projectile is in
	cStructureA* pStruct;
	PROJTYPEINFO* pInfo = &pCA->pPTypeTable->ProjTypes[iType];

	//accelerate
	fSpeed += fAccel;

	if (fMaxSpeed > 0)
	if (fSpeed > fMaxSpeed)
		fSpeed = fMaxSpeed;

	//temp(06.21.2002) (just to see if I can make moving projectiles)
	Pos_ABS.x += Heading.x * fSpeed;
	Pos_ABS.y += Heading.y * fSpeed;

	fRemrange -= fSpeed;

	//temp (06.13.2002)
	Update_Position();
	iDrawFrame = 0;

if (Pos_square.x >= 0 && Pos_square.x < pCA->pGameWorld->iWidth && Pos_square.y >= 0 && Pos_square.y < pCA->pGameWorld->iHeight)
{
	//draw trail if necessary
	if (int(fRemrange) % 3 == 0)
	{

	if (pCA->pPTypeTable->ProjTypes[iType].iAnimType_Trail >= 0)
		pCA->pEventBuffer->Add_Event(pCA->pPTypeTable->ProjTypes[iType].iAnimType_Trail, pCA->pTimer->iCurrTick, -1, 0, 3, Pos_ABS.x - Heading.x, Pos_ABS.y - Heading.y);
	}

	//determine if the projectile has struck an enemy unit
	pUnit = (cUnitA*)pCA->pGameWorld->pMap[Pos_square.index].pOccupant_Unit;
	pStruct = (cStructureA*)pCA->pGameWorld->pMap[Pos_square.index].pOccupant_Structure;

	BOOL bDetonate = FALSE;

	if (pInfo->bLineOfFire == TRUE)
	{
		if (pUnit != NULL)
		if (pUnit->iControl != iControl)
			bDetonate = TRUE;
		if (pStruct != NULL)
		if (pStruct->iControl != iControl)
			bDetonate = TRUE;
	}
	if (pInfo->bLineOfFire == FALSE)
	{
		int iX = Pos_target.x / TILE_SIZE;
		int iY = Pos_target.y / TILE_SIZE;


	int iXOffset = (Pos_target.x  - (iX * TILE_SIZE));
	int iYOffset = (Pos_target.y - (iY * TILE_SIZE));

if (iXOffset > (TILE_SIZE/2))
	{
	iX++;
	}
if (iYOffset > (TILE_SIZE/2))
	{
		iY++;
	}

		SQCOORDSET Pos_sqtarget;
		Pos_sqtarget.x = iX;
		Pos_sqtarget.y = iY;
		Pos_sqtarget.Calc_Index(pCA->pGameWorld->iWidth);
		if (Pos_square.index  == Pos_sqtarget.index)
			bDetonate = TRUE;
	}

	if (bDetonate == TRUE)	
	{
		//destroy the projectile
		retval = 0;

		//HIT UNIT here
		if (pUnit != NULL)
		if (pUnit->iControl != iControl)
			RegisterHitOnUnit(pUnit, iDamage, Pos_ABS.x, Pos_ABS.y);
		//HIT STRUCTURE here		
		if (pStruct != NULL)
		if (pStruct->iControl != iControl)
			pStruct->iHP -= iDamage;

		//TEST  play animation at contact point
		if (pInfo->iAnimType_Detonate >= 0)
		{
				int iVol = pCA->pDA_Audio->Determine_Standard_Volume(Pos_square.x, Pos_square.y);
				pCA->pEventBuffer->Add_Event(pInfo->iAnimType_Detonate, pCA->pTimer->iCurrTick, pInfo->iSoundSeg_Detonate, iVol, NUM_ZLEVELS-1, Pos_ABS.x, Pos_ABS.y);
		}

	}
	//if the projectile is being destroyed, apply secondary damage
	if (retval == 0)
	{
		//now apply secondary damage
		for (int lc7 = 0; lc7 < pInfo->iNumSecondarySquares; lc7++)
		{
			SQCOORDSET Pos_secondary;
			Pos_secondary.x = Pos_square.x + pInfo->Pos_secondarydamage[lc7].x;
			Pos_secondary.y = Pos_square.y + pInfo->Pos_secondarydamage[lc7].y;
			Pos_secondary.Calc_Index(pCA->pGameWorld->iWidth);

			if (Pos_secondary.x >= 0)
			if (Pos_secondary.x < pCA->pGameWorld->iWidth)
			if (Pos_secondary.y >= 0)
			if (Pos_secondary.y < pCA->pGameWorld->iHeight)
			{
				cUnitA* pUnitSecondary = (cUnitA*)pCA->pGameWorld->pMap[Pos_secondary.index].pOccupant_Unit;
				if (pUnitSecondary != NULL)
				if (pUnitSecondary->iControl != iControl)
				{


					RegisterHitOnUnit(pUnitSecondary, double(iDamage) * pInfo->ddDamageRatios[lc7],-1, -1);
				}
			}
		}
	}
}
	else
		retval = 1;//destroy the projectile if it has wandered off the map

	//if it ran out of range, flag to destroy
	if (fRemrange <= 0)
	{

		retval = 0;
	}

	//


	return retval;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProjListA::Create_Projectile(int iType, int iControl, float fXPos_ABS, float fYPos_ABS, float fXPos_Target, float fYPos_Target, int iID_Target)
{

//DEBUG
Write_Log("CREATING PROJECTILE");
WriteLog_Enter();

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	PROJTYPEINFO* pInfo = &pCA->pPTypeTable->ProjTypes[iType];

//DEBUG
Write_Log("Starting Class Access");
WriteLog_Enter();


	
//DEBUG
Write_Log("Allocating Projectile");
WriteLog_Enter();
	cProjectileA*	pProjNew = new cProjectileA;
//DEBUG
Write_Log(" Time: ");
char NText[50];
itoa(pCA->pTimer->iCurrTick, NText, 10);
Write_Log(NText);
WriteLog_Enter();


//DEBUG
Write_Log(" Create: Calculating Distance to Target.");
WriteLog_Enter();
	double targdist = pProjNew->Calc_Distance(fXPos_ABS, fYPos_ABS, fXPos_Target, fYPos_Target); //used here so function isn't run redundnatly twice


//DEBUG
Write_Log(" Create: Initializing Projectile.");
WriteLog_Enter();
	//initialize
	pProjNew->Init(pCAccess);
	//increment ID_Last_Assigned
	iID_Last_Assigned++;
//DEBUG
Write_Log(" Create: Setting Data.");
WriteLog_Enter();
	//put in the settings (06.10.2002) incomplete
	pProjNew->iType = iType;
	pProjNew->iControl = iControl;
	pProjNew->fPos_Z = 0; //TEMP 01.16.2004
	pProjNew->Pos_ABS.x = fXPos_ABS;
	pProjNew->Pos_ABS.y = fYPos_ABS;
	pProjNew->Update_Position();
//	pProjNew->Heading.x = fXHeading;
//	pProjNew->Heading.y = fYHeading;
	//temp - 06.21.2002
	pProjNew->fSpeed = pInfo->fSpeed;
	pProjNew->fAccel = pInfo->fAccel;
	pProjNew->fMaxSpeed = pInfo->fMaxSpeed;
	pProjNew->fRemrange = pInfo->fRemrange;
	pProjNew->iDamage = pInfo->iDamage;
	pProjNew->Pos_target.x = fXPos_Target;
	pProjNew->Pos_target.y = fYPos_Target;
	//fSpeed/targdist could be further optimized, but no compelling reason to(06.21.2002)
	pProjNew->Heading.x = (fXPos_Target - fXPos_ABS) / targdist;
	pProjNew->Heading.y = (fYPos_Target - fYPos_ABS) / targdist;
	pProjNew->fDirection = pProjNew->Calc_Direction(pProjNew->Heading.x, pProjNew->Heading.y);
	pProjNew->iID = iID_Last_Assigned;

//DEBUG
Write_Log(" Create: Inserting into Projectile List.");
WriteLog_Enter();
	//slip it into the projectile list (slip it in the back)
	pPRL_Trav = (cProjectileA*)pPRL_Last->pPrev;

	pProjNew->pNext = (VOID*)pPRL_Last;
	pProjNew->pPrev = (VOID*)pPRL_Trav;

	pPRL_Trav->pNext = (VOID*)pProjNew;
	pPRL_Last->pPrev = (VOID*)pProjNew;

	
//play projectile sound (degrade based on distance from viewer
	if (pInfo->iSoundSeg_Fire >= 0)
	{

		//pCA->pDA_Audio->Play_Primary(pInfo->iSoundSeg_Fire);
		int iVol = pCA->pDA_Audio->Determine_Standard_Volume(pProjNew->Pos_square.x, pProjNew->Pos_square.y);
		pCA->pEventBuffer->Add_Event(-1, pCA->pTimer->iCurrTick + 1, pInfo->iSoundSeg_Fire, iVol,  6, fXPos_ABS + TILE_SIZE /2 , fYPos_ABS + TILE_SIZE/2);
	}


//DEBUG
Write_Log(" Create: Completed.");
WriteLog_Enter();


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProjListA::Destroy_Projectile_byaddr(cProjectileA* pPA)
{
	//get the back and forward links
	pPRL_Trav = (cProjectileA*)pPA->pPrev;
	pPRL_Trav2 = (cProjectileA*)pPA->pNext;

	//unlink
	pPRL_Trav->pNext = (VOID*)pPRL_Trav2;
	pPRL_Trav2->pPrev = (VOID*)pPRL_Trav;

	//destroy the offending projectile(jk)
	delete(pPA);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cProjListA::~cProjListA()
{

//go through and delete up anything still in the list
	pPRL_Trav = pPRL_First;

	while(pPRL_Trav != NULL)
	{
	pPRL_Trav2 = (cProjectileA*)pPRL_Trav->pNext;
	delete(pPRL_Trav);
	pPRL_Trav = pPRL_Trav2;
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProjectileA::Update_Position()
{
//essentially copied from the corresponding function from cUnitA (06.13.2002)

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//calculate the rest of the positions based on the ABS positions
	Pos_square.x = (int)((int)Pos_ABS.x / TILE_SIZE);
	Pos_square.y = (int)((int)Pos_ABS.y / TILE_SIZE);
	Pos_offset.x = (int)((int)Pos_ABS.x % TILE_SIZE);
	Pos_offset.y = (int)((int)Pos_ABS.y % TILE_SIZE);

if (Pos_offset.x > (TILE_SIZE/2))
	{
	Pos_square.x++;
	Pos_offset.x = -(TILE_SIZE - Pos_offset.x);
	}
if (Pos_offset.y > (TILE_SIZE/2))
	{
	Pos_square.y++;
	Pos_offset.y = -(TILE_SIZE - Pos_offset.y);
	}

//calculate the index square position
	Pos_square.Calc_Index(pCA->pGameWorld->iWidth);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cProjectileA::Calc_DrawFrame(double Angle, int iNumDirs)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	char NText[10];
	
	//a copy of it's equivalent in cUnitA

	//modified 01.07.2004

	//figures out what the drawframe is
	//iDrawFrame = iDir_Current;
//DEBUG
/*
pCA->pUnitList->Write_Log("[Calc_DrawFrame] Incoming Angle: ");
gcvt(Angle, 7, NText);
pCA->pUnitList->Write_Log(NText);
pCA->pUnitList->WriteLog_Enter();
pCA->pUnitList->Write_Log("[Calc_DrawFrame] Incoming Directions: ");
itoa(iNumDirs, NText, 10);
pCA->pUnitList->Write_Log(NText);
pCA->pUnitList->WriteLog_Enter();
*/


	//wrap-around a 360 degree angle or higher (or a negative angle) before continuing
	while (Angle >= 360)
	{
		Angle -= 360;
	}
	while (Angle < 0)
	{
		Angle += 360;
	}

	//figure out the number of degrees per distinct direction
	double DirSize = 360 / double(iNumDirs);

	double ddret = Angle / DirSize;

	//determines the "floor"
	int iret = (int)ddret;

	//now perform rounding if necessary
	if ( ddret - iret >= 0.5)
		iret++;

	//wrap-around a direction max angle if necessary
	if ( iret == iNumDirs)
		iret = 0;

	//and now add one to meet convention
//	iret++; //(NO! apparently this is not the convention!) 01.07.2004
//DEBUG
/*
pCA->pUnitList->Write_Log("[Calc_DrawFrame] Output Frame: ");
itoa(iret, NText, 10);
pCA->pUnitList->Write_Log(NText);
pCA->pUnitList->WriteLog_Enter();
*/


	return iret;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
float cProjectileA::Calc_Direction(float fXHeading, float fYHeading)  //06.16.2002
{
double	Dir_Angle; //angle measure	
int	Ret_Dir;
	
	//use atan2 to figure out the angle of what this is
	if (fXHeading != 0)
	{
		Dir_Angle = atan2(-fYHeading, fXHeading);
		//convert to degrees
		Dir_Angle = Dir_Angle * 57.296;

		//subtract 90, conjugate negative angles
		Dir_Angle = Dir_Angle - 90;
		if (Dir_Angle < 0)
			Dir_Angle = Dir_Angle + 360;
		//reverse angle direction
		if (Dir_Angle != 0)  //actually this IF statement isn't necessary because the angle can never be 0(fXHeading would have to be 0)
			Dir_Angle = 360 - Dir_Angle;

	}
		else //either 0 or 180 degrees (comparisons are inverted from expected because on the map, larger numbers are DOWN...)
		{
			if (fYHeading <= 0)
				Dir_Angle = 0;					//0 degrees
			if (fYHeading > 0)	
				Dir_Angle = 180;		//180 degrees
		}


	return Dir_Angle;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProjListA::Process_Projectiles() //(06.13.2002)
{
	
	cProjectileA* pPRL_Trav = (cProjectileA*)pPRL_First->pNext;
	cProjectileA* pPRL_Next;

	//run through the list and do run_cycle()
	while (pPRL_Trav != pPRL_Last)
	{
		pPRL_Next = (cProjectileA*)pPRL_Trav->pNext;
		
		if (pPRL_Trav->Run_Cycle() == 0)
			Destroy_Projectile_byaddr(pPRL_Trav);

		pPRL_Trav = pPRL_Next;
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cProjectileA::Calc_Distance(float xstart, float ystart, float xend, float yend)
{
	float xdiff = abs(xend - xstart);
	float ydiff = abs(yend - ystart);
	double dist = sqrt( (xdiff * xdiff) + (ydiff * ydiff));

	return dist;
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void PROJTYPEINFO::Set_1TileSplash(double ddTier1, double ddTier2)
{
	Clear_Secondaries();

	Pos_secondarydamage[0].x = 1;
	Pos_secondarydamage[0].y = 0;
	ddDamageRatios[0] = ddTier1;

	Pos_secondarydamage[1].x = 0;
	Pos_secondarydamage[1].y = 1;
	ddDamageRatios[1] = ddTier1;

	Pos_secondarydamage[2].x = -1;
	Pos_secondarydamage[2].y = 0;
	ddDamageRatios[2] = ddTier1;

	Pos_secondarydamage[3].x = 0;
	Pos_secondarydamage[3].y = -1;
	ddDamageRatios[3] = ddTier1;

	Pos_secondarydamage[4].x = 1;
	Pos_secondarydamage[4].y = 1;
	ddDamageRatios[4] = ddTier2;

	Pos_secondarydamage[5].x = 1;
	Pos_secondarydamage[5].y = -1;
	ddDamageRatios[5] = ddTier2;

	Pos_secondarydamage[6].x = -1;
	Pos_secondarydamage[6].y = 1;
	ddDamageRatios[6] = ddTier2;

	Pos_secondarydamage[7].x = -1;
	Pos_secondarydamage[7].y = -1;
	ddDamageRatios[7] = ddTier2;

	iNumSecondarySquares = 8;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProjTT::Init(VOID* pCA)
{

	//simply add in values
	ProjTypes[PTYPE_TEST00].fRemrange = 120;
	ProjTypes[PTYPE_TEST00].fSpeed = 5;
	ProjTypes[PTYPE_TEST00].fMaxSpeed = -1;
	ProjTypes[PTYPE_TEST00].fAccel = 0;
	ProjTypes[PTYPE_TEST00].iDamage = 12;
	//ProjTypes[PTYPE_TEST00].iImageSurfaceIndex = ISURF_PROJ_TEST01;
	ProjTypes[PTYPE_TEST00].iAnimType_Trail = VAL_INVALID;
	ProjTypes[PTYPE_TEST00].iAnimType_Detonate = ANIMTYPE_EXPL01;
	ProjTypes[PTYPE_TEST00].iSoundSeg_Detonate = SOUNDSEG_IMPACT2;
	ProjTypes[PTYPE_TEST00].iSoundSeg_Fire = SOUNDSEG_FIRE_120MM;
	ProjTypes[PTYPE_TEST00].iRotateFrames = 1;
	ProjTypes[PTYPE_TEST00].SourceGraphics.Init(pCA, NULL);
	ProjTypes[PTYPE_TEST00].SourceGraphics.Add_Entry(0, 0, 0, ISURF_PROJ_TEST01, 0, TILE_SIZE, 0, TILE_SIZE, 10);
	ProjTypes[PTYPE_TEST00].SourceGraphics.Update();
	//test
	ProjTypes[PTYPE_TEST00].Set_1TileSplash(0.7, 0.3);


	ProjTypes[PTYPE_TEST01].fRemrange = 180;
	ProjTypes[PTYPE_TEST01].fSpeed = 9;
	ProjTypes[PTYPE_TEST01].fMaxSpeed = -1;
	ProjTypes[PTYPE_TEST01].fAccel = 0;
	ProjTypes[PTYPE_TEST01].iDamage = 5;
	//ProjTypes[PTYPE_TEST01].iImageSurfaceIndex = ISURF_PROJ_TEST02;
	ProjTypes[PTYPE_TEST01].iAnimType_Trail = VAL_INVALID;
	ProjTypes[PTYPE_TEST01].iAnimType_Detonate = ANIMTYPE_EXPL02;
	ProjTypes[PTYPE_TEST01].iSoundSeg_Detonate = SOUNDSEG_IMPACT2;
	ProjTypes[PTYPE_TEST01].iSoundSeg_Fire = SOUNDSEG_FIRE_80MM;
	ProjTypes[PTYPE_TEST01].iRotateFrames = 1;
	ProjTypes[PTYPE_TEST01].SourceGraphics.Init(pCA, NULL);
	ProjTypes[PTYPE_TEST01].SourceGraphics.Add_Entry(0, 0, 0, ISURF_PROJ_TEST02, 0, TILE_SIZE, 0, TILE_SIZE, 10);
	ProjTypes[PTYPE_TEST01].SourceGraphics.Update();

	ProjTypes[PTYPE_TEST02].fRemrange = 400;
	ProjTypes[PTYPE_TEST02].fSpeed = 3;
	ProjTypes[PTYPE_TEST02].fMaxSpeed = -1;
	ProjTypes[PTYPE_TEST02].fAccel = 0;
	ProjTypes[PTYPE_TEST02].iDamage = 20;
	//ProjTypes[PTYPE_TEST02].iImageSurfaceIndex = ISURF_PROJ_TEST01;
	ProjTypes[PTYPE_TEST02].iAnimType_Trail = VAL_INVALID;
	ProjTypes[PTYPE_TEST02].iAnimType_Detonate = ANIMTYPE_EXPL01;
	ProjTypes[PTYPE_TEST02].iSoundSeg_Detonate = VAL_INVALID;
	ProjTypes[PTYPE_TEST02].iSoundSeg_Fire = VAL_INVALID;
	ProjTypes[PTYPE_TEST02].iRotateFrames = 1;
	ProjTypes[PTYPE_TEST02].SourceGraphics.Init(pCA, NULL);
	ProjTypes[PTYPE_TEST02].SourceGraphics.Add_Entry(0, 0, 0, ISURF_PROJ_TEST01, 0, TILE_SIZE, 0, TILE_SIZE, 10);
	ProjTypes[PTYPE_TEST02].SourceGraphics.Update();

	ProjTypes[PTYPE_NAVAL120].fRemrange = 240;
	ProjTypes[PTYPE_NAVAL120].fSpeed = 8;
	ProjTypes[PTYPE_NAVAL120].fMaxSpeed = -1;
	ProjTypes[PTYPE_NAVAL120].fAccel = 0;
	ProjTypes[PTYPE_NAVAL120].iDamage = 20;
	//ProjTypes[PTYPE_NAVAL120].iImageSurfaceIndex = ISURF_PROJ_NAVAL120;
	ProjTypes[PTYPE_NAVAL120].iAnimType_Trail = VAL_INVALID;
	ProjTypes[PTYPE_NAVAL120].iAnimType_Detonate = ANIMTYPE_EXPL01;
	ProjTypes[PTYPE_NAVAL120].iSoundSeg_Detonate = SOUNDSEG_IMPACT2;
	ProjTypes[PTYPE_NAVAL120].iSoundSeg_Fire = SOUNDSEG_FIRE_NAVAL120;
	ProjTypes[PTYPE_NAVAL120].iRotateFrames = 1;
	ProjTypes[PTYPE_NAVAL120].SourceGraphics.Init(pCA, NULL);
	ProjTypes[PTYPE_NAVAL120].SourceGraphics.Add_Entry(0, 0, 0, ISURF_PROJ_NAVAL120, 0, TILE_SIZE, 0, TILE_SIZE, 10);
	ProjTypes[PTYPE_NAVAL120].SourceGraphics.Update();

	ProjTypes[PTYPE_AIRGUN].fRemrange = 240;
	ProjTypes[PTYPE_AIRGUN].fSpeed = 8;
	ProjTypes[PTYPE_AIRGUN].fMaxSpeed = -1;
	ProjTypes[PTYPE_AIRGUN].fAccel = 0;
	ProjTypes[PTYPE_AIRGUN].iDamage = 10;
	//ProjTypes[PTYPE_AIRGUN].iImageSurfaceIndex = ISURF_PROJ_NAVAL120;
	ProjTypes[PTYPE_AIRGUN].iAnimType_Trail = ANIMTYPE_TRAIL01;
	ProjTypes[PTYPE_AIRGUN].iAnimType_Detonate = ANIMTYPE_EXPL01;
	ProjTypes[PTYPE_AIRGUN].iSoundSeg_Detonate = SOUNDSEG_IMPACT2;
	ProjTypes[PTYPE_AIRGUN].iSoundSeg_Fire = SOUNDSEG_FIRE_MSL;
	ProjTypes[PTYPE_AIRGUN].iRotateFrames = 1;
	ProjTypes[PTYPE_AIRGUN].SourceGraphics.Init(pCA, NULL);
	ProjTypes[PTYPE_AIRGUN].SourceGraphics.Add_Entry(0, 0, 0, ISURF_PROJ_NAVAL120, 0, TILE_SIZE, 0, TILE_SIZE, 10);
	ProjTypes[PTYPE_AIRGUN].SourceGraphics.Update();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProjListA::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cProjListA.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProjListA::Write_Log(LPSTR Text)
{
	if (DEBUG + DEBUG_PROJECTILE_A >= 1)
{

DWORD numbyteswritten = 0;

WriteFile( hLogFile,
		   Text,
		   strlen(Text),
		   &numbyteswritten,
		   NULL);
}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProjListA::WriteLog_Enter()
{
if (DEBUG + DEBUG_PROJECTILE_A >= 1)
{
DWORD numbyteswritten = 0;

//write in an automatic ENTER line in the file
char etr[3];

etr[0] = 13;
etr[1] = 10;

WriteFile( hLogFile,
		   etr,
		   2,
		   &numbyteswritten,
		   NULL);
}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProjListA::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
