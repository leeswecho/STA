#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cMinimap::Init(VOID* pCA)
{
	pCAccess = pCA;
	cClassAccess* pCA2 = (cClassAccess*)pCA;
	HRESULT hr21;

//create a surface the same size as the map 
	ZeroMemory(&MapSurface.DDSDImage, sizeof(DDSURFACEDESC2));
    MapSurface.DDSDImage.dwSize = sizeof(DDSURFACEDESC2);
    MapSurface.DDSDImage.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    
	//force offscreenplain because the minimap is ALWAYS display during play
	MapSurface.DDSDImage.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    //change it to sysmem if we're in SYSMEM mode
	if (pCA2->pDD_Display->iDDRenderMode == DDRM_SYSMEM)
		MapSurface.DDSDImage.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY;
		
    MapSurface.DDSDImage.dwWidth = pCA2->pGameWorld->iWidth;
    MapSurface.DDSDImage.dwHeight = pCA2->pGameWorld->iHeight;

	hr21 = pCA2->pDD_Display->pDD7->CreateSurface(&MapSurface.DDSDImage, &MapSurface.pDDSImage, NULL);

	//get surface desc
	MapSurface.pDDSImage->GetSurfaceDesc(&MapSurface.DDSDImage);

	const char* KText;
	//KText = DXGetErrorString9(hr21);
	//pCA2->pGDisplay->Write_Log("Create Surface: ");
	//pCA2->pGDisplay->Write_Log((char*)KText);

	//LOCK surface to access it
	MapSurface.DDSDLock.dwSize = sizeof(DDSURFACEDESC2);
hr21 = MapSurface.pDDSImage->Lock(NULL, &MapSurface.DDSDLock, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);

	//KText = DXGetErrorString9(hr21);
	//pCA2->pGDisplay->Write_Log("LOCK: ");
	//pCA2->pGDisplay->Write_Log((char*)KText);
	
//argh. I will have to do this in three separate loops to account for possible color depth differences

	//8-bit
	if (pCA2->pDD_Display->iBPP == 8)
	{
		BYTE* pbStart = (BYTE*)MapSurface.DDSDLock.lpSurface;
		BYTE* pbPoint = pbStart;
		DWORD dwIndex;
		DWORD MapIndex;
		//read in data from mapfile and blit
		for(DWORD lc5 = 0; lc5 < pCA2->pGameWorld->iHeight; lc5++)
		for(DWORD lc6 = 0; lc6 < pCA2->pGameWorld->iWidth; lc6++)
		{
			dwIndex = (lc5 * MapSurface.DDSDImage.lPitch) + lc6;
			MapIndex = (lc5 * pCA2->pGameWorld->iWidth) + lc6;
			pbPoint = pbStart + dwIndex;
			*pbPoint = BYTE( pCA2->pColorTable->MM_Terrain[pCA2->pGameWorld->pMap[MapIndex].iType] );
//			*pbPoint = 0;
		}
	}
	//16-bit
	if (pCA2->pDD_Display->iBPP == 16)
	{
		WORD* pbStart = (WORD*)MapSurface.DDSDLock.lpSurface;
		WORD* pbPoint = pbStart;
		DWORD dwIndex;
		DWORD MapIndex;
		//read in data from mapfile and blit
		for(DWORD lc5 = 0; lc5 < pCA2->pGameWorld->iHeight; lc5++)
		for(DWORD lc6 = 0; lc6 < pCA2->pGameWorld->iWidth; lc6++)
		{
			dwIndex = (lc5 * (MapSurface.DDSDImage.lPitch/2)) + lc6;
			MapIndex = (lc5 * pCA2->pGameWorld->iWidth) + lc6;
			pbPoint = pbStart + dwIndex;
			*pbPoint = WORD( pCA2->pColorTable->MM_Terrain[pCA2->pGameWorld->pMap[MapIndex].iType]);
//			*pbPoint = 1984;
//			*pbPoint = 0;

		}
	}
	//32-bit
	if (pCA2->pDD_Display->iBPP == 32)
	{
		DWORD* pbStart = (DWORD*)MapSurface.DDSDLock.lpSurface;
		DWORD* pbPoint = pbStart;
		DWORD dwIndex;
		DWORD MapIndex;
		//read in data from mapfile and blit
		for(DWORD lc5 = 0; lc5 < pCA2->pGameWorld->iHeight; lc5++)
		for(DWORD lc6 = 0; lc6 < pCA2->pGameWorld->iWidth; lc6++)
		{
			dwIndex = (lc5 * (MapSurface.DDSDImage.lPitch/4)) + lc6;
			MapIndex = (lc5 * pCA2->pGameWorld->iWidth) + lc6;
			pbPoint = pbStart + dwIndex;
			*pbPoint = pCA2->pColorTable->MM_Terrain[pCA2->pGameWorld->pMap[MapIndex].iType];
//			*pbPoint = 0;
		}
	}

/*		WORD* pbStart = (WORD*)MapSurface.DDSDLock.lpSurface;
		WORD* pbPoint = pbStart;
		DWORD dwIndex;
		DWORD MapIndex;
	for (int lc8 = 0; lc8 < 100; lc8++)
	for (int lc7 = 0; lc7 < 40; lc7++)
	{
			
			pbPoint = pbStart + (lc8*(MapSurface.DDSDImage.lPitch/2)) + lc7;
			*pbPoint = 0;
	} */

//unlock and get on with it
hr21 = MapSurface.pDDSImage->Unlock(NULL);

//DEBUG
//	KText = DXGetErrorString9(hr21);
//	pCA2->pGDisplay->Write_Log("UNLOCK: ");
//	pCA2->pGDisplay->Write_Log((char*)KText);
//	pCA2->pGDisplay->WriteLog_Enter();

//set the initial mmzoffset values(03.24.2003)
	sqMMZOffset.x = 0;
	sqMMZOffset.y = 0;

	MMZDrawFlag = FALSE;

//disabled for now, due to there not being enough to space to show it! (10.02.2004)
//determine whether the map is large enough to necessitate the "zoom" minimap
//	if (MapSurface.DDSDImage.dwWidth > pCA2->pDD_Display->i_XResD * MMZDRAW_XSCR ||
//		MapSurface.DDSDImage.dwHeight > pCA2->pDD_Display->i_YResD * MMZDRAW_YSCR)
//			MMZDrawFlag = TRUE;

//create the processsurface
//create a surface the same size as the map 
	ZeroMemory(&MapSurface.DDSDImage, sizeof(DDSURFACEDESC2));
    ProcessSurface.DDSDImage.dwSize = sizeof(DDSURFACEDESC2);
    ProcessSurface.DDSDImage.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    
	//force offscreenplain because the minimap is ALWAYS display during play
	ProcessSurface.DDSDImage.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    //change it to sysmem if we're in SYSMEM mode
	if (pCA2->pDD_Display->iDDRenderMode == DDRM_SYSMEM)
		ProcessSurface.DDSDImage.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY;
		
    ProcessSurface.DDSDImage.dwWidth = pCA2->pDD_Display->i_XResD * MMDRAW_XSCR;
    ProcessSurface.DDSDImage.dwHeight = pCA2->pDD_Display->i_YResD * MMDRAW_YSCR;

	hr21 = pCA2->pDD_Display->pDD7->CreateSurface(&ProcessSurface.DDSDImage, &ProcessSurface.pDDSImage, NULL);

	KText;
//	KText = DXGetErrorString9(hr21);
//	pCA2->pGDisplay->Write_Log("Create Process Surface: ");
//	pCA2->pGDisplay->Write_Log((char*)KText);

	//get surface desc
	ProcessSurface.pDDSImage->GetSurfaceDesc(&ProcessSurface.DDSDImage);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cMinimap::Process_Minimap(BOOL bRevealAll)
{

	cClassAccess* pCA = (cClassAccess*)pCAccess;

	RECT MinimapDrawRect = pCA->pGDisplay->MinimapDrawRect;
	RECT SelectRect;
	HRESULT hr20, hr21;

	DDBLTFX DDBlitEffects;
	ZeroMemory(&DDBlitEffects, sizeof(DDBLTFX));
	DDBlitEffects.dwSize = sizeof(DDBLTFX);

	hr20 = ProcessSurface.pDDSImage->Blt(NULL, MapSurface.pDDSImage, NULL, DDBLT_WAIT, &DDBlitEffects);


	//figure out where on the screen we are currently looking at
	int rectheight = MinimapDrawRect.bottom - MinimapDrawRect.top;
	int rectwidth = MinimapDrawRect.right - MinimapDrawRect.left;
	int xbar_low = (float(pCA->pInterface->map_xview) / pCA->pGameWorld->iWidth) * rectwidth + MinimapDrawRect.left;
	int xbar_high = (float(pCA->pInterface->map_xview + pCA->pGDisplay->MDRAW_NUMCOLS) / pCA->pGameWorld->iWidth) * rectwidth + MinimapDrawRect.left;
	int ybar_low = (float(pCA->pInterface->map_yview) / pCA->pGameWorld->iHeight) * rectheight + MinimapDrawRect.top;
	int ybar_high = (float(pCA->pInterface->map_yview + pCA->pGDisplay->MDRAW_NUMROWS) / pCA->pGameWorld->iHeight) * rectheight + MinimapDrawRect.top;


	MTile* pMap = pCA->pGameWorld->pMap;
	int index = pCA->pInterface->PosMinimap_Mouse.index;

	int sindex;
	cColorTable* colortable = pCA->pColorTable;
	cSector** sectors = pCA->pSectorTable->Sectors;
	cStructureA* pCapital;
	int xmap, ymap;
	double xfactor = pCA->pGameWorld->iWidth / float(rectwidth);
	double yfactor = pCA->pGameWorld->iHeight / float(rectheight);
	BOOL illuminatesectorflag = FALSE; //disabled 09.20.2006// TRUE;

		//a hack to not illuminate "ocean" or "undefined sector" HACK (10.03.2004)
	if (sectors[pMap[index].iSector] == NULL)
	{
		illuminatesectorflag = FALSE;
	}	else if (pMap[index].iSector == MAPFILE_SCT_NOSECTOR)
	{
		illuminatesectorflag = FALSE;
	}
	{
		if (sectors[pMap[index].iSector]->Name[0] == 'O')
		if (sectors[pMap[index].iSector]->Name[1] == 'c')
		if (sectors[pMap[index].iSector]->Name[2] == 'e')
		if (sectors[pMap[index].iSector]->Name[3] == 'a')
		if (sectors[pMap[index].iSector]->Name[4] == 'n')
		{
			illuminatesectorflag = FALSE;
		}
		if (sectors[pMap[index].iSector]->Name[0] == 'U')
		if (sectors[pMap[index].iSector]->Name[1] == 'n')
		if (sectors[pMap[index].iSector]->Name[2] == 'd')
		if (sectors[pMap[index].iSector]->Name[3] == 'e')
		if (sectors[pMap[index].iSector]->Name[4] == 'f')
		if (sectors[pMap[index].iSector]->Name[5] == 'i')
		if (sectors[pMap[index].iSector]->Name[6] == 'n')
		if (sectors[pMap[index].iSector]->Name[7] == 'e')
		if (sectors[pMap[index].iSector]->Name[8] == 'd')

		{
			illuminatesectorflag = FALSE;
		}
	}

	int iBPP = pCA->pDD_Display->iBPP;
	int jump = iBPP/8;

	


	//LOCK surface to access it
	ProcessSurface.DDSDLock.dwSize = sizeof(DDSURFACEDESC2);
hr21 = ProcessSurface.pDDSImage->Lock(NULL, &ProcessSurface.DDSDLock, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);

	BYTE* pbSurface = (BYTE*)ProcessSurface.DDSDLock.lpSurface;

	BYTE* pbPoint;
	BYTE* pbHold;
	int pitch = ProcessSurface.DDSDLock.lPitch / jump;

	int icheck = 1;
	icheck = icheck << pCA->pGameOptions->iControl;
	int iControl = pCA->pGameOptions->iControl;
	DWORD black = colortable->Black;
	DWORD white = colortable->White;

	bool blackflag = FALSE;


	DWORD* Colors = pCA->pColorTable->MM_Players;

	//drawing currently-over sector

	for (int cy = 0; cy < rectheight; cy++)
	for (int cx = 0; cx < rectwidth; cx++)
	{
		//calculate rect positions for the particular dot
		//re-using the ybar, xbar variables
		xmap = (cx) * xfactor;
		ymap = (cy) * yfactor;
		sindex = ymap*pCA->pGameWorld->iWidth + xmap;
			//we depend on little-endian data storage here
			pbPoint = pbSurface + ((cy * pitch + cx) * jump);

		blackflag = TRUE;
		if ((pMap[sindex].barrRevealed & icheck) != 0)
			blackflag = FALSE;
		if ((sectors[pMap[sindex].iSector]->iControl) == pCA->pGameOptions->iControl)
			blackflag = FALSE;

		if (bRevealAll == TRUE)
			blackflag = FALSE;

		//blank out minimap
		if ( blackflag == TRUE)
		{
			pbHold = (BYTE*)&black;
			memcpy(pbPoint, pbHold + 4 - jump, jump);
		} 
		else 
		{
			if (cx % 2 == cy % 2)
			if ( pCA->pInputState->IGState_Keyboard[DIK_TAB] > 0)
			if (pMap[sindex].iControl >= 0)
			{
					pbHold = (BYTE*)&Colors[pMap[sindex].iControl];  //note, we stored it as a DWORD (4 bytes)
														 //if we set the color when BPP is only, say 16,
													     //then the color information will be in the lower-order
														//two bits (we're little-endian)
														//that's why we have "4- jump" there
					memcpy(pbPoint, pbHold + 4 - jump, jump);  


			}
			//FOUR ifs have to hold to run this block
			if (pCA->pInterface->mouse_currbox == IBOX_MINIMAP)
			if (cx % 2 == cy % 2)
			{
				

				if ( illuminatesectorflag == TRUE)
				if ( pMap[index].iSector == pMap[sindex].iSector)
				{
					pCapital = sectors[pMap[index].iSector]->pCapital;

					if (pCapital != NULL)
						pbHold = (BYTE*)&colortable->MM_Players[pCapital->iControl]; //DDBlitEffects.dwFillColor = colortable->MM_Players[pCapital->iControl];
							else
						pbHold = (BYTE*)&colortable->White;
						//DDBlitEffects.dwFillColor = colortable->White;


					memcpy(pbPoint, pbHold + 4 - jump, jump);

					//hr20 = ProcessSurface.pDDSImage->Blt(&SelectRect, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &DDBlitEffects);
				}
			}
		}

	}



	//drawing units
	cUnitA* pUTrav = (cUnitA*)pCA->pUnitList->pUL_First->pNext_UL;
	while( pUTrav != pCA->pUnitList->pUL_Last)
	{	
		blackflag = TRUE;
		pUTrav->Pos_sqcurr.Calc_Index(pCA->pGameWorld->iWidth);
		if ((pMap[pUTrav->Pos_sqcurr.index].barrRevealed & icheck) != 0)
			blackflag = FALSE;
		if ((sectors[pMap[pUTrav->Pos_sqcurr.index].iSector]->iControl) == pCA->pGameOptions->iControl)
			blackflag = FALSE;
		
		if (blackflag == FALSE)
		{

			//calculate rect positions for the particular unit
			//re-using the ybar, xbar variables
			ybar_low = (float(pUTrav->Pos_sqcurr.y) / pCA->pGameWorld->iHeight) * rectheight;
			xbar_low = (float(pUTrav->Pos_sqcurr.x) / pCA->pGameWorld->iWidth) * rectwidth;
		
			//determine blit color
			DDBlitEffects.dwFillColor = pCA->pColorTable->MM_Players[pUTrav->iControl];
			pbHold = (BYTE*)&DDBlitEffects.dwFillColor;

			//we depend on little-endian data storage here
			pbPoint = pbSurface + ((ybar_low * pitch + xbar_low) * jump);

			memcpy(pbPoint, pbHold + 4 - jump, jump);
					if (xfactor < 1)
					{
						memcpy(pbPoint + jump, pbHold + 4 - jump, jump);
						memcpy(pbPoint + pitch*jump, pbHold + 4 - jump, jump);
						memcpy(pbPoint + jump + pitch*jump, pbHold + 4 - jump, jump);

					}
		
		}

				pUTrav = (cUnitA*)pUTrav->pNext_UL;
	}

	//drawing structures
	cStructureA* pSTrav = (cStructureA*)pCA->pStructList->pSL_First->pNext;
	SQCOORDSET*	Positions;

	while( pSTrav != pCA->pStructList->pSL_Last)
	{
		blackflag = TRUE;

		if ((pMap[pSTrav->Pos_primary.index].barrRevealed & icheck) != 0)
			blackflag = FALSE;
		if ((sectors[pMap[pSTrav->Pos_primary.index].iSector]->iControl) == pCA->pGameOptions->iControl)
			blackflag = FALSE;
		
		if (blackflag == FALSE)
		{
		

			//fill in an array with the entire array of offset positions
			Positions = new SQCOORDSET[pSTrav->iNumArrayEntries + 1];
			Positions[0].index = pSTrav->Pos_primary.index;
			Positions[0].x = pSTrav->Pos_primary.x;
			Positions[0].y = pSTrav->Pos_primary.y;
			for (int c36 = 0; c36 < pSTrav->iNumArrayEntries; c36++)
			{
				Positions[c36+1].index = pSTrav->Pos_array[c36].index;
				Positions[c36+1].x = pSTrav->Pos_array[c36].x;
				Positions[c36+1].y = pSTrav->Pos_array[c36].y;
			}

			//now draw each of these positions
			for (int c37 = 0; c37 < pSTrav->iNumArrayEntries + 1; c37++)
			{
				//calculate rect positions for the particular unit
				//re-using the ybar, xbar variables
				ybar_low = (float(Positions[c37].y) / pCA->pGameWorld->iHeight) * rectheight;
				xbar_low = (float(Positions[c37].x) / pCA->pGameWorld->iWidth) * rectwidth;

				//determine blit color
				DDBlitEffects.dwFillColor = pCA->pColorTable->MM_Players[pSTrav->iControl];
				pbHold = (BYTE*)&DDBlitEffects.dwFillColor;

				//we depend on little-endian data storage here
				pbPoint = pbSurface + ((ybar_low * pitch + xbar_low) * jump);

				memcpy(pbPoint, pbHold + 4 - jump, jump);
					if (xfactor < 1)
					{
						memcpy(pbPoint + jump, pbHold + 4 - jump, jump);
						memcpy(pbPoint + pitch*jump, pbHold + 4 - jump, jump);
						memcpy(pbPoint + jump + pitch*jump, pbHold + 4 - jump, jump);
					}
		
			}

		delete(Positions);
		}

		pSTrav = (cStructureA*)pSTrav->pNext;


	}

//unlock and get on with it
hr21 = ProcessSurface.pDDSImage->Unlock(NULL);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cMinimap::Exit()
{

	//release the mapsurface
	MapSurface.pDDSImage->Release();


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
