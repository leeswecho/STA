#include "classes.h"

//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitListA::Init(VOID* pCA)
{

	pCAccess = pCA;

if (DEBUG + DEBUG_UNITLIST >= 1)
	Init_LogFile();

	//allocate memory for two list anchors
	pUL_First = new cUnitA;
	pUL_Last = new cUnitA;

	ZeroMemory(pUL_First, sizeof(cUnitA));
	ZeroMemory(pUL_Last, sizeof(cUnitA));

	//tie them together
	pUL_First->pNext_UL = (VOID*)pUL_Last;
	pUL_First->pPrev_UL = NULL;
	pUL_First->CmpAttached.Init(pCA, "Unit List Anchor");
	pUL_Last->pPrev_UL = (VOID*)pUL_First;
	pUL_Last->pNext_UL = NULL;
	pUL_Last->CmpAttached.Init(pCA, "Unit List Anchor");

	//set ID_Last_Assigned
	ID_Last_Assigned = -1;  //no IDs have yet been assigned yet



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cUnitListA::~cUnitListA()
{

//moved to Exit() 03.28.2003
/*
	//go through the list and destroy everything in the list
	pUL_Trav = pUL_First;

	while (pUL_Trav!= NULL)
	{
		pUL_Trav2 = (cUnitA*)pUL_Trav->pNext_UL;
		delete(pUL_Trav);
		pUL_Trav = pUL_Trav2;

	}
*/

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitListA::Exit()
{

	//go through the list and destroy everything in the list
	pUL_Trav = pUL_First;

	while (pUL_Trav!= NULL)
	{
		pUL_Trav2 = (cUnitA*)pUL_Trav->pNext_UL;
		pUL_Trav->CmpAttached.Exit();
		delete(pUL_Trav);
		pUL_Trav = pUL_Trav2;

	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitListA::Destroy_AllUnits()
{

	cClassAccess* pCA = (cClassAccess*) pCAccess;
	
	//go through the list and destroy everything in the list
	pUL_Trav = (cUnitA*)pUL_First->pNext_UL;

	while (pUL_Trav!= pUL_Last)
	{
		pUL_Trav2 = (cUnitA*)pUL_Trav->pNext_UL;

//DEBUG
Write_Log("Freeing... ");
Write_Log(pUL_Trav->Name);
WriteLog_Enter();
//DEBUG
Write_Log("Deleting Unit...ID: ");
char NText[50];
itoa(pUL_Trav->iID, NText, 10);
Write_Log(NText);
Write_Log("...");

//clueless as to why Destroy_unit_byaddr works and the routine commented out here below does not (05.21.2004)
/*		
		pUL_Trav->CmpAttached.Exit();



		//free name
		delete(pUL_Trav->Name);
		pCA->pGameWorld->Unregister_Unit(pUL_Trav->Pos_sqcurr, pUL_Trav);



		delete(pUL_Trav);
*/

		Destroy_Unit_byaddr(pUL_Trav);

		pUL_Trav = pUL_Trav2;
//DEBUG
Write_Log("Unit Deleted.");
WriteLog_Enter();

	}

	//relink anchors
	pUL_First->pNext_UL = (void*)pUL_Last;
	pUL_Last->pPrev_UL = (void*)pUL_First;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitListA::Destroy_Unit_byaddr(cUnitA* pUA)
{
	cClassAccess* pCA = (cClassAccess*) pCAccess;

		for (int lc94 = 0; lc94 < TEMP_DP_MAXPLAYERS; lc94++)
		{
			if (pCA->pNationManager->ppNations[lc94] != NULL)
			{
				pCA->pNationManager->ppNations[lc94]->AINew.Event_Unit_Destroyed(pUA);
			}

		}


//consistently, it is attempting to unregister unit 8 and failing. consequently, i am going to check 
//the unittable for parity check with the unit list at this point(03.09.2004)

	//get the back and forward links
	pUL_Trav = (cUnitA*)pUA->pPrev_UL;
	pUL_Trav2 = (cUnitA*)pUA->pNext_UL;

	//unlink
	pUL_Trav->pNext_UL = (VOID*)pUL_Trav2;
	pUL_Trav2->pPrev_UL = (VOID*)pUL_Trav;


	//unlink from the unitIDtable
	pCA->pUnitTable->pUnitTable[pUA->iID] = NULL;

	//decrement unit counter
	pCA->pNationManager->ppNations[pUA->iControl]->iNumUnits--;

//DEBUG
char NText[10];
Write_Log("[Destroy] Unit ");
itoa(pUA->iID, NText, 10);
Write_Log(NText);
Write_Log(" has been destroyed.");
WriteLog_Enter();
//END DB

	//destroy the offending Unit
//DEBUG
Write_Log(" Exiting Component List.");
WriteLog_Enter();
//END DB
	pUA->CmpAttached.Exit();


//free names
	//delete [] pUA->Name;  don't delete the full name because the full name is typically shared
	delete [] pUA->ShortName;

	//free works(when it shouldn't), delete does weird freaky things to the rest of my
	//buffer(when it should work...) not sure what to do :-/ (03.10.2004)
	free(pUA);

//DEBUG
Write_Log(" Function Complete.");
WriteLog_Enter();
//END DB
	pUA = NULL;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cUnitA* cUnitListA::Create_Unit(LPSTR Name, int iType, int iControl, int iXPos_square, int iYPos_square)
{
	cClassAccess* pCA = (cClassAccess*) pCAccess;

	int lc1; //local counter
//DEBUG
	char NText[50];
Write_Log(" CREATE UNIT ID: ");
itoa(ID_Last_Assigned + 1, NText, 10);
Write_Log(NText);
WriteLog_Enter();
//END DB

//DEBUG
Write_Log(" Create Unit: Allocating.");
WriteLog_Enter();
//END DB
	//allocate memory for a new unit
	cUnitA* pUnitNew = new cUnitA;


//DEBUG
Write_Log(" Create Unit: Initialize.");
WriteLog_Enter();
//END DB

	//initialize the unit
	pUnitNew->Init(pCAccess);

//DEBUG (removed if not _DEBUG)
	//_ASSERT( _CrtCheckMemory() );


//DEBUG
Write_Log(" Create Unit: Setting information.");
WriteLog_Enter();
//END DB
	//fill the unit's attributes with the correct(or at least appropriate :-) information
	pUnitNew->iDrawFrame = 0;
	pUnitNew->Pos_ABS.x = iXPos_square * TILE_SIZE;
	pUnitNew->Pos_ABS.y = iYPos_square * TILE_SIZE;
	pUnitNew->Pos_ABS.z = 0;
	pUnitNew->fDir_Current = 0;
	pUnitNew->fDir_Target = 0;

	//initialize
	pUnitNew->iTick_LastRepath = 0;
//DEBUG
Write_Log(" Create Unit: Initializing Positions.");
WriteLog_Enter();
//END DB

	pUnitNew->Update_Position();
	//(06.04.2002) Initialize target and sqimtarget positions
	pUnitNew->Pos_sqimtarget.index = pUnitNew->Pos_sqcurr.index;
	pUnitNew->Pos_sqimtarget.Calc_xy(pCA->pGameWorld->iWidth);

//DEBUG
Write_Log(" Create Unit: Set Name. Name Length: ");
itoa(strlen(Name), NText, 10);
Write_Log(NText);
WriteLog_Enter();
//END DB

	pUnitNew->iType = iType;   //obsolete 03.04.2003   //re-enabled 06.12.2003

	pUnitNew->iPersonality = rand() % 3;   //random personality

	//pCA->pChatBuffer->Add_CBNode("Unit Created", 600);

	pUnitNew->Name = new char[strlen(Name) + 1];
	strcpy(pUnitNew->Name, Name);
//		//NULL terminate
		pUnitNew->Name[strlen(Name)] = 0;

	UNITTYPEINFO* pType = &pCA->pUTypeTable->UnitTypes[iType];
	char* ShortName = pCA->pUTypeTable->UnitTypes[iType].ShortName;

	pUnitNew->ShortName = new char[strlen(ShortName) + 7];
	ZeroMemory( pUnitNew->ShortName, sizeof(char)*(strlen(ShortName) + 7));
	strcpy(pUnitNew->ShortName, ShortName);
	itoa(pType->iInstances[iControl], NText, 10);
	//concatenate
	strcat(pUnitNew->ShortName, NText);
	pCA->pUTypeTable->UnitTypes[iType].iInstances[iControl] += 1;




	pUnitNew->iControl = iControl;

//increment unit counter
	pCA->pNationManager->ppNations[iControl]->iNumUnits++;

//DEBUG (removed if not _DEBUG)
	//_ASSERT( _CrtCheckMemory() );


//DEBUG
Write_Log(" Create Unit: Set More Information.");
WriteLog_Enter();	
//END DB

	//TEMP until unit info table goes up(unit table replaced by component info table) 03.21.2003
//	pUnitNew->iHPMax = TEMP_UHP;
//	pUnitNew->iHP = pUnitNew->iHPMax;
	//new 03.21.2003
	pUnitNew->fHPMax = 1;  //a "unit" without any components, has only 1HP
	pUnitNew->fHP = pUnitNew->fHPMax;
	pUnitNew->iSize = 0;
	pUnitNew->iMass = 0;
	pUnitNew->fCurrSpeed = 0;
	pUnitNew->fMaxSpeed = 0;
	pUnitNew->VisibleComponents = NULL;
	pUnitNew->bLoaded = FALSE;
	pUnitNew->fArmor = 0;

	pUnitNew->pCurrentWeapon = NULL;
	pUnitNew->pPrimaryWeapon = NULL;
	pUnitNew->pBuilder = NULL;

	pUnitNew->L6_LastFlagged = 0;

	pUnitNew->OverloadInfo.bActive = FALSE;
	pUnitNew->OverloadInfo.iOverLoadType = -1;


	//TEMP(?) set all of the initial modes of the unit to IDLE (01.19.2002)
	for (lc1 = 0; lc1 < MODES_SIMUL_L1; lc1++)
		pUnitNew->bModes_L1[lc1] = UMODE_L1_IDLE;

	pUnitNew->bMode_L2 = UMODE_L2_NOTHING;
	pUnitNew->bMode_L3 = UMODE_L3_NOTHING;
	pUnitNew->bMode_L4 = UMODE_L4_NOTHING;
	pUnitNew->bMode_L5 = UMODE_L5_NOTHING;
	pUnitNew->bMode_L6 = UMODE_L6_NOTHING;
	pUnitNew->bExternalOrderCompleted = TRUE;

	//set attack and guard info structures to null(06.22.2002)
	pUnitNew->pAttackInfo = NULL;
	pUnitNew->pGuardInfo = NULL;
	pUnitNew->pBuildInfo = NULL; //10.11.2003

	//set strutures inside MultiFireInfo to NULL to init
	pUnitNew->MultiFireInfo.ppSelectedComponents = NULL;
	pUnitNew->MultiFireInfo.arrAttackDirections = NULL;
	pUnitNew->MultiFireInfo.iNumSelectedComponents = 0;
	//set timer vals
//	pUnitNew->Timer_NextFire = pCA->pTimer->iCurrTick + TEMP_FIRETIME;
	pUnitNew->Timer_NextScan = pCA->pTimer->iCurrTick + TEMP_SCANTIME;

	pUnitNew->iTimeLastL1Change = pCA->pTimer->iCurrTick;

//DEBUG
Write_Log(" Create Unit: Linking into unit list.");
WriteLog_Enter();
//END DB
	//stick this unit at the end of the list
	cUnitA* pNextToLast = (cUnitA*)pUL_Last->pPrev_UL;
//DEBUG
Write_Log("  Line 2");
WriteLog_Enter();
//END DB
	pNextToLast->pNext_UL = (VOID*)pUnitNew;
//DEBUG
Write_Log("  Line 3");
WriteLog_Enter();
//END DB
	pUL_Last->pPrev_UL = (VOID*)pUnitNew;
//DEBUG
Write_Log("  Line 4");
WriteLog_Enter();
//END DB
	pUnitNew->pNext_UL = (VOID*)pUL_Last;
//DEBUG
Write_Log("  Line 5");
WriteLog_Enter();
//END DB
	pUnitNew->pPrev_UL = (VOID*)pNextToLast;

//DEBUG
Write_Log(" Create Unit: Initializing Component List.");
WriteLog_Enter();
//END DB

	char LFileName[TEMP_STD_STRLEN];
	ZeroMemory(LFileName, sizeof(char) * TEMP_STD_STRLEN);
	strcat(LFileName, Name);

//DEBUG (removed if not _DEBUG)
	//_ASSERT( _CrtCheckMemory() );


	//initialize the unit's component list
	pUnitNew->CmpAttached.Init(pCAccess, LFileName);
	
//DEBUG (removed if not _DEBUG)
	//_ASSERT( _CrtCheckMemory() );
	
	//initialize this unit's list of supported commands(as of now, none)
	pUnitNew->piSupportedCommands = NULL;

//DEBUG
Write_Log(" Create Unit: Adding to Unit Table.");
WriteLog_Enter();
//END DB
	//add this unit to the ID table
	//increase ID_Last_Assigned
	ID_Last_Assigned++;
	pCA->pUnitTable->Add_Unit(pUnitNew, ID_Last_Assigned);
	//set the iID on the unit too! (05.14.2002)
	pUnitNew->iID = ID_Last_Assigned;

//DEBUG (removed if not _DEBUG)
	//_ASSERT( _CrtCheckMemory() );

//DEBUG
Write_Log(" Create Unit: Registering onto GameWorld.");
WriteLog_Enter();
//END DB
	//register this unit on the gameworld
		//not doing this yet because I am tired and I need to figure out how the maptile will sort out all of the stacked units(01.17.2002)
		//figured it out (01.18.2002) have written a register command to do this
	//I am going to continue requiring the coords to be sent to the register unit function so that
	//I won't have to worry about debugging problems with this later
	pCA->pGameWorld->Register_Unit(pUnitNew->Pos_sqcurr, (VOID*)pUnitNew);


//DEBUG (removed if not _DEBUG)
	//_ASSERT( _CrtCheckMemory() );

//DEBUG
Write_Log(" Create Unit: HACK forcing unit into guard mode.");
WriteLog_Enter();
//END DB
	//HACK(01.02.2003) Force new units to start in guard mode
	pUnitNew->bMode_L5 = UMODE_L5_GUARD;
	pUnitNew->pGuardInfo = new UNITGUARDINFO;
	pUnitNew->pGuardInfo->Pos_guard.index = pUnitNew->Pos_sqcurr.index;
	pUnitNew->pGuardInfo->Pos_guard.Calc_xy(pCA->pGameWorld->iWidth);

//DEBUG
Write_Log(" Create Unit: Attaching components. Number of components to attach: ");
itoa(pCA->pUTypeTable->UnitTypes[iType].CmpAttached.iNumComponents, NText, 10);
Write_Log(NText);
WriteLog_Enter();
//END DB
	

	//attach components if necessary
	cComponentA* pCmpTrav = (cComponentA*)pCA->pUTypeTable->UnitTypes[iType].CmpAttached.pCmpList_First->pNext_list;
	while (pCmpTrav !=  pCA->pUTypeTable->UnitTypes[iType].CmpAttached.pCmpList_Last)
	{
//DEBUG
Write_Log("  Adding Component...");
WriteLog_Enter();
//END DB
		//add the component type listed here
		cComponentA* pComponent = pUnitNew->CmpAttached.Add_Component(pCmpTrav->iType);
		pComponent->Set_OwnerType(OBJTYPE_UNIT); 
		pComponent->Set_OwnerID(pUnitNew->iID);

		pCmpTrav = (cComponentA*)pCmpTrav->pNext_list;
	}

//DEBUG (removed if not _DEBUG)
	//_ASSERT( _CrtCheckMemory() );

//DEBUG
Write_Log(" Create Unit: Recalculating Unit Attributes.");
WriteLog_Enter();
//END DB
	pUnitNew->Recalc_Attributes();

//DEBUG (removed if not _DEBUG)
	//_ASSERT( _CrtCheckMemory() );

//DEBUG
Write_Log(" Create Unit: Creation done.");
WriteLog_Enter();
WriteLog_Enter();
//END DB

//AI stuff
/////////////////////////
	for (int lc8 = 0; lc8 < TEMP_DP_MAXPLAYERS; lc8++)
	{
		pUnitNew->arrDestroyEnemyMarked[lc8] = 0;
	}
//////////////////////////////////////////
		for (int lc94 = 0; lc94 < TEMP_DP_MAXPLAYERS; lc94++)
		{
			if (pCA->pNationManager->ppNations[lc94] != NULL)
			{
				pCA->pNationManager->ppNations[lc94]->AINew.Event_Unit_Created(pUnitNew);
			}
		}

    	

	return pUnitNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitListA::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cUnitListA.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitListA::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_UNITLIST >= 1)
{
WriteFile( hLogFile,
		   Text,
		   strlen(Text),
		   &numbyteswritten,
		   NULL);
}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////


//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitListA::Process_Units()
{
	int iRetVal = 1;

	cClassAccess* pCA = (cClassAccess*)pCAccess;

//DEBUG
char NText[20];


	//start at the beginning of the list and go through each of the units in the list
	pUL_Trav = (cUnitA*)pUL_First->pNext_UL;

	while (pUL_Trav != NULL && pUL_Trav != pUL_Last)
		{

			pUL_Trav2 = (cUnitA*)pUL_Trav->pNext_UL;
			if (pUL_Trav->bLoaded == FALSE)
			{
//DEBUG
Write_Log("Reveal.");
WriteLog_Enter();
			//for now, simply reveal

				pCA->pGameWorld->Reveal(pUL_Trav->Pos_sqcurr.x, pUL_Trav->Pos_sqcurr.y, 2, pUL_Trav->iControl);

			//unregister the unit from the Gameworld array(the unit is "lifted" from the game map while it decides what its going to do)
			pCA->pGameWorld->Unregister_Unit(pUL_Trav->Pos_sqcurr, pUL_Trav);

//DEBUG
/*
if (_CrtIsValidHeapPointer(pUL_Trav) == FALSE)
{
	Write_Log("ERROR 104: Invalid Heap Pointer!");
	WriteLog_Enter();

}
*/

//DEBUG 
/*Write_Log("[Process Units] Unregistering Unit. Pointer: ");
itoa((int)pUL_Trav, NText, 10);
Write_Log(NText);
Write_Log(" Pos: ");
itoa(pUL_Trav->Pos_sqcurr.index, NText, 10);
Write_Log(NText);
Write_Log(" ID: ");
itoa(pUL_Trav->iID, NText, 10);
Write_Log(NText);
WriteLog_Enter();
*///END DB

//DEBUG
Write_Log("Unregister.");
WriteLog_Enter();




//DEBUG
/*Write_Log("[Process Units] Running Cycle for Unit: ");
itoa(pUL_Trav->iID, NText, 10);
Write_Log(NText);
WriteLog_Enter();
*///END DB

//DEBUG
Write_Log("Run Cycle.");
WriteLog_Enter();


//pCA->pTimer->StartCount(TIMER_COUNTER2);			
			//allow the unit to perform its cycle (NOTE: here's an idea for dealing with units having to kill themselves(they post an event) 01.19.2002
			iRetVal = pUL_Trav->Run_Cycle();

//DEBUG
Write_Log("Done Run Cycle.");
WriteLog_Enter();




//pCA->pTimer->Write_Log(NText);
//pCA->pTimer->Write_Log(": RUN CYCLE: ");
//itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER2], NText, 10);
//pCA->pTimer->Write_Log(NText);
//pCA->pTimer->WriteLog_Enter();
//*/
			//destroy the unit if it returned 0("I need to be destroyed"), and don't register it
			if (iRetVal == 0)
			{
				
				//post destruction event
				int iVol = pCA->pDA_Audio->Determine_Standard_Volume(pUL_Trav->Pos_sqcurr.x, pUL_Trav->Pos_sqcurr.y);
				pCA->pEventBuffer->Add_Event(ANIMTYPE_UEXPL01, pCA->pTimer->iCurrTick + 1, SOUNDSEG_UEXPL01, iVol, 6, pUL_Trav->Pos_ABS.x, pUL_Trav->Pos_ABS.y);

				Destroy_Unit_byaddr(pUL_Trav);

			}
					else
					{
//DEBUG
/*Write_Log("[Process Units] Reregistering Unit: ");
itoa(pUL_Trav->iID, NText, 10);
Write_Log(NText);
WriteLog_Enter();
*///END DB
//DEBUG
Write_Log("Reregister.");
WriteLog_Enter();


				//otherwise, re-register the unit  (don't if the unit has been loaded unto a transport)
				if (pUL_Trav->bLoaded == FALSE)
					pCA->pGameWorld->Register_Unit(pUL_Trav->Pos_sqcurr, pUL_Trav);

					}
			}
			//advance the pointer
			pUL_Trav = pUL_Trav2;
		}
//DEBUG
/*Write_Log("[Process Units] Function Complete ");
WriteLog_Enter();
*///END DB


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cUnitTableA::~cUnitTableA()
{

		

	delete(pUnitTable);







}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitTableA::Init()
{

	pUnitTable = new cUnitA*[UNIT_NUMIDS];


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitTableA::Remove_All()
{

	for (int c22 = 0; c22 < UNIT_NUMIDS; c22++)
	{
		Remove_Unit(c22);
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitTableA::Remove_Unit(int iID)
{

	pUnitTable[iID] = NULL;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitTableA::Add_Unit(cUnitA* pUA, int iID)
{

	pUnitTable[iID] = pUA;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitA::Update_Position()
{

	//DEBUG
	char NText[50];
	//END DB

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//calculate the rest of the positions based on the ABS positions
	Pos_sqcurr.x = (int)(Pos_ABS.x / TILE_SIZE);
	Pos_sqcurr.y = (int)(Pos_ABS.y / TILE_SIZE);

	Pos_offset.x = (Pos_ABS.x - (Pos_sqcurr.x * TILE_SIZE));
	Pos_offset.y = (Pos_ABS.y - (Pos_sqcurr.y * TILE_SIZE));

if (Pos_offset.x > (TILE_SIZE/2))
	{
	Pos_sqcurr.x++;
	Pos_offset.x = -(TILE_SIZE - Pos_offset.x);
	}
if (Pos_offset.y > (TILE_SIZE/2))
	{
	Pos_sqcurr.y++;
	Pos_offset.y = -(TILE_SIZE - Pos_offset.y);
	}

//now account for precision errors!
if (fabs(Pos_offset.x) < FP_TOLERANCE)
	Pos_offset.x = 0;
if (fabs(Pos_offset.y) < FP_TOLERANCE)
	Pos_offset.y = 0;

//DEBUG
/*
//pCA->pUnitList->Write_Log("Current Position: ABSx: ");
gcvt(Pos_ABS.x, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->Write_Log(" ABSy: ");
gcvt(Pos_ABS.y, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->Write_Log(" SqX: ");
itoa(Pos_sqcurr.x, NText, 10);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->Write_Log(" SqY: ");
itoa(Pos_sqcurr.y, NText, 10);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->Write_Log(" OffsetX: ");
gcvt(Pos_offset.x, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->Write_Log(" OffsetY: ");
gcvt(Pos_offset.y, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/

//calculate the index square position
	Pos_sqcurr.Calc_Index(pCA->pGameWorld->iWidth);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************//////////////////
void cUnitA::Init(VOID* pCA)
{

	pCAccess = pCA;

	pUnitPath = NULL; //initialize path pointer

	iLastUpdateWeaponTargets = 0;
	Timer_TicksJammed = 0;

//DEBUG
////_ASSERTE( _CrtCheckMemory() );

	//
if (DEBUG + DEBUG_UNIT_A >= 1)
	Init_LogFile();


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************//////////////////
int cUnitA::Run_Cycle()
{

	//debug
	char NText[50];
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cPATHENTRY* pUP = (cPATHENTRY*)pUnitPath;    //casting the unit path pointer...
	int	sdepth;
	int iRetVal = 1;  //nominally flag that the unit is not dead
	int iScanResult = 0;
	double fDir_Absolute = 0; //used later; cannot initialize inside CASE statement
	int lc1; //local counter
	int lc2; //local counter

	BOOL bReadyToFire; //used later down

	//get and store the pointer to the component information table for easier access
	cCmpInfo** pCmpTable = pCA->pCmpInfoTable->pCmpTable;
	cComponentA* pComponent;
	cCmpInfo* pCmpInfo;

	double ddx; //used later down
	double ddx1, ddx2, ddx3, ddx4;
	

	//this is a HACK for enabling aircraft to fly (02.14.2004)
	if (pFrameType->pFrameInfo->MoveCharacteristics[MAPTILE_AIR] > 0)
		Pos_ABS.z = 24;

	//if the unit HP dropped below 0, flag to destroy
	if (fHP <= 0)
	{
		if (iControl == pCA->pGameOptions->iControl)
		{
				//set acknowledgement
				int iRet = pCA->pAckTable->Get_Ack(FALSE, iType, ACK_TYPE_STATUS_UNIT_DIE, iPersonality, rand() % 3);
				pCA->pCommQueue->Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, iID, iControl + 100, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
		}

		return 0;
	}


	//store current(old L1 mode)
	int ibModeL1Old = bModes_L1[0];


//DEBUG
//pCA->pTimer->StartCount(TIMER_COUNTER3);

	//don't know if this is going to be permanent or not....this is a fairly satisfactory fix of the "unpathable path"
	//problem, by having the unit store the location of the last path it failed to calculate, so that it doesn't try
	//a full search(wasting processing time) again and again and again on an unviable path(05.15.2002)
	if (Pos_sqcurr.index == Pos_sqfail_start.index && 
		Pos_sqtarget.index == Pos_sqfail_target.index)
			sdepth = PF_SEARCHDEPTH_SHORT;
				else
			sdepth = PF_SEARCHDEPTH;

//HACK: if all unit modes are idle(unit isn't doing anything, set to GUARD 02.01.2003)

	if  (bExternalOrderCompleted == TRUE && bMode_L5 != UMODE_L5_GUARD)
	{
		if (pGuardInfo != NULL)
		{
			delete pGuardInfo;
			pGuardInfo = NULL;
		}
		pGuardInfo = new UNITGUARDINFO;
		pGuardInfo->Pos_guard.index = Pos_sqcurr.index;
		pGuardInfo->Pos_guard.Calc_xy(pCA->pGameWorld->iWidth);
		bMode_L5 = UMODE_L5_GUARD;
	}


//sort of a HACK....this is meant to resolve FP precision(?) errors (ex. 90.0005 is not quite 90 and thus units don't know what to do)
	//usage of floating-point requires abs statements requires this :-(
	if (fabs(fDir_Current - fDir_Target) < FP_TOLERANCE)
			fDir_Current = fDir_Target;


//DEBUG

char QText[50];
itoa(iID, QText, 10);
pCA->pUnitList->Write_Log("****PROCESSING UNIT ");
pCA->pUnitList->Write_Log(QText);
pCA->pUnitList->Write_Log(" ********");
pCA->pUnitList->WriteLog_Enter(); 


//DEBUG
pCA->pUnitList->Write_Log("Begin L5");
pCA->pUnitList->WriteLog_Enter(); 



//DEBUG

pCA->pTimer->EndCount(TIMER_COUNTER3);
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER3], NText, 10);
pCA->pTimer->Write_Log("   [Run cycle]Pre-L5: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();

//DEBUG
pCA->pTimer->StartCount(TIMER_COUNTER3);


//level6 modes
		if (bMode_L6 == UMODE_L6_IDLE)
		{
			bMode_L5 = UMODE_L5_IDLE;
		}
		if (bMode_L6 == UMODE_L6_DISTRACTED)
		{
			if ( (pCA->pTimer->iCurrTick - L6_LastFlagged) > TEMP_ATTACKED_UNIT_DISTRACT_TICKS)
			{
				//release
				bMode_L6 = UMODE_L6_NOTHING;
				bMode_L5 = UMODE_L5_NOTHING;
				bMode_L4 = UMODE_L4_NOTHING;
				bMode_L3 = UMODE_L3_MOVE;
				//clear path
				pCA->pPathFinder->Destroy_Path( (cPATHENTRY*)pUnitPath);
				pUnitPath = NULL;
				pUP = NULL;
				//reset target
				Pos_sqtarget.index = L6_sqOriginalTarget.index;
				Pos_sqtarget.Calc_xy(pCA->pGameWorld->iWidth);

			} else
			{
				bMode_L5 = UMODE_L5_GUARD;
			}
		}

//level5 modes

		if (bMode_L5 == UMODE_L5_IDLE)
		{
//DEBUG
//pCA->pUnitList->Write_Log(" L5: IDLE");
//pCA->pUnitList->WriteLog_Enter(); //END DB
			bMode_L4 = UMODE_L4_IDLE;
		}
		if (bMode_L5 == UMODE_L5_TRANSFER_MATERIAL)
		{
				if (Storage.ddMaterialStored > 0)
				{
					bMode_L4 = UMODE_L4_UNLOAD_MATERIAL;
					//clear the target if we've got the wrong one
					if (Pos_sqtarget.index != sqUnloadMaterialLocation.index)
					{
						pCA->pPathFinder->Destroy_Path( (cPATHENTRY*)pUnitPath);
						pUnitPath = NULL;
						pUP = NULL;
					}
				}
				if (Storage.ddMaterialStored <= 0)
				{
					bMode_L4 = UMODE_L4_LOAD_MATERIAL;
					//clear the target if we've got the wrong one
					if (Pos_sqtarget.index != sqLoadMaterialLocation.index)
					{
						pCA->pPathFinder->Destroy_Path( (cPATHENTRY*)pUnitPath);
						pUnitPath = NULL;
						pUP = NULL;
					}
				}
		
			//if we have no unit path, we need to do something
			if (pUnitPath == NULL)
			{
				//figure out what the storages look like
				cStorageModule* pStorageSource = NULL;
				cStorageModule* pStorageDest = NULL;
				MTile* pTile_Source = &pCA->pGameWorld->pMap[sqLoadMaterialLocation.index];
				MTile* pTile_Dest = &pCA->pGameWorld->pMap[sqUnloadMaterialLocation.index];
				if (pTile_Source->pOccupant_Structure != NULL)
				if ( ((cStructureA*)(pTile_Source->pOccupant_Structure))->Storage.ddMaterialCapacity > 0)
					pStorageSource = &((cStructureA*)(pTile_Source->pOccupant_Structure))->Storage;
				if ( ((cStructureA*)(pTile_Dest->pOccupant_Structure))->Storage.ddMaterialCapacity > 0)
					pStorageDest = &((cStructureA*)(pTile_Dest->pOccupant_Structure))->Storage;
				
				//default mode is idle
				bMode_L4 = UMODE_L4_IDLE;
				//if we have something in storage, let's go unload it
				if (Storage.ddMaterialStored > 0)
				{
					pCA->pPathFinder->Destroy_Path( (cPATHENTRY*)pUnitPath);
					pUnitPath = NULL;
					pUP = NULL;
					Pos_sqtarget.index = sqUnloadMaterialLocation.index;
					Pos_sqtarget.Calc_xy(pCA->pGameWorld->iWidth);
					bMode_L4 = UMODE_L4_UNLOAD_MATERIAL;
					//pCA->pChatBuffer->Add_CBNode("Let's go unload", 6000);

				}
				//if we don't have any lets go get some
				if (Storage.ddMaterialStored == 0)
				{
					pCA->pPathFinder->Destroy_Path( (cPATHENTRY*)pUnitPath);
					pUnitPath = NULL;
					pUP = NULL;
					Pos_sqtarget.index = sqLoadMaterialLocation.index;
					Pos_sqtarget.Calc_xy(pCA->pGameWorld->iWidth);
					bMode_L4 = UMODE_L4_LOAD_MATERIAL;
					//pCA->pChatBuffer->Add_CBNode("Let's go load", 6000);
				}
			}
		}
		if (bMode_L5 == UMODE_L5_GUARD)
		{

//DEBUG
//pCA->pUnitList->Write_Log(" L5: GUARD");
//pCA->pUnitList->WriteLog_Enter(); ///END DB

//			//DEBUG
//			if (pGuardInfo == NULL)
//			{
//				//pCA->pUnitList->Write_Log("WARNING! NULL POS GUARD");
//				//pCA->pUnitList->WriteLog_Enter();
//
//			}

//DEBUG
//pCA->pUnitList->Write_Log("		Scanning...");
//pCA->pUnitList->WriteLog_Enter();
			//if ready for scan, scan
			if (pCA->pTimer->iCurrTick >= Timer_NextScan)
			{

				iScanResult = Guard_Scan(0, TEMP_SCANRANGE);

				Timer_NextScan = pCA->pTimer->iCurrTick + TEMP_SCANTIME;

			}
			//if there is an enemy detected(non-null ATTACKINFO), set mode L4_ATTACK and attack it)
			if (pAttackInfo != NULL)
			{
//DEBUG
//pCA->pUnitList->Write_Log("		Enemy Detected...");
//pCA->pUnitList->WriteLog_Enter(); 
				bMode_L4 = UMODE_L4_ATTACK_MULTI;
			}
				//else if (0 > Calc_Distance(Pos_ABS.x, Pos_ABS.y, pGuardInfo->Pos_guard.x * TILE_SIZE, pGuardInfo->Pos_guard.y * TILE_SIZE)) //Pos_sqcurr.index != pGuardInfo->Pos_guard.index ||
				 else if (Pos_ABS.x == pGuardInfo->Pos_guard.x * TILE_SIZE && Pos_ABS.y == pGuardInfo->Pos_guard.y * TILE_SIZE)
					{
//DEBUG
//pCA->pUnitList->Write_Log("		Enemy Not Detected, Unit on Guard Target...");
//pCA->pUnitList->WriteLog_Enter(); 
						//this is a HACK to introduce drift-like effects to airborne vehicles
						if (Pos_ABS.z > 0)
						if (pCA->pTimer->iCurrTick % 15 == 0)
						{
							//Pos_ABS.x += -cos(fDir_Current / RAD_DEG_CONV);
							//Pos_ABS.y += sin(fDir_Current / RAD_DEG_CONV);
							fDir_Current += ((pCA->pRandGen->rand2() % 100) - 50) * 0.1;
							Update_Position();
						}
						
						return 1;//bMode_L4 = UMODE_L4_IDLE;  //changed 02.14.2004
					}	
						else					
						{
//DEBUG
/*
pCA->pUnitList->Write_Log("		Enemy Not Detected, Unit NOT on Guard Target... Guard Target: ");
itoa(pGuardInfo->Pos_guard.index, NText, 10);
pCA->pUnitList->Write_Log(NText);
pCA->pUnitList->Write_Log(" Target Index: ");
itoa(Pos_sqtarget.index, NText, 10);
pCA->pUnitList->Write_Log(NText);
pCA->pUnitList->WriteLog_Enter();	
*/
							//HACK: adjust guard target if necessary 02.06.2003
							if (pCA->pGameWorld->pMap[pGuardInfo->Pos_guard.index].pOccupant_Unit != NULL || pCA->pGameWorld->pMap[pGuardInfo->Pos_guard.index].iType == ISURF_OCEAN)
							if (pAttackInfo == NULL) //only adjust the target if this not an attack COMMAND
								{ 
/*DEBUG
//pCA->pUnitList->Write_Log("		Adjusting guard target...");
//pCA->pUnitList->WriteLog_Enter(); *///END DB
									/* adjust target code here */ 
									pGuardInfo->Pos_guard.index = pCA->pPathFinder->Adjust_Target2(pGuardInfo->Pos_guard.index, iID);
									pGuardInfo->Pos_guard.Calc_xy(pCA->pGameWorld->iWidth);
								}	
							//simply set target, and then set L3 mode(jump L4 mode)

							//don't reset the path if the unit was already heading towards it
							if (Pos_sqtarget.index != pGuardInfo->Pos_guard.index)
							{
/*DEBUG
//pCA->pUnitList->Write_Log("		Destroying Pathlist...");
//pCA->pUnitList->WriteLog_Enter();*///END DB
									//...and destroy the unit's current path list
									pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pUnitPath);
									pUnitPath = NULL;
									pUP = (cPATHENTRY*)pUnitPath;
							}
/*DEBUG
//pCA->pUnitList->Write_Log("		Setting target index for move...");
//pCA->pUnitList->WriteLog_Enter(); *///END DB
							Pos_sqtarget.index = pGuardInfo->Pos_guard.index;
							Pos_sqtarget.Calc_xy(pCA->pGameWorld->iWidth);
							bMode_L4 = UMODE_L4_NOTHING;
							bMode_L3 = UMODE_L3_MOVE;
							if (pPrimaryWeapon != NULL)
								ddAllowableBackOffDistance = (ddMainRange / TILE_SIZE) - 1;
							else
								ddAllowableBackOffDistance = 0;
						}
		}

//DEBUG
/*
pCA->pTimer->EndCount(TIMER_COUNTER3);
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER3], NText, 10);
pCA->pTimer->Write_Log("   [Run cycle]L5: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();

//DEBUG
pCA->pTimer->StartCount(TIMER_COUNTER3);
*/


//DEBUG
pCA->pUnitList->Write_Log("Begin L4");
pCA->pUnitList->WriteLog_Enter(); 

//level4 modes
		if (bMode_L4 == UMODE_L4_BUILD)
		{
		//10.11.2003
			
///*DEBUG
//pCA->pUnitList->Write_Log("L4: Build");
//pCA->pUnitList->WriteLog_Enter(); 
			//set target
			Pos_sqtarget.index = pBuildInfo->Pos_build.index;
			Pos_sqtarget.Calc_xy(pCA->pGameWorld->iWidth);
			bMode_L3 = UMODE_L3_MOVE; //default
				ddAllowableBackOffDistance = 0;
				//temp fix 01.16.2004
//				if (pFrameType->pFrameInfo->MoveCharacteristics[MAPTILE_AIR] != -1)
//					bMode_L3 = UMODE_L3_MOVE_AIR; 
			//if we
			if (Pos_offset.x == 0 && Pos_offset.y == 0)
			if (pBuildInfo != NULL)
			{
					BOOL bInRange = FALSE;
					

					cStructInfo* pStructInfo = pCA->pStructInfoTable->pTable[pBuildInfo->iBuildType];
					if (Calc_Distance(Pos_sqcurr.x, Pos_sqcurr.y, pBuildInfo->Pos_build.x, pBuildInfo->Pos_build.y) <= ((pCA->pCmpInfoTable->pCmpTable[pBuilder->iType]->pToolInfo->fRange / TILE_SIZE)+1))
						bInRange = TRUE;
					//basically the idea here is that if it is in range of any of the building's tiles, go ahead and build it
					if (bInRange == FALSE)
					for (int lc93 = 0; lc93 < (pStructInfo->iOccMaptiles-1); lc93++)
					{
						int iX = pBuildInfo->Pos_build.x + pStructInfo->Pos_offset_array[lc93].x;
						int iY = pBuildInfo->Pos_build.y + pStructInfo->Pos_offset_array[lc93].y;
						if (Calc_Distance(Pos_sqcurr.x, Pos_sqcurr.y, iX, iY) <= ((pCA->pCmpInfoTable->pCmpTable[pBuilder->iType]->pToolInfo->fRange / TILE_SIZE)+1))
						{
							bInRange = TRUE;
							break;
						}
						 

					}

					//if (Calc_Distance(Pos_ABS.x, Pos_ABS.y, pBuildInfo->Pos_ABSbuild.x, pBuildInfo->Pos_ABSbuild.y) <= pCA->pCmpInfoTable->pCmpTable[pBuilder->iType]->pToolInfo->fRange)
					if (bInRange == TRUE)
					{
						/*disabled because this looks potentially dangerous...
						//cStructureA* pStruct = (cStructureA*)pCA->pGameWorld->pMap[pBuildInfo->Pos_build.index].pOccupant_Structure;
						//if (pStruct != NULL)
						//{
						//	if (pStruct->iBuildOrderNumber == pBuildInfo->iBuildOrderNumber)
						//		pBuildInfo->iStructureID = pStruct->iID;
								//pBuildInfo->pStructure = (void*)pStruct;
						//}
						*/

						if (pBuildInfo->iStructureID >= 0)
						{
							cStructureA* pStruct = pCA->pStructIDTable->pTable[pBuildInfo->iStructureID];
							
							if (pStruct != NULL)
							{
								//structure not finished.
								if (pStruct->fCompletion < 1)
								{
									bMode_L3 = UMODE_L3_NOTHING;
									bMode_L2 = UMODE_L2_NOTHING;
									bModes_L1[0] = UMODE_L1_ADD_FCMP;
										for (lc2 = 1; lc2 < MODES_SIMUL_L1; lc2++)
											bModes_L1[lc2] = UMODE_L1_IDLE;
								}
								if (pStruct->fCompletion >= 1)
								{
										//post construction complete acknowledgement
										int iAck = pCA->pAckTable->Get_Ack(FALSE, 0, ACK_TYPE_MENTAT_CONSTRUCTION_COMPLETE, 0, 0);
										pCA->pCommQueue->Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iAck, iID, 0, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);

										if (wExternalOrderIssued == CQ2TYPE_BUILD_STRUCTURE)
											bExternalOrderCompleted = TRUE;
										//we're done
										bMode_L4 = UMODE_L4_NOTHING;
										//update supported commands
										Update_SupportedCommands();
											bMode_L3 = UMODE_L3_IDLE; 
										
								}
							}
							//if we had a structure ID but the structure itself is NULL, the structure has been blown up from under us!
							if (pStruct == NULL)
							{
										//pCA->pChatBuffer->Add_CBNode("My structure was blown up from under me! =(", 60);
										if (wExternalOrderIssued == CQ2TYPE_BUILD_STRUCTURE)
											bExternalOrderCompleted = TRUE;
										//we're done
										bMode_L4 = UMODE_L4_NOTHING;
										//update supported commands
										Update_SupportedCommands();
										bMode_L3 = UMODE_L3_IDLE; 
							}
									
						}
							else
							{
								bMode_L3 = UMODE_L3_NOTHING;
								bMode_L2 = UMODE_L2_NOTHING;
								bModes_L1[0] = UMODE_L1_INIT_CONSTRUCT;
									for (lc2 = 1; lc2 < MODES_SIMUL_L1; lc2++)
										bModes_L1[lc2] = UMODE_L1_IDLE;
								//after this point, the unit shouldn't be able to accept new commands(until its done)
								iiNumSupportedCommands = 0;

							}
					}
			}


		}
		else if (bMode_L4 == UMODE_L4_UNLOAD)
		{
//DEBUG
//pCA->pCommQueue->Write_Log("Entered mode L4 UNLOAD");
//pCA->pCommQueue->WriteLog_Enter();
			bMode_L3 = UMODE_L3_MOVE;
				ddAllowableBackOffDistance = 1.5; //within diagonal

			//find first grassland tile
			if (Calc_Distance(Pos_ABS.x, Pos_ABS.y, Pos_sqtarget.x * TILE_SIZE, Pos_sqtarget.y * TILE_SIZE) < (1.5*TILE_SIZE))
			{
//DEBUG
//pCA->pCommQueue->Write_Log("Range calculated, within range of unload location");
//pCA->pCommQueue->WriteLog_Enter();

				for (int lc17 = 0; lc17 < 10; lc17++)
				if (UnloadBuffer[lc17] >= 0) //if this is actually a valid unit...otherwise the below logic will fail in difficult ways
				{
//DEBUG
//char NText[50];
//itoa(lc17, NText, 10);
//pCA->pCommQueue->Write_Log("unloading unit ");
//pCA->pCommQueue->Write_Log(NText);
//pCA->pCommQueue->Write_Log(". Adjusting drop target.");
//pCA->pCommQueue->WriteLog_Enter();

				//adjust target
				int iRevisedTarget = pCA->pPathFinder->Adjust_Target2(Pos_sqcurr.index, UnloadBuffer[lc17]);
				//if the new target is different, revise target
				SQCOORDSET sqRevisedTarget;
				sqRevisedTarget.index = iRevisedTarget;
				sqRevisedTarget.Calc_xy(pCA->pGameWorld->iWidth);

//pCA->pCommQueue->Write_Log("Drop target adjusted");
//pCA->pCommQueue->WriteLog_Enter();

					if (Calc_Distance(Pos_ABS.x, Pos_ABS.y, sqRevisedTarget.x * TILE_SIZE, sqRevisedTarget.y * TILE_SIZE) < (3.5*TILE_SIZE))
					{
//pCA->pCommQueue->Write_Log("Target is within 3.5 of current position");
//pCA->pCommQueue->WriteLog_Enter();

						int iOrder = Storage.Get_UnitOrder_byID(UnloadBuffer[lc17]); //this is NOT lc17, because units are getting removed the whole time
						int iDropID = UnloadBuffer[lc17];
						//char NText[50];
						//itoa(iOrder, NText, 10);
						//pCA->pChatBuffer->Add_CBNode(NText, 60);

						if ( iOrder >= 0)
						{
							//DEBUG
							char NText[50];
							pCA->pCommQueue->Write_Log("Unloading unit ");
							itoa(UnloadBuffer[lc17], NText, 10);
							pCA->pCommQueue->Write_Log(NText);
							pCA->pCommQueue->WriteLog_Enter();

							cUnitA* pUnit = pCA->pUnitTable->pUnitTable[iDropID];
							if (pUnit != NULL)
							{	
								pUnit->Pos_ABS.x = sqRevisedTarget.x * TILE_SIZE;
								pUnit->Pos_ABS.y = sqRevisedTarget.y * TILE_SIZE;
								pUnit->Pos_sqimtarget.index = sqRevisedTarget.index;
								pUnit->Pos_sqimtarget.Calc_xy(pCA->pGameWorld->iWidth);
								pUnit->Update_Position();
								pUnit->bMode_L4 = UMODE_L4_IDLE;

//pCA->pCommQueue->Write_Log("Actually performing unloading");
//pCA->pCommQueue->WriteLog_Enter();

								pCA->pGameWorld->Register_Unit(sqRevisedTarget, pUnit);
//pCA->pCommQueue->Write_Log("Unloading Complete.");
//pCA->pCommQueue->WriteLog_Enter();
								//"unload it"
								pUnit->bLoaded = FALSE;
								//remove the unit
								Storage.Remove_Unit(iDropID);
								UnloadBuffer[lc17] = -1;
//pCA->pCommQueue->Write_Log("Code Complete.");
//pCA->pCommQueue->WriteLog_Enter();								
							}
						}

					}

				}
				//then end the order
				if (wExternalOrderIssued == CQ2TYPE_UNLOAD)
					bExternalOrderCompleted = TRUE;
				//re-establish the unloading unit's available commands
//pCA->pCommQueue->Write_Log("Re-establishing supported commands.");
//pCA->pCommQueue->WriteLog_Enter();	
				Update_SupportedCommands();
				bMode_L3 = UMODE_L3_IDLE;
//pCA->pCommQueue->Write_Log("Supporting commands re-established.");
//pCA->pCommQueue->WriteLog_Enter();	
			}

		}
		else if (bMode_L4 == UMODE_L4_LOAD_MATERIAL)
		{
			//default is move
			bMode_L3 = UMODE_L3_MOVE;
				ddAllowableBackOffDistance = 1.5;
			//check distance to target
			if (Calc_Distance(Pos_ABS.x, Pos_ABS.y, sqLoadMaterialLocation.x * TILE_SIZE , sqLoadMaterialLocation.y * TILE_SIZE) <= (double(TILE_SIZE)*1.5))
			{
				//find out what we're here to extract from
				cStorageModule* pStorageTarget = NULL;

				//if there's a unit that contains something, set that
				cUnitA* pUnit = (cUnitA*)pCA->pGameWorld->pMap[sqLoadMaterialLocation.index].pOccupant_Unit;
				if (pUnit != NULL)
				if (pUnit->Storage.ddMaterialStored > 0)
				{
					pStorageTarget = &pUnit->Storage;					
				}
				//if there's a structure that contains something, set that
				cStructureA* pStruct = (cStructureA*)pCA->pGameWorld->pMap[sqLoadMaterialLocation.index].pOccupant_Structure;
				if (pStruct != NULL)
				if (pStruct->Storage.ddMaterialStored > 0)
				{
					pStorageTarget = &pStruct->Storage;
				}	
						//
				//if we found nothing, we're done
				if (pStorageTarget != NULL)
				if ( (pStorageTarget->iMaterialType == Storage.iMaterialType) || (Storage.ddMaterialStored  <= 0))
				{	
						//transfer as much as we can
						double ddAmountToMove = Storage.ddMaterialCapacity - Storage.ddMaterialStored;
						//if there's less than that much available at the target...
						if (ddAmountToMove > pStorageTarget->ddMaterialStored)
							ddAmountToMove = pStorageTarget->ddMaterialStored;
						//now do it!
						pStorageTarget->ddMaterialStored -= ddAmountToMove;
						Storage.ddMaterialStored += ddAmountToMove;
						//set material types
						Storage.iMaterialType = pStorageTarget->iMaterialType ;
						//update unit's supportedcommands
						Update_SupportedCommands();
						Recalc_Attributes();
						
					
				}
				//then we're done (no matter what path we took)
				bMode_L4 = UMODE_L4_NOTHING;
				bMode_L3 = UMODE_L3_IDLE;
				if (wExternalOrderIssued == CQ2TYPE_LOAD_MATERIAL)
					bExternalOrderCompleted = TRUE;
			}
		}
		else if (bMode_L4 == UMODE_L4_UNLOAD_MATERIAL)
		{
			//default is move
			bMode_L3 = UMODE_L3_MOVE;
				ddAllowableBackOffDistance = 1.5;
			//check distance to target
			if (Calc_Distance(Pos_ABS.x, Pos_ABS.y, sqUnloadMaterialLocation.x * TILE_SIZE , sqUnloadMaterialLocation.y * TILE_SIZE) <= (double(TILE_SIZE)*1.5))
			{
				//find out what we're here to drop to
				cStorageModule* pStorageTarget = NULL;

				//replace with just this line	
				pStorageTarget = pCA->pPathFinder->Find_ConnectedTank(sqUnloadMaterialLocation.index, Storage.iMaterialType, 0, FINDTANKMODE_PUTINTO, 0);
						//
				//if we found nothing, we're done
				if (pStorageTarget != NULL)
				{	
						//transfer as much as we can
						double ddAmountToMove = pStorageTarget->ddMaterialCapacity - pStorageTarget->ddMaterialStored;
						//if there's less than that much available at the target...
						if (ddAmountToMove > Storage.ddMaterialStored)
							ddAmountToMove = Storage.ddMaterialStored;
						//now do it!
						pStorageTarget->ddMaterialStored += ddAmountToMove;
						Storage.ddMaterialStored -= ddAmountToMove;
						//set material types
						pStorageTarget->iMaterialType = Storage.iMaterialType ;
						//
						//pCA->pChatBuffer->Add_CBNode("Went ahead.", 60);
						if (Storage.ddMaterialStored <= 0)
						{	
							Storage.ddMaterialStored = 0;
						}
						//update unit's supportedcommands
						Update_SupportedCommands();
						Recalc_Attributes();
				}
				if (pStorageTarget == NULL)
					pCA->pChatBuffer->Add_CBNode("Couldn't find tnnk.", 60);
				//then we're done (no matter what path we took)
				bMode_L4 = UMODE_L4_NOTHING;
				bMode_L3 = UMODE_L3_IDLE;
				if (wExternalOrderIssued == CQ2TYPE_UNLOAD_MATERIAL)
					bExternalOrderCompleted = TRUE;
			}
		}
		else if (bMode_L4 == UMODE_L4_LOAD)
		{
		//10.14.2006
			bMode_L3 = UMODE_L3_MOVE;
				ddAllowableBackOffDistance = 1.5;
			
			//check range to target
			cUnitA* pLoadTarget = pCA->pUnitTable->pUnitTable[iLoadTarget_ID];
			//stop command if unit was destroyed
			if (pLoadTarget == NULL)
			{
				if (wExternalOrderIssued == CQ2TYPE_LOAD)
					bExternalOrderCompleted = TRUE;
				bMode_L3 = UMODE_L3_IDLE;
			}
			if (pLoadTarget != NULL)
			if (Calc_Distance(Pos_ABS.x, Pos_ABS.y, pLoadTarget->Pos_ABS.x , pLoadTarget->Pos_ABS.y) <= (double(TILE_SIZE)*1.5))
			{
				//make sure unit's storage is not full
				if (pLoadTarget->Storage.iNumStored < 4)
				{
					//for now, only allow ground units (no ocean and no air) to be transported
					if ((pFrameType->pFrameInfo->MoveCharacteristics[ MAPTILE_OCEAN] <= 0) &&
						(pFrameType->pFrameInfo->MoveCharacteristics[ MAPTILE_AIR] <= 0) )
					{
						//
						bLoaded = TRUE;   //loaded
						//load to target unit storage
						pLoadTarget->Storage.Add_Unit(iID);
						//re-establish the loading unit's available commands
						pLoadTarget->Update_SupportedCommands();
					}
				} else
				{
					//TEMP, replace with an actual voice later 10.16.2006
					pCA->pChatBuffer->Add_CBNode("Sorry I can't load anymore!", 120);
				}

				if (wExternalOrderIssued == CQ2TYPE_LOAD)
					bExternalOrderCompleted = TRUE;
				bMode_L3 = UMODE_L3_IDLE;
			}
		}
		else if (bMode_L4 == UMODE_L4_ATTACK_MULTI)  //this is almost a full duplicate of L4_ATTACK (hopefully to supersede L4_ATTACK)  //01.08.2004
		{
/*DEBUG */
//pCA->pUnitList->Write_Log(" L4: ATTACK_MULTI");
//pCA->pUnitList->WriteLog_Enter();///END DB
		bMode_L3 = UMODE_L3_IDLE;
		//safeguard here: do not attack if you don't have a weapon!

		if (pAttackInfo != NULL && MultiFireInfo.iNumSelectedComponents != 0) 
		{
				//default mode is move
//			bMode_L3 = UMODE_L3_APPROACH;
			bMode_L3 = UMODE_L3_MOVE;
				ddAllowableBackOffDistance = (ddMainRange / TILE_SIZE) - 1;

//DEBUG
//pCA->pUnitList->Write_Log(" L4: checking for target type");
//pCA->pUnitList->WriteLog_Enter();
			//if the target has been destroyed, set to IDLE
			if (pAttackInfo->iTargetType == CQATTACKTYPE_UNIT)
			{
//DEBUG
//char NText[50];
//pCA->pUnitList->Write_Log(" L4: checking whether target of said target type has been destroyed");
//pCA->pUnitList->WriteLog_Enter();
//itoa(pAttackInfo->iEnemyID, NText, 10);
//pCA->pUnitList->Write_Log("   Enemy ID: ");
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
			if (pCA->pUnitTable->pUnitTable[pAttackInfo->iEnemyID] == NULL)
			{
//DEBUG
//pCA->pUnitList->Write_Log(" L4: target unit destroyed");
//pCA->pUnitList->WriteLog_Enter();

				delete(pAttackInfo);
				pAttackInfo = NULL;
				if (wExternalOrderIssued == CQ2TYPE_ATTACK_B)
					bExternalOrderCompleted = TRUE;

				bMode_L3 = UMODE_L3_IDLE;

			}
				else
				{
//DEBUG
//pCA->pUnitList->Write_Log(" L4: target unit NOT destroyed");
//pCA->pUnitList->WriteLog_Enter();
//DEBUG
//pCA->pUnitList->Write_Log(" L4: updating targets");
//pCA->pUnitList->WriteLog_Enter();
					Update_Targets((cUnitA*)pCA->pUnitTable->pUnitTable[pAttackInfo->iEnemyID]);
//pCA->pUnitList->Write_Log(" L4: updating local weapon targets");
//pCA->pUnitList->WriteLog_Enter();
					//added for multi to calculate local weapon directions
					Update_LocalWeaponTargets( pAttackInfo->Pos_ABSTarget.x, pAttackInfo->Pos_ABSTarget.y, &MultiFireInfo);
				}
			}

			//do the same for structures
			//(check: make sure pAttackInfo hasn't gone NULL from above 04.03.2003)
			if (pAttackInfo != NULL)
			if (pAttackInfo->iTargetType == CQATTACKTYPE_STRUCT)
			if (pCA->pStructIDTable->pTable[pAttackInfo->iEnemyID] == NULL)
			{
//DEBUG
//pCA->pUnitList->Write_Log(" L4: target structure destroyed");
//pCA->pUnitList->WriteLog_Enter();
				bMode_L3 = UMODE_L3_IDLE;
				delete(pAttackInfo);
				pAttackInfo = NULL;
				if (wExternalOrderIssued == CQ2TYPE_ATTACK_B)
					bExternalOrderCompleted = TRUE;
			}


				//temp fix 01.16.2004
//				if (pFrameType->pFrameInfo->MoveCharacteristics[MAPTILE_AIR] != -1)
//					bMode_L3 = UMODE_L3_MOVE_AIR; 
//DEBUG
//DEBUG
//pCA->pUnitList->Write_Log(" L4: min range check: ");
//gcvt(MultiFireInfo.fMinRange, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();

			//if the unit has gone for some time without repathing to the opponent, clear any existing path
			if (pCA->pTimer->iCurrTick % TEMP_TIMER_ATTACK_RESCAN == 0)

			{
				pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pUP);
				//pCA->pChatBuffer->Add_CBNode("Attack Repathing...");
				pUP = NULL;
			}

			//if the unit has reached a center square, and the target is in range(nest the ifs to prevent range-checking every tick),
			//attack

			//go to slightly inside minrange to allow projectiles to actually hit

			if (pAttackInfo != NULL)
			if (Calc_Distance(Pos_ABS.x, Pos_ABS.y, pAttackInfo->Pos_ABSTarget.x, pAttackInfo->Pos_ABSTarget.y) <= (MultiFireInfo.fMinRange * 0.9))
			{
				//HACK? An air unit does not need to have 0 offsets
				//currently check only for airborne capability
				//changed to any AIRBORNE unit (02.14.2004)
				if (Pos_ABS.z > 0)
					bMode_L3 = UMODE_L3_ATTACK_MULTI;

				if (Pos_offset.x == 0 && Pos_offset.y == 0)
				{
//DEBUG
//pCA->pUnitList->Write_Log(" L4: all go for attack");
//pCA->pUnitList->WriteLog_Enter();
				bMode_L3 = UMODE_L3_ATTACK_MULTI;
				//HACK (01.08.2004) special turning-ship mode for naval units
				//if (pFrameType->pFrameInfo->MoveCharacteristics[MAPTILE_OCEAN] > 0 && pFrameType->pFrameInfo->MoveCharacteristics[MAPTILE_GRASSLAND] <= 0)
				if (bBroadsideFlag == TRUE)
					bMode_L3 = UMODE_L3_ATTACK_MULTI_NAVAL;
				}
			}

		}
		}
		else if (bMode_L4 == UMODE_L4_ATTACK) //L4 attack is an order to move into target range and attack
		{
				ddAllowableBackOffDistance = (ddMainRange / TILE_SIZE) - 1;
			//update target information(if the target is a unit, in which case it moves)
		//default(the target has been destroyed) is IDLE
//DEBUG
/*//pCA->pUnitList->Write_Log(" L4: ATTACK");
//pCA->pUnitList->WriteLog_Enter();*///END DB
		bMode_L3 = UMODE_L3_IDLE;
		//safeguard here: do not attack if you don't have a weapon!

		if (pAttackInfo != NULL && MultiFireInfo.iNumSelectedComponents != 0) 
		{
//DEBUG
////pCA->pUnitList->Write_Log(" L4: checking for target type");
////pCA->pUnitList->WriteLog_Enter();

				//default mode is move
//			bMode_L3 = UMODE_L3_APPROACH;
			bMode_L3 = UMODE_L3_MOVE;
				ddAllowableBackOffDistance = (ddMainRange / TILE_SIZE) - 1;
				//
				//if the target has been destroyed, set to IDLE
			if (pAttackInfo->iTargetType == CQATTACKTYPE_UNIT)
			{
//DEBUG
//char NText[50];
////pCA->pUnitList->Write_Log(" L4: checking whether target of said target type has been destroyed");
////pCA->pUnitList->WriteLog_Enter();
//itoa(pAttackInfo->iEnemyID, NText, 10);
////pCA->pUnitList->Write_Log("   Enemy ID: ");
////pCA->pUnitList->Write_Log(NText);
////pCA->pUnitList->WriteLog_Enter();
			if (pCA->pUnitTable->pUnitTable[pAttackInfo->iEnemyID] == NULL)
			{
//DEBUG
////pCA->pUnitList->Write_Log(" L4: target unit destroyed");
////pCA->pUnitList->WriteLog_Enter();
				bMode_L3 = UMODE_L3_IDLE;
				delete(pAttackInfo);
				pAttackInfo = NULL;
				if (wExternalOrderIssued == CQ2TYPE_ATTACK_B)
					bExternalOrderCompleted = TRUE;
			}
				else
				{
//DEBUG
////pCA->pUnitList->Write_Log(" L4: target unit NOT destroyed");
////pCA->pUnitList->WriteLog_Enter();
//DEBUG
////pCA->pUnitList->Write_Log(" L4: updating targets");
////pCA->pUnitList->WriteLog_Enter();
					Update_Targets((cUnitA*)pCA->pUnitTable->pUnitTable[pAttackInfo->iEnemyID]);

				}
			}

			//do the same for structures
			//(check: make sure pAttackInfo hasn't gone NULL from above 04.03.2003)
			if (pAttackInfo != NULL)
			if (pAttackInfo->iTargetType == CQATTACKTYPE_STRUCT)
			if (pCA->pStructIDTable->pTable[pAttackInfo->iEnemyID] == NULL)
			{
//DEBUG
////pCA->pUnitList->Write_Log(" L4: target structure destroyed");
////pCA->pUnitList->WriteLog_Enter();
				bMode_L3 = UMODE_L3_IDLE;
				delete(pAttackInfo);
				pAttackInfo = NULL;
				if (wExternalOrderIssued == CQ2TYPE_ATTACK_B)
					bExternalOrderCompleted = TRUE;
			}
			//if the unit has gone for some time without repathing to the opponent, clear any existing path
			if (pCA->pTimer->iCurrTick % TEMP_TIMER_ATTACK_RESCAN == 0)
			{
				pCA->pPathFinder->Destroy_Path(pUP);
				pUP = NULL;
				//pCA->pChatBuffer->Add_CBNode("Attack Repathing...", 1000);
			}

				//temp fix 01.16.2004
//				if (pFrameType->pFrameInfo->MoveCharacteristics[MAPTILE_AIR] != -1)
//					bMode_L3 = UMODE_L3_MOVE_AIR; 
			//if the unit has reached a center square, and the target is in range(nest the ifs to prevent range-checking every tick),
			//attack
			if (Pos_offset.x == 0 && Pos_offset.y == 0)
			if (pAttackInfo != NULL)
			if (Calc_Distance(Pos_ABS.x, Pos_ABS.y, pAttackInfo->Pos_ABSTarget.x, pAttackInfo->Pos_ABSTarget.y) <= pCA->pCmpInfoTable->pCmpTable[pCurrentWeapon->iType]->pWeaponInfo->fRange)
			{
//DEBUG
////pCA->pUnitList->Write_Log(" L4: all go for attack");
////pCA->pUnitList->WriteLog_Enter();
				bMode_L3 = UMODE_L3_ATTACK;
			}

		}
		}
		else if (bMode_L4 == UMODE_L4_IDLE)
		{
//DEBUG
/*//pCA->pUnitList->Write_Log(" L4: IDLE");
//pCA->pUnitList->WriteLog_Enter();*///END DB
			bMode_L3 = UMODE_L3_IDLE;
		}



//DEBUG
/*
pCA->pTimer->EndCount(TIMER_COUNTER3);
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER3], NText, 10);
pCA->pTimer->Write_Log("   [Run cycle]L4: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();

//DEBUG
pCA->pTimer->StartCount(TIMER_COUNTER3);
*/


//DEBUG
pCA->pUnitList->Write_Log("Begin L3");
pCA->pUnitList->WriteLog_Enter();
//level3 modes
	switch(bMode_L3)
	{
		case UMODE_L3_MOVE:      //L3_MOVE is an order to move from the current position to the target. (02.02.2002)
//DEBUG
//pCA->pUnitList->Write_Log(" L3: MOVE");
//pCA->pUnitList->WriteLog_Enter(); //END DB

			fCurrSpeed = fMaxSpeed;

				//enabled airborne craft to repath en-route
				if (pUP == NULL)
				{

//DEBUG
//pCA->pUnitList->Write_Log("   l3: no path found. repathing....");
//pCA->pUnitList->WriteLog_Enter();
					pUP = (cPATHENTRY*)Repath((void*)pUP, sdepth);
					//DEBUG
					//pCA->pPathFinder->Path_Diag(&Pos_sqcurr, pUP);
			
					Pos_sqimtarget.index = pUP->Position.index;
					Pos_sqimtarget.Calc_xy(pCA->pGameWorld->iWidth);
					Update_Heading_Bearing_C(Pos_sqimtarget.x * TILE_SIZE, Pos_sqimtarget.y * TILE_SIZE);
				}


//DEBUG
//pCA->pUnitList->Write_Log("   l3: checking for im move completion....");
//pCA->pUnitList->WriteLog_Enter();
			//check to see the unit has completed a move yet
			//if (Pos_offset.x == 0 && Pos_offset.y == 0)  //use a more general checking condition
			if (Pos_ABS.x == Pos_sqimtarget.x * TILE_SIZE && Pos_ABS.y == Pos_sqimtarget.y * TILE_SIZE)
			{
//DEBUG
//pCA->pUnitList->Write_Log("   l3: checking for laid-in unit path....");
//pCA->pUnitList->WriteLog_Enter();
				//if there hasn't been a unitpath laid in at all(pUnitPath is null), get a new course, and update bearings)


//DEBUG
//pCA->pUnitList->Write_Log("   l3: im move was completed, checking for whether we have reached final target....");
//pCA->pUnitList->WriteLog_Enter();
				//initially set mode UMODE_L2_MOVE
				bMode_L2 = UMODE_L2_MOVE;
				//if the unit has reached its final target, and has reached center position, set L2 mode to IDLE
				if (Pos_ABS.x == Pos_sqtarget.x * TILE_SIZE && Pos_ABS.y == Pos_sqtarget.y * TILE_SIZE)
				{
//DEBUG
//pCA->pUnitList->Write_Log("   l3: final target reached....");
//pCA->pUnitList->WriteLog_Enter();
					bMode_L2 = UMODE_L2_IDLE;
					if (wExternalOrderIssued == CQ2TYPE_BATCH_MOVE)
						bExternalOrderCompleted = TRUE;
				}
						else //otherwise, just make sure the IMtarget is valid and update the correct headings. --if the unit has reached IMTarget... 
						{

							if (Pos_ABS.x == Pos_sqimtarget.x * TILE_SIZE && Pos_ABS.y == Pos_sqimtarget.y * TILE_SIZE)
							{

								//if it is the units turn for a repath
								if ( (pCA->pTimer->iCurrTick - iTick_LastRepath) > (TEMP_UNIT_REPATH_INTERVAL + (iID % 60)))
								{
										pUP = (cPATHENTRY*)Repath((void*)pUP, sdepth);
								}
//DEBUG
//pCA->pUnitList->Write_Log("   l3: im move completed, checking for paths....");
//pCA->pUnitList->WriteLog_Enter();
//DEBUG
								//...and there are still more valid nodes in the list, increment and keep moving
								if (pUP->pNext != NULL)
								{
//DEBUG
//pCA->pUnitList->Write_Log("   l3: nodes remaining, continuing....");
//pCA->pUnitList->WriteLog_Enter();

									//if the IMT square is unoccupied or is occupied by friendly units, all good
									//added "do anything you want if you're in the air" (02.14.2004)
									if (  pCA->pGameWorld->pMap[((cPATHENTRY*)pUP->pNext)->Position.index].wIMTargeted_Count == 0 || Pos_ABS.z > 0) //|| unit stack "disabled" for the moment(doesn't make sense with the parameters currently in effect(06.27.2002)
									     // pCA->pGameWorld->pMap[((cPATHENTRY*)pUP->pNext)->Position.index].bIMTargeted_Nation == iControl)
										{	

											pUP = (cPATHENTRY*)pUP->pNext;   
											Pos_sqimtarget.index = pUP->Position.index;
											Pos_sqimtarget.Calc_xy(pCA->pGameWorld->iWidth);
											Update_Heading_Bearing_C(Pos_sqimtarget.x * TILE_SIZE, Pos_sqimtarget.y * TILE_SIZE);	

										}
											else
											{
												//do nothing //changed to idle
												bMode_L2 = UMODE_L2_IDLE;
											}
								}
									else
									{
//DEBUG
//pCA->pUnitList->Write_Log("   l3: no more viable nodes. repathing....");
//pCA->pUnitList->WriteLog_Enter();	
										//if no more valid nodes, repath (after 02.01.2007, we shouldn't ever get here...)
										//YET WE ARE.... (07.18.2007)
										//revised: do nothing
										//revised again: idle.
										bMode_L2 = UMODE_L2_IDLE;
											//pUP = (cPATHENTRY*)Repath((void*)pUP, sdepth);
											//if we get back nothing, flag that we're having trouble reaching 
											//DEBUG
											//pCA->pPathFinder->Path_Diag(&Pos_sqcurr, pUP);
									}
								
//DEBUG
//pCA->pUnitList->Write_Log("   l3: complete.");
//pCA->pUnitList->WriteLog_Enter();
							}
						}
		
			}
				else  //(if the unit has not completed a move)
				{
					//keep going until it reaches its IM Target
					bMode_L2 = UMODE_L2_MOVE;
				}
		break;  //end codefor UMODE_L3_MOVE
		case UMODE_L3_MOVE_AIR:   //01.16.2004
//DEBUG
/*//pCA->pUnitList->Write_Log(" L3: MOVE AIR");
//pCA->pUnitList->WriteLog_Enter(); *///END DB
			if (fCurrSpeed < fMaxSpeed)
				fCurrSpeed += 0.03;
			if (fCurrSpeed > fMaxSpeed)
				fCurrSpeed = fMaxSpeed;

//DEBUG
////pCA->pUnitList->Write_Log(" L3: MOVE AIR");
////pCA->pUnitList->WriteLog_Enter();
			Pos_sqimtarget.x = Pos_sqtarget.x;
			Pos_sqimtarget.y = Pos_sqtarget.y;
			Update_Heading_Bearing_C(Pos_sqtarget.x * TILE_SIZE, Pos_sqtarget.y * TILE_SIZE);
					bMode_L2 = UMODE_L2_MOVE_AIR;

//changed to account for the fact that an aircraft cannot stop on a target....(at least for now)
//instead, it drops speed and idles
			if (TRUE)
			{
			double ddR = Calc_Distance( Pos_ABS.x, Pos_ABS.y, Pos_sqtarget.x * TILE_SIZE, Pos_sqtarget.y * TILE_SIZE);
			if (ddR < fMaxSpeed * 30)
			{
				//drop to 0
				fCurrSpeed = fMaxSpeed * ((ddR/ (fMaxSpeed*30)) + 0.1);

				//if (fCurrSpeed > fMaxSpeed / 2)
				//	fCurrSpeed -= 0.05 * ddR / (30 * fMaxSpeed);

//					Pos_ABS.x = Pos_sqtarget.x * TILE_SIZE;
//					Pos_ABS.y = Pos_sqtarget.y * TILE_SIZE;
//					bMode_L2 = UMODE_L2_IDLE;
			}
				if (ddR < fCurrSpeed * 2)
				{
					Pos_ABS.x = Pos_sqtarget.x * TILE_SIZE;
					Pos_ABS.y = Pos_sqtarget.y * TILE_SIZE;
					if (wExternalOrderIssued == CQ2TYPE_BATCH_MOVE)
						bExternalOrderCompleted = TRUE;
					bMode_L2 = UMODE_L2_IDLE;
				}
			}
/*				else
				{
					Update_Heading_Bearing_C(Pos_sqtarget.x * TILE_SIZE, Pos_sqtarget.y * TILE_SIZE);
					bMode_L2 = UMODE_L2_MOVE_AIR;
				}
*/
		break;
		case UMODE_L3_ATTACK:			//check the current weapon's attack procedure
//DEBUG
/*//pCA->pUnitList->Write_Log(" L3: ATTACK");
//pCA->pUnitList->WriteLog_Enter(); *///END DB
			if (pCmpTable[pCurrentWeapon->iType]->pWeaponInfo->iAttackProcedure == ATTACK_PROJECTILE)
					bMode_L2 = UMODE_L2_ATTACK_PROJECTILE;			
		break;
		case UMODE_L3_ATTACK_MULTI:
//DEBUG
/*//pCA->pUnitList->Write_Log(" L3: ATTACK MULTI");
//pCA->pUnitList->WriteLog_Enter(); *///END DB
			bMode_L2 = UMODE_L2_ATTACK_MULTI;
				//divert to air if necssary
//				if (Pos_ABS.z > 0)
//					bMode_L2 = UMODE_L2_ATTACK_MULTI_AIR;

		break;
		case UMODE_L3_ATTACK_MULTI_NAVAL:    //01.08.2004 "special" mode
//DEBUG
/*//pCA->pUnitList->Write_Log("L3: ATTACK_MULTI_NAVAL");
//pCA->pUnitList->WriteLog_Enter(); *///END DB
			ddx = fabs(fDir_Current - pAttackInfo->fAttackDirection);

			if (ddx > 360 - ddx)
				ddx = 360 - ddx;
			
			if (fabs(ddx - 90) > FP_TOLERANCE)
			{
//DEBUG
////pCA->pUnitList->Write_Log(" Current Ship Direction: ");
//gcvt(fDir_Current, 7, NText);
////pCA->pUnitList->Write_Log(NText);
////pCA->pUnitList->WriteLog_Enter();
////pCA->pUnitList->Write_Log(" Current Target Direction: ");
//gcvt(pAttackInfo->fAttackDirection, 7, NText);
////pCA->pUnitList->Write_Log(NText);
////pCA->pUnitList->WriteLog_Enter();
				int iCondition = -1;

				//this is clunky...
				//calculate what the two target angles are...
				ddx1 = pAttackInfo->fAttackDirection - 90;
				if (ddx1 < 0)
					ddx1 += 360;

				ddx2 = pAttackInfo->fAttackDirection + 90;
				if (ddx2 >= 360)
					ddx2 -= 360;

				//figure out which one, ddx1 or ddx2 is closer
				ddx3 = fabs(fDir_Current - ddx1);
				if (ddx3 > 360 - ddx3)
					  ddx3 = 360 - ddx3;
				ddx4 = fabs(fDir_Current - ddx2);
				if (ddx4 > 360 - ddx4)
					  ddx4 = 360 - ddx4;

				//approach ddx1
				if (ddx3 < ddx4)
				{
					
					clockwise = ddx1 - fDir_Current;
					counterclockwise = fDir_Current - ddx1;
					
					//the approach would go like this:
					//only one of these is "correct" (as in, no angle wraparound occurred)
					//how do we check? we have case 1:
						//clockwise is incorrect(it is negative), counterclockwise is correct:
						if (clockwise < 0)
						{
							clockwise = 360 - counterclockwise;
						}
					//case 2: counterclockwise is incorrect(it is negative), clockwise is correct:
						if (counterclockwise < 0)
						{
							counterclockwise = 360 - clockwise;
						}
						
				}	else  //approach ddx2
					{
					clockwise = ddx2 - fDir_Current;
					counterclockwise = fDir_Current - ddx2;
					
					//we have case 1:
						//clockwise is incorrect(it is negative), counterclockwise is correct:
						if (clockwise < 0)
						{
							clockwise = 360 - counterclockwise;
						}
					//case 2: counterclockwise is incorrect(it is negative), clockwise is correct:
						if (counterclockwise < 0)
						{
							counterclockwise = 360 - clockwise;
						}
					}

//DEBUG
////pCA->pUnitList->Write_Log(" Clockwise: ");
//gcvt(clockwise, 7, NText);
////pCA->pUnitList->Write_Log(NText);
////pCA->pUnitList->WriteLog_Enter();
////pCA->pUnitList->Write_Log(" Counterclockwise: ");
//gcvt(counterclockwise, 7, NText);
////pCA->pUnitList->Write_Log(NText);
////pCA->pUnitList->WriteLog_Enter();

				//perform the turn
				if (clockwise < counterclockwise)
				{
					fDir_Current += TEMP_UROTATESPEED;
				}	else
						fDir_Current -= TEMP_UROTATESPEED;

				//perform dead-on calculations  //(note: the fabs() is really a patch on a somewhat broken angle determination routine above) (01.08.2004)
				if (clockwise < TEMP_UROTATESPEED || counterclockwise < TEMP_UROTATESPEED)
				{
					if (ddx3 < ddx4)
						fDir_Current = ddx1;
							else
						fDir_Current = ddx2;
				}

				//wrap-around
				if (fDir_Current >= 360)
					fDir_Current -= 360;
				if (fDir_Current < 0)
					fDir_Current += 360;
//DEBUG
/*
//pCA->pUnitList->Write_Log(" END Current Ship Direction: ");
gcvt(fDir_Current, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
//pCA->pUnitList->Write_Log(" END Current Target Direction: ");
gcvt(fDir_Target, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/

				bMode_L2 = UMODE_L2_ATTACK_MULTI;
			}	else
				bMode_L2 = UMODE_L2_ATTACK_MULTI;

 		break;
		case UMODE_L3_IDLE:
//DEBUG
/*//pCA->pUnitList->Write_Log(" L3: IDLE");
//pCA->pUnitList->WriteLog_Enter(); *///END DB
			bMode_L2 = UMODE_L2_IDLE;
		break;
	}

//DEBUG
/*
pCA->pTimer->EndCount(TIMER_COUNTER3);
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER3], NText, 10);
pCA->pTimer->Write_Log("   [Run cycle]L3: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();

//DEBUG
pCA->pTimer->StartCount(TIMER_COUNTER3);
*/

//DEBUG
pCA->pUnitList->Write_Log("Begin L2");
pCA->pUnitList->WriteLog_Enter(); 

//level2 modes
	switch(bMode_L2)
	{
		case UMODE_L2_MOVE_AIR:
//DEBUG
////pCA->pUnitList->Write_Log(" L2: MOVE AIR");
////pCA->pUnitList->WriteLog_Enter();
			//basically pass off mode to begin translating, 
			//if need to turn, turn
			bModes_L1[0] = UMODE_L1_TRANSLATE_AIR;
				for (lc2 = 1; lc2 < MODES_SIMUL_L1; lc2++)
				{
					bModes_L1[lc2] = UMODE_L1_NOTHING;
				}
			if (fabs(fDir_Current - fDir_Target) > FP_TOLERANCE)
			{	
				
				bModes_L1[1] = UMODE_L1_TURN;
				double ddR = Calc_Distance( Pos_ABS.x, Pos_ABS.y, Pos_sqtarget.x * TILE_SIZE, Pos_sqtarget.y * TILE_SIZE);
				
				if (ddR < fMaxSpeed * 50)
				{
					bModes_L1[1] = UMODE_L1_TURN_SLOW;				
				}

			}
			if (Pos_ABS.z != 24)
			{
				bModes_L1[2] = UMODE_L1_ASCEND;
			}

/*  //prevent it from landing...or idling
			if (Calc_Distance(Pos_ABS.x, Pos_ABS.y, Pos_sqtarget.x * TILE_SIZE, Pos_sqtarget.y * TILE_SIZE) < fMaxSpeed * 25)
			{
				if (pCA->pGameWorld->pMap[Pos_sqtarget.index].pOccupant_Unit == NULL && pFrameType->pFrameInfo->MoveCharacteristics[pCA->pGameWorld->pMap[Pos_sqtarget.index].iType] >= 0)
				{
					//disabled for now(01.17.2004)
					bModes_L1[2] = UMODE_L1_DESCEND;
				}
			}
			if (Calc_Distance(Pos_ABS.x, Pos_ABS.y, Pos_sqtarget.x * TILE_SIZE, Pos_sqtarget.y * TILE_SIZE) < 5)
			{
				
				bModes_L1[0] = UMODE_L1_IDLE;
			}
*/
		break;
		case UMODE_L2_MOVE:  //L2_MOVE is an order to move to the next immediate target
//DEBUG

//pCA->pUnitList->Write_Log(" L2: MOVE");
//pCA->pUnitList->WriteLog_Enter();

gcvt(Pos_ABS.x, 7, NText);
//pCA->pUnitList->Write_Log("	X Pos: ");
//pCA->pUnitList->Write_Log(NText);
gcvt(Pos_ABS.y, 7, NText);
//pCA->pUnitList->Write_Log("	Y Pos: ");
//pCA->pUnitList->Write_Log(NText);
gcvt(Pos_sqtarget.x * TILE_SIZE, 7, NText);
//pCA->pUnitList->Write_Log("	X Target: ");
//pCA->pUnitList->Write_Log(NText);
gcvt(Pos_sqtarget.y * TILE_SIZE, 7, NText);
//pCA->pUnitList->Write_Log("	Y Target: ");
//pCA->pUnitList->Write_Log(NText);
gcvt(Pos_sqimtarget.x * TILE_SIZE, 7, NText);
//pCA->pUnitList->Write_Log("	X IMTarget: ");
//pCA->pUnitList->Write_Log(NText);
gcvt(Pos_sqimtarget.y * TILE_SIZE, 7, NText);
//pCA->pUnitList->Write_Log("	Y IMTarget: ");
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();

			//set L1 mode depending on current status of the unit
			//initially set mode IDLE
			bModes_L1[0] = UMODE_L1_IDLE;
			bModes_L1[1] = UMODE_L1_RETRAIN_ALL;
				for (lc2 = 2; lc2 < MODES_SIMUL_L1; lc2++)
				{
					bModes_L1[lc2] = UMODE_L1_NOTHING;
				}
			
			//check test: recalc
			Pos_sqcurr.Calc_Index(pCA->pGameWorld->iWidth);
			Pos_sqimtarget.Calc_Index(pCA->pGameWorld->iWidth);
	

			if (Pos_ABS.x != Pos_sqimtarget.x * TILE_SIZE || Pos_ABS.y != Pos_sqimtarget.y * TILE_SIZE)
			{

				if (fDir_Current != fDir_Target) 
				{
					bModes_L1[0] = UMODE_L1_TURN;
					bModes_L1[1] = UMODE_L1_RETRAIN_ALL;
						for (lc2 = 2; lc2 < MODES_SIMUL_L1; lc2++)
						{
							bModes_L1[lc2] = UMODE_L1_NOTHING;
						}

				}
						else
						{
							bModes_L1[0] = UMODE_L1_TRANSLATE;
							bModes_L1[1] = UMODE_L1_RETRAIN_ALL;
							for (lc2 = 2; lc2 < MODES_SIMUL_L1; lc2++)
							{
								bModes_L1[lc2] = UMODE_L1_NOTHING;
							}
//			if (Pos_ABS.z != 24)
//			{
//				bModes_L1[2] = UMODE_L1_ASCEND;
//			}
//DEBUG
/*
//pCA->pUnitList->Write_Log("		Turn break reached! Angle: ");
gcvt(fDir_Target, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/						
			}
			}
		break;
		case UMODE_L2_ATTACK_MULTI:
//DEBUG
//pCA->pUnitList->Write_Log(" L2: ATTACK MULTI");
//pCA->pUnitList->WriteLog_Enter();
			bReadyToFire = TRUE;
			for (lc1 = 0; lc1 < MultiFireInfo.iNumSelectedComponents; lc1++)
			{
				double checkquantity = FP_TOLERANCE;
//HACK: correct here for FP problems
if (fabs(fDir_Current + MultiFireInfo.ppSelectedComponents[lc1]->fDir_Offset - MultiFireInfo.arrAttackDirections[lc1]) < checkquantity)
{
	MultiFireInfo.ppSelectedComponents[lc1]->fDir_Offset = MultiFireInfo.arrAttackDirections[lc1] - fDir_Current;
}
				if (fabs(MultiFireInfo.arrAttackDirections[lc1] - fDir_Current - MultiFireInfo.ppSelectedComponents[lc1]->fDir_Offset) > ALLOW_AIM_DEVIATION && pCmpTable[MultiFireInfo.ppSelectedComponents[lc1]->iType]->pWeaponInfo->iPreAttackProcedure == PREATTACK_AIM)
				{
					bReadyToFire = FALSE;
					// break;  //?	
				}
			}
			if (bReadyToFire == TRUE)
				bModes_L1[0] = UMODE_L1_FIRE_MULTI;
					else
					{
						if (pCA->pTimer->iCurrTick - iLastUpdateWeaponTargets > 60)
						{
							Update_LocalWeaponTargets(pAttackInfo->Pos_ABSTarget.x, pAttackInfo->Pos_ABSTarget.y, &MultiFireInfo);
						}
						bModes_L1[0] = UMODE_L1_AIM_MULTI;
					}
	
				for (lc2 = 1; lc2 < MODES_SIMUL_L1; lc2++)
				{
					bModes_L1[lc2] = UMODE_L1_NOTHING;
				}

		break;
		case UMODE_L2_ATTACK_MULTI_AIR:
//DEBUG
////pCA->pUnitList->Write_Log("L2: ATTACK_MULTI_AIR: ");
//gcvt(fDir_Target, 7, NText);
////pCA->pUnitList->Write_Log(NText);
////pCA->pUnitList->WriteLog_Enter();

			
			bReadyToFire = TRUE;
			for (lc1 = 0; lc1 < MultiFireInfo.iNumSelectedComponents; lc1++)
			{
//HACK: correct here for FP problems
if (fabs(fDir_Current + MultiFireInfo.ppSelectedComponents[lc1]->fDir_Offset - MultiFireInfo.arrAttackDirections[lc1]) < FP_TOLERANCE)
{
	MultiFireInfo.ppSelectedComponents[lc1]->fDir_Offset = MultiFireInfo.arrAttackDirections[lc1] - fDir_Current;
}
				if (fabs(MultiFireInfo.arrAttackDirections[lc1] - fDir_Current - MultiFireInfo.ppSelectedComponents[lc1]->fDir_Offset) > ALLOW_AIM_DEVIATION && pCmpTable[MultiFireInfo.ppSelectedComponents[lc1]->iType]->pWeaponInfo->iPreAttackProcedure == PREATTACK_AIM)
				{
					bReadyToFire = FALSE;
					// break;  //?	
				}
			}

				for (lc2 = 0; lc2 < MODES_SIMUL_L1; lc2++)
				{
					bModes_L1[lc2] = UMODE_L1_NOTHING;
				}			
			if (bReadyToFire == TRUE)
				bModes_L1[0] = UMODE_L1_FIRE_MULTI;
					else
				bModes_L1[0] = UMODE_L1_AIM_MULTI;

				bModes_L1[1] = UMODE_L1_TRANSLATE_AIR;

				Update_Heading_Bearing_C(Pos_sqtarget.x, Pos_sqtarget.y);
	
//			if (fabs(fDir_Current - fDir_Target) > FP_TOLERANCE)
//			{	
				
//				bModes_L1[1] = UMODE_L1_TURN;
//				double ddR = Calc_Distance( Pos_ABS.x, Pos_ABS.y, Pos_sqtarget.x * TILE_SIZE, Pos_sqtarget.y * TILE_SIZE);
				
//				if (ddR < fMaxSpeed * 50)
//				{
//					bModes_L1[1] = UMODE_L1_TURN_SLOW;				
//				}

//			}
		break;
		case UMODE_L2_ATTACK_PROJECTILE:  //attack the enemy using projectile attack method
//DEBUG
////pCA->pUnitList->Write_Log(" L2: ATTACK PROJECTILE");
////pCA->pUnitList->WriteLog_Enter();
//HACK: correct here for FP problems
if (fabs(fDir_Current + pCurrentWeapon->fDir_Offset - pAttackInfo->fAttackDirection) < FP_TOLERANCE)
{
	pCurrentWeapon->fDir_Offset = pAttackInfo->fAttackDirection - fDir_Current;
}

			if (fabs(pAttackInfo->fAttackDirection - fDir_Current - pCurrentWeapon->fDir_Offset) > FP_TOLERANCE && pCmpTable[pCurrentWeapon->iType]->pWeaponInfo->iPreAttackProcedure == PREATTACK_AIM)
				bModes_L1[0] = UMODE_L1_AIM;
					else
				bModes_L1[0] = UMODE_L1_FIRE_PROJECTILE;

				for (lc2 = 1; lc2 < MODES_SIMUL_L1; lc2++)
				{
					bModes_L1[lc2] = UMODE_L1_NOTHING;
				}
		break;
		case UMODE_L2_IDLE:
//DEBUG
/*//pCA->pUnitList->Write_Log(" L2: IDLE");
//pCA->pUnitList->WriteLog_Enter(); *///END DB

				bModes_L1[0] = UMODE_L1_IDLE;
				for (lc2 = 1; lc2 < MODES_SIMUL_L1; lc2++)
				{
					bModes_L1[lc2] = UMODE_L1_IDLE;
				}
		break;
	}
//DEBUG
 pCA->pUnitList->Write_Log("Begin L1");
pCA->pUnitList->WriteLog_Enter(); //END DB

//DEBUG
/*
pCA->pTimer->EndCount(TIMER_COUNTER3);
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER3], NText, 10);
pCA->pTimer->Write_Log("   [Run cycle]L2: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();

//DEBUG
pCA->pTimer->StartCount(TIMER_COUNTER3);
*/

	//run based on what L1 mode(s) are:
for (lc1 = 0; lc1 < MODES_SIMUL_L1; lc1++)
{
		//DEBUG
		if (bModes_L1[1] == UMODE_L1_TRANSLATE_AIR)
		{
//			//pCA->pUnitList->Write_Log("DETECTED!");
//			//pCA->pUnitList->WriteLog_Enter();
		}
		if (bModes_L1[lc1] == UMODE_L1_INIT_CONSTRUCT)
		{
//DEBUG
pCA->pUnitList->Write_Log(" L1: INIT_CONSTRUCT");
pCA->pUnitList->WriteLog_Enter();
			if (pBuildInfo->iStructureID < 0)
			{

				//temporary implementation 10.12.2003)
				char NewName[TEMP_STD_STRLEN];
				ZeroMemory( NewName, sizeof(char)*TEMP_STD_STRLEN);
				
				int iSector = pCA->pGameWorld->pMap[Pos_sqcurr.index].iSector;
				strcat( NewName, pCA->pStructInfoTable->pTable[pBuildInfo->iBuildType]->GeneralName);
				//no longer append sector name
				//strcat( NewName, " ");
				//strcat( NewName, pCA->pSectorTable->Sectors[iSector]->Name);
					
				cStructureA* pStruct = pCA->pStructList->Create_Structure( NewName, 0, pBuildInfo->iBuildType, iControl, pBuildInfo->Pos_build.x, pBuildInfo->Pos_build.y);
				pBuildInfo->iStructureID = pStruct->iID;
				pStruct->iBuildOrderNumber = pBuildInfo->iBuildOrderNumber;

				pBuilder->llNextUse = pCA->pTimer->iCurrTick;

						//TUTORIAL HOOK!
						//if struct is capital, set hook
						if (pStruct->iType == TEMP_STYPE_RSC)
						{
							pCA->pInterface->Tutorial.iTick_MostRecentCapitalStartConstruction = pCA->pTimer->iCurrTick;
						}

			}
		}
		if (bModes_L1[lc1] == UMODE_L1_ADD_FCMP)
		{
//DEBUG
pCA->pUnitList->Write_Log(" L1: ADD_FCMP");
pCA->pUnitList->WriteLog_Enter();
			//add fcompletion to the designated structure(temporary implementation 10.12.2003)
			if (pBuildInfo->iStructureID < 0)
			{
				pCA->pChatBuffer->Add_CBNode("ERROR 117: structure ID somehow negative here. ", 60);
			} else if (pCA->pStructIDTable->pTable[pBuildInfo->iStructureID] != NULL)
			{
				
//DEBUG
////pCA->pUnitList->Write_Log(" L1:  Checking Times:");
////pCA->pUnitList->WriteLog_Enter();
				if (pCA->pTimer->iCurrTick >= pBuilder->llNextUse)
				{
					cStructureA* pStruct = pCA->pStructIDTable->pTable[pBuildInfo->iStructureID];
					cStructInfo* pInfo = pCA->pStructInfoTable->pTable[pBuildInfo->iBuildType];
//DEBUG
////pCA->pUnitList->Write_Log(" L1:  Checking Completion");
////pCA->pUnitList->WriteLog_Enter();
					if (pStruct->fCompletion < 1)
					{
//DEBUG
////pCA->pUnitList->Write_Log(" L1:  Adding FCMP:");
////pCA->pUnitList->WriteLog_Enter();
						//
						BOOL bEnough = TRUE;
						for (int lc4 = 0; lc4 < NUM_MATERIAL_TYPES; lc4++)
						if (pInfo->MaterialCost.Materials[lc4] * TEMP_BUILDRATE > 0)
						{
							cStorageModule* pStorage = pCA->pPathFinder->Find_ConnectedTank(pBuildInfo->Pos_build.index, lc4, pInfo->MaterialCost.Materials[lc4] * TEMP_BUILDRATE, FINDTANKMODE_TAKEFROM, 0);
							if (pStorage == NULL)
							{
								bEnough = FALSE;
								break;
							}
						}

						if (bEnough == TRUE)
						{
						
						for (int lc5 = 0; lc5 < NUM_MATERIAL_TYPES; lc5++)
						if (pInfo->MaterialCost.Materials[lc5] * TEMP_BUILDRATE > 0)
						{
								double deductamount = pInfo->MaterialCost.Materials[lc5] * TEMP_BUILDRATE;
								cStorageModule* pStorage = pCA->pPathFinder->Find_ConnectedTank(pBuildInfo->Pos_build.index, lc5, deductamount, FINDTANKMODE_TAKEFROM, 0);
                                								
								//decrease storage by this much
								pStorage->ddMaterialStored -= deductamount;
								if (pStorage->ddMaterialStored <= 0)
									{
										pStorage->ddMaterialStored = 0;
									}
							}
							//for now, add 100 resources worth of FCMP (12.28.2004)
							pStruct->fCompletion += TEMP_BUILDRATE; 
							//add HP as well
							pStruct->iHP += pStruct->iHPMax * TEMP_BUILDRATE * 1.2;
							//if structure is now complete, set tutorial completion hook
							//TUTORIAL HOOK//////////////////////////
							if (pStruct->iControl == pCA->pGameOptions->iControl)
							if (pStruct->fCompletion >= 1)
							{
								if (pStruct->iType == TEMP_STYPE_RSC)
								{
									//OK!	
									pCA->pInterface->Signal_Chime("Task Completed!");
									pCA->pInterface->Tutorial.iTick_MostRecentCapitalFinishConstruction = pCA->pTimer->iCurrTick;
								}
								if (pStruct->iType == TEMP_STYPE_TANK)
								{
									pCA->pInterface->Tutorial.iNumStoragesCompleted++;
									if (pCA->pInterface->Tutorial.iNumStoragesCompleted == 2)
									{
										//OK!	
										pCA->pInterface->Signal_Chime("Task Completed!");
										pCA->pInterface->Tutorial.iTick_SecondStorageCompleted = pCA->pTimer->iCurrTick;
									}
								}	
								if (pStruct->iType == TEMP_STYPE_FORGE)
								{
									pCA->pInterface->Tutorial.iTick_MostRecentForgeFinishConstruction = pCA->pTimer->iCurrTick;
									//OK!	
									pCA->pInterface->Signal_Chime("Task Completed!");
								}
								if (pStruct->iType == TEMP_STYPE_FACTORY_ABC)
								{
									pCA->pInterface->Tutorial.iTick_MostRecentFactoryFinishConstruction = pCA->pTimer->iCurrTick;
									//OK!	
									pCA->pInterface->Signal_Chime("Task Completed!");		
								}

							}
							////END TUTORIAL HOOK////////////////////////
						}
						//
						pBuilder->llNextUse += pCA->pCmpInfoTable->pCmpTable[pBuilder->iType]->pToolInfo->iUseRate;
					}
				}
			}
		}
		if (bModes_L1[lc1] == UMODE_L1_IDLE)
		{
			//HACK? (this is to cause air units to drift while in the air)
			/*
			if (pFrameType->pFrameInfo->MoveCharacteristics[MAPTILE_AIR] != -1)
			if (Pos_ABS.z > 0 && pCA->pTimer->iCurrTick % ((rand() % 60)+1) == 0)
			{
					Pos_ABS.x += ((rand() % 255)-128) / 100.0;
					Pos_ABS.y += ((rand() % 255)-128) / 100.0;

			}
			*/
			fCurrSpeed = 0;
//DEBUG
////pCA->pUnitList->Write_Log(" L1: IDLE");
////pCA->pUnitList->WriteLog_Enter();
		}
		if (bModes_L1[lc1] == UMODE_L1_NOTHING)
		{
//DEBUG
////pCA->pUnitList->Write_Log(" L1: NOTHING");
////pCA->pUnitList->WriteLog_Enter();
		}
		if (bModes_L1[lc1] == UMODE_L1_TRANSLATE)
		{
//DEBUG

//pCA->pUnitList->Write_Log(" L1: TRANSLATE");
//pCA->pUnitList->WriteLog_Enter();
gcvt(Heading.x, 7, NText);
//pCA->pUnitList->Write_Log("X Heading: ");
//pCA->pUnitList->Write_Log(NText);
gcvt(Heading.y, 7, NText);
//pCA->pUnitList->Write_Log(" Y Heading: ");
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();

			//check(using the alternate scheme) for a to-target distance of less than the speed (extra? 06.13.2002)
			if (Calc_Distance(Pos_ABS.x, Pos_ABS.y, Pos_sqimtarget.x * TILE_SIZE, Pos_sqimtarget.y * TILE_SIZE) < fCurrSpeed)
				{
					Pos_ABS.x = (Pos_sqimtarget.x * TILE_SIZE);
					Pos_ABS.y = (Pos_sqimtarget.y * TILE_SIZE);
				}	
					else
					{
						//very simple....change coordinates based on the unit headings (if a higher level unit command needs to move, they need to set the heading first)
						Pos_ABS.x = Pos_ABS.x + Heading.x;
						Pos_ABS.y = Pos_ABS.y + Heading.y;
					}
			Update_Position();
		}

		if (bModes_L1[lc1] == UMODE_L1_TRANSLATE_AIR)
		{
//DEBUG
////pCA->pUnitList->Write_Log(" L1: TRANSLATE_AIR");
////pCA->pUnitList->WriteLog_Enter();
						//very simple....change coordinates based on the unit headings (if a higher level unit command needs to move, they need to set the heading first)
						Pos_ABS.x = Pos_ABS.x + Heading.x;
						Pos_ABS.y = Pos_ABS.y + Heading.y;
			Update_Position();
		}
		if (bModes_L1[lc1] == UMODE_L1_ASCEND)
		{
//DEBUG
////pCA->pUnitList->Write_Log(" L1: ASCEND");
////pCA->pUnitList->WriteLog_Enter();
			if ( 24 - Pos_ABS.z < fCurrSpeed)
			{
				Pos_ABS.z = 24;
			} else
				{
					Pos_ABS.z += fCurrSpeed;			
				}
		}
		if (bModes_L1[lc1] == UMODE_L1_DESCEND)
		{
//DEBUG
////pCA->pUnitList->Write_Log(" L1: DESCEND");
////pCA->pUnitList->WriteLog_Enter();
			if (Pos_ABS.z < 2)
			{
				Pos_ABS.z = 0;
			}
				else
				{
					Pos_ABS.z -= 2;
				}
		}
		if (bModes_L1[lc1] == UMODE_L1_TURN)
		{
//DEBUG
/*
//pCA->pUnitList->Write_Log(" L1: TURN");
//pCA->pUnitList->WriteLog_Enter();
//DEBUG
//pCA->pUnitList->Write_Log(" Current Angle: ");
gcvt(fDir_Current, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->Write_Log(" Target Angle: ");
gcvt(fDir_Target, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/
			if (fDir_Target != fDir_Current) //just another little safeguard against unnecessary turns
			{
				if (fDir_Target > fDir_Current)
					{
	
						clockwise = fDir_Target - fDir_Current;    
						counterclockwise = fDir_Current + 360 - fDir_Target;    
					}
					else
						{
							clockwise = 360 - fDir_Current + fDir_Target;
							counterclockwise = fDir_Current - fDir_Target;
						}
				if (clockwise <= counterclockwise)    
					fDir_Current += TEMP_UROTATESPEED;
					else
						fDir_Current -= TEMP_UROTATESPEED; 
					
				if (clockwise < TEMP_UROTATESPEED || counterclockwise < TEMP_UROTATESPEED)
					fDir_Current = fDir_Target;

				if (fDir_Current >= 360)
					fDir_Current =- 360;
				if (fDir_Current < 0)                 
					fDir_Current += 360;	
			}
		
		}
		if (bModes_L1[lc1] == UMODE_L1_TURN_SLOW)
		{
//DEBUG
////pCA->pUnitList->Write_Log(" L1: TURN SLOW");
////pCA->pUnitList->WriteLog_Enter();
			if (fDir_Target != fDir_Current) //just another little safeguard against unnecessary turns
			{
				double ddR = Calc_Distance( Pos_ABS.x, Pos_ABS.y, Pos_sqtarget.x * TILE_SIZE, Pos_sqtarget.y * TILE_SIZE);
				double turnspeed = ((TEMP_UROTATESPEED) * (ddR / (2* 30 * fMaxSpeed)) + 0.01) + (pCA->pRandGen->rand2() % 10) / 5.0;		
				if (fDir_Target > fDir_Current)
					{
	
						clockwise = fDir_Target - fDir_Current;    
						counterclockwise = fDir_Current + 360 - fDir_Target;    
					}
					else
						{
							clockwise = 360 - fDir_Current + fDir_Target;
							counterclockwise = fDir_Current - fDir_Target;
						}
				if (clockwise <= counterclockwise)    
					fDir_Current += turnspeed;
					else
						fDir_Current -= turnspeed; 
					
				if (clockwise < turnspeed || counterclockwise < turnspeed)
					fDir_Current = fDir_Target;

				if (fDir_Current >= 360)
					fDir_Current =- 360;
				if (fDir_Current < 0)                 
					fDir_Current += 360;	
			}
		}
		if (bModes_L1[lc1] == UMODE_L1_RETRAIN_ALL)
		{
			
			//aim all weapons simultaneously
			for (int lc13 = 0; lc13 < MultiFireInfo.iNumSelectedComponents; lc13++)
			{
				pComponent = MultiFireInfo.ppSelectedComponents[lc13];
				pCmpInfo = pCmpTable[pComponent->iType];

				//fmod returns the "floating-point remainder"
				float fTarget = fmod(pComponent->fDir_Offset_Default + fDir_Current, 360);
				float fCurr = fmod(pComponent->fDir_Offset + fDir_Current, 360);

				
				if (fabs(fCurr -  fTarget) > TEMP_UROTATESPEED)
				{
					if (fTarget > fCurr)
					{
						clockwise = fTarget - fCurr;    
						counterclockwise = fCurr + 360 - fTarget; 
					} else
							{
								clockwise = 360 - fCurr + fTarget;
								counterclockwise = fCurr - fTarget;
							}
				if (clockwise <= counterclockwise) 
						pComponent->fDir_Offset += TEMP_UROTATESPEED;
							else
						pComponent->fDir_Offset -= TEMP_UROTATESPEED;		
	
					
				} else
					{
					pComponent->fDir_Offset = pComponent->fDir_Offset_Default;
					}
				
							//normalize the component so that it's absolute point value is inside 0..360
							if (fDir_Current + pComponent->fDir_Offset >= 360)
								pComponent->fDir_Offset -= 360;
							if (fDir_Current + pComponent->fDir_Offset < 0)
								pComponent->fDir_Offset += 360;
			}

		}
		if (bModes_L1[lc1] == UMODE_L1_AIM_MULTI)
		{

//DEBUG
////pCA->pUnitList->Write_Log(" L1: AIM_MULTI");
////pCA->pUnitList->WriteLog_Enter();


			
			//aim all weapons simultaneously
			for (int lc12 = 0; lc12 < MultiFireInfo.iNumSelectedComponents; lc12++)
			{
				pComponent = MultiFireInfo.ppSelectedComponents[lc12];
				pCmpInfo = pCmpTable[pComponent->iType];

				if (pCmpInfo->pWeaponInfo->iPreAttackProcedure == PREATTACK_AIM)
				if (fabs(fDir_Current + pComponent->fDir_Offset - MultiFireInfo.arrAttackDirections[lc12]) > FP_TOLERANCE)
				{
//DEBUG
/*					
//pCA->pUnitList->Write_Log("Weapon ");
itoa(lc12, NText, 10);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->Write_Log(" Current Weapon Point Direction: ");
gcvt(fDir_Current + pComponent->fDir_Offset, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->Write_Log(" Target Direction: ");
gcvt(MultiFireInfo.arrAttackDirections[lc12], 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/
					//modified to turn the direction of the primary weapon(and the unit too, if direction-locked) (03.26.2003)
					//get the current absolute direction of the unit
					fDir_Absolute = fDir_Current + pComponent->fDir_Offset;
	
						if (fDir_Absolute >= 360)
							fDir_Absolute -= 360;
						if (fDir_Absolute < 0)                 
							fDir_Absolute += 360;

						double fDir_Absolute_old = fDir_Absolute;

					if (MultiFireInfo.arrAttackDirections[lc12] != fDir_Absolute) //just another little safeguard against unnecessary turns
					{
						if (MultiFireInfo.arrAttackDirections[lc12] > fDir_Absolute)
							{
		
								clockwise = MultiFireInfo.arrAttackDirections[lc12] - fDir_Absolute;    
								counterclockwise = 360 - clockwise;
							}
							else
								{
									counterclockwise = fDir_Absolute - MultiFireInfo.arrAttackDirections[lc12];
									clockwise = 360 - counterclockwise;
								}
	
						if (clockwise < TEMP_UROTATESPEED)
							fDir_Absolute += clockwise;
								else
								if (counterclockwise < TEMP_UROTATESPEED)
									fDir_Absolute -= counterclockwise;
									else
									if (clockwise <= counterclockwise)    
										fDir_Absolute += TEMP_UROTATESPEED;
											else
										fDir_Absolute -= TEMP_UROTATESPEED; 

							//identifies the amount the absolute position of the weapon needs to move
							double fDir_Diff = fDir_Absolute - fDir_Absolute_old;
							
						//now, if the weapon's direction characteristics are LOCKED, move the unit
						if (pCmpInfo->DrawSchemes[ZLEVEL_MIDBASE].bDirCharacteristics == CMPDIR_LOCK)
						{
							//in multi mode we CANNOT move the unit if there are multiple locked weapons involved...
							//to resolve this we are going to have to modify Set_MultiFirePattern to not allow two locked
							//weapons in multi fire modes that face in different directions(can they even face different directions currently?)
							//for now, just allow it (we currently don't have any locked weapons at all) (01.08.2004)
							//(12.28.2004) ONLY move the unit if this is primary weapon

								fDir_Current += fDir_Diff;
								if (fDir_Current >= 360)
									fDir_Current -= 360;
								if (fDir_Current < 0)                 
									fDir_Current += 360;
						}

						//if FREE, move the weapon
						if (pCmpInfo->DrawSchemes[ZLEVEL_MIDBASE].bDirCharacteristics == CMPDIR_FREE)
						{
							pComponent->fDir_Offset += fDir_Diff;

							//normalize the component so that it's absolute point value is inside 0..360
							if (fDir_Current + pComponent->fDir_Offset >= 360)
								pComponent->fDir_Offset -= 360;
							if (fDir_Current + pComponent->fDir_Offset < 0)
								pComponent->fDir_Offset += 360;

							//keep the offset within bounds  //(what is supposed to be here???) (01.08.2004)
						}
					}
				}
			}
		}
		if (bModes_L1[lc1] == UMODE_L1_AIM)
		{
		 //identical to turning except the goal direction is the attack target(the redundant code could be consolidated as a function, but,...ah, so lazy(06.21.2002)
//DEBUG
//pCA->pUnitList->Write_Log(" L1: AIM");
//pCA->pUnitList->WriteLog_Enter();//END DB

//DEBUG		
/*
//pCA->pUnitList->Write_Log(" Current Weapon Point Direction: ");
gcvt(fDir_Current + pCurrentWeapon->fDir_Offset, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->Write_Log(" Target Direction: ");
gcvt(pAttackInfo->fAttackDirection, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();		
*/
			//...		
			//modified to turn the direction of the primary weapon(and the unit too, if direction-locked) (03.26.2003)
			//get the current absolute direction of the unit
			fDir_Absolute = fDir_Current + pCurrentWeapon->fDir_Offset;

			if (pAttackInfo->fAttackDirection != fDir_Absolute) //just another little safeguard against unnecessary turns
			{
				if (pAttackInfo->fAttackDirection > fDir_Absolute)
					{
	
						clockwise = pAttackInfo->fAttackDirection - fDir_Absolute;    
						counterclockwise = fDir_Absolute + 360 - pAttackInfo->fAttackDirection;    
					}
					else
						{
							clockwise = 360 - fDir_Absolute + pAttackInfo->fAttackDirection;
							counterclockwise = fDir_Absolute - pAttackInfo->fAttackDirection;
						}

				if (clockwise <= counterclockwise)    
					fDir_Absolute += TEMP_UROTATESPEED;
					else
						fDir_Absolute -= TEMP_UROTATESPEED; 

				//(if either clockwise or counterclockwise was within TEMP_UROTATESPEED of the target, simply set it
				if (fabs(clockwise) < TEMP_UROTATESPEED || fabs(counterclockwise) < TEMP_UROTATESPEED)
				{
					pCurrentWeapon->fDir_Offset = pAttackInfo->fAttackDirection - fDir_Current;
					fDir_Absolute = fDir_Current + pCurrentWeapon->fDir_Offset;	
				}

//DEBUG		
/*
//pCA->pUnitList->Write_Log(" Post Weapon Point Direction: ");
gcvt(fDir_Current + pCurrentWeapon->fDir_Offset, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->Write_Log(" Target Direction: ");
gcvt(pAttackInfo->fAttackDirection, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();	//END DB
*/
				if (fDir_Absolute >= 360)
					fDir_Absolute =- 360;
				if (fDir_Absolute < 0)                 
					fDir_Absolute += 360;

				//identifies the amount the absolute position of the weapon needs to move
				double fDir_Diff = fDir_Absolute - fDir_Current - pCurrentWeapon->fDir_Offset;

//DEBUG
////pCA->pUnitList->Write_Log(" fDirDiff: ");
//gcvt(fDir_Diff, 7, NText);
////pCA->pUnitList->Write_Log(NText);//END DB
				//now, if the weapon's direction characteristics are LOCKED, move the unit
				if (pCmpTable[pCurrentWeapon->iType]->DrawSchemes[ZLEVEL_MIDBASE].bDirCharacteristics == CMPDIR_LOCK)
				{
					
					fDir_Current += fDir_Diff;
				}
				//if FREE, move the weapon
				if (pCmpTable[pCurrentWeapon->iType]->DrawSchemes[ZLEVEL_MIDBASE].bDirCharacteristics == CMPDIR_FREE)
				{
					pCurrentWeapon->fDir_Offset += fDir_Diff;
					//keep the offset within bounds
				}
			}

		}
		if (bModes_L1[lc1] == UMODE_L1_FIRE_MULTI)
		{

//DEBUG
//pCA->pUnitList->Write_Log(" L1: FIRE_MULTI");
//pCA->pUnitList->WriteLog_Enter();//END DB

			for (int lc13 = 0; lc13 < MultiFireInfo.iNumSelectedComponents; lc13++)
			{
				pComponent = MultiFireInfo.ppSelectedComponents[lc13];
				pCmpInfo = pCmpTable[pComponent->iType];


				if (pCA->pTimer->iCurrTick >= pComponent->llNextUse)
				{
					if (pCmpInfo->pWeaponInfo->iAttackProcedure == ATTACK_PROJECTILE)
					{
						
						COORDSET Pos_A = Calc_Position_CmpOffset(pComponent, pFrameType);
						if (pAttackInfo != NULL)
						{
							int iProjectileType = pCmpInfo->pWeaponInfo->iProjectileType;
							//if override info is active:
							if (OverloadInfo.bActive == TRUE)
							{
								//disable it now that it's used up.
								OverloadInfo.bActive = FALSE;
								//now attempt to set the projectile
								if (OverloadInfo.iOverLoadType >= 0)
								if ( pCA->pMaterialInfoTable->Types[OverloadInfo.iOverLoadType].iOverloadProjectileType >= 0)
									iProjectileType = pCA->pMaterialInfoTable->Types[OverloadInfo.iOverLoadType].iOverloadProjectileType;
							}
							pCA->pProjList->Create_Projectile(iProjectileType, iControl, Pos_A.x, Pos_A.y, pAttackInfo->Pos_ABSTarget.x, pAttackInfo->Pos_ABSTarget.y, 0);
						}
						else
						{
							pCA->pChatBuffer->Add_CBNode("ERROR: NULL ATTACK INFO", 50);
						}		
						//slight randomization of firing times here to deliberately de-synch multiply weapons firing simultaneously
						pComponent->llNextUse = pCA->pTimer->iCurrTick + pCmpInfo->pWeaponInfo->iFireRate + ((pCA->pRandGen->rand2() % 2) + 1);
						//HACK/TEST: make a firing sound here
						//pCA->pDA_Audio->Play_Primary(SOUNDSEG_GUN1);

					}
				}
			}
//DEBUG
//pCA->pUnitList->Write_Log(" L1: FIRE MULTI COMPLETE");
//pCA->pUnitList->WriteLog_Enter();//END DB

		}
		if (bModes_L1[lc1] == UMODE_L1_FIRE_PROJECTILE)
		{

//DEBUG
//pCA->pUnitList->Write_Log(" L1: FIRE PROJECTILE");
//pCA->pUnitList->WriteLog_Enter();//END DB
			//have the unit actually create the projectile? here? Or have it post an event to an "event buffer"?
			//(there appearing to be no compelling reason to stick it in the buffer at the moment, just create the projectile(06.21.2002)

//DEBUG
//pCA->pUnitList->Write_Log(" L1: waiting for weapon to ready");
//pCA->pUnitList->WriteLog_Enter();//END DB
			if (pCA->pTimer->iCurrTick >= pCurrentWeapon->llNextUse)
			{
//DEBUG

char NText[50];
//pCA->pUnitList->Write_Log(" L1: firing weapon. creating projectile.");
//pCA->pUnitList->WriteLog_Enter();
//pCA->pUnitList->Write_Log("   weapon type: ");
//pCA->pUnitList->Write_Log(pCA->pCmpInfoTable->pCmpTable[pCurrentWeapon->iType]->Name);
//pCA->pUnitList->WriteLog_Enter();
//pCA->pUnitList->Write_Log("   projectile type: ");
itoa(pCA->pCmpInfoTable->pCmpTable[pCurrentWeapon->iType]->pWeaponInfo->iProjectileType, NText, 10);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
//pCA->pUnitList->Write_Log("   target position(x,y): ");
itoa(pAttackInfo->Pos_ABSTarget.x, NText, 10);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->Write_Log(" ");
itoa(pAttackInfo->Pos_ABSTarget.y, NText, 10);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
//pCA->pUnitList->Write_Log("   unit iControl: ");
itoa(iControl, NText, 10);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();

				pCA->pProjList->Create_Projectile(pCA->pCmpInfoTable->pCmpTable[pCurrentWeapon->iType]->pWeaponInfo->iProjectileType, iControl, Pos_ABS.x, Pos_ABS.y, pAttackInfo->Pos_ABSTarget.x, pAttackInfo->Pos_ABSTarget.y, 0);
//DEBUG
//pCA->pUnitList->Write_Log(" L1: setting next fire time");
//pCA->pUnitList->WriteLog_Enter();//END DB
				pCurrentWeapon->llNextUse = pCA->pTimer->iCurrTick + pCA->pCmpInfoTable->pCmpTable[pCurrentWeapon->iType]->pWeaponInfo->iFireRate;
//DEBUG
//pCA->pUnitList->Write_Log(" L1: playing test fire sound");
//pCA->pUnitList->WriteLog_Enter();//END DB
				//HACK/TEST: make a firing sound here
				//pCA->pDA_Audio->Play_Primary(SOUNDSEG_GUN1);
			}
		}

	}
//DEBUG
//pCA->pUnitList->Write_Log("L Completion");
//pCA->pUnitList->WriteLog_Enter(); //END DB

//DEBUG
/*
pCA->pTimer->EndCount(TIMER_COUNTER3);
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER3], NText, 10);
pCA->pTimer->Write_Log("   [Run cycle]L1: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();

//DEBUG
pCA->pTimer->StartCount(TIMER_COUNTER3);
*/
	

Update_Position();

	//temporary direction "maintenance" code(03.26.2003)
					//keep directions in bounds
					if (fDir_Current >= 360)
						fDir_Current =- 360;
					if (fDir_Current < 0)                 
						fDir_Current += 360;	
					//gone above....wrap around
					if (pCurrentWeapon != NULL)
					{
						if (fDir_Current + pCurrentWeapon->fDir_Offset >= 360)
							pCurrentWeapon->fDir_Offset -= 360;
						if (fDir_Current + pCurrentWeapon->fDir_Offset < 0)
							pCurrentWeapon->fDir_Offset += 360;
					}

//TEMP lock unit positions in position
		if (Pos_ABS.x < 0)
			Pos_ABS.x = 0;
		if (Pos_ABS.y < 0)
			Pos_ABS.y = 0;
		if (Pos_ABS.x > pCA->pGameWorld->iWidth * TILE_SIZE)
			Pos_ABS.x = pCA->pGameWorld->iWidth * TILE_SIZE;
		if (Pos_ABS.y > pCA->pGameWorld->iHeight * TILE_SIZE)
			Pos_ABS.y = pCA->pGameWorld->iHeight * TILE_SIZE;

	//check whether b1mode changed
	if (ibModeL1Old != bModes_L1[0])
	{
		//update time
		iTimeLastL1Change = pCA->pTimer->iCurrTick;
	}


	//decide the drawframe (don't know what it does, disable for now) (01.07.2004)
	//iDrawFrame = Calc_DrawFrame(fDir_Current, p?);
	//reset the pUnitPath pointer to whatever it should be now
	pUnitPath = (VOID*)pUP;

//DEBUG
/*
pCA->pTimer->EndCount(TIMER_COUNTER3);
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER3], NText, 10);
pCA->pTimer->Write_Log("   [Run cycle]Post-L1: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();
*/


	return iRetVal;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitA::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cUnitA.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitA::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_UNIT_A >= 1)
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
void cUnitA::WriteLog_Enter()
{
if (DEBUG + DEBUG_UNIT_A >= 1)
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
int cUnitA::Calc_DrawFrame(double Angle, int iNumDirs)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	char NText[10];

	//modified 01.07.2004

	//figures out what the drawframe is
	//iDrawFrame = iDir_Current;
//DEBUG
/*
//pCA->pUnitList->Write_Log("[Calc_DrawFrame] Incoming Angle: ");
gcvt(Angle, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
//pCA->pUnitList->Write_Log("[Calc_DrawFrame] Incoming Directions: ");
itoa(iNumDirs, NText, 10);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
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
//pCA->pUnitList->Write_Log("[Calc_DrawFrame] Output Frame: ");
itoa(iret, NText, 10);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/


	return iret;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitA::Update_Heading_Bearing()   //updates the correct heading and bearing depending on what the IMTarget is
{

	//heading numbers are the difference between the final and current square positions
	Heading.x  = Pos_sqimtarget.x - Pos_sqcurr.x;
	Heading.y = Pos_sqimtarget.y - Pos_sqcurr.y;

	//nothing fancy here, just a lot of if statements to determine the target direction
	//safeguard here if the value is nonsense don't move it

	//we can calculate this directly using atan2, which would be more robust, but I think would be slower (01.07.2004)
	
	//UP
	if (Heading.x == 0 && Heading.y == -1)
		fDir_Target = 0;

	//UP RIGHT
	if (Heading.x == 1 && Heading.y == -1)
		fDir_Target = 45;

	//RIGHT
	if (Heading.x == 1 && Heading.y == 0)
		fDir_Target = 90;

	//DOWN RIGHT
	if (Heading.x == 1 && Heading.y == 1)
		fDir_Target = 135;

	//DOWN
	if (Heading.x == 0 && Heading.y == 1)
		fDir_Target = 180;

	//DOWN LEFT
	if (Heading.x == -1 && Heading.y == 1)
		fDir_Target = 225;

	//LEFT
	if (Heading.x == -1 && Heading.y == 0)
		fDir_Target = 270;

	//UP LEFT
	if (Heading.x == -1 && Heading.y == -1)
		fDir_Target = 315;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitA::Update_Heading_Bearing_B(double ABSTargetX, double ABSTargetY) //update heading_bearing alternate method better suited for something real-time (06.13.2002)
{
	//modified 01.07.2004 ... as units move to completely angle-based directioning
	
	double Dir_Angle; //angle of the direction (?)
	cClassAccess* pCA = (cClassAccess*)pCAccess;

//DEBUG
//char NText[20];

	double xdiff = ABSTargetX - Pos_ABS.x;
	double ydiff = ABSTargetY - Pos_ABS.y;

//DEBUG
/*
//pCA->pUnitList->Write_Log("[Update Heading_Bearing_B] XCurr: ");
gcvt(Pos_ABS.x, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->Write_Log(" YCurr: ");
gcvt(Pos_ABS.y, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/

	if (xdiff != 0)
	{
		Dir_Angle = atan2(-ydiff, xdiff); //negative y because up-down direction is different between the map and the drawframes
		//convert to degrees
		Dir_Angle = Dir_Angle * RAD_DEG_CONV;
		
		//subtract 90
		Dir_Angle = Dir_Angle - 90;

		//flip up a negative value by 360 degrees
		if (Dir_Angle < 0)
			Dir_Angle = Dir_Angle + 360;

		//....reverse the degree values (so they go clockwise instead of counter-clockwise)		0
		//																					270	  90
		//																					   180
		if (Dir_Angle != 0)
			Dir_Angle = 360 - Dir_Angle;
		//calculate the target direction 
		fDir_Target = Dir_Angle;
//DEBUG
/*
//pCA->pUnitList->Write_Log("[Update Heading_Bearing_B] Calculated Target Angle: ");
gcvt(fDir_Target, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/
	}
		else //if vertical, it's either 0 or 180 degrees
		{
			if (ydiff > 0) //going DOWN, 180 degrees
			{
				Dir_Angle = 180;
				fDir_Target = 180;
			}
			if (ydiff < 0) //going UP, 0 degrees
			{
				Dir_Angle = 0;
				fDir_Target = 0;
			}
		}

	//get the unit's frame information
	float fMult = 1;

	
	if (pFrame != NULL)
	{
		cCmpInfo* pFrameInf = pCA->pCmpInfoTable->pCmpTable[pFrame->iType];
		//this might become a problem in the future, if the TERRAIN defines and the ISURF defines ever differ(03.21.2003)
		//note, changed to accomodate AIR (01.16.2004)
		if (Pos_ABS.z > 0)
			fMult = pFrameInf->pFrameInfo->MoveCharacteristics[MAPTILE_AIR];
				else
			fMult = pFrameInf->pFrameInfo->MoveCharacteristics[pCA->pGameWorld->pMap[Pos_sqcurr.index].iType];

			if (fMult == -1)
				fMult = 0;
	}


	//heading is determined off the angle and the speed capabilities of the unit
	//due to the way the coords were converted, heading.x is sine, heading.y is negative cosine
	Heading.x = sin(Dir_Angle / RAD_DEG_CONV) * fCurrSpeed * fMult;
	Heading.y = -cos(Dir_Angle / RAD_DEG_CONV) * fCurrSpeed * fMult;



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitA::Update_Heading_Bearing_C(double ABSTargetX, double ABSTargetY) //update heading_bearing alternate method better suited for something real-time (06.13.2002)
{
	//modified 01.07.2004 ... as units move to completely angle-based directioning
	
	double Dir_Angle; //angle of the direction (?)
	cClassAccess* pCA = (cClassAccess*)pCAccess;

//DEBUG
//char NText[20];
//pCA->pTimer->StartCount(TIMER_COUNTER1);

	double xdiff = ABSTargetX - Pos_ABS.x;
	double ydiff = ABSTargetY - Pos_ABS.y;

//DEBUG
/*
//pCA->pUnitList->Write_Log("[Update Heading_Bearing_B] XCurr: ");
gcvt(Pos_ABS.x, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->Write_Log(" YCurr: ");
gcvt(Pos_ABS.y, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/

	if (xdiff != 0)
	{
//pCA->pTimer->StartCount(TIMER_COUNTER1);
		Dir_Angle = atan2(-ydiff, xdiff); //negative y because up-down direction is different between the map and the drawframes
//pCA->pTimer->EndCount(TIMER_COUNTER1);

//DEBUG (testing the speed of atan2 function
/*
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER1], NText, 10);
pCA->pTimer->Write_Log("ATAN2: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();	
*/	
		
		//convert to degrees
		Dir_Angle = Dir_Angle * RAD_DEG_CONV;
		
		//subtract 90
		Dir_Angle = Dir_Angle - 90;

		//flip up a negative value by 360 degrees
		if (Dir_Angle < 0)
			Dir_Angle = Dir_Angle + 360;

		//....reverse the degree values (so they go clockwise instead of counter-clockwise)		0
		//																					270	  90
		//																					   180
		if (Dir_Angle != 0)
			Dir_Angle = 360 - Dir_Angle;
		//calculate the target direction 
		fDir_Target = Dir_Angle;
//DEBUG
/*
//pCA->pUnitList->Write_Log("[Update Heading_Bearing_B] Calculated Target Angle: ");
gcvt(fDir_Target, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/

	}
		else //if vertical, it's either 0 or 180 degrees
		{
			if (ydiff > 0) //going DOWN, 180 degrees
			{
				Dir_Angle = 180;
				fDir_Target = 180;
			}
			if (ydiff < 0) //going UP, 0 degrees
			{
				Dir_Angle = 0;
				fDir_Target = 0;
			}
		}

	//get the unit's frame information
	float fMult = 1;

	
	if (pFrame != NULL)
	{
		cCmpInfo* pFrameInf = pCA->pCmpInfoTable->pCmpTable[pFrame->iType];
		//this might become a problem in the future, if the TERRAIN defines and the ISURF defines ever differ(03.21.2003)
		//note, changed to accomodate AIR (01.16.2004)
		if (Pos_ABS.z > 0)
			fMult = pFrameInf->pFrameInfo->MoveCharacteristics[MAPTILE_AIR];
				else
			fMult = pFrameInf->pFrameInfo->MoveCharacteristics[pCA->pGameWorld->pMap[Pos_sqcurr.index].iType];

			if (fMult == -1)
				fMult = 0;
	}


	//heading is determined off the angle and the speed capabilities of the unit
	//due to the way the coords were converted, heading.x is sine, heading.y is negative cosine
double fVal;

//note that a ground unit determines bearing based on its target direction, an air unit based on its crrent direction
if (Pos_ABS.z > 0)
	fVal = fDir_Target / RAD_DEG_CONV;
		else
	fVal = fDir_Target / RAD_DEG_CONV;


	Heading.x = sin(fVal) * fCurrSpeed * fMult;

	Heading.y = -cos(fVal) * fCurrSpeed * fMult;

/*
pCA->pTimer->EndCount(TIMER_COUNTER1);
//DEBUG (testing the speed of function
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER1], NText, 10);
pCA->pTimer->Write_Log("   Heading Bearing C: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();
*/

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cUnitA::Calc_Distance(float xstart, float ystart, float xend, float yend)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

//DEBUG
/*
char NText[20];
pCA->pTimer->StartCount(TIMER_COUNTER1);
*/

	float xdiff = abs(xend - xstart);
	float ydiff = abs(yend - ystart);
	double dist = sqrt( (xdiff * xdiff) + (ydiff * ydiff));


/*
pCA->pTimer->EndCount(TIMER_COUNTER1);
//DEBUG (testing the speed of function
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER1], NText, 10);
pCA->pTimer->Write_Log("   Calc_Distance C: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();
*/

	return dist;
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void* cUnitA::Repath(void* pOldPath, int searchdepth)
{
//(06.05.2002) Not sure whether to use this, but a procedure for any repathing process
//.returns pointer to the new pathlist
	cPATHENTRY* pUP;	
	cClassAccess* pCA = (cClassAccess*)pCAccess;

//DEBUG
//char NText[20];
//pCA->pTimer->StartCount(TIMER_COUNTER1);

									Update_Position();  //will this work?!?  02.14.2004

									//Destroy the existing path
									pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pOldPath);
//DEBUG
/*
itoa(Pos_sqcurr.index, NText, 10);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->Write_Log(" target: ");
itoa(Pos_sqtarget.index, NText, 10);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/
									//regenerate the unit path
										//pUP = pCA->pPathFinder->Find_Path(iID, Pos_sqcurr.index, Pos_sqtarget.index, searchdepth, &bPathDeferredFlag);
									//see what happens if we path to the next node like we used to	
									pUP = pCA->pPathFinder->Find_Path(iID, Pos_sqimtarget.index, Pos_sqtarget.index, 10000, &bPathDeferredFlag, TRUE, ddAllowableBackOffDistance);
									iTick_LastRepath = pCA->pTimer->iCurrTick;			  
									  //idle for a cycle because I can't figure out what exactly I need to do here! (02.02.2002)
									bMode_L2 = UMODE_L2_IDLE;

					Pos_sqimtarget.index = pUP->Position.index;
					Pos_sqimtarget.Calc_xy(pCA->pGameWorld->iWidth);
					Update_Heading_Bearing_C(Pos_sqimtarget.x * TILE_SIZE, Pos_sqimtarget.y * TILE_SIZE);

/*
pCA->pTimer->EndCount(TIMER_COUNTER1);
//DEBUG (testing the speed of function
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER1], NText, 10);
pCA->pTimer->Write_Log("   Repath: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();
*/

	//reset pUnitPath
	pUnitPath = (void*)pUP;
	return pUnitPath;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cUnitA::Calc_Direction(float fXHeading, float fYHeading)  //06.22.2002 (copied from cProjectileA class function)
{
double	Dir_Angle; //angle measure	
double	Ret_Dir;
	

cClassAccess* pCA = (cClassAccess*)pCAccess;


//DEBUG
//char NText[20];
//pCA->pTimer->StartCount(TIMER_COUNTER1);


//DEBUG
/*
char NText[20];
//pCA->pUnitList->Write_Log("[Calc Direction] Input Heading: X:");
gcvt(fXHeading, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->Write_Log(" Y:");
gcvt(fYHeading, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/

	//use atan2 to figure out the angle of what this is
	if (fXHeading != 0)
	{
//pCA->pTimer->StartCount(TIMER_COUNTER1);
		Dir_Angle = atan2(-fYHeading, fXHeading);
//pCA->pTimer->EndCount(TIMER_COUNTER1);

//DEBUG (testing the speed of atan2 function
/*
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER1], NText, 10);
pCA->pTimer->Write_Log("ATAN2: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();
*/

		//convert to degrees
		Dir_Angle = Dir_Angle * RAD_DEG_CONV;

		//subtract 90, conjugate negative angles
		Dir_Angle = Dir_Angle - 90;
		if (Dir_Angle < 0)
			Dir_Angle = Dir_Angle + 360;
		//reverse angle direction
		if (Dir_Angle != 0)  //actually this IF statement isn't necessary because the angle can never be 0(fXHeading would have to be 0)
			Dir_Angle = 360 - Dir_Angle;

		//calculate the return angle
		Ret_Dir = Dir_Angle;//Dir_Angle / double(360.00 / UNIT_NUMDIRS);

	}
		else //either 0 or 180 degrees (comparisons are inverted from expected because on the map, larger numbers are DOWN...)
		{
			if (fYHeading <= 0)
				Ret_Dir = 0;					//0 degrees
			if (fYHeading > 0)	
				Ret_Dir = 180;//UNIT_NUMDIRS / 2;		//180 degrees
		}
//DEBUG
/*
//pCA->pUnitList->Write_Log("[Calc Direction] Returned Angle: ");
gcvt(Ret_Dir, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/


/*
pCA->pTimer->EndCount(TIMER_COUNTER1);
//DEBUG (testing the speed of function
itoa(pCA->pTimer->lTimeElapsed[TIMER_COUNTER1], NText, 10);
pCA->pTimer->Write_Log("   Calc_Directions: ");
pCA->pTimer->Write_Log(NText);
pCA->pTimer->WriteLog_Enter();
*/

	return Ret_Dir;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitA::Update_Targets(cUnitA* pTarget)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//find the nearest unoccupied spot to the enemy target so that the 
	//the pather doesn't slow up on finding a path

//DEBUG
////pCA->pUnitList->Write_Log("   Update Targets: Finding nearest unoccupied square");
////pCA->pUnitList->WriteLog_Enter();

	//see if that speeds up anything...
	//int iIndex_New = pCA->pPathFinder->Adjust_Target(pTarget->Pos_sqcurr.index, iID);

	Pos_sqtarget.index = pTarget->Pos_sqcurr.index;//iIndex_New;
	Pos_sqtarget.Calc_xy(pCA->pGameWorld->iWidth);

//DEBUG
////pCA->pUnitList->Write_Log("   Update Targets: Setting information");
////pCA->pUnitList->WriteLog_Enter();

	pAttackInfo->Pos_ABSTarget.x = pTarget->Pos_ABS.x;
	pAttackInfo->Pos_ABSTarget.y = pTarget->Pos_ABS.y;

//DEBUG
////pCA->pUnitList->Write_Log("   Update Targets: Calculation Attack direction");
////pCA->pUnitList->WriteLog_Enter();
	pAttackInfo->fAttackDirection = Calc_Direction(pAttackInfo->Pos_ABSTarget.x - Pos_ABS.x, pAttackInfo->Pos_ABSTarget.y - Pos_ABS.y);
//DEBUG
////pCA->pUnitList->Write_Log("   Update Targets: completed.");
////pCA->pUnitList->WriteLog_Enter();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitA::Update_LocalWeaponTargets(double PosABStargetx, double PosABStargety, UNITMULTIFIREINFO* pMFI)
{
	//all this needs to do is set local attack directions
COORDSET Pos_A;
int lc1; //local counter
	cClassAccess* pCA = (cClassAccess*)pCAccess;

//DEBUG
////pCA->pUnitList->Write_Log("   Update Local Weapon Targets: started.");
////pCA->pUnitList->WriteLog_Enter();

cComponentA* pComponent;
cCmpInfo* pCmpInfo;
cCmpInfo* pFrameInfo = pCA->pCmpInfoTable->pCmpTable[pFrame->iType];

	for (lc1 = 0; lc1 < pMFI->iNumSelectedComponents; lc1++)
	{
//DEBUG
////pCA->pUnitList->Write_Log("   Update Local Weapon Targets: beginning calculations for weapon.");
////pCA->pUnitList->WriteLog_Enter();

		pComponent = pMFI->ppSelectedComponents[lc1];
		pCmpInfo = pCA->pCmpInfoTable->pCmpTable[pComponent->iType];

		//calculation position of component    (I could specify the exact ZLevel this component is attached to
		Pos_A = Calc_Position_CmpOffset(pComponent, pFrameInfo);

		//now calculate the direction!
		pMFI->arrAttackDirections[lc1] = Calc_Direction(pAttackInfo->Pos_ABSTarget.x - Pos_A.x, pAttackInfo->Pos_ABSTarget.y - Pos_A.y);
//DEBUG
////pCA->pUnitList->Write_Log("   Update Local Weapon Targets: calculations complete.");
////pCA->pUnitList->WriteLog_Enter();
	}
//DEBUG
////pCA->pUnitList->Write_Log("   Update Local Weapon Targets: function complete.");
////pCA->pUnitList->WriteLog_Enter();
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
COORDSET cUnitA::Calc_Position_CmpOffset(cComponentA* pComponent, cCmpInfo* pFrameInfo)
{
	
COORDSET Pos_A;

		//calculation position of component    
		int iFrame = pComponent->iStartingIndex + Calc_DrawFrame(fDir_Current, pFrameInfo->DrawSchemes[pComponent->iZLevel_attached].iRotateFrames);
		if (iFrame >= pFrameInfo->DrawSchemes[pComponent->iZLevel_attached].iRotateFrames)
			iFrame -= pFrameInfo->DrawSchemes[pComponent->iZLevel_attached].iRotateFrames;

		Pos_A.x = Pos_ABS.x + pComponent->pOffsetTable->XOffsets[iFrame];
		Pos_A.y = Pos_ABS.y + pComponent->pOffsetTable->YOffsets[iFrame];

return Pos_A;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cUnitA::Guard_Scan(BYTE bScanMode, WORD wScanRange)
{
int iEnemyID = -1;
cClassAccess* pCA = (cClassAccess*)pCAccess;
cUnitA* pEnemy = NULL;
cUnitA* pDetectedUnit = NULL;
int lIndex_Scan;
int iSC = wScanRange;
double ddClosestRange = -1; //comfortable lead
double range = 0;
int lcx, lcy;



	//scan in each direction the scan range
	for (lcx = Pos_sqcurr.x - wScanRange; lcx <= Pos_sqcurr.x + wScanRange; lcx++)
	for (lcy = Pos_sqcurr.y - wScanRange; lcy <= Pos_sqcurr.y + wScanRange; lcy++)
	{
		lIndex_Scan = lcy * pCA->pGameWorld->iWidth + lcx;
		//check for onscreen(and check against wrap-around)
		if (lIndex_Scan >= 0 && lIndex_Scan < pCA->pGameWorld->iWidth * pCA->pGameWorld->iHeight - pCA->pGameWorld->iWidth
			&& lcx >= 0 && lcx < pCA->pGameWorld->iWidth)
		{
			pDetectedUnit = (cUnitA*)pCA->pGameWorld->pMap[ lIndex_Scan ].pOccupant_Unit;
			if (pDetectedUnit != NULL)
			if (pDetectedUnit->iControl != iControl)
			{
				range = Calc_Distance(pDetectedUnit->Pos_ABS.x, pDetectedUnit->Pos_ABS.y, Pos_ABS.x, Pos_ABS.y);
				if (range < ddClosestRange || ddClosestRange == -1)
				{
					pEnemy = pDetectedUnit;
					ddClosestRange = range;
				}
			}
		}
	}




	//don't do anything if no enemy found
	if (pEnemy == NULL)
	{
		if (pAttackInfo != NULL)
		{
			delete(pAttackInfo);
			pAttackInfo = NULL;
		}


		return 0;
	}

	//otherwise, create an attackinfo structure
	if (pAttackInfo != NULL)
	if (pAttackInfo->iEnemyID >= 0)
	if (pAttackInfo->iEnemyID != pEnemy->iID)
	{
		delete(pAttackInfo);
		pAttackInfo = NULL;
	}	
		else
		{

			
			return 1;   //return without creating a new attack info (the unit already has the correct one)
		}


//DEBUG
/*
char NText[50];
itoa(pEnemy->iID, NText, 10);
//pCA->pUnitList->Write_Log(" Guard Scan: Acquired Enemy ID: ");
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/
	//create a new pAttackInfo structure
	pAttackInfo = new UNITATTACKINFO;
	//fill with applicable information
	pAttackInfo->iEnemyID = pEnemy->iID;
	//pAttackInfo->pEnemy = pEnemy;
	pAttackInfo->iTargetType = CQATTACKTYPE_UNIT;
	pAttackInfo->Pos_ABSTarget.x = pEnemy->Pos_ABS.x;
	pAttackInfo->Pos_ABSTarget.y = pEnemy->Pos_ABS.y;
	//note...half the TILE_SIZE is added to correct for the fact that the unit's "technical" position is the corner of the tile, while it's apparent position is the middle of the tile
	pAttackInfo->fAttackDirection = Calc_Direction(pAttackInfo->Pos_ABSTarget.x - Pos_ABS.x, pAttackInfo->Pos_ABSTarget.y - Pos_ABS.y);

	//(01.08.2004) Calculate local attack directions
	Update_LocalWeaponTargets(pEnemy->Pos_ABS.x , pEnemy->Pos_ABS.y, &MultiFireInfo);

	//return
	return 1;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cUnitA::~cUnitA()
{
//delete ATTACKINFO and GUARDINFO
	if (pAttackInfo != NULL)
	{
//		if (pAttackInfo->pEnemy != NULL)
//			delete(pAttackInfo->pEnemy);
		delete(pAttackInfo);
	}

	if (pGuardInfo != NULL)
		delete(pGuardInfo);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitA::Set_MultiFirePattern(int iPattern)
{
//01.08.2004
	int iNumSelected = 0;
	float fMinRange = -1;

	cClassAccess* pCA = (cClassAccess*)pCAccess;

//DEBUG
char NText[50];

//size an initial array based on the total number of components(guarantees that all the selected components will fit in it
//and we can go through in one pass
cComponentA** ppPreArray = new cComponentA* [CmpAttached.iNumComponents];

ZeroMemory(ppPreArray, sizeof(cComponentA*)* CmpAttached.iNumComponents);

	cComponentA* pCmpTrav;

	//just select all weapons
	if (iPattern == MULTIFIRE_ALL)
	{
		pCmpTrav = (cComponentA*)CmpAttached.pLByClass_First[CMPCLASS_WEAPON]->pNext_classlist;

		while (pCmpTrav != CmpAttached.pLByClass_Last[CMPCLASS_WEAPON])
		{




			cCmpInfo* pInfo = pCA->pCmpInfoTable->pCmpTable[pCmpTrav->iType];

//DEBUG
/*
//pCA->pUnitList->Write_Log("Range Check. Component Name: ");
//pCA->pUnitList->Write_Log(pInfo->Name);
//pCA->pUnitList->Write_Log(" Component Range: ");
gcvt(pInfo->pWeaponInfo->fRange, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/

			//range check
			if (fMinRange == -1)
				fMinRange = pInfo->pWeaponInfo->fRange;
					else
				if (pInfo->pWeaponInfo->fRange < fMinRange)
					fMinRange = pInfo->pWeaponInfo->fRange;

			//no conditions
			ppPreArray[iNumSelected] = pCmpTrav;
			iNumSelected++;

			pCmpTrav = (cComponentA*)pCmpTrav->pNext_classlist;
		}
	}

	MultiFireInfo.fMinRange = fMinRange;

//DEBUG
/*
//pCA->pUnitList->Write_Log("Range Check. Min Range: ");
gcvt(MultiFireInfo.fMinRange, 7, NText);
//pCA->pUnitList->Write_Log(NText);
//pCA->pUnitList->WriteLog_Enter();
*/

//now just set the preArray to the multifireinfo
	MultiFireInfo.iNumSelectedComponents = iNumSelected;

	if (MultiFireInfo.ppSelectedComponents != NULL)
		delete(MultiFireInfo.ppSelectedComponents);
	MultiFireInfo.ppSelectedComponents = ppPreArray;

	//allocate the arrAttackDirections array
	if 	(MultiFireInfo.arrAttackDirections != NULL)
		delete(MultiFireInfo.arrAttackDirections);

	MultiFireInfo.arrAttackDirections = new double[iNumSelected];

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitA::Recalc_Attributes()
{

cClassAccess* pCA = (cClassAccess*)pCAccess;
//initialize the values
	fHPMax = 0;
	fHP = fHPMax;
	iSize = 0;
	iMass = 0;
	fMaxSpeed = 0;
	ddSupportCost = 0;
	iStorageVolume = 0;
	ddMainRange = 0;
	Storage.ddMaterialCapacity = 1;
	fArmor = 0;

pFrame = NULL;

//used to calculate speed of unit
int iPower = 0;
int iHighestDamage = 0;
//used to find the number of visible components that need to be drawn
int iNumVisibleComponents = 0;
int iNumComponents = 0;


cCmpInfo* pInfo; //to store component info(and save having to reference through the CmpTable each time)

	//run through the component list
	cComponentA* pCmpTrav = (cComponentA*)CmpAttached.pCmpList_First->pNext_list;


	bBroadsideFlag = TRUE;

	while (pCmpTrav != CmpAttached.pCmpList_Last)
	{
		pInfo = pCA->pCmpInfoTable->pCmpTable[pCmpTrav->iType];
		//add attribute size and mass
		iSize += pInfo->iSize;
		iMass += pInfo->iMass;
		ddSupportCost += pInfo->ddSupport;
		//increment visible components, if visible
		if (pInfo->DrawSchemes[ZLEVEL_MIDBASE].bDrawCharacteristics != CMPDRAW_NODRAW)
		if (pInfo->pFrameInfo == NULL)  //frames don't count as "visible" components as they are drawn regardless!
			iNumVisibleComponents++;
		//always increment the number of components
		iNumComponents++;



		//add class-specific components
		if (pInfo->pDefenseInfo != NULL)
		{
			fHPMax += pInfo->pDefenseInfo->iDefenseBonus * pCmpTrav->fEfficiency;
			fHP += pInfo->pDefenseInfo->iDefenseBonus * pCmpTrav->fEfficiency;
		}
		if (pInfo->pStoreInfo != NULL)
		{
			iStorageVolume += pInfo->pStoreInfo->iStorageBonus;
			Storage.ddMaterialCapacity  += pInfo->pStoreInfo->ddMaterialStorageBonus;
		}
		if (pInfo->pEngineInfo != NULL)
		{
			iPower += pInfo->pEngineInfo->iPower * pCmpTrav->fEfficiency;
		}
		if (pInfo->pFrameInfo != NULL)
		{
			pFrame = pCmpTrav;
			pFrameType = pInfo;

			//if not a seagoing unit, disable broadside
			if (pInfo->pFrameInfo->MoveCharacteristics[MAPTILE_OCEAN] < 0.5)
				bBroadsideFlag = FALSE;

			//08.10.2004
			fHPMax += pInfo->pFrameInfo->iIntegrity;
			fHP += pInfo->pFrameInfo->iIntegrity;

		}
		if (pInfo->pToolInfo != NULL)
		if (pInfo->pToolInfo->iUseProcedure == TOOL_BUILD)
		{
			pBuilder = pCmpTrav;
		}
		//NOTE: tools and weapons do not add anything to attributes
		//however, set unit primary weapon
		//default, set the primary weapon as the highest-damage weapon available
		if (pInfo->pWeaponInfo != NULL)
		{
			//if weapon is LOCKed, do not broadside
			if (pInfo->DrawSchemes[ZLEVEL_MIDBASE].bDirCharacteristics == CMPDIR_LOCK)
				bBroadsideFlag = FALSE;
		
			if (pCA->pPTypeTable->ProjTypes[pInfo->pWeaponInfo->iProjectileType].iDamage > iHighestDamage)
			{
				pPrimaryWeapon = pCmpTrav;
					ddMainRange = pInfo->pWeaponInfo->fRange;
				pCurrentWeapon = pCmpTrav;

				iHighestDamage = pCA->pPTypeTable->ProjTypes[pInfo->pWeaponInfo->iProjectileType].iDamage;
			}
		}

		pCmpTrav = (cComponentA*)pCmpTrav->pNext_list;
	}



//don't attempt to find and sort visible components if no frame is attached
if (pFrame != NULL)
{
	
	//now go through the list again and gather the pointers to all the components
	cComponentA** ArrayCmp = /**/new cComponentA*[iNumComponents];

		pCmpTrav = (cComponentA*)CmpAttached.pCmpList_First->pNext_list;
		int ic1 = 0;
	
		while (pCmpTrav != CmpAttached.pCmpList_Last)
		{	
			ArrayCmp[ic1] = pCmpTrav;
			ic1++;
			pCmpTrav = (cComponentA*)pCmpTrav->pNext_list;
		}
	

	//figure out which limit was hit first, the number of visible components or 
	//the number of visible hardpoints
		pInfo = pCA->pCmpInfoTable->pCmpTable[pFrame->iType];

		if (iNumVisibleComponents > pInfo->pFrameInfo->iNumVisibleHardPoints)
			iNumDrawVisibleComponents = pInfo->pFrameInfo->iNumVisibleHardPoints;
				else
			iNumDrawVisibleComponents = iNumVisibleComponents;


	//allocate array
			VisibleComponents = /**/new UNITCMPDRAWINFO[iNumDrawVisibleComponents];
			
	//now do a search for the largest components and place them into the draw list
			for (int c12 = 0; c12 < iNumDrawVisibleComponents; c12++)
			{
				int iLargestSize = 0;
				int iSelectComponent = -1;
				for (int c13 = 0; c13 < iNumComponents; c13++)
				{
					if (ArrayCmp[c13] != NULL)
					{
						pInfo = pCA->pCmpInfoTable->pCmpTable[ArrayCmp[c13]->iType];
					
					
						if (pInfo->iSize > iLargestSize && pInfo->DrawSchemes[ZLEVEL_MIDBASE].bDrawCharacteristics != CMPDRAW_NODRAW && pInfo->pFrameInfo == NULL) //once again, no frames
						{
							iLargestSize = pInfo->iSize;
							iSelectComponent = c13;
						}
					}
					//should finish this search with the largest unselected visible component
				}
				if (iSelectComponent != -1)
				{
					pInfo = pCA->pCmpInfoTable->pCmpTable[pFrame->iType];
					VisibleComponents[c12].pComponent = ArrayCmp[iSelectComponent];
					VisibleComponents[c12].pOffsets = pInfo->pFrameInfo->VisHardPoints[c12].pOffsets;
					VisibleComponents[c12].iStartingIndex = pInfo->pFrameInfo->VisHardPoints[c12].iStartingIndex;
					VisibleComponents[c12].iZLevel = pInfo->pFrameInfo->VisHardPoints[c12].iZLevel;
					VisibleComponents[c12].iZLevelBase = pInfo->pFrameInfo->VisHardPoints[c12].iZLevelBase;
					//VisibleComponents[c12].iImageSurfIndex = pCA->pCmpInfoTable->pCmpTable[VisibleComponents[c12].pComponent->iType]->DrawSchemes[ZLEVEL_MIDBASE].iImageSurfIndex;

					//tell the component where it has been attached to save on headaches later (01.08.2004)
					ArrayCmp[iSelectComponent]->pOffsetTable = pInfo->pFrameInfo->VisHardPoints[c12].pOffsets;
					ArrayCmp[iSelectComponent]->iStartingIndex = pInfo->pFrameInfo->VisHardPoints[c12].iStartingIndex;
					ArrayCmp[iSelectComponent]->iZLevel_attached = pInfo->pFrameInfo->VisHardPoints[c12].iZLevelBase;
					//this last one basically puts the unit facing outwards from it's hardpoint position(m
					if (TEMP_CMPDEFOFFSET)
						ArrayCmp[iSelectComponent]->fDir_Offset_Default = (double(VisibleComponents[c12].iStartingIndex) / UNIT_NUMDIRS) * 360.0;

					//now clear it out so it doesn't show up on the next pass
					ArrayCmp[iSelectComponent] = NULL;
				}
			}

	//at the end of this, the unit should now have a list of components to draw, along with offset values to draw them at
	if (ArrayCmp != NULL)
		delete(ArrayCmp);


}


//calculate the unit's speed
	fMaxSpeed = float(iPower) / iMass;
//augment based on overloads 
	if (Storage.ddMaterialStored > 0)
	{
		fMaxSpeed *= pCA->pMaterialInfoTable->Types[Storage.iMaterialType].ddSpeedMultiplier;
		//apply the multiplier first
		fArmor *= pCA->pMaterialInfoTable->Types[Storage.iMaterialType].ddArmorMultiplier;
		//then apply the bonus
		fArmor += pCA->pMaterialInfoTable->Types[Storage.iMaterialType].ddArmorBonus;
	}
	//fCurrSpeed = fMaxSpeed;
	//note that it is currently too cumbersome, and it feels "inappropriate" to rigorously model the physics
	//of unit motion (ex. specify "drag" resistances for the various types of terrain/air, determine acceleration
	//and maximum speed by the amount of kinetic energy(power) the engine can give to the craft....
	//(this would require solving physics equations concerning a completely fictional virtual environment, which
	//makes me chuckle just a little)
	//this might change in the future, but currently the Maximum speed is linearly proportional to engine power,
	//as is the acceleration and deceleration capability (01.16.2004)

	fAccel = 0;
	fDecel = 0;
if (pFrame != NULL)
{
	cCmpInfo* pInfFrame = pCA->pCmpInfoTable->pCmpTable[pFrame->iType];

	//0.5 and 1.5 here are "tuning parameters" to change to make things look better
	fAccel = fMaxSpeed * 0.5 * pInfFrame->pFrameInfo->fAccelBonus;
	fDecel = fMaxSpeed * 1.5 * pInfFrame->pFrameInfo->fDecelBonus;
}

//take the time here to recalculate the list of supported commands
	Update_SupportedCommands();

//01.08.2004  Set the unit's default multifire pattern as MULTIFIRE_ALL
	Set_MultiFirePattern(MULTIFIRE_ALL);
	


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitTT::Init(VOID* pCA)
{
	pCAccess = pCA;

	//initialize all lists
	for (int c6 = 0; c6 < UTYPE_NUMTYPES; c6++)
	{
		
		UnitTypes[c6].CmpAttached.Init(pCA, "./debuglogs/Nothing.txt" );
		ZeroMemory(UnitTypes[c6].Name, sizeof(char) * 80);
		ZeroMemory(UnitTypes[c6].ShortName, sizeof(char) * 80);
		for (int c7 = 0; c7 < TEMP_DP_MAXPLAYERS; c7++)
			UnitTypes[c6].iInstances[c7] = 0;
		UnitTypes[c6].iNumAcks = 0;
		UnitTypes[c6].iNameLength = 0;
	//	strcat(UnitTypes[c6].Name, "Unspecified");
	}
	iHighestTypeID = 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitA::Update_SupportedCommands()
{
	
	BYTE SupportList[NUM_CQ2COMMANDS];
	int iNumSupportedCommands = 0;
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int lc1 = 0; //local counter

	//initialize the support list
	for (lc1 = 0; lc1 < NUM_CQ2COMMANDS; lc1++)
		SupportList[lc1] = 0;

	//perhaps temporary decision of viable commands (05.30.2003)
	//now....if a unit has a frame, it supports the move command
	if (pFrame != NULL)
	{
		SupportList[CQ2TYPE_MOVE_B] = 1;
		iNumSupportedCommands++;

		//land units support load command
		cCmpInfo* pFrameInfo = pCA->pCmpInfoTable->pCmpTable[pFrame->iType];
		if (pFrameInfo->pFrameInfo->MoveCharacteristics[MAPTILE_GRASSLAND] > 0)
		{
			SupportList[CQ2TYPE_LOAD] = 1;
			iNumSupportedCommands++;
		}
		//if unit has anything stored, support unload all command
		if (iStorageVolume > 0)
		{
			SupportList[CQ2TYPE_UNLOAD_ALL] = 1;
			iNumSupportedCommands++;
		}
		//if unit has any material storage capacity, it supports tanker commands
		if (Storage.ddMaterialCapacity > 0)
		{
			SupportList[CQ2TYPE_LOAD_MATERIAL] = 1;
			iNumSupportedCommands++;
			SupportList[CQ2TYPE_UNLOAD_MATERIAL] = 1;
			iNumSupportedCommands++;
			SupportList[CQ2TYPE_TRANSFER_MATERIAL] = 1;
			iNumSupportedCommands++;
		}
	}
	//if the unit has a weapon, it supports the attack command
	if (pPrimaryWeapon != NULL)
	{
		SupportList[CQ2TYPE_ATTACK_B] = 1;
		iNumSupportedCommands++;
		//if the weapon fires a projectile
		//if the unit is carrying materials
		//if the materials have an overload projectile,
		//then support CQ2TYPE_ARM_MATERIAL
		cCmpInfo* pInfo = pCA->pCmpInfoTable->pCmpTable[pPrimaryWeapon->iType];
		if (pInfo->pWeaponInfo->iAttackProcedure == ATTACK_PROJECTILE)
		if (Storage.iMaterialType >= 0)
		if (Storage.ddMaterialStored > 0)
		if (pCA->pMaterialInfoTable->Types[Storage.iMaterialType].iOverloadProjectileType >= 0)
		{
			SupportList[CQ2TYPE_ARM_MATERIAL] = 1;
			iNumSupportedCommands++;
		}
		
	}
	//if the unit has a "builder", it supports the build command
	if (pBuilder != NULL)
	{
		SupportList[CQ2TYPE_BUILD_STRUCTURE] = 1;
		iNumSupportedCommands++;
	}


/*
	//go through list of tools and update available commands(10.11.2003)
	cComponentA* pCmpTrav = (cComponentA*)CmpAttached.pLByClass_First[CMPCLASS_TOOL]->pNext_classlist;

	while (pCmpTrav != CmpAttached.pLByClass_Last[CMPCLASS_TOOL])
	{
		cCmpInfo* pCmpInfo = pCA->pCmpInfoTable->pCmpTable[pCmpTrav->iType];

		if (pCmpInfo->pToolInfo->iUseProcedure == TOOL_BUILD)
		if (SupportList[CQ2TYPE_BUILD_STRUCTURE] == 0)
		{
			SupportList[CQ2TYPE_BUILD_STRUCTURE] = 1;
			iNumSupportedCommands++;
		}

		pCmpTrav = (cComponentA*)pCmpTrav->pNext_classlist;
	}
*/ //not necessary if we do it in Recalc_Attributes(10.12.2003)

	

	if (piSupportedCommands != NULL)
		delete(piSupportedCommands);

	//now, re-allocate the buffer and cull the list of supported commands
	piSupportedCommands = /**/new int[iNumSupportedCommands];

int lc2 = 0; //local counter

	for (lc1 = 0; lc1 < NUM_CQ2COMMANDS; lc1++)
	{
		if (SupportList[lc1] == 1)
		{
			piSupportedCommands[lc2] = lc1;
			lc2++;
		}
	}

	iiNumSupportedCommands = iNumSupportedCommands;





}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cUnitListA::WriteLog_Enter()
{
if (DEBUG + DEBUG_UNITLIST >= 1)
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
//08.09.2003
void UNITTYPEINFO::Set_Name(char* pName, int _iNameLength)
{

	memcpy(Name, pName, sizeof(char) * _iNameLength);
	iNameLength = _iNameLength;



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void UNITTYPEINFO::Set_ShortName(char* pName, int _iNameLength)
{

	memcpy(ShortName, pName, sizeof(char) * _iNameLength);
	iShortNameLength = _iNameLength;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void UNITTYPEINFO::Update_Cost(VOID* pCAccess)  //I am forced to supply a class access pointer here, otherwise I cannot access the component type list
{
	//for now, simply add up the costs of all the attached components, and multiply that by 1.0 for the full assembly(05.29.2004)
	cComponentA* pComponent = (cComponentA*)CmpAttached.pCmpList_First->pNext_list;
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	ddCost = 0;

	while (pComponent != CmpAttached.pCmpList_Last)
	{
		ddCost += pCA->pCmpInfoTable->pCmpTable[pComponent->iType]->ddResourceCost;
		//add material cost
		MaterialCost.Add(pCA->pCmpInfoTable->pCmpTable[pComponent->iType]->MaterialCost);

		pComponent = (cComponentA*)pComponent->pNext_list;
	}

		ddCost *= 1.0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void UNITTYPEINFO::Update_Permissions(VOID* pCAccess)  //I am forced to supply a class access pointer here, otherwise I cannot access the component type list
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//
	for (int lc1 = 0; lc1 < NUM_MATERIAL_TYPES; lc1++)
	{
		if ( MaterialCost.Materials[lc1] > 0)
		{
			Permissions_Required_Build.Add_Permission(10000 + lc1);  //permissions for materials over 10000
		}
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
