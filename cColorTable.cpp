#include "classes.h"
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void cColorTable::Init(VOID* pCA)
{
	pCAccess = pCA;
	cClassAccess* pCA2 = (cClassAccess*)pCA;

	PixelFormat.dwSize = sizeof(DDPIXELFORMAT);

	//find pixel format of screen
	pCA2->pDD_Display->pDDSMain->GetPixelFormat(&PixelFormat);

	//DWORDS to store and minimap mask values
	RMask = PixelFormat.dwRBitMask;
	iRShift = 0; //number of shifts needed to return value to mask
	iBShift = 0;
	iGShift = 0;
	GMask = PixelFormat.dwGBitMask;
	BMask = PixelFormat.dwBBitMask;

	DWORD RComp, GComp, BComp;

	//shift values until they are at low bit values
	while (RMask % 2 == 0 && iRShift < pCA2->pDD_Display->iBPP)
	{
		RMask = RMask / 2;
		iRShift++;
	}
	while (GMask % 2 == 0 && iGShift < pCA2->pDD_Display->iBPP)
	{
		GMask = GMask / 2;
		iGShift++;
	}
	while(BMask % 2 == 0 && iBShift < pCA2->pDD_Display->iBPP)
	{
		BMask = BMask / 2;
		iBShift++;
	}

//hard code values in
/*
	BMask = 31;
	GMask = 1984;
	RMask = 63488;
	iBShift = 0;
	iGShift = 5;
	iRShift = 11;
*/

	//the bit masks will be used to calculate screen terrain minimap colors
	//in RGB
	//(NOTE. This will likely be a temporary measure as it would probably be better
	//for people to be able to EXTERNALLY select minimap representation colors 02.01.2003)
	
	//for the replace marker color as 250
	Std_ReplaceMarkerColor = EMBLEM_REPLACE_COLOR;

	//for regular grassland: 30% Red, 80% Green, 20% Blue
	MM_Terrain[ISURF_GRASSLAND] = DetColor(0.3,0.8,0.2);

	//for ocean:			15% Red, 45% Green, 85% Blue
	MM_Terrain[ISURF_OCEAN] = DetColor(0.15,0.45,0.85);

	//for hill: 60% Red, 60% Green, 45% Blue
	MM_Terrain[ISURF_HILL] = DetColor(0.6,0.6,0.45);
	
	//for mountain: 45% Red, 45% Green, 35% Blue
	MM_Terrain[ISURF_MOUNTAIN] = DetColor(0.45, 0.45, 0.35);

	//for river(ocean +5%): 20 Red, 50% Green, 95% Blue
	MM_Terrain[ISURF_RIVER] = DetColor(0.2, 0.5, 0.95);

	//for forest: 25% Red, 55% Green, 25% Blue
	MM_Terrain[ISURF_FOREST] = DetColor(0.25, 0.55, 0.25);

	//for player1(blue emblem):
	MM_Players[0] = DetColor(0,0,1);
	MM_Players_Background[0] = DetColor(0.25, 0.6, 1);
	MM_Players_ObjectMark[0] = PixelFormat.dwBBitMask;//RComp | BComp | GComp;
	MM_Players_Console[0] = DetColor(0.28,0.4,0.6);

	
	//for player2(red emblem)	
	MM_Players[1] = DetColor(1, 0, 0);
	MM_Players_Background[1] = DetColor(1, 0.4, 0.4);
	MM_Players_ObjectMark[1] = PixelFormat.dwRBitMask;
	MM_Players_Console[1] = DetColor(0.6,0.28,0.28);

	//for player3(purple emblem)
	MM_Players[2] = DetColor(1, 0, 1);
	MM_Players_Background[2] = DetColor(1, 0.4, 1);
	MM_Players_ObjectMark[2] = PixelFormat.dwRBitMask + PixelFormat.dwBBitMask;//RComp | BComp | GComp;
	MM_Players_Console[2] = DetColor(0.6,0.4,0.6);

	//for player4(yellow emblem)
	MM_Players[3] = DetColor(1, 1, 0);
	MM_Players_Background[3] = DetColor(1, 1, 0.4);
	MM_Players_Console[3] = DetColor(0.6,0.6,0.4);


	MM_Players_ObjectMark[3] = PixelFormat.dwRBitMask + PixelFormat.dwGBitMask;//RComp | BComp | GComp;

	//for player's own (light green): 55% Red, 100% Green, 0% Blue
	MM_Own = DetColor(0.55, 1, 0);

	//for player's enemy (dark red): 80% Red, 25% Green, 25% Blue
	MM_Enemy = DetColor(0.8, 0.25, 0.25);

	White = DetColor(1, 1, 1);

	Red = DetColor(1, 1, 0);

	Yellow = DetColor(1, 1, 0);

	Green = DetColor(0, 1, 0);

	Black = DetColor(0, 0, 0);

	DarkRed = DetColor(0.5, 0.0, 0);

	DarkGreen = DetColor(0, 0.5, 0);

	Gray = DetColor(0.5, 0.5, 0.5);

	DarkGray = DetColor(0.2, 0.2, 0.2);

	WinBlue1 = DetColor(0.4, 0.65, 0.7);

	Blue = DetColor(0, 0, 1);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
DWORD cColorTable::DetColor(double Rratio, double Gratio, double Bratio)
{
	DWORD retdw;
	DWORD RComp = DWORD( RMask * Rratio) << iRShift;
	DWORD GComp = DWORD( GMask * Gratio) << iGShift;
	DWORD BComp = DWORD( BMask * Bratio) << iBShift;

	retdw = RComp | BComp | GComp;

	return retdw;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cColorTable::Det_HPGreenCmp(double ddHealthRatio)
{
	if (ddHealthRatio >= 0.5)
		return 255;

	//map between 0 and 0.5
	return ddHealthRatio * 510; //this is ddHealthRatio * 2 * 255 (so that it reaches 255 at 0.5 health)
	
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
int cColorTable::Det_HPRedCmp(double ddHealthRatio)
{
	//this is just an inversion vs. the green function, so just invert....
	return Det_HPGreenCmp(1.0 - ddHealthRatio);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

