#include "classes.h"



//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cDD_Display::cDD_Display()
{
	//initially set render mode to invalid (09.17.2004)
	iDDRenderMode = VAL_INVALID;
	//default graphics
		iXR_TempStore = -1;
		iYR_TempStore = -1;
		iBPP_TempStore = -1;
bFlippable = TRUE;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDD_Display::Init(HWND hMainWnd, VOID* pCA)
{
if (DEBUG + DEBUG_DISPLAYDD >= 1)
	Init_LogFile();	

	char NText[50];
	HRESULT hr21;
	const char* KText;

	pCAccess = pCA;

	cClassAccess* pCA2 = (cClassAccess*)pCA;

	//get the rendering mode
	iDDRenderMode = pCA2->pConfig->iTempStore_DDRenderMode;

	//we need to define sys render logic flags
	if (iDDRenderMode == DDRM_NORMAL)
	{
		sysmem_flag_alpharender = 0;
		sysmem_flag_menu = 1;
		sysmem_flag_normal = 0;
	}
	if (iDDRenderMode == DDRM_ALPHA)
	{
		sysmem_flag_alpharender = 1;
		sysmem_flag_menu = 1;
		sysmem_flag_normal = 0;
	}
	if (iDDRenderMode == DDRM_SYSMEM)
	{
		sysmem_flag_alpharender = 1;
		sysmem_flag_menu = 1;
		sysmem_flag_normal = 1;
	}

	//create the directdraw object
//	DirectDrawCreateEx(NULL, (void **)&pDD7, IID_IDirectDraw7, NULL);
	//try using CoCreateInstance( since the Direct..Create functions are apparently not being supported in DX9)
	CoCreateInstance(CLSID_DirectDraw7, NULL, CLSCTX_INPROC, IID_IDirectDraw7, (VOID**)&pDD7);
//	CoCreateInstance(CLSID_DirectDraw, NULL, CLSCTX_INPROC, IID_IDirectDraw, (VOID**)&pDD7);
	pDD7->Initialize(NULL);

	//get caps and output to log
	SystemCaps.dwSize = sizeof(DDCAPS_DX7);
	hr21 = pDD7->GetCaps(&SystemCaps, NULL);

	DDSCAPS2 DDCaps;
	DWORD dwTotal;
	DWORD dwFree;

	ZeroMemory(&DDCaps, sizeof(DDSCAPS2));
	DDCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM;


pDD7->GetAvailableVidMem(&DDCaps, &dwTotal, &dwFree);


//DEBUG
/*
Write_Log("Get DD Caps: ");
KText = DXGetErrorString9(hr21);
Write_Log((char*)KText);
WriteLog_Enter();*/
//DEBUG
Write_Log("Total Video Memory: ");
//ultoa(SystemCaps.dwVidMemTotal, NText, 10);
ultoa(dwTotal, NText, 10);

Write_Log(NText);
WriteLog_Enter();
//DEBUG
Write_Log("Free Video Memory: ");
//ultoa(SystemCaps.dwVidMemFree, NText, 10);
ultoa(dwFree, NText, 10);
Write_Log(NText);
WriteLog_Enter();
	


	HRESULT hrx;

	//set cooperative level, display mode
	hrx = pDD7->SetCooperativeLevel(hMainWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT);
	//hrx = pDD7->SetCooperativeLevel(hMainWnd, DDSCL_NORMAL);

	if (hrx != DD_OK)
		Write_Log("Error at Set Cooperative Level!");
	if (hrx == DD_OK)
		Write_Log("Set Cooperative Level successful.");

	//don't bother with changing the resolution unless it's less than 640x480x16
	//get the surface description(use DMDesc)
	DMDesc.dwSize = sizeof(DDSURFACEDESC2);
	pDD7->GetDisplayMode(&DMDesc);
	iXRes = DMDesc.dwWidth;
	iYRes = DMDesc.dwHeight;
	iBPP = DMDesc.ddpfPixelFormat.dwRGBBitCount;


if (iXR_TempStore > 0)
	iXRes = iXR_TempStore;
if (iYR_TempStore > 0)
	iYRes = iYR_TempStore;
if (iBPP_TempStore > 0)
	iBPP = iBPP_TempStore;


	if (iXRes < 640 || iYRes < 480 || iBPP < 16)
		{
			pDD7->SetDisplayMode(640, 480, 16, 0, 0);
//			pDD7->SetDisplayMode(640,480,16);
			iXRes = 640;
			iYRes = 480;
			iBPP = 16;
		}

	hrx = pDD7->SetDisplayMode(iXRes, iYRes, iBPP, 0, 0);//640, 480, 16, 0, 0);//iXRes, iYRes, 16, 0, 0);
//	hrx = pDD7->SetDisplayMode(iXRes, iYRes, iBPP);
//	hrx = pDD7->SetDisplayMode(800, 600, 16);
//for now, force x16  (12.26.2003)
//	hrx = pDD7->SetDisplayMode(iXRes, iYRes, 16);
//	hrx = pDD7->SetDisplayMode(640, 480, 16,0,0);
	if (hrx != DD_OK)
		Write_Log("Error at Set Display Mode");

	//the game needs an aspect ratio of 4:3 to draw correctly, so check the aspect ratio and try to 
	//maiximize viewing area
	double aspect_ratio = double(iXRes) / double(iYRes);

	i_XResD = iXRes;
	i_YResD = iYRes;
/*
	if (aspect_ratio < 1.32) 
	{
		//if it's too tall, cut the drawable yRes
		i_YResD = iXRes * 0.75;
		i_XResD = iXRes;
	}
	if (aspect_ratio > 1.34)
	{
		//if it's too wide, cut the drawable xRes
		i_XResD = iYRes * 1.3333333333333;
		i_YResD = iYRes;
	}
*/

	iResProduct = i_XResD * i_YResD;

	//get the display description again, just in case it was changed(07.04.2002)
	DMDesc.dwSize = sizeof(DDSURFACEDESC2);
	pDD7->GetDisplayMode(&DMDesc);
	iXRes = DMDesc.dwWidth;
	iYRes = DMDesc.dwHeight;
	iBPP = DMDesc.ddpfPixelFormat.dwRGBBitCount;

	//create a complex display surface
	//fill out a surface description
	DDSDmain.dwSize = sizeof(DDSURFACEDESC2);
	DDSDmain.dwFlags = DDSD_BACKBUFFERCOUNT | DDSD_CAPS;
	DDSDmain.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX; 
	DDSDmain.dwBackBufferCount = 1;

	//create the surface
	HRESULT hr22 = pDD7->CreateSurface(&DDSDmain, &pDDSMain, NULL);

//DEBUG
/*
Write_Log("Create Complex Surface: ");
KText = DXGetErrorString9(hr22);
Write_Log((char*)KText);
WriteLog_Enter();
*/

	//get access to the backsurface
	ZeroMemory(&DDSCAPSback, sizeof(DDSCAPS2));
	DDSCAPSback.dwCaps = DDSCAPS_BACKBUFFER;
	pDDSMain->GetAttachedSurface(&DDSCAPSback, &pDDSBack);

	//get display mode description
	pDD7->GetDisplayMode(&DMDesc);	


	//create an alternate "straight blit" backsurface
	//fill out a surface description
	DDSDmain.dwSize = sizeof(DDSURFACEDESC2);
	DDSDmain.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	DDSDmain.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY; 
	DDSDmain.dwHeight = iYRes * MDRAW_YSCR * 1.1;
	DDSDmain.dwWidth = iXRes * MDRAW_XSCR * 1.1;
	if (DDSDmain.dwHeight > iYRes)
		DDSDmain.dwHeight = iYRes;
	if (DDSDmain.dwWidth > iXRes)
		DDSDmain.dwWidth = iXRes;


		//alter this to the ENTIRE screen for DDRM_SYSMEM (09.17.2004)
		if (iDDRenderMode == DDRM_SYSMEM)
		{
			DDSDmain.dwHeight = iYRes;
			DDSDmain.dwWidth = iXRes;
		}
	
	//create the surface 
	if (iDDRenderMode == DDRM_ALPHA || iDDRenderMode == DDRM_SYSMEM)
	{
		pDD7->CreateSurface(&DDSDmain,&pRenderBuffer, NULL);
	}
		else
		{
			pRenderBuffer = NULL;
		}

//set rendering targets based on DD Render Mode (01.17.2004)
	if (iDDRenderMode == DDRM_ALPHA)
	{
		pPrimaryRenderTarget = pRenderBuffer;
		pSecondaryRenderTarget = pDDSBack;
	}
	if (iDDRenderMode == DDRM_NORMAL)
	{
		pPrimaryRenderTarget = pDDSBack;
		pSecondaryRenderTarget = pDDSBack;
	}
	if (iDDRenderMode == DDRM_SYSMEM)
	{
		pPrimaryRenderTarget = pRenderBuffer;
		pSecondaryRenderTarget = pRenderBuffer;
	}


//get display mode information needed to set up alpha effects(12.26.2003)
bRShift = 0; 
bGShift = 0; 
bBShift = 0;

		//get the bitmasks
		wRBitMask = (DWORD)DMDesc.ddpfPixelFormat.dwRBitMask;
		wGBitMask = (DWORD)DMDesc.ddpfPixelFormat.dwGBitMask;
		wBBitMask = (DWORD)DMDesc.ddpfPixelFormat.dwBBitMask;

		int dwValTest = 1;
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
if (iBPP == 16)
	AMask = 65535 - (1 << bBShift)  - (1 << bRShift) - (1 << bGShift);
if (iBPP == 32)
	AMask = ULONG_MAX - (1 << bBShift)  - (1 << bRShift) - (1 << bGShift);	

	//set color key on surface
	DDCKEmblem.dwColorSpaceHighValue = EMBLEM_REPLACE_COLOR;//Find_ColorKeyIndex(pDDSImage);//RGB(255,255,255);//65535;//RGB(255,255,255);
	DDCKEmblem.dwColorSpaceLowValue = EMBLEM_REPLACE_COLOR;//Find_ColorKeyIndex(pDDSImage);//65535;//RGB(255,255,255);   

	hr22 = pSecondaryRenderTarget->SetColorKey(DDCKEY_DESTBLT, &DDCKEmblem);

//DEBUG
/*
Write_Log("Set ColorKey Secondary: ");
KText = DXGetErrorString9(hr22);
Write_Log((char*)KText);
WriteLog_Enter();
*/
	hr22 = pPrimaryRenderTarget->SetColorKey(DDCKEY_DESTBLT, &DDCKEmblem);

//DEBUG
/*
Write_Log("Set ColorKey Primary: ");
KText = DXGetErrorString9(hr22);
Write_Log((char*)KText);
WriteLog_Enter();
*/

	//initialize "scratch" surface (06.23.2004)
	DDSDscratch.dwSize = sizeof(DDSURFACEDESC2);
	DDSDscratch.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_CAPS;
	DDSDscratch.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	DDSDscratch.dwHeight = 1;
	DDSDscratch.dwWidth =1 ;
	
	hr22 = pDD7->CreateSurface(&DDSDscratch, &pDDS_Scratch, NULL);

//DEBUG
/*
Write_Log("Create Scratch Surface: ");
KText = DXGetErrorString9(hr22);
Write_Log((char*)KText);
WriteLog_Enter();
*/

	//initialize "scratch" surface (06.23.2004)
	DDSDalpha.dwSize = sizeof(DDSURFACEDESC2);
	DDSDalpha.dwFlags = DDSD_HEIGHT | DDSD_WIDTH | DDSD_CAPS;
	DDSDalpha.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	DDSDalpha.dwHeight = TEMP_MAXALPHASIZE;
	DDSDalpha.dwWidth =  TEMP_MAXALPHASIZE;
	
	hr22 = pDD7->CreateSurface(&DDSDalpha, &pDDS_Alpha, NULL);

//DEBUG
	/*
Write_Log("Create Alpha Intermediate Surface: ");
KText = DXGetErrorString9(hr22);
Write_Log((char*)KText);
WriteLog_Enter();
*/

//DEBUG
Write_Log("Initialization Complete.");
WriteLog_Enter();




}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cDD_Display::~cDD_Display()
{

/*  //Exit functions moved to Exit() 07.03.2002
	//close the logfile
	Exit_LogFile();

//	pDDSBack->Release();
//	pDDSMain = NULL;
	pDDSMain->Release();
	pDDSMain = NULL;
	
	//delete the alternate backsurface
	pDDSBack2->Release();
	pDDSBack2 = NULL;

	//restore Displaymode
	pDD7->RestoreDisplayMode();

	pDD7->Release();
	pDD7 = NULL;
*/

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDD_Display::Exit()
{

	//close the logfile
	Exit_LogFile();

//	pDDSBack->Release();
//	pDDSMain = NULL;
	pDDSMain->Release();
	pDDSMain = NULL;
	
	//delete the alternate backsurface
	if (iDDRenderMode == DDRM_SYSMEM || iDDRenderMode == DDRM_ALPHA)
		pRenderBuffer->Release();
	pRenderBuffer = NULL;

	//restore Displaymode
	pDD7->RestoreDisplayMode();

	pDD7->Release();
	pDD7 = NULL;



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDD_Display::Draw_Text_HDC(LPDIRECTDRAWSURFACE7 pDDS, char* Text, int xpos, int ypos)
{
	
	cClassAccess* pCA = (cClassAccess*)pCAccess;
	
	pDDS->GetDC(&hdc00);

	TextOut(hdc00, xpos, ypos, Text, strlen(Text));

	pDDS->ReleaseDC(hdc00);
	
/*
	text_xdraw = xpos;
	text_ydraw = ypos;

//implement a faster way to load text
	//blit the characters from the font01 image surface
	for (c1 = 0; c1 <= strlen(Text)-1; c1++)
		{
		//the character positions on the font image correspond exactly to ASCII text mappings so they are more or less equivalent
		//set the source rect on the source surface
		TextSrcRect.top = (((int)Text[c1]) % BLIT_FPCOL)*FONT01_HEIGHT;
		TextSrcRect.left = (((int)Text[c1]) / BLIT_FPCOL)*FONT01_WIDTH;
		TextSrcRect.bottom = TextSrcRect.top + FONT01_HEIGHT;
		TextSrcRect.right = TextSrcRect.left + FONT01_WIDTH;

		pDDS->BltFast(text_xdraw, text_ydraw, pCA->pImages[ISURF_FONT01]->pDDSImage, &TextSrcRect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		
		//advance the character draw positions
		text_xdraw = text_xdraw + FONT01_WIDTH;
		}
*/
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDD_Display::Colorfill(LPDIRECTDRAWSURFACE7 pDDS, BYTE bRcolor, BYTE bGcolor, BYTE bBcolor)
{

	ZeroMemory(&ddblitfx, sizeof(ddblitfx));
	ddblitfx.dwSize = sizeof(DDBLTFX);

	pDDS->GetSurfaceDesc(&DDSDmain);

double RRatio = float(bRcolor) / 255;
double BRatio = float(bBcolor) / 255;
double GRatio = float(bGcolor) / 255;

DWORD RBitMask = DDSDmain.ddpfPixelFormat.dwRBitMask;
DWORD GBitMask = DDSDmain.ddpfPixelFormat.dwGBitMask;
DWORD BBitMask = DDSDmain.ddpfPixelFormat.dwBBitMask;

DWORD RIn = DWORD(RBitMask * RRatio) & RBitMask;
DWORD GIn = DWORD(GBitMask * GRatio) & GBitMask;
DWORD BIn = DWORD(BBitMask * BRatio) & BBitMask;



	ddblitfx.dwFillColor = RIn + GIn + BIn;

	pDDS->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddblitfx);		

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDD_Display::Colorfill2(LPDIRECTDRAWSURFACE7 pDDS, DWORD dwColorIndex)
{

	ZeroMemory(&ddblitfx, sizeof(ddblitfx));
	ddblitfx.dwSize = sizeof(DDBLTFX);
	ddblitfx.dwFillColor = dwColorIndex;

	pDDS->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &ddblitfx);	


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDD_Display::Colorfill3(LPDIRECTDRAWSURFACE7 pDDS, DWORD dwColorIndex, int iTop, int iBottom, int iLeft, int iRight)
{
	RECT R2;
	R2.top = iTop;
	R2.bottom = iBottom;
	R2.left = iLeft;
	R2.right = iRight;

	ZeroMemory(&ddblitfx, sizeof(ddblitfx));
	ddblitfx.dwSize = sizeof(DDBLTFX);
	ddblitfx.dwFillColor = dwColorIndex;

	pDDS->Blt(&R2, NULL, NULL, DDBLT_COLORFILL, &ddblitfx);	


}

//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDD_Display::Change_Resolution(int xr, int yr, int bpp)
{
	cClassAccess* pCA = (cClassAccess*)pCAccess;

	iXRes = xr;
	iYRes = yr;
	iBPP = bpp;

	//TEMPORARY (1.02.2002)
	//I can't seem to do a successful res change just by that command, so I"m going to go the hard way
	//and release the surfaces, change res, and then reacquire them
//(01.04.2002 - It appears that I keep losing GDI16 objects everytime I do a reschange, so for now lets not do this while the game
//is running...)
	HRESULT hr1 = pDDSMain->Release();

	if (hr1 != DD_OK)
		Write_Log("Error at Release!");
//	pDDSBack->Release();   //crashes when you try to deallocate the back surface

Write_Log("A");

//release all of the image surfaces
	hr1 = pCA->pImages[ISURF_GRASSLAND]->pDDSImage->Release();
		if (hr1 != DD_OK)
		Write_Log("Error at Release!");
Write_Log("A2");
	hr1 = pCA->pImages[ISURF_MOUNTAIN]->pDDSImage->Release();
		if (hr1 != DD_OK)
		Write_Log("Error at Release!");
	hr1 = pCA->pImages[ISURF_HILL]->pDDSImage->Release();
		if (hr1 != DD_OK)
		Write_Log("Error at Release!");
	hr1 = pCA->pImages[ISURF_FOREST]->pDDSImage->Release();
		if (hr1 != DD_OK)
		Write_Log("Error at Release!");
	hr1 = pCA->pImages[ISURF_OCEAN]->pDDSImage->Release();
		if (hr1 != DD_OK)
		Write_Log("Error at Release!");
	hr1 = pCA->pImages[ISURF_RIVER]->pDDSImage->Release();
		if (hr1 != DD_OK)
		Write_Log("Error at Release!");
	hr1 = pCA->pImages[ISURF_CURSOR]->pDDSImage->Release();
		if (hr1 != DD_OK)
		Write_Log("Error at Release!");
	hr1 = pCA->pImages[ISURF_MLAYER2]->pDDSImage->Release();
		if (hr1 != DD_OK)
		Write_Log("Error at Release!");
	hr1 = pCA->pImages[ISURF_FONT01]->pDDSImage->Release();
		if (hr1 != DD_OK)
		Write_Log("Error at Release!");

Write_Log("B");

//	pDD7->SetCooperativeLevel(pCA->pAppData->hMainWnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT);
	pDD7->SetDisplayMode(iXRes, iYRes, iBPP, 0, 0);  //12.26.2003
//	pDD7->SetDisplayMode(iXRes, iYRes, iBPP);

	//create a complex display surface
	//fill out a surface description
	DDSDmain.dwSize = sizeof(DDSURFACEDESC2);
	DDSDmain.dwFlags = DDSD_BACKBUFFERCOUNT | DDSD_CAPS;
	DDSDmain.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX; 
	DDSDmain.dwBackBufferCount = 1;

	//create the surface
	pDD7->CreateSurface(&DDSDmain, &pDDSMain, NULL);

	//get access to the backsurface
	ZeroMemory(&DDSCAPSback, sizeof(DDSCAPS2));
	DDSCAPSback.dwCaps = DDSCAPS_BACKBUFFER;
	hr1 = pDDSMain->GetAttachedSurface(&DDSCAPSback, &pDDSBack);

	//wow....all of the image surfaces have to be reinitialized
//	pCA->pImages[ISURF_GRASSLAND]->Init(pCA->pDD_Display->pDD7, "TGrassland.bmp");
//	pCA->pImages[ISURF_MOUNTAIN]->Init(pCA->pDD_Display->pDD7, "TMountain.bmp");
//	pCA->pImages[ISURF_HILL]->Init(pCA->pDD_Display->pDD7, "THill.bmp");
//	pCA->pImages[ISURF_FOREST]->Init(pCA->pDD_Display->pDD7, "TForest.bmp");
//	pCA->pImages[ISURF_OCEAN]->Init(pCA->pDD_Display->pDD7, "TOcean.bmp");
//	pCA->pImages[ISURF_RIVER]->Init(pCA->pDD_Display->pDD7, "TRiver.bmp");
//	pCA->pImages[ISURF_CURSOR]->Init(pCA->pDD_Display->pDD7, "GCursor.bmp");
//	pCA->pImages[ISURF_MLAYER2]->Init(pCA->pDD_Display->pDD7, "TMLayer2.bmp");
//	pCA->pImages[ISURF_FONT01]->Init(pCA->pDD_Display->pDD7, "Font01.bmp");

	if (hr1 != DD_OK)
		Write_Log("error at get attached surface!");

	//Colorfill(pDDSMain, 100, 100, 100);

	//set values tied to the display mode
	pCA->pGDisplay->MDRAW_NUMROWS = (iYRes * MDRAW_YSCR)/(TILE_SIZE);
	pCA->pGDisplay->MDRAW_NUMCOLS = (iXRes * MDRAW_XSCR)/(TILE_SIZE);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDD_Display::Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_cDD_Display.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDD_Display::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_DISPLAYDD >= 1)
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
void cDD_Display::Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cDD_Display::WriteLog_Enter()
{
if (DEBUG + DEBUG_DISPLAYDD >= 1)
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




