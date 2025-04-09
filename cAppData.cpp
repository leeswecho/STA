#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

void cAPPDATA::Init(HINSTANCE IHandle, HINSTANCE PIHandle, LPSTR CLine, LPSTR Windowname, WNDPROC CallBackProc, WNDPROC CallBackProc2, VOID* pCA)
{

	pCAccess = pCA;

	//init the logfile
if (DEBUG + DEBUG_APPDATA >= 1)
	Init_LogFile();

	InstanceHandle = IHandle;
	PrevInstanceHandle = PIHandle;
	CommandLine = CLine;

	bRunFlag = 1;
	bRunStage = APP_RUNSTAGE_INIT;

	//set new memory handler mode
	_set_new_mode(1);

//create an instance of a window class
WNDCLASS wclass, wclass2;

wclass.lpszClassName = "WindowS1";
wclass.hInstance = InstanceHandle;
wclass.hCursor = LoadCursor(InstanceHandle, (LPCTSTR)IDC_ARROW);
wclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
wclass.lpfnWndProc = CallBackProc;
wclass.lpszMenuName = NULL;
wclass.style = CS_DBLCLKS;
wclass.hIcon = LoadIcon(InstanceHandle, (LPCTSTR)IDI_ICON1);
wclass.cbClsExtra = 0;
wclass.cbWndExtra = 0;

RegisterClass(&wclass);

wclass2.lpszClassName = "WindowS2";
wclass2.hInstance = InstanceHandle;
wclass2.hCursor = LoadCursor(InstanceHandle, (LPCTSTR)IDC_ARROW);
wclass2.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
wclass2.lpfnWndProc = CallBackProc2;
wclass2.lpszMenuName = NULL;
wclass2.style = CS_DBLCLKS;
wclass2.hIcon = LoadIcon(InstanceHandle, (LPCTSTR)IDI_ICON1);
wclass2.cbClsExtra = 0;
wclass2.cbWndExtra = 0;

RegisterClass(&wclass);
RegisterClass(&wclass2);

hMainWnd = CreateWindow("WindowS1",
						Windowname,
						WS_VISIBLE ,
						1000,
						0,
						500,
						500,
						NULL,
						NULL,
						InstanceHandle,
						NULL);


Create_Secondary_Window();

//initialize COM
	CoInitialize(NULL);

//initialize iHighestMapIndex anyway
	iHighestMapIndex = 0;
	for (int c44 = 0; c44 < MAX_MAPS; c44++)
	{
		achMapNames[c44] = NULL;
	}

//hide the window since its not necessary to program execution, and I think the surface is getting lost during res changes...(01.04.2002)
ShowWindow(hMainWnd, SW_SHOW);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cAPPDATA::~cAPPDATA()
{
	//CloseWindow(hMainWnd);
	DestroyWindow(hMainWnd);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAPPDATA::Exit()
{

	DestroyWindow(hMainWnd);
	for (int c46 = 0; c46 < iHighestMapIndex; c46++)
	{
		delete(achMapNames[c46]);
	}	


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAPPDATA::Find_Maps()
{
	//reinitialize
	iHighestMapIndex = 0;
	
	for (int c46 = 0; c46 < iHighestMapIndex; c46++)
	{
		delete(achMapNames[c46]);
	}	
	for (int c44 = 0; c44 < MAX_MAPS; c44++)
	{
		achMapNames[c44] = NULL;
	}		
int c45 = 0;
	
ZeroMemory(&DirFileInfo, sizeof(WIN32_FIND_DATA));
HANDLE hRet = FindFirstFile("./maps/*.map", &DirFileInfo);

if (hRet != INVALID_HANDLE_VALUE)
{

		//DEBUG
		Write_Log(DirFileInfo.cFileName);
		WriteLog_Enter();

	achMapNames[c45] = new char[ strlen(DirFileInfo.cFileName) + 10];
	ZeroMemory(achMapNames[c45], sizeof(char) * (strlen(DirFileInfo.cFileName) + 10) );

		strcat(  achMapNames[c45], "./maps/");
		strcat( achMapNames[c45], DirFileInfo.cFileName); //, sizeof(char)*strlen(DirFileInfo.cFileName) );
	
	c45++;


}

ZeroMemory(&DirFileInfo, sizeof(WIN32_FIND_DATA));

	while ( FindNextFile(hRet, &DirFileInfo) == TRUE)
	{
		//DEBUG
		Write_Log(DirFileInfo.cFileName);
		WriteLog_Enter();

		achMapNames[c45] = new char[ strlen(DirFileInfo.cFileName) + 10];   //10, because I am prepending "./maps/"
		ZeroMemory(achMapNames[c45], sizeof(char) * (strlen(DirFileInfo.cFileName) + 10) );

		strcat(  achMapNames[c45], "./maps/");
		strcat( achMapNames[c45], DirFileInfo.cFileName); //, sizeof(char)*strlen(DirFileInfo.cFileName) );
		c45++;		
		
		ZeroMemory(&DirFileInfo, sizeof(WIN32_FIND_DATA));


	}

iHighestMapIndex = c45 - 1;

FindClose(hRet);

	//get map indexes
	for (int c82 = 0; c82 <= iHighestMapIndex; c82++)
	{
		achMapIDs[c82] = Check_Map(achMapNames[c82]);

	}


}
//////////**************************************************************************************************************************************///////////////////////
int cAPPDATA::Check_Map(LPSTR FileName)
{
	hMapFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	//incoming variables
	char LoadBuffer[80]; //buffer _max 
	char inchar = ' ';
	char NText[50];

	int chcount;
	int iHeight = 0;
	int iWidth = 0;
	float fVersion = 2.01;
	int iID = -1;
	DWORD dwNumBytesRead;
	int iIDBlock;

	ZeroMemory(LoadBuffer, sizeof(char)*80);

dwNumBytesRead = 1;

		//use tilde as indicator of EOF or use EOF itself
	while (LoadBuffer[0] != '~' && dwNumBytesRead != 0)
	{
		
		//check for a tilde
		ReadFile(hMapFile, LoadBuffer, 1, &dwNumBytesRead, NULL);

//DEBUG
itoa(LoadBuffer[0], NText, 10);
Write_Log(LoadBuffer); Write_Log(" "); Write_Log(NText); WriteLog_Enter();

		//if the character is an open bracket, read until closed bracket
		if (LoadBuffer[0] == '[')
		{

			Read_To_Stop(LoadBuffer, ']');
//DEBUG
itoa(LoadBuffer[0], NText, 10);
Write_Log(LoadBuffer); Write_Log(" "); Write_Log(NText); WriteLog_Enter();

			iIDBlock = MAP_V2_IDBLOCK_NOTHING;
			//figure out what was in the identifier block
			//'BASIC' (first, map data block)
			if (LoadBuffer[0] == 'B' && 
				LoadBuffer[1] == 'A' &&
				LoadBuffer[2] == 'S' &&
				LoadBuffer[3] == 'I' &&
				LoadBuffer[4] == 'C' &&
				LoadBuffer[5] == 0)
			{
				iIDBlock = MAP_V2_IDBLOCK_BASIC;
			}
			
			//BASIC block
			if (iIDBlock == MAP_V2_IDBLOCK_BASIC)
			{
				//search for the next open bracket
				Read_To_Stop(LoadBuffer, '{');
//DEBUG
itoa(LoadBuffer[0], NText, 10);
Write_Log(LoadBuffer); Write_Log(" "); Write_Log(NText); WriteLog_Enter();
				
				Read_To_Stop(LoadBuffer, ',');
				fVersion = atof(LoadBuffer);
//DEBUG
itoa(LoadBuffer[0], NText, 10);
Write_Log(LoadBuffer); Write_Log(" "); Write_Log(NText); WriteLog_Enter();

				Read_To_Stop(LoadBuffer, ',');
				iID = atoi(LoadBuffer);
//DEBUG
itoa(LoadBuffer[0], NText, 10);
Write_Log(LoadBuffer); Write_Log(" "); Write_Log(NText); WriteLog_Enter();

				Read_To_Stop(LoadBuffer, ',');
				iWidth = atoi(LoadBuffer);
//DEBUG
itoa(LoadBuffer[0], NText, 10);
Write_Log(LoadBuffer); Write_Log(" "); Write_Log(NText); WriteLog_Enter();
				
				Read_To_Stop(LoadBuffer, ',');
				iHeight = atoi(LoadBuffer);
//DEBUG
itoa(LoadBuffer[0], NText, 10);
Write_Log(LoadBuffer); Write_Log(" "); Write_Log(NText); WriteLog_Enter();				

				return iID;
			}

		}
	}

	return iID;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAPPDATA::Read_To_Stop(char* LoadBuffer, char stopchar)
{
	DWORD dwNumBytesRead;

	int chcount = 0;
	char inchar = 0;
	
	while (inchar != stopchar)
	{
		ReadFile(hMapFile, &inchar, 1, &dwNumBytesRead, NULL);
		if (inchar != stopchar)
		{
			LoadBuffer[chcount] = inchar;
			chcount++;
		}

	}
	LoadBuffer[chcount] = 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAPPDATA::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cAPPDATA.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAPPDATA::Write_Log(LPSTR Text)
{

if (DEBUG + DEBUG_APPDATA >= 1)
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
void cAPPDATA::WriteLog_Enter()
{

if (DEBUG + DEBUG_APPDATA >= 1)
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
void cAPPDATA::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAPPDATA::Secondary_Window_Processing()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	char NText[50];
	char NCurrTick[50];

	itoa(pCA->pTimer->iCurrTick, NCurrTick, 10);

	SendMessage(hEditBox, WM_SETTEXT, 0, (LONG)NCurrTick);

	for (int lc4 = 1; lc4 < 4; lc4++)
	{

	ZeroMemory(BoxBuffer, sizeof(char)*10000);
	strcat(BoxBuffer, NCurrTick);
	strcat(BoxBuffer, "\r\n");

	cAINew* pAINew = &pCA->pNationManager->ppNations[lc4]->AINew;
	cAIEntity* pTrav = pAINew->pRequest_First->pStatusQueue_Next;

		int icount = 0;
		while (pTrav != pAINew->pRequest_Last)
		{
				if (pTrav->bTaskRaised)
				{
					itoa(pTrav->iID, NText, 10);
					strcat(BoxBuffer, NText);
					itoa(pTrav->HostInfo.iTaskStep, NText, 10);
					strcat(BoxBuffer, " ");
					strcat(BoxBuffer, NText);
					if (pTrav->iType == AI_ENTITY_TYPE_STRUCT)
						strcat(BoxBuffer, " Struct ");
					if (pTrav->iType == AI_ENTITY_TYPE_UNIT)
						strcat(BoxBuffer, " Unit ");
					if (pTrav->iType == AI_ENTITY_TYPE_SEED)
						strcat(BoxBuffer, " Seed ");
					if (pTrav->iType == AI_ENTITY_TYPE_SECTOR)
						strcat(BoxBuffer, " Sector ");
					gcvt(pTrav->fImmediacy, 4, NText);
					strcat(BoxBuffer, NText);
					strcat(BoxBuffer, "   ");
					pAINew->Get_Task_Name(pTrav->iTaskType, NText);					
					strcat(BoxBuffer, NText);

					strcat(BoxBuffer, "\r\n");
				}
				
				icount++;
				if (icount > 10)
					break;
				pTrav = pTrav->pStatusQueue_Next;
		}
		if (lc4 == 1)
			SendMessage(hEditBox2, WM_SETTEXT, 0, (LONG)BoxBuffer);
		if (lc4 == 2)
			SendMessage(hEditBox3, WM_SETTEXT, 0, (LONG)BoxBuffer);
		if (lc4 == 3)
			SendMessage(hEditBox4, WM_SETTEXT, 0, (LONG)BoxBuffer);

		cAILocation* pATrav = pAINew->pLoc_Cap_First->pNext;
		ZeroMemory(BoxBuffer, sizeof(char)*10000);
		icount = 0;
		while (pATrav != pAINew->pLoc_Cap_Last)
		{
			itoa(pATrav->sqPosition.index, NText, 10);
			strcat(BoxBuffer, NText);
			strcat(BoxBuffer, "  ");
			gcvt(pATrav->ddEfficacy, 4, NText);
			strcat(BoxBuffer, NText);
			strcat(BoxBuffer, "  ");
			itoa(pCA->pTimer->iCurrTick - pATrav->iTick_Evaluated, NText, 10);
			strcat(BoxBuffer, NText);
			strcat(BoxBuffer, "\r\n");

			icount++;
			if (icount > 10)
				break;
			pATrav = pATrav->pNext;
		}
		if (lc4 == 1)
			SendMessage(hEditBox5, WM_SETTEXT, 0, (LONG)BoxBuffer);
		if (lc4 == 2)
			SendMessage(hEditBox6, WM_SETTEXT, 0, (LONG)BoxBuffer);
		if (lc4 == 3)
			SendMessage(hEditBox7, WM_SETTEXT, 0, (LONG)BoxBuffer);

	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cAPPDATA::Create_Secondary_Window()
{
hSecondWnd = CreateWindow("WindowS2",
						"Secondary Window",
						WS_VISIBLE ,
						-800,
						0,
						800,
						500,
						NULL,
						NULL,
						InstanceHandle,
						NULL);

		//create the log window
		hEditBox = CreateWindowEx(WS_EX_CLIENTEDGE,
								"EDIT",
								"Latest Data",
								WS_VISIBLE | WS_CHILD | ES_READONLY,
								10, 40, 
								60,20,
								hSecondWnd,
								(HMENU)ID_EDITBOX1,
								InstanceHandle,
								NULL);
		SendMessage(hEditBox, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

		hEditBox2 = CreateWindowEx(WS_EX_CLIENTEDGE,
								"EDIT",
								"Latest Data",
								WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE,
								80, 20, 
								200,200,
								hSecondWnd,
								(HMENU)ID_EDITBOX2,
								InstanceHandle,
								NULL);
		SendMessage(hEditBox2, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

		hEditBox3 = CreateWindowEx(WS_EX_CLIENTEDGE,
								"EDIT",
								"Latest Data",
								WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE,
								280, 20, 
								200,200,
								hSecondWnd,
								(HMENU)ID_EDITBOX3,
								InstanceHandle,
								NULL);
		SendMessage(hEditBox3, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

		hEditBox4 = CreateWindowEx(WS_EX_CLIENTEDGE,
								"EDIT",
								"Latest Data",
								WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE,
								480, 20, 
								200,200,
								hSecondWnd,
								(HMENU)ID_EDITBOX4,
								InstanceHandle,
								NULL);
		SendMessage(hEditBox4, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

		hEditBox5 = CreateWindowEx(WS_EX_CLIENTEDGE,
								"EDIT",
								"Latest Data",
								WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE,
								80, 200, 
								200,200,
								hSecondWnd,
								(HMENU)ID_EDITBOX5,
								InstanceHandle,
								NULL);
		SendMessage(hEditBox5, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

		hEditBox6 = CreateWindowEx(WS_EX_CLIENTEDGE,
								"EDIT",
								"Latest Data",
								WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE,
								280, 200, 
								200,200,
								hSecondWnd,
								(HMENU)ID_EDITBOX6,
								InstanceHandle,
								NULL);
		SendMessage(hEditBox6, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);

		hEditBox7 = CreateWindowEx(WS_EX_CLIENTEDGE,
								"EDIT",
								"Latest Data",
								WS_VISIBLE | WS_CHILD | ES_READONLY | ES_MULTILINE,
								480, 200, 
								200,200,
								hSecondWnd,
								(HMENU)ID_EDITBOX7,
								InstanceHandle,
								NULL);
		SendMessage(hEditBox7, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);


		HWND hText1 = CreateWindowEx(0,
								"STATIC",
								"Tick:",
								WS_VISIBLE | WS_CHILD,
								10, 20, 
								60,20,
								hSecondWnd,
								NULL,
								InstanceHandle,
								NULL);
		SendMessage(hText1, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////


