#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cCmpInfo::cCmpInfo()
{
	ddResourceCost = 0;
	ddSupport = 0;
	Name = NULL;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpInfoTable::Init(VOID* pCA)
{
	pCAccess = pCA;
if (DEBUG + DEBUG_COMPONENT_A >= 1)
	Init_LogFile();


	//NULL the table pointer because we have no components
	pCmpTable = NULL;

	//initialize ALL anchors
	pCTL_First = new cCmpInfo;
	pCTL_Last = new cCmpInfo;
	pCTL_First->pNext_cmplist = (void*)pCTL_Last;
	pCTL_Last->pPrev_cmplist = (void*)pCTL_First;
	pCTL_First->pPrev_cmplist = NULL;
	pCTL_Last->pNext_cmplist = NULL;

	//initialize values

	pFrameTypes_First = new cCmpInfo;
	pFrameTypes_Last = new cCmpInfo;
	pFrameTypes_First->pNext_classlist = (void*)pFrameTypes_Last;
	pFrameTypes_Last->pPrev_classlist = (void*)pFrameTypes_First;
	pFrameTypes_First->pPrev_classlist = NULL;
	pFrameTypes_Last->pNext_classlist = NULL;

	pEngineTypes_First = new cCmpInfo;
	pEngineTypes_Last = new cCmpInfo;
	pEngineTypes_First->pNext_classlist = (void*)pEngineTypes_Last;
	pEngineTypes_Last->pPrev_classlist = (void*)pEngineTypes_First;
	pEngineTypes_First->pPrev_classlist = NULL;
	pEngineTypes_Last->pNext_classlist = NULL;

	pDefenseTypes_First = new cCmpInfo;
	pDefenseTypes_Last = new cCmpInfo;
	pDefenseTypes_First->pNext_classlist = (void*)pDefenseTypes_Last;
	pDefenseTypes_Last->pPrev_classlist = (void*)pDefenseTypes_First;
	pDefenseTypes_First->pPrev_classlist = NULL;
	pDefenseTypes_Last->pNext_classlist = NULL;

	pWeaponTypes_First = new cCmpInfo;
	pWeaponTypes_Last = new cCmpInfo;
	pWeaponTypes_First->pNext_classlist = (void*)pWeaponTypes_Last;
	pWeaponTypes_Last->pPrev_classlist = (void*)pWeaponTypes_First;
	pWeaponTypes_First->pPrev_classlist = NULL;
	pWeaponTypes_Last->pNext_classlist = NULL;

	pToolTypes_First = new cCmpInfo;
	pToolTypes_Last = new cCmpInfo;
	pToolTypes_First->pNext_classlist = (void*)pToolTypes_Last;
	pToolTypes_Last->pPrev_classlist = (void*)pToolTypes_First;
	pToolTypes_First->pPrev_classlist = NULL;
	pToolTypes_Last->pNext_classlist = NULL;


	pStorageTypes_First = new cCmpInfo;
	pStorageTypes_Last = new cCmpInfo;
	pStorageTypes_First->pNext_classlist = (void*)pStorageTypes_Last;
	pStorageTypes_Last->pPrev_classlist = (void*)pStorageTypes_First;
	pStorageTypes_First->pPrev_classlist = NULL;
	pStorageTypes_Last->pNext_classlist = NULL;

	Reconstruct();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpInfoTable::Reconstruct()
{
//reconstructs info table based on new list data
	//delete the old table if NULL
	if (pCmpTable == NULL)
		delete(pCmpTable);

	//find the highest type ID in the list
iHighestTypeID = -1;
	
cCmpInfo* pCTL_Trav = (cCmpInfo*)pCTL_First->pNext_cmplist;

	while (pCTL_Trav != pCTL_Last)
	{
		if (pCTL_Trav->iType > iHighestTypeID)
			iHighestTypeID = pCTL_Trav->iType;
	pCTL_Trav = (cCmpInfo*)pCTL_Trav->pNext_cmplist;
	}

//allocate the necessary pointers
	pCmpTable = new cCmpInfo*[iHighestTypeID+1];
	//Zero Memory
	ZeroMemory(pCmpTable, sizeof(cCmpInfo*) * (iHighestTypeID + 1));

	//go through the list again and set links to correspodning IDs
	//(this code overwrites same-ID entries)
pCTL_Trav = (cCmpInfo*)pCTL_First->pNext_cmplist;

	while (pCTL_Trav != pCTL_Last)
	{
		pCmpTable[pCTL_Trav->iType] = pCTL_Trav;		
	pCTL_Trav = (cCmpInfo*)pCTL_Trav->pNext_cmplist;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpInfoTable::Add_FrameType_Simple(int iType, 
							 LPSTR Name, 
							 int iSize,
							 int iMass,
							 double ddResourceCost,
							 double ddSupportCost,
							 BOOL bMarkFlag,
							 int iIntegrity,
							 int iVisHardPoints,
							 float fAccelBonus,
							 float fDecelBonus,
							 int iCapacity,
							 CMPDRAWOFFSETARRAY* pOffsetArray1,
							 int iStartingIndex1,
							 int iZLevel1,
							 int iZLevelBase1,
							 CMPDRAWOFFSETARRAY* pOffsetArray2,
							 int iStartingIndex2,
							 int iZLevel2,
							 int iZLevelBase2,
							 CMPDRAWOFFSETARRAY* pOffsetArray3,
							 int iStartingIndex3,
							 int iZLevel3,
							 int iZLevelBase3,
							 CMPDRAWOFFSETARRAY* pOffsetArray4,
							 int iStartingIndex4,
							 int iZLevel4,
							 int iZLevelBase4,
							 CMPDRAWOFFSETARRAY* pOffsetArray5,
							 int iStartingIndex5,
							 int iZLevel5,
							 int iZLevelBase5,
							 float fTrav_Grassland,
							 float fTrav_Ocean,
							 float fTrav_Mountain,
							 float fTrav_Hill,
							 float fTrav_River,
							 float fTrav_Forest,
							 float fTrav_Air,
							 BYTE bMoveExtCharacteristics,
							 BYTE bDrawCharacteristics_z0,
							 BYTE bDirCharacteristics_z0,
							 int iRotateFrames_z0,
							 BOOL bValid_z0,
							 BYTE bDrawCharacteristics_z1,
							 BYTE bDirCharacteristics_z1,
							 int iRotateFrames_z1,
							 BOOL bValid_z1,
							 BOOL bReconstructFlag)
{
//local counter
	int lc1;
//allocate CmpInfo structure
	cCmpInfo* pCINew =  new cCmpInfo;
//allocate FrameInfo structure
	CMPFRAMEINFO* pFrameInfo = new CMPFRAMEINFO;

//DEBUG
Write_Log("Starting.");
WriteLog_Enter();
		//allocate and set the hard point info
	pFrameInfo->iNumVisibleHardPoints = iVisHardPoints;
	pFrameInfo->Capacity = iCapacity;
	pFrameInfo->iIntegrity = iIntegrity;
	pFrameInfo->VisHardPoints = new CMPFRAMEDRAWINFO[iVisHardPoints];
	if (iVisHardPoints >= 1)
	{
		pFrameInfo->VisHardPoints[0].pOffsets = pOffsetArray1;
		pFrameInfo->VisHardPoints[0].iStartingIndex = iStartingIndex1;
		pFrameInfo->VisHardPoints[0].iZLevel = iZLevel1;
		pFrameInfo->VisHardPoints[0].iZLevelBase = iZLevelBase1;
	}
	if (iVisHardPoints >= 2)
	{
		pFrameInfo->VisHardPoints[1].pOffsets = pOffsetArray2;
		pFrameInfo->VisHardPoints[1].iStartingIndex = iStartingIndex2;
		pFrameInfo->VisHardPoints[1].iZLevel = iZLevel2;
		pFrameInfo->VisHardPoints[1].iZLevelBase = iZLevelBase2;
	}
	if (iVisHardPoints >= 3)
	{
		pFrameInfo->VisHardPoints[2].pOffsets = pOffsetArray3;
		pFrameInfo->VisHardPoints[2].iStartingIndex = iStartingIndex3;
		pFrameInfo->VisHardPoints[2].iZLevel = iZLevel3;
		pFrameInfo->VisHardPoints[2].iZLevelBase = iZLevelBase3;
	}
	if (iVisHardPoints >= 4)
	{
		pFrameInfo->VisHardPoints[3].pOffsets = pOffsetArray4;
		pFrameInfo->VisHardPoints[3].iStartingIndex = iStartingIndex4;
		pFrameInfo->VisHardPoints[3].iZLevel = iZLevel4;
		pFrameInfo->VisHardPoints[3].iZLevelBase = iZLevelBase4;
	}
	if (iVisHardPoints >= 5)
	{
		pFrameInfo->VisHardPoints[4].pOffsets = pOffsetArray5;
		pFrameInfo->VisHardPoints[4].iStartingIndex = iStartingIndex5;
		pFrameInfo->VisHardPoints[4].iZLevel = iZLevel5;
		pFrameInfo->VisHardPoints[4].iZLevelBase = iZLevelBase5;
	}

	pFrameInfo->fAccelBonus = fAccelBonus;
	pFrameInfo->fDecelBonus = fDecelBonus;

//DEBUG
Write_Log("Setting up terrain moveability info.");
WriteLog_Enter();

	//set the terrain movability info
	pFrameInfo->MoveCharacteristics[MAPTILE_GRASSLAND] = fTrav_Grassland;
	pFrameInfo->MoveCharacteristics[MAPTILE_OCEAN] = fTrav_Ocean;
	pFrameInfo->MoveCharacteristics[MAPTILE_MOUNTAIN] = fTrav_Mountain;
	pFrameInfo->MoveCharacteristics[MAPTILE_HILL] = fTrav_Hill;
	pFrameInfo->MoveCharacteristics[MAPTILE_RIVER] = fTrav_River;
	pFrameInfo->MoveCharacteristics[MAPTILE_FOREST] = fTrav_Forest;	
	pFrameInfo->MoveCharacteristics[MAPTILE_AIR] = fTrav_Air;
	pFrameInfo->MoveExtCharacteristics = bMoveExtCharacteristics;

//add the FrameInfo to the Main Cmp Info
	pCINew->pFrameInfo = pFrameInfo;
	//NULL everything else just in case
	pCINew->pEngineInfo = NULL;
	pCINew->pDefenseInfo = NULL;
	pCINew->pWeaponInfo = NULL;
	pCINew->pToolInfo = NULL;
	pCINew->pStoreInfo = NULL;

//set type and name, resources and class
	pCINew->iType = iType;
	//pCINew->Name = Name;
	//instead of simply applying the name, copy the pointer (07.12.2004)
	pCINew->Name = new char[TEMP_STD_STRLEN];
	ZeroMemory(pCINew->Name, sizeof(char)*TEMP_STD_STRLEN);
	strcpy(pCINew->Name, Name);

//DEBUG
Write_Log(Name);
WriteLog_Enter();


	pCINew->iSize = iSize;
	pCINew->iMass = iMass;
	pCINew->iClass = CMPCLASS_FRAME;
	pCINew->ddResourceCost = ddResourceCost;
	pCINew->ddSupport = ddSupportCost;

//DEBUG
Write_Log("Setting up ZLevel 0 info.");
WriteLog_Enter();

	//set draw schemes for ZLevels 0 and 1 as specified
	pCINew->DrawSchemes[0].bDrawCharacteristics = bDrawCharacteristics_z0;
	pCINew->DrawSchemes[0].bDirCharacteristics = bDirCharacteristics_z0;
	pCINew->DrawSchemes[0].iRotateFrames = iRotateFrames_z0;
	pCINew->DrawSchemes[0].SourceGraphics.Init(pCAccess, hLogFile);
	pCINew->DrawSchemes[0].bValid = bValid_z0;
	pCINew->DrawSchemes[0].bEmFlag = bMarkFlag;

//DEBUG
Write_Log("Setting up ZLevel 1 info.");
WriteLog_Enter();

	pCINew->DrawSchemes[1].bDrawCharacteristics = bDrawCharacteristics_z1;
	pCINew->DrawSchemes[1].bDirCharacteristics = bDirCharacteristics_z1;
	pCINew->DrawSchemes[1].iRotateFrames = iRotateFrames_z1;
	pCINew->DrawSchemes[1].SourceGraphics.Init(pCAccess, hLogFile);
	pCINew->DrawSchemes[1].bValid = bValid_z1;
	pCINew->DrawSchemes[1].bEmFlag = bMarkFlag;

//DEBUG
Write_Log("Setting up hard point info.");
WriteLog_Enter();

	//in this proc, all other ZLevels are invalid
	for (lc1 = 2; lc1 < NUM_ZLEVELS; lc1++)
		pCINew->DrawSchemes[lc1].bValid = FALSE;


//NOTE: I am forgetting cost in all the cofigurations
	//TO DO: LINK EVERYTHING TOGETHER (03.01.2003)
	//link to the mainc component list and the frames list
//reference lines
cCmpInfo* pCmpList_Prev = (cCmpInfo*)pCTL_Last->pPrev_cmplist;
cCmpInfo* pClassList_Prev = (cCmpInfo*)pFrameTypes_Last->pPrev_classlist;

	//link up four-way
	pCmpList_Prev->pNext_cmplist = (void*)pCINew;
	pCTL_Last->pPrev_cmplist = (void*)pCINew;
	pCINew->pNext_cmplist = (void*)pCTL_Last;
	pCINew->pPrev_cmplist = (void*)pCmpList_Prev;
	pCINew->pNext_classlist = (void*)pFrameTypes_Last;
	pCINew->pPrev_classlist = (void*)pClassList_Prev;
	pFrameTypes_Last->pPrev_classlist = (void*)pCINew;
	pClassList_Prev->pNext_classlist = (void*)pCINew;

//initialize cSrcGraphicsInfomodule
	pCINew->DrawSchemes->SourceGraphics.Init(pCAccess, hLogFile);

//reconstruct based on boolean flag
	if (bReconstructFlag == TRUE)
		Reconstruct();



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpInfoTable::Add_StorageType_Simple(int iType, 
							 LPSTR Name, 
							 int iSize,
							 int iMass,
							 double ddResourceCost,
							 double ddSupportCost,
							 BOOL bMarkFlag,
							 int iStorageBonus,
							 double ddMaterialStorageBonus,
							 BYTE bDrawCharacteristics,
							 BYTE bDirCharacteristics,
							 int iRotateFrames,
							 BOOL bValid,
							 BOOL bReconstructFlag)
{

	//local counter
	int lc1;

cCmpInfo* pCINew = new cCmpInfo;
CMPSTORAGEINFO* pStorageInfo = new CMPSTORAGEINFO;

	//fill in information
	pStorageInfo->iStorageBonus = iStorageBonus;
	pStorageInfo->ddMaterialStorageBonus = ddMaterialStorageBonus;

//set
	pCINew->pEngineInfo = NULL;
	pCINew->pFrameInfo = NULL;
	pCINew->pDefenseInfo = NULL;
	pCINew->pWeaponInfo = NULL;
	pCINew->pToolInfo = NULL;
	pCINew->pStoreInfo = pStorageInfo;


//set main info
	pCINew->iType = iType;
	//pCINew->Name = Name;
	//instead of simply applying the name, copy the pointer (07.12.2004)
	pCINew->Name = new char[TEMP_STD_STRLEN];
	ZeroMemory(pCINew->Name, sizeof(char)*TEMP_STD_STRLEN);
	strcpy(pCINew->Name, Name);
	
	pCINew->iSize = iSize;
	pCINew->iMass = iMass;
	pCINew->iClass = CMPCLASS_STORAGE;
	pCINew->ddResourceCost = ddResourceCost;
	pCINew->ddSupport = ddSupportCost;

	//for non-frame components, we set the ZLEVEL_MIDBASE ... in the future, if a component
	//needs to draw more than one image to draw itself, it would specify ZLevels "offset" from the base to do that (would be tricky to implement though...)
	//(01.07.2004)
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bDrawCharacteristics = bDrawCharacteristics;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bDirCharacteristics = bDirCharacteristics;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].iRotateFrames = iRotateFrames;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].SourceGraphics.Init(pCAccess, hLogFile);
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bValid = bValid;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bEmFlag = bMarkFlag;

	//set all other levels as invalid
	for (lc1 = 0; lc1 < NUM_ZLEVELS; lc1++)
		if (lc1 != ZLEVEL_MIDBASE)
		pCINew->DrawSchemes[lc1].bValid = FALSE;

	//link to the mainc component list and the frames list
//reference lines
cCmpInfo* pCmpList_Prev = (cCmpInfo*)pCTL_Last->pPrev_cmplist;
cCmpInfo* pClassList_Prev = (cCmpInfo*)pStorageTypes_Last->pPrev_classlist;

	//link up four-way
	pCmpList_Prev->pNext_cmplist = (void*)pCINew;
	pCTL_Last->pPrev_cmplist = (void*)pCINew;
	pCINew->pNext_cmplist = (void*)pCTL_Last;
	pCINew->pPrev_cmplist = (void*)pCmpList_Prev;
	pCINew->pNext_classlist = (void*)pStorageTypes_Last;
	pCINew->pPrev_classlist = (void*)pClassList_Prev;
	pStorageTypes_Last->pPrev_classlist = (void*)pCINew;
	pClassList_Prev->pNext_classlist = (void*)pCINew;

//reconstruct based on boolean flag
	if (bReconstructFlag == TRUE)
		Reconstruct();
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpInfoTable::Add_EngineType_Simple(int iType, 
							 LPSTR Name, 
							 int iSize,
							 int iMass,
							 double ddResourceCost,
							 double ddSupportCost,
							 BOOL bMarkFlag,
							 int iPower,
							 BYTE bDrawCharacteristics,
							 BYTE bDirCharacteristics,
							 int iRotateFrames,
							 BOOL bValid,
							 BOOL bReconstructFlag)
{

	//local counter
	int lc1;

cCmpInfo* pCINew = new cCmpInfo;
CMPENGINEINFO* pEngineInfo = new CMPENGINEINFO;

	//fill in information
	pEngineInfo->iPower = iPower;

//set
pCINew->pEngineInfo = pEngineInfo;
	pCINew->pFrameInfo = NULL;
	pCINew->pDefenseInfo = NULL;
	pCINew->pWeaponInfo = NULL;
	pCINew->pToolInfo = NULL;
	pCINew->pStoreInfo = NULL;

//set main info
	pCINew->iType = iType;
	//pCINew->Name = Name;
	//instead of simply applying the name, copy the pointer (07.12.2004)
	pCINew->Name = new char[TEMP_STD_STRLEN];
	ZeroMemory(pCINew->Name, sizeof(char)*TEMP_STD_STRLEN);
	strcpy(pCINew->Name, Name);
	
	pCINew->iSize = iSize;
	pCINew->iMass = iMass;
	pCINew->iClass = CMPCLASS_ENGINE;
	pCINew->ddResourceCost = ddResourceCost;
	pCINew->ddSupport = ddSupportCost;

	//for non-frame components, we set the ZLEVEL_MIDBASE ... in the future, if a component
	//needs to draw more than one image to draw itself, it would specify ZLevels "offset" from the base to do that (would be tricky to implement though...)
	//(01.07.2004)
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bDrawCharacteristics = bDrawCharacteristics;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bDirCharacteristics = bDirCharacteristics;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].iRotateFrames = iRotateFrames;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].SourceGraphics.Init(pCAccess, hLogFile);
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bValid = bValid;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bEmFlag = bMarkFlag;


	//set all other levels as invalid
	for (lc1 = 0; lc1 < NUM_ZLEVELS; lc1++)
		if (lc1 != ZLEVEL_MIDBASE)
		pCINew->DrawSchemes[lc1].bValid = FALSE;

	//link to the mainc component list and the frames list
//reference lines
cCmpInfo* pCmpList_Prev = (cCmpInfo*)pCTL_Last->pPrev_cmplist;
cCmpInfo* pClassList_Prev = (cCmpInfo*)pEngineTypes_Last->pPrev_classlist;

	//link up four-way
	pCmpList_Prev->pNext_cmplist = (void*)pCINew;
	pCTL_Last->pPrev_cmplist = (void*)pCINew;
	pCINew->pNext_cmplist = (void*)pCTL_Last;
	pCINew->pPrev_cmplist = (void*)pCmpList_Prev;
	pCINew->pNext_classlist = (void*)pEngineTypes_Last;
	pCINew->pPrev_classlist = (void*)pClassList_Prev;
	pEngineTypes_Last->pPrev_classlist = (void*)pCINew;
	pClassList_Prev->pNext_classlist = (void*)pCINew;

//reconstruct based on boolean flag
	if (bReconstructFlag == TRUE)
		Reconstruct();



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpInfoTable::Add_DefenseType_Simple(int iType, 
							 LPSTR Name, 
							 int iSize,
							 int iMass,
							 double ddResourceCost,
							 double ddSupportCost,
							 BOOL bMarkFlag,
							 int iDefenseBonus,
							 BYTE bDrawCharacteristics,
							 BYTE bDirCharacteristics,
							 int iRotateFrames,
							 BOOL bValid,
							 BOOL bReconstructFlag)
{
int lc1; //local counter

cCmpInfo* pCINew = new cCmpInfo;
CMPDEFENSEINFO* pDefenseInfo = new CMPDEFENSEINFO;

pDefenseInfo->iDefenseBonus = iDefenseBonus;

//add
pCINew->pDefenseInfo = pDefenseInfo;
	pCINew->pEngineInfo = NULL;
	pCINew->pFrameInfo = NULL;
	pCINew->pWeaponInfo = NULL;
	pCINew->pToolInfo = NULL;
	pCINew->pStoreInfo = NULL;
//set
//set main info
	pCINew->iType = iType;
	//pCINew->Name = Name;
	//instead of simply applying the name, copy the pointer (07.12.2004)
	pCINew->Name = new char[TEMP_STD_STRLEN];
	ZeroMemory(pCINew->Name, sizeof(char)*TEMP_STD_STRLEN);
	strcpy(pCINew->Name, Name);
	
	pCINew->iSize = iSize;
	pCINew->iMass = iMass;
	pCINew->iClass = CMPCLASS_DEFENSE;
	pCINew->ddResourceCost = ddResourceCost;
	pCINew->ddSupport = ddSupportCost;

	//for non-frame components, we set the ZLEVEL_MIDBASE ... in the future, if a component
	//needs to draw more than one image to draw itself, it would specify ZLevels "offset" from the base to do that (would be tricky to implement though...)
	//(01.07.2004)
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bDrawCharacteristics = bDrawCharacteristics;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bDirCharacteristics = bDirCharacteristics;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].iRotateFrames = iRotateFrames;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].SourceGraphics.Init(pCAccess, hLogFile);
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bValid = bValid;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bEmFlag = bMarkFlag;

	//set all other levels as invalid
	for (lc1 = 0; lc1 < NUM_ZLEVELS; lc1++)
		pCINew->DrawSchemes[lc1].bValid = FALSE;
//link
	//link to the mainc component list and the frames list
//reference lines
cCmpInfo* pCmpList_Prev = (cCmpInfo*)pCTL_Last->pPrev_cmplist;
cCmpInfo* pClassList_Prev = (cCmpInfo*)pDefenseTypes_Last->pPrev_classlist;

	//link up four-way
	pCmpList_Prev->pNext_cmplist = (void*)pCINew;
	pCTL_Last->pPrev_cmplist = (void*)pCINew;
	pCINew->pNext_cmplist = (void*)pCTL_Last;
	pCINew->pPrev_cmplist = (void*)pCmpList_Prev;
	pCINew->pNext_classlist = (void*)pDefenseTypes_Last;
	pCINew->pPrev_classlist = (void*)pClassList_Prev;
	pDefenseTypes_Last->pPrev_classlist = (void*)pCINew;
	pClassList_Prev->pNext_classlist = (void*)pCINew;


//reconstruct based on boolean flag
	if (bReconstructFlag == TRUE)
		Reconstruct();


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpInfoTable::Add_WeaponType_Simple(int iType, 
							 LPSTR Name, 
							 int iSize,
							 int iMass,
							 double ddResourceCost,
							 double ddSupportCost,
							 BOOL bMarkFlag,
							 int iProjectileType,
							 int iFireRate,
							 int iPreAttackProcedure,
							 int iAttackProcedure,
							 float fRange,
							 BYTE bDrawCharacteristics,
							 BYTE bDirCharacteristics,
							 int iRotateFrames,
							 BOOL bValid,
							 BOOL bReconstructFlag)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;

int lc1; //local counter

cCmpInfo* pCINew = new cCmpInfo;
CMPWEAPONINFO* pWeaponInfo = new CMPWEAPONINFO;

pWeaponInfo->iProjectileType = iProjectileType;
pWeaponInfo->iPreAttackProcedure = iPreAttackProcedure;
pWeaponInfo->iAttackProcedure = iAttackProcedure;
pWeaponInfo->iFireRate = iFireRate;
pWeaponInfo->fRange = fRange;


pWeaponInfo->fDmgRate = 0;
//if it was a projectile, compute the damage rate using the projectile information
if (iAttackProcedure == ATTACK_PROJECTILE)
{
	//to prevent a division by 0
	if (pWeaponInfo->iFireRate > 0)
		pWeaponInfo->fDmgRate = double(pCA->pPTypeTable->ProjTypes[iProjectileType].iDamage) / pWeaponInfo->iFireRate;
}

//add
pCINew->pWeaponInfo = pWeaponInfo;
	pCINew->pEngineInfo = NULL;
	pCINew->pDefenseInfo = NULL;
	pCINew->pFrameInfo = NULL;
	pCINew->pToolInfo = NULL;
	pCINew->pStoreInfo = NULL;
//set
//set main info
	pCINew->iType = iType;
	//pCINew->Name = Name;
	//instead of simply applying the name, copy the pointer (07.12.2004)
	pCINew->Name = new char[TEMP_STD_STRLEN];
	ZeroMemory(pCINew->Name, sizeof(char)*TEMP_STD_STRLEN);
	strcpy(pCINew->Name, Name);
	
	pCINew->iSize = iSize;
	pCINew->iMass = iMass;
	pCINew->iClass = CMPCLASS_WEAPON;
	pCINew->ddResourceCost = ddResourceCost;
	pCINew->ddSupport = ddSupportCost;
	//for non-frame components, we set the ZLEVEL_MIDBASE ... in the future, if a component
	//needs to draw more than one image to draw itself, it would specify ZLevels "offset" from the base to do that (would be tricky to implement though...)
	//(01.07.2004)
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bDrawCharacteristics = bDrawCharacteristics;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bDirCharacteristics = bDirCharacteristics;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].iRotateFrames = iRotateFrames;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].SourceGraphics.Init(pCAccess, hLogFile);
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bValid = bValid;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bEmFlag = bMarkFlag;


	//set all other levels as invalid
	for (lc1 = 0; lc1 < NUM_ZLEVELS; lc1++)
	if (lc1 != ZLEVEL_MIDBASE)
		pCINew->DrawSchemes[lc1].bValid = FALSE;
