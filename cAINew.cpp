#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Init(void* pCA, HANDLE hLF, int _iControl)
{
	pCAccess = pCA;
	hLogFile = hLF;
	LocationsManager.Init(pCA, _iControl);

	iControl = _iControl;
	iAI_Override = 0;

	Init_Entity_Queues();
	Init_Location_Lists();
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Run()
{
	cClassAccess* pCA = (cClassAccess*) pCAccess;

	//DEBUG
	AI_Integrity_Check();

	Location_Processing();

	//for now, go through the whole list of entities and execute their tasking and run procedures
	cAIEntity* pTrav = pEntities_First->pNext;

	Update_TopLevel();
	//go through the list and update tactical infos
	while (pTrav != pEntities_Last)
	{
		if (pTrav->iType == AI_ENTITY_TYPE_SEED)
		{
			Update_Seed_Tactical_Info(pTrav);
		}

		pTrav = pTrav->pNext;
	}

	//sort the current status queue list
	Sort_Status_Queue();

int icount = 0;
	pTrav = pEntities_First->pNext;
	//tasking
	while (pTrav != pEntities_Last)
	{
		pTrav->Tasking();

		icount++;
		if (icount > 1000)
		{
			pCA->pChatBuffer->Add_CBNode("List corruption", 10);
			break;
		}

		pTrav = pTrav->pNext;
	}

icount = 0;
	//execution
	pTrav = pEntities_First->pNext;
	while (pTrav != pEntities_Last)
	{

		pTrav->Execute();
		
		icount++;
		if (icount > 1000)
		{
			pCA->pChatBuffer->Add_CBNode("List corruption", 10);
			break;
		}

		pTrav = pTrav->pNext;
	}



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Update_TopLevel()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//gather all needed information and give the relevant information to the AI
	//the idea is *aggregation*, to compress the flood of information
	//into an amount that is actually processable
	AITopLevel.iNumBuildings = 0;
	AITopLevel.iNumSeedGroups = 0;
	AITopLevel.iNumCapitals = 0;
	AITopLevel.iNumUnits = 0;
	AITopLevel.fOwnHP = 0;
	AITopLevel.fOwnFirepower = 0;
	AITopLevel.fOwnInfrastructure = 0;
	for (int lc3 = 0; lc3 < TEMP_DP_MAXPLAYERS; lc3++)
	{
		AITopLevel.fEnemyHP[ lc3 ] = 0;
		AITopLevel.fEnemyFirePower[lc3] = 0;
		AITopLevel.fEnemyInfrastructure[lc3] = 0;
	}

	//do a run through of seeds
	cBuildLocationSeedGroup* pGroupTrav = LocationsManager.pGroup_First->pGroup_Next;

	while (pGroupTrav != LocationsManager.pGroup_Last)
	{
		AITopLevel.iNumSeedGroups++;

		BOOL bHaveCapital = FALSE;
		cBuildLocationSeed* pSeedTrav = pGroupTrav->pSeed_First->pNextSeed;
		while (pSeedTrav != pGroupTrav->pSeed_Last)
		{
			cStructureA* pStruct = (cStructureA*)pCA->pGameWorld->pMap[pSeedTrav->sqLocation.index].pOccupant_Structure;
			if (pStruct != NULL)
			if (pStruct->iControl == iControl)
			{
				AITopLevel.iNumBuildings++;
				if (pStruct->iType == TEMP_STYPE_RSC)
				if (pStruct->fCompletion >= 1.0)
				{
					bHaveCapital = TRUE;
     			}

			}
			pSeedTrav = pSeedTrav->pNextSeed;
		}
		if (bHaveCapital == TRUE)
		{
			AITopLevel.iNumCapitals++;
		}

		pGroupTrav = pGroupTrav->pGroup_Next;
	}
    //save information
	AITopLevel.fOwnInfrastructure = AITopLevel.iNumCapitals;
	

    //do a runthrough of the structure list to figure out what structures the enemies have
	cStructureA* pStructTrav = (cStructureA*)pCA->pStructList->pSL_First->pNext;

	while (pStructTrav != pCA->pStructList->pSL_Last)
	{
		if (pStructTrav->iControl != iControl)
		if (pStructTrav->iType == TEMP_STYPE_RSC)
		{
				AITopLevel.fEnemyInfrastructure[pStructTrav->iControl] += 1.0;
		}

		pStructTrav = (cStructureA*)pStructTrav->pNext;
	}


	//now do a runthrough of the unit queue and find out how many units we have
	cUnitA* pUnitTrav = (cUnitA*)pCA->pUnitList->pUL_First->pNext_UL;

	while (pUnitTrav != pCA->pUnitList->pUL_Last)
	{
         double fFirePower = 0;
		 double fHP = pUnitTrav->fHP;

 			cComponentA* pPrimaryWeapon = pUnitTrav->pPrimaryWeapon;
			if (pPrimaryWeapon != NULL)
			{
				cCmpInfo* pInfo = pCA->pCmpInfoTable->pCmpTable[pPrimaryWeapon->iType];
				fFirePower = pInfo->pWeaponInfo->fDmgRate;
			}

		if (pUnitTrav->iControl == iControl)
		{
			AITopLevel.iNumUnits++;
			//
			AITopLevel.fOwnHP += fHP;
			AITopLevel.fOwnFirepower += fFirePower;
		} else
		{
			AITopLevel.fEnemyHP[ pUnitTrav->iControl ] += fHP;
			AITopLevel.fEnemyFirePower[ pUnitTrav->iControl ] += fFirePower;
		}

		pUnitTrav = (cUnitA*)pUnitTrav->pNext_UL;
	}

	//
	//calculate enemy averages
	double fTotalEnemyHP = 0;
	double fTotalEnemyFirePower = 0;
	double fTotalEnemyInfrastructure = 0;
	AITopLevel.iNumEnemies = 0;
	int lc2 = 0;
	for (lc2 = 0; lc2 < TEMP_DP_MAXPLAYERS; lc2++)
	if (lc2 != iControl)
	{
		if (pCA->pNationManager->ppNations[lc2] != NULL)
		{
			AITopLevel.iNumEnemies++;
			fTotalEnemyHP += AITopLevel.fEnemyHP[lc2];
			fTotalEnemyFirePower += AITopLevel.fEnemyFirePower[lc2];
			fTotalEnemyInfrastructure += AITopLevel.fEnemyInfrastructure[lc2];
		}
	}
	//then get averages.
	//if num of enemies is not 0...
	AITopLevel.fEnemyAverageHP = 0;
	AITopLevel.fEnemyAverageFirePower = 0;
	AITopLevel.fEnemyAverageInfrastructure = 0;
	if (AITopLevel.iNumEnemies > 0)
	{
		AITopLevel.fEnemyAverageHP = fTotalEnemyHP / AITopLevel.iNumEnemies;
		AITopLevel.fEnemyAverageFirePower = fTotalEnemyFirePower / AITopLevel.iNumEnemies;
		AITopLevel.fEnemyAverageInfrastructure = fTotalEnemyInfrastructure / AITopLevel.iNumEnemies;
	}

	//check map (this doesn't check every tile, but a grid sampling to the space
	//this should give a resonably accurate estimation of the real figure
	//with the advantage that the processing cost doesn't increase
	//with the map size
	int iResolution = 15;
	int iNumTilesSeen = 0;
	SQCOORDSET sqCheck;
	int iWidth = pCA->pGameWorld->iWidth;
	int iHeight = pCA->pGameWorld->iHeight;
	MTILE* pMap = pCA->pGameWorld->pMap;
	int iCheck = 1 << iControl;
	for (int lcx = 0; lcx < iResolution; lcx++)
    for (int lcy = 0; lcy < iResolution; lcy++)
	{
		sqCheck.x = (double(lcx)/double(iResolution))*iWidth;
		sqCheck.y = (double(lcy)/double(iResolution))*iHeight;
		sqCheck.Calc_Index(iWidth);
	
		//check reveal
		if ( (pMap[sqCheck.index].barrRevealed & iCheck) != 0)
			iNumTilesSeen++;
	}
	AITopLevel.fRevealedMapFraction = double(iNumTilesSeen)/double(iResolution*iResolution);

    //compute dispositions
	double ddFirePowerRatio = (AITopLevel.fOwnFirepower + AITopLevel.fOwnHP) / (AITopLevel.fEnemyAverageFirePower + AITopLevel.fEnemyAverageHP + 0.00001);
    AITopLevel.fAttackDisposition = ddFirePowerRatio / (ddFirePowerRatio + 1.0);
	AITopLevel.fDefenseDisposition = 1.0 - AITopLevel.fAttackDisposition;
	double ddInfrastructureRatio = AITopLevel.fEnemyAverageInfrastructure / (AITopLevel.fOwnInfrastructure + 0.000001);
	AITopLevel.fDevelopDisposition = ddInfrastructureRatio / (ddInfrastructureRatio + 1.0);
	AITopLevel.fExploreDisposition = 1.0 - AITopLevel.fRevealedMapFraction;

	//compute material demand
	//I don't really know how to do this, so I'll settle for making it all 1 for the time being (2008.11.17)
	for ( int lc9 = 0; lc9 < NUM_MATERIAL_TYPES; lc9++)
		AITopLevel.MaterialDemand[lc9] = 1;
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Update_Seed_Tactical_Info(cAIEntity* pEnt_Seed)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cBuildLocationSeedGroup* pBLSG = pEnt_Seed->pSeed;

	//reset all the group information
	pBLSG->Clear_Gather_Info();

	//DEBUG
	char NText[50];

	//now go through all the seeds
	cBuildLocationSeed* pBLS_Trav = pBLSG->pSeed_First->pNextSeed;
	while (pBLS_Trav != pBLSG->pSeed_Last)
		{
			//get the structure occupying the site
			cStructureA* pStruct = (cStructureA*)pCA->pGameWorld->pMap[pBLS_Trav->sqLocation.index].pOccupant_Structure;

			if ((pStruct == NULL) && (pBLS_Trav->bFudged == FALSE))
			{
				char NText[200];
				itoa(pBLS_Trav->sqLocation.index, NText, 10);
				strcat(NText, " Error 170: Seed found for which no building exists");
				pCA->pChatBuffer->Add_CBNode(NText, 600);
			}
	
			if (pStruct != NULL)
			{
				//if it's a capital, flag and update production
				if (pStruct->iType == TEMP_STYPE_RSC)
				if (pStruct->fCompletion >= 1.0)
				{
					pBLSG->bContainsCapital = TRUE;
					pBLSG->pCapital = pBLS_Trav;
					if (pStruct->pResourceCollection != NULL)
					for (int lc5 = 0; lc5 < NUM_MATERIAL_TYPES; lc5++)
						if ( pStruct->pResourceCollection[lc5] > 0)
							pBLSG->bContainsProductionFor[lc5] = TRUE;
				}
				//if it's a forge, flag and update production
				if (pStruct->iType == TEMP_STYPE_FORGE)
				if (pStruct->fCompletion >= 1.0)
				{
					pBLSG->bContainsForge = TRUE;
					if (pStruct->pResourceCollection != NULL)
					for (int lc5 = 0; lc5 < NUM_MATERIAL_TYPES; lc5++)
						if ( pStruct->pResourceCollection[lc5] > 0)
							pBLSG->bContainsProductionFor[lc5] = TRUE;
				}
				//if it's a refinery, flag and update production
				if (pStruct->iType == TEMP_STYPE_REFINERY)
				if (pStruct->fCompletion >= 1.0)
				{
					pBLSG->bContainsRefinery = TRUE;
					if (pStruct->pResourceCollection != NULL)
					for (int lc5 = 0; lc5 < NUM_MATERIAL_TYPES; lc5++)
						if ( pStruct->pResourceCollection[lc5] > 0)
							pBLSG->bContainsProductionFor[lc5] = TRUE;
				}
				//if it's a factory, flag and update production
				if (pStruct->iType == TEMP_STYPE_FACTORY_ABC)
				if (pStruct->fCompletion >= 1.0)
				{
					pBLSG->bContainsFactory = TRUE;
				}
				//if it's a shipyard flag and update production
				if (pStruct->iType == TEMP_STYPE_SHIPYARD)
				if (pStruct->fCompletion >= 1.0)
				{
					pBLSG->bContainsShipyard = TRUE;
				}

				//if its a storage unit, update storage
				if (pStruct->iType == TEMP_STYPE_TANK)
				if (pStruct->fCompletion >= 1.0)
				if (pStruct->Storage.iMaterialType >= 0)
				{
					pBLSG->bContainsStorageFor[pStruct->Storage.iMaterialType] = TRUE;
					pBLSG->fContains[pStruct->Storage.iMaterialType] += pStruct->Storage.ddMaterialStored;
				}
			}



			pBLS_Trav = pBLS_Trav->pNextSeed;
		}

		//DEBUG
	/*	if (iControl == 1)
		for (int lc8 = 0; lc8 < NUM_MATERIAL_TYPES; lc8++)
		{
		//DEBUG
		if (pBLSG->bContainsProductionFor[lc8])
		{
			itoa(lc8, NText, 10);
			strcat(NText, " produce");
			pCA->pChatBuffer->Add_CBNode(NText, 10);
		}
		//DEBUG
		if (pBLSG->bContainsStorageFor[lc8])
		{
			itoa(lc8, NText, 10);
			strcat(NText, " storage");
			pCA->pChatBuffer->Add_CBNode(NText, 10);
		}
		}*/



		//check and update whether there is adequate storage
		pBLSG->bAdequateStorage = TRUE;
		for (int lc11 = 0; lc11 < NUM_MATERIAL_TYPES; lc11++)
		{
			if (  (pBLSG->bContainsProductionFor[lc11] == TRUE) &&
				  (pBLSG->bContainsStorageFor[lc11] == FALSE) )
			{
				  pBLSG->bAdequateStorage = FALSE;
				  //clear the whole adequate
				  //Evaluation.bTotalAdequateStorage = FALSE;
			}
		}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Init_Entity_Queues()
{
	//initialize anchors
	pEntities_First = new cAIEntity;
	pEntities_Last = new cAIEntity;
	pEntities_First->pPrev = NULL;
	pEntities_First->pNext = pEntities_Last;
	pEntities_Last->pPrev = pEntities_First;
	pEntities_Last->pNext = NULL;

	//
	pRequest_First = new cAIEntity;
	pRequest_Last = new cAIEntity;
	pRequest_First->pStatusQueue_Prev = NULL;
	pRequest_First->pStatusQueue_Next = pRequest_Last;
	pRequest_Last->pStatusQueue_Prev = pRequest_First;
	pRequest_Last->pStatusQueue_Next = NULL;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Exit()
{
	cClassAccess* pCA = (cClassAccess*) pCAccess;

		//DEBUG
		pCA->pDD_Display->Write_Log("Clearing entries\r\n");
	//clear queues
	Clear_Entities();

	Exit_Location_Lists();
		//DEBUG
		pCA->pDD_Display->Write_Log("Exiting location manager\r\n");
	//exit location manager
	LocationsManager.Exit();
		//DEBUG
		pCA->pDD_Display->Write_Log("Done\r\n");


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cAIEntity* cAINew::Find_Entity(int iID, int iType)
{
	cAIEntity* pTrav = pEntities_First->pNext;

	while (pTrav != pEntities_Last)
	{
		//wow there was a pretty nasty unrealized bug here
		if ( (pTrav->iID == iID) && (pTrav->iType == iType))
			return pTrav;

		pTrav = pTrav->pNext;
	}

	//if we didn't find
	return NULL;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Clear_Assistants()
{
	cClassAccess* pCA = (cClassAccess*) pCAccess;

	cAIEntity* pSubQueue1 = pAsstRecv_Head;
	cAIEntity* pSubQueue2;
	
	//disband those who were assisting it
	//the other 
	while (pSubQueue1 != NULL)
	{
		//DEBUG
		char NText[50];
		pCA->pDD_Display->Write_Log("Assistant cleared. ID: ");
		itoa(pSubQueue1->iID, NText, 10);
		pCA->pDD_Display->Write_Log(NText);
		pCA->pDD_Display->Write_Log(" Type: ");
		itoa(pSubQueue1->iType, NText, 10);
		pCA->pDD_Display->Write_Log(NText);
		pCA->pDD_Display->Write_Log(" Tick: ");
		itoa(pCA->pTimer->iCurrTick, NText, 10);
		pCA->pDD_Display->Write_Log(NText);
		pCA->pDD_Display->Write_Log(" Control: ");
		itoa(pSubQueue1->iControl, NText, 10);
		pCA->pDD_Display->Write_Log(NText);
		pCA->pDD_Display->Write_Log("\r\n");

		pSubQueue2 = pSubQueue1->pAsstGive_Next;
		pSubQueue1->pAsstGive_Next = NULL;
		pSubQueue1->pAsstGive_Prev = NULL;
		pSubQueue1->pAsstGive_Target = NULL;
		pSubQueue1->iTaskType = -1;
		pSubQueue1->iRole = -1;
		
		pSubQueue1 = pSubQueue2;
	}
	//reset number of assistants
	iNumAsstRecv = 0;
	//reset num in role array
	for (int lc1 = 0; lc1 < 10; lc1++)
		arrNumInRole[lc1] = 0;
	//reset pointers
	pAsstRecv_Head = NULL;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Clear_Assisting()
{
	cAIEntity* pTarget = pAsstGive_Target;
	cAIEntity* pNext = pAsstGive_Next; 
	cAIEntity* pPrev = pAsstGive_Prev;

	//clear pointers
	pAsstGive_Target = NULL;
	pAsstGive_Next = NULL;
	pAsstGive_Prev = NULL;

	//reset the head if necessary(since there are no anchors)
	if (pTarget != NULL)
	if (pTarget->pAsstRecv_Head == this)
			pTarget->pAsstRecv_Head = pNext;

	//link around each other
	if (pNext != NULL)
		pNext->pAsstGive_Prev = pPrev;
	if (pPrev != NULL)
		pPrev->pAsstGive_Next = pNext;
	//decrement number of assisting at the target
	if (pTarget != NULL)
		pTarget->iNumAsstRecv--;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Remove_Entity(int iID, int iType)
{
	cAIEntity* pRemove = Find_Entity(iID, iType);

	//if we couldn't find it
	if (pRemove == NULL)
		return;
	
	//disband those who were assisting it
	pRemove->Clear_Assistants();
	//notify that it is not assisting anymore either
	pRemove->Clear_Assisting();

	//unlink from both
	cAIEntity* pPrev = pRemove->pPrev;
	cAIEntity* pNext = pRemove->pNext;
	//	
	pPrev->pNext = pNext;
	pNext->pPrev = pPrev;

	pPrev = pRemove->pStatusQueue_Prev;
	pNext = pRemove->pStatusQueue_Next;

	pNext->pStatusQueue_Prev = pPrev;
	pPrev->pStatusQueue_Next = pNext;

	//now delete the entity itself
	delete pRemove;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Clear_Entities()
{
	cClassAccess* pCA = (cClassAccess*) pCAccess;

	cAIEntity* pTrav;
	cAIEntity* pTrav2;

	pTrav = pEntities_First->pNext;

	while (pTrav != pEntities_Last)
	{
		pTrav2 = pTrav->pNext;

		delete pTrav;   // <-- need to find out why this doesn't work...
		pTrav = pTrav2;
	}
	//relink all queries
	pEntities_First->pNext = pEntities_Last;
	pEntities_Last->pPrev = pEntities_First;

	pRequest_First->pStatusQueue_Next = pRequest_Last;
	pRequest_Last->pStatusQueue_Prev = pRequest_First;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cAIEntity* cAINew::Create_Add_Entity(int iID, int iType)
{
	cAIEntity* pNew = Create_Entity(iID, iType);
	Add_Entity(pNew);

	return pNew;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Add_Entity(cAIEntity* pNew)
{
	cClassAccess* pCA = (cClassAccess*) pCAccess;


	if (pNew == NULL)
			return;

	//link to both queues
	cAIEntity* pPrev = pEntities_Last->pPrev;
	cAIEntity* pNext = pEntities_Last;
	
	//link in main queue
	pPrev->pNext = pNew;
	pNext->pPrev = pNew;
	pNew->pPrev = pPrev;
	pNew->pNext = pNext;

	//now link to 
	pPrev = pRequest_Last->pStatusQueue_Prev;
	pNext = pRequest_Last;

	//now link again
	pPrev->pStatusQueue_Next = pNew;
	pNext->pStatusQueue_Prev = pNew;
	pNew->pStatusQueue_Next = pNext;
	pNew->pStatusQueue_Prev = pPrev;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cAIEntity* cAINew::Create_Entity(int iID, int iType)
{
	cClassAccess* pCA = (cClassAccess*) pCAccess;

	cAIEntity* pNew = new cAIEntity;

//DEBUG
	
char NText[50];
pCA->pDD_Display->Write_Log("New Entity Created. ID: ");
itoa(iID, NText, 10);
pCA->pDD_Display->Write_Log(NText);
if (iType == AI_ENTITY_TYPE_UNIT)
	pCA->pDD_Display->Write_Log(" Type: UNIT");
if (iType == AI_ENTITY_TYPE_SEED)
	pCA->pDD_Display->Write_Log(" Type: SEED");
if (iType == AI_ENTITY_TYPE_SECTOR)
	pCA->pDD_Display->Write_Log(" Type: SECTOR");
if (iType == AI_ENTITY_TYPE_STRUCT)
	pCA->pDD_Display->Write_Log(" Type: STRUCT");
	pCA->pDD_Display->WriteLog_Enter();
	


	pNew->Init(pCAccess, iControl );
	pNew->iID = iID;
	pNew->iType = iType;
	

	return pNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Event_Structure_Created(cStructureA* pStruct)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	Create_Add_Entity(pStruct->iID, AI_ENTITY_TYPE_STRUCT);

	//now go through the rest of this only if the struct being added is our own
	if (pStruct->iControl != iControl)
		return;

	//now go through and check which structure it is connected to, and closest to
	cBuildLocationSeedGroup* pGroup_Best = NULL;
	double ddDistance_Best = -1;  //yeah....

	cBuildLocationSeedGroup* pTrav = LocationsManager.pGroup_First->pGroup_Next;


	int iCount = 0;

	while (pTrav != LocationsManager.pGroup_Last)
	{
		double ddDistance = -1;
		if (pTrav->pSeed_First->pNextSeed != pTrav->pSeed_Last)
		{

			ddDistance = pCA->pPathFinder->arePointsConnected(pStruct->Pos_primary.index, pTrav->pSeed_First->pNextSeed->sqLocation.index);
			iCount++;
		} else
		{
		}

		if (ddDistance != -1)
		{
			if (ddDistance_Best == -1)
			{
				ddDistance_Best = ddDistance;
				pGroup_Best = pTrav;
			} else if (ddDistance < ddDistance_Best)
				{
					ddDistance_Best = ddDistance;
					pGroup_Best = pTrav;			
				}
		}

		pTrav = pTrav->pGroup_Next;
	}

	//if we found a group, add it to the seed we found
	if (pGroup_Best != NULL)
	{
		pGroup_Best->Create_Add_Seed(pStruct->Pos_primary.index);
	}
	//otherwise just add it as a new seed
	if (pGroup_Best == NULL)
	{
		LocationsManager.Create_Add_BuildLocationSeedGroup(pStruct->Pos_primary.index);
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Event_Structure_Destroyed(cStructureA* pStruct)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	//when the structure is destroyed, both it and the seed die
	//find and delete entity
	cAIEntity* pDelete = Find_Entity(pStruct->iID, AI_ENTITY_TYPE_STRUCT);

	//only try to remove corresponding seed if we're on same team
	if (pStruct->iControl != iControl)
	{
		Remove_Entity(pDelete->iID, pDelete->iType);	
		return;
	}

	if (pDelete != NULL)
	{
		//for now, mainly find seed corresponding to this structure, and remove it
		cBuildLocationSeed* pSeed = LocationsManager.Find_Seed(pStruct->Pos_primary.index);

		if (pSeed == NULL)
		{
			//DEBUG
			pCA->pChatBuffer->Add_CBNode("Top: Seed not found.", 60);
		}

		if (pSeed != NULL)
		{
			cBuildLocationSeedGroup* pSeedGroup = (cBuildLocationSeedGroup*)pSeed->pSeedGroup;
			//DEBUG
			pCA->pChatBuffer->Add_CBNode("Seed destroyed.", 60);

			pSeedGroup->Delete_Seed(pSeed);
		}
		Remove_Entity(pDelete->iID, pDelete->iType);	
		return;
	}

	pCA->pChatBuffer->Add_CBNode("Entity not found.", 60);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Event_Unit_Created(cUnitA* pUnit)
{

	Create_Add_Entity(pUnit->iID, AI_ENTITY_TYPE_UNIT);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Event_Unit_Destroyed(cUnitA* pUnit)
{
	//find and delete entity
	cAIEntity* pDelete = Find_Entity(pUnit->iID, AI_ENTITY_TYPE_UNIT);

	if (pDelete != NULL)
		Remove_Entity(pDelete->iID, pDelete->iType);	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Event_Sector_Created(cSector* pSector)
{
	Create_Add_Entity(pSector->iID, AI_ENTITY_TYPE_SECTOR);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Event_Sector_Destroyed(cSector* pSector)
{
	//I don't anticipate there ever being anything here
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Event_Seed_Created(cBuildLocationSeedGroup* pSeedGroup)
{
	//for a seedgroup, the ID is the maptile
	cAIEntity* pEntity = Create_Add_Entity(pSeedGroup->iID, AI_ENTITY_TYPE_SEED);
	//hack in the seedgroup pointer
	pEntity->pSeed = pSeedGroup;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Event_Seed_Destroyed(cBuildLocationSeedGroup* pSeedGroup)
{
	//find and delete entity
	cAIEntity* pDelete = Find_Entity(pSeedGroup->iID, AI_ENTITY_TYPE_SEED);

	if (pDelete != NULL)
		Remove_Entity(pDelete->iID, pDelete->iType);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Sort_Status_Queue()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//do a one-pass bubblesort through the list
	cAIEntity* pBack = pRequest_Last->pStatusQueue_Prev;
	cAIEntity* pFront = pBack->pStatusQueue_Prev;

	cAIEntity* pNextup;

	while (pFront != pRequest_First)
	{
		pNextup = pFront->pStatusQueue_Prev;
		//housekeeping, if tasks have not been raised, set immediacy to 0
		if (pBack->bTaskRaised == FALSE)
			pBack->fImmediacy = 0;
		if (pFront->bTaskRaised == FALSE)
			pFront->fImmediacy = 0;

		//compare immediacies...if Back if greater than front, then swap
		if (pBack->fImmediacy > pFront->fImmediacy)
		{
			cAIEntity* pFrontfront = pNextup;
			cAIEntity* pBackback = pBack->pStatusQueue_Next;
			cAIEntity* pOldFront = pFront;
			cAIEntity* pOldBack = pBack;

			//swap
			pBack = pOldFront;
			pFront = pOldBack;

			//and swap their connecting pointers
			pFrontfront->pStatusQueue_Next = pFront;
			pFront->pStatusQueue_Prev = pFrontfront;
			pFront->pStatusQueue_Next = pBack;
			pBack->pStatusQueue_Prev = pFront;
			pBack->pStatusQueue_Next = pBackback;
			pBackback->pStatusQueue_Prev = pBack;

		} 

		//now increment
		pBack = pFront;
		pFront = pNextup;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Set_Asst(cAIEntity* pAsst)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;
		//unlink self from current assistance
		Clear_Assisting();

		//then re-link
		pAsstGive_Target = pAsst;
		if (pAsst != NULL)
		{
			pAsst->iNumAsstRecv++;
			//then link in
				//note that the head has a NULL pPrev.
				//we don't try to link the target
				//because it has no pointers for this purpose
			cAIEntity* pNewPrev = NULL;
			cAIEntity* pNewNext = pAsst->pAsstRecv_Head;

			if (pNewPrev != NULL)
				pNewPrev->pAsstGive_Next = this;
			if (pNewNext != NULL)
				pNewNext->pAsstGive_Prev = this;
			this->pAsstGive_Next = pNewNext;
			this->pAsstGive_Prev = pNewPrev;
			//then reset the head
			pAsst->pAsstRecv_Head = this;
		}

	}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Init(VOID* pCA, int iCtrl)
	{
		pCAccess = pCA;

		pPrev = NULL;
		pNext = NULL;
		bTaskRaised = FALSE;

		Init_SubQueue();
		Init_Asst();

		iControl = iCtrl;

		Reset_RoleNums();
		
	}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Reset_RoleNums()
{
	for (int lc1 = 0; lc1 < TEMP_AITASK_MAX_ROLES; lc1++)
	{
		arrNumInRole[lc1] = 0;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Init_SubQueue()
	{
		pSubQueue_Head = NULL;
		pSubQueue_Tail = NULL;
		iNumSubEntities = 0;
	}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Init_Asst()
	{
		pAsstRecv_Head = NULL;
		iNumAsstRecv = 0;

		pAsstGive_Target = NULL;
	    pAsstGive_Prev = NULL; 
	    pAsstGive_Next = NULL;
	}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Add_SubEntity(cAIEntity* pAdd)
	{	
		iNumSubEntities = 0;
		//strip from old queue
		cAIEntity* pOPrev = pAdd->pPrev;
		cAIEntity* pONext = pAdd->pNext;
		if (pOPrev != NULL)
			pOPrev->pNext = pONext;
		if (pONext != NULL)
			pONext->pPrev = pOPrev;

		//special case: 0 entries
		if (pSubQueue_Head == NULL)
		{
			pSubQueue_Head = pAdd;
			pSubQueue_Tail = pAdd;
			pAdd->pPrev = NULL;
			pAdd->pNext = NULL;
			return;
		}
		//special case : 1 entry
		if (pSubQueue_Head == pSubQueue_Tail)
		{
			pSubQueue_Tail = pAdd;
			pSubQueue_Head->pNext = pSubQueue_Tail;
			pSubQueue_Tail->pPrev = pSubQueue_Head;
			pSubQueue_Tail->pNext = NULL;
			return;
		}
		//normal case
		cAIEntity* pNPrev = pSubQueue_Tail;
		pSubQueue_Tail = pAdd;
		pNPrev->pNext = pAdd;
		pAdd->pPrev = pNPrev;
		pAdd->pNext = NULL;
	}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Tasking()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	if (iType == AI_ENTITY_TYPE_UNIT)
		Tasking_Unit();
	if (iType == AI_ENTITY_TYPE_STRUCT)
		Tasking_Struct();
	if (iType == AI_ENTITY_TYPE_SEED)
		Tasking_Seed();
	if (iType == AI_ENTITY_TYPE_SECTOR)
		Tasking_Sector();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cAIModeStack cAIEntity::Choose_Modes()
{
	cClassAccess* pCA = (cClassAccess*) pCAccess;

	if (iType == AI_ENTITY_TYPE_UNIT)
		return Choose_Modes_Unit();
	if (iType == AI_ENTITY_TYPE_STRUCT)
		return Choose_Modes_Struct();
	if (iType == AI_ENTITY_TYPE_SEED)
		return Choose_Modes_Seed();
	if (iType == AI_ENTITY_TYPE_SECTOR)
		return Choose_Modes_Sector();

	pCA->pChatBuffer->Add_CBNode("Error 124: unrecognized entity type.", 600);

	return Modes;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIModeStack::Clear()
{
	for (int lc1 = 0; lc1 < AI_MODESTACK_MAX_MODES; lc1++)
	{
		Modes[lc1].fEfficacy = 0;
		Modes[lc1].iMode = -1;
		Modes[lc1].iParam1 = -1;
		Modes[lc1].iParam2 = -1;
		Modes[lc1].iParam3 = -1;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIModeStack::Push(int iMode, double fEfficacy)
{
	//move everythign back (or...up)
	for (int lc1 = AI_MODESTACK_MAX_MODES -1; lc1 > 0; lc1--)
	{
		Modes[lc1].fEfficacy = Modes[lc1-1].fEfficacy;
		Modes[lc1].iMode = Modes[lc1-1].fEfficacy;
	}
	//then set mode 0
	Modes[0].iMode = iMode;
	Modes[0].fEfficacy = fEfficacy;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cAIModeStack cAIEntity::Choose_Modes_Unit()
{
	cClassAccess* pCA = (cClassAccess*) pCAccess;
	int iLast_Selected_Mode = -1;
	double fTest;
	double fHighest_Efficacy = 0;
	int iHighest_Mode = -1;
	
	Modes_Consider.Clear();
	
	while (TRUE)
	{
		//right now, for units, there's nothing to choose
		break;

		iHighest_Mode = -1;
		fHighest_Efficacy = 0;
		//we haven't done anything yet
		if (iLast_Selected_Mode < 0)
		{
			//right now there's no modes the unit can create anyway
/* 
			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_;
				fHighest_Efficacy = fTest;
			}

			fTest = Evaluate_
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = ...;
				fHighest_Efficacy = fTest;
			}
*/
		} 
		//right now there's no multi-level modes
/*		else if (iLast_Selected_Mode == )
		{




		} else if (iLast_Selected_Mode == )
		{


		} */

		//push mode onto stack
		Modes_Consider.Push(iHighest_Mode, fHighest_Efficacy);

		//
		if (iLast_Selected_Mode >= 0)
		if (iLast_Selected_Mode % 10 != 0)
			break;

		//also break if we didn't get anything
		if (iLast_Selected_Mode < 0)
			break;
	}

	return Modes_Consider;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cAIModeStack cAIEntity::Choose_Modes_Struct()
{
	cClassAccess* pCA = (cClassAccess*) pCAccess;
	int iLast_Selected_Mode = -1;
	double fTest;
	double fHighest_Efficacy = 0;
	int iHighest_Mode = -1;
	
	Modes_Consider.Clear();
	
	while (TRUE)
	{
		iHighest_Mode = -1;
		fHighest_Efficacy = 0;
		//we haven't done anything yet
		if (iLast_Selected_Mode < 0)
		{
			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_BUILD_UNIT);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_BUILD_UNIT;
				fHighest_Efficacy = fTest;
			}
		} 
		else if (iLast_Selected_Mode == AI_TASKTYPE_BUILD_UNIT)
		{
		
			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_BUILD_UNIT_BASE);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_BUILD_UNIT_BASE;
				fHighest_Efficacy = fTest;
			}

			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_BUILD_TANKER_A);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_BUILD_TANKER_A;
				fHighest_Efficacy = fTest;
			}

		} 

		//push mode onto stack
		Modes_Consider.Push(iHighest_Mode, fHighest_Efficacy);
		//set mode selection
		iLast_Selected_Mode = iHighest_Mode;

		//
		if (iLast_Selected_Mode >= 0)
		if (iLast_Selected_Mode % 10 != 0)
			break;

		//also break if we didn't get anything
		if (iLast_Selected_Mode < 0)
			break;
	}

	return Modes_Consider;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cAIModeStack cAIEntity::Choose_Modes_Seed()
{
	cClassAccess* pCA = (cClassAccess*) pCAccess;
	int iLast_Selected_Mode = -1;
	double fTest;
	double fHighest_Efficacy = 0;
	int iHighest_Mode = -1;
	
	Modes_Consider.Clear();
	
	while (TRUE)
	{
		iHighest_Mode = -1;
		fHighest_Efficacy = 0;
		//we haven't done anything yet
		if (iLast_Selected_Mode < 0)
		{
			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_ATTACK_SEED);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_ATTACK_SEED;
				fHighest_Efficacy = fTest;
			}

			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_EXPLORE_SEED);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_EXPLORE_SEED;
				fHighest_Efficacy = fTest;
			}
			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_DEFEND_SEED);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_DEFEND_SEED;
				fHighest_Efficacy = fTest;
			}
			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_DEVELOP_SEED);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_DEVELOP_SEED;
				fHighest_Efficacy = fTest;
			}
		} 

		else if (iLast_Selected_Mode == AI_TASKTYPE_DEVELOP_SEED)
		{


			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_BUILD_STORAGE);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_BUILD_STORAGE;
				fHighest_Efficacy = fTest;
			}
			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_BUILD_FACTORY);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_BUILD_FACTORY;
				fHighest_Efficacy = fTest;
			}
			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_BUILD_REFINERY);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_BUILD_REFINERY;
				fHighest_Efficacy = fTest;
			}
			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_BUILD_SHIPYARD);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_BUILD_SHIPYARD;
				fHighest_Efficacy = fTest;
			}
			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_BUILD_FORGE);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_BUILD_FORGE;
				fHighest_Efficacy = fTest;
			}
			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_BUILD_EXTRACTOR);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_BUILD_EXTRACTOR;
				fHighest_Efficacy = fTest;
			}
		} 

		//push mode onto stack
		Modes_Consider.Push(iHighest_Mode, fHighest_Efficacy);
		//set mode selection
		iLast_Selected_Mode = iHighest_Mode;

		//
		if (iLast_Selected_Mode >= 0)
		if (iLast_Selected_Mode % 10 != 0)
			break;

		//also break if we didn't get anything
		if (iLast_Selected_Mode < 0)
			break;
	}

	return Modes_Consider;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cAIModeStack cAIEntity::Choose_Modes_Sector()
{
	cClassAccess* pCA = (cClassAccess*) pCAccess;
	int iLast_Selected_Mode = -1;
	double fTest;
	double fHighest_Efficacy = 0;
	int iHighest_Mode = -1;
	
	Modes_Consider.Clear();
	


	while (TRUE)
	{
		iHighest_Mode = -1;
		fHighest_Efficacy = 0;
		//we haven't done anything yet
		if (iLast_Selected_Mode < 0)
		{
			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_ATTACK_SECTOR);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_ATTACK_SECTOR;
				fHighest_Efficacy = fTest;
			}

			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_EXPLORE_SECTOR);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_EXPLORE_SECTOR;
				fHighest_Efficacy = fTest;
			}
			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_DEFEND_SECTOR);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_DEFEND_SECTOR;
				fHighest_Efficacy = fTest;
			}
			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_DEVELOP_SECTOR);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_DEVELOP_SECTOR;
				fHighest_Efficacy = fTest;

			}


		} 
		//right now there's no multi-level modes
		else if (iLast_Selected_Mode == AI_TASKTYPE_DEVELOP_SECTOR)
		{



			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_ESTABLISH_CAPITAL);

			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_ESTABLISH_CAPITAL;
				fHighest_Efficacy = fTest;

			}

			fTest = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, AI_TASKTYPE_ESTABLISH_OUTPOST);
			if (fTest > fHighest_Efficacy)
			{
				iHighest_Mode = AI_TASKTYPE_ESTABLISH_OUTPOST;
				fHighest_Efficacy = fTest;
			}

		} 

		//push mode onto stack
		Modes_Consider.Push(iHighest_Mode, fHighest_Efficacy);
		//set mode selection
		iLast_Selected_Mode = iHighest_Mode;

		//
		if (iLast_Selected_Mode >= 0)
		if (iLast_Selected_Mode % 10 != 0)
		{
			//DEBUG
			//char NText[50];
			//itoa(iLast_Selected_Mode, NText, 10);
			//pCA->pDD_Display->Write_Log("Broke on task: ");
			//pCA->pDD_Display->Write_Log(NText);
			//pCA->pDD_Display->Write_Log("\r\n");

			break;
		}

		//also break if we didn't get anything
		if (iLast_Selected_Mode < 0)
		{
			//DEBUG
			//if (iLast_Selected_Mode % 10 == 0)
			//	pCA->pDD_Display->Write_Log("Broke on even-10 task\r\n");

			break;
		}
	}

	return Modes_Consider;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task(cAIEntity* Entity, cAIEntity* Entity_Join, int iTask, int iRoleNum)
{
	
	if (iTask == AI_TASKTYPE_ATTACK_SECTOR)
		return Evaluate_Unit_Join_Task_Attack_Sector(Entity, Entity_Join, iRoleNum);
//	if (iTask == AI_TASKTYPE_DEVELOP_SECTOR)
//		return Evaluate_Unit_Join_Task_Develop_Sector(Entity, Entity_Join, iRoleNum);
	if (iTask == AI_TASKTYPE_DEFEND_SECTOR)
		return Evaluate_Unit_Join_Task_Defend_Sector(Entity, Entity_Join, iRoleNum);
	if (iTask == AI_TASKTYPE_EXPLORE_SECTOR)
		return Evaluate_Unit_Join_Task_Explore_Sector(Entity, Entity_Join, iRoleNum);
	if (iTask == AI_TASKTYPE_ESTABLISH_CAPITAL)
		return Evaluate_Unit_Join_Task_Establish_Capital(Entity, Entity_Join, iRoleNum);
	if (iTask == AI_TASKTYPE_EXPLORE_SEED)
		return Evaluate_Unit_Join_Task_Explore_Seed(Entity, Entity_Join, iRoleNum);
	if (iTask == AI_TASKTYPE_ESTABLISH_OUTPOST)
		return Evaluate_Unit_Join_Task_Establish_Outpost(Entity, Entity_Join, iRoleNum);
	if (iTask == AI_TASKTYPE_ATTACK_SEED)
		return Evaluate_Unit_Join_Task_Attack_Seed(Entity, Entity_Join, iRoleNum);
	if (iTask == AI_TASKTYPE_DEFEND_SEED)
		return Evaluate_Unit_Join_Task_Defend_Seed(Entity, Entity_Join, iRoleNum);
//	if (iTask == AI_TASKTYPE_DEVELOP_SEED)
//		return Evaluate_Unit_Join_Task_Develop_Seed(Entity, Entity_Join, iRoleNum);
	if (iTask == AI_TASKTYPE_BUILD_STORAGE)
		return Evaluate_Unit_Join_Task_Build_Storage(Entity, Entity_Join, iRoleNum);
	if (iTask == AI_TASKTYPE_BUILD_FORGE)
		return Evaluate_Unit_Join_Task_Build_Forge(Entity, Entity_Join, iRoleNum);
	if (iTask == AI_TASKTYPE_BUILD_FACTORY)
		return Evaluate_Unit_Join_Task_Build_Factory(Entity, Entity_Join, iRoleNum);
	if (iTask == AI_TASKTYPE_BUILD_SHIPYARD)
		return Evaluate_Unit_Join_Task_Build_Shipyard(Entity, Entity_Join, iRoleNum);
	if (iTask == AI_TASKTYPE_BUILD_REFINERY)
		return Evaluate_Unit_Join_Task_Build_Refinery(Entity, Entity_Join, iRoleNum);
	if (iTask == AI_TASKTYPE_BUILD_EXTRACTOR)
		return Evaluate_Unit_Join_Task_Build_Extractor(Entity, Entity_Join, iRoleNum);

	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task(cAIEntity* Entity, int iTask)
{
	
	if (iTask == AI_TASKTYPE_ATTACK_SECTOR)
		return Evaluate_Create_Task_Attack_Sector(Entity);
	if (iTask == AI_TASKTYPE_DEVELOP_SECTOR)
		return Evaluate_Create_Task_Develop_Sector(Entity);
	if (iTask == AI_TASKTYPE_DEFEND_SECTOR)
		return Evaluate_Create_Task_Defend_Sector(Entity);
	if (iTask == AI_TASKTYPE_EXPLORE_SECTOR)
		return Evaluate_Create_Task_Explore_Sector(Entity);
	if (iTask == AI_TASKTYPE_ESTABLISH_CAPITAL)
		return Evaluate_Create_Task_Establish_Capital(Entity);
	if (iTask == AI_TASKTYPE_ESTABLISH_OUTPOST)
		return Evaluate_Create_Task_Establish_Outpost(Entity);
	if (iTask == AI_TASKTYPE_EXPLORE_SEED)
		return Evaluate_Create_Task_Explore_Seed(Entity);
	if (iTask == AI_TASKTYPE_ATTACK_SEED)
		return Evaluate_Create_Task_Attack_Seed(Entity);
	if (iTask == AI_TASKTYPE_DEFEND_SEED)
		return Evaluate_Create_Task_Defend_Seed(Entity);
	if (iTask == AI_TASKTYPE_DEVELOP_SEED)
		return Evaluate_Create_Task_Develop_Seed(Entity);
	if (iTask == AI_TASKTYPE_BUILD_STORAGE)
		return Evaluate_Create_Task_Build_Storage(Entity);
	if (iTask == AI_TASKTYPE_BUILD_FORGE)
		return Evaluate_Create_Task_Build_Forge(Entity);
	if (iTask == AI_TASKTYPE_BUILD_FACTORY)
		return Evaluate_Create_Task_Build_Factory(Entity);
	if (iTask == AI_TASKTYPE_BUILD_SHIPYARD)
		return Evaluate_Create_Task_Build_Shipyard(Entity);
	if (iTask == AI_TASKTYPE_BUILD_REFINERY)
		return Evaluate_Create_Task_Build_Refinery(Entity);
	if (iTask == AI_TASKTYPE_BUILD_EXTRACTOR)
		return Evaluate_Create_Task_Build_Extractor(Entity);
	if (iTask == AI_TASKTYPE_BUILD_UNIT)
		return Evaluate_Create_Task_Build_Unit(Entity);
	if (iTask == AI_TASKTYPE_BUILD_UNIT_BASE)
		return Evaluate_Create_Task_Build_Unit_Base(Entity);
	if (iTask == AI_TASKTYPE_BUILD_TANKER_A)
		return Evaluate_Create_Task_Build_Tanker_A(Entity);

	return 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAIEntity::Compute_Renege_Threshold(cAIModeStack Stack_A, cAIModeStack Stack_B)
{
	

	return 0.700;	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAIEntity::Compute_RoleChange_Threshold(int iCurRole, int iNewRole)
{
	if (pAsstGive_Target != NULL) 
	{
		//temp for now, we will have to do something more sophisticated later

		return 0.700;

	}
	
	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Tasking_Unit()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	double fCurrent_Efficacy = 0;
	int iCurrTask = -1;
	double fHighest_Efficacy = 0;
	int iHighest_Task = -1;
	int iBest_Role = -1;
	cAIEntity* Best_Join_Entity = NULL;

	//if the unit is currently in a task, evaluate it's efficacy
	if (pAsstGive_Target != NULL)
	{
		fCurrent_Efficacy = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Unit_Join_Task(this, pAsstGive_Target,pAsstGive_Target->iTaskType, iRole); 
		iCurrTask = pAsstGive_Target->iTaskType;
	}

	//update its own immediacy
	if (bTaskRaised == TRUE)
	if (iTaskType >= 0)
	{
		//fImmediacy = fCurrent_Efficacy;
		fImmediacy =  pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, iTaskType);
	}

	//now check the efficacy of all the other things it can do
	//what tasks could it raise?
	Choose_Modes();
	if (Modes_Consider.Modes[0].fEfficacy > fHighest_Efficacy)
	{
		fHighest_Efficacy = Modes_Consider.Modes[0].fEfficacy;
		iHighest_Task = Modes_Consider.Modes[0].iMode;
	}	

		//DEBUG
		//pCA->pDD_Display->Write_Log("---Start---");
		//pCA->pDD_Display->WriteLog_Enter();

	//what tasks can it join?
	cAIEntity* pEnt_Request = pCA->pNationManager->ppNations[iControl]->AINew.pRequest_First->pStatusQueue_Next;
	int iCount = 0;
	while (pEnt_Request != pCA->pNationManager->ppNations[iControl]->AINew.pRequest_Last)
	{
		double val = 0;
		if (pEnt_Request->bTaskRaised == TRUE)
		{
			int iNumRoles = pEnt_Request->iTaskType / 10000; //the number of roles is embedded in the 5th digit
			//+1 because the role numbers start from 1 instead of 0
			//(0 is reserved for the hosting entity)
			for (int lc72 = 0; lc72 < iNumRoles; lc72++)
			{
				val = pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Unit_Join_Task(this, pEnt_Request, pEnt_Request->iTaskType, lc72+1); 
				if (val > fHighest_Efficacy)
				{
					fHighest_Efficacy = val;
					iHighest_Task = -1;		//the best candidate is no longer raising its own task
					iBest_Role = lc72 + 1;
					Best_Join_Entity = pEnt_Request; //it is now joining another task
				}
			}
		}
		//DEBUG
		/*char NText[50];
		itoa(iControl, NText, 10);
		pCA->pDD_Display->Write_Log("iControl: ");
		pCA->pDD_Display->Write_Log(NText);
		gcvt(val, 7, NText);
		pCA->pDD_Display->Write_Log(" val: ");
		pCA->pDD_Display->Write_Log(NText);
		pCA->pDD_Display->WriteLog_Enter();*/

		iCount++;
		if (iCount > 10)
			break;

		pEnt_Request = pEnt_Request->pStatusQueue_Next;
	}

		//DEBUG
		//pCA->pDD_Display->Write_Log("---End---");
		//pCA->pDD_Display->WriteLog_Enter();

	//now do something with this information

	//note, I know that there is a more elegant way to do this, involving
	//a "matrix" of transition states, and doing everything with arrays,
	//but it would make understanding this code for me (the infrequent programmer)
	//harder, while still in an early stage of development.
	//so I'll stick with the straightforward way for now,
	//and swap it out when I know it works

	//switch a role
	if (bTaskRaised == FALSE) //not current having a task)
	if (pAsstGive_Target != NULL)  //currently joined a task
	if (Best_Join_Entity != NULL)  //best thing to do is join a task
	if (pAsstGive_Target == Best_Join_Entity) //the task join is the current one...
	if (iBest_Role != iRole) //...but the best role is not the current one
	if (fHighest_Efficacy >  Compute_RoleChange_Threshold(iRole, iBest_Role))
	{
	//switch role here
	}

	//raise a task
	if (bTaskRaised == FALSE)  //not currently having a task
	if (pAsstGive_Target == NULL) //not currently joined a task
	if (iHighest_Task >= 0)    //best thing to do is raising a task
	if (fHighest_Efficacy > AI_DECISION_THRESHOLD_RAISE_TASK)
	{
		Raise_Task(iHighest_Task, fHighest_Efficacy);
		return;
	}
	//renege a task
	if (bTaskRaised == TRUE)  //currently have a task
    if (pAsstGive_Target == NULL) //not currently joined a task
	if (iHighest_Task >= 0)   //best thing to do is raising a task
	if (iHighest_Task != iTaskType) //the task that's best is not the current one
	if (fHighest_Efficacy > Compute_Renege_Threshold(Modes, Modes_Consider))
	{
		Renege_Task(iHighest_Task, fHighest_Efficacy);
		return;
	}
	//join a task
	if (bTaskRaised == FALSE)	//not currently having a task
	if (pAsstGive_Target == NULL)	//not currently joined a task
	if (Best_Join_Entity != NULL)	//best thing to do is join a task
	if (fHighest_Efficacy > AI_DECISION_THRESHOLD_JOIN_TASK)
	{
		Join_Task(Best_Join_Entity, iBest_Role);
		return;
	}
	//switch a task
	if (bTaskRaised == FALSE)	//not currently having a task
	if (pAsstGive_Target != NULL)  //currently joined a task
	if (Best_Join_Entity != NULL)  //best thing to do is join a task
	if (pAsstGive_Target != Best_Join_Entity) //the task join is not the current one
	if (fHighest_Efficacy > AI_DECISION_THRESHOLD_SWITCH_TASK)
	{
		Switch_Task(pAsstGive_Target, Best_Join_Entity, iBest_Role);
		return;
	}
	//join->host
	if (bTaskRaised == FALSE) //not currently having a task
	if (pAsstGive_Target != NULL) //currently joined a task
	if (iHighest_Task >= 0)	//best thing to do is raise a task
	if (fHighest_Efficacy > AI_DECISION_THRESHOLD_JOIN_TO_HOST)
	{
		Leave_Task();
		Raise_Task(iHighest_Task, fHighest_Efficacy);
		return;
	}
	//host->join
	if (bTaskRaised == TRUE)  //current having a task
	if (pAsstGive_Target == NULL) //not currently joined a task
	if (Best_Join_Entity != NULL) //best thing to do is join a task
	if (fHighest_Efficacy > AI_DECISION_THRESHOLD_HOST_TO_JOIN)
	{
		Disband_Task();
		Join_Task(Best_Join_Entity, iBest_Role);
		return;
	}
	//disband a task
	if (bTaskRaised == TRUE) //currently having a task
	if (pAsstGive_Target == NULL) //not currently joined a task
	if (fCurrent_Efficacy < AI_DECISION_THRESHOLD_DISBAND_TASK)
	{

		//DEBUG
		char NText[50];
		gcvt(fCurrent_Efficacy, 7, NText);
		pCA->pDD_Display->Write_Log("Task(unit) was disbanded because efficacy was: ");
		pCA->pDD_Display->Write_Log(NText);
		pCA->pDD_Display->WriteLog_Enter();

		Disband_Task();
		return;
	}
	//leave a task
	if (bTaskRaised == FALSE) //not currently having a task
	if (pAsstGive_Target != NULL) //currently joined a task
	if (fCurrent_Efficacy < AI_DECISION_THRESHOLD_LEAVE_TASK)
	{
		Leave_Task();
		return;
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Tasking_Struct()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iHighest_Task = -1;
	double fHighest_Efficacy = 0;
	double val = 0;
	double fCurrent_Efficacy = 0;


	//regardless of whether we are currently in a task or not,
	//we evaluate how efficacious it is to go to another task
	Choose_Modes();
	if (Modes_Consider.Modes[0].fEfficacy > fHighest_Efficacy)
	{
		fHighest_Efficacy = Modes_Consider.Modes[0].fEfficacy;
		iHighest_Task = Modes_Consider.Modes[0].iMode;
	}	

	//update its own immediacy
	if (bTaskRaised == TRUE)
	if (iTaskType >= 0)
	{
		fImmediacy =  pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, iTaskType);
		fCurrent_Efficacy = fImmediacy;
	}

	//now do things based on this information

	//raise task
	if (bTaskRaised == FALSE) 
	if (fHighest_Efficacy > AI_DECISION_THRESHOLD_RAISE_TASK)
	{
		Raise_Task(iHighest_Task, fHighest_Efficacy); //use the efficacy unmodified as the immediacy, for now
		return;
	}
	//disband task
	if (bTaskRaised == TRUE)
	if (fCurrent_Efficacy < AI_DECISION_THRESHOLD_DISBAND_TASK)
	{
		//DEBUG
		char NText[50];
		gcvt(fCurrent_Efficacy, 7, NText);
		pCA->pDD_Display->Write_Log("Task was disbanded because efficacy was: ");
		pCA->pDD_Display->Write_Log(NText);
		pCA->pDD_Display->WriteLog_Enter();

		Disband_Task();
		return;
	}
	//renege task
	if (bTaskRaised == TRUE)
	if (iHighest_Task != iTaskType)
	if (fHighest_Efficacy > Compute_Renege_Threshold(Modes, Modes_Consider))
	{
		//DEBUG
		char NText[50];
		gcvt(fHighest_Efficacy, 7, NText);
		pCA->pDD_Display->Write_Log("Task reneged because efficacy was: ");
		pCA->pDD_Display->Write_Log(NText);
		pCA->pDD_Display->WriteLog_Enter();

		Renege_Task(iHighest_Task, fHighest_Efficacy); //use the efficacy unmodified as the immediacy, for now
		return;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Tasking_Seed()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iHighest_Task = -1;
	double fHighest_Efficacy = 0;
	double val = 0;
	double fCurrent_Efficacy = 0;


	//regardless of whether we are currently in a task or not,
	//we evaluate how efficacious it is to go to another task
	Choose_Modes();
	if (Modes_Consider.Modes[0].fEfficacy > fHighest_Efficacy)
	{
		fHighest_Efficacy = Modes_Consider.Modes[0].fEfficacy;
		iHighest_Task = Modes_Consider.Modes[0].iMode;
	}	

	//update its own immediacy
	if (bTaskRaised == TRUE)
	if (iTaskType >= 0)
	{
		fImmediacy =  pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, iTaskType);
		fCurrent_Efficacy = fImmediacy;
	}

	//now do things based on this information

	//raise task
	if (bTaskRaised == FALSE) 
	if (fHighest_Efficacy > AI_DECISION_THRESHOLD_RAISE_TASK)
	{
		Raise_Task(iHighest_Task, fHighest_Efficacy); //use the efficacy unmodified as the immediacy, for now
		return;
	}
	//disband task
	if (bTaskRaised == TRUE)
	if (fCurrent_Efficacy < AI_DECISION_THRESHOLD_DISBAND_TASK)
	{
		//DEBUG
		char NText[50];
		gcvt(fCurrent_Efficacy, 7, NText);
		pCA->pDD_Display->Write_Log("Task was disbanded because efficacy was: ");
		pCA->pDD_Display->Write_Log(NText);
		pCA->pDD_Display->WriteLog_Enter();

		Disband_Task();
		return;
	}
	//renege task
	if (bTaskRaised == TRUE)
	if (iHighest_Task != iTaskType)
	if (fHighest_Efficacy > Compute_Renege_Threshold(Modes, Modes_Consider))
	{
		//DEBUG
		char NText[50];
		gcvt(fHighest_Efficacy, 7, NText);
		pCA->pDD_Display->Write_Log("Task reneged because efficacy was: ");
		pCA->pDD_Display->Write_Log(NText);
		pCA->pDD_Display->WriteLog_Enter();

		Renege_Task(iHighest_Task, fHighest_Efficacy); //use the efficacy unmodified as the immediacy, for now
		return;
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Tasking_Sector()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iHighest_Task = -1;
	double fHighest_Efficacy = 0;
	double val = 0;
	double fCurrent_Efficacy = 0;


	//regardless of whether we are currently in a task or not,
	//we evaluate how efficacious it is to go to another task
	Choose_Modes();
	if (Modes_Consider.Modes[0].fEfficacy > fHighest_Efficacy)
	{
		fHighest_Efficacy = Modes_Consider.Modes[0].fEfficacy;
		iHighest_Task = Modes_Consider.Modes[0].iMode;
	}	

	//update its own immediacy
	if (bTaskRaised == TRUE)
	if (iTaskType >= 0)
	{
		fImmediacy =  pCA->pNationManager->ppNations[iControl]->AINew.Evaluate_Create_Task(this, iTaskType);
		fCurrent_Efficacy = fImmediacy;
	}

	//now do things based on this information

	//raise task
	if (bTaskRaised == FALSE) 
	if (fHighest_Efficacy > AI_DECISION_THRESHOLD_RAISE_TASK)
	{
		Raise_Task(iHighest_Task, fHighest_Efficacy); //use the efficacy unmodified as the immediacy, for now
		return;
	}
	//disband task
	if (bTaskRaised == TRUE)
	if (fCurrent_Efficacy < AI_DECISION_THRESHOLD_DISBAND_TASK)
	{
		//DEBUG
		char NText[50];
		gcvt(fCurrent_Efficacy, 7, NText);
		pCA->pDD_Display->Write_Log("Task(sector) was disbanded because efficacy was: ");
		pCA->pDD_Display->Write_Log(NText);
		pCA->pDD_Display->WriteLog_Enter();


		Disband_Task();
		return;
	}
	//renege task
	if (bTaskRaised == TRUE)
	if (iHighest_Task != iTaskType)
	if (fHighest_Efficacy > Compute_Renege_Threshold(Modes, Modes_Consider))
	{
		Renege_Task(iHighest_Task, fHighest_Efficacy); //use the efficacy unmodified as the immediacy, for now
		return;
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Raise_Task(int iTaskType_, double fImmediacy_)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	bTaskRaised = TRUE;
	iTaskType = iTaskType_;
	fImmediacy = fImmediacy_;
	//reset role numbers (since we know nobody should be in the task at this point
	Reset_RoleNums();
	iRole = 0; //
	//reset task step
	HostInfo.iTaskStep = 0;

	if (iTaskType == AI_TASKTYPE_ATTACK_SECTOR)
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_ATTACK_SECTOR");
//	if (iTask == AI_TASKTYPE_DEVELOP_SECTOR)
//		return Evaluate_Unit_Join_Task_Develop_Sector(Entity, Entity_Join);
	if (iTaskType == AI_TASKTYPE_DEFEND_SECTOR)
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_DEFEND_SECTOR");
	if (iTaskType == AI_TASKTYPE_EXPLORE_SECTOR)
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_EXPLORE_SECTOR");
	if (iTaskType == AI_TASKTYPE_EXPLORE_SECTOR)
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_EXPLORE_SECTOR");
	if (iTaskType == AI_TASKTYPE_ESTABLISH_OUTPOST)
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_ESTABLISH_OUTPOST");
	if (iTaskType == AI_TASKTYPE_ESTABLISH_CAPITAL)
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_ESTABLISH_CAPITAL");
	if (iTaskType == AI_TASKTYPE_ATTACK_SEED)
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_ATTACK_SEED");
	if (iTaskType == AI_TASKTYPE_DEFEND_SEED)
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_DEFEND_SEED");
//	if (iTask == AI_TASKTYPE_DEVELOP_SEED)
//		return Evaluate_Unit_Join_Task_Develop_Seed(Entity, Entity_Join);
	if (iTaskType == AI_TASKTYPE_BUILD_STORAGE)
	{
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_BUILD_STORAGE");
	}
	if (iTaskType == AI_TASKTYPE_BUILD_FORGE)
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_BUILD_FORGE");
	if (iTaskType == AI_TASKTYPE_BUILD_FACTORY)
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_BUILD_FACTORY");
	if (iTaskType == AI_TASKTYPE_BUILD_SHIPYARD)
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_BUILD_SHIPYARD");
	if (iTaskType == AI_TASKTYPE_BUILD_REFINERY)
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_BUILD_REFINERY");
	if (iTaskType == AI_TASKTYPE_BUILD_EXTRACTOR)
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_BUILD_EXTRACTOR");
	if (iTaskType == AI_TASKTYPE_BUILD_UNIT_BASE)
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_BUILD_UNIT_BASE");
	if (iTaskType == AI_TASKTYPE_BUILD_TANKER_A)
		pCA->pDD_Display->Write_Log("Created Task AI_TASKTYPE_BUILD_TANKER_A");



	char NText[50];
	itoa(pCA->pTimer->iCurrTick, NText, 10);
	pCA->pDD_Display->Write_Log(" Tick: ");
	pCA->pDD_Display->Write_Log(NText);

		itoa(iID, NText, 10);
	pCA->pDD_Display->Write_Log(" ID: ");
	pCA->pDD_Display->Write_Log(NText);

	pCA->pDD_Display->WriteLog_Enter();



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Disband_Task()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	if (iTaskType == AI_TASKTYPE_ATTACK_SECTOR)
		pCA->pDD_Display->Write_Log("Disbanded Task AI_TASKTYPE_ATTACK_SECTOR");
//	if (iTask == AI_TASKTYPE_DEVELOP_SECTOR)
//		return Evaluate_Unit_Join_Task_Develop_Sector(Entity, Entity_Join);
	if (iTaskType == AI_TASKTYPE_DEFEND_SECTOR)
		pCA->pDD_Display->Write_Log("Disbanded Task AI_TASKTYPE_DEFEND_SECTOR");
	if (iTaskType == AI_TASKTYPE_EXPLORE_SECTOR)
		pCA->pDD_Display->Write_Log("Disbanded Task AI_TASKTYPE_EXPLORE_SECTOR");
	if (iTaskType == AI_TASKTYPE_ESTABLISH_CAPITAL)
		pCA->pDD_Display->Write_Log("Disbanded Task AI_TASKTYPE_ESTABLISH_CAPITAL");
	if (iTaskType == AI_TASKTYPE_ESTABLISH_OUTPOST)
		pCA->pDD_Display->Write_Log("Disbanded Task AI_TASKTYPE_ESTABLISH_OUTPOST");
	if (iTaskType == AI_TASKTYPE_ATTACK_SEED)
		pCA->pDD_Display->Write_Log("Disbanded Task AI_TASKTYPE_ATTACK_SEED");
	if (iTaskType == AI_TASKTYPE_DEFEND_SEED)
		pCA->pDD_Display->Write_Log("Disbanded Task AI_TASKTYPE_DEFEND_SEED");
//	if (iTask == AI_TASKTYPE_DEVELOP_SEED)
//		return Evaluate_Unit_Join_Task_Develop_Seed(Entity, Entity_Join);
	if (iTaskType == AI_TASKTYPE_BUILD_STORAGE)
		pCA->pDD_Display->Write_Log("Disbanded Task AI_TASKTYPE_BUILD_STORAGE");
	if (iTaskType == AI_TASKTYPE_BUILD_FORGE)
		pCA->pDD_Display->Write_Log("Disbanded Task AI_TASKTYPE_BUILD_FORGE");
	if (iTaskType == AI_TASKTYPE_BUILD_FACTORY)
		pCA->pDD_Display->Write_Log("Disbanded Task AI_TASKTYPE_BUILD_FACTORY");
	if (iTaskType == AI_TASKTYPE_BUILD_SHIPYARD)
		pCA->pDD_Display->Write_Log("Disbanded Task AI_TASKTYPE_BUILD_SHIPYARD");
	if (iTaskType == AI_TASKTYPE_BUILD_REFINERY)
		pCA->pDD_Display->Write_Log("Disbanded Task AI_TASKTYPE_BUILD_REFINERY");
	if (iTaskType == AI_TASKTYPE_BUILD_EXTRACTOR)
		pCA->pDD_Display->Write_Log("Disbanded Task AI_TASKTYPE_BUILD_EXTRACTOR");
		if (iTaskType == AI_TASKTYPE_BUILD_UNIT_BASE)
		pCA->pDD_Display->Write_Log("Disbanded Task AI_TASKTYPE_BUILD_UNIT_BASE");
	if (iTaskType == AI_TASKTYPE_BUILD_TANKER_A)
		pCA->pDD_Display->Write_Log("Disbanded Task AI_TASKTYPE_BUILD_TANKER_A");
	char NText[50];
	itoa(pCA->pTimer->iCurrTick, NText, 10);
	pCA->pDD_Display->Write_Log(" Tick: ");
	pCA->pDD_Display->Write_Log(NText);

	itoa(iID, NText, 10);
	pCA->pDD_Display->Write_Log(" ID: ");
	pCA->pDD_Display->Write_Log(NText);

	pCA->pDD_Display->WriteLog_Enter();

	//disband all assistants
	Clear_Assistants();


	bTaskRaised = FALSE;
	iTaskType = -1;
	fImmediacy = 0;
	//reset more stuff
	HostInfo.pStructure = NULL;
	HostInfo.iTaskStep = 0;
	HostInfo.iCommenceTick = 0;
	HostInfo.MapTileLocation.index = -1;

	//reset role numbers (since we know nobody should be in the task at this point
	Reset_RoleNums();
	iRole = -1;  //it's own role is invalid

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Join_Task(cAIEntity* pEnt_Join, int iRole_)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	Set_Asst(pEnt_Join);

	iTaskType = pEnt_Join->iTaskType;
	iRole = iRole_; //no role defined yet
	//report itself to target
	pEnt_Join->arrNumInRole[iRole]++;

	

	if (iTaskType == AI_TASKTYPE_ATTACK_SECTOR)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_ATTACK_SECTOR");
//	if (iTask == AI_TASKTYPE_DEVELOP_SECTOR)
//		return Evaluate_Unit_Join_Task_Develop_Sector(Entity, Entity_Join);
	if (iTaskType == AI_TASKTYPE_DEFEND_SECTOR)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_DEFEND_SECTOR");
	if (iTaskType == AI_TASKTYPE_EXPLORE_SECTOR)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_EXPLORE_SECTOR");
	if (iTaskType == AI_TASKTYPE_ESTABLISH_CAPITAL)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_ESTABLISH_CAPITAL");
	if (iTaskType == AI_TASKTYPE_ESTABLISH_OUTPOST)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_ESTABLISH_OUTPOST");
	if (iTaskType == AI_TASKTYPE_ATTACK_SEED)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_ATTACK_SEED");
	if (iTaskType == AI_TASKTYPE_DEFEND_SEED)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_DEFEND_SEED");
//	if (iTask == AI_TASKTYPE_DEVELOP_SEED)
//		return Evaluate_Unit_Join_Task_Develop_Seed(Entity, Entity_Join);
	if (iTaskType == AI_TASKTYPE_BUILD_STORAGE)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_BUILD_STORAGE");
	if (iTaskType == AI_TASKTYPE_BUILD_FORGE)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_BUILD_FORGE");
	if (iTaskType == AI_TASKTYPE_BUILD_FACTORY)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_BUILD_FACTORY");
	if (iTaskType == AI_TASKTYPE_BUILD_SHIPYARD)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_BUILD_SHIPYARD");
	if (iTaskType == AI_TASKTYPE_BUILD_REFINERY)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_BUILD_REFINERY");
	if (iTaskType == AI_TASKTYPE_BUILD_EXTRACTOR)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_BUILD_EXTRACTOR");
	if (iTaskType == AI_TASKTYPE_BUILD_UNIT_BASE)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_BUILD_UNIT_BASE");
	if (iTaskType == AI_TASKTYPE_BUILD_TANKER_A)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_BUILD_TANKER_A");

	char NText[50];
	itoa(pCA->pTimer->iCurrTick, NText, 10);
	pCA->pDD_Display->Write_Log(" Tick: ");
	pCA->pDD_Display->Write_Log(NText);

	itoa(iID, NText, 10);
	pCA->pDD_Display->Write_Log(" ID: ");
	pCA->pDD_Display->Write_Log(NText);

	itoa(iRole, NText, 10);
	pCA->pDD_Display->Write_Log(" Role: ");
	pCA->pDD_Display->Write_Log(NText);


	pCA->pDD_Display->WriteLog_Enter();
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Leave_Task()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	if (iTaskType == AI_TASKTYPE_ATTACK_SECTOR)
		pCA->pDD_Display->Write_Log("Left Test AI_TASKTYPE_ATTACK_SECTOR");
//	if (iTask == AI_TASKTYPE_DEVELOP_SECTOR)
//		return Evaluate_Unit_Join_Task_Develop_Sector(Entity, Entity_Join);
	if (iTaskType == AI_TASKTYPE_DEFEND_SECTOR)
		pCA->pDD_Display->Write_Log("Left Task AI_TASKTYPE_DEFEND_SECTOR");
	if (iTaskType == AI_TASKTYPE_EXPLORE_SECTOR)
		pCA->pDD_Display->Write_Log("Left Task AI_TASKTYPE_EXPLORE_SECTOR");
	if (iTaskType == AI_TASKTYPE_ESTABLISH_CAPITAL)
		pCA->pDD_Display->Write_Log("Left Task AI_TASKTYPE_ESTABLISH_CAPITAL");
	if (iTaskType == AI_TASKTYPE_ESTABLISH_OUTPOST)
		pCA->pDD_Display->Write_Log("Left Task AI_TASKTYPE_ESTABLISH_OUTPOST");
	if (iTaskType == AI_TASKTYPE_ATTACK_SEED)
		pCA->pDD_Display->Write_Log("Left Task AI_TASKTYPE_ATTACK_SEED");
	if (iTaskType == AI_TASKTYPE_DEFEND_SEED)
		pCA->pDD_Display->Write_Log("Left Task AI_TASKTYPE_DEFEND_SEED");
//	if (iTask == AI_TASKTYPE_DEVELOP_SEED)
//		return Evaluate_Unit_Join_Task_Develop_Seed(Entity, Entity_Join);
	if (iTaskType == AI_TASKTYPE_BUILD_STORAGE)
		pCA->pDD_Display->Write_Log("Left Task AI_TASKTYPE_BUILD_STORAGE");
	if (iTaskType == AI_TASKTYPE_BUILD_FORGE)
		pCA->pDD_Display->Write_Log("Left Task AI_TASKTYPE_BUILD_FORGE");
	if (iTaskType == AI_TASKTYPE_BUILD_FACTORY)
		pCA->pDD_Display->Write_Log("Left Task AI_TASKTYPE_BUILD_FACTORY");
	if (iTaskType == AI_TASKTYPE_BUILD_SHIPYARD)
		pCA->pDD_Display->Write_Log("Joined Task AI_TASKTYPE_BUILD_SHIPYARD");
	if (iTaskType == AI_TASKTYPE_BUILD_REFINERY)
		pCA->pDD_Display->Write_Log("Left Task AI_TASKTYPE_BUILD_REFINERY");
	if (iTaskType == AI_TASKTYPE_BUILD_EXTRACTOR)
		pCA->pDD_Display->Write_Log("Left Task AI_TASKTYPE_BUILD_EXTRACTOR");
	if (iTaskType == AI_TASKTYPE_BUILD_TANKER_A)
		pCA->pDD_Display->Write_Log("Left Task AI_TASKTYPE_BUILD_TANKER_A");
	char NText[50];
	itoa(pCA->pTimer->iCurrTick, NText, 10);
	pCA->pDD_Display->Write_Log(" Tick: ");
	pCA->pDD_Display->Write_Log(NText);

	itoa(iID, NText, 10);
	pCA->pDD_Display->Write_Log(" ID: ");
	pCA->pDD_Display->Write_Log(NText);

	pCA->pDD_Display->WriteLog_Enter();


	Clear_Assisting();
	iTaskType = -1;
	iRole = -1; //no role because no task

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Switch_Task(cAIEntity* pEnt_Leave, cAIEntity* pEnt_Switch, int iRole)
{
	Leave_Task();
	Join_Task(pEnt_Switch, iRole);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Renege_Task(int iTaskType_, double fImmediacy_)
{
	Disband_Task();
	Raise_Task(iTaskType_, fImmediacy_);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAIEntity::Execute()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	pCA->pNationManager->ppNations[iControl]->AINew.Execute_Task(this, iTaskType, iRole);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Attack_Sector(cAIEntity* Entity)
{

	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task_Attack_Sector(cAIEntity* Entity, cAIEntity* Ent_Sector, int iRoleNum)
{

	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task_Explore_Seed(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum)
{
	return 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Establish_Capital(cAIEntity* Entity)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//if we already have too many capitals to deal with, 
	if (AITopLevel.iNumCapitals > 0)
	if ((AITopLevel.iNumBuildings / AITopLevel.iNumCapitals) < 10.0)
		return 0;

	//for now, if it gets here, it can only choose between establishing a capital or a outpost, so here, just return 1.0 for now
	return 1.0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Defend_Sector(cAIEntity* Entity)
{
	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Develop_Sector(cAIEntity* Entity)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	//right now just return the sector size
	cSector* pSector = pCA->pSectorTable->Sectors[Entity->iID];

	int iBaseCheckRate = TEMP_SECTOR_TACTICAL_INFO_UPDATE_RATE_BASE + (pCA->pRandGen->rand2() % 100);

	//if sector's tactical info is out of date, update
	if (   (pCA->pTimer->iCurrTick - pSector->TacticalInfo.iLastUpdatedTick) > iBaseCheckRate )
		Update_Sector_Tactical_Info(Entity->iID);
	//init
	if (pSector->TacticalInfo.iLastUpdatedTick == 0)
		Update_Sector_Tactical_Info(Entity->iID);

	//determine base rate
	double ddRet = pSector->TacticalInfo.arrNumUnits[Entity->iControl];

	if (ddRet > 1.0)
		ddRet = 1.0;

	//now kill if there's enemy units
	int iEnemyUnits = pSector->TacticalInfo.iTotalUnits - pSector->TacticalInfo.arrNumUnits[Entity->iControl];

	//
	if (iEnemyUnits > 0)
		ddRet = 0;
	//for now, if there's no units here, cut to 0
	if (pSector->TacticalInfo.arrNumUnits[Entity->iControl] == 0)
		ddRet = 0;
	//if there are already capitals here (for now), cut to 0
	if (pSector->TacticalInfo.arrNumCapitals[Entity->iControl] > 0)
		ddRet = 0;
	//if we already have too many capitals to deal with, 
	if (AITopLevel.iNumCapitals > 0)
	if ((AITopLevel.iNumBuildings / AITopLevel.iNumCapitals) < 10.0)
		ddRet = 0;



	return ddRet;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Develop_Seed(cAIEntity* Entity)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//if it's an enemy seed, we can't develop anyway
	if (Entity->iControl != iControl)
		   return 0;

	//if there are more enemy units than friendly units in the sector, 
	//switch defend
	cBuildLocationSeed* pCapital = Entity->pSeed->pCapital;
	//
	if (pCapital != NULL)
	{
		//wow this is a lengthy reference
		MTILE* pTile = &pCA->pGameWorld->pMap[pCapital->sqLocation.index];

		//if the tile hasn't been explored yet, flag to explore
		int icheck = 1 << iControl;
		if ((pTile->barrRevealed & icheck) == 0)
			return 0;

		//don't do this for now
	//	cSector* pSector = pCA->pSectorTable->Sectors[ pTile->iSector ];
	//	double ddRatio = double(pSector->TacticalInfo.arrNumUnits[iControl]) / double(pSector->TacticalInfo.iTotalUnits); 
//		if ( ddRatio < 0.5 ) //less than half of the units in the sector are our own
//			return 0;

	}

	//if isn't any of these, pass
	//DEBUG
	//if (iControl == 1)
	//	pCA->pChatBuffer->Add_CBNode("Develop_Seed Pass", 30);

	return 1.0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Explore_Sector(cAIEntity* Entity)
{
	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Build_Unit(cAIEntity* Entity)
{	
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	//if entity is a factory, return 1.0
	cStructureA* pStruct = pCA->pStructIDTable->pTable[Entity->iID];

	if (pStruct->iType == TEMP_STYPE_FACTORY_ABC)
	if (pStruct->OrderQueue.iNumOrders == 0) //if there's nothing in the queue
		return 1.0;

	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Build_Unit_Base(cAIEntity* Entity)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	//eventually, some complex logic should go here; 
	//for now just return 0.8

	//decide on a unit type
	Entity->HostInfo.iUnitType = Compute_Build_Unit_Base_Type(Entity);

	//if we couldn't find anything to build, return 0;
	if (Entity->HostInfo.iUnitType < 0)
	{
		return 0;
	}
	return 0.9;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Build_Tanker_A(cAIEntity* Entity)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	//eventually, some complex logic should go here; 
	//for now just return 0.8
	
	//if we're the enemy, don't pass
	if (iControl != pCA->pStructIDTable->pTable[Entity->iID]->iControl)
		return 0;


	//decide on a unit type
	Entity->HostInfo.iUnitType = Find_Tanker_Type(Entity);

	//if we couldn't find anything to build, return 0;
	if (Entity->HostInfo.iUnitType < 0)
	{
		return 0;
	}
	return 1.0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task_Defend_Sector(cAIEntity* Entity, cAIEntity* Ent_Sector, int iRoleNum)
{
	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task_Develop_Sector(cAIEntity* Entity, cAIEntity* Ent_Sector, int iRoleNum)
{
	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task_Explore_Sector(cAIEntity* Entity, cAIEntity* Ent_Sector, int iRoleNum)
{
	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task_Attack_Seed(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum)
{
	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task_Build_Storage(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cUnitA* pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];
	//first seed location
	SQCOORDSET sqFirstSeed = Ent_Seed->pSeed->pSeed_First->pNextSeed->sqLocation;
	
	double ddRet = 0;

	if (pUnit->iControl != Ent_Seed->iControl)
		return 0;

	//if trying out for role 1 (the builder itself)
	if (iRoleNum == 1)
	if (pUnit->pBuilder != NULL)
	{
		//if the unit has a builder, return 0.9
		ddRet = 0.7;
	
		//temper the unit's efficacy by how far it is from the site 
		//traversability can't be computed, because--we don't know where the storage will be yet!
		//(standing policy has the hosting entity deciding these details upon committing to the task--
		//if the joined units can't form a satisfactory solution, they will be kicked, or
		//efficacy will drop until the whole task is dissolved)
		double ddDist = pUnit->Calc_Distance(pUnit->Pos_sqcurr.x, pUnit->Pos_sqcurr.y, sqFirstSeed.x, sqFirstSeed.y);

		//scale against 90-tile map
		ddRet -= ddDist * (90.0 / pCA->pGameWorld->iWidth) * 0.0075;
	}

	return ddRet;
}
//////////**************************************************************************************************************************************///////////////////////
/////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task_Build_Shipyard(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cUnitA* pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];
	//first seed location
	SQCOORDSET sqFirstSeed = Ent_Seed->pSeed->pSeed_First->pNextSeed->sqLocation;
	
	double ddRet = 0;

	//if trying out for role 1 (the builder itself)
	if (iRoleNum == 1)
	if (pUnit->pBuilder != NULL)
	{
		//if the unit has a builder, return 0.9
		ddRet = 0.7;
	
		//temper the unit's efficacy by how far it is from the site 
		//traversability can't be computed, because--we don't know where the storage will be yet!
		//(standing policy has the hosting entity deciding these details upon committing to the task--
		//if the joined units can't form a satisfactory solution, they will be kicked, or
		//efficacy will drop until the whole task is dissolved)
		double ddDist = pUnit->Calc_Distance(pUnit->Pos_sqcurr.x, pUnit->Pos_sqcurr.y, sqFirstSeed.x, sqFirstSeed.y);

		//scale against 90-tile map
		ddRet -= ddDist * (90.0 / pCA->pGameWorld->iWidth) * 0.0075;
	}

	return ddRet;

}
//////////**************************************************************************************************************************************///////////////////////
/////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task_Build_Forge(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cUnitA* pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];
	//first seed location
	SQCOORDSET sqFirstSeed = Ent_Seed->pSeed->pSeed_First->pNextSeed->sqLocation;
	
	double ddRet = 0;

	//if trying out for role 1 (the builder itself)
	if (iRoleNum == 1)
	if (pUnit->pBuilder != NULL)
	{
		//if the unit has a builder, return 0.9
		ddRet = 0.7;
	
		//temper the unit's efficacy by how far it is from the site 
		//traversability can't be computed, because--we don't know where the storage will be yet!
		//(standing policy has the hosting entity deciding these details upon committing to the task--
		//if the joined units can't form a satisfactory solution, they will be kicked, or
		//efficacy will drop until the whole task is dissolved)
		double ddDist = pUnit->Calc_Distance(pUnit->Pos_sqcurr.x, pUnit->Pos_sqcurr.y, sqFirstSeed.x, sqFirstSeed.y);

		//scale against 90-tile map
		ddRet -= ddDist * (90.0 / pCA->pGameWorld->iWidth) * 0.0075;
	}

	return ddRet;
}
//////////**************************************************************************************************************************************///////////////////////
/////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task_Build_Refinery(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cUnitA* pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];
	//first seed location
	SQCOORDSET sqFirstSeed = Ent_Seed->pSeed->pSeed_First->pNextSeed->sqLocation;
	
	double ddRet = 0;

	//if trying out for role 1 (the builder itself)
	if (iRoleNum == 1)
	if (pUnit->pBuilder != NULL)
	{
		//if the unit has a builder, return 0.9
		ddRet = 0.7;
	
		//temper the unit's efficacy by how far it is from the site 
		//traversability can't be computed, because--we don't know where the storage will be yet!
		//(standing policy has the hosting entity deciding these details upon committing to the task--
		//if the joined units can't form a satisfactory solution, they will be kicked, or
		//efficacy will drop until the whole task is dissolved)
		double ddDist = pUnit->Calc_Distance(pUnit->Pos_sqcurr.x, pUnit->Pos_sqcurr.y, sqFirstSeed.x, sqFirstSeed.y);

		//scale against 90-tile map
		ddRet -= ddDist * (90.0 / pCA->pGameWorld->iWidth) * 0.0075;
	}


	return ddRet;
}
//////////**************************************************************************************************************************************///////////////////////
/////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task_Build_Extractor(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cUnitA* pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];
	//first seed location
	SQCOORDSET sqFirstSeed = Ent_Seed->pSeed->pSeed_First->pNextSeed->sqLocation;
	
	double ddRet = 0;

	//if trying out for role 1 (the builder itself)
	if (iRoleNum == 1)
	if (pUnit->pBuilder != NULL)
	{
		//if the unit has a builder, return 0.9
		ddRet = 0.7;
	
		//temper the unit's efficacy by how far it is from the site 
		//traversability can't be computed, because--we don't know where the storage will be yet!
		//(standing policy has the hosting entity deciding these details upon committing to the task--
		//if the joined units can't form a satisfactory solution, they will be kicked, or
		//efficacy will drop until the whole task is dissolved)
		double ddDist = pUnit->Calc_Distance(pUnit->Pos_sqcurr.x, pUnit->Pos_sqcurr.y, sqFirstSeed.x, sqFirstSeed.y);

		//scale against 90-tile map
		ddRet -= ddDist * (90.0 / pCA->pGameWorld->iWidth) * 0.0075;
	}

	return ddRet;
}
//////////**************************************************************************************************************************************///////////////////////
/////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task_Build_Factory(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cUnitA* pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];
	//first seed location
	SQCOORDSET sqFirstSeed = Ent_Seed->pSeed->pSeed_First->pNextSeed->sqLocation;
	
	double ddRet = 0;

	//if trying out for role 1 (the builder itself)
	if (iRoleNum == 1)
	if (pUnit->pBuilder != NULL)
	{
		//if the unit has a builder, return 0.9
		ddRet = 0.7;
	
		//temper the unit's efficacy by how far it is from the site 
		//traversability can't be computed, because--we don't know where the storage will be yet!
		//(standing policy has the hosting entity deciding these details upon committing to the task--
		//if the joined units can't form a satisfactory solution, they will be kicked, or
		//efficacy will drop until the whole task is dissolved)
		double ddDist = pUnit->Calc_Distance(pUnit->Pos_sqcurr.x, pUnit->Pos_sqcurr.y, sqFirstSeed.x, sqFirstSeed.y);

		//scale against 90-tile map
		ddRet -= ddDist * (90.0 / pCA->pGameWorld->iWidth) * 0.0075;
	}


	return ddRet;
}
//////////**************************************************************************************************************************************///////////////////////
/////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task_Defend_Seed(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum)
{
	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task_Establish_Capital(cAIEntity* Entity, cAIEntity* Ent_Sector, int iRoleNum)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cUnitA* pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];
	cSector* pSector = pCA->pSectorTable->Sectors[Ent_Sector->iID];
	MTILE* pTile = pSector->pMTList_First->pSectorList_Next;

	BOOL bReachable = pCA->pPathFinder->CanUnitTraverse(pUnit->iID, pUnit->Pos_sqcurr.index, pTile->Pos.index);

	if (iRoleNum == 1)
	{
		if (bReachable == TRUE)
		if (pUnit->pBuilder != NULL)
		{
			return 0.8;
		}

	}
	if (iRoleNum == 2)
	{
		if (bReachable == TRUE)
		if (pUnit->Storage.iMaterialType == MATERIAL_ELEMENT_S)
		if (pUnit->Storage.ddMaterialStored >= 1.0)
		{
			return 0.8;
		}
	}
	if (iRoleNum == 3)
	{
		if (bReachable == TRUE)
		if (pUnit->Storage.iMaterialType == MATERIAL_ELEMENT_L)
		if (pUnit->Storage.ddMaterialStored >= 1.0)
		{
			return 0.8;
		}
	}
	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Unit_Join_Task_Establish_Outpost(cAIEntity* Entity, cAIEntity* Ent_Sector, int iRoleNum)
{
	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Build_Storage(cAIEntity* Entity)
{
		cClassAccess* pCA = (cClassAccess*)pCAccess;
	//set to 0.6 if storage shortage found
	cBuildLocationSeedGroup* pBLSG = Entity->pSeed;

	int iNumShort = 0;

	for (int lc1 = 0; lc1 < NUM_MATERIAL_TYPES; lc1++)
	{

		if ( pBLSG->bContainsProductionFor[lc1] && !pBLSG->bContainsStorageFor[lc1] )
			iNumShort++;
	}

/*
		//DEBUG
		char NText[50];
		if (iControl == 1)
		for (int lc8 = 0; lc8 < NUM_MATERIAL_TYPES; lc8++)
		{
		//DEBUG
		if (pBLSG->bContainsProductionFor[lc8])
		{
			itoa(lc8, NText, 10);
			strcat(NText, " produce");
			pCA->pChatBuffer->Add_CBNode(NText, 10);
		}
		//DEBUG
		if (pBLSG->bContainsStorageFor[lc8])
		{
			itoa(lc8, NText, 10);
			strcat(NText, " storage");
			pCA->pChatBuffer->Add_CBNode(NText, 10);
		}
		}
*/
	if (iNumShort > 0)
	{
	//DEBUG
//	if (iControl == 1)
//		pCA->pChatBuffer->Add_CBNode("Build_Seed Pass", 30);

		return 0.8 + 0.02*iNumShort;
	}
	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Establish_Outpost(cAIEntity* Entity)
{
	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Build_Shipyard(cAIEntity* Entity)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//for the moment, short circuirt if we're already in the task and halfway through
	if (Entity->iTaskType == AI_TASKTYPE_BUILD_SHIPYARD)
	if (Entity->HostInfo.iTaskStep > 0)
		return 0.8;

	//set to 0.6 if storage shortage found
	cBuildLocationSeedGroup* pBLSG = Entity->pSeed;

	int iNumShort = 0;

	for (int lc1 = 0; lc1 < NUM_MATERIAL_TYPES; lc1++)
	{
		if ( pBLSG->bContainsProductionFor[lc1] && !pBLSG->bContainsStorageFor[lc1] )
			iNumShort++;
	}

	if (iNumShort == 0)
	if (pBLSG->bContainsShipyard == FALSE)
	if (pBLSG->bContainsProductionFor[MATERIAL_ELEMENT_S] > 0)
	{
		//Test compute a shipyard location to determine if we can even do it
//		SQCOORDSET sqRet = Compute_Shipyard_Location(Entity, NULL);

//		if (sqRet.index < 0)
//		{
			//DEBUG
//			pCA->pChatBuffer->Add_CBNode("Could not find shipyard location", 60);

//			return 0;
//		}

		return 0.8;
	}

	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Build_Forge(cAIEntity* Entity)
{
	//set to 0.6 if storage shortage found
	cBuildLocationSeedGroup* pBLSG = Entity->pSeed;

	int iNumShort = 0;

	for (int lc1 = 0; lc1 < NUM_MATERIAL_TYPES; lc1++)
	{
		if ( pBLSG->bContainsProductionFor[lc1] && !pBLSG->bContainsStorageFor[lc1] )
			iNumShort++;
	}

	if (iNumShort == 0)
	if (pBLSG->bContainsForge == FALSE)
	if (pBLSG->bContainsProductionFor[MATERIAL_ELEMENT_I] > 0)
		return 0.8;

	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Build_Refinery(cAIEntity* Entity)
{
	//for the moment, short circuirt if we're already in the task and halfway through
	if (Entity->iTaskType == AI_TASKTYPE_BUILD_REFINERY)
	if (Entity->HostInfo.iTaskStep > 0)
		return 0.8;

	//set to 0.6 if storage shortage found
	cBuildLocationSeedGroup* pBLSG = Entity->pSeed;

	int iNumShort = 0;

	for (int lc1 = 0; lc1 < NUM_MATERIAL_TYPES; lc1++)
	{
		if ( pBLSG->bContainsProductionFor[lc1] && !pBLSG->bContainsStorageFor[lc1] )
			iNumShort++;
	}

	if (iNumShort == 0)
	if (pBLSG->bContainsRefinery == FALSE)
	if (pBLSG->bContainsProductionFor[MATERIAL_ELEMENT_S] > 0)
		return 0.8;

	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Build_Extractor(cAIEntity* Entity)
{

	//for the moment, short circuirt if we're already in the task and halfway through
	if (Entity->iTaskType == AI_TASKTYPE_BUILD_EXTRACTOR)
	if (Entity->HostInfo.iTaskStep > 0)
		return 0.8;

	//set to 0.6 if storage shortage found
	cBuildLocationSeedGroup* pBLSG = Entity->pSeed;

	int iNumShort = 0;

	for (int lc1 = 0; lc1 < NUM_MATERIAL_TYPES; lc1++)
	{
		if ( pBLSG->bContainsProductionFor[lc1] && !pBLSG->bContainsStorageFor[lc1] )
			iNumShort++;
	}

	if (iNumShort == 0)
	if (pBLSG->bContainsProductionFor[MATERIAL_ELEMENT_L] == 0)
	if (pBLSG->bContainsProductionFor[MATERIAL_ELEMENT_S] > 0)
		return 0.8;

	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Build_Factory(cAIEntity* Entity)
{
	//set to 0.6 if storage shortage found
	cBuildLocationSeedGroup* pBLSG = Entity->pSeed;

	//for the moment, short circuirt if we're already in the task and halfway through
	if (Entity->iTaskType == AI_TASKTYPE_BUILD_FACTORY)
	if (Entity->HostInfo.iTaskStep > 0)
		return 0.8;

	if (pBLSG->bContainsFactory == FALSE)
	if (pBLSG->bContainsProductionFor[MATERIAL_ELEMENT_S] > 0)
		return 0.8;

	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Explore_Seed(cAIEntity* Entity)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//if there are more enemy units than friendly units in the sector, 
	//switch defend
	cBuildLocationSeed* pCapital = Entity->pSeed->pCapital;
	//
	if (pCapital != NULL)
	{
		//wow this is a lengthy reference
		MTILE* pTile = &pCA->pGameWorld->pMap[pCapital->sqLocation.index];

		//if the tile hasn't been explored yet, flag to explore
		int icheck = 1 << iControl;
		if ((pTile->barrRevealed & icheck) == 0)
			return 0.2;
	}

	//if isn't any of these, fail
	return 0.0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Attack_Seed(cAIEntity* Entity)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//if it's an enemy seed
	if (Entity->iControl != iControl)
		   return 0.2;

	return 0.0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Create_Task_Defend_Seed(cAIEntity* Entity)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//if it's an enemy seed
	if (Entity->iControl != iControl)
		   return 0;

	//if there are more enemy units than friendly units in the sector, 
	//switch defend
	cBuildLocationSeed* pCapital = Entity->pSeed->pCapital;
	//
	if (pCapital != NULL)
	{
		//wow this is a lengthy reference
		MTILE* pTile = &pCA->pGameWorld->pMap[pCapital->sqLocation.index];

		//if the tile hasn't been explored yet, flag to explore
		int icheck = 1 << iControl;
		if ((pTile->barrRevealed & icheck) == 0)
			return 0;

		cSector* pSector = pCA->pSectorTable->Sectors[ pTile->iSector ];
		double ddRatio = double(pSector->TacticalInfo.arrNumUnits[iControl]) / double(pSector->TacticalInfo.iTotalUnits); 
		if ( ddRatio < 0.5 ) //less than half of the units in the sector are our own
			return 0.2;

	}

	//if isn't any of these, fail
	return 0.0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Execute_Task(cAIEntity* Entity, int iTaskType, int iRole)
{
	if (iTaskType == AI_TASKTYPE_ESTABLISH_CAPITAL)
	{
		if (iRole == 0)
			Execute_Task_Establish_Capital_Role0(Entity);
		if (iRole == 1)
			Execute_Task_Establish_Capital_Role1(Entity);
		if (iRole == 2)
			Execute_Task_Establish_Capital_Role2(Entity);
		if (iRole == 3)
			Execute_Task_Establish_Capital_Role3(Entity);

		return;
	}
	if (iTaskType == AI_TASKTYPE_BUILD_STORAGE)
	{
		if (iRole == 0)
			Execute_Task_Build_Storage_Role0(Entity);
		if (iRole == 1)
			Execute_Task_Build_Storage_Role1(Entity);
		return;
	}
	if (iTaskType == AI_TASKTYPE_BUILD_FORGE)
	{
		if (iRole == 0)
			Execute_Task_Build_Forge_Role0(Entity);
		if (iRole == 1)
			Execute_Task_Build_Forge_Role1(Entity);
		return;
	}
	if (iTaskType == AI_TASKTYPE_BUILD_REFINERY)
	{
		if (iRole == 0)
			Execute_Task_Build_Refinery_Role0(Entity);
		if (iRole == 1)
			Execute_Task_Build_Refinery_Role1(Entity);
		return;
	}
	if (iTaskType == AI_TASKTYPE_BUILD_FACTORY)
	{
		if (iRole == 0)
			Execute_Task_Build_Factory_Role0(Entity);
		if (iRole == 1)
			Execute_Task_Build_Factory_Role1(Entity);
		return;
	}
	if (iTaskType == AI_TASKTYPE_BUILD_SHIPYARD)
	{
		if (iRole == 0)
			Execute_Task_Build_Shipyard_Role0(Entity);
		if (iRole == 1)
			Execute_Task_Build_Shipyard_Role1(Entity);
		return;
	}


	if (iTaskType == AI_TASKTYPE_BUILD_UNIT_BASE)
	{
		if (iRole == 0)
			Execute_Task_Build_Unit_Base_Role0(Entity);
		return;
	}

	if (iTaskType == AI_TASKTYPE_BUILD_TANKER_A)
	{
		if (iRole == 0)
			Execute_Task_Build_Tanker_A_Role0(Entity);
		return;
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//Hosting Entity Role
void cAINew::Execute_Task_Establish_Capital_Role0(cAIEntity* Entity)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//initial step (decide where to put the capital)
	if (Entity->HostInfo.iTaskStep == 0)
	{
		//set maptile location
		Entity->HostInfo.MapTileLocation.index = -1;
		Entity->HostInfo.MapTileLocation = Compute_Capital_Location(Entity->iID);
		Entity->HostInfo.pStructure = NULL;

 		if (Entity->HostInfo.MapTileLocation.index >= 0)
			Entity->HostInfo.iTaskStep = 1;
	}
	//next step (wait for all needed players)
	if (Entity->HostInfo.iTaskStep == 1)
	{
		//check for presence of necessary units
		if (Entity->arrNumInRole[1] == 1)
		if (Entity->arrNumInRole[2] > 0)
		if (Entity->arrNumInRole[3] > 0)
		{
			Entity->HostInfo.iTaskStep = 2;
			//start count
			Entity->HostInfo.iCommenceTick = pCA->pTimer->iCurrTick;
		}
	}
	if (Entity->HostInfo.iTaskStep == 2)
	{
		//this is where we check whether the units have finished building
		if (Entity->HostInfo.pStructure != NULL)
		if (Entity->HostInfo.pStructure->fCompletion >= 1.0)
			Entity->HostInfo.iTaskStep = 3;

	}
	if (Entity->HostInfo.iTaskStep == 3)
	{
		//disband
		Entity->Disband_Task();
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//Construction Vehicle Role
void cAINew::Execute_Task_Establish_Capital_Role1(cAIEntity* Entity)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cAIEntity* pTarget = Entity->pAsstGive_Target;

	if (pTarget->HostInfo.iTaskStep == 2)
	{
		cUnitA* pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];

		if (pUnit->pBuildInfo != NULL)
		if (pUnit->pBuildInfo->iStructureID >= 0)
			pTarget->HostInfo.pStructure  = pCA->pStructIDTable->pTable[pUnit->pBuildInfo->iStructureID];

		BOOL bIssue = FALSE;

		if (pUnit->pBuildInfo == NULL)
			bIssue = TRUE;
		if (pUnit->pBuildInfo != NULL)
		if (pUnit->pBuildInfo->Pos_build.index != pTarget->HostInfo.MapTileLocation.index)
			bIssue = TRUE;
		
        if (bIssue == TRUE)
		{
			int iretsize = 0;
			BYTE* pData;
			pData = pCA->pCommQueue->Assemble_CommData_Build_Structure_ONE(pCA->pTimer->iCurrTick + 1, Entity->iID, TEMP_STYPE_RSC, pTarget->HostInfo.MapTileLocation.index, &iretsize);  

			pCA->pCommQueue->Issue_Command_Q2(CQ2TYPE_BUILD_STRUCTURE, pData, iretsize, FALSE);
			
		}
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//Tanker role
void cAINew::Execute_Task_Establish_Capital_Role2(cAIEntity* Entity)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cAIEntity* pTarget = Entity->pAsstGive_Target;

	int iMod = pTarget->HostInfo.MapTileLocation.index;

	if (pTarget->HostInfo.iTaskStep == 2)
	{
		cUnitA* pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];

		BOOL bIssue = FALSE;

		if (pUnit->Pos_sqtarget.index != pTarget->HostInfo.MapTileLocation.index )
			bIssue = TRUE;

		if ( pTarget->HostInfo.pStructure != NULL)
		if ( pTarget->HostInfo.pStructure->fCompletion < 0.1)
        if (  ((pCA->pTimer->iCurrTick / 100) % 2) == 0)
		if ( (pCA->pTimer->iCurrTick - pTarget->HostInfo.iCommenceTick) > 300)
		{
			bIssue = TRUE;
			iMod = pTarget->HostInfo.MapTileLocation.index + (pCA->pRandGen->rand2() % 5) - 2;
		}
		
        if (bIssue == TRUE)
		{
			int iretsize = 0;
			BYTE* pData;
			pData = pCA->pCommQueue->Assemble_CommData_Move_B_Single(pCA->pTimer->iCurrTick + 1, Entity->iID, iMod, &iretsize);  

			pCA->pCommQueue->Issue_Command_Q2(CQ2TYPE_MOVE_B, pData, iretsize, FALSE);
		}

	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Execute_Task_Establish_Capital_Role3(cAIEntity* Entity) //Tanker (Silicon) role
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cAIEntity* pTarget = Entity->pAsstGive_Target;

	int iMod = pTarget->HostInfo.MapTileLocation.index;

	if (pTarget->HostInfo.iTaskStep == 2)
	{
		cUnitA* pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];

		BOOL bIssue = FALSE;

		if (pUnit->Pos_sqtarget.index != pTarget->HostInfo.MapTileLocation.index )
			bIssue = TRUE;

		if ( pTarget->HostInfo.pStructure != NULL)
		if ( pTarget->HostInfo.pStructure->fCompletion < 0.1)
        if (  ((pCA->pTimer->iCurrTick / 100) % 2) == 0)
		if ( (pCA->pTimer->iCurrTick - pTarget->HostInfo.iCommenceTick) > 300)
		{
			bIssue = TRUE;
			iMod = pTarget->HostInfo.MapTileLocation.index + (pCA->pRandGen->rand2() % 5) - 2;
		}
		
        if (bIssue == TRUE)
		{
			int iretsize = 0;
			BYTE* pData;
			pData = pCA->pCommQueue->Assemble_CommData_Move_B_Single(pCA->pTimer->iCurrTick + 1, Entity->iID, iMod, &iretsize);  

			pCA->pCommQueue->Issue_Command_Q2(CQ2TYPE_MOVE_B, pData, iretsize, FALSE);
		}

	}	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Execute_Task_Build_Factory_Role0(cAIEntity* Entity) //Hosting Entity Role
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//initial step (initialize)
	if (Entity->HostInfo.iTaskStep == 0)
	{
		Entity->HostInfo.pStructure = NULL;
		Entity->HostInfo.MapTileLocation.index = -1;
		Entity->HostInfo.iTaskStep = 1;
	}
	//next step (wait for all needed players)
	if (Entity->HostInfo.iTaskStep == 1)
	{
		//check for presence of necessary units
		if (Entity->arrNumInRole[1] == 1)
		{
			if (Entity->HostInfo.MapTileLocation.index < 0)
			{
			//attempt to decide where to put the storage 
			//based on the unit that signed up
			cAIEntity* pFirstEntity = Entity->pAsstRecv_Head;
			SQCOORDSET sqCheck = Compute_Factory_Location(Entity, pFirstEntity);
				//if we couldn't find anything (negative index) kick the unit and start over
				//although this really risks the unit pair getting join/reject/join/reject loop,
				//I'm not sure I can do anything other than specify some kind of memory
				//that remembers the last task a unit was rejected from (not implementing that yet)
				if (sqCheck.index < 0)
				{		
					Entity->Clear_Assistants();
					Entity->HostInfo.iTaskStep = 0;
					return;
				}
				Entity->HostInfo.MapTileLocation = sqCheck;
			}
			Entity->HostInfo.iTaskStep = 2;
			//link from first seed (I don't really know what to do about this...)
			pCA->pGameWorld->Link_Point_to_Point(Entity->pSeed->pSeed_First->pNextSeed->sqLocation.index, Entity->HostInfo.MapTileLocation.index);
			//start count
			Entity->HostInfo.iCommenceTick = pCA->pTimer->iCurrTick;
		}
	}
	if (Entity->HostInfo.iTaskStep == 2)
	{
		//this is where we check whether the units have finished building
		if (Entity->HostInfo.pStructure != NULL)
		{
			if (Entity->HostInfo.pStructure->fCompletion >= 1.0)
			{
				Entity->HostInfo.iTaskStep = 3;
			}
		}
	}
	if (Entity->HostInfo.iTaskStep == 3)
	{

		//disband
		Entity->Disband_Task();
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Execute_Task_Build_Factory_Role1(cAIEntity* Entity) //Construction Vehicle Role
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cAIEntity* pTarget = Entity->pAsstGive_Target;

	if (pTarget->HostInfo.iTaskStep == 2)
	{
		cUnitA* pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];


		BOOL bIssue = TRUE;

		if (pUnit->pBuildInfo != NULL)
		if (pUnit->pBuildInfo->iBuildType == TEMP_STYPE_FACTORY_ABC)
		if (pUnit->pBuildInfo->Pos_build.index == pTarget->HostInfo.MapTileLocation.index)
		{
			if (pUnit->pBuildInfo->iStructureID >= 0)
				pTarget->HostInfo.pStructure  = pCA->pStructIDTable->pTable[pUnit->pBuildInfo->iStructureID];
			bIssue = FALSE;
		}	
		
        if (bIssue == TRUE)
		{

			int iretsize = 0;
			BYTE* pData;
			pData = pCA->pCommQueue->Assemble_CommData_Build_Structure_ONE(pCA->pTimer->iCurrTick + 1, Entity->iID, TEMP_STYPE_FACTORY_ABC, pTarget->HostInfo.MapTileLocation.index, &iretsize);  

			pCA->pCommQueue->Issue_Command_Q2(CQ2TYPE_BUILD_STRUCTURE, pData, iretsize, FALSE);
			
		}
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Execute_Task_Build_Refinery_Role0(cAIEntity* Entity) //Hosting Entity Role
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//initial step (initialize)
	if (Entity->HostInfo.iTaskStep == 0)
	{
		Entity->HostInfo.pStructure = NULL;
		Entity->HostInfo.MapTileLocation.index = -1;
		Entity->HostInfo.iTaskStep = 1;
	}
	//next step (wait for all needed players)
	if (Entity->HostInfo.iTaskStep == 1)
	{
		//check for presence of necessary units
		if (Entity->arrNumInRole[1] == 1)
		{
			if (Entity->HostInfo.MapTileLocation.index < 0)
			{
			//attempt to decide where to put the storage 
			//based on the unit that signed up
			cAIEntity* pFirstEntity = Entity->pAsstRecv_Head;
			SQCOORDSET sqCheck = Compute_Refinery_Location(Entity, pFirstEntity);
				//if we couldn't find anything (negative index) kick the unit and start over
				//although this really risks the unit pair getting join/reject/join/reject loop,
				//I'm not sure I can do anything other than specify some kind of memory
				//that remembers the last task a unit was rejected from (not implementing that yet)
				if (sqCheck.index < 0)
				{		
					Entity->Clear_Assistants();
					Entity->HostInfo.iTaskStep = 0;
					return;
				}
				Entity->HostInfo.MapTileLocation = sqCheck;
			}
			Entity->HostInfo.iTaskStep = 2;
			//link from first seed (I don't really know what to do about this...)
			pCA->pGameWorld->Link_Point_to_Point(Entity->pSeed->pSeed_First->pNextSeed->sqLocation.index, Entity->HostInfo.MapTileLocation.index);
			//start count
			Entity->HostInfo.iCommenceTick = pCA->pTimer->iCurrTick;
		}
	}
	if (Entity->HostInfo.iTaskStep == 2)
	{
		//this is where we check whether the units have finished building
		if (Entity->HostInfo.pStructure != NULL)
		{
			if (Entity->HostInfo.pStructure->fCompletion >= 1.0)
			{
				Entity->HostInfo.iTaskStep = 3;
			}
		}
	}
	if (Entity->HostInfo.iTaskStep == 3)
	{

		//disband
		Entity->Disband_Task();
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Execute_Task_Build_Refinery_Role1(cAIEntity* Entity) //Construction Vehicle Role
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cAIEntity* pTarget = Entity->pAsstGive_Target;

	if (pTarget->HostInfo.iTaskStep == 2)
	{
		cUnitA* pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];


		BOOL bIssue = TRUE;

		if (pUnit->pBuildInfo != NULL)
		if (pUnit->pBuildInfo->iBuildType == TEMP_STYPE_REFINERY)
		if (pUnit->pBuildInfo->Pos_build.index == pTarget->HostInfo.MapTileLocation.index)
		{
			if (pUnit->pBuildInfo->iStructureID >= 0)
				pTarget->HostInfo.pStructure  = pCA->pStructIDTable->pTable[pUnit->pBuildInfo->iStructureID];
			bIssue = FALSE;
		}	
		
        if (bIssue == TRUE)
		{

			int iretsize = 0;
			BYTE* pData;
			pData = pCA->pCommQueue->Assemble_CommData_Build_Structure_ONE(pCA->pTimer->iCurrTick + 1, Entity->iID, TEMP_STYPE_REFINERY, pTarget->HostInfo.MapTileLocation.index, &iretsize);  

			pCA->pCommQueue->Issue_Command_Q2(CQ2TYPE_BUILD_STRUCTURE, pData, iretsize, FALSE);
			
		}
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Execute_Task_Build_Shipyard_Role0(cAIEntity* Entity) //Hosting Entity Role
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//initial step (initialize)
	if (Entity->HostInfo.iTaskStep == 0)
	{
		Entity->HostInfo.pStructure = NULL;
		Entity->HostInfo.MapTileLocation.index = -1;
		Entity->HostInfo.iTaskStep = 1;
	}
	//next step (wait for all needed players)
	if (Entity->HostInfo.iTaskStep == 1)
	{
		//check for presence of necessary units
		if (Entity->arrNumInRole[1] == 1)
		{
			if (Entity->HostInfo.MapTileLocation.index < 0)
			{
			//attempt to decide where to put the storage 
			//based on the unit that signed up
			cAIEntity* pFirstEntity = Entity->pAsstRecv_Head;
			SQCOORDSET sqCheck = Compute_Shipyard_Location(Entity, pFirstEntity);
				//if we couldn't find anything (negative index) kick the unit and start over
				//although this really risks the unit pair getting join/reject/join/reject loop,
				//I'm not sure I can do anything other than specify some kind of memory
				//that remembers the last task a unit was rejected from (not implementing that yet)
				if (sqCheck.index < 0)
				{		
					Entity->Clear_Assistants();
					Entity->HostInfo.iTaskStep = 0;
					return;
				}
				Entity->HostInfo.MapTileLocation = sqCheck;
			}
			Entity->HostInfo.iTaskStep = 2;
			//link from first seed (I don't really know what to do about this...)
			pCA->pGameWorld->Link_Point_to_Point(Entity->pSeed->pSeed_First->pNextSeed->sqLocation.index, Entity->HostInfo.MapTileLocation.index);
			//start count
			Entity->HostInfo.iCommenceTick = pCA->pTimer->iCurrTick;
		}
	}
	if (Entity->HostInfo.iTaskStep == 2)
	{
		//this is where we check whether the units have finished building
		if (Entity->HostInfo.pStructure != NULL)
		{
			if (Entity->HostInfo.pStructure->fCompletion >= 1.0)
			{
				Entity->HostInfo.iTaskStep = 3;
			}
		}
	}
	if (Entity->HostInfo.iTaskStep == 3)
	{

		//disband
		Entity->Disband_Task();
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Execute_Task_Build_Shipyard_Role1(cAIEntity* Entity) //Construction Vehicle Role
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cAIEntity* pTarget = Entity->pAsstGive_Target;

	if (pTarget->HostInfo.iTaskStep == 2)
	{
		cUnitA* pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];


		BOOL bIssue = TRUE;

		if (pUnit->pBuildInfo != NULL)
		if (pUnit->pBuildInfo->iBuildType == TEMP_STYPE_SHIPYARD)
		if (pUnit->pBuildInfo->Pos_build.index == pTarget->HostInfo.MapTileLocation.index)
		{
			if (pUnit->pBuildInfo->iStructureID >= 0)
				pTarget->HostInfo.pStructure  = pCA->pStructIDTable->pTable[pUnit->pBuildInfo->iStructureID];
			bIssue = FALSE;
		}	
		
        if (bIssue == TRUE)
		{

			int iretsize = 0;
			BYTE* pData;
			pData = pCA->pCommQueue->Assemble_CommData_Build_Structure_ONE(pCA->pTimer->iCurrTick + 1, Entity->iID, TEMP_STYPE_SHIPYARD, pTarget->HostInfo.MapTileLocation.index, &iretsize);  

			pCA->pCommQueue->Issue_Command_Q2(CQ2TYPE_BUILD_STRUCTURE, pData, iretsize, FALSE);
			
		}
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Execute_Task_Build_Forge_Role0(cAIEntity* Entity) //Hosting Entity Role
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//initial step (initialize)
	if (Entity->HostInfo.iTaskStep == 0)
	{
		Entity->HostInfo.pStructure = NULL;
		Entity->HostInfo.MapTileLocation.index = -1;
		Entity->HostInfo.iTaskStep = 1;
	}
	//next step (wait for all needed players)
	if (Entity->HostInfo.iTaskStep == 1)
	{
		//check for presence of necessary units
		if (Entity->arrNumInRole[1] == 1)
		{
			if (Entity->HostInfo.MapTileLocation.index < 0)
			{
			//attempt to decide where to put the storage 
			//based on the unit that signed up
			cAIEntity* pFirstEntity = Entity->pAsstRecv_Head;
			SQCOORDSET sqCheck = Compute_Forge_Location(Entity, pFirstEntity);
				//if we couldn't find anything (negative index) kick the unit and start over
				//although this really risks the unit pair getting join/reject/join/reject loop,
				//I'm not sure I can do anything other than specify some kind of memory
				//that remembers the last task a unit was rejected from (not implementing that yet)
				if (sqCheck.index < 0)
				{		
					Entity->Clear_Assistants();
					Entity->HostInfo.iTaskStep = 0;
					return;
				}
				Entity->HostInfo.MapTileLocation = sqCheck;
			}
			Entity->HostInfo.iTaskStep = 2;
			//link from first seed (I don't really know what to do about this...)
			pCA->pGameWorld->Link_Point_to_Point(Entity->pSeed->pSeed_First->pNextSeed->sqLocation.index, Entity->HostInfo.MapTileLocation.index);
			//start count
			Entity->HostInfo.iCommenceTick = pCA->pTimer->iCurrTick;
		}
	}
	if (Entity->HostInfo.iTaskStep == 2)
	{
		//this is where we check whether the units have finished building
		if (Entity->HostInfo.pStructure != NULL)
		{
			if (Entity->HostInfo.pStructure->fCompletion >= 1.0)
			{
				Entity->HostInfo.iTaskStep = 3;
			}
		}
	}
	if (Entity->HostInfo.iTaskStep == 3)
	{

		//disband
		Entity->Disband_Task();
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Execute_Task_Build_Forge_Role1(cAIEntity* Entity) //Construction Vehicle Role
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cAIEntity* pTarget = Entity->pAsstGive_Target;

	if (pTarget->HostInfo.iTaskStep == 2)
	{
		cUnitA* pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];


		BOOL bIssue = TRUE;

		if (pUnit->pBuildInfo != NULL)
		if (pUnit->pBuildInfo->iBuildType == TEMP_STYPE_FORGE)
		if (pUnit->pBuildInfo->Pos_build.index == pTarget->HostInfo.MapTileLocation.index)
		{
			if (pUnit->pBuildInfo->iStructureID >= 0)
				pTarget->HostInfo.pStructure  = pCA->pStructIDTable->pTable[pUnit->pBuildInfo->iStructureID];
			bIssue = FALSE;
		}	
		
        if (bIssue == TRUE)
		{

			int iretsize = 0;
			BYTE* pData;
			pData = pCA->pCommQueue->Assemble_CommData_Build_Structure_ONE(pCA->pTimer->iCurrTick + 1, Entity->iID, TEMP_STYPE_FORGE, pTarget->HostInfo.MapTileLocation.index, &iretsize);  

			pCA->pCommQueue->Issue_Command_Q2(CQ2TYPE_BUILD_STRUCTURE, pData, iretsize, FALSE);
			
		}
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Execute_Task_Build_Storage_Role0(cAIEntity* Entity) //Hosting Entity Role
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//initial step (initialize)
	if (Entity->HostInfo.iTaskStep == 0)
	{
		Entity->HostInfo.pStructure = NULL;
		Entity->HostInfo.MapTileLocation.index = -1;
		Entity->HostInfo.iTaskStep = 1;
	}
	//next step (wait for all needed players)
	if (Entity->HostInfo.iTaskStep == 1)
	{
		//check for presence of necessary units
		if (Entity->arrNumInRole[1] == 1)
		{
			if (Entity->HostInfo.MapTileLocation.index < 0)
			{
			//attempt to decide where to put the storage 
			//based on the unit that signed up
			cAIEntity* pFirstEntity = Entity->pAsstRecv_Head;
			SQCOORDSET sqCheck = Compute_Storage_Location(Entity, pFirstEntity);
				//if we couldn't find anything (negative index) kick the unit and start over
				//although this really risks the unit pair getting join/reject/join/reject loop,
				//I'm not sure I can do anything other than specify some kind of memory
				//that remembers the last task a unit was rejected from (not implementing that yet)
				if (sqCheck.index < 0)
				{		
					Entity->Clear_Assistants();
					Entity->HostInfo.iTaskStep = 0;
					return;
				}
				Entity->HostInfo.MapTileLocation = sqCheck;
			}
			Entity->HostInfo.iTaskStep = 2;
			//link from first seed (I don't really know what to do about this...)
			pCA->pGameWorld->Link_Point_to_Point(Entity->pSeed->pSeed_First->pNextSeed->sqLocation.index, Entity->HostInfo.MapTileLocation.index);
			//start count
			Entity->HostInfo.iCommenceTick = pCA->pTimer->iCurrTick;
		}
	}
	if (Entity->HostInfo.iTaskStep == 2)
	{
		//this is where we check whether the units have finished building
		if (Entity->HostInfo.pStructure != NULL)
		{
			//set the type while building
			if (Entity->HostInfo.pStructure->Storage.iMaterialType < 0)
			{			

				int iTypeToSet = Decide_Most_Urgent_Storage(Entity);
				if (iTypeToSet >= 0)
				{
					Entity->HostInfo.pStructure->Storage.iMaterialType = iTypeToSet;
				}
			}

			if (Entity->HostInfo.pStructure->fCompletion >= 1.0)
			{
				Entity->HostInfo.iTaskStep = 3;
			}
		}
	}
	if (Entity->HostInfo.iTaskStep == 3)
	{
		//disband
		Entity->Disband_Task();
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Execute_Task_Build_Storage_Role1(cAIEntity* Entity) //Construction Vehicle Role
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cAIEntity* pTarget = Entity->pAsstGive_Target;

	if (pTarget->HostInfo.iTaskStep == 2)
	{
		cUnitA* pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];


		BOOL bIssue = TRUE;

		if (pUnit->pBuildInfo != NULL)
		if (pUnit->pBuildInfo->iBuildType == TEMP_STYPE_TANK)
		if (pUnit->pBuildInfo->Pos_build.index == pTarget->HostInfo.MapTileLocation.index)
		{
			if (pUnit->pBuildInfo->iStructureID >= 0)
				pTarget->HostInfo.pStructure  = pCA->pStructIDTable->pTable[pUnit->pBuildInfo->iStructureID];
			bIssue = FALSE;
		}	
		
        if (bIssue == TRUE)
		{

			int iretsize = 0;
			BYTE* pData;
			pData = pCA->pCommQueue->Assemble_CommData_Build_Structure_ONE(pCA->pTimer->iCurrTick + 1, Entity->iID, TEMP_STYPE_TANK, pTarget->HostInfo.MapTileLocation.index, &iretsize);  

			pCA->pCommQueue->Issue_Command_Q2(CQ2TYPE_BUILD_STRUCTURE, pData, iretsize, FALSE);
			
		}
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Execute_Task_Build_Unit_Base_Role0(cAIEntity* Entity)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//initial step (initialize)
	if (Entity->HostInfo.iTaskStep == 0)
	{
		Entity->HostInfo.pStructure = NULL;
		Entity->HostInfo.MapTileLocation.index = -1;
		Entity->HostInfo.iTaskStep = 1;
	}
	//next step (dispatch the order)
	if (Entity->HostInfo.iTaskStep == 1)
	{
			int iretsize = 0;
			BYTE* pData;
			pData = pCA->pCommQueue->Assemble_CommData_Start_Build_Unit_Preconfig(pCA->pTimer->iCurrTick + 1, Entity->iID, Entity->HostInfo.iUnitType, &iretsize);  

			pCA->pCommQueue->Issue_Command_Q2(CQ2TYPE_START_BUILD_UNIT_PRECONFIG, pData, iretsize, FALSE);
			Entity->HostInfo.iTaskStep = 2;
	}
	//next step (close the order
	if (Entity->HostInfo.iTaskStep == 2)
	{
		//disband
		Entity->Disband_Task();
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Execute_Task_Build_Tanker_A_Role0(cAIEntity* Entity) //Hosting Entity Role
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//initial step (initialize)
	if (Entity->HostInfo.iTaskStep == 0)
	{
		Entity->HostInfo.pStructure = NULL;
		Entity->HostInfo.MapTileLocation.index = -1;
		Entity->HostInfo.iTaskStep = 1;
	}
	//next step (dispatch the order)
	if (Entity->HostInfo.iTaskStep == 1)
	{
			int iretsize = 0;
			BYTE* pData;
			pData = pCA->pCommQueue->Assemble_CommData_Start_Build_Unit_Preconfig(pCA->pTimer->iCurrTick + 1, Entity->iID, Entity->HostInfo.iUnitType, &iretsize);  

			pCA->pCommQueue->Issue_Command_Q2(CQ2TYPE_START_BUILD_UNIT_PRECONFIG, pData, iretsize, FALSE);
			Entity->HostInfo.iTaskStep = 2;
	}
	//next step (close the order
	if (Entity->HostInfo.iTaskStep == 2)
	{
		//disband
		Entity->Disband_Task();
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Render_Entities_All(LPDIRECTDRAWSURFACE7 pDrawSurf)
{
	cAIEntity* pTrav = pEntities_First->pNext;

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	 
	while (pTrav != pEntities_Last)
	{
		Render_Entity(pTrav, pDrawSurf);

		pTrav = pTrav->pNext;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Render_Entity(cAIEntity* Entity, LPDIRECTDRAWSURFACE7 pDrawSurf)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	DWORD dwColor;

	int iR = 0;// ((pCA->pTimer->iCurrTick + Entity->iID * 20) % 60) * 2;
	int iG = 0;//128 - iR;
	int iB = 0;//64;


    dwColor =  pCA->pColorTable->DetColor(iR/255.0, iG/255.0, iB/255.0);
	pCA->pFontInfoTable->SetFontColor(dwColor);

	int iX = 0;
	int iY = 0;

	cUnitA* pUnit;
	cStructureA* pStruct;
	char Text[50];
	ZeroMemory(Text, sizeof(char)*50);

	int iControl = -1;

		char NText[50];
		ZeroMemory(NText, sizeof(char)*50);

	//compute screen coordinates
	if (Entity->iType == AI_ENTITY_TYPE_UNIT)
	{

		pUnit = pCA->pUnitTable->pUnitTable[Entity->iID];
		iX = (pUnit->Pos_sqcurr.x - pCA->pInterface->map_xview) * TILE_SIZE + pUnit->Pos_offset.x;
		iY = (pUnit->Pos_sqcurr.y - pCA->pInterface->map_yview) * TILE_SIZE + pUnit->Pos_offset.y;

		iControl = pUnit->iControl;
	}
	if (Entity->iType == AI_ENTITY_TYPE_STRUCT)
	{

		pStruct = pCA->pStructIDTable->pTable[Entity->iID];
		iX = (pStruct->Pos_primary.x - pCA->pInterface->map_xview) * TILE_SIZE;
		iY = (pStruct->Pos_primary.y - pCA->pInterface->map_yview) * TILE_SIZE;

		iControl = pStruct->iControl;
	}



		if (iControl == Entity->iControl)
		{
			//add task
			if (Entity->iTaskType == AI_TASKTYPE_ATTACK_SECTOR)
				strcat(Text, "ATTACK_SECTOR ");
			else if (Entity->iTaskType == AI_TASKTYPE_DEFEND_SECTOR)
				strcat(Text, "DEFEND_SECTOR ");
			else if (Entity->iTaskType == AI_TASKTYPE_DEVELOP_SECTOR)
				strcat(Text, "DEVELOP_SECTOR ");
			else if (Entity->iTaskType == AI_TASKTYPE_ESTABLISH_CAPITAL)
				strcat(Text, "ESTABLISH_CAPITAL ");
			else if (Entity->iTaskType == AI_TASKTYPE_ESTABLISH_OUTPOST)
				strcat(Text, "ESTABLISH_OUTPOST ");
			else if (Entity->iTaskType == AI_TASKTYPE_ATTACK_SEED)
				strcat(Text, "ATTACK_SEED ");
			else if (Entity->iTaskType == AI_TASKTYPE_DEFEND_SEED)
				strcat(Text, "DEFEND_SEED ");
			else if (Entity->iTaskType == AI_TASKTYPE_DEVELOP_SEED)
				strcat(Text, "ATTACK_SECTOR ");
			else if (Entity->iTaskType == AI_TASKTYPE_BUILD_STORAGE)
				strcat(Text, "BUILD_STORAGE ");
			else if (Entity->iTaskType == AI_TASKTYPE_BUILD_FORGE)
				strcat(Text, "BUILD_FORGE ");
			else if (Entity->iTaskType == AI_TASKTYPE_BUILD_FACTORY)
				strcat(Text, "BUILD_FACTORY ");
			else if (Entity->iTaskType == AI_TASKTYPE_BUILD_SHIPYARD)
				strcat(Text, "BUILD_SHIPYARD ");
			else if (Entity->iTaskType == AI_TASKTYPE_BUILD_REFINERY)
				strcat(Text, "BUILD_REFINERY ");
			else if (Entity->iTaskType == AI_TASKTYPE_BUILD_EXTRACTOR)
				strcat(Text, "BUILD_EXTRACTOR ");
			else if (Entity->iTaskType == AI_TASKTYPE_BUILD_UNIT_BASE)
				strcat(Text, "BUILD_UNIT_BASE ");
			else if (Entity->iTaskType == AI_TASKTYPE_BUILD_TANKER_A)
				strcat(Text, "BUILD_TANKER_A ");
			else 
				strcat(Text, "NO TASK");

			itoa(Entity->iRole, NText, 10);
			strcat(Text, NText);
			strcat(Text, " ");
			if (Entity->pAsstGive_Target != NULL)
			{
				itoa(Entity->pAsstGive_Target->iID, NText, 10);
				strcat(Text, NText);
			}

			if (iX > 0)
			if (iY > 0)
			if (iX < pCA->pDD_Display->iXRes * MDRAW_XSCR)
			if (iY < pCA->pDD_Display->iYRes * MDRAW_YSCR)
			if (strlen(Text) > 1)
			{
				pCA->pGDisplay->Draw_Text(pDrawSurf, Text, iX, iY, FONT_STANDARD1);
			}
	
		}





}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
SQCOORDSET cAINew::Compute_Capital_Location(int iSector)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	
	//for now, just return the first node on the list
	cSector* pSector = pCA->pSectorTable->Sectors[iSector];

	//MTILE* pTrav = pSector->pMTList_First->pSectorList_Next;

	int iCount = 0;

	SQCOORDSET sqRet;
	sqRet.index = -1;

	cAILocation* pLocTrav = pLoc_Cap_First->pNext;

	while (pLocTrav != pLoc_Cap_Last)
	{
		if ( pCA->pGameWorld->pMap[pLocTrav->sqPosition.index].iSector == iSector)
		if ( pCA->pGameWorld->BigIsValidBuildLocation(pLocTrav->sqPosition.x, pLocTrav->sqPosition.y, TEMP_STYPE_RSC))
		if ( pLocTrav->ddEfficacy > 600.0)
			return pLocTrav->sqPosition;
			
		pLocTrav = pLocTrav->pNext;
	}

	return sqRet;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
SQCOORDSET cAINew::Compute_Refinery_Location(cAIEntity* pEnt_Seed, cAIEntity* pBuilderUnit)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	
	int iCount = 0;

	//the strategy here is to take the first seed location and keep picking random locations until one
	//matches the requirements

int iX = 0;
int iY = 0;
SQCOORDSET sqCheck;

SQCOORDSET sqFirstSeed;
sqFirstSeed.index = pEnt_Seed->pSeed->pSeed_First->pNextSeed->sqLocation.index;

sqFirstSeed.Calc_xy(pCA->pGameWorld->iWidth);

while (TRUE)
{
	//for now, just randomize...
	//five steps in x and y
	iX = (pCA->pRandGen->rand2() % 12) - 6;
	iY = (pCA->pRandGen->rand2() % 12) - 6;

	sqCheck.x = sqFirstSeed.x + iX*2;
	sqCheck.y = sqFirstSeed.y + iY*2;
	sqCheck.Calc_Index(pCA->pGameWorld->iWidth);

	if (pCA->pGameWorld->BigIsValidBuildLocation(sqCheck.x, sqCheck.y, TEMP_STYPE_REFINERY) == TRUE)
		break;

	iCount++;
	if (iCount > 200)
	{
		//pack up an invalid result
		sqCheck.index = -1;
		sqCheck.x = -1;
		sqCheck.y = -1;
		break;
	}

}
	return sqCheck;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
SQCOORDSET cAINew::Compute_Shipyard_Location(cAIEntity* pEnt_Seed, cAIEntity* pBuilderUnit)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	
	int iCount = 0;

	//the strategy here is to take the first seed location and keep picking random locations until one
	//matches the requirements

int iX = 0;
int iY = 0;
SQCOORDSET sqCheck;

SQCOORDSET sqFirstSeed = pEnt_Seed->pSeed->pSeed_First->pNextSeed->sqLocation;

while (TRUE)
{
	//for now, just randomize...
	//five steps in x and y
	iX = (pCA->pRandGen->rand2() % 12) - 6;
	iY = (pCA->pRandGen->rand2() % 12) - 6;

	sqCheck.x = sqFirstSeed.x + iX*2;
	sqCheck.y = sqFirstSeed.y + iY*2;
	sqCheck.Calc_Index(pCA->pGameWorld->iWidth);

	if (pCA->pGameWorld->BigIsValidBuildLocation(sqCheck.x, sqCheck.y, TEMP_STYPE_SHIPYARD) == TRUE)
		break;

	iCount++;
	if (iCount > 200)
	{
		//pack up an invalid result
		sqCheck.index = -1;
		sqCheck.x = -1;
		sqCheck.y = -1;
		break;
	}

}
	return sqCheck;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
SQCOORDSET cAINew::Compute_Forge_Location(cAIEntity* pEnt_Seed, cAIEntity* pBuilderUnit)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	
	int iCount = 0;

	//the strategy here is to take the first seed location and keep picking random locations until one
	//matches the requirements

int iX = 0;
int iY = 0;
SQCOORDSET sqCheck;

SQCOORDSET sqFirstSeed = pEnt_Seed->pSeed->pSeed_First->pNextSeed->sqLocation;

while (TRUE)
{
	//for now, just randomize...
	//five steps in x and y
	iX = (pCA->pRandGen->rand2() % 12) - 6;
	iY = (pCA->pRandGen->rand2() % 12) - 6;

	sqCheck.x = sqFirstSeed.x + iX*2;
	sqCheck.y = sqFirstSeed.y + iY*2;
	sqCheck.Calc_Index(pCA->pGameWorld->iWidth);

	if (pCA->pGameWorld->BigIsValidBuildLocation(sqCheck.x, sqCheck.y, TEMP_STYPE_FORGE) == TRUE)
		break;

	iCount++;
	if (iCount > 200)
	{
		//pack up an invalid result
		sqCheck.index = -1;
		sqCheck.x = -1;
		sqCheck.y = -1;
		break;
	}

}
	return sqCheck;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
SQCOORDSET cAINew::Compute_Factory_Location(cAIEntity* pEnt_Seed, cAIEntity* pBuilderUnit)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	
	int iCount = 0;

	//the strategy here is to take the first seed location and keep picking random locations until one
	//matches the requirements

int iX = 0;
int iY = 0;
SQCOORDSET sqCheck;

SQCOORDSET sqFirstSeed = pEnt_Seed->pSeed->pSeed_First->pNextSeed->sqLocation;

while (TRUE)
{
	//for now, just randomize...
	//five steps in x and y
	iX = (pCA->pRandGen->rand2() % 12) - 6;
	iY = (pCA->pRandGen->rand2() % 12) - 6;

	sqCheck.x = sqFirstSeed.x + iX*2;
	sqCheck.y = sqFirstSeed.y + iY*2;
	sqCheck.Calc_Index(pCA->pGameWorld->iWidth);

	if (pCA->pGameWorld->BigIsValidBuildLocation(sqCheck.x, sqCheck.y, TEMP_STYPE_FACTORY_ABC) == TRUE)
		break;

	iCount++;
	if (iCount > 200)
	{
		//pack up an invalid result
		sqCheck.index = -1;
		sqCheck.x = -1;
		sqCheck.y = -1;
		break;
	}

}
	return sqCheck;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
SQCOORDSET cAINew::Compute_Storage_Location(cAIEntity* pEnt_Seed, cAIEntity* pBuilderUnit)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	
	int iCount = 0;

	//the strategy here is to take the first seed location and keep picking random locations until one
	//matches the requirements

int iX = 0;
int iY = 0;
SQCOORDSET sqCheck;

SQCOORDSET sqFirstSeed = pEnt_Seed->pSeed->pSeed_First->pNextSeed->sqLocation;

while (TRUE)
{
	//for now, just randomize...
	//five steps in x and y
	iX = (pCA->pRandGen->rand2() % 12) - 6;
	iY = (pCA->pRandGen->rand2() % 12) - 6;

	sqCheck.x = sqFirstSeed.x + iX*2;
	sqCheck.y = sqFirstSeed.y + iY*2;
	sqCheck.Calc_Index(pCA->pGameWorld->iWidth);

	if (pCA->pGameWorld->BigIsValidBuildLocation(sqCheck.x, sqCheck.y, TEMP_STYPE_TANK) == TRUE)
		break;

	iCount++;
	if (iCount > 200)
	{
		//pack up an invalid result
		sqCheck.index = -1;
		sqCheck.x = -1;
		sqCheck.y = -1;
		break;
	}

}
	return sqCheck;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cAINew::Decide_Most_Urgent_Storage(cAIEntity* pEnt_Seed)
{

	//the function name, for now is a lie.
	//there is currently no way to determine which is the most urgent
	Update_Seed_Tactical_Info(pEnt_Seed);

	for (int lc1 = 0; lc1 < NUM_MATERIAL_TYPES; lc1++)
	{
		if (pEnt_Seed->pSeed->bContainsProductionFor[lc1] && !pEnt_Seed->pSeed->bContainsStorageFor[lc1])
			return lc1;
	}

	return -1;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Update_Sector_Tactical_Info(int iSector)
{
	int lc1;
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cSector* pSector = pCA->pSectorTable->Sectors[iSector];
	//re-init information
	pSector->TacticalInfo.iTotalUnits = 0;
	pSector->TacticalInfo.iTotalStructs = 0;
	pSector->TacticalInfo.iTotalCapitals = 0;

	for (lc1 = 0; lc1 < TEMP_DP_MAXPLAYERS; lc1++)
	{
		pSector->TacticalInfo.arrNumUnits[lc1] = 0;
		pSector->TacticalInfo.arrNumStructs[lc1] = 0;
		pSector->TacticalInfo.arrNumCapitals[lc1] = 0;
	}

	//now go through and count

	MTILE* pTrav = pSector->pMTList_First->pSectorList_Next;

	for (lc1 = 0; lc1 < pSector->iNumMapTiles; lc1++)
	{
		//count units in stack
		cUnitA* pUnit = (cUnitA*)pTrav->pOccupant_Unit;
		while (pUnit != NULL)
		{
			pSector->TacticalInfo.arrNumUnits[pUnit->iControl]++;
			pSector->TacticalInfo.iTotalUnits++;
			pUnit = (cUnitA*)pUnit->pNext_Stack;				
		}

		//add structure
		cStructureA* pStruct = (cStructureA*)pTrav->pOccupant_Structure;
		if (pStruct != NULL)
		if (pStruct->Pos_primary.index == pTrav->Pos.index)
		{
			pSector->TacticalInfo.arrNumStructs[pStruct->iControl]++;
			pSector->TacticalInfo.iTotalStructs++;
			if (pStruct->iType == TEMP_STYPE_RSC)
			{
				pSector->TacticalInfo.arrNumCapitals[pStruct->iControl]++;
				pSector->TacticalInfo.iTotalCapitals++;
			}
		}

		pTrav = pTrav->pSectorList_Next;
	}

	//now update tick
	pSector->TacticalInfo.iLastUpdatedTick = pCA->pTimer->iCurrTick;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cBuildLocationSeedGroup::Clear_Gather_Info()
{
	//initial values
	bContainsCapital = FALSE;
	bContainsForge = FALSE;
	bContainsFactory = FALSE;
	bContainsRefinery = FALSE;
	bContainsShipyard = FALSE;
	pCapital = NULL;
	bAdequateStorage = FALSE;
	for (int lc4 = 0; lc4 < NUM_MATERIAL_TYPES; lc4++)
	{
		bContainsStorageFor[lc4] = FALSE;
		bContainsProductionFor[lc4] = FALSE;
		fContains[lc4] = 0;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cBuildLocationSeedGroup::Delete_Seed(cBuildLocationSeed* pSeed)
{
	//re-link everything else around it
	if (pSeed->pNextSeed  != NULL)
		pSeed->pNextSeed->pPrevSeed = pSeed->pPrevSeed;
	if (pSeed->pPrevSeed != NULL)
		pSeed->pPrevSeed->pNextSeed = pSeed->pNextSeed;

	//then delete the seed itself
	delete pSeed;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cBuildLocationSeed* cBuildLocationSeedGroup::Create_Add_Seed(int iMapIndex)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cBuildLocationSeed* pNew = new cBuildLocationSeed;

	pNew->sqLocation.index = iMapIndex;
	pNew->sqLocation.Calc_xy(pCA->pGameWorld->iWidth);
	pNew->pSeedGroup = (void*)this;

	//link into list (always into back)
	cBuildLocationSeed* pNext = pSeed_Last;
	cBuildLocationSeed* pPrev = pSeed_Last->pPrevSeed;

	pNew->pNextSeed = pNext;
	pNew->pPrevSeed = pPrev;
	pNext->pPrevSeed = pNew;
	pPrev->pNextSeed = pNew;
	
	//return the pointer
	return pNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cBuildLocationSeedGroup::Exit()
{
	//attempt to untie itself from everything around it
	if (pGroup_Prev != NULL)
		pGroup_Prev->pGroup_Next = pGroup_Next;
	if (pGroup_Next != NULL)
		pGroup_Next->pGroup_Prev = pGroup_Prev;
	//close down its own links
	pGroup_Prev = NULL;
	pGroup_Next = NULL;

	//then delete the seeds it has inside
	while (pSeed_First->pNextSeed != pSeed_Last)
	{
		Delete_Seed(pSeed_First->pNextSeed);
	}
	//then delete anchors
	delete pSeed_First;
	delete pSeed_Last;

	pSeed_First = NULL;
	pSeed_Last = NULL;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cBuildLocationSeed* cBuildLocationSeedGroup::Find_Seed(int iMapIndex)
{
	cBuildLocationSeed* pTrav = pSeed_First->pNextSeed;
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	while (pTrav != pSeed_Last)
	{
		if (pTrav->sqLocation.index == iMapIndex)
		{
			return pTrav;
		}
		pTrav = pTrav->pNextSeed;
	}
	return NULL;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cBuildLocationSeedGroup::Init(void* pCA, cBuildLocationSeedGroup* pPrev, cBuildLocationSeedGroup* pNext)
{
	pCAccess = pCA;

	//initialize values
	pGroup_Prev = pPrev;
	pGroup_Next = pNext;

	Clear_Gather_Info();

	pSeed_First = new cBuildLocationSeed;
	pSeed_Last = new cBuildLocationSeed;

	//link together
	pSeed_First->pPrevSeed = NULL;
	pSeed_First->pNextSeed = pSeed_Last;
	//pSeed_First->pSeedGroup = (void*)this;

	pSeed_Last->pPrevSeed = pSeed_First;
	pSeed_Last->pNextSeed = NULL;
	//pSeed_Last->pSeedGroup = (void*)this;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cLocationsManager::Init(void* pCA, int _iCtrl)
{
	pCAccess = pCA;
	//initialize anchors
	pGroup_First = new cBuildLocationSeedGroup;
	pGroup_Last = new cBuildLocationSeedGroup;

	iNumSeedGroups = 0;

	iControl = _iCtrl;

	//initialize links
	pGroup_First->pGroup_Prev = NULL;
	pGroup_First->pGroup_Next = pGroup_Last;
	pGroup_Last->pGroup_Prev = pGroup_First;
	pGroup_Last->pGroup_Next = NULL;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cLocationsManager::Delete_BuildLocationSeedGroup(cBuildLocationSeedGroup* pGroup)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//fire off AI event
//fire off AI event:
		for (int lc94 = 0; lc94 < TEMP_DP_MAXPLAYERS; lc94++)
		{
			if (pCA->pNationManager->ppNations[lc94] != NULL)
			{
				pCA->pNationManager->ppNations[lc94]->AINew.Event_Seed_Destroyed(pGroup);
			}
		}

	//relink external links around each other
	if (pGroup->pGroup_Next != NULL)
		pGroup->pGroup_Next->pGroup_Prev = pGroup->pGroup_Prev;
	if (pGroup->pGroup_Prev != NULL)
		pGroup->pGroup_Prev->pGroup_Next = pGroup->pGroup_Next;
	//exit internally
	pGroup->Exit();
	//then delete
	delete pGroup;

	iNumSeedGroups--;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cLocationsManager::Exit()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//delete everything
	while (pGroup_First->pGroup_Next != pGroup_Last)
	{
		Delete_BuildLocationSeedGroup(pGroup_First->pGroup_Next);
	}
	//then delete own anchors
	delete pGroup_First; pGroup_First = NULL;
	delete pGroup_Last; pGroup_Last = NULL;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cBuildLocationSeedGroup* cLocationsManager::Find_SeedGroup(int iMapIndex)
{
	cBuildLocationSeedGroup* pTrav = pGroup_First->pGroup_Next;

	while (pTrav != pGroup_Last)
	{
		if (pTrav->iID == iMapIndex)
			return pTrav;

		pTrav = pTrav->pGroup_Next;
	}
	return NULL;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cBuildLocationSeedGroup* cLocationsManager::Create_Add_BuildLocationSeedGroup(int iMapIndex)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
		//DEBUG
		//pCA->pChatBuffer->Add_CBNode("Seed added (new group).", 60);
	//add to end
	cBuildLocationSeedGroup* pNew = new cBuildLocationSeedGroup;

	//initialize
	pNew->Init(pCAccess, pGroup_Last->pGroup_Prev, pGroup_Last);
	//set links around it

	pGroup_Last->pGroup_Prev->pGroup_Next = pNew;
	pGroup_Last->pGroup_Prev = pNew;

	//set ID
	pNew->iID = iMapIndex;
	//add seed
	pNew->Create_Add_Seed(iMapIndex);

	iNumSeedGroups++;
	//DEBUG
	//char NText[200];
	//itoa(iMapIndex, NText, 10);
	//strcat(NText, " New seed group created");
	//pCA->pChatBuffer->Add_CBNode(NText, 100);

	//fire off AI event
//fire off AI event:
		//DEBUG
		pCA->pNationManager->ppNations[iControl]->AINew.Event_Seed_Created(pNew);

	//return pointer
	return pNew;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cAINew::Find_First_ID(int iControl)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//
	for (int lc1 = 0; lc1 < UNIT_NUMIDS; lc1++)
	{
		if (pCA->pUnitTable->pUnitTable[lc1] != NULL)
		if (pCA->pUnitTable->pUnitTable[lc1]->iControl == iControl)
		if (pCA->pUnitTable->pUnitTable[lc1]->fHP > 0)
		{
			return lc1;
		}
	}

	return -1;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cBuildLocationSeed* cLocationsManager::Find_Seed(int iMapIndex)
{
	cBuildLocationSeedGroup* pTrav = pGroup_First->pGroup_Next;
	cClassAccess* pCA = (cClassAccess*)pCAccess;
//DEBUG
	pCA->pChatBuffer->Add_CBNode("Entry: LM Find Seed", 200);

	while (pTrav != pGroup_Last)
	{
		cBuildLocationSeed* pSeed = pTrav->Find_Seed(iMapIndex);
		if (pSeed != NULL)
		{
			return pSeed;
		}

		pTrav = pTrav->pGroup_Next;
	}
	return NULL;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::AI_Integrity_Check()
{	
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cUnitA* pTrav = (cUnitA*)pCA->pUnitList->pUL_First->pNext_UL;

	while (pTrav != pCA->pUnitList->pUL_Last)
	{
		pTrav->AI_Misc_Use_Counter = 0;

		pTrav = (cUnitA*)pTrav->pNext_UL;
	}

	//now go through entities
	cAIEntity* pAITrav = pEntities_First->pNext;

	while (pAITrav != pEntities_Last)
	{
		if (pAITrav->iType == AI_ENTITY_TYPE_UNIT)
		{
			pTrav = pCA->pUnitTable->pUnitTable[pAITrav->iID];
			if (pTrav == NULL)
			{
				char NText[50];
				itoa(pAITrav->iID, NText, 10);
				strcat(NText, " ERROR Entity for unit which doesn't exist");
				pCA->pDD_Display->Write_Log(NText);
				pCA->pDD_Display->Write_Log("\r\n");
				pCA->pChatBuffer->Add_CBNode(NText, 600);
			} else
				pTrav->AI_Misc_Use_Counter++;

		}
		pAITrav = pAITrav->pNext;
	}

	//now go through the other direction
	pTrav = (cUnitA*)pCA->pUnitList->pUL_First->pNext_UL;

	while (pTrav != pCA->pUnitList->pUL_Last)
	{
		if (pTrav->AI_Misc_Use_Counter == 0)
		{
				char NText[50];
				itoa(pTrav->iID, NText, 10);
				strcat(NText, " ERROR No Entity for this ID");
				pCA->pDD_Display->Write_Log(NText);
				pCA->pDD_Display->Write_Log("\r\n");
				pCA->pChatBuffer->Add_CBNode(NText, 600);
		}
		if (pTrav->AI_Misc_Use_Counter > 1)
		{
				char NText[50];
				itoa(pTrav->iID, NText, 10);
				strcat(NText, " Greater than 1 entity fort his ID");
				pCA->pDD_Display->Write_Log(NText);
				pCA->pDD_Display->Write_Log("\r\n");
				pCA->pChatBuffer->Add_CBNode(NText, 600);
		}
		pTrav = (cUnitA*)pTrav->pNext_UL;
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cAINew::Compute_Build_Unit_Base_Type(cAIEntity* pEnt_Struct)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iType = -1;
	
	//go through list and find first unit with a weapon 
	//note: more complex logic should eventually go here
	for (int lc1 = 0; lc1 < pCA->pUTypeTable->iHighestTypeID; lc1++)
	{
		//if it has a weapon
		if (pCA->pUTypeTable->UnitTypes[lc1].CmpAttached.pLByClass_First[CMPCLASS_WEAPON]->pNext_classlist != pCA->pUTypeTable->UnitTypes[lc1].CmpAttached.pLByClass_Last[CMPCLASS_WEAPON])
			return lc1;
	}

	return iType;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cAINew::Find_Tanker_Type(cAIEntity* pEnt_Struct)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iType = -1;
	
	//go through list and find first unit with a storage 
	//note: more complex logic should eventually go here
	for (int lc1 = 0; lc1 < pCA->pUTypeTable->iHighestTypeID; lc1++)
	{
		cComponentA* pTrav = (cComponentA*)pCA->pUTypeTable->UnitTypes[lc1].CmpAttached.pCmpList_First->pNext_list;

		//if it has a storage
		while (pTrav != pCA->pUTypeTable->UnitTypes[lc1].CmpAttached.pCmpList_Last)
		{
			if (pCA->pCmpInfoTable->pCmpTable[pTrav->iType]->pStoreInfo != NULL)
			if (pCA->pCmpInfoTable->pCmpTable[pTrav->iType]->pStoreInfo->ddMaterialStorageBonus > 0)
				return lc1;
	
			pTrav = (cComponentA*) pTrav->pNext_list;
		}
			
	}

	return iType;	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Get_Task_Name(int iTaskType, char* Text)
{
	if (Text == NULL)
		return;

	Text[0] = 0;

	if (iTaskType == AI_TASKTYPE_ATTACK_SECTOR)
		strcat(Text, "AI_TASKTYPE_ATTACK_SECTOR");
	if (iTaskType == AI_TASKTYPE_DEFEND_SECTOR)
		strcat(Text, "AI_TASKTYPE_DEFEND_SECTOR");
	if (iTaskType == AI_TASKTYPE_DEVELOP_SECTOR)
		strcat(Text, "AI_TASKTYPE_DEVELOP_SECTOR");
	if (iTaskType == AI_TASKTYPE_ESTABLISH_CAPITAL)
		strcat(Text, "AI_TASKTYPE_ESTABLISH_CAPITAL");
	if (iTaskType == AI_TASKTYPE_ESTABLISH_OUTPOST)
		strcat(Text, "AI_TASKTYPE_ESTABLISH_OUTPOST");
	if (iTaskType == AI_TASKTYPE_EXPLORE_SECTOR)
		strcat(Text, "AI_TASKTYPE_EXPLORE_SECTOR");
	if (iTaskType == AI_TASKTYPE_ATTACK_SEED)
		strcat(Text, "AI_TASKTYPE_ATTACK_SEED");
	if (iTaskType == AI_TASKTYPE_DEFEND_SEED)
		strcat(Text, "AI_TASKTYPE_DEFEND_SEED");
	if (iTaskType == AI_TASKTYPE_DEVELOP_SEED)
		strcat(Text, "AI_TASKTYPE_DEVELOP_SEED");
	if (iTaskType == AI_TASKTYPE_BUILD_STORAGE)
		strcat(Text, "AI_TASKTYPE_BUILD_STORAGE");
	if (iTaskType == AI_TASKTYPE_BUILD_FORGE)
		strcat(Text, "AI_TASKTYPE_BUILD_FORGE");
	if (iTaskType == AI_TASKTYPE_BUILD_FACTORY)
		strcat(Text, "AI_TASKTYPE_BUILD_FACTORY");
	if (iTaskType == AI_TASKTYPE_BUILD_SHIPYARD)
		strcat(Text, "AI_TASKTYPE_BUILD_SHIPYARD");
	if (iTaskType == AI_TASKTYPE_BUILD_REFINERY)
		strcat(Text, "AI_TASKTYPE_BUILD_REFINERY");
	if (iTaskType == AI_TASKTYPE_BUILD_EXTRACTOR)
		strcat(Text, "AI_TASKTYPE_BUILD_EXTRACTOR");
	if (iTaskType == AI_TASKTYPE_EXPLORE_SEED)
		strcat(Text, "AI_TASKTYPE_EXPLORE_SEED");
	if (iTaskType == AI_TASKTYPE_BUILD_UNIT)
		strcat(Text, "AI_TASKTYPE_BUILD_UNIT");
	if (iTaskType == AI_TASKTYPE_BUILD_UNIT_BASE)
		strcat(Text, "AI_TASKTYPE_BUILD_UNIT_BASE");
	if (iTaskType == AI_TASKTYPE_BUILD_TANKER_A)
		strcat(Text, "AI_TASKTYPE_BUILD_TANKER_A");

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Init_Location_Lists()
{
	pLoc_Cap_First = new cAILocation;
	pLoc_Cap_Last = new cAILocation;

	pLoc_Outpost_First = new cAILocation;
	pLoc_Outpost_Last = new cAILocation;

	pLoc_Cap_First->pPrev = NULL;
	pLoc_Cap_First->pNext = pLoc_Cap_Last;
	pLoc_Cap_Last->pPrev = pLoc_Cap_First;
	pLoc_Cap_Last->pNext = NULL;

	pLoc_Outpost_First->pPrev = NULL;
	pLoc_Outpost_First->pNext = pLoc_Outpost_Last;
	pLoc_Outpost_Last->pPrev = pLoc_Outpost_First;
	pLoc_Outpost_Last->pNext = NULL;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Exit_Location_Lists()
{
	cAILocation*	pTrav = pLoc_Cap_First->pNext;
	cAILocation*	pTrav2;

	while (pTrav != pLoc_Cap_Last)
	{
		pTrav2 = pTrav->pNext;
		delete pTrav;

		pTrav = pTrav2;
	}

	pTrav = pLoc_Outpost_First->pNext;

	while (pTrav != pLoc_Outpost_Last)
	{
		pTrav2 = pTrav->pNext;
		delete pTrav;

		pTrav = pTrav2;
	}

	delete pLoc_Cap_First;
	delete pLoc_Cap_Last;
	delete pLoc_Outpost_First;
	delete pLoc_Outpost_Last;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Create_Add_Location(int iIndex, double _ddEfficacy, int iTick_Evaluated, int iQueue)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cAILocation* pNew = new cAILocation;

	pNew->iTick_Evaluated = iTick_Evaluated;
	pNew->ddEfficacy = _ddEfficacy;
	pNew->sqPosition.index = iIndex;
	pNew->sqPosition.Calc_xy(pCA->pGameWorld->iWidth);

	Add_Location(pNew, iQueue);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Add_Location(cAILocation* pAdd, int iQueue)
{
    cAILocation* pPrev;
	cAILocation* pNext;

	if (iQueue == 0)
	{
		pPrev = pLoc_Cap_First;
		pNext = pLoc_Cap_First->pNext;
		while (pNext != pLoc_Cap_Last)
		{
			if (pAdd->ddEfficacy > pNext->ddEfficacy)
				break;
				
			pNext = pNext->pNext;
			pPrev = pPrev->pNext;
		}
	}
	if (iQueue == 1)
	{
		pPrev = pLoc_Outpost_First;
		pNext = pLoc_Outpost_First->pNext;
		while (pNext != pLoc_Outpost_Last)
		{
			if (pAdd->ddEfficacy > pNext->ddEfficacy)
				break;

			pNext = pNext->pNext;
			pPrev = pPrev->pNext;
		}

	}
	//link together
	pPrev->pNext = pAdd;
	pNext->pPrev = pAdd;
	pAdd->pPrev = pPrev;
	pAdd->pNext = pNext;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Location_Processing()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	double ddScore = 0;

	for (int lc1 = 0; lc1 < 10; lc1++)
	{
		int iLoc = pCA->pRandGen->rand2() % (pCA->pGameWorld->iHeight * pCA->pGameWorld->iWidth);
		ddScore = Evaluate_Location_Capital(iLoc);
		Create_Add_Location(iLoc, ddScore, pCA->pTimer->iCurrTick, 0);
		ddScore = Evaluate_Location_Outpost(iLoc);
		Create_Add_Location(iLoc, ddScore, pCA->pTimer->iCurrTick, 1);

	}
	//go through queue and delete old locations
	cAILocation* pTrav = pLoc_Cap_First->pNext;
	cAILocation* pTrav2;
	while (pTrav != pLoc_Cap_Last)
	{
		pTrav2 = pTrav->pNext;
		if ((pCA->pTimer->iCurrTick - pTrav->iTick_Evaluated) > 400)
			Delete_Location(pTrav);
		pTrav = pTrav2;
	}
	pTrav = pLoc_Outpost_First->pNext;
	while (pTrav != pLoc_Outpost_Last)
	{
		pTrav2 = pTrav->pNext;
		if ((pCA->pTimer->iCurrTick - pTrav->iTick_Evaluated) > 400)
			Delete_Location(pTrav);
		pTrav = pTrav2;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAINew::Delete_Location(cAILocation* pDelete)
{
	//unlink
	cAILocation* pPrev = pDelete->pPrev;
	cAILocation* pNext = pDelete->pNext;

	if (pPrev != NULL)
		pPrev->pNext = pNext;
	if (pNext != NULL)
		pNext->pPrev = pPrev;

	//now delete
	delete pDelete;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Location_Outpost(int iIndex)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//compute carbon and iron amts claimable here
	double ResourcePotentials[NUM_MATERIAL_TYPES];
	ZeroMemory(ResourcePotentials, sizeof(double)*NUM_MATERIAL_TYPES);

	int iWidth = pCA->pGameWorld->iWidth;
	int iHeight = pCA->pGameWorld->iHeight;

	int iRad = TEMP_CLAIMRADIUS_OUTPOST;
	SQCOORDSET sqCheck;
	sqCheck.index = iIndex;
	sqCheck.Calc_xy(pCA->pGameWorld->iWidth);

	int iBound_XLow = sqCheck.x - iRad;
	if (iBound_XLow < 0)
		iBound_XLow = 0;
	
	int iBound_XHigh = sqCheck.x + iRad;
	if (iBound_XHigh >= (iWidth - 1))
		iBound_XHigh = iWidth - 2;

	int iBound_YLow = sqCheck.y - iRad;
	if (iBound_YLow < 0)
		iBound_YLow = 0;
	
	int iBound_YHigh = sqCheck.y + iRad;
	if (iBound_YHigh >= (iHeight - 1))
		iBound_YHigh = iHeight - 2;


	int iHigherManhattan = iRad * 1.41;  //sqrt(2)

	for (int cy = iBound_YLow; cy <= iBound_YHigh; cy++)
	for (int cx = iBound_XLow; cx <= iBound_XHigh; cx++)
	{
			int index = (cy * iWidth) + cx;
			MTile* pTile = &pCA->pGameWorld->pMap[ index];
		if ( abs(cy - sqCheck.y) + abs(cx-sqCheck.x) <= iHigherManhattan)
		{
			for (int lc8 = 0; lc8 < NUM_MATERIAL_TYPES; lc8++)
				ResourcePotentials[lc8] += pCA->pGameWorld->TileInfos[pTile->iType].ResourcePayout[lc8];
		}
	} 

	//the "score" for each tile is the linear combination of the potentials and the demand
	double ddScore = 0;

	for (int lc9 = 0; lc9 < NUM_MATERIAL_TYPES; lc9++)
		ddScore += ResourcePotentials[lc9] * AITopLevel.MaterialDemand[lc9];

	return ddScore;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cAINew::Evaluate_Location_Capital(int iIndex)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//compute carbon and iron amts claimable here
	double ResourcePotentials[NUM_MATERIAL_TYPES];
	ZeroMemory(ResourcePotentials, sizeof(double)*NUM_MATERIAL_TYPES);

	int iWidth = pCA->pGameWorld->iWidth;
	int iHeight = pCA->pGameWorld->iHeight;

	int iRad = TEMP_CLAIMRADIUS_CAPITAL;
	SQCOORDSET sqCheck;
	sqCheck.index = iIndex;
	sqCheck.Calc_xy(pCA->pGameWorld->iWidth);

	int iBound_XLow = sqCheck.x - iRad;
	if (iBound_XLow < 0)
		iBound_XLow = 0;
	
	int iBound_XHigh = sqCheck.x + iRad;
	if (iBound_XHigh >= (iWidth - 1))
		iBound_XHigh = iWidth - 2;

	int iBound_YLow = sqCheck.y - iRad;
	if (iBound_YLow < 0)
		iBound_YLow = 0;
	
	int iBound_YHigh = sqCheck.y + iRad;
	if (iBound_YHigh >= (iHeight - 1))
		iBound_YHigh = iHeight - 2;


	int iHigherManhattan = iRad * 1.41;  //sqrt(2)

	for (int cy = iBound_YLow; cy <= iBound_YHigh; cy++)
	for (int cx = iBound_XLow; cx <= iBound_XHigh; cx++)
	{
			int index = (cy * iWidth) + cx;
			MTile* pTile = &pCA->pGameWorld->pMap[ index];
		if ( abs(cy - sqCheck.y) + abs(cx-sqCheck.x) <= iHigherManhattan)
		{
			for (int lc8 = 0; lc8 < NUM_MATERIAL_TYPES; lc8++)
				ResourcePotentials[lc8] += pCA->pGameWorld->TileInfos[pTile->iType].ResourcePayout[lc8];
		}
	} 

	//the "score" for each tile is the linear combination of the potentials and the demand
	double ddScore = 0;

	for (int lc9 = 0; lc9 < NUM_MATERIAL_TYPES; lc9++)
		ddScore += ResourcePotentials[lc9] * AITopLevel.MaterialDemand[lc9];

	return ddScore;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
