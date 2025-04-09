#include "classes.h"

//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameOptions::Init(VOID* pCA, int iCtrl, int iLtcy)
{
	pCAccess = pCA;
	iControl = iCtrl; //
	iControl = 0; //for now, maybe for all time, default: local player starts with ID 0 (07.10.2002)
	bHighestActivePlayerID = 0; 
	int lc1, lc2; //local counters
	//set latency 
	iLatency = iLtcy;
	iSectorTickCount = TEMP_DEFAULT_STC;

	bMPStatusesInUse = FALSE;

	bTutorial = FALSE; //by default

	iMapIndex = -1; //init zero map index
	bMapLocationFinalized = FALSE;
	iMapUniqueID = -1;
	strMultiplayerMapPath[0] = 0; 


	//initialize the scratch info table
	ScratchInfoTable = new PDNODE*[TEMP_DP_MAXPLAYERS];

	for (lc1 = 0; lc1 < TEMP_DP_MAXPLAYERS; lc1++)
	{
		ScratchInfoTable[lc1] = NULL;
	}

	//initialize list anchors
	pPD_First = new PDNODE;
	pPD_Last = new PDNODE;
	pPD_AIFirst = new PDNODE;
	pPD_AILast = new PDNODE;

	pPD_First->pNext = (void*)pPD_Last;
	pPD_Last->pPrev = (void*)pPD_First;
	pPD_AIFirst->pNext = (void*)pPD_AILast;
	pPD_AILast->pPrev = (void*)pPD_AIFirst;


/* //switched to linked lists (12.23.2002)
	//init the array initialize to completely empty
	for (lc1 = 0; lc1 < 8; lc1++)
	{
		Players[lc1].pvData = new PLAYERDATA;
		Players[lc1].dwDataSize = sizeof(PLAYERDATA);
		Players[lc1].pwszName = new unsigned short[TEMP_STD_STRLEN]; //80 character buffer
		Players[lc1].pwszName[0] = '-';
		Players[lc1].pwszName[1] = 0;
		Players[lc1].dwInfoFlags = DPNINFO_NAME | DPNINFO_DATA;
		Players[lc1].dwSize = sizeof(DPN_PLAYER_INFO);
	}
*/

	//initialize the local player data
	InfoPlayer_Local.pwszName = new WCHAR[TEMP_STD_STRLEN];
	ZeroMemory(InfoPlayer_Local.pwszName, sizeof(WCHAR) * TEMP_STD_STRLEN);
	InfoPlayer_Local.pvData = new PLAYERDATA;
	InfoPlayer_Local.dwDataSize = sizeof(PLAYERDATA);
		wcscat(InfoPlayer_Local.pwszName, L"Name");

		InfoPlayer_Local.dwSize = sizeof(DPN_PLAYER_INFO);
		InfoPlayer_Local.dwInfoFlags = DPNINFO_NAME | DPNINFO_DATA;

		bMusic = TRUE;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameOptions::Add_Player(WCHAR* wcsName, int iNameLength, int iControl_Requested, int iTeam_Requested, DPNID iDPNID)
{
	PDNODE* pPD_New = new PDNODE;
	
	//deep copy the name(if it exists)
	if (wcsName != NULL)
	{
		//pad the name string with an extra 0 to prevent the noticed problem
		// of "too long" names (02.08.2003)
		pPD_New->wcsName = new WCHAR[iNameLength + 1];
		memcpy(pPD_New->wcsName, wcsName, sizeof(WCHAR)*iNameLength);
		pPD_New->wcsName[iNameLength] = 0;
	}
		else
		pPD_New->wcsName = NULL;

	//set the other values
	pPD_New->iControl_Requested = iControl_Requested;
	pPD_New->iTeam_Requested = iTeam_Requested;
	pPD_New->iDPNID = iDPNID;
	pPD_New->bHaveCurrentMap = FALSE; //assume false until indicated otherwise
	pPD_New->bAIFlag = FALSE;

	//insert
	PDNODE* pPD_Prev = (PDNODE*)pPD_Last->pPrev;

	pPD_New->pNext = (void*)pPD_Last;
	pPD_New->pPrev = (void*)pPD_Prev;
	pPD_Prev->pNext = (void*)pPD_New;
	pPD_Last->pPrev = (void*)pPD_New;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameOptions::Add_AIPlayer(int iControl_Requested, int iTeam_Requested)
{
	//at the moment, not sure if anything else needs to be set other than Control and Team(12.23.2002)
	PDNODE* pPD_New = new PDNODE;

	pPD_New->iControl_Requested = iControl_Requested;
	pPD_New->iTeam_Requested = iTeam_Requested;

	pPD_New->bAIFlag = TRUE;



	//insert
	PDNODE* pPD_Prev = (PDNODE*)pPD_AILast->pPrev;

	pPD_New->pNext = (void*)pPD_AILast;
	pPD_New->pPrev = (void*)pPD_Prev;
	pPD_Prev->pNext = (void*)pPD_New;
	pPD_AILast->pPrev = (void*)pPD_New;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameOptions::Clear_PlayerList(PDNODE* pFirst, PDNODE* pLast)
{
	PDNODE* pPD_Trav1 = (PDNODE*)pFirst->pNext;
	PDNODE* pPD_Trav2 = (PDNODE*)pFirst->pNext;

cClassAccess* pCA = (cClassAccess*)pCAccess;

	while (pPD_Trav1 != pLast)
	{
		pPD_Trav2 = (PDNODE*)pPD_Trav1->pNext;

		//now kill trav1(first get rid of name if it exists
//DEBUG
pCA->pInterface->Write_Log("[GameOpts.ClearPlayerList]Attempting to clear name.");
pCA->pInterface->WriteLog_Enter();

		if (pPD_Trav1->wcsName != NULL)
		{
			delete(pPD_Trav1->wcsName);
		}
//DEBUG
pCA->pInterface->Write_Log("[GameOpts.ClearPlayerList]Name Freed. Attempting to Free node.");
pCA->pInterface->WriteLog_Enter();
		delete(pPD_Trav1);
//DEBUG
pCA->pInterface->Write_Log("[GameOpts.ClearPlayerList]Node deleted.");
pCA->pInterface->WriteLog_Enter();

		//reset
		pPD_Trav1 = pPD_Trav2;
	}

	//re-link First and Last to each other
	pFirst->pNext = (void*)pLast;
	pLast->pPrev = (void*)pFirst;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cGameOptions::Remove_AIPlayer_Team(int iTeam_Requested)
{
	PDNODE* pPD_Trav1 = (PDNODE*)pPD_AIFirst->pNext;
	PDNODE* pPD_RmPrev;
	PDNODE* pPD_RmNext;

	while (pPD_Trav1 != pPD_AILast)
	{
		if (pPD_Trav1->iTeam_Requested == iTeam_Requested)
		{
			pPD_RmPrev = (PDNODE*)pPD_Trav1->pPrev;
			pPD_RmNext = (PDNODE*)pPD_Trav1->pNext;
			if (pPD_Trav1->wcsName != NULL)
				delete(pPD_Trav1->wcsName);
			delete(pPD_Trav1);
			pPD_RmPrev->pNext = (void*)pPD_RmNext;
			pPD_RmNext->pPrev = (void*)pPD_RmPrev;
		
			return true;
		}



	}

return false;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cGameOptions::Remove_AIPlayer_Control(int iControl_Requested)
{
//identical to the "TEAM" function above, except for the control being tested (12.23.2002)
	PDNODE* pPD_Trav1 = (PDNODE*)pPD_AIFirst->pNext;
	PDNODE* pPD_RmPrev;
	PDNODE* pPD_RmNext;

	while (pPD_Trav1 != pPD_AILast)
	{
		if (pPD_Trav1->iControl_Requested == iControl_Requested)
		{
			pPD_RmPrev = (PDNODE*)pPD_Trav1->pPrev;
			pPD_RmNext = (PDNODE*)pPD_Trav1->pNext;
			if (pPD_Trav1->wcsName != NULL)
				delete(pPD_Trav1->wcsName);
			delete(pPD_Trav1);
			pPD_RmPrev->pNext = (void*)pPD_RmNext;
			pPD_RmNext->pPrev = (void*)pPD_RmPrev;
		
			return true;
		}



	}

return false;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGameOptions::Exit()
{
int lc1, lc2; //local counters

	//nothing here...yet

	//clear player and AIPlayer lists
	Clear_PlayerList(pPD_First, pPD_Last);
	Clear_PlayerList(pPD_AIFirst, pPD_AILast);
	delete(pPD_First);
	delete(pPD_Last);
	delete(pPD_AIFirst);
	delete(pPD_AILast);

	delete(ScratchInfoTable);

	//de-allocate player data
/*	for (lc1 = 0; lc1 < 8; lc1++)
	{	
		delete(Players[lc1].pwszName);
		delete(Players[lc1].pvData);
		
	}
*/

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
