#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Init(VOID* pCA)
{

	//store the class access pointer
	pCAccess = pCA;

	cClassAccess* pCA2 = (cClassAccess*)pCA;

if (DEBUG + DEBUG_INTERFACE >= 1)
	Init_LogFile();

	iConsoleDrawMode = 0;
	pConsole_Unit_Select = NULL;
	pConsole_Struct_Select = NULL;

	PacketProcessor.Init(pCA);
	iScanType = -6996;
	llCutOffTime = -6996;

	pDisplayText = NULL;

	//Zeromem
	ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*100);
	this->bTutorialPersistentMessageBoxActive = FALSE;

	//initialize the UNIT selection buffer, begin by allocating memory
	pUSB_First = new SBE;
	pUSB_Last = new SBE;
	pSSB_First = new SBE;
	pSSB_Last = new SBE;
	
	//create IDs(so if it crashes we can check the ID and see what happened) and link the anchors together
	pUSB_First->iID = -6996;
	pUSB_Last->iID = -6994;
	pSSB_First->iID = -6996;
	pSSB_Last->iID = -6994;

	pUSB_First->pPrev = NULL;
	pUSB_First->pNext = (VOID*)pUSB_Last;
	pUSB_Last->pPrev = (VOID*)pUSB_First;
	pUSB_Last->pNext = NULL;
	pSSB_First->pPrev = NULL;
	pSSB_First->pNext = (VOID*)pSSB_Last;
	pSSB_Last->pPrev = (VOID*)pSSB_First;
	pSSB_Last->pNext = NULL;

	//initialize the mainmenu IBOX (07.08.2002)
	IBoxes[IBOX_MAINMENU].Init(pCAccess);
	IBoxes[IBOX_MAINMENU].bActiveBox = TRUE;

	//initialize the primary IBOX (08.01.2002)
	IBoxes[IBOX_PRIMARY].Init(pCAccess);
	IBoxes[IBOX_PRIMARY].bActiveBox = FALSE;

	//just set its position (TEMP?)
	IBoxes[IBOX_PRIMARY].BoxPos.top = 0;
	IBoxes[IBOX_PRIMARY].BoxPos.left = 0;
	IBoxes[IBOX_PRIMARY].BoxPos.bottom = (int(pCA2->pDD_Display->i_YResD * MDRAW_YSCR / TILE_SIZE)+1) * TILE_SIZE;
	IBoxes[IBOX_PRIMARY].BoxPos.right = (int(pCA2->pDD_Display->i_XResD * MDRAW_XSCR / TILE_SIZE)+1) * TILE_SIZE;

	//initialize minimap IBOX (02.01.2003)
	IBoxes[IBOX_MINIMAP].Init(pCAccess);
	IBoxes[IBOX_MINIMAP].bActiveBox = FALSE;
	IBoxes[IBOX_MINIMAP].BoxPos.top = pCA2->pDD_Display->i_YResD * (MMDRAW_YSCR_START);
	IBoxes[IBOX_MINIMAP].BoxPos.left = pCA2->pDD_Display->i_XResD * (MMDRAW_XSCR_START);
	IBoxes[IBOX_MINIMAP].BoxPos.bottom = pCA2->pDD_Display->i_YResD * (MMDRAW_YSCR_START + MMDRAW_YSCR);
	IBoxes[IBOX_MINIMAP].BoxPos.right = pCA2->pDD_Display->i_XResD * (MMDRAW_XSCR_START + MMDRAW_XSCR);

	//initialize minimap ZOOM IBOX (03.24.2003)
	IBoxes[IBOX_MINIMAPZOOM].Init(pCAccess);
	IBoxes[IBOX_MINIMAPZOOM].bActiveBox = FALSE;
	IBoxes[IBOX_MINIMAPZOOM].BoxPos.top = pCA2->pDD_Display->i_YResD * (MMZDRAW_YSCR_START);
	IBoxes[IBOX_MINIMAPZOOM].BoxPos.left = pCA2->pDD_Display->i_XResD * (MMZDRAW_XSCR_START);
	IBoxes[IBOX_MINIMAPZOOM].BoxPos.bottom = pCA2->pDD_Display->i_YResD * (MMZDRAW_YSCR_START + MMZDRAW_YSCR);
	IBoxes[IBOX_MINIMAPZOOM].BoxPos.right = pCA2->pDD_Display->i_XResD * (MMZDRAW_XSCR_START + MMZDRAW_XSCR);

	//initialize equip console IBOX (03.12.2003)
	IBoxes[IBOX_CONSOLE_EQUIP].Init(pCAccess);
	IBoxes[IBOX_CONSOLE_EQUIP].bActiveBox = FALSE;
	IBoxes[IBOX_CONSOLE_EQUIP].BoxPos.top = CONSOLE_EQUIP_TOP * pCA2->pDD_Display->i_YResD;
	IBoxes[IBOX_CONSOLE_EQUIP].BoxPos.left = CONSOLE_EQUIP_LEFT * pCA2->pDD_Display->i_XResD;
	IBoxes[IBOX_CONSOLE_EQUIP].BoxPos.bottom = CONSOLE_EQUIP_BOTTOM * pCA2->pDD_Display->i_YResD;
	IBoxes[IBOX_CONSOLE_EQUIP].BoxPos.right = CONSOLE_EQUIP_RIGHT * pCA2->pDD_Display->i_XResD;

	//initialize LIST CONSOLE IBOX (03.12.2003)
	IBoxes[IBOX_CONSOLE_LIST].Init(pCAccess);
	IBoxes[IBOX_CONSOLE_LIST].bActiveBox = FALSE;
	IBoxes[IBOX_CONSOLE_LIST].BoxPos.top = CONSOLE_LIST_TOP * pCA2->pDD_Display->i_YResD;
	IBoxes[IBOX_CONSOLE_LIST].BoxPos.left = CONSOLE_LIST_LEFT * pCA2->pDD_Display->i_XResD;
	IBoxes[IBOX_CONSOLE_LIST].BoxPos.bottom = CONSOLE_LIST_BOTTOM * pCA2->pDD_Display->i_YResD;
	IBoxes[IBOX_CONSOLE_LIST].BoxPos.right = CONSOLE_LIST_RIGHT * pCA2->pDD_Display->i_XResD;

	//initialize COMMAND console IBOX (03.12.2003)
	IBoxes[IBOX_CONSOLE_COMMAND].Init(pCAccess);
	IBoxes[IBOX_CONSOLE_COMMAND].bActiveBox = FALSE;
	IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.top = CONSOLE_COMMAND_TOP * pCA2->pDD_Display->i_YResD;
	IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.left = CONSOLE_COMMAND_LEFT * pCA2->pDD_Display->i_XResD;
	IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.bottom = CONSOLE_COMMAND_BOTTOM * pCA2->pDD_Display->i_YResD;
	IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.right = CONSOLE_COMMAND_RIGHT * pCA2->pDD_Display->i_XResD;

	//initialize INFO console IBOX (03.12.2003)
	IBoxes[IBOX_CONSOLE_INFO].Init(pCAccess);
	IBoxes[IBOX_CONSOLE_INFO].bActiveBox = FALSE;
	IBoxes[IBOX_CONSOLE_INFO].BoxPos.top = CONSOLE_INFO_TOP * pCA2->pDD_Display->i_YResD;
	IBoxes[IBOX_CONSOLE_INFO].BoxPos.left = CONSOLE_INFO_LEFT * pCA2->pDD_Display->i_XResD;
	IBoxes[IBOX_CONSOLE_INFO].BoxPos.bottom = CONSOLE_INFO_BOTTOM * pCA2->pDD_Display->i_YResD;
	IBoxes[IBOX_CONSOLE_INFO].BoxPos.right = CONSOLE_INFO_RIGHT * pCA2->pDD_Display->i_XResD;

	//initialize DISPLAY console IBOX (09.19.2005)
	IBoxes[IBOX_CONSOLE_DISPLAY].Init(pCAccess);
	IBoxes[IBOX_CONSOLE_DISPLAY].bActiveBox = FALSE;
	IBoxes[IBOX_CONSOLE_DISPLAY].BoxPos.top = CONSOLE_DISPLAY_TOP * pCA2->pDD_Display->i_YResD;
	IBoxes[IBOX_CONSOLE_DISPLAY].BoxPos.left = CONSOLE_DISPLAY_LEFT * pCA2->pDD_Display->i_XResD;
	IBoxes[IBOX_CONSOLE_DISPLAY].BoxPos.bottom = CONSOLE_DISPLAY_BOTTOM * pCA2->pDD_Display->i_YResD;
	IBoxes[IBOX_CONSOLE_DISPLAY].BoxPos.right = CONSOLE_DISPLAY_RIGHT * pCA2->pDD_Display->i_XResD;

	//initialize CHAT console IBOX (01.23.2007)
	IBoxes[IBOX_CONSOLE_CHAT].Init(pCAccess);
	IBoxes[IBOX_CONSOLE_CHAT].bActiveBox = FALSE;
	IBoxes[IBOX_CONSOLE_CHAT].BoxPos.top = CONSOLE_CHAT_BOTTOM * pCA2->pDD_Display->i_YResD - (pCA2->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.5 + 15); 
	IBoxes[IBOX_CONSOLE_CHAT].BoxPos.left = CONSOLE_CHAT_LEFT * pCA2->pDD_Display->i_XResD;
	IBoxes[IBOX_CONSOLE_CHAT].BoxPos.bottom = CONSOLE_CHAT_BOTTOM * pCA2->pDD_Display->i_YResD;
	IBoxes[IBOX_CONSOLE_CHAT].BoxPos.right = CONSOLE_CHAT_RIGHT * pCA2->pDD_Display->i_XResD;

	//initialize CHAT console IBOX (01.23.2007)
	IBoxes[IBOX_NOTIFY].Init(pCAccess);
	IBoxes[IBOX_NOTIFY].bActiveBox = FALSE;
	IBoxes[IBOX_NOTIFY].BoxPos.top = CONSOLE_NOTIFY_TOP * pCA2->pDD_Display->i_YResD; 
	IBoxes[IBOX_NOTIFY].BoxPos.left = CONSOLE_NOTIFY_LEFT * pCA2->pDD_Display->i_XResD;
	IBoxes[IBOX_NOTIFY].BoxPos.bottom = CONSOLE_NOTIFY_BOTTOM * pCA2->pDD_Display->i_YResD;
	IBoxes[IBOX_NOTIFY].BoxPos.right = CONSOLE_NOTIFY_RIGHT * pCA2->pDD_Display->i_XResD;

//initialize chattext string
for (int lc26 = 0; lc26 < 1000; lc26++)
{
	caTestBox[lc26] = '\0';
	caTestBox2[lc26] = '\0';
	caChatText[lc26] = '\0';
	wcaTestBox[lc26] = 0;
}

	//Initialize the IGRID_PRIMARY grid(the actual grid won't be used because it's functionality is not applicable,
	//but it will be used to hold data about the map "window"
//some work done here to make the interface windows match the Gdisplay EXACTLY....don't know what I should do to make this solution more elegant
//	IGrids[IGRID_PRIMARY].Init(0, 0, 1, 1, (int(pCA2->pDD_Display->i_XResD * MDRAW_XSCR / TILE_SIZE)+1) * TILE_SIZE, (int(pCA2->pDD_Display->i_YResD * MDRAW_YSCR / TILE_SIZE)+1) * TILE_SIZE);

	//TEST: initialize ONE interface grid
//	IGrids[IGRID_BOTTOM].Init(0, (int(pCA2->pDD_Display->i_YResD * MDRAW_YSCR / TILE_SIZE)+1) * TILE_SIZE, 50, 20, 5, 5);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Exit()
{
	Clear_SBs();
	IBoxes[IBOX_CONSOLE_EQUIP].Exit();
	IBoxes[IBOX_CONSOLE_COMMAND].Exit();
	IBoxes[IBOX_CONSOLE_LIST].Exit();
	IBoxes[IBOX_CONSOLE_INFO].Exit();
	IBoxes[IBOX_CONSOLE_CHAT].Exit();
	IBoxes[IBOX_NOTIFY].Exit();
	IBoxes[IBOX_MAINMENU].Exit();
	IBoxes[IBOX_PRIMARY].Exit();

	Exit_LogFile();
	//TEST close the interface grid created
//	IGrids[IGRID_BOTTOM].Exit();
	//close the IGRID_PRIMARY grid
//	IGrids[IGRID_PRIMARY].Exit();
	PacketProcessor.Exit(); //08.20.2003

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cInterface::~cInterface()
{
/* //moved to Exit() 07.03.2002
	Clear_USB();
	//TEST close the interface grid created
	IGrids[IGRID_BOTTOM].Exit();
	//close the IGRID_PRIMARY grid
	IGrids[IGRID_PRIMARY].Exit();
*/
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Init_Briefing()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//clear out anything old before starting
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_BRIEFING].Clear_IfcElems();

int DX = pCA->pDD_Display->i_XResD;
int DY = pCA->pDD_Display->i_YResD;

	//add interface element(interface panel graphic)
cIfcElement* pElemNew = NULL;
PCMD2* pCmdNew = NULL;
int iretsize = 0;
	pElemNew = new cIfcElement;	
	pElemNew->Init(DX*0.05, DX*0.45, DY*0.05, DY*0.85);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcPanel(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.right, pElemNew->Rect_Pos.top, pElemNew->Rect_Pos.bottom, &iretsize);  
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWIFCPANEL;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_BRIEFING].Add_IfcElem(pElemNew);

	//add interface element(PROCEED button)
	pElemNew = Create_Button("PROCEED", DX * 0.25, DY * 0.75);
		pCmdNew = new PCMD2;
		pCmdNew->pData = new BYTE[1]; //dummy byte
		pCmdNew->wDataSize = 1;
		pCmdNew->wType = CQ2TYPE_SELECT_PROCEED;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_BRIEFING].Add_IfcElem(pElemNew);

	//add static text element (briefing text)
	pElemNew = Create_StaticTextElem(pCA->pGameWorld->Briefing.BriefText, FONT_STANDARD1, pCA->pColorTable->Black, DX * 0.1, DY * 0.1);
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_BRIEFING].Add_IfcElem(pElemNew);


	//the "choose nation" listbox

int XRes = pCA->pDD_Display->i_XResD;
int YRes = pCA->pDD_Display->i_YResD;


	//display panel for the listbox
	pElemNew = Create_IfcPanel(XRes*0.58, XRes*0.92, YRes*0.55, YRes*0.9);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_BRIEFING].Add_IfcElem(pElemNew);	
		pElemNew = Create_StaticTextElem("Choose Nation:", FONT_STANDARD1, pCA->pColorTable->White -1 , XRes*0.6, YRes*0.56);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_BRIEFING].Add_IfcElem(pElemNew);	

	//actually draw the listbox
	pElemNew = Create_IfcListBox(XRes*0.6, XRes*0.9, YRes*0.6, YRes*0.85);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_BRIEFING].Add_IfcElem(pElemNew);	
		//now create the front-end listbox for that data
		pElemNew = Create_IfcListBox2Ex(XRes*0.6, XRes*0.9, YRes*0.6, YRes*0.85, pBriefingNationSelectListBox, pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2);
			//store this address
			pBriefingNationSelectListBoxFrontEnd = pElemNew;
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_BRIEFING].Add_IfcElem(pElemNew);	



//draw the map terrain
		cImageSurface* pMinimapSurface = &pCA->pMinimap->ProcessSurface;

	pElemNew = Create_IfcPanel(XRes*0.6, XRes*0.6 + 20 + pCA->pMinimap->ProcessSurface.DDSDImage.dwWidth, YRes*0.25, YRes*0.25 + 30 + pCA->pMinimap->ProcessSurface.DDSDImage.dwHeight);
			pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_BlitBySurface(RTC_SECONDARY, pMinimapSurface->pDDSImage, 10 + pElemNew->Rect_Pos.left,  20 + pElemNew->Rect_Pos.top, 0, pMinimapSurface->DDSDImage.dwWidth, 0 ,pMinimapSurface->DDSDImage.dwHeight, &iretsize); 
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_BLITBYSURFACE;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_BRIEFING].Add_IfcElem(pElemNew);	
	pElemNew = Create_StaticTextElem("Strategic: ", FONT_STANDARD1, pCA->pColorTable->Black, XRes*0.62, YRes * 0.26); 
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_BRIEFING].Add_IfcElem(pElemNew);	
//only expose player listbox if multiplayer
if (pCA->pGameOptions->bGameType == GAMETYPE_MULTI)
{
	
//now the player listbox
	pElemNew = Create_IfcPanel(XRes*0.6, XRes*0.9, YRes*0, YRes*0.2);
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_BRIEFING].Add_IfcElem(pElemNew);
	pElemNew = Create_StaticTextElem("List of Players:", FONT_STANDARD1, pCA->pColorTable->White -1 , XRes*0.62, YRes*0.02);
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_BRIEFING].Add_IfcElem(pElemNew);

	//actually draw the listbox
	pElemNew = Create_IfcListBox(XRes*0.65, XRes*0.88, YRes*0.05, YRes*0.18);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_BRIEFING].Add_IfcElem(pElemNew);	
		//now create the front-end listbox for that data
		pElemNew = Create_IfcListBox2Ex(XRes*0.65, XRes*0.88, YRes*0.05, YRes*0.18, pBriefingPlayerListBox, pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2);
			//store this address
			pBriefingPlayerListBoxFrontEnd = pElemNew;
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_BRIEFING].Add_IfcElem(pElemNew);	
}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Process_Fake_Hotkeys()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;



	//show/disable CHAT module
	if (pCA->pInputState->IGState_Keyboard[DIK_GRAVE] == 2)
	{
		if (IBoxes[IBOX_CONSOLE_CHAT].bActiveBox == FALSE)
		{
			IBoxes[IBOX_CONSOLE_CHAT].bActiveBox = TRUE;
			pKeyEdit = caChatText;
			dwKeyEditMaxBuffer = 1000;

		}
		else
		{
			IBoxes[IBOX_CONSOLE_CHAT].bActiveBox = FALSE;
			pKeyEdit = NULL;
		}
			
	}

	//if chat box is on, don't allow anything else
		if (IBoxes[IBOX_CONSOLE_CHAT].bActiveBox == TRUE)
			return;

	//build unit
	if (pPendingCommand == NULL)
	if (pCA->pInputState->IGState_Keyboard[DIK_B] == 2)
	if (pSSB_First->pNext != pSSB_Last)
	if (iiSupportedCommandsBuffer[CQ2TYPE_START_BUILD_UNIT_PRECONFIG] > 0)
	{
		Initiate_PendingCommand(CQ2TYPE_START_BUILD_UNIT_PRECONFIG);
		//set active grid
		pCA->pInterface->IBoxes[IBOX_CONSOLE_INFO].iActiveGrid = 1;
	}
	//build structure
	if (pPendingCommand == NULL)
	if (pCA->pInputState->IGState_Keyboard[DIK_B] == 2)
	if (pUSB_First->pNext != pUSB_Last)
	if (iiSupportedCommandsBuffer[CQ2TYPE_BUILD_STRUCTURE] > 0)
	{
		Initiate_PendingCommand(CQ2TYPE_BUILD_STRUCTURE);
	}
	//arm material
	if (pPendingCommand == NULL)
	if (pCA->pInputState->IGState_Keyboard[DIK_S] == 2)
	if (pUSB_First->pNext != pUSB_Last)
	if (iiSupportedCommandsBuffer[CQ2TYPE_ARM_MATERIAL] > 0)
	{
		Initiate_PendingCommand(CQ2TYPE_ARM_MATERIAL);
	}
	//unload units
	if (pPendingCommand == NULL)
	if (pCA->pInputState->IGState_Keyboard[DIK_U] == 2)
	if (pUSB_First->pNext != pUSB_Last)
	if (iiSupportedCommandsBuffer[CQ2TYPE_UNLOAD_ALL] > 0)
	{
		Initiate_PendingCommand(CQ2TYPE_UNLOAD_ALL);
	}
	//move
	if (pPendingCommand == NULL)
	if (pCA->pInputState->IGState_Keyboard[DIK_M] == 2)
	if (pUSB_First->pNext != pUSB_Last)
	if (iiSupportedCommandsBuffer[CQ2TYPE_MOVE_B] > 0)
	{
		Initiate_PendingCommand(CQ2TYPE_MOVE_B);
	}
	//attack
	if (pPendingCommand == NULL)
	if (pCA->pInputState->IGState_Keyboard[DIK_A] == 2)
	if (pUSB_First->pNext != pUSB_Last)
	if (iiSupportedCommandsBuffer[CQ2TYPE_ATTACK_B] > 0)
	{
		Initiate_PendingCommand(CQ2TYPE_ATTACK_B);
	}
	//load
	if (pPendingCommand == NULL)
	if (pCA->pInputState->IGState_Keyboard[DIK_L] == 2)
	if (pUSB_First->pNext != pUSB_Last)
	if (iiSupportedCommandsBuffer[CQ2TYPE_LOAD] > 0)
	{
		Initiate_PendingCommand(CQ2TYPE_LOAD);
	}
	//lay link
	if (pPendingCommand == NULL)
	if (pCA->pInputState->IGState_Keyboard[DIK_L] == 2)
	if (pSSB_First->pNext != pSSB_Last)
	if (iiSupportedCommandsBuffer[CQ2TYPE_LAY_LINK] > 0)
	{
		Initiate_PendingCommand(CQ2TYPE_LAY_LINK);
	}

	//fill tank
	//load
	if (pPendingCommand == NULL)
	if (pCA->pInputState->IGState_Keyboard[DIK_F] == 2)
	if (pUSB_First->pNext != pUSB_Last)
	if (iiSupportedCommandsBuffer[CQ2TYPE_LOAD_MATERIAL] > 0)
	{
		Initiate_PendingCommand(CQ2TYPE_LOAD_MATERIAL);
	}

	//empty tank
	if (pPendingCommand == NULL)
	if (pCA->pInputState->IGState_Keyboard[DIK_E] == 2)
	if (pUSB_First->pNext != pUSB_Last)
	if (iiSupportedCommandsBuffer[CQ2TYPE_UNLOAD_MATERIAL] > 0)
	{
		Initiate_PendingCommand(CQ2TYPE_UNLOAD_MATERIAL);
	}
	//transfer
	if (pPendingCommand == NULL)
	if (pCA->pInputState->IGState_Keyboard[DIK_T] == 2)
	if (pUSB_First->pNext != pUSB_Last)
	if (iiSupportedCommandsBuffer[CQ2TYPE_TRANSFER_MATERIAL] > 0)
	{
		Initiate_PendingCommand(CQ2TYPE_TRANSFER_MATERIAL);
	}

	//tank set type
	if (pPendingCommand == NULL)
	if (pCA->pInputState->IGState_Keyboard[DIK_S] == 2)
	if (pSSB_First->pNext != pSSB_Last)
	if (iiSupportedCommandsBuffer[CQ2TYPE_TANK_SET_TYPE] > 0)
	{
		Initiate_PendingCommand(CQ2TYPE_TANK_SET_TYPE);
		return; //don't allow further commands to come in
	}
	//tank empty
	if (pPendingCommand == NULL)
	if (pCA->pInputState->IGState_Keyboard[DIK_E] == 2)
	if (pSSB_First->pNext != pSSB_Last)
	if (iiSupportedCommandsBuffer[CQ2TYPE_TANK_FLUSH] > 0)
	{
		Initiate_PendingCommand(CQ2TYPE_TANK_FLUSH);
	}

    //material hotkeys
	if (pPendingCommand != NULL)
	if (pPendingCommand->pCommand2->wType == CQ2TYPE_TANK_SET_TYPE)
	if (pPendingCommand->iCurrentSequenceNumber == 1)
	{
		for (int lc7 = 0; lc7 < 256; lc7++)
		{
			if (pCA->pInputState->IGState_Keyboard[lc7] == 2)
			{
				char RefChar = pCA->pInputState->DI_ANSI_Table_Shift[lc7];
				int iType = pCA->pMaterialInfoTable->FindType_ByRefChar(RefChar);
				if (iType >= 0)
				{
					DWORD dwType = (DWORD)iType;
					pCA->pInputPacketBuffer->Compile_Add_Packet(IPACKETTYPE_OBJREF, OBJTYPE_MATTYPE, 1, NULL, &dwType, 0, 0, NULL, TRUE);
				}
			}
		}
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Process_Input()
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int	wGridIndex;
IGRIDCELL* pGrid; //the array of cells
cIGrid* pIGrid;  //the entire grid
	//very simple here....for now
	char NText[50];

	//check for mapview change commands (currently move on keydown only) TEMPORARY 12.29.2001
	if (pCA->pInputState->IGState_Keyboard[DIK_DOWN] == 1 || pCA->pInputState->IGState_Mouse.iYPos >= pCA->pDD_Display->i_YResD - 3) 
	if (map_yview + pCA->pGDisplay->MDRAW_NUMROWS + 1 < pCA->pGameWorld->iHeight)
		map_yview++;
	if (pCA->pInputState->IGState_Keyboard[DIK_RIGHT] == 1 || pCA->pInputState->IGState_Mouse.iXPos >= pCA->pDD_Display->i_XResD - 3) 
	if (map_xview + pCA->pGDisplay->MDRAW_NUMCOLS + 1 < pCA->pGameWorld->iWidth)
		map_xview++;
	if (pCA->pInputState->IGState_Keyboard[DIK_UP] == 1 || pCA->pInputState->IGState_Mouse.iYPos <= 3) 
	if (map_yview > 0)
		map_yview--;
	if (pCA->pInputState->IGState_Keyboard[DIK_LEFT] == 1 || pCA->pInputState->IGState_Mouse.iXPos <= 3)
	if (map_xview > 0)
		map_xview--;

	//this is TEMPORARY...change resolution if a key is pressed(key down) (01.02.2002)
	//disable this for now because it is causing leaks of GDI16 system resources! - 01.04.2002
//	if (pCA->pInputState->IGState_Keyboard[DIK_U] == 2)
//		pCA->pDD_Display->Change_Resolution(640, 480, 16);

//	if (pCA->pInputState->IGState_Keyboard[DIK_I] == 2)
//		pCA->pDD_Display->Change_Resolution(800, 600, 16);

//	if (pCA->pInputState->IGState_Keyboard[DIK_O] == 2)
//		pCA->pDD_Display->Change_Resolution(1024, 768, 16);

//	if (pCA->pInputState->IGState_Keyboard[DIK_P] == 2)
//		pCA->pDD_Display->Change_Resolution(1400, 1050, 16);

	//keep the mapview inside viewable range
	if (map_xview + pCA->pGDisplay->MDRAW_NUMCOLS >= pCA->pGameWorld->iWidth)
		map_xview = pCA->pGameWorld->iWidth - pCA->pGDisplay->MDRAW_NUMCOLS - 1;
	if (map_yview + pCA->pGDisplay->MDRAW_NUMROWS >= pCA->pGameWorld->iHeight)
		map_yview = pCA->pGameWorld->iWidth - pCA->pGDisplay->MDRAW_NUMROWS - 1;

	if (pCA->pInputState->IGState_Keyboard[DIK_MINUS] == 1 && pCA->pTimer->iCountsPerTick > 200)
		pCA->pTimer->iCountsPerTick = pCA->pTimer->iCountsPerTick - 200;
	if (pCA->pInputState->IGState_Keyboard[DIK_EQUALS] == 1)
		pCA->pTimer->iCountsPerTick = pCA->pTimer->iCountsPerTick + 200;		



//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//

	//determine the current interface mode
	iMode = IFCM_SELECTION;  //default mode

	//if there is anything in the USB then the interface is in command mode	
	if ((LPSBE)pUSB_First->pNext != pUSB_Last)
	{
		iMode = IFCM_UCOMMAND;
			//if there are enemy units in the buffer, mode is VIEW
			if (pCA->pUnitTable->pUnitTable[((LPSBE)pUSB_First->pNext)->iID]->iControl != pCA->pGameOptions->iControl)
				iMode = IFCM_VIEW;
	}
	if ((LPSBE)pSSB_First->pNext != pSSB_Last)
	{
		iMode = IFCM_SCOMMAND;
			//if there are enemy units in the buffer, mode is VIEW
			if (pCA->pStructIDTable->pTable[((LPSBE)pSSB_First->pNext)->iID]->iControl != pCA->pGameOptions->iControl)
				iMode = IFCM_VIEW;
	}
	//if the pending command is online, mode is IFCM_SELECT_EXTENDED
	if (pPendingCommand != NULL)
		iMode = IFCM_SELECT_EXTENDED;


	//determine where the cursor is...
	mouse_currbox = IBOX_NOWHERE;	//nominally nowhere

	//ehehe, arrayed search(of the grids)
	for (int c3 = 0; c3 < IBOX_NUMBOXES; c3++)
	{
	if (IBoxes[c3].bActiveBox == TRUE)
	if (pCA->pInputState->IGState_Mouse.iXPos <= IBoxes[c3].BoxPos.right &&
		pCA->pInputState->IGState_Mouse.iYPos <= IBoxes[c3].BoxPos.bottom &&
		pCA->pInputState->IGState_Mouse.iYPos >= IBoxes[c3].BoxPos.top &&
		pCA->pInputState->IGState_Mouse.iXPos >= IBoxes[c3].BoxPos.left)
			mouse_currbox = c3;
	}

	//mousewheel
if (pCA->pInputState->IGState_Mouse.iWheel != 0)
if (mouse_currbox == IBOX_CONSOLE_LIST || mouse_currbox == IBOX_CONSOLE_EQUIP || mouse_currbox == IBOX_CONSOLE_COMMAND || mouse_currbox == IBOX_CONSOLE_INFO)
if (IBoxes[mouse_currbox].pIGrids[IBoxes[mouse_currbox].iActiveGrid].bScrollbarFlag == TRUE)
{
	cIGrid* pGrid = &IBoxes[mouse_currbox].pIGrids[IBoxes[mouse_currbox].iActiveGrid];

	pGrid->iScrollVal += pCA->pInputState->IGState_Mouse.iWheel;
	if (pGrid->iScrollVal < pGrid->iScrollMin)
	{
		pGrid->iScrollVal = pGrid->iScrollMin;
	}
	if (pGrid->iScrollVal > pGrid->iScrollMax)
	{
		pGrid->iScrollVal = pGrid->iScrollMax;
	}
}


	//set the text EVERY CYCLE(a HACK 06.03.2003)
	pDisplayText = NULL;
	if (pPendingCommand != NULL)
		pDisplayText = pCA->pCmdReqTable->CommandTypes[pPendingCommand->iRequestIndex ].RequestText[pPendingCommand->iCurrentSequenceNumber];


//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&MINIMAP GRID COMMANDS&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&	
//the minimap now has much copied functionality from the IBOX_PRIMARY section(12.25.2004)
if (mouse_currbox == IBOX_MINIMAP)
{
	//determine the minimap square the mouse is over
	PosMinimap_Mouse = Calc_PosMinimap(pCA->pInputState->IGState_Mouse.iXPos, pCA->pInputState->IGState_Mouse.iYPos);

	pCA->pInputState->iDrawFrame = 0;
	//change cursor color depending on what's on the minimap (12.25.2004) 
	if (pCA->pGameWorld->pMap[PosMinimap_Mouse.index].pOccupant_Unit != NULL && iMode == IFCM_UCOMMAND)
	if (((cUnitA*)pCA->pGameWorld->pMap[PosMinimap_Mouse.index].pOccupant_Unit)->iControl != pCA->pGameOptions->iControl)
		pCA->pInputState->iDrawFrame = 2;
	if (pCA->pGameWorld->pMap[PosMinimap_Mouse.index].pOccupant_Structure != NULL && iMode == IFCM_UCOMMAND)
	if (((cStructureA*)pCA->pGameWorld->pMap[PosMinimap_Mouse.index].pOccupant_Structure)->iControl != pCA->pGameOptions->iControl)
		pCA->pInputState->iDrawFrame = 2;
/* //functionality disabled
		//RIGHT(CANCEL) BUTTON DOWN(code 2)  (release all units from USB, clear pending command)
		if (pCA->pInputState->IGState_Mouse.Buttons[1] == 2)
		{	
			ReleaseUnits_SB_all(pUSB_First, pUSB_Last);
			ReleaseStructs_SB_all(pSSB_First, pSSB_Last);
			pConsole_Unit_Select = NULL;
			pConsole_Struct_Select = NULL;
			if (pPendingCommand != NULL)
			{
				pPendingCommand->Exit();
				pPendingCommand = NULL;
			}
		}
*/
	//if RIGHT button is ONMOUSEUP, issue commands
	if (iMode == IFCM_UCOMMAND)
	{
		if (pCA->pInputState->IGState_Mouse.Buttons[1] == 3)
		{
			cUnitA* pUnit = (cUnitA*)pCA->pGameWorld->pMap[PosMinimap_Mouse.index].pOccupant_Unit;
			cStructureA* pStruct = (cStructureA*)pCA->pGameWorld->pMap[PosMinimap_Mouse.index].pOccupant_Structure;
			LPSBE pUSB_Trav = (LPSBE)pUSB_First->pNext;
		
			//HACK (01.18.2005)
			int iGroupFlag = 0;
			if (pUSB_Trav->pNext != pUSB_Last)
				iGroupFlag = 1;

			//07.30.2003 (attempt to switch both these issue mechanisms to CQ2TYPE_MOVE_B and CQ2TYPE_ATTACK_B)			
			int iCommandToIssue = CQ2TYPE_MOVE_B;

			if (pUnit != NULL)
			if (pUnit->iControl != pCA->pGameOptions->iControl)
				iCommandToIssue = CQ2TYPE_ATTACK_B;


			if (pStruct != NULL)
			if (pStruct->iControl != pCA->pGameOptions->iControl)
				iCommandToIssue = CQ2TYPE_ATTACK_B;
				
			Initiate_PendingCommand(iCommandToIssue);
			
			//immediately provide an Input Packet on the enemy unit or structure if found
			if (pUnit != NULL)
			//if (pUnit->iControl != pCA->pGameOptions->iControl)  (changed to issue a packet even if on a friendly unit(for load command)) 10.13.2006
			{
				DWORD* pdwID = new DWORD;
					*pdwID = pUnit->iID;
					pCA->pInputPacketBuffer->Compile_Add_Packet(IPACKETTYPE_OBJREF, OBJTYPE_UNIT, 1, NULL, pdwID, 0, 0, NULL, FALSE);
					//this will cause the command to immediately dispatch next cycle, so issue the acknowledgement here
					//pCA->pCommQueue->Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, pCA->pAckTable->Supply_Random_Ack(), pUSB_Trav->iID, iGroupFlag, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);

			}
			if (pStruct != NULL)
			if (pStruct->iControl != pCA->pGameOptions->iControl)
			{
				DWORD* pdwID = new DWORD;
					*pdwID = pStruct->iID;
				pCA->pInputPacketBuffer->Compile_Add_Packet(IPACKETTYPE_OBJREF, OBJTYPE_STRUCTURE, 1, NULL, pdwID, 0, 0, NULL, FALSE);
					//this will cause the command to immediately dispatch next cycle, so issue the acknowledgement here
					//pCA->pCommQueue->Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, pCA->pAckTable->Supply_Random_Ack(), pUSB_Trav->iID, iGroupFlag, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);

			}
			//if just a MOVE, give an input packet on the maptile position
			if (iCommandToIssue == CQ2TYPE_MOVE_B)
			{
				DWORD* pdwID = new DWORD;
					*pdwID = PosMinimap_Mouse.index;
				pCA->pInputPacketBuffer->Compile_Add_Packet(IPACKETTYPE_OBJREF, OBJTYPE_MAPTILE, 1, NULL, pdwID, 0, 0, NULL, FALSE);
					//this will cause the command to immediately dispatch next cycle, so issue the acknowledgement here
					//pCA->pCommQueue->Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, pCA->pAckTable->Supply_Random_Ack(), pUSB_Trav->iID, iGroupFlag, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);

			}
		}



	} 
//	if (iMode != IFCM_UCOMMAND)
//	{ //no longer. do this regardless
		if (pCA->pInputState->IGState_Mouse.Buttons[0] == 1)
		{

			map_xview = PosMinimap_Mouse.x;
			map_yview = PosMinimap_Mouse.y;

			//curb the numbers inside viewbounds
			if ( map_xview > pCA->pGameWorld->iWidth - pCA->pGDisplay->MDRAW_NUMCOLS - 1)
				map_xview = pCA->pGameWorld->iWidth - pCA->pGDisplay->MDRAW_NUMCOLS - 1;
			if ( map_yview > pCA->pGameWorld->iHeight - pCA->pGDisplay->MDRAW_NUMROWS - 1)
				map_yview = pCA->pGameWorld->iHeight - pCA->pGDisplay->MDRAW_NUMROWS - 1;
			
			//reset offsets
			pCA->pMinimap->sqMMZOffset.x = 0;
			pCA->pMinimap->sqMMZOffset.y = 0;
		}
//	}

}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&MINIMAP ZOOM GRID COMMANDS&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&	
if (mouse_currbox == IBOX_MINIMAPZOOM)
{
	//determine the minimap square the mouse is over
	PosMinimap_Mouse = Calc_PosMinimapZoom(pCA->pInputState->IGState_Mouse.iXPos, pCA->pInputState->IGState_Mouse.iYPos);

//store the old value and reset the offset so that the zoom minimap doesn't move
int ioldx = map_xview;
int ioldy = map_yview;

	//if left button is ON, move map there
	if (pCA->pInputState->IGState_Mouse.Buttons[0] == 1)
	{
		map_xview = PosMinimap_Mouse.x;
		map_yview = PosMinimap_Mouse.y;
		pCA->pMinimap->sqMMZOffset.x -= map_xview - ioldx;
		pCA->pMinimap->sqMMZOffset.y -= map_yview - ioldy;

	}
//make sure minimap zoom is inside map bounds
if (pCA->pMinimap->sqMMZOffset.x > 0)
	pCA->pMinimap->sqMMZOffset.x = 0;
if (pCA->pMinimap->sqMMZOffset.y > 0)
	pCA->pMinimap->sqMMZOffset.y = 0;

}



//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&PRIMARY GRID COMMANDS&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&	

if (mouse_currbox == IBOX_PRIMARY)
{
	//determine the map square the mouse is over
	PosMap_Mouse = Calc_PosMap(pCA->pInputState->IGState_Mouse.iXPos, pCA->pInputState->IGState_Mouse.iYPos);
	PosMap_Mouse_ABS = Calc_PosMap_ABS(pCA->pInputState->IGState_Mouse.iXPos, pCA->pInputState->IGState_Mouse.iYPos);

	//set the drawframe for the cursor(don't know where this code should go....(06.26.2002)
	//default at 0
	pCA->pInputState->iDrawFrame = 0;
	//for now only change the cursor drawframe if left button is ON
	if (pCA->pInputState->IGState_Mouse.Buttons[0] == 1)
		pCA->pInputState->iDrawFrame = 1;
	//TEMP(until player info is set up), if the square the unit is over contains a unit whose ID is different from that of the first
	//unit in the USB, set the cursor to the attack cursor
	if (pCA->pGameWorld->pMap[PosMap_Mouse.index].pOccupant_Unit != NULL && iMode == IFCM_UCOMMAND)
	if (((cUnitA*)pCA->pGameWorld->pMap[PosMap_Mouse.index].pOccupant_Unit)->iControl != pCA->pGameOptions->iControl)
		pCA->pInputState->iDrawFrame = 2;
	//duplicate again for structures! (04.03.2003)
	if (pCA->pGameWorld->pMap[PosMap_Mouse.index].pOccupant_Structure != NULL && iMode == IFCM_UCOMMAND)
	if (((cStructureA*)pCA->pGameWorld->pMap[PosMap_Mouse.index].pOccupant_Structure)->iControl != pCA->pGameOptions->iControl)
		pCA->pInputState->iDrawFrame = 2;

	//SELECTION MODE
	
	//SELECT EXTENDED mode
	if (iMode == IFCM_SELECT_EXTENDED)
	{
		/////LEFT(SELECTION) BUTTON up(code 3)
		//compile an input packet containing the map coord references
		if (pCA->pInputState->IGState_Mouse.Buttons[0] == 3)
		{
				
			DWORD* pdwCoord = new DWORD;
			*pdwCoord = PosMap_Mouse.index;
			
			//note I have not attempted to provide ppObjRefs to save on hassle since I currently don't use it anyway(05.31.2003)
			pCA->pInputPacketBuffer->Compile_Add_Packet(IPACKETTYPE_OBJREF, OBJTYPE_MAPTILE, 1, NULL, pdwCoord, 0, 0, NULL, FALSE);

			//immediately provide an Input Packet on the enemy unit or structure if found
			cUnitA* pUnit = (cUnitA*)pCA->pGameWorld->pMap[PosMap_Mouse.index].pOccupant_Unit;
			if (pUnit != NULL)
			//if (pUnit->iControl != pCA->pGameOptions->iControl) (changed to issue a packet even if on a friendly unit(for load command)) 10.13.2006
			{
				DWORD* pdwID = new DWORD;
					*pdwID = pUnit->iID;
					pCA->pInputPacketBuffer->Compile_Add_Packet(IPACKETTYPE_OBJREF, OBJTYPE_UNIT, 1, NULL, pdwID, 0, 0, NULL, FALSE);
					
					//this will cause the command to immediately dispatch next cycle, so issue the acknowledgement here
//					if (pUSB_Trav != pUSB_Last)
//						pCA->pCommQueue->Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, pCA->pAckTable->Supply_Random_Ack(), pUSB_Trav->iID, iGroupFlag, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
			}


		}
		//RIGHT(CANCEL) BUTTON DOWN(code 2)  (release all units from USB, clear pending command)
		if (pCA->pInputState->IGState_Mouse.Buttons[1] == 2)
		{	
			ReleaseUnits_SB_all(pUSB_First, pUSB_Last);
			ReleaseStructs_SB_all(pSSB_First, pSSB_Last);
			pConsole_Unit_Select = NULL;
			pConsole_Struct_Select = NULL;
			pPendingCommand->Exit();
			pPendingCommand = NULL;
		}

	}

	//added IFCM_UCOMMAND to make into War2/SC style
	if (iMode == IFCM_SELECTION || iMode == IFCM_VIEW || iMode == IFCM_UCOMMAND || iMode == IFCM_SCOMMAND)
	{
		/////LEFT(SELECTION) BUTTON DOWN(code 2)
		//store the coordinates in the selection box
		if (pCA->pInputState->IGState_Mouse.Buttons[0] == 2)
		{
		//by default, they go in the top and left . 
		MSelectBox_GW.top = PosMap_Mouse.y;
		MSelectBox_GW.left = PosMap_Mouse.x;
		MSelectBox_GW_ABS.top = PosMap_Mouse_ABS.y;
		MSelectBox_GW_ABS.left = PosMap_Mouse_ABS.x;  //I am effectively doubling the processing required by dealing with the 
													  //absolute coordinates, but I don't see any way to do group vs. single-select (05.13.2002)
		}
		/////LEFT(SELECTION) BUTTON UP(code 3)
		//finish storing in the selection box, correct the box, add the selected units to the unit buffer
		if (pCA->pInputState->IGState_Mouse.Buttons[0] == 3)
		{
		//by default, they go in the bottom and right 
		MSelectBox_GW.bottom = PosMap_Mouse.y;
		MSelectBox_GW.right = PosMap_Mouse.x;
		MSelectBox_GW_ABS.right = PosMap_Mouse_ABS.x;
		MSelectBox_GW_ABS.bottom = PosMap_Mouse_ABS.y;
		//correct the boxes
		Fix_RectBox(&MSelectBox_GW);
		Fix_RectBox(&MSelectBox_GW_ABS);

		//(05.13.2002) Perhaps try using a position-based method for choosing single-select vs. group select:
		//if the mouse changes position by less than some number of pixels and remains in the same square,
		//do a single select instead of a group select
		//check for single-select condition
		if (MSelectBox_GW.top == MSelectBox_GW.bottom &&
			MSelectBox_GW.left == MSelectBox_GW.right && 
			abs(MSelectBox_GW_ABS.top - MSelectBox_GW_ABS.bottom) <= MSELECT_DRPIXELS && 
			abs(MSelectBox_GW_ABS.left - MSelectBox_GW_ABS.right) <= MSELECT_DRPIXELS)
			{
				pConsole_Unit_Select = NULL;
				pConsole_Struct_Select = NULL;
			
				//if we are in UVIEW mode, purge the SBs, and select the STACK that is here
				//if (iMode == IFCM_VIEW)  //disabled to always purge (war2/SC style processing)
				//{

/*
					AddUnits_SB_stack(MSelectBox_GW.top * pCA->pGameWorld->iWidth + MSelectBox_GW.left, FALSE, pUSB_First, pUSB_Last);
					if (pUSB_First->pNext != pUSB_Last)
							pConsole_Unit_Select = pUSB_First->pNext;
					if (pSSB_First->pNext != pSSB_Last)
							pConsole_Struct_Select = pSSB_First->pNext;
				//}
			*/

					ReleaseUnits_SB_all(pUSB_First, pUSB_Last);
					ReleaseStructs_SB_all(pSSB_First, pSSB_Last);

				//SINGLE-SELECT PROCESS HERE !!!!!!
				//here....always selected units over structures, at least for now....
				if (0 == AddUnits_SB_single(MSelectBox_GW.top * pCA->pGameWorld->iWidth + MSelectBox_GW.left, pUSB_First, pUSB_Last))
					AddStructs_SB_single(MSelectBox_GW.top * pCA->pGameWorld->iWidth + MSelectBox_GW.left, pSSB_First, pSSB_Last);
				if (pUSB_First->pNext != pUSB_Last)
							pConsole_Unit_Select = pUSB_First->pNext;
				if (pSSB_First->pNext != pSSB_Last)
							pConsole_Struct_Select = pSSB_First->pNext;
			}
				else  //otherwise, do a GROUP SELECT
				{
					//PURGE first (war2/SC)
					ReleaseUnits_SB_all(pUSB_First, pUSB_Last);
					ReleaseStructs_SB_all(pSSB_First, pSSB_Last);
					pConsole_Unit_Select = NULL;
					pConsole_Struct_Select = NULL;

					//perform a check of the selected area and place the selected units into the unit buffer
					int c1 = 0;
					int c2 = 0;
					for (c1 = MSelectBox_GW.top; c1<= MSelectBox_GW.bottom; c1++)
					for (c2 = MSelectBox_GW.left; c2 <= MSelectBox_GW.right; c2++)
					{
						//this function changed to only group-select friendly units
						AddUnits_SB_stack(c1 * pCA->pGameWorld->iWidth + c2, TRUE, pUSB_First, pUSB_Last);
						if (pUSB_First->pNext != pUSB_Last)
							pConsole_Unit_Select = pUSB_First->pNext;
					}
				}

			//if the USB is not empty and the first unit is not hostile, post an ack
				LPSBE pUSB_Trav = (LPSBE)pUSB_First->pNext;
			if (pUSB_Trav != pUSB_Last)
			{
				cUnitA* pUnit = pCA->pUnitTable->pUnitTable[LPSBE(pUSB_First->pNext)->iID];

				if (pUnit->iControl == pCA->pGameOptions->iControl)
				{
//						//set acknowledgement
						int iRet = pCA->pAckTable->Get_Ack(FALSE, pUnit->iType, ACK_TYPE_HELLO, pUnit->iPersonality, rand() % 3);

					if (pUSB_Trav->pNext == pUSB_Last)
					{
						pCA->pCommQueue->Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, pUSB_Trav->iID, 0, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
					}
					else
					{
						pCA->pCommQueue->Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, iRet, pUSB_Trav->iID, 1, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
					}
				}

			}
		
		}
		//RIGHT(CANCEL) BUTTON DOWN(code 2)  (release all units from USB)
		/* no longer C&C style
		if (pCA->pInputState->IGState_Mouse.Buttons[1] == 2)
		{
			ReleaseUnits_SB_all(pUSB_First, pUSB_Last);
			ReleaseStructs_SB_all(pSSB_First, pSSB_Last);
			pConsole_Unit_Select = NULL;
			pConsole_Struct_Select = NULL;
		}
		*/ 
	}

	//UCOMMAND MODE//////////////////////////////////
	if (iMode == IFCM_UCOMMAND)
	{

		//RIGHT(CANCEL) BUTTON DOWN(code 2)  (release all units from USB)
/* no longer C&C style
		if (pCA->pInputState->IGState_Mouse.Buttons[1] == 2)
		{	
			ReleaseUnits_SB_all(pUSB_First, pUSB_Last);
			ReleaseStructs_SB_all(pSSB_First, pSSB_Last);
			pConsole_Unit_Select = NULL;
			pConsole_Struct_Select = NULL;
		}
*/
		//LEFT(ISSUE) BUTTON DOWN(code 2) (issue "move" commands to units(may be extended in the the future to deal with attacking enemies 05.14.2002) extended 06.27.2002
		//if (pCA->pInputState->IGState_Mouse.Buttons[0] == 2)		
		//changed to RIGHT BUTTON DOWN (code 2) to make it War2/SC style
		if (pCA->pInputState->IGState_Mouse.Buttons[1] == 2)		
		{
			cUnitA* pUnit = (cUnitA*)pCA->pGameWorld->pMap[PosMap_Mouse.index].pOccupant_Unit;
			cStructureA* pStruct = (cStructureA*)pCA->pGameWorld->pMap[PosMap_Mouse.index].pOccupant_Structure;
			LPSBE pUSB_Trav = (LPSBE)pUSB_First->pNext;

			//HACK (01.18.2005)
			int iGroupFlag = 0;
			if (pUSB_Trav->pNext != pUSB_Last)
				iGroupFlag = 1;
			
			//07.30.2003 (attempt to switch both these issue mechanisms to CQ2TYPE_MOVE_B and CQ2TYPE_ATTACK_B)			
			int iCommandToIssue = CQ2TYPE_MOVE_B;

			if (pUnit != NULL)
			if (pUnit->iControl != pCA->pGameOptions->iControl)
				iCommandToIssue = CQ2TYPE_ATTACK_B;

			//07.07.2007
			if (pUnit != NULL)
			if (pUnit->iControl == pCA->pGameOptions->iControl)
			if (pUnit->iStorageVolume > 0)
			{
				
				iCommandToIssue = CQ2TYPE_LOAD;
			}


			if (pStruct != NULL)
			if (pStruct->iControl != pCA->pGameOptions->iControl)
				iCommandToIssue = CQ2TYPE_ATTACK_B;
				
			Initiate_PendingCommand(iCommandToIssue);
			
			//immediately provide an Input Packet on the enemy(or friendly) unit or structure if found
			if (pUnit != NULL)
			//if (pUnit->iControl != pCA->pGameOptions->iControl) (changed to issue a packet even if on a friendly unit(for load command)) 10.13.2006
			{
				DWORD* pdwID = new DWORD;
					*pdwID = pUnit->iID;
					pCA->pInputPacketBuffer->Compile_Add_Packet(IPACKETTYPE_OBJREF, OBJTYPE_UNIT, 1, NULL, pdwID, 0, 0, NULL, FALSE);
					
					//this will cause the command to immediately dispatch next cycle, so issue the acknowledgement here
//					if (pUSB_Trav != pUSB_Last)
//						pCA->pCommQueue->Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, pCA->pAckTable->Supply_Random_Ack(), pUSB_Trav->iID, iGroupFlag, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
			}
			if (pStruct != NULL)
			if (pStruct->iControl != pCA->pGameOptions->iControl)
			{
				DWORD* pdwID = new DWORD;
					*pdwID = pStruct->iID;
				
					if (pUSB_Trav != pUSB_Last)	
						pCA->pInputPacketBuffer->Compile_Add_Packet(IPACKETTYPE_OBJREF, OBJTYPE_STRUCTURE, 1, NULL, pdwID, 0, 0, NULL, FALSE);

				//this will cause the command to immediately dispatch next cycle, so issue the acknowledgement here
		//		pCA->pCommQueue->Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, pCA->pAckTable->Supply_Random_Ack(), pUSB_Trav->iID, iGroupFlag, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
			}
			//if just a MOVE, give an input packet on the maptile position
	//		if (iCommandToIssue == CQ2TYPE_MOVE_B)   //changed to simply issue always (10.16.2006)
	//		{
				DWORD* pdwID = new DWORD;
					*pdwID = PosMap_Mouse.index;
					
					if (pUSB_Trav != pUSB_Last)
						pCA->pInputPacketBuffer->Compile_Add_Packet(IPACKETTYPE_OBJREF, OBJTYPE_MAPTILE, 1, NULL, pdwID, 0, 0, NULL, FALSE);

					//this will cause the command to immediately dispatch next cycle, so issue the acknowledgement here
		//			pCA->pCommQueue->Issue_Command(CQDEST_INTERFACE, CQTYPE_POST_ACK_U, pCA->pAckTable->Supply_Random_Ack(), pUSB_Trav->iID, iGroupFlag, 0, 0, NULL, pCA->pTimer->iCurrTick + 1);
	//		}
		}


	}
	//MODE SCOMMAND
	if (iMode == IFCM_SCOMMAND)
	{
/* no longer C&C style
		//RIGHT(CANCEL) BUTTON DOWN(code 2)  (release all units from USB)
		if (pCA->pInputState->IGState_Mouse.Buttons[1] == 2)
		{	
			ReleaseUnits_SB_all(pUSB_First, pUSB_Last);
			ReleaseStructs_SB_all(pSSB_First, pSSB_Last);
			pConsole_Unit_Select = NULL;
			pConsole_Struct_Select = NULL;
		}
*/
	}


}
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//CONSOLE INTERFACING &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//


/*
//handle scroll grid
if (pCA->pInputState->IGState_Mouse.Buttons[0] == 3)
{
	pScrollGrid = NULL;
}
if (pScrollGrid != NULL)
{
	//yes, it's weird, the problem is that the graphics dimensions are with the box,
	//but the data "dimensions" are with the grid
	int iDim = pCA->pImages[ISURF_SCROLLBAR]->DDSDImage.dwWidth;
	float fDiff = (pCA->pInputState->IGState_Mouse.iYPos + iDim) - (pScrollBox->BoxPos.top);// + iDim);
	float fRange = pScrollBox->BoxPos.bottom - (pScrollBox->BoxPos.top);// + iDim);

	if (fDiff < 0)
		fDiff = 0;

	if (fDiff > fRange)
		fDiff = fRange;

	float fRatio = fDiff / fRange;
	fRatio *= pScrollGrid->iScrollMax - pScrollGrid->iScrollMin;
	fRatio += pScrollGrid->iScrollMin;

	//set this as value
	pScrollGrid->iScrollVal = fRatio;

}
*/
	//if click on EQUIP_LIST, activate the selected command box
if (mouse_currbox == IBOX_CONSOLE_LIST || mouse_currbox == IBOX_CONSOLE_DISPLAY || mouse_currbox == IBOX_CONSOLE_EQUIP || mouse_currbox == IBOX_NOTIFY || mouse_currbox == IBOX_CONSOLE_COMMAND || mouse_currbox == IBOX_CONSOLE_INFO)
	{
	
//DEBUG
Write_Log("[Process Input] Beginning Console Command Dispatch");
WriteLog_Enter();
		pIGrid = &IBoxes[mouse_currbox].pIGrids[IBoxes[mouse_currbox].iActiveGrid];
		pGrid = pIGrid->pGrid;
		//wGridIndex = pIGrid->Calc_GridIndex(pCA->pInputState->IGState_Mouse.iXPos, pCA->pInputState->IGState_Mouse.iYPos);

		//do interface element processing
		Process_Interface_Elements(pIGrid->pIfcElem_First, pIGrid->pIfcElem_Last);
	}

//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//PACKET PROCESSOR &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//
	BOOL bRet;
	if (iScanType >= 0)  //08.20.2003
	{
		bRet = PacketProcessor.PeekAcquire_IPB_Specific(pCA->pInputPacketBuffer->pIPB_First, 
												 pCA->pInputPacketBuffer->pIPB_Last,
												 iScanType,
												 &llCutOffTime, 
												 TRUE);
		if (bRet == TRUE)
		{
			//DEBUG
			Write_Log("[Packet Processor] Input Packet received by PACKET PROCESSOR. Time: ");
			itoa(pCA->pTimer->iCurrTick, NText, 10);
			Write_Log(NText);
			WriteLog_Enter();
			
		}

	}
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//CHAT CONTROL &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&//




}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Process_Interface_Elements(cIfcElement* pFirst, cIfcElement* pLast)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//determine what interface element we are over
	cIfcElement* pElem = NULL;

	cIfcElement* pTrav = pFirst->pNext;

	int iX = pCA->pInputState->IGState_Mouse.iXPos;
	int iY = pCA->pInputState->IGState_Mouse.iYPos;

	while (pTrav != pLast)
	{
		if (iX > pTrav->Rect_Pos.left &&
			iX < pTrav->Rect_Pos.right &&
			iY > pTrav->Rect_Pos.top && 
			iY < pTrav->Rect_Pos.bottom)
		{
				pElem = pTrav;
				//at this point, we execute the commands in the list 
			if (pElem != NULL)
			{
					//default mode is "onhover"
					int iCategory = IFC_ELEM_CAT_ONHOVER;	

				//determine which categories to execute
				//if mouse down
				if ( pCA->pInputState->IGState_Mouse.Buttons[0] == 1)
				{
					iCategory = IFC_ELEM_CAT_ONLON;
				}
				if ( pCA->pInputState->IGState_Mouse.Buttons[0] == 2)
				{
					iCategory = IFC_ELEM_CAT_ONLDOWN;
				}
				if ( pCA->pInputState->IGState_Mouse.Buttons[0] == 3)
				{
					iCategory = IFC_ELEM_CAT_ONLUP;
				}
				//execute commands
				PCMD2* pCmdTrav = pElem->pCmds[iCategory];

				while (pCmdTrav != NULL)
				{
					//broadcast the command if its actually an gameworld command
					//broadcast the command if its actually an gameworld command
					//additionally, issue it through the CommQueue(so that it stays there and can be called at the right time)
					//since it is dependent on the exec tick
					if (pCmdTrav->wType <= NUM_CQ2COMMANDS)
					{
						pCA->pCommQueue->Issue_Command_Q2(pCmdTrav->wType, pCmdTrav->pData, pCmdTrav->wDataSize, TRUE);
						pCA->pDP_Multiplay->Send_Command2(pCmdTrav->wType, pCmdTrav->pData, pCmdTrav->wDataSize, FALSE, TRUE);
					} else
					{
						pCA->pCommQueue->Execute_Command2(pCmdTrav);
					}

					pCmdTrav = pCmdTrav->pNext;
				}

				//check child elements
				Process_Interface_Elements_B(pElem->pChildren);

			}
	
		}

		pTrav = pTrav->pNext;
	}

	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Process_Interface_Elements_B(cIfcElement* pHead)
{
	//this version of the function is for anchor-less lists
	//two(now three, sort of) versions of the function are used due to the fact that 
	//the IGrids have anchored lists of ifcelems while the internal lists
	//of ifcelems themselves do not have anchors (07.22.2005)

	if (pHead == NULL)
		return;

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//determine what interface element we are over
	cIfcElement* pElem = NULL;

	cIfcElement* pTrav = pHead;

	int iX = pCA->pInputState->IGState_Mouse.iXPos;
	int iY = pCA->pInputState->IGState_Mouse.iYPos;

	while (pTrav != NULL)
	{
		Process_Interface_Elements_C(pTrav);
		pTrav = pTrav->pNext;
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Process_Interface_Elements_C(cIfcElement* pElem)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;


	//only performs the "execute single element part", doesn't traverse
	int iX = pCA->pInputState->IGState_Mouse.iXPos;
	int iY = pCA->pInputState->IGState_Mouse.iYPos;

	if (iX > pElem->Rect_Pos.left &&
			iX < pElem->Rect_Pos.right &&
			iY > pElem->Rect_Pos.top && 
			iY < pElem->Rect_Pos.bottom)
		{


				//at this point, we execute the commands in the list 
			if (pElem != NULL)
			{
					//default mode is "onhover"
					int iCategory = IFC_ELEM_CAT_ONHOVER;	

				//determine which categories to execute
				//if mouse down
				if ( pCA->pInputState->IGState_Mouse.Buttons[0] == 1)
				{
					iCategory = IFC_ELEM_CAT_ONLON;
				}
				if ( pCA->pInputState->IGState_Mouse.Buttons[0] == 2)
				{
					iCategory = IFC_ELEM_CAT_ONLDOWN;
				}
				if ( pCA->pInputState->IGState_Mouse.Buttons[0] == 3)
				{
					iCategory = IFC_ELEM_CAT_ONLUP;
				}

				//execute commands
			PCMD2* pCmdTrav = pElem->pCmds[iCategory];

				while (pCmdTrav != NULL)
				{
					//execute
					//broadcast the command if its actually an gameworld command
					//additionally, issue it through the CommQueue(so that it stays there and can be called at the right time)
					//since it is dependent on the exec tick
					if (pCmdTrav->wType <= NUM_CQ2COMMANDS)
					{
						pCA->pCommQueue->Issue_Command_Q2(pCmdTrav->wType, pCmdTrav->pData, pCmdTrav->wDataSize, TRUE);
						pCA->pDP_Multiplay->Send_Command2(pCmdTrav->wType, pCmdTrav->pData, pCmdTrav->wDataSize, FALSE, TRUE);
					} else
					{
						pCA->pCommQueue->Execute_Command2(pCmdTrav);

					}
					pCmdTrav = pCmdTrav->pNext;
				}
	
				//check child elements
				Process_Interface_Elements_B(pElem->pChildren);
			}
		}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Clear_SBs()
{

	//destroy the USB(the unit selection buffer, NOT the universal serial bus! :-)
	//set the traversers
	LPSBE pUSB_Trav = pUSB_First;

	while (pUSB_Trav != NULL)
	{
		//advance
		LPSBE pUSB_Trav2 = (LPSBE)pUSB_Trav->pNext;
		//destroy
		delete(pUSB_Trav);
		//re-assign
		pUSB_Trav = pUSB_Trav2;
	}

	//re-use the traversers, they're the same anyway
	pUSB_Trav = pSSB_First;

	while (pUSB_Trav != NULL)
	{
		//advance
		LPSBE pUSB_Trav2 = (LPSBE)pUSB_Trav->pNext;
		//destroy
		delete(pUSB_Trav);
		//re-assign
		pUSB_Trav = pUSB_Trav2;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Reset_CommandFlags(WORD wType)
{
	LPSBE pUSB_TravL = (LPSBE)pUSB_First->pNext;

	while (pUSB_TravL != pUSB_Last)
	{
		if (pUSB_TravL->iCommandFlag == wType)
			pUSB_TravL->iCommandFlag = -1;
		pUSB_TravL = (LPSBE)pUSB_TravL->pNext;

	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
SQCOORDSET cInterface::Calc_PosMap(int iXPos, int iYPos)
{
//calculates what map square this screen coordinate is over
	
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	SQCOORDSET RetSQ;

	RetSQ.x = ((iXPos - IBoxes[IBOX_PRIMARY].BoxPos.left) / TILE_SIZE) + map_xview; //once again, +1 to add the partial-square the cursor is OVER, -1 because indexing starts at 0...hence...NOTHING
	RetSQ.y = ((iYPos - IBoxes[IBOX_PRIMARY].BoxPos.top)/ TILE_SIZE) + map_yview;

	RetSQ.Calc_Index(pCA->pGameWorld->iWidth);  //this might or might not be necessary...not sure yet 05.06.2002
												//this line is now used in Process_Input (05.31.2003)

	return RetSQ;	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
COORDSET cInterface::Calc_PosMap_ABS(int iXPos, int iYPos)
{
//calculates the absolute map coordinates this screen coord is over

	COORDSET RetCR;

	RetCR.x = iXPos - IBoxes[IBOX_PRIMARY].BoxPos.left + (map_xview * TILE_SIZE);
	RetCR.y = iYPos - IBoxes[IBOX_PRIMARY].BoxPos.top + (map_yview * TILE_SIZE);


	return RetCR;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
LPRECT cInterface::Fix_RectBox(LPRECT pInRect)
{
	int iTempStore;

	//just two things (if there is an incorrect arrangement, flip values
	if (pInRect->right < pInRect->left)
	{
		iTempStore = pInRect->right;
		pInRect->right = pInRect->left;
		pInRect->left = iTempStore;
	}

	if (pInRect->bottom < pInRect->top)
	{
		iTempStore = pInRect->top;
		pInRect->top = pInRect->bottom;
		pInRect->bottom = iTempStore;
	}

	return pInRect;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::AddUnits_SB_stack(int iMapIndex, BOOL ControlFlag, LPSBE pFirst, LPSBE pLast)   //05.14.2002   //changed to 
{
	cUnitA* pUS_Trav; //unit stack traverser
	LPSBE	pBufferEntry, pBufferNext;
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	pUS_Trav = (cUnitA*)pCA->pGameWorld->pMap[iMapIndex].pOccupant_Unit;

	while (pUS_Trav != NULL)
	{
		//only add if the unit is a friendly (or, the ControlFlag is OFF)
		if (pUS_Trav->iControl == pCA->pGameOptions->iControl || ControlFlag == FALSE)

		{
			//allocate a new buffer entry
			pBufferEntry = new SBE;
			//fill with appropriate values
			pBufferEntry->iID = pUS_Trav->iID;
			pBufferEntry->iCommandFlag = -1;
			//perform insertion into USB
			pBufferNext = (LPSBE)pFirst->pNext;
			pBufferEntry->pNext = (VOID*)pBufferNext;
			pBufferEntry->pPrev = (VOID*)pFirst;
			pFirst->pNext = (VOID*)pBufferEntry;
			pBufferNext->pPrev = (VOID*)pBufferEntry;
		}
			//advance pUS_Trav
			pUS_Trav = (cUnitA*)pUS_Trav->pNext_Stack;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cInterface::AddUnits_SB_single(int iMapIndex, LPSBE pFirst, LPSBE pLast)  //05.14.2002
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cUnitA* pUS_Entry = (cUnitA*)pCA->pGameWorld->pMap[iMapIndex].pOccupant_Unit;
	LPSBE pBufferEntry = new SBE;
	LPSBE pBufferNext;
	

	int iNumSelected = 0;

	//attempt to enter this unit into the unit selection buffer if it exists
	if (pUS_Entry != NULL)
	{
		iNumSelected++;
		//fill with appropriate values
		pBufferEntry->iID = pUS_Entry->iID;
		pBufferEntry->iCommandFlag = -1;
		//perform insertion into USB
		pBufferNext = (LPSBE)pFirst->pNext;
		pBufferEntry->pNext = (VOID*)pBufferNext;
		pBufferEntry->pPrev = (VOID*)pUSB_First;
		pUSB_First->pNext = (VOID*)pBufferEntry;
		pBufferNext->pPrev = (VOID*)pBufferEntry;
	}

return iNumSelected;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cInterface::AddUnits_SB_singleB(int iID, LPSBE pFirst, LPSBE pLast)  //05.14.2002
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cUnitA* pUS_Entry = pCA->pUnitTable->pUnitTable[iID];
	LPSBE pBufferEntry = new SBE;
	LPSBE pBufferNext;
	
	int iNumSelected = 0;

	//attempt to enter this unit into the unit selection buffer if it exists
	if (pUS_Entry != NULL)
	{
		iNumSelected++;
		//fill with appropriate values
		pBufferEntry->iID = pUS_Entry->iID;
		pBufferEntry->iCommandFlag = -1;

		//perform insertion into USB
		pBufferNext = (LPSBE)pFirst->pNext;

		pBufferEntry->pNext = (VOID*)pBufferNext;
		pBufferEntry->pPrev = (VOID*)pUSB_First;

		pUSB_First->pNext = (VOID*)pBufferEntry;
		pBufferNext->pPrev = (VOID*)pBufferEntry;

	}

return iNumSelected;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cInterface::AddStructs_SB_single(int iMapIndex, LPSBE pFirst, LPSBE pLast)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cStructureA* pSS_Entry = (cStructureA*)pCA->pGameWorld->pMap[iMapIndex].pOccupant_Structure;
	LPSBE pBufferEntry = new SBE;
	LPSBE pBufferNext;

	int iNumSelected = 0;

	//attempt to enter this unit into the unit selection buffer if it exists
	if (pSS_Entry != NULL)
	{
		iNumSelected++;
		//fill with appropriate values
		pBufferEntry->iID = pSS_Entry->iID;
		pBufferEntry->iCommandFlag = -1;
		//perform insertion into USB
		pBufferNext = (LPSBE)pFirst->pNext;
		pBufferEntry->pNext = (VOID*)pBufferNext;
		pBufferEntry->pPrev = (VOID*)pSSB_First;
		pSSB_First->pNext = (VOID*)pBufferEntry;
		pBufferNext->pPrev = (VOID*)pBufferEntry;
	}	

return iNumSelected;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cInterface::AddStructs_SB_singleB(int iID, LPSBE pFirst, LPSBE pLast)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cStructureA* pSS_Entry = (cStructureA*)pCA->pStructIDTable->pTable[iID];
	LPSBE pBufferEntry = new SBE;
	LPSBE pBufferNext;

	int iNumSelected = 0;

	//attempt to enter this unit into the unit selection buffer if it exists
	if (pSS_Entry != NULL)
	{
		iNumSelected++;
		//fill with appropriate values
		pBufferEntry->iID = pSS_Entry->iID;
		pBufferEntry->iCommandFlag = -1;
		//perform insertion into USB
		pBufferNext = (LPSBE)pFirst->pNext;
		pBufferEntry->pNext = (VOID*)pBufferNext;
		pBufferEntry->pPrev = (VOID*)pSSB_First;
		pSSB_First->pNext = (VOID*)pBufferEntry;
		pBufferNext->pPrev = (VOID*)pBufferEntry;
	}	

return iNumSelected;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::ReleaseUnits_SB_all(LPSBE pFirst, LPSBE pLast)   //05.14.2002
{
	//NOTE: function was wreaking havoc on the buffers when I didn't
	//specify pUSB_First and pUSB_Last explicitly.
	//so now the function just ignores the other two passed parameters

	LPSBE pSB_Trav = (LPSBE)pUSB_First->pNext;
	LPSBE pSB_Next = (LPSBE)pSB_Trav->pNext;


	//delete the entire buffer, then re-link the anchors afterwards
	while (pSB_Trav != pUSB_Last)
	{
		delete(pSB_Trav);
		pSB_Trav = pSB_Next;
		pSB_Next = (LPSBE)pSB_Next->pNext;
	}

	//now re-link the anchors
	pUSB_First->pNext = (VOID*)pUSB_Last;
	pUSB_Last->pPrev = (VOID*)pUSB_First;
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::ReleaseStructs_SB_all(LPSBE pFirst, LPSBE pLast)   //03.29.2003
{
	//IDENTICAL CODE from ReleaseUnits, as of 03.29.2003
	//now ignores the passed pointers in favor of the KNOWN pSSB pointers
	//I suspect the problem may be due to pInterface possibly calling pCommQueue
	//which then calls back pInterface ?
	LPSBE pSB_Trav = (LPSBE)pSSB_First->pNext;
	LPSBE pSB_Next = (LPSBE)pSB_Trav->pNext;
	//delete the entire buffer, then re-link the anchors afterwards
	while (pSB_Trav != pLast)
	{
		delete(pSB_Trav);
		pSB_Trav = pSB_Next;
		pSB_Next = (LPSBE)pSB_Next->pNext;
	}

	//now re-link the anchors
	pSSB_First->pNext = (VOID*)pSSB_Last;
	pSSB_Last->pPrev = (VOID*)pSSB_First;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Process_SBs()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	//just make sure no invalid units are in the USB(that have been destroyed while selected)
	LPSBE pUSB_Trav = (LPSBE)pUSB_First->pNext;

	while (pUSB_Trav != pUSB_Last)
	{
		LPSBE pUSB_Trav2 = (LPSBE)pUSB_Trav->pNext;
		//erase pConsole_Unit_Select if it is pointing to this selection
		if (pCA->pUnitTable->pUnitTable[pUSB_Trav->iID] == NULL)
		{
			if (pConsole_Unit_Select == (void*)pUSB_Trav)
				pConsole_Unit_Select = NULL;
			ReleaseUnits_SB_single(pUSB_Trav);
		} else if (pCA->pUnitTable->pUnitTable[pUSB_Trav->iID]->bLoaded == TRUE) //if the unit has been loaded, also release it
		{
			if (pConsole_Unit_Select == (void*)pUSB_Trav)
				pConsole_Unit_Select = NULL;
			ReleaseUnits_SB_single(pUSB_Trav);					
		}
		pUSB_Trav = pUSB_Trav2;
	}

	//analogous with structures(re-use the traversers)
	pUSB_Trav = (LPSBE)pSSB_First->pNext;

	while (pUSB_Trav != pSSB_Last)
	{
		LPSBE pUSB_Trav2 = (LPSBE)pUSB_Trav->pNext;
		//erase pConsole_Structure_Select if it is pointing to this selection
		if (pCA->pStructIDTable->pTable[pUSB_Trav->iID] == NULL)
		{
			if (pConsole_Struct_Select == (void*)pUSB_Trav)
				pConsole_Struct_Select = NULL;
			ReleaseStructs_SB_all(pSSB_First, pSSB_Last);
		}
		pUSB_Trav = pUSB_Trav2;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::ReleaseUnits_SB_single(LPSBE pUS_Entry)
{
	LPSBE pSB_Prev = (LPSBE)pUS_Entry->pPrev;
	LPSBE pSB_Next = (LPSBE)pUS_Entry->pNext;

	//seal around
	pSB_Prev->pNext = (VOID*)pSB_Next;
	pSB_Next->pPrev = (VOID*)pSB_Prev;

	//delete the entry
	delete(pUS_Entry);

}

//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Process_PendingCommand()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

//error check. do not process if pending command doesn't exist(05.31.2003)
if (pPendingCommand != NULL)
{
	//basically, peek in the IPB for entries
	BOOL bRet = pPendingCommand->PeekAcquire_IPB(FALSE);

	if (bRet == TRUE)
	{	
		Write_Log("Data Packet Acquired. from IPB");
		WriteLog_Enter();

	}

	//TO GO HERE: if the entry that the pending command on is now 0 (NO FLAGS ENABLED = DATA ACQUISITION COMPLETE)
	//begin command compilation and issue, and exit
	if (pCA->pCmdReqTable->CommandTypes[pPendingCommand->iRequestIndex].Sequence[pPendingCommand->iCurrentSequenceNumber] == 0)
	{	
		//DEBUG
		Write_Log("Data Acquisition Completed. Beginning command compilation");
		WriteLog_Enter();

		pPendingCommand->Compile_Command2();

		pPendingCommand->Exit();
		delete(pPendingCommand);
		pPendingCommand = NULL;
	}
}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Initiate_PendingCommand(DWORD dwType)
{
cClassAccess* pCA = (cClassAccess*)pCAccess;

	//start a pending command
	//allocate
	pPendingCommand = new cPendingCommand; 
	pPendingCommand->Init(pCAccess, dwType);
	//reset cutoff time
	llCutOffTime = pCA->pTimer->iCurrTick;

	//scan the SB here for matching entries
	if (pPendingCommand->PeekAcquire_SB() == TRUE)
	{
		//DEBUG
		Write_Log("Data Packet Acquired from SB");
		WriteLog_Enter();

	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Process_MainMenu()
{
cClassAccess* pCA = (cClassAccess*)pCAccess;
WORD	wGridIndex;
IGRIDCELL* pGrid; //the array of cells
cIGrid* pIGrid;  //the entire grid
char NText[50];

//update MPConnect display
	Update_MPConnect_Display(80);

//process commands when inside main menu
//if mouse is inside main menu box
	if (pCA->pInputState->IGState_Mouse.iXPos <= IBoxes[IBOX_MAINMENU].BoxPos.right &&
		pCA->pInputState->IGState_Mouse.iYPos <= IBoxes[IBOX_MAINMENU].BoxPos.bottom &&
		pCA->pInputState->IGState_Mouse.iYPos >= IBoxes[IBOX_MAINMENU].BoxPos.top &&
		pCA->pInputState->IGState_Mouse.iXPos >= IBoxes[IBOX_MAINMENU].BoxPos.left)
//if main menu box is active
	if (IBoxes[IBOX_MAINMENU].bActiveBox == TRUE)
	{
		pIGrid = &IBoxes[IBOX_MAINMENU].pIGrids[IBoxes[IBOX_MAINMENU].iActiveGrid];
		pGrid = pIGrid->pGrid;

		//process interface elements
		Process_Interface_Elements(pIGrid->pIfcElem_First, pIGrid->pIfcElem_Last);


//if mouse click was detected(mouse down -> code 2)
//Disabled to prevent crash in VC6 (2008.11.28)
/*	if (pCA->pInputState->IGState_Mouse.Buttons[0] == 2)
	{
		//calc the index that the mouse is over on the active grid
		wGridIndex = pIGrid->Calc_GridIndex(pCA->pInputState->IGState_Mouse.iXPos, pCA->pInputState->IGState_Mouse.iYPos);
		//execute command, if active
		if (pGrid[wGridIndex].bActiveCommand == TRUE)
		{
			//if DSOURCE_ALLCELL, execute command from cell data only
			if (pGrid[wGridIndex].bDataSources == IGRIDCELL_DSOURCE_ALLCELL)
				pCA->pCommQueue->Issue_Command(pGrid[wGridIndex].ProgCommand.wDest, pGrid[wGridIndex].ProgCommand.wType,
												pGrid[wGridIndex].ProgCommand.wParam1, pGrid[wGridIndex].ProgCommand.wParam2,
												pGrid[wGridIndex].ProgCommand.wParam3, pGrid[wGridIndex].ProgCommand.fParam4,
												pGrid[wGridIndex].ProgCommand.fParam5, pGrid[wGridIndex].ProgCommand.pParam6, 
												pGrid[wGridIndex].ProgCommand.lExecTick);


		}
	} */
	} 

//if key is pressed, edit strings
	for (int lc11 = 0; lc11 < 256; lc11++)
	if (pCA->pInputState->IGState_Keyboard[lc11] == 2)    //edit on keydown
	{
		if (pKeyEdit != NULL)
			Edit_String(lc11, pKeyEdit, dwKeyEditMaxBuffer);
		if (pKeyEdit_Wide != NULL)
			Edit_String_Wide(lc11, pKeyEdit_Wide, dwKeyEditWideMaxBuffer);
		//if enter key is pressed, text entry is done
		if (lc11 == DIK_RETURN)
		{
			BOOL bDoNotClear = FALSE;
			if ( IBoxes[IBOX_MAINMENU].bActiveBox == TRUE)
				if ( IBoxes[IBOX_MAINMENU].iActiveGrid == TEMP_IGRID_MULTI3)
				{
					if (pKeyEdit != NULL)
					{
						pCA->pDP_Multiplay->Send_ChatText(pKeyEdit, pCA->pGameOptions->InfoPlayer_Local.pwszName, dwKeyEditMaxBuffer );
						pKeyEdit[0] = 0;
						bDoNotClear = TRUE;
					} 
				}
				if(bDoNotClear == FALSE)
					pKeyEdit = NULL;
			pKeyEdit_Wide = NULL;
		}
	}

	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Update_MainMenu(int iGridNum)
{
IGRIDCELL* pGrid;
cClassAccess* pCA = (cClassAccess*)pCAccess;

cIfcElement* pElem = NULL;
PCMD2* pCmd = NULL;
int iretsize = 0;

int XRes = pCA->pDD_Display->i_XResD;
int YRes = pCA->pDD_Display->i_YResD;


//if this is the single player or the multiplayer map select grid, do the same thing(populate the listbox)
	if (iGridNum == TEMP_IGRID_SINGLE)
	{

		//
	pGrid = IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SINGLE].pGrid;


		//update the map name
		//update the map name
if (pCA->pGameOptions->iMapIndex >= 0)
	pMapNameSelect = pCA->pAppData->achMapNames[pCA->pGameOptions->iMapIndex];
else
	pMapNameSelect = (char*)"None Selected";

		cIfcElement* pElemNew;


		if (pSPMapsListBox != NULL)
		{
			int xspan = pSPMapsListBoxFrontEnd->Rect_Pos.right - pSPMapsListBoxFrontEnd->Rect_Pos.left - 10;

			pSPMapsListBox->Clear_IfcElems();

			//list out all of the maps
			for (int c901 = 0; c901 <= pCA->pAppData->iHighestMapIndex; c901++)
			{

				pElemNew = new cIfcElement;
				pElemNew->Init(0, xspan, 0, pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2);
				
				PCMD2* pCmdNew = NULL;

				 if (c901 == pCA->pGameOptions->iMapIndex)
				 {

					pCmdNew = new PCMD2;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->DetColor(0,0, 0.5), pElemNew->Rect_Pos.left,pElemNew->Rect_Pos.right,pElemNew->Rect_Pos.top,pElemNew->Rect_Pos.bottom, &iretsize);
					pCmdNew->wDataSize = iretsize;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
				 }

				pCmdNew = new PCMD2;
				CommDataDrawText2* newBuffer = new CommDataDrawText2;
				pCA->pCommQueue->Assemble_CommData_DrawText2ex(newBuffer, 0, RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, pCA->pAppData->achMapNames[c901]);
				pCmdNew->pData =  reinterpret_cast<BYTE*>(newBuffer);
				pCmdNew->wDataSize = sizeof(CommDataDrawText2);
				pCmdNew->wType = CQ2TYPE_DRAWTEXT2;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
				pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1]; DWORD* pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = c901;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SELECT_MAP;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
				pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->WinBlue1, pElemNew->Rect_Pos.left,pElemNew->Rect_Pos.right,pElemNew->Rect_Pos.top,pElemNew->Rect_Pos.bottom, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_COLORFILL;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

				 if (c901 == pCA->pGameOptions->iMapIndex)
				 {
					pCmdNew = new PCMD2;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->DetColor(0,0, 0.5), pElemNew->Rect_Pos.left,pElemNew->Rect_Pos.right,pElemNew->Rect_Pos.top,pElemNew->Rect_Pos.bottom, &iretsize);
					pCmdNew->wDataSize = iretsize;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
				 }

				//clone the text command for ONHOVER_DRAW also
				pCmdNew = new PCMD2;
				newBuffer = new CommDataDrawText2;
				pCA->pCommQueue->Assemble_CommData_DrawText2ex(newBuffer, 0, RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, pCA->pAppData->achMapNames[c901]);
				pCmdNew->pData = reinterpret_cast<BYTE*>(newBuffer);
				pCmdNew->wDataSize = sizeof(CommDataDrawText2);
				pCmdNew->wType = CQ2TYPE_DRAWTEXT2;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

				pSPMapsListBox->Add_IfcElem(pElemNew);

			}
		}
/*
		//this is a 5x20 grid (at standard IBOX size 640x480)
		Set_IGridCell_DrawFlags(&pGrid[2], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"SINGLE-PLAYER", FONT_STANDARD1);
		Set_IGridCell_DrawFlags(&pGrid[8], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Return", FONT_STANDARD1);	
	
		Set_IGridCell_CmdFlags_Box(pGrid, 8, 1, 1, 5, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SET_MAINMENUMODE, 0,
								   0, 0, 0, 0, NULL, 0);
		Set_IGridCell_DrawFlags(&pGrid[26], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBOXES00, 0, 0, 128, 0, 360, NULL, 0);
	
		Set_IGridCell_DrawFlags(&pGrid[21], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "CHOOSE MAP:", FONT_STANDARD1);
		Set_IGridCell_DrawFlags(&pGrid[22], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, pCA->pAppData->achMapNames[pCA->pGameOptions->iMapIndex], FONT_STANDARD1);
	
		
		//list maps
		for (int c901 = 0; c901 <= pCA->pAppData->iHighestMapIndex; c901++)
		{
			Set_IGridCell_DrawFlags(&pGrid[31 + 5*c901], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, pCA->pAppData->achMapNames[c901], FONT_STANDARD1);
			Set_IGridCell_CmdFlags_Box( pGrid, 31 + 5*c901, 1, 1, 5, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SELECT_MAP, c901, 0, 0, 0, 0, NULL, 0);
		}

		//a start button
		Set_IGridCell_DrawFlags(&pGrid[13], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBUTTONS00, 0, 0, MBUTTON_SIZE_X - 1, MBUTTON_SIZE_Y * 6, MBUTTON_SIZE_Y * 7, NULL, 0);
		Set_IGridCell_CmdFlags_Box(pGrid, 13, 1, 2, 5, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SELECT_START, 0, 0, 0, 0, 0, NULL, 0);
*/
	}
	if (iGridNum == TEMP_IGRID_MULTI)
	{
		Update_MPConnect_Display(80);
	}
	if (iGridNum == TEMP_IGRID_MULTI4)
	{
		//
	pGrid = IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI4].pGrid;


		//update the map name
if (pCA->pGameOptions->iMapIndex >= 0)
	pMapNameSelect = pCA->pAppData->achMapNames[pCA->pGameOptions->iMapIndex];
else
	pMapNameSelect = (char*)"None Selected";

		cIfcElement* pElemNew;

		if (pMPMapsListBox != NULL)
		{
			int xspan = pMPMapsListBoxFrontEnd->Rect_Pos.right - pMPMapsListBoxFrontEnd->Rect_Pos.left - 10;

			pMPMapsListBox->Clear_IfcElems();

			//list out all of the maps
			for (int c901 = 0; c901 <= pCA->pAppData->iHighestMapIndex; c901++)
			{

				pElemNew = new cIfcElement;
				pElemNew->Init(0, xspan, 0, pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2);
				
				PCMD2* pCmdNew = NULL;

				 if (c901 == pCA->pGameOptions->iMapIndex)
				 {

					pCmdNew = new PCMD2;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->DetColor(0,0, 0.5), pElemNew->Rect_Pos.left,pElemNew->Rect_Pos.right,pElemNew->Rect_Pos.top,pElemNew->Rect_Pos.bottom, &iretsize);
					pCmdNew->wDataSize = iretsize;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
				 }

				pCmdNew = new PCMD2;
				 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, 0, RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, pCA->pAppData->achMapNames[c901]);
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
				pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1]; DWORD* pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = c901;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SELECT_MAP;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
				pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->WinBlue1, pElemNew->Rect_Pos.left,pElemNew->Rect_Pos.right,pElemNew->Rect_Pos.top,pElemNew->Rect_Pos.bottom, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_COLORFILL;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

				 if (c901 == pCA->pGameOptions->iMapIndex)
				 {
					pCmdNew = new PCMD2;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->DetColor(0,0, 0.5), pElemNew->Rect_Pos.left,pElemNew->Rect_Pos.right,pElemNew->Rect_Pos.top,pElemNew->Rect_Pos.bottom, &iretsize);
					pCmdNew->wDataSize = iretsize;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
				 }

				//clone the text command for ONHOVER_DRAW also
					pCmdNew = new PCMD2;
				 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, 0, RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, pCA->pAppData->achMapNames[c901]);
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

				pMPMapsListBox->Add_IfcElem(pElemNew);

			}
		}
	}
	if (iGridNum == TEMP_IGRID_BRIEFING)
	{
		//

		PCMD2* pCmdNew = NULL;
		cIfcElement* pElemNew = NULL;

		//if we're in multiplay
		if (pCA->pGameOptions->bGameType == GAMETYPE_MULTI)
		{
			pBriefingPlayerListBox->Clear_IfcElems();
			//then fill with names
			for (int lc55 = 0; lc55 < TEMP_DP_MAXPLAYERS; lc55++)
			{
				if (pCA->pTimer->iTickReportIDCodes[lc55] > 0)
				if (pCA->pTimer->iControls[lc55] >= 0)
				{		
					char Text[256];
					ZeroMemory(Text, sizeof(char)*256);
					strcat(Text, pCA->pTimer->Names[lc55]);
					strcat(Text, ": ");
					if (pCA->pNationManager->ppNations[ pCA->pTimer->iControls[lc55] ] != NULL)
					strcat(Text, pCA->pNationManager->ppNations[ pCA->pTimer->iControls[lc55] ]->Name);
					//strcat(Text, "Banana");
					//itoa(pCA->pTimer->iControls[lc55], Text, 10);

					pElemNew = new cIfcElement;
					pElemNew->Init(0, pBriefingPlayerListBoxFrontEnd->Rect_Pos.right -  pBriefingPlayerListBoxFrontEnd->Rect_Pos.left,
							   0, pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2);

						pCmdNew = new PCMD2;
						pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, FONT_STANDARD1, pCA->pColorTable->White - 1, strlen(Text), Text, &iretsize);
						pCmdNew->wDataSize = iretsize;
						pCmdNew->wType = CQ2TYPE_DRAWTEXT;
						pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);	
					pBriefingPlayerListBox->Add_IfcElem(pElemNew);
				}
			}
		}
		//now clear all current elements from the listbox data and adding the nation names
		pBriefingNationSelectListBox->Clear_IfcElems();

		for (int lc56 = 0; lc56 < TEMP_DP_MAXPLAYERS; lc56++)
		{
			if ( pCA->pNationManager->ppNations[lc56] != NULL)
			if ( pCA->pNationManager->ppNations[lc56]->bHumanPlayable == TRUE)
			{
				pElemNew = new cIfcElement;
				pElemNew->Init(0, pBriefingNationSelectListBoxFrontEnd->Rect_Pos.right -  pBriefingNationSelectListBoxFrontEnd->Rect_Pos.left,
							   0, pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2);



				 if (lc56 == pCA->pGameOptions->iControl)
				 {
					pCmdNew = new PCMD2;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->DetColor(0,0, 0.5), pElemNew->Rect_Pos.left,pElemNew->Rect_Pos.right,pElemNew->Rect_Pos.top,pElemNew->Rect_Pos.bottom, &iretsize);
					pCmdNew->wDataSize = iretsize;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
				 }
					pCmdNew = new PCMD2;
					CommDataDrawText2* newBuffer = new CommDataDrawText2;
					pCA->pCommQueue->Assemble_CommData_DrawText2ex(newBuffer, 0, RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, pCA->pNationManager->ppNations[lc56]->Name);
					pCmdNew->pData = reinterpret_cast<BYTE*>(newBuffer);
					pCmdNew->wDataSize = sizeof(CommDataDrawText2);
					pCmdNew->wType = CQ2TYPE_DRAWTEXT2;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);


				pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new int[1]; int* piTrav = (int*)pCmdNew->pData; *piTrav = lc56;
				pCmdNew->wDataSize = sizeof(int);
				pCmdNew->wType = CQ2TYPE_SET_IDCONTROL;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);

				pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->WinBlue1, pElemNew->Rect_Pos.left,pElemNew->Rect_Pos.right,pElemNew->Rect_Pos.top,pElemNew->Rect_Pos.bottom, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_COLORFILL;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

				 if (lc56 == pCA->pGameOptions->iControl)
				 {
					pCmdNew = new PCMD2;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->DetColor(0,0, 0.5), pElemNew->Rect_Pos.left,pElemNew->Rect_Pos.right,pElemNew->Rect_Pos.top,pElemNew->Rect_Pos.bottom, &iretsize);
					pCmdNew->wDataSize = iretsize;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
				 }

				//clone the text command for ONHOVER_DRAW also
					pCmdNew = new PCMD2;
					newBuffer = new CommDataDrawText2;
					pCA->pCommQueue->Assemble_CommData_DrawText2ex(newBuffer, 0, RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, pCA->pNationManager->ppNations[lc56]->Name);
					pCmdNew->pData = reinterpret_cast<BYTE*>(newBuffer);
					pCmdNew->wDataSize = sizeof(CommDataDrawText2);
					pCmdNew->wType = CQ2TYPE_DRAWTEXT2;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

				pBriefingNationSelectListBox->Add_IfcElem(pElemNew);
			}
		}

	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Init_MainMenu()
{
cClassAccess* pCA = (cClassAccess*)pCAccess;
char NText[50];
int lc1, lc2; //local counters

	//possibly a temp hardcode of Main Menu data, until some other way can be figured of doing it...

//find the difference between the resolution and 640x480 and move the IBOX such that it moves to the middle of the screen
int DX = pCA->pDD_Display->i_XResD;
int DY = pCA->pDD_Display->i_YResD;
int XRes = pCA->pDD_Display->i_XResD;
int YRes = pCA->pDD_Display->i_YResD;

	IBoxes[IBOX_MAINMENU].BoxPos.top = pCA->pDD_Display->i_YResD * 0;
	IBoxes[IBOX_MAINMENU].BoxPos.bottom = pCA->pDD_Display->i_YResD * 1;
	IBoxes[IBOX_MAINMENU].BoxPos.left = pCA->pDD_Display->i_XResD * 0;
	IBoxes[IBOX_MAINMENU].BoxPos.right = pCA->pDD_Display->i_XResD * 1;

	//init one grid for the main menu
	IBoxes[IBOX_MAINMENU].pIGrids = new cIGrid[20]; //init 10 IGrids for the main menu
	//call init for the grids
	//first grid(only 6x20 grid needed for main menu)
	IBoxes[IBOX_MAINMENU].pIGrids[0].Init(IBoxes[IBOX_MAINMENU].BoxPos.left, IBoxes[IBOX_MAINMENU].BoxPos.top, 6, 20, DX / 6.0, (DY / 20.0));
	//set single player grid to 5x40
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SINGLE].Init(IBoxes[IBOX_MAINMENU].BoxPos.left, IBoxes[IBOX_MAINMENU].BoxPos.top, 5, 40, (DX / 5.0), (DY / 40.0));
	//multiplayer grid, 200x120
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI].Init(IBoxes[IBOX_MAINMENU].BoxPos.left, IBoxes[IBOX_MAINMENU].BoxPos.top, 200, 120, (DX / 200.0), (DY / 120.0));
	//multiplayer 2A grid, 1x1
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2A].Init(IBoxes[IBOX_MAINMENU].BoxPos.left, IBoxes[IBOX_MAINMENU].BoxPos.top, 1, 1, 1, 1);
	//multiplayer SERIAL grid
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SERIAL].Init(IBoxes[IBOX_MAINMENU].BoxPos.left, IBoxes[IBOX_MAINMENU].BoxPos.top, 1, 1, 1, 1);
	//multiplayer MODEM grid
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MODEM].Init(IBoxes[IBOX_MAINMENU].BoxPos.left, IBoxes[IBOX_MAINMENU].BoxPos.top, 1, 1, 1, 1);
	//multiplayer TCPIP grid
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_TCPIP].Init(IBoxes[IBOX_MAINMENU].BoxPos.left, IBoxes[IBOX_MAINMENU].BoxPos.top, 1, 1, 1, 1);
	//multiplayer 2B grid, 1x1
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2B].Init(IBoxes[IBOX_MAINMENU].BoxPos.left, IBoxes[IBOX_MAINMENU].BoxPos.top, 1, 1, 1, 1);
	//options grid, for now, also same size
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_OPTIONS].Init(IBoxes[IBOX_MAINMENU].BoxPos.left, IBoxes[IBOX_MAINMENU].BoxPos.top, 200, 120, (DX / 200.0),(DY / 200.0));
	//multi2 grid, initialize to 40x30 boxes 
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2].Init(IBoxes[IBOX_MAINMENU].BoxPos.left, IBoxes[IBOX_MAINMENU].BoxPos.top, 40, 30, (DX / 40.0), (DY / 30.0));
	//multi3 grid, initialize to 1x1
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI3].Init(IBoxes[IBOX_MAINMENU].BoxPos.left, IBoxes[IBOX_MAINMENU].BoxPos.top, 1, 1, 1, 1);
	//init One grid for the game briefing
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_BRIEFING].Init(IBoxes[IBOX_MAINMENU].BoxPos.left, IBoxes[IBOX_MAINMENU].BoxPos.top, 6, 20, 10, 10);
	//init One grid for the multiplayer map selection
	IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI4].Init(IBoxes[IBOX_MAINMENU].BoxPos.left, IBoxes[IBOX_MAINMENU].BoxPos.top, 6, 20, 10, 10);

	//set the active grid to 0 (the first screen)
	IBoxes[IBOX_MAINMENU].iActiveGrid = 0;

	//set box to active
	IBoxes[IBOX_MAINMENU].bActiveBox = TRUE;

IGRIDCELL* pGrid = IBoxes[IBOX_MAINMENU].pIGrids[0].pGrid;

	cIfcElement* pElemNew;
	PCMD2* pCmdNew;
	int iretsize;
	int retsize;
	int strlength;

	//add interface element(interface panel graphic)
	pElemNew = new cIfcElement;	
	pElemNew->Init(DX*0.05, DX*0.3, DY*0.05, DY*0.5);
	pCmdNew = new PCMD2;
	pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcPanel(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.right, pElemNew->Rect_Pos.top, pElemNew->Rect_Pos.bottom, &iretsize);  
	pCmdNew->wDataSize = iretsize;
	pCmdNew->wType = CQ2TYPE_DRAWIFCPANEL;
	pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
	IBoxes[IBOX_MAINMENU].pIGrids[0].Add_IfcElem(pElemNew);

	//add interface element(title)
	pElemNew = new cIfcElement;	
	pElemNew->Init(DX*0.5, DX*0.51, DY*0.05, DY*0.07);
	pCmdNew = new PCMD2;
	pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, FONT_STANDARD1, pCA->pColorTable->DetColor(0, 1, 0), 9, "MAIN MENU", &iretsize);  
	pCmdNew->wDataSize = iretsize;
	pCmdNew->wType = CQ2TYPE_DRAWTEXT;
	pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
	IBoxes[IBOX_MAINMENU].pIGrids[0].Add_IfcElem(pElemNew);

	//add an interface element to this grid (single player button)
	pElemNew = Create_Button("SINGLE PLAYER", XRes * 0.08, YRes * 0.08);
		//add second command
		retsize = 0;
		pCmdNew = new PCMD2;
		pCmdNew->pData = new BYTE[1]; //dummy byte (currently this command doesn't take any parameters)
		pCmdNew->wDataSize = 1;
		pCmdNew->wType = CQ2TYPE_SELECT_GAMETYPE_SINGLE;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
	//if there's more than 1 map available, than issue a set to map 0 command
		if (pCA->pAppData->iHighestMapIndex > 0)
		{	
			pCmdNew = new PCMD2;
				DWORD* pdwTrav = new DWORD[1];
				*pdwTrav = 0;
			pCmdNew->pData = (BYTE*) pdwTrav; //(1 DWORD - the mode number
			pCmdNew->wDataSize = 1;
			pCmdNew->wType = CQ2TYPE_SELECT_MAP;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);			
		}

	IBoxes[IBOX_MAINMENU].pIGrids[0].Add_IfcElem(pElemNew);

	//add an interface element to this grid (multi player button)
		pElemNew = Create_Button("MULTI-PLAYER", XRes * 0.08, YRes * 0.18);
		//add second command
		retsize = 0;
		pCmdNew = new PCMD2;
		pCmdNew->pData = new BYTE[1]; //dummy byte (currently this command doesn't take any parameters)
		pCmdNew->wDataSize = 1;
		pCmdNew->wType = CQ2TYPE_SELECT_GAMETYPE_MULTI;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
	//if there's more than 1 map available, than issue a set to map 0 command
		if (pCA->pAppData->iHighestMapIndex > 0)
		{	
			pCmdNew = new PCMD2;
				DWORD* pdwTrav = new DWORD[1];
				*pdwTrav = 0;
			pCmdNew->pData = (BYTE*) pdwTrav; //(1 DWORD - the mode number
			pCmdNew->wDataSize = 1;
			pCmdNew->wType = CQ2TYPE_SELECT_MAP;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);			
		}
	IBoxes[IBOX_MAINMENU].pIGrids[0].Add_IfcElem(pElemNew);

	//add an interface element to this grid (options button)
		pElemNew = Create_Button("OPTIONS", XRes * 0.08, YRes * 0.28);
		//add second command
		retsize = 0;
		pCmdNew = new PCMD2;
		DWORD* pdwTrav = new DWORD[1];
			*pdwTrav = TEMP_IGRID_OPTIONS;
		pCmdNew->pData = (BYTE*) pdwTrav; //(1 DWORD - the mode number
		pCmdNew->wDataSize = sizeof(DWORD);
		pCmdNew->wType = CQ2TYPE_SETMAINMENUMODE;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
	IBoxes[IBOX_MAINMENU].pIGrids[0].Add_IfcElem(pElemNew);

	//add an interface element to this grid (quit button)
		pElemNew = Create_Button("QUIT", XRes * 0.08, YRes * 0.38);
		//add second command
		retsize = 0;
		pCmdNew = new PCMD2;
		pCmdNew->pData = new BYTE[1]; //dummy byte
		pCmdNew->wDataSize = 1;
		pCmdNew->wType = CQ2TYPE_SELECT_QUIT;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
	IBoxes[IBOX_MAINMENU].pIGrids[0].Add_IfcElem(pElemNew);
/*
	//add a test button
	//create some list data
	cIfcElement* pListData = new cIfcElement;
	pListData->Init(0, 0, 0, 0);
		for (int lc40 = 0; lc40 < 100; lc40++)
		{
			char NText[50];
			itoa(lc40, NText, 10);
			pElemNew = Create_Button(NText, 0, 0);
			pListData->Add_IfcElem(pElemNew);
		}

	//create list box
	cIfcElement* pListBox = Create_IfcListBox2(XRes * 0.5, XRes * 0.7, YRes * 0.5, YRes * 0.8, pListData);
		for (int lc41 = 0; lc41 < 5; lc41++)
		{
			Add_ListBoxListing(pListBox, lc41, XRes * 0.5, XRes * 0.7 - 16, YRes * (0.5 + 0.05*lc41), YRes * (0.55 + 0.05*lc41));
		}
		((cIfcListBoxInfo*)pListBox->pListBoxInfo)->iPrimaryOffset = 5;
	//and tie together
	pListBox->pAttachedElement = pListData;

	//add back to grid
	IBoxes[IBOX_MAINMENU].pIGrids[0].Add_IfcElem(pListBox);
*/

	//set the menu values for the appropriate squares
	//we already know that we're going to use entries 4, 10, 16, 22 for the current 4 buttons on the main menu box
	//entry 1 we'll use for now(maybe later there will be a different way of doing this) as a title display
	//Set_IGridCell_DrawFlags(&pGrid[14], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "MAIN MENU", FONT_STANDARD1);

/* replace this button with the interface element that does the same thing
	Set_IGridCell_AllFlags(&pGrid[19], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBUTTONS00,
						  0, 0, MBUTTON_SIZE_X - 1, 0, MBUTTON_SIZE_Y, NULL, 0, CQDEST_INTERFACE, CQTYPE_SELECT_GAMETYPE_SINGLE, 0, 0,
						  0, 0, 0, NULL, 0, 0, 0, NULL, NULL);

    Set_IGridCell_AllFlags(&pGrid[31], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBUTTONS00,
						  0, 0, MBUTTON_SIZE_X - 1, MBUTTON_SIZE_Y, MBUTTON_SIZE_Y * 2, NULL, 0, CQDEST_INTERFACE, CQTYPE_SELECT_GAMETYPE_MULTI, 0, 0,
						  0, 0, 0, NULL, 0, 0, 0, NULL, NULL);

	Set_IGridCell_AllFlags(&pGrid[43], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBUTTONS00,
						  0, 0, MBUTTON_SIZE_X - 1, MBUTTON_SIZE_Y * 2, MBUTTON_SIZE_Y * 3, NULL, 0, CQDEST_INTERFACE, CQTYPE_SET_MAINMENUMODE, TEMP_IGRID_OPTIONS, 0,
						  0, 0, 0, NULL, 0, 0, 0, NULL, NULL);
	Set_IGridCell_AllFlags(&pGrid[55], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBUTTONS00,
						  0, 0, MBUTTON_SIZE_X - 1, MBUTTON_SIZE_Y * 3, MBUTTON_SIZE_Y *  4, NULL, 0, CQDEST_INTERFACE, CQTYPE_SELECT_QUIT, 0, 0,
						  0, 0, 0, NULL, 0, 0, 0, NULL, NULL);
*/

//set the active grid to TEMP_IGRID_SINGLE (the single player screen)
pGrid = IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SINGLE].pGrid;

		//background interface panel
		pElemNew = new cIfcElement;
		pElemNew->Init(XRes*0.2, XRes*0.8, YRes*0.2, YRes*0.8);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcPanel(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.right, pElemNew->Rect_Pos.top, pElemNew->Rect_Pos.bottom, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWIFCPANEL;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SINGLE].Add_IfcElem(pElemNew);

		//background screen
		//this is the ifcelement that will contain the actual listing data
		pSPMapsListBox = new cIfcElement;
		pSPMapsListBox->Init(0,1,0,1);

		//this actual draws the IFC LISTBOX graphic
		pElemNew = Create_IfcListBox(XRes*0.25, XRes*0.6, YRes*0.35, YRes*0.65);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SINGLE].Add_IfcElem(pElemNew);
		//this is the listbox presentation "front-end" (i need to change the confusing names for the "create" functions eventually :-/
		pElemNew = Create_IfcListBox2Ex(XRes*0.25, XRes*0.6, YRes*0.35, YRes*0.65, pSPMapsListBox, pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SINGLE].Add_IfcElem(pElemNew);
			pSPMapsListBoxFrontEnd = pElemNew;

		//"Select Map Text"
		strlength = 20;
		LPSTR Text = "PLEASE SELECT A MAP:";
		pElemNew = new cIfcElement;
		pElemNew->Init(XRes*0.25, XRes*0.6, YRes*0.3, YRes*0.4);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left + 1, pElemNew->Rect_Pos.top + 1, FONT_STANDARD1, pCA->pColorTable->Black, strlength, Text, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, FONT_STANDARD1, pCA->pColorTable->White - 1, strlength, Text, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SINGLE].Add_IfcElem(pElemNew);

		//dlistout the currently selected map 
pMapNameSelect = new char[50]; pMapNameSelect[0] = 'H'; pMapNameSelect[1] = 0;
		pElemNew = new cIfcElement;
		pElemNew->Init(XRes*0.45, XRes*0.5, YRes*0.3, YRes*0.34);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText3(0, RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, FONT_STANDARD1, pCA->pColorTable->White - 1, &pMapNameSelect, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT3;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SINGLE].Add_IfcElem(pElemNew);

		//"SINGLE PLAYER" title
		pElemNew = new cIfcElement;
		pElemNew->Init(XRes*0.4, XRes*0.48, YRes*0.23, YRes*0.03);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left + 1, pElemNew->Rect_Pos.top + 1, FONT_STANDARD1, pCA->pColorTable->Black, 13, "SINGLE PLAYER", &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, FONT_STANDARD1, pCA->pColorTable->White - 1, 13, "SINGLE PLAYER", &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SINGLE].Add_IfcElem(pElemNew);

		//RETURN button
		pElemNew = Create_Button("RETURN", XRes * 0.25, YRes * 0.7);

			pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1];  pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = 0;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SETMAINMENUMODE;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SINGLE].Add_IfcElem(pElemNew);

		//START button
		pElemNew = Create_Button("START", XRes * 0.59, YRes * 0.7);
		pCmdNew = new PCMD2;
		pCmdNew->pData = new BYTE[1]; //dummy byte
		pCmdNew->wDataSize = 1;
		pCmdNew->wType = CQ2TYPE_SELECT_START;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SINGLE].Add_IfcElem(pElemNew);

	//umm....not quite explained currently
	Update_MainMenu(TEMP_IGRID_SINGLE);

	//set the active grid to TEMP_IGRID_MULTI (the multi player screen)
pGrid = IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI].pGrid;	

		//background interface panel
		pElemNew = Create_IfcPanel(XRes*0.2, XRes*0.8, YRes*0.2, YRes*0.7);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI].Add_IfcElem(pElemNew);
	
		//service provider list box
		pElemNew = new cIfcElement;
			pServiceProviderListBox = pElemNew;
		pElemNew->Init(XRes*0.25, XRes*0.75, YRes*0.28, YRes*0.57);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcListBox(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.right, pElemNew->Rect_Pos.top, pElemNew->Rect_Pos.bottom, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWIFCLISTBOX;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI].Add_IfcElem(pElemNew);
	
		//"select service provider"
		Text = "PLEASE SELECT A SERVICE PROVIDER:";
		strlength = strlen(Text);
		pElemNew = new cIfcElement;
		pElemNew->Init(XRes*0.25, XRes*0.65, YRes*0.23, YRes*0.25);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left + 1, pElemNew->Rect_Pos.top + 1, FONT_STANDARD1, pCA->pColorTable->Black, strlength, Text, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, FONT_STANDARD1, pCA->pColorTable->White - 1, strlength, Text, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI].Add_IfcElem(pElemNew);
		
		//list name of service provider
		pElemNew = new cIfcElement;
		pElemNew->Init(XRes*0.25, XRes*0.65, YRes*0.26, YRes*0.28);
			pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText3(TRUE, RTC_SECONDARY, pElemNew->Rect_Pos.left + 1, pElemNew->Rect_Pos.top + 1, FONT_STANDARD1, pCA->pColorTable->White - 1, &pSPNameSelect, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_DRAWTEXT3;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI].Add_IfcElem(pElemNew);

		//RETURN button (TEMP_MULTI)
		pElemNew = Create_Button("RETURN", XRes * 0.35, YRes * 0.6);
			pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1];  pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = 0;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SETMAINMENUMODE;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI].Add_IfcElem(pElemNew);

		//NEXT button
		pElemNew = Create_Button("NEXT", XRes * 0.5, YRes * 0.6);
		//add the command
			pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1];  pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = TEMP_IGRID_MULTI2A;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SETMAINMENUMODE;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI].Add_IfcElem(pElemNew);

////////////////////////////////////////////////////
		//TEMP_MULTI_2A (device select)
		//background interface panel
		pElemNew = new cIfcElement;
		pElemNew->Init(XRes*0.2, XRes*0.8, YRes*0.25, YRes*0.75);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcPanel(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.right, pElemNew->Rect_Pos.top, pElemNew->Rect_Pos.bottom, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWIFCPANEL;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2A].Add_IfcElem(pElemNew);
	
		//devices list box
		pElemNew = new cIfcElement;
			pDeviceListBox = pElemNew;
		pElemNew->Init(XRes*0.25, XRes*0.75, YRes*0.33, YRes*0.62);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcListBox(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.right, pElemNew->Rect_Pos.top, pElemNew->Rect_Pos.bottom, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWIFCLISTBOX;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2A].Add_IfcElem(pElemNew);
	
		//"select device for selected service provider"
		Text = "SELECT DEVICE FOR SELECTED SERVICE PROVIDER:";
		strlength = strlen(Text);
		pElemNew = new cIfcElement;
		pElemNew->Init(XRes*0.25, XRes*0.65, YRes*0.28, YRes*0.31);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left + 1, pElemNew->Rect_Pos.top + 1, FONT_STANDARD1, pCA->pColorTable->Black, strlength, Text, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, FONT_STANDARD1, pCA->pColorTable->White - 1, strlength, Text, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2A].Add_IfcElem(pElemNew);
		
		//list name of device
		pElemNew = new cIfcElement;
		pElemNew->Init(XRes*0.25, XRes*0.65, YRes*0.31, YRes*0.34);
		
			pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText3(TRUE, RTC_SECONDARY, pElemNew->Rect_Pos.left + 1, pElemNew->Rect_Pos.top + 1, FONT_STANDARD1, pCA->pColorTable->White - 1, &pDeviceNameSelect, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_DRAWTEXT3;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2A].Add_IfcElem(pElemNew);

		//BACK button (TEMP_MULTI_2A)
		pElemNew = Create_Button("BACK", XRes * 0.35, YRes * 0.65);
			pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1];  pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = TEMP_IGRID_MULTI;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SETMAINMENUMODE;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2A].Add_IfcElem(pElemNew);

		//NEXT button
		pElemNew = Create_Button("NEXT", XRes * 0.5, YRes * 0.65);
			pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1];  pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = TEMP_IGRID_MULTI2B;
//store this address so that other commands can change it later
pdwSPDest = pdwTrav;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SETMAINMENUMODE;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
//add a "Finalize" command to build the next interface panels
				pCmdNew = new PCMD2;
				pCmdNew->pData = new BYTE[1];  //dummy byte
				pCmdNew->wDataSize = 1;
				pCmdNew->wType = CQ2TYPE_FINALIZE_MP_MENUS;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2A].Add_IfcElem(pElemNew);

////////////////////
		//TEMP_IGRID_MULTI2B (after the SP-specific panels

		//background panel
		pElemNew = Create_IfcPanel(XRes * 0.2, XRes * 0.8, YRes * 0.2, YRes * 0.8);
			Add_StaticTextElem(pElemNew, IFC_ELEM_CAT_DEFAULT_DRAW, "GAME HOST/CONNECT", FONT_STANDARD1, pCA->pColorTable->White - 1, XRes * 0.4, YRes * 0.22);
			Add_StaticTextElem(pElemNew, IFC_ELEM_CAT_DEFAULT_DRAW, "Games Currently Available:", FONT_STANDARD1, pCA->pColorTable->White - 1, XRes * 0.22, YRes * 0.35);
			Add_StaticTextElem(pElemNew, IFC_ELEM_CAT_DEFAULT_DRAW, "Name:", FONT_STANDARD1, pCA->pColorTable->White - 1, XRes * 0.22, YRes * 0.25);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2B].Add_IfcElem(pElemNew);

		//player's name
		pElemNew = Create_TextInputBox(XRes * 0.3, YRes * 0.25, XRes * 0.3, FONT_STANDARD1, TRUE, (void*)pCA->pGameOptions->InfoPlayer_Local.pwszName, 80);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2B].Add_IfcElem(pElemNew);

		//list box
		pElemNew = Create_IfcListBox(XRes * 0.24, XRes * 0.6, YRes * 0.37, YRes * 0.6);
		pHostsListBox = pElemNew; //store location of host list box
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2B].Add_IfcElem(pElemNew);

		//connect, find hosts, host buttons
		pElemNew = Create_Button("Connect to Game", XRes * 0.24, YRes * 0.62);
			//enumerate hosts
			pCmdNew = new PCMD2;
			pCmdNew->pData = new BYTE[1]; //dummy byte
			pCmdNew->wDataSize = 1;
			pCmdNew->wType = CQ2TYPE_CONNECT_STD;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2B].Add_IfcElem(pElemNew);

		pElemNew = Create_Button("Find Games", XRes * 0.64, YRes * 0.37);
			//enumerate hosts
			pCmdNew = new PCMD2;
			pCmdNew->pData = new BYTE[1]; //dummy byte
			pCmdNew->wDataSize = 1;
			pCmdNew->wType = CQ2TYPE_ENUMHOSTS_STD;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2B].Add_IfcElem(pElemNew);

		pElemNew = Create_Button("Create Game", XRes * 0.64, YRes * 0.25);
			//go to select map
			pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1];  pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = TEMP_IGRID_MULTI4;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SETMAINMENUMODE;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2B].Add_IfcElem(pElemNew);

		//BACK button (TEMP_MULTI_2B)
		pElemNew = Create_Button("BACK", XRes * 0.22, YRes * 0.7);
			pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1];  pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = TEMP_IGRID_MULTI2A;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SETMAINMENUMODE;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2B].Add_IfcElem(pElemNew);

//IGRID_MULTI3

		//background panel
		pElemNew = Create_IfcPanel(XRes * 0.1, XRes * 0.9, YRes * 0.1, YRes * 0.9);
			Add_StaticTextElem(pElemNew, IFC_ELEM_CAT_DEFAULT_DRAW, "GAME LAUNCH!", FONT_STANDARD1, pCA->pColorTable->White - 1, XRes * 0.4, YRes * 0.12);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI3].Add_IfcElem(pElemNew);

		//chat interface box
		pElemNew = Create_TextInputBox(XRes * 0.15, YRes * 0.7, XRes * 0.5, FONT_STANDARD1, FALSE, (void*)caChatText, 1000);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI3].Add_IfcElem(pElemNew);

		//chat window
		pElemNew = Create_IfcListBox(XRes * 0.5, XRes * 0.85, YRes * 0.2, YRes * 0.7);
				pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DispChatText(RTC_SECONDARY, pElemNew->Rect_Pos.left + 10, pElemNew->Rect_Pos.top + 10, (pElemNew->Rect_Pos.bottom - pElemNew->Rect_Pos.top) / pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight , &iretsize);
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_DISPCHATTEXT;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI3].Add_IfcElem(pElemNew);

		//create updatable region
		pElemNew = new cIfcElement;
		pElemNew->Init(XRes * 0.1, XRes * 0.9, YRes * 0.1, YRes * 0.9);
		pGameLaunchUpdateRgn = pElemNew;
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI3].Add_IfcElem(pElemNew);

		//"SEND" button
		pElemNew = Create_Button("SEND", XRes * 0.76, YRes * 0.7);
				pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Send_Text2(pCA->pGameOptions->InfoPlayer_Local.pwszName, caChatText, 1000, 10000, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_SENDTEXT2;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
				pCmdNew = new PCMD2;
				pCmdNew->pData = new BYTE[1]; //dummy byte
				pCmdNew->wDataSize = 1;
				pCmdNew->wType = CQ2TYPE_CLEARCHATBUFFER;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI3].Add_IfcElem(pElemNew);

		//start button
		pElemNew = Create_Button("START(Host only)", XRes * 0.22, YRes * 0.8);
		pMPStartButton = pElemNew;
			pCmdNew = new PCMD2;
				pCmdNew->pData = new BYTE[1];  
				pCmdNew->wDataSize = 1;
				pCmdNew->wType = CQ2TYPE_SELECT_START;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI3].Add_IfcElem(pElemNew);

		//CANCEL button (TEMP_MULTI_3)
		pElemNew = Create_Button("CANCEL", XRes * 0.52, YRes * 0.8);
			pCmdNew = new PCMD2;
				pCmdNew->pData = new BYTE[1];  
				pCmdNew->wDataSize = 1;
				pCmdNew->wType = CQ2TYPE_SELECT_DISCONNECT;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI3].Add_IfcElem(pElemNew);


//IGRID_BRIEFING

		//this is the ifcelement that will contain the actual listing data
		pBriefingNationSelectListBox = new cIfcElement;
		pBriefingNationSelectListBox->Init(0,1,0,1);

		//player list
		pBriefingPlayerListBox = new cIfcElement;
		pBriefingPlayerListBox->Init(0,1,0,1);


//IGRID_MULTI4

		//background interface panel
		pElemNew = new cIfcElement;
		pElemNew->Init(XRes*0.2, XRes*0.8, YRes*0.2, YRes*0.8);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcPanel(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.right, pElemNew->Rect_Pos.top, pElemNew->Rect_Pos.bottom, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWIFCPANEL;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI4].Add_IfcElem(pElemNew);

		//create the MP Maps List Box Data Location
		pElemNew = new cIfcElement;
			pElemNew->Init(0,1,0,1);
			pMPMapsListBox = pElemNew;
		//draw the listbox
		pElemNew = Create_IfcListBox(XRes*0.25, XRes*0.6, YRes*0.35, YRes*0.65);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI4].Add_IfcElem(pElemNew);
		//create its front end
		pElemNew = Create_IfcListBox2Ex(XRes*0.25, XRes*0.6, YRes*0.35, YRes*0.65, pMPMapsListBox, pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2);
			pMPMapsListBoxFrontEnd = pElemNew; //save its location
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI4].Add_IfcElem(pElemNew);

		//"Select Map Text"
		strlength = 20;
		Text = "PLEASE SELECT A MAP:";
		pElemNew = new cIfcElement;
		pElemNew->Init(XRes*0.25, XRes*0.6, YRes*0.3, YRes*0.4);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left + 1, pElemNew->Rect_Pos.top + 1, FONT_STANDARD1, pCA->pColorTable->Black, strlength, Text, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, FONT_STANDARD1, pCA->pColorTable->White - 1, strlength, Text, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI4].Add_IfcElem(pElemNew);

		//dlistout the currently selected map 
pMapNameSelect = new char[50]; pMapNameSelect[0] = 'H'; pMapNameSelect[1] = 0;
		pElemNew = new cIfcElement;
		pElemNew->Init(XRes*0.45, XRes*0.5, YRes*0.3, YRes*0.34);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText3(0, RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, FONT_STANDARD1, pCA->pColorTable->White - 1, &pMapNameSelect, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT3;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI4].Add_IfcElem(pElemNew);

		//"MULTIPLAYER" title
		pElemNew = new cIfcElement;
		pElemNew->Init(XRes*0.4, XRes*0.48, YRes*0.23, YRes*0.03);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left + 1, pElemNew->Rect_Pos.top + 1, FONT_STANDARD1, pCA->pColorTable->Black, 13, "MULTI PLAYER", &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, FONT_STANDARD1, pCA->pColorTable->White - 1, 13, "MULTI PLAYER", &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI4].Add_IfcElem(pElemNew);

		//RETURN button
		pElemNew = Create_Button("RETURN", XRes * 0.25, YRes * 0.7);
			pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1];  pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = 0;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SETMAINMENUMODE;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI4].Add_IfcElem(pElemNew);

		//Begin hosting game
		pElemNew = Create_Button("Create Game", XRes * 0.59, YRes * 0.7);
		pCmdNew = new PCMD2;
			pCmdNew->pData = new BYTE[1]; //dummy byte
			pCmdNew->wDataSize = 1;
			pCmdNew->wType = CQ2TYPE_HOST_STD;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI4].Add_IfcElem(pElemNew);


	//umm....not quite explained currently
	Update_MainMenu(TEMP_IGRID_MULTI4);

/*
//this is a 200x120 at 4x4 grid
	Set_IGridCell_DrawFlags(&pGrid[20], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"MULTI-PLAYER" , FONT_STANDARD1);
	Set_IGridCell_DrawFlags(&pGrid[115*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Return", FONT_STANDARD1);	
	//because the text will take up(at the moment) 15x48 pixels, the return area is 4x12 boxes
	Set_IGridCell_CmdFlags_Box(pGrid, 115*200 + 130, 12, 4, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SET_MAINMENUMODE, 0,
							   0, 0, 0, 0, NULL, 0);
	//now to set up boxes for multiplay menu
	Set_IGridCell_DrawFlags(&pGrid[5*200 + 2], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Service Providers:", FONT_STANDARD1);
			Set_IGridCell_DrawFlags(&pGrid[9*200 + 2], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBOXES00, 0, 0, 300, 0, 100, NULL, 0);
	Set_IGridCell_DrawFlags(&pGrid[36*200 + 2], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Devices:", FONT_STANDARD1);	
			Set_IGridCell_DrawFlags(&pGrid[40*200 + 2], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBOXES00, 0, 0, 300, 0, 100, NULL, 0);
	Set_IGridCell_DrawFlags(&pGrid[70*200 + 2], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Detected Hosts:", FONT_STANDARD1);
			Set_IGridCell_DrawFlags(&pGrid[74*200 + 2], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBOXES00, 0, 0, 300, 0, 100, NULL, 0);
	Set_IGridCell_DrawFlags(&pGrid[100*200 + 2], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Selected Host:", FONT_STANDARD1);

//	Set_IGridCell_DrawFlags(&pGrid[5*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Players:");
//			Set_IGridCell_DrawFlags(&pGrid[9*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBOXES00, 0, 0, 200, 0, 240, NULL);
	//boxes for bottom 4 fields of "STRING" component selection (mainly for serial modes selection
			Set_IGridCell_DrawFlags(&pGrid[30*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBOXES00, 0, 0, 200, 0, 60, NULL, 0);
			Set_IGridCell_DrawFlags(&pGrid[50*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBOXES00, 0, 0, 200, 0, 60, NULL, 0);
			Set_IGridCell_DrawFlags(&pGrid[70*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBOXES00, 0, 0, 200, 0, 60, NULL, 0);
			Set_IGridCell_DrawFlags(&pGrid[90*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBOXES00, 0, 0, 200, 0, 60, NULL, 0);			
	//2 entry boxes for text entry
			Set_IGridCell_DrawFlags(&pGrid[10*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBOXES00, 0, 0, 200, 0, 20, NULL, 0);
			Set_IGridCell_DrawFlags(&pGrid[20*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBOXES00, 0, 0, 200, 0, 20, NULL, 0);
	//the first will be entry for NAME
			Set_IGridCell_DrawFlags(&pGrid[6*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Name/Handle:", FONT_STANDARD1);
			Set_IGridCell_DrawFlags(&pGrid[11*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,(char*)pCA->pGameOptions->InfoPlayer_Local.pwszName, FONT_STANDARD1);
			Set_IGridCell_CmdFlags_Box(pGrid, 10*200 + 90, 50, 5, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SET_INPUTSTRING_WIDE, 80, 0, 0, 0, 0, (void*)pCA->pGameOptions->InfoPlayer_Local.pwszName, 0);

//			Set_IGridCell_DrawFlags(&pGrid[16*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"IP Address:", FONT_STANDARD1);
//			Set_IGridCell_DrawFlags(&pGrid[21*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,(char*)pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcaIP, FONT_STANDARD1);
//			Set_IGridCell_CmdFlags_Box(pGrid, 20*200 + 90, 50, 5, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SET_INPUTSTRING_WIDE, 80, 0, 0, 0, 0, (void*)pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcaIP, 0);
						
	
	//enumerate, connect, create game buttons
	Set_IGridCell_DrawFlags(&pGrid[67*200 + 50], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBUTTONS00, 0, 0, MBUTTON_SIZE_X - 1, MBUTTON_SIZE_Y * 4, MBUTTON_SIZE_Y * 5, NULL, 0);
		Set_IGridCell_CmdFlags_Box(pGrid, 67*200 + 50, 20, 6, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_ENUMHOSTS_STD, 0,
							   0, 0, 0, 0, NULL, 0);
	Set_IGridCell_DrawFlags(&pGrid[108*200 + 5], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBUTTONS00, 0, 0, MBUTTON_SIZE_X - 1, MBUTTON_SIZE_Y * 5, MBUTTON_SIZE_Y * 6, NULL, 0);
		Set_IGridCell_CmdFlags_Box(pGrid, 108*200 + 5, 20, 6, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_CONNECT_STD, 0,
							   0, 0, 0, 0, NULL, 0);
	Set_IGridCell_DrawFlags(&pGrid[108*200 + 100], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBUTTONS00, 0, 0, MBUTTON_SIZE_X - 1, MBUTTON_SIZE_Y * 7, MBUTTON_SIZE_Y * 8, NULL, 0);
		Set_IGridCell_CmdFlags_Box(pGrid, 108*200 + 100, 20, 6, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_HOST_STD, 0,
							   0, 0, 0, 0, NULL, 0);
*/		
		


pGrid = IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_OPTIONS].pGrid;	
//this is a 200x120 at 4x4 grid
	Set_IGridCell_DrawFlags(&pGrid[20], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"OPTIONS", FONT_STANDARD1);
	Set_IGridCell_DrawFlags(&pGrid[100*200 + 100], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Return", FONT_STANDARD1);		
	Set_IGridCell_CmdFlags_Box(pGrid, 100*200 + 100, 12, 4, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SET_MAINMENUMODE, 0,
							   0, 0, 0, 0, NULL,  0);

//the multi2 grid
pGrid = IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2].pGrid;
//this is a 40x30 at 16x16 grid
	Set_IGridCell_DrawFlags(&pGrid[28*40 + 20], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Disconnect", FONT_STANDARD1);		
	Set_IGridCell_CmdFlags_Box(pGrid, 28*40 + 20, 4, 1, 40, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SELECT_DISCONNECT, 0,
							   0, 0, 0, 0, NULL,  0);
/*	//text boxes for each player's name and iControl ID (disabled due to change in pGameOptions structure
	for (lc1 = 0; lc1 < 8; lc1++)
	{
		Set_IGridCell_DrawFlags(&pGrid[(2 + lc1)*40 + 3], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,(char*)pCA->pGameOptions->Players[lc1].pwszName);	
	}
*/
	//the numbers for the iControl ID numbers have to be put in manually as text literals, otherwise they'll all point to the same buffer
		Set_IGridCell_DrawFlags(&pGrid[(2 + 0)*40 + 2], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,"1", FONT_STANDARD1);	
		Set_IGridCell_DrawFlags(&pGrid[(2 + 1)*40 + 2], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,"2", FONT_STANDARD1);	
		Set_IGridCell_DrawFlags(&pGrid[(2 + 2)*40 + 2], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,"3", FONT_STANDARD1);	
		Set_IGridCell_DrawFlags(&pGrid[(2 + 3)*40 + 2], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,"4", FONT_STANDARD1);	
		Set_IGridCell_DrawFlags(&pGrid[(2 + 4)*40 + 2], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,"5", FONT_STANDARD1);	
		Set_IGridCell_DrawFlags(&pGrid[(2 + 5)*40 + 2], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,"6", FONT_STANDARD1);	
		Set_IGridCell_DrawFlags(&pGrid[(2 + 6)*40 + 2], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,"7", FONT_STANDARD1);	
		Set_IGridCell_DrawFlags(&pGrid[(2 + 7)*40 + 2], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,"8", FONT_STANDARD1);	


	//a textbox for chat text input
	//Set_IGridCell_DrawFlags(&pGrid[25*40 + 2], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBOXES00, 0, 0, 300, 0, 15, NULL, 0);
	//Set_IGridCell_DrawFlags(&pGrid[25*40 + 3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,(char*)caChatText, FONT_STANDARD1);
	//Set_IGridCell_CmdFlags_Box(pGrid, 25*40 + 2, 15, 1, 40, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SET_INPUTSTRING, 80, 0, 0, 0, 0, (void*)caChatText, 0);
	//a send button
	//Set_IGridCell_DrawFlags(&pGrid[25*40 + 20], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBUTTONS00, 0, 0, MBUTTON_SIZE_X - 1, MBUTTON_SIZE_Y * 8, MBUTTON_SIZE_Y * 9, NULL, 0);
	//Set_IGridCell_CmdFlags_Box(pGrid, 25*40 + 20, 3, 1, 40, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SENDTEXT, 80, 0, 0, 0, 0, (void*)caChatText, 0);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::MPSP_RunOnce()
{
	//this function sets up the multiplayer Service Provider specific screens(hence, MPSP),
	//once the user has selected a service provider and device 
	//(08.09.2005)
	cClassAccess* pCA = (cClassAccess*)pCAccess;

int DX = pCA->pDD_Display->i_XResD;
int DY = pCA->pDD_Display->i_YResD;
int XRes = pCA->pDD_Display->i_XResD;
int YRes = pCA->pDD_Display->i_YResD;

DWORD* pdwTrav;

	cIfcElement* pElemNew;
	PCMD2* pCmdNew;
	int iretsize;
	int retsize;
	int strlength;

	if (pCA->pDP_Multiplay->iActiveProvider < 0)
		return;

	DPN_SERVICE_PROVIDER_INFO* pInfo = &pCA->pDP_Multiplay->ServiceProviders[pCA->pDP_Multiplay->iActiveProvider];
	DPSPCONNECTINFO* pConnectInfo = &pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider];

	if (pInfo->guid == CLSID_DP8SP_SERIAL)
	{
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SERIAL].Clear_IfcElems();

		//ifcpanel
		pElemNew = Create_IfcPanel(XRes * 0.2, XRes * 0.8, YRes * 0.2, YRes * 0.8);
			Add_StaticTextElem(pElemNew, IFC_ELEM_CAT_DEFAULT_DRAW, "SERIAL CONNECTION SETUP", FONT_STANDARD1, pCA->pColorTable->White - 1, XRes * 0.4, YRes * 0.21);
			Add_StaticTextElem(pElemNew, IFC_ELEM_CAT_DEFAULT_DRAW, "Baud Rate", FONT_STANDARD1, pCA->pColorTable->White - 1, XRes * 0.22, YRes * 0.24);
			Add_StaticTextElem(pElemNew, IFC_ELEM_CAT_DEFAULT_DRAW, "Flow Control", FONT_STANDARD1, pCA->pColorTable->White - 1, XRes * 0.22, YRes * 0.47);
			Add_StaticTextElem(pElemNew, IFC_ELEM_CAT_DEFAULT_DRAW, "Parity", FONT_STANDARD1, pCA->pColorTable->White - 1, XRes * 0.52, YRes * 0.24);
			Add_StaticTextElem(pElemNew, IFC_ELEM_CAT_DEFAULT_DRAW, "Stop Bits", FONT_STANDARD1, pCA->pColorTable->White - 1, XRes * 0.52, YRes * 0.47);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SERIAL].Add_IfcElem(pElemNew);
	
		//the corresponding list boxes
		pElemNew = Create_IfcListBox(XRes * 0.22, XRes * 0.48, YRes * 0.26, YRes * 0.43);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SERIAL].Add_IfcElem(pElemNew);

		pElemNew = Create_IfcListBox(XRes * 0.22, XRes * 0.48, YRes * 0.49, YRes * 0.68);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SERIAL].Add_IfcElem(pElemNew);

		pElemNew = Create_IfcListBox(XRes * 0.52, XRes * 0.78, YRes * 0.26, YRes * 0.43);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SERIAL].Add_IfcElem(pElemNew);

		pElemNew = Create_IfcListBox(XRes * 0.52, XRes * 0.78, YRes * 0.49, YRes * 0.68);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SERIAL].Add_IfcElem(pElemNew);

		//Back BUTTON
		pElemNew = Create_Button("BACK", XRes * 0.24, YRes * 0.72);
			pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1];  pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = TEMP_IGRID_MULTI2A;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SETMAINMENUMODE;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SERIAL].Add_IfcElem(pElemNew);
		//NEXT button
		pElemNew = Create_Button("NEXT", XRes * 0.7, YRes * 0.72);
			pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1];  pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = TEMP_IGRID_MULTI2B;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SETMAINMENUMODE;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_SERIAL].Add_IfcElem(pElemNew);

	}	
	if (pInfo->guid == CLSID_DP8SP_MODEM)
	{
		//clear elements
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MODEM].Clear_IfcElems();

		//ifcpanel
		pElemNew = Create_IfcPanel(XRes * 0.2, XRes * 0.8, YRes * 0.2, YRes * 0.5);
			Add_StaticTextElem(pElemNew, IFC_ELEM_CAT_DEFAULT_DRAW, "MODEM CONNECTION SETUP", FONT_STANDARD1, pCA->pColorTable->White - 1, XRes * 0.4, YRes * 0.21);
			Add_StaticTextElem(pElemNew, IFC_ELEM_CAT_DEFAULT_DRAW, "Phone Number", FONT_STANDARD1, pCA->pColorTable->White - 1, XRes * 0.22, YRes * 0.24);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MODEM].Add_IfcElem(pElemNew);

		//text input box
		pElemNew = Create_TextInputBox(XRes * 0.25, YRes * 0.3, XRes * 0.3, FONT_STANDARD1, TRUE, pConnectInfo->wcaPhoneNumber, 80);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MODEM].Add_IfcElem(pElemNew);

		//Back BUTTON
		pElemNew = Create_Button("BACK", XRes * 0.24, YRes * 0.42);
			pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1];  pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = TEMP_IGRID_MULTI2A;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SETMAINMENUMODE;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MODEM].Add_IfcElem(pElemNew);
		//NEXT button
		pElemNew = Create_Button("NEXT", XRes * 0.7, YRes * 0.42);
			pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1];  pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = TEMP_IGRID_MULTI2B;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SETMAINMENUMODE;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MODEM].Add_IfcElem(pElemNew);

	}
	if (pInfo->guid == CLSID_DP8SP_TCPIP)
	{
		//clear elements
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_TCPIP].Clear_IfcElems();

		//ifcpanel
		pElemNew = Create_IfcPanel(XRes * 0.2, XRes * 0.8, YRes * 0.2, YRes * 0.5);
			Add_StaticTextElem(pElemNew, IFC_ELEM_CAT_DEFAULT_DRAW, "TCP/IP CONNECTION SETUP", FONT_STANDARD1, pCA->pColorTable->White - 1, XRes * 0.4, YRes * 0.21);
			Add_StaticTextElem(pElemNew, IFC_ELEM_CAT_DEFAULT_DRAW, "IP(leave as 0.0.0.0 for default)", FONT_STANDARD1, pCA->pColorTable->White - 1, XRes * 0.22, YRes * 0.24);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_TCPIP].Add_IfcElem(pElemNew);

		//right now just always fill the field with "0.0.0.0"
		pConnectInfo->wcaIP[0] = 0;
		wcscat(pConnectInfo->wcaIP, L"0.0.0.0");
		//text input box
		pElemNew = Create_TextInputBox(XRes * 0.25, YRes * 0.3, XRes * 0.3, FONT_STANDARD1, TRUE, pConnectInfo->wcaIP, 80);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_TCPIP].Add_IfcElem(pElemNew);

		//Back BUTTON
		pElemNew = Create_Button("BACK", XRes * 0.24, YRes * 0.42);
			pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1];  pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = TEMP_IGRID_MULTI2A;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SETMAINMENUMODE;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_TCPIP].Add_IfcElem(pElemNew);
		//NEXT button
		pElemNew = Create_Button("NEXT", XRes * 0.7, YRes * 0.42);
			pCmdNew = new PCMD2;
				pCmdNew->pData = (BYTE*)new DWORD[1];  pdwTrav = (DWORD*)pCmdNew->pData; *pdwTrav = TEMP_IGRID_MULTI2B;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SETMAINMENUMODE;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_TCPIP].Add_IfcElem(pElemNew);

	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Update_MPConnect_Display(int iDispStringLimit)
{   //07.12.2002
	WORD lc1, lc2;  //local counters
cClassAccess* pCA = (cClassAccess*)pCAccess;	
char NText[50];
		
		int dxanch = 0;
		int dxanch2 = 0;
		int dyanch = 0;
		int dyanch2 = 0;
		int ftheight = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight;
		int retsize = 0;
		int XRes = pCA->pDD_Display->i_XResD;
		int YRes = pCA->pDD_Display->i_YResD;



if (IBoxes[IBOX_MAINMENU].iActiveGrid == TEMP_IGRID_MULTI)
{
	pSPNameSelect = (char*)L"None Selected";
	//set currently active name
	if (pCA->pDP_Multiplay->iActiveProvider >= 0)
	{
		pSPNameSelect = (char*)pCA->pDP_Multiplay->ServiceProviders[pCA->pDP_Multiplay->iActiveProvider].pwszName;
	}
	if (pServiceProviderListBox != NULL)
	{
		//clear and fill service provider list box
		pServiceProviderListBox->Clear_IfcElems();
		
		dxanch = pServiceProviderListBox->Rect_Pos.left;
		dxanch2 = pServiceProviderListBox->Rect_Pos.right;
		dyanch = pServiceProviderListBox->Rect_Pos.top;
		dyanch2 = pServiceProviderListBox->Rect_Pos.bottom;

		for (int lc1 = 0; lc1 < pCA->pDP_Multiplay->dwNumProviders; lc1++)
		{
			//now add back elems
			cIfcElement* pNew = new cIfcElement;
			pNew->Init( dxanch + 10, dxanch2 - 10, dyanch + (ftheight * lc1) + 10, dyanch + (ftheight * (lc1+1)) + 10);
				//add commands
				 if (lc1 == pCA->pDP_Multiplay->iActiveProvider)
				 {
					PCMD2* pCmdNew = new PCMD2;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->DetColor(0, 0, 0.5), pNew->Rect_Pos.left, pNew->Rect_Pos.right, pNew->Rect_Pos.top, pNew->Rect_Pos.bottom, &retsize);
					pCmdNew->wDataSize = retsize;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
				 }
				PCMD2* pCmdNew = new PCMD2;
				 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, TRUE, RTC_SECONDARY, pNew->Rect_Pos.left, pNew->Rect_Pos.top, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, (char*)pCA->pDP_Multiplay->ServiceProviders[lc1].pwszName);
				pNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
				//add commands
				pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->WinBlue1, pNew->Rect_Pos.left, pNew->Rect_Pos.right, pNew->Rect_Pos.top, pNew->Rect_Pos.bottom, &retsize);
				pCmdNew->wDataSize = retsize;
				pCmdNew->wType = CQ2TYPE_COLORFILL;
				pNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
				//add commands
				 if (lc1 == pCA->pDP_Multiplay->iActiveProvider)
				 {
					PCMD2* pCmdNew = new PCMD2;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->DetColor(0, 0, 0.5), pNew->Rect_Pos.left, pNew->Rect_Pos.right, pNew->Rect_Pos.top, pNew->Rect_Pos.bottom, &retsize);
					pCmdNew->wDataSize = retsize;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
				 }
				//add commands
				pCmdNew = new PCMD2;
				 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, TRUE, RTC_SECONDARY, pNew->Rect_Pos.left, pNew->Rect_Pos.top, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, (char*)pCA->pDP_Multiplay->ServiceProviders[lc1].pwszName);
				pNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
				//add commands
				pCmdNew = new PCMD2;
				DWORD* pdwdata = new DWORD;
				*pdwdata = lc1;
				pCmdNew->pData = (BYTE*)pdwdata;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SET_ACTIVEPROVIDER;
				pNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
				//the "change menu path" command
					DWORD Val = TEMP_IGRID_MULTI;
					if (pCA->pDP_Multiplay->ServiceProviders[lc1].guid == CLSID_DP8SP_SERIAL)
					{
						Val = TEMP_IGRID_SERIAL;
					}
					if (pCA->pDP_Multiplay->ServiceProviders[lc1].guid == CLSID_DP8SP_MODEM)
					{
						Val = TEMP_IGRID_MODEM;
					}
					if (pCA->pDP_Multiplay->ServiceProviders[lc1].guid == CLSID_DP8SP_TCPIP)
					{
						Val = TEMP_IGRID_TCPIP;
					}
				pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Set_Pdword(Val, pdwSPDest, &retsize);
				pCmdNew->wDataSize = retsize;
				pCmdNew->wType = CQ2TYPE_SET_PDWORD;
				pNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
			pServiceProviderListBox->Add_IfcElem(pNew);

		}
	}
}
if (IBoxes[IBOX_MAINMENU].iActiveGrid == TEMP_IGRID_MULTI2A)
{

	pDeviceListBox->Clear_IfcElems();

		dxanch = pDeviceListBox->Rect_Pos.left;
		dxanch2 = pDeviceListBox->Rect_Pos.right;
		dyanch = pDeviceListBox->Rect_Pos.top;
		dyanch2 = pDeviceListBox->Rect_Pos.bottom;

	pDeviceNameSelect = (char*)L"None Selected";
	//set currently active name
	if (pCA->pDP_Multiplay->iActiveProvider >= 0)
	{
		int iProvider = pCA->pDP_Multiplay->iActiveProvider;
		int iDevice = pCA->pDP_Multiplay->pDeviceArrays[iProvider].wActiveDevice;
		int iNumDevices = pCA->pDP_Multiplay->pDeviceArrays[iProvider].dwNumDevices;

		if (iDevice >= 0)
			pDeviceNameSelect = (char*)pCA->pDP_Multiplay->pDeviceArrays[iProvider].SPDevices[iDevice].pwszName;

			//fill device boxes
		for (int lc1 = 0; lc1 < iNumDevices; lc1++)
		{
			//now add back elems
			cIfcElement* pNew = new cIfcElement;
			pNew->Init( dxanch + 10, dxanch2 - 10, dyanch + (ftheight * lc1) + 10, dyanch + (ftheight * (lc1+1)) + 10);
				if (lc1 == pCA->pDP_Multiplay->pDeviceArrays[iProvider].wActiveDevice)
				 {
					PCMD2* pCmdNew = new PCMD2;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->DetColor(0, 0, 0.5), pNew->Rect_Pos.left, pNew->Rect_Pos.right, pNew->Rect_Pos.top, pNew->Rect_Pos.bottom, &retsize);
					pCmdNew->wDataSize = retsize;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
				 }
				//add commands
				PCMD2* pCmdNew = new PCMD2;
				 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, TRUE, RTC_SECONDARY, pNew->Rect_Pos.left, pNew->Rect_Pos.top, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, (char*)pCA->pDP_Multiplay->pDeviceArrays[iProvider].SPDevices[lc1].pwszName);
				pNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
				//add commands
				pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->WinBlue1, pNew->Rect_Pos.left, pNew->Rect_Pos.right, pNew->Rect_Pos.top, pNew->Rect_Pos.bottom, &retsize);
				pCmdNew->wDataSize = retsize;
				pCmdNew->wType = CQ2TYPE_COLORFILL;
				pNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
				if (lc1 == pCA->pDP_Multiplay->pDeviceArrays[iProvider].wActiveDevice)
				 {
					PCMD2* pCmdNew = new PCMD2;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->DetColor(0, 0, 0.5), pNew->Rect_Pos.left, pNew->Rect_Pos.right, pNew->Rect_Pos.top, pNew->Rect_Pos.bottom, &retsize);
					pCmdNew->wDataSize = retsize;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
				 }
				//add commands
				pCmdNew = new PCMD2;
				 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, TRUE, RTC_SECONDARY, pNew->Rect_Pos.left, pNew->Rect_Pos.top, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, (char*)pCA->pDP_Multiplay->pDeviceArrays[iProvider].SPDevices[lc1].pwszName);
				pNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
				//add commands
				pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Set_ActiveDevice(iProvider, lc1, &retsize);
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SET_ACTIVEDEVICE;
				pNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
			pDeviceListBox->Add_IfcElem(pNew);

		}
	}

	

}
if (IBoxes[IBOX_MAINMENU].iActiveGrid == TEMP_IGRID_MULTI2B)
{

	int top = pHostsListBox->Rect_Pos.top;
	int bottom = pHostsListBox->Rect_Pos.bottom;
	int right = pHostsListBox->Rect_Pos.right;
	int left = pHostsListBox->Rect_Pos.left;

	pHostsListBox->Clear_IfcElems();

	for (int lc1 = 0; lc1 < pCA->pDP_Multiplay->wHostsFound; lc1++)
	{
		cIfcElement* pNew = new cIfcElement;
		pNew->Init( left + 10, right - 10, top + (ftheight * lc1) + 10, top + (ftheight * (lc1+1)) + 10);
				 if (lc1 == pCA->pDP_Multiplay->wSelectedHost)
				 {
					PCMD2* pCmdNew = new PCMD2;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->DetColor(0, 0, 0.5), pNew->Rect_Pos.left, pNew->Rect_Pos.right, pNew->Rect_Pos.top, pNew->Rect_Pos.bottom, &retsize);
					pCmdNew->wDataSize = retsize;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
				 }
				//add commands
				PCMD2* pCmdNew = new PCMD2;
				 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, TRUE, RTC_SECONDARY, pNew->Rect_Pos.left, pNew->Rect_Pos.top, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, (char*)pCA->pDP_Multiplay->HostsFound[lc1].pAppDesc->pwszSessionName);
				pNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
				//add commands
				pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->WinBlue1, pNew->Rect_Pos.left, pNew->Rect_Pos.right, pNew->Rect_Pos.top, pNew->Rect_Pos.bottom, &retsize);
				pCmdNew->wDataSize = retsize;
				pCmdNew->wType = CQ2TYPE_COLORFILL;
				pNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
				 if (lc1 == pCA->pDP_Multiplay->wSelectedHost)
				 {
					PCMD2* pCmdNew = new PCMD2;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, pCA->pColorTable->DetColor(0, 0, 0.5), pNew->Rect_Pos.left, pNew->Rect_Pos.right, pNew->Rect_Pos.top, pNew->Rect_Pos.bottom, &retsize);
					pCmdNew->wDataSize = retsize;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
				 }
				//add commands
				pCmdNew = new PCMD2;
				 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, TRUE, RTC_SECONDARY, pNew->Rect_Pos.left, pNew->Rect_Pos.top, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, (char*)pCA->pDP_Multiplay->HostsFound[lc1].pAppDesc->pwszSessionName);
				pNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
				//add commands
				pCmdNew = new PCMD2;
				DWORD* pdw = new DWORD; *pdw = lc1;
				pCmdNew->pData = (BYTE*)pdw;
				pCmdNew->wDataSize = sizeof(DWORD);
				pCmdNew->wType = CQ2TYPE_SET_ACTIVEHOST;
				pNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
			pHostsListBox->Add_IfcElem(pNew);
	}	


}

if (IBoxes[IBOX_MAINMENU].iActiveGrid == TEMP_IGRID_MULTI3)
{
	//if not the host, clear away the command attachment for the start button
	pMPStartButton->Clear_Cmds(IFC_ELEM_CAT_ONLUP);
		//only if host, add back command
		if (pCA->pGameOptions->bAmHost == TRUE)
		{
			PCMD2* pCmdNew = new PCMD2;
				pCmdNew->pData = new BYTE[1];  
				pCmdNew->wDataSize = 1;
				pCmdNew->wType = CQ2TYPE_SELECT_START;
				pMPStartButton->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		}
	//
	if (pGameLaunchUpdateRgn != NULL)
	{
		pGameLaunchUpdateRgn->Clear_IfcElems();

		//for now, simply write in the names of the players (12.23.2002)
		PDNODE* pPD_Trav1 = (PDNODE*)pCA->pGameOptions->pPD_AIFirst->pNext;
		lc1 = 0;
			while (pPD_Trav1 != pCA->pGameOptions->pPD_AILast)
			{
				cIfcElement* pElemNew = new cIfcElement;
				pElemNew->Init(XRes * 0.15, XRes * 0.35, YRes * (0.25 + lc1*0.05), YRes * (0.30 + lc1*0.05));
					PCMD2* pCmdNew = new PCMD2;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcListBox(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.right, pElemNew->Rect_Pos.top, pElemNew->Rect_Pos.bottom, &retsize);
					pCmdNew->wDataSize = retsize;
					pCmdNew->wType = CQ2TYPE_DRAWIFCLISTBOX;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
					pCmdNew = new PCMD2;
					char Text[100];
					Text[0] = 0;
					char* NormName = Conv_String_WideToReg2(reinterpret_cast<unsigned short*>(pPD_Trav1->wcsName));
					strcat(Text, NormName);
					delete [] NormName;
					if (pPD_Trav1->bHaveCurrentMap == TRUE)
					{
						strcat(Text, " (READY)");
					}
					if (pPD_Trav1->iDPNID == pCA->pGameOptions->DPNID_Host)
					{
						strcat(Text, " (HOST)");
					}
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left + 10, 0.5 * (pElemNew->Rect_Pos.top + pElemNew->Rect_Pos.bottom) - (ftheight * 0.5) , FONT_STANDARD1, pCA->pColorTable->White - 1,strlen(Text), Text, &retsize);
					pCmdNew->wDataSize = retsize;
					pCmdNew->wType = CQ2TYPE_DRAWTEXT;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
				pGameLaunchUpdateRgn->Add_IfcElem(pElemNew);
				lc1++;
				pPD_Trav1 = (PDNODE*)pPD_Trav1->pNext;
			}
		pPD_Trav1 = (PDNODE*)pCA->pGameOptions->pPD_First->pNext;
			while (pPD_Trav1 != pCA->pGameOptions->pPD_Last)
			{
				cIfcElement* pElemNew = new cIfcElement;
				pElemNew->Init(XRes * 0.15, XRes * 0.35, YRes * (0.25 + lc1*0.05), YRes * (0.30 + lc1*0.05));
					PCMD2* pCmdNew = new PCMD2;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcListBox(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.right, pElemNew->Rect_Pos.top, pElemNew->Rect_Pos.bottom, &retsize);
					pCmdNew->wDataSize = retsize;
					pCmdNew->wType = CQ2TYPE_DRAWIFCLISTBOX;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
					pCmdNew = new PCMD2;
					char Text[100];
					Text[0] = 0;
					char* NormName = Conv_String_WideToReg2(reinterpret_cast<unsigned short*>(pPD_Trav1->wcsName));
					strcat(Text, NormName);
					delete [] NormName;
					if (pPD_Trav1->bHaveCurrentMap == TRUE)
					{
						strcat(Text, " (READY)");
					}
					if (pPD_Trav1->iDPNID == pCA->pGameOptions->DPNID_Host)
					{
						strcat(Text, " (HOST)");
					}
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left + 10, 0.5 * (pElemNew->Rect_Pos.top + pElemNew->Rect_Pos.bottom) - (ftheight * 0.5) , FONT_STANDARD1, pCA->pColorTable->White - 1,strlen(Text), Text, &retsize);
					pCmdNew->wDataSize = retsize;
					pCmdNew->wType = CQ2TYPE_DRAWTEXT;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
				pGameLaunchUpdateRgn->Add_IfcElem(pElemNew);
				lc1++;
				pPD_Trav1 = (PDNODE*)pPD_Trav1->pNext;
			}		
		}
}

/*
	//clear out what was originally in the boxes
	for (lc1 = 0; lc1 < 6; lc1++)
	{
		Set_IGridCell_DrawFlags(&pGrid[(10+(lc1*3))*200 + 3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);	
			Set_IGridCell_CmdFlags_Box(pGrid, (10+(lc1*3))*200 + 3, 300/4, 3, 200, FALSE, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0);
		Set_IGridCell_DrawFlags(&pGrid[(41+(lc1*3))*200 + 3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);
			Set_IGridCell_CmdFlags_Box(pGrid, (41+(lc1*3))*200 + 3, 300/4, 3, 200, FALSE, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0);
		Set_IGridCell_DrawFlags(&pGrid[(75+(lc1*3))*200 + 3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);
			Set_IGridCell_CmdFlags_Box(pGrid, (75+(lc1*3))*200 + 3, 300/4, 3, 200, FALSE, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0);
	}

	//for now, simply set text strings in the Service Providers and Device boxes
	for (lc1 = 0; lc1 < pCA->pDP_Multiplay->dwNumProviders; lc1++)
	{
		Set_IGridCell_DrawFlags(&pGrid[(10+(lc1*3))*200 + 3], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0, (char*)pCA->pDP_Multiplay->ServiceProviders[lc1].pwszName, FONT_STANDARD1); 
			Set_IGridCell_CmdFlags_Box(pGrid, (10+(lc1*3))*200 + 3, 300/4, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SET_ACTIVEPROVIDER, lc1, 0, 0, 0, 0, NULL, 0);
	}
//fill in text strings if an active provider was selected

if (pCA->pDP_Multiplay->iActiveProvider >= 0)
	if (pCA->pDP_Multiplay->iActiveProvider >= 0)
	{
		//draw in the name of the SELECTED provider
		DPDEVICEINFOARRAY* pArray = &pCA->pDP_Multiplay->pDeviceArrays[pCA->pDP_Multiplay->iActiveProvider];
		if (pArray->wActiveDevice != -1)
			Set_IGridCell_DrawFlags(&pGrid[36*200 + 24], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,(char*)pArray->SPDevices[pArray->wActiveDevice].pwszName, FONT_STANDARD1);//Conv_String_WideToReg(pCA->pDP_Multiplay->pDeviceArrays[pCA->pDP_Multiplay->iActiveProvider].SPDevices[lc1].pwszName));		
		
		for (lc1 = 0; lc1 < pCA->pDP_Multiplay->pDeviceArrays[pCA->pDP_Multiplay->iActiveProvider].dwNumDevices; lc1++)
		{
//			WCHAR* wcSrc = pCA->pDP_Multiplay->pDeviceArrays[pCA->pDP_Multiplay->iActiveProvider].SPDevices[lc1].pwszName;
//			CHAR* cDest = new char[50];
//			if (pGrid[(41+(lc1*3))*200 + 3].Text != NULL) delete(pGrid[(41+(lc1*3))*200 + 3].Text);
//			WideCharToMultiByte(CP_ACP, 0, wcSrc, -1, cDest, 10, NULL, NULL);
			Set_IGridCell_DrawFlags(&pGrid[(41+(lc1*3))*200 + 3], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0, (char*)pArray->SPDevices[lc1].pwszName, FONT_STANDARD1);//Conv_String_WideToReg(pCA->pDP_Multiplay->pDeviceArrays[pCA->pDP_Multiplay->iActiveProvider].SPDevices[lc1].pwszName));		
			Set_IGridCell_CmdFlags_Box(pGrid, (41+(lc1*3))*200 + 3, 300/4, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SET_ACTIVEDEVICE, lc1, 0, 0, 0, 0, NULL, 0);
		}
	}

	//fill in text string for selected host
	if (pCA->pDP_Multiplay->wHostsFound > 0)
	{     
	Set_IGridCell_DrawFlags(&pGrid[(100)*200 + 35], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,(char*)pCA->pDP_Multiplay->HostsFound[pCA->pDP_Multiplay->wSelectedHost].pAppDesc->pwszSessionName, FONT_STANDARD1);
	}
	//fill in text strings if active hosts are found
	for (lc1 = 0; lc1 < pCA->pDP_Multiplay->wHostsFound; lc1++)
	{
		Set_IGridCell_DrawFlags(&pGrid[(75+(lc1*3))*200 + 3], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,(char*)pCA->pDP_Multiplay->HostsFound[lc1].pAppDesc->pwszSessionName, FONT_STANDARD1);//pCA->pDP_Multiplay->Write_Log_Special(pCA->pDP_Multiplay->HostsFound[lc1].pAppDesc->pwszSessionName));//Conv_String_WideToReg(pCA->pDP_Multiplay->pDeviceArrays[pCA->pDP_Multiplay->iActiveProvider].S			
		Set_IGridCell_CmdFlags_Box(pGrid, (75+(lc1*3))*200 + 3, 300/4, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SET_ACTIVEHOST, lc1, 0, 0, 0, 0, NULL, 0);
	}

	//TEST
	Set_IGridCell_DrawFlags(&pGrid[6*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Name/Handle:", FONT_STANDARD1);
	Set_IGridCell_DrawFlags(&pGrid[11*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,(char*)pCA->pGameOptions->InfoPlayer_Local.pwszName, FONT_STANDARD1);
	Set_IGridCell_CmdFlags_Box(pGrid, 10*200 + 90, 50, 5, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SET_INPUTSTRING_WIDE, 80, 0, 0, 0, 0, (void*)pCA->pGameOptions->InfoPlayer_Local.pwszName, 0);


	//beforehand...clear out all the text
	//clear out the two entry boxes and the top text for the six over entries
//	Set_IGridCell_DrawFlags(&pGrid[11*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0," ");  //for now, disable the top box because it will always hold NAME
	Set_IGridCell_DrawFlags(&pGrid[21*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);
//	Set_IGridCell_DrawFlags(&pGrid[6*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ");
	Set_IGridCell_DrawFlags(&pGrid[16*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);
	Set_IGridCell_DrawFlags(&pGrid[26*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);
	Set_IGridCell_DrawFlags(&pGrid[46*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);
	Set_IGridCell_DrawFlags(&pGrid[66*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);
	Set_IGridCell_DrawFlags(&pGrid[86*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);
	//and the four report entries next to the over entries
	Set_IGridCell_DrawFlags(&pGrid[26*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);
	Set_IGridCell_DrawFlags(&pGrid[46*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);
	Set_IGridCell_DrawFlags(&pGrid[66*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);
	Set_IGridCell_DrawFlags(&pGrid[86*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);

	for (lc1 = 0; lc1 < 5; lc1++)
	{
		Set_IGridCell_DrawFlags(&pGrid[(31+(lc1*3))*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);	
			Set_IGridCell_CmdFlags_Box(pGrid, (30+(lc1*3))*200 + 90, 50, 5, 200, FALSE, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0);
		Set_IGridCell_DrawFlags(&pGrid[(51+(lc1*3))*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);	
			Set_IGridCell_CmdFlags_Box(pGrid, (50+(lc1*3))*200 + 90, 50, 5, 200, FALSE, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0);
		Set_IGridCell_DrawFlags(&pGrid[(71+(lc1*3))*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);	
			Set_IGridCell_CmdFlags_Box(pGrid, (70+(lc1*3))*200 + 90, 50, 5, 200, FALSE, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0);
		Set_IGridCell_DrawFlags(&pGrid[(91+(lc1*3))*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ", FONT_STANDARD1);	
			Set_IGridCell_CmdFlags_Box(pGrid, (90+(lc1*3))*200 + 90, 50, 5, 200, FALSE, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0);
//		Set_IGridCell_DrawFlags(&pGrid[(41+(lc1*3))*200 + 3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0," ");
//			Set_IGridCell_CmdFlags_Box(pGrid, (41+(lc1*3))*200 + 3, 300/4, 3, 200, FALSE, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0);	

			
	}

	//set the right side of the window depending on what service provider(which GUID) is selected

if (pCA->pDP_Multiplay->iActiveProvider >= 0)
{
	if (pCA->pDP_Multiplay->ServiceProviders[pCA->pDP_Multiplay->iActiveProvider].guid == CLSID_DP8SP_SERIAL)
		{
		//baud window is the first
			Set_IGridCell_DrawFlags(&pGrid[26*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Baud:", FONT_STANDARD1);
			//a BIG kluge here, hard-coding BAUD values because I don't want to allocate a string to pass to the drawfield
			switch(pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].dwBaud)
			{
				case 19200:
				Set_IGridCell_DrawFlags(&pGrid[26*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "19200", FONT_STANDARD1);
				break;
				case 38400:
				Set_IGridCell_DrawFlags(&pGrid[26*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "38400", FONT_STANDARD1);
				break;
				case 57600:
				Set_IGridCell_DrawFlags(&pGrid[26*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "58600", FONT_STANDARD1);
				break;
				case 115200:
				Set_IGridCell_DrawFlags(&pGrid[26*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, 	"115200", FONT_STANDARD1);
				break;
			}
				//(sigh...)
				Set_IGridCell_DrawFlags(&pGrid[(31+0*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"19200", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (31+(0*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_BAUD, 19200, 0, 0, 0, 0, NULL, 0);
				Set_IGridCell_DrawFlags(&pGrid[(31+1*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"38400", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (31+(1*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_BAUD, 38400, 0, 0, 0, 0, NULL, 0);
				Set_IGridCell_DrawFlags(&pGrid[(31+2*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"57600", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (31+(2*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_BAUD, 57600, 0, 0, 0, 0, NULL, 0);
				Set_IGridCell_DrawFlags(&pGrid[(31+3*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"115200", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (31+(3*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_BAUD, 115200, 0, 0, 0, 0, NULL, 0);
				
			//then flow control window
			Set_IGridCell_DrawFlags(&pGrid[46*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, (char*)pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsFlowControl, FONT_STANDARD1);	
			Set_IGridCell_DrawFlags(&pGrid[46*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Flow Control:", FONT_STANDARD1);
			if (pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsFlowControl == DPNA_FLOW_CONTROL_RTSDTR)
				Set_IGridCell_DrawFlags(&pGrid[46*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "RTSDTR", FONT_STANDARD1);
			if (pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsFlowControl == DPNA_FLOW_CONTROL_RTS)
				Set_IGridCell_DrawFlags(&pGrid[46*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "RTS", FONT_STANDARD1);
			if (pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsFlowControl == DPNA_FLOW_CONTROL_DTR)
				Set_IGridCell_DrawFlags(&pGrid[46*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "DTR", FONT_STANDARD1);
			if (pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsFlowControl == DPNA_FLOW_CONTROL_XONXOFF)
				Set_IGridCell_DrawFlags(&pGrid[46*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "XONXOFF", FONT_STANDARD1);
				//(sigh...)
				Set_IGridCell_DrawFlags(&pGrid[(51+0*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"RTSDTR", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (51+(0*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_FLOWCONTROL_RTSDTR, 0, 0, 0, 0, 0, NULL, 0);
				Set_IGridCell_DrawFlags(&pGrid[(51+1*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"RTS", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (51+(1*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_FLOWCONTROL_RTS, 0, 0, 0, 0, 0, NULL, 0);
				Set_IGridCell_DrawFlags(&pGrid[(51+2*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"DTR", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (51+(2*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_FLOWCONTROL_DTR, 0, 0, 0, 0, 0, NULL, 0);
				Set_IGridCell_DrawFlags(&pGrid[(51+3*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"XONXOFF", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (51+(3*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_FLOWCONTROL_XONXOFF, 0, 0, 0, 0, 0, NULL, 0);

			//then parity window
			Set_IGridCell_DrawFlags(&pGrid[66*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Parity:", FONT_STANDARD1);		
			if (pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsParity == DPNA_PARITY_EVEN)
				Set_IGridCell_DrawFlags(&pGrid[66*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "EVEN", FONT_STANDARD1);
			if (pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsParity == DPNA_PARITY_ODD)
				Set_IGridCell_DrawFlags(&pGrid[66*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "ODD", FONT_STANDARD1);
			if (pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsParity == DPNA_PARITY_MARK)
				Set_IGridCell_DrawFlags(&pGrid[66*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "MARK", FONT_STANDARD1);
			if (pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsParity == DPNA_PARITY_SPACE)
				Set_IGridCell_DrawFlags(&pGrid[66*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "SPACE", FONT_STANDARD1);
			if (pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsParity == DPNA_PARITY_NONE)
				Set_IGridCell_DrawFlags(&pGrid[66*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "NONE", FONT_STANDARD1);
				//(sigh...)
				Set_IGridCell_DrawFlags(&pGrid[(71+0*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"EVEN", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (71+(0*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_PARITY_EVEN, 0, 0, 0, 0, 0, NULL, 0);
				Set_IGridCell_DrawFlags(&pGrid[(71+1*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"ODD", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (71+(1*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_PARITY_ODD, 0, 0, 0, 0, 0, NULL, 0);
				Set_IGridCell_DrawFlags(&pGrid[(71+2*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"MARK", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (71+(2*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_PARITY_MARK, 0, 0, 0, 0, 0, NULL, 0);
				Set_IGridCell_DrawFlags(&pGrid[(71+3*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"SPACE", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (71+(3*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_PARITY_SPACE, 0, 0, 0, 0, 0, NULL, 0);
				Set_IGridCell_DrawFlags(&pGrid[(71+4*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"NONE", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (71+(4*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_PARITY_NONE, 0, 0, 0, 0, 0, NULL, 0);

			//then stop bits window
			Set_IGridCell_DrawFlags(&pGrid[86*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Stop Bits:", FONT_STANDARD1);
			if (pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsStopBits == DPNA_STOP_BITS_ONE)
				Set_IGridCell_DrawFlags(&pGrid[86*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "1", FONT_STANDARD1);
			if (pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsStopBits == DPNA_STOP_BITS_ONE_FIVE)
				Set_IGridCell_DrawFlags(&pGrid[86*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "1.5", FONT_STANDARD1);
			if (pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsStopBits == DPNA_STOP_BITS_TWO)
				Set_IGridCell_DrawFlags(&pGrid[86*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "2", FONT_STANDARD1);
				//(sigh..)
				Set_IGridCell_DrawFlags(&pGrid[(91+0*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"1", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (91+(0*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_STOPBITS_ONE, 0, 0, 0, 0, 0, NULL, 0);
				Set_IGridCell_DrawFlags(&pGrid[(91+1*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"1.5", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (91+(1*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_STOPBITS_ONEFIVE, 0, 0, 0, 0, 0, NULL, 0);
				Set_IGridCell_DrawFlags(&pGrid[(91+2*3)*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"2", FONT_STANDARD1);				
					Set_IGridCell_CmdFlags_Box(pGrid, (91+(2*3))*200 + 90, 50, 3, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SETSERIAL_STOPBITS_TWO, 0, 0, 0, 0, 0, NULL, 0);

	}
	if (pCA->pDP_Multiplay->ServiceProviders[pCA->pDP_Multiplay->iActiveProvider].guid == CLSID_DP8SP_MODEM)
		{
//			pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcsPhoneNumber[0] = 0;
		
			//second string box receives phone number text
			Set_IGridCell_DrawFlags(&pGrid[16*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"Phone Number:", FONT_STANDARD1);
			Set_IGridCell_DrawFlags(&pGrid[21*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,(char*)pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcaPhoneNumber, FONT_STANDARD1);
			Set_IGridCell_CmdFlags_Box(pGrid, 20*200 + 90, 50, 5, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SET_INPUTSTRING_WIDE, 80, 0, 0, 0, 0, (void*)pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcaPhoneNumber, 0);
				

		}
	if (pCA->pDP_Multiplay->ServiceProviders[pCA->pDP_Multiplay->iActiveProvider].guid == CLSID_DP8SP_TCPIP)	
		{
			//second string box receives IP address
			Set_IGridCell_DrawFlags(&pGrid[16*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"IP Address:", FONT_STANDARD1);
			Set_IGridCell_DrawFlags(&pGrid[21*200 + 91], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,(char*)pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcaIP, FONT_STANDARD1);
			Set_IGridCell_CmdFlags_Box(pGrid, 20*200 + 90, 50, 5, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SET_INPUTSTRING_WIDE, 80, 0, 0, 0, 0, (void*)pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcaIP, 0);
			//third string box receives IP port 
			Set_IGridCell_DrawFlags(&pGrid[26*200 + 90], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,"IP Port:", FONT_STANDARD1);
			Set_IGridCell_DrawFlags(&pGrid[26*200 + 130], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,(char*)pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].wcaPortIP, FONT_STANDARD1);
			Set_IGridCell_CmdFlags_Box(pGrid, 26*200 + 130, 50, 5, 200, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SET_INPUTSTRING_WIDE, 80, 0, 0, 0, 0, (void*)pCA->pDP_Multiplay->pSPConnectInfo[pCA->pDP_Multiplay->iActiveProvider].dwPortIP, 0);
		}
}

//this section for updating the MULTI2 grid
pGrid = IBoxes[IBOX_MAINMENU].pIGrids[TEMP_IGRID_MULTI2].pGrid;
//this is a 40x30 at 16x16 grid

	//for now, simply write in the names of the players (12.23.2002)
	PDNODE* pPD_Trav1 = (PDNODE*)pCA->pGameOptions->pPD_AIFirst->pNext;
	lc1 = 0;
		while (pPD_Trav1 != pCA->pGameOptions->pPD_AILast)
		{
			Set_IGridCell_DrawFlags(&pGrid[(2 + lc1)*40 + 3], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,(char*)pPD_Trav1->wcsName, FONT_STANDARD1);	
			lc1++;
			pPD_Trav1 = (PDNODE*)pPD_Trav1->pNext;
		}
	pPD_Trav1 = (PDNODE*)pCA->pGameOptions->pPD_First->pNext;
		while (pPD_Trav1 != pCA->pGameOptions->pPD_Last)
		{
			Set_IGridCell_DrawFlags(&pGrid[(2 + lc1)*40 + 3], TRUE, IGRIDCELL_DRAWTYPE_TEXTWIDE, 0, 0, 0, 0, 0, 0,(char*)pPD_Trav1->wcsName, FONT_STANDARD1);	
			lc1++;
			pPD_Trav1 = (PDNODE*)pPD_Trav1->pNext;
		}
	
	//start button ONLY if Host
	//a start button
	if(pCA->pGameOptions->bAmHost == TRUE)
	{
		Set_IGridCell_DrawFlags(&pGrid[20*40 + 20], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBUTTONS00, 0, 0, MBUTTON_SIZE_X - 1, MBUTTON_SIZE_Y * 6, MBUTTON_SIZE_Y * 7, NULL, 0);
		Set_IGridCell_CmdFlags_Box(pGrid, 20*40 + 20, 3, 1, 40, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SELECT_START, 0, 0, 0, 0, 0, NULL, 0);
	}
/*
	//a send button
	Set_IGridCell_DrawFlags(&pGrid[25*40 + 20], TRUE, IGRIDCELL_DRAWTYPE_BLIT, ISURF_MENUBUTTONS00, 0, 0, MBUTTON_SIZE_X - 1, MBUTTON_SIZE_Y * 8, MBUTTON_SIZE_Y * 9, NULL, 0);
	Set_IGridCell_CmdFlags_Box(pGrid, 25*40 + 20, 3, 1, 40, TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SENDTEXT, 80, 0, 0, 0, 0, (void*)caChatText, 0);
*/

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

void cIBox::Init(VOID* pCA)
{
	pCAccess = pCA;

	iNumGrids = 0;
	pIGrids = NULL;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cIBox::Exit()
{
	//here, just delete the number of cIGrids that were created
	//first call exit on each of the created grids
	for (c1 = 0; c1 < iNumGrids; c1++)
	{
		pIGrids[c1].Exit();
	}

	//then, delete the entire pointer
	if (pIGrids != NULL)
		delete(pIGrids);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Set_IGridCell_AllFlags(IGRIDCELL* pCell, BOOL bDrawCell, BYTE bActiveCommand, BYTE bDataSources, BYTE bDrawType,
									WORD wISurfIndex, WORD wFrame, WORD wSurfRect_left, WORD wSurfRect_right, WORD wSurfRect_top,
									WORD wSurfRect_bottom, char* Text, int iFont, WORD PC_wDest, WORD PC_wType, LONG PC_wParam1, WORD PC_wParam2, WORD PC_wParam3,
									double PC_fParam4, double PC_fParam5, void* PC_pParam6, LONG PC_lExecTick,
									DWORD dwNumObjRefs, DWORD dwObjRefType, VOID** pObjRefArray, DWORD* pdwIDArray)
{
	pCell->bDrawCell = bDrawCell;
	pCell->bActiveCommand = bActiveCommand;
	pCell->bDataSources = bDataSources;
	pCell->DrawType = bDrawType;
	pCell->wISurfIndex = wISurfIndex;
	pCell->wFrame = wFrame;
	pCell->SurfRect.left = wSurfRect_left;
	pCell->SurfRect.right = wSurfRect_right;
	pCell->SurfRect.top = wSurfRect_top;
	pCell->SurfRect.bottom = wSurfRect_bottom;

	pCell->ColorR = VAL_INVALID;
	pCell->ColorG = VAL_INVALID;
	pCell->ColorB = VAL_INVALID;


	//add the object references(05.20.2003)
	pCell->dwNumObjRefs = dwNumObjRefs;
	pCell->dwObjRefType = dwObjRefType;
	
if (pCell->pObjRefArray != NULL)
	delete(pCell->pObjRefArray);
	
	pCell->pObjRefArray = pObjRefArray;

if (pCell->pdwIDArray != NULL)
	delete(pCell->pdwIDArray);

	pCell->pdwIDArray = pdwIDArray;

//delete anything Text might have been before reassigning it
// delete(pCell->Text);
//	pCell->Text = Text;
//	if (Text != NULL)
//	{
//		if (pCell->Text == NULL)
//			pCell->Text = new char[80];
//
//		ZeroMemory(pCell->Text, sizeof(char) * 80);
//		if (bDrawType != IGRIDCELL_DRAWTYPE_TEXTWIDE)
//			strcat(pCell->Text, Text);
//				else
//			wcscat((WCHAR*)pCell->Text, (WCHAR*)Text);
//	}
	if (Text != NULL)
	{

		if (pCell->Text == NULL)
			pCell->Text = new char[80];
	
		ZeroMemory(pCell->Text, sizeof(char) * 80);
	
		int iStrlen = strlen(Text);
		int iWStrlen = 0;
			if (bDrawType == IGRIDCELL_DRAWTYPE_TEXTWIDE)
				wcslen((WCHAR*)Text);

		if (iStrlen >= 80)
			iStrlen = 80;
		if (iWStrlen >= 35)
			iWStrlen = 35;

		if (bDrawType != IGRIDCELL_DRAWTYPE_TEXTWIDE)
			memcpy(pCell->Text, Text, sizeof(char) * iStrlen);
				else
			memcpy(pCell->Text, Text, sizeof(char) * iWStrlen * 2);


	}
		else
			pCell->Text = NULL;
	//memcpy(pCell->Text, Text, strlen(Text));
	pCell->iFont = iFont;

	pCell->ProgCommand.wDest = PC_wDest;
	pCell->ProgCommand.wType = PC_wType;
	pCell->ProgCommand.wParam1 = PC_wParam1;
	pCell->ProgCommand.wParam2 = PC_wParam2;
	pCell->ProgCommand.wParam3 = PC_wParam3;
	pCell->ProgCommand.fParam4 = PC_fParam4;
	pCell->ProgCommand.fParam5 = PC_fParam5;
	pCell->ProgCommand.pParam6 = PC_pParam6;
	pCell->ProgCommand.lExecTick = PC_lExecTick;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Set_IGridCell_DrawFlags(IGRIDCELL* pCell, BOOL bDrawCell, BYTE bDrawType,
									WORD wISurfIndex, WORD wFrame, WORD wSurfRect_left, WORD wSurfRect_right, WORD wSurfRect_top,
									WORD wSurfRect_bottom, char* Text, int iFont)
{ //07.11.2002
	pCell->bDrawCell = bDrawCell;
	pCell->DrawType = bDrawType;
	pCell->wISurfIndex = wISurfIndex;
	pCell->wFrame = wFrame;
	pCell->SurfRect.left = wSurfRect_left;
	pCell->SurfRect.right = wSurfRect_right;
	pCell->SurfRect.top = wSurfRect_top;
	pCell->SurfRect.bottom = wSurfRect_bottom;

	pCell->ColorR = VAL_INVALID;
	pCell->ColorG = VAL_INVALID;
	pCell->ColorB = VAL_INVALID;

//delete anything Text might have been before reassigning it
//	if(pCell->Text != NULL) {	delete( pCell->Text);}//char NText[50];
//								itoa(_msize(pCell->Text),NText, 10);
//								Write_Log("-"); Write_Log(NText); Write_Log("-");}
//pCell->Text = Text;	
	//memcpy(pCell->Text, Text, strlen(Text));
	if (Text != NULL)
	{
		if (pCell->Text == NULL)
			pCell->Text = new char[80];
		
		ZeroMemory(pCell->Text, sizeof(char) * 80);
		
		int iStrlen = strlen(Text);
		int iWStrlen = wcslen((WCHAR*)Text);

		if (iStrlen >= 80)
			iStrlen = 80;
		if (iWStrlen >= 35)
			iWStrlen = 35;

		if (bDrawType != IGRIDCELL_DRAWTYPE_TEXTWIDE)
			memcpy(pCell->Text, Text, sizeof(char) * iStrlen);
				else
			memcpy(pCell->Text, Text, sizeof(char) * iWStrlen * 2);

	}
		else
			pCell->Text = NULL;
//delete(pCell->Text);
	//(08.12.2003)
	//try this alternate scheme for text...ing
	//this relies heavily on the implication that a pCell->Text pointer that is not NULL
	//actually points to a valid buffer of length 80 chars
	//thus, we must make sure that all cells start out with NULL Text pointers.
	//then, if a cell is given a DrawText command, this "enables" the buffer, allocating it.
	//then, in the future, when future draw text calls are issued to it, it uses the same buffer

//	pCell->Text = Text;
	pCell->iFont = iFont;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Set_IGridCell_CmdFlags(IGRIDCELL* pCell, BYTE bActiveCommand, BYTE bDataSources, WORD PC_wDest, WORD PC_wType, LONG PC_wParam1, WORD PC_wParam2, WORD PC_wParam3,
									double PC_fParam4, double PC_fParam5, void* PC_pParam6, LONG PC_lExecTick)
{	//07.11.2002
	pCell->bActiveCommand = bActiveCommand;
	pCell->bDataSources = bDataSources;
	pCell->ProgCommand.wDest = PC_wDest;
	pCell->ProgCommand.wType = PC_wType;
	pCell->ProgCommand.wParam1 = PC_wParam1;
	pCell->ProgCommand.wParam2 = PC_wParam2;
	pCell->ProgCommand.wParam3 = PC_wParam3;
	pCell->ProgCommand.fParam4 = PC_fParam4;
	pCell->ProgCommand.fParam5 = PC_fParam5;
	pCell->ProgCommand.pParam6 = PC_pParam6;
	pCell->ProgCommand.lExecTick = PC_lExecTick;


	pCell->ColorR = VAL_INVALID;
	pCell->ColorG = VAL_INVALID;
	pCell->ColorB = VAL_INVALID;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Set_IGridCell_CmdFlags_Box(IGRIDCELL* pCellArray, WORD wStartIndex, WORD wWidth, WORD wHeight, WORD wElements_Array_Width,
									BYTE bActiveCommand, BYTE bDataSources, WORD PC_wDest, WORD PC_wType, LONG PC_wParam1, WORD PC_wParam2, WORD PC_wParam3,
									double PC_fParam4, double PC_fParam5, void* PC_pParam6, LONG PC_lExecTick)
{  //07.11.2002
WORD wIG = 0;

	WORD lc1, lc2; //local counters
	for (lc1 = 0; lc1 < wWidth; lc1++)
	for (lc2 = 0; lc2 < wHeight; lc2++)
	{
		wIG = wStartIndex + (lc2 * wElements_Array_Width) + lc1;
		pCellArray[wIG].bActiveCommand = bActiveCommand;
		pCellArray[wIG].bDataSources = bDataSources;
		pCellArray[wIG].ProgCommand.wDest = PC_wDest;
		pCellArray[wIG].ProgCommand.wType = PC_wType;
		pCellArray[wIG].ProgCommand.wParam1 = PC_wParam1;
		pCellArray[wIG].ProgCommand.wParam2 = PC_wParam2;
		pCellArray[wIG].ProgCommand.wParam3 = PC_wParam3;
		pCellArray[wIG].ProgCommand.fParam4 = PC_fParam4;
		pCellArray[wIG].ProgCommand.fParam5 = PC_fParam5;
		pCellArray[wIG].ProgCommand.pParam6 = PC_pParam6;
		pCellArray[wIG].ProgCommand.lExecTick = PC_lExecTick;

		pCellArray[wIG].ColorR = VAL_INVALID;
		pCellArray[wIG].ColorG = VAL_INVALID;
		pCellArray[wIG].ColorB = VAL_INVALID;

	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Set_IGridCell_ObjRefFlags(IGRIDCELL* pCell, DWORD dwNumObjRefs, DWORD dwObjRefType, VOID** pObjRefArray, DWORD* pdwIDArray)
{
	//add the object references(05.20.2003)
	pCell->dwNumObjRefs = dwNumObjRefs;
	pCell->dwObjRefType = dwObjRefType;
	
if (pCell->pObjRefArray != NULL)
	delete(pCell->pObjRefArray);
	
	pCell->pObjRefArray = pObjRefArray;

if (pCell->pdwIDArray != NULL)
	delete(pCell->pdwIDArray);

	pCell->pdwIDArray = pdwIDArray;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Conv_String_WideToReg(unsigned short* Text)
{

DWORD numbyteswritten = 0;
char* cText = (char*)Text;
char* RetText = new char[ lstrlenW(reinterpret_cast<LPCWSTR>(Text)) + 1]; //+1 to add NULL-termination

for (int lc6 = 0; lc6 < lstrlenW(reinterpret_cast<LPCWSTR>(Text)); lc6++)
{

RetText[lc6] = *(cText + (lc6 *2));
}
RetText[lstrlenW(reinterpret_cast<LPCWSTR>(Text))] = '\0';

//instead of returning, switch the pointers:
//delete(Text);
Text = (unsigned short*)RetText;



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

//08.09.2003
char* cInterface::Conv_String_WideToReg2(unsigned short* Text)
{

DWORD numbyteswritten = 0;
char* cText = (char*)Text;
char* RetText = new char[lstrlenW(reinterpret_cast<LPCWSTR>(Text)) + 1]; //+1 to add NULL-termination

for (int lc6 = 0; lc6 < lstrlenW(reinterpret_cast<LPCWSTR>(Text)); lc6++)
{

RetText[lc6] = *(cText + (lc6 *2));
}
RetText[lstrlenW(reinterpret_cast<LPCWSTR>(Text))] = '\0';

return RetText; //note that the user is now responsible for also de-allocating the pointer that this function returns



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

void cInterface::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cInterface.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_INTERFACE >= 1)
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
void cInterface::Edit_String(int iVirtualKey, char* TextEdit, WORD wMaxBufferSize)
{
	char* TextEdit_New = TextEdit;//(char*)realloc(TextEdit, sizeof(char) * (strlen(TextEdit)+2));  //+2 so as to make sure the null-terminator was included
	WORD wStrlen = strlen(TextEdit_New);
	cClassAccess* pCA = (cClassAccess*)pCAccess;

char NText[50];
itoa(wStrlen, NText, 10);
Write_Log("-");
Write_Log(NText);
Write_Log("-");



if (wStrlen < wMaxBufferSize - 1)
{
	//normal editing keys are listed in entry table
	TextEdit_New[wStrlen] = pCA->pInputState->DI_ANSI_Table[iVirtualKey];
//if shift key ON (code 1), use the shift table
	if (pCA->pInputState->IGState_Keyboard[DIK_LSHIFT] == 1 || pCA->pInputState->IGState_Keyboard[DIK_RSHIFT] == 1)
	TextEdit_New[wStrlen] = (unsigned short)pCA->pInputState->DI_ANSI_Table_Shift[iVirtualKey];	
	
	TextEdit_New[wStrlen+1] = '\0';
	
	switch(iVirtualKey)  //special formatting keys
	{
		case DIK_BACK:
			//replace prev character with null-terminate
			TextEdit_New[wStrlen - 1] = '\0';			
		break;
	}
}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Edit_String_Wide(int iVirtualKey, WCHAR* TextEdit, WORD wMaxBufferSize)
{
	WCHAR* TextEdit_New = TextEdit; //(WCHAR*)realloc(TextEdit, sizeof(WCHAR) * (lstrlenW(TextEdit)+2));
	WORD wStrlen = wcslen(TextEdit_New);//lstrlenW(TextEdit_New);
	cClassAccess* pCA = (cClassAccess*)pCAccess;

//DEBUG
Write_Log("[Edit String Wide] Function was called.");
WriteLog_Enter();


if (wStrlen < wMaxBufferSize - 1)
{
	//normal editing keys are listed in entry table
	TextEdit_New[wStrlen] = (unsigned short)pCA->pInputState->DI_ANSI_Table[iVirtualKey];

//if shift key ON (code 1), use the shift table
	if (pCA->pInputState->IGState_Keyboard[DIK_LSHIFT] == 1 || pCA->pInputState->IGState_Keyboard[DIK_RSHIFT] == 1)
	TextEdit_New[wStrlen] = (unsigned short)pCA->pInputState->DI_ANSI_Table_Shift[iVirtualKey];	
		
	TextEdit_New[wStrlen+1] = 0;
		
	
	switch(iVirtualKey)  //special formatting keys
	{
		case DIK_BACK:
			//replace prev character with null-terminate
			TextEdit_New[wStrlen - 1] = 0;			
		break;
	}
}

//DEBUG
Write_Log("[Edit String Wide] Function was completed.");
WriteLog_Enter();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
SQCOORDSET cInterface::Calc_PosMinimap(int iXPos, int iYPos)
{
	int iX = iXPos;
	int iY = iYPos;
	SQCOORDSET RetSQ;
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//first subtract by the minimap position
	iX -= pCA->pGDisplay->MinimapDrawRect.left;
	iY -= pCA->pGDisplay->MinimapDrawRect.top;

	//then multiply by the rectwidth/rectheight ratio
	int rectheight = pCA->pGDisplay->MinimapDrawRect.bottom - pCA->pGDisplay->MinimapDrawRect.top;
	int rectwidth = pCA->pGDisplay->MinimapDrawRect.right - pCA->pGDisplay->MinimapDrawRect.left;

	RetSQ.x = iX * ( float(pCA->pGameWorld->iWidth)/ rectwidth);
	RetSQ.y = iY * ( float(pCA->pGameWorld->iHeight)/ rectheight);

	//curb the numbers inside viewbounds
	if ( RetSQ.x  < 0)
		 RetSQ.x = 0;
	if ( RetSQ.y < 0)
		 RetSQ.y = 0;
	if ( RetSQ.x >= pCA->pGameWorld->iWidth)// - pCA->pGDisplay->MDRAW_NUMCOLS - 1)
		RetSQ.x = pCA->pGameWorld->iWidth - 1;// - pCA->pGDisplay->MDRAW_NUMCOLS - 1;
	if ( RetSQ.y >= pCA->pGameWorld->iHeight)// - pCA->pGDisplay->MDRAW_NUMROWS - 1)
		RetSQ.y = pCA->pGameWorld->iHeight - 1;// - pCA->pGDisplay->MDRAW_NUMROWS - 1;

	RetSQ.Calc_Index(pCA->pGameWorld->iWidth);

	return RetSQ;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
SQCOORDSET cInterface::Calc_PosMinimapZoom(int iXPos, int iYPos)
{
	int iX = iXPos;
	int iY = iYPos;
	SQCOORDSET RetSQ;
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//first subtract by the minimap position
	iX -= pCA->pGDisplay->MinimapZoomDrawRect.left;
	iY -= pCA->pGDisplay->MinimapZoomDrawRect.top;

	//then multiply by the rectwidth/rectheight ratio
//	int rectheight = pCA->pGDisplay->MinimapDrawRect.bottom - pCA->pGDisplay->MinimapDrawRect.top;
//	int rectwidth = pCA->pGDisplay->MinimapDrawRect.right - pCA->pGDisplay->MinimapDrawRect.left;

	//the zoomed minimap has the advantage of having 1:1 pixel/tile ratio
	RetSQ.x = iX + pCA->pMinimap->sqMMZOffset.x + map_xview; 
	RetSQ.y = iY + pCA->pMinimap->sqMMZOffset.y + map_yview;

	//curb the numbers inside viewbounds
	if ( RetSQ.x  < 0)
		 RetSQ.x = 0;
	if ( RetSQ.y < 0)
		 RetSQ.y = 0;
	if ( RetSQ.x >= pCA->pGameWorld->iWidth)// - pCA->pGDisplay->MDRAW_NUMCOLS - 1)
		RetSQ.x = pCA->pGameWorld->iWidth - 1;// - pCA->pGDisplay->MDRAW_NUMCOLS - 1;
	if ( RetSQ.y >= pCA->pGameWorld->iHeight)// - pCA->pGDisplay->MDRAW_NUMROWS - 1)
		RetSQ.y = pCA->pGameWorld->iHeight - 1;// - pCA->pGDisplay->MDRAW_NUMROWS - 1;

	RetSQ.Calc_Index(pCA->pGameWorld->iWidth);

	return RetSQ;
}

//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Init_Console()
{
		cClassAccess* pCA = (cClassAccess*)pCAccess;

int iScrollBarWidth = pCA->pImages[ISURF_SCROLLBAR]->DDSDImage.dwWidth;

//initialize offsite storage boxes
		pConsoleCommandListBox = new cIfcElement;
			pConsoleCommandListBox->Init(0, 0, 1, 1);
		pConsoleInfoListBox = new cIfcElement;
			pConsoleInfoListBox->Init(0,0,1,1);
		pConsoleDisplayUpdate = new cIfcElement;
			pConsoleDisplayUpdate->Init(IBoxes[IBOX_CONSOLE_DISPLAY].BoxPos.left, IBoxes[IBOX_CONSOLE_DISPLAY].BoxPos.right, IBoxes[IBOX_CONSOLE_DISPLAY].BoxPos.top, IBoxes[IBOX_CONSOLE_DISPLAY].BoxPos.bottom);
		pConsoleGroupList = new cIfcElement;
			pConsoleGroupList->Init(0,0,1,1);
		pConsoleEquipListBox = new cIfcElement;
			pConsoleEquipListBox->Init(0,0,1,1);
	

			
cIBox* pIBox = &IBoxes[IBOX_CONSOLE_EQUIP];

cIfcElement* pElemNew;

//initialize the console (03.12.2003) //redundant!
	//temporary-just-to-get-it-working initialization(03.12.2003)
	pIBox->pIGrids = new cIGrid[1];  //for now, init only 1 igrid
	pIBox->iNumGrids = 1;
		//find out the number of equip "slots" that would fit in this grid
		int iSlots = (pIBox->BoxPos.bottom - pIBox->BoxPos.top) / (pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight);
		//initialize the igrids inside(this will be a 1xiSlots grid) (changed to 1x1) (11.13.2005)
		pIBox->pIGrids[0].Init(pIBox->BoxPos.left + 15, pIBox->BoxPos.top + 10, 1, 1, pIBox->BoxPos.right - pIBox->BoxPos.left, pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight);
		//initially set active box to 0
		pIBox->iActiveGrid = 0;
		pElemNew = Create_IfcPanel(pIBox->BoxPos.left, pIBox->BoxPos.right, pIBox->BoxPos.top, pIBox->BoxPos.bottom);
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);

		//
		pElemNew = Create_IfcListBox2(pIBox->BoxPos.left, pIBox->BoxPos.right, pIBox->BoxPos.top, pIBox->BoxPos.bottom, pConsoleEquipListBox);
		int iLeft = pIBox->BoxPos.left + 5;
		int iRight = pIBox->BoxPos.right - 10;
		int iTop = pIBox->BoxPos.top + 5;
		int iYSpan = pIBox->BoxPos.bottom -  pIBox->BoxPos.top - 10;
		int iYSpanListing = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;
		int iNumSlots = iYSpan / iYSpanListing;

		for (int lcy = 0; lcy < iNumSlots; lcy++)
		{
			Add_ListBoxListing(pElemNew, lcy, iLeft, iRight, iTop + iYSpanListing * lcy, iTop + iYSpanListing * (lcy+1));
		}
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);	

		//pIBox->pIGrids[0].Set_Scrollbar(TRUE, 0, 1, 0);

/*
		cIfcElement* pElemNew = Create_IfcPanel(IBoxes[IBOX_CONSOLE_EQUIP].BoxPos.left,IBoxes[IBOX_CONSOLE_EQUIP].BoxPos.right,IBoxes[IBOX_CONSOLE_EQUIP].BoxPos.top,IBoxes[IBOX_CONSOLE_EQUIP].BoxPos.bottom);
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);		
*/		
//now set the list menus
pIBox = &IBoxes[IBOX_CONSOLE_DISPLAY];
	//for now, init 3 IGrids, a unit list(0), a list of unit types/component types for prompting a user during construction(1), and a screen for showing/manipulating a structure's build queue (08.09.2003)
	pIBox->pIGrids = new cIGrid[1];
	pIBox->iNumGrids = 1;
	pIBox->iActiveGrid = 0;
	pIBox->pIGrids[0].Init(pIBox->BoxPos.left, pIBox->BoxPos.top, 1, 1, pIBox->BoxPos.right - pIBox->BoxPos.left, pIBox->BoxPos.bottom - pIBox->BoxPos.top);
		//TEST
		//pIBox->pIGrids[0].Set_Scrollbar(TRUE, 0, 100, 0);
		pElemNew = Create_IfcPanel(pIBox->BoxPos.left, pIBox->BoxPos.right, pIBox->BoxPos.top, pIBox->BoxPos.bottom);
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);

		pIBox->pIGrids[0].Add_IfcElem(pConsoleDisplayUpdate);
/*
	    pElemNew = Create_SquareButton(pCA->pDD_Display->i_XResD * 0.5, pCA->pDD_Display->i_YResD * 0.9, 0);
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);
	    pElemNew = Create_SquareButton(pCA->pDD_Display->i_XResD * 0.5-24, pCA->pDD_Display->i_YResD * 0.9, 1);
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);
	    pElemNew = Create_SquareButton(pCA->pDD_Display->i_XResD * 0.5-48, pCA->pDD_Display->i_YResD * 0.9, 2);
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);
	    pElemNew = Create_SquareButton(pCA->pDD_Display->i_XResD * 0.5-72, pCA->pDD_Display->i_YResD * 0.9, 3);
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);
		pElemNew = Create_ObjectListing(pCA->pDD_Display->i_XResD * 0.4, pCA->pDD_Display->i_YResD * 0.85, 1, "Lexington");
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);
*/

//now set the list menus
pIBox = &IBoxes[IBOX_CONSOLE_LIST];
	//for now, init 3 IGrids, a unit list(0), a list of unit types/component types for prompting a user during construction(1), and a screen for showing/manipulating a structure's build queue (08.09.2003)
	pIBox->pIGrids = new cIGrid[3];
	pIBox->iNumGrids = 3;

	//put the GROUP list in slot 0
	iSlots = (pIBox->BoxPos.bottom - pIBox->BoxPos.top) / (pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight);
	//initialize the igrids 0 and 1 (these will be 1xiSlots grids)
	pIBox->pIGrids[0].Init(pIBox->BoxPos.left + 15, pIBox->BoxPos.top + 10, 2, iSlots, (pIBox->BoxPos.right - pIBox->BoxPos.left)/3, pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight);
		//add panel
		pElemNew = Create_IfcPanel(pIBox->BoxPos.left, pIBox->BoxPos.right, pIBox->BoxPos.top, pIBox->BoxPos.bottom);
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);
		//set up listbox
		pElemNew = Create_IfcListBox2(pIBox->BoxPos.left, pIBox->BoxPos.right, pIBox->BoxPos.top, pIBox->BoxPos.bottom, pConsoleGroupList);
		iLeft = pIBox->BoxPos.left + 5;
		iRight = pIBox->BoxPos.right - 10;
		iTop = pIBox->BoxPos.top + 5;
		iYSpan = pIBox->BoxPos.bottom -  pIBox->BoxPos.top - 10;
		iYSpanListing = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;
		iNumSlots = iYSpan / iYSpanListing;

		for (int lcy2 = 0; lcy2 < iNumSlots; lcy2++)
		{
			Add_ListBoxListing(pElemNew, lcy2, iLeft, iRight, iTop + iYSpanListing * lcy2, iTop + iYSpanListing * (lcy2+1));
		}
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);

	pIBox->pIGrids[1].Init(pIBox->BoxPos.left + 15, pIBox->BoxPos.top + 10, 1, iSlots, (pIBox->BoxPos.right - pIBox->BoxPos.left)/1, pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight);
		//TEST
		//pIBox->pIGrids[1].Set_Scrollbar(TRUE, 0, 1, 0);
		//add panel
		pElemNew = Create_IfcPanel(pIBox->BoxPos.left, pIBox->BoxPos.right, pIBox->BoxPos.top, pIBox->BoxPos.bottom);
		pIBox->pIGrids[1].Add_IfcElem(pElemNew);
		//add "proceed button"
		pElemNew = Create_Button("Proceed", pIBox->BoxPos.left + 5, pIBox->BoxPos.top + 5);

		PCMD2* pCmdNew = new PCMD2;
		pCmdNew->wType = CQ2TYPE_DISABLE_ISSUE_PP;
		DWORD* pdwType = new DWORD;
		*pdwType = OBJTYPE_CTYPEGROUP;
		pCmdNew->pData = (BYTE*)pdwType;
		pCmdNew->wDataSize = sizeof(DWORD);
		pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		pIBox->pIGrids[1].Add_IfcElem(pElemNew);

		//add "flush" button
		pElemNew = Create_Button("Clear", pIBox->BoxPos.left + 5 + MBUTTON_SIZE_X, pIBox->BoxPos.top + 5);

		pCmdNew = new PCMD2;
		pCmdNew->wType = CQ2TYPE_FLUSH_PP;
		pCmdNew->pData = new BYTE[1];
		pCmdNew->wDataSize = sizeof(BYTE);
		pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		pIBox->pIGrids[1].Add_IfcElem(pElemNew);

		//set up listbox
		pElemNew = Create_IfcListBox2(pIBox->BoxPos.left, pIBox->BoxPos.right, pIBox->BoxPos.top, pIBox->BoxPos.bottom, pConsoleGroupList);
		iLeft = pIBox->BoxPos.left + 5;
		 iRight = pIBox->BoxPos.right - 10;
		 iTop = pIBox->BoxPos.top + 5 + MBUTTON_SIZE_Y;
		 iYSpan = pIBox->BoxPos.bottom -  pIBox->BoxPos.top - 10 - MBUTTON_SIZE_Y;
		 iYSpanListing = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;
		 iNumSlots = iYSpan / iYSpanListing;

		for (int lcy3 = 0; lcy3 < iNumSlots; lcy3++)
		{
			Add_ListBoxListing(pElemNew, lcy3, iLeft, iRight, iTop + iYSpanListing * lcy3, iTop + iYSpanListing * (lcy3+1));
		}
		pIBox->pIGrids[1].Add_IfcElem(pElemNew);

	
	//initialize igrid 2 ( this will be a 5xiSlots grid)
	pIBox->pIGrids[2].Init(pIBox->BoxPos.left + 15, pIBox->BoxPos.top + 10, 5, iSlots, (pIBox->BoxPos.right - pIBox->BoxPos.left)/5, pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight);
		//TEST
		//pIBox->pIGrids[2].Set_Scrollbar(TRUE, 0, 100, 0);
		pElemNew = Create_IfcPanel(pIBox->BoxPos.left, pIBox->BoxPos.right, pIBox->BoxPos.top, pIBox->BoxPos.bottom);
		pIBox->pIGrids[2].Add_IfcElem(pElemNew);



double fXCommandSpan = CONSOLE_COMMAND_RIGHT - CONSOLE_COMMAND_LEFT;
double fYCommandSpan = CONSOLE_COMMAND_BOTTOM - CONSOLE_COMMAND_TOP;
//sert the command menu
pIBox = &IBoxes[IBOX_CONSOLE_COMMAND];
	//for now, init 1 grid (for now it will be a 3x3 grid)
	//now, init 2 grids ( the second to show the build queue
	pIBox->pIGrids = new cIGrid[3];
	pIBox->iNumGrids = 3;
		//TEST
		//pIBox->pIGrids[0].Set_Scrollbar(TRUE, 0, 100, 0);
	iSlots = (pIBox->BoxPos.bottom - pIBox->BoxPos.top) / (pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight);
	//initialize the igrids inside(this will be a 3x3 grid)
	pIBox->pIGrids[0].Init(pIBox->BoxPos.left + 15, pIBox->BoxPos.top + 10, 4, 4, pCA->pDD_Display->i_XResD * (fXCommandSpan / 3.5), pCA->pDD_Display->i_YResD * (fYCommandSpan / 3.5));
		pElemNew = Create_IfcPanel(pIBox->BoxPos.left, pIBox->BoxPos.right, pIBox->BoxPos.top, pIBox->BoxPos.bottom);
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);
		pElemNew = Create_IfcListBox2(pIBox->BoxPos.left, pIBox->BoxPos.right - iScrollBarWidth, pIBox->BoxPos.top, pIBox->BoxPos.bottom, pConsoleCommandListBox);
		
		iNumSlots = 4;
		if (pCA->pDD_Display->iResProduct < 480000)
			iNumSlots = 3;
		int iX = pIBox->BoxPos.left;
		int iY = pIBox->BoxPos.top;
		int iXSpan = pIBox->BoxPos.right - pIBox->BoxPos.left - 10 - iScrollBarWidth;
		iYSpan = pIBox->BoxPos.bottom - pIBox->BoxPos.top - 10;
		iYSpanListing = iYSpan / iNumSlots;
		

		for (int lcy4 = 0; lcy4 < iNumSlots; lcy4++)
		{
			Add_ListBoxListing(pElemNew, lcy4, iX + 5, iX + iXSpan + 5, iY + iYSpanListing * lcy4 + 5, iY + iYSpanListing * (lcy4+1) + 5);
		}

		pIBox->pIGrids[0].Add_IfcElem(pElemNew);

	pIBox->pIGrids[1].Init(pIBox->BoxPos.left + 15, pIBox->BoxPos.top + 10, 1, iSlots, pIBox->BoxPos.right - pIBox->BoxPos.left, pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight);
		pElemNew = Create_IfcPanel(pIBox->BoxPos.left, pIBox->BoxPos.right, pIBox->BoxPos.top, pIBox->BoxPos.bottom);
		pIBox->pIGrids[1].Add_IfcElem(pElemNew);

	pIBox->pIGrids[2].Init(pIBox->BoxPos.left + 15, pIBox->BoxPos.top + 10, 1, iSlots, pIBox->BoxPos.right - pIBox->BoxPos.left, pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight);
		pElemNew = Create_IfcPanel(pIBox->BoxPos.left, pIBox->BoxPos.right, pIBox->BoxPos.top, pIBox->BoxPos.bottom);
		pIBox->pIGrids[2].Add_IfcElem(pElemNew);

pIBox = &IBoxes[IBOX_CONSOLE_INFO];
	//init 4 grids (08.20.2003)
	pIBox->pIGrids = new cIGrid[11]; 
	pIBox->iNumGrids = 11;
	
	Init_InfoGrids();


	IBoxes[IBOX_CONSOLE_INFO].iActiveGrid = 0;

pIBox = &IBoxes[IBOX_CONSOLE_CHAT];
	pIBox->pIGrids = new cIGrid[1]; 
	pIBox->iNumGrids = 1;
	pIBox->iActiveGrid = 0;
	pIBox->pIGrids[0].Init(pIBox->BoxPos.left, pIBox->BoxPos.top, 1, 1,1,1);
		pElemNew = Create_IfcPanel(pIBox->BoxPos.left, pIBox->BoxPos.right, pIBox->BoxPos.top, pIBox->BoxPos.bottom);
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);
		cIfcElement* pElemNew2 = Create_TextInputBox(pIBox->BoxPos.left + 5, pIBox->BoxPos.top + 5, pIBox->BoxPos.right - pIBox->BoxPos.left - 5 * 2, FONT_STANDARD1, FALSE, caChatText, 1000); 
		pElemNew->Add_IfcElem(pElemNew2);

pIBox = &IBoxes[IBOX_NOTIFY];
	pIBox->pIGrids = new cIGrid[1]; 
	pIBox->iNumGrids = 1;
	pIBox->iActiveGrid = 0;
	pIBox->pIGrids[0].Init(pIBox->BoxPos.left, pIBox->BoxPos.top, 1, 1,1,1);
		pElemNew = Create_IfcPanel(pIBox->BoxPos.left, pIBox->BoxPos.right, pIBox->BoxPos.top, pIBox->BoxPos.bottom);
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);
		pElemNew = Create_StaticTextElem("Game has Ended.", FONT_STANDARD1, pCA->pColorTable->Black, pIBox->BoxPos.left + 10, pIBox->BoxPos.top + 10);
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);
		pElemNew = Create_Button("OK", pIBox->BoxPos.left + 10, pIBox->BoxPos.bottom - 35);
			pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_END_GAME;
				pCmdNew->pData = NULL;
				pCmdNew->wDataSize = 0;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		pIBox->pIGrids[0].Add_IfcElem(pElemNew);
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Info_UnitTypes()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	pConsoleInfoListBox->Clear_IfcElems();

	int iXSpan = (IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - IBoxes[IBOX_CONSOLE_INFO].BoxPos.left) - 10;
	int iYSpan = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;
	int iretsize = 0;
	//
			
	for (int lc1 = 0; lc1 < UTYPE_NUMTYPES; lc1++)
	if (pCA->pUTypeTable->UnitTypes[lc1].CmpAttached.iNumComponents > 0)
	{
		//
		BOOL bDisplay = TRUE;

		//
		if (pConsole_Struct_Select != NULL)
		{		
			LPSBE pSelect = (LPSBE)pConsole_Struct_Select;
			cStructureA* pStruct = pCA->pStructIDTable->pTable[pSelect->iID];
			if (pStruct != NULL)
			{
				cStructInfo* pInfo = pCA->pStructInfoTable->pTable[pStruct->iType];
				if (pInfo->Permissions.Check_Permissions(&pCA->pUTypeTable->UnitTypes[lc1].Permissions_Required_Build) == FALSE)
					bDisplay = FALSE;
			}		
		}

		if (bDisplay == TRUE)
		{

			cIfcElement* pElemNew = new cIfcElement;
				pElemNew = Create_ObjectListing(0, 0, pCA->pGameOptions->iControl, pCA->pUTypeTable->UnitTypes[lc1].Name);
					PCMD2* pCmdNew = NULL;
/*
					pElemNew->Init(0, iXSpan, 0, iYSpan);

					PCMD2* pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY,
																				  pCA->pColorTable->MM_Players_Console[pCA->pGameOptions->iControl],
																				  pElemNew->Rect_Pos.left,
																				  pElemNew->Rect_Pos.right,
																				  pElemNew->Rect_Pos.top,
																				  pElemNew->Rect_Pos.bottom,
																				  &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_DRAWTEXT2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 0,
																		 iYSpan * 0.2,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 pCA->pUTypeTable->UnitTypes[lc1].Name,
																		 &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_DRAWTEXT2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 1,
																		 iYSpan * 0.2,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->Black,
																		 pCA->pUTypeTable->UnitTypes[lc1].Name,
																		 &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
*/
					int iNumNonZero = 0;
					for (int lc93 = 0; lc93 < NUM_MATERIAL_TYPES; lc93++)
						if ( pCA->pUTypeTable->UnitTypes[lc1].MaterialCost.Materials[lc93] > 0)
							iNumNonZero++;

					//create panel
								pCmdNew = new PCMD2;
								pCmdNew->wType = CQ2TYPE_DRAWIFCPANEL;
								pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcPanel(RTC_SECONDARY, -100, 0, iYSpan * 1.1,
																					iYSpan * (2.1 + iNumNonZero)+10, &iretsize);
								pCmdNew->wDataSize = iretsize;
								pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

								int lc96 = 1;
								char NText[50];
								itoa(pCA->pUTypeTable->UnitTypes[lc1].ddCost, NText, 10);
								pCmdNew = new PCMD2;
								pCmdNew->wType = CQ2TYPE_DRAWTEXT;
								pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY,
																					-95,
																					iYSpan * (0.2 + lc96),
																					FONT_STANDARD1,
																					pCA->pColorTable->White - 1,
																					strlen(NText),
																					NText,
																					&iretsize);
								pCmdNew->wDataSize = iretsize;
								pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
								lc96++;
					for (int lc95 = 0; lc95 < NUM_MATERIAL_TYPES; lc95++)
					{
						if ( pCA->pUTypeTable->UnitTypes[lc1].MaterialCost.Materials[lc95] > 0)
							{
								char Buffer[50];
								ZeroMemory(Buffer, sizeof(char)*50);
								strcat(Buffer, pCA->pMaterialInfoTable->Types[lc95].Name);
								strcat(Buffer, ": ");
								char NText[10];
								itoa(pCA->pUTypeTable->UnitTypes[lc1].MaterialCost.Materials[lc95], NText, 10);
								strcat(Buffer, NText);
								pCmdNew = new PCMD2;
								pCmdNew->wType = CQ2TYPE_DRAWTEXT;
								pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY,
																					-95,
																					iYSpan * (0.2 + lc96),
																					FONT_STANDARD1,
																					pCA->pColorTable->White - 1,
																					strlen(Buffer),
																					Buffer,
																					&iretsize);
								pCmdNew->wDataSize = iretsize;
								pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
								lc96++;
							}
					}

/*
					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_DRAWTEXT2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 0,
																		 iYSpan * 0.2,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 pCA->pUTypeTable->UnitTypes[lc1].Name,
																		 &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
			
					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_DRAWTEXT2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 1,
																		 iYSpan * 0.2,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->Black,
																		 pCA->pUTypeTable->UnitTypes[lc1].Name,
																		 &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
*/
					DWORD dwStore = lc1;
					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_SEND_INPUTPACKET;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Send_InputPacket_ObjRef(IPACKETTYPE_OBJREF,
																				OBJTYPE_UTYPE,
																				1,
																				&dwStore,
																				NULL,
																			    &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);

			pConsoleInfoListBox->Add_IfcElem(pElemNew);
		}
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Info_MaterialTypes()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	pConsoleInfoListBox->Clear_IfcElems();

	int iXSpan = (IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - IBoxes[IBOX_CONSOLE_INFO].BoxPos.left) - 10;
	int iYSpan = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;
	int iretsize = 0;
	//
			
	for (int lc1 = 0; lc1 < NUM_MATERIAL_TYPES; lc1++)
	if (pCA->pMaterialInfoTable->Types[lc1].RefChar != ' ')
	{
		cIfcElement* pElemNew = Create_ObjectListing(0, 0, pCA->pGameOptions->iControl, pCA->pMaterialInfoTable->Types[lc1].Name);
		PCMD2* pCmdNew = NULL;

					//create panel
								pCmdNew = new PCMD2;
								pCmdNew->wType = CQ2TYPE_DRAWIFCPANEL;
								pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcPanel(RTC_SECONDARY, -100, 0, iYSpan * 0,
																					iYSpan * 1.5, &iretsize);
								pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
					//text
								char Buffer[50];
								ZeroMemory(Buffer, sizeof(char)*50);
								strcat(Buffer, "Density: ");
								char NText[50];
								itoa(pCA->pMaterialInfoTable->Types[lc1].ddDensity, NText, 10);
								strcat(Buffer, NText);
								pCmdNew = new PCMD2;
								pCmdNew->wType = CQ2TYPE_DRAWTEXT;
								pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY,
																					-95,
																					iYSpan * 0.2,
																					FONT_STANDARD1,
																					pCA->pColorTable->White - 1,
																					strlen(Buffer),
																					Buffer,
																					&iretsize);
								pCmdNew->wDataSize = iretsize;
								pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);								
/*
					int iNumNonZero = 0;
					for (int lc93 = 0; lc93 < NUM_MATERIAL_TYPES; lc93++)
						if ( pCA->pStructInfoTable->pTable[lc1]->MaterialCost.Materials[lc93] > 0)
							iNumNonZero++;

					//create panel
								pCmdNew = new PCMD2;
								pCmdNew->wType = CQ2TYPE_DRAWIFCPANEL;
								pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcPanel(RTC_SECONDARY, -100, 0, iYSpan * 1.1,
																					iYSpan * (2.1 + iNumNonZero)+10, &iretsize);
								pCmdNew->wDataSize = iretsize;
								pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

								int lc96 = 1;
								char NText[50];
								itoa(pCA->pStructInfoTable->pTable[lc1]->ddResourceCost, NText, 10);
								pCmdNew = new PCMD2;
								pCmdNew->wType = CQ2TYPE_DRAWTEXT;
								pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY,
																					-95,
																					iYSpan * (0.2 + lc96),
																					FONT_STANDARD1,
																					pCA->pColorTable->White - 1,
																					strlen(NText),
																					NText,
																					&iretsize);
								pCmdNew->wDataSize = iretsize;
								pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
								lc96++;

					for (int lc95 = 0; lc95 < NUM_MATERIAL_TYPES; lc95++)
					{
						if ( pCA->pStructInfoTable->pTable[lc1]->MaterialCost.Materials[lc95] > 0)
							{
								char Buffer[50];
								ZeroMemory(Buffer, sizeof(char)*50);
								strcat(Buffer, pCA->pMaterialInfoTable->Types[lc95].Name);
								strcat(Buffer, ": ");
								char NText[10];
								itoa(pCA->pStructInfoTable->pTable[lc1]->MaterialCost.Materials[lc95], NText, 10);
								strcat(Buffer, NText);
								pCmdNew = new PCMD2;
								pCmdNew->wType = CQ2TYPE_DRAWTEXT;
								pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY,
																					-95,
																					iYSpan * (0.2 + lc96),
																					FONT_STANDARD1,
																					pCA->pColorTable->White - 1,
																					strlen(Buffer),
																					Buffer,
																					&iretsize);
								pCmdNew->wDataSize = iretsize;
								pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
								lc96++;
							}
					}

*/
					DWORD dwStore = lc1;
					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_SEND_INPUTPACKET;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Send_InputPacket_ObjRef(IPACKETTYPE_OBJREF,
																				OBJTYPE_MATTYPE,
																				1,
																				&dwStore,
																				NULL,
																			    &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);

			pConsoleInfoListBox->Add_IfcElem(pElemNew);
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

void cInterface::Fill_ListBox_Info_StructTypes()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	pConsoleInfoListBox->Clear_IfcElems();

	int iXSpan = (IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - IBoxes[IBOX_CONSOLE_INFO].BoxPos.left) - 10;
	int iYSpan = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;
	int iretsize = 0;
	//
			
	for (int lc1 = 0; lc1 <= pCA->pStructInfoTable->iHighestTypeID; lc1++)
	if (pCA->pStructInfoTable->pTable[lc1] != NULL)
	{


		cIfcElement* pElemNew = Create_ObjectListing(0, 0, pCA->pGameOptions->iControl, pCA->pStructInfoTable->pTable[lc1]->GeneralName);
		PCMD2* pCmdNew = NULL;

					int iNumNonZero = 0;
					for (int lc93 = 0; lc93 < NUM_MATERIAL_TYPES; lc93++)
						if ( pCA->pStructInfoTable->pTable[lc1]->MaterialCost.Materials[lc93] > 0)
							iNumNonZero++;

					//create panel
								pCmdNew = new PCMD2;
								pCmdNew->wType = CQ2TYPE_DRAWIFCPANEL;
								pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcPanel(RTC_SECONDARY, -100, 0, iYSpan * 1.1,
																					iYSpan * (2.1 + iNumNonZero)+10, &iretsize);
								pCmdNew->wDataSize = iretsize;
								pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);


								int lc96 = 1;
								char NText[50];
								itoa(pCA->pStructInfoTable->pTable[lc1]->ddResourceCost, NText, 10);
								pCmdNew = new PCMD2;
								pCmdNew->wType = CQ2TYPE_DRAWTEXT;
								pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY,
																					-95,
																					iYSpan * (0.2 + lc96),
																					FONT_STANDARD1,
																					pCA->pColorTable->White - 1,
																					strlen(NText),
																					NText,
																					&iretsize);
								pCmdNew->wDataSize = iretsize;
								pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
								lc96++;

					for (int lc95 = 0; lc95 < NUM_MATERIAL_TYPES; lc95++)
					{
						if ( pCA->pStructInfoTable->pTable[lc1]->MaterialCost.Materials[lc95] > 0)
							{
								char Buffer[50];
								ZeroMemory(Buffer, sizeof(char)*50);
								strcat(Buffer, pCA->pMaterialInfoTable->Types[lc95].Name);
								strcat(Buffer, ": ");
								char NText[10];
								itoa(pCA->pStructInfoTable->pTable[lc1]->MaterialCost.Materials[lc95], NText, 10);
								strcat(Buffer, NText);
								pCmdNew = new PCMD2;
								pCmdNew->wType = CQ2TYPE_DRAWTEXT;
								pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY,
																					-95,
																					iYSpan * (0.2 + lc96),
																					FONT_STANDARD1,
																					pCA->pColorTable->White - 1,
																					strlen(Buffer),
																					Buffer,
																					&iretsize);
								pCmdNew->wDataSize = iretsize;
								pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
								lc96++;
							}
					}


					DWORD dwStore = lc1;
					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_SEND_INPUTPACKET;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Send_InputPacket_ObjRef(IPACKETTYPE_OBJREF,
																				OBJTYPE_STYPE,
																				1,
																				&dwStore,
																				NULL,
																			    &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);

			pConsoleInfoListBox->Add_IfcElem(pElemNew);
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Info_Structures()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	pConsoleInfoListBox->Clear_IfcElems();

	int iXSpan = (IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - IBoxes[IBOX_CONSOLE_INFO].BoxPos.left) - 10;
	int iYSpan = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;

	//
	cStructureA* pSL_Trav = (cStructureA*)pCA->pStructList->pSL_First->pNext;
	int iretsize = 0;

	while (pSL_Trav != pCA->pStructList->pSL_Last)
	{
		if (pSL_Trav->iControl == pCA->pGameOptions->iControl)
		{
			cIfcElement* pElemNew = new cIfcElement;
				pElemNew = new cIfcElement;
					pElemNew->Init(0, iXSpan, 0, iYSpan);

					PCMD2* pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY,
																				  pCA->pColorTable->MM_Players_Console[pCA->pGameOptions->iControl],
																				  pElemNew->Rect_Pos.left,
																				  pElemNew->Rect_Pos.right,
																				  pElemNew->Rect_Pos.top,
																				  pElemNew->Rect_Pos.bottom,
																				  &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					float ratio = pSL_Trav->iHP / float(pSL_Trav->iHPMax);
					int iLeft = pElemNew->Rect_Pos.left;
					int iRight = pElemNew->Rect_Pos.right - 10;
		
					int iTop = pElemNew->Rect_Pos.top;
					int iBottom = pElemNew->Rect_Pos.bottom; 
					int iThick = iBottom - iTop;

					iTop += iThick * 0.3; 
					iBottom -= iThick * 0.3;

					int iGap = iRight - iLeft;

					iRight = iLeft + iGap * ratio;

					int iG = pCA->pColorTable->Det_HPGreenCmp(ratio);
					int iR = pCA->pColorTable->Det_HPRedCmp(ratio);

					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY,
																				  pCA->pColorTable->DetColor(iR/255.0, iG/255.0, 0),
																				  iLeft,
																				  iRight,
																				  iTop,
																				  iBottom,
																				  &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY,
																				  pCA->pColorTable->DetColor(iR/255.0, iG/255.0, 0),
																				  iLeft,
																				  iRight,
																				  iTop,
																				  iBottom,
																				  &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 0,
																		 iYSpan * 0.2,
																		 0,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 pSL_Trav->Name);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 0,
																		 iYSpan * 0.2,
																		 0,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 pSL_Trav->Name);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

					DWORD dwStore = pSL_Trav->iID;
					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_SEND_INPUTPACKET;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Send_InputPacket_ObjRef(IPACKETTYPE_OBJREF,
																				OBJTYPE_STRUCTURE,
																				1,
																				&dwStore,
																				NULL,
																			    &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_SELECT_STRUCT;
					DWORD* pdwNew = new DWORD; 
					*pdwNew = pSL_Trav->iID;
					pCmdNew->pData = (BYTE*)pdwNew;
					pCmdNew->wDataSize = sizeof(DWORD);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);

					int iX = pSL_Trav->Pos_primary.x - (pCA->pGDisplay->MDRAW_NUMCOLS / 2);
					int iY = pSL_Trav->Pos_primary.y - (pCA->pGDisplay->MDRAW_NUMROWS / 2);
					if (iX < 0)
						iX = 0;
					if (iY < 0)
						iY = 0;
					if (iX >= (pCA->pGameWorld->iWidth - pCA->pGDisplay->MDRAW_NUMCOLS))
						iX = pCA->pGameWorld->iWidth - pCA->pGDisplay->MDRAW_NUMCOLS - 1;
					if (iY >= (pCA->pGameWorld->iHeight - pCA->pGDisplay->MDRAW_NUMROWS))
						iY = pCA->pGameWorld->iHeight - pCA->pGDisplay->MDRAW_NUMROWS - 1;

					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_SET_MAPVIEW;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Two_Dword(iX, iY, &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);

			pConsoleInfoListBox->Add_IfcElem(pElemNew);
		}

		pSL_Trav = (cStructureA*)pSL_Trav->pNext;
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Info_Components()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	pConsoleInfoListBox->Clear_IfcElems();

	int iXSpan = (IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - IBoxes[IBOX_CONSOLE_INFO].BoxPos.left) - 10;
	int iYSpan = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;
	//
	cComponentListB* pNationalInventory = &pCA->pNationManager->ppNations[pCA->pGameOptions->iControl]->NationalInventory;

for (int lc1 = 0; lc1 < NUM_COMPONENT_CLASSES; lc1++)
{
	cComponentA* pCmpTrav = (cComponentA*)pNationalInventory->pLByClass_First[lc1]->pNext_classlist;
	int iretsize = 0;

	while (pCmpTrav != pNationalInventory->pLByClass_Last[lc1])
	{
			int iType = pCmpTrav->iType;
			char* pName = pCA->pCmpInfoTable->pCmpTable[iType]->Name;

			cIfcElement* pElemNew = new cIfcElement;
				pElemNew = new cIfcElement;
					pElemNew->Init(0, iXSpan, 0, iYSpan);

					PCMD2* pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY,
																				  pCA->pColorTable->MM_Players_Console[pCA->pGameOptions->iControl],
																				  pElemNew->Rect_Pos.left,
																				  pElemNew->Rect_Pos.right,
																				  pElemNew->Rect_Pos.top,
																				  pElemNew->Rect_Pos.bottom,
																				  &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 0,
																		 iYSpan * 0.2,
																		 0,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 pName);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 0,
																		 iYSpan * 0.2,
																		 0,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 pName);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
			
					DWORD dwStore = pCmpTrav->iID;
					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_SEND_INPUTPACKET;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Send_InputPacket_ObjRef(IPACKETTYPE_OBJREF,
																				OBJTYPE_COMPONENT,
																				1,
																				&dwStore,
																				NULL,
																			    &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);

			pConsoleInfoListBox->Add_IfcElem(pElemNew);

		pCmpTrav = (cComponentA*)pCmpTrav->pNext_classlist;
	}
}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Info_CmpTypes()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	pConsoleInfoListBox->Clear_IfcElems();

	int iXSpan = (IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - IBoxes[IBOX_CONSOLE_INFO].BoxPos.left) - 10;
	int iYSpan = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;

	//
	cCmpInfo* pCmpInfo = (cCmpInfo*)pCA->pCmpInfoTable->pCTL_First->pNext_cmplist;

	int iretsize = 0;

	while (pCmpInfo != pCA->pCmpInfoTable->pCTL_Last)
	{
			cIfcElement* pElemNew = new cIfcElement;
				pElemNew = new cIfcElement;
					pElemNew->Init(0, iXSpan, 0, iYSpan);

					PCMD2* pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY,
																				  pCA->pColorTable->MM_Players_Console[pCA->pGameOptions->iControl],
																				  pElemNew->Rect_Pos.left,
																				  pElemNew->Rect_Pos.right,
																				  pElemNew->Rect_Pos.top,
																				  pElemNew->Rect_Pos.bottom,
																				  &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 0,
																		 iYSpan * 0.2,
																		 0,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 pCmpInfo->Name);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 0,
																		 iYSpan * 0.2,
																		 0,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 pCmpInfo->Name);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
	
					DWORD dwStore = pCmpInfo->iType;
					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_SEND_INPUTPACKET;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Send_InputPacket_ObjRef(IPACKETTYPE_OBJREF,
																				OBJTYPE_CTYPE,
																				1,
																				&dwStore,
																				NULL,
																			    &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);


			pConsoleInfoListBox->Add_IfcElem(pElemNew);

		pCmpInfo = (cCmpInfo*)pCmpInfo->pNext_cmplist;
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Info_Sectors()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	pConsoleInfoListBox->Clear_IfcElems();

	int iXSpan = (IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - IBoxes[IBOX_CONSOLE_INFO].BoxPos.left) - 10;
	int iYSpan = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;
	int iretsize = 0;
	//
			
	for (int lc1 = 0; lc1 < MAX_SECTORS; lc1++)
	if (pCA->pSectorTable->Sectors[lc1] != NULL)
	if (pCA->pSectorTable->Sectors[lc1]->iControl == pCA->pGameOptions->iControl)
	{
			cIfcElement* pElemNew = new cIfcElement;
				pElemNew = new cIfcElement;
					pElemNew->Init(0, iXSpan, 0, iYSpan);

					PCMD2* pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY,
																				  pCA->pColorTable->MM_Players_Console[pCA->pGameOptions->iControl],
																				  pElemNew->Rect_Pos.left,
																				  pElemNew->Rect_Pos.right,
																				  pElemNew->Rect_Pos.top,
																				  pElemNew->Rect_Pos.bottom,
																				  &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 0,
																		 iYSpan * 0.2,
																		 0,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 pCA->pSectorTable->Sectors[lc1]->Name);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 0,
																		 iYSpan * 0.2,
																		 0,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 pCA->pSectorTable->Sectors[lc1]->Name);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
			



			pConsoleInfoListBox->Add_IfcElem(pElemNew);
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Info()
{

	//empty grid
	IBoxes[IBOX_CONSOLE_INFO].iActiveGrid = 7;
	pConsoleInfoListBox->Clear_IfcElems();

	//build unit
	if (pPendingCommand != NULL)
	if (pPendingCommand->pCommand2 != NULL)
	if (pPendingCommand->pCommand2->wType == CQ2TYPE_START_BUILD_UNIT_PRECONFIG)
	if (pPendingCommand->iCurrentSequenceNumber == 1)
		IBoxes[IBOX_CONSOLE_INFO].iActiveGrid = 1;

	//set tank type
	if (pPendingCommand != NULL)
	if (pPendingCommand->pCommand2 != NULL)
	if (pPendingCommand->pCommand2->wType == CQ2TYPE_TANK_SET_TYPE)
	if (pPendingCommand->iCurrentSequenceNumber == 1)
		IBoxes[IBOX_CONSOLE_INFO].iActiveGrid = 8;


	//build structure
	if (pPendingCommand != NULL)
	if (pPendingCommand->pCommand2 != NULL)
	if (pPendingCommand->pCommand2->wType == CQ2TYPE_BUILD_STRUCTURE)
	if (pPendingCommand->iCurrentSequenceNumber == 1)
		IBoxes[IBOX_CONSOLE_INFO].iActiveGrid = 3;

	//build custom unit
	if (pPendingCommand != NULL)
	if (pPendingCommand->pCommand2 != NULL)
	if (pPendingCommand->pCommand2->wType == CQ2TYPE_START_BUILD_UNIT_CUSTOM)
	if (pPendingCommand->iCurrentSequenceNumber == 1)
		IBoxes[IBOX_CONSOLE_INFO].iActiveGrid = 5;

	//build component
	if (pPendingCommand != NULL)
	if (pPendingCommand->pCommand2 != NULL)
	if (pPendingCommand->pCommand2->wType == CQ2TYPE_START_BUILD_COMPONENT)
	if (pPendingCommand->iCurrentSequenceNumber == 1)
		IBoxes[IBOX_CONSOLE_INFO].iActiveGrid = 5;

	if (IBoxes[IBOX_CONSOLE_INFO].iActiveGrid == 0)
		Fill_ListBox_Info_Units();
	if (IBoxes[IBOX_CONSOLE_INFO].iActiveGrid == 1)
		Fill_ListBox_Info_UnitTypes();
	if (IBoxes[IBOX_CONSOLE_INFO].iActiveGrid == 2)
		Fill_ListBox_Info_Structures();
	if (IBoxes[IBOX_CONSOLE_INFO].iActiveGrid == 3)
		Fill_ListBox_Info_StructTypes();
	if (IBoxes[IBOX_CONSOLE_INFO].iActiveGrid == 4)
		Fill_ListBox_Info_Components();
	if (IBoxes[IBOX_CONSOLE_INFO].iActiveGrid == 5)
		Fill_ListBox_Info_CmpTypes();
	if (IBoxes[IBOX_CONSOLE_INFO].iActiveGrid == 6)
		Fill_ListBox_Info_Sectors();
	if (IBoxes[IBOX_CONSOLE_INFO].iActiveGrid == 8)
		Fill_ListBox_Info_MaterialTypes();


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Info_Units()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	pConsoleInfoListBox->Clear_IfcElems();

	int iXSpan = (IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - IBoxes[IBOX_CONSOLE_INFO].BoxPos.left) - 10;
	int iYSpan = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;

	//
	cUnitA* pUL_Trav = (cUnitA*)pCA->pUnitList->pUL_First->pNext_UL;
	int iretsize = 0;

	while (pUL_Trav != pCA->pUnitList->pUL_Last)
	{
		if (pUL_Trav->iControl == pCA->pGameOptions->iControl)
		{
			cIfcElement* pElemNew = new cIfcElement;
				pElemNew = new cIfcElement;
					pElemNew->Init(0, iXSpan, 0, iYSpan);


					PCMD2* pCmdNew;
			
					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY,
																				  pCA->pColorTable->MM_Players_Console[pCA->pGameOptions->iControl],
																				  pElemNew->Rect_Pos.left,
																				  pElemNew->Rect_Pos.right,
																				  pElemNew->Rect_Pos.top,
																				  pElemNew->Rect_Pos.bottom,
																				  &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					float ratio = pUL_Trav->fHP / pUL_Trav->fHPMax;
					int iLeft = pElemNew->Rect_Pos.left;
					int iRight = pElemNew->Rect_Pos.right - 10;
		
					int iTop = pElemNew->Rect_Pos.top;
					int iBottom = pElemNew->Rect_Pos.bottom; 
					int iThick = iBottom - iTop;

					iTop += iThick * 0.3; 
					iBottom -= iThick * 0.3;

					int iGap = iRight - iLeft;

					iRight = iLeft + iGap * ratio;

					int iG = pCA->pColorTable->Det_HPGreenCmp(ratio);
					int iR = pCA->pColorTable->Det_HPRedCmp(ratio);

					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY,
																				  pCA->pColorTable->DetColor(iR/255.0, iG/255.0, 0),
																				  iLeft,
																				  iRight,
																				  iTop,
																				  iBottom,
																				  &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY,
																				  pCA->pColorTable->DetColor(iR/255.0, iG/255.0, 0),
																				  iLeft,
																				  iRight,
																				  iTop,
																				  iBottom,
																				  &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 0,
																		 iYSpan * 0.2,
																		 0,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 pUL_Trav->ShortName);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);			

					pCmdNew = new PCMD2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 0,
																		 iYSpan * 0.2,
																		 0,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 pUL_Trav->ShortName);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					DWORD dwStore = pUL_Trav->iID;
					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_SEND_INPUTPACKET;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Send_InputPacket_ObjRef(IPACKETTYPE_OBJREF,
																				OBJTYPE_UNIT,
																				1,
																				&dwStore,
																				NULL,
																			    &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);

					int iX = pUL_Trav->Pos_sqcurr.x - (pCA->pGDisplay->MDRAW_NUMCOLS / 2);
					int iY = pUL_Trav->Pos_sqcurr.y - (pCA->pGDisplay->MDRAW_NUMROWS / 2);
					if (iX < 0)
						iX = 0;
					if (iY < 0)
						iY = 0;
					if (iX >= (pCA->pGameWorld->iWidth - pCA->pGDisplay->MDRAW_NUMCOLS))
						iX = pCA->pGameWorld->iWidth - pCA->pGDisplay->MDRAW_NUMCOLS - 1;
					if (iY >= (pCA->pGameWorld->iHeight - pCA->pGDisplay->MDRAW_NUMROWS))
						iY = pCA->pGameWorld->iHeight - pCA->pGDisplay->MDRAW_NUMROWS - 1;

					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_SET_MAPVIEW;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Two_Dword(iX, iY, &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_SELECT_UNIT;
					DWORD* pdwNew = new DWORD; 
					*pdwNew = pUL_Trav->iID;
					pCmdNew->pData = (BYTE*)pdwNew;
					pCmdNew->wDataSize = sizeof(DWORD);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
				
			pConsoleInfoListBox->Add_IfcElem(pElemNew);
		}

		pUL_Trav = (cUnitA*)pUL_Trav->pNext_UL;
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Display()
{
if (pConsole_Unit_Select != NULL)
	Fill_ListBox_Display_Unit();
if (pConsole_Struct_Select != NULL)
	Fill_ListBox_Display_Structure();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Display_Structure()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	pConsoleDisplayUpdate->Clear_IfcElems();
	//IBoxes[IBOX_CONSOLE_DISPLAY].pIGrids[0].Clear_IfcElems();

		int iXRes = pCA->pDD_Display->i_XResD;
		int iYRes = pCA->pDD_Display->i_YResD;
	int iretsize = 0;
	int iTextHeight = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight;
	int iTextWidth = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iWidth;
	
	int iXBase = iXRes * 0.3;
	int iYBase = iYRes * 0.84;
	

		cIfcElement* pElemNew;
		PCMD2* pCmdNew;

	if (pCA->pInterface->pConsole_Struct_Select != NULL)
	{
		LPSBE pSelect = (LPSBE)pCA->pInterface->pConsole_Struct_Select;
		cStructureA* pStruct = pCA->pStructIDTable->pTable[pSelect->iID];

		if (pStruct != NULL)
		{
		
		pElemNew = Create_SecondaryButton(iXBase, iXBase + iTextWidth * (strlen(pStruct->Name)+4), iYBase, iYBase + (iTextHeight*1.4), pStruct->Name, FONT_STANDARD1, pStruct->iControl);
			pConsoleDisplayUpdate->Add_IfcElem(pElemNew);

		pElemNew = new cIfcElement;
		pElemNew->Init(iXRes * 0.5, iXRes * 0.7, iYRes * 0.5, iYRes * 0.7);
	
			if (pStruct->bOn == TRUE)
			{
				cIfcElement* pElemNew2;

				pElemNew2 = Create_Button("Turn Off", iXBase - 100, iYBase);
				int iretsize = 0;
				BYTE* pBuffer;
				pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_STRUCT_TURN_OFF;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Struct_Turn_Off_On(pCA->pTimer->iCurrTick + pCA->pGameOptions->iLatency + 10, pStruct->iID, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew2->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);	
				pConsoleDisplayUpdate->Add_IfcElem(pElemNew2);
			}
			if (pStruct->bOn == FALSE)
			{
				cIfcElement* pElemNew2;
				pElemNew2 = Create_Button("Turn On", iXBase - 100, iYBase);
				int iretsize = 0;
				BYTE* pBuffer;
				pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_STRUCT_TURN_ON;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Struct_Turn_Off_On(pCA->pTimer->iCurrTick + pCA->pGameOptions->iLatency + 10, pStruct->iID, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew2->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);	
				pConsoleDisplayUpdate->Add_IfcElem(pElemNew2);
			}
		if (pStruct->iType == TEMP_STYPE_TANK)
		{

			pElemNew = Create_StaticTextElem("Capacity:", 
											FONT_STANDARD1, 
											pCA->pColorTable->White - 1, 
											iXBase,
											iYBase + iTextHeight * 1.6);
			pConsoleDisplayUpdate->Add_IfcElem(pElemNew);
			char NText[10];
			itoa(pStruct->Storage.ddMaterialCapacity, NText, 10);
			pElemNew = Create_StaticTextElem(NText, 
											FONT_STANDARD1, 
											pCA->pColorTable->White - 1, 
											iXBase + iTextWidth * 12,
											iYBase + iTextHeight * 1.6);
			pConsoleDisplayUpdate->Add_IfcElem(pElemNew);
			
			pElemNew = Create_StaticTextElem("Currently Stored:", 
											FONT_STANDARD1, 
											pCA->pColorTable->White - 1, 
											iXBase,
											iYBase + iTextHeight * 3.2);
			pConsoleDisplayUpdate->Add_IfcElem(pElemNew);

			itoa(pStruct->Storage.ddMaterialStored, NText, 10);
			pElemNew = Create_StaticTextElem(NText, 
											FONT_STANDARD1, 
											pCA->pColorTable->White - 1, 
											iXBase + iTextWidth * 20,
											iYBase + iTextHeight * 3.2);
			pConsoleDisplayUpdate->Add_IfcElem(pElemNew);

			pElemNew = Create_StaticTextElem("Type:", 
											FONT_STANDARD1, 
											pCA->pColorTable->White - 1, 
											iXBase,
											iYBase + iTextHeight * 4.8);
			pConsoleDisplayUpdate->Add_IfcElem(pElemNew);
			if (pStruct->Storage.iMaterialType >= 0)
				pElemNew = Create_StaticTextElem(pCA->pMaterialInfoTable->Types[pStruct->Storage.iMaterialType].Name, 
											FONT_STANDARD1, 
											pCA->pColorTable->White - 1, 
											iXBase + iTextWidth * 7,
											iYBase + iTextHeight * 4.8);
			pConsoleDisplayUpdate->Add_IfcElem(pElemNew);

		}

		}
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Display_Unit()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	pConsoleDisplayUpdate->Clear_IfcElems();
	//IBoxes[IBOX_CONSOLE_DISPLAY].pIGrids[0].Clear_IfcElems();

		int iXRes = pCA->pDD_Display->i_XResD;
		int iYRes = pCA->pDD_Display->i_YResD;
	int iretsize = 0;
	int iTextHeight = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight;
	int iTextWidth = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iWidth;
	
	int iXBase = iXRes * 0.3;
	int iYBase = iYRes * 0.84;
	

		cIfcElement* pElemNew;
		PCMD2* pCmdNew;

	if (pCA->pInterface->pConsole_Unit_Select != NULL)
	{
		LPSBE pSelect = (LPSBE)pCA->pInterface->pConsole_Unit_Select;
		cUnitA* pUnit = pCA->pUnitTable->pUnitTable[pSelect->iID];

		if (pUnit != NULL)
		{
		
		pElemNew = Create_SecondaryButton(iXBase, iXBase + iTextWidth * (strlen(pUnit->Name)+4), iYBase, iYBase + (iTextHeight*1.4), pUnit->Name, FONT_STANDARD1, pUnit->iControl);
			pConsoleDisplayUpdate->Add_IfcElem(pElemNew);

			pElemNew = Create_StaticTextElem("Loaded:", FONT_STANDARD1, pCA->pColorTable->White - 1,iXBase + iTextWidth * 15, iYBase);
			pConsoleDisplayUpdate->Add_IfcElem(pElemNew);
		//list units 
		for (int lc4 = 0; lc4 < 4; lc4++)
		{
			int iID = pUnit->Storage.Get_UnitID_byOrder(lc4);
			if (iID >= 0)
			{	
					cUnitA* pUnitStored = pCA->pUnitTable->pUnitTable[iID];
					pElemNew = Create_ObjectListing(iXBase + iTextWidth * 15, iYBase + iTextHeight * (1 + lc4), pUnitStored->iControl, pUnitStored->ShortName);
						//add the unload command
						int iretsize = 0;
						BYTE* pBuffer = pCA->pCommQueue->Assemble_CommData_Unload_B(pCA->pTimer->iCurrTick + pCA->pGameOptions->iLatency + 15, pUnitStored->iID, pUnit->iID, pUnit->Pos_sqcurr.index, &iretsize);
						pCmdNew = new PCMD2;
						pCmdNew->wType = CQ2TYPE_UNLOAD;
						pCmdNew->pData = pBuffer;
						pCmdNew->wDataSize = iretsize;
						pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);

				pConsoleDisplayUpdate->Add_IfcElem(pElemNew);
			}
		}

		pElemNew = new cIfcElement;
		pElemNew->Init(iXRes * 0.5, iXRes * 0.7, iYRes * 0.5, iYRes * 0.7);

					
			pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE,
																		  RTC_SECONDARY,
																		  iXBase,
																		  iYBase + iTextHeight * 1.6,
																		  0,
																		  FONT_STANDARD1,
																		  pCA->pColorTable->White - 1,
																		  pUnit->ShortName);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

			
			pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE,
																		  RTC_SECONDARY,
																		  iXBase,
																		  iYBase + iTextHeight * 3,
																		  0,
																		  FONT_STANDARD1,
																		  pCA->pColorTable->White - 1,
																		  "Speed:");
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
			
			char NText[10];
			gcvt(pUnit->fMaxSpeed, 3, NText);
			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_DRAWTEXT;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText( RTC_SECONDARY,
																		  iXBase + iTextWidth * 7,
																		  iYBase + iTextHeight * 3,
																		  FONT_STANDARD1,
																		  pCA->pColorTable->White - 1,
																		  strlen(NText),
																		  NText,
																		  &iretsize);
			pCmdNew->wDataSize = iretsize;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

			gcvt(pUnit->Storage.ddMaterialStored, 3, NText);
			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_DRAWTEXT;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText( RTC_SECONDARY,
																		  iXBase + iTextWidth * 7,
																		  iYBase + iTextHeight * 4,
																		  FONT_STANDARD1,
																		  pCA->pColorTable->White - 1,
																		  strlen(NText),
																		  NText,
																		  &iretsize);
			pCmdNew->wDataSize = iretsize;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);


			itoa(pUnit->Storage.iMaterialType, NText, 10);
			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_DRAWTEXT;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText( RTC_SECONDARY,
																		  iXBase + iTextWidth * 7,
																		  iYBase + iTextHeight * 5,
																		  FONT_STANDARD1,
																		  pCA->pColorTable->White - 1,
																		  strlen(NText),
																		  NText,
																		  &iretsize);
			pCmdNew->wDataSize = iretsize;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);


			//if armed
			if (pUnit->OverloadInfo.bActive == TRUE)
			{
				pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_DRAWTEXT;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText( RTC_SECONDARY,
																			iXBase + iTextWidth * 7,
																			iYBase + iTextHeight * 6,
																			FONT_STANDARD1,
																			pCA->pColorTable->White - 1,
																			10,
																			"~bARMED.~b",
																			&iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
			}
			pConsoleDisplayUpdate->Add_IfcElem(pElemNew);
		}
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_List()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	IBoxes[IBOX_CONSOLE_LIST].iActiveGrid = 0;

	pConsoleGroupList->Clear_IfcElems();

	if (pUSB_First->pNext != pUSB_Last)
		Fill_ListBox_List_Group();
	if (pSSB_First->pNext != pSSB_Last)
	{
		LPSBE pSB = (LPSBE)pSSB_First->pNext;
		cStructureA* pStruct = pCA->pStructIDTable->pTable[pSB->iID];

		if (pPendingCommand == NULL)
		{
			Fill_ListBox_List_OrderQueue();
		} else if (pPendingCommand->pCommand2->wType == CQ2TYPE_START_BUILD_UNIT_CUSTOM)
		{
			Fill_ListBox_List_CustBuild();
		}
	}
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_List_Group()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	IBoxes[IBOX_CONSOLE_LIST].iActiveGrid = 0;
	pConsoleGroupList->Clear_IfcElems();

	LPSBE pSB_Trav = (LPSBE)pUSB_First->pNext;

	int iXSpan = IBoxes[IBOX_CONSOLE_LIST].BoxPos.right - IBoxes[IBOX_CONSOLE_LIST].BoxPos.left;
	int iYSpan = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;

	cIfcElement* pElemNew;
	PCMD2* pCmdNew;
	int iretsize;



	while (pSB_Trav != pUSB_Last)
	{
		int iID = pSB_Trav->iID;

		cUnitA* pUnit = pCA->pUnitTable->pUnitTable[iID];
			
		if (pUnit != NULL)
		if (pUnit->fHP > 0)
		{
		pElemNew = new cIfcElement;
		pElemNew->Init(0, iXSpan, 0, iYSpan);

			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_COLORFILL;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY,
												pCA->pColorTable->MM_Players_Console[pUnit->iControl],
												0,
												iXSpan,
												0,
												iYSpan,
												&iretsize);
			pCmdNew->wDataSize = iretsize;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					float ratio = pUnit->fHP / pUnit->fHPMax;
					int iLeft = pElemNew->Rect_Pos.left;
					int iRight = pElemNew->Rect_Pos.right - 10;
		
					int iTop = pElemNew->Rect_Pos.top;
					int iBottom = pElemNew->Rect_Pos.bottom; 
					int iThick = iBottom - iTop;

					iTop += iThick * 0.3; 
					iBottom -= iThick * 0.3;

					int iGap = iRight - iLeft;

					iRight = iLeft + iGap * ratio;

					int iG = pCA->pColorTable->Det_HPGreenCmp(ratio);
					int iR = pCA->pColorTable->Det_HPRedCmp(ratio);

					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY,
																				  pCA->pColorTable->DetColor(iR/255.0, iG/255.0, 0),
																				  iLeft,
																				  iRight,
																				  iTop,
																				  iBottom,
																				  &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY,
																				  pCA->pColorTable->DetColor(iR/255.0, iG/255.0, 0),
																				  iLeft,
																				  iRight,
																				  iTop,
																				  iBottom,
																				  &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);



			pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE,
																	      RTC_SECONDARY,
																		  1,
																		  1,
																		  0,
																		  FONT_STANDARD1,
																		  pCA->pColorTable->White - 1,
																		  pUnit->ShortName);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);



			pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE,
																	      RTC_SECONDARY,
																		  1,
																		  1,
																		  0,
																		  FONT_STANDARD1,
																		  pCA->pColorTable->White - 1,
																		  pUnit->ShortName);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
			
			if (pConsole_Unit_Select != NULL)
			if (pUnit->iID == ((LPSBE)pConsole_Unit_Select)->iID)
			{
				pCmdNew = new PCMD2;
				 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE,
																		      RTC_SECONDARY,
																			  0,
																			  0,
																			  0,
																			  FONT_STANDARD1,
																			  pCA->pColorTable->Black,
																			  pUnit->ShortName);
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

				pCmdNew = new PCMD2;
				 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE,
																		      RTC_SECONDARY,
																			  0,
																			  0,
																			  0,
																			  FONT_STANDARD1,
																			  pCA->pColorTable->Black,
																			  pUnit->ShortName);
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
			}

					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_SET_CONSOLE_UNIT_SELECT;
					DWORD* pdwID = new DWORD;
					*pdwID = pUnit->iID;
					pCmdNew->pData = (BYTE*)pdwID;
					pCmdNew->wDataSize = sizeof(DWORD);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);

		pConsoleGroupList->Add_IfcElem(pElemNew);
		}

		pSB_Trav = (LPSBE)pSB_Trav->pNext;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_List_OrderQueue()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	IBoxes[IBOX_CONSOLE_LIST].iActiveGrid = 0;
	pConsoleGroupList->Clear_IfcElems();



	int iXSpan = IBoxes[IBOX_CONSOLE_LIST].BoxPos.right - IBoxes[IBOX_CONSOLE_LIST].BoxPos.left;
	int iYSpan = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;

	cIfcElement* pElemNew;
	PCMD2* pCmdNew;
	int iretsize;

	LPSBE pSB = (LPSBE)pSSB_First->pNext;
	cStructureA* pStruct = pCA->pStructIDTable->pTable[pSB->iID];

	cProgressiveOrder* pPO_Trav = (cProgressiveOrder*)pStruct->OrderQueue.pPO_First->pNext;

	while (pPO_Trav != pStruct->OrderQueue.pPO_Last)
	{

		pElemNew = new cIfcElement;
		pElemNew->Init(0, iXSpan, 0, iYSpan);


			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_COLORFILL;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY,
												pCA->pColorTable->MM_Players_Console[pCA->pGameOptions->iControl],
												0,
												iXSpan,
												0,
												iYSpan,
												&iretsize);
			pCmdNew->wDataSize = iretsize;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

					float ratio = pPO_Trav->fCompletion;
					int iLeft = pElemNew->Rect_Pos.left;
					int iRight = pElemNew->Rect_Pos.right - 10;
		
					int iTop = pElemNew->Rect_Pos.top;
					int iBottom = pElemNew->Rect_Pos.bottom; 
					int iThick = iBottom - iTop;

					int iGap = iRight - iLeft;

					iRight = iLeft + iGap * ratio;

					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY,
																		          pCA->pColorTable->Green,
																				  iLeft,
																				  iRight,
																				  iTop,
																				  iBottom,
																				  &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

			pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE,
																		RTC_SECONDARY,
																		0,
																		0,
																		0,
																		FONT_STANDARD1,
																		pCA->pColorTable->White -1,
																		pPO_Trav->pName);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

			pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE,
																		RTC_SECONDARY,
																		0,
																		0,
																		0,
																		FONT_STANDARD1,
																		pCA->pColorTable->White -1,
																		pPO_Trav->pName);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

		//if this is the first element, offer a cancel button
		if (pPO_Trav->pPrev == pStruct->OrderQueue.pPO_First)
		{
			cIfcElement* pSubElemNew = new cIfcElement;
			pSubElemNew->Init(iXSpan*0.5, iXSpan, 0, 12);

			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_BLIT;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, 
				                                                     ISURF_ICONS,
																	 pSubElemNew->Rect_Pos.left,
																	 pSubElemNew->Rect_Pos.top,
																	 12,
																	 12+12,
                                                                     0,
																	 12,
																	 &iretsize);
     		pCmdNew->wDataSize = iretsize;
			pSubElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, TRUE);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);


			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_BLIT;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, 
				                                                     ISURF_ICONS,
																	 pSubElemNew->Rect_Pos.left,
																	 pSubElemNew->Rect_Pos.top,
																	 0+12,
																	 12+12,
                                                                     0,
																	 12,
																	 &iretsize);
     		pCmdNew->wDataSize = iretsize;
			pSubElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, TRUE);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);


			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_BLIT;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, 
				                                                     ISURF_ICONS,
																	 pSubElemNew->Rect_Pos.left,
																	 pSubElemNew->Rect_Pos.top,
																	 0+12+12,
																	 12+12+12,
                                                                     0,
																	 12,
																	 &iretsize);
     		pCmdNew->wDataSize = iretsize;
			pSubElemNew->Add_Cmd(IFC_ELEM_CAT_ONLDOWN_DRAW, pCmdNew, TRUE);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLDOWN_DRAW, pCmdNew, FALSE);

			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_DRAWTEXT;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY,
																		pSubElemNew->Rect_Pos.left + 15,
																		pSubElemNew->Rect_Pos.top,
																		FONT_STANDARD1,
																		pCA->pColorTable->White -1,
																		7,
																		"Cancel?",
																		&iretsize);
			pCmdNew->wDataSize = iretsize;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, TRUE);
			pSubElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_DRAWTEXT;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY,
																		pSubElemNew->Rect_Pos.left + 15,
																		pSubElemNew->Rect_Pos.top,
																		FONT_STANDARD1,
																		pCA->pColorTable->White -1,
																		7,
																		"Cancel?",
																		&iretsize);
			pCmdNew->wDataSize = iretsize;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, TRUE);
			pSubElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_DRAWTEXT;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY,
																		pSubElemNew->Rect_Pos.left + 15,
																		pSubElemNew->Rect_Pos.top,
																		FONT_STANDARD1,
																		pCA->pColorTable->White -1,
																		7,
																		"Cancel?",
																		&iretsize);
			pCmdNew->wDataSize = iretsize;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, TRUE);
			pSubElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, FALSE);

			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_CANCEL_PROGORDER_TOP;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Cancel_Progorder_Top(pCA->pTimer->iCurrTick + pCA->pGameOptions->iLatency + 10, 
				                                                pStruct->iID, &iretsize);
		

     		pCmdNew->wDataSize = iretsize;
			pSubElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, TRUE);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);

			pElemNew->Add_IfcElem(pSubElemNew);
		}

		pConsoleGroupList->Add_IfcElem(pElemNew);



		pPO_Trav = (cProgressiveOrder*)pPO_Trav->pNext;
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_List_CustBuild()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	IBoxes[IBOX_CONSOLE_LIST].iActiveGrid = 1;
	pConsoleGroupList->Clear_IfcElems();

	int iXSpan = IBoxes[IBOX_CONSOLE_LIST].BoxPos.right - IBoxes[IBOX_CONSOLE_LIST].BoxPos.left;
	int iYSpan = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;

	cIfcElement* pElemNew;
	PCMD2* pCmdNew;
	int iretsize;
	
	int iX = IBoxes[IBOX_CONSOLE_LIST].BoxPos.left;
	int iY = IBoxes[IBOX_CONSOLE_LIST].BoxPos.top;

			iScanType = OBJTYPE_CTYPE;
			if (llCutOffTime == -6996)
			{
				llCutOffTime = pCA->pTimer->iCurrTick;
				//also use "time has not been set" to do an initial "help-set" of the info menu to the ctypes tab
				PacketProcessor.Flush_Buffer2(PacketProcessor.pIPB_First, PacketProcessor.pIPB_Last);
			}
			//and cull the buffer too
			PacketProcessor.Cull_ComponentPackets_By_Class(PacketProcessor.pIPB_First, PacketProcessor.pIPB_Last, TRUE);

			cCmpInfo* pCmpInfo;
			cComponentA* pCmp;

			//display all the component types in the PacketProcessor
			INPUTPACKET* pIPB_Trav = (INPUTPACKET*)PacketProcessor.pIPB_First->pNext;

			while (pIPB_Trav != PacketProcessor.pIPB_Last)
			{
				if (pIPB_Trav->dwObjectsType == OBJTYPE_COMPONENT || pIPB_Trav->dwObjectsType == OBJTYPE_CTYPE)
				if (pIPB_Trav->dwNumObjects == 1)
				{
					if (pIPB_Trav->dwObjectsType == OBJTYPE_COMPONENT)
					{
						pCmp = pCA->pCmpIDTable->pTable[pIPB_Trav->pdwIDs[0]];
						pCmpInfo = pCA->pCmpInfoTable->pCmpTable[pCmp->iType];
					}
					if (pIPB_Trav->dwObjectsType == OBJTYPE_CTYPE)
					{
						pCmpInfo = pCA->pCmpInfoTable->pCmpTable[pIPB_Trav->pdwIDs[0]];
					}

					pElemNew = new cIfcElement;
					pElemNew->Init(0, iXSpan, 0, iYSpan);

						pCmdNew = new PCMD2;
						 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE,
																					RTC_SECONDARY,
																					0,
																					0,
																					0,
																					FONT_STANDARD1,
																					pCA->pColorTable->White -1,
																					pCmpInfo->Name);
						pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

						pConsoleGroupList->Add_IfcElem(pElemNew);
				}

				pIPB_Trav = (INPUTPACKET*)pIPB_Trav->pNext;
			}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Command_Pending()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;


	//clear lists
	pConsoleCommandListBox->Clear_IfcElems();

	int iXSpan = (IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.right - IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.left) - 10;
	int iYSpan = (IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.bottom - IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.top - 10) / 4;

	int iretsize = 0;

	cIfcElement* pElemNew = new cIfcElement;
		pElemNew = new cIfcElement;
			pElemNew->Init(0, iXSpan, 0, iYSpan);

				PCMD2* pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_DRAWTEXT;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY,
																		 0,
																		 iYSpan * 0.2,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 20,
																		 "Command In Progress...",
																		 &iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

			
	pConsoleCommandListBox->Add_IfcElem(pElemNew);


	pElemNew = new cIfcElement;
		pElemNew = new cIfcElement;
			pElemNew->Init(0, iXSpan, 0, iYSpan);

			//so it blinks
			if ( (pCA->pTimer->iCurrTick % 30) > 15)
			{
				pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_COLORFILL;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, 
																		 pCA->pColorTable->MM_Players_Console[pCA->pGameOptions->iControl], 
																		 0, 
																		 pElemNew->Rect_Pos.right - pElemNew->Rect_Pos.left, 
																		 0, 
																		 pElemNew->Rect_Pos.bottom - pElemNew->Rect_Pos.top,
																		 &iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
			}

				pCmdNew = new PCMD2;
				//figure out what to get
				int iCmdType = pPendingCommand->iRequestIndex;
				int iRequestNumber = pPendingCommand->iCurrentSequenceNumber;

				 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 0,
																		 iYSpan * 0.2,
																		 0,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White -1,
																		 pCA->pCmdReqTable->CommandTypes[iCmdType].RequestText[iRequestNumber]);
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

			
	pConsoleCommandListBox->Add_IfcElem(pElemNew);

	//blank element (at above 800x600
	if (pCA->pDD_Display->iResProduct > 480000)
	{
	pElemNew = new cIfcElement;
		pElemNew = new cIfcElement;
			pElemNew->Init(0, iXSpan, 0, iYSpan);

	pConsoleCommandListBox->Add_IfcElem(pElemNew);
	}

	pElemNew = new cIfcElement;
		pElemNew = new cIfcElement;
			pElemNew->Init(0, iXSpan, 0, iYSpan);
	
				pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_CANCEL_PENDINGCOMMAND;
				pCmdNew->pData = new BYTE[1]; //dummy byte
				pCmdNew->wDataSize = sizeof(BYTE);
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);


				pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_BLIT;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, 
																		 ISURF_CMDBUTTONS, 
																		 0, 
																		 (iYSpan - CMDBUTTON_SIZE_Y) / 2, 
																		 0, 
																		 CMDBUTTON_SIZE_X,
																		 CMDBUTTON_SIZE_Y * NUM_CQ2COMMANDS,
																		 CMDBUTTON_SIZE_Y * (NUM_CQ2COMMANDS+1),
																		 &iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

				pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_COLORFILL;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, 
																		 pCA->pColorTable->MM_Players_Console[pCA->pGameOptions->iControl], 
																		 0, 
																		 pElemNew->Rect_Pos.right - pElemNew->Rect_Pos.left, 
																		 0, 
																		 pElemNew->Rect_Pos.bottom - pElemNew->Rect_Pos.top,
																		 &iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

				pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_BLIT;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, 
																		 ISURF_CMDBUTTONS, 
																		 0, 
																		 (iYSpan - CMDBUTTON_SIZE_Y) / 2, 
																		 CMDBUTTON_SIZE_X, 
																		 CMDBUTTON_SIZE_X * 2,
																		 CMDBUTTON_SIZE_Y * NUM_CQ2COMMANDS,
																		 CMDBUTTON_SIZE_Y * (NUM_CQ2COMMANDS+1),
																		 &iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

				pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_BLIT;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, 
																		 ISURF_CMDBUTTONS, 
																		 0, 
																		 (iYSpan - CMDBUTTON_SIZE_Y) / 2, 
																		 CMDBUTTON_SIZE_X * 2, 
																		 CMDBUTTON_SIZE_X * 3,
																		 CMDBUTTON_SIZE_Y * NUM_CQ2COMMANDS,
																		 CMDBUTTON_SIZE_Y * (NUM_CQ2COMMANDS+1),
																		 &iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, FALSE);

				pCmdNew = new PCMD2;
				 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 iXSpan * 0.2,
																		 iYSpan * 0.2,
																		 0,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 pCA->pCmdInfoTable->Names[NUM_CQ2COMMANDS]);
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

				pCmdNew = new PCMD2;
				 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 iXSpan * 0.2,
																		 iYSpan * 0.2,
																		 0,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 pCA->pCmdInfoTable->Names[NUM_CQ2COMMANDS]);
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

				pCmdNew = new PCMD2;
				 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																		 iXSpan * 0.2,
																		 iYSpan * 0.2,
																		 0,
																		 FONT_STANDARD1,
																		 pCA->pColorTable->White - 1,
																		 pCA->pCmdInfoTable->Names[NUM_CQ2COMMANDS]);
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, FALSE);


		pConsoleCommandListBox->Add_IfcElem(pElemNew);
	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Command_NoPending()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//clear lists
	pConsoleCommandListBox->Clear_IfcElems();

	//command listbox
	BOOL bDispCommFlag = TRUE;
	int iNumUnits = 0;
	int iNumStructs = 0;

	int piSupportedCommands[NUM_CQ2COMMANDS]; ZeroMemory(piSupportedCommands, sizeof(int)*NUM_CQ2COMMANDS);


	LPSBE pUSB_Trav = (LPSBE)pUSB_First->pNext;
	while (pUSB_Trav != pUSB_Last)
	{
		cUnitA* pUnit = pCA->pUnitTable->pUnitTable[pUSB_Trav->iID];

		if (pUnit->iControl != pCA->pGameOptions->iControl)
			bDispCommFlag = FALSE;

		for (int lc1 = 0; lc1 < pUnit->iiNumSupportedCommands; lc1++)
		{
			piSupportedCommands[ pUnit->piSupportedCommands[lc1]]++;
		}

		iNumUnits++;

		pUSB_Trav = (LPSBE)pUSB_Trav->pNext;
	}

	if (iNumUnits == 0)
	{
	pUSB_Trav = (LPSBE)pSSB_First->pNext;
	while (pUSB_Trav != pSSB_Last)
	{
	
		cStructureA* pStruct = pCA->pStructIDTable->pTable[pUSB_Trav->iID];

		if (pStruct->iControl != pCA->pGameOptions->iControl)
			bDispCommFlag = FALSE;
		if (pStruct->bOn == FALSE)
			bDispCommFlag = FALSE;
	
		for (int lc2 = 0; lc2 < pStruct->iNumSupportedCommands; lc2++)
		{
			piSupportedCommands[ pStruct->pSupportedCommands[lc2]]++;
		}

		iNumStructs++;

		pUSB_Trav = (LPSBE)pUSB_Trav->pNext;
	}
	}
	int iQuant = 0;
	if (iNumUnits > 0)
		iQuant = iNumUnits;
	if (iNumStructs > 0)
		iQuant = iNumStructs;

	int iXSpan = (IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.right - IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.left) - 10;
	int iYSpan = (IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.bottom - IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.top - 10) / 4;

	//if there is more than one unit, do not enable "fill tank" or "empty tank"
	if (iNumUnits > 1)
	{
		piSupportedCommands[CQ2TYPE_LOAD_MATERIAL] = 0;
		piSupportedCommands[CQ2TYPE_UNLOAD_MATERIAL] = 0;
	}

	//save information to temporary holding buffer for "fake hotkey control"
	for (int lc22 = 0; lc22 < NUM_CQ2COMMANDS; lc22++)
	{
		iiSupportedCommandsBuffer[lc22] = piSupportedCommands[lc22];
	}

//console_info igrid set active determinor
	int igrid_set_active = -1;
		cIfcElement* pElemNew;
	int iretsize = 0;
	//we now have the array of commands to support, so now add the buttons
	if (iQuant > 0)
	for (int lc2 = 0; lc2 < NUM_CQ2COMMANDS; lc2++)
	if (piSupportedCommands[lc2] == iQuant)
	{

		pElemNew = new cIfcElement;
			pElemNew->Init(0, iXSpan, 0, iYSpan);

				igrid_set_active = -1;
				if (lc2 == CQ2TYPE_START_BUILD_UNIT_PRECONFIG)
					igrid_set_active = 1;
				if (lc2 == CQ2TYPE_START_BUILD_UNIT_CUSTOM)
					igrid_set_active = 5;
				if (lc2 == CQ2TYPE_START_BUILD_COMPONENT)
					igrid_set_active = 5;
				if (lc2 == CQ2TYPE_BUILD_STRUCTURE)
					igrid_set_active = 3;

				if (igrid_set_active >= 0)
				{
					PCMD2* pCmdNew2 = new PCMD2;
					pCmdNew2->wType = CQ2TYPE_SET_ACTIVEIGRID;
					pCmdNew2->pData = pCA->pCommQueue->Assemble_CommData_Set_ActiveIGrid(IBOX_CONSOLE_INFO, igrid_set_active, &iretsize);
					pCmdNew2->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew2, FALSE);
				}
				

				PCMD2* pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_START_PENDINGCOMMAND;
				DWORD* pdwStartCommandType = new DWORD;
				*pdwStartCommandType = lc2;
				pCmdNew->pData = (BYTE*)pdwStartCommandType;
				pCmdNew->wDataSize = sizeof(DWORD);
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);

					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_BLIT;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, 
																			 ISURF_CMDBUTTONS, 
																			 0, 
																			 (iYSpan - CMDBUTTON_SIZE_Y) / 2, 
																			 0, 
																			 CMDBUTTON_SIZE_X,
																		 CMDBUTTON_SIZE_Y * lc2,
																			 CMDBUTTON_SIZE_Y * (lc2+1),
																			 &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
	
					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_COLORFILL;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, 
																			 pCA->pColorTable->MM_Players_Console[pCA->pGameOptions->iControl], 
																			 0, 
																			 pElemNew->Rect_Pos.right - pElemNew->Rect_Pos.left, 
																				 0, 
																			 pElemNew->Rect_Pos.bottom - pElemNew->Rect_Pos.top,
																			 &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);				
	
					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_BLIT;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, 
																		 ISURF_CMDBUTTONS, 
																				 0, 
																			 (iYSpan - CMDBUTTON_SIZE_Y) / 2, 
																			 CMDBUTTON_SIZE_X, 
																			 CMDBUTTON_SIZE_X * 2,
																			 CMDBUTTON_SIZE_Y * lc2,
																			 CMDBUTTON_SIZE_Y * (lc2+1),
																			 &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
	
					pCmdNew = new PCMD2;
					pCmdNew->wType = CQ2TYPE_BLIT;
					pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, 
																			 ISURF_CMDBUTTONS, 
																			 0, 
																			 (iYSpan - CMDBUTTON_SIZE_Y) / 2, 
																			 CMDBUTTON_SIZE_X * 2, 
																				 CMDBUTTON_SIZE_X * 3,
																			 CMDBUTTON_SIZE_Y * lc2,
																				 CMDBUTTON_SIZE_Y * (lc2+1),
																			 &iretsize);
					pCmdNew->wDataSize = iretsize;
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, FALSE);
	
					pCmdNew = new PCMD2;	
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																			 iXSpan * 0.2,
																			 iYSpan * 0.2,
																			 0,
																			 FONT_STANDARD1,
																			 pCA->pColorTable->White - 1,
																				 pCA->pCmdInfoTable->Names[lc2]);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		
					pCmdNew = new PCMD2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																			 iXSpan * 0.2,
																			 iYSpan * 0.2,
																			 0,
																			 FONT_STANDARD1,
																			 pCA->pColorTable->White - 1,
																			 pCA->pCmdInfoTable->Names[lc2]);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);		

					pCmdNew = new PCMD2;
					 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																				 iXSpan * 0.2,
																				 iYSpan * 0.2,
																				 0,
																				 FONT_STANDARD1,
																				 pCA->pColorTable->White - 1,
																			 pCA->pCmdInfoTable->Names[lc2]);
					pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, FALSE);



		pConsoleCommandListBox->Add_IfcElem(pElemNew);
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Command()
{
	if (pPendingCommand == NULL)
		Fill_ListBox_Command_NoPending();

	if (pPendingCommand != NULL)
		Fill_ListBox_Command_Pending();
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Equip_Unit()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	pConsoleEquipListBox->Clear_IfcElems();

	int iXSpan = IBoxes[IBOX_CONSOLE_EQUIP].BoxPos.right - IBoxes[IBOX_CONSOLE_EQUIP].BoxPos.left;
	int iYSpan = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;

	cIfcElement* pElemNew;
	PCMD2* pCmdNew;
	int iretsize;


	LPSBE pSB = (LPSBE)pConsole_Unit_Select;

	cUnitA* pUnit = pCA->pUnitTable->pUnitTable[pSB->iID];

	cComponentA* pCmpTrav = (cComponentA*) pUnit->CmpAttached.pCmpList_First->pNext_list;

	while (pCmpTrav != pUnit->CmpAttached.pCmpList_Last)
	{
		cCmpInfo* pCmpInfo = pCA->pCmpInfoTable->pCmpTable[pCmpTrav->iType];
		pElemNew = new cIfcElement;
		pElemNew->Init(0, iXSpan, 0, iYSpan);
		
			pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY,
																				 0,
																				 0,
																				 0,
																				 FONT_STANDARD1,
																				 pCA->pColorTable->White - 1,
																				 pCmpInfo->Name);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

			pConsoleEquipListBox->Add_IfcElem(pElemNew);
		pCmpTrav = (cComponentA*)pCmpTrav->pNext_list;
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_ListBox_Equip()
{
	pConsoleEquipListBox->Clear_IfcElems();

	if (pConsole_Unit_Select != NULL)
	{
		Fill_ListBox_Equip_Unit();
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Fill_Listboxes()
{

	Fill_ListBox_Command();
	Fill_ListBox_Info();
	Fill_ListBox_Display();
	Fill_ListBox_List();
	Fill_ListBox_Equip();
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Process_Console()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;


cIBox* pIBox = &IBoxes[IBOX_CONSOLE_COMMAND];
cIGrid* pIGrid;
char NText[50];
int lc50 = 0; //local counter;



//DEBUG
Write_Log("[Process_Console] Processing Pending Command.");
WriteLog_Enter();
	//for now, process the IPB EVERY CYCLE (05.31.2003)
	Process_PendingCommand();


/////////////////////////////////////////////////////////////////////////////////////////////////

	Fill_Listboxes();

/////////////////////////////////////////////////////////////////////////////////////////////////

	//for now, process the IPB EVERY CYCLE (05.31.2003)
	Process_PendingCommand();
/*
//process commands when inside main menu
//if mouse is inside main menu box
	if (pCA->pInputState->IGState_Mouse.iXPos <= IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.right &&
		pCA->pInputState->IGState_Mouse.iYPos <= IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.bottom &&
		pCA->pInputState->IGState_Mouse.iYPos >= IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.top &&
		pCA->pInputState->IGState_Mouse.iXPos >= IBoxes[IBOX_CONSOLE_COMMAND].BoxPos.left)
//if main menu box is active
	if (IBoxes[IBOX_CONSOLE_COMMAND].bActiveBox == TRUE)
	{
		pIGrid = &IBoxes[IBOX_CONSOLE_COMMAND].pIGrids[IBoxes[IBOX_CONSOLE_COMMAND].iActiveGrid];

		//process interface elements
		Process_Interface_Elements(pIGrid->pIfcElem_First, pIGrid->pIfcElem_Last);

	}
//process commands when inside main menu
//if mouse is inside main menu box
	if (pCA->pInputState->IGState_Mouse.iXPos <= IBoxes[IBOX_CONSOLE_LIST].BoxPos.right &&
		pCA->pInputState->IGState_Mouse.iYPos <= IBoxes[IBOX_CONSOLE_LIST].BoxPos.bottom &&
		pCA->pInputState->IGState_Mouse.iYPos >= IBoxes[IBOX_CONSOLE_LIST].BoxPos.top &&
		pCA->pInputState->IGState_Mouse.iXPos >= IBoxes[IBOX_CONSOLE_LIST].BoxPos.left)
//if main menu box is active
	if (IBoxes[IBOX_CONSOLE_LIST].bActiveBox == TRUE)
	{
		pIGrid = &IBoxes[IBOX_CONSOLE_LIST].pIGrids[IBoxes[IBOX_CONSOLE_LIST].iActiveGrid];

		//process interface elements
		Process_Interface_Elements(pIGrid->pIfcElem_First, pIGrid->pIfcElem_Last);

	}
//process commands when inside main menu
//if mouse is inside main menu box
	if (pCA->pInputState->IGState_Mouse.iXPos <= IBoxes[IBOX_CONSOLE_INFO].BoxPos.right &&
		pCA->pInputState->IGState_Mouse.iYPos <= IBoxes[IBOX_CONSOLE_INFO].BoxPos.bottom &&
		pCA->pInputState->IGState_Mouse.iYPos >= IBoxes[IBOX_CONSOLE_INFO].BoxPos.top &&
		pCA->pInputState->IGState_Mouse.iXPos >= IBoxes[IBOX_CONSOLE_INFO].BoxPos.left)
//if main menu box is active
	if (IBoxes[IBOX_CONSOLE_INFO].bActiveBox == TRUE)
	{
		pIGrid = &IBoxes[IBOX_CONSOLE_INFO].pIGrids[IBoxes[IBOX_CONSOLE_INFO].iActiveGrid];

		//process interface elements
		Process_Interface_Elements(pIGrid->pIfcElem_First, pIGrid->pIfcElem_Last);

	}
//process commands when inside main menu
//if mouse is inside main menu box
	if (pCA->pInputState->IGState_Mouse.iXPos <= IBoxes[IBOX_CONSOLE_EQUIP].BoxPos.right &&
		pCA->pInputState->IGState_Mouse.iYPos <= IBoxes[IBOX_CONSOLE_EQUIP].BoxPos.bottom &&
		pCA->pInputState->IGState_Mouse.iYPos >= IBoxes[IBOX_CONSOLE_EQUIP].BoxPos.top &&
		pCA->pInputState->IGState_Mouse.iXPos >= IBoxes[IBOX_CONSOLE_EQUIP].BoxPos.left)
//if main menu box is active
	if (IBoxes[IBOX_CONSOLE_EQUIP].bActiveBox == TRUE)
	{
		pIGrid = &IBoxes[IBOX_CONSOLE_EQUIP].pIGrids[IBoxes[IBOX_CONSOLE_EQUIP].iActiveGrid];

		//process interface elements
		Process_Interface_Elements(pIGrid->pIfcElem_First, pIGrid->pIfcElem_Last);

	}
	*/
//if key is pressed, edit strings
	for (int lc11 = 0; lc11 < 256; lc11++)
	if (pCA->pInputState->IGState_Keyboard[lc11] == 2)    //edit on keydown
	{
		if (pKeyEdit != NULL)
			Edit_String(lc11, pKeyEdit, dwKeyEditMaxBuffer);
		if (pKeyEdit_Wide != NULL)
			Edit_String_Wide(lc11, pKeyEdit_Wide, dwKeyEditWideMaxBuffer);
//		if enter key is pressed, text entry is done
		if (lc11 == DIK_RETURN)
		{
			IBoxes[IBOX_CONSOLE_CHAT].bActiveBox = FALSE;
			if (pKeyEdit != NULL)
			{
				if (pCA->pGameOptions->bGameType == GAMETYPE_MULTI)
					pCA->pDP_Multiplay->Send_ChatText(pKeyEdit, pCA->pNationManager->ppNations[pCA->pGameOptions->iControl]->Name , dwKeyEditMaxBuffer);
				else
					pCA->pChatBuffer->Add_CBNode(pKeyEdit, 100);
				pKeyEdit[0] = 0;
				pKeyEdit = NULL;
			}
			if (pKeyEdit_Wide != NULL)
			{
				pKeyEdit_Wide[0] = 0;
			}
		}
	}

	
}

//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Init_Tutorial()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
int DX = pCA->pDD_Display->i_XResD;
int DY = pCA->pDD_Display->i_YResD;

	char etr[3];
	etr[0] = 13;
	etr[1] = 10;
	etr[2] = 0;

	//flush anything old out
	Tutorial.Exit();
	for (int lc1 = 0; lc1 < 300; lc1++)
	{
		Tutorial.iNum_Times_Element_Has_Been_Called[lc1] = 0;
		Tutorial.Elements[lc1].Init(DX*0, DX*1, DY*0, DY*1);

		Tutorial.iActiveElement = 0; 

		Tutorial.iMostRecentActiveElement = -1;
		Tutorial.iTicksSince = 0;

		Tutorial.iTick_MostRecentCapitalStartConstruction = -1;
		Tutorial.iTick_MostRecentCapitalFinishConstruction = -1;
		Tutorial.iTick_MostRecentTABKeyUp = -1;
		Tutorial.iNumStoragesCompleted = 0;
		Tutorial.iTick_SecondStorageCompleted = -1;
		Tutorial.iTick_AllStoragesSet = -1;
		Tutorial.iTick_MostRecentConstructionVehicleSelect = -1;
		Tutorial.iTick_MostRecentUraniumVehicleSelect = -1;
		Tutorial.iTick_MostRecentForgeFinishConstruction = -1;
		Tutorial.iTick_MostRecentFactoryFinishConstruction = -1;
		Tutorial.ddAmtSteel = 0;
		Tutorial.ddAmtUranium = 0;
		Tutorial.iTick_StartProcessingSteel = -1;
		Tutorial.iTick_FirstMechInfCreated = -1;
		Tutorial.iTick_MostRecentVehicleArmed = -1;

	}
	//initially keep it off
	Tutorial.iActiveElement = -1;
	Tutorial.iActivePersistentElement = -1;



////////////////////////////////////////////////////////tutorial pages////////////////////////////////
char MsgText[1000];
cIfcElement* pElemNew = NULL;
int iretsize = 0;

//destination RECTs
RECT RS;
RS.left = DX*0.1;
RS.right = RS.left + 1440*0.2; //I sized them on a screen with 1440 pixels res
RS.top = DY * 0.1;
RS.bottom = RS.top + 900*0.2; //900 pixels y-res

int iSTextX = RS.left + 10;
int iSTextY = RS.top + 10;
int iSTextXLimit = RS.right - 40;
int iSButtonX = RS.left + 20;
int iSButtonY = RS.bottom - 30;

RECT RL;

RL.left = DX*0.1;
RL.right = RS.left + 1440*0.25; //I sized them on a screen with 1440 pixels res
RL.top = DY * 0.1;
RL.bottom = RS.top + 900*0.2; //900 pixels y-res

int iLTextX = RL.left + 10;
int iLTextY = RL.top + 10;
int iLTextXLimit = RL.right - 40;
int iLButtonX = RL.left + 20;
int iLButtonY = RL.bottom - 30;

RECT RR;
RR.left = DX*0.5;
RR.right = RR.left + 1440*0.2; //I sized them on a screen with 1440 pixels res
RR.top = DY * 0.1;
RR.bottom = RR.top + 900*0.1; //900 pixels y-res

int iRTextX = RR.left + 10;
int iRTextY = RR.top + 10;
int iRTextXLimit = RR.right - 40;
int iRButtonX = RR.left + 20;
int iRButtonY = RR.bottom - 30;

RECT RP;
RP.left = DX*0.5;
RP.right = RP.left + 1440*0.2; //I sized them on a screen with 1440 pixels res
RP.top = DY * 0.3;
RP.bottom = RP.top + 900*0.3; //900 pixels y-res

int iPTextX = RP.left + 10;
int iPTextY = RP.top + 10;
int iPTextXLimit = RP.right - 40;
int iPButtonX = RP.left + 20;
int iPButtonY = RP.bottom - 30;

	//ready box
	iLastPersistentButtonClick = 0;
	TutorialPersistentMessageBox.Init(DX*0, DX*1, DY*0, DY*1);
    TutorialPersistentMessageBox.Clear_IfcElems();
	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Push this button when you have finished linking.");
			pElemNew = Create_TutorialPage(RR.left, RR.right, RR.top, RR.bottom, iRTextX, iRTextY, iRTextXLimit, MsgText, iRButtonX, iRButtonY, "OK", FONT_STANDARD1, -2);
				//add button
				cIfcElement* pElemNew2 = Create_Button("I'm Ready", iRButtonX, iRButtonY);
					PCMD2* pCmdNew = new PCMD2;
					int* piTrav = new int;
					*piTrav = 0; //dummy
					pCmdNew->pData = (BYTE*)piTrav; 
					pCmdNew->wDataSize = sizeof(int);
					pCmdNew->wType = CQ2TYPE_PERSBOXCLICK;
					pElemNew2->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
				//add this to the tutorial page
				pElemNew->Add_IfcElem(pElemNew2);
    TutorialPersistentMessageBox.Add_IfcElem(pElemNew);

	//page 0
	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Welcome to ~bSTA~b.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "This tutorial will attempt to walk you through some of the things your forces will need to accomplish their mission.");

			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY, "OK", FONT_STANDARD1, -1);
     Tutorial.Elements[0].Add_IfcElem(pElemNew);

	//page 1
	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Let's begin. ");
		strcat(MsgText, "(The game is paused while you view this screen.)");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "The first thing you'll need is a ~bCapital~b to establish your territory. ");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Next", FONT_STANDARD1, 2);
     Tutorial.Elements[1].Add_IfcElem(pElemNew);

	//page 2
	ZeroMemory(MsgText, sizeof(char)*1000);
	strcat(MsgText, "Move your ~bConstruction Vehicle~b around to find a fair-sized open space, and build a Capital there.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "Press the ~bResume Game~b button, below, when you are ready to begin");
			pElemNew = Create_TutorialPage(RL.left, RL.right, RL.top, RL.bottom, iLTextX, iLTextY, iLTextXLimit, MsgText, iLButtonX, iLButtonY,  "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[2].Add_IfcElem(pElemNew);

	 //page 3
	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Notice that your Capital won't finish building.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "This is because you are lacking some materials your Capital needs."); 		
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Next", FONT_STANDARD1, 4);
     Tutorial.Elements[3].Add_IfcElem(pElemNew);

	 //page 4
	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Your Capital is an advanced structure and needs high-strength ~bTitanium~b to be completed.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "To address this, you have been given a ~bTanker~b unit containing the needed Titanium."); 	
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "To use it, select your Tanker, and tell it to ~bMove~b adjacent to your Capital."); 	
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[4].Add_IfcElem(pElemNew);

	 //page 5
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Well done.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "Your Capital drew the resources it needed from the adjacent Tanker and finished construction."); 	
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY, "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[5].Add_IfcElem(pElemNew);

	//page 6
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Now that your Capital has completed, a medium-sized area around your Capital is now your ~bTerritory~b.");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY, "Next", FONT_STANDARD1, 7);
     Tutorial.Elements[6].Add_IfcElem(pElemNew);
	 
	//page 7
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "To view it, push and hold the ~bTAB~b key. Your Territory will be highlighted in blue. Go ahead and do that when you return to the game.");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY, "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[7].Add_IfcElem(pElemNew);

	//page 8
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Good.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "Now, notice the words ~bIron~b and ~bCarbon~b appearing over your Capital. ");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "This indicates that your Capital is collecting the base materials Carbon and Iron from the territory you control. ");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Next", FONT_STANDARD1, 9);
     Tutorial.Elements[8].Add_IfcElem(pElemNew);


	//page 9
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "~bRESOURCE COLLECTION~b");
		strcat(MsgText, etr);
		strcat(MsgText, "The progress indicator to the left of each name indicates when the next batch of Iron or Carbon will be ready.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "The rate the indicator fills up depends on what kind of terrain is in your territory. ");
		strcat(MsgText, "More ~bforests~b will improve your Carbon production, while more ~bhills~b or ~bmountains~b will improve your Iron production. ");
			pElemNew = Create_TutorialPage(RL.left, RL.right, RL.top, RL.bottom, iLTextX, iLTextY, iLTextXLimit, MsgText, iLButtonX, iLButtonY,  "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[9].Add_IfcElem(pElemNew);

	//page 10
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "~bSTORING YOUR RESOURCES~b");
		strcat(MsgText, etr);
		strcat(MsgText, "You will need a place to store that Iron and Carbon when it is ready. You ");
		strcat(MsgText, "can't build more Tankers yet, but you can build ~bStorage~b buildings.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "Select your Construction Vehicle again, and tell it to build two ~bStorages~b.");
		strcat(MsgText, etr);
		strcat(MsgText, "(They need not be adjacent to anything. We will tell you how to connect them to your Capital in a second.)");
			pElemNew = Create_TutorialPage(RL.left, RL.right, RL.top, RL.bottom, iLTextX, iLTextY, iLTextXLimit, MsgText, iLButtonX, iLButtonY,  "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[10].Add_IfcElem(pElemNew);


	//page 11
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Notice that the Storage buildings finish building without any special materials. ");
		strcat(MsgText, "Storage buildings are simple buildings and require only ~bEnergy~b.");
		strcat(MsgText, etr);
		strcat(MsgText, "(the number below your map in the top-right hand corner.)");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "Now, Storage buildings initially say ~bNot Set~b, which means you need to tell them what ");
		strcat(MsgText, "type of material they should be configured to store.");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Next", FONT_STANDARD1, 12);
     Tutorial.Elements[11].Add_IfcElem(pElemNew);

	//page 12
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Since you need to store Carbon and Iron, set your one Storage to ~bCarbon~b and the other to ~bIron~b");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[12].Add_IfcElem(pElemNew);
 

	//page 13
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "~bLINKING YOUR STRUCTURES~b");
		strcat(MsgText, etr);
		strcat(MsgText, "Excellent! ");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "If your storage buildings were adjacent to your capital building, then your ");
		strcat(MsgText, "Capital will have unloaded its Carbon and Iron to your Storages and started on a new batch.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "(if they weren't, don't worry, keep reading)");
			pElemNew = Create_TutorialPage(RL.left, RL.right, RL.top, RL.bottom, iLTextX, iLTextY, iLTextXLimit, MsgText, iLButtonX, iLButtonY,  "Next", FONT_STANDARD1, 14);
     Tutorial.Elements[13].Add_IfcElem(pElemNew);

	//page 14
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "If they weren't adjacent, you can Link your Capital to your Storages by laying ~bLinks~b between them.");		
			pElemNew = Create_TutorialPage(RL.left, RL.right, RL.top, RL.bottom, iLTextX, iLTextY, iLTextXLimit, MsgText, iLButtonX, iLButtonY, "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[14].Add_IfcElem(pElemNew);

	 	//page 15 (obsolete)
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Select your Construction Vehicle when you are ready to proceed.");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[15].Add_IfcElem(pElemNew);

	 	//page 16
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Good job so far.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "Now let's attempt to build some units.");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Next", FONT_STANDARD1, 17);
     Tutorial.Elements[16].Add_IfcElem(pElemNew);

	 	//page 17
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Now, that Iron is too weak for us to build units with.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "We will need to convert that Iron into ~bSteel~b, which is done with a ~bForge~b.");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY, "Next", FONT_STANDARD1, 18);
     Tutorial.Elements[17].Add_IfcElem(pElemNew);

	 	//page 18
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Go ahead and build one.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "The Forge needs ~bIron~b to be built, so make sure its construction site is linked to your Capital.");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[18].Add_IfcElem(pElemNew);

	 	//page 19
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Good.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "Your Forge has started converting Iron and Carbon into ~bSteel~b at a rate of 1 Carbon ");
		strcat(MsgText, "and 1 Iron for 1 Steel."); 
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Next", FONT_STANDARD1, 20);
     Tutorial.Elements[19].Add_IfcElem(pElemNew);

	 	//page 20
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Soon, you will need a place to store that Steel, so go ahead and build another ~bStorage~b for it.");
		strcat(MsgText, etr);
		strcat(MsgText, "(Key ~b'S'~b");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "It can be anywhere as long as its linked somewhere to your existing buildings.");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[20].Add_IfcElem(pElemNew);

	 	//page 21
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Side Note:");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "There may be times when you don't want your Forge to be running. At those times you ");
		strcat(MsgText, "can ~bturn it off~b by selecting it and pushing the ~bTurn Off~b button in the status display.");
		strcat(MsgText, etr);
		strcat(MsgText, "Pushing the button again will turn your building back on, and it will resume what it was doing.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "(You don't have to do this now; just telling you that you can.)");
			pElemNew = Create_TutorialPage(RL.left, RL.right, RL.top, RL.bottom, iLTextX, iLTextY, iLTextXLimit, MsgText, iLButtonX, iLButtonY,  "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[21].Add_IfcElem(pElemNew);

	 	//page 22
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "While your Forge is busy preparing Steel, you may now want to build a ~bFactory~b to begin ");
		strcat(MsgText, " building your units.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "Go ahead and start building a Factory.");
		strcat(MsgText, etr);
		strcat(MsgText, "(and make sure it gets Linked to your other buildings.)");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[22].Add_IfcElem(pElemNew);

	 	//page 23
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Very good!");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "Now that your Factory is ready and you have some Steel, let's start building some units.");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Next", FONT_STANDARD1, 24);
     Tutorial.Elements[23].Add_IfcElem(pElemNew);

	 	//page 24
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "We'll start by building some ~bMech Infantry~b.");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[24].Add_IfcElem(pElemNew);

	 	//page 25
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Wonderful! You have just built your first unit in STA.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "We're going to gear up to engage those enemy patrols. Our Intelligence estimates that a force of 12 MechInfs should be optimal.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "If you find that you keep running out of steel, build another ~bForge~b to increase your production.");
			pElemNew = Create_TutorialPage(RL.left, RL.right, RL.top, RL.bottom, iLTextX, iLTextY, iLTextXLimit, MsgText, iLButtonX, iLButtonY, "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[25].Add_IfcElem(pElemNew);

	 	//page 26
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "It looks like the enemy has found us first! Prepare for battle!");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "(Sit tight and pump out those MechInfs and you'll be OK.)");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "Remember that you can order your units to attack an enemy simply by ~bright-clicking~b on them in the map. Selecting the ~bAttack!~b command also works too.");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY, "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[26].Add_IfcElem(pElemNew);

	 	//page 27
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Awesome work! You survived the enemy attack!");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY, "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[27].Add_IfcElem(pElemNew);

	 	//page 28
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "~bPART 2: ENHANCING YOUR UNITS~b");
		strcat(MsgText, etr);
		strcat(MsgText, "Now, it appears that the enemy still has one building in the southeast corner of the map.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "You could send out your units right now to take care of it, but we're going to show you how to take it out with just one shot...");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Next", FONT_STANDARD1, 29);
     Tutorial.Elements[28].Add_IfcElem(pElemNew);

	 	//page 29
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "The key is that funny green rock that your Construction Vehicle was sitting on when you first arrived. (If you accidentally built a structure on the rock earlier, you can use another rock that is Northeast of you)");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "That green rock indicates that the square is a source of the element ~bUranium~b. You can harvest Uranium by building an ~bExtractor~b on top of that square.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "Go ahead and do that now. Also, build another Storage to hold the Uranium that the Extractor will ... extract.");
			pElemNew = Create_TutorialPage(RL.left, RL.right, RL.top, RL.bottom, iLTextX, iLTextY, iLTextXLimit, MsgText, iLButtonX, iLButtonY, "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[29].Add_IfcElem(pElemNew);

 	//page 30
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Very good.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "~bUranium~b is useful for many things; one of these is for enhancing your units.");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Next", FONT_STANDARD1, 31);
     Tutorial.Elements[30].Add_IfcElem(pElemNew);

 	//page 31
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "We are going to ~bFill~b a unit with some of your new Uranium.");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY, "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[31].Add_IfcElem(pElemNew);

 	//page 32
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Good. Your unit is now holding 1 unit of Uranium.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "Uranium grants your unit enhanced speed. Similarly, other materials can grant different powers. For example, ~bTitanium~b increases a unit's effective armor.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "However, you can also choose to use that material as a weapon, loading it into your unit's firing chamber. This is called ~bArming~b. We are going to do that now.");
//		strcat(MsgText, "To do this, select ~bArm Special~b (Hotkey S).");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[32].Add_IfcElem(pElemNew);

 	//page 33
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Your unit is now ~bArmed~b, that is, it is ready to use your Uranium as a weapon.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "The next time your order this unit to attack a target, the first projectile to be fired from your unit will be a ~bTactical Nuke~b.");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY, "Resume Game", FONT_STANDARD1, -1);
     Tutorial.Elements[33].Add_IfcElem(pElemNew);

 	//page 34
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "To conclude this Tutorial, go ahead and order your armed unit to destroy the enemy structure.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "The Tutorial will end when the structure has been destroyed. ");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "Good Luck! We here at the Tutorial Center look forward to serving under you in the future.");
			pElemNew = Create_TutorialPage(RS.left, RS.right, RS.top, RS.bottom, iSTextX, iSTextY, iSTextXLimit, MsgText, iSButtonX, iSButtonY,  "Return to Game", FONT_STANDARD1, -1);
     Tutorial.Elements[34].Add_IfcElem(pElemNew);

 	//page 61
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "~bTO MOVE YOUR CONSTRUCTION VEHICLE:~b");		strcat(MsgText, etr);
		strcat(MsgText, "~b1.~b Select your Construction Vehicle, (left click)"); 		strcat(MsgText, etr);
		strcat(MsgText, "~b2.~b Move your mouse to the location to which the unit should move, "); 		strcat(MsgText, etr);
		strcat(MsgText, "~b3.~b Right-click"); 		strcat(MsgText, etr);
		
		strcat(MsgText, etr);

		strcat(MsgText, "~bTO BUILD YOUR CAPITAL:~b");		strcat(MsgText, etr);
		strcat(MsgText, "~b1.~b Select your Construction Vehicle, (left click)"); 		strcat(MsgText, etr);
		strcat(MsgText, "~b2.~b Select ~bBuild Struct~b ,"); 		strcat(MsgText, etr);
		strcat(MsgText, "~b3.~b Choose ~bCAPITAL~b from the choices that appear in the Side Menu, "); 		strcat(MsgText, etr);
		strcat(MsgText, "~b4.~b Hover your mouse over the location you want to place it. ");
		strcat(MsgText, "~b5.~b Left-click to confirm the location. ");
		pElemNew = Create_TutorialPage(RP.left, RP.right, RP.top, RP.bottom, iPTextX, iPTextY, iPTextXLimit, MsgText, iPButtonX, iPButtonY, "Resume Game", FONT_STANDARD1, -2);
     Tutorial.Elements[61].Add_IfcElem(pElemNew);

	 //page 62
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Note:");		strcat(MsgText, etr); 
		strcat(MsgText, etr);
		strcat(MsgText, "To scroll on the map, move your mouse to the edges of the screen.");		strcat(MsgText, etr);
		strcat(MsgText, "Or left-click the desired view location on the ~bMinimap~b in the top-right corner.");
		pElemNew = Create_TutorialPage(RP.left, RP.right, RP.top, RP.bottom, iPTextX, iPTextY, iPTextXLimit, MsgText, iPButtonX, iPButtonY, "Resume Game", FONT_STANDARD1, -2);
     Tutorial.Elements[62].Add_IfcElem(pElemNew);

 	//page 64
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "To ~bSet~b your Storage buildings:");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "~b1.~b Select a Storage building,");
		strcat(MsgText, etr);
		strcat(MsgText, "~b2.~b Choose command ~bSet Type~b (or hotkey ~bS~b),");
		strcat(MsgText, etr);
		strcat(MsgText, "~b3.~b Then select the Material Type from the choices that appear in the Side Menu.");
		strcat(MsgText, etr);
		strcat(MsgText, "(pushing the first letter of the Material's name (ex. ~bC~b for Carbon) works too)");
		pElemNew = Create_TutorialPage(RP.left, RP.right, RP.top, RP.bottom, iPTextX, iPTextY, iPTextXLimit, MsgText, iPButtonX, iPButtonY, "Resume Game", FONT_STANDARD1, -2);
     Tutorial.Elements[64].Add_IfcElem(pElemNew);

 	//page 65
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "To lay a ~bLink~b:");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "~b1.~b Select your ~bCapital~b (not your Construction Vehicle),");
		strcat(MsgText, etr);
		strcat(MsgText, "~b2.~b Select ~bLay Link~b, (or hotkey ~bL~b)");
		strcat(MsgText, etr);
		strcat(MsgText, "~b3.~b Hover your mouse over the location you want to place the link,");
		strcat(MsgText, etr);
		strcat(MsgText, "~b4.~b Left-click to confirm the location,");
		strcat(MsgText, etr);
		strcat(MsgText, "~b5.~b Repeat from step 2 until your chosen path is finished.");
		strcat(MsgText, etr);
		strcat(MsgText, "(note: the links don't connect diagonally)");
		pElemNew = Create_TutorialPage(RP.left, RP.right, RP.top, RP.bottom, iPTextX, iPTextY, iPTextXLimit, MsgText, iPButtonX, iPButtonY, "Resume Game", FONT_STANDARD1, -2);
     Tutorial.Elements[65].Add_IfcElem(pElemNew);

 	//page 66
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "To build a ~bMech Infantry~b unit:");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "~b1.~b Select your Factory,");
		strcat(MsgText, etr);
		strcat(MsgText, "~b2.~b Select ~bBuild Unit~b, (Hotkey ~bB~b)");
		strcat(MsgText, etr);
		strcat(MsgText, "~b3.~b Choose ~bMechInf~b from the Side Menu.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "(Notice that hovering your mouse over each unit type shows you the materials that unit needs.)");
		pElemNew = Create_TutorialPage(RP.left, RP.right, RP.top, RP.bottom, iPTextX, iPTextY, iPTextXLimit, MsgText, iPButtonX, iPButtonY, "Resume Game", FONT_STANDARD1, -2);
     Tutorial.Elements[66].Add_IfcElem(pElemNew);

 	//page 67
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "Note:");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "If you submit a build order while another build is in progress, your newest order will be queued up behind the in-progress one. Also, you can cancel in-progress orders using the ~bCancel~b button.");
		pElemNew = Create_TutorialPage(RP.left, RP.right, RP.top, RP.bottom, iPTextX, iPTextY, iPTextXLimit, MsgText, iPButtonX, iPButtonY, "Resume Game", FONT_STANDARD1, -2);
     Tutorial.Elements[67].Add_IfcElem(pElemNew);

 	//page 68
	 	ZeroMemory(MsgText, sizeof(char)*1000);
		strcat(MsgText, "To ~bFill~b up a unit with Uranium:");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "~b1.~b Select any one your units(except for your CV or Tanker),");
		strcat(MsgText, etr);
		strcat(MsgText, "~b2.~b Select command ~bFill Tank~b (Hotkey ~bF~b),");
		strcat(MsgText, etr);
		strcat(MsgText, "~b3.~b Left-click on the Storage containing the Uranium.");
		strcat(MsgText, etr);
		strcat(MsgText, etr);
		strcat(MsgText, "Your unit will move over to the Storage and pick it up.");
		pElemNew = Create_TutorialPage(RP.left, RP.right, RP.top, RP.bottom, iPTextX, iPTextY, iPTextXLimit, MsgText, iPButtonX, iPButtonY, "Resume Game", FONT_STANDARD1, -2);
     Tutorial.Elements[68].Add_IfcElem(pElemNew);


}
/////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Signal_Chime(char* Text)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	pCA->pDA_Audio->Play_Primary(SOUNDSEG_CHIME);
	pCA->pChatBuffer->Add_CBNode(Text, 120);

}
/////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

void cInterface::Process_Tutorial()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	char etr[3];
	etr[0] = 13;
	etr[1] = 10;
	etr[2] = 0;


	//initialize
	if (pCA->pTimer->iCurrTick == 0)
		Tutorial.iActivePersistentElement = -1;

	Tutorial.iActiveElement = -1;

	if (pCA->pTimer->iCurrTick == 60)  //say hello after 1 seconds
	if (Tutorial.iNum_Times_Element_Has_Been_Called[TUTORIAL_MSG_WELCOME] <= 0) //only call this once
	{
		Tutorial.iActiveElement = TUTORIAL_MSG_WELCOME;
		Tutorial.iNum_Times_Element_Has_Been_Called[TUTORIAL_MSG_WELCOME]+= 1;

		//turn off enemy AI beyond simple guard duties
		pCA->pNationManager->ppNations[1]->AINew.iAI_Override = 1;

	}

	//trigger the second page 120 ticks (2 reference seconds) after the first page has gone down
	if (Tutorial.iMostRecentActiveElement == 0)
	if (Tutorial.iTicksSince >= 20)
	if (Tutorial.iNum_Times_Element_Has_Been_Called[1] <= 0)
	{
		Tutorial.iActiveElement = 1;
		//Tutorial.iActivePersistentElement = 61;
		Tutorial.iNum_Times_Element_Has_Been_Called[1] += 1;

		//seleect the CV
		ReleaseStructs_SB_all(pSSB_First, pSSB_Last);
		ReleaseUnits_SB_all(pUSB_First, pUSB_Last); 
		if (Tutorial.pCV != NULL)
			AddUnits_SB_single( ((cUnitA*)Tutorial.pCV)->Pos_sqcurr.index, pUSB_First, pUSB_Last);
		pConsole_Struct_Select = NULL;
		pConsole_Unit_Select = (LPSBE)pUSB_First->pNext;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #1~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Find an open spot, build your Capital.");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "~bNotes:~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Select ~bConst. Veh.~b, select ~b'Build Struct'~b");
	}

	//page 3
	if (Tutorial.iMostRecentActiveElement == 1)
	if (Tutorial.iTick_MostRecentCapitalStartConstruction > ( pCA->pTimer->iCurrTick - Tutorial.iTicksSince) )
	if ( (pCA->pTimer->iCurrTick -Tutorial.iTick_MostRecentCapitalStartConstruction) > 180 )
	if (Tutorial.iNum_Times_Element_Has_Been_Called[3] <= 0)
	{
		Tutorial.iActiveElement = 3;
		//Tutorial.iActivePersistentElement = 62;
		Tutorial.iNum_Times_Element_Has_Been_Called[3]+= 1;
		//seleect the tanker
		ReleaseStructs_SB_all(pSSB_First, pSSB_Last);
		ReleaseUnits_SB_all(pUSB_First, pUSB_Last); 
		if (Tutorial.pTanker != NULL)
			AddUnits_SB_single( ((cUnitA*)Tutorial.pTanker)->Pos_sqcurr.index, pUSB_First, pUSB_Last);
		pConsole_Struct_Select = NULL;
		pConsole_Unit_Select = (LPSBE)pUSB_First->pNext;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #2~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Move your Tanker to a square adjacent to the Capital.");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "~bNotes:~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Tanker is unit with gray crane on top.");
	}

	//page 5
	if (Tutorial.iMostRecentActiveElement == 3)
	if (Tutorial.iTick_MostRecentCapitalFinishConstruction > ( pCA->pTimer->iCurrTick - Tutorial.iTicksSince) )
	if ( (pCA->pTimer->iCurrTick -Tutorial.iTick_MostRecentCapitalFinishConstruction) > 15 )
	if (Tutorial.iNum_Times_Element_Has_Been_Called[5] <= 0)
	{
		Tutorial.iActiveElement = 5;
		Tutorial.iActivePersistentElement = -1;
		Tutorial.iNum_Times_Element_Has_Been_Called[5]+= 1;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000); //clear text
	}

	//page 6
	if (Tutorial.iMostRecentActiveElement == 5)
	if (Tutorial.iTicksSince >= 15)
	if (Tutorial.iNum_Times_Element_Has_Been_Called[6] <= 0)
	{
		Tutorial.iActiveElement = 6;
		Tutorial.iNum_Times_Element_Has_Been_Called[6]+= 1;
		Tutorial.iTick_MostRecentTABKeyUp = -1;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #3~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Push and release the TAB key.");

	}
	
	//page 8
	if (Tutorial.iMostRecentActiveElement == 6)
//	if (Tutorial.iTicksSince >= 120)
	if (Tutorial.iTick_MostRecentTABKeyUp > ( pCA->pTimer->iCurrTick - Tutorial.iTicksSince) )
    if ( (pCA->pTimer->iCurrTick -Tutorial.iTick_MostRecentTABKeyUp ) > 60 )
	if (Tutorial.iNum_Times_Element_Has_Been_Called[8] <= 0)
	{
		Tutorial.iActiveElement = 8;
		Tutorial.iNum_Times_Element_Has_Been_Called[8]+= 1;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000); //clear text
	}

	//page 10
	if (Tutorial.iMostRecentActiveElement == 8)
	if (Tutorial.iTicksSince >= 15)
	if (Tutorial.iNum_Times_Element_Has_Been_Called[10] <= 0)
	{
		Tutorial.iActiveElement = 10;
		Tutorial.iNum_Times_Element_Has_Been_Called[10]+= 1;
		//seleect the CV
		ReleaseStructs_SB_all(pSSB_First, pSSB_Last);
		ReleaseUnits_SB_all(pUSB_First, pUSB_Last); 
		if (Tutorial.pCV != NULL)
			AddUnits_SB_single( ((cUnitA*)Tutorial.pCV)->Pos_sqcurr.index, pUSB_First, pUSB_Last);
		pConsole_Struct_Select = NULL;
		pConsole_Unit_Select = (LPSBE)pUSB_First->pNext;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #4~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Build two Storage Buildings.");
	}

	//page 11
	if (Tutorial.iMostRecentActiveElement == 10)
	if (Tutorial.iTick_SecondStorageCompleted  > ( pCA->pTimer->iCurrTick - Tutorial.iTicksSince) )
	if ( (pCA->pTimer->iCurrTick -Tutorial.iTick_SecondStorageCompleted ) > 30 )
	if (Tutorial.iNum_Times_Element_Has_Been_Called[11] <= 0)
	{	
		Tutorial.iActiveElement = 11;
		Tutorial.iNum_Times_Element_Has_Been_Called[11]+= 1;

	//	Tutorial.iActivePersistentElement = 64;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #5~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Set Storages to Carbon and Iron.");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "~bNotes:~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Select ~bStorage~b, select ~b'Set Type'~b");
	}

	//page 13
	if (Tutorial.iMostRecentActiveElement == 11)
	if (Tutorial.iTick_AllStoragesSet  > ( pCA->pTimer->iCurrTick - Tutorial.iTicksSince) )
	if ( (pCA->pTimer->iCurrTick - Tutorial.iTick_AllStoragesSet ) > 30 )
	if (Tutorial.iNum_Times_Element_Has_Been_Called[13] <= 0)
	{
		Tutorial.iActiveElement = 13;

		Tutorial.iNum_Times_Element_Has_Been_Called[13]+= 1;

		for (int lc6 = 0; lc6 <= pCA->pStructList->iID_Last_Assigned; lc6++)
		{
			if (pCA->pStructIDTable->pTable[lc6] != NULL)
			{
				cStructureA* pStruct = pCA->pStructIDTable->pTable[lc6];
				if (pStruct->iControl == pCA->pGameOptions->iControl)
				if (pStruct->iType == TEMP_STYPE_RSC)
				{
					ReleaseStructs_SB_all(pSSB_First, pSSB_Last);
					ReleaseUnits_SB_all(pUSB_First, pUSB_Last); 
					AddStructs_SB_single( pStruct->Pos_primary.index, pSSB_First, pSSB_Last);
					pConsole_Struct_Select = (LPSBE)pSSB_First->pNext;
					pConsole_Unit_Select = NULL;
				}
			}
		}

	//	Tutorial.iActivePersistentElement = 65;
		bTutorialPersistentMessageBoxActive = TRUE;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #6~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Link Storages to Capital. Press 'I'm Ready' button when finished.");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "~bNotes:~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Select ~bCapital~b, select ~b'Lay Link'~b");

	}
/*
	//page 15
	if (Tutorial.iMostRecentActiveElement == 13)
	if (Tutorial.iTicksSince >= 15)
	if (Tutorial.iNum_Times_Element_Has_Been_Called[15] <= 0)
	{
		Tutorial.iActiveElement = 15;
		Tutorial.iNum_Times_Element_Has_Been_Called[15]+= 1;
		//release all units from selection buffer
		ReleaseUnits_SB_all(pUSB_First, pUSB_Last);
		pConsole_Unit_Select = NULL;
		pConsole_Struct_Select = NULL;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #6~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Link Storages to Capital. Select CV when ready.");
	}
*/
	//page 16		
	if (Tutorial.iMostRecentActiveElement == 13)
//	if (Tutorial.iTick_MostRecentConstructionVehicleSelect > ( pCA->pTimer->iCurrTick - Tutorial.iTicksSince) )
//	if ( (pCA->pTimer->iCurrTick - Tutorial.iTick_MostRecentConstructionVehicleSelect ) > 15 )
	if (iLastPersistentButtonClick  > ( pCA->pTimer->iCurrTick - Tutorial.iTicksSince) )
	if ( (pCA->pTimer->iCurrTick - iLastPersistentButtonClick ) > 5 )
	if (Tutorial.iNum_Times_Element_Has_Been_Called[16] <= 0)
	{
		bTutorialPersistentMessageBoxActive = FALSE;
	//	Tutorial.iActivePersistentElement = -1;
		Tutorial.iTick_AllStoragesSet = -1;
		Tutorial.iActiveElement = 16;
		Tutorial.iNum_Times_Element_Has_Been_Called[16] += 1;


	//	Tutorial.iActivePersistentElement = -1;
		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #7~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Build Forge.");
	}

	//page 19		
	if (Tutorial.iMostRecentActiveElement == 16)
	if (Tutorial.iTick_StartProcessingSteel > ( pCA->pTimer->iCurrTick - Tutorial.iTicksSince) )
	if ( (pCA->pTimer->iCurrTick - Tutorial.iTick_StartProcessingSteel ) > 15 )
	if (Tutorial.iNum_Times_Element_Has_Been_Called[19] <= 0)
	{
		Tutorial.iTick_AllStoragesSet = -1;
		Tutorial.iActiveElement = 19;
		Tutorial.iNum_Times_Element_Has_Been_Called[19] += 1;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #8~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Build Storage and accumulate 2 steel.");
	}

	//page 21
	if (Tutorial.iMostRecentActiveElement == 19)
	if (Tutorial.ddAmtSteel >= 1.9)
	if (Tutorial.iNum_Times_Element_Has_Been_Called[21] <= 0)
	{
		for (int lc6 = 0; lc6 <= pCA->pStructList->iID_Last_Assigned; lc6++)
		{
			if (pCA->pStructIDTable->pTable[lc6] != NULL)
			{
				cStructureA* pStruct = pCA->pStructIDTable->pTable[lc6];
				if (pStruct->iControl == pCA->pGameOptions->iControl)
				if (pStruct->iType == TEMP_STYPE_FORGE)
				{
					ReleaseStructs_SB_all(pSSB_First, pSSB_Last);
					ReleaseUnits_SB_all(pUSB_First, pUSB_Last); 
					AddStructs_SB_single( pStruct->Pos_primary.index, pSSB_First, pSSB_Last);
					pConsole_Struct_Select = (LPSBE)pSSB_First->pNext;
					pConsole_Unit_Select = NULL;
				}
			}
		}
		Tutorial.iTick_AllStoragesSet = -1;
		Tutorial.iActiveElement = 21;
		Tutorial.iNum_Times_Element_Has_Been_Called[21]+= 1;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000); //clear text
	}

	//page 22
	if (Tutorial.iMostRecentActiveElement == 21)
	if (Tutorial.iTicksSince >= 15)
	if (Tutorial.iNum_Times_Element_Has_Been_Called[22] <= 0)
	{
		Tutorial.iActiveElement = 22;
		Tutorial.iNum_Times_Element_Has_Been_Called[22]+= 1;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #9~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Build Factory.");
	}

	//page 23
	if (Tutorial.iMostRecentActiveElement == 22)
	if (Tutorial.iTick_MostRecentFactoryFinishConstruction  > ( pCA->pTimer->iCurrTick - Tutorial.iTicksSince) )
	if ( (pCA->pTimer->iCurrTick - Tutorial.iTick_MostRecentFactoryFinishConstruction ) > 15 )
	if (Tutorial.iNum_Times_Element_Has_Been_Called[23] <= 0)
	{
		Tutorial.iTick_AllStoragesSet = -1;
		Tutorial.iActiveElement = 23;
		Tutorial.iNum_Times_Element_Has_Been_Called[23] += 1;

		for (int lc6 = 0; lc6 <= pCA->pStructList->iID_Last_Assigned; lc6++)
		{
			if (pCA->pStructIDTable->pTable[lc6] != NULL)
			{
				cStructureA* pStruct = pCA->pStructIDTable->pTable[lc6];
				if (pStruct->iControl == pCA->pGameOptions->iControl)
				if (pStruct->iType == TEMP_STYPE_FACTORY_ABC)
				{
					ReleaseStructs_SB_all(pSSB_First, pSSB_Last);
					ReleaseUnits_SB_all(pUSB_First, pUSB_Last); 
					AddStructs_SB_single( pStruct->Pos_primary.index, pSSB_First, pSSB_Last);
					pConsole_Struct_Select = (LPSBE)pSSB_First->pNext;
					pConsole_Unit_Select = NULL;
				}
			}
		}

	//	Tutorial.iActivePersistentElement = 66;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #10~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Build One MechInf.");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "~bNotes:~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Select ~bFactory~b, select ~b'Build Unit'~b");
	}

	//page 25
	if (Tutorial.iMostRecentActiveElement == 23)
	if (Tutorial.iTick_FirstMechInfCreated   > ( pCA->pTimer->iCurrTick - Tutorial.iTicksSince) )
	if ( (pCA->pTimer->iCurrTick - Tutorial.iTick_FirstMechInfCreated ) > 20 )
	if (Tutorial.iNum_Times_Element_Has_Been_Called[25] <= 0)
	{
		Tutorial.iTick_AllStoragesSet = -1;
		Tutorial.iActiveElement = 25;
		Tutorial.iNum_Times_Element_Has_Been_Called[25] += 1;

	//	Tutorial.iActivePersistentElement = 67;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #10~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Build a Group of MechInfs and Await Further Orders.");
	}

	//display a report from command ship
	if (Tutorial.iMostRecentActiveElement == 25)
	if (Tutorial.iTicksSince == 11200)
	{
		char Text[1000];
		ZeroMemory(Text, sizeof(char)*1000);
		char etr[3];
		etr[0] = 13;
		etr[1] = 10;
		etr[2] = 0;

	//	Tutorial.iActivePersistentElement = -1;

		strcat(Text, "REPORT FROM COMMAND SHIP: ");
		strcat(Text, etr);
		strcat(Text, "Enemy units detected. converging on your location. ");
		strcat(Text, etr);
		strcat(Text, "Streaming local image data...");
		pCA->pChatBuffer->Add_CBNode(Text, 50000);
	}

	//unveil the map
	if (Tutorial.iMostRecentActiveElement == 25)
	if (Tutorial.iTicksSince == 11390)
	{
			for (int lcy = 0; lcy < pCA->pGameWorld->iHeight; lcy++)
			for (int lcx = 0; lcx < pCA->pGameWorld->iWidth; lcx++)
			{
				pCA->pGameWorld->pMap[lcy * pCA->pGameWorld->iWidth + lcx].barrRevealed = 65535;
			}
			//re-enable the AI
			pCA->pNationManager->ppNations[1]->AINew.iAI_Override = 0;
	}

	//now show the last panels
	//page 26
	if (Tutorial.iMostRecentActiveElement == 25)
	if (Tutorial.iTicksSince >= 11550)
	if (Tutorial.iNum_Times_Element_Has_Been_Called[26] <= 0)
	{
		Tutorial.iActiveElement = 26;
		Tutorial.iNum_Times_Element_Has_Been_Called[26]+= 1;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #11~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Defend Yourself!");
	}

	//page 27
	if (Tutorial.iMostRecentActiveElement == 26)
	if (pCA->pNationManager->ppNations[1]->iNumUnits == 0)
	if (Tutorial.iNum_Times_Element_Has_Been_Called[27] <= 0)
	{
		Tutorial.iActiveElement = 27;
		Tutorial.iNum_Times_Element_Has_Been_Called[26]+= 1;
		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000); //clear text
	}

	//page 28
	if (Tutorial.iMostRecentActiveElement == 27)
	if (Tutorial.iTicksSince >= 60)
	if (Tutorial.iNum_Times_Element_Has_Been_Called[28] <= 0)
	{
		Tutorial.iActiveElement = 28;
		Tutorial.iNum_Times_Element_Has_Been_Called[28]+= 1;
		Tutorial.iTick_AllStoragesSet = -1;

		//seleect the CV
		ReleaseStructs_SB_all(pSSB_First, pSSB_Last);
		ReleaseUnits_SB_all(pUSB_First, pUSB_Last); 
		if (Tutorial.pCV != NULL)
			AddUnits_SB_single( ((cUnitA*)Tutorial.pCV)->Pos_sqcurr.index, pUSB_First, pUSB_Last);
		pConsole_Struct_Select = NULL;
		pConsole_Unit_Select = (LPSBE)pUSB_First->pNext;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #12~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Build an Extractor on the Uranium patch and accumulate 1 Uranium.");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "~bNotes:~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Uranium Patch is square with funny green rock on it.");

	}

	//page 30
	if (Tutorial.iMostRecentActiveElement == 28)
	if (Tutorial.ddAmtUranium > 0.9)
	if (Tutorial.iNum_Times_Element_Has_Been_Called[30] <= 0)
	{
		Tutorial.iActiveElement = 30;
		Tutorial.iNum_Times_Element_Has_Been_Called[30]+= 1;
		Tutorial.iTick_AllStoragesSet = -1;

	//	Tutorial.iActivePersistentElement = 68;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #13~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Fill Uranium into a fighting unit.");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "~bNotes:~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Select (any) Unit, select ~b'Fill Tank'~b");
	}

	//page 32		
	if (Tutorial.iMostRecentActiveElement == 30)
	if (Tutorial.iTick_MostRecentUraniumVehicleSelect > ( pCA->pTimer->iCurrTick - Tutorial.iTicksSince) )
	if ( (pCA->pTimer->iCurrTick - Tutorial.iTick_MostRecentUraniumVehicleSelect ) > 15 )
	if (Tutorial.iNum_Times_Element_Has_Been_Called[32] <= 0)
	{
		Tutorial.iTick_AllStoragesSet = -1;
		Tutorial.iActiveElement = 32;
		Tutorial.iNum_Times_Element_Has_Been_Called[32] += 1;

	//	Tutorial.iActivePersistentElement = -1;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #14~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Arm unit.");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "~bNotes:~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Select (filled) Unit, select ~b'Arm Special'~b");
	}

	//page 33		
	if (Tutorial.iMostRecentActiveElement == 32)
	if (Tutorial.iTick_MostRecentVehicleArmed  > ( pCA->pTimer->iCurrTick - Tutorial.iTicksSince) )
	if ( (pCA->pTimer->iCurrTick - Tutorial.iTick_MostRecentVehicleArmed ) > 15 )
	if (Tutorial.iNum_Times_Element_Has_Been_Called[33] <= 0)
	{
		Tutorial.iTick_AllStoragesSet = -1;
		Tutorial.iActiveElement = 33;
		Tutorial.iNum_Times_Element_Has_Been_Called[33] += 1;
		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000); //clear text
	}

	//last panel
	//page 34
	if (Tutorial.iMostRecentActiveElement == 33)
	if (Tutorial.iTicksSince >= 15)
	if (Tutorial.iNum_Times_Element_Has_Been_Called[34] <= 0)
	{
		Tutorial.iActiveElement = 34;
		Tutorial.iNum_Times_Element_Has_Been_Called[34]+= 1;

		ZeroMemory(TutorialPersistentMessageBuffer, sizeof(char)*1000);
		strcat(TutorialPersistentMessageBuffer, "~bTutorial Task #15~b");
		strcat(TutorialPersistentMessageBuffer, etr);
		strcat(TutorialPersistentMessageBuffer, "Terminate the enemy base.");
	}


	//housekeeping
	Tutorial.iTicksSince++;
	if (Tutorial.iActiveElement >= 0)
	{
		Tutorial.iMostRecentActiveElement = Tutorial.iActiveElement;
		Tutorial.iTicksSince = 0;
	}
	if (Tutorial.iMostRecentActiveElement == 6)
	if (Tutorial.iActiveElement < 0)
	if (pCA->pInputState->IGState_Keyboard[DIK_TAB] == 3)
	{
		Tutorial.iTick_MostRecentTABKeyUp = pCA->pTimer->iCurrTick;
		//OK!	
		pCA->pInterface->Signal_Chime("Task Completed!");

	}



	//find out which of your units is the CV or tanker
	Tutorial.pCV = NULL;
	Tutorial.pTanker = NULL;
	for (int lc1 = 0; lc1 <= pCA->pUnitList->ID_Last_Assigned; lc1++)
	{
		cUnitA* pUnit = pCA->pUnitTable->pUnitTable[lc1];
		if (pUnit != NULL)
		if (pUnit->iControl == pCA->pGameOptions->iControl)
		{
			if (pUnit->Storage.iMaterialType == MATERIAL_ELEMENT_T)
			{
				Tutorial.pTanker = pUnit;
			}

			if (pUnit->pBuilder != NULL)
			{
				Tutorial.pCV = pUnit;
			}
		}

	}

	//check to see whether a uranium-containing vehicle with a weapon has been selected
	if (Tutorial.iTick_MostRecentUraniumVehicleSelect < 0)
	if (Tutorial.iMostRecentActiveElement == 30)
	if (Tutorial.iActiveElement < 0)
	if (pConsole_Unit_Select != NULL)
	{
		LPSBE pSelect = (LPSBE)pConsole_Unit_Select;
		if (pSelect->iID >= 0)
		{
			cUnitA* pUnit = pCA->pUnitTable->pUnitTable[pSelect->iID];
			if (pUnit->pPrimaryWeapon != NULL)
			if (pUnit->Storage.iMaterialType == MATERIAL_ELEMENT_U)
			if (pUnit->Storage.ddMaterialStored > 0)
			{
				Tutorial.iTick_MostRecentUraniumVehicleSelect = pCA->pTimer->iCurrTick;
				//OK!	
				pCA->pInterface->Signal_Chime("Task Completed!");
			}
		}
	}
	//check to see if an armed vehicle has been selected
	//check to see whether a uranium-containing vehicle with a weapon has been selected
	if (Tutorial.iTick_MostRecentVehicleArmed < 0)
	if (Tutorial.iMostRecentActiveElement == 32)
	if (Tutorial.iActiveElement < 0)
	if (pConsole_Unit_Select != NULL)
	{
		LPSBE pSelect = (LPSBE)pConsole_Unit_Select;
		if (pSelect->iID >= 0)
		{
			cUnitA* pUnit = pCA->pUnitTable->pUnitTable[pSelect->iID];
			if (pUnit->pPrimaryWeapon != NULL)
			if (pUnit->OverloadInfo.bActive == TRUE)
			{
				Tutorial.iTick_MostRecentVehicleArmed  = pCA->pTimer->iCurrTick;
				//OK!	
				pCA->pInterface->Signal_Chime("Task Completed!");
			}
		}
	}
	//check to see whether the construction vehicle has been selected
	if (Tutorial.iTick_MostRecentConstructionVehicleSelect < 0)
	if (Tutorial.iMostRecentActiveElement == 15)
	if (Tutorial.iActiveElement < 0)
	if (pConsole_Unit_Select != NULL)
	{
		LPSBE pSelect = (LPSBE)pConsole_Unit_Select;
		if (pSelect->iID >= 0)
		{
			cUnitA* pUnit = pCA->pUnitTable->pUnitTable[pSelect->iID];
			if (pUnit->pBuilder != NULL)
			{
				Tutorial.iTick_MostRecentConstructionVehicleSelect = pCA->pTimer->iCurrTick;
			}
		}
	}


	//check to see whether the user has set all of the storages
	if (Tutorial.iTick_AllStoragesSet < 0)
	if (Tutorial.iMostRecentActiveElement == 11 || Tutorial.iMostRecentActiveElement == 28 || Tutorial.iMostRecentActiveElement == 19 || Tutorial.iMostRecentActiveElement == 16)
	if (Tutorial.iActiveElement < 0)
	{
		Tutorial.ddAmtSteel = 0;
		int iNumStorages = 0;
		BOOL bAllSet = TRUE;
		for (int lc1 = 0; lc1 <= pCA->pStructList->iID_Last_Assigned; lc1++)
		{
			cStructureA* pStruct = pCA->pStructIDTable->pTable[lc1];
			if (pStruct != NULL)
			if (pStruct->iControl == pCA->pGameOptions->iControl)
			{
				if (pStruct->iType == TEMP_STYPE_TANK)
				{
					iNumStorages++;
					if (pStruct->Storage.iMaterialType < 0)
					{
						bAllSet = FALSE;
						//break;
					}
					if (pStruct->Storage.iMaterialType == MATERIAL_ELEMENT_S)
					if (pStruct->Storage.ddMaterialStored >= 0)
					{
						Tutorial.ddAmtSteel += pStruct->Storage.ddMaterialStored;
					}
					if (pStruct->Storage.iMaterialType == MATERIAL_ELEMENT_U)
					if (pStruct->Storage.ddMaterialStored >= 0)
					{
						Tutorial.ddAmtUranium += pStruct->Storage.ddMaterialStored;
					}

				}
				if (pStruct->iType == TEMP_STYPE_FORGE)
				{
					if (Tutorial.iTick_StartProcessingSteel < (pCA->pTimer->iCurrTick - Tutorial.iTicksSince))
					if (pStruct->pResourceCollection[MATERIAL_ELEMENT_I] > 0.5)
					{
						Tutorial.iTick_StartProcessingSteel = pCA->pTimer->iCurrTick;
					}
				}
			}
		}
		if (bAllSet == TRUE)
		{
			if (iNumStorages >= 2 &&  Tutorial.iMostRecentActiveElement == 11 )
			{
				Tutorial.iTick_AllStoragesSet = pCA->pTimer->iCurrTick;
				//OK!	
				pCA->pInterface->Signal_Chime("Task Completed!");
			}
		//	if (iNumStorages >= 3 &&  Tutorial.iMostRecentActiveElement == 19 )
		//		Tutorial.iTick_AllStoragesSet = pCA->pTimer->iCurrTick;
		}
	}

}
/////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cCmdDataReqTable::Init()
{

	//NOTHING HERE YET! (05.20.2003)
	//begin adding command request stuff here(05.30.2003)
	//initialize everything
	for (int lc1 = 0; lc1 < NUM_CQ2COMMANDS; lc1++)
	for (int lc2 = 0; lc2 < MAX_COMMINFO_REQUESTS; lc2++)
	{
		CommandTypes[lc1].RequestText[lc2] = NULL;
		CommandTypes[lc1].Sequence[lc2] = 0;
	}

	//the new MOVE command (05.30.2003)
	CommandTypes[CQ2TYPE_MOVE_B].Sequence[0] = CompileAttributeValue(OBJTYPE_UNIT, 
																	 CDR_BITFLAG_PLURAL,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	//CommandTypes[CQ2TYPE_MOVE_B].RequestText[0] = 	

	CommandTypes[CQ2TYPE_MOVE_B].Sequence[1] = CompileAttributeValue(OBJTYPE_MAPTILE, 
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
CommandTypes[CQ2TYPE_MOVE_B].RequestText[1] = "DESTINATION?";

	CommandTypes[CQ2TYPE_MOVE_B].Sequence[2] = 0; //the "I'm done" signal

	//the new ATTACK command (07.30.2003)
	CommandTypes[CQ2TYPE_ATTACK_B].Sequence[0] = CompileAttributeValue(OBJTYPE_UNIT, 
																	 CDR_BITFLAG_PLURAL,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_ATTACK_B].Sequence[1] = CompileAttributeValue(OBJTYPE_UNIT, 
																	 OBJTYPE_STRUCTURE,
																	 OBJTYPE_MAPTILE,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_ATTACK_B].RequestText[1] = "TARGET?";	
	CommandTypes[CQ2TYPE_ATTACK_B].Sequence[2] = 0;

	//the command to order a structure to start building a pre-configured unit
	CommandTypes[CQ2TYPE_START_BUILD_UNIT_PRECONFIG].Sequence[0] = CompileAttributeValue(OBJTYPE_STRUCTURE, 
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_START_BUILD_UNIT_PRECONFIG].Sequence[1] = CompileAttributeValue(OBJTYPE_UTYPE, 
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_START_BUILD_UNIT_PRECONFIG].RequestText[1] = "CHOOSE UNIT TYPE...";
	CommandTypes[CQ2TYPE_START_BUILD_UNIT_PRECONFIG].Sequence[2] = 0;

	//the command to order a unit to build with a custom-defined configuration
	CommandTypes[CQ2TYPE_START_BUILD_UNIT_CUSTOM].Sequence[0] = CompileAttributeValue(OBJTYPE_STRUCTURE, 
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_START_BUILD_UNIT_CUSTOM].Sequence[1] = CompileAttributeValue(OBJTYPE_CTYPEGROUP, 
																	 CDR_BITFLAG_PLURAL,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_START_BUILD_UNIT_CUSTOM].RequestText[1] = "CHOOSE COMPONENTS...";
	CommandTypes[CQ2TYPE_START_BUILD_UNIT_CUSTOM].Sequence[2] = 0;



	//the command to order a structure to start building a pre-configured unit
	CommandTypes[CQ2TYPE_START_BUILD_COMPONENT].Sequence[0] = CompileAttributeValue(OBJTYPE_STRUCTURE, 
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_START_BUILD_COMPONENT].Sequence[1] = CompileAttributeValue(OBJTYPE_CTYPE, 
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_START_BUILD_COMPONENT].RequestText[1] = "CHOOSE CMP TYPE...";
	CommandTypes[CQ2TYPE_START_BUILD_COMPONENT].Sequence[2] = 0;


	//the command to order a unit to start building a structure
	CommandTypes[CQ2TYPE_BUILD_STRUCTURE].Sequence[0] = CompileAttributeValue(OBJTYPE_UNIT, 
																	 CDR_BITFLAG_PLURAL,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_BUILD_STRUCTURE].Sequence[1] = CompileAttributeValue(OBJTYPE_STYPE, 
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_BUILD_STRUCTURE].RequestText[1] = "CHOOSE STRUCT TYPE";
	CommandTypes[CQ2TYPE_BUILD_STRUCTURE].Sequence[2] = CompileAttributeValue(OBJTYPE_MAPTILE, 
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_BUILD_STRUCTURE].RequestText[2] = "CHOOSE BUILD LOCATION.";
	CommandTypes[CQ2TYPE_BUILD_STRUCTURE].Sequence[3] = 0;

	//the command to order a unit to start building a structure
	CommandTypes[CQ2TYPE_LOAD].Sequence[0] = CompileAttributeValue(OBJTYPE_UNIT,   //first one is the unit being ordered
																	 CDR_BITFLAG_PLURAL,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_LOAD].RequestText[0] = "UNITS TO LOAD?";
	CommandTypes[CQ2TYPE_LOAD].Sequence[1] = CompileAttributeValue(OBJTYPE_UNIT,  //second is unit to load into
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_LOAD].RequestText[1] = "LOAD UNIT INTO?";
	CommandTypes[CQ2TYPE_LOAD].Sequence[2] = 0;

	CommandTypes[CQ2TYPE_UNLOAD_ALL].Sequence[0] = CompileAttributeValue(OBJTYPE_UNIT,  //unit(s) loading out from
																	 CDR_BITFLAG_PLURAL,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_UNLOAD_ALL].RequestText[0] = "UNITS UNLOADING?";
	CommandTypes[CQ2TYPE_UNLOAD_ALL].Sequence[1] = CompileAttributeValue(OBJTYPE_MAPTILE,  //unload location
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_UNLOAD_ALL].RequestText[1] = "UNLOAD TO?";
	CommandTypes[CQ2TYPE_UNLOAD_ALL].Sequence[2] = 0;


	CommandTypes[CQ2TYPE_TRANSFER_MATERIAL].Sequence[0] = CompileAttributeValue(OBJTYPE_UNIT,  //unit(s) loading out from
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_TRANSFER_MATERIAL].RequestText[0] = "UNIT?";
	CommandTypes[CQ2TYPE_TRANSFER_MATERIAL].Sequence[1] = CompileAttributeValue(OBJTYPE_MAPTILE,  //unload location
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_TRANSFER_MATERIAL].RequestText[1] = "TAKE FROM?";
	CommandTypes[CQ2TYPE_TRANSFER_MATERIAL].Sequence[2] = CompileAttributeValue(OBJTYPE_MAPTILE,  //unload location
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_TRANSFER_MATERIAL].RequestText[2] = "TAKE TO?";
	CommandTypes[CQ2TYPE_TRANSFER_MATERIAL].Sequence[3] = 0;

	CommandTypes[CQ2TYPE_LOAD_MATERIAL].Sequence[0] = CompileAttributeValue(OBJTYPE_UNIT,  //unit(s) loading out from
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_LOAD_MATERIAL].RequestText[0] = "UNIT TO LOAD?";
	CommandTypes[CQ2TYPE_LOAD_MATERIAL].Sequence[1] = CompileAttributeValue(OBJTYPE_MAPTILE,  //unload location
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_LOAD_MATERIAL].RequestText[1] = "LOAD FROM?";
	CommandTypes[CQ2TYPE_LOAD_MATERIAL].Sequence[2] = 0;
	
	CommandTypes[CQ2TYPE_UNLOAD_MATERIAL].Sequence[0] = CompileAttributeValue(OBJTYPE_UNIT,  //unit(s) loading out from
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_UNLOAD_MATERIAL].RequestText[0] = "UNIT TO UNLOAD?";
	CommandTypes[CQ2TYPE_UNLOAD_MATERIAL].Sequence[1] = CompileAttributeValue(OBJTYPE_MAPTILE,  //unload location
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_UNLOAD_MATERIAL].RequestText[1] = "UNLOAD TO?";
	CommandTypes[CQ2TYPE_UNLOAD_MATERIAL].Sequence[2] = 0;

	CommandTypes[CQ2TYPE_TANK_SET_TYPE].Sequence[0] = CompileAttributeValue(OBJTYPE_STRUCTURE,  //struct changing
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_TANK_SET_TYPE].RequestText[0] = "STRUCT?";
	CommandTypes[CQ2TYPE_TANK_SET_TYPE].Sequence[1] = CompileAttributeValue(OBJTYPE_MATTYPE,  //set type
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_TANK_SET_TYPE].RequestText[1] = "TYPE?";
	CommandTypes[CQ2TYPE_TANK_SET_TYPE].Sequence[2] = 0;

	CommandTypes[CQ2TYPE_TANK_FLUSH].Sequence[0] = CompileAttributeValue(OBJTYPE_STRUCTURE,  //struct changing
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_TANK_FLUSH].RequestText[0] = "STRUCT?";
	CommandTypes[CQ2TYPE_TANK_FLUSH].Sequence[1] = 0;  //set type

	CommandTypes[CQ2TYPE_CANCEL_PROGORDER_TOP].Sequence[0] = CompileAttributeValue(OBJTYPE_STRUCTURE,  //struct changing
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_CANCEL_PROGORDER_TOP].RequestText[0] = "STRUCT?";
	CommandTypes[CQ2TYPE_CANCEL_PROGORDER_TOP].Sequence[1] = 0;  //set type

	CommandTypes[CQ2TYPE_LAY_LINK].Sequence[0] = CompileAttributeValue(OBJTYPE_MAPTILE,  //target maptile
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_LAY_LINK].RequestText[0] = "LOCATION?";
	CommandTypes[CQ2TYPE_LAY_LINK].Sequence[1] = 0;  //set type

	CommandTypes[CQ2TYPE_ARM_MATERIAL].Sequence[0] = CompileAttributeValue(OBJTYPE_UNIT,  //target maptile
																	 CDR_BITFLAG_PLURAL,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID,
																	 CDR_BITFLAG_INVALID);
	CommandTypes[CQ2TYPE_ARM_MATERIAL].RequestText[0] = "Units?";
	CommandTypes[CQ2TYPE_ARM_MATERIAL].Sequence[1] = 0;  //set type

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
ULONGLONG cCmdDataReqTable::CompileAttributeValue(int Flag1, int Flag2, int Flag3, int Flag4, int Flag5, int Flag6, int Flag7, int Flag8, int Flag9, int Flag10)
{
	ULONGLONG i64Ret = 0;

	int FlagArray[10];

	FlagArray[0] = Flag1;
	FlagArray[1] = Flag2;
	FlagArray[2] = Flag3;
	FlagArray[3] = Flag4;
	FlagArray[4] = Flag5;
	FlagArray[5] = Flag6;
	FlagArray[6] = Flag7;
	FlagArray[7] = Flag8;
	FlagArray[8] = Flag9;
	FlagArray[9] = Flag10;

	//add the request bit shift values one by one
	for (int lc1 = 0; lc1 < 10; lc1++)
	if (FlagArray[lc1] != CDR_BITFLAG_INVALID)
	{
		i64Ret |= 1ULL << FlagArray[lc1];
	}

	return i64Ret;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInputPacketBuffer::Init(VOID* pCA)
{
	//05.20.2003
	//store pCA
	pCAccess = pCA;

	cClassAccess* pCA2 = (cClassAccess*)pCA;

	//initialize log file  <--disabled, otherwise the buffers inside the input packets would continuously overwrite the main buffer's logs (05.20.2003)
	//(to set up log for the main buffer, call InitLogFile separately)
//	Init_LogFile();  
	
	//set up anchors
	pIPB_First = new INPUTPACKET;
	pIPB_Last = new INPUTPACKET;

	pIPB_First->pNext = (VOID*)pIPB_Last;
	pIPB_Last->pPrev = (VOID*)pIPB_First;
	pIPB_First->pPrev = NULL;
	pIPB_Last->pNext = NULL;

	//the anchors are invalid as nodes, so give them invalid object type
	pIPB_First->dwPacketType = IPACKETTYPE_INVALID;
	pIPB_First->pdwIDs = NULL;
	pIPB_First->pItemArray = NULL;
	pIPB_First->pObjectArray = NULL;
//	pIPB_First->iTick = -6996;
	pIPB_Last->dwPacketType = IPACKETTYPE_INVALID;
	pIPB_Last->pdwIDs = NULL;
	pIPB_Last->pItemArray = NULL;
	pIPB_Last->pObjectArray = NULL;
//	pIPB_Last->iTick = -6996;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInputPacketBuffer::Exit()
{
//DEBUG
Write_Log("[IPB] Flushing Buffer...");
WriteLog_Enter();
	//flush the buffer
	Flush_Buffer(pIPB_First, pIPB_Last);

//	Exit_LogFile();



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
INPUTPACKET* cInputPacketBuffer::Add_Packet(INPUTPACKET* pInputPacket)
{
	//here, if we assume that, because packets always added as soon as they are created,
	//their rightful place will always be at the end of the list, and thus putting them
	//in the right spot chronologically becomes very easy.
	//as of 05.20.2003 we will work under this assumption and implement the "it's always at the end" method
	//instead of the correct, more comprehensive one

	INPUTPACKET*	pIPB_OldNext = (INPUTPACKET*)pInputPacket->pNext;
	INPUTPACKET*	pIPB_OldPrev = (INPUTPACKET*)pInputPacket->pPrev;

	INPUTPACKET*	pIPB_Prev = (INPUTPACKET*)pIPB_Last->pPrev;

	//if the packet's ORIGINAL links weren't NULL, re-link them behind it
	if (pIPB_OldNext != NULL)
		pIPB_OldNext->pPrev = NULL;
	if (pIPB_OldPrev != NULL)
		pIPB_OldPrev->pNext = NULL;
	if (pIPB_OldNext != NULL && pIPB_OldPrev != NULL)
	{		
		pIPB_OldNext->pPrev = (VOID*)pIPB_OldPrev;
		pIPB_OldPrev->pNext = (VOID*)pIPB_OldNext;
	}

	//link
	pInputPacket->pNext = (void*)pIPB_Last;
	pInputPacket->pPrev = (void*)pIPB_Prev;
	pIPB_Prev->pNext = (void*)pInputPacket;
	pIPB_Last->pPrev = (void*)pInputPacket;

	//for DEBUG
	Write_Log("Input Packet Added. Time: ");
	char NText[50];
	itoa(pInputPacket->dwTick, NText, 10);
	Write_Log(NText);
	Write_Log(" Type: ");
	if (pInputPacket->dwPacketType == IPACKETTYPE_OBJREF)
		Write_Log("OBJREF");
	if (pInputPacket->dwPacketType == IPACKETTYPE_PARAM)
		Write_Log("PARAM");
	Write_Log(" Subtype: ");
	if (pInputPacket->dwObjectsType == OBJTYPE_UNIT)
		Write_Log("OBJTYPE_UNIT");
	if (pInputPacket->dwObjectsType == OBJTYPE_STRUCTURE)
		Write_Log("OBJTYPE_STRUCTURE");
	if (pInputPacket->dwObjectsType == OBJTYPE_COMPONENT)
		Write_Log("OBJTYPE_COMPONENT");
	if (pInputPacket->dwObjectsType == OBJTYPE_STYPE)
		Write_Log("OBJTYPE_STYPE");
	if (pInputPacket->dwObjectsType == OBJTYPE_UTYPE)
		Write_Log("OBJTYPE_UTYPE");
	if (pInputPacket->dwObjectsType == OBJTYPE_CTYPE)
		Write_Log("OBJTYPE_CTYPE");
	if (pInputPacket->dwObjectsType == OBJTYPE_CTYPEGROUP)
		Write_Log("OBJTYPE_CTYPEGROUP");
	WriteLog_Enter();
	

	
	return pIPB_OldNext;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInputPacketBuffer::Compile_Add_Packet(DWORD dwPacketType,
											DWORD dwObjectsType,
											DWORD dwNumObjects,
											VOID** pObjectArray,
											DWORD* pdwIDs,
											DWORD dwParameterType,
											DWORD dwNumParamItems,
											VOID* pItemArray, 
											BOOL bCopyFlag)
{
	INPUTPACKET* pIPB_New = new INPUTPACKET;
	ZeroMemory(pIPB_New, sizeof(INPUTPACKET));

	VOID** ppObjRefs = NULL;
	DWORD* pdwIDArray = NULL;
	VOID* pItems = NULL;

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//deep copy moved to inside the Compile_Add function(05.30.2003)
	if (dwPacketType == IPACKETTYPE_OBJREF && bCopyFlag == TRUE)
	{
		if (pObjectArray != NULL)   //just to protect against anyone taking shortcuts by specifying NULL for the objrefs (08.12.2003)
		{
			ppObjRefs = new VOID*[dwNumObjects];
			memcpy(ppObjRefs, pObjectArray, sizeof(VOID*) * dwNumObjects);
		}
			else
				ppObjRefs = NULL;

		pdwIDArray = new DWORD[dwNumObjects];
		memcpy(pdwIDArray, pdwIDs, sizeof(DWORD) * dwNumObjects);
	}
	if (dwPacketType == IPACKETTYPE_OBJREF && bCopyFlag == FALSE)
	{
		ppObjRefs = pObjectArray;
		pdwIDArray = pdwIDs;
	}


	if (dwPacketType == IPACKETTYPE_PARAM && bCopyFlag == TRUE)
	{
		pItems = new DWORD[dwNumParamItems]; 
		memcpy(pItems, pItemArray, sizeof(DWORD)*dwNumParamItems);

	}
	if (dwPacketType == IPACKETTYPE_PARAM && bCopyFlag == FALSE)
		pItems = pItemArray;

	//fill with information
	pIPB_New->dwTick = pCA->pTimer->iCurrTick;
	pIPB_New->dwPacketType = dwPacketType;
	pIPB_New->dwObjectsType = dwObjectsType;
	pIPB_New->dwNumObjects = dwNumObjects;
	pIPB_New->pObjectArray = ppObjRefs;
	pIPB_New->pdwIDs = pdwIDArray;
	pIPB_New->dwParameterType = dwParameterType;
	pIPB_New->dwNumParamItems = dwNumParamItems;
	pIPB_New->pItemArray = pItems;

	pIPB_New->pPrev = NULL;
	pIPB_New->pNext = NULL;

	//now just add the packet
	Add_Packet(pIPB_New);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInputPacketBuffer::Flush_Buffer(INPUTPACKET* pIPB_Start, INPUTPACKET* pIPB_End)
{

	int lc1;   //local counter
//NOTE: this function flushes all entries in the buffer between and including pIPB_Start and pIPB_End
//and attempts to re-link the entries immediately outside if it finds them(05.20.2003)

	cClassAccess* pCA = (cClassAccess*)pCAccess;



	INPUTPACKET* pIPB_Trav = pIPB_Start;
	INPUTPACKET* pIPB_Trav2;
	INPUTPACKET* pIPB_PostEnd = (INPUTPACKET*)pIPB_End->pNext;
	INPUTPACKET* pIPB_PreStart = (INPUTPACKET*)pIPB_Start->pPrev;

//DEBUG
Write_Log("[IPB]Attempting to Flush Buffer");
WriteLog_Enter();


	while (pIPB_Trav != pIPB_PostEnd)
	{
		pIPB_Trav2 = (INPUTPACKET*)pIPB_Trav->pNext;

		if (pIPB_Trav->pItemArray != NULL)
			delete(pIPB_Trav->pItemArray);
		if (pIPB_Trav->pdwIDs != NULL)
			delete(pIPB_Trav->pdwIDs);
		if (pIPB_Trav->pObjectArray != NULL)
			delete(pIPB_Trav->pObjectArray);

//DEBUG
Write_Log("[IPB]Attempting to Erase Memory of Packet");
WriteLog_Enter();

		delete(pIPB_Trav);
		pIPB_Trav = pIPB_Trav2;

	}

/*
	//if either one is valid, set their pertinent link value is NULL(on the assumption that the other isn't, which is fixed by the following if)
	if (pIPB_PreStart != NULL)
	{
		pIPB_PreStart->pNext = NULL;
	}
	if (pIPB_PostEnd != NULL)
	{
		pIPB_PostEnd->pPrev = NULL;
	}

	//link them together if both PreStart and PostEnd are valid
	if (pIPB_PreStart != NULL && pIPB_PostEnd != NULL)
	{
		pIPB_PreStart->pNext = (void*)pIPB_PostEnd;
		pIPB_PostEnd->pPrev = (void*)pIPB_PreStart;
	}
*/

//DEBUG
pCA->pInterface->Write_Log("[IPB]Packet Flushing Completed");
pCA->pInterface->WriteLog_Enter();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInputPacketBuffer::Flush_Buffer2(INPUTPACKET* pIPB_Start, INPUTPACKET* pIPB_End)
{

	int lc1;   //local counter

	cClassAccess* pCA = (cClassAccess*)pCAccess;



	INPUTPACKET* pIPB_Trav = (INPUTPACKET*)pIPB_Start->pNext;
	INPUTPACKET* pIPB_Trav2;

//DEBUG
Write_Log("[IPB]Attempting to Flush Buffer");
WriteLog_Enter();


	while (pIPB_Trav != pIPB_End)
	{
		pIPB_Trav2 = (INPUTPACKET*)pIPB_Trav->pNext;

		if (pIPB_Trav->pItemArray != NULL)
			delete(pIPB_Trav->pItemArray);
		if (pIPB_Trav->pdwIDs != NULL)
			delete(pIPB_Trav->pdwIDs);
		if (pIPB_Trav->pObjectArray != NULL)
			delete(pIPB_Trav->pObjectArray);

//DEBUG
Write_Log("[IPB]Attempting to Erase Memory of Packet");
WriteLog_Enter();

		delete(pIPB_Trav);
		pIPB_Trav = pIPB_Trav2;

	}

	//relink start and end
	pIPB_Start->pNext = (void*)pIPB_End;
	pIPB_End->pPrev = (void*)pIPB_Start;


//DEBUG
pCA->pInterface->Write_Log("[IPB]Packet Flushing Completed");
pCA->pInterface->WriteLog_Enter();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInputPacketBuffer::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cInputPacketBuffer.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInputPacketBuffer::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_IPB >= 1)
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
void cInputPacketBuffer::WriteLog_Enter()
{

if (DEBUG + DEBUG_IPB >= 1)
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
void cInputPacketBuffer::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cInputPacketBuffer::PeekAcquire_IPB_Specific(INPUTPACKET* pIPB_Start, INPUTPACKET* pIPB_End, int iObjType, LONGLONG* pllCutoffTime, BOOL bCopyFlag)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	BOOL bAcquiredFlag = FALSE;

	INPUTPACKET* pIPB_Trav = (INPUTPACKET*)pIPB_Start->pNext;

	//go through the IPB and find the first input packet with a valid tick time
	while (*pllCutoffTime > pIPB_Trav->dwTick && pIPB_Trav != pIPB_End)
	{

		pIPB_Trav = (INPUTPACKET*)pIPB_Trav->pNext;
	}

	//now cycle through the rest of the entries for the matching entry
	while (pIPB_Trav != pIPB_End)
	{
		BOOL PacketClear = FALSE;

		//for now, only allow single-entry packets to go through
		if (pIPB_Trav->dwPacketType == IPACKETTYPE_OBJREF)
		{
		if ((pIPB_Trav->dwObjectsType == iObjType) && (pIPB_Trav->dwNumObjects == 1))
		{
			PacketClear = TRUE;		
		}
		//a kludge to allow if CTYPE sepcified, allow both CTYPE and COMPONENT (08.20.2003)
		if (iObjType == OBJTYPE_CTYPE)
		if (pIPB_Trav->dwNumObjects == 1)
		if (pIPB_Trav->dwObjectsType == OBJTYPE_CTYPE || pIPB_Trav->dwObjectsType == OBJTYPE_COMPONENT)
			PacketClear = TRUE;


		}


		//acquire this packet if it passes all the test
		if (PacketClear == TRUE)
		{
			//we can take it straight out of the IPB if copy flag is not set
			if (bCopyFlag == FALSE)
			{
				Add_Packet(pIPB_Trav);
				*pllCutoffTime = pIPB_Trav->dwTick + 1;
				return TRUE;
			}
			//if it is, compile a new packet
			if (bCopyFlag == TRUE)
			{
				Compile_Add_Packet(pIPB_Trav->dwPacketType,
												pIPB_Trav->dwObjectsType,
												pIPB_Trav->dwNumObjects,
												pIPB_Trav->pObjectArray,
												pIPB_Trav->pdwIDs,
												pIPB_Trav->dwParameterType,
												pIPB_Trav->dwNumParamItems,
												pIPB_Trav->pItemArray,
												TRUE);
				*pllCutoffTime = pIPB_Trav->dwTick + 1;
				return TRUE;
			}
		}
		
		//otherwise, continue searching
		pIPB_Trav = (INPUTPACKET*)pIPB_Trav->pNext;
		
	}

	//if it reaches here and fails to return add a packet, return false
	return FALSE;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInputPacketBuffer::Cull_ComponentPackets_By_Class(INPUTPACKET* pIPB_Start, INPUTPACKET* pIPB_End, BOOL bFrameOnlyFlag)
{
	INPUTPACKET* pIPB_Trav = (INPUTPACKET*)pIPB_End->pPrev;
	INPUTPACKET* pIPB_Trav2;

	cClassAccess* pCA = (cClassAccess*)pCAccess;


	//allocate an array to store whether we have found certain component classes or not
	BYTE bFoundClass[NUM_COMPONENT_CLASSES];

	ZeroMemory(bFoundClass, sizeof(BYTE) * NUM_COMPONENT_CLASSES);

	cComponentA* pCmp;
	cCmpInfo* pCmpInfo;


	//go through packets in reverse order
	while (pIPB_Trav != pIPB_Start)
	{
		pIPB_Trav2 = (INPUTPACKET*)pIPB_Trav->pPrev;
		if (pIPB_Trav->dwPacketType == IPACKETTYPE_OBJREF)
		{
			if (pIPB_Trav->dwObjectsType == OBJTYPE_CTYPE)
			{
				pCmpInfo = pCA->pCmpInfoTable->pCmpTable[ pIPB_Trav->pdwIDs[0] ];

				if (bFoundClass[ pCmpInfo->iClass ] == 0)
				{
					bFoundClass[ pCmpInfo->iClass ] = 1;
				}	else  if (bFrameOnlyFlag == FALSE || pCmpInfo->iClass == CMPCLASS_FRAME) 
					{	
						//otherwise, get rid of this packet
						Delete_Packet(pIPB_Trav);
					}

			}
			if (pIPB_Trav->dwObjectsType == OBJTYPE_COMPONENT)
			{
				pCmp = pCA->pCmpIDTable->pTable[pIPB_Trav->pdwIDs[0]];

				if (bFoundClass[ pCmp->iClass ] == 0)
				{
					bFoundClass[ pCmp->iClass ] = 1;
				}	else if (bFrameOnlyFlag == FALSE || pCmpInfo->iClass == CMPCLASS_FRAME) 
					{	
						//otherwise, get rid of this packet
						Delete_Packet(pIPB_Trav);
					} 
			}
		}


		pIPB_Trav = pIPB_Trav2;
	}




}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInputPacketBuffer::Issue_Group_Packet(void* pIPB, int iObjectGroupType)
{
	INPUTPACKET* pIPB_Trav = (INPUTPACKET*)pIPB_First->pNext;

	cClassAccess* pCA = (cClassAccess*)pCAccess;

int iValidPackets = 0;
	while (pIPB_Trav != pIPB_Last)
	{
		if (pIPB_Trav->dwPacketType == IPACKETTYPE_OBJREF)
			iValidPackets++;

		pIPB_Trav = (INPUTPACKET*)pIPB_Trav->pNext;

	}
DWORD* pdwIDs;
//allocate an array 
//for now, don't do anything if no valid packets were found (08.20.2003)
if (iValidPackets != 0)
{
	pdwIDs = new DWORD[iValidPackets];

	


int lc1 = 0;
pIPB_Trav = (INPUTPACKET*)pIPB_First->pNext;

	while (pIPB_Trav != pIPB_Last)
	{
		if (pIPB_Trav->dwPacketType == IPACKETTYPE_OBJREF)
		{
			//special case. if we being asked to issue of type CTYPEGROUP, take both CTYPE and COMPONENT refs,
			//but convert component refs to types (08.20.2003)
			if (iObjectGroupType == OBJTYPE_CTYPEGROUP)
			{
				if (pIPB_Trav->dwObjectsType == OBJTYPE_COMPONENT)
				{
					cComponentA* pCmp = pCA->pCmpIDTable->pTable[pIPB_Trav->pdwIDs[0]];
					pdwIDs[lc1] = pCmp->iType;
					lc1++;
				}
					else
					{
						pdwIDs[lc1] = pIPB_Trav->pdwIDs[0];
						lc1++;
					}
			} else
				{

					pdwIDs[lc1] = pIPB_Trav->pdwIDs[0];
					lc1++;
				}
		}
		pIPB_Trav = (INPUTPACKET*)pIPB_Trav->pNext;
	}

	//now get pointer to destination IPB
	cInputPacketBuffer* pDestIPB = (cInputPacketBuffer*)pIPB;

	//and issue that the calling IPB
	pDestIPB->Compile_Add_Packet(IPACKETTYPE_OBJREF, iObjectGroupType, iValidPackets, NULL, pdwIDs, 0, 0, NULL, FALSE);
}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInputPacketBuffer::Delete_Packet(INPUTPACKET* pInputPacket)
{
	INPUTPACKET* pPrev = (INPUTPACKET*)pInputPacket->pPrev;
	INPUTPACKET* pNext = (INPUTPACKET*)pInputPacket->pNext;

	//link behind if
	pPrev->pNext = (void*)pNext;
	pNext->pPrev = (void*)pPrev;

	//delete the packet
		if (pInputPacket->pItemArray != NULL)
			delete [] pInputPacket->pItemArray;
		if (pInputPacket->pdwIDs != NULL)
			delete [] pInputPacket->pdwIDs;
		if (pInputPacket->pObjectArray != NULL)
			delete [] pInputPacket->pObjectArray;

//DEBUG
Write_Log("[IPB]Attempting to Erase Memory of Packet");
WriteLog_Enter();

		delete(pInputPacket);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPendingCommand::Init(VOID* pCA, WORD wType)
{

	pCAccess = pCA;

	cClassAccess* pCA2 = (cClassAccess*)pCA;
	//initialize the internal IPB
	AcquiredData.Init(pCA);
	//for now, pending command IPBs will gain access to output logs by accepting the main IPB's log file(07.30.2003)
	AcquiredData.hLogFile = pCA2->pInputPacketBuffer->hLogFile;

	//NULL the command structures 
	pCommand = NULL;  //NOTE: all PendingCommand issue assumed to be of type command2 so actually this is not used
	pCommand2 = new PCMD2;

	pCommand2->wType = wType;
	pCommand2->pData = NULL;

	//initialize cutoff time
	llCutoffTime = pCA2->pTimer->iCurrTick;

	//set the access point in the cmddatareqtable
	iRequestIndex = wType;
	iCurrentSequenceNumber = 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cPendingCommand::PeekAcquire_IPB(BOOL bCopyFlag)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	BOOL bAcquiredFlag = FALSE;

	INPUTPACKET* pIPB_Trav = (INPUTPACKET*)pCA->pInputPacketBuffer->pIPB_First->pNext;

	//go through the IPB and find the first input packet with a valid tick time
	while (llCutoffTime > pIPB_Trav->dwTick && pIPB_Trav != pCA->pInputPacketBuffer->pIPB_Last)
	{

		pIPB_Trav = (INPUTPACKET*)pIPB_Trav->pNext;
	}

	//now cycle through the rest of the entries for the matching entry
	while (pIPB_Trav != pCA->pInputPacketBuffer->pIPB_Last)
	{

		ULONGLONG llAttributes = pCA->pCmdReqTable->CommandTypes[iRequestIndex].Sequence[iCurrentSequenceNumber];
		ULONGLONG llAttStore = llAttributes;
		int	iCheck = -1;

		BOOL PacketClear = TRUE;

		BOOL bPluralFlag = FALSE;

		//check the parameter type, and the plurality
		if (pIPB_Trav->dwPacketType == IPACKETTYPE_OBJREF)
			iCheck = pIPB_Trav->dwObjectsType;
		if (pIPB_Trav->dwPacketType == IPACKETTYPE_PARAM)
			iCheck = pIPB_Trav->dwParameterType;

		//check for valid parameter type
		llAttStore >>= iCheck;

		if (llAttStore % 2 == 0)
			PacketClear = FALSE;
		
		if (pIPB_Trav->dwPacketType == IPACKETTYPE_OBJREF && pIPB_Trav->dwNumObjects > 1)
			bPluralFlag = TRUE;
		if (pIPB_Trav->dwPacketType == IPACKETTYPE_PARAM && pIPB_Trav->dwNumParamItems > 1)
			bPluralFlag = TRUE;

		//check for valid plurality type if this input packet has multiple references in it
		if (bPluralFlag == TRUE)
		{
			llAttStore = llAttributes;
			llAttStore >>= CDR_BITFLAG_PLURAL;

			if (llAttStore % 2 == 0)
				PacketClear = FALSE;

		}

		if (PacketClear == FALSE)
			pCA->pInterface->Write_Log("PacketClear Failed.");
		if (PacketClear == TRUE)
			pCA->pInterface->Write_Log("PacketClear Passed.");		

		//acquire this packet if it passes all the test
		if (PacketClear == TRUE)
		{
			//we can take it straight out of the IPB if copy flag is not set
			if (bCopyFlag == FALSE)
			{
				AcquiredData.Add_Packet(pIPB_Trav);
				llCutoffTime = pIPB_Trav->dwTick;
				iCurrentSequenceNumber++;
				return TRUE;
			}
			//if it is, compile a new packet
			if (bCopyFlag == TRUE)
			{
				AcquiredData.Compile_Add_Packet(pIPB_Trav->dwPacketType,
												pIPB_Trav->dwObjectsType,
												pIPB_Trav->dwNumObjects,
												pIPB_Trav->pObjectArray,
												pIPB_Trav->pdwIDs,
												pIPB_Trav->dwParameterType,
												pIPB_Trav->dwNumParamItems,
												pIPB_Trav->pItemArray,
												TRUE);
				llCutoffTime = pIPB_Trav->dwTick;
				iCurrentSequenceNumber++;
				return TRUE;
			}
		}
		
		//otherwise, continue searching
		pIPB_Trav = (INPUTPACKET*)pIPB_Trav->pNext;
		
	}

	//if it reaches here and fails to return add a packet, return false
	return FALSE;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
BOOL cPendingCommand::PeekAcquire_SB()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//there are currently only two buffers that are SBs basically check them both and file input packets if anything is found
	LPSBE pSBE_Trav = (LPSBE)pCA->pInterface->pUSB_First->pNext;
	LPSBE pSBE_Last = pCA->pInterface->pUSB_Last;
	LPSBE pSBE_First = pCA->pInterface->pUSB_First;

//unit buffer first
	int iNumEntries = 0;
//check for type validity before trying anything
		ULONGLONG llAttributes = pCA->pCmdReqTable->CommandTypes[iRequestIndex].Sequence[iCurrentSequenceNumber];
		ULONGLONG llAttStore = llAttributes;

llAttStore >>= OBJTYPE_UNIT;

if (llAttStore % 2 == 1)
{
	while (pSBE_Trav != pSBE_Last)
	{
		iNumEntries++;
		pSBE_Trav = (LPSBE)pSBE_Trav->pNext;
	}

	//attemptt to compile buffer if number of entries is 1 or above
	if (iNumEntries != 0)
	{
		VOID** ppObjRefs = new VOID*[iNumEntries];
		DWORD* pdwIDs = new DWORD[iNumEntries];

		int lc1 = 0; //local counter
		pSBE_Trav = (LPSBE)pSBE_First->pNext;

		while (pSBE_Trav != pSBE_Last)
		{

			cUnitA* pUnit = pCA->pUnitTable->pUnitTable[pSBE_Trav->iID];
			ppObjRefs[lc1] = (VOID*)pUnit;
			pdwIDs[lc1] = pSBE_Trav->iID;
		
			//DEBUG
			pCA->pInterface->Write_Log("[IPB]ID Added to packet: ");
			char NText[50];
			itoa(pdwIDs[lc1], NText, 10);
			pCA->pInterface->Write_Log(NText);
			pCA->pInterface->WriteLog_Enter();

			//increment
			lc1++;
			pSBE_Trav = (LPSBE)pSBE_Trav->pNext;

		}

		//check for plurality
		llAttStore = (llAttributes >> CDR_BITFLAG_PLURAL) % 2;

		//only pass if the plural flag is enabled or it is a single entry
		if (iNumEntries == 1 || llAttStore == 1)
		{
			//compile and send the entry (copyflag FALSE, so no need to delete the allocated data here)
			AcquiredData.Compile_Add_Packet(IPACKETTYPE_OBJREF, OBJTYPE_UNIT, iNumEntries, ppObjRefs, pdwIDs, 0, 0, NULL, FALSE);

			//in practice there should only be one part of the buffer that has entries in it, so return here
			iCurrentSequenceNumber++;
			return TRUE;
		}

	}
}

	//now STRUCTURES
	iNumEntries = 0;

	pSBE_First = pCA->pInterface->pSSB_First;
	pSBE_Last = pCA->pInterface->pSSB_Last;
	pSBE_Trav = (LPSBE)pSBE_First->pNext;

llAttStore = llAttributes;
llAttStore >>= OBJTYPE_STRUCTURE;

if (llAttStore % 2 == 1)
{	
	while (pSBE_Trav != pSBE_Last)
	{
		iNumEntries++;
		pSBE_Trav = (LPSBE)pSBE_Trav->pNext;
	}

	//attemptt to compile buffer if number of entries is 1 or above
	if (iNumEntries != 0)
	{
		VOID** ppObjRefs = new VOID*[iNumEntries];
		DWORD* pdwIDs = new DWORD[iNumEntries]; 

		int lc1 = 0; //local counter
		pSBE_Trav = (LPSBE)pSBE_First->pNext;

		while (pSBE_Trav != pSBE_Last)
		{

			cStructureA* pStructure = pCA->pStructIDTable->pTable[pSBE_Trav->iID];
			ppObjRefs[lc1] = (VOID*)pStructure;
			pdwIDs[lc1] = pSBE_Trav->iID;
			
			//DEBUG
			pCA->pInterface->Write_Log("[IPB]ID Added to packet: ");
			char NText[50];
			itoa(pdwIDs[lc1], NText, 10);
			pCA->pInterface->Write_Log(NText);
			pCA->pInterface->WriteLog_Enter();

			//increment
			lc1++;
			pSBE_Trav = (LPSBE)pSBE_Trav->pNext;

		}

		//check for plurality
		llAttStore = (llAttributes >> CDR_BITFLAG_PLURAL) % 2;

		//only pass if the plural flag is enabled or it is a single entry
		if (iNumEntries == 1 || llAttStore == 1)
		{
			//compile and send the entry (copyflag FALSE, so no need to delete the allocated data here)
			AcquiredData.Compile_Add_Packet(IPACKETTYPE_OBJREF, OBJTYPE_STRUCTURE, iNumEntries, ppObjRefs, pdwIDs, 0, 0, NULL, FALSE);

			//in practice there should only be one part of the buffer that has entries in it, so return here
			iCurrentSequenceNumber++;
			return TRUE;
		}

	}	
}

//if we are here, no entries found, return FALSE
return FALSE;



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPendingCommand::Compile_Command2()
{

		cClassAccess* pCA = (cClassAccess*)pCAccess;
//05.30.2003
//NOTE: this actually issues the command2 command too

	//DEBUG
	pCA->pInterface->Write_Log("[Pending Command]Command Compilation Started");
	pCA->pInterface->WriteLog_Enter();

	//at this point, the Command2 command should already be allocated. the system is going through the list of inputpackets to compile the data
	//needs to go through once to find the size of the data block
int datasize = 0;

	INPUTPACKET* pIPB_Trav = (INPUTPACKET*)AcquiredData.pIPB_First->pNext;

	while (pIPB_Trav != AcquiredData.pIPB_Last)
	{
		if (pIPB_Trav->dwPacketType == IPACKETTYPE_OBJREF)
		{
			//add the size of two DWORDs (the reference type and the number of references)
			datasize += sizeof(DWORD) * 2;
			//add the size of all following DWORDs (NOTE: the existing command format for most COMMAND2 command uses WORDs for IDs. 
			//at the moment it is planned to begin using a new command format for command2 using DWORDs for all objref IDs.
			//as time goes on, the new format will supplant the old format in all applications) (05.30.2003)
			datasize += sizeof(DWORD) * pIPB_Trav->dwNumObjects;
		}
		if (pIPB_Trav->dwPacketType == IPACKETTYPE_PARAM)
		{
			//add the size of two DWORDs
			datasize += sizeof(DWORD) * 2;
			//depending on the datatype, add accordingly
			//IPARAMTYPE_FLOAT -> double
			if (pIPB_Trav->dwParameterType == IPARAMTYPE_FLOATS)
				datasize += sizeof(double) * pIPB_Trav->dwNumParamItems;
			if (pIPB_Trav->dwParameterType == IPARAMTYPE_LONGS)
				datasize += sizeof(LONG) * pIPB_Trav->dwNumParamItems;
			if (pIPB_Trav->dwParameterType == IPARAMTYPE_STRING)
				datasize += sizeof(char) * pIPB_Trav->dwNumParamItems;
			if (pIPB_Trav->dwParameterType == IPARAMTYPE_DWORDS)
				datasize += sizeof(DWORD) * pIPB_Trav->dwNumParamItems;
		}
		pIPB_Trav = (INPUTPACKET*)pIPB_Trav->pNext;
	}

	//when finished, add the size for the execution tick time(DWORD)
	datasize += sizeof(DWORD);

//DEBUG
pCA->pInterface->Write_Log("[Pending Command]Datasize Determined. Size:");
char NText[50];
itoa(datasize, NText, 10);
pCA->pInterface->Write_Log(NText);
pCA->pInterface->WriteLog_Enter();




//now allocate the buffer
	pCommand2->pData = new BYTE[datasize];
	pCommand2->wDataSize = datasize;

	//we have to go back through and gather all the information now
	pIPB_Trav = (INPUTPACKET*)AcquiredData.pIPB_First->pNext;

	DWORD* pdwPointDest = (DWORD*)pCommand2->pData;

	//add the lexectick
	*pdwPointDest = pCA->pTimer->iCurrTick + pCA->pGameOptions->iLatency;
	pdwPointDest++;

	while (pIPB_Trav != AcquiredData.pIPB_Last)
	{
		if (pIPB_Trav->dwPacketType == IPACKETTYPE_OBJREF)
		{


			//add the two dwords (the TYPE and then the NUMBER)
			*pdwPointDest = pIPB_Trav->dwObjectsType;
			pdwPointDest++;
			*pdwPointDest = pIPB_Trav->dwNumObjects;
			pdwPointDest++;
			
			//DEBUG
			DWORD* pdwPointDestdb = pdwPointDest;

			//add the size of all following DWORDs (NOTE: the existing command format for most COMMAND2 command uses WORDs for IDs. 
			//at the moment it is planned to begin using a new command format for command2 using DWORDs for all objref IDs.
			//as time goes on, the new format will supplant the old format in all applications) (05.30.2003)
			memcpy(pdwPointDest, pIPB_Trav->pdwIDs, sizeof(DWORD) * pIPB_Trav->dwNumObjects);
			pdwPointDest += pIPB_Trav->dwNumObjects;

			//DEBUG
			pCA->pInterface->Write_Log("[IPB]Data Added: ");
			for (int lc2 = 0; lc2 < pIPB_Trav->dwNumObjects; lc2++)
			{
				itoa(*pdwPointDestdb, NText, 10);
				pCA->pInterface->Write_Log(NText);
				pCA->pInterface->Write_Log(" ");
				pdwPointDestdb++;
				
			}
			pCA->pInterface->WriteLog_Enter();

		}
		if (pIPB_Trav->dwPacketType == IPACKETTYPE_PARAM)
		{
			//add the two dwords (the TYPE and then the NUMBER)
			*pdwPointDest = pIPB_Trav->dwParameterType;
			pdwPointDest++;
			*pdwPointDest = pIPB_Trav->dwNumParamItems;
			pdwPointDest++;
			//depending on the datatype, add accordingly
			//IPARAMTYPE_FLOAT -> double
			if (pIPB_Trav->dwParameterType == IPARAMTYPE_FLOATS)
			{
				double* pddPointDest = (double*)pdwPointDest;
				memcpy(pddPointDest, pIPB_Trav->pItemArray,  sizeof(double) * pIPB_Trav->dwNumParamItems);
				pddPointDest += pIPB_Trav->dwNumParamItems;
				pdwPointDest = (DWORD*)pddPointDest;


			}
			if (pIPB_Trav->dwParameterType == IPARAMTYPE_LONGS)
			{
				long* plPointDest = (long*)pdwPointDest;
				memcpy(plPointDest, pIPB_Trav->pItemArray,  sizeof(long) * pIPB_Trav->dwNumParamItems);
				plPointDest += pIPB_Trav->dwNumParamItems;
				pdwPointDest = (DWORD*)plPointDest;


			}
			if (pIPB_Trav->dwParameterType == IPARAMTYPE_STRING)
			{
				char* pchPointDest = (char*)pdwPointDest;
				memcpy(pchPointDest, pIPB_Trav->pItemArray,  sizeof(char) * pIPB_Trav->dwNumParamItems);
				pchPointDest += pIPB_Trav->dwNumParamItems;
				pdwPointDest = (DWORD*)pchPointDest;


			}
			if (pIPB_Trav->dwParameterType == IPARAMTYPE_DWORDS)
			{
				memcpy(pdwPointDest, pIPB_Trav->pItemArray, sizeof(DWORD) * pIPB_Trav->dwNumParamItems);
				pdwPointDest += pIPB_Trav->dwNumParamItems;
			}
		}
		pIPB_Trav = (INPUTPACKET*)pIPB_Trav->pNext;
	}

//
//DEBUG
pCA->pInterface->Write_Log("[Pending Command]Data Compiled. Issuing Command");
pCA->pInterface->WriteLog_Enter();
			//DEBUG
			pCA->pInterface->Write_Log("[IPB]Total Command Data: ");
			pdwPointDest = (DWORD*)pCommand2->pData;
			for (int lc2 = 0; lc2 < pCommand2->wDataSize / sizeof(DWORD); lc2++)
			{
				itoa(*pdwPointDest, NText, 10);
				pCA->pInterface->Write_Log(NText);
				pCA->pInterface->Write_Log(" ");
				pdwPointDest++;
				
			}
			pCA->pInterface->WriteLog_Enter();

	//and then, set whatever else we need, and then issue
	pCA->pDP_Multiplay->Send_Command2(pCommand2->wType, pCommand2->pData, pCommand2->wDataSize, FALSE, TRUE);
	pCA->pCommQueue->Issue_Command_Q2(pCommand2->wType, pCommand2->pData, pCommand2->wDataSize, TRUE);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cPendingCommand::Exit()
{
		cClassAccess* pCA = (cClassAccess*)pCAccess;

	//kill the internal IPB
	AcquiredData.Exit();
	//delete command and command2 structures if they are not NULL
//	if (pCommand != NULL)
//		delete(pCommand);

//DEBUG
pCA->pInterface->Write_Log("Attempting to Delete Command 2");
pCA->pInterface->WriteLog_Enter();

	if (pCommand2 != NULL)
	{
		if (pCommand2->pData != NULL)
			delete(pCommand2->pData);
		delete(pCommand2);
	}

//DEBUG
pCA->pInterface->Write_Log("Command2 Freed");
pCA->pInterface->WriteLog_Enter();

	pCommand2 = NULL;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::WriteLog_Enter()
{
if (DEBUG + DEBUG_INTERFACE >= 1)
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
void cIGrid::Exit()
{
	//clear ifc elems
	Clear_IfcElems();
	//delete anchors
	delete pIfcElem_First;
	delete pIfcElem_Last;

	int lc1; //local counter
	for (lc1 = 0; lc1 < iElements_x * iElements_y; lc1++)
	{
		if (pGrid[lc1].Text != NULL)
			delete(pGrid[lc1].Text);
	}

	delete(pGrid);
	pGrid = NULL;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cIfcElement* cIGrid::Get_IfcElem(int iElemNumber)
{
	int icount = 0;

	cIfcElement* pTrav = pIfcElem_First; 

while (icount < iElemNumber)
{
	pTrav = pTrav->pNext;
	icount++;

	if (pTrav == pIfcElem_Last)
		return NULL;
}

	return pTrav;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
RECT cIGrid::Calc_GridcellRect(int iGridIndex)
{
	RECT RectRet; 
	
	int ix = iGridIndex % iElements_x; 
	int iy = iGridIndex / iElements_x; 
	RectRet.top = (iy * iCellY_pixels) + GridPos.top; 
	RectRet.bottom = RectRet.top + iCellY_pixels; 
	RectRet.left = ix * iCellX_pixels + GridPos.left; 
	RectRet.right = RectRet.left + iCellX_pixels; 
	
	return RectRet;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cIGrid::Init(int iXOff, int iYOff, int iElemX, int iElemY, int iCellX, int iCellY)
{

 pGrid = new IGRIDCELL[iElemY * iElemX];
//zero memory
ZeroMemory(pGrid, sizeof(IGRIDCELL)*iElemY*iElemX);

//loop to NULL text pointers
for (int lc11 = 0; lc11 < iElemX * iElemY; lc11++)
		pGrid[lc11].Text = NULL;

iHeight_pixels = iElemY * iCellY; 
iWidth_pixels = iElemX * iCellX;
GridPos.left = iXOff; 
GridPos.top = iYOff; 
GridPos.right = GridPos.left + iWidth_pixels; 
GridPos.bottom = GridPos.top + iHeight_pixels;
iElements_x = iElemX; 
iElements_y = iElemY; 
iCellX_pixels = iCellX; 
iCellY_pixels = iCellY;

	//initialize interface element list (06.11.2005)
	pIfcElem_First = new cIfcElement;
		pIfcElem_First->pPrev = NULL;
	pIfcElem_Last = new cIfcElement;
		pIfcElem_Last->pNext = NULL;

	pIfcElem_First->pNext = pIfcElem_Last;
	pIfcElem_Last->pPrev = pIfcElem_First;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cIGrid::Clear_IfcElems()
{
	cIfcElement* pTrav = pIfcElem_First->pNext;
	cIfcElement* pTrav2;

	while (pTrav != pIfcElem_Last)
	{
		pTrav2 = pTrav->pNext;

		pTrav->Exit();
		delete(pTrav);

		pTrav = pTrav2;
	}
	//re-link anchors
	pIfcElem_First->pNext = pIfcElem_Last;
	pIfcElem_Last->pPrev = pIfcElem_First;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cIGrid::Add_IfcElem(cIfcElement* pElem)
{
	cIfcElement* pNext = pIfcElem_Last;
	cIfcElement* pPrev = pIfcElem_Last->pPrev;

	

	//link in
	pElem->pNext = pNext;
	pElem->pPrev = pPrev;
	pNext->pPrev = pElem;
	pPrev->pNext = pElem;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cIGrid::Calc_GridIndex(int xpos_scr, int ypos_scr)
{

	//about the below formula, it works because it errs by +1 because it doesn't take into the account that the grid starts at 0,
	//and at the same time errs by -1 because integer division truncates the very last entry from calculation

int iG =  ((xpos_scr - GridPos.left) / iCellX_pixels) + ( int((ypos_scr - GridPos.top) / iCellY_pixels) * iElements_x); 

return iG;



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cIGrid::Set_Scrollbar(BOOL bEnabled, int iScrVal, int iScrMax, int iScrMin)
{
	bScrollbarFlag = bEnabled;

	if (iScrVal >= 0)
	{ iScrollVal = iScrVal; } //enables user to specify invalid("do not set") values
	if (iScrMax >= 0)
	{	iScrollMax = iScrMax; }
	if (iScrMin >= 0)
	{ iScrollMin = iScrMin; }

	if (iScrollVal > iScrollMax)
		iScrollVal = iScrollMax;
	if (iScrollVal < iScrollMin)
		iScrollVal = iScrollMin;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cIfcElement::Init(int left, int right, int top, int bottom)
{
	//
	Rect_Pos.bottom = bottom;
	Rect_Pos.left = left;
	Rect_Pos.right = right;
	Rect_Pos.top = top;

	pListBoxInfo = NULL;

	iNumChildren = 0;

	//initialize pointers
	for (int lc1 = 0; lc1 < IFC_ELEM_CAT_NUMELEMS; lc1++)
	{
		pCmds[lc1] = NULL;
	}

	//for now, do not use pParent pointer
	pParent = NULL;
	pChildren = NULL; //initialize
	pTail = NULL;
	pAttachedElement = NULL;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cIfcElement::Add_IfcElem_Front(cIfcElement* pElem)
{
	if (pChildren == NULL)
	{
		pElem->pNext = NULL;
		pElem->pPrev = NULL;

		pChildren = pElem;
		pTail = pElem;
		iNumChildren = 1;

		return;
	}

	//otherwise, link off the front
	pElem->pPrev = NULL;
	pElem->pNext = pChildren;

	pChildren->pPrev = pElem;

	pChildren = pElem;

	iNumChildren++;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cIfcElement::Add_IfcElem(cIfcElement* pElem)
{
	if (pChildren == NULL)
	{
		pElem->pNext = NULL;
		pElem->pPrev = NULL;

		pChildren = pElem;
		pTail = pElem;
		iNumChildren = 1;

		return;
	}

	//otherwise, link off the tail
	pElem->pPrev = pTail;
	pTail->pNext = pElem;

	pElem->pNext = NULL;
	pElem->pPrev = pTail;

	pTail = pElem;

	iNumChildren++;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cIfcElement::Clear_IfcElems()
{
	cIfcElement* pTrav = pChildren;
	cIfcElement* pTrav2 = pChildren;

	while (pTrav != NULL)
	{
		pTrav2 = pTrav->pNext;
		pTrav->Exit();
		delete pTrav;

		pTrav = pTrav2;
	}

	//reset pointers
	pChildren = NULL;
	pTail = NULL;

	iNumChildren = 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cIfcElement::Clear_Cmds(int iCategory)
{
	//
	PCMD2* pTrav = pCmds[iCategory];
	PCMD2* pTrav2;

	while (pTrav != NULL)
	{
		pTrav2 = pTrav->pNext;

		delete [] pTrav->pData;
		delete pTrav;
		
		pTrav = pTrav2;
	}

	pCmds[iCategory] = NULL;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cIfcElement::Exit()
{
	//clear subelements
	Clear_IfcElems();  //recursive!

	//clear categories
	for (int lc1 = 0; lc1 < IFC_ELEM_CAT_NUMELEMS; lc1++)
	{
		Clear_Cmds(lc1);
	}
	//clear the attached element (recursive!)
	if (pAttachedElement != NULL)
        pAttachedElement->Exit();

}
/////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
/////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cIfcElement::Add_Cmd(int iCategory, PCMD2* pCommand, BOOL bCopyFlag)
{
	PCMD2* pPrev = pCmds[iCategory];
	PCMD2* pAdd = pCommand;

	if (bCopyFlag == TRUE)
	{		
		pAdd = new PCMD2;

		pAdd->wType = pCommand->wType;
		pAdd->wDataSize = pCommand->wDataSize;
		pAdd->pData = new BYTE[pAdd->wDataSize];
			
		memcpy(pAdd->pData, pCommand->pData, pAdd->wDataSize);
	}

	if (pPrev == NULL)
	{
		pCmds[iCategory] = pAdd;

		pAdd->pPrev = NULL;
		pAdd->pNext = NULL;
	} else
	{
		while( pPrev->pNext != NULL)
		{
			pPrev = pPrev->pNext;
		}

		pPrev->pNext = pAdd;
		pAdd->pPrev = pPrev;
		pAdd->pNext = NULL;

	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cIfcElement* cInterface::Create_Button(LPSTR ButtonText, int iX, int iY)
{
		cClassAccess* pCA = (cClassAccess*)pCAccess;
		
		PCMD2* pCmdNew = NULL;

		int iretsize = 0;
		int strlength = strlen(ButtonText);
		int ixoffset = 0.5*MBUTTON_SIZE_X - (0.5*pCA->pFontInfoTable->TextLen_Pixels(ButtonText, strlength, FONT_STANDARD1));
		int iyoffset = 0.5*MBUTTON_SIZE_Y - (0.5* pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight);
		cIfcElement* pElemNew = new cIfcElement;
		pElemNew->Init(iX, iX + MBUTTON_SIZE_X, iY, iY + MBUTTON_SIZE_Y);
			pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, ISURF_MENUBUTTONS01, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, MBUTTON_SIZE_X * 0, MBUTTON_SIZE_X*1, MBUTTON_SIZE_Y*0, MBUTTON_SIZE_Y*1, &iretsize); 
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_BLIT;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
			pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left + ixoffset + 1, pElemNew->Rect_Pos.top + iyoffset + 1, FONT_STANDARD1, pCA->pColorTable->Black, strlength, ButtonText, &iretsize); 
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_DRAWTEXT;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
			pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left + ixoffset, pElemNew->Rect_Pos.top + iyoffset, FONT_STANDARD1, pCA->pColorTable->White - 1, strlength, ButtonText, &iretsize); 
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_DRAWTEXT;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
			pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, ISURF_MENUBUTTONS01, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, MBUTTON_SIZE_X * 1, MBUTTON_SIZE_X*2, MBUTTON_SIZE_Y*0, MBUTTON_SIZE_Y*1, &iretsize); 
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_BLIT;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
			pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left + ixoffset, pElemNew->Rect_Pos.top + iyoffset, FONT_STANDARD1, pCA->pColorTable->White - 1, strlength, ButtonText, &iretsize); 
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_DRAWTEXT;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
			pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, ISURF_MENUBUTTONS01, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, MBUTTON_SIZE_X * 2, MBUTTON_SIZE_X*3, MBUTTON_SIZE_Y*0, MBUTTON_SIZE_Y*1, &iretsize); 
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_BLIT;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, FALSE);
			pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left + ixoffset, pElemNew->Rect_Pos.top + iyoffset, FONT_STANDARD1, pCA->pColorTable->White - 1, strlength, ButtonText, &iretsize); 
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_DRAWTEXT;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, FALSE);

		return pElemNew;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cIfcElement* cInterface::Create_TextInputBox(int iX, int iY, int iWidth_Pixels, int iFont, BOOL bUnicodeFlag, void* pText, int iStringLimit)
{
		cClassAccess* pCA = (cClassAccess*)pCAccess;
		int ftheight = pCA->pFontInfoTable->Fonts[iFont].iHeight;
		PCMD2* pCmdNew;
		int iretsize;

		//Test Text Entry Box
		cIfcElement* pElemNew = new cIfcElement;
		pElemNew->Init(iX, iX + iWidth_Pixels, iY, iY + 10 + ftheight * 1.2);
			pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcListBox(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.right, pElemNew->Rect_Pos.top, pElemNew->Rect_Pos.bottom, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_DRAWIFCLISTBOX;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

			pCmdNew = new PCMD2;
				cCQ2TYPE_DRAWTEXT4_DATA* pData = new cCQ2TYPE_DRAWTEXT4_DATA;
				pData->Set(bUnicodeFlag, RTC_SECONDARY, pElemNew->Rect_Pos.left + 5, iY + 5 + ftheight * 0.1, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, (void*)pText);
				pCmdNew->pData = (BYTE*)pData; 		//pCA->pCommQueue->Assemble_CommData_DrawText2(bUnicodeFlag, RTC_SECONDARY, pElemNew->Rect_Pos.left + 5, iY + 5 + ftheight * 0.1, FONT_STANDARD1, pCA->pColorTable->White - 1, (char*)pText, &iretsize);
				pCmdNew->wDataSize = sizeof(cCQ2TYPE_DRAWTEXT4_DATA);
				pCmdNew->wType = CQ2TYPE_DRAWTEXT4;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

			pCmdNew = new PCMD2;
		if (bUnicodeFlag == FALSE)
		{
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Set_Inputstring((char*)pText, iStringLimit, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_SET_INPUTSTRING;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		}
		if (bUnicodeFlag == TRUE)
		{
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Set_Inputstring_Wide((WCHAR*)pText, iStringLimit, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_SET_INPUTSTRING_WIDE;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		}

		return pElemNew;	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cIfcElement* cInterface::Create_IfcPanel(int iLeft, int iRight, int iTop, int iBottom)
{
	
		cClassAccess* pCA = (cClassAccess*)pCAccess;
		PCMD2* pCmdNew;
		int iretsize;
		cIfcElement* pElemNew = new cIfcElement;

//background interface panel
		pElemNew->Init(iLeft, iRight, iTop, iBottom);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcPanel(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.right, pElemNew->Rect_Pos.top, pElemNew->Rect_Pos.bottom, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWIFCPANEL;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
	
return pElemNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cIfcElement* cInterface::Create_IfcSecondaryPanel(int iLeft, int iRight, int iTop, int iBottom, int iColorIndex)
{
		cClassAccess* pCA = (cClassAccess*)pCAccess;
		PCMD2* pCmdNew;
		int iretsize;
		cIfcElement* pElemNew = new cIfcElement;

//background interface panel
		pElemNew->Init(iLeft, iRight, iTop, iBottom);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcSecondaryPanel(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.right, pElemNew->Rect_Pos.top, pElemNew->Rect_Pos.bottom, iColorIndex, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWIFCSECONDARYPANEL;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
	
return pElemNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cIfcElement* cInterface::Create_SecondaryButton(int iLeft, int iRight, int iTop, int iBottom, LPSTR DispText, int iFont, int iColorIndex)
{
		cClassAccess* pCA = (cClassAccess*)pCAccess;
		PCMD2* pCmdNew;
		int iretsize;
		cIfcElement* pElemNew = new cIfcElement;

int ilength = strlen(DispText);
//int iXSpan = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iWidth * ilength * 1.5;
//int iYSpan = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;

int iYMid = (iTop + iBottom ) * 0.5;
int iXMid = (iLeft + iRight ) * 0.5;
int iFontHeight = pCA->pFontInfoTable->Fonts[iFont].iHeight;
int iStringWidth = pCA->pFontInfoTable->TextLen_Pixels(DispText, ilength, FONT_STANDARD1);


//background interface list box
		pElemNew->Init(iLeft, iRight, iTop, iBottom);
		pCmdNew = new PCMD2;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcSecondaryPanel(RTC_SECONDARY, iLeft, iRight, iTop, iBottom, iColorIndex, &iretsize);
			pCmdNew->wDataSize = iretsize;
			pCmdNew->wType = CQ2TYPE_DRAWIFCSECONDARYPANEL;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY, iXMid - (iStringWidth * 0.5) + 1, iYMid - (iFontHeight * 0.5) + 1, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, DispText);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY, iXMid - (iStringWidth * 0.5), iYMid - (iFontHeight * 0.5), 0, FONT_STANDARD1, pCA->pColorTable->Black, DispText);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcSecondaryPanel(RTC_SECONDARY, iLeft, iRight, iTop, iBottom, iColorIndex + BLIT_FPCOL, &iretsize);
			pCmdNew->wDataSize = iretsize;
			pCmdNew->wType = CQ2TYPE_DRAWIFCSECONDARYPANEL;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY, iXMid - (iStringWidth * 0.5) + 1, iYMid - (iFontHeight * 0.5) + 1, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, DispText);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY, iXMid - (iStringWidth * 0.5), iYMid - (iFontHeight * 0.5), 0, FONT_STANDARD1, pCA->pColorTable->Black, DispText);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

		pCmdNew = new PCMD2;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcSecondaryPanel(RTC_SECONDARY, iLeft, iRight, iTop, iBottom, iColorIndex + BLIT_FPCOL + BLIT_FPCOL, &iretsize);
			pCmdNew->wDataSize = iretsize;
			pCmdNew->wType = CQ2TYPE_DRAWIFCSECONDARYPANEL;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY, iXMid - (iStringWidth * 0.5) + 1, iYMid - (iFontHeight * 0.5) + 1, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, DispText);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY, iXMid - (iStringWidth * 0.5), iYMid - (iFontHeight * 0.5), 0, FONT_STANDARD1, pCA->pColorTable->Black, DispText);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, FALSE);	

return pElemNew;
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cIfcElement* cInterface::Create_ObjectListing(int iX, int iY, int iControl, LPSTR DispText)
{
		cClassAccess* pCA = (cClassAccess*)pCAccess;
		PCMD2* pCmdNew;
		int iretsize;
		cIfcElement* pElemNew = new cIfcElement;

int ilength = strlen(DispText);
int iXSpan = pCA->pFontInfoTable->TextLen_Pixels(DispText, ilength, FONT_STANDARD1) * 1.5;
int iYSpan = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.2;


//background interface list box
		pElemNew->Init(iX, iX + iXSpan, iY, iY + iYSpan);
		pCmdNew = new PCMD2;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcSecondaryPanel(RTC_SECONDARY, iX, iX + iXSpan, iY, iY + iYSpan, iControl, &iretsize);
			pCmdNew->wDataSize = iretsize;
			pCmdNew->wType = CQ2TYPE_DRAWIFCSECONDARYPANEL;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY, iX + (iXSpan * 0.25) + 1, iY + (iYSpan * 0.1) + 1, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, DispText);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY, iX + (iXSpan * 0.25), iY + (iYSpan * 0.1), 0, FONT_STANDARD1, pCA->pColorTable->Black, DispText);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcSecondaryPanel(RTC_SECONDARY, iX, iX + iXSpan, iY, iY + iYSpan, iControl + BLIT_FPCOL, &iretsize);
			pCmdNew->wDataSize = iretsize;
			pCmdNew->wType = CQ2TYPE_DRAWIFCSECONDARYPANEL;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY, iX + (iXSpan * 0.25) + 1, iY + (iYSpan * 0.1) + 1, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, DispText);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY, iX + (iXSpan * 0.25), iY + (iYSpan * 0.1), 0, FONT_STANDARD1, pCA->pColorTable->Black, DispText);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

		pCmdNew = new PCMD2;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcSecondaryPanel(RTC_SECONDARY, iX, iX + iXSpan, iY, iY + iYSpan, iControl + BLIT_FPCOL + BLIT_FPCOL, &iretsize);
			pCmdNew->wDataSize = iretsize;
			pCmdNew->wType = CQ2TYPE_DRAWIFCSECONDARYPANEL;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY, iX + (iXSpan * 0.25) + 1, iY + (iYSpan * 0.1) + 1, 0, FONT_STANDARD1, pCA->pColorTable->White - 1, DispText);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			 pCA->pCommQueue->Pack_CommData_DrawText2(pCmdNew, FALSE, RTC_SECONDARY, iX + (iXSpan * 0.25), iY + (iYSpan * 0.1), 0, FONT_STANDARD1, pCA->pColorTable->Black, DispText);
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, FALSE);	

return pElemNew;
	



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cIfcElement* cInterface::Create_IfcListBox(int iLeft, int iRight, int iTop, int iBottom)
{
	
		cClassAccess* pCA = (cClassAccess*)pCAccess;
		PCMD2* pCmdNew;
		int iretsize;
		cIfcElement* pElemNew = new cIfcElement;

//background interface list box
		pElemNew->Init(iLeft, iRight, iTop, iBottom);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawIfcListBox(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.right, pElemNew->Rect_Pos.top, pElemNew->Rect_Pos.bottom, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWIFCLISTBOX;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
	
return pElemNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cIfcElement* cInterface::Create_StaticTextElem(LPSTR Text, int iFont, int iColor, int iX, int iY)
{
	return Create_StaticTextElem(Text, iFont, iColor, iX, iY, 0);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cIfcElement* cInterface::Create_StaticTextElem(LPSTR Text, int iFont, int iColor, int iX, int iY, DWORD dwXLimit)
{
		cClassAccess* pCA = (cClassAccess*)pCAccess;
		PCMD2* pCmdNew;
		int iretsize;
		cIfcElement* pElemNew = new cIfcElement;

		pElemNew->Init(iX, iX + 1, iY, iY + 1);
		pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top, dwXLimit, iFont, iColor, strlen(Text), Text, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
	
return pElemNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
PCMD2* cInterface::Add_StaticTextElem(cIfcElement* pElem, int iCategory, LPSTR Text, int iFont, int iColor, int iX, int iY)
{
		cClassAccess* pCA = (cClassAccess*)pCAccess;
		int iretsize;

		PCMD2* pCmdNew = new PCMD2;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_DrawText(RTC_SECONDARY, iX, iY, iFont, iColor, strlen(Text), Text, &iretsize);
		pCmdNew->wDataSize = iretsize;
		pCmdNew->wType = CQ2TYPE_DRAWTEXT;
		pElem->Add_Cmd(iCategory, pCmdNew, FALSE);
	
return pCmdNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cIfcElement* cInterface::Create_TutorialPage(int iLeft, int iRight, int iTop, int iBottom, int iXText, int iYText, int iXTextLimit, LPSTR MsgText, int iXButton, int iYButton, LPSTR ButtonText, int iFont, int iButtonJumpTo)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int iretsize = 0;

	cIfcElement* pElemNew = NULL;
	PCMD2* pCmdNew = NULL;

	cIfcElement* pMasterElem = Create_IfcPanel(iLeft, iRight, iTop, iBottom);

//don't add button if we had a negative 2 iButtonJumpTO
if (iButtonJumpTo != -2)
{
	//add button
	pElemNew = Create_Button(ButtonText, iXButton, iYButton);
		pCmdNew = new PCMD2;
		int* piTrav = new int;
		*piTrav = iButtonJumpTo;
		pCmdNew->pData = (BYTE*)piTrav; 
		pCmdNew->wDataSize = sizeof(int);
		pCmdNew->wType = CQ2TYPE_SET_TUTORIAL_PAGE;
		pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
    pMasterElem->Add_IfcElem(pElemNew);
}
	//display text
	pElemNew = Create_StaticTextElem(MsgText, iFont, pCA->pColorTable->Black, iXText, iYText, iXTextLimit);
	pMasterElem->Add_IfcElem(pElemNew);

	return pMasterElem;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cIfcElement* cInterface::Create_SquareButton(int iX, int iY, int iColorIndex)
{
		cClassAccess* pCA = (cClassAccess*)pCAccess;
		
		PCMD2* pCmdNew = NULL;

		int iretsize = 0;
		cIfcElement* pElemNew = new cIfcElement;
		pElemNew->Init(iX, iX + SQCMDBUTTON_SIZE_X, iY, iY + SQCMDBUTTON_SIZE_Y);
			pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, ISURF_CMDBUTTONS_SQ, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top,  0, SQCMDBUTTON_SIZE_X, 0 + iColorIndex * SQCMDBUTTON_SIZE_Y, SQCMDBUTTON_SIZE_Y + iColorIndex * SQCMDBUTTON_SIZE_Y, &iretsize); 
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_BLIT;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
//			pCmdNew = new PCMD2;
//				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, ISURF_CMDBUTTONS_SQ, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top,  SQCMDBUTTON_SIZE_X * 3, SQCMDBUTTON_SIZE_X * 4, 0 , 24, &iretsize); 
//				pCmdNew->wDataSize = iretsize;
//				pCmdNew->wType = CQ2TYPE_BLIT;
//				pElemNew->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
			pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, ISURF_CMDBUTTONS_SQ, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top,  SQCMDBUTTON_SIZE_X, SQCMDBUTTON_SIZE_X * 2, iColorIndex * SQCMDBUTTON_SIZE_Y, SQCMDBUTTON_SIZE_Y + iColorIndex * SQCMDBUTTON_SIZE_Y, &iretsize); 
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_BLIT;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
//			pCmdNew = new PCMD2;
//				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, ISURF_CMDBUTTONS_SQ, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top,  72, 72 + 24, 0 , 24, &iretsize); 
//				pCmdNew->wDataSize = iretsize;
//				pCmdNew->wType = CQ2TYPE_BLIT;
//				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
			pCmdNew = new PCMD2;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, ISURF_CMDBUTTONS_SQ, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top,  SQCMDBUTTON_SIZE_X * 2, SQCMDBUTTON_SIZE_X * 3, iColorIndex * SQCMDBUTTON_SIZE_Y, SQCMDBUTTON_SIZE_Y + iColorIndex * SQCMDBUTTON_SIZE_Y, &iretsize); 
				pCmdNew->wDataSize = iretsize;
				pCmdNew->wType = CQ2TYPE_BLIT;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, FALSE);
//			pCmdNew = new PCMD2;
//				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Blit(RTC_SECONDARY, ISURF_CMDBUTTONS_SQ, pElemNew->Rect_Pos.left, pElemNew->Rect_Pos.top,  72, 72 + 24, 0 , 24, &iretsize); 
//				pCmdNew->wDataSize = iretsize;
//				pCmdNew->wType = CQ2TYPE_BLIT;
//				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLON_DRAW, pCmdNew, FALSE);

		return pElemNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cIfcElement* cInterface::Create_IfcListBox2(int iLeft, int iRight, int iTop, int iBottom, cIfcElement* pDataList)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	
	//DEBUG
	Write_Log("Create ListBox2 called.");
	WriteLog_Enter();
	int iWidth = pCA->pImages[ISURF_SCROLLBAR]->DDSDImage.dwWidth;

	PCMD2* pCmdNew = NULL;

	cIfcElement* pElemNew = new cIfcElement;
	//
	pElemNew->Init(iLeft, iRight + iWidth, iTop, iBottom);
	//add a listboxinfo
	cIfcListBoxInfo* pListBoxInfo = new cIfcListBoxInfo;

	pListBoxInfo->iListWindowSize = 0; //start with 0
	pListBoxInfo->iPrimaryOffset = 0; //currently not possible to make any offset
	pListBoxInfo->pListBox = pDataList;
	pListBoxInfo->pZeroOffsetElement = pDataList->pChildren;

	//add 
	pElemNew->pListBoxInfo = pListBoxInfo;

	//add up and down arrows

	cIfcElement* pArrowUp = new cIfcElement;
	//usage of iwidth in both is not a typo(for now)
	pArrowUp->Init(iRight, iRight + iWidth, iTop, iTop + iWidth);
		pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_LISTBOX_ARROW_UP;
			cIfcElement** ppParent = new cIfcElement*;
			*ppParent = pElemNew;
			pCmdNew->pData = (BYTE*)ppParent;
			pCmdNew->wDataSize = sizeof(cIfcElement*);
			pArrowUp->Add_Cmd(IFC_ELEM_CAT_ONLDOWN, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_LISTBOX_CONDITIONAL_BLIT;
			ppParent = new cIfcElement*;
			*ppParent = pElemNew;
			int iretsize = 0;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_ListBox_Conditional_Blit(pElemNew, RTC_SECONDARY, ISURF_SCROLLBAR, pArrowUp->Rect_Pos.left, pArrowUp->Rect_Pos.top, 0, 16, 0, 16, &iretsize);
			pArrowUp->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
			pCmdNew->wDataSize = iretsize;
		pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_LISTBOX_CONDITIONAL_BLIT;
			ppParent = new cIfcElement*;
			*ppParent = pElemNew;
			iretsize = 0;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_ListBox_Conditional_Blit(pElemNew, RTC_SECONDARY, ISURF_SCROLLBAR, pArrowUp->Rect_Pos.left, pArrowUp->Rect_Pos.top, 0, 16, 48, 64, &iretsize);
			pArrowUp->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
			pCmdNew->wDataSize = iretsize;
	pElemNew->Add_IfcElem(pArrowUp);



	cIfcElement* pArrowDown = new cIfcElement;
	pArrowDown->Init(iRight, iRight + iWidth, iBottom - iWidth, iBottom);
		pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_LISTBOX_ARROW_DOWN;
			ppParent = new cIfcElement*;
			*ppParent = pElemNew;
			pCmdNew->pData = (BYTE*)ppParent;
			pCmdNew->wDataSize = sizeof(cIfcElement*);
			pArrowDown->Add_Cmd(IFC_ELEM_CAT_ONLDOWN, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_LISTBOX_CONDITIONAL_BLIT;
			ppParent = new cIfcElement*;
			*ppParent = pElemNew;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_ListBox_Conditional_Blit(pElemNew, RTC_SECONDARY, ISURF_SCROLLBAR, pArrowDown->Rect_Pos.left, pArrowDown->Rect_Pos.top, 0, 16, 16, 32, &iretsize);
			pCmdNew->wDataSize = iretsize;
			pArrowDown->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);
		pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_LISTBOX_CONDITIONAL_BLIT;
			ppParent = new cIfcElement*;
			*ppParent = pElemNew;
			iretsize = 0;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_ListBox_Conditional_Blit(pElemNew, RTC_SECONDARY, ISURF_SCROLLBAR, pArrowDown->Rect_Pos.left, pArrowDown->Rect_Pos.top, 0, 16, 64, 80, &iretsize);
			pArrowDown->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
			pCmdNew->wDataSize = iretsize;
		pElemNew->Add_IfcElem(pArrowDown);


	cIfcElement* pSlider = new cIfcElement;
		pSlider->Init(iRight, iRight + iWidth, iTop + iWidth, iBottom - iWidth);
			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_LISTBOX_DISP_SLIDER;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_ListBox_Disp_Slider(pElemNew, RTC_SECONDARY, 0, &iretsize);
			pCmdNew->wDataSize = iretsize;
			pSlider->Add_Cmd(IFC_ELEM_CAT_DEFAULT_DRAW, pCmdNew, FALSE);

			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_SLIDER_INPUT;
			cIfcElement** ppElem = new cIfcElement*;
			*ppElem = pElemNew;
			pCmdNew->pData = (BYTE*)ppElem;
			pCmdNew->wDataSize = sizeof(cIfcElement*);
			pSlider->Add_Cmd(IFC_ELEM_CAT_ONLON, pCmdNew, FALSE);

			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_LISTBOX_DISP_SLIDER;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_ListBox_Disp_Slider(pElemNew, RTC_SECONDARY, 1, &iretsize);
			pCmdNew->wDataSize = iretsize;
			pSlider->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);

			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_LISTBOX_DISP_SLIDER;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_ListBox_Disp_Slider(pElemNew, RTC_SECONDARY, 1, &iretsize);
			pCmdNew->wDataSize = iretsize;
			pSlider->Add_Cmd(IFC_ELEM_CAT_ONLDOWN_DRAW, pCmdNew, FALSE);

		pCmdNew = new PCMD2;

		pElemNew->Add_IfcElem(pSlider);

	return pElemNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cIfcElement* cInterface::Create_IfcListBox2Ex(int iLeft, int iRight, int iTop, int iBottom, cIfcElement* pDataList, int iUniformListingHeight)   //extended version
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cIfcElement* pElemNew = Create_IfcListBox2(iLeft, iRight, iTop, iBottom, pDataList);

RECT TargRect;

		TargRect.left = iLeft + 5;
		TargRect.right = iRight - 10;
		TargRect.top = iTop + 5;
		int iYSpan = iBottom -  iTop - 10;
		int iYSpanListing = iUniformListingHeight;
		int iNumSlots = iYSpan / iYSpanListing;

		for (int lcy = 0; lcy < iNumSlots; lcy++)
		{
			Add_ListBoxListing(pElemNew, lcy, TargRect.left, TargRect.right, TargRect.top + iYSpanListing * lcy, TargRect.top + iYSpanListing * (lcy+1));
		}

	return pElemNew;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Add_ListBoxListing(cIfcElement* pParentElem, int iOffset, int iLeft, int iRight, int iTop, int iBottom)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	cIfcElement* pNewListing = new cIfcElement;
	
	pNewListing->Init(iLeft, iRight, iTop, iBottom);
	
	int iretsize = 0;
	for (int lc1 = 0; lc1 < IFC_ELEM_CAT_NUMELEMS; lc1++)
	{
		PCMD2* pCmdNew = new PCMD2;

		pCmdNew->wType = CQ2TYPE_LISTBOX_EXECUTE_IFC;
		iretsize = 0;
		pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_ListBox_Execute_Ifc(pParentElem, pNewListing, iOffset, lc1, &iretsize);
		pCmdNew->wDataSize = iretsize;

		pNewListing->Add_Cmd(lc1, pCmdNew, FALSE);
	}
	
	//finally, add the element to the parent element
	pParentElem->Add_IfcElem(pNewListing);
	cIfcListBoxInfo* pListBoxInfo = (cIfcListBoxInfo*)pParentElem->pListBoxInfo;
	pListBoxInfo->iListWindowSize++;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cInterface::Init_InfoGrids()
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//add display panel
		int iSpacingFactor = pCA->pGDisplay->iMinimumHeight; //

		int iTextHeight = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight;
		int iTextWidth = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iWidth;

		if ( (iTextHeight * 1.2) > iSpacingFactor)
			iSpacingFactor = iTextHeight * 1.2;

		int iSpan = IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - IBoxes[IBOX_CONSOLE_INFO].BoxPos.left;
		int iYOffset = iSpacingFactor * 3;
PCMD2* pCmdNew = NULL;
int iretsize = 0;

for (int lc1 = 0; lc1 < 11; lc1++)
{
		IBoxes[IBOX_CONSOLE_INFO].pIGrids[lc1].Init(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top, 2, 2, 2, 2);
		IBoxes[IBOX_CONSOLE_INFO].pIGrids[lc1].Set_Scrollbar(TRUE, 0, 100, 0);

	//add ifcpanel
		cIfcElement* pElemNew;
		pElemNew = Create_IfcPanel(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left, IBoxes[IBOX_CONSOLE_INFO].BoxPos.right, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top, IBoxes[IBOX_CONSOLE_INFO].BoxPos.bottom);
		IBoxes[IBOX_CONSOLE_INFO].pIGrids[lc1].Add_IfcElem(pElemNew);
		cIfcElement* pPanel = pElemNew;
		int iMid = (pPanel->Rect_Pos.left + pPanel->Rect_Pos.right) / 2;

//disable the buttons
/*
	//add buttons
		pElemNew = Create_SecondaryButton(pPanel->Rect_Pos.left + 5, iMid, pPanel->Rect_Pos.top + 5, pPanel->Rect_Pos.top + 5 + iSpacingFactor, "Unit", FONT_STANDARD1, 0);
			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_SET_ACTIVEIGRID;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Set_ActiveIGrid(IBOX_CONSOLE_INFO, 0, &iretsize);
			pCmdNew->wDataSize = iretsize;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
        IBoxes[IBOX_CONSOLE_INFO].pIGrids[lc1].Add_IfcElem(pElemNew);
	
		//shorten "Struct" to "Str" at low resolutions
		if (pCA->pDD_Display->iResProduct > 480000)
			pElemNew = Create_SecondaryButton(iMid, pPanel->Rect_Pos.right - 5, pPanel->Rect_Pos.top + 5, pPanel->Rect_Pos.top + 5 + iSpacingFactor, "Struct", FONT_STANDARD1, 1);
		else
			pElemNew = Create_SecondaryButton(iMid, pPanel->Rect_Pos.right - 5, pPanel->Rect_Pos.top + 5, pPanel->Rect_Pos.top + 5 + iSpacingFactor, "Str", FONT_STANDARD1, 1);
			
			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_SET_ACTIVEIGRID;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Set_ActiveIGrid(IBOX_CONSOLE_INFO, 2, &iretsize);
			pCmdNew->wDataSize = iretsize;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_CONSOLE_INFO].pIGrids[lc1].Add_IfcElem(pElemNew);

		//because "component" is a long string that may overrun at low resolutions, shorten it to "Cmp" if 800x600 and below)
		if (pCA->pDD_Display->iResProduct > 480000)
			pElemNew = Create_SecondaryButton(iMid, pPanel->Rect_Pos.right - 5, pPanel->Rect_Pos.top + 5 + iSpacingFactor, pPanel->Rect_Pos.top + 5 + (iSpacingFactor * 2), "Component", FONT_STANDARD1, 2);
		else
			pElemNew = Create_SecondaryButton(iMid, pPanel->Rect_Pos.right - 5, pPanel->Rect_Pos.top + 5 + iSpacingFactor, pPanel->Rect_Pos.top + 5 + (iSpacingFactor * 2), "Cmp", FONT_STANDARD1, 2);
			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_SET_ACTIVEIGRID;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Set_ActiveIGrid(IBOX_CONSOLE_INFO, 4, &iretsize);
			pCmdNew->wDataSize = iretsize;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
		IBoxes[IBOX_CONSOLE_INFO].pIGrids[lc1].Add_IfcElem(pElemNew);

		//"Sector" shorten to "Sctr" at low res
		if (pCA->pDD_Display->iResProduct > 480000)
			pElemNew = Create_SecondaryButton(pPanel->Rect_Pos.left + 5, iMid, pPanel->Rect_Pos.top + 5 + iSpacingFactor, pPanel->Rect_Pos.top + 5 + (iSpacingFactor * 2), "Sector", FONT_STANDARD1, 3);
		else
			pElemNew = Create_SecondaryButton(pPanel->Rect_Pos.left + 5, iMid, pPanel->Rect_Pos.top + 5 + iSpacingFactor, pPanel->Rect_Pos.top + 5 + (iSpacingFactor * 2), "Sctr", FONT_STANDARD1, 3);

			pCmdNew = new PCMD2;
			pCmdNew->wType = CQ2TYPE_SET_ACTIVEIGRID;
			pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Set_ActiveIGrid(IBOX_CONSOLE_INFO, 6, &iretsize);
			pCmdNew->wDataSize = iretsize;
			pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);	
		IBoxes[IBOX_CONSOLE_INFO].pIGrids[lc1].Add_IfcElem(pElemNew);
*/
	//add listbox
		pElemNew = Create_IfcListBox2(pPanel->Rect_Pos.left, pPanel->Rect_Pos.right, pPanel->Rect_Pos.top + iYOffset + 22, pPanel->Rect_Pos.bottom, pConsoleInfoListBox);
		int iX = pPanel->Rect_Pos.left;
		int iY = pPanel->Rect_Pos.top + iYOffset + 22;
		int iXSpan = pPanel->Rect_Pos.right - pPanel->Rect_Pos.left - 10;
		int iYSpan = pPanel->Rect_Pos.bottom - pPanel->Rect_Pos.top - iYOffset - 22;
		int iYSpanListing = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 1.5;
		int NumSlots = iYSpan / iYSpanListing;
		for (int lcy = 0; lcy < NumSlots; lcy++)
		{
			Add_ListBoxListing(pElemNew, lcy, iX + 5, iX + iXSpan + 5 - 10, iY + iYSpanListing * lcy + 5, iY + iYSpanListing * (lcy+1) + 5);
		}
			IBoxes[IBOX_CONSOLE_INFO].pIGrids[lc1].Add_IfcElem(pElemNew);

}

//grid 0: units
		cIfcElement* pElemNew = new cIfcElement;
			pElemNew->Init(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 22);
			cIfcElement* pElemNewChild = Create_IfcSecondaryPanel(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 16, 0);
				pElemNew->Add_IfcElem(pElemNewChild);
			pElemNewChild = Create_IfcSecondaryPanel(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5+5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16+5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5 + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 16 + 5, 1);
				pElemNew->Add_IfcElem(pElemNewChild);
			pElemNewChild = Create_StaticTextElem("Showing Units:", FONT_STANDARD1, pCA->pColorTable->White-1, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16+10, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5);
				pElemNew->Add_IfcElem(pElemNewChild);
			pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_SET_ACTIVEIGRID;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Set_ActiveIGrid(IBOX_CONSOLE_INFO, 1, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLDOWN, pCmdNew, FALSE);	
			pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_COLORFILL;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, 
																	pCA->pColorTable->MM_Players_Background[pCA->pGameOptions->iControl],
                                                                    pElemNew->Rect_Pos.left,
																	pElemNew->Rect_Pos.right,
																	pElemNew->Rect_Pos.top,
																	pElemNew->Rect_Pos.bottom,
																	&iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_CONSOLE_INFO].pIGrids[0].Add_IfcElem(pElemNew);
//grid 1: unit types
		pElemNew = new cIfcElement;
			pElemNew->Init(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 22);
			pElemNewChild = Create_IfcSecondaryPanel(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 16, 1);
				pElemNew->Add_IfcElem(pElemNewChild);
			pElemNewChild = Create_IfcSecondaryPanel(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5+5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16+5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5 + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 16 + 5, 0);
				pElemNew->Add_IfcElem(pElemNewChild);
			pElemNewChild = Create_StaticTextElem("Showing Types:", FONT_STANDARD1, pCA->pColorTable->White-1, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16+10, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5);
				pElemNew->Add_IfcElem(pElemNewChild);
			pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_SET_ACTIVEIGRID;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Set_ActiveIGrid(IBOX_CONSOLE_INFO, 0, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLDOWN, pCmdNew, FALSE);	
			pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_COLORFILL;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, 
																	pCA->pColorTable->MM_Players_Background[pCA->pGameOptions->iControl],
                                                                    pElemNew->Rect_Pos.left,
																	pElemNew->Rect_Pos.right,
																	pElemNew->Rect_Pos.top,
																	pElemNew->Rect_Pos.bottom,
																	&iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_CONSOLE_INFO].pIGrids[1].Add_IfcElem(pElemNew);
//grid 2: structures
		pElemNew = new cIfcElement;
			pElemNew->Init(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 22);
			pElemNewChild = Create_IfcSecondaryPanel(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 16, 0);
				pElemNew->Add_IfcElem(pElemNewChild);
			pElemNewChild = Create_IfcSecondaryPanel(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5+5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16+5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5 + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 16 + 5, 1);
				pElemNew->Add_IfcElem(pElemNewChild);
			pElemNewChild = Create_StaticTextElem("Showing Structs:", FONT_STANDARD1, pCA->pColorTable->White-1, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16+10, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5);
				pElemNew->Add_IfcElem(pElemNewChild);
			pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_SET_ACTIVEIGRID;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Set_ActiveIGrid(IBOX_CONSOLE_INFO, 3, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);
			pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_COLORFILL;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, 
																	pCA->pColorTable->MM_Players_Background[pCA->pGameOptions->iControl],
                                                                    pElemNew->Rect_Pos.left,
																	pElemNew->Rect_Pos.right,
																	pElemNew->Rect_Pos.top,
																	pElemNew->Rect_Pos.bottom,
																	&iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_CONSOLE_INFO].pIGrids[2].Add_IfcElem(pElemNew);
//grid 3: structure types
		pElemNew = new cIfcElement;
			pElemNew->Init(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 22);
			pElemNewChild = Create_IfcSecondaryPanel(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 16, 1);
				pElemNew->Add_IfcElem(pElemNewChild);
			pElemNewChild = Create_IfcSecondaryPanel(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5+5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16+5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5 + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 16 + 5, 0);
				pElemNew->Add_IfcElem(pElemNewChild);
			pElemNewChild = Create_StaticTextElem("Showing Types:", FONT_STANDARD1, pCA->pColorTable->White-1, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16+10, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5);
				pElemNew->Add_IfcElem(pElemNewChild);
			pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_SET_ACTIVEIGRID;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Set_ActiveIGrid(IBOX_CONSOLE_INFO, 2, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);	
			pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_COLORFILL;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, 
																	pCA->pColorTable->MM_Players_Background[pCA->pGameOptions->iControl],
                                                                    pElemNew->Rect_Pos.left,
																	pElemNew->Rect_Pos.right,
																	pElemNew->Rect_Pos.top,
																	pElemNew->Rect_Pos.bottom,
																	&iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_CONSOLE_INFO].pIGrids[3].Add_IfcElem(pElemNew);
//grid 4: components
		pElemNew = new cIfcElement;
			pElemNew->Init(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 22);
			pElemNewChild = Create_IfcSecondaryPanel(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 16, 0);
				pElemNew->Add_IfcElem(pElemNewChild);
			pElemNewChild = Create_IfcSecondaryPanel(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5+5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16+5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5 + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 16 + 5, 1);
				pElemNew->Add_IfcElem(pElemNewChild);
			pElemNewChild = Create_StaticTextElem("Showing Cmpnts:", FONT_STANDARD1, pCA->pColorTable->White-1, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16+10, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5);
				pElemNew->Add_IfcElem(pElemNewChild);
			pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_SET_ACTIVEIGRID;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Set_ActiveIGrid(IBOX_CONSOLE_INFO, 5, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);	
			pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_COLORFILL;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, 
																	pCA->pColorTable->MM_Players_Background[pCA->pGameOptions->iControl],
                                                                    pElemNew->Rect_Pos.left,
																	pElemNew->Rect_Pos.right,
																	pElemNew->Rect_Pos.top,
																	pElemNew->Rect_Pos.bottom,
																	&iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_CONSOLE_INFO].pIGrids[4].Add_IfcElem(pElemNew);
//grid 5: component types
		pElemNew = new cIfcElement;
			pElemNew->Init(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 22);
			pElemNewChild = Create_IfcSecondaryPanel(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 16, 1);
				pElemNew->Add_IfcElem(pElemNewChild);
			pElemNewChild = Create_IfcSecondaryPanel(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5+5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16+5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5 + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 16 + 5, 0);
				pElemNew->Add_IfcElem(pElemNewChild);
			pElemNewChild = Create_StaticTextElem("Showing Types:", FONT_STANDARD1, pCA->pColorTable->White-1, IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 16+10, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5);
				pElemNew->Add_IfcElem(pElemNewChild);
			pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_SET_ACTIVEIGRID;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Set_ActiveIGrid(IBOX_CONSOLE_INFO, 4, &iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONLUP, pCmdNew, FALSE);	
			pCmdNew = new PCMD2;
				pCmdNew->wType = CQ2TYPE_COLORFILL;
				pCmdNew->pData = pCA->pCommQueue->Assemble_CommData_Colorfill(RTC_SECONDARY, 
																	pCA->pColorTable->MM_Players_Background[pCA->pGameOptions->iControl],
                                                                    pElemNew->Rect_Pos.left,
																	pElemNew->Rect_Pos.right,
																	pElemNew->Rect_Pos.top,
																	pElemNew->Rect_Pos.bottom,
																	&iretsize);
				pCmdNew->wDataSize = iretsize;
				pElemNew->Add_Cmd(IFC_ELEM_CAT_ONHOVER_DRAW, pCmdNew, FALSE);
		IBoxes[IBOX_CONSOLE_INFO].pIGrids[5].Add_IfcElem(pElemNew);

//grid 6: sectors
		pElemNew = new cIfcElement;
			pElemNew->Init(IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.right - 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 22);
			pElemNewChild = Create_StaticTextElem("Showing Sectors:", FONT_STANDARD1, pCA->pColorTable->White-1,IBoxes[IBOX_CONSOLE_INFO].BoxPos.left + 5, IBoxes[IBOX_CONSOLE_INFO].BoxPos.top + iYOffset + 5);
				pElemNew->Add_IfcElem(pElemNewChild);
		IBoxes[IBOX_CONSOLE_INFO].pIGrids[6].Add_IfcElem(pElemNew);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

/* obsolete code 08.09.2003
	if (pConsole_Unit_Select != NULL)
	{

		cUnitA* pUnit = pCA->pUnitTable->pUnitTable[LPSBE(pConsole_Unit_Select)->iID];
		int c3 = 0;
		for (c3 = 0; c3 < pUnit->iiNumSupportedCommands; c3++)
		{

			//Set_IGridCell_DrawFlags(&IGrid.pGrid[c3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0,0, pCA->pCmdInfoTable->Names[pUnit->piSupportedCommands[c3]], FONT_STANDARD1);
			//Set_IGridCell_CmdFlags(&IGrid.pGrid[c3], TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_START_PENDINGCOMMAND, pUnit->piSupportedCommands[c3], 0, 0, 0, 0, NULL, 0);
				Set_IGridCell_AllFlags(&IGrid.pGrid[c3], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0,
										pCA->pCmdInfoTable->Names[pUnit->piSupportedCommands[c3]], FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_START_PENDINGCOMMAND, pUnit->piSupportedCommands[c3], 0, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);	
		}
		for (c3 = pUnit->iiNumSupportedCommands; c3 < IGrid.iElements_y; c3++)
			Set_IGridCell_DrawFlags(&IGrid.pGrid[c3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0,0, " ", FONT_STANDARD1);
			Set_IGridCell_CmdFlags(&IGrid.pGrid[c3], FALSE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_START_PENDINGCOMMAND, pUnit->piSupportedCommands[c3], 0, 0, 0, 0, NULL, 0);

	}
		else
		{
			for (int c3 = 0; c3 < IGrid.iElements_y; c3++)
				Set_IGridCell_DrawFlags(&IGrid.pGrid[c3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0,0, " ", FONT_STANDARD1);
				Set_IGridCell_CmdFlags(&IGrid.pGrid[c3], FALSE, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0);
		}
*/
/*
//DEBUG
Write_Log("[Process_Console] Listing components attached to selected unit.");
WriteLog_Enter();

//if unit select isn't NULL, list all components on it
	if (pConsole_Unit_Select != NULL)
	{
		//cIGrid IGrid = pIBox->pIGrids[0];
		cIGrid* pIGrid = &pIBox->pIGrids[0]; //TEST
		pIBox->iActiveGrid = 0;
		LPSBE pSBE = (LPSBE)pConsole_Unit_Select;
		cUnitA* pUnit = pCA->pUnitTable->pUnitTable[pSBE->iID];
		cComponentA* pCmpTrav = (cComponentA*)pUnit->CmpAttached.pCmpList_First->pNext_list;
		

		//set scrollbar if number of attached components exceeds number of available spaces
		if (pUnit->CmpAttached.iNumComponents > pIGrid->iElements_y)
		{
			 pIGrid->Set_Scrollbar(TRUE, -1, pUnit->CmpAttached.iNumComponents - pIGrid->iElements_y, 0);
		} else
			 pIGrid->Set_Scrollbar(FALSE, 0, 1, 0);

		//scroll forward that number of entries
		for (int c22 = 0; c22 < pIGrid->iScrollVal; c22++)
		if (pCmpTrav != pUnit->CmpAttached.pCmpList_Last)
			pCmpTrav = (cComponentA*)pCmpTrav->pNext_list;

		int c2 = 0;
		while (c2 < pIGrid->iElements_y && pCmpTrav != pUnit->CmpAttached.pCmpList_Last)
		{
			LPSTR Name = pCA->pCmpInfoTable->pCmpTable[pCmpTrav->iType]->Name;
			Set_IGridCell_DrawFlags(&pIGrid->pGrid[c2], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, Name, FONT_STANDARD1);
			//populate the object reference fields of the grid cell
			VOID** ppObjRefs = new void*;
			*ppObjRefs = (VOID*)pCmpTrav;
			DWORD* pdwID = new DWORD; 
			*pdwID = pCmpTrav->iID;
			Set_IGridCell_ObjRefFlags(&pIGrid->pGrid[c2], 1, OBJTYPE_COMPONENT, ppObjRefs, pdwID); 
			pCmpTrav = (cComponentA*)pCmpTrav->pNext_list;
			c2++;
		}
		int c3;
		for(c3 = c2; c3 < pIGrid->iElements_y; c3++)
		{
			Set_IGridCell_DrawFlags(&pIGrid->pGrid[c3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, " ", FONT_STANDARD1);
		}
	}
	else //otherwise clear everything on it
	{
		int c3;
		cIGrid* pIGrid = &pIBox->pIGrids[0];
		for (c3 = 0; c3 < pIGrid->iElements_y; c3++)
		{
			Set_IGridCell_DrawFlags(&pIGrid->pGrid[c3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, " ", FONT_STANDARD1);
		}

	}

//DEBUG
Write_Log("[Process_Console] Listing units selected in CONSOLE_LIST box.");
WriteLog_Enter();

pIBox = &IBoxes[IBOX_CONSOLE_LIST];

int iDrawBlankFlag = 1;
	
	
	
//for the moment, list units/structures selected in the LIST box
	if (pUSB_First->pNext != (void*)pUSB_Last)
	{
		cIGrid* pIGrid = &pIBox->pIGrids[0];
		pIBox->iActiveGrid = 0;		
		LPSBE pSBE = (LPSBE)pUSB_First->pNext;		
		//
		int iNumUnits = 0;
		//the way the "list units" loop was coded does not let us know how many units
		//can be listed, so we that we cannot make a scroll bar.
		//consequently, we have to go through a "pre-pass" to get the number of listings
		while (pSBE != pUSB_Last)
		{
			cUnitA* pUnit = pCA->pUnitTable->pUnitTable[pSBE->iID];

			if (pUnit != NULL)
				iNumUnits++;

			pSBE = (LPSBE)pSBE->pNext;
		}

		int iNumRows = iNumUnits / pIGrid->iElements_x;
		if (iNumUnits % pIGrid->iElements_x != 0)
			iNumRows++;

		//now we decide whether we need a scrollbar (07.27.2004)
		if (iNumRows > pIGrid->iElements_y)
			pIGrid->Set_Scrollbar(TRUE, -1, iNumRows - pIGrid->iElements_y, 0);
				else
			pIGrid->Set_Scrollbar(FALSE, 0, 1, 0);

		pSBE = (LPSBE)pUSB_First->pNext;
		//scroll through the prescribed number of entries
		for (int c23 = 0; c23 < pIGrid->iScrollVal; c23++)
		for (int c24 = 0; c24 < pIGrid->iElements_x; c24++)
		if (pSBE != pUSB_Last)
		{
			pSBE = (LPSBE)pSBE->pNext;
		}

		iDrawBlankFlag = 0; //to prevent drawing over everything with a blank
		int c2 = 0;

		while (c2 < (pIGrid->iElements_y * pIGrid->iElements_x) && pSBE != pUSB_Last)
		{
//DEBUG
Write_Log("[Process_Console] [Listing Units] Beginning list of unit ");
itoa(pSBE->iID, NText, 10);
Write_Log(NText);
WriteLog_Enter();	
			
			LPSTR Name;
			LPSTR LongName;
			cUnitA* pUnit = pCA->pUnitTable->pUnitTable[pSBE->iID];
			 
			if (pUnit != NULL)
			{
//DEBUG
Write_Log("Name is: ");
				Name = pCA->pUnitTable->pUnitTable[pSBE->iID]->ShortName;
				LongName = pCA->pUnitTable->pUnitTable[pSBE->iID]->Name;
//DEBUG
Write_Log(Name);
WriteLog_Enter();
			}
					else
					{
						Write_Log("ERROR: Attempting to list a destroyed unit.");
						WriteLog_Enter();
					}
			//HACK...just to check  07.30.2003
			if (pConsole_Unit_Select == NULL)
				pConsole_Unit_Select = (void*)pSBE;


			if (pSBE->iID == ((LPSBE)pConsole_Unit_Select)->iID)
			{
//DEBUG
Write_Log("[Process_Console] [Listing Units] Setting main flags + selected icon of Grid Cell.");
itoa(c2, NText, 10);
Write_Log(NText);
WriteLog_Enter();
				Set_IGridCell_AllFlags(&pIGrid->pGrid[c2], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_BLITTEXT, ISURF_EMBLEMS, 0, 0, 8, 8 * pUnit->iControl, 8 * (pUnit->iControl + 1),
										Name, FONT_STANDARD1, CQDEST_INTERFACE, CQTYPE_SET_CONSOLE_UNIT, 0, 0, 0, 
										0, 0, pSBE, pCA->pTimer->iCurrTick + pCA->pGameOptions->iLatency, 0, 0, NULL, NULL);

//DEBUG
Write_Log("[Process_Console] [Listing Units] Setting objref flags of Grid Cell.");
Write_Log(NText);
WriteLog_Enter();
			//populate the object reference fields of the grid cell
			VOID** ppObjRefs = new VOID*;
			*ppObjRefs = (VOID*)pUnit;
			DWORD* pdwID = new DWORD;
			*pdwID = pUnit->iID;
			Set_IGridCell_ObjRefFlags(&pIGrid->pGrid[c2], 1, OBJTYPE_UNIT, ppObjRefs, pdwID); 

					double HR = pUnit->iHP / double(pUnit->iHPMax);
					int iG = pCA->pColorTable->Det_HPGreenCmp(HR);
					int iR = pCA->pColorTable->Det_HPRedCmp(HR);
					pIGrid->pGrid[c2].ColorB = 0;
					pIGrid->pGrid[c2].ColorG = iG;
					pIGrid->pGrid[c2].ColorR = iR;
	
//DEBUG
Write_Log("[Process_Console] [Listing Units] Completed setting cell flags.");
WriteLog_Enter();			

			}
					else
					{
//DEBUG
Write_Log("[Process_Console] [Listing Units] Setting main flags of Grid Cell.");
itoa(c2, NText, 10);
Write_Log(NText);
WriteLog_Enter();
					Set_IGridCell_AllFlags(&pIGrid->pGrid[c2], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, ISURF_EMBLEMS, 0, 0, 8, 0, 8,
										Name, FONT_STANDARD1, CQDEST_INTERFACE, CQTYPE_SET_CONSOLE_UNIT, 0, 0, 0, 
										0, 0, pSBE, pCA->pTimer->iCurrTick + pCA->pGameOptions->iLatency, 0, 0, NULL, NULL);			
					
//DEBUG
Write_Log("[Process_Console] [Listing Units] Setting objref flags of Grid Cell.");
Write_Log(NText);
WriteLog_Enter();
					//populate the object reference fields of the grid cell
					VOID** ppObjRefs = new VOID*; 
					*ppObjRefs = (VOID*)pUnit;
					DWORD* pdwID = new DWORD;
					*pdwID = pUnit->iID;
					Set_IGridCell_ObjRefFlags(&pIGrid->pGrid[c2], 1, OBJTYPE_UNIT, ppObjRefs, pdwID); 

					double HR = pUnit->iHP / double(pUnit->iHPMax);
					int iG = pCA->pColorTable->Det_HPGreenCmp(HR);
					int iR = pCA->pColorTable->Det_HPRedCmp(HR);
					pIGrid->pGrid[c2].ColorB = 0;
					pIGrid->pGrid[c2].ColorG = iG;
					pIGrid->pGrid[c2].ColorR = iR;

					
//DEBUG
Write_Log("[Process_Console] [Listing Units] Completed setting cell flags.");
WriteLog_Enter();
					}
				pSBE = (LPSBE)pSBE->pNext;
			c2++;
		}	
			

		int c3;
		for(c3 = c2; c3 < (pIGrid->iElements_y * pIGrid->iElements_x); c3++)
		{
				Set_IGridCell_AllFlags(&pIGrid->pGrid[c3], TRUE, FALSE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 8, 0, 8,
										" ", FONT_STANDARD1, 0, 0, 0, 0, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);			
		}
	}
	else
	{
			
		int c3;
		cIGrid* pIGrid = &pIBox->pIGrids[0];
		pIBox->iActiveGrid = 0;	
		for (c3 = 0; c3 < pIGrid->iElements_y; c3++)
		{
				Set_IGridCell_AllFlags(&pIGrid->pGrid[c3], TRUE, FALSE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 8, 0, 8,
										" ", FONT_STANDARD1, CQDEST_INTERFACE, 0, 0, 0, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);			
		}

		

	}
//DEBUG
Write_Log("[Process_Console] Listing stuctures selected in CONSOLE_LIST box.");
WriteLog_Enter();

//now fill the structures box
//because I am pretty sure that as far as Project SamCraft goes, we will only be able to select one structure at a time
//the purpose of this box is pretty pointless. Consequently, this grid will be used for showing available unit/component types
//if a BUILD_UNIT_BY_TYPE or a BUILD_COMPONENT pending command active, this menu will show all buildable component/unit types.
//the current implementation will be partially TEMPORARY (08.09.2003)
//modified 10.11.2003: to add BUILD_STRUCTURE options too



	if (pPendingCommand != NULL)
	{
		//10.11.2003
		if (pPendingCommand->pCommand2->wType == CQ2TYPE_BUILD_STRUCTURE && pPendingCommand->iCurrentSequenceNumber == 1)
		{
			int lc49 = 2;

			cIGrid* pIGrid = &pIBox->pIGrids[1];
			pIBox->iActiveGrid = 1;	

			//as before, we need to find out whether there are too many entries to fit in the box
			int iNumTypes = 0;
			for (lc50 = 0; lc50 < pCA->pStructInfoTable->iHighestTypeID + 1; lc50++)
			{
				if (pCA->pStructInfoTable->pTable[lc50] != NULL)
					iNumTypes++;
			}
			int lc35 = 0;
			//decide if we need a scrollbar
			if (iNumTypes > pIGrid->iElements_y)
			{
				pIGrid->Set_Scrollbar(TRUE, -1, iNumTypes - pIGrid->iElements_y, 0);

				int lc34 = 0;

				//and then we need to move up to the starting entry to start listing
				for (lc35 = 0; lc35 < pCA->pStructInfoTable->iHighestTypeID + 1; lc35++)
				{
					if (pCA->pStructInfoTable->pTable[lc35] != NULL)
						lc34++;
					if (lc34 == pIGrid->iScrollVal)
						break;
				}
			}
					else
				pIGrid->Set_Scrollbar(FALSE, 0, 1, 0);

			for (lc50 = lc35; lc50 < pCA->pStructInfoTable->iHighestTypeID + 1; lc50++)
			if (lc49 < pIGrid->iElements_y)
			if (pCA->pStructInfoTable->pTable[lc50] != NULL)
			{
				
				Set_IGridCell_DrawFlags(&pIGrid->pGrid[lc49], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, pCA->pStructInfoTable->pTable[lc50]->GeneralName, FONT_STANDARD1);
				DWORD* pdwType = new DWORD;
				*pdwType = lc50;
				Set_IGridCell_ObjRefFlags(&pIGrid->pGrid[lc49], 1, OBJTYPE_STYPE, NULL, pdwType);
				lc49++;
			}
			iDrawBlankFlag = 0;
		}



		if (pPendingCommand->pCommand2->wType == CQ2TYPE_START_BUILD_UNIT_CUSTOM && pPendingCommand->iCurrentSequenceNumber == 1)
		{

			//sort of a HACK for now, but enable the Interface PacketProcessor (type CTYPE) and if CutOffTime has not been set,
			//set it
			iScanType = OBJTYPE_CTYPE;
			if (llCutOffTime == -6996)
			{
				llCutOffTime = pCA->pTimer->iCurrTick;
				//also use "time has not been set" to do an initial "help-set" of the info menu to the ctypes tab
				IBoxes[IBOX_CONSOLE_INFO].iActiveGrid = 1; //the CTYPES grid
				PacketProcessor.Flush_Buffer2(PacketProcessor.pIPB_First, PacketProcessor.pIPB_Last);
			}
			//and cull the buffer too
			PacketProcessor.Cull_ComponentPackets_By_Class(PacketProcessor.pIPB_First, PacketProcessor.pIPB_Last, TRUE);

			cCmpInfo* pCmpInfo;
			cComponentA* pCmp;

			cIGrid* pIGrid = &pIBox->pIGrids[1];
			pIBox->iActiveGrid = 1;	
			//display all the component types in the PacketProcessor
			INPUTPACKET* pIPB_Trav = (INPUTPACKET*)PacketProcessor.pIPB_First->pNext;

			//as before, we need to find out whether there are too many entries to fit in the box
			int iNumEntries = 0;
			while(pIPB_Trav != (INPUTPACKET*)PacketProcessor.pIPB_Last)
			{
				if (pIPB_Trav->dwObjectsType == OBJTYPE_COMPONENT || pIPB_Trav->dwObjectsType == OBJTYPE_CTYPE)
					iNumEntries++;
					
				pIPB_Trav = (INPUTPACKET*)pIPB_Trav->pNext;
			}
			//reset
			pIPB_Trav = (INPUTPACKET*)PacketProcessor.pIPB_First->pNext;
			int lc35 = 0;
			//decide if we need a scrollbar (notice that we don't start until entry 3 for this table)
			if (iNumEntries > pIGrid->iElements_y - 3)
			{
				pIGrid->Set_Scrollbar(TRUE, -1, iNumEntries - (pIGrid->iElements_y - 3), 0);

				int lc34 = 0;

				//and then we need to move up to the starting entry to start listing
				while (pIPB_Trav != PacketProcessor.pIPB_Last)
				{

					if (lc34 >= pIGrid->iScrollVal)
						break;

					if (pIPB_Trav->dwObjectsType == OBJTYPE_COMPONENT || pIPB_Trav->dwObjectsType == OBJTYPE_CTYPE)
					{
						lc34++;
					}
					pIPB_Trav = (INPUTPACKET*)pIPB_Trav->pNext;


				}

			}
					else
				pIGrid->Set_Scrollbar(FALSE, 0, 1, 0);	

			//clear entries
			for (int lc57 = 0; lc57 < pIGrid->iElements_y; lc57++)
			{
				Set_IGridCell_DrawFlags(&pIGrid->pGrid[lc57], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, " ", FONT_STANDARD1);
			}

			int lc53 = 0; //local counter

			//set the top line to the PROCEED button (08.20.2003)
					Set_IGridCell_AllFlags(&pIGrid->pGrid[lc53], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										"<PROCEED>", FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_DISABLE_ISSUE_PP, OBJTYPE_CTYPEGROUP, 0, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);		

			lc53+= 1;

			//set the next one to "CLEAR ENTRIES"
					Set_IGridCell_AllFlags(&pIGrid->pGrid[lc53], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										"<CLEAR ENTRIES>", FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_FLUSH_PP, 0, 0, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);
			lc53+=2;

			while (pIPB_Trav != PacketProcessor.pIPB_Last)
			{
				//get the type name				
				if (pIPB_Trav->dwObjectsType == OBJTYPE_COMPONENT)
				{
					pCmp = pCA->pCmpIDTable->pTable[pIPB_Trav->pdwIDs[0]];
					pCmpInfo = pCA->pCmpInfoTable->pCmpTable[pCmp->iType];
					if (lc53 < pIGrid->iElements_y)
						Set_IGridCell_DrawFlags(&pIGrid->pGrid[lc53], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, pCmpInfo->Name, FONT_STANDARD1);
					lc53++;
				}
				if (pIPB_Trav->dwObjectsType == OBJTYPE_CTYPE)
				{
					pCmpInfo = pCA->pCmpInfoTable->pCmpTable[pIPB_Trav->pdwIDs[0]];
					if (lc53 < pIGrid->iElements_y)					
						Set_IGridCell_DrawFlags(&pIGrid->pGrid[lc53], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, pCmpInfo->Name, FONT_STANDARD1);
					lc53++;
				}
				pIPB_Trav = (INPUTPACKET*)pIPB_Trav->pNext;
			}

			//then clear the rest of the entries
			for (lc53 = lc53; lc53 < pIGrid->iElements_y; lc53++)
			{
				Set_IGridCell_DrawFlags(&pIGrid->pGrid[lc53], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, " ", FONT_STANDARD1);
			}

			iDrawBlankFlag = 0;
		}
		if (pPendingCommand->pCommand2->wType == CQ2TYPE_START_BUILD_COMPONENT && pPendingCommand->iCurrentSequenceNumber == 1)
		{
			//almost the same buildability issues as units, so we are for now only going to list all listed types for now
			//08.12.2003
			cIGrid* pIGrid = &pIBox->pIGrids[1];
			pIBox->iActiveGrid = 1;	

			//as before, we need to find out whether there are too many entries to fit in the box
			int iNumTypes = 0;
			for (lc50 = 0; lc50 < pCA->pCmpInfoTable->iHighestTypeID + 1; lc50++)
			{
				if (pCA->pCmpInfoTable->pCmpTable[lc50] != NULL)
					iNumTypes++;
			}

			int lc35 = 0;
			//decide if we need a scrollbar
			if (iNumTypes > pIGrid->iElements_y)
			{
				pIGrid->Set_Scrollbar(TRUE, -1, iNumTypes - pIGrid->iElements_y, 0);

				int lc34 = 0;

				//and then we need to move up to the starting entry to start listing
				for (lc35 = 0; lc35 < pCA->pCmpInfoTable->iHighestTypeID + 1; lc35++)
				{
					if (lc34 == pIGrid->iScrollVal)
						break;
					if (pCA->pCmpInfoTable->pCmpTable[lc35] != NULL)
						lc34++;

				}
			}
					else
				pIGrid->Set_Scrollbar(FALSE, 0, 1, 0);

			int lc51 = 0; //local counter
			int lc52 = 0;

			for (lc51 = lc35; lc51 < pCA->pCmpInfoTable->iHighestTypeID + 1; lc51++)
			if (lc52 < pIGrid->iElements_y)
			if (pCA->pCmpInfoTable->pCmpTable[lc51] != NULL)
			{
				
				Set_IGridCell_DrawFlags(&pIGrid->pGrid[lc52], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, pCA->pCmpInfoTable->pCmpTable[lc51]->Name, FONT_STANDARD1);
				DWORD* pdwType = new DWORD;
				*pdwType = lc51;
				Set_IGridCell_ObjRefFlags(&pIGrid->pGrid[lc52], 1, OBJTYPE_CTYPE, NULL, pdwType);
				lc52 += pIGrid->iElements_x;
			}


			iDrawBlankFlag = 0;
		}

	}   //if no build unit order is found, and a structure is selected with a usable OrderQueue, pull up the format screen for that (08.12.2003)
	else if (pSSB_First->pNext != (void*)pSSB_Last)
	{
		LPSBE pEntry = (LPSBE)pSSB_First->pNext;
		cStructureA* pStruct = pCA->pStructIDTable->pTable[pEntry->iID];
		cStructInfo* pStructInfo = pCA->pStructInfoTable->pTable[pStruct->iType];

		if (pStructInfo->bActiveOrderQueue == TRUE)
		{
			pIBox->iActiveGrid = 2;
			cIGrid* pIGrid = &pIBox->pIGrids[2];
			//for now, for lack of a "status bar" graphic, write up the orders queued and fCompletion of the top one
			Set_IGridCell_DrawFlags(&pIGrid->pGrid[pIGrid->iElements_x], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, "BUILD/REPAIR ORDERS:", FONT_STANDARD1);
			
			int lc1 = 2; // local counter
			cProgressiveOrder* pPO = (cProgressiveOrder*)pStruct->OrderQueue.pPO_First->pNext;

			//as before, we need to find out whether there are too many entries to fit in the box
			int iNumEntries = 0;
			while(pPO != pStruct->OrderQueue.pPO_Last)
			{
					iNumEntries++;
		
				pPO = (cProgressiveOrder*)pPO->pNext;
			}
			//reset
			pPO = (cProgressiveOrder*)pStruct->OrderQueue.pPO_First->pNext;
			int lc35 = 0;
			//decide if we need a scrollbar
			if (iNumEntries > (pIGrid->iElements_y - 3))
			{
				pIGrid->Set_Scrollbar(TRUE, -1, iNumEntries - (pIGrid->iElements_y - 3), 0);

				int lc34 = 0;

				//and then we need to move up to the starting entry to start listing
				for (lc35 = 0; lc35 < pIGrid->iScrollVal; lc35++)
				{
					pPO = (cProgressiveOrder*)pPO->pNext;
				}

			}
					else
				pIGrid->Set_Scrollbar(FALSE, 0, 1, 0);

			while (pPO != pStruct->OrderQueue.pPO_Last && lc1 < pIGrid->iElements_y)
			{

				//turn it into percentage points
				gcvt(pPO->fCompletion * 100, 3, NText);
				strcat(NText, "%");

				
				Set_IGridCell_DrawFlags(&pIGrid->pGrid[lc1 * pIGrid->iElements_x], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, pPO->pName, FONT_STANDARD1);
				Set_IGridCell_DrawFlags(&pIGrid->pGrid[lc1 * pIGrid->iElements_x + 3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, NText, FONT_STANDARD1);

				lc1++;

				pPO = (cProgressiveOrder*)pPO->pNext;
			}
			//now blank out the rest of the entries
			while (lc1 < pIGrid->iElements_y)
			{
				Set_IGridCell_DrawFlags(&pIGrid->pGrid[lc1 * pIGrid->iElements_x], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, " ", FONT_STANDARD1);
				Set_IGridCell_DrawFlags(&pIGrid->pGrid[lc1 * pIGrid->iElements_x + 3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, " ", FONT_STANDARD1);
				lc1++;
			}

			iDrawBlankFlag = 0;
		}

	} 

	if (iDrawBlankFlag == 1)
	{
		int c3;
		cIGrid* pIGrid = &pIBox->pIGrids[1];
		pIBox->iActiveGrid = 1;	
		pIGrid->Set_Scrollbar(FALSE, 0, 1, 0);
		for (c3 = 0; c3 < pIGrid->iElements_y; c3++)
		{
			Set_IGridCell_DrawFlags(&pIGrid->pGrid[c3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, " ", FONT_STANDARD1);
		}
	}


/*	if (pSSB_First->pNext != (void*)pSSB_Last)
	{
		cIGrid IGrid = pIBox->pIGrids[1];
		pIBox->iActiveGrid = 1;
		LPSBE pSBE = (LPSBE)pSSB_First->pNext;		

		int c2 = 1;

		while (c2 < IGrid.iElements_y && pSBE != pSSB_Last)
		{
			LPSTR Name = pCA->pStructIDTable->pTable[pSBE->iID]->Name;
			cStructureA* pStruct =  pCA->pStructIDTable->pTable[pSBE->iID];
			Set_IGridCell_DrawFlags(&IGrid.pGrid[c2], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, Name, FONT_STANDARD1);
			//populate the object reference fields of the grid cell
			VOID** ppObjRefs = new VOID*;
			*ppObjRefs = (VOID*)pStruct;
			DWORD* pdwID = new DWORD;
			*pdwID = pStruct->iID;
			Set_IGridCell_ObjRefFlags(&IGrid.pGrid[c2], 1, OBJTYPE_STRUCTURE, ppObjRefs, pdwID); 
			pSBE = (LPSBE)pSBE->pNext;
			c2++;
		}	
		int c3;
		for(c3 = c2; c3 < IGrid.iElements_y; c3++)
		{
			Set_IGridCell_DrawFlags(&IGrid.pGrid[c3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, " ", FONT_STANDARD1);
		}
	}
	else
	{
		int c3;
		cIGrid IGrid = pIBox->pIGrids[1];
		for (c3 = 0; c3 < IGrid.iElements_y; c3++)
		{
			Set_IGridCell_DrawFlags(&IGrid.pGrid[c3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, " ", FONT_STANDARD1);
		}
	}
*/
/*/*
//DEBUG
Write_Log("[Process_Console] Listing available commands in CONSOLE_COMMAND box.");
WriteLog_Enter();

//temporary setup just to see something. fill the command box with commands if either the console select or unit select is active
	//05.31.2003)
pIBox = &IBoxes[IBOX_CONSOLE_COMMAND];
cIGrid* pIGrid = &pIBox->pIGrids[0];
//NOTE: this grid isn't simply one column, so keep in mind the number of columns (06.03.2003)
int iNumEntries = pIGrid->iElements_x * pIGrid->iElements_y;
//if there are units in the buffer, begin processing (the more "real" implementation, 06.01.2003)
//and there is NO pending command
//NOTE: only enable command issue if the units in the buffer are friendly
BOOL bDispCommFlag = FALSE;
	if (pUSB_First->pNext != pUSB_Last && pPendingCommand == NULL)
	{
		cUnitA* pUnit = pCA->pUnitTable->pUnitTable[ ((LPSBE)pUSB_First->pNext)->iID ];

		if (pUnit != NULL)
		if (pCA->pUnitTable->pUnitTable[ ((LPSBE)pUSB_First->pNext)->iID ]->iControl == pCA->pGameOptions->iControl)
		{

			bDispCommFlag = TRUE;

		}
	
	}


	if (bDispCommFlag == TRUE)
	{

		BYTE AllComms[NUM_CQ2COMMANDS];
		ZeroMemory(AllComms, sizeof(BYTE)*NUM_CQ2COMMANDS);
		int NumUnits = 0;

		LPSBE pSBE_Trav = (LPSBE)pUSB_First->pNext;

		//accumulate data
		while (pSBE_Trav != pUSB_Last)
		{
			
			cUnitA* pUnit2 = pCA->pUnitTable->pUnitTable[pSBE_Trav->iID];

			for (int lc3 = 0; lc3 < pUnit2->iiNumSupportedCommands; lc3++)
			{
				AllComms[ pUnit2->piSupportedCommands[lc3] ]++;
			}

			NumUnits++;

			pSBE_Trav = (LPSBE)pSBE_Trav->pNext;
		}

		//initialize the grid
			for (int c3 = 0; c3 < iNumEntries; c3++)
			{
				Set_IGridCell_DrawFlags(&pIGrid->pGrid[c3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0,0, " ", FONT_STANDARD1);
				Set_IGridCell_CmdFlags(&pIGrid->pGrid[c3], FALSE, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0);
			}


		//decide whether a scrollbar is necessary or not
		int iNumComms = 0;
		int lc22 = 0;
		for (int lc21 = 0; lc21 < NUM_CQ2COMMANDS; lc21++)
		{
			if (AllComms[lc21] == NumUnits)
			{
				iNumComms++;
			}
		}
		if (iNumComms > pIGrid->iElements_x * pIGrid->iElements_y)
		{
			pIGrid->Set_Scrollbar(TRUE, -1, iNumComms - (pIGrid->iElements_x * pIGrid->iElements_y), 0);

				for (int lc21 = 0; lc21 < NUM_CQ2COMMANDS; lc21++)
				{
					if (AllComms[lc21] == NumUnits)
						lc22++;
					if (lc22 >= pIGrid->iScrollVal)
						break;
				}
		} else
			{
				pIGrid->Set_Scrollbar(FALSE, 0, 1, 0);
			}

		int lc5 = 0;
		//only list the commands that all units in the list support
		for (int lc4 = lc22; lc4 < NUM_CQ2COMMANDS; lc4++)
		{
			if (AllComms[lc4] == NumUnits && lc5 < pIGrid->iElements_y * pIGrid->iElements_x)
			{
				Set_IGridCell_AllFlags(&pIGrid->pGrid[lc5], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_BLITFIT, ISURF_CMDBUTTONS, 0, 
										pCA->pCmdInfoTable->SourceButtonFrameRects[lc4].left, 
										pCA->pCmdInfoTable->SourceButtonFrameRects[lc4].right, 
										pCA->pCmdInfoTable->SourceButtonFrameRects[lc4].top, 
										pCA->pCmdInfoTable->SourceButtonFrameRects[lc4].bottom,
										NULL, FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_START_PENDINGCOMMAND, lc4, 0, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);				
				lc5++;
			}

		}

	}

	//a separate command displaying function is needed for structures simply due to the differing data type and organization :-( (08.09.2003)
BOOL bDispStructCommFlag = FALSE;
		LPSBE pSBE_Trav = (LPSBE)pUSB_First->pNext;
//DEBUG
Write_Log("[Process_Console] Listing available commands FOR structures in CONSOLE_COMMAND box.");
WriteLog_Enter();

cStructureA* pStruct = NULL;
	if (pSSB_First->pNext != pSSB_Last && pPendingCommand == NULL)
	{	

		pStruct = pCA->pStructIDTable->pTable[ ((LPSBE)pSSB_First->pNext)->iID ];

		//added 06.18.2004 only allow commands if structure is COMPLETED(the other way to implement this functionality
		//more robustly would be to modify the supported commands on completion, but this way is easier for now, so it 
		//will stick until there is a need to do it the other way
		if (pStruct->iControl == pCA->pGameOptions->iControl && pStruct->fCompletion >= 1.0)
		{
			bDispStructCommFlag = TRUE;
		}
	}
	if (bDispStructCommFlag == TRUE)
	{

		//thankfully because there will only be one structure in the SB at a time, we can simply display the supported commands
	
		//get structure pointer
		pStruct = pCA->pStructIDTable->pTable[ ((LPSBE)pSSB_First->pNext)->iID ];

		//we can decide immediately whether a scrollbar is necessary or not
		if (pStruct->iNumSupportedCommands > pIGrid->iElements_y * pIGrid->iElements_x)
		{
			pIGrid->Set_Scrollbar(TRUE, -1, pStruct->iNumSupportedCommands - (pIGrid->iElements_y * pIGrid->iElements_x), 0);
		} else
			pIGrid->Set_Scrollbar(FALSE, 0, 1, 0);

		//initialize the grid
		int c3; //counter
			for (c3 = 0; c3 < iNumEntries; c3++)
			{
				Set_IGridCell_DrawFlags(&pIGrid->pGrid[c3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0,0, " ", FONT_STANDARD1);
				Set_IGridCell_CmdFlags(&pIGrid->pGrid[c3], FALSE, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0);
			}
		//set the grid
			for (c3 = pIGrid->iScrollVal; c3 < pStruct->iNumSupportedCommands; c3++)
			{
				if (c3 < iNumEntries)
				Set_IGridCell_AllFlags( &pIGrid->pGrid[c3], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_BLITFIT, ISURF_CMDBUTTONS, 0, 
										pCA->pCmdInfoTable->SourceButtonFrameRects[pStruct->pSupportedCommands[c3]].left, 
										pCA->pCmdInfoTable->SourceButtonFrameRects[pStruct->pSupportedCommands[c3]].right, 
										pCA->pCmdInfoTable->SourceButtonFrameRects[pStruct->pSupportedCommands[c3]].top, 
										pCA->pCmdInfoTable->SourceButtonFrameRects[pStruct->pSupportedCommands[c3]].bottom,
										NULL, FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_START_PENDINGCOMMAND, pStruct->pSupportedCommands[c3], 0, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);
			}

	}


	
	//blank screen if neither passes
	if (bDispCommFlag == FALSE && bDispStructCommFlag == FALSE)
		{
			for (int c3 = 0; c3 <  iNumEntries; c3++)
			{
				Set_IGridCell_DrawFlags(&pIGrid->pGrid[c3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0,0, " ", FONT_STANDARD1);
				Set_IGridCell_CmdFlags(&pIGrid->pGrid[c3], FALSE, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0);
			}
		}


	//not exactly the most efficient if-sorting procedure 06.03.2003
	if (pPendingCommand == NULL)
	{
			pIBox->iActiveGrid = 0;	
	}
	if (pPendingCommand != NULL)
	{


			for (int c3 = 0; c3 <  iNumEntries; c3++)
			{
				Set_IGridCell_DrawFlags(&pIGrid->pGrid[c3], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0,0, " ", FONT_STANDARD1);
				Set_IGridCell_CmdFlags(&pIGrid->pGrid[c3], FALSE, 0, 0, 0, 0, 0, 0, 0, 0, NULL, 0);
			}


		if (pPendingCommand->pCommand2->wType == CQ2TYPE_START_BUILD_UNIT_PRECONFIG && pPendingCommand->iCurrentSequenceNumber == 1)
		{
			cIGrid* pIGrid = &pIBox->pIGrids[1];
			pIBox->iActiveGrid = 1;		



			//for now, because we lack most of the infrastructure needed to make a differentiation regarding building units,
			//just output all listed unit types
			//NOTE: actually we sort of do, but I'm too lazy to do it right now. At the moment, there should be three considerations
			//affecting unit buildability:
			//		1. availability of components
			//		2. special considerations possessed by components
			//		3. whether the frame chosen for by the unit can be placed in the immediate vicinity of the structure
			//			in terrain that can accomodate it (the biggest one, ships in water)
			//
			//the first I could do right now, but would be better if I waited until after implementing double-linking
			//for components(so that nations can hold components in type-sorted arrangement)
			//the second, there is no system to handle special considerations of components(sigh...so many SYSTEMS...)
			//the third, can wait later, no big deal
			//(08.09.2003)

			//as before, we need to find out whether there are too many entries to fit in the box
			int iNumTypes = 0;
			for (lc50 = 0; lc50 < UTYPE_NUMTYPES; lc50++)
			{
				if (pCA->pUTypeTable->UnitTypes[lc50].CmpAttached.iNumComponents > 0)
					iNumTypes++;
			}
			int lc35 = 0;

			//add two to put in a "CANCEL" button
			iNumTypes += 2;

			//decide if we need a scrollbar
			if (iNumTypes > pIGrid->iElements_y)
			{
				pIGrid->Set_Scrollbar(TRUE, -1, iNumTypes - pIGrid->iElements_y, 0);

				int lc34 = 0;

				//and then we need to move up to the starting entry to start listing
				for (lc35 = 0; lc35 < UTYPE_NUMTYPES; lc35++)
				{
					if (lc34 == pIGrid->iScrollVal)
						break;
					if (pCA->pUTypeTable->UnitTypes[lc35].CmpAttached.iNumComponents > 0)
						lc34++;

				}
			}
					else
				pIGrid->Set_Scrollbar(FALSE, 0, 1, 0);			


				Set_IGridCell_DrawFlags(&pIGrid->pGrid[0], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0,0, "CANCEL", FONT_STANDARD1);
				Set_IGridCell_CmdFlags(&pIGrid->pGrid[0], TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_CANCEL_PENDINGCOMMAND, 0, 0, 0, 0, 0, NULL, 0);


			int lc82 = 2;
			for (lc50 = lc35; lc50 < UTYPE_NUMTYPES; lc50++)
			if (pCA->pUTypeTable->UnitTypes[lc50].CmpAttached.iNumComponents > 0)
			if (lc82 < pIGrid->iElements_y)
			{
				
				Set_IGridCell_DrawFlags(&pIGrid->pGrid[lc82], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, pCA->pUTypeTable->UnitTypes[lc50].Name, FONT_STANDARD1);
				DWORD* pdwType = new DWORD;
				*pdwType = lc50;
				Set_IGridCell_ObjRefFlags(&pIGrid->pGrid[lc82], 1, OBJTYPE_UTYPE, NULL, pdwType);
				lc82++;
			}
			for (int lc57 = lc82; lc57 < pIGrid->iElements_y; lc57++)
			{
				Set_IGridCell_DrawFlags(&pIGrid->pGrid[lc57], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0, 0, " ", FONT_STANDARD1);
			}
			iDrawBlankFlag = 0;

		}



			cIGrid* pIGrid = &pIBox->pIGrids[0];
				Set_IGridCell_AllFlags(&pIGrid->pGrid[(pIGrid->iElements_y - 2) * pIGrid->iElements_x], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_BLITFIT, ISURF_CMDBUTTONS, 0, 
										pCA->pCmdInfoTable->SourceButtonFrameRects[NUM_CQ2COMMANDS].left, 
										pCA->pCmdInfoTable->SourceButtonFrameRects[NUM_CQ2COMMANDS].right, 
										pCA->pCmdInfoTable->SourceButtonFrameRects[NUM_CQ2COMMANDS].top, 
										pCA->pCmdInfoTable->SourceButtonFrameRects[NUM_CQ2COMMANDS].bottom,
										NULL, FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_CANCEL_PENDINGCOMMAND, 0, 0, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);

//				Set_IGridCell_DrawFlags(&pIGrid->pGrid[pIGrid->iElements_y - 4], TRUE, IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 0, 0, 0,0, "CANCEL", FONT_STANDARD1);
//				Set_IGridCell_CmdFlags(&pIGrid->pGrid[pIGrid->iElements_y - 4], TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_CANCEL_PENDINGCOMMAND, 0, 0, 0, 0, 0, NULL, 0);

	}

//DEBUG
Write_Log("[Process_Console] Populating CONSOLE_INFO box.");
WriteLog_Enter();


//figure out how the "INFO" box will work.
//the current implementation will likely be temporary(08.11.2003)
pIBox = &IBoxes[IBOX_CONSOLE_INFO];

	//fill with a list of all components in national inventory
	//set the first three cells to "CMP", "STRCT", and "UNIT"

	//do this for all four iGrids(for now) 08.12.2003
int lc1; //local counter
	for (lc1 = 0; lc1 < 4; lc1++)
	{
			cIGrid* pIGrid = &pIBox->pIGrids[lc1];

				Set_IGridCell_AllFlags(&pIGrid->pGrid[0], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_BLITFIT, ISURF_CMDBUTTONS2, 0, 
										CMDBUTTON2_SIZE_X * 0, 
										CMDBUTTON2_SIZE_X * 1,
										CMDBUTTON2_SIZE_Y * 0,
										CMDBUTTON2_SIZE_Y * 1,
										NULL, FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, 0, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);
				Set_IGridCell_AllFlags(&pIGrid->pGrid[1], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_BLITFIT, ISURF_CMDBUTTONS2, 0, 
										CMDBUTTON2_SIZE_X * 0, 
										CMDBUTTON2_SIZE_X * 1,
										CMDBUTTON2_SIZE_Y * 1,
										CMDBUTTON2_SIZE_Y * 2,
										"<CTYPE>", FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, 1, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);	
				Set_IGridCell_AllFlags(&pIGrid->pGrid[2], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_BLITFIT, ISURF_CMDBUTTONS2, 0, 
										CMDBUTTON2_SIZE_X * 0, 
										CMDBUTTON2_SIZE_X * 1,
										CMDBUTTON2_SIZE_Y * 2,
										CMDBUTTON2_SIZE_Y * 3,
										"<UNIT>", FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, 2, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);
				Set_IGridCell_AllFlags(&pIGrid->pGrid[3], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_BLITFIT, ISURF_CMDBUTTONS2, 0, 
										CMDBUTTON2_SIZE_X * 0, 
										CMDBUTTON2_SIZE_X * 1,
										CMDBUTTON2_SIZE_Y * 3,
										CMDBUTTON2_SIZE_Y * 4,
										"<STRUCT>", FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, 3, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);
				Set_IGridCell_AllFlags(&pIGrid->pGrid[lc1], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_BLITFIT, ISURF_CMDBUTTONS2, 0, 
										CMDBUTTON2_SIZE_X * 1, 
										CMDBUTTON2_SIZE_X * 2,
										CMDBUTTON2_SIZE_Y * lc1,
										CMDBUTTON2_SIZE_Y * (lc1 + 1),
										"<STRUCT>", FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, lc1, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);
	}
//DEBUG
Write_Log("[Process_Console] Component Tab.");
WriteLog_Enter();
//reset so we can go back to setting the components grid
//IGrid = pIBox->pIGrids[0];
pIGrid = &pIBox->pIGrids[0];
	//set the next line to "components" and such
				Set_IGridCell_AllFlags(&pIGrid->pGrid[pIGrid->iElements_x * 1], TRUE, FALSE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										"-Available Components-", FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, 2, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);
	//now list as far as elements Y will allow
	lc1 = 3; //local counter
		cComponentA* pCmpTrav = (cComponentA*)pCA->pNationManager->ppNations[pCA->pGameOptions->iControl]->NationalInventory.pCmpList_First->pNext_list;
		cNation* pNation = pCA->pNationManager->ppNations[pCA->pGameOptions->iControl];
		cCmpInfo* pCmpInfo;

		//check to see whether the number of entries will fit
		if (pNation->NationalInventory.iNumComponents > pIGrid->iElements_y - 4)
		{
			pIGrid->Set_Scrollbar(TRUE, -1, pNation->NationalInventory.iNumComponents - (pIGrid->iElements_y - 4), 0);

			//and then go forward if necessary
			for (int lc36 = 0; lc36 < pIGrid->iScrollVal; lc36++)
				pCmpTrav = (cComponentA*)pCmpTrav->pNext_list;
		} else
			{
				pIGrid->Set_Scrollbar(FALSE, 0, 1, 0);
			}

		while (lc1 < pIGrid->iElements_y && pCmpTrav != pNation->NationalInventory.pCmpList_Last)
		{

				pCmpInfo = pCA->pCmpInfoTable->pCmpTable[pCmpTrav->iType];
				DWORD* pdwID = new DWORD;
				*pdwID = pCmpTrav->iID;

				Set_IGridCell_AllFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x], TRUE, FALSE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										pCmpInfo->Name, FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, 2, 0, 
										0, 0, NULL, 0, 1, OBJTYPE_COMPONENT, NULL, pdwID);


			for (int lc80 = 1; lc80 < pIGrid->iElements_x; lc80++)
			{
				pCmpInfo = pCA->pCmpInfoTable->pCmpTable[pCmpTrav->iType];
				DWORD* pdwID = new DWORD;
				*pdwID = pCmpTrav->iID;

				Set_IGridCell_ObjRefFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x + lc80], 1, OBJTYPE_COMPONENT, NULL, pdwID);
			}

			lc1++;
			pCmpTrav = (cComponentA*)pCmpTrav->pNext_list;
		}
		//and clear the rest of the list
		for (lc1 = lc1; lc1 < pIGrid->iElements_y; lc1++)
		{
					Set_IGridCell_AllFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x], TRUE, FALSE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										" ", FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, 2, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);		
			for (int lc80 = 1; lc80 < pIGrid->iElements_x; lc80++)
			{

				Set_IGridCell_ObjRefFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x + lc80], 0, OBJTYPE_COMPONENT, NULL, NULL);
			}
		}
//DEBUG
Write_Log("[Process_Console] Component Types Tab.");
WriteLog_Enter();

//set to component types grid
//IGrid = pIBox->pIGrids[1];
pIGrid = &pIBox->pIGrids[1];



	//set the next line to "component types" and such
				Set_IGridCell_AllFlags(&pIGrid->pGrid[pIGrid->iElements_x * 1], TRUE, FALSE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										"-Available Cmp Types-", FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, 2, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);
	//now list as far as elements Y will allow
	lc1 = 3; //local counter
		pNation = pCA->pNationManager->ppNations[pCA->pGameOptions->iControl];
		cCmpInfo* pCI_Trav = (cCmpInfo*)pCA->pCmpInfoTable->pCTL_First->pNext_cmplist;

			//as before, we need to find out whether there are too many entries to fit in the box
			int iNumTypes = 0;
			for (lc50 = 0; lc50 < pCA->pCmpInfoTable->iHighestTypeID + 1; lc50++)
			{
				if (pCA->pCmpInfoTable->pCmpTable[lc50] != NULL)
					iNumTypes++;
			}
			int lc35 = 0;
			//decide if we need a scrollbar
			if (iNumTypes > pIGrid->iElements_y - 4)
			{
				pIGrid->Set_Scrollbar(TRUE, -1, iNumTypes - (pIGrid->iElements_y - 4), 0);

				//we listed units from a list in this case, so now we traverse a list
				for (lc35 = 0; lc35 < pIGrid->iScrollVal; lc35++)
					pCI_Trav = (cCmpInfo*)pCI_Trav->pNext_cmplist;

				
			}
					else
				pIGrid->Set_Scrollbar(FALSE, 0, 1, 0);

		while (lc1 < pIGrid->iElements_y && pCI_Trav != pCA->pCmpInfoTable->pCTL_Last)
		{

				DWORD* pdwID = new DWORD;
				*pdwID = pCI_Trav->iType;

				Set_IGridCell_AllFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x], TRUE, FALSE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										pCI_Trav->Name, FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, 2, 0, 
										0, 0, NULL, 0, 1, OBJTYPE_CTYPE, NULL, pdwID);


			for (int lc80 = 1; lc80 < pIGrid->iElements_x; lc80++)
			{

				DWORD* pdwID = new DWORD;
				*pdwID = pCI_Trav->iType;

				Set_IGridCell_ObjRefFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x + lc80], 1, OBJTYPE_CTYPE, NULL, pdwID);

			}

			lc1++;
			pCI_Trav = (cCmpInfo*)pCI_Trav->pNext_cmplist;
		}
		//and clear the rest of the list
		for (lc1 = lc1; lc1 < pIGrid->iElements_y; lc1++)
		{
					Set_IGridCell_AllFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x], TRUE, FALSE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										" ", FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, 2, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);
					
			for (int lc80 = 1; lc80 < pIGrid->iElements_x; lc80++)
			{

				Set_IGridCell_ObjRefFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x + lc80], 0, OBJTYPE_COMPONENT, NULL, NULL);
			}
		}

//DEBUG
Write_Log("[Process_Console] Units Tab.");
WriteLog_Enter();

//now set the units grid
pIGrid = &pIBox->pIGrids[2];
	//set the next line to "units" and such
				Set_IGridCell_AllFlags(&pIGrid->pGrid[pIGrid->iElements_x * 1], TRUE, FALSE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										"-Active Units-", FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, 2, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);
	//go through the main unit list and list all friendly units
	lc1 = 3;
	cUnitA* pUL_Trav = (cUnitA*)pCA->pUnitList->pUL_First->pNext_UL;

	//we need to figure out how many friendly units there are, so we have to traverse the list....
	int iNumUnits = 0;
	while (pUL_Trav != pCA->pUnitList->pUL_Last)
	{
		if (pUL_Trav->iControl == pCA->pGameOptions->iControl)
			iNumUnits++;

		pUL_Trav = (cUnitA*)pUL_Trav->pNext_UL;
	}
	//reset
	pUL_Trav = (cUnitA*)pCA->pUnitList->pUL_First->pNext_UL;

	//decide if we need a scrollbar or not (4 is the starting point of listing)
	if (iNumUnits > pIGrid->iElements_y - 4)
	{
		pIGrid->Set_Scrollbar(TRUE, -1, iNumUnits - (pIGrid->iElements_y - 3), 0);

		//then increment forward the number of times we need
		int c34 = 0;
		while (pUL_Trav != pCA->pUnitList->pUL_Last)
		{
			if (pUL_Trav->iControl == pCA->pGameOptions->iControl)
				c34++;

			pUL_Trav = (cUnitA*)pUL_Trav->pNext_UL;

			if (c34 >= pIGrid->iScrollVal)
				break;
		}

	} else
		pIGrid->Set_Scrollbar(FALSE, 0, 1, 0);

	while (pUL_Trav != pCA->pUnitList->pUL_Last && lc1 < pIGrid->iElements_y)
	{
//DEBUG
Write_Log("[Process_Console] Unit ID: ");
itoa(pUL_Trav->iID, NText, 10);
Write_Log(NText);
Write_Log(" Name: ");
Write_Log(pUL_Trav->Name);
WriteLog_Enter();

		if (pUL_Trav->iControl == pCA->pGameOptions->iControl)
		{
Write_Log("[Process_Console] Listing Unit....");
WriteLog_Enter();

				int iElem2 = lc1*pIGrid->iElements_x;
				Set_IGridCell_AllFlags(&pIGrid->pGrid[iElem2], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										pUL_Trav->ShortName, FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SELECT_UNIT, pUL_Trav->iID, 2, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);

				//set the color depending on the unit's health


				double HR = pUL_Trav->iHP / double(pUL_Trav->iHPMax);
				int iG = pCA->pColorTable->Det_HPGreenCmp(HR);
				int iR = pCA->pColorTable->Det_HPRedCmp(HR);
				pIGrid->pGrid[iElem2].ColorB = 0;
				pIGrid->pGrid[iElem2].ColorG = iG;
				pIGrid->pGrid[iElem2].ColorR = iR;

				iElem2 = lc1*pIGrid->iElements_x + 1;
				Set_IGridCell_AllFlags(&pIGrid->pGrid[iElem2], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										pUL_Trav->Name, FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SELECT_UNIT, pUL_Trav->iID, 2, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);
				pIGrid->pGrid[iElem2].ColorB = 0;
				pIGrid->pGrid[iElem2].ColorG = iG;
				pIGrid->pGrid[iElem2].ColorR = iR;
				for (int lc80 = 2; lc80 < pIGrid->iElements_x; lc80++)
				{
						Set_IGridCell_CmdFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x+lc80], TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SELECT_UNIT, pUL_Trav->iID, 2, 0, 0, 0, NULL,  0);
				}
				


Write_Log("[Process_Console] Listing Done.");
WriteLog_Enter();
				lc1++;
		}

		pUL_Trav = (cUnitA*)pUL_Trav->pNext_UL;

	}
Write_Log("[Process_Console] Clearing Rest of List.");
WriteLog_Enter();

	//clear the restof the list
		for (lc1 = lc1; lc1 < pIGrid->iElements_y; lc1++)
		{
					Set_IGridCell_AllFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x], TRUE, FALSE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										" ", FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, 2, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);		
					Set_IGridCell_AllFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x + 1], TRUE, FALSE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										" ", FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, 2, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);	
				for (int lc80 = 2; lc80 < pIGrid->iElements_x; lc80++)
				{
						Set_IGridCell_CmdFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x+lc80], FALSE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SELECT_UNIT, 0, 2, 0, 0, 0, NULL,  0);
				}
		}

//DEBUG
Write_Log("[Process_Console] Structures Tab.");
WriteLog_Enter();

//now set the structure grid
pIGrid = &pIBox->pIGrids[3];
	//set the next line to "components" and such
				Set_IGridCell_AllFlags(&pIGrid->pGrid[pIGrid->iElements_x * 1], TRUE, FALSE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										"-Active Structures-", FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, 2, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);
	//go through the main unit list and list all friendly units
	lc1 = 3;
	cStructureA* pSL_Trav = (cStructureA*)pCA->pStructList->pSL_First->pNext;

	//once again, we have to decide whether we need a scrollbar
	//we need to figure out how many structures there are, so we have to traverse the list....
	int iNumStructs = 0;
	while (pSL_Trav != pCA->pStructList->pSL_Last)
	{
		if (pSL_Trav->iControl == pCA->pGameOptions->iControl)
			iNumStructs++;

		pSL_Trav = (cStructureA*)pSL_Trav->pNext;
	}
	//reset
	pSL_Trav = (cStructureA*)pCA->pStructList->pSL_First->pNext;

	//decide if we need a scrollbar or not (4 is the starting point of listing)
	if (iNumStructs > pIGrid->iElements_y - 4)
	{
		pIGrid->Set_Scrollbar(TRUE, -1, iNumStructs - (pIGrid->iElements_y - 4), 0);

		//then increment forward the number of times we need
		int c34 = 0;
		while (pSL_Trav != pCA->pStructList->pSL_Last)
		{
			if (pSL_Trav->iControl == pCA->pGameOptions->iControl)
				c34++;

			pSL_Trav = (cStructureA*)pSL_Trav->pNext;

			if (c34 >= pIGrid->iScrollVal)
				break;
		}

	} else
		pIGrid->Set_Scrollbar(FALSE, 0, 1, 0);

	while (pSL_Trav != pCA->pStructList->pSL_Last && lc1 < pIGrid->iElements_y)
	{

		if (pSL_Trav->iControl == pCA->pGameOptions->iControl)
		{
					Set_IGridCell_AllFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x], TRUE, TRUE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										pSL_Trav->Name, FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SELECT_STRUCT, pSL_Trav->iID, 2, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);
					for (int lc80 = 1; lc80 < pIGrid->iElements_x; lc80++)
					{
						Set_IGridCell_CmdFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x+lc80], TRUE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SELECT_STRUCT, pSL_Trav->iID, 2, 0, 0, 0, NULL,  0);
			
					}
				lc1++;
		}

		pSL_Trav = (cStructureA*)pSL_Trav->pNext;

	}
		//and clear the rest of the list
		for (lc1 = lc1; lc1 < pIGrid->iElements_y; lc1++)
		{
					Set_IGridCell_AllFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x], TRUE, FALSE, IGRIDCELL_DSOURCE_ALLCELL, 
										IGRIDCELL_DRAWTYPE_TEXT, 0, 0, 
										0, 
										0,
										0,
										0,
										" ", FONT_STANDARD1, CQDEST_INTERFACE, 
										CQTYPE_SET_ACTIVE_IGRID, IBOX_CONSOLE_INFO, 2, 0, 
										0, 0, NULL, 0, 0, 0, NULL, NULL);		
					for (int lc80 = 1; lc80 < pIGrid->iElements_x; lc80++)
					{
						Set_IGridCell_CmdFlags(&pIGrid->pGrid[lc1*pIGrid->iElements_x+lc80], FALSE, IGRIDCELL_DSOURCE_ALLCELL, CQDEST_INTERFACE, CQTYPE_SELECT_STRUCT, 0, 2, 0, 0, 0, NULL,  0);
			
					}

		}
//DEBUG
Write_Log("[Process_Console] *Function Complete.");
WriteLog_Enter();
*/

/*
	//if button is DOWN (== 2) inside scrollbar region, setup a scrollgrid
	if (pCA->pInputState->IGState_Mouse.Buttons[0] == 2)
	if (pIGrid->bScrollbarFlag == TRUE)
	if (pCA->pInputState->IGState_Mouse.iXPos >= (IBoxes[mouse_currbox].BoxPos.right - (pCA->pImages[ISURF_SCROLLBAR]->DDSDImage.dwWidth)))
	{
		pScrollGrid = pIGrid;
		pScrollBox = &IBoxes[mouse_currbox];
	}
	//if mouse-up, release the scroll grid


	//for now, prevent clicks in the scroll bar region from being processed (07.26.2004)
	if (pIGrid->bScrollbarFlag == FALSE || pCA->pInputState->IGState_Mouse.iXPos <= IBoxes[mouse_currbox].BoxPos.right - (pCA->pImages[ISURF_SCROLLBAR]->DDSDImage.dwWidth))
	if (pCA->pInputState->IGState_Mouse.Buttons[0] == 2)
	{
//DEBUG
Write_Log("[Process Input] Mouse Click Detected");
WriteLog_Enter();
		
		if (wGridIndex < 0)
		{
			//DEBUG
			Write_Log("Error! Negative Grid Index.");
			WriteLog_Enter();
			wGridIndex = 0;

		}

		//execute command, if active
		if (pGrid[wGridIndex].bActiveCommand == TRUE)
		{
//DEBUG
Write_Log("[Process Input] Executing Command");
WriteLog_Enter();
			//if DSOURCE_ALLCELL, execute command from cell data only
//			if (pGrid[wGridIndex].bDataSources == IGRIDCELL_DSOURCE_ALLCELL)
//				pCA->pCommQueue->Issue_Command(pGrid[wGridIndex].ProgCommand.wDest, pGrid[wGridIndex].ProgCommand.wType,
//												pGrid[wGridIndex].ProgCommand.wParam1, pGrid[wGridIndex].ProgCommand.wParam2,
//												pGrid[wGridIndex].ProgCommand.wParam3, pGrid[wGridIndex].ProgCommand.fParam4,
//												pGrid[wGridIndex].ProgCommand.fParam5, pGrid[wGridIndex].ProgCommand.pParam6, 
//												pGrid[wGridIndex].ProgCommand.lExecTick);

//DEBUG
Write_Log("[Process Input] Execute Command Complete");
WriteLog_Enter();

		}
*/ /*
		//check for any object references, and if found, compile an input packet and send it to the main IPB (05.20.2003)
		if (pGrid[wGridIndex].dwNumObjRefs > 0)
		{
//DEBUG
Write_Log("[Process Input] Adding Input Packet ObjRef");
WriteLog_Enter();
			//now call the IPB to compile and assemble(pass TRUE copyflag because the packet inside the igridcell refreshes constantly
			pCA->pInputPacketBuffer->Compile_Add_Packet( IPACKETTYPE_OBJREF,
														pGrid[wGridIndex].dwObjRefType,
														pGrid[wGridIndex].dwNumObjRefs,
														pGrid[wGridIndex].pObjRefArray,
														pGrid[wGridIndex].pdwIDArray,
														0,
														0,
														NULL,
														TRUE);
//DEBUG
Write_Log("[Process Input] Adding Input Packet ObjRef done");
WriteLog_Enter();
		}
	}

//DEBUG
Write_Log("[Process Input] Stuff with Pending Command");
WriteLog_Enter();
		//if current grid is over a command or CANCEL, set display text
		if (pGrid[wGridIndex].bActiveCommand == TRUE)
		{
			if (pGrid[wGridIndex].ProgCommand.wType == CQTYPE_START_PENDINGCOMMAND)
			{
				pDisplayText = pCA->pCmdInfoTable->Names[pGrid[wGridIndex].ProgCommand.wParam1];
			}
			if (pGrid[wGridIndex].ProgCommand.wType == CQTYPE_CANCEL_PENDINGCOMMAND)
			{
				pDisplayText = "CANCEL";
			}
	
		}
			if (pGrid[wGridIndex].dwNumObjRefs == 1)
			{
				if (pGrid[wGridIndex].dwObjRefType == OBJTYPE_COMPONENT)
				{
					//display the component cost
					cComponentA* pComponent = pCA->pCmpIDTable->pTable[pGrid[wGridIndex].pdwIDArray[0]];
					cCmpInfo* pCmpInfo = pCA->pCmpInfoTable->pCmpTable[pComponent->iType];

					ZeroMemory(StandingTextBuffer, sizeof(char)*TEMP_STD_STRLEN);
					strcat(StandingTextBuffer, "Cost: ");
					itoa(pCmpInfo->ddResourceCost, NText, 10);
					strcat(StandingTextBuffer, NText);
					pDisplayText = StandingTextBuffer;

				}
				if (pGrid[wGridIndex].dwObjRefType == OBJTYPE_CTYPE)
				{
					//display the component cost

					cCmpInfo* pCmpInfo = pCA->pCmpInfoTable->pCmpTable[pGrid[wGridIndex].pdwIDArray[0]];
					ZeroMemory(StandingTextBuffer, sizeof(char)*TEMP_STD_STRLEN);
					strcat(StandingTextBuffer, "Cost: ");
					itoa(pCmpInfo->ddResourceCost, NText, 10);
					strcat(StandingTextBuffer, NText);
					pDisplayText = StandingTextBuffer;

				}
				if (pGrid[wGridIndex].dwObjRefType == OBJTYPE_UTYPE)
				{
					//display the component cost

					UNITTYPEINFO* pUInfo = &pCA->pUTypeTable->UnitTypes[pGrid[wGridIndex].pdwIDArray[0]];
					ZeroMemory(StandingTextBuffer, sizeof(char)*TEMP_STD_STRLEN);
					strcat(StandingTextBuffer, "Cost: ");
					itoa(pUInfo->ddCost, NText, 10);
					strcat(StandingTextBuffer, NText);
					pDisplayText = StandingTextBuffer;

				}
				if (pGrid[wGridIndex].dwObjRefType == OBJTYPE_STYPE)
				{
					//display the component cost

					cStructInfo* pSInfo = pCA->pStructInfoTable->pTable[pGrid[wGridIndex].pdwIDArray[0]];
					ZeroMemory(StandingTextBuffer, sizeof(char)*TEMP_STD_STRLEN);
					strcat(StandingTextBuffer, "Cost: ");
					itoa(pSInfo->ddResourceCost, NText, 10);
					strcat(StandingTextBuffer, NText);
					pDisplayText = StandingTextBuffer;

				}


			}	


*/
//DEBUG

//Write_Log("[Process Input] Stuff done");
//WriteLog_Enter();
