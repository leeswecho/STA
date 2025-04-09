#include "classes.h"

void cGDisplay::Init(VOID* pCA)
{

if (DEBUG + DEBUG_GDISPLAY >= 1)
	Init_LogFile();

	pCAccess = pCA;
	
	cClassAccess* pCA2 = (cClassAccess*)pCAccess;

	UBlitQueue.Init(pCA);
	

	MDRAW_NUMCOLS = (int)(pCA2->pDD_Display->i_XResD * MDRAW_XSCR)/(TILE_SIZE);
	MDRAW_NUMROWS = (int)(pCA2->pDD_Display->i_YResD * MDRAW_YSCR)/(TILE_SIZE);

	//find the minimap draw rect
	MinimapDrawRect.top = (int)(pCA2->pDD_Display->i_YResD * MMDRAW_YSCR_START);
	MinimapDrawRect.left = (int)(pCA2->pDD_Display->i_XResD * MMDRAW_XSCR_START);
	MinimapDrawRect.right = MinimapDrawRect.left + (int)(pCA2->pDD_Display->i_XResD * MMDRAW_XSCR);
	MinimapDrawRect.bottom = MinimapDrawRect.top + (int)(pCA2->pDD_Display->i_YResD * MMDRAW_YSCR);
	MinimapZoomDrawRect.top = (int)(pCA2->pDD_Display->i_YResD * MMZDRAW_YSCR_START);
	MinimapZoomDrawRect.left = (int)(pCA2->pDD_Display->i_XResD * MMZDRAW_XSCR_START);
	MinimapZoomDrawRect.right = MinimapZoomDrawRect.left + (int)(pCA2->pDD_Display->i_XResD * MMZDRAW_XSCR);
	MinimapZoomDrawRect.bottom = MinimapZoomDrawRect.top + (int)(pCA2->pDD_Display->i_YResD * MMZDRAW_YSCR);

	//set interface draw rectangles
	int iBC = 5; //this is just to help in quickly filling out the panel charactersitics
	int iW = 160;
	int iH = 120;
	iMinimumWidth = 11;
	iMinimumHeight = 11;

		RectIfcPanels[IFCRECT_TOPLEFT].left = 0;
		RectIfcPanels[IFCRECT_TOPLEFT].right = iBC;
		RectIfcPanels[IFCRECT_TOPLEFT].top = 0;
		RectIfcPanels[IFCRECT_TOPLEFT].bottom = iBC;

		RectIfcPanels[IFCRECT_TOPRIGHT].left = iW - iBC;
		RectIfcPanels[IFCRECT_TOPRIGHT].right = iW;
		RectIfcPanels[IFCRECT_TOPRIGHT].top = 0;
		RectIfcPanels[IFCRECT_TOPRIGHT].bottom = iBC;
		
		RectIfcPanels[IFCRECT_BOTTOMRIGHT].left = iW - iBC;
		RectIfcPanels[IFCRECT_BOTTOMRIGHT].right = iW;
		RectIfcPanels[IFCRECT_BOTTOMRIGHT].top = iH - iBC;
		RectIfcPanels[IFCRECT_BOTTOMRIGHT].bottom = iH;

		RectIfcPanels[IFCRECT_BOTTOMLEFT].left = 0;
		RectIfcPanels[IFCRECT_BOTTOMLEFT].right = iBC;
		RectIfcPanels[IFCRECT_BOTTOMLEFT].top = iH - iBC;
		RectIfcPanels[IFCRECT_BOTTOMLEFT].bottom = iH;

		RectIfcPanels[IFCRECT_LEFT].left = 0;
		RectIfcPanels[IFCRECT_LEFT].right = iBC;
		RectIfcPanels[IFCRECT_LEFT].top = iBC;
		RectIfcPanels[IFCRECT_LEFT].bottom = iH - iBC;

		RectIfcPanels[IFCRECT_RIGHT].left = iW - iBC;
		RectIfcPanels[IFCRECT_RIGHT].right = iW;
		RectIfcPanels[IFCRECT_RIGHT].top = iBC;
		RectIfcPanels[IFCRECT_RIGHT].bottom = iH - iBC;

		RectIfcPanels[IFCRECT_TOP].left = iBC;
		RectIfcPanels[IFCRECT_TOP].right = iW - iBC;
		RectIfcPanels[IFCRECT_TOP].top = 0;
		RectIfcPanels[IFCRECT_TOP].bottom = iBC;

		RectIfcPanels[IFCRECT_BOTTOM].left = iBC;
		RectIfcPanels[IFCRECT_BOTTOM].right = iW - iBC;
		RectIfcPanels[IFCRECT_BOTTOM].top = iH - iBC;
		RectIfcPanels[IFCRECT_BOTTOM].bottom = iH;

		RectIfcPanels[IFCRECT_CENTER].left = iBC;
		RectIfcPanels[IFCRECT_CENTER].right = iW - iBC;
		RectIfcPanels[IFCRECT_CENTER].top = iBC;
		RectIfcPanels[IFCRECT_CENTER].bottom = iH - iBC;

	//do again for list boxes
	iBC = 5; //this is just to help in quickly filling out the panel charactersitics
	iW = 100;
	iH = 100;
	iMinimumWidth = 11;
	iMinimumHeight = 11;

		RectIfcListBoxes[IFCRECT_TOPLEFT].left = 0;
		RectIfcListBoxes[IFCRECT_TOPLEFT].right = iBC;
		RectIfcListBoxes[IFCRECT_TOPLEFT].top = 0;
		RectIfcListBoxes[IFCRECT_TOPLEFT].bottom = iBC;

		RectIfcListBoxes[IFCRECT_TOPRIGHT].left = iW - iBC;
		RectIfcListBoxes[IFCRECT_TOPRIGHT].right = iW;
		RectIfcListBoxes[IFCRECT_TOPRIGHT].top = 0;
		RectIfcListBoxes[IFCRECT_TOPRIGHT].bottom = iBC;
		
		RectIfcListBoxes[IFCRECT_BOTTOMRIGHT].left = iW - iBC;
		RectIfcListBoxes[IFCRECT_BOTTOMRIGHT].right = iW;
		RectIfcListBoxes[IFCRECT_BOTTOMRIGHT].top = iH - iBC;
		RectIfcListBoxes[IFCRECT_BOTTOMRIGHT].bottom = iH;

		RectIfcListBoxes[IFCRECT_BOTTOMLEFT].left = 0;
		RectIfcListBoxes[IFCRECT_BOTTOMLEFT].right = iBC;
		RectIfcListBoxes[IFCRECT_BOTTOMLEFT].top = iH - iBC;
		RectIfcListBoxes[IFCRECT_BOTTOMLEFT].bottom = iH;

		RectIfcListBoxes[IFCRECT_LEFT].left = 0;
		RectIfcListBoxes[IFCRECT_LEFT].right = iBC;
		RectIfcListBoxes[IFCRECT_LEFT].top = iBC;
		RectIfcListBoxes[IFCRECT_LEFT].bottom = iH - iBC;

		RectIfcListBoxes[IFCRECT_RIGHT].left = iW - iBC;
		RectIfcListBoxes[IFCRECT_RIGHT].right = iW;
		RectIfcListBoxes[IFCRECT_RIGHT].top = iBC;
		RectIfcListBoxes[IFCRECT_RIGHT].bottom = iH - iBC;

		RectIfcListBoxes[IFCRECT_TOP].left = iBC;
		RectIfcListBoxes[IFCRECT_TOP].right = iW - iBC;
		RectIfcListBoxes[IFCRECT_TOP].top = 0;
		RectIfcListBoxes[IFCRECT_TOP].bottom = iBC;

		RectIfcListBoxes[IFCRECT_BOTTOM].left = iBC;
		RectIfcListBoxes[IFCRECT_BOTTOM].right = iW - iBC;
		RectIfcListBoxes[IFCRECT_BOTTOM].top = iH - iBC;
		RectIfcListBoxes[IFCRECT_BOTTOM].bottom = iH;

		RectIfcListBoxes[IFCRECT_CENTER].left = iBC;
		RectIfcListBoxes[IFCRECT_CENTER].right = iW - iBC;
		RectIfcListBoxes[IFCRECT_CENTER].top = iBC;
		RectIfcListBoxes[IFCRECT_CENTER].bottom = iH - iBC;

	//do again for secondary interface panels
	iBC = 5; //this is just to help in quickly filling out the panel charactersitics
	iW = 40;
	iH = 40;
	iMinimumWidth = 11;
	iMinimumHeight = 11;

		RectIfcSecondaryPanels[IFCRECT_TOPLEFT].left = 0;
		RectIfcSecondaryPanels[IFCRECT_TOPLEFT].right = iBC;
		RectIfcSecondaryPanels[IFCRECT_TOPLEFT].top = 0;
		RectIfcSecondaryPanels[IFCRECT_TOPLEFT].bottom = iBC;

		RectIfcSecondaryPanels[IFCRECT_TOPRIGHT].left = iW - iBC;
		RectIfcSecondaryPanels[IFCRECT_TOPRIGHT].right = iW;
		RectIfcSecondaryPanels[IFCRECT_TOPRIGHT].top = 0;
		RectIfcSecondaryPanels[IFCRECT_TOPRIGHT].bottom = iBC;
		
		RectIfcSecondaryPanels[IFCRECT_BOTTOMRIGHT].left = iW - iBC;
		RectIfcSecondaryPanels[IFCRECT_BOTTOMRIGHT].right = iW;
		RectIfcSecondaryPanels[IFCRECT_BOTTOMRIGHT].top = iH - iBC;
		RectIfcSecondaryPanels[IFCRECT_BOTTOMRIGHT].bottom = iH;

		RectIfcSecondaryPanels[IFCRECT_BOTTOMLEFT].left = 0;
		RectIfcSecondaryPanels[IFCRECT_BOTTOMLEFT].right = iBC;
		RectIfcSecondaryPanels[IFCRECT_BOTTOMLEFT].top = iH - iBC;
		RectIfcSecondaryPanels[IFCRECT_BOTTOMLEFT].bottom = iH;

		RectIfcSecondaryPanels[IFCRECT_LEFT].left = 0;
		RectIfcSecondaryPanels[IFCRECT_LEFT].right = iBC;
		RectIfcSecondaryPanels[IFCRECT_LEFT].top = iBC;
		RectIfcSecondaryPanels[IFCRECT_LEFT].bottom = iH - iBC;

		RectIfcSecondaryPanels[IFCRECT_RIGHT].left = iW - iBC;
		RectIfcSecondaryPanels[IFCRECT_RIGHT].right = iW;
		RectIfcSecondaryPanels[IFCRECT_RIGHT].top = iBC;
		RectIfcSecondaryPanels[IFCRECT_RIGHT].bottom = iH - iBC;

		RectIfcSecondaryPanels[IFCRECT_TOP].left = iBC;
		RectIfcSecondaryPanels[IFCRECT_TOP].right = iW - iBC;
		RectIfcSecondaryPanels[IFCRECT_TOP].top = 0;
		RectIfcSecondaryPanels[IFCRECT_TOP].bottom = iBC;

		RectIfcSecondaryPanels[IFCRECT_BOTTOM].left = iBC;
		RectIfcSecondaryPanels[IFCRECT_BOTTOM].right = iW - iBC;
		RectIfcSecondaryPanels[IFCRECT_BOTTOM].top = iH - iBC;
		RectIfcSecondaryPanels[IFCRECT_BOTTOM].bottom = iH;

		RectIfcSecondaryPanels[IFCRECT_CENTER].left = iBC;
		RectIfcSecondaryPanels[IFCRECT_CENTER].right = iW - iBC;
		RectIfcSecondaryPanels[IFCRECT_CENTER].top = iBC;
		RectIfcSecondaryPanels[IFCRECT_CENTER].bottom = iH - iBC;

	//set font color originally as something(03.24.2003)
	dwFontColor = 10000;

	//initialize DrawQueue
	DrawQueue.Init();


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cGDisplay::~cGDisplay()   //obsolete 09.26.2003
{

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Exit()
{
	DrawQueue.Exit();
	UBlitQueue.Exit();
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_Pie(double ddFraction, int iX, int iY)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

//prevent going where we aren't prepared for
if (ddFraction > 1.0)
{
	if (ddFraction > 1.1)
	{
	char NText[10];
	pCA->pChatBuffer->Add_CBNode("Warning. Disp_Pie fraction above limit.", 60);
	gcvt(ddFraction, 7, NText);
	pCA->pChatBuffer->Add_CBNode(NText, 60);
	}

	ddFraction = 1.0;
}

if (ddFraction < 0.0)
{
	pCA->pChatBuffer->Add_CBNode("Warning. Disp_Pie fraction below limit.", 60);
	ddFraction = 0.0;
}

	int iFrame = (1.0 - ddFraction) * 24;  //(1- ddFraction) is because I accidentally drew the pies in reverse order,
											//floor to int is deliberate
	int iFrameX = iFrame / 6;
	int iFrameY = iFrame % 6;

	RECT SourceRect;
	SourceRect.left = iFrameX * pCA->pImages[ISURF_PIE1]->iXFrameSize;	
	SourceRect.right = SourceRect.left + pCA->pImages[ISURF_PIE1]->iXFrameSize;
	SourceRect.top = iFrameY * pCA->pImages[ISURF_PIE1]->iYFrameSize;
	SourceRect.bottom = SourceRect.top + pCA->pImages[ISURF_PIE1]->iYFrameSize;

	UBlitQueue.Submit_BlitRequest(NUM_ZLEVELS - 2, iX, iY, pCA->pImages[ISURF_PIE1]->pDDSImage, &SourceRect,  DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_ALPHA, 0, NULL);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_Map(LPDIRECTDRAWSURFACE7 pDrawSurf)
{

	int   iType;
	int   iDrawFrame;
	int   iMapIndex;

	int iOverX; //used for recentering on the square
	int iOverY;

	int c1, c2, c3; //counters

	int iZLevel = 0; //used so that stuff is drawn in a better order

	//cast the class access pointer
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	
	LPDIRECTDRAWSURFACE7 pDDSImage;
	LPDIRECTDRAWSURFACE7 pDDSLayer2 = pCA->pImages[ISURF_MLAYER2]->pDDSImage;
	LPDIRECTDRAWSURFACE7 pDDSLinks = pCA->pImages[ISURF_LINKS]->pDDSImage;

	RECT  SRect;
	ZeroMemory(&ddblitfx, sizeof(DDBLTFX));
	ddblitfx.dwSize = sizeof(DDBLTFX);

	MTILE* pMap = pCA->pGameWorld->pMap;

		//arrcheck val
		int icheck = 1;
		icheck = icheck << pCA->pGameOptions->iControl;
			char NText[50];
	//in here just for now (12.14.2001)  //too late, tired to write the real code that will go here!
	for (c1 = 0; c1 <= MDRAW_NUMCOLS; c1++)
	for (c2 = 0; c2 <= MDRAW_NUMROWS; c2++)
	{
		//DEBUG
		if (c1 > 70)
		{
			int a = 1;
		}

		iZLevel = 0;
		//more temporary stuff until design of drawing bitmaps can be finalized(12.15.2001)
		//what appears here is probably close to final design(12.25.2001 YES, I was working on Christmas day....
		//woke up too late to get to Magic Kingdom!
		iMapIndex = ((c2+pCA->pInterface->map_yview)*pCA->pGameWorld->iWidth) + c1 + pCA->pInterface->map_xview;
		MTILE* pTile = &pMap[iMapIndex];
		iType = pMap[iMapIndex].iType;

//this functionality moved to Process_Sector (01.08.2005)
//		if (pCA->pSectorTable->Sectors[pMap[iMapIndex].iSector]->iControl == pCA->pGameOptions->iControl)
//			pMap[iMapIndex].barrRevealed |= icheck;

		if (pMap[iMapIndex].barrRevealed & icheck)
			
		{

			pDDSImage = pCA->pImages[iType]->pDDSImage;
			iDrawFrame = pMap[iMapIndex].iDrawFrame;

			//pMap[iMapIndex].barrRevealed |= icheck;  //why is this line here??? (12.25.2004)
			
		}	
				else
				{
					pDDSImage = pCA->pImages[ISURF_BLACK]->pDDSImage;
					iDrawFrame = 0;
					iType = ISURF_BLACK;
				}


			if (iType > 6 && iType != ISURF_BLACK)
				iType = 1;
	
			if (iType == MAPTILE_FOREST)
			{
				iZLevel = 2;
			}
			if (iType == ISURF_BLACK)
			{
				iZLevel = 4;
			}

			iOverX = (pCA->pImages[iType]->iXFrameSize - TILE_SIZE) / 2;
			iOverY = (pCA->pImages[iType]->iYFrameSize - TILE_SIZE) / 2;

			int iOffX = pCA->pImages[iType]->iXCenterOffset;
			int iOffY = pCA->pImages[iType]->iYCenterOffset;


			SRect.top = (iDrawFrame % BLIT_FPCOL) * pCA->pImages[iType]->iYFrameSize;
			SRect.bottom = SRect.top + pCA->pImages[iType]->iYFrameSize;
			SRect.left = (iDrawFrame / BLIT_FPCOL) * pCA->pImages[iType]->iXFrameSize;
			SRect.right = SRect.left + pCA->pImages[iType]->iXFrameSize;

			if (c1 == 0)
			{
				SRect.left += iOverX;
				iOverX = 0;
			}
			if (c2 == 0)
			{
				SRect.top += iOverY;
				iOverY = 0;
			}

			//HACK, we can save some effort here by immediately blitting everything that is zlevel 0
			//(since we already know it blits first
			if (iZLevel == 0)
			{
				pDrawSurf->BltFast( (c1*TILE_SIZE) - iOverX + iOffX, (c2*TILE_SIZE) - iOverY + iOffY, pDDSImage, &SRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			}
			else
				UBlitQueue.Submit_BlitRequest(iZLevel, (c1*TILE_SIZE) - iOverX+ iOffX, (c2*TILE_SIZE) - iOverY + iOffY, pDDSImage, &SRect,  DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_NORMAL, 0, NULL);
		
			//draw Layer2 if it exists
			iDrawFrame = pMap[iMapIndex].iDrawFrame_Layer2;

			//HACK to allow showing IM targets when pushing down TILDE key (for debugging)
			if (pCA->pInputState->IGState_Keyboard[DIK_GRAVE] > 0)
			if (pTile->wIMTargeted_Count > 0)
				iDrawFrame = 16;

			if (pCA->pInputState->IGState_Keyboard[DIK_E] > 0)
			{
				itoa(pCA->pSectorTable->Sectors[pTile->iSector]->iNumLinks, NText, 10);
				Draw_Text(pDrawSurf, NText, c1*TILE_SIZE, c2*TILE_SIZE, FONT_ALPHA_STANDARD1);
			}
			if (pCA->pInputState->IGState_Keyboard[DIK_W] > 0)
			{
				itoa(pTile->iLastParticipatedJobNumber, NText, 10);
				Draw_Text(pDrawSurf, NText, c1*TILE_SIZE, c2*TILE_SIZE, FONT_ALPHA_STANDARD1);
			}
			if (iDrawFrame != 0)
			{
				iZLevel = 0;
				iOverX = (pCA->pImages[ISURF_MLAYER2]->iXFrameSize - TILE_SIZE) / 2;
				iOverY = (pCA->pImages[ISURF_MLAYER2]->iYFrameSize - TILE_SIZE) / 2;

				SRect.top = (iDrawFrame % BLIT_FPCOL) * pCA->pImages[ISURF_MLAYER2]->iYFrameSize;
				SRect.bottom = SRect.top + pCA->pImages[ISURF_MLAYER2]->iYFrameSize;
				SRect.left = (iDrawFrame / BLIT_FPCOL) * pCA->pImages[ISURF_MLAYER2]->iXFrameSize;
				SRect.right = SRect.left + pCA->pImages[ISURF_MLAYER2]->iXFrameSize;
	
				if (c1 == 0)
				{
					SRect.left += iOverX;
					iOverX = 0;
				}
				if (c2 == 0)
				{
					SRect.top += iOverY;
					iOverY = 0;
				}				

				//Blitqueue disabled for map due to it's heavily processor load and lack-of-need(since it's always on the bottom
				//re-enabled to draw forests correct(01.24.2005)
				UBlitQueue.Submit_BlitRequest(iZLevel, (c1*TILE_SIZE) - iOverX + iOffX, (c2*TILE_SIZE) - iOverY + iOffY, pDDSLayer2, &SRect,  DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_NORMAL, 0, NULL);
				//pDrawSurf->BltFast(c1 *TILE_SIZE - iOverX + iOffX, c2*TILE_SIZE - iOverY + iOffY, pDDSLayer2, &SRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			}
				//now draw pipes
				if ( (pTile->barrOverlay & MAPOVERLAY_LINK_MASK) > 0)
				{
					iDrawFrame = pTile->iDrawFrame_LinkGraphic;

					if (iDrawFrame >= 0)
					{
						iZLevel = 0;
						iOverX = (pCA->pImages[ISURF_MLAYER2]->iXFrameSize - TILE_SIZE) / 2;
						iOverY = (pCA->pImages[ISURF_MLAYER2]->iYFrameSize - TILE_SIZE) / 2;

						SRect.top = (iDrawFrame % BLIT_FPCOL) * pCA->pImages[ISURF_LINKS]->iYFrameSize;
						SRect.bottom = SRect.top + pCA->pImages[ISURF_LINKS]->iYFrameSize;
						SRect.left = (iDrawFrame / BLIT_FPCOL) * pCA->pImages[ISURF_LINKS]->iXFrameSize;
						SRect.right = SRect.left + pCA->pImages[ISURF_LINKS]->iXFrameSize;
			
						if (c1 == 0)
						{
							SRect.left += iOverX;
							iOverX = 0;
						}
						if (c2 == 0)
						{
							SRect.top += iOverY;
							iOverY = 0;
						}				
						UBlitQueue.Submit_BlitRequest(iZLevel, (c1*TILE_SIZE) - iOverX, (c2*TILE_SIZE) - iOverY, pDDSLinks, &SRect,  DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_ALPHA, 0, NULL);
					}

				}


	}

/* (dispatch later...)
//DEBUG
Write_Log("Dispatching Requests...");
WriteLog_Enter();

		UBlitQueue.Dispatch_BlitRequests(pDrawSurf, 0, 3);

//DEBUG
Write_Log("Dispatching Requests Complete...");
WriteLog_Enter();
*/

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_Cursor(LPDIRECTDRAWSURFACE7 pDrawSurf)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int iDrawFrame = pCA->pInputState->iDrawFrame;
	RECT SRect;

	SRect.left = (iDrawFrame / BLIT_FPCOL) * CURSOR_SIZE;
	SRect.top = (iDrawFrame % BLIT_FPCOL) * CURSOR_SIZE;
	SRect.right = SRect.left + CURSOR_SIZE;
	SRect.bottom = SRect.top + CURSOR_SIZE;

	//correct for draws that are partially off-screen by adjusting the source rectangle
	if(pCA->pInputState->IGState_Mouse.iXPos > pCA->pDD_Display->i_XResD - CURSOR_SIZE)
		SRect.right = SRect.right - CURSOR_SIZE + pCA->pDD_Display->i_XResD - pCA->pInputState->IGState_Mouse.iXPos;
	if(pCA->pInputState->IGState_Mouse.iYPos > pCA->pDD_Display->i_YResD - CURSOR_SIZE)
		SRect.bottom = SRect.bottom - CURSOR_SIZE + pCA->pDD_Display->i_YResD - pCA->pInputState->IGState_Mouse.iYPos;

	pDrawSurf->BltFast(pCA->pInputState->IGState_Mouse.iXPos, pCA->pInputState->IGState_Mouse.iYPos, pCA->pImages[ISURF_CURSOR]->pDDSImage, &SRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

	//if a build command is underway and we are selecting a structure to build, show the gray patch of the structure
	if (pCA->pInterface->mouse_currbox == IBOX_PRIMARY)
	if (pCA->pInterface->pPendingCommand != NULL)
	if (pCA->pInterface->pPendingCommand->pCommand2->wType == CQ2TYPE_BUILD_STRUCTURE && pCA->pInterface->pPendingCommand->iCurrentSequenceNumber == 2)
	{
		//this works based on the assumption that the last acquired input packet contains the indicated structure type
		INPUTPACKET* pPacket = (INPUTPACKET*)pCA->pInterface->pPendingCommand->AcquiredData.pIPB_Last->pPrev;
		//woks based on the assumption that that type is stored in the first item in the object array
		DWORD dwType = pPacket->pdwIDs[0];

		//now we can draw the patches corresponding to the squares!
		cStructInfo* pStructInfo = pCA->pStructInfoTable->pTable[dwType];

			RECT SrcRect;
			int iFrameSize = pCA->pImages[ISURF_SELECT]->iXFrameSize;
			SrcRect.left = 0;
			SrcRect.right = pCA->pImages[ISURF_SELECT]->DDSDImage.dwWidth;
			SrcRect.top = iFrameSize;
			SrcRect.bottom = iFrameSize * 2;

			//if we can't actually build here, change cursor to red
			if (pCA->pGameWorld->IsValidBuildLocation(pCA->pInterface->PosMap_Mouse.x, pCA->pInterface->PosMap_Mouse.y, dwType) == FALSE, dwType)
			{
				SrcRect.top = iFrameSize * 4;
				SrcRect.bottom = iFrameSize * 5;	
			}


			//blit at 0,0
			pDrawSurf->BltFast( (pCA->pInterface->PosMap_Mouse.x - pCA->pInterface->map_xview) *TILE_SIZE, (pCA->pInterface->PosMap_Mouse.y - pCA->pInterface->map_yview) * TILE_SIZE, pCA->pImages[ISURF_SELECT]->pDDSImage, &SrcRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			

		for (int c920 = 0; c920 < pStructInfo->iOccMaptiles; c920++)
		{
			SrcRect.left = 0;
			SrcRect.right = pCA->pImages[ISURF_SELECT]->DDSDImage.dwWidth;
			SrcRect.top = iFrameSize;
			SrcRect.bottom = iFrameSize * 2;

			//if we can't actually build here, change cursor to red
			if (pCA->pGameWorld->IsValidBuildLocation(pCA->pInterface->PosMap_Mouse.x + pStructInfo->Pos_offset_array[c920].x, pCA->pInterface->PosMap_Mouse.y + pStructInfo->Pos_offset_array[c920].y, dwType) == FALSE)
			{
				SrcRect.top = iFrameSize * 4;
				SrcRect.bottom = iFrameSize * 5;	
			}

			//wow, perhaps this should be simplified :-)
			pDrawSurf->BltFast( (pCA->pInterface->PosMap_Mouse.x - pCA->pInterface->map_xview + pStructInfo->Pos_offset_array[c920].x) *TILE_SIZE, (pCA->pInterface->PosMap_Mouse.y - pCA->pInterface->map_yview + pStructInfo->Pos_offset_array[c920].y) * TILE_SIZE, pCA->pImages[ISURF_SELECT]->pDDSImage, &SrcRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		}
	}

	//if we are in the middle of laying a link
	if (pCA->pInterface->mouse_currbox == IBOX_PRIMARY)
	if (pCA->pInterface->pPendingCommand != NULL)
	if (pCA->pInterface->pPendingCommand->pCommand2->wType == CQ2TYPE_LAY_LINK && pCA->pInterface->pPendingCommand->iCurrentSequenceNumber == 0)
	{

			RECT SrcRect;
			int iFrameSize = pCA->pImages[ISURF_SELECT]->iXFrameSize;
			SrcRect.left = 0;
			SrcRect.right = pCA->pImages[ISURF_SELECT]->DDSDImage.dwWidth;
			SrcRect.top = iFrameSize;
			SrcRect.bottom = iFrameSize * 2;

			if ( pCA->pGameWorld->IsValidLinkLocation(pCA->pInterface->PosMap_Mouse.x, pCA->pInterface->PosMap_Mouse.y) == FALSE)
			{
				SrcRect.top = iFrameSize * 4;
				SrcRect.bottom = iFrameSize * 5;
			}

			//blit at 0,0
			pDrawSurf->BltFast( (pCA->pInterface->PosMap_Mouse.x - pCA->pInterface->map_xview) *TILE_SIZE, (pCA->pInterface->PosMap_Mouse.y - pCA->pInterface->map_yview) * TILE_SIZE, pCA->pImages[ISURF_SELECT]->pDDSImage, &SrcRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			
	}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_Units(LPDIRECTDRAWSURFACE7 pDrawSurf)
{

	//a temporary function to display units at their correct positions(doesn't support multiple unit stuff types and whatever)
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iMapIndex; 
	int iDrawFrame = 0;
	int iEmblemFrame = 0;
	int blit_x, blit_y;
	int num_stacked_home = 0;
	int lc1; //counter
	SRCIMGFRAME* pFF = NULL;
	int c1, c2, c3; //counters


/*DEBUG
	char NText[20];
END DB*/

/*DEBUG
//Write_Log("[Disp Units] Beginning Function");
//WriteLog_Enter();
END DB*/

	//01.06.2004
	int iTop_Mod = 0;
//bottom and right are virtually not needed because the bottom and right are occupied by the interface bars
//	int iBottom_Mod = 0;
	int iLeft_Mod = 0;
//	int iRight_Mod = 0;

	cUnitA* pStackTr;  //stack traverser
	RECT  SRect, Emblem_Rect;

	for (c1 = 0; c1 <= MDRAW_NUMCOLS; c1++)
	for (c2 = 0; c2 <= MDRAW_NUMROWS; c2++)
	{
		iMapIndex = ((c2+pCA->pInterface->map_yview)* pCA->pGameWorld->iWidth) + (c1 + pCA->pInterface->map_xview);
		pStackTr = (cUnitA*)pCA->pGameWorld->pMap[iMapIndex].pOccupant_Unit;   

		//arrcheck val
		int icheck = 1;
		icheck = icheck << pCA->pGameOptions->iControl;

		if (pCA->pGameWorld->pMap[iMapIndex].barrRevealed & icheck)
		{

			num_stacked_home = 0;

		//draw all of the units stacked here
			while ( pStackTr != NULL)  //only draw the top unit
			//if (pStackTr != NULL)
				{
					//only draw the top-stacked unit (12.26.2003)
					//if (pStackTr->pPrev_Stack == NULL || pStackTr->Pos_ABS.z != 0)
					if (TRUE)
					{

						//get access to the component's type data
						cCmpInfo* pInfFrame = pCA->pCmpInfoTable->pCmpTable[pStackTr->pFrame->iType];
						cCmpInfo* pInfo;
						cComponentA* pComponent;

							iEmblemFrame = pStackTr->iControl;
							Emblem_Rect.top = (iEmblemFrame % BLIT_FPCOL) * EMBLEM_SIZE;
							Emblem_Rect.bottom = Emblem_Rect.top + EMBLEM_SIZE;
							Emblem_Rect.left = (iEmblemFrame / BLIT_FPCOL) * EMBLEM_SIZE;
							Emblem_Rect.right = Emblem_Rect.left + EMBLEM_SIZE;

					//draw all valid parts of the frame (01.07.2004)
						for (lc1 = 0; lc1 < NUM_ZLEVELS; lc1++)
						if (pInfFrame->DrawSchemes[lc1].bValid == TRUE)
						{
							//yeah...to translate from angular direction to frame directions, we do this....
							iDrawFrame = pStackTr->Calc_DrawFrame(pStackTr->fDir_Current, pInfFrame->DrawSchemes[lc1].iRotateFrames); //pStackTr->iDrawFrame;
							//get the frame information for this unit
//DEBUG
Write_Log("Getting entry information...");
WriteLog_Enter();
							
							int iRet = 0;
							SRCIMGFRAME* pImgFrame = pInfFrame->DrawSchemes[lc1].SourceGraphics.Get_Entry2( pStackTr->bModes_L1[0], iDrawFrame, pStackTr->iTimeLastL1Change, &iRet);
						
//DEBUG
char NText[50];
Write_Log("Img Surface: ");
itoa(pImgFrame->iImageSurfIndex, NText, 10);
Write_Log(NText);
WriteLog_Enter();
							//we can figure out the frame sizes from here
							int iXFrameSize = pImgFrame->Imgcoords.right - pImgFrame->Imgcoords.left;
							int iYFrameSize = pImgFrame->Imgcoords.bottom - pImgFrame->Imgcoords.top;
							//copy over the source rect because we may need to modify it

							SRect.top = pImgFrame->Imgcoords.top;
							SRect.bottom = pImgFrame->Imgcoords.bottom;
							SRect.left = pImgFrame->Imgcoords.left;
							SRect.right = pImgFrame->Imgcoords.right;
							
							blit_x = (c1*TILE_SIZE) + pStackTr->Pos_offset.x;
							blit_y = (c2*TILE_SIZE) + pStackTr->Pos_offset.y;

							//modify by half the framesize
							blit_x -= iXFrameSize / 2;
							blit_y -= iYFrameSize / 2;

							//offset by half the tilesize to compensate for the offset system(0..24 instead of -12..12)
							blit_x += TILE_SIZE / 2;
							blit_y += TILE_SIZE / 2;

							//blit shadow at this point 
							if (pStackTr->Pos_ABS.z > 0)
							{
								
								UBlitQueue.Submit_BlitRequest(lc1 + (pStackTr->Pos_ABS.z / 6), blit_x + iLeft_Mod, blit_y + iTop_Mod, pCA->pImages[pImgFrame->iImageSurfIndex]->pDDSImage, &pImgFrame->Imgcoords, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_SHADOW, pCA->pColorTable->Black, pCA->pImages[pImgFrame->iImageSurfIndex]->pExBuffer);
							}

							//now add effect of Z-value (01.16.2004)
							blit_y -= pStackTr->Pos_ABS.z * sin(TILT_ANGLE / RAD_DEG_CONV);

							//clip to fit on screen
							if (blit_x < 1)
							{
								iLeft_Mod = 1 - blit_x;
								SRect.left  += iLeft_Mod;
							}
							if (blit_y < 1)
							{
								iTop_Mod = 1 - blit_y;
								SRect.top += iTop_Mod;
							}

							//blit the unit frame at the specified location (might be temp...sigh....03.26.2003)
							//submit a blit request
							if (pInfFrame->DrawSchemes[lc1].bEmFlag == TRUE)
							{
								UBlitQueue.Submit_BlitRequest(lc1 + 1 + (pStackTr->Pos_ABS.z / 6), blit_x + iLeft_Mod, blit_y + iTop_Mod, pCA->pImages[pImgFrame->iImageSurfIndex]->pDDSImage, &SRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_OBJECT_01, pCA->pColorTable->MM_Players_ObjectMark[pStackTr->iControl], pCA->pImages[pImgFrame->iImageSurfIndex]->pExBuffer);
							} else
							{
								UBlitQueue.Submit_BlitRequest(lc1 + 1 + (pStackTr->Pos_ABS.z / 6), blit_x + iLeft_Mod, blit_y + iTop_Mod, pCA->pImages[pImgFrame->iImageSurfIndex]->pDDSImage, &SRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_NORMAL, pCA->pColorTable->MM_Players_ObjectMark[pStackTr->iControl], pCA->pImages[pImgFrame->iImageSurfIndex]->pExBuffer);
							}
							//reset
							iLeft_Mod = 0;
							iTop_Mod = 0;

							//gather the address of pImgFrame so that stuff below can use it later
							pFF = pImgFrame;
						}

					//now blit all attached components(sigh...do this later) (03.26.2003)
						int blit_x2;
						int blit_y2;

					
						for (int c19 = 0; c19 < pStackTr->iNumDrawVisibleComponents; c19++)
						{

							pComponent = pStackTr->VisibleComponents[c19].pComponent;
							pInfo = pCA->pCmpInfoTable->pCmpTable[pComponent->iType];

							//wow, that looks ugly
							//what that is basically saying, it calculates what frame direction orientation the frame is currently in, and looks in the table of offsets to determine
							//what offset position to draw the position the current component should be drawn in(I have no choice but to currently force it to tack to ZLEVEL_MAIN
							int offsetindex = pStackTr->Calc_DrawFrame(pStackTr->fDir_Current, pInfFrame->DrawSchemes[pStackTr->VisibleComponents[c19].iZLevelBase].iRotateFrames) + pStackTr->VisibleComponents[c19].iStartingIndex;
								if (offsetindex >= pInfFrame->DrawSchemes[pStackTr->VisibleComponents[c19].iZLevelBase].iRotateFrames)
									offsetindex -= pInfFrame->DrawSchemes[pStackTr->VisibleComponents[c19].iZLevelBase].iRotateFrames;
							iDrawFrame = pStackTr->Calc_DrawFrame(pComponent->fDir_Offset + pStackTr->fDir_Current, pInfo->DrawSchemes[ZLEVEL_MIDBASE].iRotateFrames);
								if (iDrawFrame >= pInfo->DrawSchemes[ZLEVEL_MIDBASE].iRotateFrames)
									iDrawFrame -= pInfo->DrawSchemes[ZLEVEL_MIDBASE].iRotateFrames;

							//get the source rectangle for the frame
							int iRet = 0;
							SRCIMGFRAME* pImgFrame = pInfo->DrawSchemes[ZLEVEL_MIDBASE].SourceGraphics.Get_Entry2(pStackTr->bModes_L1[0], iDrawFrame, pStackTr->iTimeLastL1Change, &iRet);

							//get the framesize
							int iXFrameSize = pImgFrame->Imgcoords.right - pImgFrame->Imgcoords.left;
							int iYFrameSize = pImgFrame->Imgcoords.bottom - pImgFrame->Imgcoords.top;

							//figure out destination coordinates
							//shoot for the middle of the frame before adding offset
							//
							blit_x2 = ( (c1+0.5)*TILE_SIZE) + pStackTr->Pos_offset.x;
							blit_y2 = ((c2+0.5)*TILE_SIZE) + pStackTr->Pos_offset.y;
							//add the offset, center it by subtracting half the component frame size
							blit_x2 += pStackTr->VisibleComponents[c19].pOffsets->XOffsets[offsetindex];
							blit_y2 += pStackTr->VisibleComponents[c19].pOffsets->YOffsets[offsetindex];
							blit_x2 -= iXFrameSize / 2;
							blit_y2 -= iYFrameSize / 2;  
						
							//copy the source rect on the source surface
							SRect.top = pImgFrame->Imgcoords.top;
							SRect.bottom = pImgFrame->Imgcoords.bottom;
							SRect.left = pImgFrame->Imgcoords.left;
							SRect.right = pImgFrame->Imgcoords.right;

							//clip to fit on screen
							if (blit_x2 < 1)
							{
								iLeft_Mod = 1 - blit_x2;
								SRect.left += iLeft_Mod;
							}
							if (blit_y2 < 1)
							{
								iTop_Mod = 1 - blit_y2;
								SRect.top += iTop_Mod;
							}

							if (pInfo->DrawSchemes[ZLEVEL_MIDBASE].bEmFlag == TRUE)
							{
							//finally, blit
								UBlitQueue.Submit_BlitRequest(pStackTr->VisibleComponents[c19].iZLevel + 1 + (pStackTr->Pos_ABS.z / 6), blit_x2 + iLeft_Mod, blit_y2 + iTop_Mod, pCA->pImages[pImgFrame->iImageSurfIndex]->pDDSImage, &SRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_OBJECT_01, pCA->pColorTable->MM_Players_ObjectMark[pStackTr->iControl], pCA->pImages[pImgFrame->iImageSurfIndex]->pExBuffer);
							} else
							{
								UBlitQueue.Submit_BlitRequest(pStackTr->VisibleComponents[c19].iZLevel + 1 + (pStackTr->Pos_ABS.z / 6), blit_x2 + iLeft_Mod, blit_y2 + iTop_Mod, pCA->pImages[pImgFrame->iImageSurfIndex]->pDDSImage, &SRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_NORMAL, pCA->pColorTable->MM_Players_ObjectMark[pStackTr->iControl], pCA->pImages[pImgFrame->iImageSurfIndex]->pExBuffer);
							}
							//reset
							iTop_Mod = 0;
							iLeft_Mod = 0;
	
						}
					}
					
					//blit an emblem on top of the image(if the unit is offset from home-square
					if (pStackTr->Pos_offset.x != 0 || pStackTr->Pos_offset.y != 0)
					{
						UBlitQueue.Submit_BlitRequest(NUM_ZLEVELS - 1, blit_x, blit_y, pCA->pImages[ISURF_EMBLEMS]->pDDSImage, &Emblem_Rect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_NORMAL, 0, pCA->pImages[ISURF_EMBLEMS]->pExBuffer);
						//if this is a tanker, draw the type of material contained
						if (pStackTr->Storage.iMaterialType >= 0)
						if (pStackTr->Storage.ddMaterialStored > 0)
						{
							pCA->pFontInfoTable->SetFontColor(pCA->pColorTable->Black);
							Draw_Text_BlitRequest(pDrawSurf, pCA->pMaterialInfoTable->Types[pStackTr->Storage.iMaterialType].Name, blit_x + 4, blit_y, FONT_ALPHA_STANDARD1);	
						}
					}
							else //otherwise the unit is stacked in the home position
								num_stacked_home++;


					//advance the traverser
					pStackTr = (cUnitA*)pStackTr->pNext_Stack;
				}
			pStackTr = (cUnitA*)pCA->pGameWorld->pMap[iMapIndex].pOccupant_Unit;
			int modx = 0;
			int mody = 0;

			if (pStackTr != NULL)
				{
						cCmpInfo* pInfFrame = pCA->pCmpInfoTable->pCmpTable[pStackTr->pFrame->iType];
						//emblem modifiers (12.26.2003)
						modx = (pFF->Imgcoords.right - pFF->Imgcoords.left - TILE_SIZE) / 2;
						mody = (pFF->Imgcoords.bottom - pFF->Imgcoords.top - TILE_SIZE) / 2;
				

				}

			if (num_stacked_home > 0) //blit ONE emblem if there is more than 0 units in the square
			{
					UBlitQueue.Submit_BlitRequest(NUM_ZLEVELS - 1, c1*TILE_SIZE - modx, c2*TILE_SIZE - mody, pCA->pImages[ISURF_EMBLEMS]->pDDSImage, &Emblem_Rect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_NORMAL, 0, pCA->pImages[ISURF_EMBLEMS]->pExBuffer);
					//if this is a tanker, draw the type of material contained
					if (pStackTr->Storage.iMaterialType >= 0)
					if (pStackTr->Storage.ddMaterialStored > 0)
					{
						pCA->pFontInfoTable->SetFontColor(pCA->pColorTable->Black);
						Draw_Text_BlitRequest(pDrawSurf, pCA->pMaterialInfoTable->Types[pStackTr->Storage.iMaterialType].Name, c1*TILE_SIZE - modx + 4, c2*TILE_SIZE - mody, FONT_ALPHA_STANDARD1);	
					}

			}
			if (num_stacked_home > 1)  //blit ANOTHER emblem if there is more than 1 unit in the square
			{
					UBlitQueue.Submit_BlitRequest(NUM_ZLEVELS - 1, (c1*TILE_SIZE)+2 - modx, (c2*TILE_SIZE)+2 - mody, pCA->pImages[ISURF_EMBLEMS]->pDDSImage, &Emblem_Rect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_NORMAL, 0, pCA->pImages[ISURF_EMBLEMS]->pExBuffer);
			}	

		}
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_Structures(LPDIRECTDRAWSURFACE7 pDrawSurf)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;	
	char NText[20];
	cStructureA*	pStruct;
	cStructInfo*	pStructInfo;

	int iMapIndex;
	int xblit, yblit;
	int iEmblemFrame;
	RECT Emblem_Rect;
	RECT Src_Rect;

	int c1, c2, c3; //counters

	for (c2 = 0; c2 <= MDRAW_NUMROWS; c2++)
	for (c1 = 0; c1 <= MDRAW_NUMCOLS; c1++)
	{

		iMapIndex = ((c2+pCA->pInterface->map_yview)* pCA->pGameWorld->iWidth) + (c1 + pCA->pInterface->map_xview);

		//arrcheck val
		int icheck = 1;
		icheck = icheck << pCA->pGameOptions->iControl;

		//a bit rough(03.28.2003)
		if ((pCA->pGameWorld->pMap[iMapIndex].barrRevealed & icheck) > 0)
		if (pCA->pGameWorld->pMap[iMapIndex].pOccupant_Structure != NULL)
		{

			pStruct = (cStructureA*)pCA->pGameWorld->pMap[iMapIndex].pOccupant_Structure;

			pStructInfo = pCA->pStructInfoTable->pTable[pStruct->iType];
			xblit = c1 * TILE_SIZE + pStructInfo->iXDrawOffset;
			yblit = c2 * TILE_SIZE + pStructInfo->iYDrawOffset;
		
			//TEMP 02.15.2004 (yep! Structures need an iDrawFrame) 06.18.2004
			//I'm adding some more temporary stuff because the change required here is actually
			//a more comprehensive frame-finding system for components, units and structures
			if (pStruct->fCompletion >= 1)
			{
				Src_Rect.top =  pStructInfo->iYFramesize;// pCA->pImages[pStructInfo->iImageSurfIndex]->DDSDImage.dwHeight / 2;
				Src_Rect.left = 0;
				Src_Rect.right = pStructInfo->iXFramesize; //pCA->pImages[pStructInfo->iImageSurfIndex]->DDSDImage.dwWidth;
				Src_Rect.bottom = pStructInfo->iYFramesize * 2;
			} else
				{
					Src_Rect.top = 0;
					Src_Rect.left = 0;
					Src_Rect.right = pStructInfo->iXFramesize;  //pCA->pImages[pStructInfo->iImageSurfIndex]->DDSDImage.dwWidth;
					Src_Rect.bottom = pStructInfo->iYFramesize; //pCA->pImages[pStructInfo->iImageSurfIndex]->DDSDImage.dwHeight / 2;
				}
			//"blink"
			if (pStruct->bOn == TRUE)
			if ( pCA->pTimer->iCurrTick % 60 > 30)
			{
				Src_Rect.left += pStructInfo->iXFramesize;
				Src_Rect.right += pStructInfo->iXFramesize;
			}

			//if we're drawing a (storage) tank, do something different
			/* disabled 06.12.2007
			if ( pStruct->iType == TEMP_STYPE_TANK)
			if ( pStruct->fCompletion >= 1)
			{
				double ddRatio = pStruct->Storage.ddMaterialStored / pStruct->Storage.ddMaterialCapacity;
				int iX = ddRatio * 4.1; //we deliberately want the flooring effect
				int iY = 1;
				if (pStruct->Storage.iMaterialType >= MATERIAL_ELEMENT_A)
					iY = 2;
				if (pStruct->Storage.iMaterialType >= MATERIAL_ELEMENT_F)
					iY = 3;
				if (pStruct->Storage.iMaterialType >= MATERIAL_ELEMENT_L)
					iY = 4;
		
				Src_Rect.left = pStructInfo->iXFramesize * iX;
				Src_Rect.right = Src_Rect.left + pStructInfo->iXFramesize;
				Src_Rect.top = pStructInfo->iYFramesize * iY;
				Src_Rect.bottom = Src_Rect.top + pStructInfo->iYFramesize;
			}
			*/

			//only blit if this is Pos_primary
			if (pStruct->Pos_primary.index == iMapIndex)
			{
				//TEMP (02.15.2004)
				if (pStruct->fCompletion < 1)
					UBlitQueue.Submit_BlitRequest(0, xblit, yblit, pCA->pImages[pStructInfo->iImageSurfIndex]->pDDSImage, &Src_Rect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_ALPHA, 0, 0);
				else
					UBlitQueue.Submit_BlitRequest(0, xblit, yblit, pCA->pImages[pStructInfo->iImageSurfIndex]->pDDSImage, &Src_Rect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_NORMAL, 0, 0);
				
				//blit the emblem only if it is Pos_primary
					iEmblemFrame = pStruct->iControl;
					Emblem_Rect.top = (iEmblemFrame % BLIT_FPCOL) * EMBLEM_SIZE;
					Emblem_Rect.bottom = Emblem_Rect.top + EMBLEM_SIZE;
					Emblem_Rect.left = (iEmblemFrame / BLIT_FPCOL) * EMBLEM_SIZE;
					Emblem_Rect.right = Emblem_Rect.left + EMBLEM_SIZE;
//				pDrawSurf->BltFast(xblit + 2, yblit + 2, pCA->pImages[ISURF_EMBLEMS]->pDDSImage, &Emblem_Rect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

				UBlitQueue.Submit_BlitRequest(NUM_ZLEVELS - 1, xblit + 2, yblit + 2,  pCA->pImages[ISURF_EMBLEMS]->pDDSImage, &Emblem_Rect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_NORMAL, 0,  pCA->pImages[ISURF_EMBLEMS]->pExBuffer);

			//if the structure is incomplete, list its percent completion
			if (pStruct->fCompletion < 1)
			{
						char NText[10];
						ZeroMemory(NText, sizeof(char)*10);
						itoa(double(pStruct->fCompletion * 100), NText, 10);
						strcat(NText, "%");
						char Text[20];

						Draw_Text_BlitRequest(pDrawSurf, "Progress: ", 
											  xblit + 17 , yblit + 5 , FONT_ALPHA_STANDARD1);
						Draw_Text_BlitRequest(pDrawSurf, NText, 
											  xblit + 17, yblit + 5 + pCA->pFontInfoTable->Fonts[FONT_ALPHA_STANDARD1].iHeight + 2, FONT_ALPHA_STANDARD1);
						Draw_Text_BlitRequest(pDrawSurf, NText, 
											  xblit + 17 + 1, yblit + 5 + pCA->pFontInfoTable->Fonts[FONT_ALPHA_STANDARD1].iHeight + 2, FONT_ALPHA_STANDARD1);


			}

			//only do this if the structure is finished!
			if (pStruct->fCompletion >= 1)
			if ( pStruct->iType == TEMP_STYPE_RSC || pStruct->iType == TEMP_STYPE_OUTPOST)
			{
				int iNumDrawn = 0;


				for (int lc79 = 0; lc79 < NUM_MATERIAL_TYPES; lc79++)
				{
					if (pStruct->pResourceCollection[lc79] > 0)
					{
						int iXPie = iNumDrawn % 2;
						int iYPie = iNumDrawn / 2;

						//draw the pie
						double ddFraction = (pCA->pTimer->iCurrTick - pStruct->iLastAward[lc79]) / (TEMP_MATERIAL_ACCUMULATE_MASTER_RATE / (pStruct->pResourceCollection[lc79] + 0.01));

						RECT srcRect;
						srcRect.top = 0;
						srcRect.bottom = 0 + 0 + pCA->pFontInfoTable->Fonts[FONT_ALPHA_STANDARD1].iHeight ;
						srcRect.left = 0;
						srcRect.right = 0 + 4 + pCA->pFontInfoTable->Fonts[FONT_ALPHA_STANDARD1].iWidth * strlen(pCA->pMaterialInfoTable->Types[lc79].ShortName) * 0.7;  //0.7 is  hack

						Disp_Pie(ddFraction, xblit + 5 + iXPie * 60, yblit + 10 + iYPie * 20);  
						if (pCA->pInputState->IGState_Keyboard[DIK_T] != 0)
							UBlitQueue.Submit_BlitRequest(NUM_ZLEVELS - 2, xblit + 15 + iXPie * 60, yblit + 10 + iYPie * 20 + 5, pCA->pImages[ISURF_WHITE]->pDDSImage, &srcRect, DDBLTFAST_WAIT, BQU_BLITTYPE_NORMAL, 0,  NULL);
						else
							UBlitQueue.Submit_BlitRequest(NUM_ZLEVELS - 2, xblit + 15 + iXPie * 60, yblit + 10 + iYPie * 20 + 5, pCA->pImages[ISURF_WHITE]->pDDSImage, &srcRect, DDBLTFAST_WAIT, BQU_BLITTYPE_ALPHA, 0,  NULL);
						
						pCA->pFontInfoTable->SetFontColor(pCA->pColorTable->Black);
						Draw_Text_BlitRequest(pDrawSurf, pCA->pMaterialInfoTable->Types[lc79].ShortName, 
											  xblit + 17 + iXPie * 60 + 1, yblit + 10 + iYPie * 20 + 5, FONT_ALPHA_STANDARD1);
						pCA->pFontInfoTable->SetFontColor(pCA->pColorTable->Black);
						Draw_Text_BlitRequest(pDrawSurf, pCA->pMaterialInfoTable->Types[lc79].ShortName, 
											  xblit + 17 + iXPie * 60, yblit + 10 + iYPie * 20 + 5, FONT_ALPHA_STANDARD1);

						iNumDrawn++;
					}
				}


			} else if ( pStruct->pResourceCollection != NULL) 
			{
				BOOL bDrawPie = FALSE;
				int iLastAward = -1;
				int iNextAward = -1;
				for (int lc80 = 0; lc80 < NUM_MATERIAL_TYPES; lc80++)
				{
					if (pStruct->pResourceCollection[lc80] > 0)
					{
						iLastAward = pStruct->iLastAward[lc80];
						bDrawPie = TRUE;
						break;
					}
				}
				if (bDrawPie == TRUE)
				{
					//this assignment temporary for now
					iNextAward = iLastAward + 1000;

					double ddFraction = double(pCA->pTimer->iCurrTick - iLastAward) / double(iNextAward - iLastAward);

					Disp_Pie(ddFraction, xblit + 5, yblit + 10);  
						RECT srcRect;
						srcRect.top = 0;
						srcRect.bottom = 0 + 0 + pCA->pFontInfoTable->Fonts[FONT_ALPHA_STANDARD1].iHeight;
						srcRect.left = 0;
						srcRect.right = 0 + 4 + pCA->pFontInfoTable->Fonts[FONT_ALPHA_STANDARD1].iWidth * strlen(pStruct->ProcessText) * 0.7;  //0.7 is a hack
					if (pCA->pInputState->IGState_Keyboard[DIK_T] != 0)
							UBlitQueue.Submit_BlitRequest(NUM_ZLEVELS - 2, xblit + 8 + 14, yblit + 10 + 5, pCA->pImages[ISURF_WHITE]->pDDSImage, &srcRect, DDBLTFAST_WAIT, BQU_BLITTYPE_NORMAL, 0,  NULL);
					else
							UBlitQueue.Submit_BlitRequest(NUM_ZLEVELS - 2, xblit + 8 + 14, yblit + 10 + 5, pCA->pImages[ISURF_WHITE]->pDDSImage, &srcRect, DDBLTFAST_WAIT, BQU_BLITTYPE_ALPHA, 0,  NULL);

						pCA->pFontInfoTable->SetFontColor(pCA->pColorTable->Black);
					Draw_Text_BlitRequest(pDrawSurf, pStruct->ProcessText, 
											  xblit + 8 +  16 + 1 , yblit + 10 + 5, FONT_ALPHA_STANDARD1);
					pCA->pFontInfoTable->SetFontColor(pCA->pColorTable->Black);
					Draw_Text_BlitRequest(pDrawSurf, pStruct->ProcessText, 
											  xblit + 8 +  16, yblit + 10 + 5, FONT_ALPHA_STANDARD1);

				}

			}  


			//	UBlitQueue.Dispatch_BlitRequests(pDrawSurf, 0, NUM_ZLEVELS-1);

				//if this is a tank, draw the type of material contained
				//and the amount of material inside
				//only do this if the structure is finished!
				if (pStruct->fCompletion >= 1)
				if (pStruct->iType == TEMP_STYPE_TANK)
				{
					char BlitText[80];
					char BlitText2[80];
					ZeroMemory(BlitText2, sizeof(char)*80);
					ZeroMemory(BlitText, sizeof(char)*80);

					if (pStruct->Storage.iMaterialType >= 0)
						{
							int iStorageAmt = pStruct->Storage.ddMaterialStored; //flooring is deliberate
							char NText[10];
							itoa(iStorageAmt, NText, 10);
			

							strcat(BlitText, " [");
							strcat(BlitText, NText);
							strcat(BlitText, "]");

							strcat(BlitText2, pCA->pMaterialInfoTable->Types[pStruct->Storage.iMaterialType].ShortName);
						}	else
							{
								strcat(BlitText2, "NoType");
								strcat(BlitText, "Set");
							}

						RECT srcRect;
						srcRect.top = 0;
						srcRect.bottom = 0 + 0 + pCA->pFontInfoTable->Fonts[FONT_ALPHA_STANDARD1].iHeight * 2;
						srcRect.left = 0;
						srcRect.right = 0 + 4 + pCA->pFontInfoTable->Fonts[FONT_ALPHA_STANDARD1].iWidth * strlen(BlitText2) * 0.7; //the 0.7 is a hack

					if (pCA->pInputState->IGState_Keyboard[DIK_T] != 0)
						UBlitQueue.Submit_BlitRequest(NUM_ZLEVELS - 2, xblit + 4, yblit + 5, pCA->pImages[ISURF_WHITE]->pDDSImage, &srcRect, DDBLTFAST_WAIT, BQU_BLITTYPE_NORMAL, 0,  NULL);
					else
						UBlitQueue.Submit_BlitRequest(NUM_ZLEVELS - 2, xblit + 4, yblit + 5, pCA->pImages[ISURF_WHITE]->pDDSImage, &srcRect, DDBLTFAST_WAIT, BQU_BLITTYPE_ALPHA, 0,  NULL);

					pCA->pFontInfoTable->SetFontColor(pCA->pColorTable->Black);
					Draw_Text_BlitRequest(pDrawSurf, BlitText2, xblit + 6 + 1, yblit + 5, FONT_ALPHA_STANDARD1);
					Draw_Text_BlitRequest(pDrawSurf, BlitText, xblit + 6 + 1, yblit + 5 + pCA->pFontInfoTable->Fonts[FONT_ALPHA_STANDARD1].iHeight, FONT_ALPHA_STANDARD1);
					pCA->pFontInfoTable->SetFontColor(pCA->pColorTable->Black);
					Draw_Text_BlitRequest(pDrawSurf, BlitText2, xblit + 6, yblit + 5, FONT_ALPHA_STANDARD1);
					Draw_Text_BlitRequest(pDrawSurf, BlitText, xblit + 6, yblit + 5 + pCA->pFontInfoTable->Fonts[FONT_ALPHA_STANDARD1].iHeight, FONT_ALPHA_STANDARD1);

					/* don't draw the storage bar for now, opt instead to put the amount in parenthesis
					//blit the storage capacity also (Not, right now, the "storage" bar is in the "hpbar" file, hence the funny naming
						RECT HPBar_rect2;
						HPBar_rect2.top = 3;
						HPBar_rect2.bottom = 6;
						HPBar_rect2.left = 0;
						HPBar_rect2.right = pCA->pImages[ISURF_HPBAR]->DDSDImage.dwWidth;

						RECT HPBar_rect;
						HPBar_rect.top = 7;
						HPBar_rect.bottom = 10;
						HPBar_rect.left = 0;
						double ddStorageRatio = pStruct->Storage.ddMaterialStored / pStruct->Storage.ddMaterialCapacity;
						double ddSubStorageRatio = ddStorageRatio * 3;
						if (ddStorageRatio > 0.33)//(pStruct->Storage.iMaterialType >= 10)
						{
							HPBar_rect.top += 4;
							HPBar_rect.bottom += 4;
							HPBar_rect2.top += 4;
							HPBar_rect2.bottom += 4;
							ddSubStorageRatio -= 1;
						}
						if (ddStorageRatio > 0.67)//(pStruct->Storage.iMaterialType >= 20)
						{
							HPBar_rect.top += 4;
							HPBar_rect.bottom += 4;
							HPBar_rect2.top += 4;
							HPBar_rect2.bottom += 4;
							ddSubStorageRatio -= 1;
						}
						HPBar_rect.left = 0;
						//TEMP (until unit info goes up) 
						HPBar_rect.right = pCA->pImages[ISURF_HPBAR]->DDSDImage.dwWidth * ddSubStorageRatio; 
						UBlitQueue.Submit_BlitRequest(1,xblit,yblit + TILE_SIZE*0.6, pCA->pImages[ISURF_HPBAR]->pDDSImage, &HPBar_rect2 ,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_NORMAL, 0, NULL);
						UBlitQueue.Submit_BlitRequest(NUM_ZLEVELS-1,xblit,yblit + TILE_SIZE*0.6, pCA->pImages[ISURF_HPBAR]->pDDSImage, &HPBar_rect ,DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_NORMAL, 0, NULL);
						*/
				//disabled 09.20.2006
				//Draw_Text_Shadow(pDrawSurf, pCA->pSectorTable->Sectors[pCA->pGameWorld->pMap[pStruct->Pos_primary.index].iSector]->Name, xblit + 2, yblit + 8, FONT_TEST01, pCA->pColorTable->MM_Players[pStruct->iControl],pCA->pColorTable->MM_Players_Console[pStruct->iControl]);
				}

			}

		}
	}


/*DEBUG
Write_Log("[Disp Str]Function Complete: ");
WriteLog_Enter();
END DB*/

		


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cGDisplay.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_GDISPLAY >= 1)
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
void cGDisplay::WriteLog_Enter()
{
if (DEBUG + DEBUG_GDISPLAY >= 1)
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
int cGDisplay::Draw_Text_BlitRequest(LPDIRECTDRAWSURFACE7 pDDS, char* Text, int xpos, int ypos, int iFont)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iLines = 1;
	int iHeight = pCA->pFontInfoTable->Fonts[iFont].iHeight;
	int iWidth = pCA->pFontInfoTable->Fonts[iFont].iWidth;
	int iDrawSurfIndex = pCA->pFontInfoTable->Fonts[iFont].iFontISurfIndex;
	LPDIRECTDRAWSURFACE7 pDDSBlitMask = pCA->pFontInfoTable->Fonts[iFont].pDDS_BlitMask;
	LPDIRECTDRAWSURFACE7 pDDSBlitMaskC = pCA->pFontInfoTable->Fonts[iFont].pDDS_BlitMask_Colorstore;
	LPDIRECTDRAWSURFACE7 pDDSImage = pCA->pImages[iDrawSurfIndex]->pDDSImage;


	BOOL bTextZero = pCA->pConfig->bTextZero;
	/*
	pDDS->GetDC(&hdc00);

	TextOut(hdc00, xpos, ypos, Text, strlen(Text));

	pDDS->ReleaseDC(hdc00);
	*/

	text_xdraw = xpos;
	text_ydraw = ypos;

	int c1; //local counter;

//implement a faster way to load text
	//blit the characters from the font01 image surface
	for (c1 = 0; c1 < strlen(Text); c1++)  //removing the +1 seems to work(why? I dunno 07.23.2002)
		{


		if ( int(Text[c1]) >= 32) //(ASCII 32 is space bar
		{


			//the character positions on the font image correspond exactly to ASCII text mappings so they are more or less equivalent
			//set the source rect on the source surface
			TextSrcRect.top = (((int)Text[c1]) % BLIT_FPCOL)* iHeight;
			TextSrcRect.left = (((int)Text[c1]) / BLIT_FPCOL)*iWidth;
			//change all text to 1s and zeros
			if (bTextZero == TRUE)
			if (int(Text[c1]) > 32)
			{
				if ( (Text[c1] % 2) == 0)
				{
					TextSrcRect.top = (((int)'1') % BLIT_FPCOL)* iHeight;
					TextSrcRect.left = (((int)'1') / BLIT_FPCOL)*iWidth;
				} else
				{
					TextSrcRect.top = (((int)'0') % BLIT_FPCOL)* iHeight;
					TextSrcRect.left = (((int)'0') / BLIT_FPCOL)*iWidth;
				}
			}

			TextSrcRect.bottom = TextSrcRect.top + iHeight;
			TextSrcRect.right = TextSrcRect.left + iWidth;
		//	TextSrcRect.top = 0;//(((int)Text[c1]) % BLIT_FPCOL)*iHeight_Font;
	//		TextSrcRect.left = 0;//(((int)Text[c1]) / BLIT_FPCOL)*iWidth_Font;
	//		TextSrcRect.bottom = 30; //TextSrcRect.top + iHeight_Font;
	//		TextSrcRect.right = 30; //TextSrcRect.left + iWidth_Font;

			//colorfill font's BlitMask surface to text color
			pCA->pDD_Display->Colorfill2(pDDSBlitMask, dwFontColor);

			//fill the blitmask with the correct color
	//		pDDSBlitMask->BltFast(0 , 0, pDDSBlitMaskC, NULL, DDBLTFAST_WAIT);
			//blit the appropriate box from the source text to the blitmask
			pDDSBlitMask->BltFast(0 , 0, pDDSImage, &TextSrcRect, DDBLTFAST_WAIT |DDBLTFAST_SRCCOLORKEY);
			UBlitQueue.Submit_BlitRequest(NUM_ZLEVELS-1, text_xdraw, text_ydraw, pDDSImage, &TextSrcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT, BQU_BLITTYPE_NORMAL, 0, NULL);
			//pDDS->BltFast(text_xdraw, text_ydraw, pDDSBlitMask, NULL,  DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			//UBlitQueue.Submit_BlitRequest(NUM_ZLEVELS-1, text_xdraw, text_ydraw, pDDSBlitMask, NULL, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY, BQU_BLITTYPE_NORMAL, 0, NULL);
			//pDDS->BltFast(text_xdraw, text_ydraw, pDDSImage, &TextSrcRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

			//advance the character draw positions
			if (bTextZero == FALSE)
				text_xdraw = text_xdraw + iWidth + pCA->pFontInfoTable->Fonts[iFont].CharacterOffsetTable[Text[c1]];
			else
				text_xdraw = text_xdraw + iWidth - 3;
		}
		//if has a 10 (carriage return), go to the next line
		if ( int(Text[c1]) == 10)
		{
				text_xdraw = xpos;
				text_ydraw += iHeight;
				iLines++;			
		}

		//if it passed the end of the screen, return
		if (text_xdraw > pCA->pDD_Display->i_XResD - iWidth)
			{
				break;
				//disable the wrapping feature; just break out of the loop
//				text_xdraw = xpos;
//				text_ydraw += iHeight;
//				iLines++;
			}
		}
	
//now draw the cursor
	if (Text != NULL)
	if (Text == pCA->pInterface->pKeyEdit)
	{
		Draw_TextCursor(pDDS, iFont, text_xdraw, text_ydraw);
	}

return iLines;  //returns the number of lines the drawtext took

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cGDisplay::Draw_Text(LPDIRECTDRAWSURFACE7 pDDS, char* Text, int xpos, int ypos, int iFont)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	return Draw_Text(pDDS, Text, xpos, ypos, iFont, pCA->pDD_Display->i_XResD);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cGDisplay::Draw_Text(LPDIRECTDRAWSURFACE7 pDDS, char* Text, int xpos, int ypos, int iFont, int iXLimit)
{


	cClassAccess* pCA = (cClassAccess*)pCAccess;

	BOOL bTextZero = pCA->pConfig->bTextZero;

	int iLines = 1;
	int iHeight = pCA->pFontInfoTable->Fonts[iFont].iHeight;
	int iWidth = pCA->pFontInfoTable->Fonts[iFont].iWidth;
	int iDrawSurfIndex = pCA->pFontInfoTable->Fonts[iFont].iFontISurfIndex;
	LPDIRECTDRAWSURFACE7 pDDSBlitMask = pCA->pFontInfoTable->Fonts[iFont].pDDS_BlitMask;
	LPDIRECTDRAWSURFACE7 pDDSBlitMaskC = pCA->pFontInfoTable->Fonts[iFont].pDDS_BlitMask_Colorstore;
	LPDIRECTDRAWSURFACE7 pDDSImage = pCA->pImages[iDrawSurfIndex]->pDDSImage;

	/*
	pDDS->GetDC(&hdc00);

	TextOut(hdc00, xpos, ypos, Text, strlen(Text));

	pDDS->ReleaseDC(hdc00);
	*/

	text_xdraw = xpos;
	text_ydraw = ypos;

	int c1; //local counter;

	BOOL bBold = FALSE;
//implement a faster way to load text
	//blit the characters from the font01 image surface
	for (c1 = 0; c1 < strlen(Text); c1++)  //removing the +1 seems to work(why? I dunno 07.23.2002)
		{


		//if this Text[c1] contains the escape character(~) and the next character contains the b key, toggle "bold" mode
		//and increment 2
		if (Text[c1] == '~')
		if (Text[c1+1] == 'b')
		{
			bBold = !bBold;
			if ( (c1 + 2) < strlen(Text))
				c1 += 2; //jump 2
			else
				break;
		}
		if ( int(Text[c1]) >= 32)  //ASCII 32 is space bar
 		{

			//the character positions on the font image correspond exactly to ASCII text mappings so they are more or less equivalent
			//set the source rect on the source surface
			TextSrcRect.top = (((int)Text[c1]) % BLIT_FPCOL)* iHeight;
			TextSrcRect.left = (((int)Text[c1]) / BLIT_FPCOL)*iWidth;
			//change all text to 1s and zeros
			if (bTextZero == TRUE)
			if (int(Text[c1]) > 32)
			{
				if ( (Text[c1] % 2) == 0)
				{
					TextSrcRect.top = (((int)'1') % BLIT_FPCOL)* iHeight;
					TextSrcRect.left = (((int)'1') / BLIT_FPCOL)*iWidth;
				} else
				{
					TextSrcRect.top = (((int)'0') % BLIT_FPCOL)* iHeight;
					TextSrcRect.left = (((int)'0') / BLIT_FPCOL)*iWidth;
				}
			}
			TextSrcRect.bottom = TextSrcRect.top + iHeight;
			TextSrcRect.right = TextSrcRect.left + iWidth;
		//	TextSrcRect.top = 0;//(((int)Text[c1]) % BLIT_FPCOL)*iHeight_Font;
	//		TextSrcRect.left = 0;//(((int)Text[c1]) / BLIT_FPCOL)*iWidth_Font;
	//		TextSrcRect.bottom = 30; //TextSrcRect.top + iHeight_Font;
	//		TextSrcRect.right = 30; //TextSrcRect.left + iWidth_Font;

			//colorfill font's BlitMask surface to text color
			pCA->pDD_Display->Colorfill2(pDDSBlitMask, dwFontColor);

			//fill the blitmask with the correct color
	//		pDDSBlitMask->BltFast(0 , 0, pDDSBlitMaskC, NULL, DDBLTFAST_WAIT);
			//blit the appropriate box from the source text to the blitmask
			pDDSBlitMask->BltFast(0 , 0, pDDSImage, &TextSrcRect, DDBLTFAST_WAIT |DDBLTFAST_SRCCOLORKEY);
			pDDS->BltFast(text_xdraw, text_ydraw, pDDSBlitMask, NULL,  DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			//draw another if BOLD is enabled
			if (bBold == TRUE)
				pDDS->BltFast(text_xdraw + 1, text_ydraw, pDDSBlitMask, NULL,  DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

			//pDDS->BltFast(text_xdraw, text_ydraw, pDDSImage, &TextSrcRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

			//advance the character draw positions
			if (bTextZero == FALSE)
			text_xdraw = text_xdraw + iWidth + pCA->pFontInfoTable->Fonts[iFont].CharacterOffsetTable[Text[c1]];
			else
				text_xdraw = text_xdraw + iWidth - 3;
		}


		//if has a 10 (carriage return), go to the next line
		if ( int(Text[c1]) == 10)
		{
				text_xdraw = xpos;
				text_ydraw += iHeight;
				iLines++;			
		}
		//if we are within 2 character of passing the xlimit, go to the next line
		if (iXLimit > 0)
		if (Text[c1] == ' ')
		if ( (text_xdraw + (iWidth*2)) > iXLimit)
		{
				text_xdraw = xpos;
				text_ydraw += iHeight;
				iLines++;			
		}

		
		//if it passed the end of the screen, return
		if (text_xdraw > pCA->pDD_Display->i_XResD - iWidth)
			{
				break;
				//disable the wrapping feature; just break out of the loop
//				text_xdraw = xpos;
//				text_ydraw += iHeight;
//				iLines++;
			}
		}
	
//now draw the cursor
	if (Text != NULL)
	if (Text == pCA->pInterface->pKeyEdit)
	{
		Draw_TextCursor(pDDS, iFont, text_xdraw, text_ydraw);
	}

return iLines;  //returns the number of lines the drawtext took

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cGDisplay::Draw_Text_Shadow(LPDIRECTDRAWSURFACE7 pDDS, char* Text, int xpos, int ypos, int iFont, DWORD dwTextColor, DWORD dwShadowColor)
{
	DWORD OldColor = dwFontColor;

		cClassAccess* pCA = (cClassAccess*)pCAccess;

	pCA->pFontInfoTable->SetFontColor(dwShadowColor);
	Draw_Text(pDDS, Text, xpos+1,ypos+1, iFont);
	pCA->pFontInfoTable->SetFontColor(dwTextColor);
	int iRet = Draw_Text(pDDS, Text, xpos, ypos, iFont);
	pCA->pFontInfoTable->SetFontColor(OldColor);

	return iRet;
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cGDisplay::Draw_Text_Wide(LPDIRECTDRAWSURFACE7 pDDS, unsigned short* Text, int xpos, int ypos, int iFont)
{
cClassAccess* pCA = (cClassAccess*)pCAccess;
int iLines = 1;

	text_xdraw = xpos;
	text_ydraw = ypos;
	char* cText;
	int iHeight = pCA->pFontInfoTable->Fonts[iFont].iHeight;
	int iWidth = pCA->pFontInfoTable->Fonts[iFont].iWidth;
	int iDrawSurfIndex = pCA->pFontInfoTable->Fonts[iFont].iFontISurfIndex;
	LPDIRECTDRAWSURFACE7 pDDSBlitMask = pCA->pFontInfoTable->Fonts[iFont].pDDS_BlitMask;
	LPDIRECTDRAWSURFACE7 pDDSBlitMaskC = pCA->pFontInfoTable->Fonts[iFont].pDDS_BlitMask_Colorstore;
	LPDIRECTDRAWSURFACE7 pDDSImage = pCA->pImages[iDrawSurfIndex]->pDDSImage;

	int c1, c3; //counters

//implement a faster way to load text
	//blit the characters from the font01 image surface
	for (c1 = 0; c1 <= lstrlenW(reinterpret_cast<LPCWSTR>(Text))-1; c1++)
		{
		if (int(Text[c1]) >= 32)  //ASCII 32 is space bar
		{
			//the character positions on the font image correspond exactly to ASCII text mappings so they are more or less equivalent
			//set the source rect on the source surface
			cText = (char*)Text;
			TextSrcRect.top = (((int)cText[c1*2]) % BLIT_FPCOL)*iHeight;
			TextSrcRect.left = (((int)cText[c1*2]) / BLIT_FPCOL)*iWidth;
			TextSrcRect.bottom = TextSrcRect.top + iHeight;
			TextSrcRect.right = TextSrcRect.left + iWidth;
			//fill the blitmask with the correct color
	//		pDDSBlitMask->BltFast(0 , 0, pDDSBlitMaskC, NULL, DDBLTFAST_WAIT);
			//colorfill font's BlitMask surface to text color
			pCA->pDD_Display->Colorfill2(pDDSBlitMask, dwFontColor);
			//blit the appropriate box from the source text to the blitmask
			pDDSBlitMask->BltFast(0 , 0, pDDSImage, &TextSrcRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			pDDS->BltFast(text_xdraw, text_ydraw, pDDSBlitMask, NULL, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);		
			//advance the character draw positions
			text_xdraw = text_xdraw + iWidth  + pCA->pFontInfoTable->Fonts[iFont].CharacterOffsetTable[Text[c1]];
		}
		//if has a 10 (carriage return), go to the next line
		if ( int(Text[c1]) == 10)
		{
				text_xdraw = xpos;
				text_ydraw += iHeight;
				iLines++;			
		}
		//if it passed the end of the screen, return
		if (text_xdraw > pCA->pDD_Display->i_XResD - iWidth)
			{
				break;
//				text_xdraw = xpos;
//				text_ydraw += iHeight;
//				iLines++;
			}
	
		}

//now draw the cursor
	if (Text != NULL)
	if (Text == reinterpret_cast<unsigned short*>(pCA->pInterface->pKeyEdit_Wide))
	{
		Draw_TextCursor(pDDS, iFont, text_xdraw, text_ydraw);
	}


	return iLines;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_RunText(LPDIRECTDRAWSURFACE7 pDrawSurf)
{
	//I think this will all be TEMPORARY, this is here to provide diagnostic control while programming this thing
	char NText[50];  //will be used to hold itoas and _gcvt strings
	LPSTR Text01;
	
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	int iHeight_Font = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight;
	int iWidth_Font = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iWidth;

	pCA->pFontInfoTable->SetFontColor(pCA->pColorTable->Green);

	//write the x position and y position of the mouse to screen
//	Draw_Text(pDrawSurf, "Mouse Pos:", (int)(pCA->pDD_Display->i_XResD*(MDRAW_XSCR + 0.04)),(int)(pCA->pDD_Display->i_XResD*(MMDRAW_YSCR + MMDRAW_YSCR_START + 0.04)) , FONT_STANDARD1);

//	itoa(pCA->pInputState->IGState_Mouse.iXPos, NText, 10);
//	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD*(MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_XResD*(MMDRAW_YSCR + MMDRAW_YSCR_START + 0.04)) + (iHeight_Font), FONT_STANDARD1);
//	itoa(pCA->pInputState->IGState_Mouse.iYPos, NText, 10);
//	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD*(MDRAW_XSCR + 0.04))+ (5*iWidth_Font), (int)(pCA->pDD_Display->i_XResD*(MMDRAW_YSCR + MMDRAW_YSCR_START + 0.04)) + (iHeight_Font), FONT_STANDARD1);	

	//write the last projectile ID assigned

	//write the number of counts it took everything to execute
//	Draw_Text(pDrawSurf, "Times:",(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)) , (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04)), FONT_STANDARD1);	


//write REQUEST INDEX and SEQUENCE NUMBER
//	if (pCA->pInterface->pPendingCommand != NULL)
//	{
//		itoa(pCA->pInterface->pPendingCommand->iRequestIndex, NText, 10);
//		Draw_Text(pDrawSurf, NText, 1, 40, FONT_STANDARD1);		
//		itoa(pCA->pInterface->pPendingCommand->iCurrentSequenceNumber, NText, 10);
//		Draw_Text(pDrawSurf, NText, 1, 60, FONT_STANDARD1);	

//	}


/*DEBUG...update DDCAPS  END DB*/
/*	pCA->pDD_Display->pDD7->GetCaps(&pCA->pDD_Display->SystemCaps, NULL);
	ultoa(pCA->pDD_Display->SystemCaps.dwVidMemFree, NText, 10);
//	Draw_Text(pDrawSurf, NText,(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04))+ iHeight_Font, FONT_STANDARD1);	

	_gcvt(double(pCA->pTimer->lTimeElapsed[TIMER_MAPDRAW])/pCA->pTimer->lFrequency, 5, NText);
	Draw_Text(pDrawSurf, NText,(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04))+ iHeight_Font, FONT_STANDARD1);	
	Draw_Text(pDrawSurf, "MAPDRAW:",(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)) - (iWidth_Font * 9) , (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04))+ iHeight_Font, FONT_STANDARD1);	

	itoa(pCA->pTimer->lTimeElapsed[TIMER_PROCESS_UNITS], NText, 10);
	Draw_Text(pDrawSurf, NText,(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04))+ iHeight_Font * 2, FONT_STANDARD1);	
	Draw_Text(pDrawSurf, "PRUNITS:",(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)) - (iWidth_Font * 9) , (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04))+ (iHeight_Font*2), FONT_STANDARD1);	

	itoa(pCA->pTimer->lTimeElapsed[TIMER_PATHFIND], NText, 10);
	Draw_Text(pDrawSurf, NText,(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04))+ (iHeight_Font*3), FONT_STANDARD1);	
	Draw_Text(pDrawSurf, "PATHFIND:",(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)) - (iWidth_Font * 9) , (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04))+ (iHeight_Font*3), FONT_STANDARD1);	
	
	itoa(pCA->pTimer->lTimeElapsed[TIMER_PF_A], NText, 10);
	Draw_Text(pDrawSurf, NText,(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04))+ (iHeight_Font*4), FONT_STANDARD1);	
	Draw_Text(pDrawSurf, "PFA:",(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)) - (iWidth_Font * 9) , (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04))+ (iHeight_Font*4), FONT_STANDARD1);	

	itoa(pCA->pTimer->lTimeElapsed[TIMER_AI], NText, 10);
	Draw_Text(pDrawSurf, NText,(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04))+ (iHeight_Font*5), FONT_STANDARD1);	
	Draw_Text(pDrawSurf, "AI:",(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)) - (iWidth_Font * 9) , (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04))+ (iHeight_Font*5), FONT_STANDARD1);	


	itoa(pCA->pTimer->lTimeElapsed[TIMER_DDFLIP], NText, 10);
	Draw_Text(pDrawSurf, NText,(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04))+ (iHeight_Font*6), FONT_STANDARD1);	
	Draw_Text(pDrawSurf, "DDFLIP:",(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)) - (iWidth_Font * 9) , (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04))+ (iHeight_Font*6), FONT_STANDARD1);	

	
	itoa(pCA->pTimer->lTimeElapsed[TIMER_TEST1A], NText, 10);
	Draw_Text(pDrawSurf, NText,(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04))+ (iHeight_Font*7), FONT_STANDARD1);	
	Draw_Text(pDrawSurf, "TEST1A:",(int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)) - (iWidth_Font * 9) , (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04))+ (iHeight_Font*7), FONT_STANDARD1);	



	_gcvt((double(pCA->pTimer->iCurrTick) * double( pCA->pTimer->lFrequency)) / double(pCA->pTimer->lCurrTime - pCA->pTimer->lStartTime)  , 4, NText);
//	itoa(pCA->pTimer->iCurrExecCount, NText, 10);
//	Draw_Text(pDrawSurf, "Actual Ticks/Sec:", 1, (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04)), FONT_STANDARD1 );
//	Draw_Text(pDrawSurf, NText, 1, (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04)) + iHeight_Font, FONT_STANDARD1);
	ultoa((long)(pCA->pTimer->lFrequency / pCA->pTimer->iCountsPerTick), NText, 10);
	Draw_Text(pDrawSurf, "Ticks/Sec:", 1, (int)(pCA->pDD_Display->i_YResD * (0.04)) + (iHeight_Font*2), FONT_STANDARD1);
	Draw_Text(pDrawSurf, NText, 1, (int)(pCA->pDD_Display->i_YResD * (0.04)) + (iHeight_Font*3), FONT_STANDARD1);
	itoa(pCA->pTimer->iCurrTick, NText, 10);
//	Draw_Text(pDrawSurf, "Current Tick Count:", 1, (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04)) + (iHeight_Font*4), FONT_STANDARD1);
//	Draw_Text(pDrawSurf, NText, 1, (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04)) + (iHeight_Font*5), FONT_STANDARD1);
	_gcvt((double(pCA->pTimer->iCurrFrame) * double( pCA->pTimer->lFrequency)) / double(pCA->pTimer->lCurrTime - pCA->pTimer->lStartTime)  , 4, NText);
	Draw_Text(pDrawSurf, "Frames/Sec:", 1, (int)(pCA->pDD_Display->i_YResD * (0.04)) + (iHeight_Font*6), FONT_STANDARD1 );
	Draw_Text(pDrawSurf, NText, 1, (int)(pCA->pDD_Display->i_YResD * (0.04)) + (iHeight_Font*7), FONT_STANDARD1);
	_gcvt(double(pCA->pTimer->lTimeElapsed[TIMER_MAPDRAW])/ double(pCA->pTimer->lFrequency), 7, NText);
	Draw_Text(pDrawSurf, "MAPDRAW:", 1, (int)(pCA->pDD_Display->i_YResD * (0.04)) + (iHeight_Font*8), FONT_STANDARD1 );
	Draw_Text(pDrawSurf, NText, 1, (int)(pCA->pDD_Display->i_YResD * (0.04)) + (iHeight_Font*9), FONT_STANDARD1);
	_gcvt(double(pCA->pTimer->lTimeElapsed[TIMER_EXECT] * 60 ) / double(pCA->pTimer->lFrequency), 7, NText);
	Draw_Text(pDrawSurf, "EXEC TICK:", 1, (int)(pCA->pDD_Display->i_YResD * (0.04)) + (iHeight_Font*10), FONT_STANDARD1 );
	Draw_Text(pDrawSurf, NText, 1, (int)(pCA->pDD_Display->i_YResD * (0.04)) + (iHeight_Font*11), FONT_STANDARD1);
	_gcvt(double(pCA->pTimer->lTimeElapsed[TIMER_EXECF]) / double(pCA->pTimer->lFrequency), 7, NText);
	Draw_Text(pDrawSurf, "EXEC FRAME:", 1, (int)(pCA->pDD_Display->i_YResD * (0.04)) + (iHeight_Font*12), FONT_STANDARD1 );
	Draw_Text(pDrawSurf, NText, 1, (int)(pCA->pDD_Display->i_YResD * (0.04)) + (iHeight_Font*13), FONT_STANDARD1);
	_gcvt(double(pCA->pTimer->lFrequency) / double(pCA->pTimer->lTimeElapsed[TIMER_EXECF] + pCA->pTimer->lTimeElapsed[TIMER_EXECF_GAP]), 7, NText);
		pCA->pTimer->Write_Log(NText);
		pCA->pTimer->WriteLog_Enter();
*/
/*

for (int lc9 = 0; lc9 < TEMP_DP_MAXPLAYERS; lc9++)
{
//	itoa(pCA->pTimer->iTickReports[lc9], NText, 10);
//	Draw_Text(pDrawSurf, "Tick Reports: ", 200, (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04)) + (iHeight_Font*0), FONT_STANDARD1);
//	Draw_Text(pDrawSurf, NText, 200 + (lc9 * iWidth_Font * 9), (int)(pCA->pDD_Display->i_YResD * (MDRAW_YSCR + 0.04)) + (iHeight_Font*1), FONT_STANDARD1);
}
	
//	itoa(sizeof(cUnitA), NText, 10);
//	Draw_Text(pDrawSurf, NText, 1, 1);
/* 
	if (pCA->pInterface->mouse_currbox == IBOX_PRIMARY)
		Draw_Text(pDrawSurf, "IBOX_PRIMARY", 1, 1, FONT_STANDARD1);
	if (pCA->pInterface->mouse_currbox == IBOX_NOWHERE)
		Draw_Text(pDrawSurf, "IBOX_NOWHERE", 1, 1, FONT_STANDARD1);
	if (pCA->pInterface->mouse_currbox == IBOX_MAINMENU)
		Draw_Text(pDrawSurf, "IBOX_MAINMENU", 1, 1, FONT_STANDARD1);
	if (pCA->pInterface->mouse_currbox == IBOX_MINIMAP)
		Draw_Text(pDrawSurf, "IBOX_MINIMAP", 1, 1, FONT_STANDARD1);
	if (pCA->pInterface->mouse_currbox == IBOX_MINIMAPZOOM)
		Draw_Text(pDrawSurf, "IBOX_MINIMAPZOOM", 1, 1, FONT_STANDARD1);
	if (pCA->pInterface->mouse_currbox == IBOX_CONSOLE_EQUIP)
		Draw_Text(pDrawSurf, "IBOX_CONSOLE_EQUIP", 1, 1, FONT_STANDARD1);
	if (pCA->pInterface->mouse_currbox == IBOX_CONSOLE_COMMAND)
		Draw_Text(pDrawSurf, "IBOX_CONSOLE_COMMAND", 1, 1, FONT_STANDARD1);
	if (pCA->pInterface->mouse_currbox == IBOX_CONSOLE_LIST)
		Draw_Text(pDrawSurf, "IBOX_CONSOLE_LIST", 1, 1, FONT_STANDARD1);
	if (pCA->pInterface->mouse_currbox == IBOX_CONSOLE_INFO)
		Draw_Text(pDrawSurf, "IBOX_CONSOLE_INFO", 1, 1, FONT_STANDARD1);
*/	
//	if (pCA->pInterface->mouse_currgrid == IGRID_BOTTOM)
//		Draw_Text(pDrawSurf, "IGRID_BOTTOM", 1, 1);   

	//write the last unit ID assigned to projectiles
//	Draw_Text(pDrawSurf, "Projectiles Last ID Assigned:", 0, 0);
//	itoa(pCA->pProjList->iID_Last_Assigned, NText, 10);
//	Draw_Text(pDrawSurf, NText, 0, iHeight_Font);



	
/*
	LPSBE pUSB_Trav = pCA->pInterface->pUSB_First;
	int c0 = 1;

	while (pUSB_Trav != NULL)
	{
		itoa(pUSB_Trav->iID, NText, 10);
		Draw_Text(pDrawSurf, NText, 1, 16*c0, FONT_STANDARD1);
		pUSB_Trav = (LPSBE)pUSB_Trav->pNext;
		c0++;
	}
	pUSB_Trav = pCA->pInterface->pSSB_First;
	c0 = 1;
	while (pUSB_Trav != NULL)
	{
		itoa(pUSB_Trav->iID, NText, 10);
		Draw_Text(pDrawSurf, NText, 40, 16*c0, FONT_STANDARD1);
		pUSB_Trav = (LPSBE)pUSB_Trav->pNext;
		c0++;
	}
*/
	//itoa(pCA->pInterface->PosMap_Mouse.y, NText, 10);
	//Draw_Text(pDrawSurf, NText, 40, 1);
/*
	if (pCA->pInterface->iMode == IFCM_SELECTION)
		Draw_Text(pDrawSurf, "IFCM_SELECTION", 1, 20, FONT_STANDARD1);
	if (pCA->pInterface->iMode == IFCM_UCOMMAND)
		Draw_Text(pDrawSurf, "IFCM_UCOMMAND", 1, 20, FONT_STANDARD1);
	if (pCA->pInterface->iMode == IFCM_SCOMMAND)
		Draw_Text(pDrawSurf, "IFCM_UCOMMAND", 1, 20, FONT_STANDARD1);
	if (pCA->pInterface->iMode == IFCM_SELECT_EXTENDED)
		Draw_Text(pDrawSurf, "IFCM_SELECT_EXTENDED", 1, 20, FONT_STANDARD1);
*/

if (LPSBE(pCA->pInterface->pUSB_First->pNext) != pCA->pInterface->pUSB_Last)
{
	cUnitA* pUnit = pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID];

/*
	if (pUnit->pGuardInfo != NULL)
	{
		itoa(pUnit->pGuardInfo->Pos_guard.x, NText, 10);
//		Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) - (iHeight_Font*2), FONT_STANDARD1);
		itoa(pUnit->pGuardInfo->Pos_guard.y, NText, 10);
//		Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) - (iHeight_Font*1), FONT_STANDARD1);
	}
		
	itoa(pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->Pos_sqcurr.x, NText, 10);
//	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*0), FONT_STANDARD1);
	itoa(pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->Pos_sqcurr.y, NText, 10);
//	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*1), FONT_STANDARD1);
	itoa(pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->Pos_sqimtarget.x, NText, 10);
//	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*2), FONT_STANDARD1);
	itoa(pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->Pos_sqimtarget.y, NText, 10);
//	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*3), FONT_STANDARD1);
	itoa(pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->Pos_sqtarget.x, NText, 10);
//	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*4), FONT_STANDARD1);
	itoa(pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->Pos_sqtarget.y, NText, 10);
//	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*5), FONT_STANDARD1);

	cUnitA* pUnit2 = pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID];
	itoa(pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->fDir_Current, NText, 10);
//	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*13), FONT_STANDARD1);
	itoa(pUnit2->pCurrentWeapon->fDir_Offset + pUnit2->fDir_Current, NText, 10);
//	itoa(40, NText, 10);
//	Draw_Text(pDrawSurf, NText, 1, 1, FONT_STANDARD1);

	itoa(pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->fDir_Target, NText, 10);
//	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*14), FONT_STANDARD1);
	_gcvt(pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->Pos_ABS.x, 10, NText);
//	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*15), FONT_STANDARD1);
	_gcvt(pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->Pos_ABS.y, 10, NText);
//	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*16), FONT_STANDARD1);
*/
	pCA->pFontInfoTable->SetFontColor(pCA->pColorTable->White-1);

	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L6 == UMODE_L6_NOTHING)
		Text01 = "UMODE_L6_NOTHING";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L6 == UMODE_L6_DISTRACTED)
		Text01 = "UMODE_L6_DISTRACTED";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L6 == UMODE_L6_IDLE)
		Text01 = "UMODE_L6_IDLE";
	Draw_Text(pDrawSurf, Text01, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*5), FONT_STANDARD1);


	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L5 == UMODE_L5_NOTHING)
		Text01 = "UMODE_L5_NOTHING";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L5 == UMODE_L5_GUARD)
		Text01 = "UMODE_L5_GUARD";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L5 == UMODE_L5_IDLE)
		Text01 = "UMODE_L5_IDLE";
	Draw_Text(pDrawSurf, Text01, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*6), FONT_STANDARD1);


	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L4 == UMODE_L4_NOTHING)
		Text01 = "UMODE_L4_NOTHING";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L4 == UMODE_L4_ATTACK)
		Text01 = "UMODE_L4_ATTACK";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L4 == UMODE_L4_IDLE)
		Text01 = "UMODE_L4_IDLE";
	Draw_Text(pDrawSurf, Text01, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*7), FONT_STANDARD1);


	
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L3 == UMODE_L3_MOVE)
		Text01 = "UMODE_L3_MOVE";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L3 == UMODE_L3_ATTACK)
		Text01 = "UMODE_L3_ATTACK";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L3 == UMODE_L3_IDLE)
		Text01 = "UMODE_L3_IDLE";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L3 == UMODE_L3_APPROACH)
		Text01 = "UMODE_L3_APPROACH";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L3 == UMODE_L3_NOTHING)
		Text01 = "UMODE_L3_NOTHING";
	Draw_Text(pDrawSurf, Text01, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*8), FONT_STANDARD1);


	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L2 == UMODE_L2_MOVE)
		Text01 = "UMODE_L2_MOVE";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L2 == UMODE_L2_IDLE)
		Text01 = "UMODE_L2_IDLE";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L2 == UMODE_L2_ATTACK_PROJECTILE)
		Text01 = "UMODE_L2_ATTACK_PROJECTILE";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bMode_L2 == UMODE_L2_NOTHING)
		Text01 = "UMODE_L2_NOTHING";
	Draw_Text(pDrawSurf, Text01, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*9), FONT_STANDARD1);

	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bModes_L1[0] == UMODE_L1_TRANSLATE)
		Text01 = "UMODE_L1_TRANSLATE";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bModes_L1[0] == UMODE_L1_TURN)
		Text01 = "UMODE_L1_TURN";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bModes_L1[0] == UMODE_L1_IDLE)
		Text01 = "UMODE_L1_IDLE";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bModes_L1[0] == UMODE_L1_AIM)
		Text01 = "UMODE_L1_AIM";
	if (pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->bModes_L1[0] == UMODE_L1_FIRE_PROJECTILE)
		Text01 = "UMODE_L1_FIRE";
	Draw_Text(pDrawSurf, Text01, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*10), FONT_STANDARD1);

	//put the UNit's ID to screen
	itoa(LPSBE(pCA->pInterface->pUSB_First->pNext)->iID, NText, 10);
	Draw_Text(pDrawSurf, "Unit ID: ", (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*14), FONT_STANDARD1);
	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*15), FONT_STANDARD1);
	//and its target
	itoa(pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->Pos_sqtarget.x, NText, 10);
	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*16), FONT_STANDARD1);
	itoa(pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->Pos_sqtarget.y, NText, 10);
	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)) + (iWidth_Font*5), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*16), FONT_STANDARD1);
	itoa(pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->L6_sqOriginalTarget.x, NText, 10);
	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*17), FONT_STANDARD1);
	itoa(pCA->pUnitTable->pUnitTable[LPSBE(pCA->pInterface->pUSB_First->pNext)->iID]->L6_sqOriginalTarget.y, NText, 10);
	Draw_Text(pDrawSurf, NText, (int)(pCA->pDD_Display->i_XResD * (MDRAW_XSCR + 0.04)) + (iWidth_Font*5), (int)(pCA->pDD_Display->i_YResD * 0.5) + (iHeight_Font*17), FONT_STANDARD1);


}


	RECT val;
	val.left = 0;
	val.top = 620;
	val.bottom = 630;
	double GDRAWval = double(pCA->pTimer->lTimeElapsed[TIMER_GDRAW]) / double(pCA->pTimer->lFrequency);
	val.right = 50 * (GDRAWval / 0.03);

	char Buffer[50];
	ZeroMemory(Buffer, sizeof(char)*50);
	gcvt(double(pCA->pTimer->lTimeElapsed[TIMER_GDRAW]) / double(pCA->pTimer->lFrequency), 4, NText);
	strcat(Buffer, "GDRAW: ");
	strcat(Buffer, NText);
	Draw_Text(pDrawSurf, Buffer, 1, 600, FONT_STANDARD1 );
	pCA->pDD_Display->Colorfill3(pDrawSurf, RGB(255,255,255), val.top, val.bottom, val.left, val.right);

	double GTICKval = double(pCA->pTimer->lTimeElapsed[TIMER_GTICK]) / double(pCA->pTimer->lFrequency);
	val.left = 0;
	val.top = 660;
	val.bottom = 670;
	val.right = 50 * GTICKval / 0.00003;


	ZeroMemory(Buffer, sizeof(char)*50);
	gcvt(double(pCA->pTimer->lTimeElapsed[TIMER_GTICK]) / double(pCA->pTimer->lFrequency), 4, NText);
	strcat(Buffer, "GTICK: ");
	strcat(Buffer, NText);
	Draw_Text(pDrawSurf, Buffer, 1, 640, FONT_STANDARD1 );
	pCA->pDD_Display->Colorfill3(pDrawSurf, RGB(255,255,255), val.top, val.bottom, val.left, val.right);

