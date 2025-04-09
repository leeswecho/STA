#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDI_Input::Init(HINSTANCE hInst_App, HWND hMainWnd)
{
if (DEBUG + DEBUG_INPUTDI >= 1)
	Init_LogFile();

	//create the Direct Input object
	DirectInput8Create(hInst_App, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID **)&pDI8, NULL);
	
	//create keyboard and mouse devices
	pDI8->CreateDevice(GUID_SysKeyboard, &pDID8_Keyboard, NULL);
	pDI8->CreateDevice(GUID_SysMouse, &pDID8_Mouse, NULL);
	
	//set cooperative levels for keyboard and mouse
	//set the keyboard level to NONEXCLUSIVE for now so that we can receive windows keymessages!
	pDID8_Mouse->SetCooperativeLevel(hMainWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	pDID8_Keyboard->SetCooperativeLevel(hMainWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	//set data formats for keyboard and mouse
	pDID8_Mouse->SetDataFormat(&c_dfDIMouse2);
	pDID8_Keyboard->SetDataFormat(&c_dfDIKeyboard);

	//acquire both devices
	pDID8_Mouse->Acquire();
	pDID8_Keyboard->Acquire();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDI_Input::Get_MouseState()
{

	//set the "current" state to the previous state
	DIState_Mouse_Prev.lX = DIState_Mouse_Curr.lX;
	DIState_Mouse_Prev.lY = DIState_Mouse_Curr.lY;
	DIState_Mouse_Prev.lZ = DIState_Mouse_Curr.lZ;



	for (c1 = 0; c1 <= 7; c1++)
		DIState_Mouse_Prev.rgbButtons[c1] = DIState_Mouse_Curr.rgbButtons[c1];

	//get the new "current" state
	HRESULT hr1 = pDID8_Mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &DIState_Mouse_Curr);

	if (hr1 == DIERR_INPUTLOST)
	{
		pDID8_Mouse->Acquire();
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDI_Input::Get_KeyboardState()
{
	
	//set the "current" state to the previous state
	for (c1 = 0; c1 <= 255; c1++)
		DIState_Keyboard_Prev[c1] = DIState_Keyboard_Curr[c1];



	//get the new "current" state (size of keyboard array is 256 bytes)
	HRESULT hr1 = pDID8_Keyboard->GetDeviceState(256, &DIState_Keyboard_Curr);


	if (hr1 == DIERR_INPUTLOST)
	{
		pDID8_Keyboard->Acquire();
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDI_Input::Get_InputState()
{

	Get_MouseState();
	Get_KeyboardState();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cDI_Input::~cDI_Input()
{

/*  moved to exit() 07.03.2002
//unacquire both devices
	pDID8_Mouse->Unacquire();
	pDID8_Keyboard->Unacquire();

//release both devices, set to NULL;
	pDID8_Mouse->Release();
	pDID8_Keyboard->Release();

	pDID8_Mouse = NULL;
	pDID8_Keyboard = NULL;
*/
}

//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDI_Input::Exit()
{
//unacquire both devices
	pDID8_Mouse->Unacquire();
	pDID8_Keyboard->Unacquire();

//release both devices, set to NULL;
	pDID8_Mouse->Release();
	pDID8_Keyboard->Release();

	pDID8_Mouse = NULL;
	pDID8_Keyboard = NULL;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDI_Input::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cDI_Input.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDI_Input::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_INPUTDI >= 1)
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
void cDI_Input::WriteLog_Enter()
{
if (DEBUG + DEBUG_INPUTDI >= 1)
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