//link
	//link to the mainc component list and the frames list
//reference lines
cCmpInfo* pCmpList_Prev = (cCmpInfo*)pCTL_Last->pPrev_cmplist;
cCmpInfo* pClassList_Prev = (cCmpInfo*)pWeaponTypes_Last->pPrev_classlist;

	//link up four-way
	pCmpList_Prev->pNext_cmplist = (void*)pCINew;
	pCTL_Last->pPrev_cmplist = (void*)pCINew;
	pCINew->pNext_cmplist = (void*)pCTL_Last;
	pCINew->pPrev_cmplist = (void*)pCmpList_Prev;
	pCINew->pNext_classlist = (void*)pWeaponTypes_Last;
	pCINew->pPrev_classlist = (void*)pClassList_Prev;
	pWeaponTypes_Last->pPrev_classlist = (void*)pCINew;
	pClassList_Prev->pNext_classlist = (void*)pCINew;

//reconstruct based on boolean flag
	if (bReconstructFlag == TRUE)
		Reconstruct();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpInfoTable::Add_ToolType_Simple(int iType, 
							 LPSTR Name, 
							 int iSize,
							 int iMass,
							 double ddResourceCost,
							 double ddSupportCost,
							 BOOL bMarkFlag,
							 int iUseRate,
							 int iPreUseProcedure,
							 int iUseProcedure,
							 float fRange,
							 BYTE bDrawCharacteristics,
							 BYTE bDirCharacteristics,
							 int iRotateFrames,
							 BOOL bValid,
							 BOOL bReconstructFlag)
{
int lc1; //local counter	

cCmpInfo* pCINew = new cCmpInfo;
CMPTOOLINFO* pToolInfo = new CMPTOOLINFO;

pToolInfo->iPreUseProcedure = iPreUseProcedure;
pToolInfo->iUseProcedure = iUseProcedure;
pToolInfo->iUseRate = iUseRate;
pToolInfo->fRange = fRange;

//add
pCINew->pToolInfo = pToolInfo;
	pCINew->pEngineInfo = NULL;
	pCINew->pDefenseInfo = NULL;
	pCINew->pWeaponInfo = NULL;
	pCINew->pFrameInfo = NULL;
	pCINew->pStoreInfo = NULL;
//set
//set main info
	pCINew->iType = iType;
	//pCINew->Name = Name;
	//instead of simply applying the name, copy the pointer (07.12.2004)
	pCINew->Name = new char[TEMP_STD_STRLEN];
	ZeroMemory(pCINew->Name, sizeof(char)*TEMP_STD_STRLEN);
	strcpy(pCINew->Name, Name);
	
	pCINew->iSize = iSize;
	pCINew->iMass = iMass;
	pCINew->iClass = CMPCLASS_TOOL;
	pCINew->ddResourceCost = ddResourceCost;
	pCINew->ddSupport = ddSupportCost;
	//for non-frame components, we set the ZLEVEL_MIDBASE ... in the future, if a component
	//needs to draw more than one image to draw itself, it would specify ZLevels "offset" from the base to do that (would be tricky to implement though...)
	//(01.07.2004)
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bDrawCharacteristics = bDrawCharacteristics;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bDirCharacteristics = bDirCharacteristics;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].iRotateFrames = iRotateFrames;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bValid = bValid;
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].SourceGraphics.Init(pCAccess, hLogFile);
	pCINew->DrawSchemes[ZLEVEL_MIDBASE].bEmFlag = bMarkFlag;


	//set all other levels as invalid
	for (lc1 = 0; lc1 < NUM_ZLEVELS; lc1++)
	if (lc1 != ZLEVEL_MIDBASE)
		pCINew->DrawSchemes[lc1].bValid = FALSE;
	//link to the mainc component list and the frames list
