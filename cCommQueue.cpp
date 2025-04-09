#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCommQueue::Init(VOID* pCA)
{
	//init the logfile
if (DEBUG + DEBUG_COMMQUEUE >= 1)
	Init_LogFile();

	//store the addresses
	pCAccess = pCA;

	//allocate memory for the two anchors
	pQFirst = new PCMD;
	pQLast = new PCMD;
	//allocate for Q2 anchors
	pQ2First = new PCMD2;
	pQ2Last = new PCMD2; 

	pQLast->wType = 789;
	
	bQueueInUse = FALSE;
	bQueue2InUse = FALSE;


	//link the first and the last to each other and set their other links to NULL
	pQFirst->pNext = pQLast;
	pQLast->pPrev = pQFirst;
	pQFirst->pPrev = NULL;
	pQLast->pNext = NULL;
	//perform the same using the pQ2 anchors
	pQ2First->pNext = pQ2Last;
	pQ2Last->pPrev = pQ2First;
	pQ2First->pPrev = NULL;
	pQ2Last->pNext = NULL;
	//set also the pData members to NULL
	pQ2First->pData = NULL;
	pQ2Last->pData = NULL;

	lNumCommands = 0;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cCommQueue::~cCommQueue()
{
	/*  //moved to exit() 07.03.2002
	
	//deallocate everything still in the command queue
	pQTr = pQFirst;

	while (pQTr != NULL)
	{
		pQTrNext = (LPPCMD)pQTr->pNext;
		delete(pQTr);
		pQTr = pQTrNext;
	}
	
	//exit the log file
	Exit_LogFile();

	//and...that's it

	*/

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCommQueue::Exit()
{
//wait and hold Queues-In-Use
	while (bQueueInUse == TRUE)
	{     }

	bQueueInUse = TRUE;

	//deallocate everything still in the command queue
	pQTr = pQFirst;

	while (pQTr != NULL)
	{
		pQTrNext = (LPPCMD)pQTr->pNext;
		delete(pQTr);
		pQTr = pQTrNext;
	}
//release QueueInUse
	bQueueInUse = FALSE;

//wait and hold Queue2InUse
	while (bQueue2InUse == TRUE)
	{    }

	bQueue2InUse = TRUE;


	//de-allocate in the Q2 Queue as well
	PCMD2* pQ2Tr = pQ2First;
	PCMD2* pQ2Next = pQ2Tr;

	while (pQ2Tr != NULL)
	{
		pQ2Next = pQ2Tr->pNext;
		//delete data insize pQ2Tr, then free pQ2Tr
		if (pQ2Tr->pData != NULL)
			delete(pQ2Tr->pData);
		delete(pQ2Tr);

		pQ2Tr = pQ2Next;
	}

//release Queue2InUse
	bQueue2InUse = FALSE;
	
	//exit the log file
	Exit_LogFile();

	//and...that's it


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCommQueue::Issue_Command(WORD wDest, WORD wType, LONG wParam1, WORD wParam2, WORD wParam3, double fParam4, double fParam5, void* pParam6, LONG lExecTick)
{

char NText[50];

cClassAccess* pCA = (cClassAccess*)pCAccess;


//wait for release if in use
	while(bQueueInUse == TRUE)
	{
		Write_Log("Waiting for Queue1 to free....");
		itoa(pCA->pTimer->iCurrTick, NText, 10);
		Write_Log(NText);
		WriteLog_Enter();
	}

//set Queue-In-Use to prevent other threads from accessing queue
	bQueueInUse = TRUE;

	//allocate memory for a new node
	LPPCMD pQNew = new PCMD;

	//safety precaution
	ZeroMemory(pQNew, sizeof(PCMD));

	//set the new nodes values to their desired quantities
	pQNew->wDest = wDest;
	pQNew->wType = wType;
	pQNew->wParam1 = wParam1;
	pQNew->wParam2 = wParam2;
	pQNew->wParam3 = wParam3;
	pQNew->fParam4 = fParam4;
	pQNew->fParam5 = fParam5;
	pQNew->pParam6 = pParam6;
	pQNew->lExecTick = lExecTick;
	//insert the node at the end of the comm queue, between the last and the next-to-last
	//start by setting the new node's links
	pQNew->pNext = (VOID*)pQLast;
	pQNew->pPrev = pQLast->pPrev;

	//set the last and next-to-last node's links
	//set a traverser to the next-to-last
	pQTr2 = (LPPCMD)pQLast->pPrev;
	pQTr2->pNext = (VOID*)pQNew;

	pQLast->pPrev = (VOID*)pQNew;


//release Queue-In-Use
	bQueueInUse = FALSE;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCommQueue::Execute_Commands()
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	HRESULT hRet;
	char NText[50];
	BOOL SkipFlag = FALSE;
	int lc44; //local counter
	int ival;
	BOOL bHaveMap = FALSE;

	cUnitA* pUnit;
	cStructureA* pStruct;

	PDNODE* pPD_Trav;

//wait and hold Queue-In-Use
	while(bQueueInUse == TRUE)
	{ 
		Write_Log("Waiting for Queue1 to free....");
		itoa(pCA->pTimer->iCurrTick, NText, 10);
		Write_Log(NText);
		WriteLog_Enter();
	}

	bQueueInUse = TRUE;

	//set the traverser to the first "real" node in the list, operate on it, remove, and proceed to the next one...
	pQTr = (LPPCMD)pQFirst->pNext;
	pQTrNext = pQTr;
	pQTrPrev = pQTr;


while (pQTr != pQLast)
{

	SkipFlag = FALSE;

	//execute whatever the command is. (this is going to be one big clusterf**k of switch and case statements
	switch(pQTr->wDest)
	{
	case CQDEST_GAMEWORLD:  //this will be the only valid destination, at least for now(02.09.2002)

		if (pQTr->lExecTick == pCA->pTimer->iCurrTick)
		{
			//get a pointer for the unit so there does not have to be so much referencing
			pUnit = pCA->pUnitTable->pUnitTable[pQTr->wParam1];
			
			switch(pQTr->wType)
			{
			case CQTYPE_MOVE:  //move command  //OBSOLETE
				//set the L4 mode to "nothing"(it doesn't set the L3 mode)
				pUnit->bMode_L4 = UMODE_L4_NOTHING;
				//execute a mode command on the selected unit to its target (first param is the UnitID, second is the target index)
				pUnit->bMode_L3 = UMODE_L3_MOVE;
					pUnit->ddAllowableBackOffDistance = 0; //unit is ordered to go EXACTLY to the target
				//set the target index
				pUnit->Pos_sqtarget.index = pQTr->wParam2;
				//calc the rest of the numbers
				pUnit->Pos_sqtarget.Calc_xy(pCA->pGameWorld->iWidth);
				//...and destroy the unit's current path list
				pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pUnit->pUnitPath);
				pUnit->pUnitPath = NULL;
			break;
			case CQTYPE_ATTACK: //attack command  //OBSOLETE

				//set L4 mode
				pUnit->bMode_L4 = UMODE_L4_ATTACK;
				//create a UATTACKINFO structure(to be safe, free any previous structure that was there
				if (pUnit->pAttackInfo != NULL)
					delete(pUnit->pAttackInfo);
				pUnit->pAttackInfo = new UNITATTACKINFO; 
				//fill it out 
				pUnit->pAttackInfo->iEnemyID = pQTr->wParam3;
				pUnit->pAttackInfo->Pos_ABSTarget.x = pQTr->fParam4;
				pUnit->pAttackInfo->Pos_ABSTarget.y = pQTr->fParam5;
				pUnit->pAttackInfo->fAttackDirection = pUnit->Calc_Direction(pQTr->fParam4 - pUnit->Pos_ABS.x, pQTr->fParam5 - pUnit->Pos_ABS.y);
				pUnit->pAttackInfo->iTargetType = pQTr->wParam2;
				//set the target location to move to
				pUnit->Pos_sqtarget.x = pQTr->fParam4 / TILE_SIZE;
				pUnit->Pos_sqtarget.y = pQTr->fParam5 / TILE_SIZE;
				pUnit->Pos_sqtarget.Calc_Index(pCA->pGameWorld->iWidth);
				//depending on what CQATTACKTYPE is, set pEnemy
				if (pQTr->wParam2 == CQATTACKTYPE_NOTHING)
					pUnit->pAttackInfo->iEnemyID = -1;
//				if (pQTr->wParam2 == CQATTACKTYPE_UNIT)
//					pUnit->pAttackInfo->pEnemy = pCA->pUnitTable->pUnitTable[pQTr->wParam3];
				//destroy unit's current path list
				pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pUnit->pUnitPath);
				pUnit->pUnitPath = NULL;
			break;
			}
		}
			else
			SkipFlag = TRUE;

	break;
	case CQDEST_INTERFACE:
		switch(pQTr->wType)
		{
		case CQTYPE_REPORTFRAME:
			pCA->pTimer->iTickReports[pQTr->wParam1] = pQTr->wParam2;
		break;
		case CQTYPE_SET_MAINMENUMODE:
			pCA->pInterface->IBoxes[IBOX_MAINMENU].iActiveGrid = pQTr->wParam1;
			//TEST: play a sound
			pCA->pDA_Audio->Play_Primary(SOUNDSEG_TEST1);
		break;
		case CQTYPE_SELECT_GAMETYPE_SINGLE: //temp as written, 07.09.2002
			pCA->pInterface->IBoxes[IBOX_MAINMENU].iActiveGrid = TEMP_IGRID_SINGLE;
			pCA->pGameOptions->bGameType = GAMETYPE_SINGLE;
			//set latency (01.02.2003)
			pCA->pGameOptions->iLatency = 1;  //to account for the fact that the command MIGHT not execute until the next tick
			//TEST: play a sound
			pCA->pDA_Audio->Play_Primary(SOUNDSEG_TEST1);
		break;
		case CQTYPE_SELECT_GAMETYPE_MULTI: //temp as written, 07.09.2002
			pCA->pInterface->IBoxes[IBOX_MAINMENU].iActiveGrid = TEMP_IGRID_MULTI;			
			pCA->pGameOptions->bGameType = GAMETYPE_MULTI;
			pCA->pGameOptions->iLatency = TEMP_DEFAULT_LATENCY;
			//TEST: play a sound
			pCA->pDA_Audio->Play_Primary(SOUNDSEG_TEST1);
		break;
		case CQTYPE_SELECT_QUIT: //temp as written, 07.09.2002

			pCA->pAppData->bRunFlag = 0;
			pCA->pAppData->bRunStage = APP_RUNSTAGE_EXIT;	
		break;
		case CQTYPE_SET_ACTIVEPROVIDER:
			pCA->pDP_Multiplay->iActiveProvider = pQTr->wParam1;
		break;
		case CQTYPE_SET_ACTIVEDEVICE:
			pCA->pDP_Multiplay->pDeviceArrays[pCA->pDP_Multiplay->iActiveProvider].wActiveDevice = pQTr->wParam1;
		break;
		case CQTYPE_SET_INPUTSTRING:
			pCA->pInterface->pKeyEdit = (char*)pQTr->pParam6;
			pCA->pInterface->dwKeyEditMaxBuffer = pQTr->wParam1;
		break;
		case CQTYPE_SET_INPUTSTRING_WIDE:
			pCA->pInterface->pKeyEdit_Wide = (WCHAR*)pQTr->pParam6;
			pCA->pInterface->dwKeyEditWideMaxBuffer = pQTr->wParam1;
		break;
		case CQTYPE_SETSERIAL_FLOWCONTROL_RTSDTR:
			pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsFlowControl = DPNA_FLOW_CONTROL_RTSDTR;
		break;
		case CQTYPE_SETSERIAL_FLOWCONTROL_XONXOFF:
			pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsFlowControl = DPNA_FLOW_CONTROL_XONXOFF;
		break;
		case CQTYPE_SETSERIAL_FLOWCONTROL_RTS:
			pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsFlowControl = DPNA_FLOW_CONTROL_RTS;
		break;
		case CQTYPE_SETSERIAL_FLOWCONTROL_DTR:
			pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsFlowControl = DPNA_FLOW_CONTROL_DTR;
		break;
		case CQTYPE_SETSERIAL_FLOWCONTROL_NONE:
			pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsFlowControl = DPNA_FLOW_CONTROL_NONE;
		break;
		case CQTYPE_SETSERIAL_PARITY_NONE:
			pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsParity = DPNA_PARITY_NONE;
		break;
		case CQTYPE_SETSERIAL_PARITY_EVEN:
			pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsParity = DPNA_PARITY_EVEN;
		break;
		case CQTYPE_SETSERIAL_PARITY_ODD:
			pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsParity = DPNA_PARITY_ODD;
		break;
		case CQTYPE_SETSERIAL_PARITY_MARK:
			pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsParity = DPNA_PARITY_MARK;
		break;
		case CQTYPE_SETSERIAL_PARITY_SPACE:
			pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsParity = DPNA_PARITY_SPACE;
		break;
		case CQTYPE_SETSERIAL_STOPBITS_ONE:
			pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsStopBits = DPNA_STOP_BITS_ONE;
		break;
		case CQTYPE_SETSERIAL_STOPBITS_ONEFIVE:
			pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsStopBits = DPNA_STOP_BITS_ONE_FIVE;
		break;
		case CQTYPE_SETSERIAL_STOPBITS_TWO:
			pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsStopBits = DPNA_STOP_BITS_TWO;
		break;
		case CQTYPE_SETSERIAL_BAUD:
			pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].dwBaud = pQTr->wParam1;
		break;
		case CQTYPE_SET_ACTIVEHOST:
			pCA->pDP_Multiplay->wSelectedHost = pQTr->wParam1;
		break;
		case CQTYPE_ENUMHOSTS_STD:
			//call enumhosts
			pCA->pDP_Multiplay->EnumHosts();
		break;
		case CQTYPE_HOST_STD:
			//call host
			hRet = pCA->pDP_Multiplay->HostSession(NULL);
			if (hRet == S_OK)
			{
				pCA->pInterface->IBoxes[IBOX_MAINMENU].iActiveGrid = TEMP_IGRID_MULTI2; //temp
				//clear any edit boxes open
				pCA->pInterface->pKeyEdit = NULL;
				pCA->pInterface->pKeyEdit_Wide = NULL;
			}
		break;
		case CQTYPE_CONNECT_STD:
			//call connect
			hRet = pCA->pDP_Multiplay->ConnectSession();
			if (hRet == S_OK)
			{
				pCA->pInterface->IBoxes[IBOX_MAINMENU].iActiveGrid = TEMP_IGRID_MULTI2; //temp
				//clear any edit boxes open
				pCA->pInterface->pKeyEdit = NULL;
				pCA->pInterface->pKeyEdit_Wide = NULL;
			}
		break;
		case CQTYPE_SELECT_DISCONNECT: //disconnect from multiplayer game and return to main menu
			//close sessionClose(0):
			pCA->pDP_Multiplay->CloseSession();
			pCA->pInterface->IBoxes[IBOX_MAINMENU].iActiveGrid = 0;
		break;
		case CQTYPE_SENDTEXT:
			pCA->pDP_Multiplay->Send_ChatText((char*)pQTr->pParam6, pCA->pGameOptions->InfoPlayer_Local.pwszName, pQTr->wParam1);
			//clear out the buffer
			ZeroMemory(pQTr->pParam6, pQTr->wParam1);
		break;
		case CQTYPE_SELECT_START:

			pCA->pTimer->Init(30000, pCA); //reinitialize timer
			pCA->pTimer->Set_CPT_FPS(60); //need this otherwise it'll get set wrong (timer frequency varies between computers)

			pCA->pAppData->bRunStage = APP_RUNSTAGE_MAININIT; //new mode 05.10.2004
 
			pCA->pDP_Multiplay->bRandKeysReceived = FALSE;
			pCA->pDP_Multiplay->bTotalPlayerCountReceived = FALSE;
			pCA->pDP_Multiplay->iTotalPlayerCount = 0;
			pCA->pDP_Multiplay->iNumNationInitsReceived = 0;
			pCA->pDP_Multiplay->iNumTimerIDInitsReceived = 0;
			pCA->pDP_Multiplay->bTotalTimerIDCountReceived = FALSE;
			pCA->pDP_Multiplay->iTotalTimerIDCount = 0;

			//if this is the host, send the message to other computres
			if (pCA->pGameOptions->bAmHost == TRUE)
				pCA->pDP_Multiplay->Send_Command(CQDEST_INTERFACE, CQTYPE_SELECT_START, pQTr->wParam1, pQTr->wParam2,pQTr->wParam3 , pQTr->fParam4, pQTr->fParam5, pQTr->pParam6, pQTr->lExecTick, FALSE, TRUE);

		break;
		case CQTYPE_SET_IDCONTROLANDTIMER:
			pCA->pGameOptions->iControl = pQTr->wParam1;
			pCA->pGameOptions->iTimerID = pQTr->wParam2;
		break;
		case CQTYPE_SET_CONSOLE_UNIT:
			pCA->pInterface->pConsole_Unit_Select = pQTr->pParam6;
		break;
		case CQTYPE_START_PENDINGCOMMAND:
			pCA->pInterface->Initiate_PendingCommand(pQTr->wParam1);
		break;
		case CQTYPE_CANCEL_PENDINGCOMMAND:
			pCA->pInterface->pPendingCommand->Exit();
			delete(pCA->pInterface->pPendingCommand);
			pCA->pInterface->pPendingCommand = NULL;
		break;
		case CQTYPE_SET_ACTIVE_IGRID:
			pCA->pInterface->IBoxes[pQTr->wParam1].iActiveGrid = pQTr->wParam2;
		break;
		case CQTYPE_SET_TOTAL_PLAYER_COUNT:
			pCA->pDP_Multiplay->bTotalPlayerCountReceived = TRUE;
			pCA->pDP_Multiplay->iTotalPlayerCount = pQTr->wParam1;
		break;
		case CQTYPE_SET_TOTAL_TIMERID_COUNT:
			pCA->pDP_Multiplay->bTotalTimerIDCountReceived = TRUE;
			pCA->pDP_Multiplay->iTotalTimerIDCount = pQTr->wParam1;
		break;
		case CQTYPE_ENABLE_PACKETPROCESSOR:  //this is not used because there is no way to "tack" this command onto the START_PENDINGCOMMAND without an overhaul of the interface(08.20.2003)
			pCA->pInterface->iScanType = pQTr->wParam1;
			pCA->pInterface->llCutOffTime = pCA->pTimer->iCurrTick;
		break;
		case CQTYPE_DISABLE_PACKETPROCESSOR:
			pCA->pInterface->iScanType = VAL_INVALID;
			pCA->pInterface->PacketProcessor.Flush_Buffer2(pCA->pInterface->PacketProcessor.pIPB_First, pCA->pInterface->PacketProcessor.pIPB_Last);
			pCA->pInterface->llCutOffTime = VAL_INVALID;
		break;
		case CQTYPE_DISABLE_ISSUE_PP:
			pCA->pInterface->PacketProcessor.Issue_Group_Packet(pCA->pInputPacketBuffer, pQTr->wParam1);
			pCA->pInterface->iScanType = VAL_INVALID;
			pCA->pInterface->PacketProcessor.Flush_Buffer2(pCA->pInterface->PacketProcessor.pIPB_First, pCA->pInterface->PacketProcessor.pIPB_Last);
			pCA->pInterface->llCutOffTime = VAL_INVALID;
		break;
		case CQTYPE_FLUSH_PP:
			pCA->pInterface->PacketProcessor.Flush_Buffer2(pCA->pInterface->PacketProcessor.pIPB_First, pCA->pInterface->PacketProcessor.pIPB_Last);
		break;
		case CQTYPE_SELECT_MAP:
			pCA->pGameOptions->iMapIndex = pQTr->wParam1;
			pCA->pInterface->Update_MainMenu(TEMP_IGRID_SINGLE);
			ZeroMemory(pCA->pGameOptions->strMultiplayerMapPath, sizeof(char)*256);
			strcat(pCA->pGameOptions->strMultiplayerMapPath, pCA->pAppData->achMapNames[pCA->pGameOptions->iMapIndex]);
			//get the map ID as well
			pCA->pGameOptions->iMapUniqueID = pCA->pGameWorld->UniqueMapIDOf(pCA->pGameOptions->strMultiplayerMapPath);
			//and set that YOU have this map
			pCA->pGameOptions->bMapLocationFinalized = TRUE;
		break;
		case CQTYPE_SELECT_MAP_MULTI:
			//pCA->pGameOptions->iMapIndex = pQTr->wParam1; (wait until reception of SET_MAP_MULTI command to do this)
			pCA->pInterface->Update_MainMenu(TEMP_IGRID_MULTI2);
			pCA->pDP_Multiplay->Send_Command( CQDEST_INTERFACE, CQTYPE_REQUEST_MAP, pCA->pAppData->achMapIDs[pCA->pGameOptions->iMapIndex], 0, 0, 0, 0, NULL, 0, FALSE, TRUE); 
			pCA->pGameOptions->iNumPlayersWithMap = 0;
		break;
		case CQTYPE_REQUEST_MAP:
			bHaveMap = FALSE;
			//immediately check for map, and then send back a confirmation/rejection command to the host
			for (lc44 = 0; lc44 <= pCA->pAppData->iHighestMapIndex; lc44++)
			{
				if ( pCA->pAppData->achMapIDs[lc44] == pQTr->wParam1)
					bHaveMap = TRUE;
			}
			//I would plan to have each player tag this send with their own DPNID, but at the moment it turns out that each player
			//does not know it's own DPNID! This glaring lack of information does not bode well for extensibility of function
			//and signals the need for rewriting for a more robust information system(one that does not need GetPeerInfo...)
			//for the moment however, we must "fix" the send return request to not send the DPNID (05.15.2004)
			//THIRD parameter wParam3 holds the ID of the map
			if (bHaveMap == TRUE)
				pCA->pDP_Multiplay->Send_Command(CQDEST_INTERFACE, CQTYPE_RETURN_REQUEST_MAP, 1, 0, pQTr->wParam1, 0, 0, NULL, 0, FALSE, TRUE);
					else
				pCA->pDP_Multiplay->Send_Command(CQDEST_INTERFACE, CQTYPE_RETURN_REQUEST_MAP, 0, 0, pQTr->wParam1, 0, 0, NULL, 0, FALSE, TRUE);
		break;
		case CQTYPE_RETURN_REQUEST_MAP:
			/*
			pPD_Trav = (PDNODE*)pCA->pGameOptions->pPD_First->pNext;		
		
			while (pPD_Trav != pCA->pGameOptions->pPD_Last)
			{
				if (pPD_Trav->iDPNID == pQTr->wParam2)
				{
					pPD_Trav->bHaveCurrentMap = (BOOL)pQTr->wParam1;
				}

				pPD_Trav = (PDNODE*)pPD_Trav->pNext;
			}
			*/
			//TEMP (above disabled until DP routines rewritten)
			//(all other players other than host ignore this command)
			if (pCA->pGameOptions->bAmHost == TRUE)
			{
				pCA->pGameOptions->iNumPlayersWithMap++;
			}
			//if all players now have the map, send out a SET_MAP_MULTI command (the -1 because the host did not participate in the check)
			if (pCA->pGameOptions->iNumPlayersWithMap >= pCA->pGameOptions->iNumPlayers - 1 && pCA->pGameOptions->bAmHost == TRUE)
			{
				pCA->pDP_Multiplay->Send_Command( CQDEST_INTERFACE, CQTYPE_SET_MAP_MULTI, pQTr->wParam3, 0, 0, 0, 0, NULL, 0, TRUE, TRUE);
			}
		break;
		case CQTYPE_SET_MAP_MULTI:
			//actually set the map
			pCA->pGameOptions->iMapIndex = pQTr->wParam1;

		break;
		case CQTYPE_SELECT_UNIT:
			pUnit = pCA->pUnitTable->pUnitTable[pQTr->wParam1];
			pCA->pInterface->ReleaseUnits_SB_all(pCA->pInterface->pUSB_First, pCA->pInterface->pUSB_Last);
			pCA->pInterface->ReleaseStructs_SB_all(pCA->pInterface->pSSB_First, pCA->pInterface->pSSB_Last);
			pCA->pInterface->pConsole_Unit_Select = NULL;
			pCA->pInterface->pConsole_Struct_Select = NULL;
			pCA->pInterface->AddUnits_SB_singleB(pQTr->wParam1, pCA->pInterface->pUSB_First, pCA->pInterface->pUSB_Last);

			pCA->pInterface->map_xview = pUnit->Pos_sqcurr.x - pCA->pGDisplay->MDRAW_NUMCOLS / 2;
			pCA->pInterface->map_yview = pUnit->Pos_sqcurr.y - pCA->pGDisplay->MDRAW_NUMROWS / 2;
			if (pCA->pInterface->map_xview < 0)
				pCA->pInterface->map_xview = 0;
			if (pCA->pInterface->map_yview < 0)
				pCA->pInterface->map_yview = 0;

		break;
		case CQTYPE_SELECT_STRUCT:
			pStruct = pCA->pStructIDTable->pTable[pQTr->wParam1];
			pCA->pInterface->ReleaseUnits_SB_all(pCA->pInterface->pUSB_First, pCA->pInterface->pUSB_Last);
			pCA->pInterface->ReleaseStructs_SB_all(pCA->pInterface->pSSB_First, pCA->pInterface->pSSB_Last);
			pCA->pInterface->pConsole_Unit_Select = NULL;
			pCA->pInterface->pConsole_Struct_Select = NULL;
			pCA->pInterface->AddStructs_SB_singleB(pQTr->wParam1, pCA->pInterface->pSSB_First, pCA->pInterface->pSSB_Last);

			pCA->pInterface->map_xview = pStruct->Pos_primary.x - pCA->pGDisplay->MDRAW_NUMCOLS / 2;
			pCA->pInterface->map_yview = pStruct->Pos_primary.y - pCA->pGDisplay->MDRAW_NUMROWS / 2;
			if (pCA->pInterface->map_xview < 0)
				pCA->pInterface->map_xview = 0;
			if (pCA->pInterface->map_yview < 0)
				pCA->pInterface->map_yview = 0;
			pCA->pInterface->pConsole_Struct_Select = pStruct;

		break;
		case CQTYPE_POST_ACK_U:

			pUnit = pCA->pUnitTable->pUnitTable[pQTr->wParam2];
									/*
									char Buffer[50];
									ZeroMemory(Buffer, sizeof(char)*50);
									strcat(Buffer, "Ack: ");
									char NText[50];
									itoa(pQTr->wParam1, NText, 10);
									strcat(Buffer, NText);
									strcat(Buffer, " Personality: ");

									if (pUnit != NULL) 
									{
										itoa(pUnit->iPersonality, NText, 10);
										strcat(Buffer, NText);
									}
									pCA->pChatBuffer->Add_CBNode(Buffer, 150);
									*/

			if (pUnit != NULL)
			{
				if (pUnit->iControl != pCA->pGameOptions->iControl)
				{
					//deliberate empty control statement
				} else
				{
					pCA->pDA_Audio->Play_Primary(pCA->pAckTable->Table[pQTr->wParam1].iSndSeg);
				}
			} else
			{
				//override in case unit is dead (special case for unit death animation)
				if (pQTr->wParam3 >= 100)
				{
					if ( (pQTr->wParam3 - 100) != pCA->pGameOptions->iControl)
					{
					//deliberate empty control statement
					} else
					{
						pCA->pDA_Audio->Play_Primary(pCA->pAckTable->Table[pQTr->wParam1].iSndSeg);
					}
				}
			}
/*	
			if (pUnit != NULL)
			{
				NText[0] = 0;

				if (pQTr->wParam3 == 1)
				{
					strcat(NText, "GROUP ");
				}

				strcat(NText, pUnit->ShortName);
				strcat(NText, ": ");
                
				strcat(NText, pCA->pAckTable->Table[pQTr->wParam1].AckText);

				pCA->pChatBuffer->Add_CBNode(NText, 1000);
			}
*/
			break;
		}
	break;
	}
	if (SkipFlag == FALSE)
	{
		//advance pQTrNext forward
			pQTrNext = (LPPCMD)pQTr->pNext;
		//move pQTrPrev backward (full linking procedure needed with the need to execute commands at certain times 12.24.2002)
			pQTrPrev = (LPPCMD)pQTr->pPrev;
		//delete pQTr
			delete(pQTr);
		//re-link Next and Prev
			pQTrNext->pPrev = (void*)pQTrPrev;
			pQTrPrev->pNext = (void*)pQTrNext;
		//reset pQTr
			pQTr = pQTrNext;
	}
		else
		{
			pQTrNext = (LPPCMD)pQTr->pNext;
			pQTr = pQTrNext;
		}
}

//release Queue-In-Use
bQueueInUse = FALSE;

}

//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCommQueue::Issue_Command_Q2(WORD wType, BYTE* pData, WORD wDataSize, BOOL bCopyFlag)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	char NText[50];

//HACK
	//force bCopyFlag as true (03.05.2004)
	//bCopyFlag = TRUE;

//wait and set Queue2-In-Use
	while(bQueue2InUse == TRUE)
	{  
		Write_Log("Waiting for Queue2 to free....");
		itoa(pCA->pTimer->iCurrTick, NText, 10);
		Write_Log(NText);
			WriteLog_Enter();
	}

bQueue2InUse = TRUE;

	PCMD2* pQ2Next = pQ2Last;
	PCMD2* pQ2Prev = pQ2Last->pPrev;

	PCMD2* pQ2New = new PCMD2; 
	ZeroMemory(pQ2New, sizeof(PCMD2));				


	//set information inside pQ2New
	pQ2New->wType = wType;
	pQ2New->pData = pData;
	if (bCopyFlag == TRUE)
	{
		BYTE* pDataDeepCopy = new BYTE[wDataSize];
		memcpy(pDataDeepCopy, pData, wDataSize);
		pQ2New->pData = pDataDeepCopy;
	}

	pQ2New->wDataSize = wDataSize;


	//link it into the queue
	pQ2New->pNext = pQ2Next;
	pQ2New->pPrev = pQ2Prev;
	pQ2Prev->pNext = pQ2New;
	pQ2Next->pPrev = pQ2New;


