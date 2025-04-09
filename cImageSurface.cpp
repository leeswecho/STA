#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cImageSurface::Init(VOID* pCA, int iSurfIdx, LPDIRECTDRAW7 PDD7, LPSTR FName, BOOL bSysMemFlag, BYTE bExBufferFlags, DWORD dwColorKey, BOOL bForceDimensions, int iForceX, int iForceY)
{

	pCAccess = pCA;
	cClassAccess* pCA2 = (cClassAccess*)pCA;
	pDD7 = PDD7;
	FileName = FName;
	bExtraBufferFlags = bExBufferFlags;
	iSurfIndex = iSurfIdx; //06.25.2004


	char NText[50];


//create the debug log file --> overwrite any files with the same name
if (DEBUG + DEBUG_IMAGESURFACE >= 1)
	hLogFile = CreateFile("./debuglogs/Log_cImageSurface.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	HANDLE                  hbm;
    HRESULT                 hr;

	HDC                     hdcImage;
    HDC                     hdc;
    BITMAP                  bm;

	int						ret, dx, dy;
	BOOL					retb;

	//load the bitmap
	hbm = (HBITMAP) LoadImage(NULL, FName, IMAGE_BITMAP, 0, 0,
                                  LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	hdcImage = CreateCompatibleDC(NULL);

	if (hdcImage == NULL)
		Write_Log("Error at CreateCompatDC!");

	ret = GetObject(hbm, sizeof(bm), &bm);
	
	hbm = (HBITMAP)SelectObject(hdcImage, hbm);
	if (hbm == NULL)
		Write_Log("Error at SelectObject!");

	dx = bm.bmWidth;     
    dy = bm.bmHeight;

	//create a surface to copy the bitmap to 
	ZeroMemory(&DDSDImage, sizeof(DDSURFACEDESC2));
    DDSDImage.dwSize = sizeof(DDSURFACEDESC2);
    DDSDImage.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    if (bSysMemFlag == TRUE)
	{
		bSysMemFlag = TRUE;
		DDSDImage.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY;//OFFSCREENPLAIN;    //DDSCAPS_SYSTEMMEMORY works, but it is somewhat slower
	}
			else
			{
				bSysMemFlag = FALSE;
		DDSDImage.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY;//DDSCAPS_OFFSCREENPLAIN;

			}
 
if (bForceDimensions == FALSE)
{			
	DDSDImage.dwWidth = dx;
    DDSDImage.dwHeight = dy;
}
	else
	{
	DDSDImage.dwWidth = iForceX;
    DDSDImage.dwHeight = iForceY;
	}

	hr = pDD7->CreateSurface(&DDSDImage, &pDDSImage, NULL);


//DEBUG
const char* KText;
pCA2->pDD_Display->Write_Log("Create Image Surface(");
pCA2->pDD_Display->Write_Log(FName);
pCA2->pDD_Display->Write_Log("): ");
/*
KText = DXGetErrorString9(hr);
pCA2->pDD_Display->Write_Log((char*)KText); */
pCA2->pDD_Display->WriteLog_Enter();


	//since both the surface and the bitmap are the same size, a bitblt can be done between them
	pDDSImage->GetDC(&hdc);

//	retb = BitBlt(hdc, 0, 0,dx, dy, hdcImage, 0, 0, SRCCOPY);
	retb = StretchBlt(hdc, 0, 0, DDSDImage.dwWidth, DDSDImage.dwHeight, hdcImage, 0, 0, dx, dy,  SRCCOPY);

	pDDSImage->ReleaseDC(hdc);

	
	DeleteDC(hdcImage); 
	//delete the bitmap....(somehow this got lost in the conversion...I think 01.03.2002)
	DeleteObject(hbm);

	//set the colorkey for the image
	DDCKImage.dwColorSpaceHighValue = dwColorKey;//Find_ColorKeyIndex(pDDSImage);//RGB(255,255,255);//65535;//RGB(255,255,255);
	DDCKImage.dwColorSpaceLowValue = dwColorKey;//Find_ColorKeyIndex(pDDSImage);//65535;//RGB(255,255,255);   

	//TEST 8-bit depth colorkey setting
	if (pCA2->pDD_Display->iBPP == 8)
	{
	DDCKImage.dwColorSpaceHighValue = 255;//Find_ColorKeyIndex(pDDSImage);//RGB(255,255,255);//65535;//RGB(255,255,255);
	DDCKImage.dwColorSpaceLowValue = 255;//Find_ColorKeyIndex(pDDSImage);//65535;//RGB(255,255,255);   

	}

	HRESULT hr1 = pDDSImage->SetColorKey(DDCKEY_SRCBLT, &DDCKImage);

	if (hr1 != DD_OK)
		Write_Log("Error at SetColorKey!");
	switch(hr1)
	{
		case DDERR_GENERIC:
		Write_Log("DDERR_GENERIC");
		break;
		case DDERR_INVALIDOBJECT: 
		Write_Log("DDERR_INVALIDOBJECT");
		break;
		case DDERR_INVALIDPARAMS:
		Write_Log("DDERR_INVALIDPARAMS");
		break;
		case DDERR_INVALIDSURFACETYPE:
		Write_Log("DDERR_INVALIDSURFACETYPE");
		break;
		case DDERR_NOOVERLAYHW:  
		Write_Log("DDERR_NOOVERLAYHW");
		break;
		case DDERR_NOTAOVERLAYSURFACE:
		Write_Log("DDERR_NOTAOVERLAYSURFACE");
		break;
		case DDERR_SURFACELOST:
		Write_Log("DDERR_SURFACELOST");
		break;
		case DDERR_UNSUPPORTED:
		Write_Log("DDERR_UNSUPPORTED");
		break;
		case DDERR_WASSTILLDRAWING: 
		Write_Log("DDERR_WASSTILLDRAWING");
		break;
	}



//get surface desc
ZeroMemory(&DDSDImage, sizeof(DDSURFACEDESC2));
DDSDImage.dwSize = sizeof(DDSURFACEDESC2);
hr1 = pDDSImage->GetSurfaceDesc(&DDSDImage);
/*
	KText = DXGetErrorString9(hr1);
	pCA2->pDD_Display->Write_Log("Get SurfaceDesc: ");
	pCA2->pDD_Display->Write_Log((char*)KText); */
	pCA2->pDD_Display->WriteLog_Enter();


//call marker replace HERE instead of in the main function
	if ( (bExBufferFlags & EXBUFFER_MASK_COLORMARK) != 0)
		pCA2->pGDisplay->ProcessImage_MarkerReplace(iSurfIdx);


	

	pExBuffer = NULL; //initialize

	if (bExBufferFlags > 0) //(0 would indicate no flags, and so don't bother with allocating the buffer)
	{
		int lc1, lc2; //local counters
		pExBuffer = new BYTE[DDSDImage.dwHeight*DDSDImage.dwWidth];
		ZeroMemory(pExBuffer, sizeof(BYTE)*DDSDImage.dwHeight*DDSDImage.dwWidth);

		WORD* pInWord;
		DWORD* pInDWord;
		WORD* pSurfaceStart;
		DWORD* pDWSurfaceStart;
		BYTE* pBSurfaceStart;
		BYTE* pInByte;

		//lock the surface down
		DDSDLock.dwSize = sizeof(DDSURFACEDESC2);

		pDDSImage->Lock(NULL, &DDSDLock, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
		pSurfaceStart = (WORD*)DDSDLock.lpSurface;
		pDWSurfaceStart = (DWORD*)DDSDLock.lpSurface;
		pBSurfaceStart = (BYTE*)DDSDLock.lpSurface;
		
		int iIndex_Source = 0;  //source location on the image
		int iIndex_Dest = 0; //destination location on the exbuffer



		//depending on whether we're in 8-bit or 16-bit drawing mode...
		if (pCA2->pDD_Display->iBPP == 16)
		{
			for (lc2 = 0; lc2 < DDSDImage.dwHeight; lc2++)
			for (lc1 = 0; lc1 < DDSDImage.dwWidth; lc1++)
			{

				iIndex_Source = lc1 + (lc2 * (DDSDImage.lPitch / 2));
				iIndex_Dest = lc1 + (lc2 * DDSDImage.dwWidth);
				pInWord = pSurfaceStart + iIndex_Source;
				pInByte = (BYTE*)pInWord;

				if ( (bExBufferFlags & EXBUFFER_MASK_KEY) != 0)
				{
					if (*pInWord != dwColorKey)
					{
						pExBuffer[iIndex_Dest] = pExBuffer[iIndex_Dest] | EXBUFFER_MASK_KEY;
					}
				}
				if ( (bExBufferFlags & EXBUFFER_MASK_COLORMARK) != 0)
				{
					if (*pInWord == EMBLEM_REPLACE_COLOR)
					{
						pExBuffer[iIndex_Dest] = pExBuffer[iIndex_Dest] | EXBUFFER_MASK_COLORMARK;
					}
				}

			}
		}


		//depending on whether we're in 8-bit or 16-bit drawing mode...
		if (pCA2->pDD_Display->iBPP == 32)
		{
			for (lc2 = 0; lc2 < DDSDImage.dwHeight; lc2++)
			for (lc1 = 0; lc1 < DDSDImage.dwWidth; lc1++)
			{

				iIndex_Source = lc1 + (lc2 * (DDSDImage.lPitch / 4));
				iIndex_Dest = lc1 + (lc2 * DDSDImage.dwWidth);
				pInDWord = pDWSurfaceStart + iIndex_Source;
				pInByte = (BYTE*)pInDWord;

//DEBUG
/*
pCA2->pDD_Display->Write_Log("lc1: ");
itoa(lc1, NText, 10);
pCA2->pDD_Display->Write_Log(NText);
pCA2->pDD_Display->Write_Log("lc2: ");
itoa(lc2, NText, 10);
pCA2->pDD_Display->Write_Log(NText);
pCA2->pDD_Display->Write_Log("Value: ");
itoa(*pInDWord, NText, 10);
pCA2->pDD_Display->Write_Log(NText); 
pCA2->pDD_Display->WriteLog_Enter(); */
				//set opacity to whatever is in first 5 bits
				if ( (bExBufferFlags & EXBUFFER_MASK_KEY) != 0)
				{
					if (*pInDWord != dwColorKey)
					{
						pExBuffer[iIndex_Dest] = pExBuffer[iIndex_Dest] | EXBUFFER_MASK_KEY;
					}
				}
				if ( (bExBufferFlags & EXBUFFER_MASK_COLORMARK) != 0)
				{
					if (*pInDWord == EMBLEM_REPLACE_COLOR)
					{
						pExBuffer[iIndex_Dest] = pExBuffer[iIndex_Dest] | EXBUFFER_MASK_COLORMARK;
					}
				}
//DEBUG
/*
pCA2->pDD_Display->Write_Log(" ExBuffer: ");
itoa(pExBuffer[lc1], NText, 10);
pCA2->pDD_Display->Write_Log(NText);
pCA2->pDD_Display->WriteLog_Enter();   */
			}
		}
		

		if (pCA2->pDD_Display->iBPP == 8)
		{
			for (lc1 = 0; lc1 < dx * dy; lc1++)
			{
				pInByte = pBSurfaceStart + lc1;
				
				//set opacity to whatever is in first 5 bits
				if ( (bExBufferFlags & EXBUFFER_MASK_KEY) != 0)
				if (*pInByte != dwColorKey)
				{
					pExBuffer[lc1] += 1 << EXBUFFER_KEY;
				}
				if ( (bExBufferFlags & EXBUFFER_MASK_COLORMARK) != 0)
				if (*pInByte == EMBLEM_REPLACE_COLOR)
				{
					pExBuffer[lc1] += 1 << EXBUFFER_COLORMARK;
				}

			}
		}

		pDDSImage->Unlock(NULL);

	}

//DEBUG update system caps
	DWORD dwTotal;
	DWORD dwFree;
	DDSCAPS2 DDCaps;

	ZeroMemory(&DDCaps, sizeof(DDSCAPS2));
	DDCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM;


PDD7->GetCaps(&pCA2->pDD_Display->SystemCaps, NULL);
PDD7->GetAvailableVidMem(&DDCaps, &dwTotal, &dwFree);
	itoa(pCA2->pDD_Display->SystemCaps.dwVidMemTotal - pCA2->pDD_Display->SystemCaps.dwVidMemFree, NText, 10);
pCA2->pDD_Display->Write_Log(FName);
pCA2->pDD_Display->Write_Log(": ");
pCA2->pDD_Display->Write_Log(NText);
pCA2->pDD_Display->Write_Log(" ");
	itoa(dwFree, NText, 10);
pCA2->pDD_Display->Write_Log(NText);
pCA2->pDD_Display->WriteLog_Enter();


	//signal that Init has been called for this class
	bInit_Run = TRUE;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cImageSurface::cImageSurface()
{

bInit_Run = FALSE;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
cImageSurface::~cImageSurface()
{
	
/*   //Exit Procedures moved to Exit() 07.03.2002
if (bInit_Run == TRUE)
	pDDSImage->Release();

pDDSImage = NULL; */

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cImageSurface::Exit()
{

	if (bInit_Run == TRUE)
	pDDSImage->Release();

pDDSImage = NULL;

	//delete the opacity buffer if allocated
	if (pExBuffer != NULL)
		delete(pExBuffer);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cImageSurface::Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_IMAGESURFACE >= 1)
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
void cImageSurface::WriteLog_Enter()
{
if (DEBUG + DEBUG_IMAGESURFACE >= 1)
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
DWORD cImageSurface::Find_ColorKeyIndex(LPDIRECTDRAWSURFACE7 pImageSurf)
{

//find what the correct key value for this surface is(what value represents white for this surface)
DDSURFACEDESC2 ddsdc;
DWORD Color_Return = 0;

	ddsdc.dwSize = sizeof(DDSURFACEDESC2);	

	pImageSurf->GetSurfaceDesc(&ddsdc);

	//the value for white will be all of the red, white, and blue bit masks combined...

	Color_Return = ddsdc.ddpfPixelFormat.dwRBitMask + ddsdc.ddpfPixelFormat.dwGBitMask + ddsdc.ddpfPixelFormat.dwBBitMask;

	return Color_Return;

}