//reference lines
cCmpInfo* pCmpList_Prev = (cCmpInfo*)pCTL_Last->pPrev_cmplist;
cCmpInfo* pClassList_Prev = (cCmpInfo*)pToolTypes_Last->pPrev_classlist;

	//link up four-way
	pCmpList_Prev->pNext_cmplist = (void*)pCINew;
	pCTL_Last->pPrev_cmplist = (void*)pCINew;
	pCINew->pNext_cmplist = (void*)pCTL_Last;
	pCINew->pPrev_cmplist = (void*)pCmpList_Prev;
	pCINew->pNext_classlist = (void*)pToolTypes_Last;
	pCINew->pPrev_classlist = (void*)pClassList_Prev;
	pToolTypes_Last->pPrev_classlist = (void*)pCINew;
	pClassList_Prev->pNext_classlist = (void*)pCINew;


//reconstruct based on boolean flag
	if (bReconstructFlag == TRUE)
		Reconstruct();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpInfoTable::Exit()
{

//traverse and exit
	cCmpInfo* pCTL_Trav = (cCmpInfo*)pCTL_First->pNext_cmplist;
	cCmpInfo* pCTL_Trav2;

	while (pCTL_Trav != pCTL_Last)
	{
		pCTL_Trav2 = (cCmpInfo*)pCTL_Trav->pNext_cmplist;
		
		if (pCTL_Trav->pFrameInfo != NULL)
		{
			if (pCTL_Trav->pFrameInfo->VisHardPoints != NULL)
				delete(pCTL_Trav->pFrameInfo->VisHardPoints);
			delete(pCTL_Trav->pFrameInfo);
		}
		if (pCTL_Trav->pStoreInfo != NULL)
		{
			delete(pCTL_Trav->pStoreInfo);
		}
		if (pCTL_Trav->pEngineInfo != NULL)
		{
			delete(pCTL_Trav->pEngineInfo);
		}
		if (pCTL_Trav->pDefenseInfo != NULL)
		{
			delete(pCTL_Trav->pDefenseInfo);
		}		
		if (pCTL_Trav->pWeaponInfo != NULL)
		{
			delete(pCTL_Trav->pWeaponInfo);
		}	
		if (pCTL_Trav->pToolInfo != NULL)
		{
			delete(pCTL_Trav->pToolInfo);
		}	
//NOTE: VC6 debugger says that deleting one of the type nodes is illegal(invalid heap pointer), and it is currently inexplicable, so we are going to compile with release(06.01.2003)
	if (pCTL_Trav != NULL)
		delete(pCTL_Trav);
		pCTL_Trav = pCTL_Trav2;
	}
//delete all anchors
//	delete(pCTL_First);
//	delete(pCTL_Last);
//	delete(pFrameTypes_First);
//	delete(pFrameTypes_Last);
//	delete(pEngineTypes_First);
//	delete(pEngineTypes_Last);
//	delete(pDefenseTypes_First);
//	delete(pDefenseTypes_Last);
//	delete(pWeaponTypes_First);
//	delete(pWeaponTypes_Last);
//	delete(pToolTypes_First);
//	delete(pToolTypes_Last);


//delete the array
	delete(pCmpTable);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpInfoTable::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cCmpInfoTable.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpInfoTable::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_COMPONENT_A >= 1)
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
void cCmpInfoTable::WriteLog_Enter()
{
if (DEBUG + DEBUG_COMPONENT_A >= 1)
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
void cCmpInfoTable::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpInfoTable::Diag_Output()
{

//DEBUG
Write_Log("Starting Diagnostic of Component List");
WriteLog_Enter();
	//for now, simply output the names of the components in the list(03.01.2003)
	cCmpInfo* pCTL_Trav = (cCmpInfo*)pCTL_First->pNext_cmplist;
//DEBUG
Write_Log("Step 1");
WriteLog_Enter();

	while (pCTL_Trav != pCTL_Last)
	{
//DEBUG
Write_Log("Sttep 2");
WriteLog_Enter();

		Write_Log(pCTL_Trav->Name);
		WriteLog_Enter();

		pCTL_Trav = (cCmpInfo*)pCTL_Trav->pNext_cmplist;
	}

//DEBUG
Write_Log("Step 3");
WriteLog_Enter();

	//do an indexed one as well:
	char NText[50];

	for (int c16 = 0; c16 <= iHighestTypeID; c16++)
	{
		if (pCmpTable[c16] != NULL)
		{
			itoa(pCmpTable[c16]->iType, NText, 10);
			Write_Log(NText);
			Write_Log(": ");
			Write_Log(pCmpTable[c16]->Name);
			WriteLog_Enter();
		}
	}

//DEBUG
Write_Log("Finished Diagnostic of Component List");
WriteLog_Enter();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cComponentListA::Init(VOID* pCA, LPSTR LogFileName)
{
	pCAccess = pCA;

	cClassAccess* pCA2 = (cClassAccess*)pCA;

	iNumComponents = 0;

	int lc1; //local counter

char TName[TEMP_STD_STRLEN];
ZeroMemory(TName, sizeof(char)*TEMP_STD_STRLEN);
strcat(TName, "./debuglogs/");
strcat(TName, LogFileName);
strcat(TName, ".txt");

if (DEBUG + DEBUG_COMPONENT_A >= 1)
	Init_LogFile(TName);

//DEBUG
Write_Log("Allocating Anchors.");
WriteLog_Enter();
	//initialize anchors. all of them
	pCmpList_First = new cComponentA;
	pCmpList_Last = new cComponentA; 
//at the moment, these are currently unnecessary (06.12.2003)
//	pFrames_First = new cComponentA;
//	pFrames_Last = new cComponentA;
//	pEngines_First = new cComponentA;
//	pEngines_Last = new cComponentA;
//	pDefenses_First = new cComponentA;
//	pDefenses_Last = new cComponentA;
//	pWeapons_First = new cComponentA;
//	pWeapons_Last = new cComponentA;
//	pTools_First = new cComponentA;
//	pTools_Last = new cComponentA;
	
//DEBUG
Write_Log("Linking Anchors.");
WriteLog_Enter();

	pCmpList_First->pNext_list = (void*)pCmpList_Last;
	pCmpList_Last->pPrev_list = (void*)pCmpList_First;
	pCmpList_First->pPrev_list = NULL;
	pCmpList_Last->pNext_list = NULL;

//at the moment, these are currently unnecessary (06.12.2003)
//	pFrames_First->pNext_classlist = (void*)pFrames_Last;
//	pFrames_Last->pPrev_classlist = (void*)pFrames_First;
//	pFrames_First->pPrev_classlist = NULL;
//	pFrames_Last->pNext_classlist = NULL;

//	pEngines_First->pNext_classlist = (void*)pEngines_Last;
//	pEngines_Last->pPrev_classlist = (void*)pEngines_First;
//	pEngines_First->pPrev_classlist = NULL;
//	pEngines_Last->pNext_classlist = NULL;

//	pDefenses_First->pNext_classlist = (void*)pDefenses_Last;
//	pDefenses_Last->pPrev_classlist = (void*)pDefenses_First;
//	pDefenses_First->pPrev_classlist = NULL;
//	pDefenses_Last->pNext_classlist = NULL;

//	pWeapons_First->pNext_classlist = (void*)pWeapons_Last;
//	pWeapons_Last->pPrev_classlist = (void*)pWeapons_First;
//	pWeapons_First->pPrev_classlist = NULL;
//	pWeapons_Last->pNext_classlist = NULL;

//	pTools_First->pNext_classlist = (void*)pTools_Last;
//	pTools_Last->pPrev_classlist = (void*)pTools_First;
//	pTools_First->pPrev_classlist = NULL;
//	pTools_Last->pNext_classlist = NULL;

//indexed initializations
	for (lc1 = 0; lc1 < NUM_COMPONENT_CLASSES; lc1++)
	{	
		pLByClass_First[lc1] = new cComponentA;
		pLByClass_Last[lc1] = new cComponentA;
		
		pLByClass_First[lc1]->pNext_classlist = (void*)pLByClass_Last[lc1];
		pLByClass_Last[lc1]->pPrev_classlist = (void*)pLByClass_First[lc1];
		pLByClass_First[lc1]->pPrev_classlist = NULL;
		pLByClass_Last[lc1]->pNext_classlist = NULL;
		
	}

//DEBUG
Write_Log("Initialization Done.");
WriteLog_Enter();


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cComponentListA::Exit()
{
	//go through list and kill
	cComponentA* pCmpTrav = pCmpList_First;
	cComponentA* pCmpTrav2;


	cClassAccess* pCA = (cClassAccess*)pCAccess;

//DEBUG
char NText[20];
pCA->pCmpInfoTable->Write_Log("[List Exit] Beginning Function.");
pCA->pCmpInfoTable->WriteLog_Enter();

	while (pCmpTrav != pCmpList_Last)
	{

//DEBUG
pCA->pCmpInfoTable->Write_Log("Traversing. Data Pointer: ");
itoa((int)pCmpTrav, NText, 10);
pCA->pCmpInfoTable->Write_Log(NText);
pCA->pCmpInfoTable->Write_Log(" ID: ");
itoa(pCmpTrav->iID, NText, 10);
pCA->pCmpInfoTable->Write_Log(NText);
pCA->pCmpInfoTable->Write_Log("Component Type: ");
itoa(pCmpTrav->iType, NText, 10);
pCA->pCmpInfoTable->Write_Log(NText);
pCA->pCmpInfoTable->WriteLog_Enter();

		pCmpTrav2 = (cComponentA*)pCmpTrav->pNext_list;
		delete(pCmpTrav);
		pCmpTrav = pCmpTrav2;
	}


//indexed delete
	int lc1; //local counter
	for (lc1 = 0; lc1 < NUM_COMPONENT_CLASSES; lc1++)
	{
//DEBUG
pCA->pCmpInfoTable->Write_Log("[List Exit] Deleting Component Class Anchors.");
pCA->pCmpInfoTable->WriteLog_Enter();
//DEBUG
pCA->pCmpInfoTable->Write_Log("Anchor Data Pointer: ");
itoa((int)pLByClass_First[lc1], NText, 10);
pCA->pCmpInfoTable->Write_Log(NText);
//DEBUG
pCA->pCmpInfoTable->Write_Log("Anchor Data Pointer: ");
itoa((int)pLByClass_Last[lc1], NText, 10);
pCA->pCmpInfoTable->Write_Log(NText);
		delete(pLByClass_First[lc1]);
		delete(pLByClass_Last[lc1]);
	}
//delete all the anchors
//at the moment, these are currently unnecessary (06.12.2003)
//	delete(pFrames_First);
//	delete(pFrames_Last);
//	delete(pEngines_First);
//	delete(pEngines_Last);
//	delete(pDefenses_First);
//	delete(pDefenses_Last);
//	delete(pWeapons_First);
//	delete(pWeapons_Last);
//	delete(pTools_First);
//	delete(pTools_Last);

//DEBUG
pCA->pCmpInfoTable->Write_Log("[List Exit] deleteng Anchor.");
pCA->pCmpInfoTable->WriteLog_Enter();

//DEBUG
pCA->pCmpInfoTable->Write_Log("Main Anchor Data Pointer: ");
itoa((int)pCmpList_Last, NText, 10);
pCA->pCmpInfoTable->Write_Log(NText);

delete(pCmpList_Last);

//DEBUG
pCA->pCmpInfoTable->Write_Log("[List Exit] Function Complete.");
pCA->pCmpInfoTable->WriteLog_Enter();

}

//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cComponentA* cComponentListA::Add_Component(int iType)
{
//DEBUG
Write_Log("ADD COMPONENT");
WriteLog_Enter();

	cClassAccess* pCA = (cClassAccess*)pCAccess;

//DEBUG
Write_Log(" Add Component: Allocating Component");
WriteLog_Enter();

	//allocate
	cComponentA* pCmpNew = new cComponentA;
//DEBUG
char NText[20];
Write_Log(" Add Component: Pointer alloated. Pointer: ");
itoa((int)pCmpNew, NText, 10);
Write_Log(NText);
WriteLog_Enter();

	
//DEBUG
Write_Log(" Add Component: Setting information");
WriteLog_Enter();

	itoa(iType, NText, 10);
	Write_Log(NText);
	WriteLog_Enter();

//DEBUG (check for invalid incoming types)
if (iType > pCA->pCmpInfoTable->iHighestTypeID || iType < 0)
{

	Write_Log(" WARNING 101: Invalid Type ID. Type: ");
	itoa(iType, NText, 10);
	Write_Log(NText);
	WriteLog_Enter();

	//DEBUG
	//assert(1);
}


//set 
	pCmpNew->iType = iType;
	pCmpNew->iClass = pCA->pCmpInfoTable->pCmpTable[iType]->iClass;
	pCmpNew->fEfficiency = 1.00;
	pCmpNew->llNextUse = 0;
	pCmpNew->fDir_Offset = 0; //for now(might be changeable later) 03.26.2003
	pCmpNew->iID = pCA->pCmpIDTable->iID_Last_Assigned + 1;
	//NULL the offset table (or maybe for safety, assign it the zero table!
	pCmpNew->pOffsetTable = &pCA->pCmpOffsetTable->StdDist_From_Center[0];
	//initialize to "safe" values (01.08.2004)
	pCmpNew->iStartingIndex = 0;
	pCmpNew->iZLevel_attached = ZLEVEL_MAIN;   //guarantees it's attached to something
	pCmpNew->fDir_Offset = 0;
	pCmpNew->fDir_Offset_Default = 0; 

	//must also increment the last ID assigned value as well
	pCA->pCmpIDTable->iID_Last_Assigned++;

//DEBUG
Write_Log(" Add Component: Registering Component on ID Table");
WriteLog_Enter();

	//register this component on the ID table
	pCA->pCmpIDTable->Register_Component(pCmpNew, pCmpNew->iID);

//DEBUG
Write_Log(" Add Component: Link Component to List");
WriteLog_Enter();

//link
	//link up four-way(nevermind, link 2-way only until we figure out whether
	//we want to array to individual class lists or not(03.01.2003)
	//NOTE: we do want to do it(10.11.2003)
	cComponentA* pCmpList_Prev = (cComponentA*)pCmpList_Last->pPrev_list;
	
	pCmpList_Prev->pNext_list = (void*)pCmpNew;
	pCmpList_Last->pPrev_list = (void*)pCmpNew;

	pCmpNew->pNext_list = (void*)pCmpList_Last;
	pCmpNew->pPrev_list = (void*)pCmpList_Prev;

	//perform the inclass link(10.11.2003)
	pCmpList_Prev = (cComponentA*)pLByClass_Last[pCmpNew->iClass]->pPrev_classlist;
	cComponentA* pCmpList_Next = pLByClass_Last[pCmpNew->iClass];
	
	pCmpList_Prev->pNext_classlist = (void*)pCmpNew;
	pCmpList_Next->pPrev_classlist = (void*)pCmpNew;
	pCmpNew->pNext_classlist = (void*)pCmpList_Next;
	pCmpNew->pPrev_classlist = (void*)pCmpList_Prev;


	iNumComponents++;



//DEBUG
Write_Log(" Add Component: Adding Done");
WriteLog_Enter();

	return pCmpNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cComponentListA::Destroy_Component_byaddr(cComponentA* pComponent, BOOL bLocalFlag)
{
	
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	if (bLocalFlag == TRUE)
		iNumComponents--;

	pCA->pCmpIDTable->Unregister_Component(pComponent->iID);

	cComponentA* pNext = (cComponentA*)pComponent->pNext_list;
	cComponentA* pPrev = (cComponentA*)pComponent->pPrev_list;

	pCA->pCmpIDTable->Unregister_Component(pComponent->iID);	

	delete(pComponent);

	pNext->pPrev_list = (void*)pPrev;
	pPrev->pNext_list = (void*)pNext;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cComponentListA::Destroy_Component_byID(int iID)
{

	//not implemented. components currently do not have unique IDs.(03.01.2003)

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cComponentListA::Insert_Component(cComponentA* pComponent)
{
//removes component from whatever list it was from and add it here
	cComponentA* pOldPrev = (cComponentA*)pComponent->pPrev_list;
	cComponentA* pOldNext = (cComponentA*)pComponent->pNext_list;
	cComponentA* pNewPrev = (cComponentA*)pCmpList_Last->pPrev_list;
	cComponentA* pNewNext = pCmpList_Last;

//remove from old list(and re-link)....
	if (pOldPrev != NULL)
		pOldPrev->pNext_list = (void*)pOldNext;
	if (pOldNext != NULL)
		pOldNext->pPrev_list = (void*)pOldPrev;
//add to new list(link)
	pNewPrev->pNext_list = (void*)pComponent;
	pNewNext->pPrev_list = (void*)pComponent;
	pComponent->pNext_list = (void*)pNewNext;
	pComponent->pPrev_list = (void*)pNewPrev;

	iNumComponents++;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cComponentListA::Export_Component(cComponentA* pComponent, cComponentA* pLinkTarget, BOOL bLocalFlag)
{
//after writing this function, I just realized this is essentially the same
//code as insert component, backwards. (oh well)

//removes component from whatever list it was from and add it here
	cComponentA* pOldPrev = (cComponentA*)pComponent->pPrev_list;
	cComponentA* pOldNext = (cComponentA*)pComponent->pNext_list;
	cComponentA* pNewPrev = (cComponentA*)pLinkTarget->pPrev_list;
	cComponentA* pNewNext = pLinkTarget;

//remove from old list(and re-link)....
	if (pOldPrev != NULL)
		pOldPrev->pNext_list = (void*)pOldNext;
	if (pOldNext != NULL)
		pOldNext->pPrev_list = (void*)pOldPrev;
//add to new list(link)
	if (pNewPrev != NULL)
		pNewPrev->pNext_list = (void*)pComponent;
	if (pNewNext != NULL)
	pNewNext->pPrev_list = (void*)pComponent;
	
	pComponent->pNext_list = (void*)pNewNext;
	pComponent->pPrev_list = (void*)pNewPrev;
	
	if (bLocalFlag == TRUE)
		iNumComponents--;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cComponentListA::Init_LogFile(LPSTR LogFileName)
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile(LogFileName, 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cComponentListA::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_COMPONENT_A >= 1)
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
void cComponentListA::WriteLog_Enter()
{
if (DEBUG + DEBUG_COMPONENT_A >= 1)
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
void cComponentListA::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpOffsetTable::Init()
{

	int c17;
	//right now I can get away with absolutely nothing....(03.26.2003)
	//fill in only the center (0) numbers right now because I don't want to bother with the rest right now
	//note that UNIT_NUMDIRS is still here....this is fine for now, as long as the components/units that use it actually have rotations = UNIT_NUMDIRS
	for (c17 = 0; c17 < UNIT_NUMDIRS; c17++)
	{
		StdDist_From_Center[0].XOffsets[c17] = 0;
		StdDist_From_Center[0].YOffsets[c17]= 0;
		for (int c18 = 0; c18 < 40; c18++)
		{
			double Angle = c17 * (360.0 / UNIT_NUMDIRS);

//			while (Angle >= 360)
//			{	Angle -= 360;  }
//			while (Angle < 0)
//			{	Angle += 360;  }
			//to match conventional coord system, invert value....			
			Angle = 360 - Angle;
			//then add 90
			Angle += 90;

			//then note that the y's are "inverted"...(i know this is clunky)
		StdDist_From_Center[c18].XOffsets[c17] = (int) (cos(Angle / RAD_DEG_CONV) * c18);
		StdDist_From_Center[c18].YOffsets[c17] = (int) (-sin(Angle / RAD_DEG_CONV) * c18 * 0.88);   //0.88 is roughly cos(28.125), which is the angle I have "canted" the unit views
		}
	}
	//the STANDARD8 set
	for (c17 = 0; c17 < 8; c17++)
	{
		StdDist_From_Center8[0].XOffsets[c17] = 0;
		StdDist_From_Center8[0].YOffsets[c17]= 0;
		for (int c18 = 0; c18 < 40; c18++)
		{
			double Angle = c17 * (360.0 / 8);

//			while (Angle >= 360)
//			{	Angle -= 360;  }
//			while (Angle < 0)
//			{	Angle += 360;  }
			//to match conventional coord system, invert value....			
			Angle = 360 - Angle;
			//then add 90
			Angle += 90;

			//then note that the y's are "inverted"...(i know this is clunky)
		StdDist_From_Center8[c18].XOffsets[c17] = (int) (cos(Angle / RAD_DEG_CONV) * c18);
		StdDist_From_Center8[c18].YOffsets[c17] = (int) (-sin(Angle / RAD_DEG_CONV) * c18 * 0.88);   //0.88 is roughly cos(28.125), which is the angle I have "canted" the unit views
		}
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpIDTable::Init(VOID* pCA)
{

	pCAccess = pCA;
	
	pTable = new cComponentA*[CMP_NUMIDS]; 

	int iID_Last_Assigned = -1;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpIDTable::Exit()
{
	
	delete(pTable);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpIDTable::Register_Component(cComponentA* pComponent, int iID)
{

	pTable[iID] = pComponent;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmpIDTable::Unregister_Component(int iID)
{

	pTable[iID] = NULL;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cComponentListB::Init_B(VOID* pCA, LPSTR LogFileName, int iNumComponentTypes)
{
	//call init_A
	Init(pCA, LogFileName);

	int lc1; //local counter

//DEBUG
Write_Log("Initializing TL's");
WriteLog_Enter();

	//now initialize the TLs
	ppCmpTL_First = new cComponentA*[iNumComponentTypes];
	ppCmpTL_Last = new cComponentA*[iNumComponentTypes];
	ZeroMemory(ppCmpTL_First, sizeof(cComponentA*) * iNumComponentTypes);
	ZeroMemory(ppCmpTL_Last, sizeof(cComponentA*) * iNumComponentTypes);
	
	iNumCmpTypes = iNumComponentTypes;


//DEBUG
Write_Log("Linking TL's");
WriteLog_Enter();
	//
	for (lc1 = 0; lc1 < iNumComponentTypes; lc1++)
	{

		ppCmpTL_First[lc1] = new cComponentA;
		ppCmpTL_First[lc1]->pPrev_extlink = NULL;
		ppCmpTL_First[lc1]->pNext_extlink = (void*)ppCmpTL_Last[lc1];
		ppCmpTL_First[lc1]->iID = -6996;  //invalid

		ppCmpTL_Last[lc1] = new cComponentA;
		ppCmpTL_Last[lc1]->pPrev_extlink = (void*)ppCmpTL_First[lc1];
		ppCmpTL_Last[lc1]->pNext_extlink = NULL;
		ppCmpTL_Last[lc1]->iID = -6994; //invalid
	}

//DEBUG
Write_Log("Done TL's");
WriteLog_Enter();
	Recompile_TL();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cComponentListB::Exit_B()
{
	Exit();

	int lc1; //local counter;
	//delete anchors
	for (lc1 = 0; lc1 < iNumCmpTypes; lc1++)
	{
		delete(ppCmpTL_First[lc1]);

		delete(ppCmpTL_Last[lc1]);
	}
	delete(ppCmpTL_First);
	delete(ppCmpTL_Last);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cComponentListB::Link_TL(cComponentA* pComponent)
{
	//
	cComponentA* pNext = (cComponentA*)ppCmpTL_First[pComponent->iType]->pNext_extlink;
	cComponentA* pPrev = ppCmpTL_First[pComponent->iType];

	//do not attempt to link a component to itself
	if (pNext->iID == pComponent->iID || pPrev->iID == pComponent->iID)
	{
		Write_Log("ERROR: Double-link during Link_TL attempted!");
		WriteLog_Enter();
		return;
	}


	pComponent->pNext_extlink = (void*)pNext;
	pComponent->pPrev_extlink = (void*)pPrev;
	
	pNext->pPrev_extlink = (void*)pComponent;
	pPrev->pNext_extlink = (void*)pComponent;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cComponentListB::Recompile_TL()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

//DEBUG
Write_Log("[Recompile TL's] Starting");
WriteLog_Enter();

	//first re-link the anchors to each other
	int lc1; //local counter;
	//relink anchors
	for (lc1 = 0; lc1 < iNumCmpTypes; lc1++)
	{

//DEBUG
Write_Log("[Recompile TL's]    re-linking anchor");
WriteLog_Enter();
		ppCmpTL_First[lc1]->pNext_extlink = (void*)ppCmpTL_Last[lc1];
		ppCmpTL_Last[lc1]->pPrev_extlink = (void*)ppCmpTL_First[lc1];

	}

	//go through the main list and link every thing by type (08.11.2003)
	cComponentA* pCmpTrav = (cComponentA*)pCmpList_First->pNext_list;

	while (pCmpTrav != pCmpList_Last)
	{

//DEBUG
Write_Log("[Recompile TL's] Linking TL's");
WriteLog_Enter();
		Link_TL(pCmpTrav);
		
		pCmpTrav = (cComponentA*)pCmpTrav->pNext_list;
	}


//DEBUG
Write_Log("[Recompile TL's] Setting update time");
WriteLog_Enter();
	//set the update time
	llLastRecompiledTick = pCA->pTimer->iCurrTick;
	

//DEBUG
Write_Log("[Recompile TL's] Done");
WriteLog_Enter();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