//release Queue2-In-Use
bQueue2InUse = FALSE;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cCommQueue::Execute_Command2(PCMD2* pCommand2)
{


	cClassAccess* pCA = (cClassAccess*)pCAccess;
	char NText[50];
	BOOL SkipFlag = FALSE;
	//variables used in case statements which cannot be initialized inside
	BYTE* pDTrav;
	LONG* plTrav;
	WORD* pwTrav;
	DWORD* pdwTrav;
	DWORD* pdwTrav2;
	cUnitA* pUnit;
	cUnitA* pEnemy;
	VOID** ppTrav;
	cStructureA* pStruct;
	cStructureA* pEnemy_Struct;
	cProgressiveOrder* pPO;
	cComponentListB* pNatInventory;

//DEBUG
	Write_Log("Execute_Command2 Begun. Type: ");
	itoa(pCommand2->wType, NText, 10);
	Write_Log(NText);
	WriteLog_Enter();

	int iType;
		
	cProgressiveOrder* pPO_New;

	PCMD2* pCmd2;

	int iCmpType;
	int iUnitType;

	int*	arrIDs;

	int lc1; //local counter

	cComponentA* pComponent;
	cComponentA* pCmp_Trav;
	cComponentA* pCmp_Trav2;

	LONG lExecTick;
	WORD wUnitID;
	LONG lTargetIndex;
	LONG lGuardIndex;
	int iRetAssemble;
	int iNumComponents;

	DWORD dwTargetType, dwID;

	WORD wEnemyID;
	BYTE bControlID;

	int iIssueFlag;

DWORD* pUnitsIssue;

	WORD wUnitsIssue;


int iRet = 0;   //return flag

	PCMD2* pQ2Tr = pCommand2; //mainly so we don't have to change all instances of pQ2Tr (08.07.2003)

//DEBUG (removed if not _DEBUG)
	//_ASSERTE( _CrtCheckMemory() );
//END DB


	//DEBUG
	if (DEBUG_COMMQUEUE)
	{
	pdwTrav = (DWORD*)pQ2Tr->pData;
	for (int lc1 = 0; lc1 < (pQ2Tr->wDataSize / 4); lc1++)
	{	
		itoa(*pdwTrav,NText,10);
		Write_Log(NText);
		Write_Log("\t");
		pdwTrav++;
	}
	WriteLog_Enter();
	}

//get rid of the stupid switch statement(since I can't declare variables inside it) and start using IFs (08.11.2003)
	if (pQ2Tr->wType == CQ2TYPE_START_PENDINGCOMMAND)
	{
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;

			pCA->pInterface->Initiate_PendingCommand(*pdwTrav);
			
	} else if (pQ2Tr->wType == CQ2TYPE_CANCEL_PENDINGCOMMAND)
	{
			pCA->pInterface->pPendingCommand->Exit();
			delete(pCA->pInterface->pPendingCommand);
			pCA->pInterface->pPendingCommand = NULL;
	}
	else if (pQ2Tr->wType == CQ2TYPE_SLIDER_INPUT)
	{

			cIfcElement** ppIfcETrav = (cIfcElement**)pQ2Tr->pData;
			cIfcElement* pElem = *ppIfcETrav;

			cIfcListBoxInfo* pListBoxInfo = (cIfcListBoxInfo*)pElem->pListBoxInfo;

			cIfcElement* pListDataElem = pListBoxInfo->pListBox;

			BOOL bExecFlag = TRUE;

			//don't execute if number of slots in listbox is higher than the number of elements in the parent datalist
			if (pListBoxInfo->iListWindowSize >= pListDataElem->iNumChildren)
				bExecFlag = FALSE;

			if (bExecFlag == TRUE)
			{
				int iTopRange = pElem->Rect_Pos.top + 16 + 8;
				int iBottomRange = pElem->Rect_Pos.bottom - 32 + 8;
				
				int iBottomOffset = pListBoxInfo->pListBox->iNumChildren - pListBoxInfo->iListWindowSize;

				float fperc = float( pCA->pInputState->IGState_Mouse.iYPos - iTopRange) / float(iBottomRange - iTopRange);

				pListBoxInfo->iPrimaryOffset = fperc * iBottomOffset;
				//correction
				if (pListBoxInfo->iPrimaryOffset < 0)
					pListBoxInfo->iPrimaryOffset = 0;
				if (pListBoxInfo->iPrimaryOffset > iBottomOffset)
					pListBoxInfo->iPrimaryOffset = iBottomOffset;

			}

	}

	if (pQ2Tr->wType == CQ2TYPE_LISTBOX_ARROW_UP)
	{
			cIfcElement** ppIfcETrav = (cIfcElement**)pQ2Tr->pData;

			cIfcElement* pElem = *ppIfcETrav;

			cIfcListBoxInfo* pListBoxInfo = (cIfcListBoxInfo*)pElem->pListBoxInfo;

			cIfcElement* pListDataElem = pListBoxInfo->pListBox;

			BOOL bExecFlag = TRUE;

			//don't execute if number of slots in listbox is higher than the number of elements in the parent datalist
			if (pListBoxInfo->iListWindowSize >= pListDataElem->iNumChildren)
				bExecFlag = FALSE;
			//dont' execute if offset is already 0
			if (pListBoxInfo->iPrimaryOffset <= 0)
			{
				bExecFlag = FALSE;
				pListBoxInfo->iPrimaryOffset = 0;
			}
			
			if (bExecFlag == TRUE)
			{
				pListBoxInfo->iPrimaryOffset--;
			}

	}
	if (pQ2Tr->wType == CQ2TYPE_LISTBOX_ARROW_DOWN)
	{
			cIfcElement** ppIfcETrav = (cIfcElement**)pQ2Tr->pData;

			cIfcElement* pElem = *ppIfcETrav;

			cIfcListBoxInfo* pListBoxInfo = (cIfcListBoxInfo*)pElem->pListBoxInfo;

			cIfcElement* pListDataElem = pListBoxInfo->pListBox;

			BOOL bExecFlag = TRUE;

			//don't execute if number of slots in listbox is higher than the number of elements in the parent datalist
			if (pListBoxInfo->iListWindowSize >= pListDataElem->iNumChildren)
				bExecFlag = FALSE;
			//dont' execute if primaryoffset + listwindowsize is equal to or greater than listdata size
			if ( (pListBoxInfo->iPrimaryOffset+pListBoxInfo->iListWindowSize) >= pListDataElem->iNumChildren)
			{
				bExecFlag = FALSE;
					if ( (pListBoxInfo->iPrimaryOffset+pListBoxInfo->iListWindowSize) > pListDataElem->iNumChildren)
						pListBoxInfo->iPrimaryOffset = 0;
			}
			
			if (bExecFlag == TRUE)
			{
				pListBoxInfo->iPrimaryOffset++;
			}

	}
	if (pQ2Tr->wType == CQ2TYPE_LISTBOX_EXECUTE_IFC)
		{

			//grab the parent element
			cIfcElement** ppIfcETrav = (cIfcElement**)pQ2Tr->pData;
			cIfcElement* pParent = *ppIfcETrav;
			ppIfcETrav++;
			cIfcElement* pListing = *ppIfcETrav;
			ppIfcETrav++;
			//grab the offset number
			DWORD* pdwData = (DWORD*)ppIfcETrav;
			DWORD dwOffset = *pdwData; pdwData++;
			//grab the element category
			DWORD dwElemCat = *pdwData;


			if (pParent->pListBoxInfo != NULL)
			{
				//grab the list traverser
				cIfcListBoxInfo* pListBoxInfo = (cIfcListBoxInfo*)pParent->pListBoxInfo;
				cIfcElement* pTrav = pListBoxInfo->pListBox->pChildren;
				if (pTrav == NULL)
				{
					return 0; //no entries in list, which is fine
				}
			
				//HACK to pull the list primary offset back into bounds
				while ( ((pListBoxInfo->iPrimaryOffset + pListBoxInfo->iListWindowSize) > pListBoxInfo->pListBox->iNumChildren) && pListBoxInfo->iPrimaryOffset != 0)
				{

					pListBoxInfo->iPrimaryOffset--;
				}
	

				//increment
				for (int lc1 = 0; lc1 < (dwOffset + pListBoxInfo->iPrimaryOffset); lc1++)
				{

					pTrav = pTrav->pNext;
					if (pTrav == NULL)
						return 0; //this entry doesn't point to anything. That's fine
				}

				//then execute the head of the specified list
				PCMD2* pCmd = pTrav->pCmds[dwElemCat];
	
				itoa((int)pTrav->pCmds[dwElemCat], NText, 10);
				Write_Log(NText);
				WriteLog_Enter();

				//NOTE that there is no subelement processing here. Therefore you cannot put subelements (as of yet)
				//into 
				while (pCmd != NULL)
				{
					
					if (pCmd->wType <= NUM_CQ2COMMANDS)   //dispatch game commands to full queue
					{
						Issue_Command_Q2(pCmd->wType, pCmd->pData, pCmd->wDataSize, TRUE);
						pCA->pDP_Multiplay->Send_Command2(pCmd->wType, pCmd->pData, pCmd->wDataSize, FALSE, TRUE);
					} else
					{

						//CAREFUL! recursive call
						Execute_Command2(pCmd);
						
					//Disp_IfcElems_C(pIfcElem, dwX, dwY);
					}
					pCmd = pCmd->pNext;
				}    

			}
		}
		if (pQ2Tr->wType == CQ2TYPE_NOTIFY_ICONTROL)
		{

				//basically just...
				pDTrav = pQ2Tr->pData;
				//get DPNID
				pdwTrav = (DWORD*)pDTrav;
				DWORD dwDPNID = *pdwTrav;
				pdwTrav++;

				int* piTrav = (int*)pdwTrav;
				//
				int iControl_Set = *piTrav;
			
				//now set it
				//find the DPNID (and set)
				for (int lc25 =0; lc25 < TEMP_DP_MAXPLAYERS; lc25++)
				{
					if (pCA->pTimer->dwDPNIDs[lc25] == dwDPNID)
					{
							pCA->pTimer->iControls[lc25] = iControl_Set;
					}
				}

		}
		if (pQ2Tr->wType == CQ2TYPE_SET_IDCONTROLANDTIMER)
		{
			int* piTrav = (int*)pQ2Tr->pData;
						
			pCA->pGameOptions->iControl = *piTrav;
			piTrav++;
			pCA->pGameOptions->iTimerID = *piTrav;
		}
		if (pQ2Tr->wType == CQ2TYPE_INITIATE_ENDGAME)
		{
				//basically just...
				pDTrav = pQ2Tr->pData;
				//get execution tick time
				plTrav = (LONG*)pDTrav;
				lExecTick = *plTrav;
				plTrav++;

				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					//SkipFlag = TRUE;
					return 2; //return to calling function that it is not yet time to execute
				}	
					else
					{
						pCA->pAppData->bRunStage = APP_RUNSTAGE_ENDGAME;
						pCA->pInterface->IBoxes[IBOX_NOTIFY].bActiveBox = TRUE;
					}
		}
		if (pQ2Tr->wType == CQ2TYPE_CANCEL_PROGORDER_TOP)
		{

				//basically just...
				pDTrav = pQ2Tr->pData;
				//get execution tick time
				plTrav = (LONG*)pDTrav;
				lExecTick = *plTrav;
				plTrav++;

				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					//SkipFlag = TRUE;
					return 2; //return to calling function that it is not yet time to execute
				}	
					else
					{
						pdwTrav = (DWORD*)plTrav;
						pdwTrav++;
						//the second DWORD is the type of IDs being presented. it can be skipped(for now, error checking possibly later)
						pdwTrav++;
						//the third DWORD is the structure ID
						int iID = *pdwTrav;
						pStruct = pCA->pStructIDTable->pTable[iID];

						if (pStruct->OrderQueue.iNumOrders > 0)
						{
							pStruct->OrderQueue.Delete_PO((cProgressiveOrder*)pStruct->OrderQueue.pPO_First->pNext); 
						}
					}
		}
		if (pQ2Tr->wType == CQ2TYPE_REPORTFRAME)
		{
				int iTimerID = -1;
				//extract the iTimer ID
				pDTrav = pQ2Tr->pData;
				iTimerID = *pDTrav;
				pDTrav++;
				//extract the tick count
				plTrav = (LONG*)pDTrav;
				lExecTick = *plTrav;

				//DEBUG
				char TextBuffer[100];
				ZeroMemory(TextBuffer, sizeof(char)*100);
				strcat(TextBuffer, "Player ");
				itoa(iTimerID, NText, 10);
				strcat(TextBuffer, NText);
				strcat(TextBuffer, ": ");
				itoa(lExecTick, NText, 10);
				strcat(TextBuffer, NText);

				pCA->pTimer->iTickReports[iTimerID] = lExecTick;
                
				//pCA->pChatBuffer->Add_CBNode(TextBuffer, 60); 

		}
		else if (pQ2Tr->wType == CQ2TYPE_INIT_TIMERID)
		{
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get the iControl ID
				int iTimerID = *pdwTrav;

				pdwTrav++;
				DWORD dwDPNID = *pdwTrav;
				pdwTrav++;
				DWORD dwNameLength = *pdwTrav;
				pdwTrav++;

				pCA->pTimer->iTickReportIDCodes[iTimerID] = 1; //active
				pCA->pTimer->dwDPNIDs[iTimerID] = dwDPNID;

				char* pcTrav = (char*)pdwTrav;

				//set name
				for (int lc1 = 0; lc1 < dwNameLength; lc1++)
				{
					pCA->pTimer->Names[iTimerID][lc1] = *pcTrav;
					pcTrav++;
				}
				//zero-terminate
				pCA->pTimer->Names[iTimerID][dwNameLength] = 0;
	
				pCA->pDP_Multiplay->iNumTimerIDInitsReceived++;
		}
		else if (pQ2Tr->wType == CQ2TYPE_SET_HOSTDPNID)
		{
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get the iControl ID
				DPNID HostID = (DPNID)*pdwTrav;

				pCA->pGameOptions->DPNID_Host = HostID;

				//say it
					char Text[100];
					Text[0] = 0;
					char NText[20];
					strcat(Text, "Received Host DPNID as ");
					ultoa(pCA->pGameOptions->DPNID_Host, NText, 10);
					strcat(Text, NText);
					pCA->pDP_Multiplay->Send_ChatText(Text, pCA->pGameOptions->strName, 100);

				//if we are in the prestart stage, initiate the next request: the unique mapid
				//
					if (pCA->pAppData->bRunStage == APP_RUNSTAGE_PRESTART)
					{
						pCA->pDP_Multiplay->Send_Command2(pCA->pGameOptions->DPNID_Host, CQ2TYPE_SEND_UMAPID, (BYTE*)&pCA->pGameOptions->DPNID_Local, sizeof(DPNID), FALSE, TRUE);
						//say it
							char Text[100];
							Text[0] = 0;
							char NText[20];
							ultoa(pCA->pGameOptions->DPNID_Host, NText, 10);
							//strcat(Text, NText);
							strcat(Text, " Host, what is the Map ID?");
							pCA->pDP_Multiplay->Send_ChatText(Text, pCA->pGameOptions->strName, 100);
					}

		}
		else if (pQ2Tr->wType == CQ2TYPE_SET_UMAPID)
		{
				int* piTrav = (int*)pQ2Tr->pData;
				//get the iControl ID
				int iMapID = *piTrav;
				//now set it
				pCA->pGameOptions->iMapUniqueID = iMapID;

				//say it
					char Text[100];
					Text[0] = 0;
					char NText[20];
					strcat(Text, "Received Map ID as ");
					itoa(pCA->pGameOptions->iMapUniqueID, NText, 10);
					strcat(Text, NText);
					pCA->pDP_Multiplay->Send_ChatText(Text, pCA->pGameOptions->strName, 100);

				//if we are in the prestart stage, initiate the next request: the map path
				//
					if (pCA->pAppData->bRunStage == APP_RUNSTAGE_PRESTART)
					{
						pCA->pDP_Multiplay->Send_Command2(pCA->pGameOptions->DPNID_Host, CQ2TYPE_SEND_MAPPATH, (BYTE*)&pCA->pGameOptions->DPNID_Local, sizeof(DPNID), FALSE, TRUE);
						//say it
							char Text[100];
							Text[0] = 0;
							char NText[20];
							ultoa(pCA->pGameOptions->DPNID_Host, NText, 10);
							//strcat(Text, NText);
							strcat(Text, " Host, what is the Map Path?");
							pCA->pDP_Multiplay->Send_ChatText(Text, pCA->pGameOptions->strName, 100);
					}
		}
		else if (pQ2Tr->wType == CQ2TYPE_SEND_UMAPID)
		{
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get the iControl ID
				DPNID TargetID = (DPNID)*pdwTrav;

				if (pCA->pGameOptions->bAmHost == TRUE)
				{
				//send it back to target
				pCA->pDP_Multiplay->Send_Command2(TargetID, CQ2TYPE_SET_UMAPID, (BYTE*)&pCA->pGameOptions->iMapUniqueID, sizeof(DWORD), FALSE, TRUE);

					//say it
					char Text[100];
					Text[0] = 0;
					char NText[20];
					ultoa(TargetID, NText, 10);
					strcat(Text, NText);
					strcat(Text, ", the map ID is ");
					ultoa(pCA->pGameOptions->iMapUniqueID, NText, 10);
					strcat(Text, NText);
					pCA->pDP_Multiplay->Send_ChatText(Text, pCA->pGameOptions->strName, 100);
				}
		}
		else if (pQ2Tr->wType == CQ2TYPE_SET_MAPPATH)
		{
				char* pcTrav = (char*)pQ2Tr->pData;
				//concatenate it directly 
				ZeroMemory(pCA->pGameOptions->strMultiplayerMapPath, sizeof(char)*256);
				strcat(pCA->pGameOptions->strMultiplayerMapPath, pcTrav);

				//say it
					char Text[100];
					ZeroMemory(Text, sizeof(char)*100);
					strcat(Text, "Received Map Path as ");
					strcat(Text, pCA->pGameOptions->strMultiplayerMapPath);
					pCA->pDP_Multiplay->Send_ChatText(Text, pCA->pGameOptions->strName, 100);

					//now check the ID of the LOCAL version of the file at this map path to see if it has the same
					//unique ID
					BOOL bMatch = FALSE;

					if ( pCA->pGameOptions->iMapUniqueID == pCA->pGameWorld->UniqueMapIDOf(pCA->pGameOptions->strMultiplayerMapPath) )
					{
						bMatch = TRUE;
					}

				//now, if we are in prestart stage...
					if (pCA->pAppData->bRunStage == APP_RUNSTAGE_PRESTART)
					{
						if (bMatch == TRUE)
						{
							pCA->pGameOptions->bMapLocationFinalized = TRUE;
							//if we have a match, then report OK to the host
							pCA->pDP_Multiplay->Send_Command2(pCA->pGameOptions->DPNID_Host, CQ2TYPE_INDICATE_READY, (BYTE*)&pCA->pGameOptions->DPNID_Local, sizeof(DPNID), FALSE, TRUE);
							//say it
								char Text[100];
								Text[0] = 0;
								char NText[20];
								ultoa(pCA->pGameOptions->DPNID_Host, NText, 10);
								//strcat(Text, NText);
								strcat(Text, " Host, okay, I am ready!");
								pCA->pDP_Multiplay->Send_ChatText(Text, pCA->pGameOptions->strName, 100);
						}
						if (bMatch == FALSE)
						{
								pCA->pDP_Multiplay->Send_ChatText("I don't have the map!", pCA->pGameOptions->strName, 100);
						}
					}

		}
		else if (pQ2Tr->wType == CQ2TYPE_INDICATE_READY)
		{
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get the iControl ID
				DPNID TargetID = (DPNID)*pdwTrav;

				//wait for statuses in use to free
				while (pCA->pGameOptions->bMPStatusesInUse == TRUE)
				{
					//waiting
				}
				pCA->pGameOptions->bMPStatusesInUse = TRUE;

				//now, find that ID in the scratch table
				for (int lc1 = 0; lc1 < TEMP_DP_MAXPLAYERS; lc1++)
				{	
					if (pCA->pGameOptions->ScratchInfoTable[lc1] != NULL)
					{
						if (pCA->pGameOptions->ScratchInfoTable[lc1]->iDPNID == TargetID)
						{
							pCA->pGameOptions->ScratchInfoTable[lc1]->bHaveCurrentMap = TRUE;	
							//say it 
							/*
								char Text[100];
								Text[0] = 0;
								char NText[20];
								ultoa(TargetID, NText, 10);
								strcat(Text, NText);
								strcat(Text, ", recording that you have the map.");
								pCA->pDP_Multiplay->Send_ChatText(Text, pCA->pGameOptions->strName, 100);
							*/
						}
					}
				}

				//release
				pCA->pGameOptions->bMPStatusesInUse = FALSE;
		}
		else if (pQ2Tr->wType == CQ2TYPE_SEND_MAPPATH)
		{
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get the iControl ID
				DPNID TargetID = (DPNID)*pdwTrav;

				if (pCA->pGameOptions->bAmHost == TRUE)
				{
				//send it back to target (+1  on string length to force it to pick up the terminating 0)
				pCA->pDP_Multiplay->Send_Command2(TargetID, CQ2TYPE_SET_MAPPATH, (BYTE*)pCA->pGameOptions->strMultiplayerMapPath, strlen(pCA->pGameOptions->strMultiplayerMapPath)+1, FALSE, TRUE);

				//say it
					//say it
					char Text[100];
					Text[0] = 0;
					char NText[20];
					ultoa(TargetID, NText, 10);
					strcat(Text, NText);
					strcat(Text, ", the mappath is ");
					strcat(Text, pCA->pGameOptions->strMultiplayerMapPath);
					pCA->pDP_Multiplay->Send_ChatText(Text, pCA->pGameOptions->strName, 100);
				}
		}
		else if (pQ2Tr->wType == CQ2TYPE_SEND_HOSTDPNID)
		{
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get the iControl ID
				DPNID TargetID = (DPNID)*pdwTrav;
				//if I am the host, send back own DPNID  (otherwise ignore)
				if (pCA->pGameOptions->bAmHost == TRUE)
				{
					DWORD* pNewID = new DWORD;
					*pNewID = (DWORD)pCA->pGameOptions->DPNID_Host;

					pCA->pDP_Multiplay->Send_Command2(TargetID, CQ2TYPE_SET_HOSTDPNID, (BYTE*)pNewID, sizeof(DWORD), FALSE, TRUE);

					//say it
					char Text[100];
					Text[0] = 0;
					char NText[20];
					ultoa(TargetID, NText, 10);
					strcat(Text, NText);
					strcat(Text, ", my DPNID is ");
					ultoa(pCA->pGameOptions->DPNID_Host, NText, 10);
					strcat(Text, NText);
					pCA->pDP_Multiplay->Send_ChatText(Text, pCA->pGameOptions->strName, 100);
				}
		}
		else if (pQ2Tr->wType == CQ2TYPE_INIT_NATION)
		{
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get the iControl ID
				int iCtrl = *pdwTrav;
				pdwTrav++;
				//get bAIFlag(which will be a DWORD, 0 or 1
				DWORD iAI = *pdwTrav;
				BOOL bAI;
				if (iAI == 0)
					bAI = FALSE;
						else
					bAI = TRUE;
				pdwTrav++;
				//get the number of letters
				int iStrlen = *pdwTrav;
				pdwTrav++;
				char* pName = new char[iStrlen+1];
				ZeroMemory(pName, sizeof(char) * iStrlen+1);
				//now get the name
				char* pSource = (char*)pdwTrav;
				memcpy(pName, pSource, sizeof(char) * iStrlen);
			
				//now init nation
				pCA->pNationManager->Init_Nation(iCtrl, pName, iStrlen+1, bAI, TRUE);

				pCA->pDP_Multiplay->iNumNationInitsReceived++;

				delete(pName);

		}
		else if (pQ2Tr->wType == CQ2TYPE_SET_IDCONTROL)
		{
				//basically just...
				pDTrav = pQ2Tr->pData;
				//get execution tick time
				int* piTrav = (int*)pDTrav;
				int iControl = *piTrav;

				pCA->pGameOptions->iControl = iControl;

		}
		else if (pQ2Tr->wType == CQ2TYPE_BATCH_GUARD)
		{
				//basically just...
				pDTrav = pQ2Tr->pData;
				//get execution tick time
				plTrav = (LONG*)pDTrav;
				lExecTick = *plTrav;
				plTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					//SkipFlag = TRUE;
					return 1; //return to calling function that it is not yet time to execute
				}	
					else
					{
						//extract the guard index
						lGuardIndex = *plTrav;
						plTrav++;
						//get the number of units being issued to
						pwTrav = (WORD*)plTrav;
						wUnitsIssue = *pwTrav;
						//now issue order all units here to move
						for (int lc6 = 0; lc6 < wUnitsIssue; lc6++)
						{
						//increment pwTrav
						pwTrav++;
						//get unit pointer
						pUnit = pCA->pUnitTable->pUnitTable[*pwTrav];

						//set mode
						pUnit->bMode_L5 = UMODE_L6_NOTHING;
						pUnit->bMode_L5 = UMODE_L5_GUARD;
						pUnit->bExternalOrderCompleted = FALSE;
						pUnit->wExternalOrderIssued = CQ2TYPE_BATCH_GUARD;

						//create a guard info structure
						pUnit->pGuardInfo = new UNITGUARDINFO; 
						//add information
						pUnit->pGuardInfo->Pos_guard.index = lGuardIndex;
						pUnit->pGuardInfo->Pos_guard.Calc_xy(pCA->pGameWorld->iWidth);
						
						}
					}
		}
		else if (pQ2Tr->wType == CQ2TYPE_CREATE_UNIT_B)
		{
				pdwTrav = (DWORD*)pQ2Tr->pData;
				int iMapIndex = *(pdwTrav);
				pdwTrav++;
				int iNation = *(pdwTrav);
				pdwTrav++;
				int iType = *(pdwTrav);

				pCA->pUnitList->Create_Unit("Unit", iType, iNation, iMapIndex % pCA->pGameWorld->iWidth, iMapIndex / pCA->pGameWorld->iWidth);

		}
		else if (pQ2Tr->wType == CQ2TYPE_MOVE_B)
		{
			

				//i need i
				//extract the execute tick from the beginning of the command
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					SkipFlag = TRUE;

					if (lExecTick > pCA->pTimer->iCurrTick)
					{

						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{

						return 2;
					}
				}
					else
					{

						//the second DWORD is the type of IDs being presented. it can be skipped(for now, error checking possibly later)
						pdwTrav++;
						//the third DWORD is the number of units being issued to
						wUnitsIssue = *pdwTrav;
							//lTargetIndex = *plTrav;
						//save the current address
						pdwTrav2 = pdwTrav;
						pdwTrav++;
						//we have to get the target index before issuing to the units, so jump past to the end to get the target index
						pdwTrav += wUnitsIssue;
						//now jump the TYPE and NUMBER ( error checking possibly later)
						pdwTrav += 2;
						//get target index
						lTargetIndex = *pdwTrav;
						//now get back and begin issuing
						pdwTrav = pdwTrav2;

						//keep track of number of acknowledgements issued
						int iAcknowledgments = 0;	
						//now issue order all units here to move
						for (int lc6 = 0; lc6 < wUnitsIssue; lc6++)
						{

							//increment pwTrav
							pdwTrav++;
							//get unit pointer
							pUnit = pCA->pUnitTable->pUnitTable[*pdwTrav];
						//make sure unit supports command
						BOOL bExecutable = FALSE;
						for (int lc90 = 0; lc90 < pUnit->iiNumSupportedCommands; lc90++)
						if (pUnit->piSupportedCommands[lc90] == CQ2TYPE_MOVE_B)
							bExecutable = TRUE;
						if (bExecutable == TRUE)
						{
							//(copied from original queue code 01.01.2003)
							//set the L4 mode to "nothing"(it doesn't set the L3 mode)
							pUnit->bExternalOrderCompleted = FALSE;
							pUnit->wExternalOrderIssued = CQ2TYPE_MOVE_B;
							pUnit->bMode_L6 = UMODE_L6_NOTHING;
							pUnit->bMode_L5 = UMODE_L5_NOTHING;
							pUnit->bMode_L4 = UMODE_L4_NOTHING;
							//execute a mode command on the selected unit to its target (first param is the UnitID, second is the target index)
							pUnit->bMode_L3 = UMODE_L3_MOVE;
								pUnit->ddAllowableBackOffDistance = 0; //must go exactly to target
								//possibly a temporary fix...but if a unit is air-capable, divert it's mode to UMODE_L3_MOVE_AIR (01.16.2004)
								if (pUnit->pFrameType->pFrameInfo->MoveCharacteristics[MAPTILE_AIR] != -1)
									pUnit->bMode_L3 = UMODE_L3_MOVE_AIR;
							//set the target index
							pUnit->Pos_sqtarget.index = lTargetIndex;

							//calc the rest of the numbers
							pUnit->Pos_sqtarget.Calc_xy(pCA->pGameWorld->iWidth);
							//...and destroy the unit's current path list
							pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pUnit->pUnitPath);
							pUnit->pUnitPath = NULL;


							//issue acknowledgment
							if (iAcknowledgments <= 2)
							if (pUnit->iControl == pCA->pGameOptions->iControl)
							{
							int iRet = pCA->pAckTable->Get_Ack(FALSE, pUnit->iType, ACK_TYPE_CMDRECV_UNIT_MOVE, pUnit->iPersonality, rand() % 3);
								Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, pUnit->iID, 0, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
							iAcknowledgments++;
							}

						}
						}
					}
		}
		else if (pQ2Tr->wType == CQ2TYPE_ATTACK_B)
		{
				//begin extracting data from the command
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					if (lExecTick > pCA->pTimer->iCurrTick)
					{
						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{
						return 2;
					}
				}
					else
					{
						//the second DWORD is the type of IDs being presented. it can be skipped(for now, error checking possibly later)
						//because it is already known that the type of ID being presented are UNIT IDs.
						pdwTrav++;
						//the third DWORD is the number of units being issued to
						wUnitsIssue = *pdwTrav;
						//save the position of the current pointer as the position just before we issue to the UNIT IDs
						pUnitsIssue = pdwTrav;
						//now get the target type and ID
						pdwTrav++;
						pdwTrav += wUnitsIssue; //this will jump us just past the last to-issue Unit ID
						//read in the target type
						dwTargetType = *pdwTrav;
						pdwTrav += 2; // jump over the NUMBER of targets because we know there is only 1
						//now read in the ID
						dwID = *pdwTrav;
						//now go back and issue ATTACK orders to the units
						pdwTrav = pUnitsIssue;

						iIssueFlag = 0;
						//check for continued existence of specified target UNIT or STRUCTURE
						if (dwTargetType == OBJTYPE_UNIT)
						{
							pEnemy = pCA->pUnitTable->pUnitTable[dwID];
							if (pEnemy == NULL)
								iIssueFlag = 1;
						}
						if (dwTargetType == OBJTYPE_STRUCTURE)
						{
							pEnemy_Struct = pCA->pStructIDTable->pTable[dwID];
							if (pEnemy_Struct == NULL)
								iIssueFlag = 2;
						}
						//keep track of number of acknowledgements issued
						int iAcknowledgments = 0;
						//don't issue if a target unit or structure was not found
						if (iIssueFlag == 0)
						for (int lc27 = 0; lc27 < wUnitsIssue; lc27++)
						{
								//increment pdwTrav
								pdwTrav++;
								//now get the unit pointer
								pUnit = pCA->pUnitTable->pUnitTable[*pdwTrav];
							//check if unit actually supports the command
							BOOL bExecutable = FALSE;
							for (int lc90 = 0; lc90 < pUnit->iiNumSupportedCommands; lc90++)
							if (pUnit->piSupportedCommands[lc90] == CQ2TYPE_ATTACK_B)
								bExecutable = TRUE;
							if (bExecutable == TRUE)
							{
								if (pUnit->pAttackInfo != NULL)
									delete(pUnit->pAttackInfo);
								pUnit->pAttackInfo = new UNITATTACKINFO;
								//now issue relevant attack information to this unit
								//some information must be issued differently if the target is a unit, structure or maptile
								//initialize pAttackInfo
									if (dwTargetType == OBJTYPE_UNIT)
									{
										pUnit->pAttackInfo->iEnemyID = pEnemy->iID;
										pUnit->pAttackInfo->iTargetType = CQATTACKTYPE_UNIT;
//										pUnit->pAttackInfo->pEnemy = (void*)pEnemy;
										pUnit->pAttackInfo->Pos_ABSTarget.x = pEnemy->Pos_ABS.x;
										pUnit->pAttackInfo->Pos_ABSTarget.y = pEnemy->Pos_ABS.y;
										pUnit->Pos_sqtarget.x = pEnemy->Pos_sqcurr.x;
										pUnit->Pos_sqtarget.y = pEnemy->Pos_sqcurr.y;
										pUnit->Pos_sqtarget.index = pEnemy->Pos_sqcurr.index;
									}
									if (dwTargetType == OBJTYPE_STRUCTURE)
									{
										pUnit->pAttackInfo->iEnemyID = pEnemy_Struct->iID;
										pUnit->pAttackInfo->iTargetType = CQATTACKTYPE_STRUCT;
										pUnit->pAttackInfo->Pos_ABSTarget.x = pEnemy_Struct->Pos_primary.x * TILE_SIZE;
										pUnit->pAttackInfo->Pos_ABSTarget.y = pEnemy_Struct->Pos_primary.y * TILE_SIZE;
										pUnit->Pos_sqtarget.x = pEnemy_Struct->Pos_primary.x;
										pUnit->Pos_sqtarget.y = pEnemy_Struct->Pos_primary.y;
										pUnit->Pos_sqtarget.index = pEnemy_Struct->Pos_primary.index;

									}
									if (dwTargetType == OBJTYPE_MAPTILE)
									{
										pUnit->pAttackInfo->iEnemyID = dwID;
										pUnit->pAttackInfo->iTargetType = CQATTACKTYPE_MAPTILE;
										//?
										pUnit->pAttackInfo->Pos_ABSTarget.x = ((dwID % pCA->pGameWorld->iWidth)+0.5) * TILE_SIZE;
										pUnit->pAttackInfo->Pos_ABSTarget.y = ((dwID / pCA->pGameWorld->iWidth)+0.5) * TILE_SIZE;
										pUnit->Pos_sqtarget.index = dwID;
										pUnit->Pos_sqtarget.Calc_xy(pCA->pGameWorld->iWidth);
									}
									pUnit->pAttackInfo->fAttackDirection = pUnit->Calc_Direction(pUnit->pAttackInfo->Pos_ABSTarget.x - pUnit->Pos_ABS.x, pUnit->pAttackInfo->Pos_ABSTarget.y - pUnit->Pos_ABS.y);
									//update local weapon targets
									pUnit->Update_LocalWeaponTargets(pUnit->pAttackInfo->Pos_ABSTarget.x, pUnit->pAttackInfo->Pos_ABSTarget.y, &pUnit->MultiFireInfo);
									//set L4 mode
									pUnit->bExternalOrderCompleted = FALSE;
									pUnit->wExternalOrderIssued = CQ2TYPE_ATTACK_B;
									pUnit->bMode_L6 = UMODE_L6_NOTHING;
									pUnit->bMode_L5 = UMODE_L5_NOTHING;
									pUnit->bMode_L4 = UMODE_L4_ATTACK_MULTI;
									//NOTE: this is a convention that was started earlier that is followed now....but, revised plans for the extent of Project SamCraft limit units to ONE weapon(07.30.2003)
									pUnit->pCurrentWeapon = pUnit->pPrimaryWeapon;
									//destroy unit's current path list
									pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pUnit->pUnitPath);
									pUnit->pUnitPath = NULL;

								//issue acknowledgment
								if (iAcknowledgments <= 2)
								if (pUnit->iControl == pCA->pGameOptions->iControl)
								{

									int iRet = pCA->pAckTable->Get_Ack(FALSE, pUnit->iType, ACK_TYPE_CMDRECV_UNIT_ATK, pUnit->iPersonality, rand() % 3);
									Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, pUnit->iID, 0, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
								iAcknowledgments++;
								}		
							}
						}
					}
		}
		else if (pQ2Tr->wType == CQ2TYPE_START_BUILD_COMPONENT)  //08.12.2003
		{
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{

					if (lExecTick > pCA->pTimer->iCurrTick)
					{
						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{

						return 2;
					}
				}
					else
					{
						//we know the next two DWORDS are OBJTYPE_STRUCTURE and 1, so skip them
						pdwTrav += 2;
						//the next one is the struture ID, so get that
						dwID = *pdwTrav;
						//get pointer to structure
						pStruct = pCA->pStructIDTable->pTable[dwID];

						pdwTrav++;
						//the next two DWORDS are IPARAMTYPE_DWORD and 1, so skip them
						pdwTrav += 2;
						//get a hold of the component type
						iType = *pdwTrav;

						//the requisite prog order needed is:
								//init: nothing
								//prog: build_progress_component
								//exit: create_component_a
						//create a new prog order
						cProgressiveOrder* pPO_New = new cProgressiveOrder;
						pPO_New->Init(pCAccess);

						PCMD2* pCmd2New = new PCMD2;
						

						pCmd2New->wType = CQ2TYPE_BUILD_PROGRESS_COMPONENT;

						pCmd2New->pData = Assemble_CommData_Build_Progress_Component(pPO_New, dwID, iType, &iRetAssemble);
						pCmd2New->wDataSize = iRetAssemble; 

						pPO_New->Add_Cmd2(pCmd2New, FALSE, 1);

						pCmd2New = new PCMD2;

						pCmd2New->wType = CQ2TYPE_CREATE_COMPONENT_A;
						pCmd2New->pData = Assemble_CommData_Create_Component_A(pPO_New, dwID, iType, &iRetAssemble);
						pCmd2New->wDataSize = iRetAssemble; //rememmber to change this if the it changes in the procedure!
						
						pPO_New->Add_Cmd2(pCmd2New, FALSE, 2);
						//don't need to free because the Prog Order is taking it.
						//now add the prog order to the structure
						//OH, first set the prog order's name
						//for now, just say "Build component"	
							//set the PO's name as "Build Component" (for now) 08.12.2003
							pPO_New->pName = pCA->pStringTable->pTable[STRI_BUILD_COMPONENT];
							pPO_New->fCompletion = 0;

						//add the prog order
						pStruct->OrderQueue.Add_PO(pPO_New, FALSE);

					}
		}
		else if (pQ2Tr->wType == CQ2TYPE_BUILD_STRUCTURE)
		{
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{

					if (lExecTick > pCA->pTimer->iCurrTick)
					{

						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{

						return 2;
					}
				}
					else
					{

						//the next dword, we know already is OBJTYPE_UNIT, skip it
						pdwTrav++;
						//the next dword is the number of units
						int numIDs = *pdwTrav;
						//the array of IDs is next
						int* arrIDs = new int[numIDs];
						int lc5 = 0; //local counter
						for (lc5 = 0; lc5 < numIDs; lc5++)
						{
							pdwTrav++;
							arrIDs[lc5] = *pdwTrav;
						}
						//get to the next entry 
						pdwTrav++;
						//we know the this entry is OBJTYPE_STYPE, skip it
						pdwTrav++;
						//we know that this entry is 1, skip it
						pdwTrav++;
						//get the location ID
						int iStructType = *pdwTrav;
						pdwTrav++;
						//skip the next two OBJTYPE_MAPTILE, 1
						pdwTrav += 2;
						//get the struct type
						int iCoord = *pdwTrav;

						cUnitA* pUnit;

						//06.18.2004 SPECIAL case, we check whether an RSC already exists in this sector. If so,
						//simply kick this command! (post a "Structure already here" notification)
						/* disabled 09.20.2006
						int iSector = pCA->pGameWorld->pMap[ iCoord ].iSector;
						if (iStructType == TEMP_STYPE_RSC) 
						if (pCA->pSectorTable->Sectors[iSector]->pCapital != NULL)
						{
							//pCA->pNotifBuffer->Create_Add_Node( "Regional Capital already exists in sector!", pCA->pColorTable->Red, pCA->pColorTable->DarkRed , FALSE);
							
							//apparently pNotifBuffer doesn't actually work, and it's use is almost identical to that of cChatBuffer, so just use that
							pCA->pChatBuffer->Add_CBNode("Regional Capital already exists in sector!",1000);

							//return with intent to destroy
							return 0;
						}
						*/
						//prepare for location checks
							cStructInfo* pInfo = pCA->pStructInfoTable->pTable[iStructType];
						//if shipyard, check each of the tiles and make sure one of them is an ocean...
						if (iStructType == TEMP_STYPE_SHIPYARD)
						{

							BOOL bOK = FALSE;
							for (int lc94 = 0; lc94 < (pInfo->iOccMaptiles-1); lc94++)
							{
								int iTile = iCoord + pInfo->Pos_offset_array[lc94].x + pInfo->Pos_offset_array[lc94].y * pCA->pGameWorld->iWidth;
								if (pCA->pGameWorld->pMap[iTile].iType == MAPTILE_OCEAN)
								{
									bOK = TRUE; 
									break;
								}
							}
							if (bOK == FALSE)
							{
								pCA->pChatBuffer->Add_CBNode("ERROR: Shipyards needs at least 1 square over ocean.",1000);

								//return with intent to destroy
								return 0;								
							}
						}
							for (int lc95 = 0; lc95 < (pInfo->iOccMaptiles-1); lc95++)
							{

                                int iTile = iCoord + pInfo->Pos_offset_array[lc95].x + pInfo->Pos_offset_array[lc95].y * pCA->pGameWorld->iWidth;
								SQCOORDSET sqTile;
								sqTile.index = iTile;
								sqTile.Calc_xy(pCA->pGameWorld->iWidth);
								if ( pCA->pGameWorld->IsValidBuildLocation(sqTile, iStructType) == FALSE)
								{
									if (pCA->pUnitTable->pUnitTable[arrIDs[0]]->iControl == pCA->pGameOptions->iControl)
										pCA->pChatBuffer->Add_CBNode("ERROR: I can't build here.",1000);

									return 0;
								}
							}
						//keep track of how many acknowledgements we've just issued!
						int iAcknowledgements = 0;
						//we got all the information, begin doing stuff
						for (lc5 = 0; lc5 < numIDs; lc5++)
						{
							pUnit = pCA->pUnitTable->pUnitTable[arrIDs[lc5]];
							//set L5 to NOTHING
							pUnit->bMode_L6 = UMODE_L6_NOTHING;
							pUnit->bMode_L5 = UMODE_L5_NOTHING;
							pUnit->bMode_L4 = UMODE_L4_BUILD;
								if (pUnit->pBuildInfo != NULL)
								delete(pUnit->pBuildInfo);
							pUnit->pBuildInfo = new UNITBUILDINFO;
								pUnit->bExternalOrderCompleted = FALSE;
								pUnit->wExternalOrderIssued = CQ2TYPE_BUILD_STRUCTURE;
							pUnit->pBuildInfo->Pos_build.index = iCoord;
							pUnit->pBuildInfo->iStructureID = -1;

							pUnit->pBuildInfo->Pos_build.Calc_xy(pCA->pGameWorld->iWidth);
							pUnit->pBuildInfo->Pos_ABSbuild.x = pUnit->pBuildInfo->Pos_build.x * TILE_SIZE;
							pUnit->pBuildInfo->Pos_ABSbuild.y = pUnit->pBuildInfo->Pos_build.y * TILE_SIZE;
							pUnit->pBuildInfo->iBuildOrderNumber = pCA->pStructList->iBuildOrder_Last_Assigned;
							pUnit->pBuildInfo->iBuildType = iStructType;

							//issue acknowledgment
							if (iAcknowledgements <= 2)
							if (pUnit->iControl == pCA->pGameOptions->iControl)
							{
									int iRet = pCA->pAckTable->Get_Ack(FALSE, pUnit->iType, ACK_TYPE_CMDRECV_UNIT_BUILD, pUnit->iPersonality, rand() % 3);
									Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, pUnit->iID, 0, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
							iAcknowledgements++;
							}

						}
						
						delete [] arrIDs;
					}
		}
		else if (pQ2Tr->wType == CQ2TYPE_TANK_SET_TYPE)
		{
		//this is almost identical to CQ2TYPE_LOAD_MATERIAL
				//begin extracting data from the command
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					if (lExecTick > pCA->pTimer->iCurrTick)
					{
						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{
						return 2;
					}
				}
					else
					{

						//1st set, 1st dword is type. skip  (OBJTYPE_STRUCT)
						pdwTrav++;
						//1st set, 2nd dword is quantity, skip (1)
						pdwTrav++;
						//1st set 3rd quanttiy is struct ID
						int iStructID = *pdwTrav; pdwTrav++;
						//2nd set 1st dword is type (OBJTYPE_MATTYPE) skip
						pdwTrav++;
						//2nd set 2nd dword is quantity (1) skip
						pdwTrav++;
						//2nd set 3rd dword is type
						int iMatType = *pdwTrav;

						//
						cStructureA* pStruct = pCA->pStructIDTable->pTable[iStructID];
						if (pStruct != NULL)
						{
							if (pStruct->Storage.iMaterialType != iMatType)
							{
								pStruct->Storage.iMaterialType = iMatType;
								pStruct->Storage.ddMaterialStored = 0;
							}

						}
						//flag done
						return 0;
					}
		}
		else if (pQ2Tr->wType == CQ2TYPE_TANK_FLUSH)
		{
		//this is almost identical to CQ2TYPE_LOAD_MATERIAL
				//begin extracting data from the command
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					if (lExecTick > pCA->pTimer->iCurrTick)
					{
						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{
						return 2;
					}
				}
					else
					{

						//1st set, 1st dword is type. skip  (OBJTYPE_STRUCT)
						pdwTrav++;
						//1st set, 2nd dword is quantity, skip (1)
						pdwTrav++;
						//1st set 3rd quanttiy is struct ID
						int iStructID = *pdwTrav; 

						//
						cStructureA* pStruct = pCA->pStructIDTable->pTable[iStructID];
						if (pStruct != NULL)
						{
							//store old type
							int iOldType = pStruct->Storage.iMaterialType;
							//set type to disqualify
							pStruct->Storage.iMaterialType = -1;
							//now try to find tank to transfer material
							cStorageModule* pStorage = pCA->pPathFinder->Find_ConnectedTank(pStruct->Pos_primary.index, iOldType, 0, FINDTANKMODE_PUTINTO, 0); 

							//transfer much as much material as possible
							while ((pStorage != NULL) && (pStruct->Storage.ddMaterialStored > 0))
							{
								double ddTransfer = pStorage->ddMaterialCapacity - pStorage->ddMaterialStored;
								if (ddTransfer > pStruct->Storage.ddMaterialStored)
									ddTransfer = pStruct->Storage.ddMaterialStored;

								pStorage->ddMaterialStored += ddTransfer;
								pStruct->Storage.ddMaterialStored -= ddTransfer;

								if (pStruct->Storage.ddMaterialStored == 0)
									break;
                                
								pStorage = pCA->pPathFinder->Find_ConnectedTank(pStruct->Pos_primary.index, iOldType, 0, FINDTANKMODE_PUTINTO, 0); 
							
							}

							pStruct->Storage.iMaterialType = iOldType;
							if (pStruct->Storage.ddMaterialStored == 0)
								pStruct->Storage.iMaterialType = -1;

						}

						//flag done
						return 0;
					}
		}
		else if (pQ2Tr->wType == CQ2TYPE_TRANSFER_MATERIAL)
		{
		//this is almost identical to CQ2TYPE_LOAD_MATERIAL
				//begin extracting data from the command
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					if (lExecTick > pCA->pTimer->iCurrTick)
					{
						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{
						return 2;
					}
				}
					else
					{
						//1st set, 1st dword is type. skip  (OBJTYPE_UNIT)
						pdwTrav++;
						//1st set, 2nd dword is quantity, skip (1)
						pdwTrav++;
						//1st set, 3rd dword is ID, get
						int iID = *pdwTrav;
						pdwTrav++;
						//2nd set, 1st dword is type. skip (OBJTYPE_MAPTILE)
						pdwTrav++;
						//2nd set, 2nd dword is quantity, skip (1)
						pdwTrav++;
						//2nd set, 3rd dword is Map index, get
						int iMapIndexSource = *pdwTrav;
						pdwTrav++;
						//3rd set, 1st dword is type. skip (OBJTYPE_MAPTILE)
						pdwTrav++;
						//3rd set, 2nd dword is quantity, skip (1)
						pdwTrav++;
						//3rd set, 3rd dword is Map index, get
						int iMapIndexDest = *pdwTrav;

						cUnitA* pUnit = pCA->pUnitTable->pUnitTable[iID];

						if (pUnit != NULL)
						{
							//set mode
							pUnit->bMode_L6 = UMODE_L6_NOTHING;
							pUnit->bMode_L5 = UMODE_L5_TRANSFER_MATERIAL;

							//set information
							pUnit->sqUnloadMaterialLocation.index = iMapIndexDest;
							pUnit->sqUnloadMaterialLocation.Calc_xy(pCA->pGameWorld->iWidth);
							pUnit->sqLoadMaterialLocation.index = iMapIndexSource;
							pUnit->sqLoadMaterialLocation.Calc_xy(pCA->pGameWorld->iWidth);


							//destroy unit's current path list
							pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pUnit->pUnitPath);
							pUnit->pUnitPath = NULL;

							//set external order
							pUnit->bExternalOrderCompleted = FALSE;
							pUnit->wExternalOrderIssued = CQ2TYPE_TRANSFER_MATERIAL;

							//set acknowledgement
							if (pUnit->iControl == pCA->pGameOptions->iControl)
							{
									int iRet = pCA->pAckTable->Get_Ack(FALSE, pUnit->iType, ACK_TYPE_CMDRECV_UNIT_FILL, pUnit->iPersonality, rand() % 3);
									Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, pUnit->iID, 0, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
							}

						}
					}

		}
		else if (pQ2Tr->wType == CQ2TYPE_UNLOAD_ALL)
		{

				//begin extracting data from the command
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					if (lExecTick > pCA->pTimer->iCurrTick)
					{
						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{
						return 2;
					}
				}
					else
					{
						//read in the target type
						dwTargetType = *pdwTrav;
						pdwTrav++; 
						DWORD dwNumTargets = *pdwTrav;
						pdwTrav++;
						//store this location as where the IDs start
						DWORD* pTargets = pdwTrav;
						//then jump over the number of targets to get to the information
						pdwTrav += dwNumTargets;
						//now skip two more and get the target index
						pdwTrav += 2;
						int iTargetTile = *pdwTrav;
						SQCOORDSET sqTarget;
						sqTarget.index = iTargetTile;
						sqTarget.Calc_xy(pCA->pGameWorld->iWidth);

						//now go back to where the IDs are:
						pdwTrav = pTargets;
						cUnitA* pFirstUnit = pCA->pUnitTable->pUnitTable[*pdwTrav];

						for (int lc18 = 0; lc18 < dwNumTargets; lc18++)
						{
						//DEBUG
						char NText[50];
						itoa(*pdwTrav, NText, 10);
						pCA->pCommQueue->Write_Log("Unloading unit: ");
						pCA->pCommQueue->Write_Log(NText);
						pCA->pCommQueue->WriteLog_Enter();
							cUnitA* pUnit = pCA->pUnitTable->pUnitTable[*pdwTrav];
							if (pUnit != NULL)  //if the unit is still alive
							if (pUnit->Storage.iNumStored > 0)  //if the unit actually has anything to unload
							{
								//now re-issue the command
								int iretsize = 0;
						//DEBUG
						pCA->pCommQueue->Write_Log("Assembling CommData");
						pCA->pCommQueue->WriteLog_Enter();
								BYTE* pBuffer = Assemble_CommData_Unload_A(pCA->pTimer->iCurrTick, pUnit->iID, iTargetTile, &iretsize);
								//we proceed to execute a Command2 directly here 
								//because if we use Issue a command we'll hit
								//the Queue Lock that we've currently set and then the program will just stop
								PCMD2* pCmdNew = new PCMD2;
								pCmdNew->wType = CQ2TYPE_UNLOAD;
								pCmdNew->pData = pBuffer;
								pCmdNew->wDataSize = iretsize;
						//DEBUG
						pCA->pCommQueue->Write_Log("Executing Unload Command-in-Command");
						pCA->pCommQueue->WriteLog_Enter();
								Execute_Command2(pCmdNew);
						//DEBUG
						pCA->pCommQueue->Write_Log("Executing Unload Command-in-Command Complete");
						pCA->pCommQueue->WriteLog_Enter();
								delete [] pBuffer;
						//DEBUG
						pCA->pCommQueue->Write_Log("Buffer deleted");
						pCA->pCommQueue->WriteLog_Enter();
							}
							pdwTrav++;
						}

							//set acknowledgement
							if (pFirstUnit->iControl == pCA->pGameOptions->iControl)
							{
								//causing intermittent crash. not quite sure why. Disabled for now.
								//founnd the error. changed "pUnit" to "pFirstUnit" . Still disabled until I can re-test on Vista.
									int iRet = pCA->pAckTable->Get_Ack(FALSE, pFirstUnit->iType, ACK_TYPE_CMDRECV_UNIT_UNLOAD, pFirstUnit->iPersonality, rand() % 3);
									Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, pFirstUnit->iID, 0, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
							}
						//DEBUG
						pCA->pCommQueue->Write_Log("Command Complete");
						pCA->pCommQueue->WriteLog_Enter();
						//then return to kill this command
						return 0;
					}
		}
		else if (pQ2Tr->wType == CQ2TYPE_UNLOAD)
		{

				//begin extracting data from the command
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;

				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					if (lExecTick > pCA->pTimer->iCurrTick)
					{
						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{
						return 2;
					}
				}
					else
					{
						//DEBUG
						Write_Log("Beginning unload command.");
						WriteLog_Enter();

						//the second DWORD is the type of IDs being presented. it can be skipped(for now, error checking possibly later)
						//because it is already known that the type of ID being presented are UNIT IDs.
						pdwTrav++;
						//the third DWORD is the number of units being issued to
						wUnitsIssue = *pdwTrav;
						//save the position of the current pointer as the position just before we issue to the UNIT IDs
						pUnitsIssue = pdwTrav;
						//now get the target type and ID
						pdwTrav++;
						pdwTrav += wUnitsIssue; //this will jump us just past the last to-issue Unit ID
						//read in the target type
						dwTargetType = *pdwTrav;
						pdwTrav += 2; // jump over the NUMBER of targets because we know there is only 1
						//now read in the ID
						dwID = *pdwTrav;
						//now skip three more and get the target index
						pdwTrav += 3;
						int iTargetTile = *pdwTrav;
						SQCOORDSET sqTarget;
						sqTarget.index = iTargetTile;
						sqTarget.Calc_xy(pCA->pGameWorld->iWidth);
						//now go back and get list of units to unload
						pdwTrav = pUnitsIssue;

						iIssueFlag = 0;
						//check for continued existence of specified target UNIT
						cUnitA* pTarget = pCA->pUnitTable->pUnitTable[dwID];
							if (pTarget == NULL)
								iIssueFlag = 1;   //set flag if unit is NOT there
						//don't issue if a target unit or structure was not found
						if (iIssueFlag == 0)
						{
						//DEBUG
						Write_Log("Beginning Issue to targets.");
						WriteLog_Enter();
							for (int lc28 = 0; lc28 < 10; lc28++)
								pTarget->UnloadBuffer[lc28] = -1;
							//set unload buffer
							for (int lc27 = 0; lc27 < wUnitsIssue; lc27++)
							{
								//increment pdwTrav
								pdwTrav++;
								//set in buffer
								if (lc27 < 10)
									pTarget->UnloadBuffer[lc27] = *pdwTrav;
						//DEBUG
						char NText[50];
						itoa(*pdwTrav, NText, 10);
						Write_Log(NText);
						WriteLog_Enter();
							}
								//now set the information in the target unit
								//check if unit actually supports the command
								BOOL bExecutable = FALSE;

									//now issue relevant load information to this unit
											pTarget->Pos_sqtarget.x = sqTarget.x;
											pTarget->Pos_sqtarget.y = sqTarget.y; 
											pTarget->Pos_sqtarget.index = sqTarget.index;
										pTarget->bExternalOrderCompleted = FALSE;
										pTarget->wExternalOrderIssued = CQ2TYPE_UNLOAD;
										pTarget->bMode_L6 = UMODE_L6_NOTHING;
										pTarget->bMode_L5 = UMODE_L5_NOTHING;
										pTarget->bMode_L4 = UMODE_L4_UNLOAD;
										//destroy unit's current path list
										pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pTarget->pUnitPath);
										pTarget->pUnitPath = NULL;

						//DEBUG
						Write_Log("Unload command ordered");
						WriteLog_Enter();

								//set acknowledgement
								if (pTarget->iControl == pCA->pGameOptions->iControl)
								{
									//	int iRet = pCA->pAckTable->Get_Ack(FALSE, pUnit->iType, ACK_TYPE_CMDRECV_UNIT_UNLOAD, pUnit->iPersonality, rand() % 3);
									//	Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, pUnit->iID, 0, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
								}
						//DEBUG
						Write_Log("Unload command complete");
						WriteLog_Enter();
						}
						return 0;
					}


		}
		else if (pQ2Tr->wType == CQ2TYPE_UNLOAD_MATERIAL)
		{
			//this is almost identical to CQ2TYPE_LOAD_MATERIAL
				//begin extracting data from the command
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					if (lExecTick > pCA->pTimer->iCurrTick)
					{
						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{
						return 2;
					}
				}
					else
					{
						//1st set, 1st dword is type. skip  (OBJTYPE_UNIT)
						pdwTrav++;
						//1st set, 2nd dword is quantity, skip (1)
						pdwTrav++;
						//1st set, 3rd dword is ID, get
						int iID = *pdwTrav;
						pdwTrav++;
						//2nd set, 1st dword is type. skip (OBJTYPE_MAPTILE)
						pdwTrav++;
						//2nd set, 2nd dword is quantity, skip (1)
						pdwTrav++;
						//2nd set, 3rd dword is Map index, get
						int iMapIndex = *pdwTrav;

						cUnitA* pUnit = pCA->pUnitTable->pUnitTable[iID];

						if (pUnit != NULL)
						{
							//set mode
							pUnit->bMode_L6 = UMODE_L6_NOTHING;
							pUnit->bMode_L5 = UMODE_L5_NOTHING;
							pUnit->bMode_L4 = UMODE_L4_UNLOAD_MATERIAL;

							//set information
							pUnit->sqUnloadMaterialLocation.index = iMapIndex;
							pUnit->sqUnloadMaterialLocation.Calc_xy(pCA->pGameWorld->iWidth);

							//destroy unit's current path list
							pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pUnit->pUnitPath);
							pUnit->pUnitPath = NULL;

							//set target
							pUnit->Pos_sqtarget.index = iMapIndex;
							pUnit->Pos_sqtarget.Calc_xy(pCA->pGameWorld->iWidth);

							//set external order
							pUnit->bExternalOrderCompleted = FALSE;
							pUnit->wExternalOrderIssued = CQ2TYPE_LOAD_MATERIAL;

							//set acknowledgement
								//set acknowledgement
								if (pUnit->iControl == pCA->pGameOptions->iControl)
								{
										int iRet = pCA->pAckTable->Get_Ack(FALSE, pUnit->iType, ACK_TYPE_CMDRECV_UNIT_EMPTY, pUnit->iPersonality, rand() % 3);
										Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, pUnit->iID, 0, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
								}
						}
					}

		}
		else if (pQ2Tr->wType == CQ2TYPE_ARM_MATERIAL)
		{
				//begin extracting data from the command
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					if (lExecTick > pCA->pTimer->iCurrTick)
					{
						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{
						return 2;
					}
				}
					else
					{
						//1st set, 1st dword is type. skip  (OBJTYPE_UNIT)
						pdwTrav++;
						//1st set, 2nd dword is # of units, get
						int iNumUnits = *pdwTrav;

						//now, attempt to arm each of these units
						for (int lc70 = 0; lc70 < iNumUnits; lc70++)
						{
							//DEBUG
							pCA->pChatBuffer->Add_CBNode("Unit Armed.", 120);

							pdwTrav++;						
							DWORD dwID = *pdwTrav;
							
							cUnitA* pUnit = pCA->pUnitTable->pUnitTable[dwID];

							pUnit->OverloadInfo.bActive = TRUE;
							pUnit->OverloadInfo.iOverLoadType = pUnit->Storage.iMaterialType;
							//now deplete this unit's storage
							pUnit->Storage.ddMaterialStored = 0;
							pUnit->Storage.iMaterialType = 0;
							//and update unit's supported commands
							pUnit->Update_SupportedCommands();
						}
					}
		}
		else if (pQ2Tr->wType == CQ2TYPE_STRUCT_TURN_OFF || pQ2Tr->wType == CQ2TYPE_STRUCT_TURN_ON)
		{
				//begin extracting data from the command
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					if (lExecTick > pCA->pTimer->iCurrTick)
					{
						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{
						return 2;
					}
				}
					else
					{
						//1st set, 1st dword is type. skip  (OBJTYPE_STRUCT)
						pdwTrav++;
						//1st set, 2nd dword is quantity, skip (1)
						pdwTrav++;
						//1st set, 3rd dword is ID, get
						int iID = *pdwTrav;
						pdwTrav++;

						cStructureA* pStruct = pCA->pStructIDTable->pTable[iID];

						if (pQ2Tr->wType == CQ2TYPE_STRUCT_TURN_OFF)
						{
							if (pStruct->iControl == pCA->pGameOptions->iControl)
							{
								int iRet = pCA->pAckTable->Get_Ack(TRUE, pStruct->iType, ACK_TYPE_STRUCT_TURN_OFF, 0, rand() % 3);
								Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, pStruct->iID, 0, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
							}
							pStruct->bOn = FALSE;
						}
						if (pQ2Tr->wType == CQ2TYPE_STRUCT_TURN_ON)
						{
							if (pStruct->iControl == pCA->pGameOptions->iControl)
							{
								int iRet = pCA->pAckTable->Get_Ack(TRUE, pStruct->iType, ACK_TYPE_STRUCT_TURN_ON, 0, rand() % 3);
								Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, pStruct->iID, 0, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
							}
							pStruct->bOn = TRUE;
						}

					}

		}  
		else if (pQ2Tr->wType == CQ2TYPE_LAY_LINK)
		{
				//begin extracting data from the command
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					if (lExecTick > pCA->pTimer->iCurrTick)
					{
						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{
						return 2;
					}
				}
					else
					{
						//1st set, 1st dword is type. skip  (OBJTYPE_MAPTILE)
						pdwTrav++;
						//1st set, 2nd dword is quantity, skip (1)
						pdwTrav++;
						//1st set, 3rd dword is location
						int iIndex = *pdwTrav;
						pdwTrav++;

						//now do it
						pCA->pGameWorld->Add_Link(iIndex);

					}

		}
		else if (pQ2Tr->wType == CQ2TYPE_LOAD_MATERIAL)
		{
				//begin extracting data from the command
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					if (lExecTick > pCA->pTimer->iCurrTick)
					{
						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{
						return 2;
					}
				}
					else
					{
						//1st set, 1st dword is type. skip  (OBJTYPE_UNIT)
						pdwTrav++;
						//1st set, 2nd dword is quantity, skip (1)
						pdwTrav++;
						//1st set, 3rd dword is ID, get
						int iID = *pdwTrav;
						pdwTrav++;
						//2nd set, 1st dword is type. skip (OBJTYPE_MAPTILE)
						pdwTrav++;
						//2nd set, 2nd dword is quantity, skip (1)
						pdwTrav++;
						//2nd set, 3rd dword is Map index, get
						int iMapIndex = *pdwTrav;

						cUnitA* pUnit = pCA->pUnitTable->pUnitTable[iID];

						if (pUnit != NULL)
						{
							//set mode
							pUnit->bMode_L6 = UMODE_L6_NOTHING;
							pUnit->bMode_L5 = UMODE_L5_NOTHING;
							pUnit->bMode_L4 = UMODE_L4_LOAD_MATERIAL;

							//set information
							pUnit->sqLoadMaterialLocation.index = iMapIndex;
							pUnit->sqLoadMaterialLocation.Calc_xy(pCA->pGameWorld->iWidth);

							//destroy unit's current path list
							pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pUnit->pUnitPath);
							pUnit->pUnitPath = NULL;

							//set target
							pUnit->Pos_sqtarget.index = iMapIndex;
							pUnit->Pos_sqtarget.Calc_xy(pCA->pGameWorld->iWidth);

							//set external order
							pUnit->bExternalOrderCompleted = FALSE;
							pUnit->wExternalOrderIssued = CQ2TYPE_LOAD_MATERIAL;

							//set acknowledgement
							if (pUnit->iControl == pCA->pGameOptions->iControl)
							{
								int iRet = pCA->pAckTable->Get_Ack(FALSE, pUnit->iType, ACK_TYPE_CMDRECV_UNIT_FILL, pUnit->iPersonality, rand() % 3);
								Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, pUnit->iID, 0, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
							}
						}
					}
		}
		else if (pQ2Tr->wType == CQ2TYPE_LOAD)
		{
				//begin extracting data from the command
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					if (lExecTick > pCA->pTimer->iCurrTick)
					{
						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{
						return 2;
					}
				}
					else
					{
						//the second DWORD is the type of IDs being presented. it can be skipped(for now, error checking possibly later)
						//because it is already known that the type of ID being presented are UNIT IDs.
						pdwTrav++;
						//the third DWORD is the number of units being issued to
						wUnitsIssue = *pdwTrav;
						//save the position of the current pointer as the position just before we issue to the UNIT IDs
						pUnitsIssue = pdwTrav;
						//now get the target type and ID
						pdwTrav++;
						pdwTrav += wUnitsIssue; //this will jump us just past the last to-issue Unit ID
						//read in the target type
						dwTargetType = *pdwTrav;
						pdwTrav += 2; // jump over the NUMBER of targets because we know there is only 1
						//now read in the ID
						dwID = *pdwTrav;
						//now go back and issue ATTACK orders to the units
						pdwTrav = pUnitsIssue;

						iIssueFlag = 0;
						//check for continued existence of specified target UNIT
						cUnitA* pTarget = pCA->pUnitTable->pUnitTable[dwID];
							if (pTarget == NULL)
								iIssueFlag = 1;   //set flag if unit is NOT there
							//if the target isn't capable of loading, no good either
							if (pTarget->iStorageVolume <= 0)
							{
								iIssueFlag = 1;
								pCA->pChatBuffer->Add_CBNode("This unit has no storage available.", 60);
							}
						//keep track of number of acknowledgements issued
						int iAcknowledgments = 0;
						//don't issue if a target unit or structure was not found
						if (iIssueFlag == 0)
						for (int lc27 = 0; lc27 < wUnitsIssue; lc27++)
						{
								//increment pdwTrav
								pdwTrav++;
								//now get the unit pointer
								pUnit = pCA->pUnitTable->pUnitTable[*pdwTrav];
							//check if unit actually supports the command
							BOOL bExecutable = FALSE;
							for (int lc90 = 0; lc90 < pUnit->iiNumSupportedCommands; lc90++)
							if (pUnit->piSupportedCommands[lc90] == CQ2TYPE_LOAD)
								bExecutable = TRUE;
							if (bExecutable == TRUE)
							{
								//now issue relevant load information to this unit
										pUnit->Pos_sqtarget.x = pTarget->Pos_sqcurr.x;
										pUnit->Pos_sqtarget.y = pTarget->Pos_sqcurr.y;
										pUnit->Pos_sqtarget.index = pTarget->Pos_sqcurr.index;
									pUnit->iLoadTarget_ID = pTarget->iID;
									pUnit->bExternalOrderCompleted = FALSE;
									pUnit->wExternalOrderIssued = CQ2TYPE_LOAD;
									pUnit->bMode_L6 = UMODE_L6_NOTHING;
									pUnit->bMode_L5 = UMODE_L5_NOTHING;
									pUnit->bMode_L4 = UMODE_L4_LOAD;
									//destroy unit's current path list
									pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pUnit->pUnitPath);
									pUnit->pUnitPath = NULL;
									
								//issue acknowledgment
								if (iAcknowledgments <= 2)
								if (pUnit->iControl == pCA->pGameOptions->iControl)
								{
									//set acknowledgement
										int iRet = pCA->pAckTable->Get_Ack(FALSE, pUnit->iType, ACK_TYPE_CMDRECV_UNIT_LOAD, pUnit->iPersonality, rand() % 3);
										Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, pUnit->iID, 0, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
									iAcknowledgments++;
								}		
							}
						}
					}


		}
		else if (pQ2Tr->wType == CQ2TYPE_START_BUILD_UNIT_CUSTOM)
		{
			
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{

					if (lExecTick > pCA->pTimer->iCurrTick)
					{

						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{

						return 2;
					}
				}
					else
					{

						//we know the next two DWORDS are OBJTYPE_STRUCTURE and 1, so skip them
						pdwTrav += 2;
						//the next one is the struture ID, so get that
						dwID = *pdwTrav;
						//get pointer to structure
						pStruct = pCA->pStructIDTable->pTable[dwID];

						pdwTrav++;

						//the next DWORD is OBJTYPE_CTYPEGROUP, so skip it
						
						pdwTrav++;						

						//the next is the number of components, save that
						int iNumComponents = *pdwTrav;

						//the execution here is almost the same as BUILD_PRECONFIG after this;
						//in this case we just get the types from the command directly here (08.20.2003)
						arrIDs = new int[iNumComponents];
						int* arrTypes = new int[iNumComponents];
						for (int c99 = 0; c99 < iNumComponents; c99++)
						{
							arrIDs[c99] = -1; 
							arrTypes[c99] = -1; 
						}

						BOOL bFrameFound = FALSE;
						//simply copy from one to the other
						for (lc1 = 0; lc1 < iNumComponents; lc1++)
						{
							pdwTrav++;
							arrTypes[lc1] = *pdwTrav;
							if (lc1 >= 0)
							if (pCA->pCmpInfoTable->pCmpTable[arrTypes[lc1]] != NULL)
							if (pCA->pCmpInfoTable->pCmpTable[arrTypes[lc1]]->iClass == CMPCLASS_FRAME)
								bFrameFound = TRUE;
						}
						if (bFrameFound == TRUE)
						{
						//now call and get IDs
						int iRet = Scan_Inventory_A(pStruct->iControl, OBJTYPE_STRUCTURE, pStruct->iID, arrTypes, arrIDs, iNumComponents, TRUE, TRUE);

						//get component cost
						double ddCost = Calc_ResourceCost(arrTypes, arrIDs, iNumComponents);


							DWORD* pdwIDs = new DWORD[iNumComponents];

							//copy from int* to dword*
							for (lc1 = 0; lc1 < iNumComponents; lc1++)
							{
								pdwIDs[lc1] = arrIDs[lc1];
							}

							//this command 
							pPO_New = new cProgressiveOrder;
							ZeroMemory(pPO_New, sizeof(cProgressiveOrder));
							pPO_New->Init(pCA);
	
							//now add the second command(progress)
							pCmd2 = new PCMD2;

							pCmd2->wType = CQ2TYPE_BUILD_PROGRESS_UNIT;
							//for now, we're simply going to put "100" and "100" into cost and difficulty (08.09.2003)
							//but in the full cost... 
							pCmd2->pData = Assemble_CommData_Build_Progress_Unit(pPO_New, dwID, ddCost, 0.01, 0, &iRetAssemble);
							pCmd2->wDataSize = iRetAssemble;

							pPO_New->Add_Cmd2(pCmd2, FALSE, 1);

							//now add the third command(create)
							pCmd2 = new PCMD2;
	
							pCmd2->wType = CQ2TYPE_CREATE_UNIT_A;
							pCmd2->pData = Assemble_CommData_Create_Unit_A(pPO_New, dwID, 0, arrIDs, iNumComponents, arrTypes, iNumComponents,&iRetAssemble);
							pCmd2->wDataSize = iRetAssemble;

							pPO_New->Add_Cmd2(pCmd2, FALSE, 2);
						
							//set the PO's name as "Build Unit" (for now) 08.12.2003
							
							pPO_New->pName = pCA->pStringTable->pTable[STRI_BUILD_UNIT];

							//now dispatch this to the structure's Order Queue
							pStruct->OrderQueue.Add_PO(pPO_New, FALSE);  //try for now deleting pPO_NEW (03.05.2004)
								
							//because we specified nocopy, we don't have to free pPO_New
							delete [] pdwIDs;
							delete [] arrIDs;
							delete [] arrTypes;	
						}
						if (bFrameFound == FALSE)
						{
							pCA->pChatBuffer->Add_CBNode("Unit Must Have a FRAME!", 100);
							delete [] arrIDs;
							delete [] arrTypes;	
						}
					}
		}
		else if (pQ2Tr->wType == CQ2TYPE_START_BUILD_UNIT_PRECONFIG)
		{
//DEBUG
Write_Log("CQ2TYPE_START_BUILD_UNIT_PRECONFIG processing started");
WriteLog_Enter();
				pdwTrav = (DWORD*)pQ2Tr->pData;
				//get execution tick time
				lExecTick = *pdwTrav;
				pdwTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{

					if (lExecTick > pCA->pTimer->iCurrTick)
					{

						return 1;
					}
					if (lExecTick < pCA->pTimer->iCurrTick)
					{
						return 2;
					}
				}
					else
					{
						//we know the next two DWORDS are OBJTYPE_STRUCTURE and 1, so skip them
						pdwTrav += 2;

						//the next one is the struture ID, so get that
						dwID = *pdwTrav;
						//get pointer to structure
						pStruct = pCA->pStructIDTable->pTable[dwID];

						//break if structure no longer exists (was destroyed)
						if (pStruct == NULL)
						{
							//DEBUG
							pCA->pChatBuffer->Add_CBNode("Error 114: Build Order Submitted To DNE Structure",1000);
							//DEBUG
							Write_Log("Error 114: Build Order Submitted To DNE Structure");
							WriteLog_Enter();
							return 0;
						}

						pdwTrav++;

						//the next two DWORDS are OBJTYPE_UTYPE and 1, so skip them
						pdwTrav += 2;
						//get a hold of the unit type
						iUnitType = *pdwTrav;

						//we have gathered all the information we needed from the incoming command.
						//we now need to create the progressive order that will be added to the Structure' order queue
						//create a buffer of component IDs
						//get the number of components
//DEBUG
Write_Log("    Execute_Command2: Recompiling TL");
WriteLog_Enter();
						pCA->pNationManager->ppNations[pStruct->iControl]->NationalInventory.Recompile_TL();

						int iNumComponents = pCA->pUTypeTable->UnitTypes[iUnitType].CmpAttached.iNumComponents;

						arrIDs = new int[iNumComponents];
						int* arrTypes = new int[iNumComponents];
						for (int c98 = 0; c98 < iNumComponents; c98++)
						{
							arrIDs[c98] = -1;	
							arrTypes[c98] = -1;	
						}

						pCmp_Trav = (cComponentA*)pCA->pUTypeTable->UnitTypes[iUnitType].CmpAttached.pCmpList_First->pNext_list;
						lc1 = 0;
						int lc3 = 0;

						//now get the component types specified by this unit type
						for (lc1 = 0; lc1 < iNumComponents; lc1++)
						{
							arrTypes[lc1] = pCmp_Trav->iType;

							pCmp_Trav = (cComponentA*)pCmp_Trav->pNext_list;
						}
//DEBUG
Write_Log("    Execute_Command2: Scanning Inventory");
WriteLog_Enter();
						//now call and get IDs
						int iRet = Scan_Inventory_A(pStruct->iControl, OBJTYPE_STRUCTURE, pStruct->iID, arrTypes, arrIDs, iNumComponents, TRUE, TRUE);
//DEBUG
Write_Log("    Execute_Command2: Calculating Total Resource Cost");
WriteLog_Enter();
						//get component cost
						double ddCost = Calc_ResourceCost(arrTypes, arrIDs, iNumComponents);

							DWORD* pdwIDs = new DWORD[iNumComponents];

							//copy from int* to dword*
							for (lc1 = 0; lc1 < iNumComponents; lc1++)
							{
								pdwIDs[lc1] = arrIDs[lc1];
							}

							//this command 
							pPO_New = new cProgressiveOrder;
							pPO_New->Init(pCA);
	
							//now add the second command(progress)
							pCmd2 = new PCMD2;

							pCmd2->wType = CQ2TYPE_BUILD_PROGRESS_UNIT;
//DEBUG
Write_Log("    Execute_Command2 - COMM10: Assembling Progressive order - command 1");
WriteLog_Enter();
							//for now, we're simply going to put "100" and "100" into cost and difficulty (08.09.2003)
							int iret = 0;
							pCmd2->pData = Assemble_CommData_Build_Progress_Unit(pPO_New, dwID, ddCost, 0.01, iUnitType, &iret);
							pCmd2->wDataSize = iret;
							pPO_New->Add_Cmd2(pCmd2, TRUE, 1);

							delete pCmd2->pData;
							delete pCmd2;

							//now add the third command(create)
							pCmd2 = new PCMD2;
	
//DEBUG
Write_Log("    Execute_Command2 - COMM10: Assembling Progressive order - command 2");
WriteLog_Enter();

							pCmd2->wType = CQ2TYPE_CREATE_UNIT_A;
							pCmd2->pData = Assemble_CommData_Create_Unit_A(pPO_New, dwID, iUnitType, arrIDs, iNumComponents, arrTypes, iNumComponents, &iRetAssemble);
							pCmd2->wDataSize = iRetAssemble;

							pPO_New->Add_Cmd2(pCmd2, TRUE, 2);
							
							delete pCmd2->pData;
							delete pCmd2;
							
							//set the PO's name as "Build Unit" (for now) 08.12.2003
							pPO_New->pName = pCA->pUTypeTable->UnitTypes[iUnitType].Name; //pCA->pStringTable->pTable[STRI_BUILD_UNIT];

//DEBUG
Write_Log("    Execute_Command2 - COMM10: Adding Progressive Order to Structure");
WriteLog_Enter();
							//now dispatch this to the structure's Order Queue
							pStruct->OrderQueue.Add_PO(pPO_New, FALSE);
							//because we specified nocopy, we don't have to free pPO_New
							delete [] pdwIDs;
							delete [] arrIDs;
							delete [] arrTypes;	

//DEBUG
Write_Log("    Execute_Command2 - COMM10: Processing Complete");
WriteLog_Enter();
					}

		}
		else if (pQ2Tr->wType == CQ2TYPE_BUILD_PROGRESS_UNIT)
		{

			char NText[50];
			//this is temporary as for the moment I'm too lazy to specify things(and certain framework is missing) and just want to see it work (08.07.2003)
				//begin extracting data from the command  (08.07.2003)
				pdwTrav = (DWORD*)pQ2Tr->pData;
				ppTrav = (VOID**)pdwTrav;
				pPO = (cProgressiveOrder*)*ppTrav;
				ppTrav++;
				pdwTrav = (DWORD*)ppTrav;

				//the next DWORD is the structure ID, which we need to figure out which nation is building
				int iStructureID = *pdwTrav;

				//check for DNE structure
				if (pCA->pStructIDTable->pTable[iStructureID] == NULL)
				{
					//DEBUG
					pCA->pChatBuffer->Add_CBNode("Error 114: Build Order Submitted To DNE Structure",1000);
					return 0;
				}

				int iControl = pCA->pStructIDTable->pTable[iStructureID]->iControl;

				pdwTrav++;
				
				double* pddTrav = (double*)pdwTrav;
				double ddResourceDeduction = *pddTrav;
				pddTrav++;

				double ddConstructionDifficulty = *pddTrav;
				pddTrav++;

				pdwTrav = (DWORD*)pddTrav;
				int iUnitType = *pdwTrav;

				UNITTYPEINFO* pInfo = NULL;

				if (iUnitType > 0) // > 0 : not a typo
					pInfo = &pCA->pUTypeTable->UnitTypes[iUnitType];

				if (iUnitType > 0) 
				{

					BOOL bEnoughResources = TRUE;
					int iStructPosition =  pCA->pStructIDTable->pTable[iStructureID]->Pos_primary.index;
					//create a structure to store all the storage pointers
					cStorageModule* pStorageModules[NUM_MATERIAL_TYPES];
					//check to see whether resources are available
					for (int lc44 = 0; lc44 < NUM_MATERIAL_TYPES; lc44++)
					if (pInfo->MaterialCost.Materials[lc44] > 0)
					{
						pStorageModules[lc44] = pCA->pPathFinder->Find_ConnectedTank(iStructPosition, lc44, (pInfo->MaterialCost.Materials[lc44])*ddConstructionDifficulty*0.99, FINDTANKMODE_TAKEFROM, 0);
						if (pStorageModules[lc44] == NULL)  //if we couldn't find a tank...
						{
							bEnoughResources = FALSE;
						}
						else if (pStorageModules[lc44]->ddMaterialStored <= 0) //if the tank is compatible but doesn't have anything in it...
						{
							bEnoughResources = FALSE;
						}
						if (bEnoughResources == FALSE)
						{	
							break;
						}
					}

					//if the resources are there, proceed to take them away from the tank
					if (bEnoughResources == TRUE)
					{
						for (int lc45 = 0; lc45 < NUM_MATERIAL_TYPES; lc45++)
						if (pInfo->MaterialCost.Materials[lc45] > 0)
						{
							pStorageModules[lc45]->ddMaterialStored -= (pInfo->MaterialCost.Materials[lc45] * ddConstructionDifficulty);
							if (pStorageModules[lc45]->bIsCarrierStructure == TRUE)
							if (pStorageModules[lc45]->iCarrierID >= 0)
							{
								cStructureA* pStruct = pCA->pStructIDTable->pTable[pStorageModules[lc45]->iCarrierID];
								pStruct->iLastMaterialWithdrawn = pCA->pTimer->iCurrTick;
							}

							if (pStorageModules[lc45]->ddMaterialStored <= 0)
							{
								pStorageModules[lc45]->ddMaterialStored  = 0;
							}
						}
						//then advance fCompletion by construction difficulty
						pPO->fCompletion += ddConstructionDifficulty;   
						//this is to mediate a problem with rounding error involved with the user perceiving discrete amounts of 
						//resources, but the underlying system using fixed-point amounts. (07.07.2007)
						//the way this works out, I'll essentially be giving players a tiny bonus in resources that they hopefully won't notice
						//if (pPO->fCompletion >= 0.97)
						//{
					//		pCA->pChatBuffer->Add_CBNode("Rounded up.", 60);
					//		pPO->fCompletion = 1.0;
					//	}
																				 
																				 
					} else
					{
						//return immediately (progressive order will stall)
						return 0;	
					}
				}

				//note that as of now you won't be able to build a custom unit on the fly (as currently implemented)
				//because the BUILD_PROGRESS_UNIT command doesn't have any information about 
				//the "custom" unit type's components(and their cost) embedded in it.
				//if you ran the command now, as implemented, the build would stall. (10.23.2006)
				if (iUnitType <= 0)
					pPO->fCompletion += ddConstructionDifficulty;  //or, better yet, just give you free units!

				return 0;
				
/*
				//check to see whether the nation can actually afford this construction
				if (pCA->pNationManager->ppNations[iControl]->ddResources > TEMP_BUILDRATE)
				{
					//do two things....increase the completion of the pPO by 0.01, and deduct the cost from the national resources
					pPO->fCompletion += TEMP_BUILDRATE / ddResourceDeduction;
					//This DOUBLE is the resource cost for this piece of constructions, so now deduct the cost from the national resources
					pCA->pNationManager->ppNations[iControl]->ddResources -= TEMP_BUILDRATE;
				} else
						{
							return 0; //return immediately. for the moment the progressive order will simply continue to issue progress commands until the unit is complete or canceled
						}
*/

		}
		else if (pQ2Tr->wType == CQ2TYPE_BUILD_PROGRESS_COMPONENT)
		{
				//this is temporary as for the moment I'm too lazy to specify things(and certain framework is missing) and just want to see it work (08.07.2003)
				//begin extracting data from the command  (08.07.2003)
				pdwTrav = (DWORD*)pQ2Tr->pData;
				ppTrav = (VOID**)pdwTrav;
				pPO = (cProgressiveOrder*)*ppTrav;

				pdwTrav++;

				//the next DWORD is the structure ID, which we need to figure out which nation is building
				int iStructureID = *pdwTrav;
				//check for DNE structure
				if (pCA->pStructIDTable->pTable[iStructureID] == NULL)
				{
					//DEBUG
					pCA->pChatBuffer->Add_CBNode("Error 114: Build Order Submitted To DNE Structure",1000);
					return 0;
				}

				int iControl = pCA->pStructIDTable->pTable[iStructureID]->iControl;

				pdwTrav++;
			
				//the next DWORD is the component type
				int iType = *pdwTrav;

				//get the cost from this component
				cCmpInfo* pCmpInfo = pCA->pCmpInfoTable->pCmpTable[iType];
				
				//check to see whether the nation can actually afford this construction
				if (pCA->pNationManager->ppNations[iControl]->ddResources > TEMP_BUILDRATE)
				{
					
					pPO->fCompletion += TEMP_BUILDRATE / pCmpInfo->ddResourceCost;
					//This DOUBLE is the resource cost for this piece of construction, so now deduct the cost from the national resources
					pCA->pNationManager->ppNations[iControl]->ddResources -= TEMP_BUILDRATE;
				} else
						{
							return 0; //return immediately. for the moment the progressive order will simply continue to issue progress commands until the component is complete or canceled
						}

		}
		else if (pQ2Tr->wType == CQ2TYPE_EARMARK_COMPONENTS)
		{
				//get the calling ProgOrder
				pdwTrav = (DWORD*)pQ2Tr->pData;
				ppTrav = (VOID**)pdwTrav;

				pPO = (cProgressiveOrder*)*ppTrav;
				ppTrav++;
				//get the structure ID
				pdwTrav = (DWORD*)ppTrav;
				dwID = *pdwTrav;
				pdwTrav++;
				//get pointer to the structure
				pStruct = pCA->pStructIDTable->pTable[dwID];
				//check for DNE structure
				if (pStruct == NULL)
				{
					//DEBUG
					pCA->pChatBuffer->Add_CBNode("Error 114: Build Order Submitted To DNE Structure",1000);
					return 0;
				}

				//now we know the next number is OBJTYPE_COMPONENT, so skip it
				pdwTrav++;
				//get the number of components
				iNumComponents = *pdwTrav;

				int iFailFlag = 0;
				//now beginning earmarking components
				for (lc1 = 0; lc1 < iNumComponents; lc1++)
				if (iFailFlag == 0)
				{
					pdwTrav++; //this goes at the beginning to prevent incrementing into unintended memory spaces

					pComponent = pCA->pCmpIDTable->pTable[*pdwTrav];

					if (pComponent->iOwnerID == OWNER_NATIONAL_INVENTORY || pComponent->iOwnerType == OBJTYPE_STRUCTURE)
					if (pComponent->iOwnerID == dwID || pComponent->iOwnerID == OWNER_NATIONAL_INVENTORY) //not the best if-sorting, I know. I'm tired. 08.19.2003)
					{
						pComponent->iOwnerType = OBJTYPE_STRUCTURE;
						pComponent->iOwnerID = dwID;
					}
						else
							iFailFlag = 1;

					//now move this component into the structure's own inventory
					//pStruct->CmpInventory->Insert_Component(pComponent); //disabled because it is believed that this will make cancel operations more difficult (08.17.2003)
				}
				//if we failed, delete the calling prog order
				//ALERT not done. (08.19.2003)
				if (iFailFlag == 1)
				{
					pStruct->OrderQueue.Delete_PO(pPO);
				}

				//force a recompile of the National Inventory TL because we have removed components from it
				pCA->pNationManager->ppNations[pStruct->iControl]->NationalInventory.Recompile_TL();

		}
		else if (pQ2Tr->wType == CQ2TYPE_CREATE_UNIT_A)
		{
			//get the calling ProgOrder
				pdwTrav = (DWORD*)pQ2Tr->pData;
				ppTrav = (VOID**)pdwTrav;

				pPO = (cProgressiveOrder*)*ppTrav;
				ppTrav++;
				//get the structure ID
				pdwTrav = (DWORD*)ppTrav;

				dwID = *pdwTrav;
				pdwTrav++;

				DWORD dwType = *pdwTrav;

				pdwTrav++;
				//get pointer to the structure
				pStruct = pCA->pStructIDTable->pTable[dwID];
				//check for DNE structure
				if (pStruct == NULL)
				{
					//DEBUG
					pCA->pChatBuffer->Add_CBNode("Error 114: Build Order Submitted To DNE Structure",1000);
					return 0;
				}

				//now we know the next number is OBJTYPE_COMPONENT, so skip it
				pdwTrav++;

				//we now create a BLANK unit at 1 space to the left of the structure's primary position  (we need something to handle how to get the name there)
				//set the unit down at the primary position and adjust target
					pUnit = pCA->pUnitList->Create_Unit(pCA->pUTypeTable->UnitTypes[dwType].Name, dwType, pStruct->iControl, pStruct->Pos_primary.x, pStruct->Pos_primary.y);
/*
					pUnit = pCA->pUnitList->Create_Unit(pCA->pStringTable->Supply_Random_String(pStruct->iControl), 0, pStruct->iControl, pStruct->Pos_primary.x - 1, pStruct->Pos_primary.y);
						else
					pUnit = pCA->pUnitList->Create_Unit(pCA->pStringTable->Supply_Random_String(pStruct->iControl), 0, pStruct->iControl, pStruct->Pos_primary.x, pStruct->Pos_primary.y - 1);
*/
				SQCOORDSET Pos_sqcurr;
				Pos_sqcurr.index = pUnit->Pos_sqcurr.index;
				Pos_sqcurr.Calc_xy(pCA->pGameWorld->iWidth);
				//very quickly unregister the unit so that it can be placed correctly later(03.12.2004)
				pCA->pGameWorld->Unregister_Unit(pUnit->Pos_sqcurr, pUnit);

				//now add components to the unit
				//get the number of components
				iNumComponents = *pdwTrav;
				//now beginning adding components
				for (lc1 = 0; lc1 < iNumComponents; lc1++)
				{
					pdwTrav++; //this goes at the beginning to prevent incrementing into unintended memory spaces
					//no longer do this; it is directly done in the create unit command
					//pComponent = pCA->pCmpIDTable->pTable[*pdwTrav];
					//pComponent->Set_OwnerType(OBJTYPE_UNIT); 
					//pComponent->Set_OwnerID(pUnit->iID);
					//pUnit->CmpAttached.Insert_Component(pComponent);
				}

				//set the unit's shortname
				UNITTYPEINFO* pType = &pCA->pUTypeTable->UnitTypes[dwType];
				char* ShortName = pCA->pUTypeTable->UnitTypes[dwType].ShortName;

				//HACK (if type 0, put shortname as "CSTM")
				
				pUnit->ShortName = new char[pType->iShortNameLength + 7];
				ZeroMemory( pUnit->ShortName, sizeof(char)*(pType->iShortNameLength + 7));
				strcpy(pUnit->ShortName, ShortName);
				itoa(pType->iInstances[pStruct->iControl], NText, 10);
				
				if (dwType == 0)
				{
					pUnit->ShortName[0] = 'C';
					pUnit->ShortName[1] = 'S';
					pUnit->ShortName[2] = 'T';
					pUnit->ShortName[3] = 'M';

				}
				
				//concatenate
				strcat(pUnit->ShortName, NText);
				pCA->pUTypeTable->UnitTypes[dwType].iInstances[pStruct->iControl] += 1;

				//now get the second part
				//first entry is OBJTYPE_CTYPE, skip it
				pdwTrav++;
				pdwTrav++;
				//second entry is number of entries
				iNumComponents = *pdwTrav;
				//now add components again
				for (lc1 = 0; lc1 < iNumComponents; lc1++)
				{

					pdwTrav++;
					
					//don't do this anymore -- although it breaks the "build custom unit" command, that's not in the game anymore 03.15.2007
					//HACK: SOMETHING keeps generating fictitious component values....currently I am only putting this patch here to stop it
					//I think something is misaligning the pData's
					//02.17.2004
					//if (pCA->pCmpInfoTable->pCmpTable[*pdwTrav] != NULL)
					///pUnit->CmpAttached.Add_Component(*pdwTrav);
						//else
						//{
					//		break;

					//	}
				}

				pUnit->Recalc_Attributes();

				//now figure out where to put it
				int iIndex_place = pCA->pPathFinder->Adjust_Target2(Pos_sqcurr.index, pUnit->iID);
				pUnit->Pos_sqcurr.index = iIndex_place;
				pUnit->Pos_sqcurr.Calc_xy(pCA->pGameWorld->iWidth);
				pUnit->Pos_ABS.x = pUnit->Pos_sqcurr.x * TILE_SIZE;
				pUnit->Pos_ABS.y = pUnit->Pos_sqcurr.y * TILE_SIZE;

				pUnit->Update_Position();

					pUnit->Pos_sqimtarget.index = pUnit->Pos_sqcurr.index;
					pUnit->Pos_sqimtarget.Calc_xy(pCA->pGameWorld->iWidth);
					//clear the unit's modes
					pUnit->bMode_L6 = UMODE_L6_NOTHING;
					pUnit->bMode_L5 = UMODE_L5_NOTHING;
					pUnit->bMode_L4 = UMODE_L4_NOTHING;
					pUnit->bMode_L3 = UMODE_L3_IDLE;
					pUnit->bMode_L2 = UMODE_L2_IDLE;

				//now re-register the unit
				pCA->pGameWorld->Register_Unit(pUnit->Pos_sqcurr, pUnit);

				//////TUTORIAL HOOK
					if (pCA->pInterface->Tutorial.iTick_FirstMechInfCreated < 0)
					if (pUnit->Name != NULL)
					if (strlen(pUnit->Name) >= 7)
					if (pUnit->Name[0] == 'M')
					if (pUnit->Name[1] == 'e')
					if (pUnit->Name[2] == 'c')
					if (pUnit->Name[3] == 'h')
					if (pUnit->Name[4] == 'I')
					if (pUnit->Name[5] == 'n')
					if (pUnit->Name[6] == 'f')
					{
						//OK!	
						pCA->pInterface->Signal_Chime("Task Completed!");
						pCA->pInterface->Tutorial.iTick_FirstMechInfCreated = pCA->pTimer->iCurrTick;
					}
				////END TUTORIAL HOOK

								//issue ack
								if (pUnit->iControl == pCA->pGameOptions->iControl)
								{
									///

									//set acknowledgement
										int iRet = pCA->pAckTable->Get_Ack(FALSE, pUnit->iType, ACK_TYPE_STATUS_UNIT_BORN, pUnit->iPersonality, rand() % 3);
									/*
									char Buffer[50];
									ZeroMemory(Buffer, sizeof(char)*50);
									strcat(Buffer, "Ack: ");
									char NText[50];
									itoa(iRet, NText, 10);
									strcat(Buffer, NText);
									itoa(pUnit->iPersonality, NText, 10);
									strcat(Buffer, " Personality: ");
									strcat(Buffer, NText);
									pCA->pChatBuffer->Add_CBNode(Buffer, 150);
									*/

										Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, pUnit->iID, 0, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
								}		

		}
		else if (pQ2Tr->wType == CQ2TYPE_PERSBOXCLICK)
		{
				pCA->pInterface->iLastPersistentButtonClick = pCA->pTimer->iCurrTick;
		}
		else if (pQ2Tr->wType == CQ2TYPE_CREATE_COMPONENT_A)
		{

				//get the calling ProgOrder
				pdwTrav = (DWORD*)pQ2Tr->pData;
				ppTrav = (VOID**)pdwTrav;

				//pPO = (cProgressiveOrder*)*ppTrav;
				ppTrav++;
				//get the structure ID
				pdwTrav = (DWORD*)ppTrav;
				dwID = *pdwTrav;
				pdwTrav++;
				////get the component type
				iCmpType = *pdwTrav;
				//get pointer to the structure
				pStruct = pCA->pStructIDTable->pTable[dwID];

				//check for DNE structure
				if (pStruct == NULL)
				{
					//DEBUG
					pCA->pChatBuffer->Add_CBNode("Error 114: Build Order Submitted To DNE Structure",1000);
					return 0;
				}

				//now create the component in the corresponding national inventory
				pComponent = pCA->pNationManager->ppNations[pStruct->iControl]->NationalInventory.Add_Component(iCmpType);
				//set this component's iOwnerID as OWNER_NATIONAL_INVENTORY
				pComponent->Set_OwnerID(OWNER_NATIONAL_INVENTORY);
				pComponent->Set_OwnerType(OWNER_NATIONAL_INVENTORY);
				//force a recompile of the national inventory TL (doing it for every change of the national inventory
				//appears to be less expensive than recompiling it every time it is needed (08.11.2003)

				pCA->pNationManager->ppNations[pStruct->iControl]->NationalInventory.Recompile_TL();

		}
		else if (pQ2Tr->wType == CQ2TYPE_SETMAINMENUMODE)
		{
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;

			int iMode = *pdwTrav;

			//set mode
			pCA->pInterface->IBoxes[IBOX_MAINMENU].iActiveGrid = iMode;
			//TEST: play a sound
			pCA->pDA_Audio->Play_Primary(SOUNDSEG_TEST1);
		}
		else if (pQ2Tr->wType == CQ2TYPE_SELECT_GAMETYPE_SINGLE)
		{
			pCA->pInterface->IBoxes[IBOX_MAINMENU].iActiveGrid = TEMP_IGRID_SINGLE;
			pCA->pGameOptions->bGameType = GAMETYPE_SINGLE;
			//set latency (01.02.2003)
			pCA->pGameOptions->iLatency = 1;  //to account for the fact that the command MIGHT not execute until the next tick
			//TEST: play a sound
			pCA->pDA_Audio->Play_Primary(SOUNDSEG_TEST1);
		}
		else if (pQ2Tr->wType == CQ2TYPE_SELECT_GAMETYPE_MULTI)
		{
			pCA->pInterface->IBoxes[IBOX_MAINMENU].iActiveGrid = TEMP_IGRID_MULTI;			
			pCA->pGameOptions->bGameType = GAMETYPE_MULTI;
			pCA->pGameOptions->iLatency = TEMP_DEFAULT_LATENCY;
			//TEST: play a sound
			pCA->pDA_Audio->Play_Primary(SOUNDSEG_TEST1);
		}
		else if (pQ2Tr->wType == CQ2TYPE_SELECT_QUIT)
		{
			pCA->pAppData->bRunFlag = 0;
			pCA->pAppData->bRunStage = APP_RUNSTAGE_EXIT;	
		}
		else if (pQ2Tr->wType == CQ2TYPE_END_GAME)
		{
			pCA->pAppData->bRunStage = APP_RUNSTAGE_MAINEXIT;	
		}
		else if (pQ2Tr->wType == CQ2TYPE_SELECT_START)
		{
			//if the gametype is multi, check to make sure all players are ready
			//(go through scratch table and make sure all players have the map)
			BOOL bOK = TRUE;

			if (pCA->pGameOptions->bGameType == GAMETYPE_MULTI)
			for (int lc1 = 0; lc1 < TEMP_DP_MAXPLAYERS; lc1++)
			{
				if (pCA->pGameOptions->ScratchInfoTable[lc1] != NULL)
				if (pCA->pGameOptions->ScratchInfoTable[lc1]->bAIFlag  == FALSE)
				if (pCA->pGameOptions->ScratchInfoTable[lc1]->bHaveCurrentMap == FALSE)
				{
					bOK = FALSE;
				}
			}

			//make sure we've got a valid map
			if (pCA->pGameOptions->bGameType == GAMETYPE_SINGLE)
			if (pCA->pGameOptions->iMapIndex < 0)
			{
				bOK = FALSE;
			}
			if (bOK == TRUE)
			{
				pCA->pTimer->Init(30000, pCA); //reinitialize timer
				pCA->pTimer->Set_CPT_FPS(60); //need this otherwise it'll get set wrong (timer frequency varies between computers)

				pCA->pAppData->bRunStage = APP_RUNSTAGE_MAININIT; //new mode 05.10.2004
	 
				pCA->pDP_Multiplay->bRandKeysReceived = FALSE;
				pCA->pDP_Multiplay->bTotalPlayerCountReceived = FALSE;
				pCA->pDP_Multiplay->iTotalPlayerCount = 0;
				pCA->pDP_Multiplay->iNumNationInitsReceived = 0;
				pCA->pDP_Multiplay->iNumTimerIDInitsReceived = 0;
				pCA->pDP_Multiplay->bTotalTimerIDCountReceived = FALSE;
				pCA->pDP_Multiplay->iTotalTimerIDCount = 0;
				//if this is the host, send the message to other computres
				if (pCA->pGameOptions->bAmHost == TRUE)
					pCA->pDP_Multiplay->Send_Command(CQDEST_INTERFACE, CQTYPE_SELECT_START, pQTr->wParam1, pQTr->wParam2,pQTr->wParam3 , pQTr->fParam4, pQTr->fParam5, pQTr->pParam6, pQTr->lExecTick, FALSE, TRUE);
			}
			if (bOK == FALSE)
			if (pCA->pGameOptions->bGameType == GAMETYPE_MULTI)
			{
				char Buffer[100];
				ZeroMemory(Buffer, sizeof(char)*100);
				strcat(Buffer, "Can't Start Game: Not All Players Ready (not all have map)");
				pCA->pDP_Multiplay->Send_ChatText(Buffer, "ERROR: ", 100);
			}
		}  
		else if (pQ2Tr->wType == CQ2TYPE_SELECT_PROCEED)
		{
			pCA->pAppData->bRunStage = APP_RUNSTAGE_POSTBRIEFING;

			//temp IBOX initialization here (08.01.2002)
			pCA->pInterface->IBoxes[IBOX_MAINMENU].bActiveBox = FALSE;
			pCA->pInterface->IBoxes[IBOX_PRIMARY].bActiveBox = TRUE;
			pCA->pInterface->IBoxes[IBOX_MINIMAP].bActiveBox = TRUE;
			if (pCA->pMinimap->MMZDrawFlag == TRUE)
				pCA->pInterface->IBoxes[IBOX_MINIMAPZOOM].bActiveBox = TRUE;
			pCA->pInterface->IBoxes[IBOX_CONSOLE_EQUIP].bActiveBox = TRUE;
			pCA->pInterface->IBoxes[IBOX_CONSOLE_LIST].bActiveBox = TRUE;
			pCA->pInterface->IBoxes[IBOX_CONSOLE_COMMAND].bActiveBox = TRUE;
			pCA->pInterface->IBoxes[IBOX_CONSOLE_INFO].bActiveBox = TRUE;
			pCA->pInterface->IBoxes[IBOX_CONSOLE_DISPLAY].bActiveBox = TRUE;
		}
		else if (pQ2Tr->wType == CQ2TYPE_SELECT_MAP)
		{

			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			pCA->pGameOptions->iMapIndex = *pdwTrav;
			ZeroMemory(pCA->pGameOptions->strMultiplayerMapPath, sizeof(char)*256);
			strcat(pCA->pGameOptions->strMultiplayerMapPath, pCA->pAppData->achMapNames[pCA->pGameOptions->iMapIndex]);
			//get the map ID as well
			pCA->pGameOptions->iMapUniqueID = pCA->pGameWorld->UniqueMapIDOf(pCA->pGameOptions->strMultiplayerMapPath);
			//and set that YOU have this map
			pCA->pGameOptions->bMapLocationFinalized = TRUE;

		}
		else if (pQ2Tr->wType == CQ2TYPE_SET_ACTIVEPROVIDER)
		{
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			pCA->pDP_Multiplay->iActiveProvider = *pdwTrav;
		}
		else if (pQ2Tr->wType == CQ2TYPE_SET_ACTIVEDEVICE)
		{
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			int iProvider = *pdwTrav; pdwTrav++;

			int iDevice = *pdwTrav;

			pCA->pDP_Multiplay->pDeviceArrays[iProvider].wActiveDevice = iDevice;
		}
		else if (pQ2Tr->wType == CQ2TYPE_SET_INPUTSTRING)
		{
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			int iBufferLength = *pdwTrav;  pdwTrav++;

			char* Text = (char*)*pdwTrav;
			pCA->pInterface->pKeyEdit = Text;
			pCA->pInterface->dwKeyEditMaxBuffer = iBufferLength;
		}
		else if (pQ2Tr->wType == CQ2TYPE_SET_INPUTSTRING_WIDE)
		{
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			int iBufferLength = *pdwTrav;  pdwTrav++;

			WCHAR* Text = (WCHAR*)*pdwTrav;
			pCA->pInterface->pKeyEdit_Wide = Text;
			pCA->pInterface->dwKeyEditWideMaxBuffer = iBufferLength;
		}
		else if (pQ2Tr->wType == CQ2TYPE_SET_MAPVIEW)
		{
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;

			DWORD dwX = *pdwTrav; pdwTrav++;
			DWORD dwY = *pdwTrav;

			pCA->pInterface->map_xview = dwX;
			pCA->pInterface->map_yview = dwY;

		}
		else if (pQ2Tr->wType == CQ2TYPE_SET_ACTIVEIGRID)
		{
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			DWORD dwIBox = *pdwTrav; pdwTrav++;
			DWORD dwActive = *pdwTrav;

			pCA->pInterface->IBoxes[dwIBox].iActiveGrid = dwActive;
		}
		else if (pQ2Tr->wType == CQ2TYPE_SEND_INPUTPACKET)
		{
			INPUTPACKET* pPacket = new INPUTPACKET;
			pPacket->pItemArray = NULL;
			pPacket->pObjectArray = NULL;
			pPacket->pdwIDs = NULL;

			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			pPacket->dwPacketType = *pdwTrav; pdwTrav++;

			if (pPacket->dwPacketType == IPACKETTYPE_OBJREF)
			{		
				pPacket->dwObjectsType = *pdwTrav; pdwTrav++;
				pPacket->dwNumObjects = *pdwTrav; pdwTrav++;
				//note that at this point, pdwTrav is pointing an INLINE
				//array of IDs that we want
				//in other words, the pointer for pdwIDs we want is itself 
				//pdwTrav
				pPacket->pdwIDs = pdwTrav; 
				//now increment by the number of IDs that were specified
				pdwTrav += pPacket->dwNumObjects;
				//same applies here, the array of INLINE object references is currently
				//being pointed to by pdwTrav itself
				pPacket->pObjectArray = (VOID**)pdwTrav;
				VOID** ppvTrav = (VOID**)pdwTrav;
				if (*ppvTrav == NULL)
					pPacket->pObjectArray = NULL;

			}
			if (pPacket->dwPacketType == IPACKETTYPE_PARAM)
			{
				pPacket->dwParameterType = *pdwTrav; pdwTrav++;
				pPacket->dwNumParamItems = *pdwTrav; pdwTrav++;
				//note that when the type is IPACKETTYPE_PARAM, there is no pdwID block of parameters
				//the ItemArray INLINEd is here.
				pPacket->pItemArray = (VOID**)pdwTrav;
				VOID** ppvTrav = (VOID**)pdwTrav;
				if (*ppvTrav == NULL)
					pPacket->pItemArray = NULL;
			}
			//now issue
			//rather than adding the packet directly, we compile another packet
			//because the arrayed data in the PCMD2 disappears once this function finishes
			pCA->pInputPacketBuffer->Compile_Add_Packet(pPacket->dwPacketType,
														pPacket->dwObjectsType,
														pPacket->dwNumObjects,
														pPacket->pObjectArray,
														pPacket->pdwIDs,
														pPacket->dwParameterType,
														pPacket->dwNumParamItems,
														pPacket->pItemArray,
														TRUE);

		}
		else if (pQ2Tr->wType == CQ2TYPE_SET_PDWORD)
		{
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			DWORD Val = *pdwTrav;   pdwTrav++;
			//double pointer on purpose
			DWORD** ppdwTrav = (DWORD**)pdwTrav;

			//I could write this as a double de-reference, but that'd probably
			//give me a headache the next time I had to look at it
			DWORD* pdwPoint = *ppdwTrav;

			*pdwPoint = Val;
		}
		else if (pQ2Tr->wType == CQ2TYPE_COPY_STRING_CTOC)
		{
			void** ppvTrav = (void**)pQ2Tr->pData;
			LPSTR pv1 = (LPSTR)*ppvTrav; ppvTrav++;
			LPSTR pv2 = (LPSTR)*ppvTrav; 

			strncpy(pv2, pv1, strlen(pv1));
			
		}
		else if (pQ2Tr->wType == CQ2TYPE_COPY_STRING_WTOW)
		{
			void** ppvTrav = (void**)pQ2Tr->pData;
			LPWSTR pv1 = (LPWSTR)*ppvTrav; ppvTrav++;
			LPWSTR pv2 = (LPWSTR)*ppvTrav; 

			wcsncpy(pv2, pv1, lstrlenW(pv1));
			
		}		
		else if (pQ2Tr->wType == CQ2TYPE_COPY_STRING_CTOW)
		{
			void** ppvTrav = (void**)pQ2Tr->pData;
			LPSTR pv1 = (LPSTR)*ppvTrav; ppvTrav++;
			LPWSTR pv2 = (LPWSTR)*ppvTrav; 

			char* psrctrav = (char*)pv1;
			WCHAR* pdesttrav = (WCHAR*)pv2;

			//the 1 is there on purpose; the loop duration is reduced by one to prevent from incrementing out of the buffer
			for (int lc1 = 1; lc1 < strlen(pv1); lc1++)
			{
				//copy character
				*pdesttrav = (WCHAR)*psrctrav;
			
				//increment
				psrctrav++;
				pdesttrav++;
			}
			//copy the last character over
			*pdesttrav = (WCHAR)*psrctrav; pdesttrav++;
			//copy a NULL into the destination buffer just in case
			*pdesttrav = 0;
		}	
		else if (pQ2Tr->wType == CQ2TYPE_COPY_STRING_WTOC)
		{
			void** ppvTrav = (void**)pQ2Tr->pData;
			LPWSTR pv1 = (LPWSTR)*ppvTrav; ppvTrav++;
			LPSTR pv2 = (LPSTR)*ppvTrav; 

			//we copy the wide character array by simply ignoring the high byte in each WCHAR short
			char* psrctrav = (char*)pv1;  psrctrav++; //to align onto the low byte
			char* pdesttrav = (char*)pv2;

			//the 1 is there on purpose; the loop duration is reduced by one to prevent from incrementing out of the buffer
			for (int lc1 = 1; lc1 < lstrlenW(pv1); lc1++)
			{
				//copy character
				*pdesttrav = *psrctrav;
			
				//increment
				psrctrav += 2;   //source increments by 2
				pdesttrav++;   
			}
			//copy the last character over
			*pdesttrav = *psrctrav; pdesttrav++;
			//copy a NULL into the destination buffer just in case
			*pdesttrav = 0;
		}	
		else if (pQ2Tr->wType == CQ2TYPE_FINALIZE_MP_MENUS)
		{
			pCA->pInterface->MPSP_RunOnce();
		}
		else if (pQ2Tr->wType == CQ2TYPE_HOST_STD)
		{
			//do not allow them to proceed if no map selected
			if ( pCA->pGameOptions->iMapIndex >= 0)
			{
				int XRes = pCA->pDD_Display->i_XResD;
				int YRes = pCA->pDD_Display->i_YResD;
				RECT r; r.left = XRes * 0.4; r.right = XRes * 0.6; r.top = YRes * 0.4; r.bottom = YRes * 0.5;
				pCA->pGDisplay->Draw_Interface_Panel(pCA->pDD_Display->pDDSMain, r);
				pCA->pGDisplay->Draw_Text_Shadow(pCA->pDD_Display->pDDSMain, "Creating...", XRes * 0.42, YRes*0.45, FONT_STANDARD1, pCA->pColorTable->White - 1, pCA->pColorTable->Black); 
				//call host
				HRESULT hRet = pCA->pDP_Multiplay->HostSession(NULL);
				if (hRet == S_OK)
				{
					pCA->pInterface->IBoxes[IBOX_MAINMENU].iActiveGrid = TEMP_IGRID_MULTI3; //temp
					//clear any edit boxes open
					pCA->pInterface->pKeyEdit = NULL;
					pCA->pInterface->pKeyEdit_Wide = NULL;
				}
			}
		}
		else if (pQ2Tr->wType == CQ2TYPE_SELECT_DISCONNECT)
		{
			pCA->pDP_Multiplay->CloseSession();
			pCA->pInterface->IBoxes[IBOX_MAINMENU].iActiveGrid = 0;	
		}
		else if (pQ2Tr->wType == CQ2TYPE_RETURN_FROM_TUTORIAL)
		{
			pCA->pInterface->Tutorial.iActiveElement = -1;
		}
		else if (pQ2Tr->wType == CQ2TYPE_SET_TUTORIAL_PAGE)
		{
			//pCA->pChatBuffer->Add_CBNode("SET_TUTORIAL_PAGE Command Set.", 60);
			int* pint = (int*)pQ2Tr->pData;
			
			pCA->pInterface->Tutorial.iActiveElement = *pint;
		}
		else if (pQ2Tr->wType == CQ2TYPE_ENUMHOSTS_STD)
		{
			int XRes = pCA->pDD_Display->i_XResD;
			int YRes = pCA->pDD_Display->i_YResD;
			RECT r; r.left = XRes * 0.4; r.right = XRes * 0.6; r.top = YRes * 0.4; r.bottom = YRes * 0.5;
			pCA->pGDisplay->Draw_Interface_Panel(pCA->pDD_Display->pDDSMain, r);
			pCA->pGDisplay->Draw_Text_Shadow(pCA->pDD_Display->pDDSMain, "Searching...", XRes * 0.42, YRes*0.45, FONT_STANDARD1, pCA->pColorTable->White - 1, pCA->pColorTable->Black); 
			pCA->pDP_Multiplay->EnumHosts();
		}
		else if (pQ2Tr->wType == CQ2TYPE_SET_ACTIVEHOST)
		{
			DWORD* pdw = (DWORD*)pQ2Tr->pData;
			pCA->pDP_Multiplay->wSelectedHost = *pdw;
		}
		else if (pQ2Tr->wType == CQ2TYPE_CONNECT_STD)
		{
			//clear these out as you no longer confirm you have the map
				pCA->pGameOptions->iMapIndex = -1; //init zero map index
				pCA->pGameOptions->bMapLocationFinalized = FALSE;
				pCA->pGameOptions->iMapUniqueID = -1;
				pCA->pGameOptions->strMultiplayerMapPath[0] = 0; 

			int XRes = pCA->pDD_Display->i_XResD;
			int YRes = pCA->pDD_Display->i_YResD;
			RECT r; r.left = XRes * 0.4; r.right = XRes * 0.6; r.top = YRes * 0.4; r.bottom = YRes * 0.5;
			pCA->pGDisplay->Draw_Interface_Panel(pCA->pDD_Display->pDDSMain, r);
			pCA->pGDisplay->Draw_Text_Shadow(pCA->pDD_Display->pDDSMain, "Connecting...", XRes * 0.42, YRes*0.45, FONT_STANDARD1, pCA->pColorTable->White - 1, pCA->pColorTable->Black); 
			//call connect
			BOOL hRet = pCA->pDP_Multiplay->ConnectSession();
			if (hRet == S_OK)
			{
				pCA->pInterface->IBoxes[IBOX_MAINMENU].iActiveGrid = TEMP_IGRID_MULTI3; //temp
				//clear any edit boxes open
				pCA->pInterface->pKeyEdit = NULL;
				pCA->pInterface->pKeyEdit_Wide = NULL;
			}
		}
		else if (pQ2Tr->wType == CQ2TYPE_SENDTEXT)
		{
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			DWORD NameLength = *pdwTrav; pdwTrav++;
			WCHAR* pwcTrav = (WCHAR*)pdwTrav;
			WCHAR* Name = pwcTrav;
			pwcTrav += NameLength;
			pdwTrav = (DWORD*)pwcTrav; 
			DWORD TextLength = *pdwTrav; pdwTrav++;
			DWORD StayTime = *pdwTrav; pdwTrav++;
			char* pcTrav = (char*)pdwTrav;
			char* Text = pcTrav;

			pCA->pDP_Multiplay->Send_ChatText(Text, Name, TextLength);
		}
		else if (pQ2Tr->wType == CQ2TYPE_SENDTEXT2)
		{
			WCHAR** ppwcTrav = (WCHAR**)pQ2Tr->pData;
			WCHAR* pName = *ppwcTrav; ppwcTrav++;
			char** ppcTrav = (char**)ppwcTrav;
			char* pText = *ppcTrav; ppcTrav++;
			DWORD* pdwTrav = (DWORD*)ppcTrav;
			DWORD Buffersize = *pdwTrav; pdwTrav++;
			DWORD StayTIme = *pdwTrav;

			pCA->pDP_Multiplay->Send_ChatText(pText, pName, Buffersize);
			
		}	
		else if (pQ2Tr->wType == CQ2TYPE_CLEARCHATBUFFER)
		{
			ZeroMemory(pCA->pInterface->caChatText, sizeof(char)*1000);
		}
		else if (pQ2Tr->wType == CQ2TYPE_SELECT_UNIT)
		{
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			DWORD dwID = *pdwTrav;

			cUnitA* pUnit = pCA->pUnitTable->pUnitTable[dwID];
			pCA->pInterface->ReleaseUnits_SB_all(pCA->pInterface->pUSB_First, pCA->pInterface->pUSB_Last);
			pCA->pInterface->ReleaseStructs_SB_all(pCA->pInterface->pSSB_First, pCA->pInterface->pSSB_Last);
			pCA->pInterface->pConsole_Unit_Select = NULL;
			pCA->pInterface->pConsole_Struct_Select = NULL;
			pCA->pInterface->AddUnits_SB_singleB(dwID, pCA->pInterface->pUSB_First, pCA->pInterface->pUSB_Last);
			pCA->pInterface->pConsole_Unit_Select = (LPSBE)pCA->pInterface->pUSB_First->pNext;;  

		} else if (pQ2Tr->wType == CQ2TYPE_SELECT_STRUCT)
		{
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			DWORD dwID = *pdwTrav;

			cStructureA* pStruct = pCA->pStructIDTable->pTable[dwID];
			pCA->pInterface->ReleaseUnits_SB_all(pCA->pInterface->pUSB_First, pCA->pInterface->pUSB_Last);
			pCA->pInterface->ReleaseStructs_SB_all(pCA->pInterface->pSSB_First, pCA->pInterface->pSSB_Last);
			pCA->pInterface->pConsole_Unit_Select = NULL;
			pCA->pInterface->pConsole_Struct_Select = NULL;
			pCA->pInterface->AddStructs_SB_singleB(dwID, pCA->pInterface->pSSB_First, pCA->pInterface->pSSB_Last);

			pCA->pInterface->pConsole_Struct_Select = (LPSBE)pCA->pInterface->pSSB_First->pNext;

		} else if (pQ2Tr->wType == CQ2TYPE_SET_CONSOLE_UNIT_SELECT)
		{
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			DWORD dwID = *pdwTrav;

			LPSBE pSB_Trav = (LPSBE)pCA->pInterface->pUSB_First->pNext;
			while (pSB_Trav != pCA->pInterface->pUSB_Last)
			{
				if (pSB_Trav->iID == dwID)
					pCA->pInterface->pConsole_Unit_Select = (VOID*)pSB_Trav;

				pSB_Trav = (LPSBE)pSB_Trav->pNext;
			}

		} else if (pQ2Tr->wType == CQ2TYPE_DISABLE_ISSUE_PP)
		{
			DWORD* pdwParam1 = (DWORD*)pQ2Tr->pData;
		
			pCA->pInterface->PacketProcessor.Issue_Group_Packet(pCA->pInputPacketBuffer, *pdwParam1);
			pCA->pInterface->iScanType = VAL_INVALID;
			pCA->pInterface->PacketProcessor.Flush_Buffer2(pCA->pInterface->PacketProcessor.pIPB_First, pCA->pInterface->PacketProcessor.pIPB_Last);
			pCA->pInterface->llCutOffTime = VAL_INVALID;
		} else if (pQ2Tr->wType == CQ2TYPE_FLUSH_PP)
		{
			pCA->pInterface->PacketProcessor.Flush_Buffer2(pCA->pInterface->PacketProcessor.pIPB_First, pCA->pInterface->PacketProcessor.pIPB_Last);
		}

//DEBUGs
Write_Log("Execute_Command2 Ended.");
WriteLog_Enter();

return 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCommQueue::Execute_Commands_Q2()
{
//DEBUG
Write_Log("Execute_Commands_Q2 Begun.");
WriteLog_Enter();

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	char NText[50];
	BOOL SkipFlag = FALSE;
	

	//variables used in case statements which cannot be initialized inside
	BYTE* pDTrav;
	LONG* plTrav;
	WORD* pwTrav;
	DWORD* pdwTrav;
	DWORD* pdwTrav2;
	cUnitA* pUnit;
	cUnitA* pEnemy;
	cStructureA* pEnemy_Struct;

	LONG lExecTick;
	WORD wUnitID;
	LONG lTargetIndex;
	LONG lGuardIndex;

	DWORD dwTargetType, dwID;

	WORD wEnemyID;
	BYTE bControlID;

	int iIssueFlag;

DWORD* pUnitsIssue;

	WORD wUnitsIssue;
	int lc1; //local counter

	int iRet;

//wait and set Queue2-In-Use
	while(bQueue2InUse == TRUE)
	{ 
		Write_Log("Waiting for Queue2 to free: ");
		itoa(pCA->pTimer->iCurrTick, NText, 10);
		Write_Log(NText);
				WriteLog_Enter();
	}

bQueue2InUse = TRUE;

//begin executing commands
	PCMD2* pQ2Tr = pQ2First->pNext;
	PCMD2* pQ2Prev = pQ2Tr;
	PCMD2* pQ2Next = pQ2Tr;

int iNumCommands = 0;
	while (pQ2Tr != pQ2Last)
	{
		iNumCommands++;

		iRet = Execute_Command2(pQ2Tr);

//DEBUG
itoa(iRet, NText, 10);
Write_Log("Command Returned: ");
Write_Log(NText);
WriteLog_Enter();
itoa(iNumCommands, NText, 10);
char Buffer[50];
ZeroMemory(Buffer, sizeof(char)*50);
strcat(Buffer, "    -->");
strcat(Buffer, NText);
strcat(Buffer, ": ");
itoa(pQ2Tr->wType, NText, 10);
strcat(Buffer, NText);
Write_Log(Buffer);
WriteLog_Enter();
//pCA->pChatBuffer->Add_CBNode(Buffer, 15);


		//delete or skip depending on skip flag
		if (iRet == 0)
		{
			//find previous and next unit
			pQ2Next = pQ2Tr->pNext;
			pQ2Prev = pQ2Tr->pPrev;
			//unlink(or rather re-seal excluding pQ2Tr
			pQ2Next->pPrev = pQ2Prev;
			pQ2Prev->pNext = pQ2Next;

			delete [] pQ2Tr->pData;
				delete(pQ2Tr);

			pQ2Tr = pQ2Next;

			//DEBUG
			lNumCommands++;

		}
			else
			{
				pQ2Next = pQ2Tr->pNext;
				pQ2Tr = pQ2Next;
			}

	}

//release Queue2-In-Use
bQueue2InUse = FALSE;
/*
//DEBUG
itoa(pCA->pTimer->iCurrTick, NText, 10);
char Buffer[50];
ZeroMemory(Buffer, sizeof(char)*50);
strcat(Buffer, NText);
strcat(Buffer, ": ");
itoa(iNumCommands, NText, 10);
strcat(Buffer, NText);
pCA->pChatBuffer->Add_CBNode(Buffer, 15);
*/
//DEBUG
Write_Log("Execute_Commands_Q2 Ended.");
WriteLog_Enter();
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Move_B_Single(LONG lExecTick, WORD wUnitID, LONG lTargetIndex, int* iRetBufferSize)
{
	int iRBS = (sizeof(DWORD)*(6)) + sizeof(LONG) + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	LONG* plTrav = (LONG*)pBuffer;
	DWORD* pdwTrav;

	*plTrav = lExecTick;
	plTrav++;
	pdwTrav = (DWORD*)plTrav;
	
	*pdwTrav = OBJTYPE_UNIT;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = wUnitID;
	pdwTrav++;
	*pdwTrav = OBJTYPE_MAPTILE;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = lTargetIndex;

    return pBuffer;	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Move2(LONG lExecTick, WORD wUnitID, LONG lTargetIndex, int* iRetBufferSize)
{

	//need 6 DWORDS (3 for each parameter)
int iRBS = sizeof(LONG)*2 + sizeof(WORD) + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	LONG* plTrav = (LONG*)pBuffer;
	WORD* pwTrav;
	
	*plTrav = lExecTick;
	plTrav++;
	pwTrav = (WORD*)plTrav;
	*pwTrav = wUnitID;
	pwTrav++;
	plTrav = (LONG*)pwTrav;
	*plTrav = lTargetIndex;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//I know this is very confusing notation, but this is just saying that i am using the BATCH_MOVE command, but with a single unit 
//this is to allow for issuing commands to move a unit by just specifying a unit ID, instead of having to use USB entries(02.17.2004)
BYTE* cCommQueue::Assemble_CommData_Batch_Move2_Single_Unit(LONG lExecTick, DWORD dwTargetIndex, DWORD dwUnitID, int* iRetBufferSize)
{

	//need 6 DWORDS (3 for each parameter)
int iRBS = sizeof(DWORD) * 6 + sizeof(LONG) + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav;
	LONG* pllTrav;
	char* pchTrav;

	pllTrav = (LONG*)pBuffer;

	*pllTrav = lExecTick;
	pllTrav++;
	pdwTrav = (DWORD*)pllTrav;
	*pdwTrav = OBJTYPE_UNIT;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = dwUnitID;
	pdwTrav++;
	*pdwTrav = OBJTYPE_MAPTILE;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = dwTargetIndex;

	return pBuffer;




}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Batch_Move2(LONG lExecTick, LONG lTargetIndex, WORD* retUnitsIssue, VOID* USB_Buffer_List_Begin, VOID* USB_Buffer_List_End, int* iRetBufferSize)
{
	LPSBE pUSB_Begin = (LPSBE)USB_Buffer_List_Begin;
	LPSBE pUSB_End = (LPSBE)USB_Buffer_List_End;
	LPSBE pUSB_Trav = (LPSBE)pUSB_Begin->pNext;

	WORD wUnitsIssue = 0;

	//SOMEONE has to find out how many units were in the buffer, so that is done here
	while (pUSB_Trav != pUSB_End)
	{
		if (pUSB_Trav->iCommandFlag == CQ2TYPE_BATCH_MOVE)
			wUnitsIssue++;
		pUSB_Trav = (LPSBE)pUSB_Trav->pNext;
	}

	//return the number of units back to the user
	*retUnitsIssue = wUnitsIssue;

	//return a NULL buffer if no units were found
	if (wUnitsIssue == 0)
		return NULL;

	//knowing the number of units, the buffer can be allocated


	//need 6 DWORDS (3 for each parameter)
int iRBS = (sizeof(LONG)*2) + sizeof(WORD) + (sizeof(WORD)*wUnitsIssue) + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	LONG* plTrav = (LONG*)pBuffer;
	WORD* pwTrav;

	pUSB_Trav = (LPSBE)pUSB_Begin->pNext;

	*plTrav = lExecTick;
	plTrav++;
	*plTrav = lTargetIndex;
	plTrav++;
	pwTrav = (WORD*)plTrav;
	*pwTrav = wUnitsIssue;
	pwTrav++;

	while (pUSB_Trav != pUSB_End)
	{
		//add the ID
		if (pUSB_Trav->iCommandFlag == CQ2TYPE_BATCH_MOVE)
			*pwTrav = pUSB_Trav->iID;
		//increment both pointer/node
		pwTrav++;
		pUSB_Trav = (LPSBE)pUSB_Trav->pNext;
	}


	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Attack_B_single(DWORD lExecTick, WORD wUnitID, WORD wEnemyID, WORD wEnemyObjType, int* iRetBufferSize)
{
	int iRBS = sizeof(DWORD) * 7; //1 for the exec tick, 3 for the unit ID specify, 3 for the enemy ID specify

	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	DWORD* pBuffer = new DWORD[ iRBS ]; 

	//exec tick
	pBuffer[0] = lExecTick;
	//object type
	pBuffer[1] = OBJTYPE_UNIT;
	//object number
	pBuffer[2] = 1;
	//objectID
	pBuffer[3] = wUnitID;
	//object type
	pBuffer[4] = wEnemyObjType;
	//object number
	pBuffer[5] = 1;
	//object ID
	pBuffer[6] = wEnemyID;

	return (BYTE*)pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Batch_Attack2_Unit(LONG lExecTick, WORD wEnemyID, WORD* retUnitsIssue, VOID* USB_Buffer_List_Begin, VOID* USB_Buffer_List_End, int* iRetBufferSize)
{
	
	LPSBE pUSB_Begin = (LPSBE)USB_Buffer_List_Begin;
	LPSBE pUSB_End = (LPSBE)USB_Buffer_List_End;
	LPSBE pUSB_Trav = (LPSBE)pUSB_Begin->pNext;

	WORD wUnitsIssue = 0;


	//SOMEONE has to find out how many units were in the buffer, so that is done here
	while (pUSB_Trav != pUSB_End)
	{
		if (pUSB_Trav->iCommandFlag == CQ2TYPE_BATCH_ATTACK_UNIT)
			wUnitsIssue++;
		pUSB_Trav = (LPSBE)pUSB_Trav->pNext;
	}

	//return the number of units back to the user
	*retUnitsIssue = wUnitsIssue;

	//return a NULL buffer if no units were found
	if (wUnitsIssue == 0)
		return NULL;

	//knowing the number of units, the buffer can be allocated
int iRBS = (sizeof(LONG)) + sizeof(WORD) * 2 + (sizeof(WORD)*wUnitsIssue) + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	LONG* plTrav = (LONG*)pBuffer;
	WORD* pwTrav;

	pUSB_Trav = (LPSBE)pUSB_Begin->pNext;

	*plTrav = lExecTick;
	plTrav++;
	pwTrav = (WORD*)plTrav;
	*pwTrav = wEnemyID;
	pwTrav++;
	*pwTrav = wUnitsIssue;
	pwTrav++;

	while (pUSB_Trav != pUSB_End)
	{
		//add the ID
		if (pUSB_Trav->iCommandFlag == CQ2TYPE_BATCH_ATTACK_UNIT)
			*pwTrav = pUSB_Trav->iID;
		//increment both pointer/node
		pwTrav++;
		pUSB_Trav = (LPSBE)pUSB_Trav->pNext;
	}

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Batch_Attack2_Struct(LONG lExecTick, WORD wEnemyID, WORD* retUnitsIssue, VOID* USB_Buffer_List_Begin, VOID* USB_Buffer_List_End, int* iRetBufferSize)
{

	LPSBE pUSB_Begin = (LPSBE)USB_Buffer_List_Begin;
	LPSBE pUSB_End = (LPSBE)USB_Buffer_List_End;
	LPSBE pUSB_Trav = (LPSBE)pUSB_Begin->pNext;

	WORD wUnitsIssue = 0;


	//SOMEONE has to find out how many units were in the buffer, so that is done here
	while (pUSB_Trav != pUSB_End)
	{
		if (pUSB_Trav->iCommandFlag == CQ2TYPE_BATCH_ATTACK_STRUCT)
			wUnitsIssue++;
		pUSB_Trav = (LPSBE)pUSB_Trav->pNext;
	}

	//return the number of units back to the user
	*retUnitsIssue = wUnitsIssue;

	//return a NULL buffer if no units were found
	if (wUnitsIssue == 0)
		return NULL;

	//knowing the number of units, the buffer can be allocated
int iRBS = (sizeof(LONG)) + sizeof(WORD) * 2 + (sizeof(WORD)*wUnitsIssue) + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 
	
	LONG* plTrav = (LONG*)pBuffer;
	WORD* pwTrav;

	pUSB_Trav = (LPSBE)pUSB_Begin->pNext;

	*plTrav = lExecTick;
	plTrav++;
	pwTrav = (WORD*)plTrav;
	*pwTrav = wEnemyID;
	pwTrav++;
	*pwTrav = wUnitsIssue;
	pwTrav++;

	while (pUSB_Trav != pUSB_End)
	{
		//add the ID
		if (pUSB_Trav->iCommandFlag == CQ2TYPE_BATCH_ATTACK_STRUCT)
			*pwTrav = pUSB_Trav->iID;
		//increment both pointer/node
		pwTrav++;
		pUSB_Trav = (LPSBE)pUSB_Trav->pNext;
	}

	return pBuffer;




}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Batch_Guard(LONG lExecTick, LONG lGuardIndex, WORD* retUnitsIssue, VOID* USB_Buffer_List_Begin, VOID* USB_Buffer_List_End, int* iRetBufferSize)
{

	LPSBE pUSB_Begin = (LPSBE)USB_Buffer_List_Begin;
	LPSBE pUSB_End = (LPSBE)USB_Buffer_List_End;
	LPSBE pUSB_Trav = (LPSBE)pUSB_Begin->pNext;

	WORD wUnitsIssue = 0;


	//SOMEONE has to find out how many units were in the buffer, so that is done here
	while (pUSB_Trav != pUSB_End)
	{
		if (pUSB_Trav->iCommandFlag == CQ2TYPE_BATCH_GUARD)
			wUnitsIssue++;
		pUSB_Trav = (LPSBE)pUSB_Trav->pNext;
	}

	//return the number of units back to the user
	*retUnitsIssue = wUnitsIssue;

	//return a NULL buffer if no units were found
	if (wUnitsIssue == 0)
		return NULL;

	//knowing the number of units, the buffer can be allocated
	int iRBS = (sizeof(LONG)*2) + sizeof(WORD) + (sizeof(WORD)*wUnitsIssue) + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	LONG* plTrav = (LONG*)pBuffer;
	WORD* pwTrav;

	pUSB_Trav = (LPSBE)pUSB_Begin->pNext;

	*plTrav = lExecTick;
	plTrav++;

	*plTrav = lGuardIndex;
	plTrav++;
	pwTrav = (WORD*)plTrav;
	*pwTrav = wUnitsIssue;
	pwTrav++;

	while (pUSB_Trav != pUSB_End)
	{
		//add the ID
		if (pUSB_Trav->iCommandFlag == CQ2TYPE_BATCH_GUARD)
			*pwTrav = pUSB_Trav->iID;
		//increment both pointer/node
		pwTrav++;
		pUSB_Trav = (LPSBE)pUSB_Trav->pNext;
	}

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Unload_All(LONG lExecTick, int iTargetUnit, int iTargetLocation, int* iRetBufferSize)
{
	int iRBS = (sizeof(DWORD)*(6)) + sizeof(LONG) + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	LONG* plTrav = (LONG*)pBuffer;
	DWORD* pdwTrav;

	*plTrav = lExecTick;
	plTrav++;
	pdwTrav = (DWORD*)plTrav;
	
	*pdwTrav = OBJTYPE_UNIT;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = iTargetUnit;
	pdwTrav++;
	*pdwTrav = IPARAMTYPE_DWORDS;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = iTargetLocation;

    return pBuffer;	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Cancel_Progorder_Top(LONG lExecTick, int iStructID, int* iRetBufferSize)
{
	int iRBS = (sizeof(DWORD)*(3)) + sizeof(LONG) + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	LONG* plTrav = (LONG*)pBuffer;
	DWORD* pdwTrav;

	*plTrav = lExecTick;
	plTrav++;
	pdwTrav = (DWORD*)plTrav;
	
	*pdwTrav = OBJTYPE_STRUCTURE;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = iStructID;

    return pBuffer;	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Struct_Turn_Off_On(LONG lExecTick, int iStructID, int* iRetBufferSize)
{
	int iRBS = (sizeof(DWORD)*(3)) + sizeof(LONG) + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	LONG* plTrav = (LONG*)pBuffer;
	DWORD* pdwTrav;

	*plTrav = lExecTick;
	plTrav++;
	pdwTrav = (DWORD*)plTrav;
	
	*pdwTrav = OBJTYPE_STRUCTURE;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = iStructID;

    return pBuffer;	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Load_Unload_Material(LONG lExecTick, int iUnitID, int iMapIndex, int* iRetBufferSize)
{
	int iRBS = (sizeof(DWORD)*(6)) + sizeof(LONG) + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	LONG* plTrav = (LONG*)pBuffer;
	DWORD* pdwTrav;

	*plTrav = lExecTick;
	plTrav++;
	pdwTrav = (DWORD*)plTrav;
	
	*pdwTrav = OBJTYPE_UNIT;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = iUnitID;
	pdwTrav++;
	*pdwTrav = OBJTYPE_MAPTILE;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = iMapIndex;

    return pBuffer;	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//generates the full unload list as the full storage of the target unit
BYTE* cCommQueue::Assemble_CommData_Unload_A(LONG lExecTick, int iTargetUnit, int iTargetLocation, int* iRetBufferSize)
{
int iNumToUnload = 0;
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cUnitA* pUnit = pCA->pUnitTable->pUnitTable[iTargetUnit];
	if (pUnit != NULL)
		iNumToUnload = pUnit->Storage.iNumStored;

	int iRBS = (sizeof(DWORD)*(8 + iNumToUnload)) + sizeof(LONG) + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	LONG* plTrav = (LONG*)pBuffer;
	DWORD* pdwTrav;

	*plTrav = lExecTick;
	plTrav++;
	pdwTrav = (DWORD*)plTrav;
	
	*pdwTrav = OBJTYPE_UNIT;
	pdwTrav++;
	*pdwTrav = iNumToUnload;
	pdwTrav++;

	for (int lc1 = 0; lc1 < iNumToUnload; lc1++)
	{
		*pdwTrav = pUnit->Storage.Get_UnitID_byOrder(lc1);
		pdwTrav++;
	}

	*pdwTrav = OBJTYPE_UNIT;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = iTargetUnit;
	pdwTrav++;
	*pdwTrav = IPARAMTYPE_DWORDS;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = iTargetLocation;

    return pBuffer;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Unload_B(LONG lExecTick, int iCargoUnitID, int iTargetUnitID, int iTargetLocation, int* iRetBufferSize) //10.15.2006	
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iRBS = (sizeof(DWORD)*9) + sizeof(LONG) + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	LONG* plTrav = (LONG*)pBuffer;
	DWORD* pdwTrav;

	*plTrav = lExecTick;
	plTrav++;
	pdwTrav = (DWORD*)plTrav;
	
	*pdwTrav = OBJTYPE_UNIT;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = iCargoUnitID;
	pdwTrav++;

	*pdwTrav = OBJTYPE_UNIT;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = iTargetUnitID;
	pdwTrav++;
	*pdwTrav = IPARAMTYPE_DWORDS;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = iTargetLocation;

    return pBuffer;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Attack2_Unit(LONG lExecTick, WORD wUnitID, WORD wEnemyID, int* iRetBufferSize)
{
	int iRBS = (sizeof(WORD)*2) + sizeof(LONG) + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	LONG* plTrav = (LONG*)pBuffer;
	WORD* pwTrav;

	*plTrav = lExecTick;
	plTrav++;
	pwTrav = (WORD*)plTrav;
	*pwTrav = wUnitID;
	pwTrav++;
	*pwTrav = wEnemyID;

	Write_Log("CommData for Attack Assembled.");
	WriteLog_Enter();

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_ReportFrame2(BYTE bControlID, LONG lTickCount, int* iRetBufferSize)
{

int iRBS = sizeof(LONG) + 1 + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	BYTE* pbTrav = pBuffer;

	*pbTrav = bControlID;
	pbTrav++;
	LONG* plTrav = (LONG*)pbTrav;
	*plTrav = lTickCount;


	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Build_Progress_Component(VOID* pProgOrder, DWORD dwStructID, DWORD dwCmpType, int* iRetBufferSize)
{
int iRBS = sizeof(void*) + sizeof(DWORD) * 2 + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav;
	void**	ppTrav = (void**)pBuffer;

	*ppTrav = pProgOrder;
	ppTrav++;
	pdwTrav = (DWORD*)ppTrav;

	*pdwTrav = dwStructID;

	pdwTrav++;

	*pdwTrav = dwCmpType;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Earmark_Components(VOID* pProgOrder, DWORD dwStructID, DWORD* pdwCmpIDs, int iNumIDs, int* iRetBufferSize)
{
int iRBS = sizeof(void*) + sizeof(DWORD) * 3 + sizeof(DWORD) * iNumIDs + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav;
	void** ppTrav = (void**)pBuffer;

	*ppTrav = pProgOrder;
	ppTrav++;
	pdwTrav = (DWORD*)ppTrav;

	*pdwTrav = dwStructID;

	int lc1 = 0; //local counter
	pdwTrav++;
	*pdwTrav = OBJTYPE_COMPONENT;
	pdwTrav++;
	*pdwTrav = iNumIDs;

	DWORD* pdwTrav2 = pdwCmpIDs;

	for (lc1 = 0; lc1 < iNumIDs; lc1++)
	{
		pdwTrav++;
		*pdwTrav = pdwTrav2[lc1];
	
	}

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Create_Unit_A(VOID* pProgOrder, DWORD dwStructID, DWORD utypehint, int* piCmpIDs, int iNumIDs, int* piCmpTypes, int iTypes, int* iRetBufferSize)
{
//modified 08.20.2003
int iValidIDs = 0;
int iValidTypes = 0;
int lc1; //local counter;

	//count the number of valid iIDs and iTypes
	for (lc1 = 0; lc1 < iNumIDs; lc1++)
	{
		if (piCmpIDs[lc1] >= 0)
			iValidIDs++;
	}
	for (lc1 = 0; lc1 < iTypes; lc1++)
	{
		if (piCmpTypes[lc1] >= 0)
			iValidTypes++;
	}



	//this command is identical in composition to Earmark_Components(not anymore 08.20.2003)
	//reason for *5 is 2 DWORD for struct ID and unit type hint, then 2 DWORDs each for parameter type + number of paramaters for each of the IDs and types
int iRBS = sizeof(void*) + sizeof(DWORD) * 6 + sizeof(DWORD) * iValidIDs + sizeof(DWORD) * iValidTypes + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 
	
	DWORD* pdwTrav;
	void** ppTrav = (void**)pBuffer;

	*ppTrav = pProgOrder;
	ppTrav++;
	pdwTrav = (DWORD*)ppTrav;

	*pdwTrav = dwStructID;

	pdwTrav++;
	*pdwTrav = utypehint;

	lc1 = 0; //local counter
	pdwTrav++;
	*pdwTrav = OBJTYPE_COMPONENT;
	pdwTrav++;
	*pdwTrav = iValidIDs;

	for (lc1 = 0; lc1 < iNumIDs; lc1++)
	{
		if (piCmpIDs[lc1] >= 0)
		{
		pdwTrav++;
		*pdwTrav = piCmpIDs[lc1];
		
		}
	}

	//now add the ctype set
	pdwTrav++;
	*pdwTrav = OBJTYPE_CTYPE;
	pdwTrav++;
	*pdwTrav = iValidTypes;

	for (lc1 = 0; lc1 < iTypes; lc1++)
	{
		if (piCmpTypes[lc1] >= 0)
		{
		pdwTrav++;
		*pdwTrav = piCmpTypes[lc1];

		}
	}


	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Build_Progress_Unit(VOID* pProgOrder, DWORD dwStructID, double ddResourceCost, double ddConstructionDifficulty, int iUnitType, int* iRetBufferSize)
{
int iRBS = sizeof(cProgressiveOrder*) + (sizeof(DWORD) * 2) + sizeof(double) * 2 + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav;
	cProgressiveOrder** ppTrav = (cProgressiveOrder**)pBuffer;
	double* pddTrav;

	*ppTrav = (cProgressiveOrder*)pProgOrder;
	ppTrav++;
	pdwTrav = (DWORD*)ppTrav;

	*pdwTrav = dwStructID;	

	pdwTrav++;
	pddTrav = (double*)pdwTrav;

	*pddTrav = ddResourceCost;

	pddTrav++;

	*pddTrav = ddConstructionDifficulty;

	pddTrav++;
	//careful! doubles are 2 DWORDs long
	pdwTrav = (DWORD*)pddTrav;
	
	*pdwTrav = iUnitType;

if (pBuffer == NULL)
{
	Write_Log("Assemble Commdata: Warning!!! NULL pReturn!");
	WriteLog_Enter();

}
	
	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Create_Component_A(VOID* pProgOrder, DWORD dwStructID, DWORD dwCmpType, int* iRetBufferSize)
{
	//the composition of this commmand is identical to that of build_progress_component
int iRBS = sizeof(void*) + sizeof(DWORD) * 2 + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav;
	void**	ppTrav = (void**)pBuffer;

	*ppTrav = pProgOrder;
	ppTrav++;
	pdwTrav = (DWORD*)ppTrav;

	*pdwTrav = dwStructID;

	pdwTrav++;

	*pdwTrav = dwCmpType;

	return pBuffer;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Init_Nation(int iCtrl, char* pName, int iNameLength, BOOL bAIFlag, int* iRetBufferSize)
{
int iRBS = sizeof(DWORD) * 3 + sizeof(char) * iNameLength + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav;
	char* pchTrav;

	pdwTrav = (DWORD*)pBuffer;

	*pdwTrav = iCtrl;
	pdwTrav++;
	if (bAIFlag == FALSE)
		*pdwTrav = 0;
			else
		*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = iNameLength;
	pdwTrav++;
	pchTrav = (char*)pdwTrav;

	memcpy(pchTrav, pName, sizeof(char) * iNameLength);

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Start_Build_Unit_Preconfig(LONG lExecTick, DWORD dwStructID, DWORD dwUnitType, int* iRetBufferSize)
{
	//need 6 DWORDS (3 for each parameter)
int iRBS = sizeof(DWORD) * 6 + sizeof(LONG) + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav;
	LONG* pllTrav;
	char* pchTrav;

	pllTrav = (LONG*)pBuffer;

	*pllTrav = lExecTick;
	pllTrav++;
	pdwTrav = (DWORD*)pllTrav;
	*pdwTrav = OBJTYPE_STRUCTURE;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = dwStructID;
	pdwTrav++;
	*pdwTrav = OBJTYPE_UTYPE;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = dwUnitType;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_ListBox_Conditional_Blit(cIfcElement* pParent, int iRenderTargetCode, int iImageSurface, int idestleft, int idesttop, int isrcleft, int isrcright, int isrctop, int isrcbottom, int* iRetBufferSize)
{
	//need 8 DWORDS + 1 pointer
int iRBS = sizeof(DWORD) * 8 + sizeof(cIfcElement*);
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	cIfcElement** ppIfcETrav = (cIfcElement**)pBuffer;

	*ppIfcETrav = pParent;

	ppIfcETrav++;

	DWORD* pdwTrav = (DWORD*)ppIfcETrav;
	
	*pdwTrav = iRenderTargetCode;
	pdwTrav++;
	*pdwTrav = iImageSurface;
	pdwTrav++;
	*pdwTrav = idestleft;
	pdwTrav++;
	*pdwTrav = idesttop;
	pdwTrav++;
	*pdwTrav = isrcleft;
	pdwTrav++;
	*pdwTrav = isrcright;
	pdwTrav++;
	*pdwTrav = isrctop;
	pdwTrav++;
	*pdwTrav = isrcbottom;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Set_ActiveIGrid(DWORD dwIBox, DWORD dwActive, int* iRetBufferSize)
{
	//need 2 DWORD
int iRBS = sizeof(DWORD) * 2;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;

	*pdwTrav = dwIBox;
	pdwTrav++;
	*pdwTrav = dwActive;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_ListBox_Disp_Slider(cIfcElement* pParent, int iRenderTargetCode,  int iDispCode, int* iRetBufferSize)
{
	//need 1 cIfcElement* and 1 DWORD
int iRBS = (sizeof(DWORD)*2) + sizeof(cIfcElement*);
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	cIfcElement** ppIfcETrav = (cIfcElement**)pBuffer;

	*ppIfcETrav = pParent;
	ppIfcETrav++;

	DWORD* pdwTrav = (DWORD*) ppIfcETrav;

	*pdwTrav = iRenderTargetCode;

	pdwTrav++;
	*pdwTrav = iDispCode;


	return pBuffer;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Blit(int iRenderTargetCode, int iImageSurface, int idestleft, int idesttop, int isrcleft, int isrcright, int isrctop, int isrcbottom, int* iRetBufferSize)
{
	//need 8 DWORDS
int iRBS = sizeof(DWORD) * 8;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;
	
	*pdwTrav = iRenderTargetCode;
	pdwTrav++;
	*pdwTrav = iImageSurface;
	pdwTrav++;
	*pdwTrav = idestleft;
	pdwTrav++;
	*pdwTrav = idesttop;
	pdwTrav++;
	*pdwTrav = isrcleft;
	pdwTrav++;
	*pdwTrav = isrcright;
	pdwTrav++;
	*pdwTrav = isrctop;
	pdwTrav++;
	*pdwTrav = isrcbottom;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_BlitBySurface(int iRenderTargetCode, LPDIRECTDRAWSURFACE7 pDDS, int idestleft, int idesttop, int isrcleft, int isrcright, int isrctop, int isrcbottom, int* iRetBufferSize)
{
	//need 7 DWORDS and 1 surface pointer
int iRBS = (sizeof(DWORD) * 7) + sizeof(LPDIRECTDRAWSURFACE7);
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;
	
	*pdwTrav = iRenderTargetCode;
	pdwTrav++;
	LPDIRECTDRAWSURFACE7* ppDDS = (LPDIRECTDRAWSURFACE7*)pdwTrav;
	*ppDDS = pDDS;
	ppDDS++;
	pdwTrav = (DWORD*)ppDDS;
	*pdwTrav = idestleft;
	pdwTrav++;
	*pdwTrav = idesttop;
	pdwTrav++;
	*pdwTrav = isrcleft;
	pdwTrav++;
	*pdwTrav = isrcright;
	pdwTrav++;
	*pdwTrav = isrctop;
	pdwTrav++;
	*pdwTrav = isrcbottom;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_BlitFit(int iRenderTargetCode, int iImageSurface, int idestleft, int idestright, int idesttop, int idestbottom, int isrcleft, int isrcright, int isrctop, int isrcbottom, int* iRetBufferSize)
{
	//need 10 DWORDS
int iRBS = sizeof(DWORD) * 10;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;
	
	*pdwTrav = iRenderTargetCode;
	pdwTrav++;
	*pdwTrav = iImageSurface;
	pdwTrav++;
	*pdwTrav = idestleft;
	pdwTrav++;
	*pdwTrav = idestright;
	pdwTrav++;
	*pdwTrav = idesttop;
	pdwTrav++;
	*pdwTrav = idestbottom;
	pdwTrav++;
	*pdwTrav = isrcleft;
	pdwTrav++;
	*pdwTrav = isrcright;
	pdwTrav++;
	*pdwTrav = isrctop;
	pdwTrav++;
	*pdwTrav = isrcbottom;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_SetMainMenuMode(int iMode, int* iRetBufferSize)
{
	//need 1 DWORD
int iRBS = sizeof(DWORD);
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;
	
	*pdwTrav = iMode;

	return pBuffer;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Colorfill(DWORD dwRenderTargetCode, DWORD dwTargetColor, DWORD dwLeft, DWORD dwRight, DWORD dwTop, DWORD dwBottom, int* iRetBufferSize)
{
	//need 6 DWORDs
int iRBS = sizeof(DWORD) * 6;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;
	
	*pdwTrav = dwRenderTargetCode; pdwTrav++;
	*pdwTrav = dwTargetColor; pdwTrav++;
	*pdwTrav = dwLeft; pdwTrav++;
	*pdwTrav = dwRight; pdwTrav++;
	*pdwTrav = dwTop; pdwTrav++;
	*pdwTrav = dwBottom; 

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_DrawText(DWORD dwRenderTargetCode, 
											DWORD dwXPos, 
											DWORD dwYPos, 
											DWORD dwFontType,
											DWORD dwFontColor,
											DWORD dwNumChars,
											LPSTR Text,
											int* iRetBufferSize)
{
	return Assemble_CommData_DrawText(dwRenderTargetCode, dwXPos, dwYPos, 0, dwFontType, dwFontColor, dwNumChars, Text, iRetBufferSize);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_DrawText(DWORD dwRenderTargetCode, 
											DWORD dwXPos, 
											DWORD dwYPos, 
											DWORD dwXLimit,
											DWORD dwFontType,
											DWORD dwFontColor,
											DWORD dwNumChars,
											LPSTR Text,
											int* iRetBufferSize)
{

	//need 6 DWORDs + 0.5*num of characters
int iRBS = sizeof(DWORD) *( 6 + (dwNumChars/2) + 2); //extra 2 for possible zero termination and any extra things I haven't considered
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;
	
	*pdwTrav = dwRenderTargetCode; pdwTrav++;
	*pdwTrav = dwXPos; pdwTrav++;
	*pdwTrav = dwYPos; pdwTrav++;
	*pdwTrav = dwXLimit; pdwTrav++;
	*pdwTrav = dwFontType; pdwTrav++;
	*pdwTrav = dwFontColor; pdwTrav++;
	*pdwTrav = dwNumChars; pdwTrav++;

	unsigned short* pusTrav = (unsigned short*)pdwTrav;

	for (int lc1 = 0; lc1 < dwNumChars; lc1++)
	{
			*pusTrav = Text[lc1];
			pusTrav++;
	}
	//add zero termination
	*pusTrav = 0;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cCommQueue::Pack_CommData_DrawText2(PCMD2* pCmd,
											BOOL bUnicodeFlag, 
											DWORD dwRenderTargetCode, 
											DWORD dwXPos, 
											DWORD dwYPos, 
											DWORD dwXLimit,
											DWORD dwFontType,
											DWORD dwFontColor,
											char* Text)
{
	CommDataDrawText2* newBuffer = new CommDataDrawText2;
	CommDataDrawText2* pq = newBuffer;

	if (bUnicodeFlag == FALSE)
	{
		pq->dwUnicodeFlag = 0;	
	}
	else
	{
		pq->dwUnicodeFlag = 1;	
	}

	pq->dwRenderTargetCode = dwRenderTargetCode;
	pq->dwXPos = dwXPos;
	pq->dwYPos = dwYPos;
	pq->dwXLimit = dwXLimit;
	pq->dwFontType = dwFontType;
	pq->dwFontColor = dwFontColor;
	pq->Text = Text;

	pCmd->pData = reinterpret_cast<BYTE*>(newBuffer);
	pCmd->wDataSize = sizeof(CommDataDrawText2);
	pCmd->wType = CQ2TYPE_DRAWTEXT2;

	return 0;
}

//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_DrawText2(BOOL bUnicodeFlag, 
											DWORD dwRenderTargetCode, 
											DWORD dwXPos, 
											DWORD dwYPos, 
											DWORD dwFontType,
											DWORD dwFontColor,
											char* Text,
											int* iRetBufferSize)
{

	return Assemble_CommData_DrawText2(bUnicodeFlag, dwRenderTargetCode, dwXPos, dwYPos, 0, dwFontType, dwFontColor, Text, iRetBufferSize);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cCommQueue::Assemble_CommData_DrawText2ex(CommDataDrawText2* buffer,
											BOOL bUnicodeFlag, 
											DWORD dwRenderTargetCode, 
											DWORD dwXPos, 
											DWORD dwYPos, 
											DWORD dwXLimit,
											DWORD dwFontType,
											DWORD dwFontColor,
											char* Text)
{
	CommDataDrawText2* pq = buffer;

	if (bUnicodeFlag == FALSE)
	{
		pq->dwUnicodeFlag = 0;	
	}
	else
	{
		pq->dwUnicodeFlag = 1;	
	}

	pq->dwRenderTargetCode = dwRenderTargetCode;
	pq->dwXPos = dwXPos;
	pq->dwYPos = dwYPos;
	pq->dwXLimit = dwXLimit;
	pq->dwFontType = dwFontType;
	pq->dwFontColor = dwFontColor;
	pq->Text = Text;

	return 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_DrawText2(BOOL bUnicodeFlag, 
											DWORD dwRenderTargetCode, 
											DWORD dwXPos, 
											DWORD dwYPos, 
											DWORD dwXLimit,
											DWORD dwFontType,
											DWORD dwFontColor,
											char* Text,
											int* iRetBufferSize)
{

	//need 6 DWORDs + pointer
int iRBS = sizeof(DWORD) * 6 + sizeof(LPSTR); 
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;

	if (bUnicodeFlag == FALSE)
	{
		*pdwTrav = 0; pdwTrav++;}
	else
	{
        *pdwTrav = 1; pdwTrav++;
	}

	*pdwTrav = dwRenderTargetCode; pdwTrav++;
	*pdwTrav = dwXPos; pdwTrav++;
	*pdwTrav = dwYPos; pdwTrav++;
	*pdwTrav = dwXLimit; pdwTrav++;
	*pdwTrav = dwFontType; pdwTrav++;
	*pdwTrav = dwFontColor; pdwTrav++;
	char** ppText = (char**)pdwTrav;
	*ppText = Text;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Init_TimerID(DWORD dwTimerID, DWORD dwDPNID, DWORD dwNameLength, char* pName, int *iRetBufferSize)
{

	//need 6 DWORDs + pointer
int iRBS = sizeof(DWORD) * 3 + sizeof(char) * (dwNameLength + 1); 
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;
	*pdwTrav = dwTimerID;
	pdwTrav++;
	*pdwTrav = dwDPNID;
	pdwTrav++;
	*pdwTrav = dwNameLength;
	pdwTrav++;
	char* pcTrav = (char*)pdwTrav;

	for (int lc1 = 0; lc1 < dwNameLength; lc1++)
	{
		*pcTrav = pName[lc1];
		pcTrav++;
	}
	//then terminate on the last character
	*pcTrav = 0;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_DrawText3(BOOL bUnicodeFlag,
											DWORD dwRenderTargetCode, 
											DWORD dwXPos, 
											DWORD dwYPos, 
											DWORD dwFontType,
											DWORD dwFontColor,
											char** ppText,
											int* iRetBufferSize)
{
	return Assemble_CommData_DrawText3(bUnicodeFlag, dwRenderTargetCode, dwXPos, dwYPos, 0, dwFontType, dwFontColor, ppText, iRetBufferSize);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_DrawText3(BOOL bUnicodeFlag,
											DWORD dwRenderTargetCode, 
											DWORD dwXPos, 
											DWORD dwYPos, 
											DWORD dwXLimit,
											DWORD dwFontType,
											DWORD dwFontColor,
											char** ppText,
											int* iRetBufferSize)
{

	//need 6 DWORDs + pointer
int iRBS = sizeof(DWORD) * 6 + sizeof(LPSTR); 
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;


	if (bUnicodeFlag == FALSE)
	{
        *pdwTrav = 0; pdwTrav++;
	}
	else
	{
        *pdwTrav = 1; pdwTrav++;
	}

	*pdwTrav = dwRenderTargetCode; pdwTrav++;
	*pdwTrav = dwXPos; pdwTrav++;
	*pdwTrav = dwYPos; pdwTrav++;
	*pdwTrav = dwXLimit; pdwTrav++;
	*pdwTrav = dwFontType; pdwTrav++;
	*pdwTrav = dwFontColor; pdwTrav++;
	//yes, three levels of indirection
	char*** pppText = (char***)pdwTrav;
	*pppText = ppText;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_DrawIfcPanel(DWORD dwRenderTargetCode, DWORD dwLeft, DWORD dwRight, DWORD dwTop, DWORD dwBottom, int* iRetBufferSize)
{

	//need 5 DWORDs 
int iRBS = sizeof(DWORD) * 5; 
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;
	
	*pdwTrav = dwRenderTargetCode; pdwTrav++;
	*pdwTrav = dwLeft; pdwTrav++;
	*pdwTrav = dwRight; pdwTrav++;
	*pdwTrav = dwTop; pdwTrav++;
	*pdwTrav = dwBottom; 

	return pBuffer;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_DrawIfcListBox(DWORD dwRenderTargetCode, DWORD dwLeft, DWORD dwRight, DWORD dwTop, DWORD dwBottom, int* iRetBufferSize)
{

	//need 5 DWORDs 
int iRBS = sizeof(DWORD) * 5; 
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;
	
	*pdwTrav = dwRenderTargetCode; pdwTrav++;
	*pdwTrav = dwLeft; pdwTrav++;
	*pdwTrav = dwRight; pdwTrav++;
	*pdwTrav = dwTop; pdwTrav++;
	*pdwTrav = dwBottom; 

	return pBuffer;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_DrawIfcSecondaryPanel(DWORD dwRenderTargetCode, DWORD dwLeft, DWORD dwRight, DWORD dwTop, DWORD dwBottom, int iColorIndex, int* iRetBufferSize)
{

	//need 5 DWORDs 
int iRBS = sizeof(DWORD) * 6; 
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;
	
	*pdwTrav = dwRenderTargetCode; pdwTrav++;
	*pdwTrav = dwLeft; pdwTrav++;
	*pdwTrav = dwRight; pdwTrav++;
	*pdwTrav = dwTop; pdwTrav++;
	*pdwTrav = dwBottom; pdwTrav++;
	*pdwTrav = iColorIndex;

	return pBuffer;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Set_ActiveDevice(DWORD dwProviderID, DWORD dwDeviceID, int* iRetBufferSize)
{

	//need 2 DWORDs 
int iRBS = sizeof(DWORD) * 2; 
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;
	
	*pdwTrav = dwProviderID; pdwTrav++;
	*pdwTrav = dwDeviceID; 

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Build_Structure_ONE(LONG lExecTick, int iID_Unit, int iStructType, int iMapIndex, int* iRetBufferSize)
{
	int iRBS = sizeof(void*) + sizeof(LONG) + sizeof(DWORD) * 6 + CQ2_ENDPADBYTES;
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav;
	LONG* plTrav = (LONG*)pBuffer;

	*plTrav = lExecTick;
	plTrav++;
	pdwTrav = (DWORD*)plTrav;

	*pdwTrav = OBJTYPE_UNIT;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = iID_Unit;
	pdwTrav++;

	*pdwTrav = OBJTYPE_STYPE;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = iStructType;
	pdwTrav++;

	*pdwTrav = OBJTYPE_MAPTILE;
	pdwTrav++;
	*pdwTrav = 1;
	pdwTrav++;
	*pdwTrav = iMapIndex;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Set_Inputstring(char* pText, DWORD BufferLength, int* iRetBufferSize)
{
	//need 1 DWORDs + 1 char* 
int iRBS = sizeof(DWORD) + sizeof(char*);

	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;
	
	*pdwTrav = BufferLength; pdwTrav++;

	char** ppTrav = (char**)pdwTrav;
	
	*ppTrav = pText; 

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Set_Inputstring_Wide(WCHAR* pText, DWORD BufferLength, int* iRetBufferSize)
{
	//need 1 DWORDs + 1 WCHAR* 
int iRBS = sizeof(DWORD) + sizeof(WCHAR*);

	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;
	
	*pdwTrav = BufferLength; pdwTrav++;

	WCHAR** ppTrav = (WCHAR**)pdwTrav;
	
	*ppTrav = pText; 

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Set_Pdword(DWORD Val, DWORD* pdwPoint, int* iRetBufferSize)
{

	//need 1 DWORDs + 1 DWORD* 
int iRBS = sizeof(DWORD) + sizeof(DWORD*);

	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;
	
	*pdwTrav = Val; pdwTrav++;

	DWORD** ppTrav = (DWORD**)pdwTrav;
	
	*ppTrav = pdwPoint; 

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Copy_String(void* pString_Source, void* pString_Dest, int* iRetBufferSize)
{
	//need 2 void*
int iRBS = sizeof(void*) * 2;

	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	void** ppvTrav = (void**)pBuffer;
	
	*ppvTrav = pString_Source; ppvTrav++;
	*ppvTrav = pString_Dest; 
	
	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Send_Text(WCHAR* pString_Name, int iNameLength, LPSTR pString_Text, int iTextLength, int iStayTime, int* iRetBufferSize)
{
	//need 2 DWORDS and BYTES
int iRBS = (sizeof(DWORD) * 2) + sizeof(BYTE)*(iTextLength + 1) + sizeof(unsigned short)*(iNameLength+1);

	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;
	
	*pdwTrav = iNameLength + 1;
	pdwTrav++;

	WCHAR* pwcTrav = (WCHAR*)pdwTrav;
	WCHAR* pwcSource = pString_Name;

	for (int lc1 = 0; lc1 < iNameLength; lc1++)
	{
		pwcTrav[lc1] = pwcSource[lc1];
	}
	pwcTrav[iNameLength] = 0; //null terminate
	pwcTrav = pwcTrav + iNameLength + 1;
	pdwTrav = (DWORD*)pwcTrav;

	*pdwTrav = iTextLength + 1;
	pdwTrav++;
	*pdwTrav = iStayTime;
	pdwTrav++;

	char* pcTrav = (char*)pdwTrav;
	char* pcSource = (char*)pString_Text;

	for (int lc2 = 0; lc2 < iTextLength; lc2++)
	{	
		pcTrav[lc2] = pcSource[lc2];
	}
	pcTrav[iTextLength] = 0;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Send_Text2(WCHAR* pName, char* pText, int iTextBufferSize, int iStayTime, int* iRetBufferSize)
{
	//need 1 DWORD and 1 WCHAR*, 1 char*
int iRBS = sizeof(DWORD) + sizeof(WCHAR*) + sizeof(char*);

	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	WCHAR** ppwcTrav = (WCHAR**)pBuffer;
	*ppwcTrav = pName;
	ppwcTrav++;
	char** ppcTrav = (char**)ppwcTrav;
	*ppcTrav = pText;
	ppcTrav++;
	DWORD* pdwTrav = (DWORD*)ppcTrav;
	*pdwTrav = iTextBufferSize;
	*pdwTrav = iStayTime;
	pdwTrav++;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Notify_iControl(DWORD dwDPNID, int iControl, int* iRetBufferSize)
{
	//need 3 DWORDs
int iRBS = sizeof(DWORD) + sizeof(int);

	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;

	*pdwTrav = dwDPNID; pdwTrav++;
	int* piTrav = (int*)pdwTrav;
	*piTrav = iControl;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

BYTE* cCommQueue::Assemble_CommData_DispChatText(DWORD dwRenderTargetCode, int iX, int iY, int iLines, int* iRetBufferSize)
{
	//need 3 DWORDs
int iRBS = sizeof(DWORD) * 3;

	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;

	*pdwTrav = dwRenderTargetCode; pdwTrav++;
	*pdwTrav = iX; pdwTrav++;
	*pdwTrav = iY; pdwTrav++;
	*pdwTrav = iLines;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_ListBox_Execute_Ifc(cIfcElement* pParent, cIfcElement* pListing, DWORD dwOffset, DWORD dwElemCat, int* iRetBufferSize)
{
	//need 2 DWORDs, 1 cIfcElem*
int iRBS = (sizeof(DWORD) * 2) + (sizeof(cIfcElement*) * 2);

	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	cIfcElement** ppIfcETrav = (cIfcElement**)pBuffer;

	*ppIfcETrav = (cIfcElement*)pParent;

	ppIfcETrav++;

	*ppIfcETrav = (cIfcElement*)pListing;

	ppIfcETrav++;

	DWORD* pdwTrav = (DWORD*)ppIfcETrav;

	*pdwTrav = dwOffset; pdwTrav++;
	*pdwTrav = dwElemCat;

	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Send_InputPacket_ObjRef(DWORD dwPacketType, DWORD dwObjectsType, DWORD dwNumObjects, DWORD* pdwIDs, VOID** pObjectArray, int* iRetBufferSize)
{
	//need 3 DWORDs + DWORD * NumObjects + (VOID*)*NumObjects 
	int iRBS = (sizeof(DWORD) * (3 + dwNumObjects)) + (sizeof(VOID*) * dwNumObjects) + 1; //the extra 1 is for the extra increment at the end(to prevent a GPF)
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;

	*pdwTrav = dwPacketType;
	pdwTrav++;
	*pdwTrav = dwObjectsType;
	pdwTrav++;
	*pdwTrav = dwNumObjects;
	for (int lc1 = 0; lc1 < dwNumObjects; lc1++)
	{
		pdwTrav++;
		*pdwTrav = pdwIDs[lc1];
	}
	pdwTrav++;
	VOID** ppvTrav = (VOID**)pdwTrav;
	if (pObjectArray != NULL)
	for (int lc2 = 0; lc2 < dwNumObjects; lc2++)
	{
		*ppvTrav = pObjectArray[lc2];
		ppvTrav++;
	}
	if (pObjectArray == NULL)
	{
		*ppvTrav = NULL;
	}
	return pBuffer;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* Assemble_CommData_Send_InputPacket_Param(DWORD dwPacketType, DWORD dwParameterType, DWORD dwNumParamItems, VOID** pItemArray, int* iRetBufferSize)
{
	//need 3 DWORDs + DWORD * NumObjects + (VOID*)*NumObjects 
	int iRBS = (sizeof(DWORD) * 3) + (sizeof(VOID*) * dwNumParamItems) + 1; //the extra 1 is for the extra increment at the end(to prevent a GPF)
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;

	*pdwTrav = dwPacketType;
	pdwTrav++;
	*pdwTrav = dwParameterType;
	pdwTrav++;
	*pdwTrav = dwNumParamItems; pdwTrav++;
	VOID** ppvTrav = (VOID**)pdwTrav;
	if (pItemArray != NULL)
	for (int lc2 = 0; lc2 < dwNumParamItems; lc2++)
	{
		*ppvTrav = pItemArray[lc2];
		ppvTrav++;
	}
	if (pItemArray == NULL)
	{
		*ppvTrav = NULL;
	}
	return pBuffer;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Two_Dword(DWORD dword1, DWORD dword2, int* iRetBufferSize)
{
	//need 2 DWORDs 
	int iRBS = (sizeof(DWORD) * 2); 
	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	DWORD* pdwTrav = (DWORD*)pBuffer;

	*pdwTrav = dword1;
	pdwTrav++;
	*pdwTrav = dword2;

	return pBuffer;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BYTE* cCommQueue::Assemble_CommData_Tank_Set_Type(LONG lExecTick, int iStructID, int iMaterialType, int* iRetBufferSize)
{
	//need 3 DWORDs
int iRBS = sizeof(LONG) + sizeof(DWORD) * 6;

	if (iRetBufferSize != NULL)
		*iRetBufferSize = iRBS;
	BYTE* pBuffer = new BYTE[ iRBS ]; 

	LONG* plTrav = (LONG*) pBuffer;

	*plTrav = lExecTick;
	plTrav++;
	DWORD* pdwTrav = (DWORD*)plTrav;

	*pdwTrav = OBJTYPE_STRUCTURE; pdwTrav++;
	*pdwTrav = 1; pdwTrav++;
	*pdwTrav = iStructID; pdwTrav++;
	*pdwTrav = OBJTYPE_MATTYPE; pdwTrav++;
	*pdwTrav = 1; pdwTrav++;
	*pdwTrav = iMaterialType; 
	
	return pBuffer;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCommQueue::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cCommQueue.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCommQueue::Write_Log(LPSTR Text)
{

if (DEBUG + DEBUG_COMMQUEUE >= 1)
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
void cCommQueue::WriteLog_Enter()
{

if (DEBUG + DEBUG_COMMQUEUE >= 1)
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
void cCommQueue::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmdInfoTable::Init(VOID* pCA)
{
	pCAccess = pCA;
	
//NOTE: the current reason for the use of + 1 because of the "CANCEL" CQ1 command, which will always be the last in the sequence
	//NULL the other entries before hand
	for (int lc1 = 0; lc1 < NUM_CQ2COMMANDS + 1; lc1++)
	{
		SourceButtonFrameRects[lc1].top = 0;
		SourceButtonFrameRects[lc1].bottom = 0;
		SourceButtonFrameRects[lc1].left = 0;
		SourceButtonFrameRects[lc1].right = 0;
		Names[lc1] = "Not Specified";
	}

	Names[CQ2TYPE_MOVE_B] = "MOVE (M)";
	Names[CQ2TYPE_ATTACK_B] = "ATTACK! (A)";
	Names[CQ2TYPE_START_BUILD_UNIT_PRECONFIG] = "BUILD UNIT (B)";
	Names[CQ2TYPE_START_BUILD_UNIT_CUSTOM] = "BUILD CUSTOM UNIT";
	Names[CQ2TYPE_START_BUILD_COMPONENT] = "BUILD COMPONENT";
	Names[CQ2TYPE_BUILD_STRUCTURE] = "BUILD STRUCT (B)";
	Names[CQ2TYPE_LOAD] = "LOAD (L)";
	Names[CQ2TYPE_UNLOAD_ALL] = "UNLOAD (U)";
	Names[CQ2TYPE_LOAD_MATERIAL] = "FILL TANK (F)";
	Names[CQ2TYPE_UNLOAD_MATERIAL] = "EMPTY TANK (E)";
	Names[CQ2TYPE_TRANSFER_MATERIAL] = "TRANSFER (T)";
	Names[CQ2TYPE_TANK_SET_TYPE] = "SET TYPE (S)";
	Names[CQ2TYPE_TANK_FLUSH] = "EMPTY OUT (E)";
	Names[CQ2TYPE_CANCEL_PROGORDER_TOP] = "CANCEL ORDER (C)";
	Names[CQ2TYPE_LAY_LINK] = "LAY LINK (L)";
	Names[CQ2TYPE_ARM_MATERIAL] = "ARM SPECIAL (S)";


	Names[NUM_CQ2COMMANDS] = "CANCEL";

int lcx,lcy; //local counters
int lc1 = 0;
	//for now, the command number corresponds to the "frame" on CmdButtons.bmp
	for (lc1 = 0; lc1 < NUM_CQ2COMMANDS + 1; lc1++)
	{
		lcx = lc1 / BLIT_FPCOL;
		lcy = lc1 % BLIT_FPCOL;

		SourceButtonFrameRects[lc1].top = lcy * CMDBUTTON_SIZE_Y;
		SourceButtonFrameRects[lc1].bottom = (lcy + 1) * CMDBUTTON_SIZE_Y;
		SourceButtonFrameRects[lc1].left = lcx * CMDBUTTON_SIZE_X;
		SourceButtonFrameRects[lc1].right = (lcx + 1) * CMDBUTTON_SIZE_X;
		
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProgressiveOrder::Init( VOID* pCA)
{
	pCAccess = pCA;
	pCmd2_Init = NULL;
	pCmd2_Prog = NULL;
	pCmd2_Exit = NULL;

	fCompletion = 0;

	pName = NULL;

	pNext = NULL;
	pPrev = NULL;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProgressiveOrder::Flush_List(int iDest)
{
	PCMD2* pTrav;
	PCMD2* pTrav2;


	if (iDest == 0)
		pTrav = pCmd2_Init;
	if (iDest == 1)
		pTrav = pCmd2_Prog;
	if (iDest == 2)
		pTrav = pCmd2_Exit;

	while (pTrav != NULL)
	{
		pTrav2 = pTrav->pNext;
		if (pTrav->pData != NULL)
			delete(pTrav->pData);
		delete(pTrav);
		pTrav = pTrav2;
	}

	if (iDest == 0)
		pCmd2_Init = NULL;
	if (iDest == 1)
		pCmd2_Prog = NULL;
	if (iDest == 2)
		pCmd2_Exit = NULL;



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProgressiveOrder::Exit()
{
	//flush all the command lists
	Flush_List(0);
	Flush_List(1);
	Flush_List(2);
	
	//free the name
	//don't free, let the String Table handle it
//	delete(pName);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProgressiveOrder::Add_Cmd2(PCMD2* pCmd2, BOOL bCopyFlag, int iDest)
{
	PCMD2* pCmdNew = pCmd2;


	if (bCopyFlag == TRUE)
	{
		pCmdNew = new PCMD2;
		//memcpy(pCmdNew, pCmd2, sizeof(PCMD2));
		//try doing it manually
		pCmdNew->wDataSize = pCmd2->wDataSize;
		pCmdNew->wType = pCmd2->wType;
		pCmdNew->pData = pCmdNew->pData;

		pCmdNew->pData = new BYTE[pCmd2->wDataSize];
		memcpy(pCmdNew->pData, pCmd2->pData, pCmd2->wDataSize);
	}

//by default; NULL
PCMD2* pTrav = NULL;
	if (iDest == 0)
	{
		pTrav = pCmd2_Init;
		pCmd2_Init = pCmdNew;
	}
	if (iDest == 1)
	{
		pTrav = pCmd2_Prog;
		pCmd2_Prog = pCmdNew;

	}
	if (iDest == 2)
	{
		pTrav = pCmd2_Exit;
		pCmd2_Exit = pCmdNew;

	}

	//set links
	//new command is added at the head of the set
	pCmdNew->pPrev = NULL;
	pCmdNew->pNext = pTrav;
	if (pTrav != NULL)
		pTrav->pPrev = pCmdNew;



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProgOrderBuffer::Init(VOID* pCA)
{
	pCAccess = pCA;

	//allocate and link
	pPO_First = new cProgressiveOrder;
	pPO_Last = new cProgressiveOrder;

	pPO_First->Init(pCA);
	pPO_Last->Init(pCA);

	pPO_First->pPrev = NULL;
	pPO_First->pNext = (void*)pPO_Last;
	pPO_Last->pPrev = (void*)pPO_First;
	pPO_Last->pNext = NULL;

	iNumOrders = 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProgOrderBuffer::Flush_Buffer()
{
	cProgressiveOrder* pTrav = (cProgressiveOrder*)pPO_First->pNext;
	cProgressiveOrder* pTrav2;

	while (pTrav != pPO_Last)
	{	pTrav2 = (cProgressiveOrder*)pTrav->pNext;

		pTrav->Exit();
		delete(pTrav);

		pTrav = pTrav2;
	}

	iNumOrders = 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProgOrderBuffer::Add_PO(cProgressiveOrder* pPO_New, BOOL bCopyFlag)
{
	cProgressiveOrder* pNew = pPO_New;
	if (bCopyFlag == TRUE)
	{
		pNew = new cProgressiveOrder;

		//here begins the tedious and long process of copying a ProgressiveOrder
		pNew->Init(pCAccess);
		pNew->fCompletion = pPO_New->fCompletion;

		//it's actually faster to manually add each of the attached command2s
		PCMD2* pTrav = pPO_New->pCmd2_Init;
		while (pTrav != NULL)
		{	
			//note that unfortunately this will currently leave the copied PO's commands in reverse order from the original's
			pNew->Add_Cmd2(pTrav, TRUE, 0);
			pTrav = pTrav->pNext;
		}
		pTrav = pPO_New->pCmd2_Prog;
		while (pTrav != NULL)
		{	
			//note that unfortunately this will currently leave the copied PO's commands in reverse order from the original's
			pNew->Add_Cmd2(pTrav, TRUE, 1);
			pTrav = pTrav->pNext;
		}
		pTrav = pPO_New->pCmd2_Exit;
		while (pTrav != NULL)
		{	
			//note that unfortunately this will currently leave the copied PO's commands in reverse order from the original's
			pNew->Add_Cmd2(pTrav, TRUE, 2);
			pTrav = pTrav->pNext;
		}
	}

	//now perform the linking
	cProgressiveOrder* pPrev = (cProgressiveOrder*)pPO_Last->pPrev;
	cProgressiveOrder* pNext = pPO_Last;

	//add at the tail! //08.12.2003
	pPrev->pNext = (void*)pNew;
	pNext->pPrev = (void*)pNew;
	pNew->pNext = (void*)pNext;
	pNew->pPrev = (void*)pPrev;

	iNumOrders++;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProgOrderBuffer::Delete_PO(cProgressiveOrder* pPO_Kill)
{

	//
	cProgressiveOrder* pPrev = (cProgressiveOrder*)pPO_Kill->pPrev;
	cProgressiveOrder* pNext = (cProgressiveOrder*)pPO_Kill->pNext;

	if (pPrev != NULL)
		pPrev->pNext = (void*)pNext;
	if (pNext != NULL)
		pNext->pPrev = (void*)pPrev;

	//exit the kill node
	pPO_Kill->Exit();
	delete(pPO_Kill);

	iNumOrders--;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cProgOrderBuffer::Exit()
{
	Flush_Buffer();

	pPO_First->Exit();
	pPO_Last->Exit();
	delete(pPO_First);
	delete(pPO_Last);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cCommQueue::Scan_Inventory_A(int iNationControl, int iOwnerType, int iOwnerID, int* pdwTypes, int* pdwReturnIDs, int iNumEntries, BOOL bMarkFlag, BOOL bSelectOwnFlag)
{
//DEBUG
Write_Log("Scan_Inventory_A Started.");
WriteLog_Enter();
	//function takes an array of request-to-find types(pdwTypes) and when it finds it, sets the corresponding position in 
	//a return array (pdwReturnIDs) to the ID, and sets its old position (pdwTypes) to an invalid value(ex. -1 or -6996) 
	//bMarkFlag determines whether the function will mark the found entries 
	//and bSelectOwnFlag determines whether the function will also accept entries which already have been
	//marked by this particular iOwnerType and iOwnerID

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cComponentListB* pNatlInventory = &pCA->pNationManager->ppNations[iNationControl]->NationalInventory;
	cStructureA* pStruct;

	cComponentA* pTrav;
	cComponentA* pTrav2;

	int lc1; //local counter

int TEMP_A = -6994;
int TEMP_B = -6996;

int iFoundComponents = 0;

BOOL bBreakFlag;


	//recompile just in case
	pNatlInventory->Recompile_TL();

	for (lc1 = 0; lc1 < iNumEntries; lc1++)
	{


		pTrav = (cComponentA*)pNatlInventory->ppCmpTL_First[pdwTypes[lc1]]->pNext_extlink;

		bBreakFlag = FALSE;
		pdwReturnIDs[lc1] = VAL_INVALID;

		while(bBreakFlag == FALSE && pTrav != pNatlInventory->ppCmpTL_Last[pdwTypes[lc1]])
		{
			if (pTrav->iOwnerID == OWNER_NATIONAL_INVENTORY)
			{
				pdwTypes[lc1] = VAL_INVALID;
				pdwReturnIDs[lc1] = pTrav->iID;
				iFoundComponents++;
				
				//in order to guarantee that if the user specifies two entries of the same type in the pdwTypes array,
				//we don't return the same ID twice, employ a temporary marking on them
				pTrav->iOwnerID = TEMP_A;
				pTrav->iOwnerType = TEMP_A;
				bBreakFlag = TRUE;
			}
			//now if the bSelectOwn Flag is true and the marking matches, do the same
			if (bSelectOwnFlag == TRUE)
			if (pTrav->iOwnerID == iOwnerID && pTrav->iOwnerType == iOwnerType)
			{	
				pdwTypes[lc1] = VAL_INVALID;
				pdwReturnIDs[lc1] = pTrav->iID;
				iFoundComponents++;

				//employ a different marking for these so that, if bMarkFlag was NOT specified, we know what to
				//return the markings back to
				pTrav->iOwnerID = TEMP_B;
				pTrav->iOwnerID = TEMP_B;

				bBreakFlag = TRUE;
			}

			pTrav = (cComponentA*)pTrav->pNext_extlink;

		}
	}


	//now, set markings if the user specified them, if not, set them back
	for (lc1 = 0; lc1 < iNumEntries; lc1++)
	{
		if (pdwReturnIDs[lc1] != TEMP_A && pdwReturnIDs[lc1] != TEMP_B)
			pTrav = pCA->pCmpIDTable->pTable[pdwReturnIDs[lc1]];
				else
			pTrav = NULL;

		//if there is no ID here, do nothing.
		//if an ID is found here, but the scan failed to find all the needed components, even
		//if markflag was true, unmark all the components
		//if ID was found here, markflag is true and all the components were found, proceed with the marking
		if (pTrav != NULL)
		{
			if (bMarkFlag == TRUE && iFoundComponents == iNumEntries)
			{
				pTrav->iOwnerID = iOwnerID;
				pTrav->iOwnerType = iOwnerType;
			}
				else if (pTrav->iOwnerID == TEMP_B)
				{
					pTrav->iOwnerID = iOwnerID;
					pTrav->iOwnerType = iOwnerType;
				}	else if (pTrav->iOwnerID == TEMP_A)
					{
						pTrav->iOwnerID = OWNER_NATIONAL_INVENTORY;
						pTrav->iOwnerType = OWNER_NATIONAL_INVENTORY;
					}
		}
	}

//DEBUG
Write_Log("Scan_Inventory_A Ended");
WriteLog_Enter();

	//now return the correct return value
	//if all went OK, return 0
	if (iFoundComponents == iNumEntries)
		return 0;
			else
		return -1;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
double cCommQueue::Calc_ResourceCost(int* pdwAssembleTypes, int* pdwHaveIDs, int iArraySize)
{
	//the two arrays, since they are likely to be passed directly from Scan_Inventory_A, 
	//must be able to handle the combined nature of the two arrays....(-1 or -6996 invalid)

//some explanation here....so much trouble is needed to calculate the resource cost because 
//units are really groups of components, and so what is really being calculated is the cost of assembling
//the components(for now, some percentage of the total cost of the components). Further complicating the problem,
//is that build_unit commands often/usually each this stage without some of the required components, whose cost
//must also be calculated.

//thus, of these three variables, the first two, assemble cost and construct cost, represent the two parts of building
//the unit....construction of necessary components and assembly of all the components.
//the last, base cost is used to calculate the assembly cost, by keeping track of the cost of all components, ready and missing
double ddAssembleCost = 0;
double ddConstructCost = 0;
double ddBaseCost = 0;

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//check both arrays simultaneously
	for (int c919 = 0; c919 < iArraySize; c919++)
	{
		//if there is something in the assembletypes, add it's type resource cost to the base cost and the construct cost
		if (pdwAssembleTypes[c919] > 0)
		{
			//get a handle to the type
			cCmpInfo* pCmpType = pCA->pCmpInfoTable->pCmpTable[pdwAssembleTypes[c919]];

			ddBaseCost += pCmpType->ddResourceCost;
			ddConstructCost += pCmpType->ddResourceCost;
		}

		//if there is something in the pdwHaveIDs....add it's cost to the base cost only(since we already have it, it doesn't need to be built)
		if (pdwHaveIDs[c919] > 0)
		{
			cComponentA* pComponent = pCA->pCmpIDTable->pTable[pdwHaveIDs[c919]];
			cCmpInfo* pCmpType = pCA->pCmpInfoTable->pCmpTable[pComponent->iType];

			ddBaseCost += pCmpType->ddResourceCost;
		}

	}


ddAssembleCost = ddBaseCost * TEMP_ASSEMBLECOSTRATIO;

	//result is the sum of the assemble cost and the construct cost
	return (ddAssembleCost + ddConstructCost);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//OBSOLETE code from execute_commands_q2  (moved here 08.07.2003)
//			case CQ2TYPE_MOVE:    //obsolete 01.02.2003, replaced by BATCH counterpart
/*				//extract the necessary data from the command
				pDTrav = pQ2Tr->pData;
				//get the execution tick time
				plTrav = (LONG*)pDTrav;
				lExecTick = *plTrav;
				pDTrav += sizeof(LONG);

				//determine whether or not to skip this command based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					SkipFlag = TRUE;
				}
					else
					{
					//get the Unit's ID
					pwTrav = (WORD*)pDTrav;
					wUnitID = *pwTrav;
					pDTrav += sizeof(WORD);
					//get the unit's target index
					plTrav = (LONG*)pDTrav;
				    lTargetIndex = *plTrav;

					//DEBUG
					Write_Log("Command2 Issued. Unit ID: ");
					itoa(wUnitID, NText, 10);
					Write_Log(NText);
					WriteLog_Enter();
				
					//get unit pointer
					pUnit = pCA->pUnitTable->pUnitTable[wUnitID];
					//(copied from original queue code 01.01.2003)
					//set the L4 mode to "nothing"(it doesn't set the L3 mode)
					pUnit->bMode_L5 = UMODE_L5_NOTHING;
					pUnit->bMode_L4 = UMODE_L4_NOTHING;
					//execute a mode command on the selected unit to its target (first param is the UnitID, second is the target index)
					pUnit->bMode_L3 = UMODE_L3_MOVE;
					//set the target index
					pUnit->Pos_sqtarget.index = lTargetIndex;
					//calc the rest of the numbers
					pUnit->Pos_sqtarget.Calc_xy(pCA->pGameWorld->iWidth);
					//...and destroy the unit's current path list
					pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pUnit->pUnitPath);
					pUnit->pUnitPath = NULL;
					}*/
//			break;
//			case CQ2TYPE_ATTACK_UNIT: //obsolete 01.02.2003, replaced by BATCH counterpart
/*
				//extract the necessary data from the command
				pDTrav = pQ2Tr->pData;
				//get the execution tick time
				plTrav = (LONG*)pDTrav;
				lExecTick = *plTrav;
				pDTrav += sizeof(LONG);

				//determine whether or not to skip this command based on data
				if (lExecTick != pCA->pTimer->iCurrTick)				
				{
					SkipFlag = TRUE;

				}
					else
					{
					//get the Unit's ID
					pwTrav = (WORD*)pDTrav;
					wUnitID = *pwTrav;
					pDTrav += sizeof(WORD);
					//get the Enemy's ID
					pwTrav = (WORD*)pDTrav;
					wEnemyID = *pwTrav;
					//get pointers to itself and enemy
					pUnit = pCA->pUnitTable->pUnitTable[wUnitID];
					pEnemy = pCA->pUnitTable->pUnitTable[wEnemyID];
					//DEBUG
					Write_Log("Attack Command Issued. Unit ID: ");
					itoa(wUnitID, NText, 10);
					Write_Log(NText);
					Write_Log(" Enemy ID: ");
					itoa(wEnemyID, NText, 10);
					Write_Log(NText);
					WriteLog_Enter();
					//now set unit attributes ONLY if the enemy unit is still alive
					if (pEnemy != NULL)
					{
					//countermand L5 mode
					pUnit->bMode_L5 = UMODE_L5_NOTHING;
					//set L4 mode
					pUnit->bMode_L4 = UMODE_L4_ATTACK;
					//create a UATTACKINFO structure(to be safe, free any previous structure that was there
					if (pUnit->pAttackInfo != NULL)
						delete(pUnit->pAttackInfo);
					pUnit->pAttackInfo = new UNITATTACKINFO;
					//fill it out 
					pUnit->pAttackInfo->iEnemyID = wEnemyID;
					pUnit->pAttackInfo->Pos_ABSTarget.x = pEnemy->Pos_ABS.x;
					pUnit->pAttackInfo->Pos_ABSTarget.y = pEnemy->Pos_ABS.y;
					pUnit->pAttackInfo->iAttackDirection = pUnit->Calc_Direction(pEnemy->Pos_ABS.x - pUnit->Pos_ABS.x, pEnemy->Pos_ABS.y - pUnit->Pos_ABS.y);
					pUnit->pAttackInfo->iTargetType = CQATTACKTYPE_UNIT;
					//set the target location to move to
					pUnit->Pos_sqtarget.x = pEnemy->Pos_ABS.x / TILE_SIZE;
					pUnit->Pos_sqtarget.y = pEnemy->Pos_ABS.y / TILE_SIZE;
					pUnit->Pos_sqtarget.Calc_Index(pCA->pGameWorld->iWidth);
					//set pointer to enemy
					pUnit->pAttackInfo->pEnemy = pEnemy;
					//destroy unit's current path list
					pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pUnit->pUnitPath);
					pUnit->pUnitPath = NULL;
					//DEBUG
					Write_Log("Completed Issue of Attack Command. Enemy Position: ");
					_gcvt(pEnemy->Pos_ABS.x, 7, NText);
					Write_Log(NText);
					Write_Log(", ");
					_gcvt(pEnemy->Pos_ABS.y, 7, NText);
					Write_Log(NText);
					WriteLog_Enter();
					}
					}   */
/*			break;   
			case CQ2TYPE_BATCH_ATTACK_UNIT:
				//extract the necessary data from the command
				pDTrav = pQ2Tr->pData;
				//get the execution tick time
				plTrav = (LONG*)pDTrav;
				lExecTick = *plTrav;
				pDTrav += sizeof(LONG);

				//determine whether or not to skip this command based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					SkipFlag = TRUE;
				}
					else
					{
					//get the Enemy's ID
					pwTrav = (WORD*)pDTrav;
					wEnemyID = *pwTrav;
					pwTrav++;
					//get the wUnitsIssue
					wUnitsIssue = *pwTrav;
						//now, FOR LOOP
						for (int lc7 = 0; lc7 < wUnitsIssue; lc7++)
						{
							pwTrav++;

							//get pointers to itself and enemy
							pUnit = pCA->pUnitTable->pUnitTable[*pwTrav];
							pEnemy = pCA->pUnitTable->pUnitTable[wEnemyID];
							//now set unit attributes ONLY if the enemy unit is still alive
							if (pEnemy != NULL)
							{
								//set L4 mode
								pUnit->bExternalOrderCompleted = FALSE;
								pUnit->wExternalOrderIssued = CQ2TYPE_BATCH_ATTACK_UNIT;
								pUnit->bMode_L5 = UMODE_L5_NOTHING;
								pUnit->bMode_L4 = UMODE_L4_ATTACK;
								//create a UATTACKINFO structure(to be safe, free any previous structure that was there
								if (pUnit->pAttackInfo != NULL)
									delete(pUnit->pAttackInfo);
								pUnit->pAttackInfo = new UNITATTACKINFO;
								//fill it out 
								pUnit->pAttackInfo->iEnemyID = wEnemyID;
								pUnit->pAttackInfo->Pos_ABSTarget.x = pEnemy->Pos_ABS.x;
								pUnit->pAttackInfo->Pos_ABSTarget.y = pEnemy->Pos_ABS.y;
								pUnit->pAttackInfo->iAttackDirection = pUnit->Calc_Direction(pEnemy->Pos_ABS.x - pUnit->Pos_ABS.x, pEnemy->Pos_ABS.y - pUnit->Pos_ABS.y);
								pUnit->pAttackInfo->iTargetType = CQATTACKTYPE_UNIT;
								//set the target location to move to
								pUnit->Pos_sqtarget.x = pEnemy->Pos_ABS.x / TILE_SIZE;
								pUnit->Pos_sqtarget.y = pEnemy->Pos_ABS.y / TILE_SIZE;
								pUnit->Pos_sqtarget.Calc_Index(pCA->pGameWorld->iWidth);
								//set pointer to enemy
								pUnit->pAttackInfo->pEnemy = pEnemy;
								//set the unit's current weapon(a batch attack, which could occur with many different types of units, MUST use each unit's primary weapon)
								pUnit->pCurrentWeapon = pUnit->pPrimaryWeapon;
								//destroy unit's current path list
								pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pUnit->pUnitPath);
								pUnit->pUnitPath = NULL;
							}
						}
					}
			break;
			//i feel soooo dumb duplicating all this code for structures (04.03.2003)
			case CQ2TYPE_BATCH_ATTACK_STRUCT:
				//extract the necessary data from the command
				pDTrav = pQ2Tr->pData;
				//get the execution tick time
				plTrav = (LONG*)pDTrav;
				lExecTick = *plTrav;
				pDTrav += sizeof(LONG);

				//determine whether or not to skip this command based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					SkipFlag = TRUE;
				}
					else
					{
					//get the Enemy's ID
					pwTrav = (WORD*)pDTrav;
					wEnemyID = *pwTrav;
					pwTrav++;
					//get the wUnitsIssue
					wUnitsIssue = *pwTrav;
						//now, FOR LOOP
						for (int lc7 = 0; lc7 < wUnitsIssue; lc7++)
						{
							pwTrav++;

							//get pointers to itself and enemy
							pUnit = pCA->pUnitTable->pUnitTable[*pwTrav];
							pEnemy_Struct = pCA->pStructIDTable->pTable[wEnemyID];
							//now set unit attributes ONLY if the enemy unit is still alive
							if (pEnemy_Struct != NULL)
							{
								//set L4 mode
								pUnit->bExternalOrderCompleted = FALSE;
								pUnit->wExternalOrderIssued = CQ2TYPE_BATCH_ATTACK_STRUCT;
								pUnit->bMode_L5 = UMODE_L5_NOTHING;
								pUnit->bMode_L4 = UMODE_L4_ATTACK;
								//create a UATTACKINFO structure(to be safe, free any previous structure that was there
								if (pUnit->pAttackInfo != NULL)
									delete(pUnit->pAttackInfo);
								pUnit->pAttackInfo = (UNITATTACKINFO*)malloc(sizeof(UNITATTACKINFO));
								//fill it out 
								pUnit->pAttackInfo->iEnemyID = wEnemyID;
								pUnit->pAttackInfo->Pos_ABSTarget.x = pEnemy_Struct->Pos_primary.x * TILE_SIZE;
								pUnit->pAttackInfo->Pos_ABSTarget.y = pEnemy_Struct->Pos_primary.y * TILE_SIZE;
								pUnit->pAttackInfo->iAttackDirection = pUnit->Calc_Direction(pUnit->pAttackInfo->Pos_ABSTarget.x - pUnit->Pos_ABS.x, pUnit->pAttackInfo->Pos_ABSTarget.y - pUnit->Pos_ABS.y);
								pUnit->pAttackInfo->iTargetType = CQATTACKTYPE_STRUCT;
								//set the target location to move to
								pUnit->Pos_sqtarget.x = pEnemy_Struct->Pos_primary.x;
								pUnit->Pos_sqtarget.y = pEnemy_Struct->Pos_primary.y;
								pUnit->Pos_sqtarget.Calc_Index(pCA->pGameWorld->iWidth);
								//set pointer to enemy
								pUnit->pAttackInfo->pEnemy = pEnemy_Struct;
								//set the unit's current weapon(a batch attack, which could occur with many different types of units, MUST use each unit's primary weapon)
								pUnit->pCurrentWeapon = pUnit->pPrimaryWeapon;
								//destroy unit's current path list
								pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pUnit->pUnitPath);
								pUnit->pUnitPath = NULL;
							}
						}
					}
			break;
			case CQ2TYPE_REPORTFRAME:
				//extract the iControl ID
				pDTrav = pQ2Tr->pData;
				bControlID = *pDTrav;
				pDTrav++;
				//extract the tick count
				plTrav = (LONG*)pDTrav;
				lExecTick = *plTrav;

				pCA->pTimer->iTickReports[bControlID] = lExecTick;
			break;
			case CQ2TYPE_BATCH_MOVE:
				//extract the first starting three elements from the command
				pDTrav = pQ2Tr->pData;
				//get execution tick time
				plTrav = (LONG*)pDTrav;
				lExecTick = *plTrav;
				plTrav++;
				//determine to skip or execute based on data
				if (lExecTick != pCA->pTimer->iCurrTick)
				{
					SkipFlag = TRUE;
				}
					else
					{
						//extract the target index
						lTargetIndex = *plTrav;
						plTrav++;
						//get the number of units being issued to
						pwTrav = (WORD*)plTrav;
						wUnitsIssue = *pwTrav;
					
						//now issue order all units here to move
						for (int lc6 = 0; lc6 < wUnitsIssue; lc6++)
						{
						//increment pwTrav
						pwTrav++;
						//get unit pointer
						pUnit = pCA->pUnitTable->pUnitTable[*pwTrav];
						//(copied from original queue code 01.01.2003)
						//set the L4 mode to "nothing"(it doesn't set the L3 mode)
						pUnit->bExternalOrderCompleted = FALSE;
						pUnit->wExternalOrderIssued = CQ2TYPE_BATCH_MOVE;
						pUnit->bMode_L5 = UMODE_L5_NOTHING;
						pUnit->bMode_L4 = UMODE_L4_NOTHING;
						//execute a mode command on the selected unit to its target (first param is the UnitID, second is the target index)
						pUnit->bMode_L3 = UMODE_L3_MOVE;
						//set the target index
						pUnit->Pos_sqtarget.index = lTargetIndex;
						//calc the rest of the numbers
						pUnit->Pos_sqtarget.Calc_xy(pCA->pGameWorld->iWidth);
						//...and destroy the unit's current path list
						pCA->pPathFinder->Destroy_Path((cPATHENTRY*)pUnit->pUnitPath);
						pUnit->pUnitPath = NULL;
						
						//DEBUG
						Write_Log("Batch Move Command Issued to Unit: ");
						itoa(*pwTrav, NText, 10);
						Write_Log(NText);
						WriteLog_Enter();
						}
					}
			break; 
*/
/*		//obsolete code 08.19.2003				
						int lc2 = 0;
						int lc3 = 0;
						int iFailFlag = 0;	

						pNatInventory = &pCA->pNationManager->ppNations[pStruct->iControl]->NationalInventory;
						while( pCmp_Trav != pCA->pUTypeTable->UnitTypes[iUnitType].CmpAttached.pCmpList_Last)
						{
							arrTypes[lc2] = pCmp_Trav->iType;
							lc2++;

							//the commented out line doesn't work. we have to find the components in the national inventory first
							//this would be done by scanning the national inventory list and finding the necessary components(08.09.2003)
							//and will have to wait till later :-(
							//	arrIDs[lc1] = pCmp_Trav->iID;
							//this can now be done (08.11.2003) we go through the national inventory 
							//find the type, and find a component in the queue with that type
//DEBUG
Write_Log("[Build Unit] Traversing List of component types for specified unit type...");
WriteLog_Enter();
							
							pCmp_Trav2 = (cComponentA*)pNatInventory->ppCmpTL_First[pCmp_Trav->iType]->pNext_extlink;	

Write_Log("[Build Unit] Go...");
WriteLog_Enter();
							int iFoundFlag = 0;
							while (pCmp_Trav2 != pNatInventory->ppCmpTL_Last[pCmp_Trav->iType] && iFoundFlag == 0)
							{

							
//DEBUG
Write_Log("[Build Unit]    Traversing National Inventory for matching components...");
WriteLog_Enter();
								if (pCmp_Trav2->iOwnerID == OWNER_NATIONAL_INVENTORY)
								{
//DEBUG
Write_Log("[Build Unit] Found matching component. Pre-marking...");
WriteLog_Enter();
									arrIDs[lc1] = pCmp_Trav2->iID;
									lc1++;
									iFoundFlag = 1;
									//"pre-mark" this component (the time between when the BUILD order is started and when
									//the progressive order actually takes the components out of the queue should only 
									//be at most a couple of ticks, but it would still be possible for 
									//for two build orders to be issued using the same component. 
									//premarking the component here ensures that the actual "earmarking" further down the line does not encounter
									//any problems
									//(yes I know that this is sort of a HACK) (08.11.2003)
									pCmp_Trav2->iOwnerID = pStruct->iID;
									pCmp_Trav2->iOwnerType = OBJTYPE_STRUCTURE;
								}	
									else
									{
Write_Log("[Build Unit]      incrementing...");
WriteLog_Enter();
										pCmp_Trav2 = (cComponentA*)pCmp_Trav2->pNext_extlink;
							
									}
							}
							
							//if we get here without finding the component, flag a find-component failure
							if (iFoundFlag == 0)
							{
								iFailFlag = 1;
								arrNotFoundTypes[lc3] = pCmp_Trav->iType;
								lc3++;
							}

							pCmp_Trav = (cComponentA*)pCmp_Trav->pNext_list;
						}
*/
/* obsolete code from execute_command2 (start_build_unit_preconfig) 08.20.2003
						}
						if (iRet == -1)
						{
//DEBUG
Write_Log("[Build Unit] Failed to find needed component.");
WriteLog_Enter();
							pCA->pNotifBuffer->Create_Add_Node("Order Canceled: Could not find required components.", pCA->pColorTable->Black, pCA->pColorTable->Black);

							//unmark all nodes ( no longer necessary, done inside scan_invenory_a (08.19.2003)
//							for (lc1 = 0; lc1 < pCA->pUTypeTable->UnitTypes[iUnitType].CmpAttached.iNumComponents; lc1++)
//							if (arrIDs[lc1] != -1)
//							{
//								pCA->pCmpIDTable->pTable[arrIDs[lc1]]->iOwnerID = OWNER_NATIONAL_INVENTORY;
//								pCA->pCmpIDTable->pTable[arrIDs[lc1]]->iOwnerType = OWNER_NATIONAL_INVENTORY;
//							}
							delete(arrIDs);
							delete(arrTypes);
						}
					}
*/
							//warning! keep in mind that if formats change, we will have to change datasize calculations HERE (08.09.2003)
/*  //disabled. new building concept earmarks components the instant the build order is filed. (08.20.2003)
							pCmd2 = new PCMD2;

							pCmd2->wType = CQ2TYPE_EARMARK_COMPONENTS;
							pCmd2->wDataSize = sizeof(void*) + sizeof(DWORD) * 3 + sizeof(DWORD) * pCA->pUTypeTable->UnitTypes[iUnitType].CmpAttached.iNumComponents;
							pCmd2->pData = Assemble_CommData_Earmark_Components(pPO_New, dwID, pdwIDs, pCA->pUTypeTable->UnitTypes[iUnitType].CmpAttached.iNumComponents);
//DEBUG
Write_Log("[Build Unit] Number of components: ");
itoa(pCA->pUTypeTable->UnitTypes[iUnitType].CmpAttached.iNumComponents, NText, 10);
Write_Log(NText);
WriteLog_Enter();				
							
							pPO_New->Add_Cmd2(pCmd2, FALSE, 0);
*/ 
/* //disabled. command no longer issues build orders for missing components. instead, the build orders are integrated inside the unit build order instead(08.20.2003)
//there will be a change in the create_unit_a format that will simply accept both the type and the ID array, create any types that are missing
//the difficulty, cost, build time of the build is decided accordingly here (08.20.2003)
						//here is what we are going to do, if we find components that we don't have, we will attempt to start orders
						//to build them here, and if we can, we then reset iFailFlag back to 0
						//however, if other unit construction snatches the completed components(difficult but possible), 
						//EARMARK_COMPONENTS will alert the user to the problem(parts not found) and terminate 
						//it's calling PO(it would technically be possible to have EARMARK_COMPONENTS re-issue itself + components
						//again but could potentially cause headaches for a user trying to cancel the component build order
						//and keep having it re-issue(08.17.2003)
						if (iRet == -1)
						{
							for (lc3 = 0; lc3 < iNumComponents; lc3++)
							if (arrTypes[lc3] >= 0)
							{
								
								pPO_New = new cProgressiveOrder;
								pPO_New->Init(pCAccess);

								PCMD2* pCmd2New = new PCMD2;

								pCmd2New->wType = CQ2TYPE_BUILD_PROGRESS_COMPONENT;
								pCmd2New->pData = Assemble_CommData_Build_Progress_Component(pPO_New, dwID, arrTypes[lc3]);
								pCmd2New->wDataSize = sizeof(void*) + sizeof(DWORD) * 2; //rememmber to change this if the it changes in the procedure!

								pPO_New->Add_Cmd2(pCmd2New, FALSE, 1);

							pCmd2New = new PCMD2;

							pCmd2New->wType = CQ2TYPE_CREATE_COMPONENT_A;
							pCmd2New->pData = Assemble_CommData_Create_Component_A(pPO_New, dwID, arrTypes[lc3]);
							pCmd2New->wDataSize = sizeof(void*) + sizeof(DWORD) * 2; //rememmber to change this if the it changes in the procedure!
						
							pPO_New->Add_Cmd2(pCmd2New, FALSE, 2);
							//don't need to free because the Prog Order is taking it.
							//now add the prog order to the structure
							//OH, first set the prog order's name
							//for now, just say "Build component"	
							//set the PO's name as "Build component" (for now) 08.12.2003
							char* NameNew = new char[20];
							ZeroMemory(NameNew, sizeof(char) * 20);
							strcat(NameNew, "Build Component");

							pPO_New->pName = NameNew;
							pPO_New->fCompletion = 0;

						//add the prog order
						pStruct->OrderQueue.Add_PO(pPO_New, FALSE);
							}

							iRet = -1;  //0;
						}

*/