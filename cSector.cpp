#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSectorTable::Init(void* pCA)
{

	pCAccess = pCA;

if (DEBUG + DEBUG_SECTORTABLE >= 1)
	Init_LogFile();

	//zero memory
	ZeroMemory(Sectors, sizeof(cSector*) * MAX_SECTORS);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSector::Init_TacticalInfo()
{
	TacticalInfo.iTotalStructs = 0;
	TacticalInfo.iTotalUnits = 0;
	for (int lc1 = 0; lc1 < TEMP_DP_MAXPLAYERS; lc1++)
	{
		TacticalInfo.arrNumStructs[lc1] = 0;
		TacticalInfo.arrNumUnits[lc1] = 0;
	}
	TacticalInfo.iLastUpdatedTick = 0;
	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSectorTable::Register_Sector(int iID, char* Name, int iResourceBonus)
{
cClassAccess* pCA = (cClassAccess*)pCAccess;


	//clear memory if not clear
	if (Sectors[iID] != NULL)
	{
		Unregister_Sector(iID);
	}

	//now reallocate
	Sectors[iID] = new cSector;
	//re-zero memory
	ZeroMemory(Sectors[iID], sizeof(cSector));
	
	//set pointer
	Sectors[iID]->pCAccess = pCAccess;

	//now set name and iID and resourcebonus
	Sectors[iID]->iID = iID;
	Sectors[iID]->iResourceBonus = iResourceBonus;
	Sectors[iID]->pCapital = NULL;
	Sectors[iID]->iControl = -1;

	//set other information
	Sectors[iID]->iLastParticipatedJobNumber = -1;

	Sectors[iID]->Init_TacticalInfo();
	
	//initialize link table
	Sectors[iID]->Init_LinkQueue();
	

	strcpy(Sectors[iID]->Name, Name);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSector::Init_MTList()
{
	pMTList_First = new MTILE;
	pMTList_Last = new MTILE;

	pMTList_First->iSector = iID;
	pMTList_Last->iSector = iID;

	pMTList_First->pSectorList_Prev = NULL;
	pMTList_First->pSectorList_Next = pMTList_Last;
	pMTList_Last->pSectorList_Prev = pMTList_First;
	pMTList_Last->pSectorList_Next = NULL;

	iNumMapTiles = 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSector::Exit_MTList()
{
	//don't need to de-allocate
	delete pMTList_First;
	pMTList_First = NULL;
	delete pMTList_Last;
	pMTList_Last = NULL;

	iNumMapTiles = 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSector::Add_MTile(MTILE* pTile)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iOldSector = -1;

	//unlink from where it was previously
	MTILE* pOldPrev = pTile->pSectorList_Prev;
	MTILE* pOldNext = pTile->pSectorList_Next;

	if (pOldPrev != NULL)
	{
		pOldPrev->pSectorList_Next = pOldNext;
		iOldSector = pOldPrev->iSector;
	}
	if (pOldNext != NULL)
	{
		pOldNext->pSectorList_Prev = pOldPrev;
		iOldSector = pOldNext->iSector;
	}
	if (iOldSector >= 0)
	{
		cSector* pOldSector = pCA->pSectorTable->Sectors[iOldSector];
		pOldSector->iNumMapTiles--;
	}

	//now link in
	MTILE* pPrev = pMTList_Last->pSectorList_Prev;
	MTILE* pNext = pMTList_Last;

	pTile->pSectorList_Next = pNext;
	pTile->pSectorList_Prev = pPrev;
	pNext->pSectorList_Prev = pTile;
	pPrev->pSectorList_Next = pTile;

	iNumMapTiles++;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//this function created to compensate for the fact that the players aren't yet known when sectors are actually "created".
//call this function after the participating AI players are known
void cSectorTable::Hook_SectorCreates()
{
cClassAccess* pCA = (cClassAccess*)pCAccess;

	//go through sector list
	for (int lc0 = 0; lc0 < MAX_SECTORS; lc0++)
	if ( Sectors[lc0] != NULL)
	{
		for (int lc1 = 0; lc1 < TEMP_DP_MAXPLAYERS; lc1++)
		{
			if (pCA->pNationManager->ppNations[lc1] != NULL)
			{
				pCA->pNationManager->ppNations[lc1]->AINew.Event_Sector_Created(Sectors[lc0]);
			}

		}
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSectorTable::Unregister_Sector(int iID)
{

	Sectors[iID]->Exit_LinkQueue();
	Sectors[iID]->Exit_MTList();
	//currently there's nothing to do here except simply delete the entry so....
	delete Sectors[iID];
	Sectors[iID] = NULL;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSectorTable::Exit()
{
	for (int c992 = 0; c992 < MAX_SECTORS; c992++)
	{
		if (Sectors[c992] != NULL)
			Unregister_Sector(c992);
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSectorTable::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cSectorTable.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSectorTable::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_SECTORTABLE >= 1)
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
void cSectorTable::WriteLog_Enter()
{
if (DEBUG + DEBUG_SECTORTABLE >= 1)
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
void cSectorTable::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cSectorTable::Process_Sectors()
{

cClassAccess* pCA = (cClassAccess*)pCAccess;

	//check each sector, and give the resource bonus to the owner of the sector
	for (int c989 = 0; c989 < MAX_SECTORS; c989++)
	if (Sectors[c989] != NULL)
	if (Sectors[c989]->pCapital != NULL)
	{
		
		cStructureA* pStruct = Sectors[c989]->pCapital;
		
		int iControl = pStruct->iControl;
		
		pCA->pNationManager->ppNations[iControl]->ddResources += Sectors[c989]->iResourceBonus;

			
	}

	MTILE* pMap = pCA->pGameWorld->pMap;
	int height = pCA->pGameWorld->iHeight;
	int width = pCA->pGameWorld->iWidth;
	int sector;
	int icheck;
	int index;

	//reveal map belonging to sector
	for (int cy = 0; cy < height; cy++)
	for (int cx = 0; cx < width; cx++)
	{
		index = cy*width + cx;
		sector = pMap[index].iSector;
		icheck = 1 << pCA->pSectorTable->Sectors[sector]->iControl;
		//
		pMap[index].barrRevealed |= icheck;

	}
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
