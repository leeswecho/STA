#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cFontInfoTable::Add_Font(int iImageSurf, int iFontIndex, int iHeight, int iWidth, BOOL bSysMemFlag, LPDIRECTDRAW7 pDD7)
{

cClassAccess* pCA = (cClassAccess*)pCAccess;

	Fonts[iFontIndex].iFontISurfIndex = iImageSurf;
	Fonts[iFontIndex].iHeight = iHeight;
	Fonts[iFontIndex].iWidth = iWidth;

	for (int lc1 = 0; lc1 < 256; lc1++)
	{
		Fonts[iFontIndex].CharacterOffsetTable[lc1] = 0;
	}

	//add to desc here
	//fill out a surface description
	Fonts[iFontIndex].BlitMaskDesc.dwSize = sizeof(DDSURFACEDESC2);
	Fonts[iFontIndex].BlitMaskDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    if (bSysMemFlag == TRUE)
	{
		bSysMemFlag = TRUE;
		Fonts[iFontIndex].BlitMaskDesc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY;//OFFSCREENPLAIN;    //DDSCAPS_SYSTEMMEMORY works, but it is somewhat slower
	}
			else
			{
				bSysMemFlag = FALSE;
				Fonts[iFontIndex].BlitMaskDesc.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY;//DDSCAPS_OFFSCREENPLAIN;
			}
	Fonts[iFontIndex].BlitMaskDesc.dwHeight = iHeight;
	Fonts[iFontIndex].BlitMaskDesc.dwWidth = iWidth;
	
	//create the surface(s)
	pDD7->CreateSurface(&Fonts[iFontIndex].BlitMaskDesc, &Fonts[iFontIndex].pDDS_BlitMask, NULL);
	pDD7->CreateSurface(&Fonts[iFontIndex].BlitMaskDesc, &Fonts[iFontIndex].pDDS_BlitMask_Colorstore, NULL);

	//set colorkey(temp for now: set the key to "black" -> 0)
	Fonts[iFontIndex].DDCK_BlitMask.dwColorSpaceLowValue = pCA->pColorTable->White;
	Fonts[iFontIndex].DDCK_BlitMask.dwColorSpaceHighValue = pCA->pColorTable->White;

	Fonts[iFontIndex].pDDS_BlitMask->SetColorKey(DDCKEY_SRCBLT, &Fonts[iFontIndex].DDCK_BlitMask);
	Fonts[iFontIndex].pDDS_BlitMask_Colorstore->SetColorKey(DDCKEY_SRCBLT, &Fonts[iFontIndex].DDCK_BlitMask);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cFontInfoTable::TextLen_Pixels(char* Text, int iStrlen, int iFont)
{
	int iRet = 0;

	for (int lc1 = 0; lc1 < iStrlen; lc1++)
	{
		iRet += Fonts[iFont].iWidth + Fonts[iFont].CharacterOffsetTable[Text[lc1]];
	}

	return iRet;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cFontInfoTable::Trim1(int* pOffsetTable)
{
	for (int lc1 =0; lc1 < 256; lc1++)
	{
		pOffsetTable[lc1] = -2;
	}

	pOffsetTable['%'] = 0;
	pOffsetTable['('] = -3;
	pOffsetTable[')'] = -3;

	pOffsetTable[' '] = -5;
	pOffsetTable['.'] = -6;
	pOffsetTable[','] = -6;
	pOffsetTable[':'] = -6;
	pOffsetTable[';'] = -6;
	pOffsetTable[';'] = -6;
	pOffsetTable['\''] = -6;
	pOffsetTable['`'] = -6;
	pOffsetTable['?'] = -4;
	pOffsetTable['"'] = -4;
	pOffsetTable['!'] = -7;
	pOffsetTable['{'] = -4;
	pOffsetTable['}'] = -4;
	pOffsetTable['['] = -4;
	pOffsetTable[']'] = -4;
	pOffsetTable['1'] = -5;
	pOffsetTable['2'] = -4;
	pOffsetTable['3'] = -4;
	pOffsetTable['4'] = -3;
	pOffsetTable['5'] = -4;
	pOffsetTable['6'] = -3;
	pOffsetTable['7'] = -3;
	pOffsetTable['8'] = -3;
	pOffsetTable['9'] = -3;
	pOffsetTable['0'] = -3;




	pOffsetTable['a'] = -3;
	pOffsetTable['b'] = -3;
	pOffsetTable['c'] = -4;
	pOffsetTable['d'] = -3;
	pOffsetTable['e'] = -3;
	pOffsetTable['f'] = -4;
	pOffsetTable['g'] = -3;
	pOffsetTable['h'] = -3;
	pOffsetTable['i'] = -7;
	pOffsetTable['j'] = -5;
	pOffsetTable['k'] = -4;
	pOffsetTable['l'] = -7;
	pOffsetTable['m'] = -1;
	pOffsetTable['n'] = -3;
	pOffsetTable['o'] = -3;
	pOffsetTable['p'] = -3;
	pOffsetTable['q'] = -3;
	pOffsetTable['r'] = -5;
	pOffsetTable['s'] = -4;
	pOffsetTable['t'] = -5;
	pOffsetTable['u'] = -3;
	pOffsetTable['v'] = -3;
	pOffsetTable['w'] = -1;
	pOffsetTable['x'] = -3;
	pOffsetTable['y'] = -3;
	pOffsetTable['z'] = -4;

	pOffsetTable['A'] = -2;
	pOffsetTable['B'] = -2;
	pOffsetTable['C'] = -2;
	pOffsetTable['D'] = -2;
	pOffsetTable['E'] = -2;
	pOffsetTable['F'] = -2;
	pOffsetTable['G'] = -2;
	pOffsetTable['H'] = -2;
	pOffsetTable['I'] = -5;
	pOffsetTable['J'] = -4;
	pOffsetTable['K'] = -3;
	pOffsetTable['L'] = -4;
	pOffsetTable['M'] = -1;
	pOffsetTable['N'] = -2;
	pOffsetTable['O'] = -1;
	pOffsetTable['P'] = -3;
	pOffsetTable['Q'] = -1;
	pOffsetTable['R'] = -2;
	pOffsetTable['S'] = -3;
	pOffsetTable['T'] = -3;
	pOffsetTable['U'] = -2;
	pOffsetTable['V'] = -3;
	pOffsetTable['W'] = 1;
	pOffsetTable['X'] = -3;
	pOffsetTable['Y'] = -3;
	pOffsetTable['Z'] = -3;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cFontInfoTable::Init(VOID* pCA)
{
	pCAccess = pCA;


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cFontInfoTable::Exit()
{
	//release all surfaces associated with fonts
	for (int c8 = 0; c8 < FONT_NUMFONTS; c8++)
	{
		Fonts[c8].pDDS_BlitMask->Release();
		Fonts[c8].pDDS_BlitMask_Colorstore->Release();
	}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cFontInfoTable::SetFontColor(DWORD dwColor)
{
//colorfill the colorstore sets of each font to the required color
cClassAccess* pCA = (cClassAccess*)pCAccess;
/*
		//release all surfaces associated with fonts
	for (int c8 = 0; c8 < FONT_NUMFONTS; c8++)
	{
//		pCA->pDD_Display->Colorfill2(Fonts[c8].pDDS_BlitMask_Colorstore, dwColor);
//disabled in favor of colorfills, sigh, the colorfills are faster (08.12.2003)
	}
*/

	pCA->pGDisplay->dwFontColor = dwColor;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
