#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cTimer::Init(int iCPT, VOID* pCA)
{

	pCAccess = pCA;

if (DEBUG + DEBUG_TIMER >= 1)
	Init_LogFile();

	iCountsPerTick = iCPT;
	iCurrExecCount = 0;
	iExecMode = 0;
	iCurrTick = 0;
	iCurrFrame = 0;
	iContiguousDraws = 0;

	QueryPerformanceFrequency((LARGE_INTEGER *)&lFrequency);

	char NText[50];

	for (int lc1 = 0; lc1 < TEMP_DP_MAXPLAYERS; lc1++)
	{
		iTickReports[lc1] = 0;
		iTickReportIDCodes[lc1] = -1;
		iControls[lc1] = -1;
		ZeroMemory(Names[lc1], sizeof(char)*TEMP_STD_STRLEN);
	}



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cTimer::Exit()
{
		Exit_LogFile();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cTimer::StartCount(int CountIndex)
{

		cClassAccess* pCA = (cClassAccess*)pCAccess;
     if (bTimerThread == TRUE)
		lStartCounts[CountIndex] = llSubCount;
	 else
		QueryPerformanceCounter((LARGE_INTEGER *)&lStartCounts[CountIndex]);



/*if (CountIndex == 0)
{
	//DEBUG
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	char NText[50];
    pCA->pDD_Display->Write_Log("Start Count: ");
	_i64toa(lTimeElapsed[TIMER_EXECT], NText, 10);
	_i64toa(lStartCounts[TIMER_EXECT], NText, 10);
	pCA->pDD_Display->Write_Log(NText);
	pCA->pDD_Display->WriteLog_Enter();
}*/

	if (bTimerThread == TRUE)
	if (CountIndex == 0)
	if (lStartCounts[TIMER_EXECT] < lEndCounts[TIMER_EXECT])
	{
		pCA->pChatBuffer->Add_CBNode("TThread Timing Incorrect", 10);
		pCA->pDD_Display->Write_Log("TThread Timing Incorrect");
		pCA->pDD_Display->WriteLog_Enter();
	}

    //TEST 04.13.2008
	//on my development system, the high performance timer is
	//simply reading the RDTSC registers on the processor 
	//(frequency reported is the clockspeed)
	//it appears that a two cores are getting out of sync
	//and reporting errant values as the instructions go between cores
	//so basically see if I can keep pinging the timing instruction until 
	//until I get the core I want
     //lock it out until we don't have garbage
     //NOTE: on my system, the rate at which 
	int iRetryCount = 0;
	if (bTimerThread == FALSE)
    if (CountIndex == 0)
	while (lStartCounts[TIMER_EXECT] < lEndCounts[TIMER_EXECT])
	{
			//note that we directly query the performance counter regardless
			//if the timerthread is running or not.
		    //this is kind of to ensure that if the timerthread dies for some reason
		    //we can still continue long enough for the user to abort the program
			QueryPerformanceCounter((LARGE_INTEGER *)&lEndCounts[CountIndex]);
			Sleep(0);


		iRetryCount++;
		if (iRetryCount >= TIMER_DESYNC_RETRY)
		{
            pCA->pChatBuffer->Add_CBNode("(TIMER) Broke out of de-sync retry loop.", 100);
			break;
		}
			//DEBUG
			/*	cClassAccess* pCA = (cClassAccess*)pCAccess;
		    char NText[50];
			pCA->pDD_Display->Write_Log("Recheck Start Count:   ");
			_i64toa(lTimeElapsed[TIMER_EXECT], NText, 10);
			_i64toa(lStartCounts[TIMER_EXECT], NText, 10);
	    	pCA->pDD_Display->Write_Log(NText);
			pCA->pDD_Display->WriteLog_Enter(); */
	}

		if (iRetryCount > 0)
		{
			char Text[100];
			ZeroMemory(Text, sizeof(char)*100);
			char NText[50];
			itoa(iRetryCount, NText, 10);
			strcat(Text, "ERROR: HP Timer de-sync. Retried ");
			strcat(Text, NText);
			strcat(Text, " times.");
			pCA->pChatBuffer->Add_CBNode(Text, 10);
		}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cTimer::Ret_Rand()
{
	LONGLONG Val;

		QueryPerformanceCounter((LARGE_INTEGER *)&Val);

	return (int)(Val % 65536);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cTimer::EndCount(int CountIndex)
{
		cClassAccess* pCA = (cClassAccess*)pCAccess;

	if (bTimerThread == TRUE)
		lEndCounts[CountIndex] = llSubCount;
	else
		QueryPerformanceCounter((LARGE_INTEGER *)&lEndCounts[CountIndex]);

	lTimeElapsed[CountIndex] = lEndCounts[CountIndex] - lStartCounts[CountIndex];



/*if (CountIndex == 0)
{
	//DEBUG
		cClassAccess* pCA = (cClassAccess*)pCAccess;
	    char NText[50];
    pCA->pDD_Display->Write_Log("End Count:   ");
	_i64toa(lTimeElapsed[TIMER_EXECT], NText, 10);
	_i64toa(lEndCounts[TIMER_EXECT], NText, 10);
	pCA->pDD_Display->Write_Log(NText);
	pCA->pDD_Display->WriteLog_Enter();
} */

	if (bTimerThread == TRUE)
	if (CountIndex == 0)
	if (lTimeElapsed[CountIndex] < 0)
	{
		pCA->pChatBuffer->Add_CBNode("TThread Timing Incorrect", 10);
		pCA->pDD_Display->Write_Log("TThread Timing Incorrect");
		pCA->pDD_Display->WriteLog_Enter();
	}

    //TEST 04.13.2008
	//on my development system, the high performance timer is
	//simply reading the RDTSC registers on the processor 
	//(frequency reported is the clockspeed)
	//it appears that a two cores are getting out of sync
	//and reporting errant values as the instructions go between cores
	//so basically see if I can keep pinging the timing instruction until 
	//until I get the core I want
	int iRetryCount = 0;
	if (bTimerThread == FALSE)
    if (CountIndex == 0)
	while (lTimeElapsed[CountIndex] < 0)
	{
				
			//note that we directly query the performance counter regardless
			//if the timerthread is running or not.
		    //this is kind of to ensure that if the timerthread dies for some reason
		    //we can still continue long enough for the user to abort the program
			QueryPerformanceCounter((LARGE_INTEGER *)&lEndCounts[CountIndex]);
			Sleep(0);

	    lTimeElapsed[CountIndex] = lEndCounts[CountIndex] - lStartCounts[CountIndex];

		
		iRetryCount++;
		if (iRetryCount >= TIMER_DESYNC_RETRY)
		{
            pCA->pChatBuffer->Add_CBNode("(TIMER) Broke out of de-sync retry loop.", 100);
			break;
		}
		//DEBUG
		/*		cClassAccess* pCA = (cClassAccess*)pCAccess;
		    char NText[50];
			pCA->pDD_Display->Write_Log("Recheck End Count:   ");
			_i64toa(lTimeElapsed[TIMER_EXECT], NText, 10);
			_i64toa(lEndCounts[TIMER_EXECT], NText, 10);
			pCA->pDD_Display->Write_Log(NText);
			pCA->pDD_Display->WriteLog_Enter(); */
	}
	//
		
		if (iRetryCount > 0)
		{
			char Text[100];
			ZeroMemory(Text, sizeof(char)*100);
			char NText[50];
			itoa(iRetryCount, NText, 10);
			strcat(Text, "ERROR: HP Timer de-sync. Retried ");
			strcat(Text, NText);
			strcat(Text, " times.");
			pCA->pChatBuffer->Add_CBNode(Text, 10);
		}




}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cTimer::Process_Counts()
{
		cClassAccess* pCA = (cClassAccess*)pCAccess;



int iRetAssemble;

//DEBUG
/*char NText[50];
    pCA->pDD_Display->Write_Log("Process Count entry. End Count: ");
	//_i64toa(lTimeElapsed[TIMER_EXECT], NText, 10);
	_i64toa(lEndCounts[TIMER_EXECT], NText, 10);
	pCA->pDD_Display->Write_Log(NText);
	pCA->pDD_Display->Write_Log(" Start Count: ");
	_i64toa(lStartCounts[TIMER_EXECT], NText, 10);
	pCA->pDD_Display->Write_Log(NText);
	pCA->pDD_Display->WriteLog_Enter();   */



//update the current time
	QueryPerformanceCounter((LARGE_INTEGER *)&lCurrTime);
//update the current tick value
	//iCurrTick++; //wrong place
//store the old mode
	iOldMode = iExecMode;
//send report every 10 frames(temp - 07.01.2002) (only send when playing multiplayer games
	if (iCurrTick % TEMP_DEFAULT_TICKREPORT_INTERVAL == 0 && pCA->pGameOptions->bGameType == GAMETYPE_MULTI)
	{
		//pCA->pDP_Multiplay->Send_Command(CQDEST_INTERFACE, CQTYPE_REPORTFRAME, pCA->pGameOptions->iTimerID, iCurrTick, 0, 0, 0, NULL, 0, TRUE, FALSE);
		//use command2 issue(01.02.2003)

		BYTE* pBuffer = pCA->pCommQueue->Assemble_CommData_ReportFrame2(pCA->pGameOptions->iTimerID, iCurrTick, &iRetAssemble);
		pCA->pDP_Multiplay->Send_Command2(CQ2TYPE_REPORTFRAME, pBuffer, iRetAssemble, TRUE, FALSE);
	}
	//safeguard procedure against "mutual lockup" which seems to have occurred when tick reports were getting lost before startup
	if (iContiguousDraws >= 10 && pCA->pGameOptions->bGameType == GAMETYPE_MULTI)
	{
		//pCA->pDP_Multiplay->Send_Command(CQDEST_INTERFACE, CQTYPE_REPORTFRAME, pCA->pGameOptions->iTimerID, iCurrTick, 0, 0, 0, NULL, 0, TRUE, FALSE);
		//use command2 issue(01.02.2003)
		BYTE* pBuffer = pCA->pCommQueue->Assemble_CommData_ReportFrame2(pCA->pGameOptions->iTimerID, iCurrTick, &iRetAssemble);
			
		pCA->pDP_Multiplay->Send_Command2(CQ2TYPE_REPORTFRAME, pBuffer, iRetAssemble, TRUE, FALSE);
		iContiguousDraws = 0;
	}

//enable this if you want to flood the network with timing packets.... (only for testing)
//	for (int lc4 = 0; lc4 < 10; lc4++)
//	{
//		BYTE* pBuffer = pCA->pCommQueue->Assemble_CommData_ReportFrame2(pCA->pGameOptions->iTimerID, iCurrTick, &iRetAssemble);
//		pCA->pDP_Multiplay->Send_Command2(CQ2TYPE_REPORTFRAME, pBuffer, iRetAssemble, TRUE, FALSE);
//	}

//use this procedure to calculate figures such as the FPS...
//	iFPS = lFrequency / lTimeElapsed[TIMER_EXECF];  //no longer usable since one frame is never drawn at once

//add the amount of time elapsed during this cycle to the currexec count
	if (lTimeElapsed[TIMER_EXECT] >= 0) //protect possibly against a reset of the timer after 50 days
	iCurrExecCount = iCurrExecCount + lTimeElapsed[TIMER_EXECT];

//determine whether to reset the CurrExecCount based on what mode 
	//if it had just finished running game tick functions, switch it to drawing
	if (iExecMode == EXECMODE_GTICK)
		{
		//set back exec count since the program just finished executing the tick
		iCurrExecCount = iCurrExecCount - iCountsPerTick;
//		iExecMode = EXECMODE_GDRAW;
		}	



	//if the exec count is higher than the countspertick tell it execute tick
	if (iCurrExecCount > iCountsPerTick)
		iExecMode = EXECMODE_GTICK;
			else
		iExecMode = EXECMODE_GDRAW;	 //otherwise, tell it to draw again

//execute lag checking only in Multiplayer mode
if (pCA->pGameOptions->bGameType == GAMETYPE_MULTI)
{
	//if there is another player that is lagging....wait until it's back up (temp synchronization measure implementation 07.02.2002)
	bLagging = FALSE;
	int lc10;

iLaggingPlayer = 0;
	for (lc10 = 0; lc10 < pCA->pGameOptions->iNumPlayers; lc10++)
	{
		//if ( pCA->pNationManager->ppNations[lc10]->bAIFlag != TRUE )  //don't do this anymore (disconnect TimerID from iControl)
		if ( iTickReportIDCodes[lc10] >= 0) //if active  (do this instead)
		if (lc10 != pCA->pGameOptions->iTimerID && iTickReports[lc10] + TEMP_DEFAULT_MAXLAG < iCurrTick)
		{
			bLagging = TRUE;
			iLaggingPlayer = lc10;
		}

	}


	if (bLagging == TRUE)
	{
		iExecMode = EXECMODE_GDRAW;
		iCurrExecCount = 0;

		//DEBUG
		Write_Log("Lagging. Time: ");
		char NText[50];
		itoa(iCurrTick, NText, 10);
		Write_Log(NText);
		itoa(iLaggingPlayer, NText, 10);
		Write_Log("Player: ");
		Write_Log(NText);
		WriteLog_Enter();

	}
}




//update the number of contiguous draws which have occurred
	if (iExecMode == EXECMODE_GDRAW && iOldMode == EXECMODE_GDRAW)
		iContiguousDraws++;
	if (iExecMode == EXECMODE_GTICK)
		iContiguousDraws = 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cTimer::Set_CPT_FPS(int iFPS)
{
	iCountsPerTick = lFrequency / iFPS;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cTimer::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cTimer.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cTimer::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_TIMER >= 1)
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
void cTimer::WriteLog_Enter()
{
if (DEBUG + DEBUG_TIMER >= 1)
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
void cTimer::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