double lcount = 0;

	double DispUnitval = double(pCA->pTimer->lTimeElapsed[TIMER_DISP_UNITS]) / double(pCA->pTimer->lFrequency);
	val.left = lcount;
	val.top = 630;
	val.bottom = 640;
	lcount += 50 * (DispUnitval / 0.03);
	val.right = lcount;
	pCA->pDD_Display->Colorfill3(pDrawSurf, RGB(255,0,0), val.top, val.bottom, val.left, val.right);

	double BRval = double(pCA->pTimer->lTimeElapsed[TIMER_BLITREQUESTS]) / double(pCA->pTimer->lFrequency);
	val.left = lcount;
	val.top = 630;
	val.bottom = 640;
	lcount += 50 * (BRval / 0.03);
	val.right = lcount;
	pCA->pDD_Display->Colorfill3(pDrawSurf, RGB(0,0,255), val.top, val.bottom, val.left, val.right);

	double DMapval = double(pCA->pTimer->lTimeElapsed[TIMER_MAPDRAW]) / double(pCA->pTimer->lFrequency);
	val.left = lcount;
	val.top = 630;
	val.bottom = 640;
	lcount += 50 * (DMapval / 0.03);
	val.right = lcount;
	pCA->pDD_Display->Colorfill3(pDrawSurf, RGB(0,0,255), val.top, val.bottom, val.left, val.right);

	double Consoleval = double(pCA->pTimer->lTimeElapsed[TIMER_DISP_CONSOLE]) / double(pCA->pTimer->lFrequency);
	val.left = lcount;
	val.top = 630;
	val.bottom = 640;
	lcount += 50 * (Consoleval / 0.03);
	val.right = lcount;
	pCA->pDD_Display->Colorfill3(pDrawSurf, RGB(0,255,0), val.top, val.bottom, val.left, val.right);

	double Minimapval = double(pCA->pTimer->lTimeElapsed[TIMER_DISP_MINIMAP]) / double(pCA->pTimer->lFrequency);
	val.left = lcount;
	val.top = 630;
	val.bottom = 640;
	lcount += 50 * (Minimapval / 0.03);
	val.right = lcount;
	pCA->pDD_Display->Colorfill3(pDrawSurf, RGB(255,0,255), val.top, val.bottom, val.left, val.right);

	double Flipval = double(pCA->pTimer->lTimeElapsed[TIMER_DDFLIP]) / double(pCA->pTimer->lFrequency);
	val.left = lcount;
	val.top = 630;
	val.bottom = 640;
	lcount += 50 * (Flipval / 0.03);
	val.right = lcount;
	pCA->pDD_Display->Colorfill3(pDrawSurf, RGB(0,255,255), val.top, val.bottom, val.left, val.right);

	double Structval = double(pCA->pTimer->lTimeElapsed[TIMER_DISP_STRUCTS]) / double(pCA->pTimer->lFrequency);
	val.left = lcount;
	val.top = 630;
	val.bottom = 640;
	lcount += 50 * (Structval / 0.03);
	val.right = lcount;
	pCA->pDD_Display->Colorfill3(pDrawSurf, RGB(255,255,0), val.top, val.bottom, val.left, val.right);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_Selection(LPDIRECTDRAWSURFACE7 pDrawSurf)  //05.14.2002
{ 
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	LPSBE pUSB_Trav = (LPSBE)pCA->pInterface->pUSB_First->pNext; //
	cUnitA*	pUL_Trav;
	cStructureA* pSL_Trav;
	cStructInfo* pStructInfo;
	int xblit, yblit;
	RECT	SRect, DragRect, HPBar_rect;
	int xblitdir, yblitdir;
	int lc4, lc5; //local counters
	float fFractQuantity; //for structures, the HP bar could be two quantities, fCompletion if not yet finished, or the normal health (06.18.2004)


	//assign the rect to the first frame
	SRect.top = 0;
	SRect.left = 0;
	SRect.right = TILE_SIZE;
	SRect.bottom = TILE_SIZE;

	while(pUSB_Trav != pCA->pInterface->pUSB_Last)
	{

	//assign the rect to the first frame
	SRect.top = 0;
	SRect.left = 0;
	SRect.right = TILE_SIZE;
	SRect.bottom = TILE_SIZE;

		//change the selection box type if this is the console_select_unit
		if (pCA->pInterface->pConsole_Unit_Select != NULL)
		if (pUSB_Trav->iID == ((LPSBE)pCA->pInterface->pConsole_Unit_Select)->iID)
		{
		SRect.top = TILE_SIZE * 2;
		SRect.left = 0;
		SRect.right = TILE_SIZE;
		SRect.bottom = TILE_SIZE * 3;
		}
		
		pUL_Trav = pCA->pUnitTable->pUnitTable[pUSB_Trav->iID];

		//draw the selection box for all units in the USB, no matter where they are, because if they're not on the screen, they just won't be drawn
		xblit = pUL_Trav->Pos_ABS.x + pCA->pInterface->IBoxes[IBOX_PRIMARY].BoxPos.left - (pCA->pInterface->map_xview * TILE_SIZE);
		yblit = pUL_Trav->Pos_ABS.y + pCA->pInterface->IBoxes[IBOX_PRIMARY].BoxPos.top - (pCA->pInterface->map_yview * TILE_SIZE);

		pDrawSurf->BltFast(xblit, yblit, pCA->pImages[ISURF_SELECT]->pDDSImage, &SRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		
		//set the HPBar rect
		//HPBar_rect.top = 0;
		//HPBar_rect.bottom = 2;
		//HPBar_rect.left = 0;
		//TEMP (until unit info goes up) 
		//HPBar_rect.right = (pCA->pImages[ISURF_HPBAR]->DDSDImage.dwWidth * pUL_Trav->iHP) / pUL_Trav->iHPMax; 

		//blit the HP bar(06.26.2002)
//		pDrawSurf->BltFast(xblit, yblit + (TILE_SIZE * 0.8), pCA->pImages[ISURF_HPBAR]->pDDSImage, &HPBar_rect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		//try a colored BLIT colorfill(01.01.2003)
		DDBLTFX DDBlitEffects;
		DDBlitEffects.dwSize = sizeof(DDBLTFX);
		DDBlitEffects.dwFillColor = pCA->pColorTable->Green;

		RECT dest_rect;
		dest_rect.top = yblit + TILE_SIZE*0.8;
		dest_rect.bottom = dest_rect.top + 2;
		dest_rect.left = xblit;
		dest_rect.right = xblit + (pCA->pImages[ISURF_HPBAR]->DDSDImage.dwWidth * pUL_Trav->fHP) / pUL_Trav->fHPMax; 
		//adjust color depending on health(TEMP 01.02.2003)
		if ((pCA->pImages[ISURF_HPBAR]->DDSDImage.dwWidth * pUL_Trav->fHP) / pUL_Trav->fHPMax < 16)
			DDBlitEffects.dwFillColor = pCA->pColorTable->Yellow; 
		if ((pCA->pImages[ISURF_HPBAR]->DDSDImage.dwWidth * pUL_Trav->fHP) / pUL_Trav->fHPMax < 10)
			DDBlitEffects.dwFillColor = pCA->pColorTable->Red;
		pDrawSurf->Blt(&dest_rect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &DDBlitEffects);


		//advance and refresh
		pUSB_Trav = (LPSBE)pUSB_Trav->pNext;
	}



	//blit selection around all selected structures
	pUSB_Trav = (LPSBE)pCA->pInterface->pSSB_First->pNext; //
	while(pUSB_Trav != pCA->pInterface->pSSB_Last)
	{
	//assign the rect to the first frame
	SRect.top = TILE_SIZE * 0;
	SRect.left = 0;
	SRect.right = TILE_SIZE;
	SRect.bottom = TILE_SIZE * 1;

		pSL_Trav = pCA->pStructIDTable->pTable[pUSB_Trav->iID];
		pStructInfo = pCA->pStructInfoTable->pTable[pSL_Trav->iType];

		//draw the selection box for all units in the USB, no matter where they are, because if they're not on the screen, they just won't be drawn
		xblit = pSL_Trav->Pos_primary.x * TILE_SIZE + pCA->pInterface->IBoxes[IBOX_PRIMARY].BoxPos.left - (pCA->pInterface->map_xview * TILE_SIZE);
		yblit = pSL_Trav->Pos_primary.y * TILE_SIZE + pCA->pInterface->IBoxes[IBOX_PRIMARY].BoxPos.top - (pCA->pInterface->map_yview * TILE_SIZE);

		RECT StructDestRect;

		StructDestRect.top = yblit + pStructInfo->iYDrawOffset;
		StructDestRect.left = xblit + pStructInfo->iXDrawOffset;
		StructDestRect.bottom = StructDestRect.top + pStructInfo->iYFramesize;
		StructDestRect.right = StructDestRect.left + pStructInfo->iXFramesize;

		pDrawSurf->Blt(&StructDestRect ,pCA->pImages[ISURF_SELECT]->pDDSImage, &SRect, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
		
		//set the HPBar rect
		HPBar_rect.top = 0;
		HPBar_rect.bottom = 2;
		HPBar_rect.left = 0;
		//TEMP (until unit info goes up) 
		HPBar_rect.right = (pCA->pImages[ISURF_HPBAR]->DDSDImage.dwWidth * pSL_Trav->iHP ) /pSL_Trav->iHPMax ; 

		//blit the HP bar(06.26.2002)
//		pDrawSurf->BltFast(xblit, yblit + (TILE_SIZE * 0.8), pCA->pImages[ISURF_HPBAR]->pDDSImage, &HPBar_rect, DDBLTFAST | DDBLTFAST_SRCCOLORKEY);
		//try a colored BLIT colorfill(01.01.2003)
		DDBLTFX DDBlitEffects;
		DDBlitEffects.dwSize = sizeof(DDBLTFX);


		//fractional quantity for what the HPbar is to show
		if (pSL_Trav->fCompletion >= 1.0)
		{
			fFractQuantity = pSL_Trav->iHP / pSL_Trav->iHPMax;

			DDBlitEffects.dwFillColor = pCA->pColorTable->Green;
			if ((pCA->pImages[ISURF_HPBAR]->DDSDImage.dwWidth * pSL_Trav->iHP) / pSL_Trav->iHPMax < 16)
				DDBlitEffects.dwFillColor = pCA->pColorTable->Yellow; //not deliberate, this just happens to make yellow on my screen
			if ((pCA->pImages[ISURF_HPBAR]->DDSDImage.dwWidth * pSL_Trav->iHP) /pSL_Trav->iHPMax < 8)
				DDBlitEffects.dwFillColor = pCA->pColorTable->Red;
		} else
			{
				fFractQuantity = pSL_Trav->fCompletion;

				DDBlitEffects.dwFillColor = pCA->pColorTable->Red;
			}

		RECT dest_rect;
		dest_rect.top = yblit + pStructInfo->iYFramesize * 0.8;
		dest_rect.bottom = dest_rect.top + 2;
		dest_rect.left = xblit;
		dest_rect.right = xblit + (pCA->pImages[ISURF_HPBAR]->DDSDImage.dwWidth * (pStructInfo->iXFramesize / TILE_SIZE) * fFractQuantity); 
		pDrawSurf->Blt(&dest_rect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &DDBlitEffects);

		//advance and refresh
		pUSB_Trav = (LPSBE)pUSB_Trav->pNext;
	}

	//reset the rect to the second frame and draw the selection drag if the user is currently dragging
	//allowed to be in UCOMMAND or SCOMMAND or SELECTION mode to make into war2 style
	if (pCA->pInputState->IGState_Mouse.Buttons[0] == 1 && (pCA->pInterface->iMode == IFCM_SELECTION || pCA->pInterface->iMode == IFCM_UCOMMAND || pCA->pInterface->iMode == IFCM_SCOMMAND)
		&& pCA->pInterface->mouse_currbox == IBOX_PRIMARY)
	{
		xblitdir = -1;
		yblitdir = -1;
        if (pCA->pInterface->PosMap_Mouse.x <= pCA->pInterface->MSelectBox_GW.left)
				xblitdir = 1;
		if (pCA->pInterface->PosMap_Mouse.y <= pCA->pInterface->MSelectBox_GW.top)
				yblitdir = 1;

	//assign the rect to the second frame
	SRect.top = TILE_SIZE;
	SRect.left = 0;
	SRect.right = TILE_SIZE;
	SRect.bottom = TILE_SIZE * 2;
		
	DragRect.top = pCA->pInterface->MSelectBox_GW.top;
	DragRect.left = pCA->pInterface->MSelectBox_GW.left;
	DragRect.bottom = pCA->pInterface->PosMap_Mouse.y;
	DragRect.right = pCA->pInterface->PosMap_Mouse.x;

	pCA->pInterface->Fix_RectBox(&DragRect);

		for (lc4 = DragRect.top; lc4 <= DragRect.bottom; lc4++) 
		for (lc5 = DragRect.left; lc5 <= DragRect.right; lc5++)
		{
			xblit = ((lc5 - pCA->pInterface->map_xview) * TILE_SIZE) + pCA->pInterface->IBoxes[IBOX_PRIMARY].BoxPos.left;
			yblit = ((lc4 - pCA->pInterface->map_yview) * TILE_SIZE) + pCA->pInterface->IBoxes[IBOX_PRIMARY].BoxPos.top;
			pDrawSurf->BltFast(xblit, yblit, pCA->pImages[ISURF_SELECT]->pDDSImage, &SRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		}

	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_Projectiles(LPDIRECTDRAWSURFACE7 pDrawSurf) //(06.13.2002)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cProjectileA* pPRL_Trav;  //projectile list traverser
	int xblit, yblit;
	RECT SRect; //source rectangle on surface
	int iDrawFrame;

	pPRL_Trav = (cProjectileA*)pCA->pProjList->pPRL_First->pNext;

	//go through list, if projectile is on screen, draw it
	while(pPRL_Trav != pCA->pProjList->pPRL_Last)
	{
		//first attempt: draw off the projectile's Pos_square data
		if (pPRL_Trav->Pos_square.x >= pCA->pInterface->map_xview &&
			pPRL_Trav->Pos_square.x <= pCA->pInterface->map_xview + MDRAW_NUMCOLS &&
			pPRL_Trav->Pos_square.y >= pCA->pInterface->map_yview &&
			pPRL_Trav->Pos_square.y <= pCA->pInterface->map_yview + MDRAW_NUMROWS)
		{
			PROJTYPEINFO* pInfo = &pCA->pPTypeTable->ProjTypes[pPRL_Trav->iType];

			//these might need to be changed in the case to keep projectiles "centered" over their locations
			//in the event the projectile graphic size deviates from the tile size
			//this has now occurred (07.20.2004) so now we offset by half the tile size, and then
			//reverse offset by half the rect size
			//the above was incorrect. i didn't need to offset by half the tilesize, since the projectile position is the actual position, not the graphic position (12.27.2004)
			xblit = (pPRL_Trav->Pos_square.x - pCA->pInterface->map_xview) * TILE_SIZE + pPRL_Trav->Pos_offset.x;
			yblit = (pPRL_Trav->Pos_square.y - pCA->pInterface->map_yview) * TILE_SIZE + pPRL_Trav->Pos_offset.y;

			//decide the SRect based on the drawframe
			iDrawFrame = pPRL_Trav->Calc_DrawFrame( pPRL_Trav->fDirection, pInfo->iRotateFrames); //pPRL_Trav->iDrawFrame;

			//get the rect
			int iRet =0;
			SRCIMGFRAME* SrcImg = pCA->pPTypeTable->ProjTypes[pPRL_Trav->iType].SourceGraphics.Get_Entry2(0, iDrawFrame, 0, &iRet);
			int iX = SrcImg->Imgcoords.right - SrcImg->Imgcoords.left;
			int iY = SrcImg->Imgcoords.bottom - SrcImg->Imgcoords.top;

			xblit -= iX / 2;
			yblit -= iY / 2;

			//compensate
			xblit += TILE_SIZE / 2;
			yblit += TILE_SIZE / 2;
			
			SRect.bottom = SrcImg->Imgcoords.bottom;
			SRect.left = SrcImg->Imgcoords.left;
			SRect.right = SrcImg->Imgcoords.right;
			SRect.top = SrcImg->Imgcoords.top;


			/*DEBUG
			if (xblit == 0 || yblit == 0)
			{
				char NText[50];
				itoa((int)pPRL_Trav->Pos_ABS.x, NText, 10);
				Write_Log(NText);
				Write_Log(",");
				itoa((int)pPRL_Trav->Pos_ABS.y, NText, 10);
				Write_Log(NText);
				Write_Log(" ");
			}
			END DB*/

			//blit
			pDrawSurf->BltFast(xblit, yblit, pCA->pImages[SrcImg->iImageSurfIndex]->pDDSImage, &SRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		}

	//advance the traverser
		pPRL_Trav = (cProjectileA*)pPRL_Trav->pNext;
	
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_MainMenu(LPDIRECTDRAWSURFACE7 pDrawSurf, int iInc)
{
	//do an "alpha" on the main menu box
	cClassAccess* pCA = (cClassAccess*)pCAccess;

//"fast" alpha test  (12.23.2003)
/*
	//find the bitmasks
WORD wRBitMask, wBBitMask, wGBitMask;
LONG lRValue, lGValue, lBValue, dwValTest;
WORD* pInWord;
DWORD* pInDWord;
WORD* pSurfaceStart;
BOOL* pBitStart;
BOOL* pInBit;
BYTE* pInByte;
ULONGLONG* pIn64;
ULONGLONG In64Store;
WORD  WordStore;
DWORD DWordStore;
ULONGLONG llMask[4];

	llMask[0] = 65535;
	llMask[1] = llMask[0] << 16;
	llMask[2] = llMask[1] << 16;
	llMask[3] = llMask[2] << 16;


BYTE bRShift = 0; 
BYTE bGShift = 0; 
BYTE bBShift = 0;
int lcx, lcy, lcc; //counters
int iWidth = pCA->pDD_Display->i_XResD;

	//lock the display surface
DDSURFACEDESC2 DDSD2;
	DDSD2.dwSize = sizeof(DDSURFACEDESC2);
	pDrawSurf->Lock(NULL, &DDSD2, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
	pSurfaceStart = (WORD*)DDSD2.lpSurface;
	pBitStart = (BOOL*)DDSD2.lpSurface;

//only do this alpha if the bit-depth is 16(won't work at 8 bit)
	if (pCA->pDD_Display->iBPP == 16)
	{
		//get the bitmasks
		wRBitMask = (WORD)pCA->pDD_Display->DMDesc.ddpfPixelFormat.dwRBitMask;
		wGBitMask = (WORD)pCA->pDD_Display->DMDesc.ddpfPixelFormat.dwGBitMask;
		wBBitMask = (WORD)pCA->pDD_Display->DMDesc.ddpfPixelFormat.dwBBitMask;


		dwValTest = 1;
		//find out what the shifts on the values are
		while ((wRBitMask & dwValTest) == 0)
		{	
			bRShift++;
			dwValTest *= 2;
		}

		dwValTest = 1;

		//green
		while ((wGBitMask & dwValTest) == 0)
		{	
			bGShift++;
			dwValTest *= 2;
		}

		dwValTest = 1;
		//blue
		while ((wBBitMask & dwValTest) == 0)
		{	
			bBShift++;
			dwValTest *= 2;
		}

	//create an "AND mask" to remove the least significant bit of each of the color fields
	WORD AMask = 65535 - (1 << bBShift)  - (1 << bRShift) - (1 << bGShift);
	
	//now set the reference color (we want a shadow, so do white)
	WORD CRef = 65535;
	//do the shift here, might as well
	CRef >>= 1;
	
/*

		//change bits for each pixel
		for (lcx = pCA->pInterface->IBoxes[IBOX_MAINMENU].BoxPos.left; lcx <= pCA->pInterface->IBoxes[IBOX_MAINMENU].BoxPos.right; lcx+=2)
		for (lcy = pCA->pInterface->IBoxes[IBOX_MAINMENU].BoxPos.top; lcy <= pCA->pInterface->IBoxes[IBOX_MAINMENU].BoxPos.bottom; lcy++)
		{
			//hard coded....that's the pitch of the surface
			pInWord = pSurfaceStart + (lcy * DDSD2.lPitch / 2) + lcx;

			//remove the LSB
			*(pInWord) = *(pInWord) & AMask;
			//shift
			*(pInWord) >>= 1;
			*(pInWord) = *(pInWord) + CRef;


//			pInDWord = (DWORD*)pInWord;
//			pIn64 = (ULONGLONG*)pInWord;
//			pInByte = (BYTE*)pInWord;
			//pInByte = (BYTE*)pInWord;
//			*(pInByte) = 11 + rand() % 8;
//			pInByte += 2;
//			*(pInByte) = 11 + rand() % 8;
//			pInBit = (BIT*)pInWord + 1;
//			*(pInBit) = 0;
//			if ((*(pInWord) >> 3) % 2 == 0)
//				*(pInWord) += 8;

			//get R,G,B-values
//			lRValue = *(pInWord) >> bRShift; //=*(pInWord) & wRBitMask;
//			lGValue = (*(pInWord) >> bGShift) - lRValue;//*(pInWord) & wGBitMask;
//			lBValue = *(pInWord) - lRValue - lGValue;//*(pInWord) & wBBitMask;


			//memcpy(&In64Store, pInByte, 4); //ULONGLONG(*(pInDWord) << 16) + ULONGLONG(*(pInDWord + 1));
/*			In64Store = *(pInDWord);//(*(pInByte) << 32)  + (*(pInByte + 1) << 40) + (*(pInByte + 8) << 48) + (*(pInByte + 3) << 56) + (*(pInByte + 4) << 0) + (*(pInByte + 5) << 8) + (*(pInByte + 6) << 16) + (*(pInByte + 7) << 24);
			for (lcc = 0; lcc <= 1; lcc++)
			{
//				if (lcc == 0)
//				WordStore = WORD(In64Store);
//				if (lcc == 1 || lcc == 2)
				WordStore = WORD(In64Store >> (lcc*16));
//				if (lcc == 3)
//				WordStore = WORD(In64Store >> 48);

			lRValue = WordStore & wRBitMask;
			lGValue = WordStore & wGBitMask;
			lBValue = WordStore & wBBitMask;
			//shift them to independent values(effectively "separating" them)
			lRValue >>= bRShift;
			lGValue >>= bGShift;
			lBValue >>= bBShift;
			//subtract 8 from them(they're probably going to be larger than 4-bits)
			lRValue += iInc; if (lRValue < 0) lRValue = 0; if (lRValue > 31) lRValue = 31;//62 - lBValue;
			lGValue += iInc*2; if (lGValue < 0) lGValue = 0; if (lGValue > 63) lGValue = 63;//126 - lBValue;
			lBValue += iInc + rand() % 4; if (lBValue < 0) lBValue = 0; if (lBValue > 31) lBValue = 31;//62 - lBValue;
			//shift them back
			lRValue <<= bRShift;
			lGValue <<= bGShift;
			lBValue <<= bBShift;

			//set the inword value 
			//*(pInWord) = WORD(rand());//WORD(lRValue + lGValue + lBValue);
			*(pInWord) = WORD(lRValue + lGValue + lBValue);
			pInWord++;
			}

		}
*/ /*
	}


	pDrawSurf->Unlock(NULL);

*/

/*DEBUG
//char NText[50];
END DB*/
/*DEBUG
//itoa(pCA->pInterface->IBoxes[IBOX_MAINMENU].iActiveGrid, NText, 10);
//Write_Log("Active Grid: ");
//Write_Log(NText);
//WriteLog_Enter();
END DB*/
//blit the menu to the location
	//pDrawSurf->BltFast(pCA->pInterface->IBoxes[IBOX_MAINMENU].BoxPos.left, pCA->pInterface->IBoxes[IBOX_MAINMENU].BoxPos.top, pCA->pImages[ISURF_MAINMENU]->pDDSImage, NULL, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	//pDrawSurf->Blt(&pCA->pInterface->IBoxes[IBOX_MAINMENU].BoxPos, pCA->pImages[ISURF_MAINMENU]->pDDSImage, NULL, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
//blit the buttons of the active grid
/*DEBUG
//Write_Log("Blitting Cells.... ");
//WriteLog_Enter();
END DB*/

if (pCA->pInterface->IBoxes[IBOX_MAINMENU].bActiveBox == TRUE)
{

	cIGrid* pIG_Active = &pCA->pInterface->IBoxes[IBOX_MAINMENU].pIGrids[pCA->pInterface->IBoxes[IBOX_MAINMENU].iActiveGrid];

	for (int c3 = 0; c3 < pIG_Active->iElements_x * pIG_Active->iElements_y; c3++) 
	{
		if (pIG_Active->pGrid[c3].bDrawCell == TRUE)
		{
			int xblit = pIG_Active->GridPos.left + ((c3 % pIG_Active->iElements_x) * pIG_Active->iCellX_pixels);
			int yblit = pIG_Active->GridPos.top + ((c3 / pIG_Active->iElements_x) * pIG_Active->iCellY_pixels);
		
			if (pIG_Active->pGrid[c3].DrawType == IGRIDCELL_DRAWTYPE_BLIT)
			pDrawSurf->BltFast(xblit, yblit, pCA->pImages[pIG_Active->pGrid[c3].wISurfIndex]->pDDSImage, &pIG_Active->pGrid[c3].SurfRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
			if (pIG_Active->pGrid[c3].DrawType == IGRIDCELL_DRAWTYPE_TEXT)
			pCA->pGDisplay->Draw_Text(pDrawSurf, pIG_Active->pGrid[c3].Text, xblit, yblit, FONT_STANDARD1);
			if (pIG_Active->pGrid[c3].DrawType == IGRIDCELL_DRAWTYPE_TEXTWIDE)
			pCA->pGDisplay->Draw_Text_Wide(pDrawSurf, (unsigned short*)pIG_Active->pGrid[c3].Text, xblit, yblit, FONT_STANDARD1);
		}

	}
//display interface elements
Disp_IfcElems(pIG_Active, 0, 0);
}



//TEST lock the display surface (and report the color value of one of the pixels)
//	DDSURFACEDESC2 ddsd3;
//	ddsd3.dwSize = sizeof(DDSURFACEDESC2);
//	DisplayDD.pDDSBack->Lock(NULL, &ddsd3, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
//	WORD* pw = (WORD*)ddsd3.lpSurface;
//	pw = pw + GInput.IGState_Mouse.iXPos + (GInput.IGState_Mouse.iYPos * (ddsd3.lPitch / 2));
//	int scolor = (int)*pw;
//	DisplayDD.pDDSBack->Unlock(NULL);




}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
/*void cGDisplay::BlitAlpha(LPDIRECTDRAWSURFACE7 pDestSurf, VOID* pImageSurf, int xpos, int ypos)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cImageSurface* pIS = (cImageSurface*)pImageSurf;
	DDSURFACEDESC2 DDSDLock;
	WORD* pInWord;
	WORD* pSurfaceStart;
	int lc1, lc2; //counters

//only do this if bit depth is 16(others not supported)
if (pCA->pDD_Display->iBPP == 16)
{
	//lock the destination surface
	DDSDLock.dwSize = sizeof(DDSURFACEDESC2);
	pDestSurf->Lock(NULL, &DDSDLock, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
	//get the pointer
	pSurfaceStart = (WORD*)DDSDLock.lpSurface;
	
//		for (lc1 = xpos; lc1 <= xpos + pImageSurf->DDSD
	//maybe pick this up later......so much work to do an alpha scheme that may be unviable(07.05.2002)	






	pDestSurf->Unlock(NULL);
}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_ChatText(LPDIRECTDRAWSURFACE7 pDrawSurf, int iXOff, int iYOff, int iEntries)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	CBNODE* pCBL_Trav = pCA->pChatBuffer->pCBL_First;
	int iYLineOffset = 0;
	char NText[50];


	pCA->pFontInfoTable->SetFontColor( pCA->pColorTable->Green);
	//go through chat buffer to the most recent entries, and then draw those going backwards
	for (int lc27 = 0; lc27 < iEntries; lc27++)
	if (((CBNODE*)pCBL_Trav->pNext) != pCA->pChatBuffer->pCBL_Last)	
		pCBL_Trav = (CBNODE*)pCBL_Trav->pNext;

	//then go backwards and draw all the entries to screen
	while (pCBL_Trav != pCA->pChatBuffer->pCBL_First)
	{
		//draw the current text
		iYLineOffset += Draw_Text(pDrawSurf, pCBL_Trav->Text, iXOff, iYOff + (iYLineOffset * pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight), FONT_STANDARD1);
		//increment pointer
		pCBL_Trav = (CBNODE*)pCBL_Trav->pPrev;
		
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//as far as I can tell, the art of drawing the notifbuffer and the chat buffer is the same
void cGDisplay::Disp_Notifications(LPDIRECTDRAWSURFACE7 pDrawSurf, int iXOff, int iYOff, int iEntries)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cNotifNode* pNB_Trav = pCA->pNotifBuffer->pNB_First;
	int iYLineOffset = 0;
	char NText[50];

	//go through chat buffer to the most recent entries, and then draw those going backwards
	for (int lc27 = 0; lc27 < iEntries; lc27++)
	if (((cNotifNode*)pNB_Trav->pNext) != pCA->pNotifBuffer->pNB_Last)	
		pNB_Trav = (cNotifNode*)pNB_Trav->pNext;

	//then go backwards and draw all the entries to screen
	while (pNB_Trav != pCA->pNotifBuffer->pNB_First)
	{
		//draw the current text
		iYLineOffset += Draw_Text(pDrawSurf, pNB_Trav->Text, iXOff, iYOff + (iYLineOffset * pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight), FONT_STANDARD1);
		//increment pointer
		pNB_Trav = (cNotifNode*)pNB_Trav->pPrev;
		
	}

}


//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_Minimap(LPDIRECTDRAWSURFACE7 pDrawSurf)
{
//as of 03.24.2003 this is a really fudged and not-thought-out way to draw the
//various maps.....
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	DDBLTFX DDBlitEffects;
	RECT SelectRect;

	DDBlitEffects.dwSize = sizeof(DDBLTFX);
	HRESULT hr20;
	const char* KText;
			RECT RectZoomSource;
			RECT finalzoomdrawrect;

	int imouseindex = 0;
	if (pCA->pInterface->mouse_currbox == IBOX_MINIMAP)
		imouseindex = pCA->pInterface->PosMinimap_Mouse.index;
	if (pCA->pInterface->mouse_currbox == IBOX_PRIMARY)
		imouseindex = pCA->pInterface->PosMap_Mouse.index;

	//blit the sector current "sector" over
/*  //disabled 09.20.2006
	cSector* pSector = pCA->pSectorTable->Sectors[pCA->pGameWorld->pMap[imouseindex].iSector];
	
	if (pCA->pInterface->mouse_currbox == IBOX_MINIMAP || pCA->pInterface->mouse_currbox == IBOX_PRIMARY)
	if (pSector != NULL)
	if (pCA->pGameWorld->pMap[imouseindex].iSector != MAPFILE_SCT_NOSECTOR)
	{
		Draw_Text_Shadow(pDrawSurf, pSector->Name, MinimapDrawRect.right - (pCA->pFontInfoTable->Fonts[FONT_TEST01].iWidth * strlen(pSector->Name)), MinimapDrawRect.bottom + 5, FONT_TEST01, 0, pCA->pColorTable->White);
	}
*/
	//instead, blit perentage of map possessed
	char NText[50];
	char NText2[50];
	gcvt( pCA->pNationManager->ppNations[pCA->pGameOptions->iControl]->ddPossession * 100, 3, NText);
	if (pCA->pNationManager->ppNations[pCA->pGameOptions->iControl]->ddPossession < 0.001)
	{
		ZeroMemory(NText2, sizeof(char)*50);
		strcat(NText2, "0.0");
		NText2[3] = NText[0];
		NText2[4] = NText[2];
		memcpy(NText, NText2, sizeof(char)*10);
	}
	strcat(NText, "%");
	char Buffer[100]; ZeroMemory(Buffer, sizeof(char)*100);
	strcat(Buffer, "Territory:");
	char NText3[50]; itoa(pCA->pNationManager->ppNations[pCA->pGameOptions->iControl]->iPossession, NText3, 10);
	strcat(Buffer, NText3);
	strcat(Buffer, " (");
	strcat(Buffer, NText);
	strcat(Buffer, ")");
	Draw_Text_Shadow(pDrawSurf, Buffer, MinimapDrawRect.right - (pCA->pFontInfoTable->Fonts[FONT_TEST01].iWidth * strlen(Buffer)), MinimapDrawRect.bottom + 5, FONT_TEST01, pCA->pColorTable->White-1, pCA->pColorTable->Black );


	//simple, just blit surface to surface, at the moment
	hr20 = pDrawSurf->Blt(&MinimapDrawRect, pCA->pMinimap->ProcessSurface.pDDSImage, NULL, DDBLT_WAIT, &DDBlitEffects);
//hr20 = pDrawSurf->Blt(NULL, pCA->pMinimap->MapSurface.pDDSImage, NULL, DDBLT_WAIT, &DDBlitEffects);
//	hr20 = pDrawSurf->BltFast(0, 0, pCA->pMinimap->MapSurface.pDDSImage, NULL, DDBLTFAST_WAIT | DDBLTFAST_WAIT);

	//draw the select_over box
	//figure out where on the screen we are currently looking at
	int rectheight = MinimapDrawRect.bottom - MinimapDrawRect.top;
	int rectwidth = MinimapDrawRect.right - MinimapDrawRect.left;
	int zoomrectheight = MinimapZoomDrawRect.bottom - MinimapZoomDrawRect.top;
	int zoomrectwidth =  MinimapZoomDrawRect.right - MinimapZoomDrawRect.left;
	int xbar_low = (float(pCA->pInterface->map_xview) / pCA->pGameWorld->iWidth) * rectwidth + MinimapDrawRect.left;
	int xbar_high = (float(pCA->pInterface->map_xview + MDRAW_NUMCOLS) / pCA->pGameWorld->iWidth) * rectwidth + MinimapDrawRect.left;
	int ybar_low = (float(pCA->pInterface->map_yview) / pCA->pGameWorld->iHeight) * rectheight + MinimapDrawRect.top;
	int ybar_high = (float(pCA->pInterface->map_yview + MDRAW_NUMROWS) / pCA->pGameWorld->iHeight) * rectheight + MinimapDrawRect.top;

	//first, try blitting just a rectangle
DDBlitEffects.dwFillColor = pCA->pColorTable->White;
	//the top bar
	SelectRect.top = ybar_low;
	SelectRect.bottom = ybar_low + 1;
	SelectRect.left = xbar_low;
	SelectRect.right = xbar_high;
	hr20 = pDrawSurf->Blt(&SelectRect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &DDBlitEffects);
	//the bottom bar
	SelectRect.top = ybar_high;
	SelectRect.bottom = ybar_high + 1;
	SelectRect.left = xbar_low;
	SelectRect.right = xbar_high;
	hr20 = pDrawSurf->Blt(&SelectRect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &DDBlitEffects);
	//the left bar
	SelectRect.top = ybar_low;
	SelectRect.bottom = ybar_high;
	SelectRect.left = xbar_low;
	SelectRect.right = xbar_low + 1;
	hr20 = pDrawSurf->Blt(&SelectRect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &DDBlitEffects);
	//the right bar
	SelectRect.top = ybar_low;
	SelectRect.bottom = ybar_high;
	SelectRect.left = xbar_high;
	SelectRect.right = xbar_high + 1;
	hr20 = pDrawSurf->Blt(&SelectRect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &DDBlitEffects);

if (pCA->pMinimap->MMZDrawFlag == TRUE)
{
	//blit a second rectangle representing the "zoomed" minimap's field of vision (disable because it looks confusing....
	/*
		xbar_low = (float(pCA->pInterface->map_xview + pCA->pMinimap->sqMMZOffset.x) / pCA->pGameWorld->iWidth) * rectwidth + MinimapDrawRect.left;
		xbar_high = (float(pCA->pInterface->map_xview + pCA->pMinimap->sqMMZOffset.x + zoomrectwidth) / pCA->pGameWorld->iWidth) * rectwidth + MinimapDrawRect.left;
		ybar_low = (float(pCA->pInterface->map_yview + pCA->pMinimap->sqMMZOffset.y) / pCA->pGameWorld->iHeight) * rectheight + MinimapDrawRect.top;
		ybar_high = (float(pCA->pInterface->map_yview + pCA->pMinimap->sqMMZOffset.y + zoomrectheight) / pCA->pGameWorld->iHeight) * rectheight + MinimapDrawRect.top;
	*/

	//do correction on the margins
		if (xbar_high > MinimapDrawRect.right)
			xbar_high = MinimapDrawRect.right;
		if (ybar_high > MinimapDrawRect.bottom)
			ybar_high = MinimapDrawRect.bottom;
		if (xbar_low < MinimapDrawRect.left)
			xbar_low = MinimapDrawRect.left;
		if (ybar_low < MinimapDrawRect.top)
			ybar_low = MinimapDrawRect.top;

		//the top bar
		SelectRect.top = ybar_low;
		SelectRect.bottom = ybar_low + 1;
		SelectRect.left = xbar_low;
		SelectRect.right = xbar_high;
		hr20 = pDrawSurf->Blt(&SelectRect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &DDBlitEffects);
		//the bottom bar
		SelectRect.top = ybar_high;
		SelectRect.bottom = ybar_high + 1;
		SelectRect.left = xbar_low;
		SelectRect.right = xbar_high;
		hr20 = pDrawSurf->Blt(&SelectRect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &DDBlitEffects);
		//the left bar
		SelectRect.top = ybar_low;
		SelectRect.bottom = ybar_high;
		SelectRect.left = xbar_low;
		SelectRect.right = xbar_low + 1;
		hr20 = pDrawSurf->Blt(&SelectRect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &DDBlitEffects);
		//the right bar
		SelectRect.top = ybar_low;
		SelectRect.bottom = ybar_high;
		SelectRect.left = xbar_high;
		SelectRect.right = xbar_high + 1;
		hr20 = pDrawSurf->Blt(&SelectRect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &DDBlitEffects);


	//draw a second minimap for zoom operations
		//for now, simply a second rectangle

		RectZoomSource.top = pCA->pInterface->map_yview + pCA->pMinimap->sqMMZOffset.y;
		RectZoomSource.left = pCA->pInterface->map_xview + pCA->pMinimap->sqMMZOffset.x;
		RectZoomSource.bottom = RectZoomSource.top + (MMZDRAW_YSCR * pCA->pDD_Display->i_YResD); //this will force a close to 1:1 correlation
		RectZoomSource.right = RectZoomSource.left + (MMZDRAW_XSCR * pCA->pDD_Display->i_XResD); //simply for convenience, right now(03.02.2003)

	finalzoomdrawrect = MinimapZoomDrawRect;

		if (RectZoomSource.top < 0)
		{
			RectZoomSource.top = 0;
			finalzoomdrawrect.top = 0 - pCA->pInterface->map_yview - pCA->pMinimap->sqMMZOffset.y + MinimapZoomDrawRect.top;
		}
		if (RectZoomSource.left < 0)
		{
			RectZoomSource.left = 0;
			finalzoomdrawrect.left = 0 - pCA->pInterface->map_xview - pCA->pMinimap->sqMMZOffset.x + MinimapZoomDrawRect.left;
		}
		if (RectZoomSource.bottom > pCA->pMinimap->MapSurface.DDSDImage.dwHeight)
		{
			RectZoomSource.bottom = pCA->pMinimap->MapSurface.DDSDImage.dwHeight;
			finalzoomdrawrect.bottom = finalzoomdrawrect.top + RectZoomSource.bottom - RectZoomSource.top;
		}
		if (RectZoomSource.right > pCA->pMinimap->MapSurface.DDSDImage.dwWidth)
		{
			RectZoomSource.right = pCA->pMinimap->MapSurface.DDSDImage.dwWidth;
			finalzoomdrawrect.right = finalzoomdrawrect.left + RectZoomSource.right - RectZoomSource.left;
		}

		hr20 = pDrawSurf->Blt(&finalzoomdrawrect, pCA->pMinimap->MapSurface.pDDSImage, &RectZoomSource, DDBLT_WAIT, &DDBlitEffects);

	//draw a rectangle representing the source map on this surface
		xbar_low =  MinimapZoomDrawRect.left - pCA->pMinimap->sqMMZOffset.x;
		xbar_high = xbar_low + MDRAW_NUMCOLS;
		ybar_low = MinimapZoomDrawRect.top - pCA->pMinimap->sqMMZOffset.y;
		ybar_high = ybar_low + MDRAW_NUMROWS;

		//do correction on the bar values
		if (xbar_high > MinimapZoomDrawRect.right)
			xbar_high = MinimapZoomDrawRect.right;
		if (ybar_high > MinimapZoomDrawRect.bottom)
			ybar_high = MinimapZoomDrawRect.bottom;
		if (xbar_low < MinimapZoomDrawRect.left)
			xbar_low = MinimapZoomDrawRect.left;
		if (ybar_low < MinimapZoomDrawRect.top)
			ybar_low = MinimapZoomDrawRect.top;
		//the top bar
		SelectRect.top = ybar_low;
		SelectRect.bottom = ybar_low + 1;
		SelectRect.left = xbar_low;
		SelectRect.right = xbar_high;
		hr20 = pDrawSurf->Blt(&SelectRect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &DDBlitEffects);
		//the bottom bar
		SelectRect.top = ybar_high;
		SelectRect.bottom = ybar_high + 1;
		SelectRect.left = xbar_low;
		SelectRect.right = xbar_high;
		hr20 = pDrawSurf->Blt(&SelectRect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &DDBlitEffects);
		//the left bar
		SelectRect.top = ybar_low;
		SelectRect.bottom = ybar_high;
		SelectRect.left = xbar_low;
		SelectRect.right = xbar_low + 1;
		hr20 = pDrawSurf->Blt(&SelectRect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &DDBlitEffects);
		//the right bar
		SelectRect.top = ybar_low;
		SelectRect.bottom = ybar_high;
		SelectRect.left = xbar_high;
		SelectRect.right = xbar_high + 1;
		hr20 = pDrawSurf->Blt(&SelectRect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &DDBlitEffects);

}



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_Console(LPDIRECTDRAWSURFACE7 pDrawSurf)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	char NText[50];
	const char* KText;

//DEBUG
Write_Log("[Disp Console]Beginning Function");
WriteLog_Enter();
itoa(pCA->pCommQueue->lNumCommands, NText, 10);
Draw_Text(pDrawSurf, NText, 0,0 , FONT_STANDARD1);

	pCA->pFontInfoTable->SetFontColor(pCA->pColorTable->MM_Players[pCA->pGameOptions->iControl]);

	DWORD dwOldColor = 	pCA->pColorTable->MM_Players[pCA->pGameOptions->iControl];

	//get rectangles for bottom and right consoles
	RECT BottomRect, RightRect;

	RightRect.top = 0;
	RightRect.bottom = pCA->pDD_Display->i_YResD;
	RightRect.left = pCA->pDD_Display->i_XResD * MDRAW_XSCR;
	RightRect.right = pCA->pDD_Display->i_XResD;

	BottomRect.top = pCA->pDD_Display->i_YResD * MDRAW_YSCR;
	BottomRect.bottom = pCA->pDD_Display->i_YResD;
	BottomRect.left = 0;
	BottomRect.right = pCA->pDD_Display->i_XResD * MDRAW_XSCR;

	pCA->pDD_Display->Colorfill3(pDrawSurf, pCA->pColorTable->MM_Players_Console[pCA->pGameOptions->iControl], BottomRect.top, BottomRect.bottom, BottomRect.left, BottomRect.right);
	pCA->pDD_Display->Colorfill3(pDrawSurf, pCA->pColorTable->MM_Players_Console[pCA->pGameOptions->iControl], RightRect.top, RightRect.bottom, RightRect.left, RightRect.right);



//disp tutorial console
if ( pCA->pGameOptions->bTutorial == TRUE)
if (pCA->pInterface->Tutorial.iActiveElement < 0)
if (strlen(pCA->pInterface->TutorialPersistentMessageBuffer) > 0)
{
	RECT R0;
	R0.left = pCA->pDD_Display->i_XResD *0.5;
	R0.right = R0.left + 300;
	R0.top = 0;
	R0.bottom = pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 10;

	Draw_Interface_Panel(pDrawSurf, R0);
	//draw text
	Draw_Text(pDrawSurf, pCA->pInterface->TutorialPersistentMessageBuffer, R0.left + 5, R0.top + 5, FONT_STANDARD1, R0.right - 5);
}




//DEBUG
Write_Log("[Disp Console]Drawing Display Box");
WriteLog_Enter();


//draw console igrids and...stuff
//blit the buttons of the active grid

cIGrid* pIG_Active = &pCA->pInterface->IBoxes[IBOX_CONSOLE_DISPLAY].pIGrids[pCA->pInterface->IBoxes[IBOX_CONSOLE_DISPLAY].iActiveGrid];

//finish by drawing the interface elements
	Disp_IfcElems(pIG_Active, 0, 0);




//DEBUG
Write_Log("[Disp Console]Drawing Equip Box");
WriteLog_Enter();


//draw console igrids and...stuff
//blit the buttons of the active grid

pIG_Active = &pCA->pInterface->IBoxes[IBOX_CONSOLE_EQUIP].pIGrids[pCA->pInterface->IBoxes[IBOX_CONSOLE_EQUIP].iActiveGrid];

//finish by drawing the interface elements
	Disp_IfcElems(pIG_Active, 0, 0);

//DEBUG
Write_Log("[Disp Console]Drawing Console_Command Box");
WriteLog_Enter();
//03.29.2003
pIG_Active = &pCA->pInterface->IBoxes[IBOX_CONSOLE_COMMAND].pIGrids[pCA->pInterface->IBoxes[IBOX_CONSOLE_COMMAND].iActiveGrid];

//finish by drawing the interface elements
	Disp_IfcElems(pIG_Active, 0, 0);


//DEBUG
Write_Log("[Disp Console]Drawing Console_List Box");
WriteLog_Enter();
pIG_Active = &pCA->pInterface->IBoxes[IBOX_CONSOLE_LIST].pIGrids[pCA->pInterface->IBoxes[IBOX_CONSOLE_LIST].iActiveGrid];

//finish by drawing the interface elements
	Disp_IfcElems(pIG_Active, 0, 0);

//DEBUG
Write_Log("[Disp Console]Drawing Console_Info Box");
WriteLog_Enter();
pIG_Active = &pCA->pInterface->IBoxes[IBOX_CONSOLE_INFO].pIGrids[pCA->pInterface->IBoxes[IBOX_CONSOLE_INFO].iActiveGrid];

//finish by drawing the interface elements
	Disp_IfcElems(pIG_Active, 0,0);

//DEBUG
Write_Log("[Disp Console]Drawing Console_Chat Box");
WriteLog_Enter();
if (pCA->pInterface->IBoxes[IBOX_CONSOLE_CHAT].bActiveBox == TRUE)
{
pIG_Active = &pCA->pInterface->IBoxes[IBOX_CONSOLE_CHAT].pIGrids[pCA->pInterface->IBoxes[IBOX_CONSOLE_CHAT].iActiveGrid];

//finish by drawing the interface elements
	Disp_IfcElems(pIG_Active, 0,0);

}

//DEBUG
Write_Log("[Disp Console]Drawing Console_Notify Box");
WriteLog_Enter();
if (pCA->pInterface->IBoxes[IBOX_NOTIFY].bActiveBox == TRUE)
{
pIG_Active = &pCA->pInterface->IBoxes[IBOX_NOTIFY].pIGrids[pCA->pInterface->IBoxes[IBOX_NOTIFY].iActiveGrid];

//finish by drawing the interface elements
	Disp_IfcElems(pIG_Active, 0,0);

}
//DEBUG
Write_Log("[Disp Console]Function Finished");
WriteLog_Enter();

//HACK: display display text at bottom line - 20
	int iOldTextColor = pCA->pGDisplay->dwFontColor;

	pCA->pFontInfoTable->SetFontColor(pCA->pColorTable->Green);

	if (pCA->pInterface->pDisplayText != NULL)
			pCA->pGDisplay->Draw_Text(pDrawSurf, pCA->pInterface->pDisplayText, pCA->pDD_Display->i_XResD * 0.825, pCA->pDD_Display->i_YResD * 0.975, FONT_STANDARD1);	
	pCA->pFontInfoTable->SetFontColor(iOldTextColor);	

	//disp resources
	RECT Panel;
	Panel.left = pCA->pDD_Display->i_XResD * 0.82;
	Panel.right = pCA->pDD_Display->i_XResD * 0.98;
	Panel.top = pCA->pDD_Display->i_YResD * 0.27;
	Panel.bottom = (pCA->pDD_Display->i_YResD * 0.27) + pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 2.4;

	Draw_Interface_Panel(pDrawSurf, Panel);

	Panel.left = pCA->pDD_Display->i_XResD * 0.86;
	Panel.right = pCA->pDD_Display->i_XResD * 0.98 - 5;
	Panel.top = (pCA->pDD_Display->i_YResD * 0.27) + pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 0.3;
	Panel.bottom = (pCA->pDD_Display->i_YResD * 0.27) + pCA->pFontInfoTable->Fonts[FONT_STANDARD1].iHeight * 2.1;

	Draw_Interface_ListBox(pDrawSurf, Panel);

	pCA->pFontInfoTable->SetFontColor(pCA->pColorTable->White - 1);
	itoa((int)pCA->pNationManager->ppNations[pCA->pGameOptions->iControl]->ddResources, NText, 10);
	Draw_Text( pDrawSurf, NText, Panel.left + 10, Panel.top + 5, FONT_STANDARD1);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cBlitQueue_USP::Init(VOID* pCA)
{
	pCAccess = pCA;



//allocate and link
int lc1; //local counter
	for(lc1 = 0; lc1 < NUM_ZLEVELS; lc1++)
	{
		Levels[lc1] = new cBQUNode[TEMP_BLITQUEUE_SLOTS];

		iHighestQueued[lc1] = -1;

/* removed for alternate queueing scheme
		pBQU_First[lc1] = new cBQUNode;
		pBQU_Last[lc1] = new cBQUNode;

		pBQU_First[lc1]->pPrev = NULL;
		pBQU_First[lc1]->pNext = (void*)pBQU_Last[lc1];
		pBQU_Last[lc1]->pNext = NULL;
		pBQU_Last[lc1]->pPrev = (void*)pBQU_First[lc1];  */
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cBlitQueue_USP::Submit_BlitRequest(int iZLevel, DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE7 pDDS, LPRECT lpSrcRect, DWORD dwTrans, int iBQU_BlitType, DWORD dwOverlayColor, BYTE* pExBuffer)
{


	if (iHighestQueued[iZLevel] >= TEMP_BLITQUEUE_SLOTS)
		return;

	if (iHighestQueued[iZLevel] < -1)
		iHighestQueued[iZLevel] = -1;

	cBQUNode* pBQU_New = &Levels[iZLevel][iHighestQueued[iZLevel] + 1];

	pBQU_New->iZLevel = iZLevel;
	pBQU_New->dwX = dwX;
	pBQU_New->dwY = dwY;
	pBQU_New->pDDS = pDDS;
	if (lpSrcRect == NULL)
		pBQU_New->lpSrcRect = NULL;
	else
		pBQU_New->lpSrcRect = &pBQU_New->StoreRect;

	pBQU_New->wFrameHeight = 0;
	if (lpSrcRect != NULL)
	{
		pBQU_New->wFrameHeight = lpSrcRect->bottom - lpSrcRect->top;
		pBQU_New->lpSrcRect->bottom = lpSrcRect->bottom;
		pBQU_New->lpSrcRect->top = lpSrcRect->top;
		pBQU_New->lpSrcRect->left = lpSrcRect->left;
		pBQU_New->lpSrcRect->right = lpSrcRect->right;
	}
	pBQU_New->dwTrans = dwTrans;
	pBQU_New->iBQU_BlitType = iBQU_BlitType;
	pBQU_New->dwOverlayColor = dwOverlayColor;
	pBQU_New->pExBuffer = pExBuffer;
	
	iHighestQueued[iZLevel]++;
/*
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	//allocate
	cBQUNode* pBQU_New = new cBQUNode;

	int inullrectflag = 0;
/*DEBUG END DB*/
/*pCA->pGDisplay->Write_Log("[Submit BlitRequests] Beginning Function...(assigning data)");
pCA->pGDisplay->WriteLog_Enter();
char NText[50];
pCA->pGDisplay->Write_Log("ZLevel: ");
itoa(iZLevel, NText, 10);
pCA->pGDisplay->Write_Log(NText);
pCA->pGDisplay->WriteLog_Enter();
pCA->pGDisplay->Write_Log("LPRECT address: ");
itoa((int)lpSrcRect, NText, 10);
pCA->pGDisplay->Write_Log(NText);
pCA->pGDisplay->WriteLog_Enter();




	//set params
	pBQU_New->iZLevel = iZLevel;
	pBQU_New->dwX = dwX;
	pBQU_New->dwY = dwY;
	pBQU_New->pDDS = pDDS;
	pBQU_New->lpSrcRect = NULL;
	pBQU_New->wFrameHeight = 0;
	if (lpSrcRect != NULL)
	{
		pBQU_New->wFrameHeight = lpSrcRect->bottom - lpSrcRect->top;
		pBQU_New->lpSrcRect = new RECT;
		pBQU_New->lpSrcRect->bottom = lpSrcRect->bottom;
		pBQU_New->lpSrcRect->top = lpSrcRect->top;
		pBQU_New->lpSrcRect->left = lpSrcRect->left;
		pBQU_New->lpSrcRect->right = lpSrcRect->right;
	}




	pBQU_New->dwTrans = dwTrans;
	pBQU_New->iBQU_BlitType = iBQU_BlitType;
	pBQU_New->dwOverlayColor = dwOverlayColor;
	pBQU_New->pExBuffer = pExBuffer;

//DEBUG
pCA->pGDisplay->Write_Log("[Submit BlitRequests] Beginning traversal");
pCA->pGDisplay->WriteLog_Enter();
//END DB

	//link in appropriate buffer // !sort!!!!
	cBQUNode* pBQU_trav = pBQU_First[iZLevel];

BOOL bStopFlag = FALSE;


	//this will cause the traverser to stop either at the end or node that will come after the new node we are placing (either way, same insertion procedure)
	while (pBQU_trav != pBQU_Last[iZLevel] && bStopFlag == FALSE)
	{
//DEBUG
pCA->pGDisplay->Write_Log("[Submit BlitRequests]	Traverse");
pCA->pGDisplay->WriteLog_Enter();
//
		pBQU_trav = (cBQUNode*)pBQU_trav->pNext;
		if (iBQU_BlitType == BQU_BLITTYPE_SHADOW)
			bStopFlag = TRUE;
		
		//we want the LOWEST dwY values to come first in the buffer
		if (pBQU_trav->dwY + pBQU_trav->wFrameHeight > dwY + pBQU_New->wFrameHeight)
			bStopFlag = TRUE;
	}

//DEBUG
pCA->pGDisplay->Write_Log("[Submit BlitRequests] Performing Linking....");
pCA->pGDisplay->WriteLog_Enter();
//

	//now perform the linkup
	cBQUNode* pBQU_Next = pBQU_trav;
	cBQUNode* pBQU_Prev = (cBQUNode*)pBQU_Next->pPrev;

	pBQU_Next->pPrev = (void*)pBQU_New;
	pBQU_Prev->pNext = (void*)pBQU_New;
	pBQU_New->pNext = (void*)pBQU_Next;
	pBQU_New->pPrev = (void*)pBQU_Prev;

//DEBUG
pCA->pGDisplay->Write_Log("[Submit BlitRequests] Function Complete.");
pCA->pGDisplay->WriteLog_Enter();
//
*/

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cBlitQueue_USP::Flush_Buffers(int iIndex_Low, int iIndex_High)
{
	
int lc1; //local counter
	
for (lc1 = iIndex_Low; lc1 <= iIndex_High; lc1++)
	iHighestQueued[lc1] = -1;

/*	

	cBQUNode* pBQU_trav;
	cBQUNode* pBQU_trav2;

	for (lc1 = iIndex_Low; lc1 <= iIndex_High; lc1++)
	{

		pBQU_trav = (cBQUNode*)pBQU_First[lc1]->pNext;

		while (pBQU_trav != pBQU_Last[lc1])
		{
			pBQU_trav2 = pBQU_trav;
			pBQU_trav = (cBQUNode*)pBQU_trav->pNext;

			if (pBQU_trav2->lpSrcRect != NULL)
				delete(pBQU_trav2->lpSrcRect);
			delete(pBQU_trav2);
		}

		//and then just relink
		pBQU_First[lc1]->pNext = (void*)pBQU_Last[lc1];
		pBQU_Last[lc1]->pPrev = (void*)pBQU_First[lc1];
	}
*/
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cBlitQueue_USP::Dispatch_BlitRequests(LPDIRECTDRAWSURFACE7 pDrawSurf, int iIndex_Low, int iIndex_High)
{
	
int lc1; //local counter

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	//DEBUG
	pCA->pGDisplay->Write_Log("Beginning dispatch blit requests.");
	pCA->pGDisplay->WriteLog_Enter();

	cBQUNode* pBQU_trav;
	cBQUNode* pBQU_trav2;

/*DEBUG     END DB*/
char NText[20];
//pCA->pGDisplay->Write_Log("[Dispatch BlitRequests] Beginning Function...");
//pCA->pGDisplay->WriteLog_Enter();

	for (lc1 = iIndex_Low; lc1 <= iIndex_High; lc1++)
	for (int lc2 = 0; lc2 <= iHighestQueued[lc1]; lc2++)
	{
		
		pBQU_trav = &(Levels[lc1][lc2]);//(cBQUNode*)pBQU_First[lc1]->pNext;

/* changed for alternate queueing scheme
		while (pBQU_trav != pBQU_Last[lc1])
		{
*/
/*DEBUG

pCA->pGDisplay->Write_Log("[Dispatch BlitRequests] Blitting Request... ZLevel: ");
itoa(pBQU_trav->iZLevel, NText, 10);
pCA->pGDisplay->Write_Log(NText);
pCA->pGDisplay->WriteLog_Enter();
END DB*/

				//clip (not that it might not appear to make sense
				//that I am testing for negative DWORDS(which are supposed to be unsigned)...
				//the reason for that is that dwX and dwY have been changed to INTs :-/ (01.24.2005)
				if (pBQU_trav->dwX < 0)
				{
					pBQU_trav->lpSrcRect->left -= pBQU_trav->dwX ;
					pBQU_trav->dwX = 0;
				}
				if (pBQU_trav->dwY < 0)
				{
					pBQU_trav->lpSrcRect->top -= pBQU_trav->dwY ;
					pBQU_trav->dwY = 0;
				}

			if (pBQU_trav->iBQU_BlitType == BQU_BLITTYPE_NORMAL || pBQU_trav->iBQU_BlitType == BQU_BLITTYPE_OVERLAY || pBQU_trav->iBQU_BlitType == BQU_BLITTYPE_OBJECT_01)
			{


				pDrawSurf->BltFast(pBQU_trav->dwX, pBQU_trav->dwY, pBQU_trav->pDDS, pBQU_trav->lpSrcRect, pBQU_trav->dwTrans);

				/*DEBUG
				//pCA->pGDisplay->Write_Log("[Dispatch BlitRequests] IMAGE");
				//pCA->pGDisplay->WriteLog_Enter();
				END DB*/
			}

			if (pBQU_trav->iBQU_BlitType == BQU_BLITTYPE_OVERLAY || pBQU_trav->iBQU_BlitType == BQU_BLITTYPE_OV_ONLY || pBQU_trav->iBQU_BlitType == BQU_BLITTYPE_SHADOW)
			{

				if (pCA->pDD_Display->iDDRenderMode != DDRM_NORMAL)
					pCA->pGDisplay->ShadowBlt(pDrawSurf, pBQU_trav->dwX, pBQU_trav->dwY, pBQU_trav->pDDS, pBQU_trav->lpSrcRect, pCA->pColorTable->White, pBQU_trav->dwOverlayColor, pBQU_trav->pExBuffer);
				if (pCA->pDD_Display->iDDRenderMode == DDRM_NORMAL)
					pCA->pGDisplay->ShadowBltF(pDrawSurf, pBQU_trav->dwX, pBQU_trav->dwY, pBQU_trav->pDDS, pBQU_trav->lpSrcRect, pCA->pColorTable->White, pBQU_trav->dwOverlayColor, pBQU_trav->pExBuffer);
					

				/*DEBUG
				//pCA->pGDisplay->Write_Log("[Dispatch BlitRequests] SHADOW");
				//pCA->pGDisplay->WriteLog_Enter();
				END DB*/
			}
			if (pBQU_trav->iBQU_BlitType == BQU_BLITTYPE_OBJECT_01)
			{

				if (pCA->pDD_Display->iDDRenderMode != DDRM_NORMAL)
					pCA->pGDisplay->EmBlt(pDrawSurf, pBQU_trav->dwX, pBQU_trav->dwY, pBQU_trav->pDDS, pBQU_trav->lpSrcRect, pCA->pColorTable->White, pBQU_trav->dwOverlayColor);
				if (pCA->pDD_Display->iDDRenderMode == DDRM_NORMAL)
					pCA->pGDisplay->EmBltF(pDrawSurf, pBQU_trav->dwX, pBQU_trav->dwY, pBQU_trav->pDDS, pBQU_trav->lpSrcRect, pCA->pColorTable->White, pBQU_trav->dwOverlayColor, pBQU_trav->pExBuffer);

				//DEBUG
				//pCA->pGDisplay->Write_Log("[Dispatch BlitRequests] EMBLEM");
				//pCA->pGDisplay->WriteLog_Enter();
				
			}


			if (pBQU_trav->iBQU_BlitType == BQU_BLITTYPE_ALPHA)
			{

	//DEBUG
	pCA->pGDisplay->Write_Log("Beginning alphablt.");
	pCA->pGDisplay->WriteLog_Enter();

				if (pCA->pDD_Display->iDDRenderMode != DDRM_NORMAL)
					pCA->pGDisplay->AlphaBlt(pDrawSurf, pBQU_trav->dwX, pBQU_trav->dwY, pBQU_trav->pDDS, pBQU_trav->lpSrcRect, pCA->pColorTable->White);
//				if (pCA->pDD_Display->iDDRenderMode == DDRM_NORMAL)	
//					pCA->pGDisplay->AlphaBltF(pDrawSurf, pBQU_trav->dwX, pBQU_trav->dwY, pBQU_trav->pDDS, pBQU_trav->lpSrcRect, pCA->pColorTable->White);

	//DEBUG
	pCA->pGDisplay->Write_Log("End alphablt.");
	pCA->pGDisplay->WriteLog_Enter();


				//DEBUG
				//pCA->pGDisplay->Write_Log("[Dispatch BlitRequests] ALPHA");
				//pCA->pGDisplay->WriteLog_Enter();
				//END DB*/
			}

/*DEBUG
//pCA->pGDisplay->Write_Log("[Dispatch BlitRequests] Blit Complete!...");
//pCA->pGDisplay->WriteLog_Enter();
END DB*/

/* changed for alternate queueing scheme
			pBQU_trav = (cBQUNode*)pBQU_trav->pNext;

		}  */
 
	}

	//DEBUG
	pCA->pGDisplay->Write_Log("End dispatch blit requests.");
	pCA->pGDisplay->WriteLog_Enter();


/*DEBUG
//pCA->pGDisplay->Write_Log("[Dispatch BlitRequests] Flushing Buffers...");
//pCA->pGDisplay->WriteLog_Enter();
END DB*/
	//then flush the buffers
	Flush_Buffers(iIndex_Low, iIndex_High);

/*DEBUG
//pCA->pGDisplay->Write_Log("[Dispatch BlitRequests] Function Complete...");
//pCA->pGDisplay->WriteLog_Enter();
END DB*/

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cBlitQueue_USP::Exit()
{

	//flush buffers
	Flush_Buffers(0, NUM_ZLEVELS - 1);

int lc1; 
	//kill pointers
	for (lc1 = 0; lc1 < NUM_ZLEVELS; lc1++)
	{
		delete [] Levels[lc1];

/* changed for new queueing scheme
		delete(pBQU_First[lc1]);
		delete(pBQU_Last[lc1]);  */
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::AlphaBltF(LPDIRECTDRAWSURFACE7 pDDS_Dest, int dwX, int dwY, LPDIRECTDRAWSURFACE7 pDDS_Source, LPRECT lpSrcRect, DWORD dwColorKey)
{
	//("FAKE") alpha blitting
	//the first approach that could be used to do this could be through selectively only blitting every other pixel
	//to the screen(a very large number of tiny blits)
	//another way could be by blitting the entire image onto a buffer, writing every other pixel to the source color key, and then blitting that to screen
	//(then it's two blits and an access)
	//try the second way as the first way is apparently slower
cClassAccess* pCA = (cClassAccess*)pCAccess;

DDCOLORKEY DDCK;
	DDCK.dwColorSpaceHighValue = dwColorKey;
	DDCK.dwColorSpaceLowValue = dwColorKey;

	pCA->pDD_Display->pDDS_Alpha->SetColorKey(DDCKEY_SRCBLT, &DDCK);

	//blit the image to the alpha surface
	pCA->pDD_Display->pDDS_Alpha->BltFast(0, 0, pDDS_Source, lpSrcRect, DDBLTFAST_WAIT);
	
	//between these two lines, we lock the surface and set every other pixel to the color key
	ZeroMemory(&LockDescDest, sizeof(DDSURFACEDESC2) );
	LockDescDest.dwSize = sizeof(DDSURFACEDESC2);
	pCA->pDD_Display->pDDS_Alpha->Lock(NULL, &LockDescDest, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);

		//and blit this to the main surface for now
		RECT SrcRect2;
		SrcRect2.top = 0;
		SrcRect2.left = 0;
		SrcRect2.right = lpSrcRect->right - lpSrcRect->left;
		SrcRect2.bottom = lpSrcRect->bottom - lpSrcRect->top;

		//
		if (pCA->pDD_Display->iBPP == 32)
		{
			DWORD* pDWSurface = (DWORD*)LockDescDest.lpSurface;
			DWORD dwPitch = LockDescDest.lPitch / 4;

			int cx, cy;

			for (cy = 0; cy < SrcRect2.bottom; cy++)
			for (cx = 0; cx < SrcRect2.right; cx++)
			{
				if ( (cx + cy) % 2 == 0)
					*(pDWSurface + cx + (cy * dwPitch)) = dwColorKey;

			}
		}
		if (pCA->pDD_Display->iBPP == 16)
		{
			WORD* pWSurface = (WORD*)LockDescDest.lpSurface;
			DWORD dwPitch = LockDescDest.lPitch / 2;

			int cx, cy;

			for (cy = 0; cy < SrcRect2.bottom; cy++)
			for (cx = 0; cx < SrcRect2.right; cx++)
			{
				if ( (cx + cy) % 2 == 0)
					*(pWSurface + cx + (cy * dwPitch)) = dwColorKey;

			}
		}			

	pCA->pDD_Display->pDDS_Alpha->Unlock(NULL);

	pDDS_Dest->BltFast(dwX, dwY, pCA->pDD_Display->pDDS_Alpha, &SrcRect2, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::AlphaTintRectF(LPDIRECTDRAWSURFACE7 pDDS_Dest, RECT Target, DWORD dwAlphaClr)
{
cClassAccess* pCA = (cClassAccess*)pCAccess;



	//between these two lines, we lock the surface and set every other pixel to the color key
	ZeroMemory(&LockDescDest, sizeof(DDSURFACEDESC2) );
	LockDescDest.dwSize = sizeof(DDSURFACEDESC2);
	pDDS_Dest->Lock(NULL, &LockDescDest, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);

		//
		if (pCA->pDD_Display->iBPP == 32)
		{
			DWORD* pDWSurface = (DWORD*)LockDescDest.lpSurface;
			DWORD dwPitch = LockDescDest.lPitch / 4;

			int cx, cy;

			for (cy = Target.top; cy < Target.bottom; cy++)
			for (cx = Target.left; cx < Target.right; cx++)
			{
				if ( (cx + cy) % 2 == 0)
					*(pDWSurface + cx + (cy * dwPitch)) = dwAlphaClr;

			}
		}
		if (pCA->pDD_Display->iBPP == 16)
		{
			WORD* pWSurface = (WORD*)LockDescDest.lpSurface;
			DWORD dwPitch = LockDescDest.lPitch / 2;

			int cx, cy;

			for (cy = Target.top; cy < Target.bottom; cy++)
			for (cx = Target.left; cx < Target.right; cx++)
			{
				if ( (cx + cy) % 2 == 0)
					*(pWSurface + cx + (cy * dwPitch)) = dwAlphaClr;

			}
		}			

	pDDS_Dest->Unlock(NULL);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::AlphaTintRect(LPDIRECTDRAWSURFACE7 pDDS_Dest, RECT Target, DWORD dwAlphaClr)
{
cClassAccess* pCA = (cClassAccess*)pCAccess;

	//lock destination surface
	ZeroMemory(&LockDescDest, sizeof(DDSURFACEDESC2));

	//
	LockDescDest.dwSize = sizeof(DDSURFACEDESC2);
	//
	DDSURFACEDESC2 SurfaceDesc;
	ZeroMemory(&SurfaceDesc, sizeof(DDSURFACEDESC2));
	SurfaceDesc.dwSize = sizeof(DDSURFACEDESC2);

	HRESULT hr23 = pDDS_Dest->GetSurfaceDesc(&SurfaceDesc);

int Pitch = pCA->pDD_Display->DDSDmain.lPitch;

int lcx, lcy; //local counters

	pDDS_Dest->Lock(NULL, &LockDescDest, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);

		DWORD AM = pCA->pDD_Display->AMask;
		DWORD W_AM = (WORD)pCA->pDD_Display->AMask;
		

		int iDest_Height = SurfaceDesc.dwHeight;
		int iDest_Width = SurfaceDesc.dwWidth;
		int iDest_Pitch = SurfaceDesc.lPitch;

		int iDestX = Target.left;
		int iDestY = Target.top;

		

if (pCA->pDD_Display->iBPP == 16)
	{
WORD* pW_Dest;
WORD* pW_DestPoint;
WORD AlphaClr_prc = ((WORD)dwAlphaClr & W_AM) >> 1;

		pW_Dest = (WORD*)LockDescDest.lpSurface;
		pW_DestPoint = pW_Dest + (iDestY * (iDest_Pitch / 2)) + iDestX;

		//source FOR both....because in this case iDest_Width is the width of the ENTIRE destination surface
		int iDestCarriageReturn = (iDest_Pitch / 2) - (Target.right - Target.left);

		for (lcy = 0; lcy < Target.bottom - Target.top; lcy++)
			{
			for (lcx = 0; lcx < Target.right - Target.left; lcx++)
			{
				if ( (lcx%2) == (lcy%2))
					*pW_DestPoint = ((*pW_DestPoint &  W_AM) >> 1) + AlphaClr_prc;
				pW_DestPoint++;
			}
			pW_DestPoint += iDestCarriageReturn;
		}

	}
	if (pCA->pDD_Display->iBPP == 32)
	{
DWORD* pDW_Dest;
DWORD* pDW_DestPoint;
DWORD AlphaClr_prc = (dwAlphaClr & AM) >> 1;

		pDW_Dest = (DWORD*)LockDescDest.lpSurface;
		pDW_DestPoint = pDW_Dest + (iDestY * (iDest_Pitch / 4)) + iDestX;

		//source FOR both....because in this case iDest_Width is the width of the ENTIRE destination surface
		int iDestCarriageReturn = (iDest_Pitch / 4) - (Target.right - Target.left);

		for (lcy = 0; lcy < Target.bottom - Target.top; lcy++)
			{
			for (lcx = 0; lcx < Target.right - Target.left; lcx++)
			{
				if ( (lcx%2) == (lcy%2))
					*pDW_DestPoint = ((*pDW_DestPoint &  AM) >> 1) + AlphaClr_prc;
				pDW_DestPoint++;
			}
			pDW_DestPoint += iDestCarriageReturn;
		}

	}	

	pDDS_Dest->Unlock(NULL);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::AlphaBlt(LPDIRECTDRAWSURFACE7 pDDS_Dest, int dwX, int dwY, LPDIRECTDRAWSURFACE7 pDDS_Source, LPRECT lpSrcRect, DWORD dwColorKey)
{

cClassAccess* pCA = (cClassAccess*)pCAccess;



	//lock both surfaces
	ZeroMemory(&LockDescDest, sizeof(DDSURFACEDESC2) );
	ZeroMemory(&LockDescSource, sizeof(DDSURFACEDESC2) );

	LockDescDest.dwSize = sizeof(DDSURFACEDESC2);
	LockDescSource.dwSize = sizeof(DDSURFACEDESC2);

	DDSURFACEDESC2 SurfaceDesc;
	DDSURFACEDESC2 SurfaceDesc2;

	ZeroMemory(&SurfaceDesc, sizeof(DDSURFACEDESC2) );
	SurfaceDesc.dwSize = sizeof(DDSURFACEDESC2);

	ZeroMemory(&SurfaceDesc2, sizeof(DDSURFACEDESC2) );
	SurfaceDesc2.dwSize = sizeof(DDSURFACEDESC2);
	
	HRESULT hr23 = pDDS_Source->GetSurfaceDesc(&SurfaceDesc);
					pDDS_Dest->GetSurfaceDesc(&SurfaceDesc2);

const char* KText;
/*DEBUG
//KText = DXGetErrorString9(hr23);
//Write_Log("GetSurfaceDesc: ");
//Write_Log((char*)KText);
//WriteLog_Enter();
END DB*/

int Pitch = pCA->pDD_Display->DDSDmain.lPitch;

int lcx, lcy; //local counters

	HRESULT hr20 = pDDS_Dest->Lock(NULL, &LockDescDest, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
	HRESULT hr21 = pDDS_Source->Lock(NULL, &LockDescSource, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);


		DWORD AM = pCA->pDD_Display->AMask;
		WORD W_AM = (WORD)pCA->pDD_Display->AMask;

		int iSource_Height = SurfaceDesc.dwHeight;
		int iSource_Width = SurfaceDesc.dwWidth;
		int iSource_Pitch = SurfaceDesc.lPitch;
		int iDest_Height = SurfaceDesc2.dwHeight;
		int iDest_Width = SurfaceDesc2.dwWidth;
		int iDest_Pitch = SurfaceDesc2.lPitch;

		RECT SourceRect;

		SourceRect.top = lpSrcRect->top;
		SourceRect.bottom = lpSrcRect->bottom;
		SourceRect.right = lpSrcRect->right;
		SourceRect.left = lpSrcRect->left;

		int iDestX = dwX;
		int iDestY = dwY;

		if (iDestX < 0)
		{
			SourceRect.left += 0 - iDestX;
			iDestX = 0;
				
		}
		if (iDestX >= iDest_Width - 1 - (SourceRect.right - SourceRect.left))
		{
			SourceRect.right -= iDestX - (iDest_Width - 1 - (SourceRect.right - SourceRect.left));
			iDestX = iDest_Width - 2 - (SourceRect.right - SourceRect.left);
		}
		if (iDestY < 0)
		{
			SourceRect.top += 0 - iDestY;
			iDestY = 0;
				
		}
		if (iDestY >= iDest_Height - 1 - (SourceRect.bottom - SourceRect.top))
		{
			SourceRect.bottom -= iDestY - (iDest_Height - 1 - (SourceRect.bottom- SourceRect.top));
			iDestY = iDest_Height - 2 - (SourceRect.bottom - SourceRect.top);
		}


	if (pCA->pDD_Display->iBPP == 16)
	{
WORD* pW_Dest;
WORD* pW_DestPoint;
WORD* pW_Source;
WORD* pW_SourcePoint;

		pW_Dest = (WORD*)LockDescDest.lpSurface;
		pW_Source = (WORD*)LockDescSource.lpSurface;
		
		pW_SourcePoint = pW_Source + (SourceRect.top * (iSource_Pitch / 2)) + SourceRect.left;
		pW_DestPoint = pW_Dest + (iDestY * (iDest_Pitch / 2)) + iDestX;

		//source FOR both....because in this case iDest_Width is the width of the ENTIRE destination surface
		int iDestCarriageReturn = (iDest_Pitch / 2) - (SourceRect.right - SourceRect.left);
		int iSourceCarriageReturn = (iSource_Pitch / 2) - (SourceRect.right - SourceRect.left);


		for (lcy = 0; lcy < SourceRect.bottom - SourceRect.top; lcy++)
			{
			for (lcx = 0; lcx < SourceRect.right - SourceRect.left; lcx++)
			{
				if (*pW_SourcePoint != dwColorKey)
					*pW_DestPoint = ((*pW_DestPoint &  W_AM) >> 1) + ((*pW_SourcePoint & W_AM) >> 1);

				pW_DestPoint++;
				pW_SourcePoint++;
			}
			pW_DestPoint += iDestCarriageReturn;
			pW_SourcePoint += iSourceCarriageReturn;
		}

	}
	if (pCA->pDD_Display->iBPP == 32)
	{
DWORD* pDW_Dest;
DWORD* pDW_DestPoint;
DWORD* pDW_Source;
DWORD* pDW_SourcePoint;

		pDW_Dest = (DWORD*)LockDescDest.lpSurface;
		pDW_Source = (DWORD*)LockDescSource.lpSurface;
		
		pDW_SourcePoint = pDW_Source + (SourceRect.top * (iSource_Pitch / 4)) + SourceRect.left;
		pDW_DestPoint = pDW_Dest + (iDestY * (iDest_Pitch / 4)) + iDestX;

		//source FOR both....because in this case iDest_Width is the width of the ENTIRE destination surface
		int iDestCarriageReturn = (iDest_Pitch / 4) - (SourceRect.right - SourceRect.left);
		int iSourceCarriageReturn = (iSource_Pitch / 4) - (SourceRect.right - SourceRect.left);


		for (lcy = 0; lcy < SourceRect.bottom - SourceRect.top; lcy++)
			{
			for (lcx = 0; lcx < SourceRect.right - SourceRect.left; lcx++)
			{
				if (*pDW_SourcePoint != pCA->pColorTable->White)
					*pDW_DestPoint = ((*pDW_DestPoint &  AM) >> 1) + ((*pDW_SourcePoint & AM) >> 1);

				pDW_DestPoint++;
				pDW_SourcePoint++;
			}
			pDW_DestPoint += iDestCarriageReturn;
			pDW_SourcePoint += iSourceCarriageReturn;
		}

	}


	pDDS_Source->Unlock(NULL);
	pDDS_Dest->Unlock(NULL);
	

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::ShadowBltF(LPDIRECTDRAWSURFACE7 pDDS_Dest, int dwX, int dwY, LPDIRECTDRAWSURFACE7 pDDS_Source, LPRECT lpSrcRect, DWORD dwColorKey, DWORD dwRefColor, BYTE* pExBuffer)
{
cClassAccess* pCA = (cClassAccess*)pCAccess;

//it turns out that faux shadowing is even tougher than faux alpha blending, as we must also turn any non-colorkey pixels to black!
//consequently, we have to make use of the opacity buffer if it is available to tell us what is image and what is not (06.23.2004)

//clip rectangle
if (dwX < 0)
{
	lpSrcRect->left -= dwX;
	dwX -= dwX;
}
if (dwY < 0)
{
	lpSrcRect->top -= dwY;
	dwY -= dwY;
}

//get a description of the source surface width
DDSURFACEDESC2 DDSD2b;
ZeroMemory(&DDSD2b, sizeof(DDSURFACEDESC2));
DDSD2b.dwSize = sizeof(DDSURFACEDESC2);
pDDS_Source->GetSurfaceDesc(&DDSD2b);

DDSURFACEDESC2 DDSD2dest;
ZeroMemory(&DDSD2dest, sizeof(DDSURFACEDESC2));
DDSD2dest.dwSize = sizeof(DDSURFACEDESC2);
pDDS_Dest->GetSurfaceDesc(&DDSD2dest);

	//lock only the destination surface
	ZeroMemory(&LockDescDest, sizeof(DDSURFACEDESC2) );
	LockDescDest.dwSize = sizeof(DDSURFACEDESC2);
	pDDS_Dest->Lock(NULL, &LockDescDest, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);

	//
	DWORD dwPitch;
		if (pExBuffer != NULL)
		{

			int cx, cy;

			if ( pCA->pDD_Display->iBPP == 16)
			{
				
				dwPitch = DDSD2dest.lPitch / 2;
				WORD* pWSurfaceStart = (WORD*)LockDescDest.lpSurface;
				WORD* pWTrav;

				for (cy = 0; cy < lpSrcRect->bottom - lpSrcRect->top; cy++)
				for (cx = 0; cx < lpSrcRect->right - lpSrcRect->left; cx++)
				{

					if ( (cx + cy)%2 == 0)
					{
						if ( (pExBuffer[(cx + lpSrcRect->left) + (cy+lpSrcRect->top)*DDSD2b.dwWidth] & EXBUFFER_MASK_KEY) != 0)
						{	
							pWTrav = pWSurfaceStart + (cx + dwX) + ((cy + dwY) * dwPitch);
							*pWTrav = 0;
						}

					}
				}
			}
			if ( pCA->pDD_Display->iBPP == 32)
			{

				dwPitch = DDSD2dest.lPitch / 4;

				DWORD* pDWSurfaceStart = (DWORD*)LockDescDest.lpSurface;
				DWORD* pDWTrav;

				for (cy = 0; cy < lpSrcRect->bottom - lpSrcRect->top; cy++)
				for (cx = 0; cx < lpSrcRect->right - lpSrcRect->left; cx++)
				{
					if (  (cx + cy)%2 == 0  )
					{
						if ( (pExBuffer[(cx + lpSrcRect->left) + (cy+lpSrcRect->top)*DDSD2b.dwWidth] & EXBUFFER_MASK_KEY) != 0)
						{
							pDWTrav = pDWSurfaceStart + (cx + dwX) + ( (cy + dwY) * dwPitch);
							*pDWTrav = 0;			
						}

					}
				}
			}
		}

	
	pDDS_Dest->Unlock(NULL);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::ShadowBlt(LPDIRECTDRAWSURFACE7 pDDS_Dest, int dwX, int dwY, LPDIRECTDRAWSURFACE7 pDDS_Source, LPRECT lpSrcRect, DWORD dwColorKey, DWORD dwRefColor, BYTE* pExBuffer)
{

cClassAccess* pCA = (cClassAccess*)pCAccess;

	//lock both surfaces
	ZeroMemory(&LockDescDest, sizeof(DDSURFACEDESC2) );
	ZeroMemory(&LockDescSource, sizeof(DDSURFACEDESC2) );

	LockDescDest.dwSize = sizeof(DDSURFACEDESC2);
	LockDescSource.dwSize = sizeof(DDSURFACEDESC2);

	DDSURFACEDESC2 SurfaceDesc;
	DDSURFACEDESC2 SurfaceDesc2;

	ZeroMemory(&SurfaceDesc, sizeof(DDSURFACEDESC2) );
	SurfaceDesc.dwSize = sizeof(DDSURFACEDESC2);

	ZeroMemory(&SurfaceDesc2, sizeof(DDSURFACEDESC2) );
	SurfaceDesc2.dwSize = sizeof(DDSURFACEDESC2);
	
	HRESULT hr23 = pDDS_Source->GetSurfaceDesc(&SurfaceDesc);
					pDDS_Dest->GetSurfaceDesc(&SurfaceDesc2);


	pDDS_Dest->Lock(NULL, &LockDescDest, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
	pDDS_Source->Lock(NULL, &LockDescSource, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);


int Pitch = pCA->pDD_Display->DDSDmain.lPitch;

int lcx, lcy; //local counters

/*DEBUG
//Write_Log("[ShadowBlt] Locking...");
//WriteLog_Enter();
END DB*/


		DWORD AM = pCA->pDD_Display->AMask;

		int iSource_Height = SurfaceDesc.dwHeight;
		int iSource_Width = SurfaceDesc.dwWidth;
		int iSource_Pitch = SurfaceDesc.lPitch;
		int iDest_Height = SurfaceDesc2.dwHeight;
		int iDest_Width = SurfaceDesc2.dwWidth;
		int iDest_Pitch = SurfaceDesc2.lPitch;

		RECT SourceRect;

		SourceRect.top = lpSrcRect->top;
		SourceRect.bottom = lpSrcRect->bottom;
		SourceRect.right = lpSrcRect->right;
		SourceRect.left = lpSrcRect->left;

		int iDestX = dwX;
		int iDestY = dwY;

		if (iDestX < 0)
		{
			SourceRect.left += 0 - iDestX;
			iDestX = 0;
				
		}
		if (iDestX >= iDest_Width - 1 - (SourceRect.right - SourceRect.left))
		{
			SourceRect.right -= iDestX - (iDest_Width - 1 - (SourceRect.right - SourceRect.left));
			iDestX = iDest_Width - 1 - (SourceRect.right - SourceRect.left);
		}
		if (iDestY < 0)
		{
			SourceRect.top += 0 - iDestY;
			iDestY = 0;
				
		}
		if (iDestY >= iDest_Height - 1 - (SourceRect.bottom - SourceRect.top))
		{
			SourceRect.bottom -= iDestY - (iDest_Height - 1 - (SourceRect.bottom- SourceRect.top));
			iDestY = iDest_Height - 1 - (SourceRect.bottom - SourceRect.top);
		}



	if (pCA->pDD_Display->iBPP == 32)
	{
DWORD* pDW_Dest;
DWORD* pDW_DestPoint;
DWORD* pDW_Source;
DWORD* pDW_SourcePoint;

		pDW_Dest = (DWORD*)LockDescDest.lpSurface;
		pDW_Source = (DWORD*)LockDescSource.lpSurface;
		
		pDW_SourcePoint = pDW_Source + (SourceRect.top * (iSource_Pitch / 4)) + SourceRect.left;
		pDW_DestPoint = pDW_Dest + (iDestY * (iDest_Pitch / 4)) + iDestX;

		//source FOR both....because in this case iDest_Width is the width of the ENTIRE destination surface
		int iDestCarriageReturn = (iDest_Pitch / 4) - (SourceRect.right - SourceRect.left);
		int iSourceCarriageReturn = (iSource_Pitch / 4) - (SourceRect.right - SourceRect.left);

		DWORD CRef = (dwRefColor & AM) >> 1; 

		for (lcy = 0; lcy < SourceRect.bottom - SourceRect.top; lcy++)
			{
			for (lcx = 0; lcx < SourceRect.right - SourceRect.left; lcx++)
			{
				if (*pDW_SourcePoint != dwColorKey)
				{
					*pDW_DestPoint = ((*pDW_DestPoint &  AM) >> 1) + CRef;
				}

				pDW_DestPoint++;
				pDW_SourcePoint++;
			}
			pDW_DestPoint += iDestCarriageReturn;
			pDW_SourcePoint += iSourceCarriageReturn;
		}
	}

	if (pCA->pDD_Display->iBPP == 16)
	{
WORD* pW_Dest;
WORD* pW_Source;
WORD* pW_DestPoint;
WORD* pW_SourcePoint;

		pW_Dest = (WORD*)LockDescDest.lpSurface;
		pW_Source = (WORD*)LockDescSource.lpSurface;
		
		pW_SourcePoint = pW_Source + (SourceRect.top * (iSource_Pitch / 2)) + SourceRect.left;
		pW_DestPoint = pW_Dest + (iDestY * (iDest_Pitch / 2)) + iDestX;

		//source FOR both....because in this case iDest_Width is the width of the ENTIRE destination surface
		int iDestCarriageReturn = (iDest_Pitch / 2) - (SourceRect.right - SourceRect.left);
		int iSourceCarriageReturn = (iSource_Pitch / 2) - (SourceRect.right - SourceRect.left);

		DWORD CRef = (dwRefColor & AM) >> 1; 

		for (lcy = 0; lcy < SourceRect.bottom - SourceRect.top; lcy++)
			{
			for (lcx = 0; lcx < SourceRect.right - SourceRect.left; lcx++)
			{
				if (*pW_SourcePoint != dwColorKey)
				{
					*pW_DestPoint = ((*pW_DestPoint &  AM) >> 1) + CRef;
				}

				pW_DestPoint++;
				pW_SourcePoint++;
			}
			pW_DestPoint += iDestCarriageReturn;
			pW_SourcePoint += iSourceCarriageReturn;
		}
	}
	
	pDDS_Source->Unlock(NULL);
	pDDS_Dest->Unlock(NULL);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::EmBltF(LPDIRECTDRAWSURFACE7 pDDS_Dest, int dwX, int dwY, LPDIRECTDRAWSURFACE7 pDDS_Source, LPRECT lpSrcRect, DWORD dwColorKey, DWORD dwRefColor, BYTE* pExBuffer)
{
	//the suffix F implies "fake", as it follows in the tradition of AlphaBltF
	//however, this is actually a perfectly functional implementation
	//a downside to using DDRM_ALPHA is that pPrimaryRenderTarget consequently cannot support destination color keys and 
	//so emblem replacement must be done manually
	//thus EmBltF is the DDRM_NORMAL implementation that can use destination color keys (06.24.2004)
	//06.26.2004 It turns out that some systems can't use destination color keys at all. EmBltF has thus been
	//modified to work without, and is now similar in method to ShadowBltF
cClassAccess* pCA = (cClassAccess*)pCAccess;
	//

//clip rectangle
if (dwX < 0)
{
	lpSrcRect->left -= dwX;
	dwX -= dwX;
}
if (dwY < 0)
{
	lpSrcRect->top -= dwY;
	dwY -= dwY;
}

//DEBUG
char NText[50];

//get a description of the source surface width
DDSURFACEDESC2 DDSD2b;
ZeroMemory(&DDSD2b, sizeof(DDSURFACEDESC2));
DDSD2b.dwSize = sizeof(DDSURFACEDESC2);

HRESULT hr22 = pDDS_Source->GetSurfaceDesc(&DDSD2b);

while (hr22 != DD_OK)
{
	hr22 = pDDS_Source->GetSurfaceDesc(&DDSD2b);	
}

DDSURFACEDESC2 DDSD2dest;
ZeroMemory(&DDSD2dest, sizeof(DDSURFACEDESC2));
DDSD2dest.dwSize = sizeof(DDSURFACEDESC2);

HRESULT hr23 = pDDS_Dest->GetSurfaceDesc(&DDSD2dest);

while (hr23 != DD_OK)
{
	hr23 = pDDS_Dest->GetSurfaceDesc(&DDSD2dest);
}

	//lock only the destination surface
	ZeroMemory(&LockDescDest, sizeof(DDSURFACEDESC2) );
	LockDescDest.dwSize = sizeof(DDSURFACEDESC2);
	pDDS_Dest->Lock(NULL, &LockDescDest, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);

	//
	DWORD dwPitch;
		if (pExBuffer != NULL)
		{

			int cx, cy;

			if ( pCA->pDD_Display->iBPP == 16)
			{
				
				dwPitch = DDSD2dest.lPitch / 2;
				WORD* pWSurfaceStart = (WORD*)LockDescDest.lpSurface;
				WORD* pWTrav;

				for (cy = 0; cy < lpSrcRect->bottom - lpSrcRect->top; cy++)
				for (cx = 0; cx < lpSrcRect->right - lpSrcRect->left; cx++)
				{

					if ((pExBuffer[(cx + lpSrcRect->left) + (cy+lpSrcRect->top)*DDSD2b.dwWidth] & EXBUFFER_MASK_COLORMARK) != 0)
					{	
						pWTrav = pWSurfaceStart + (cx + dwX) + ((cy + dwY) * dwPitch);
						*pWTrav = dwRefColor;
					}
				}
			}
			if ( pCA->pDD_Display->iBPP == 32)
			{

				dwPitch = DDSD2dest.lPitch / 4;
/*
//DEBUG
Write_Log("Pitch: ");
itoa(dwPitch, NText, 10);
Write_Log(NText);
WriteLog_Enter();
Write_Log("Width: ");
itoa(DDSD2b.dwWidth, NText, 10);
Write_Log(NText);
WriteLog_Enter();
*/
				DWORD* pDWSurfaceStart = (DWORD*)LockDescDest.lpSurface;
				DWORD* pDWTrav;

				for (cy = 0; cy < lpSrcRect->bottom - lpSrcRect->top; cy++)
				for (cx = 0; cx < lpSrcRect->right - lpSrcRect->left; cx++)
				{
/*
//DEBUG
Write_Log("Source X: ");
itoa(cx + lpSrcRect->left, NText, 10);
Write_Log(NText);
Write_Log(" Source Y: ");
itoa(cy + lpSrcRect->top, NText, 10);
Write_Log(NText);
WriteLog_Enter();
*/
					if ((pExBuffer[(cx + lpSrcRect->left) + (cy+lpSrcRect->top)*DDSD2b.dwWidth] & EXBUFFER_MASK_COLORMARK) != 0)

					{
						pDWTrav = pDWSurfaceStart + (cx + dwX) + ( (cy + dwY) * dwPitch);

/*
//DEBUG
Write_Log("Dest X: ");
itoa(cx + dwX, NText, 10);
Write_Log(NText);
Write_Log(" Dest Y: ");
itoa(cy + dwY, NText, 10);
Write_Log(NText);
Write_Log(" Address: ");
itoa((int)pDWTrav, NText, 10);
Write_Log(NText);
WriteLog_Enter();  */

						*pDWTrav = dwRefColor;			
					}
	
				}
			}
		}

	
	pDDS_Dest->Unlock(NULL);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::EmBlt(LPDIRECTDRAWSURFACE7 pDDS_Dest, int dwX, int dwY, LPDIRECTDRAWSURFACE7 pDDS_Source, LPRECT lpSrcRect, DWORD dwColorKey, DWORD dwRefColor)
{
cClassAccess* pCA = (cClassAccess*)pCAccess;

/* Moved to EmBltF 06.24.2004
	//
RECT DestRect;
RECT EmSrcRect;
DestRect.top = dwY;
DestRect.bottom = dwY + (lpSrcRect->bottom - lpSrcRect->top);
DestRect.left = dwX;
DestRect.right = dwX + (lpSrcRect->right - lpSrcRect->left);

DDBLTFX DDBlitEffects;
ZeroMemory(&DDBlitEffects, sizeof(DDBLTFX) );

DDBlitEffects.ddckDestColorkey.dwColorSpaceHighValue = EMBLEM_REPLACE_COLOR;
DDBlitEffects.ddckDestColorkey.dwColorSpaceLowValue = EMBLEM_REPLACE_COLOR;
DDBlitEffects.dwFillColor = dwRefColor;
DDBlitEffects.dwSize = sizeof(DDBLTFX);

	//pDDS_Dest->Blt(&DestRect, pDDS_Source, lpSrcRect, DDBLT_COLORFILL, &DDBlitEffects);
	//first fill the scratch surface
	pCA->pDD_Display->pDDS_Scratch->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &DDBlitEffects);
	pDDS_Dest->Blt(&DestRect, pCA->pDD_Display->pDDS_Scratch, NULL, DDBLT_KEYDESTOVERRIDE, &DDBlitEffects);
*/


	//lock both surfaces
	ZeroMemory(&LockDescDest, sizeof(DDSURFACEDESC2) );
	ZeroMemory(&LockDescSource, sizeof(DDSURFACEDESC2) );

	LockDescDest.dwSize = sizeof(DDSURFACEDESC2);
	LockDescSource.dwSize = sizeof(DDSURFACEDESC2);

	DDSURFACEDESC2 SurfaceDesc;
	DDSURFACEDESC2 SurfaceDesc2;

	ZeroMemory(&SurfaceDesc, sizeof(DDSURFACEDESC2) );
	SurfaceDesc.dwSize = sizeof(DDSURFACEDESC2);

	ZeroMemory(&SurfaceDesc2, sizeof(DDSURFACEDESC2) );
	SurfaceDesc2.dwSize = sizeof(DDSURFACEDESC2);
	
	HRESULT hr23 = pDDS_Source->GetSurfaceDesc(&SurfaceDesc);
					pDDS_Dest->GetSurfaceDesc(&SurfaceDesc2);

const char* KText;
/*DEBUG
//KText = DXGetErrorString9(hr23);
//Write_Log("GetSurfaceDesc: ");
//Write_Log((char*)KText);
//WriteLog_Enter();
END DB*/

WORD* pW_Dest;
WORD* pW_Source;
WORD* pW_SourcePoint;
WORD* pW_DestPoint;
DWORD* pDW_Dest;
DWORD* pDW_DestPoint;
DWORD* pDW_Source;
DWORD* pDW_SourcePoint;

//int Pitch = pCA->pDD_Display->DDSDmain.lPitch;

int lcx, lcy; //local counters

/*DEBUG
//Write_Log("[AlphaBlt] Locking...");
//WriteLog_Enter();
END DB*/

	pDDS_Dest->Lock(NULL, &LockDescDest, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
	pDDS_Source->Lock(NULL, &LockDescSource, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);

		DWORD AM = pCA->pDD_Display->AMask;

		int iSource_Height = SurfaceDesc.dwHeight;
		int iSource_Width = SurfaceDesc.dwWidth;
		int iSource_Pitch = SurfaceDesc.lPitch;
		int iDest_Height = SurfaceDesc2.dwHeight;
		int iDest_Width = SurfaceDesc2.dwWidth;
		int iDest_Pitch = SurfaceDesc2.lPitch;

		RECT SourceRect;

		SourceRect.top = lpSrcRect->top;
		SourceRect.bottom = lpSrcRect->bottom;
		SourceRect.right = lpSrcRect->right;
		SourceRect.left = lpSrcRect->left;

		int iDestX = dwX;
		int iDestY = dwY;

		if (iDestX < 0)
		{
			SourceRect.left += 0 - iDestX;
			iDestX = 0;
				
		}
		if (iDestX >= iDest_Width - 1 - (SourceRect.right - SourceRect.left))
		{
			SourceRect.right -= iDestX - (iDest_Width - 1 - (SourceRect.right - SourceRect.left));
			iDestX = iDest_Width - 1 - (SourceRect.right - SourceRect.left);
		}
		if (iDestY < 0)
		{
			SourceRect.top += 0 - iDestY;
			iDestY = 0;
				
		}
		if (iDestY >= iDest_Height - 1 - (SourceRect.bottom - SourceRect.top))
		{
			SourceRect.bottom -= iDestY - (iDest_Height - 1 - (SourceRect.bottom- SourceRect.top));
			iDestY = iDest_Height - 1 - (SourceRect.bottom - SourceRect.top);
		}


	if (pCA->pDD_Display->iBPP == 32)
	{
		pDW_Dest = (DWORD*)LockDescDest.lpSurface;
		pDW_Source = (DWORD*)LockDescSource.lpSurface;
		
		pDW_SourcePoint = pDW_Source + (SourceRect.top * (iSource_Pitch / 4)) + SourceRect.left;
		pDW_DestPoint = pDW_Dest + (iDestY * (iDest_Pitch / 4)) + iDestX;

		//source FOR both....because in this case iDest_Width is the width of the ENTIRE destination surface
		int iDestCarriageReturn = (iDest_Pitch / 4) - (SourceRect.right - SourceRect.left);
		int iSourceCarriageReturn = (iSource_Pitch / 4) - (SourceRect.right - SourceRect.left);

		DWORD CRef = (dwRefColor & AM) >> 1; 
		DWORD dwMarkColor = pCA->pColorTable->Std_ReplaceMarkerColor;

		for (lcy = 0; lcy < SourceRect.bottom - SourceRect.top; lcy++)
			{
			for (lcx = 0; lcx < SourceRect.right - SourceRect.left; lcx++)
			{
				if (*pDW_SourcePoint != dwColorKey)
				{
					if (*pDW_SourcePoint == dwMarkColor)
						*pDW_DestPoint = CRef;
				}

				pDW_DestPoint++;
				pDW_SourcePoint++;
			}
			pDW_DestPoint += iDestCarriageReturn;
			pDW_SourcePoint += iSourceCarriageReturn;
		}
	}
	
	if (pCA->pDD_Display->iBPP == 16)
	{
		pW_Dest = (WORD*)LockDescDest.lpSurface;
		pW_Source = (WORD*)LockDescSource.lpSurface;
		
		pW_SourcePoint = pW_Source + (SourceRect.top * (iSource_Pitch / 2)) + SourceRect.left;
		pW_DestPoint = pW_Dest + (iDestY * (iDest_Pitch / 2)) + iDestX;

		//source FOR both....because in this case iDest_Width is the width of the ENTIRE destination surface
		int iDestCarriageReturn = (iDest_Pitch / 2) - (SourceRect.right - SourceRect.left);
		int iSourceCarriageReturn = (iSource_Pitch / 2) - (SourceRect.right - SourceRect.left);

		DWORD CRef = dwRefColor;
		DWORD dwMarkColor = pCA->pColorTable->Std_ReplaceMarkerColor;

		for (lcy = 0; lcy < SourceRect.bottom - SourceRect.top; lcy++)
			{
			for (lcx = 0; lcx < SourceRect.right - SourceRect.left; lcx++)
			{
				if (*pW_SourcePoint != dwColorKey)
				{
					if (*pW_SourcePoint == dwMarkColor)
						*pW_DestPoint = CRef;
				}

				pW_DestPoint++;
				pW_SourcePoint++;
			}
			pW_DestPoint += iDestCarriageReturn;
			pW_SourcePoint += iSourceCarriageReturn;
		}
	}
	


	pDDS_Source->Unlock(NULL);
	pDDS_Dest->Unlock(NULL);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::ProcessImage_MarkerReplace(int iImageSurf)
{
cClassAccess* pCA = (cClassAccess*)pCAccess;

//DEBUG
Write_Log("Process Marker Replace Started.");
WriteLog_Enter();

	//set up lock desc
	LPDIRECTDRAWSURFACE7 pDDS = pCA->pImages[iImageSurf]->pDDSImage;
	cImageSurface* pIS = pCA->pImages[iImageSurf];


	ZeroMemory(&pIS->DDSDLock, sizeof(DDSURFACEDESC2));
	pIS->DDSDLock.dwSize = sizeof(DDSURFACEDESC2);



	DWORD dwRBitMask = pCA->pDD_Display->wRBitMask;
	DWORD dwGBitMask = pCA->pDD_Display->wGBitMask;
	DWORD dwBBitMask = pCA->pDD_Display->wBBitMask;
	int iRShift = pCA->pDD_Display->bRShift;
	int iGShift = pCA->pDD_Display->bGShift;
	int iBShift = pCA->pDD_Display->bBShift;

	DWORD dwRVal;
	DWORD dwGVal;
	DWORD dwBVal;

	HRESULT hrlock = pDDS->Lock(NULL, &pIS->DDSDLock, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);

//			const char* KText = DXGetErrorString9(hrlock);
//			Write_Log("Marker Replace Lock: ");
//			Write_Log((char*)KText);
//			WriteLog_Enter();


	DWORD dwReplaceColor = pCA->pColorTable->Std_ReplaceMarkerColor;

	double ddReplacePoint;

		if (pCA->pDD_Display->iBPP == 32)
		{
				int iCarriageReturn = (pIS->DDSDLock.lPitch / 4) - (pIS->DDSDLock.dwWidth);
	DWORD* pDWPoint;
			pDWPoint = (DWORD*)pIS->DDSDLock.lpSurface;

			for(int lcy = 0; lcy < pIS->DDSDLock.dwHeight; lcy++)
			{
				for(int lcx = 0; lcx < pIS->DDSDLock.dwWidth; lcx++)
				{
					//for now, mark as replace, if green element is greater than the greatest other element x 1.2
					//mask and shift
					dwRVal = (*pDWPoint & dwRBitMask) >> iRShift;
					dwGVal = (*pDWPoint & dwGBitMask) >> iGShift;
					dwBVal = (*pDWPoint & dwBBitMask) >> iBShift;

					if (dwRVal > dwBVal)
						ddReplacePoint = dwRVal * 1.2;
							else
						ddReplacePoint = dwBVal * 1.2;

					if (dwGVal > ddReplacePoint)
						*pDWPoint = dwReplaceColor;
					
					pDWPoint++;

				}

				pDWPoint += iCarriageReturn;
	
			}
		}
		if (pCA->pDD_Display->iBPP == 16)
		{
	WORD* pWPoint;
			pWPoint = (WORD*)pIS->DDSDLock.lpSurface;
				int iCarriageReturn = (pIS->DDSDLock.lPitch / 2) - (pIS->DDSDLock.dwWidth);

			for(int lcy = 0; lcy < pIS->DDSDLock.dwHeight; lcy++)
			{
				for(int lcx = 0; lcx < pIS->DDSDLock.dwWidth; lcx++)
				{
					//for now, mark as replace, if green element is greater than the greatest other element x 1.2
					//mask and shift
					dwRVal = (*pWPoint & dwRBitMask) >> iRShift;
					dwGVal = (*pWPoint & dwGBitMask) >> iGShift;
					dwBVal = (*pWPoint & dwBBitMask) >> iBShift;

					if (dwRVal > dwBVal)
						ddReplacePoint = dwRVal * 3.1;
							else
						ddReplacePoint = dwBVal * 3.1;

					if (dwGVal > ddReplacePoint && *pWPoint != 65535)
						*pWPoint = dwReplaceColor;
					
					pWPoint++;

				}

				pWPoint += iCarriageReturn;
	
			}
		}

	pDDS->Unlock(NULL);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_Scrollbar(LPDIRECTDRAWSURFACE7 pDrawSurf, void* pIG_Act, void* pIB)
{
//at the end here, also blit the scroll bar if active
//TEST
	cIGrid* pIG_Active = (cIGrid*)pIG_Act;
	cIBox* pIBox = (cIBox*)pIB;

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int iDim = pCA->pImages[ISURF_SCROLLBAR]->DDSDImage.dwWidth;
	RECT SrcRect;
	if (pIG_Active->bScrollbarFlag == TRUE)
	{
		SrcRect.left = 0;
		SrcRect.right = iDim;
		SrcRect.top = iDim * 2;
		SrcRect.bottom = iDim * 3;
		float fCoord =  pIG_Active->iScrollVal - pIG_Active->iScrollMin;
		fCoord /= pIG_Active->iScrollMax - pIG_Active->iScrollMin;
		fCoord *= (pIBox->BoxPos.bottom - pIBox->BoxPos.top - iDim);
		fCoord += pIBox->BoxPos.top;// + iDim;
		pDrawSurf->BltFast(pIBox->BoxPos.right - (iDim), fCoord, pCA->pImages[ISURF_SCROLLBAR]->pDDSImage, &SrcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);

		SrcRect.top = 0;
		SrcRect.bottom = iDim;
		pDrawSurf->BltFast(pIBox->BoxPos.right - (iDim), pIBox->BoxPos.top, pCA->pImages[ISURF_SCROLLBAR]->pDDSImage, &SrcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
		SrcRect.top = iDim;
		SrcRect.bottom = iDim * 2;
		pDrawSurf->BltFast(pIBox->BoxPos.right - (iDim), pIBox->BoxPos.bottom - iDim, pCA->pImages[ISURF_SCROLLBAR]->pDDSImage, &SrcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_IfcElems_C(void* pElem_, int iXOffset, int iYOffset)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	if (pElem_ == NULL)
		return;

	cIfcElement* pElemTrav = (cIfcElement*)pElem_;

	int iX = pCA->pInputState->IGState_Mouse.iXPos;
	int iY = pCA->pInputState->IGState_Mouse.iYPos;




			//default is "default"
			int iCategory = IFC_ELEM_CAT_DEFAULT_DRAW;
		
		if (iX > pElemTrav->Rect_Pos.left &&
			iX < pElemTrav->Rect_Pos.right &&
			iY > pElemTrav->Rect_Pos.top &&
			iY < pElemTrav->Rect_Pos.bottom)
		{
			iCategory = IFC_ELEM_CAT_ONHOVER_DRAW;

			//if buttons are engaged
			if (pCA->pInputState->IGState_Mouse.Buttons[0] == 1)
			{
				iCategory = IFC_ELEM_CAT_ONLON_DRAW;
			}
			if (pCA->pInputState->IGState_Mouse.Buttons[0] == 2)
			{
				iCategory = IFC_ELEM_CAT_ONLDOWN_DRAW;
			}
			if (pCA->pInputState->IGState_Mouse.Buttons[0] == 3)
			{
				iCategory = IFC_ELEM_CAT_ONLUP_DRAW;
			}
		}
		
		PCMD2* pCmdTrav = pElemTrav->pCmds[iCategory];

		//check if commands are present(if not, default to DEFAULT)
		if (pCmdTrav == NULL)
		{
			pCmdTrav = pElemTrav->pCmds[IFC_ELEM_CAT_DEFAULT_DRAW];
		}

		//execute commands
		while (pCmdTrav != NULL)
		{
			//I realize this will effectively send control to cCommQueue, which
			//will simply go to GDisplay to draw things...
			//this arises because I am trying to process both drawing and actual command
			//orders under the same system (cCommQueue) 06.11.2005
			//fix this(sort of) by placing a special execute_command function inside GDisplay itself
			Execute_Command2_Draw(pCmdTrav, iXOffset, iYOffset);

			pCmdTrav = pCmdTrav->pNext;
		}

		//execute own element's children
		Disp_IfcElems_B(pElemTrav->pChildren, iXOffset, iYOffset);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_IfcElems_B(void* pHead_, int iXOffset, int iYOffset)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	if (pHead_ == NULL)
		return;

	cIfcElement* pHead = (cIfcElement*)pHead_;

	cIfcElement* pElemTrav = pHead;

	int iX = pCA->pInputState->IGState_Mouse.iXPos;
	int iY = pCA->pInputState->IGState_Mouse.iYPos;

	while (pElemTrav != NULL)
	{
		Disp_IfcElems_C(pElemTrav, iXOffset, iYOffset);
			
		//go to next element
		pElemTrav = pElemTrav->pNext;

	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Disp_IfcElems(void* _pIGrid, int iXOffset, int iYOffset)
{
	cIGrid* pIGrid = (cIGrid*)_pIGrid;
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	cIfcElement* pElemTrav = pIGrid->pIfcElem_First->pNext;

	int iX = pCA->pInputState->IGState_Mouse.iXPos;
	int iY = pCA->pInputState->IGState_Mouse.iYPos;

	while (pElemTrav != pIGrid->pIfcElem_Last)
	{
			//default is "default"
			int iCategory = IFC_ELEM_CAT_DEFAULT_DRAW;
		
		if (iX > pElemTrav->Rect_Pos.left &&
			iX < pElemTrav->Rect_Pos.right &&
			iY > pElemTrav->Rect_Pos.top &&
			iY < pElemTrav->Rect_Pos.bottom)
		{
			iCategory = IFC_ELEM_CAT_ONHOVER_DRAW;

			//if buttons are engaged
			if (pCA->pInputState->IGState_Mouse.Buttons[0] == 1)
			{
				iCategory = IFC_ELEM_CAT_ONLON_DRAW;
			}
			if (pCA->pInputState->IGState_Mouse.Buttons[0] == 2)
			{
				iCategory = IFC_ELEM_CAT_ONLDOWN_DRAW;
			}
			if (pCA->pInputState->IGState_Mouse.Buttons[0] == 3)
			{
				iCategory = IFC_ELEM_CAT_ONLUP_DRAW;
			}
		}
		
		PCMD2* pCmdTrav = pElemTrav->pCmds[iCategory];

		//check if commands are present(if not, default to DEFAULT)
		if (pCmdTrav == NULL)
		{
			pCmdTrav = pElemTrav->pCmds[IFC_ELEM_CAT_DEFAULT_DRAW];
		}

		//execute commands
		while (pCmdTrav != NULL)
		{
			//I realize this will effectively send control to cCommQueue, which
			//will simply go to GDisplay to draw things...
			//this arises because I am trying to process both drawing and actual command
			//orders under the same system (cCommQueue) 06.11.2005
			//fix this(sort of) by placing a special execute_command function inside GDisplay itself
			Execute_Command2_Draw(pCmdTrav, iXOffset, iYOffset);

			pCmdTrav = pCmdTrav->pNext;
		}

		//call for child elements
		Disp_IfcElems_B((void*)pElemTrav->pChildren, iXOffset, iYOffset);
			
		//go to next element
		pElemTrav = pElemTrav->pNext;

	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Execute_Command2_Draw(PCMD2* pQ2Tr, int iXOffset, int iYOffset)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	LPDIRECTDRAWSURFACE7 RenderTargetArray[10];

	RenderTargetArray[RTC_PRIMARY] = pCA->pDD_Display->pPrimaryRenderTarget;
	RenderTargetArray[RTC_SECONDARY] = pCA->pDD_Display->pSecondaryRenderTarget;
	RenderTargetArray[RTC_BACK] = pCA->pDD_Display->pDDSBack;



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
					return; //no entries in list, which is fine
				//increment

				//HACK to pull the list primary offset back into bounds
				while ( ((pListBoxInfo->iPrimaryOffset + pListBoxInfo->iListWindowSize) > pListBoxInfo->pListBox->iNumChildren) && pListBoxInfo->iPrimaryOffset != 0)
					pListBoxInfo->iPrimaryOffset--;


				if ( (dwOffset + pListBoxInfo->iPrimaryOffset) >= pListBoxInfo->pListBox->iNumChildren)
				{
					return;
				}



				for (int lc1 = 0; lc1 < (dwOffset + pListBoxInfo->iPrimaryOffset); lc1++)
				{

					if (pTrav != NULL)
					{

					 pTrav = pTrav->pNext;				

					}
				}

				if (pTrav == NULL)
					return;
				//then execute the head of the specified list
				PCMD2* pCmd = pTrav->pCmds[dwElemCat];
				  if (pCmd == NULL)
				  {
						pCmd = pTrav->pCmds[IFC_ELEM_CAT_DEFAULT_DRAW];
				  }
	
				while (pCmd != NULL)
				{
					//CAREFUL! recursive call
					//execute
					Execute_Command2_Draw(pCmd, pListing->Rect_Pos.left, pListing->Rect_Pos.top);
					//Disp_IfcElems_C(pIfcElem, dwX, dwY);
					pCmd = pCmd->pNext;
				}
			}

		}
	if (pQ2Tr->wType == CQ2TYPE_LISTBOX_DISP_SLIDER)
	{
			//get parent element
			cIfcElement** ppIfcETrav = (cIfcElement**)pQ2Tr->pData;
			cIfcElement* pParent = *ppIfcETrav;
			ppIfcETrav++;
			DWORD* pdwTrav = (DWORD*)ppIfcETrav;
			int iRenderTargetCode = *pdwTrav;
			pdwTrav++;
			int iDispCode = *pdwTrav;

		if (pParent->pListBoxInfo != NULL)
			{
				//grab the list traverser
				cIfcListBoxInfo* pListBoxInfo = (cIfcListBoxInfo*)pParent->pListBoxInfo;
				//get the number of children
				int iChildren = pListBoxInfo->pListBox->iNumChildren;

				//default
				LPDIRECTDRAWSURFACE7 pSurface = pCA->pDD_Display->pPrimaryRenderTarget;		

				if (iRenderTargetCode >= 0)
					pSurface = RenderTargetArray[iRenderTargetCode];
	
				int iRangeTop = pParent->Rect_Pos.top + 16;
				int iRangeBottom = pParent->Rect_Pos.bottom - 32;
				float fperc = float(pListBoxInfo->iPrimaryOffset) / float(iChildren - pListBoxInfo->iListWindowSize);
				int iX = pParent->Rect_Pos.right - 16;
				int iY = iRangeTop + fperc * (iRangeBottom - iRangeTop);

				RECT SrcRect;
				SrcRect.left = 0;
				SrcRect.right = 16;
				SrcRect.top = 32;
				SrcRect.bottom = 48;

				if (iDispCode == 1)
				{
						SrcRect.top += 48;
						SrcRect.bottom += 48;
				}

				if (iChildren > pListBoxInfo->iListWindowSize)
				{
					//execute
					pSurface->BltFast(iX + iXOffset, iY + iYOffset, pCA->pImages[ISURF_SCROLLBAR]->pDDSImage, &SrcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
				}
			} 
	}
		if (pQ2Tr->wType == CQ2TYPE_COLORFILL)
		{
			int RenderTargetCode = RTC_PRIMARY;
			DWORD dwFillColor = 0;
			RECT DestRect;

			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			RenderTargetCode = *pdwTrav;
			
			pdwTrav++;
			dwFillColor = *pdwTrav;
			
			pdwTrav++;
			DestRect.left = *pdwTrav + iXOffset;
			pdwTrav++;
			DestRect.right = *pdwTrav + iXOffset;
			pdwTrav++;
			DestRect.top = *pdwTrav + iYOffset;
			pdwTrav++;
			DestRect.bottom = *pdwTrav + iYOffset;

			ddblitfx.dwFillColor = dwFillColor;
			ddblitfx.dwSize = sizeof(DDBLTFX);

			pCA->pDD_Display->Colorfill3(RenderTargetArray[RenderTargetCode], dwFillColor, DestRect.top, DestRect.bottom, DestRect.left, DestRect.right);

		}
		else if (pQ2Tr->wType == CQ2TYPE_DRAWTEXT)
		{
			int iRenderTargetCode = RTC_PRIMARY;
			DWORD dwNumCharacters = 0;
			DWORD dwFontType = 0;
			DWORD dwFColor = 0;
			DWORD dwX = 0;
			DWORD dwY = 0;
			DWORD dwXLimit = 0;
			
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			iRenderTargetCode = *pdwTrav;
			
			pdwTrav++;
			dwX = *pdwTrav;
			pdwTrav++;
			dwY = *pdwTrav;
			pdwTrav++;
			dwXLimit = *pdwTrav;

			pdwTrav++;
			dwFontType = *pdwTrav;
			pdwTrav++;
			dwFColor = *pdwTrav;
			pdwTrav++;
			dwNumCharacters = *pdwTrav;
			pdwTrav++;

			char* Text = new char[dwNumCharacters+1];
			unsigned short* pusTrav = (unsigned short*)pdwTrav;

			for (int lc1 = 0; lc1 < dwNumCharacters; lc1++)
			{
				Text[lc1] = pusTrav[lc1];
			}
			//0 terminate
			Text[dwNumCharacters] = 0;
			//set color
			DWORD dwOldColor = dwFontColor; //the reason why we couldn't use "dwFontColor" before was because that is already a member of the GDisplay class
			pCA->pFontInfoTable->SetFontColor(dwFColor);

			Draw_Text(RenderTargetArray[iRenderTargetCode], Text, dwX + iXOffset, dwY + iYOffset, dwFontType, dwXLimit);
			//set font color back
			pCA->pFontInfoTable->SetFontColor(dwOldColor);
			//free memory
			delete [] Text;

		}
		else if (pQ2Tr->wType == CQ2TYPE_DRAWTEXT2)
		{

			int iRenderTargetCode = RTC_PRIMARY;
			DWORD dwNumCharacters = 0;
			DWORD dwFontType = 0;
			DWORD dwFColor = 0;
			DWORD dwX = 0;
			DWORD dwY = 0;
			DWORD dwXLimit = 0;
			DWORD UnicodeFlag = 0;

			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			
			UnicodeFlag = *pdwTrav; pdwTrav++;

			
			iRenderTargetCode = *pdwTrav;
			

			pdwTrav++;
			dwX = *pdwTrav;
			pdwTrav++;
			dwY = *pdwTrav;
			pdwTrav++;
			dwXLimit = *pdwTrav;

			pdwTrav++;
			dwFontType = *pdwTrav;
			pdwTrav++;
			dwFColor = *pdwTrav;
			pdwTrav++;
			char** ppText = (char**)pdwTrav;
			char* Text = *ppText;
			
			//set color
			DWORD dwOldColor = dwFontColor; //the reason why we couldn't use "dwFontColor" before was because that is already a member of the GDisplay class
			pCA->pFontInfoTable->SetFontColor(dwFColor);

			if (UnicodeFlag == 0)
				Draw_Text(RenderTargetArray[iRenderTargetCode], Text, dwX + iXOffset, dwY + iYOffset, dwFontType, dwXLimit);
			if (UnicodeFlag == 1)
				Draw_Text_Wide(RenderTargetArray[iRenderTargetCode], (unsigned short*)Text, dwX + iXOffset, dwY + iYOffset, dwFontType);
			//set font color back
			pCA->pFontInfoTable->SetFontColor(dwOldColor);
		}
		else if (pQ2Tr->wType == CQ2TYPE_DRAWTEXT4)
		{

			cCQ2TYPE_DRAWTEXT4_DATA* pData = (cCQ2TYPE_DRAWTEXT4_DATA*)pQ2Tr->pData;

			//set color
			DWORD dwOldColor = dwFontColor; //the reason why we couldn't use "dwFontColor" before was because that is already a member of the GDisplay class
			pCA->pFontInfoTable->SetFontColor(pData->dwColor);

			if (pData->bUnicodeFlag == FALSE)
				Draw_Text(RenderTargetArray[pData->dwRenderTargetCode], (char*)pData->Text, pData->dwX + iXOffset, pData->dwY + iYOffset, pData->dwFont, pData->dwXLimit);
			if (pData->bUnicodeFlag == TRUE)
				Draw_Text_Wide(RenderTargetArray[pData->dwRenderTargetCode], (unsigned short*)pData->Text, pData->dwX + iXOffset, pData->dwY + iYOffset, pData->dwFont);
			//set font color back
			pCA->pFontInfoTable->SetFontColor(dwOldColor);

		}
		else if (pQ2Tr->wType == CQ2TYPE_DRAWTEXT3)
		{
			int iRenderTargetCode = RTC_PRIMARY;
			DWORD dwNumCharacters = 0;
			DWORD dwFontType = 0;
			DWORD dwFColor = 0;
			DWORD dwX = 0;
			DWORD dwY = 0;
			DWORD dwXLimit = 0;
			DWORD UnicodeFlag = 0;


			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			
			UnicodeFlag = *pdwTrav; pdwTrav++;

			iRenderTargetCode = *pdwTrav;
			
			pdwTrav++;
			dwX = *pdwTrav;
			pdwTrav++;
			dwY = *pdwTrav;
			pdwTrav++;
			dwXLimit = *pdwTrav;

			pdwTrav++;
			dwFontType = *pdwTrav;
			pdwTrav++;
			dwFColor = *pdwTrav;
			pdwTrav++;
			//yes...that's no mistake, three levels of indirection... 06.14.2005
			char*** pppText = (char***)pdwTrav;
			char** ppText = *pppText;
			char* Text = *ppText;
			
			//set color
			DWORD dwOldColor = dwFontColor; //the reason why we couldn't use "dwFontColor" before was because that is already a member of the GDisplay class
			pCA->pFontInfoTable->SetFontColor(dwFColor);

			if (UnicodeFlag == 0)
				Draw_Text(RenderTargetArray[iRenderTargetCode], Text, dwX + iXOffset, dwY + iYOffset, dwFontType, dwXLimit);
			if (UnicodeFlag == 1)
				Draw_Text_Wide(RenderTargetArray[iRenderTargetCode], (unsigned short*)Text, dwX + iXOffset, dwY + iYOffset, dwFontType);
			//set font color back
			pCA->pFontInfoTable->SetFontColor(dwOldColor);
		}
		else if (pQ2Tr->wType == CQ2TYPE_LISTBOX_CONDITIONAL_BLIT)
		{

			int iSurf = 0;
			int iX = 0;
			int iY = 0;
			RECT SrcRect;
			int iRenderTargetCode = 0;
			cIfcElement** ppIfcETrav = (cIfcElement**)pQ2Tr->pData;
			cIfcElement* pElem = *ppIfcETrav;
			ppIfcETrav++;

			cIfcListBoxInfo* pListBoxInfo = (cIfcListBoxInfo*)pElem->pListBoxInfo;
			cIfcElement* pListData = pListBoxInfo->pListBox;
		
			if (pListBoxInfo->iListWindowSize < pListData->iNumChildren)
			{
				DWORD* pdwTrav = (DWORD*)ppIfcETrav;
				//extract
				iRenderTargetCode = *pdwTrav; pdwTrav++;
				iSurf = *pdwTrav; pdwTrav++;
				iX = *pdwTrav; pdwTrav++;
				iY = *pdwTrav; pdwTrav++;
				SrcRect.left = *pdwTrav; pdwTrav++;
				SrcRect.right = *pdwTrav; pdwTrav++;
					SrcRect.top = *pdwTrav; pdwTrav++;
				SrcRect.bottom = *pdwTrav; 
	
				//default
				LPDIRECTDRAWSURFACE7 pSurface = pCA->pDD_Display->pPrimaryRenderTarget;

				if (iRenderTargetCode >= 0)
					pSurface = RenderTargetArray[iRenderTargetCode];
			//execute

					pSurface->BltFast(iX + iXOffset, iY + iYOffset, pCA->pImages[iSurf]->pDDSImage, &SrcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);

			}
		}
		else if (pQ2Tr->wType == CQ2TYPE_BLIT)
		{
			int iSurf = 0;
			int iX = 0;
			int iY = 0;
			RECT SrcRect;
			int iRenderTargetCode = 0;
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			//extract
			iRenderTargetCode = *pdwTrav; pdwTrav++;
			iSurf = *pdwTrav; pdwTrav++;
			iX = *pdwTrav; pdwTrav++;
			iY = *pdwTrav; pdwTrav++;
			SrcRect.left = *pdwTrav; pdwTrav++;
			SrcRect.right = *pdwTrav; pdwTrav++;
			SrcRect.top = *pdwTrav; pdwTrav++;
			SrcRect.bottom = *pdwTrav; 
	
			//default
			LPDIRECTDRAWSURFACE7 pSurface = pCA->pDD_Display->pPrimaryRenderTarget;

			if (iRenderTargetCode >= 0)

				pSurface = RenderTargetArray[iRenderTargetCode];
			//execute
			pSurface->BltFast(iX + iXOffset, iY + iYOffset, pCA->pImages[iSurf]->pDDSImage, &SrcRect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);

		}
		else if (pQ2Tr->wType == CQ2TYPE_BLITBYSURFACE)
		{
			int iSurf = 0;
			int iX = 0;
			int iY = 0;
			RECT SrcRect;
			int iRenderTargetCode = 0;
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			//extract
			iRenderTargetCode = *pdwTrav; pdwTrav++;
			LPDIRECTDRAWSURFACE7* ppDDS = (LPDIRECTDRAWSURFACE7*)pdwTrav;
			LPDIRECTDRAWSURFACE7 pDDS = *ppDDS; ppDDS++;
			pdwTrav = (DWORD*)ppDDS;
			iX = *pdwTrav; pdwTrav++;
			iY = *pdwTrav; pdwTrav++;
			SrcRect.left = *pdwTrav; pdwTrav++;
			SrcRect.right = *pdwTrav; pdwTrav++;
			SrcRect.top = *pdwTrav; pdwTrav++;
			SrcRect.bottom = *pdwTrav; 
	
			//default
			LPDIRECTDRAWSURFACE7 pSurface = pCA->pDD_Display->pPrimaryRenderTarget;

			if (iRenderTargetCode >= 0)
				pSurface = RenderTargetArray[iRenderTargetCode];
			//execute
			HRESULT hr55 = pSurface->BltFast(iX + iXOffset, iY + iYOffset, pDDS, &SrcRect, DDBLTFAST_WAIT);

			char NText[50];
			itoa(hr55, NText, 10);
			Write_Log("Blit BY Surface Error: ");
			Write_Log(NText);
			WriteLog_Enter();
			itoa(iRenderTargetCode, NText, 10);
			Write_Log("Render Target Code: ");
			Write_Log(NText);
			WriteLog_Enter();
			itoa(iX, NText, 10);
			Write_Log("iX: ");
			Write_Log(NText);
			itoa(iY, NText, 10);
			Write_Log(" iY: ");
			Write_Log(NText);
			WriteLog_Enter();

			itoa(SrcRect.left, NText, 10);
			Write_Log("iSrcLeft: ");
			Write_Log(NText);
			itoa(SrcRect.right, NText, 10);
			Write_Log(" iSrcRight: ");
			Write_Log(NText);
			itoa(SrcRect.top, NText, 10);
			Write_Log(" iSrcTop: ");
			Write_Log(NText);
			itoa(SrcRect.bottom, NText, 10);
			Write_Log(" iSrcBottom: ");
			Write_Log(NText);
		}
		else if (pQ2Tr->wType == CQ2TYPE_BLITFIT)
		{
			int iSurf = 0;
			RECT DestRect;
			RECT SrcRect;
			int iRenderTargetCode = 0;
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			//extract
			iRenderTargetCode = *pdwTrav; pdwTrav++;
			iSurf = *pdwTrav; pdwTrav++;
			DestRect.left = *pdwTrav; pdwTrav++;
			DestRect.right = *pdwTrav; pdwTrav++;
			DestRect.top = *pdwTrav; pdwTrav++;
			DestRect.bottom = *pdwTrav; pdwTrav++;
			SrcRect.left = *pdwTrav; pdwTrav++;
			SrcRect.right = *pdwTrav; pdwTrav++;
			SrcRect.top = *pdwTrav; pdwTrav++;
			SrcRect.bottom = *pdwTrav; 
	
			//add offset
			DestRect.left += iXOffset;
			DestRect.right = iXOffset;
			DestRect.top += iYOffset;
			DestRect.bottom += iYOffset;
			//default
			LPDIRECTDRAWSURFACE7 pSurface = pCA->pDD_Display->pPrimaryRenderTarget;

			if (iRenderTargetCode >= 0)
				pSurface = RenderTargetArray[iRenderTargetCode];
			//execute
			ddblitfx.dwSize = sizeof(DDBLTFX);
			pSurface->Blt(&DestRect, pCA->pImages[iSurf]->pDDSImage, &SrcRect, DDBLT_KEYSRC, &ddblitfx);
			
		}
		else if (pQ2Tr->wType == CQ2TYPE_DRAWIFCPANEL)
		{
			int iSurf = 0;
			RECT DestRect;
			int iRenderTargetCode = 0;
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			//extract
			iRenderTargetCode = *pdwTrav; pdwTrav++;
			DestRect.left = *pdwTrav; pdwTrav++;
			DestRect.right = *pdwTrav; pdwTrav++;
			DestRect.top = *pdwTrav; pdwTrav++;
			DestRect.bottom = *pdwTrav; 

			//increase by offsets
			DestRect.left += iXOffset;
			DestRect.right += iXOffset;
			DestRect.top += iYOffset;
			DestRect.bottom += iYOffset;

			Draw_Interface_Panel(RenderTargetArray[iRenderTargetCode], DestRect);
		}
		else if (pQ2Tr->wType == CQ2TYPE_DRAWIFCSECONDARYPANEL)
		{
			int iSurf = 0;
			RECT DestRect;
			int iRenderTargetCode = 0;
			int iColorIndex = 0;
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			
			//extract
			iRenderTargetCode = *pdwTrav; pdwTrav++;
			DestRect.left = *pdwTrav; pdwTrav++;
			DestRect.right = *pdwTrav; pdwTrav++;
			DestRect.top = *pdwTrav; pdwTrav++;
			DestRect.bottom = *pdwTrav; pdwTrav++;
			iColorIndex = *pdwTrav;

			//increase by offsets
			DestRect.left += iXOffset;
			DestRect.right += iXOffset;
			DestRect.top += iYOffset;
			DestRect.bottom += iYOffset;

			Draw_Interface_Secondary_Panel(RenderTargetArray[iRenderTargetCode], DestRect, iColorIndex);
		}
		else if (pQ2Tr->wType == CQ2TYPE_DRAWIFCLISTBOX)
		{
			int iSurf = 0;
			RECT DestRect;
			int iRenderTargetCode = 0;
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;
			//extract
			iRenderTargetCode = *pdwTrav; pdwTrav++;
			DestRect.left = *pdwTrav; pdwTrav++;
			DestRect.right = *pdwTrav; pdwTrav++;
			DestRect.top = *pdwTrav; pdwTrav++;
			DestRect.bottom = *pdwTrav; 

			//increase by offsets
			DestRect.left += iXOffset;
			DestRect.right += iXOffset;
			DestRect.top += iYOffset;
			DestRect.bottom += iYOffset;

			Draw_Interface_ListBox(RenderTargetArray[iRenderTargetCode], DestRect);
		}
		else if (pQ2Tr->wType == CQ2TYPE_DISPCHATTEXT)
		{
			DWORD* pdwTrav = (DWORD*)pQ2Tr->pData;

			int iRenderTargetCode = *pdwTrav; pdwTrav++;
			int iX = *pdwTrav; pdwTrav++;
			int iY = *pdwTrav; pdwTrav++;
			int iLines = *pdwTrav;

			

			Disp_ChatText(RenderTargetArray[iRenderTargetCode], iX + iXOffset, iY + iYOffset, iLines);
		}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Draw_TextCursor(LPDIRECTDRAWSURFACE7 pDDS, int iFontType, int iX, int iY)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	
	SYSTEMTIME sysTime;

	GetSystemTime(&sysTime);

	if (sysTime.wMilliseconds > 500)
	{
		int iWidth = pCA->pFontInfoTable->Fonts[iFontType].iWidth;
		int iHeight = pCA->pFontInfoTable->Fonts[iFontType].iHeight;

		pCA->pDD_Display->Colorfill3(pDDS, pCA->pColorTable->White - 1, iY, iY + iHeight, iX, iX + iWidth);
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Draw_Interface_Special_Square(LPDIRECTDRAWSURFACE7 pDDS, RECT RectDest, int iPanelFlag)
{
int xoffset = 0;
int yoffset = 0;
RECT RectSrc[NUM_IFCRECTS];


	cClassAccess* pCA = (cClassAccess*)pCAccess;
	LPDIRECTDRAWSURFACE7 pSrc = NULL;

	RECT* SPRA = NULL;

	if (iPanelFlag == 0)
	{
		SPRA = RectIfcListBoxes;
		pSrc = pCA->pImages[ISURF_MENUBOXES01]->pDDSImage;

	}
	if (iPanelFlag == 1)
	{
		SPRA = RectIfcPanels;
		pSrc = pCA->pImages[ISURF_MAINMENU]->pDDSImage;
	}
	if (iPanelFlag >= 100)
	{
		int iFrame = iPanelFlag - 100;
		
		int iRow = iFrame % BLIT_FPCOL;
		int iColumn = iFrame / BLIT_FPCOL;

		xoffset = 40 /*Hard Coded! Caution */ * iColumn;
		yoffset = 40 * iRow;

		for (int c1 = 0; c1 < NUM_IFCRECTS; c1++)
		{
			RectSrc[c1].bottom = RectIfcSecondaryPanels[c1].bottom + yoffset;
			RectSrc[c1].top = RectIfcSecondaryPanels[c1].top + yoffset;
			RectSrc[c1].left = RectIfcSecondaryPanels[c1].left + xoffset;
			RectSrc[c1].right = RectIfcSecondaryPanels[c1].right + xoffset;

		}

		SPRA = RectSrc;
		pSrc = pCA->pImages[ISURF_MENU05]->pDDSImage;
	}

	if (SPRA == NULL)
		return;

	int width = RectDest.right - RectDest.left;
	int height = RectDest.bottom - RectDest.top;

	if (width < iMinimumWidth || height < iMinimumHeight)
		return;

	//blit the four corners
	int iX = RectDest.left;
    int iY = RectDest.top;
	pDDS->BltFast(iX, iY, pSrc, &SPRA[IFCRECT_TOPLEFT], DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
	iX = RectDest.right - (SPRA[IFCRECT_TOPRIGHT].right - SPRA[IFCRECT_TOPRIGHT].left);
    iY = RectDest.top;
	pDDS->BltFast(iX, iY, pSrc, &SPRA[IFCRECT_TOPRIGHT],DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
	iX = RectDest.right - (SPRA[IFCRECT_BOTTOMRIGHT].right - SPRA[IFCRECT_BOTTOMRIGHT].left);
	iY = RectDest.bottom - (SPRA[IFCRECT_BOTTOMRIGHT].bottom - SPRA[IFCRECT_BOTTOMRIGHT].top);
	pDDS->BltFast(iX, iY, pSrc, &SPRA[IFCRECT_BOTTOMRIGHT], DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
	iX = RectDest.left;
    iY = RectDest.bottom - (SPRA[IFCRECT_BOTTOMLEFT].bottom - SPRA[IFCRECT_BOTTOMLEFT].top);
	pDDS->BltFast(iX, iY, pSrc, &SPRA[IFCRECT_BOTTOMLEFT], DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
	//blit the sides
	RECT R;
	R.left = RectDest.left;
	//space the sides by the dimensions of the corners
	R.left = RectDest.left;
	R.right = R.left + SPRA[IFCRECT_LEFT].right - SPRA[IFCRECT_LEFT].left;
	R.top = RectDest.top + (SPRA[IFCRECT_TOPLEFT].bottom - SPRA[IFCRECT_TOPLEFT].top);
	R.bottom = RectDest.bottom - (SPRA[IFCRECT_BOTTOMLEFT].bottom - SPRA[IFCRECT_BOTTOMLEFT].top);
	ddblitfx.dwSize = sizeof(DDBLTFX);
	pDDS->Blt(&R, pSrc, &SPRA[IFCRECT_LEFT], DDBLT_KEYSRC, &ddblitfx);
	R.left = RectDest.left + (SPRA[IFCRECT_TOPLEFT].right - SPRA[IFCRECT_TOPLEFT].left);
	R.right = RectDest.right - (SPRA[IFCRECT_TOPRIGHT].right - SPRA[IFCRECT_TOPRIGHT].left);
	R.top = RectDest.top;
	R.bottom = RectDest.top + (SPRA[IFCRECT_TOP].bottom - SPRA[IFCRECT_TOP].top);
	ddblitfx.dwSize = sizeof(DDBLTFX);
	pDDS->Blt(&R, pSrc, &SPRA[IFCRECT_TOP], DDBLT_KEYSRC, &ddblitfx);
	R.left = RectDest.right - (SPRA[IFCRECT_RIGHT].right - SPRA[IFCRECT_RIGHT].left);
	R.right = RectDest.right;
	R.top = RectDest.top + (SPRA[IFCRECT_TOPRIGHT].bottom - SPRA[IFCRECT_TOPRIGHT].top);
	R.bottom = RectDest.bottom - (SPRA[IFCRECT_BOTTOMRIGHT].bottom - SPRA[IFCRECT_BOTTOMRIGHT].top);
	ddblitfx.dwSize = sizeof(DDBLTFX);
	pDDS->Blt(&R, pSrc, &SPRA[IFCRECT_RIGHT], DDBLT_KEYSRC, &ddblitfx);
	R.left = RectDest.left + (SPRA[IFCRECT_BOTTOMLEFT].right - SPRA[IFCRECT_BOTTOMLEFT].left);
	R.right = RectDest.right - (SPRA[IFCRECT_BOTTOMRIGHT].right - SPRA[IFCRECT_BOTTOMRIGHT].left);
	R.top = RectDest.bottom - (SPRA[IFCRECT_BOTTOM].bottom - SPRA[IFCRECT_BOTTOM].top);
	R.bottom = RectDest.bottom;
	ddblitfx.dwSize = sizeof(DDBLTFX);
	pDDS->Blt(&R, pSrc, &SPRA[IFCRECT_BOTTOM], DDBLT_KEYSRC, &ddblitfx);

	//blit center (space the center off the sides)
	R.left = RectDest.left + (SPRA[IFCRECT_LEFT].right - SPRA[IFCRECT_LEFT].left);
	R.right = RectDest.right - (SPRA[IFCRECT_RIGHT].right - SPRA[IFCRECT_RIGHT].left);
	R.top = RectDest.top + (SPRA[IFCRECT_TOP].bottom - SPRA[IFCRECT_TOP].top);
	R.bottom = RectDest.bottom - (SPRA[IFCRECT_BOTTOM].bottom - SPRA[IFCRECT_BOTTOM].top);
	ddblitfx.dwSize = sizeof(DDBLTFX);
	pDDS->Blt(&R, pSrc, &SPRA[IFCRECT_CENTER], DDBLT_KEYSRC, &ddblitfx);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Draw_Interface_Secondary_Panel(LPDIRECTDRAWSURFACE7 pDDS, RECT RectDest, int iColorIndex)
{

	Draw_Interface_Special_Square(pDDS, RectDest, 100 + iColorIndex);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Draw_Interface_ListBox(LPDIRECTDRAWSURFACE7 pDDS, RECT RectDest)
{
	
	Draw_Interface_Special_Square(pDDS, RectDest, 0);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Tint_MapControl(LPDIRECTDRAWSURFACE7 pDrawSurf)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	int icheck = 1 << pCA->pGameOptions->iControl;

	for (int lc4 = 0; lc4 <= MDRAW_NUMCOLS; lc4++)
	for (int lc5 = 0; lc5 <= MDRAW_NUMROWS; lc5++)
	{
		int iX = (lc4 + pCA->pInterface->map_xview);
		int iY = (lc5 + pCA->pInterface->map_yview);
		int index =  iX + iY*pCA->pGameWorld->iWidth;
		MTILE* pTile = &pCA->pGameWorld->pMap[index];

				//pDrawSurf->BltFast(lc4*TILE_SIZE, lc5*TILE_SIZE, pCA->pImages[ISURF_SELECT]->pDDSImage, &SRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
				RECT d1;
				d1.left = lc4*TILE_SIZE;
				d1.top = lc5*TILE_SIZE;
				d1.bottom = d1.top + TILE_SIZE;
				d1.right = d1.left + TILE_SIZE;
				if (pTile->iControl >= 0)
				if ( (pTile->barrRevealed & icheck) != 0)
				{
					if (pCA->pDD_Display->iDDRenderMode != DDRM_NORMAL)
						AlphaTintRect(pDrawSurf, d1, pCA->pColorTable->MM_Players[pTile->iControl]);
					else
						AlphaTintRectF(pDrawSurf, d1, pCA->pColorTable->MM_Players[pTile->iControl]);
				}
	}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cGDisplay::Draw_Interface_Panel(LPDIRECTDRAWSURFACE7 pDDS, RECT RectDest)
{
	Draw_Interface_Special_Square(pDDS, RectDest, 1);


/*
	int width = RectDest.right - RectDest.left;
	int height = RectDest.bottom - RectDest.top;

	if (width < iMinimumWidth || height < iMinimumHeight)
		return;

	//blit the four corners
	int iX = RectDest.left;
    int iY = RectDest.top;
	pDDS->BltFast(iX, iY, pCA->pImages[ISURF_MAINMENU]->pDDSImage, &RectPanelTopLeft, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	iX = RectDest.right - (RectPanelTopRight.right - RectPanelTopRight.left);
    iY = RectDest.top;
	pDDS->BltFast(iX, iY, pCA->pImages[ISURF_MAINMENU]->pDDSImage, &RectPanelTopRight, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	iX = RectDest.right - (RectPanelBottomRight.right - RectPanelBottomRight.left);
    iY = RectDest.bottom - (RectPanelBottomRight.bottom - RectPanelBottomRight.top);
	pDDS->BltFast(iX, iY, pCA->pImages[ISURF_MAINMENU]->pDDSImage, &RectPanelBottomRight, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	iX = RectDest.left;
    iY = RectDest.bottom - (RectPanelBottomLeft.bottom - RectPanelBottomLeft.top);
	pDDS->BltFast(iX, iY, pCA->pImages[ISURF_MAINMENU]->pDDSImage, &RectPanelBottomLeft, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
	//blit the sides
	RECT R;
	R.left = RectDest.left;
	//space the sides by the dimensions of the corners
	R.left = RectDest.left;
	R.right = R.left + RectPanelLeft.right - RectPanelLeft.left;
	R.top = RectDest.top + (RectPanelTopLeft.bottom - RectPanelTopLeft.top);
	R.bottom = RectDest.bottom - (RectPanelBottomLeft.bottom - RectPanelBottomLeft.top);
	ddblitfx.dwSize = sizeof(DDBLTFX);
	pDDS->Blt(&R, pCA->pImages[ISURF_MAINMENU]->pDDSImage, &RectPanelLeft, 0, &ddblitfx);
	R.left = RectDest.left + (RectPanelTopLeft.right - RectPanelTopLeft.left);
	R.right = RectDest.right - (RectPanelTopRight.right - RectPanelTopRight.left);
	R.top = RectDest.top;
	R.bottom = RectDest.top + (RectPanelTop.bottom - RectPanelTop.top);
	ddblitfx.dwSize = sizeof(DDBLTFX);
	pDDS->Blt(&R, pCA->pImages[ISURF_MAINMENU]->pDDSImage, &RectPanelTop, 0, &ddblitfx);
	R.left = RectDest.right - (RectPanelRight.right - RectPanelRight.left);
	R.right = RectDest.right;
	R.top = RectDest.top + (RectPanelTopRight.bottom - RectPanelTopRight.top);
	R.bottom = RectDest.bottom - (RectPanelBottomRight.bottom - RectPanelBottomRight.top);
	ddblitfx.dwSize = sizeof(DDBLTFX);
	pDDS->Blt(&R, pCA->pImages[ISURF_MAINMENU]->pDDSImage, &RectPanelRight, 0, &ddblitfx);
	R.left = RectDest.left + (RectPanelBottomLeft.right - RectPanelBottomLeft.left);
	R.right = RectDest.right - (RectPanelBottomRight.right - RectPanelBottomRight.left);
	R.top = RectDest.bottom - (RectPanelBottom.bottom - RectPanelBottom.top);
	R.bottom = RectDest.bottom;
	ddblitfx.dwSize = sizeof(DDBLTFX);
	pDDS->Blt(&R, pCA->pImages[ISURF_MAINMENU]->pDDSImage, &RectPanelBottom, 0, &ddblitfx);

	//blit center (space the center off the sides)
	R.left = RectDest.left + (RectPanelLeft.right - RectPanelLeft.left);
	R.right = RectDest.right - (RectPanelRight.right - RectPanelRight.left);
	R.top = RectDest.top + (RectPanelTop.bottom - RectPanelTop.top);
	R.bottom = RectDest.bottom - (RectPanelBottom.bottom - RectPanelBottom.top);
	ddblitfx.dwSize = sizeof(DDBLTFX);
	pDDS->Blt(&R, pCA->pImages[ISURF_MAINMENU]->pDDSImage, &RectPanelCenter, 0, &ddblitfx);
*/
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
