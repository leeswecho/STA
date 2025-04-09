#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDP_Multiplay::Init(VOID* pCA)
{
	pCAccess = pCA;

if (DEBUG + DEBUG_DP_MULTIPLAY >= 1)
	Init_LogFile();

iNumCallBackThreadsActive = 0;

HRESULT hr20; //debug
	const char* KText;
	int lc8; //counter
	char NText[50];

	cClassAccess* pCA2 = (cClassAccess*)pCA;

//DEBUG
Write_Log("INIT_A");
WriteLog_Enter();

	//init 
	wHostsFound = 0;
	wSelectedHost = 0;
	iActiveProvider = 0;
	dwNumProviders = 0;
	wUserContext = 116;
	iMessagesReceived = 0;
	pAddrHost = NULL;
	bConnected = FALSE;
	bSetPeerInfoRequired = FALSE;
	bUPDRequired = FALSE;

	
	bStaticBufferInUse = FALSE;
	iNumMessagesInBuffer = 0;

	int iHeight = pCA2->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight;
	int iWidth = pCA2->pFontInfoTable->Fonts[FONT_STANDARD1].iWidth;

	for (lc8 = 0; lc8 < TEMP_DP_MAXHOSTS; lc8++)
	{
		HostsFound[lc8].pAppDesc = NULL;
		HostsFound[lc8].pDeviceAddr = NULL;
		HostsFound[lc8].pHostAddr = NULL;
	}

	//initialize the critical section object
	InitializeCriticalSection(&csCriticalSection);

	//06.27.2002 Create the DirectPlay object
	CoCreateInstance(CLSID_DirectPlay8Peer, NULL, CLSCTX_INPROC, IID_IDirectPlay8Peer, (void**)&pDP8);

	//initialize
	hr20 = pDP8->Initialize(&wUserContext, DP8_Handle_Message, 0);

	//enumerate service providers (to get the right size)
	ServiceProviders = new DPN_SERVICE_PROVIDER_INFO; //(0)
	hr20 = pDP8->EnumServiceProviders(NULL, NULL, ServiceProviders, &dwSPBufferSize, &dwNumProviders, DPNENUMSERVICEPROVIDERS_ALL);
	delete(ServiceProviders);
	//re-allocate
	ServiceProviders = (DPN_SERVICE_PROVIDER_INFO*)new BYTE[dwSPBufferSize];
	//call again
	hr20 = pDP8->EnumServiceProviders(NULL, NULL, ServiceProviders, &dwSPBufferSize, &dwNumProviders, DPNENUMSERVICEPROVIDERS_ALL);
		if (hr20 != S_OK)
			Write_Log("-EnumSP failed.-");

	//allocate pDeviceArrays
	pDeviceArrays = new DPDEVICEINFOARRAY[dwNumProviders];
	ZeroMemory(pDeviceArrays, sizeof(DPDEVICEINFOARRAY) * dwNumProviders);
	//allocate pSPConnectInfo
	pSPConnectInfo = new DPSPCONNECTINFO[dwNumProviders];
	ZeroMemory(pSPConnectInfo, sizeof(DPSPCONNECTINFO) * dwNumProviders);
	//null pointers
	for (lc8 = 0; lc8 < dwNumProviders; lc8++)
	{ pSPConnectInfo[lc8].pDeviceAddr = NULL; 
	  ZeroMemory(pSPConnectInfo[lc8].wcaIP, sizeof(WCHAR) * 80);
	  ZeroMemory(pSPConnectInfo[lc8].wcaPhoneNumber, sizeof(WCHAR) * 80);
	  ZeroMemory(pSPConnectInfo[lc8].wcaPortIP, sizeof(WCHAR) * 80);
	  ZeroMemory(pSPConnectInfo[lc8].wcaSessionName, sizeof(WCHAR) * 80);
	}

int devcount = 0;


//get device info for each of the service providers availoable
	for (lc8 = 0; lc8 < dwNumProviders; lc8++)
	{
			//safeguard 08.12.2003
			pDeviceArrays[lc8].dwNumDevices = 0;
			pDeviceArrays[lc8].wActiveDevice = -1;

		//call EnumSP again to get devices (actually two more times, to get right buffer size, and then get the device data
		pDeviceArrays[lc8].SPDevices = new DPN_SERVICE_PROVIDER_INFO;
		hr20 = pDP8->EnumServiceProviders(&ServiceProviders[lc8].guid, NULL, pDeviceArrays[lc8].SPDevices, &pDeviceArrays[lc8].dwDeviceBufferSize, &pDeviceArrays[lc8].dwNumDevices, 0);
//	KText = DXGetErrorString9(hr20);
//	Write_Log("EnumServiceProviders2: ");
//	Write_Log((char*)KText);
//	WriteLog_Enter();
		
		//delete and re-allocate
		delete(pDeviceArrays[lc8].SPDevices);
		if (pDeviceArrays[lc8].dwDeviceBufferSize != 0)
		{

			pDeviceArrays[lc8].SPDevices = (DPN_SERVICE_PROVIDER_INFO*)new BYTE[pDeviceArrays[lc8].dwDeviceBufferSize];
				//call EnumSP again
				hr20 = pDP8->EnumServiceProviders(&ServiceProviders[lc8].guid, NULL, pDeviceArrays[lc8].SPDevices, &pDeviceArrays[lc8].dwDeviceBufferSize, &pDeviceArrays[lc8].dwNumDevices, 0);
//			KText = DXGetErrorString9(hr20);
//			Write_Log("EnumServiceProviders2: ");
//			Write_Log((char*)KText);
//			WriteLog_Enter();



			for (int lc9 = 0; lc9 < pDeviceArrays[lc8].dwNumDevices; lc9++)
			{
			devcount++;
			Write_Log_Special(reinterpret_cast<unsigned short*>(pDeviceArrays[lc8].SPDevices[lc9].pwszName));
			WriteLog_Enter(); 
			itoa(devcount, NText, 10);
			}
		}

		//set default values for types of providers	
		if (ServiceProviders[lc8].guid == CLSID_DP8SP_SERIAL)
		{
			pSPConnectInfo[lc8].dwBaud = 57600;
			pSPConnectInfo[lc8].wcsFlowControl = DPNA_FLOW_CONTROL_XONXOFF;
			pSPConnectInfo[lc8].wcsParity = DPNA_PARITY_NONE;
			pSPConnectInfo[lc8].wcsStopBits = DPNA_STOP_BITS_ONE;
		}
		if (ServiceProviders[lc8].guid == CLSID_DP8SP_TCPIP)
		{
			pSPConnectInfo[lc8].dwPortIP = 27006;
		}
		pDeviceArrays[lc8].wActiveDevice = -1;
	}


//set appdesc
	ZeroMemory( &AppDesc, sizeof(DPN_APPLICATION_DESC) );
	AppDesc.dwSize = sizeof(DPN_APPLICATION_DESC);
	AppDesc.guidApplication = APP_A1_GUID;


itoa(iActiveProvider, NText, 10);
Write_Log(NText);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cDP_Multiplay::~cDP_Multiplay()
{
/*	HRESULT hr21;  //moved to Exit() 07.12.2002
	const char* KText;
	
	//get rid of host infos
	for (int lc9 = 0; lc9 < TEMP_DP_MAXHOSTS; lc9++)
	{
		if (HostsFound[lc9].pAppDesc != NULL)  //any time the appdesc is created the others will also be created, so check for the first only
		{
			delete(HostsFound[lc9].pAppDesc);
			HostsFound[lc9].pDeviceAddr->Release();
			HostsFound[lc9].pHostAddr->Release();
		}
	}

	//end session
	hr21 = pDP8->TerminateSession(NULL, 0, 0);

	 	Write_Log("Terminate Session: ");
		KText = DXGetErrorString9(hr21);
		Write_Log((char*)KText);
		WriteLog_Enter();

	hr21 = pDP8->Close(0);

	 	Write_Log("Close: ");
		KText = DXGetErrorString9(hr21);
		Write_Log((char*)KText);
		WriteLog_Enter();
	
	hr21 = pDP8->Release();
	pDP8 = NULL;

	 	Write_Log("Release: ");
		KText = DXGetErrorString9(hr21);
		Write_Log((char*)KText);
		WriteLog_Enter();

//delete critical section object (07.01.2002)
	DeleteCriticalSection(&csCriticalSection);
*/
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDP_Multiplay::Exit()
{

	//delete service providers array
	for (int c37 = 0; c37 < dwNumProviders; c37++)
	{
		if (pDeviceArrays[c37].dwDeviceBufferSize != 0)	
			delete(pDeviceArrays[c37].SPDevices);

	}
	delete(pDeviceArrays);
	delete(pSPConnectInfo);

	//get rid of host infos
	for (int lc9 = 0; lc9 < TEMP_DP_MAXHOSTS; lc9++)
	{
		if (HostsFound[lc9].pAppDesc != NULL)  //any time the appdesc is created the others will also be created, so check for the first only
		{
			delete(HostsFound[lc9].pAppDesc);
			HostsFound[lc9].pDeviceAddr->Release();
			HostsFound[lc9].pHostAddr->Release();
		}
	}

	pDP8->Close(0);
	pDP8->Release();

//delete critical section object (07.01.2002)
	DeleteCriticalSection(&csCriticalSection);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDP_Multiplay::Write_Log(LPSTR Text)
{

if (DEBUG + DEBUG_DP_MULTIPLAY >= 1)
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
char* cDP_Multiplay::Write_Log_Special(unsigned short* Text)
{
if (DEBUG + DEBUG_DP_MULTIPLAY >= 1)
{

DWORD numbyteswritten = 0;
char* cText = (char*)Text;
char* RetText = new char[lstrlenW(reinterpret_cast<LPCWSTR>(Text))];

for (int lc6 = 0; lc6 < lstrlenW(reinterpret_cast<LPCWSTR>(Text)); lc6++)
{
WriteFile( hLogFile,
		   Text + lc6,//cText + (lc6 * 2),
		   1,
		   &numbyteswritten,
		   NULL);
RetText[lc6] = *(cText + (lc6 *2));
}
RetText[lstrlenW(reinterpret_cast<LPCWSTR>(Text))] = 0;

return RetText;
}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDP_Multiplay::WriteLog_Enter()
{
if (DEBUG + DEBUG_DP_MULTIPLAY >= 1)
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
void cDP_Multiplay::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cDP_Multiplay.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDP_Multiplay::Send_Command(WORD wDest, WORD wType, LONG wParam1, WORD wParam2, WORD wParam3, double fParam4, double fParam5, void* pParam6, LONG lExecTick, BOOL LoopBackFlag, BOOL ReliableFlag)
{
	//here goes nothing...(06.30.2002)
	//changed so that the FIRST byte of the data contains information about the nature of the message(07.22.2002)
	VOID* pSendBuffer = new BYTE[sizeof(PCMD)+1];
	BYTE* pHeader = (BYTE*)pSendBuffer;
	PCMD* pCommand = (PCMD*)(pHeader+1);
	DWORD SendFlags = DPNSEND_SYNC;    //guaranteed flag added to buffer against lost packets(12.30.2002)
 
	//if the LOOPback flag is false, disable loopback
	if (LoopBackFlag == FALSE)
		SendFlags = SendFlags | DPNSEND_NOLOOPBACK;

	//if guaranteed flag is true, enable guaranteed flag
	if (ReliableFlag == TRUE)
		SendFlags = SendFlags | DPNSEND_GUARANTEED;
	
	//set the header
	*pHeader = DPSENDTYPE_COMMAND;


	pCommand->wDest = wDest;
	pCommand->wType = wType;
	pCommand->wParam1 = wParam1;
	pCommand->wParam2 = wParam2;
	pCommand->wParam3 = wParam3;
	pCommand->fParam4 = fParam4;
	pCommand->fParam5 = fParam5;
	pCommand->pParam6 = pParam6;
	pCommand->lExecTick = lExecTick;

	DPN_BUFFER_DESC DPBDesc;

	//changed so that the FIRST byte of the data contains information about the nature of the message

	DPBDesc.dwBufferSize = sizeof(PCMD)+1;
	DPBDesc.pBufferData = (BYTE*)pSendBuffer;

	pDP8->SendTo(DPNID_ALL_PLAYERS_GROUP, &DPBDesc, 1, 1000, NULL, NULL, SendFlags);

	//this SHOULD be able to delete...
	delete(pSendBuffer);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDP_Multiplay::Send_Command2(WORD wType, BYTE* pData, WORD wDataSize, BOOL LoopBackFlag, BOOL ReliableFlag)
{

	Send_Command2(DPNID_ALL_PLAYERS_GROUP, wType, pData, wDataSize, LoopBackFlag, ReliableFlag);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDP_Multiplay::Send_Command2(DPNID DPNIDTarget, WORD wType, BYTE* pData, WORD wDataSize, BOOL LoopBackFlag, BOOL ReliableFlag)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	DWORD SendFlags = DPNSEND_SYNC;    //guaranteed flag added to buffer against lost packets(12.30.2002)
 
	//if the LOOPback flag is false, disable loopback
	if (LoopBackFlag == FALSE)
		SendFlags = SendFlags | DPNSEND_NOLOOPBACK;

	//if guaranteed flag is true, enable guaranteed flag
	if (ReliableFlag == TRUE)
		SendFlags = SendFlags | DPNSEND_GUARANTEED;

//size of the buffer being sent is wDataSize + (2*sizeof(WORD)) + Header Byte
	VOID* pSendBuffer = new BYTE[ wDataSize + (2*sizeof(WORD)) + 1];
	BYTE* pHeader = (BYTE*)pSendBuffer;


	//set the header
	*pHeader = DPSENDTYPE_COMMAND2;

	pHeader++;
	//NOTE that the header pointer has been incremented before setting pwCommData. the 
	//commData does NOT actually point at the header
	WORD* pwCommData = (WORD*)pHeader;

//I have elected not to send the data explicitly as a PCMD2 type, but rather the sent data will be re-assembled into a 
//PCMD2 when received on the other end(01.01.2003)
	//the two words will go first: wType and then wDataSize
	*pwCommData = wType;
	pwCommData++;
	*pwCommData = wDataSize;
	pwCommData++;

//DEBUG
Write_Log("Copying Command Data.");
WriteLog_Enter();

	//now memcopy the rest of the data into the buffer
	memcpy((void*)pwCommData, pData, wDataSize);

//DEBUG
Write_Log("Copy of Command Data Completed.");
WriteLog_Enter();

	//fill out the buffer desc
	DPN_BUFFER_DESC DPBDesc;

	DPBDesc.dwBufferSize = wDataSize + (2*sizeof(WORD)) + 1;
	DPBDesc.pBufferData = (BYTE*)pSendBuffer;

	HRESULT hr9 = pDP8->SendTo(/*DPNID_ALL_PLAYERS_GROUP*/ DPNIDTarget, &DPBDesc, 1, 1000, NULL, NULL, SendFlags);

//DEBUG
Write_Log("Freeing Send Buffer.");
WriteLog_Enter();

	//this SHOULD be able to delete...
	delete(pSendBuffer);

//DEBUG
Write_Log("Send Buffer Freed.");
WriteLog_Enter();



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDP_Multiplay::Send_Command_Directed(DPNID iDPNID, WORD wDest, WORD wType, LONG wParam1, WORD wParam2, WORD wParam3, double fParam4, double fParam5, void* pParam6, LONG lExecTick, BOOL ReliableFlag)
{
	//special version which sends a direct send_command message, currently used only for sending out control IDs(12.23.2002)
	//here goes nothing...(06.30.2002)
	//changed so that the FIRST byte of the data contains information about the nature of the message(07.22.2002)
	VOID* pSendBuffer = new BYTE[sizeof(PCMD) + 1];    
	BYTE* pHeader = (BYTE*)pSendBuffer;
	PCMD* pCommand = (PCMD*)(pHeader+1);
	DWORD SendFlags = DPNSEND_SYNC;

	//if guaranteed flag is true, enable guaranteed flag
	if (ReliableFlag == TRUE)
		SendFlags = SendFlags | DPNSEND_GUARANTEED;

	//set the header
	*pHeader = DPSENDTYPE_COMMAND;


	pCommand->wDest = wDest;
	pCommand->wType = wType;
	pCommand->wParam1 = wParam1;
	pCommand->wParam2 = wParam2;
	pCommand->wParam3 = wParam3;
	pCommand->fParam4 = fParam4;
	pCommand->fParam5 = fParam5;
	pCommand->pParam6 = pParam6;
	pCommand->lExecTick = lExecTick;

	DPN_BUFFER_DESC DPBDesc;

	//changed so that the FIRST byte of the data contains information about the nature of the message

	DPBDesc.dwBufferSize = sizeof(PCMD)+1;
	DPBDesc.pBufferData = (BYTE*)pSendBuffer;

	pDP8->SendTo(iDPNID, &DPBDesc, 1, 1000, NULL, NULL, SendFlags);

	//this SHOULD be able to delete...
	delete(pSendBuffer);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDP_Multiplay::Send_ChatText(char* pBuffer, char* Name, DWORD dwBufferSize)
{
	//07.22.2002
	WORD wStrlen = strlen(Name);
	BYTE* pSendBuffer = new BYTE[dwBufferSize + 3 + wStrlen];   //the text + the name + :(1) + space +  the first byte
	
	if (wStrlen == 0)
		return;

	//set the header
	*pSendBuffer = DPSENDTYPE_CHATTEXT;

	//copy, byte by byte (for ease of checking
	for (int lc29 = 1; lc29 <= wStrlen; lc29++)
		*(pSendBuffer + lc29) = Name[lc29 - 1];
	//put in the colon :
	*(pSendBuffer + wStrlen + 1) = ':';
	//put in the space
	*(pSendBuffer + wStrlen + 2) = ' ';

	//deep copy the text into the buffer
	memcpy(pSendBuffer + wStrlen + 3, pBuffer, dwBufferSize);

	DPN_BUFFER_DESC DPBDesc;
	DPBDesc.dwBufferSize = dwBufferSize + 3 + wStrlen;
	DPBDesc.pBufferData = (BYTE*)pSendBuffer;

	pDP8->SendTo(DPNID_ALL_PLAYERS_GROUP, &DPBDesc, 1, 1000, NULL, NULL, DPNSEND_SYNC | DPNSEND_GUARANTEED);


	delete(pSendBuffer);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDP_Multiplay::Send_ChatText(char* pBuffer, WCHAR* wcsName, DWORD dwBufferSize)
{
	//07.22.2002
	WORD wStrlen = lstrlenW(wcsName);
	BYTE* pSendBuffer = new BYTE[dwBufferSize + 3 + wStrlen];   //the text + the name + :(1) + space +  the first byte

	if (wStrlen == 0)
		return;


	//set the header
	*pSendBuffer = DPSENDTYPE_CHATTEXT;

	//the name cannot be copied directly(because it's WIDE), so copy, byte by byte
	for (int lc29 = 1; lc29 <= wStrlen; lc29++)
		*(pSendBuffer + lc29) = (char)wcsName[lc29 - 1];
	//put in the colon :
	*(pSendBuffer + wStrlen + 1) = ':';
	//put in the space
	*(pSendBuffer + wStrlen + 2) = ' ';

	//deep copy the text into the buffer
	memcpy(pSendBuffer + wStrlen + 3, pBuffer, dwBufferSize);

	DPN_BUFFER_DESC DPBDesc;
	DPBDesc.dwBufferSize = dwBufferSize + 3 + wStrlen;
	DPBDesc.pBufferData = (BYTE*)pSendBuffer;

	pDP8->SendTo(DPNID_ALL_PLAYERS_GROUP, &DPBDesc, 1, 1000, NULL, NULL, DPNSEND_SYNC | DPNSEND_GUARANTEED);


	delete(pSendBuffer);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDP_Multiplay::Send_RandKeys(LONG* pKeys, DWORD dwBufferSize)
{
	BYTE* pSendBuffer = new BYTE[dwBufferSize*sizeof(LONG) + 1];
	
	//set the header
	*pSendBuffer = DPSENDTYPE_RANDKEYS;

//DEBUG
Write_Log("Sending Rand Keys");
WriteLog_Enter();
	
	//deep copy the keys
	memcpy(pSendBuffer+1, pKeys, dwBufferSize * sizeof(LONG));

	//fill out buffer desc
	DPN_BUFFER_DESC DPBDesc;
	DPBDesc.dwBufferSize = (dwBufferSize * sizeof(LONG)) + 1;
	DPBDesc.pBufferData = (BYTE*)pSendBuffer;
	
	pDP8->SendTo(DPNID_ALL_PLAYERS_GROUP, &DPBDesc, 1, 1000, NULL, NULL, DPNSEND_SYNC | DPNSEND_GUARANTEED);	


//DEBUG
Write_Log("Rand Keys Sent");
WriteLog_Enter();

	delete(pSendBuffer);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDP_Multiplay::EnumHosts()
{  //enumerates hosts based on available device information
	
HRESULT hr20;
IDirectPlay8Address* pAddrDevice;
const char* KText;

	//clear both the hostaddr and the deviceaddr (just in case there were loaded before
	if (pSPConnectInfo[iActiveProvider].pDeviceAddr != NULL) 
		pSPConnectInfo[iActiveProvider].pDeviceAddr->Clear();
			else
			{
				CoCreateInstance(CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC, IID_IDirectPlay8Address, (void**)&pSPConnectInfo[iActiveProvider].pDeviceAddr);
			//changed to CoCreateInstance after moving to DX9 (09.15.2003)
			//DirectPlay8AddressCreate(&IID_IDirectPlay9Address, (VOID**)&pSPConnectInfo[iActiveProvider].pDeviceAddr, NULL);
			}
	
	if (pAddrHost != NULL) 
		pAddrHost->Clear();
			else
			{
				CoCreateInstance(CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC, IID_IDirectPlay8Address, (void**)&pAddrHost);
				//DirectPlay9AddressCreate(&IID_IDirectPlay9Address, (VOID**)&pAddrHost, NULL);
			}

Write_Log("A");
pAddrDevice = pSPConnectInfo[iActiveProvider].pDeviceAddr;

	//set SP on AddrHost
	pAddrHost->SetSP(&ServiceProviders[iActiveProvider].guid);
	
	//set SP and device GUIDs on the device addr
	pAddrDevice->SetSP(&ServiceProviders[iActiveProvider].guid);
	pAddrDevice->SetDevice(&pDeviceArrays[iActiveProvider].SPDevices[pDeviceArrays[iActiveProvider].wActiveDevice].guid);

	//add components depending on service provider guid
	if (ServiceProviders[iActiveProvider].guid == CLSID_DP8SP_SERIAL)
	{
		pSPConnectInfo[iActiveProvider].pDeviceAddr->AddComponent(DPNA_KEY_BAUD, &pSPConnectInfo[iActiveProvider].dwBaud, sizeof(DWORD), DPNA_DATATYPE_DWORD);
		pSPConnectInfo[iActiveProvider].pDeviceAddr->AddComponent(DPNA_KEY_FLOWCONTROL, pSPConnectInfo[iActiveProvider].wcsFlowControl, (wcslen(pSPConnectInfo[iActiveProvider].wcsFlowControl)+1)*2, DPNA_DATATYPE_STRING);
		pSPConnectInfo[iActiveProvider].pDeviceAddr->AddComponent(DPNA_KEY_PARITY, pSPConnectInfo[iActiveProvider].wcsParity, (wcslen(pSPConnectInfo[iActiveProvider].wcsParity)+1)*2, DPNA_DATATYPE_STRING);
		pSPConnectInfo[iActiveProvider].pDeviceAddr->AddComponent(DPNA_KEY_STOPBITS, pSPConnectInfo[iActiveProvider].wcsStopBits, (wcslen(pSPConnectInfo[iActiveProvider].wcsStopBits)+1)*2, DPNA_DATATYPE_STRING);
	}
	if (ServiceProviders[iActiveProvider].guid == CLSID_DP8SP_TCPIP)
	{
		WCHAR* stopstring;
		pSPConnectInfo[iActiveProvider].dwPortIP = wcstol(pSPConnectInfo[iActiveProvider].wcaPortIP, &stopstring, 10);
		pSPConnectInfo[iActiveProvider].pDeviceAddr->AddComponent(DPNA_KEY_HOSTNAME, pSPConnectInfo[iActiveProvider].wcaIP, (wcslen(pSPConnectInfo[iActiveProvider].wcaIP)+1)*2, DPNA_DATATYPE_STRING);
		pSPConnectInfo[iActiveProvider].pDeviceAddr->AddComponent(DPNA_KEY_PORT, &pSPConnectInfo[iActiveProvider].dwPortIP, sizeof(pSPConnectInfo[iActiveProvider].dwPortIP), DPNA_DATATYPE_DWORD);

	}


	//enumhosts
	hr20 = pDP8->EnumHosts(&AppDesc, pAddrHost, pAddrDevice, NULL, 0, 0, 0, 0, NULL, NULL, DPNENUMHOSTS_SYNC);// | DPNENUMHOSTS_OKTOQUERYFORADDRESSING);//&hTestAsync, DPNENUMHOSTS_OKTOQUERYFORADDRESSING);
	
//	KText = DXGetErrorString9(hr20);
//	Write_Log("EnumHosts: ");
///	Write_Log((char*)KText);
//	WriteLog_Enter();



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
HRESULT cDP_Multiplay::HostSession(WCHAR* pSessionName)
{
HRESULT hr20;
const char* KText;
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//set peer info before connecting (07.18.2002)
	PLAYERDATA* pPD = (PLAYERDATA*)pCA->pGameOptions->InfoPlayer_Local.pvData;
	//disabled 12.23.2002 (iControl IDs now assigned after game start)
	//pPD-iControl = 0;

	hr20 = pDP8->SetPeerInfo(&pCA->pGameOptions->InfoPlayer_Local, NULL, NULL, DPNSETPEERINFO_SYNC);
	
//	KText = DXGetErrorString9(hr20);
//	Write_Log("SetPeerInfo: ");
//	Write_Log((char*)KText);
//	WriteLog_Enter();

	
	//clear both the hostaddr and the deviceaddr (just in case there were loaded before
	if (pSPConnectInfo[iActiveProvider].pDeviceAddr != NULL) 
		pSPConnectInfo[iActiveProvider].pDeviceAddr->Clear();
			else
			{
				CoCreateInstance(CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC, IID_IDirectPlay8Address, (VOID**)&pSPConnectInfo[iActiveProvider].pDeviceAddr);
				//DirectPlay8AddressCreate(&IID_IDirectPlay8Address, (VOID**)&pSPConnectInfo[iActiveProvider].pDeviceAddr, NULL);
			}
	
	if (pAddrHost != NULL) 
		pAddrHost->Clear();
			else
			{
				CoCreateInstance(CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC, IID_IDirectPlay8Address, (VOID**)&pAddrHost);
				//DirectPlay8AddressCreate(&IID_IDirectPlay8Address, (VOID**)&pAddrHost, NULL);

			}

	//set SP on AddrHost
	pAddrHost->SetSP(&ServiceProviders[iActiveProvider].guid);
	
	//set SP and device GUIDs on the device addr
	pSPConnectInfo[iActiveProvider].pDeviceAddr->SetSP(&ServiceProviders[iActiveProvider].guid);
	pSPConnectInfo[iActiveProvider].pDeviceAddr->SetDevice(&pDeviceArrays[iActiveProvider].SPDevices[pDeviceArrays[iActiveProvider].wActiveDevice].guid);


	//add components depending on service provider guid
	if (ServiceProviders[iActiveProvider].guid == CLSID_DP8SP_SERIAL)
	{
		pSPConnectInfo[iActiveProvider].pDeviceAddr->AddComponent(DPNA_KEY_BAUD, &pSPConnectInfo[iActiveProvider].dwBaud, sizeof(DWORD), DPNA_DATATYPE_DWORD);
		pSPConnectInfo[iActiveProvider].pDeviceAddr->AddComponent(DPNA_KEY_FLOWCONTROL, pSPConnectInfo[iActiveProvider].wcsFlowControl, (wcslen(pSPConnectInfo[iActiveProvider].wcsFlowControl)+1)*2, DPNA_DATATYPE_STRING);
		pSPConnectInfo[iActiveProvider].pDeviceAddr->AddComponent(DPNA_KEY_PARITY, pSPConnectInfo[iActiveProvider].wcsParity, (wcslen(pSPConnectInfo[iActiveProvider].wcsParity)+1)*2, DPNA_DATATYPE_STRING);
		pSPConnectInfo[iActiveProvider].pDeviceAddr->AddComponent(DPNA_KEY_STOPBITS, pSPConnectInfo[iActiveProvider].wcsStopBits, (wcslen(pSPConnectInfo[iActiveProvider].wcsStopBits)+1)*2, DPNA_DATATYPE_STRING);
	}
	if (ServiceProviders[iActiveProvider].guid == CLSID_DP8SP_TCPIP)
	{
		WCHAR* stopstring;
		pSPConnectInfo[iActiveProvider].dwPortIP = wcstol(pSPConnectInfo[iActiveProvider].wcaPortIP, &stopstring, 10);
		pSPConnectInfo[iActiveProvider].pDeviceAddr->AddComponent(DPNA_KEY_HOSTNAME, pSPConnectInfo[iActiveProvider].wcaIP, (wcslen(pSPConnectInfo[iActiveProvider].wcaIP)+1)*2, DPNA_DATATYPE_STRING);
		pSPConnectInfo[iActiveProvider].pDeviceAddr->AddComponent(DPNA_KEY_PORT, &pSPConnectInfo[iActiveProvider].dwPortIP, sizeof(pSPConnectInfo[iActiveProvider].dwPortIP), DPNA_DATATYPE_DWORD);

	}
	//fill in session name in AppDesc
//	LPSTR InText = pCA->pGameOptions->InfoPlayer_Local.pwszName;
	AppDesc.pwszSessionName = pCA->pGameOptions->InfoPlayer_Local.pwszName;
	AppDesc.dwFlags |= DPNSESSION_MIGRATE_HOST;

	//host
	hr20 = pDP8->Host(&AppDesc, &pSPConnectInfo[iActiveProvider].pDeviceAddr, 1, NULL, NULL, NULL, 0);

	//if successful, this computer is host
	if (hr20 == S_OK)
	{
		pCA->pGameOptions->bAmHost = TRUE;
		bConnected = TRUE;
		bUPDRequired = TRUE;
	}


//	KText = DXGetErrorString9(hr20);
//	Write_Log("Host: ");
//	Write_Log((char*)KText);
//	WriteLog_Enter();

	return hr20;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
HRESULT cDP_Multiplay::ConnectSession()
{
HRESULT hr20;
const char* KText;
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	//clear both the hostaddr and the deviceaddr (just in case there were loaded before
//	if (pSPConnectInfo[iActiveProvider].pDeviceAddr != NULL) 
//		pSPConnectInfo[iActiveProvider].pDeviceAddr->Clear();
//			else
//			DirectPlay8AddressCreate(&IID_IDirectPlay8Address, (VOID**)&pSPConnectInfo[iActiveProvider].pDeviceAddr, NULL);
	
//	if (pAddrHost != NULL) 
//		pAddrHost->Clear();
//			else
//			DirectPlay8AddressCreate(&IID_IDirectPlay8Address, (VOID**)&pAddrHost, NULL);

	//set peer info COULD be done here, but it would have to be set again anyway when the peer receives its iControl index from the host(07.18.2002)
	//it WILL be done here, so that GetPeerInfo queries don't come up blank on execution
	PLAYERDATA* pPD = (PLAYERDATA*)pCA->pGameOptions->InfoPlayer_Local.pvData;
	//iControl IDs now set after game start
	//pPD->iControl = 0;

	hr20 = pDP8->SetPeerInfo(&pCA->pGameOptions->InfoPlayer_Local, NULL, NULL, DPNSETPEERINFO_SYNC);
	
//	KText = DXGetErrorString9(hr20);
//	Write_Log("SetPeerInfo: ");
//	Write_Log((char*)KText);
//	WriteLog_Enter();


	//connect from address information of selected host
if (wSelectedHost >= 0)
	hr20 = pDP8->Connect(HostsFound[wSelectedHost].pAppDesc, HostsFound[wSelectedHost].pHostAddr, HostsFound[wSelectedHost].pDeviceAddr, 
					NULL, NULL, NULL,  0, NULL, NULL, NULL, DPNCONNECT_SYNC);    

	//if successful, this computer is NOT host
	if (hr20 == S_OK)
	{
		pCA->pGameOptions->bAmHost = FALSE;
		bConnected = TRUE;
		bUPDRequired = TRUE;
	}


//	KText = DXGetErrorString9(hr20);
//	Write_Log("Connect: ");
//	Write_Log((char*)KText);
//	WriteLog_Enter();				

return hr20;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDP_Multiplay::Update_ConnectionState()   //updates status of Players in connection and places them in the player status arrays
{   //07.18.2002
	
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//wait for statuses in use to free
	while (pCA->pGameOptions->bMPStatusesInUse == TRUE)
	{
		//waiting
		
	}
	pCA->pGameOptions->bMPStatusesInUse = TRUE;

	DPNID* pPlayerIDArray = DPN_Static_ID_Array;
	DWORD	NumPlayers = 0;
	HRESULT hr20;
	DPN_PLAYER_INFO* pRecvBuffer = NULL;
	PLAYERDATA*	pPLRecvBuffer = NULL;
	DWORD*	pRB = NULL;
	PLAYERDATA* pPlayerData = NULL;
	int lc1, lc2; //local counters
	const char* KText;
	char NText[50];

//	TestBuffer.dwSize = sizeof(DPN_PLAYER_INFO);   //defeat the allocation problem by using a static buffer  (07.20.2002)

	//enumerate to get the buffersize
	hr20 = pDP8->EnumPlayersAndGroups(pPlayerIDArray, &NumPlayers, DPNENUM_PLAYERS);
//KText = DXGetErrorString9(hr20);
//Write_Log("EnumPlayers: ");
//Write_Log((char*)KText);
//WriteLog_Enter();

	//keep calling until it is no longer buffertoosmall
	while(hr20 == DPNERR_BUFFERTOOSMALL)
	{
		//delete the previous buffer and reallocate
		//delete(pPlayerIDArray); //don't de-allocate...if the program works correctly, we will never get into this loop, and we will get a crash if we try to de-allocate the static array
		pPlayerIDArray = new DPNID[NumPlayers];
		//call again
		hr20 = pDP8->EnumPlayersAndGroups(pPlayerIDArray, &NumPlayers, DPNENUM_PLAYERS);
//	KText = DXGetErrorString9(hr20);
//	Write_Log("EnumPlayers: ");
//	Write_Log((char*)KText);
//	WriteLog_Enter();
	
	}

	//set the number of players found
	pCA->pGameOptions->iNumPlayers = NumPlayers;

	//(I can't figure out any other way to get the host's own DPNID other than to do this...)
	//if you are the only player in the session, you are the host and the enumerated DPNID is yours
	if (NumPlayers == 1)
	{
		pCA->pGameOptions->DPNID_Host = pPlayerIDArray[0];
		pCA->pGameOptions->DPNID_Local = pPlayerIDArray[0];
		char NText[20];
		ultoa(pPlayerIDArray[0], NText, 10);
		char Text[100];
		Text[0] = 0;
		strcat(Text, "Host has discovered own ID as: ");
		strcat(Text, NText);
		pCA->pChatBuffer->Add_CBNode(Text, 1000);
	}

	//clear playerlist of existing entries
	pCA->pGameOptions->Clear_PlayerList(pCA->pGameOptions->pPD_First, pCA->pGameOptions->pPD_Last);

	//now get peer info for each of these players
	for (lc1 = 0; lc1 < NumPlayers; lc1++)
	{
//DEBUG
itoa(NumPlayers, NText, 10);
Write_Log("Number of Players: ");
Write_Log(NText);
WriteLog_Enter();

	DWORD RBS = 0;
	
	//get peer info appears to be VERY finicky with the pdwsize and pbuffer parameters it receives, and so far has only worked reliably if DWORD RBS is re-declared for EACH player(07.20.2002)
	hr20 = pDP8->GetPeerInfo(pPlayerIDArray[lc1], NULL, &RBS, 0);//pdwRecvBufferSize, 0); //let's try setting the testbuffer to NULL the first time and see if that works :-( 07.20.2002
//	KText = DXGetErrorString9(hr20);
//	Write_Log("GetPeerInfo: ");
//	Write_Log((char*)KText);
//	WriteLog_Enter();

		//re-allocate, call again
		//delete(pRecvBuffer);
		pRecvBuffer = (DPN_PLAYER_INFO*)new BYTE[RBS];
		pRecvBuffer->dwSize = sizeof(DPN_PLAYER_INFO);
		//call again
		hr20 = pDP8->GetPeerInfo(pPlayerIDArray[lc1], pRecvBuffer, &RBS, 0);

//	KText = DXGetErrorString9(hr20);
//	Write_Log("GetPeerInfo: ");
//	Write_Log((char*)KText);
//	WriteLog_Enter();
		//copy the information 
		pPLRecvBuffer = (PLAYERDATA*)pRecvBuffer->pvData;
	
			//DEBUG
			itoa(pPLRecvBuffer->iDPNID, NText, 10);
			Write_Log("Buffer DPNID: ");
			Write_Log(NText);
			WriteLog_Enter();
			//HACK. I don't know why this is happening, but it appears that Get Peer info is not setting the DPNID...oddly
			//so try setting it manually (08.12.2003)
			pPLRecvBuffer->iDPNID = pPlayerIDArray[lc1];


		//add the contained information to the player list
		//apparently limit the name length to 80 characters if exceeded
		if (wcslen(pRecvBuffer->pwszName) <= TEMP_STD_STRLEN)
		{

			pCA->pGameOptions->Add_Player(pRecvBuffer->pwszName, 
										  wcslen(pRecvBuffer->pwszName), 
										  pPLRecvBuffer->iControl_Requested,
										  pPLRecvBuffer->iTeam_Requested,
										  pPLRecvBuffer->iDPNID);
		}
				else
			pCA->pGameOptions->Add_Player(pRecvBuffer->pwszName, 
										  TEMP_STD_STRLEN, 
										  pPLRecvBuffer->iControl_Requested,
										  pPLRecvBuffer->iTeam_Requested,
										  pPLRecvBuffer->iDPNID);
		//copy over the name if only 1 player found
		if (NumPlayers == 1)
		{
			char* pNotWideCharName = pCA->pInterface->Conv_String_WideToReg2(reinterpret_cast<unsigned short*>(pRecvBuffer->pwszName));
			pCA->pGameOptions->strName[0] = 0;
			strcat(pCA->pGameOptions->strName, pNotWideCharName);
			delete [] pNotWideCharName;
		}


		//delete the recvbuffer
		delete(pRecvBuffer);

	}

	//refresh the scratch info table
	//clear the TEMP table of existing player entries (08.09.2003)
	for (lc1 = 0; lc1 < TEMP_DP_MAXPLAYERS; lc1++)
	{
		pCA->pGameOptions->ScratchInfoTable[lc1] = NULL;
	}
	PDNODE* pPD_Trav2 = (PDNODE*)pCA->pGameOptions->pPD_AIFirst->pNext;

lc1 = 0;
	while (pPD_Trav2 != pCA->pGameOptions->pPD_AILast)
	{
		pCA->pGameOptions->ScratchInfoTable[lc1] = pPD_Trav2;
		pPD_Trav2 = (PDNODE*)pPD_Trav2->pNext;
		lc1++;
	}
	pPD_Trav2 = (PDNODE*)pCA->pGameOptions->pPD_First->pNext;
	while (pPD_Trav2 != pCA->pGameOptions->pPD_Last)
	{
//DEBUG
Write_Log("[Update ConnectionState]Non-AI player added.");
WriteLog_Enter();
		pCA->pGameOptions->ScratchInfoTable[lc1] = pPD_Trav2;

		//if this is YOU, then identify whether YOU have the map
		if (pCA->pGameOptions->DPNID_Local == pCA->pGameOptions->ScratchInfoTable[lc1]->iDPNID)
		{
			pCA->pGameOptions->ScratchInfoTable[lc1]->bHaveCurrentMap = pCA->pGameOptions->bMapLocationFinalized;
		}

			//DEBUG
			itoa(pCA->pGameOptions->ScratchInfoTable[lc1]->iDPNID, NText, 10);
			Write_Log("Scratch DPNID: ");
			Write_Log(NText);
			itoa(pPD_Trav2->iDPNID, NText, 10);
			Write_Log(" Traverser DPNID: ");
			Write_Log(NText);
			WriteLog_Enter();
		pPD_Trav2 = (PDNODE*)pPD_Trav2->pNext;		
		lc1++;
	}

	//now release
	pCA->pGameOptions->bMPStatusesInUse = FALSE;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDP_Multiplay::AssignIDs_Host()
{
	//get a list of DPNIDs, and for now, simply pass them out in the order received(12.23.2002)
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	DPNID* pPlayerIDArray = (DPNID*)new BYTE[sizeof(DPNID)*1];
	DWORD NumPlayers = 0;

	char NText[50];


	//enumerate to get the buffersize
	HRESULT hr21 = pDP8->EnumPlayersAndGroups(pPlayerIDArray, &NumPlayers, DPNENUM_PLAYERS);
	//keep calling until it is no longer buffertoosmall
	while(hr21 == DPNERR_BUFFERTOOSMALL)
	{

		//delete the previous buffer and reallocate
		delete(pPlayerIDArray);
		pPlayerIDArray = (DPNID*)new BYTE[sizeof(DPNID)*NumPlayers];
		//call again
		hr21 = pDP8->EnumPlayersAndGroups(pPlayerIDArray, &NumPlayers, DPNENUM_PLAYERS);
	}

//sigh...I can't think of a better way to do this than to first go through the AI list, find the number of players,
//then issue that count before we start sending nation_init commands (08.12.2003)
	PDNODE* pPD_Trav1 = (PDNODE*)pCA->pGameOptions->pPD_AIFirst->pNext;

int lc1 = 0;  //counts the total number of AI players
	while (pPD_Trav1 != pCA->pGameOptions->pPD_AILast)
	{
		lc1++;
		pPD_Trav1 = (PDNODE*)pPD_Trav1->pNext;		
	}

	//now issue the total player count
	Send_Command(CQDEST_INTERFACE, CQTYPE_SET_TOTAL_PLAYER_COUNT, lc1+ NumPlayers, 0, 0, 0, 0, NULL, 0, TRUE, TRUE);
	//issue the total timerID count (TOTAL # of players MINUS the # of AI players (lc1) )
	Send_Command(CQDEST_INTERFACE, CQTYPE_SET_TOTAL_TIMERID_COUNT, NumPlayers, 0, 0, 0, 0, NULL, 0, TRUE, TRUE);
	
	//now hand out IDs (for now, with no AI currently, do not hand AI players IDs, simply increment the control index)
int iControl_Assign = 0;
int iTimerID_Assign = 0;  //for now, keep timerID the same as iControl
	pPD_Trav1 = (PDNODE*)pCA->pGameOptions->pPD_AIFirst->pNext;

int iRetAssemble;

	//now send out IDs to players
lc1 = 0;
int lc2 = 0;
PDNODE* pInfo = NULL;

//TEST
PDNODE PDTEST;
PDTEST.wcsName = new WCHAR[80];
ZeroMemory(PDTEST.wcsName, sizeof(WCHAR) * 80);
wcscat(PDTEST.wcsName, pCA->pGameOptions->InfoPlayer_Local.pwszName);

	//note, as of 07.19.2007 the ID control being sent here is only important as in initial value(the player chooses his ID later)
	for (lc1 = 0; lc1 < NumPlayers; lc1++)
	{

		int iData[2];
		iData[0] = iControl_Assign;
		iData[1] = iTimerID_Assign;
		Send_Command2(pPlayerIDArray[lc1], CQ2TYPE_SET_IDCONTROLANDTIMER, (BYTE*)&iData, sizeof(int)*2, FALSE, TRUE);

		//Send_Command_Directed(pPlayerIDArray[lc1], CQDEST_INTERFACE, CQTYPE_SET_IDCONTROLANDTIMER, iControl_Assign, iTimerID_Assign, 0, 0, 0, NULL, 0, TRUE);

		//add the corresponding nation (08.09.2003)
		//find the national info for this DPNID
		for(lc2 = 0; lc2 < TEMP_DP_MAXPLAYERS; lc2++)
		{
			if (pCA->pGameOptions->ScratchInfoTable[lc2] != NULL)
			{

			//DEBUG
			itoa(pCA->pGameOptions->ScratchInfoTable[lc2]->iDPNID, NText, 10);
			Write_Log("Scratch DPNID: ");
			Write_Log(NText);
			itoa(pPlayerIDArray[lc1], NText, 10);
			Write_Log(" Player Array DPNID: ");
			Write_Log(NText);
			WriteLog_Enter();

			if (pCA->pGameOptions->ScratchInfoTable[lc2]->iDPNID == pPlayerIDArray[lc1])
				pInfo = pCA->pGameOptions->ScratchInfoTable[lc2];

			}
		}
		//
		if (pInfo == NULL)
		{
		
			pInfo = &PDTEST;
			Write_Log("[Assign IDs] No Match.");
			WriteLog_Enter();
		}

		char* Text = pCA->pInterface->Conv_String_WideToReg2(reinterpret_cast<unsigned short*>(pInfo->wcsName));

		//send out "init timerID"
		int iretsize = 0;
		PCMD2* pCmd2 = new PCMD2;
		pCmd2->wType = CQ2TYPE_INIT_TIMERID;
		pCmd2->pData = pCA->pCommQueue->Assemble_CommData_Init_TimerID(iTimerID_Assign, pPlayerIDArray[lc1], strlen(Text), Text, &iretsize);
		pCmd2->wDataSize = iretsize;
		Send_Command2(pCmd2->wType, pCmd2->pData, pCmd2->wDataSize, TRUE, TRUE);
		delete(pCmd2->pData);
		delete(pCmd2);

		//because NationManager copies its strings, delete Text
//		delete(Text);

		iControl_Assign++;
		iTimerID_Assign++;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
HRESULT cDP_Multiplay::CloseSession()
{
HRESULT hr20;
	if (bConnected == TRUE)
	{
		hr20 = pDP8->Close(0);
		bConnected = FALSE;

	}

	//then re-intiialize
	hr20 = pDP8->Initialize(NULL, DP8_Handle_Message, 0);

return hr20;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDP_Multiplay::Process_StaticBuffer()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

int lc1 = 0;
	while( bStaticBufferInUse == TRUE)
	{
		Write_Log("Waiting_Static_Buffer");
		WriteLog_Enter();
		lc1++;

		if (lc1 > 100000)
		{
			pCA->pChatBuffer->Add_CBNode("Process_StaticBuffer has been in loop for long time. Function aborted", 1000);
			return;
		}
	}
	bStaticBufferInUse = TRUE;

	//issue commands
	for (lc1 = 0; lc1 < iNumMessagesInBuffer; lc1++)
	{
		pCA->pCommQueue->Issue_Command_Q2(StaticMsgTypeBuffer[lc1], StaticMsgBuffer + lc1*TEMP_DP_SMBMAXSIZE, StaticMsgSizeBuffer[lc1], TRUE);
		StaticMsgTypeBuffer[lc1] = VAL_INVALID;
		StaticMsgSizeBuffer[lc1] = VAL_INVALID;

	}

	//reset number of messages
	iBufferTrace = iNumMessagesInBuffer;
	iNumMessagesInBuffer = 0;

	//release lock on buffer
	bStaticBufferInUse = FALSE;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//obsolete code. moved here 08.09.2003
//		ZeroMemory(&HostsFound[lc8], sizeof(DPNMSG_ENUM_HOSTS_RESPONSE));

/*

	//06.27.2002 Create the DirectPlay object
	hr20 = DirectPlay8Create(&IID_IDirectPlay8Peer, (VOID**)&pDP8, NULL);
	
	if (hr20 != DP_OK)
	{
		Write_Log("Error at Create");
		WriteLog_Enter();
	}
	if (hr20 == DP_OK)
	{
		Write_Log("Success at Create");
		WriteLog_Enter();
	}
char NText[50];

	//initialize
	hr20 = pDP8->Initialize(&wUserContext, DP8_Handle_Message, 0);

	KText = DXGetErrorString9(hr20);
	Write_Log("Initialize: ");
	Write_Log((char*)KText);
	WriteLog_Enter();


	//enumerate service providers (to get the right size)
	ServiceProviders = new DPN_SERVICE_PROVIDER_INFO
	hr20 = pDP8->EnumServiceProviders(NULL, NULL, ServiceProviders, &dwSPBufferSize, &dwNumProviders, DPNENUMSERVICEPROVIDERS_ALL);

	KText = DXGetErrorString9(hr20);
	Write_Log("EnumServiceProviders1: ");
	Write_Log((char*)KText);
	WriteLog_Enter();
	Write_Log("Correct Buffer Size: ");
	itoa(dwSPBufferSize, NText, 10);
	Write_Log(NText);
	WriteLog_Enter();

	//
	delete(ServiceProviders);
	//re-allocate
	ServiceProviders = (DPN_SERVICE_PROVIDER_INFO*)BYTE[dwSPBufferSize];
	//call again
	hr20 = pDP8->EnumServiceProviders(NULL, NULL, ServiceProviders, &dwSPBufferSize, &dwNumProviders, DPNENUMSERVICEPROVIDERS_ALL);

	KText = DXGetErrorString9(hr20);
	Write_Log("EnumServiceProviders2: ");
	Write_Log((char*)KText);
	WriteLog_Enter();

	for (int lc5 = 0; lc5 < dwNumProviders; lc5++)
	{
		char* MText;
		
		Write_Log(" Service Provider ");
		itoa(lc5, NText, 10);
		Write_Log(NText);
		Write_Log(" : ");
		MText = Write_Log_Special(ServiceProviders[lc5].pwszName);
		WriteLog_Enter();
		//this is a rather cheap(and possibly not completely fool-proof way of searching for the serial providers, but it works for now)
//		if(MText[12] == 'S' && MText[13] == 'e')// == "DirectPlay8 Serial Service Provider")
//			iActiveProvider = lc5;
		if (lc5 == 3)
			iActiveProvider = lc5;
	}

	Write_Log("Active Provider Index: ");
	itoa((int)iActiveProvider, NText, 10);
	Write_Log(NText);
	WriteLog_Enter();

	//call EnumSP again to get devices (actually two more times, to get right buffer size, and then get the device data
	SPDevices = (DPN_SERVICE_PROVIDER_INFO*)malloc(0);
	hr20 = pDP8->EnumServiceProviders(&ServiceProviders[iActiveProvider].guid, NULL, SPDevices, &dwDeviceBufferSize, &dwNumDevices, 0);

	KText = DXGetErrorString9(hr20);
	Write_Log("EnumSPDevices1: ");
	Write_Log((char*)KText);
	WriteLog_Enter();
	Write_Log("Correct Buffer Size: ");
	itoa(dwDeviceBufferSize, NText, 10);
	Write_Log(NText);
	WriteLog_Enter();

	//delete and re-allocate
	delete(SPDevices);
	SPDevices = (DPN_SERVICE_PROVIDER_INFO*)new BYTE[dwDeviceBufferSize];

	//call EnumSP again
	hr20 = pDP8->EnumServiceProviders(&ServiceProviders[iActiveProvider].guid, NULL, SPDevices, &dwDeviceBufferSize, &dwNumDevices, 0);

	KText = DXGetErrorString9(hr20);
	Write_Log("EnumSPDevices1: ");
	Write_Log((char*)KText);
	WriteLog_Enter();

	//write out devices again, select the first (hard -code)
	for (int lc6 = 0; lc6 < dwNumDevices; lc6++)
	{
		char* MText;
		
		Write_Log(" Device ");
		itoa(lc6, NText, 10);
		Write_Log(NText);
		Write_Log(" : ");
		MText = Write_Log_Special(SPDevices[lc6].pwszName);
		WriteLog_Enter();
		//this is a rather cheap(and possibly not completely fool-proof way of searching for the serial providers, but it works for now)
//		if(MText[12] == 'S' && MText[13] == 'e')// == "DirectPlay8 Serial Service Provider")
//			iActiveProvider = lc5;
		if (lc6 == 0)
			wActiveDevice = lc6;
	}

	Write_Log("Active Device Index: ");
	itoa((int)wActiveDevice, NText, 10);
	Write_Log(NText);
	WriteLog_Enter();

DPN_APPLICATION_DESC AppDesc;
DWORD buffersize = 0;

    ZeroMemory( &AppDesc, sizeof(DPN_APPLICATION_DESC) );
	AppDesc.dwSize = sizeof(DPN_APPLICATION_DESC);
	AppDesc.guidApplication = APP_A1_GUID;

IDirectPlay8Address* pDP8Addr1; 

	DirectPlay8AddressCreate(&IID_IDirectPlay8Address, (VOID**)&pDP8Addr1, NULL);
	DirectPlay8AddressCreate(&IID_IDirectPlay8Address, (VOID**)&pDP8AddrHost, NULL);
	

//for now, with lack of UI and internet connection, use the serial connection
	  hr20 = pDP8Addr1->SetSP(&ServiceProviders[iActiveProvider].guid);
	  //hard coded
	  hr20 = pDP8Addr1->SetDevice(&SPDevices[wActiveDevice].guid);

	 Write_Log("Set Device: ");
	KText = DXGetErrorString9(hr20);
	Write_Log((char*)KText);
	WriteLog_Enter();

DWORD dwBaud = 57600;
	  hr20 = pDP8Addr1->AddComponent(DPNA_KEY_BAUD, &dwBaud, sizeof(DWORD), DPNA_DATATYPE_DWORD);

	 Write_Log("Set Baudrate: ");
	KText = DXGetErrorString9(hr20);
	Write_Log((char*)KText);
	WriteLog_Enter();

WCHAR* wcsInString = DPNA_FLOW_CONTROL_RTSDTR;
	  
	//+1 to the string length to include the null termination
	  hr20 = pDP8Addr1->AddComponent(DPNA_KEY_FLOWCONTROL, wcsInString, (wcslen(wcsInString)+1) * 2, DPNA_DATATYPE_STRING);
	  
    Write_Log("Set Flow Control: ");
	KText = DXGetErrorString9(hr20);
	Write_Log((char*)KText);
	WriteLog_Enter();

	wcsInString = DPNA_PARITY_NONE;

	  hr20 = pDP8Addr1->AddComponent(DPNA_KEY_PARITY, wcsInString, (wcslen(wcsInString)+1) * 2, DPNA_DATATYPE_STRING);

	 Write_Log("Set Parity: ");
	KText = DXGetErrorString9(hr20);
	Write_Log((char*)KText);
	WriteLog_Enter();

wcsInString = DPNA_STOP_BITS_ONE;

	  hr20 = pDP8Addr1->AddComponent(DPNA_KEY_STOPBITS, wcsInString, (wcslen(wcsInString)+1) * 2, DPNA_DATATYPE_STRING);

	 Write_Log("Set Stop Bits: ");
	KText = DXGetErrorString9(hr20);
	Write_Log((char*)KText);
	WriteLog_Enter();

	  hr20 = pDP8AddrHost->SetSP(&ServiceProviders[iActiveProvider].guid);
	  
//dead in the water because I don't know how to set device data!
//try accessing registry to find device GUID (never mind....at lesat for now (06.30.2002)


  	Write_Log("SetSP: ");
	KText = DXGetErrorString9(hr20);
	Write_Log((char*)KText);
	WriteLog_Enter();

//
//	char* SndBufferData = new char[5];

DPNHANDLE hTestAsync;

	//enumerate hosts.....if no hosts can be found, begin hosting...
	hr20 = pDP8->EnumHosts(&AppDesc, pDP8AddrHost, pDP8Addr1, NULL, 0, 0, 0, 0, NULL, NULL, DPNENUMHOSTS_SYNC);// | DPNENUMHOSTS_OKTOQUERYFORADDRESSING);//&hTestAsync, DPNENUMHOSTS_OKTOQUERYFORADDRESSING);

	if (hr20 == DPNSUCCESS_PENDING)
		Write_Log("RK");

	Write_Log("EnumHosts: ");
	KText = DXGetErrorString9(hr20);
	Write_Log((char*)KText);
	WriteLog_Enter();

	Write_Log("Hosts Found: ");
	itoa(wHostsFound, NText, 10);
	Write_Log(NText);
	WriteLog_Enter();
//
	//if no hosts..HOST!
	if (wHostsFound == 0)
	{
//		pDP8->CancelAsyncOperation(NULL, DPNCANCEL_ENUM);
		
		//setup the application description
		LPSTR SessionName = "S a m ' s   F i r s t   A t t e m p t   a t   a   S e s s i o n";
		AppDesc.pwszSessionName = (unsigned short *)SessionName;
		AppDesc.dwMaxPlayers = 3;
		AppDesc.dwFlags = 0;

		hr20 = pDP8->Host(&AppDesc, &pDP8Addr1, 1, NULL, NULL, NULL, DPNHOST_OKTOQUERYFORADDRESSING);
	 	Write_Log("Host: ");
		KText = DXGetErrorString9(hr20);
		Write_Log((char*)KText);
		WriteLog_Enter();

		//set Host Flag
		pCA2->pGameOptions->bAmHost = TRUE;



	}
	if (wHostsFound == 1) //one host found ... temp for now until some kind of way to select hosts can be setup (06.30.2002)
	{
		//test what is in hosts found
		Write_Log("Joining Session Name: ");
		Write_Log_Special(HostsFound[0].pAppDesc->pwszSessionName);
		WriteLog_Enter();

		hr20 = pDP8->Connect(HostsFound[0].pAppDesc, HostsFound[0].pHostAddr, HostsFound[0].pDeviceAddr, NULL,
			NULL, NULL, 0, NULL, NULL, NULL, DPNCONNECT_SYNC | DPNCONNECT_OKTOQUERYFORADDRESSING);  


	 	
		Write_Log("Join: ");
		KText = DXGetErrorString9(hr20);
		Write_Log((char*)KText);
		WriteLog_Enter();

		//set iControl(since you're joining a game...hard code the setting to 1(07.01.2002)
		pCA2->pGameOptions->iControl = 1;

		//set host flag
		pCA2->pGameOptions->bAmHost = FALSE;

	}
*/