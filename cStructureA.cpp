#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructInfoTable::Init(VOID* pCA)
{
	pCAccess = pCA;


	//initialize anchors
	pStructs_First = new cStructInfo;
	pStructs_Last = new cStructInfo;


	pStructs_First->pPrev = NULL;
	pStructs_First->pNext = (void*)pStructs_Last;
	pStructs_Last->pPrev = (void*)pStructs_First;
	pStructs_Last->pNext = NULL;

	//set up information for command types we'll be using

	//none so far. not decided yet (03.28.2003)

	
	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructInfoTable::Add_StructType_Simple(LPSTR GeneralName,
									   int iType, 
									   double ddResourceCost,
									   int iImageSurfIndex,
									   int iXFramesize,
									   int iYFramesize,
									   int iXDrawOffset,
									   int iYDrawOffset,
									   int iHP,
									   double ddMaterialStorageCapacity,
									   int iOccMaptiles,
									   int iXOcc_offset1,
									   int iYOcc_offset1,
									   int iXOcc_offset2,
									   int iYOcc_offset2,							   
									   int iXOcc_offset3,
									   int iYOcc_offset3,
									   int iXOcc_offset4,
									   int iYOcc_offset4,
									   int iXOcc_offset5,
									   int iYOcc_offset5,
									   int iXOcc_offset6,
									   int iYOcc_offset6,
									   int iXOcc_offset7,
									   int iYOcc_offset7,
									   int iXOcc_offset8,
									   int iYOcc_offset8,
									   int iXOcc_offset9,
									   int iYOcc_offset9,
									   int iAttachedComponents,
									   int iXAC_offset1,
									   int iYAC_offset1,
									   int iXAC_offset2,
									   int iYAC_offset2,
									   int iXAC_offset3,
									   int iYAC_offset3,
									   int iXAC_offset4,
									   int iYAC_offset4,
									   int iNumCommCaps,
									   int iComm2_1,
									   int iComm2_2,
									   int iComm2_3,
									   int iComm2_4,
									   int iComm2_5,
									   int iComm2_6,
									   int iComm2_7,
									   int iComm2_8,
									   int iComm2_9,
									   int iComm2_10,
										int iPermission1,
										int iPermission2,
										int iPermission3,
										int iPermission4,
										int iPermission5,
										int iPermission6,
										int iPermission7,
										int iPermission8,
										int iPermission9,
										char* pProcessText,
									   BOOL bActiveOrderQueue,
									   BOOL bReconstructFlag)
{

		cClassAccess* pCA = (cClassAccess*)pCAccess;

cStructInfo* pStructNew = new cStructInfo;

//initialie
pStructNew->AC_Offsets = NULL;
pStructNew->CommCaps_Array = NULL;
pStructNew->Pos_offset_array = NULL;

//single-entry information
pStructNew->GeneralName = GeneralName;
pStructNew->iHP = iHP;
pStructNew->ddMaterialStorageCapacity = ddMaterialStorageCapacity;
pStructNew->iImageSurfIndex = iImageSurfIndex;
pStructNew->iXFramesize = iXFramesize;
pStructNew->iYFramesize = iYFramesize;
pStructNew->iXDrawOffset = iXDrawOffset;
pStructNew->iYDrawOffset = iYDrawOffset;
pStructNew->iAttachedComponents = iAttachedComponents;
pStructNew->iOccMaptiles = iOccMaptiles;
pStructNew->bActiveOrderQueue = bActiveOrderQueue;
pStructNew->ddResourceCost = ddResourceCost;

ZeroMemory(pStructNew->ProcessText, sizeof(char)*80);
//copy process text
if (pProcessText != NULL)
{
	int iNumToCopy = strlen(pProcessText);
	if (iNumToCopy > 80)
		iNumToCopy = 80;

	memcpy(pStructNew->ProcessText, pProcessText, sizeof(char)*80);
}

//allocate to fill Occ array
//note that the offset array doesn't need to contain information for the PRIMARY tile
if (iOccMaptiles > 0)
pStructNew->Pos_offset_array = new COORDSET[iOccMaptiles];

if (iOccMaptiles > 1)
{
	pStructNew->Pos_offset_array[0].x = iXOcc_offset1;
	pStructNew->Pos_offset_array[0].y = iYOcc_offset1;
}
if (iOccMaptiles > 2)
{
	pStructNew->Pos_offset_array[1].x = iXOcc_offset2;
	pStructNew->Pos_offset_array[1].y = iYOcc_offset2;
}	
if (iOccMaptiles > 3)
{
	pStructNew->Pos_offset_array[2].x = iXOcc_offset3;
	pStructNew->Pos_offset_array[2].y = iYOcc_offset3;
}
if (iOccMaptiles > 4)
{
	pStructNew->Pos_offset_array[3].x = iXOcc_offset4;
	pStructNew->Pos_offset_array[3].y = iYOcc_offset4;
}
if (iOccMaptiles > 5)
{
	pStructNew->Pos_offset_array[4].x = iXOcc_offset5;
	pStructNew->Pos_offset_array[4].y = iYOcc_offset5;
}
if (iOccMaptiles > 6)
{
	pStructNew->Pos_offset_array[5].x = iXOcc_offset6;
	pStructNew->Pos_offset_array[5].y = iYOcc_offset6;
}
if (iOccMaptiles > 7)
{
	pStructNew->Pos_offset_array[6].x = iXOcc_offset7;
	pStructNew->Pos_offset_array[6].y = iYOcc_offset7;
}
if (iOccMaptiles > 8)
{
	pStructNew->Pos_offset_array[7].x = iXOcc_offset8;
	pStructNew->Pos_offset_array[7].y = iYOcc_offset8;
}
if (iOccMaptiles > 9)
{
	pStructNew->Pos_offset_array[8].x = iXOcc_offset9;
	pStructNew->Pos_offset_array[8].y = iYOcc_offset9;
}

//allocate to fill CmpAttach array
if (iAttachedComponents > 0)
	pStructNew->AC_Offsets = new COORDSET[iAttachedComponents];
if (iAttachedComponents > 0)
{
	pStructNew->AC_Offsets[0].x = iXAC_offset1;
	pStructNew->AC_Offsets[0].y = iYAC_offset1;
}
if (iAttachedComponents > 1)
{
	pStructNew->AC_Offsets[1].x = iXAC_offset2;
	pStructNew->AC_Offsets[1].y = iYAC_offset2;
}
if (iAttachedComponents > 2)
{
	pStructNew->AC_Offsets[2].x = iXAC_offset2;
	pStructNew->AC_Offsets[2].y = iYAC_offset2;
}
if (iAttachedComponents > 3)
{
	pStructNew->AC_Offsets[3].x = iXAC_offset4;
	pStructNew->AC_Offsets[3].y = iYAC_offset4;
}

//now allocate to fill commcaps array
pStructNew->iNumCommCaps = iNumCommCaps;

if (iNumCommCaps > 0)
pStructNew->CommCaps_Array = new int[iNumCommCaps];
if (iNumCommCaps > 0)
{
	pStructNew->CommCaps_Array[0] = iComm2_1;
}
if (iNumCommCaps > 1)
{
	pStructNew->CommCaps_Array[1] = iComm2_2;
}
if (iNumCommCaps > 2)
{
	pStructNew->CommCaps_Array[2] = iComm2_3;
}
if (iNumCommCaps > 3)
{
	pStructNew->CommCaps_Array[3] = iComm2_4;
}
if (iNumCommCaps > 4)
{
	pStructNew->CommCaps_Array[4] = iComm2_5;
}
if (iNumCommCaps > 5)
{
	pStructNew->CommCaps_Array[5] = iComm2_6;
}
if (iNumCommCaps > 6)
{
	pStructNew->CommCaps_Array[6] = iComm2_7;
}
if (iNumCommCaps > 7)
{
	pStructNew->CommCaps_Array[7] = iComm2_8;
}
if (iNumCommCaps > 8)
{
	pStructNew->CommCaps_Array[9] = iComm2_9;
}
if (iNumCommCaps > 9)
{
	pStructNew->CommCaps_Array[10] = iComm2_10;
}
if (iPermission1 >= 0)
	pStructNew->Permissions.Add_Permission(iPermission1);
if (iPermission2 >= 0)
	pStructNew->Permissions.Add_Permission(iPermission2);
if (iPermission3 >= 0)
	pStructNew->Permissions.Add_Permission(iPermission3);
if (iPermission4 >= 0)
	pStructNew->Permissions.Add_Permission(iPermission4);
if (iPermission5 >= 0)
	pStructNew->Permissions.Add_Permission(iPermission5);
if (iPermission6 >= 0)
	pStructNew->Permissions.Add_Permission(iPermission6);
if (iPermission7 >= 0)
	pStructNew->Permissions.Add_Permission(iPermission7);
if (iPermission8 >= 0)
	pStructNew->Permissions.Add_Permission(iPermission8);
if (iPermission9 >= 0)
	pStructNew->Permissions.Add_Permission(iPermission9);
//now add the element to the list
cStructInfo* pPrev = (cStructInfo*)pStructs_Last->pPrev;
	
	pStructNew->pNext = (void*)pStructs_Last;
	pStructNew->pPrev = (void*)pPrev;
	pStructs_Last->pPrev = (void*)pStructNew;
	pPrev->pNext = (void*)pStructNew;

	pStructNew->iType = iType;

//DEBUG
pCA->pStructList->Write_Log("[Reconstruct] Structure Type Created. Name: ");
pCA->pStructList->Write_Log(pStructNew->GeneralName);
pCA->pStructList->WriteLog_Enter();

//if reconstruct, reconstruct
	if (bReconstructFlag == TRUE)
		Reconstruct();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructInfoTable::Reconstruct()
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	//find the highest type ID;
	iHighestTypeID = 0;

	cStructInfo* pSI_Trav = (cStructInfo*)pStructs_First->pNext;

	while (pSI_Trav != pStructs_Last)
	{
		if (pSI_Trav->iType > iHighestTypeID)
			iHighestTypeID = pSI_Trav->iType;

		pSI_Trav = (cStructInfo*)pSI_Trav->pNext;
	}
//now allocate
	if (pTable != NULL)
		delete(pTable);
	pTable = new cStructInfo*[iHighestTypeID + 1];
	//initialize
	ZeroMemory(pTable, sizeof(cStructInfo*)*(iHighestTypeID + 1));

//now go through and assign pointers
	pSI_Trav = (cStructInfo*)pStructs_First->pNext;

	while (pSI_Trav != pStructs_Last)
	{

//DEBUG
pCA->pStructList->Write_Log("[Reconstruct] Structure Type Encountered. Name: ");
pCA->pStructList->Write_Log(pSI_Trav->GeneralName);
char NText[50];
pCA->pStructList->Write_Log("Type ID: ");
itoa(pSI_Trav->iType, NText, 10);
pCA->pStructList->Write_Log(NText);
pCA->pStructList->WriteLog_Enter();

		pTable[pSI_Trav->iType] = pSI_Trav;

		pSI_Trav = (cStructInfo*)pSI_Trav->pNext;
	}




}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructInfoTable::Exit()
{
	//go through list....
	cStructInfo* pSI_Trav = (cStructInfo*)pStructs_First->pNext;
	cStructInfo* pSI_Trav2;

	while (pSI_Trav != pStructs_Last)
	{
		if (pSI_Trav->AC_Offsets != NULL)
			delete(pSI_Trav->AC_Offsets);
		if (pSI_Trav->CommCaps_Array != NULL)
			delete(pSI_Trav->CommCaps_Array);
		if (pSI_Trav->Pos_offset_array != NULL)
			delete(pSI_Trav->Pos_offset_array);

		pSI_Trav2 = (cStructInfo*)pSI_Trav->pNext;

		delete(pSI_Trav);
		pSI_Trav = pSI_Trav2;
	}

	//delete the anchors
	delete(pStructs_First);
	delete(pStructs_Last);
	if (pTable != NULL)
		delete(pTable);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructListA::Init(VOID* pCA)
{


	pCAccess = pCA;
if (DEBUG + DEBUG_STRUCTLIST >= 1)
	Init_LogFile();

	//initialize anchors
	pSL_First = new cStructureA;
	pSL_Last = new cStructureA;

	pSL_First->pPrev = NULL;
	pSL_First->pNext = (void*)pSL_Last;
	pSL_Last->pPrev = (void*)pSL_First;
	pSL_Last->pNext = NULL;

	iID_Last_Assigned = 0;
	iBuildOrder_Last_Assigned = 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructListA::Exit()
{
	//go through list....
	cStructureA* pSL_Trav = (cStructureA*)pSL_First->pNext;
	cStructureA* pSL_Trav2;

	while (pSL_Trav != pSL_Last)
	{

		if (pSL_Trav->pSupportedCommands != NULL)
			delete(pSL_Trav->pSupportedCommands);
		if (pSL_Trav->Pos_array != NULL)
			delete(pSL_Trav->Pos_array);

		pSL_Trav2 = (cStructureA*)pSL_Trav->pNext;

		delete(pSL_Trav);
		pSL_Trav = pSL_Trav2;
	}

	//delete the anchors
	delete(pSL_First);
	delete(pSL_Last);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructListA::Destroy_AllStructures()
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//go through list....
	cStructureA* pSL_Trav = (cStructureA*)pSL_First->pNext;
	cStructureA* pSL_Trav2;

	while (pSL_Trav != pSL_Last)
	{
		pCA->pGameWorld->Unregister_Structure(pSL_Trav->Pos_primary, pSL_Trav);

		pSL_Trav->OrderQueue.Exit();
		//free name
		delete(pSL_Trav->Name);

		if (pSL_Trav->pSupportedCommands != NULL)
			delete(pSL_Trav->pSupportedCommands);
		if (pSL_Trav->Pos_array != NULL)
			delete(pSL_Trav->Pos_array);

		pSL_Trav2 = (cStructureA*)pSL_Trav->pNext;

		delete(pSL_Trav);
		pSL_Trav = pSL_Trav2;
	}

	//re-link anchors
	pSL_First->pNext = (void*)pSL_Last;
	pSL_Last->pPrev = (void*)pSL_First;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cStructureA* cStructListA::Create_Structure(LPSTR Name, int iSector, int iType, int iControl, int iXPos_square, int iYPos_square)
{

//DEBUG
Write_Log("Init A");
WriteLog_Enter();

	//allocate
	cStructureA* pStructNew = new cStructureA;
	//get a pointer to the relevant type information to unit
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cStructInfo* pStructInfo = pCA->pStructInfoTable->pTable[iType];

//DEBUG
Write_Log("Init A");
WriteLog_Enter();


	pStructNew->Name = new char[strlen(Name) + 1];
	strcpy(pStructNew->Name, Name);
//		//NULL terminate
		pStructNew->Name[strlen(Name)] = 0;

		//initialize Pie text
		ZeroMemory(pStructNew->ProcessText, sizeof(char)*80);
		//copy from its structure info
		memcpy(pStructNew->ProcessText, pStructInfo->ProcessText, sizeof(char)*80);

	pStructNew->fCompletion = 0;  //changed 10.12.2003
	pStructNew->iBuildOrderNumber = 0; //10.12.2003
	pStructNew->bOn = TRUE;
	pStructNew->iControl = iControl;
	pStructNew->iSector = iSector;
	pStructNew->Pos_primary.x = iXPos_square;
	pStructNew->Pos_primary.y = iYPos_square;
	pStructNew->Pos_primary.Calc_Index(pCA->pGameWorld->iWidth);
	pStructNew->iType = iType;
	pStructNew->iID = iID_Last_Assigned + 1;
	pStructNew->pCAccess = pCAccess;
	pStructNew->ExtractionInfo.iExtractionMaterialType = -1;
	iID_Last_Assigned++;

//DEBUG
Write_Log("Init A");
WriteLog_Enter();

//if the structure is a CAPITAL or outpost, initial the resource collection array
//changed to all structures 2007.05.25
	//if ( (iType == TEMP_STYPE_RSC) || (iType == TEMP_STYPE_OUTPOST))
	//{
		pStructNew->pResourceCollection = new double[NUM_MATERIAL_TYPES];
		ZeroMemory(pStructNew->pResourceCollection, sizeof(double)* NUM_MATERIAL_TYPES);
	//}

	//increment national counter
	pCA->pNationManager->ppNations[iControl]->iNumStructures++;

	//retrieve information from type info
	pStructNew->iHPMax = pStructInfo->iHP;
	pStructNew->iHP = 1;//pStructInfo->iHP;  //because fCompletion starts out at 0;
	pStructNew->Storage.ddMaterialCapacity = pStructInfo->ddMaterialStorageCapacity;
	pStructNew->iNumArrayEntries = pStructInfo->iOccMaptiles - 1;
	pStructNew->Pos_array = new SQCOORDSET[pStructNew->iNumArrayEntries];



	//TEST
	//pStructNew->Storage.ddMaterialStored = rand() % 100;
	//pStructNew->Storage.iMaterialType = ((rand() % 3) * 10) + (rand() % 3);
	//initialize
	pStructNew->Storage.bIsCarrierStructure = TRUE;
	pStructNew->Storage.iCarrierID = pStructNew->iID;
	pStructNew->iLastMaterialDeposited = 0;
	pStructNew->iLastMaterialWithdrawn = 0;
	pStructNew->iLastDumped = 0;

	for (int lc72 = 0; lc72 < NUM_MATERIAL_TYPES; lc72++)
	{
		pStructNew->iLastAward[lc72] = 0;
	}

//DEBUG
Write_Log("Init B");
WriteLog_Enter();


	for (int c19 = 0; c19 < pStructNew->iNumArrayEntries; c19++)
	{
		pStructNew->Pos_array[c19].x = pStructInfo->Pos_offset_array[c19].x + iXPos_square;
		pStructNew->Pos_array[c19].y = pStructInfo->Pos_offset_array[c19].y + iYPos_square;
		pStructNew->Pos_array[c19].Calc_Index(pCA->pGameWorld->iWidth);
		//set pipe
		pCA->pGameWorld->Add_Link(pStructNew->Pos_array[c19].index);
	}

//DEBUG
Write_Log("Init C");
WriteLog_Enter();

	
	pStructNew->iComponentSlots = pStructInfo->iAttachedComponents;
	pStructNew->CmpAttached	= new cComponentA*[pStructInfo->iAttachedComponents];
//DEBUG
Write_Log("Init D");
WriteLog_Enter();

	
	//no assigning needed here
	//now get the command caps data
	//disabled in favor of simply int-array of support command indexes(05.20.2003)
/*
	//initialize anchors
	pStructNew->pCommCaps_First = new COMMISSUEINFO;
	pStructNew->pCommCaps_Last = new COMMISSUEINFO;
	pStructNew->pCommCaps_First->pPrev = NULL;
	pStructNew->pCommCaps_First->pNext = (void*)pStructNew->pCommCaps_Last;
	pStructNew->pCommCaps_Last->pPrev = (void*)pStructNew->pCommCaps_First;
	pStructNew->pCommCaps_Last->pNext = NULL;
	//now add
	for (int c33 = 0; c33 < pStructInfo->iNumCommCaps; c33++)
	{
		COMMISSUEINFO* pCommCapNew = new COMMISSUEINFO;
		pCommCapNew->pCommand2 = pStructInfo->CommCaps_Array[c33].pCommand2;
		pCommCapNew->iDataSources2 = pStructInfo->CommCaps_Array[c33].iDataSources2;
		COMMISSUEINFO* pPrev = (COMMISSUEINFO*)pStructNew->pCommCaps_Last->pPrev;

		pCommCapNew->pPrev = (void*)pPrev;
		pCommCapNew->pNext = (void*)pStructNew->pCommCaps_Last;
		pPrev->pNext = (void*)pCommCapNew;
		pStructNew->pCommCaps_Last->pPrev = (void*)pCommCapNew;
	}
*/
//DEBUG
Write_Log("Init E");
WriteLog_Enter();


	//for now, simply copy over (05.20.2003)
	pStructNew->iNumSupportedCommands = pStructInfo->iNumCommCaps;
//DEBUG
Write_Log("Init E2");
WriteLog_Enter();

if (pStructInfo->iNumCommCaps != 0)
{
	pStructNew->pSupportedCommands = new int[pStructInfo->iNumCommCaps];

//DEBUG
Write_Log("Init E3");
WriteLog_Enter();

	memcpy(pStructNew->pSupportedCommands, pStructInfo->CommCaps_Array, sizeof(int) * pStructInfo->iNumCommCaps);//sizeof(int)*pStructInfo->iNumCommCaps);
}
	else
	pStructNew->pSupportedCommands = NULL;


	pStructNew->pCAccess = pCAccess;
	pStructNew->OrderQueue.Init(pCAccess);

//DEBUG
Write_Log("Init E4");
WriteLog_Enter();


	//now add the structure itself to the list
	cStructureA* pPrev = (cStructureA*)pSL_Last->pPrev;

//DEBUG
Write_Log("Init F");
WriteLog_Enter();


	pStructNew->pPrev = (void*)pPrev;
	pStructNew->pNext = (void*)pSL_Last;
	pPrev->pNext = (void*)pStructNew;
	pSL_Last->pPrev = (void*)pStructNew;

//DEBUG
Write_Log("Init G");
WriteLog_Enter();


	//register the structure on the ID table
	pCA->pStructIDTable->Register_Struct(pStructNew, pStructNew->iID);
	//register the structure on the gameworld
	pCA->pGameWorld->Register_Structure(pStructNew->Pos_primary, (void*)pStructNew);

if (pStructInfo->bActiveOrderQueue == TRUE)
{
	pStructNew->CmpInventory = new cComponentListA;
	pStructNew->CmpInventory->Init(pCAccess, pStructNew->Name);
}
	else
		pStructNew->CmpInventory = NULL;

//if it's a Extractor, scan the ground to figure out what its gonna do
if (pStructNew->iType == TEMP_STYPE_EXTRACTOR)
	pStructNew->Init_Extractor();

//AI stuff
/////////////////////////
//fire off AI event:
		for (int lc94 = 0; lc94 < TEMP_DP_MAXPLAYERS; lc94++)
		{
			if (pCA->pNationManager->ppNations[lc94] != NULL)
			{
				pCA->pNationManager->ppNations[lc94]->AINew.Event_Structure_Created(pStructNew);
			}
		}


//////////////////////////////////////////

//DEBUG
Write_Log("Init H");
WriteLog_Enter();

	return pStructNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructListA::Destroy_Structure_byaddr(cStructureA* pStruct)
{

	cStructureA* pNext = (cStructureA*)pStruct->pNext;
	cStructureA* pPrev = (cStructureA*)pStruct->pPrev;

	cClassAccess* pCA = (cClassAccess*)pCAccess;

//fire off AI event:
		for (int lc94 = 0; lc94 < TEMP_DP_MAXPLAYERS; lc94++)
		{
			if (pCA->pNationManager->ppNations[lc94] != NULL)
			{
				pCA->pNationManager->ppNations[lc94]->AINew.Event_Structure_Destroyed(pStruct);
			}
		}


	//unregister from the IDTable and the Gameworld
	pCA->pStructIDTable->Unregister_Struct(pStruct->iID);
	pCA->pGameWorld->Unregister_Structure(pStruct->Pos_primary, (void*)pStruct);

//decrement structures counter
	pCA->pNationManager->ppNations[pStruct->iControl]->iNumStructures--;

//DEBUG
Write_Log("Deleting OrderQueue");
WriteLog_Enter();

	pStruct->OrderQueue.Exit();
	
//free name
	delete(pStruct->Name);

//DEBUG
Write_Log("Deleting Inventory");
WriteLog_Enter();
//	delete(pStruct->CmpAttached);
//	pStruct->CmpInventory->Exit();
	if (pStruct->CmpInventory != NULL)
	{
		pStruct->CmpInventory->Exit();
		delete(pStruct->CmpInventory);
		//force main system recompile
		pCA->pNationManager->ppNations[pStruct->iControl]->NationalInventory.Recompile_TL();

	}
//DEBUG
Write_Log("Continuing with destruction");
WriteLog_Enter();
	

	//get rid of the internal commcaps list(disabled 05.20.2003)
/*
	COMMISSUEINFO* pCC_Trav = (COMMISSUEINFO*)pStruct->pCommCaps_First->pNext;
	COMMISSUEINFO* pCC_Trav2;
		while (pCC_Trav != pStruct->pCommCaps_Last)
		{
			
			pCC_Trav2 = (COMMISSUEINFO*)pCC_Trav->pNext;
			//NOTE. DON'T delete the command linked here, because it is likely
			//being used by multiple structures
			delete(pCC_Trav);
			pCC_Trav = pCC_Trav2;
		}
*/
	//just delete the commcaps array
	delete(pStruct->pSupportedCommands);

	//if its a CAPITAL or OUTPOST, delete its resource collection array
	delete [] pStruct->pResourceCollection;

	delete(pStruct->Pos_array);

	//delete the structure itself
	delete(pStruct);

  
	//relink Prev and Next
	pPrev->pNext = (void*)pNext;
	pNext->pPrev = (void*)pPrev;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructListA::Destroy_Structure_byID(int iID)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//identical to destroy by addr, just getting the pointer differently
	cStructureA* pStruct = pCA->pStructIDTable->pTable[iID];

	//fire off AI event:
		for (int lc94 = 0; lc94 < TEMP_DP_MAXPLAYERS; lc94++)
		{
			if (pCA->pNationManager->ppNations[lc94] != NULL)
			{
				pCA->pNationManager->ppNations[lc94]->AINew.Event_Structure_Destroyed(pStruct);
			}
		}

	cStructureA* pNext = (cStructureA*)pStruct->pNext;
	cStructureA* pPrev = (cStructureA*)pStruct->pPrev;

	//unregister from the IDTable and the Gameworld
	pCA->pStructIDTable->Unregister_Struct(pStruct->iID);
	pCA->pGameWorld->Unregister_Structure(pStruct->Pos_primary, (void*)pStruct);

//decrement structure counter
	pCA->pNationManager->ppNations[pStruct->iControl]->iNumStructures--;

	delete(pStruct->CmpAttached);
	pStruct->CmpInventory->Exit();
	

//free name
	delete(pStruct->Name);

	//get rid of the internal commcaps list(disabled 05.20.2003)
/*
	//get rid of the internal commcaps list
	COMMISSUEINFO* pCC_Trav = (COMMISSUEINFO*)pStruct->pCommCaps_First->pNext;
	COMMISSUEINFO* pCC_Trav2;
		while (pCC_Trav != pStruct->pCommCaps_Last)
		{
			
			pCC_Trav2 = (COMMISSUEINFO*)pCC_Trav->pNext;
			//NOTE. DON'T delete the command linked here, because it is likely
			//being used by multiple structures
			delete(pCC_Trav);
			pCC_Trav = pCC_Trav2;
		}
*/
	//just delete the commcaps array
	delete(pStruct->pSupportedCommands);
	
	//if its a CAPITAL or OUTPOST, delete its resource collection array
	delete [] pStruct->pResourceCollection;

	delete(pStruct->Pos_array);

	//delete the structure itself
	delete(pStruct);
	
	//relink Prev and Next
	pPrev->pNext = (void*)pNext;
	pNext->pPrev = (void*)pPrev;




}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructListA::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cStructListA.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructListA::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_STRUCTLIST >= 1)
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
void cStructListA::WriteLog_Enter()
{
if (DEBUG + DEBUG_STRUCTLIST >= 1)
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
void cStructListA::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructIDTable::Init(VOID* pCA)
{
	pCAccess = pCA;

	//allocate
	pTable = new cStructureA*[STRUCT_NUMIDS];
	ZeroMemory(pTable, sizeof(cStructureA*)*STRUCT_NUMIDS);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructIDTable::Exit()
{
	
	//delete
	delete(pTable);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructIDTable::Register_Struct(cStructureA* pStruct, int iID)
{

	pTable[iID] = pStruct;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructIDTable::Unregister_Struct(int iID)
{

	pTable[iID] = NULL;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructIDTable::Unregister_All()
{

	for (int c23 = 0; c23 < STRUCT_NUMIDS; c23++)
	{
		Unregister_Struct(c23);
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructListA::Process_Structures()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int iRetVal;
	cStructureA* pSL_Trav = (cStructureA*)pSL_First->pNext;
	cStructureA* pSL_Trav2;

	while (pSL_Trav != pSL_Last)
	{
		pSL_Trav2 = (cStructureA*)pSL_Trav->pNext;

		//unregister the structure from the gameworld during the run cycle
		//NOTE! structures don't move...so there is no need to do this...
//DEBUG
Write_Log("[Process Structures]Unregistering from GameWorld");
WriteLog_Enter();
		//pCA->pGameWorld->Unregister_Structure(pSL_Trav->Pos_primary, pSL_Trav);

//DEBUG
Write_Log("[Process Structures]Running cycle");
WriteLog_Enter();
		iRetVal = pSL_Trav->Run_Cycle();
		
		if (iRetVal == 0)
		{
//DEBUG
Write_Log("[Process Structures]Destroying Structure");
WriteLog_Enter();
			//generate explosion events for each tile
				//post destruction event
				int iVol = pCA->pDA_Audio->Determine_Standard_Volume(pSL_Trav->Pos_primary.x, pSL_Trav->Pos_primary.y);
				pCA->pEventBuffer->Add_Event(ANIMTYPE_UEXPL01, pCA->pTimer->iCurrTick + 1, SOUNDSEG_UEXPL01, iVol, 6, pSL_Trav->Pos_primary.x * TILE_SIZE, pSL_Trav->Pos_primary.y * TILE_SIZE);
				//then for each of the other tiles, generate explosion graphics only
				for (int lc2 = 0; lc2 < pSL_Trav->iNumArrayEntries; lc2++)
				{
					pCA->pEventBuffer->Add_Event(ANIMTYPE_UEXPL01, pCA->pTimer->iCurrTick + 1, -1, iVol, 6, pSL_Trav->Pos_array[lc2].x * TILE_SIZE, pSL_Trav->Pos_array[lc2].y * TILE_SIZE);
				}

			Destroy_Structure_byaddr(pSL_Trav);

		}
		//		else  //structures don't move, there is no need to do this
		//	pCA->pGameWorld->Register_Structure(pSL_Trav->Pos_primary, pSL_Trav);

		pSL_Trav = pSL_Trav2;

	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructureA::Init_Extractor()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iSilicon = 0;

	pCA->pChatBuffer->Add_CBNode("Init extractor", 60);

	//go through it's possessed squares and check whether it's sitting over any specials
	int iSpecialTypeFound = -1;

	if ( pCA->pGameWorld->pMap[Pos_primary.index].iDrawFrame_Layer2 > 30)
		iSpecialTypeFound = pCA->pGameWorld->pMap[Pos_primary.index].iDrawFrame_Layer2;

	if (iSpecialTypeFound < 0)
	for (int lc1 = 0; lc1 < iNumArrayEntries; lc1++)
	{
		if ( pCA->pGameWorld->isNextToOcean(Pos_array[lc1].index) == TRUE)
			iSilicon++;

		if ( pCA->pGameWorld->pMap[Pos_array[lc1].index].iDrawFrame_Layer2 > 30)
		{
			iSpecialTypeFound = pCA->pGameWorld->pMap[Pos_array[lc1].index].iDrawFrame_Layer2;
			break;
		}
	}

	if (iSpecialTypeFound == 31) //uranium
	{
	pCA->pChatBuffer->Add_CBNode("Setting up Uranium", 60);

		ExtractionInfo.iExtractionMaterialType = MATERIAL_ELEMENT_U;
		ZeroMemory(ProcessText, sizeof(char)* 80);
		strcat(ProcessText, pCA->pMaterialInfoTable->Types[MATERIAL_ELEMENT_U].Name);
	}	else if (iSilicon > 0)  //return silicon on extractor
			{
				ExtractionInfo.iExtractionMaterialType = MATERIAL_ELEMENT_L;
				ZeroMemory(ProcessText, sizeof(char)* 80);
				strcat(ProcessText, pCA->pMaterialInfoTable->Types[MATERIAL_ELEMENT_L].Name);
			}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructureA::Process_Extractor()
{

if (ExtractionInfo.iExtractionMaterialType >= 0)
{
	Process_ResourceProcessing_Simple(-1, 0,
									  -1, 0,
									  0,
									  ExtractionInfo.iExtractionMaterialType, 1,
									  -1, -1,
									  0, 1000);
}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructureA::Process_Reactor()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	Process_ResourceProcessing_Simple(MATERIAL_ELEMENT_C, 1,
									  -1, -1,
									  0,
									  -1, -1,
									  -1, -1,
									  100, 
									  1000);

	return;	

	cStorageModule* pTakeFrom = NULL;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cStructureA* cStructureA::GetStructureFromID(int iID)
{	
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	if (iID >= 0)
	{
		return pCA->pStructIDTable->pTable[iID];
	}
	return NULL;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructureA::Process_Forge()
{
	Process_ResourceProcessing_Simple(MATERIAL_ELEMENT_C, 1,
									  MATERIAL_ELEMENT_I, 1,
									  0,
									  MATERIAL_ELEMENT_S, 1,
									  -1, -1,
									  0, 1000);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructureA::Process_ResourceProcessing_Simple(int iTakeMaterial1, 
													int iTakeMaterial1Amt,
													int iTakeMaterial2, 
													int iTakeMaterial2Amt,
													double ddTakeEnergy, 
													int iProduceMaterial1,
													int iProduceMaterial1Amt,
													int iProduceMaterial2,
													int iProduceMaterial2Amt,
													double ddProduceEnergy,
													int iProcessTime)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;


		//if structure is OFF, keep current state
		if (bOn == FALSE)
		for (int lc45 = 0; lc45 < NUM_MATERIAL_TYPES; lc45++)
		{
			iLastAward[lc45] += TEMP_DISPERSE_MATERIALS_INTERVAL;
		}


	cStorageModule* pTakeFrom1 = NULL;
	cStorageModule* pTakeFrom2 = NULL;
	cStorageModule* pPutInto1 = NULL;
	cStorageModule* pPutInto2 = NULL;

	//either in the take or produce process, some material must be involved (energy - energy only exchange is...trivial)
	//this function is currently relying on checking what the structure is "chewing on" to determine when to initiate a transaction (05.28.2007)
	//therefore I have to find a material type that is not "invalid" to check
	int iCheckIndex = -1;
	if (iTakeMaterial1 >= 0)
	{
		iCheckIndex = iTakeMaterial1;
	}
	if (iTakeMaterial2 >= 0)
	{
		iCheckIndex = iTakeMaterial2;
	}
	if (iProduceMaterial1 >= 0)
	{
		iCheckIndex = iProduceMaterial1;
	}
	if (iProduceMaterial2 >= 0)
	{
		iCheckIndex = iProduceMaterial2;
	}
	if (iCheckIndex < 0)
	{
		return; //otherwise we'll get a crash on the following lines
	}

	//check whether it's time to perform a transaction 
	//enter this loop only IF there is material at the checkindex
	if (  (pCA->pTimer->iCurrTick - iLastAward[iCheckIndex]) > iProcessTime)
	if (pResourceCollection[iCheckIndex] > 0)
	{
		if (iProduceMaterial1 >= 0)
			pPutInto1 = pCA->pPathFinder->Find_ConnectedTank( Pos_primary.index, iProduceMaterial1, 0, FINDTANKMODE_PUTINTO, 0);
		if (iProduceMaterial2 >= 0)
			pPutInto2 = pCA->pPathFinder->Find_ConnectedTank( Pos_primary.index, iProduceMaterial2, 0, FINDTANKMODE_PUTINTO, 0);
		
		//reset to the brink
		for (int lc45 = 0; lc45 < NUM_MATERIAL_TYPES; lc45++)
		{
			iLastAward[lc45] = pCA->pTimer->iCurrTick - iProcessTime;
		}
		//enter this loop IF we found a tank to put the material into OR we didn't need to check this index)
		if (pPutInto1 != NULL || iProduceMaterial1 < 0)
		{
			//same with this second loop
			if (pPutInto2 != NULL || iProduceMaterial2 < 0)
			{
				//now add if available
				if (pPutInto1 != NULL)
				{
					pPutInto1->ddMaterialStored += iProduceMaterial1Amt;
					pPutInto1->iMaterialType = iProduceMaterial1;
					if (pPutInto1->ddMaterialStored > pPutInto1->ddMaterialCapacity)
						pPutInto1->ddMaterialStored = pPutInto1->ddMaterialCapacity;
				}
				if (pPutInto2 != NULL)
				{
					pPutInto2->ddMaterialStored += iProduceMaterial2Amt;
					pPutInto2->iMaterialType = iProduceMaterial2;
					if (pPutInto2->ddMaterialStored > pPutInto2->ddMaterialCapacity)
						pPutInto2->ddMaterialStored = pPutInto2->ddMaterialCapacity;
				}
				//perform energy addition
				pCA->pNationManager->ppNations[iControl]->ddResources += ddProduceEnergy;
				//if we get here, reset, and award the materials, reset the structure resource collection
				if (iProduceMaterial1 >= 0)
					pResourceCollection[iProduceMaterial1] = 0;
				if (iProduceMaterial2 >= 0)
					pResourceCollection[iProduceMaterial2] = 0;
				if (iTakeMaterial1 >= 0)
					pResourceCollection[iTakeMaterial1] = 0;
				if (iTakeMaterial2 >= 0)
					pResourceCollection[iTakeMaterial2] = 0;
			}
		}
		//notice that if we didn't find both "put" tanks, the structure will not continue functioning
		//until it can find somewhere to put the material

		return;
	}

	//if we aren't processing start processing (or try)
	if (pResourceCollection[iCheckIndex] <= 0)
	{
		if (iTakeMaterial1 >= 0)
			pTakeFrom1 = pCA->pPathFinder->Find_ConnectedTank( Pos_primary.index, iTakeMaterial1, iTakeMaterial1Amt, FINDTANKMODE_TAKEFROM, 0);

		if (iTakeMaterial2 >= 0)
			pTakeFrom2 = pCA->pPathFinder->Find_ConnectedTank( Pos_primary.index, iTakeMaterial2, iTakeMaterial2Amt, FINDTANKMODE_TAKEFROM, 0);

		//enter this loop IF we found a tank to take material from OR the index was invalid
		if (pTakeFrom1 != NULL || iTakeMaterial1 < 0)
		if (pTakeFrom2 != NULL || iTakeMaterial2 < 0)
		{
				//now subtract if available
				if (pTakeFrom1 != NULL)
				{
					pTakeFrom1->ddMaterialStored -= iTakeMaterial1Amt;
					pTakeFrom1->iMaterialType = iTakeMaterial1;
					if (pTakeFrom1->ddMaterialStored < 0)
						pTakeFrom1->ddMaterialStored = 0;
				}
				//now subtract if available
				if (pTakeFrom2 != NULL)
				{
					pTakeFrom2->ddMaterialStored -= iTakeMaterial2Amt;
					pTakeFrom2->iMaterialType = iTakeMaterial2;
					if (pTakeFrom2->ddMaterialStored < 0)
						pTakeFrom2->ddMaterialStored = 0;
				}
				//perform energy deduction
				pCA->pNationManager->ppNations[iControl]->ddResources -= ddTakeEnergy;
				//set award time and increment internal resource collections
				if (iTakeMaterial1 >= 0)
				{
					iLastAward[iTakeMaterial1] = pCA->pTimer->iCurrTick;
					pResourceCollection[iTakeMaterial1] = iTakeMaterial1Amt;
				}
				if (iTakeMaterial2 >= 0)
				{
					iLastAward[iTakeMaterial2] = pCA->pTimer->iCurrTick;
					pResourceCollection[iTakeMaterial2] = iTakeMaterial2Amt;
				}
				if (iProduceMaterial1 >= 0)
				{
					iLastAward[iProduceMaterial1] = pCA->pTimer->iCurrTick;
					pResourceCollection[iProduceMaterial1] = iProduceMaterial1Amt;
				}
				if (iProduceMaterial2 >= 0)
				{
					iLastAward[iProduceMaterial2] = pCA->pTimer->iCurrTick;
					pResourceCollection[iProduceMaterial2] = iProduceMaterial2Amt;
				}
		}

	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructureA::Process_Refinery()
{
	Process_ResourceProcessing_Simple(MATERIAL_ELEMENT_C, 1,
									  -1,-1,
									  0,
									  MATERIAL_ELEMENT_A, 1,
									  MATERIAL_ELEMENT_P, 1,
									  0, 1000);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cStructureA::Run_Cycle()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iRetVal = 1;

		//if the unit HP dropped below 0, flag to destroy
	if (iHP <= 0)
		return 0;

PCMD2* pCmd2Trav;

cStructInfo* pStructInfo = pCA->pStructInfoTable->pTable[iType];

//don't process if not complete
if (fCompletion >= 1)
{

	if (iType == TEMP_STYPE_REACTOR)
	if (pCA->pTimer->iCurrTick % TEMP_DISPERSE_MATERIALS_INTERVAL == 0)
			Process_Reactor();
	if (iType == TEMP_STYPE_REFINERY)
	if (pCA->pTimer->iCurrTick % TEMP_DISPERSE_MATERIALS_INTERVAL == 0)
		Process_Refinery();
	if (iType == TEMP_STYPE_FORGE)
	{
	if (pCA->pTimer->iCurrTick % TEMP_DISPERSE_MATERIALS_INTERVAL == 0)
		Process_Forge();
	}
	if (iType == TEMP_STYPE_EXTRACTOR)
	if (pCA->pTimer->iCurrTick % TEMP_DISPERSE_MATERIALS_INTERVAL == 0)
			Process_Extractor();
}

if (bOn == TRUE && fCompletion >= 1.0)
{
	//process the Order Queue if this structure can do so
	if (pStructInfo->bActiveOrderQueue == TRUE)
	{
		//get the first order in the queue
		cProgressiveOrder* pPO_Trav = (cProgressiveOrder*)OrderQueue.pPO_First->pNext;

		//if there are orders, process
		if (pPO_Trav != OrderQueue.pPO_Last)
		{

			if (pPO_Trav->fCompletion <= 0)
			{

				pCmd2Trav = pPO_Trav->pCmd2_Init;

				while (pCmd2Trav != NULL)
				{
					//pCA->pCommQueue->Issue_Command_Q2(pCmd2Trav->wType, pCmd2Trav->pData, pCmd2Trav->wDataSize, TRUE);
					//try simply executing from here....

					pCA->pCommQueue->Execute_Command2(pCmd2Trav);
					pCmd2Trav = pCmd2Trav->pNext;
				}

			}
				//and always do the progress command

				pCmd2Trav = pPO_Trav->pCmd2_Prog;

				while (pCmd2Trav != NULL)
				{
					//pCA->pCommQueue->Issue_Command_Q2(pCmd2Trav->wType, pCmd2Trav->pData, pCmd2Trav->wDataSize, TRUE);

					pCA->pCommQueue->Execute_Command2(pCmd2Trav);
					pCmd2Trav = pCmd2Trav->pNext;
				}


			//now if it's done, do the necessary requeueing
			if (pPO_Trav->fCompletion >= 1.0)
			{

				pCmd2Trav = pPO_Trav->pCmd2_Exit;

				while (pCmd2Trav != NULL)
				{
					//pCA->pCommQueue->Issue_Command_Q2(pCmd2Trav->wType, pCmd2Trav->pData, pCmd2Trav->wDataSize, TRUE);

					pCA->pCommQueue->Execute_Command2(pCmd2Trav);
					pCmd2Trav = pCmd2Trav->pNext;
				}

				//now get rid of the command
				OrderQueue.Delete_PO(pPO_Trav);

			}
		}
	}
}
	
//reveal

	if (iType == TEMP_STYPE_RSC)
	{
		pCA->pGameWorld->Reveal(Pos_primary.x, Pos_primary.y, TEMP_CLAIMRADIUS_CAPITAL, iControl);

		if (pCA->pTimer->iCurrTick % TEMP_DISPERSE_MATERIALS_INTERVAL == iControl)
			{
				Disperse_Materials();
				iCollectionModifiedLast = 0;
			}

	}

	if (iType == TEMP_STYPE_OUTPOST)
	{
		pCA->pGameWorld->Reveal(Pos_primary.x, Pos_primary.y, TEMP_CLAIMRADIUS_OUTPOST, iControl);

			if (pCA->pTimer->iCurrTick % TEMP_DISPERSE_MATERIALS_INTERVAL == iControl)
			{
				Disperse_Materials();
				iCollectionModifiedLast = 0;
			}
	}

	return iRetVal;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cStructureA::Disperse_Materials()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//copy the buffer over
	double pResourceCollectionDisperseCopy[NUM_MATERIAL_TYPES];

	for (int lc4 = 0; lc4 < NUM_MATERIAL_TYPES; lc4++)
	{
		//if the structure is off, "pause" the system to adding a tick to the last award
		if ( bOn == FALSE)
		{
			//
			//pCA->pChatBuffer->Add_CBNode("Resetting...", 60);
			iLastAward[lc4] += TEMP_DISPERSE_MATERIALS_INTERVAL;
		}
		//if the structure is not completed yet, reset timer to current time
		if ( fCompletion < 1.0)
			iLastAward[lc4] = pCA->pTimer->iCurrTick;

		//disperse only if enough time has elapsed (add the 0.01 to avoid division by 0
		if ( (pCA->pTimer->iCurrTick - iLastAward[lc4]) >= (TEMP_MATERIAL_ACCUMULATE_MASTER_RATE / (pResourceCollection[lc4] + 0.01)) )
		{
			pResourceCollectionDisperseCopy[lc4] = 1;//pResourceCollection[lc4];
			iLastAward[lc4] = pCA->pTimer->iCurrTick;
		}
		else
			pResourceCollectionDisperseCopy[lc4] = 0;
	}
	//for now, just run the pather on it to see what it does...
	for (int lc1 = 0; lc1 < NUM_MATERIAL_TYPES; lc1++)
	if (pResourceCollectionDisperseCopy[lc1] > 0)
	{
		int iret = 0;

		int iCount = 0;
		cStorageModule* pModule = NULL;
		while ( pResourceCollectionDisperseCopy[lc1] > 0)
		{
				pModule = pCA->pPathFinder->Find_ConnectedTank(Pos_primary.index, lc1, 0, FINDTANKMODE_PUTINTO, 0);
				//DEBUG
				iCount++;
				if (iCount > 10)
				{
					pCA->pChatBuffer->Add_CBNode("Error: Loop Broken.", 60);
					return;
				}

				if (pModule != NULL)
				{
					//determine the amount to move
					double ddAmountToMove = pModule->ddMaterialCapacity - pModule->ddMaterialStored;

					char NText[10];
					gcvt(ddAmountToMove, 7, NText);
					pCA->pCommQueue->Write_Log(NText);
					pCA->pCommQueue->WriteLog_Enter();

					if (ddAmountToMove > pResourceCollectionDisperseCopy[lc1])
						ddAmountToMove = pResourceCollectionDisperseCopy[lc1];
					//then move
					pResourceCollectionDisperseCopy[lc1] -= ddAmountToMove;
					pModule->ddMaterialStored += ddAmountToMove;
						//
						if (pModule->bIsCarrierStructure == TRUE)
						if (pModule->iCarrierID >= 0)
						{
							cStructureA* pStruct = pCA->pStructIDTable->pTable[pModule->iCarrierID];
							pStruct->iLastMaterialDeposited = pCA->pTimer->iCurrTick;

						}
					//set types
					pModule->iMaterialType = lc1;
				} else
					break; //couldn't find anything
		}
		
		//at this point, we won't be able to unload the materials and will have to dump them
	//	if (iControl == pCA->pGameOptions->iControl)  why was this here??? (this is like guaranteeing that this only works for the human player?)
		if ((pResourceCollectionDisperseCopy[lc1] > 0))
		{
			//pCA->pChatBuffer->Add_CBNode("Structure had nowhere to put materials, had to dump.", 60);
			iLastDumped = pCA->pTimer->iCurrTick;
			iLastAward[lc1] = pCA->pTimer->iCurrTick - (TEMP_MATERIAL_ACCUMULATE_MASTER_RATE / (pResourceCollection[lc1] + 0.01));
		}

		pResourceCollectionDisperseCopy[lc1] = 0;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
