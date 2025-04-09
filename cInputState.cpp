#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInputState::Init(VOID* pCA)
{
	//get the pointer to the DI class 
	pCAccess = pCA;

	iDrawFrame = 0;

	IGState_Mouse.iXPos = 100;
	IGState_Mouse.iYPos = 100;

	//init the translation table
	for (int lc12 = 0; lc12 < 256; lc12++)
	{
		DI_ANSI_Table[lc12] = 0;
		DI_ANSI_Table_Shift[lc12] = 0;
	}

	DI_ANSI_Table[DIK_A] = 'a';
	DI_ANSI_Table[DIK_B] = 'b';
	DI_ANSI_Table[DIK_C] = 'c';
	DI_ANSI_Table[DIK_D] = 'd';
	DI_ANSI_Table[DIK_E] = 'e';
	DI_ANSI_Table[DIK_F] = 'f';
	DI_ANSI_Table[DIK_G] = 'g';
	DI_ANSI_Table[DIK_H] = 'h';
	DI_ANSI_Table[DIK_I] = 'i';
	DI_ANSI_Table[DIK_J] = 'j';
	DI_ANSI_Table[DIK_K] = 'k';
	DI_ANSI_Table[DIK_L] = 'l';
	DI_ANSI_Table[DIK_M] = 'm';
	DI_ANSI_Table[DIK_N] = 'n';
	DI_ANSI_Table[DIK_O] = 'o';
	DI_ANSI_Table[DIK_P] = 'p';
	DI_ANSI_Table[DIK_Q] = 'q';
	DI_ANSI_Table[DIK_R] = 'r';
	DI_ANSI_Table[DIK_S] = 's';
	DI_ANSI_Table[DIK_T] = 't';
	DI_ANSI_Table[DIK_U] = 'u';
	DI_ANSI_Table[DIK_V] = 'v';
	DI_ANSI_Table[DIK_W] = 'w';
	DI_ANSI_Table[DIK_X] = 'x';
	DI_ANSI_Table[DIK_Y] = 'y';
	DI_ANSI_Table[DIK_Z] = 'z';
	DI_ANSI_Table[DIK_1] = '1';
	DI_ANSI_Table[DIK_2] = '2';
	DI_ANSI_Table[DIK_3] = '3';
	DI_ANSI_Table[DIK_4] = '4';
	DI_ANSI_Table[DIK_5] = '5';
	DI_ANSI_Table[DIK_6] = '6';
	DI_ANSI_Table[DIK_7] = '7';
	DI_ANSI_Table[DIK_8] = '8';
	DI_ANSI_Table[DIK_9] = '9';
	DI_ANSI_Table[DIK_0] = '0';
	DI_ANSI_Table[DIK_SPACE] = ' ';
	DI_ANSI_Table[DIK_MINUS] = '-';
	DI_ANSI_Table[DIK_SLASH] = '/';
	DI_ANSI_Table[DIK_LBRACKET] = '[';
	DI_ANSI_Table[DIK_RBRACKET] = ']';
	DI_ANSI_Table[DIK_SEMICOLON] = ';';
	DI_ANSI_Table[DIK_APOSTROPHE] = 39;
	DI_ANSI_Table[DIK_PERIOD] = '.';
	DI_ANSI_Table[DIK_COMMA] = ',';


	DI_ANSI_Table_Shift[DIK_A] = 'A';
	DI_ANSI_Table_Shift[DIK_B] = 'B';
	DI_ANSI_Table_Shift[DIK_C] = 'C';
	DI_ANSI_Table_Shift[DIK_D] = 'D';
	DI_ANSI_Table_Shift[DIK_E] = 'E';
	DI_ANSI_Table_Shift[DIK_F] = 'F';
	DI_ANSI_Table_Shift[DIK_G] = 'G';
	DI_ANSI_Table_Shift[DIK_H] = 'H';
	DI_ANSI_Table_Shift[DIK_I] = 'I';
	DI_ANSI_Table_Shift[DIK_J] = 'J';
	DI_ANSI_Table_Shift[DIK_K] = 'K';
	DI_ANSI_Table_Shift[DIK_L] = 'L';
	DI_ANSI_Table_Shift[DIK_M] = 'M';
	DI_ANSI_Table_Shift[DIK_N] = 'N';
	DI_ANSI_Table_Shift[DIK_O] = 'O';
	DI_ANSI_Table_Shift[DIK_P] = 'P';
	DI_ANSI_Table_Shift[DIK_Q] = 'Q';
	DI_ANSI_Table_Shift[DIK_R] = 'R';
	DI_ANSI_Table_Shift[DIK_S] = 'S';
	DI_ANSI_Table_Shift[DIK_T] = 'T';
	DI_ANSI_Table_Shift[DIK_U] = 'U';
	DI_ANSI_Table_Shift[DIK_V] = 'V';
	DI_ANSI_Table_Shift[DIK_W] = 'W';
	DI_ANSI_Table_Shift[DIK_X] = 'X';
	DI_ANSI_Table_Shift[DIK_Y] = 'Y';
	DI_ANSI_Table_Shift[DIK_Z] = 'Z';
	DI_ANSI_Table_Shift[DIK_1] = '!';
	DI_ANSI_Table_Shift[DIK_2] = '@';
	DI_ANSI_Table_Shift[DIK_3] = '#';
	DI_ANSI_Table_Shift[DIK_4] = '$';
	DI_ANSI_Table_Shift[DIK_5] = '%';
	DI_ANSI_Table_Shift[DIK_6] = '^';
	DI_ANSI_Table_Shift[DIK_7] = '&';
	DI_ANSI_Table_Shift[DIK_8] = '*';
	DI_ANSI_Table_Shift[DIK_9] = '(';
	DI_ANSI_Table_Shift[DIK_0] = ')';
	DI_ANSI_Table_Shift[DIK_MINUS] = '_';
	DI_ANSI_Table_Shift[DIK_SLASH] = '?';
	DI_ANSI_Table_Shift[DIK_LBRACKET] = '{';
	DI_ANSI_Table_Shift[DIK_RBRACKET] = '}';
	DI_ANSI_Table_Shift[DIK_SEMICOLON] = ':';
	DI_ANSI_Table_Shift[DIK_APOSTROPHE] = 34;
	DI_ANSI_Table_Shift[DIK_PERIOD] = '>';
	DI_ANSI_Table_Shift[DIK_COMMA] = '<';

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cInputState::~cInputState()
{

	//nothing to do....we never allocated anything!



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInputState::Update_IGStates()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

//FIGURE OUT MOUSE IGSTATE////////////////////////////////////////////////////////////////
	//update the mouse states first
	//each of the buttons
	for (c1 = 0; c1 <= 2; c1++)
	{
		//by default assume OFF status
		IGState_Mouse.Buttons[c1] = 0;
		//check the current state to check on/off status
		if (pCA->pDI_Input->DIState_Mouse_Curr.rgbButtons[c1] > 0)
			IGState_Mouse.Buttons[c1] = 1;

		//check for buttondown status
		if (pCA->pDI_Input->DIState_Mouse_Curr.rgbButtons[c1] > 0 && pCA->pDI_Input->DIState_Mouse_Prev.rgbButtons[c1] == 0)
			IGState_Mouse.Buttons[c1] = 2;
		//check for buttonup status
		if (pCA->pDI_Input->DIState_Mouse_Curr.rgbButtons[c1] == 0 && pCA->pDI_Input->DIState_Mouse_Prev.rgbButtons[c1] > 0)
			IGState_Mouse.Buttons[c1] = 3;
	}

	//mouse wheel. (my machine's mouse gives me -120's and 120's ... so....)
		IGState_Mouse.iWheel = -pCA->pDI_Input->DIState_Mouse_Curr.lZ / 100;

//DEBUG
if (IGState_Mouse.iWheel != 0)
{
char NText[50];
itoa(IGState_Mouse.iWheel, NText, 10);
pCA->pConfig->Write_Log(NText);
pCA->pConfig->WriteLog_Enter();
}

	//update the position of the cursor
	IGState_Mouse.iXPos = IGState_Mouse.iXPos + pCA->pDI_Input->DIState_Mouse_Curr.lX;
	IGState_Mouse.iYPos = IGState_Mouse.iYPos + pCA->pDI_Input->DIState_Mouse_Curr.lY;
	//make sure the cursor does not stray outside of screen dimensions
	if (IGState_Mouse.iXPos > pCA->pDD_Display->i_XResD) //- CURSOR_SIZE)
		IGState_Mouse.iXPos = pCA->pDD_Display->i_XResD; //- CURSOR_SIZE;
	if (IGState_Mouse.iXPos < 0)
		IGState_Mouse.iXPos = 0;
	if (IGState_Mouse.iYPos > pCA->pDD_Display->i_YResD) //- CURSOR_SIZE)
		IGState_Mouse.iYPos = pCA->pDD_Display->i_YResD; //- CURSOR_SIZE;
	if (IGState_Mouse.iYPos < 0)
		IGState_Mouse.iYPos = 0;
	
//FIGURE OUT KEYBOARD IGSTATE////////////////////////////////////////////////////////////////
	//now do the keyboard
	//for each of the keys, poll for key on, key up, and key down situations
	for (c1 = 0; c1 <= 255;  c1++)
	{
		//by default assume OFF states
		IGState_Keyboard[c1] = 0;
		//check for on/off status
		if (pCA->pDI_Input->DIState_Keyboard_Curr[c1] > 0)
			IGState_Keyboard[c1] = 1;
	
		//check for keydown status
		if (pCA->pDI_Input->DIState_Keyboard_Curr[c1] > 0 && pCA->pDI_Input->DIState_Keyboard_Prev[c1] == 0)
			IGState_Keyboard[c1] = 2;
		//check for keyup status
		if (pCA->pDI_Input->DIState_Keyboard_Curr[c1] == 0 && pCA->pDI_Input->DIState_Keyboard_Prev[c1] > 0)
			IGState_Keyboard[c1] = 3;
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

		