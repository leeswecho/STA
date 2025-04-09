#include "header01 - main.h"

//prototypes
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProc2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


DWORD WINAPI ThreadFunc( LPVOID lpParam ) ;

DWORD WINAPI TimerThreadFunc(LPVOID lpParam);

void Path_Test(int iIndex_Start, int iIndex_Goal);
void RandTest();

void Calc_SplashRects();
void Swap_SplashGraphics(char* NewFileName);
void Clear_SplashGraphics();
void Load_SplashGraphics(char* NewFileName);

void Test_Init_UnitStructCmp_Data();
void Test_Init_Objects();
void Test_FakeAIModule();
void Test_CheatModule();
void Test_AdditModule();  //module for additional tests (06.11.2004)

void NotifyBox(char* Text, LPDIRECTDRAWSURFACE7 pDrawSurf);

int Temp_Determine_Winner(); 
void End_Game();


void Test_Flip(); //tries to flip the buffer with the least waiting time possible

void MainLoop_GTick();
void MainLoop_GDraw();
void MainLoop_Tutorial();

		HANDLE		hLogFile;
		void Write_Log(LPSTR Text);
		void WriteLog_Enter();
		void Init_LogFile();
		void Exit_LogFile();

void Disp_LoadProgress(LPSTR InText, double Percent);


int FAR PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

if (DEBUG + DEBUG_MAIN >= 1)
	Init_LogFile();

	//program inits
	ClassAccess.Init(&DisplayDD, 
					 &SectorTable,
					 &InputDI, 
					 &NetDP,
					 &AudioDA,
					 Images, 
					 Sounds,
					 &GDisplay, 
					 &GInput, 
					 &GInterface, 
					 &GMap, 
					 &CommQueue,
					 &AppData, 
					 &TimerControl, 
					 &UnitList, 
					 &UnitIDTable, 
					 &ProjList, 
					 &PathFinder, 
					 &UnitTypeTable, 
					 &ProjTypeTable, 
					 &GameOptions, 
					 &RandGen, 
					 &ChatBuffer,
					 &Minimap,
					 &ColorTable,
					 &ComponentTypes,
					 &FontInfo,
					 &CmpOffsetReference,
					 &StructList,
					 &StructIDTable,
					 &StructureTypes,
					 &CmpMasterIDTable,
					 &CmdDataReference,
					 &InputPackets,
					 &CmdInfoReference,
					 &Nations,
					 &Notifications,
					 &Animations,
					 &Events,
					 &MemDebugger,
					 &Strings,
					 &Configuration,
					 &Acknowledgements,
					 &MaterialTypes);



/////////////////////////

//DEBUG (removed if not _DEBUG)
	//_ASSERTE( _CrtCheckMemory() );

    	AppData.Init(hInstance, hPrevInstance, lpCmdLine, "STA - Seize The Advantage", WindowProc, WindowProc2, &ClassAccess);
	AppData.Find_Maps(); //put this at the very beginning for the moment
	//DEBUG
	char NText[50];

	//HACK (sneak in a converter tool functionality(05.11.2004)
	if (HACK_MODE == 1)
	{
		srand(time(NULL));
		MapConv.Init("infile.bmp", "infile2.bmp", "Outfile.map");
		MapConv.Write_MapFile_200();

		return 0;
	}

	Configuration.Init(&ClassAccess);
	Configuration.Load_Data("./config/General_Configuration.ini");



if (Configuration.bTempStore_ForceCore0 == TRUE)
{
	//TEST for now, force affinity in place of the timer thread hack
	HANDLE hProcess = GetCurrentProcess();
	DWORD dwProcessAffinityMask, dwSystemAffinityMask;
	GetProcessAffinityMask( hProcess, &dwProcessAffinityMask, &dwSystemAffinityMask );
	SetProcessAffinityMask( hProcess, 1L );// use CPU 0 only
}
//DEBUG (removed if not _DEBUG)
	//_ASSERTE( _CrtCheckMemory() );

	DisplayDD.Init(AppData.hMainWnd, &ClassAccess);

//load the colortable(so that image surfaces can set colorkeys)
	ColorTable.Init(&ClassAccess);
	FontInfo.Init(&ClassAccess);
	//D3D! (disabled. will not be in Director Project. (05.28.2004)
	//DisplayD3D.Init(&ClassAccess, AppData.hMainWnd);
	GDisplay.Init(&ClassAccess);
		//NOTE: the "ISURF_SPLASH" surface will be continually rotated in and out of memory (as needed)
		//due to its large size. (here I am loading it in to show the background screen
		Images[ISURF_SPLASH].Init(&ClassAccess, ISURF_SPLASH, DisplayDD.pDD7, "./media/gfx/BG0.bmp", TRUE, FALSE, ColorTable.White, FALSE, 0, 0);	
		//determine the right image size to get the correct scaling
		Calc_SplashRects();

		//prime both surfaces
		DisplayDD.Colorfill2(DisplayDD.pDDSMain, 0);
		DisplayDD.Colorfill2(DisplayDD.pDDSBack, 0);

		DisplayDD.pDDSMain->Blt(&SplashRect, Images[ISURF_SPLASH].pDDSImage, &SplashSrcRect, 0, NULL);

//
AppData.bRunStage = APP_RUNSTAGE_INIT;

	//notifbuffer init might as well go here(08.11.2003)
	Notifications.Init(&ClassAccess);

	//might as well go here. Init the CmdInfo and CmdData references
	CmdDataReference.Init();
	CmdInfoReference.Init(&ClassAccess);

//DEBUG (removed if not _DEBUG)
	//_ASSERTE( _CrtCheckMemory() );


//at least must go before component types initialization
	CmpOffsetReference.Init();




//init media files
//TEST?
	Images[ISURF_FONT01].Init(&ClassAccess, ISURF_FONT01, DisplayDD.pDD7, "./media/gfx/Font03.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.Black, FALSE, 0, 0); //note that font graphics have colorkey black
	//add font info also
	//FontInfo.Add_Font(ISURF_FONT01, FONT_TEST01, 15, 8,  DisplayDD.sysmem_flag_normal, DisplayDD.pDD7);
	FontInfo.Add_Font(ISURF_FONT01, FONT_TEST01, 12, 10,  DisplayDD.sysmem_flag_normal, DisplayDD.pDD7);
	FontInfo.Trim1(FontInfo.Fonts[FONT_TEST01].CharacterOffsetTable);

//if (DisplayDD.iXRes > 800)
//{
	Images[ISURF_FONT02].Init(&ClassAccess, ISURF_FONT02, DisplayDD.pDD7, "./media/gfx/Font03.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.Black, FALSE, 0, 0);
	//add font info also
	//FontInfo.Add_Font(ISURF_FONT02, FONT_STANDARD1, 15, 8, DisplayDD.sysmem_flag_normal,  DisplayDD.pDD7);
	FontInfo.Add_Font(ISURF_FONT02, FONT_STANDARD1, 12, 10, DisplayDD.sysmem_flag_normal,  DisplayDD.pDD7);
	FontInfo.Trim1(FontInfo.Fonts[FONT_STANDARD1].CharacterOffsetTable);

	Images[ISURF_ALPHA_FONT03].Init(&ClassAccess, ISURF_ALPHA_FONT03, DisplayDD.pDD7, "./media/gfx/Font03.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
	//add font info also
	//FontInfo.Add_Font(ISURF_ALPHA_FONT03, FONT_ALPHA_STANDARD1, 15, 8, DisplayDD.sysmem_flag_alpharender, DisplayDD.pDD7);
	FontInfo.Add_Font(ISURF_ALPHA_FONT03, FONT_ALPHA_STANDARD1, 12, 10, DisplayDD.sysmem_flag_alpharender, DisplayDD.pDD7);
	FontInfo.Trim1(FontInfo.Fonts[FONT_ALPHA_STANDARD1].CharacterOffsetTable);
/*
} else
{
	Images[ISURF_FONT02].Init(&ClassAccess, ISURF_FONT02, DisplayDD.pDD7, "./media/gfx/Font03.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.Black, FALSE, 0, 0);
	//add font info also
	//FontInfo.Add_Font(ISURF_FONT02, FONT_STANDARD1, 9, 6, DisplayDD.sysmem_flag_normal,  DisplayDD.pDD7);
	FontInfo.Add_Font(ISURF_FONT02, FONT_STANDARD1, 12, 10, DisplayDD.sysmem_flag_normal,  DisplayDD.pDD7);
	FontInfo.Trim1(FontInfo.Fonts[FONT_STANDARD1].CharacterOffsetTable);

	Images[ISURF_ALPHA_FONT03].Init(&ClassAccess, ISURF_ALPHA_FONT03, DisplayDD.pDD7, "./media/gfx/Font03.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.Black, FALSE, 0, 0);
	//add font info also
	//FontInfo.Add_Font(ISURF_ALPHA_FONT03, FONT_ALPHA_STANDARD1, 9, 6,  DisplayDD.sysmem_flag_alpharender, DisplayDD.pDD7);
	FontInfo.Add_Font(ISURF_ALPHA_FONT03, FONT_ALPHA_STANDARD1, 12, 10, DisplayDD.sysmem_flag_alpharender, DisplayDD.pDD7);
	FontInfo.Trim1(FontInfo.Fonts[FONT_ALPHA_STANDARD1].CharacterOffsetTable);
}
*/
//STATUS
FontInfo.SetFontColor(ColorTable.Green);
GDisplay.Draw_Text(DisplayDD.pDDSMain, "LOADING...", SplashRect.left + (SplashRect.right - SplashRect.left) * 0.305, SplashRect.top + (SplashRect.bottom - SplashRect.top) * 0.72, FONT_STANDARD1);
Disp_LoadProgress("I: TGRASSLAND.BMP", (1.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_GRASSLAND].Init(&ClassAccess, ISURF_GRASSLAND, DisplayDD.pDD7, "./media/gfx/TGrassland.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
	Images[ISURF_GRASSLAND].SetFrameSize(TILE_SIZE, TILE_SIZE, 0, 0);
//STATUS
Disp_LoadProgress("I: TMOUNTAIN.BMP", (2.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_MOUNTAIN].Init(&ClassAccess, ISURF_MOUNTAIN, DisplayDD.pDD7, "./media/gfx/TMountain.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.Red, FALSE, 0, 0);
	Images[ISURF_MOUNTAIN].SetFrameSize(TILE_SIZE, TILE_SIZE, 0, 0);
//STATUS
Disp_LoadProgress("I: THILL.BMP", (3.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_HILL].Init(&ClassAccess, ISURF_HILL, DisplayDD.pDD7, "./media/gfx/THill.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
	Images[ISURF_HILL].SetFrameSize(TILE_SIZE, TILE_SIZE, 0, 0);
//STATUS
Disp_LoadProgress("I: TFOREST2.BMP", (4.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_FOREST].Init(&ClassAccess, ISURF_FOREST, DisplayDD.pDD7, "./media/gfx/TForest2.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
	Images[ISURF_FOREST].SetFrameSize(28, 42, 0, -7); //test
//STATUS
Disp_LoadProgress("I: TOCEAN.BMP", (5.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_OCEAN].Init(&ClassAccess, ISURF_OCEAN, DisplayDD.pDD7, "./media/gfx/TOcean.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
	Images[ISURF_OCEAN].SetFrameSize(TILE_SIZE, TILE_SIZE, 0, 0);
//STATUS
Disp_LoadProgress("I: TRIVER.BMP", (5.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_RIVER].Init(&ClassAccess, ISURF_RIVER, DisplayDD.pDD7, "./media/gfx/TRiver.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
	Images[ISURF_RIVER].SetFrameSize(TILE_SIZE, TILE_SIZE, 0, 0);
//STATUS
Disp_LoadProgress("I: GCURSOR.BMP", (7.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_CURSOR].Init(&ClassAccess, ISURF_CURSOR, DisplayDD.pDD7, "./media/gfx/GCursor.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: TMLAYER2.BMP", (8.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_MLAYER2].Init(&ClassAccess, ISURF_MLAYER2, DisplayDD.pDD7, "./media/gfx/TMLayer2.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
	Images[ISURF_MLAYER2].SetFrameSize(TILE_SIZE, TILE_SIZE, 0, 0);
//STATUS
Disp_LoadProgress("I: LINKS.BMP", (8.5/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_LINKS].Init(&ClassAccess, ISURF_LINKS, DisplayDD.pDD7, "./media/gfx/Links.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
	Images[ISURF_LINKS].SetFrameSize(TILE_SIZE, TILE_SIZE, 0, 0);
//STATUS
//Disp_LoadProgress("I: CONSOLE_RIGHT.BMP", (9.0/NUM_MAX_ISURFACES)*0.33);
//	Images[ISURF_CONSOLE_RIGHT].Init(&ClassAccess, ISURF_CONSOLE_RIGHT, DisplayDD.pDD7, "./media/gfx/Console_Right.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, TRUE, DisplayDD.i_XResD * (1 - MDRAW_XSCR), DisplayDD.i_YResD);  
//STATUS
//Disp_LoadProgress("I: CONSOLE_BOTTOM.BMP", (10.0/NUM_MAX_ISURFACES)*0.33);
//	Images[ISURF_CONSOLE_BOTTOM].Init(&ClassAccess, ISURF_CONSOLE_BOTTOM, DisplayDD.pDD7, "./media/gfx/Console_Bottom.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, TRUE, DisplayDD.i_XResD * MDRAW_XSCR, DisplayDD.i_YResD * (1 - MDRAW_YSCR));
//STATUS
Disp_LoadProgress("I: FRAME01-64.BMP", (11.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_FRAME_TEST01].Init(&ClassAccess, ISURF_FRAME_TEST01, DisplayDD.pDD7, "./media/gfx/Frame01-64.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: FRAME02-64.BMP", (12.0/NUM_MAX_ISURFACES)*0.33);	
	Images[ISURF_FRAME_02].Init(&ClassAccess, ISURF_FRAME_02, DisplayDD.pDD7, "./media/gfx/Frame02-64.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: FRAME03-64.BMP", (13.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_FRAME_03].Init(&ClassAccess, ISURF_FRAME_03, DisplayDD.pDD7, "./media/gfx/Frame03-64.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: FRAME04-64.BMP", (14.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_FRAME_04].Init( &ClassAccess, ISURF_FRAME_04, DisplayDD.pDD7, "./media/gfx/Frame04-64.bmp", DisplayDD.sysmem_flag_alpharender, EXBUFFER_MASK_KEY | EXBUFFER_MASK_COLORMARK, ColorTable.White, FALSE, 0, 0);
	//TEST (process for marker (success?) (moved INSIDE function init 06.25.2004)
	//GDisplay.ProcessImage_MarkerReplace(ISURF_FRAME_04);
//STATUS
//Disp_LoadProgress("I: TANKGUN01-64.BMP", (15.0/NUM_MAX_ISURFACES)*0.33);
//	Images[ISURF_TANKGUN_TEST01].Init(&ClassAccess, ISURF_TANKGUN_TEST01, DisplayDD.pDD7, "./media/gfx/Tankgun01-64.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: TANKGUN02-1.BMP", (16.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_TANKGUN_TEST02].Init(&ClassAccess, ISURF_TANKGUN_TEST02, DisplayDD.pDD7, "./media/gfx/Tankgun02-1.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: TANKGUN03-64.BMP", (17.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_TANKGUN_03].Init(&ClassAccess, ISURF_TANKGUN_03, DisplayDD.pDD7, "./media/gfx/Tankgun03-64.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: TANKGUN04-64.BMP", (18.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_TANKGUN_04].Init(&ClassAccess, ISURF_TANKGUN_04, DisplayDD.pDD7, "./media/gfx/Tankgun04-64.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: NAVALGUN01-16.BMP", (19.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_NAVALGUN_01].Init(&ClassAccess, ISURF_NAVALGUN_01, DisplayDD.pDD7, "./media/gfx/navalgun01-16.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: Structure-Outpost2b.bmp", (20.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_STRUCT_TEST01].Init(&ClassAccess,  ISURF_STRUCT_TEST01, DisplayDD.pDD7,  "./media/gfx/Structure-Outpost2b.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: STRUCTURE-RSC-2.BMP", (21.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_STRUCT_RSC01].Init(&ClassAccess, ISURF_STRUCT_RSC01, DisplayDD.pDD7, "./media/gfx/Structure-RSC-2.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: STRUCTURE-TANK-2.BMP", (21.5/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_STRUCT_TANK].Init(&ClassAccess, ISURF_STRUCT_TANK, DisplayDD.pDD7, "./media/gfx/Structure-Tank-2.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: STRUCTURE-FACTORY-2.BMP", (21.75/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_STRUCT_FACTORY_ABC].Init(&ClassAccess, ISURF_STRUCT_FACTORY_ABC, DisplayDD.pDD7, "./media/gfx/Structure-Factory-2.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: STRUCTURE-FORGE-2.BMP", (21.8/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_STRUCT_FORGE].Init(&ClassAccess, ISURF_STRUCT_FORGE, DisplayDD.pDD7, "./media/gfx/Structure-Forge-2.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: STRUCTURE-REFINERY-2.BMP", (21.9/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_STRUCT_REFINERY].Init(&ClassAccess, ISURF_STRUCT_REFINERY, DisplayDD.pDD7, "./media/gfx/Structure-Refinery-2.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: STRUCTURE-FUSIONREACTOR-2.BMP", (21.9/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_STRUCT_REACTOR].Init(&ClassAccess, ISURF_STRUCT_REACTOR, DisplayDD.pDD7, "./media/gfx/Structure-FusionReactor-2.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: STRUCTURE-SHIPYARD-2.BMP", (21.95/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_STRUCT_SHIPYARD].Init(&ClassAccess, ISURF_STRUCT_SHIPYARD, DisplayDD.pDD7, "./media/gfx/Structure-Shipyard-2.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: SELECTBOX.BMP", (22.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_SELECT].Init(&ClassAccess, ISURF_SELECT, DisplayDD.pDD7,"./media/gfx/SelectBox.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, FALSE, 0, 0);
	Images[ISURF_SELECT].SetFrameSize(24, 24, 0, 0);
//STATUS
Disp_LoadProgress("I: EMBLEMS0.BMP", (23.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_EMBLEMS].Init(&ClassAccess, ISURF_EMBLEMS, DisplayDD.pDD7, "./media/gfx/Emblems0.bmp", DisplayDD.sysmem_flag_alpharender, 0 + DisplayDD.iDDRenderMode, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: PROJECTILE01.BMP", (24.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_PROJ_TEST01].Init(&ClassAccess, ISURF_PROJ_TEST01, DisplayDD.pDD7, "./media/gfx/Projectile01.bmp", DisplayDD.sysmem_flag_normal , 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: ICONS.BMP", (24.5/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_ICONS].Init(&ClassAccess, ISURF_ICONS, DisplayDD.pDD7, "./media/gfx/icons.bmp", DisplayDD.sysmem_flag_alpharender , 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: PROJECTILE02.BMP", (25.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_PROJ_TEST02].Init(&ClassAccess, ISURF_PROJ_TEST02, DisplayDD.pDD7, "./media/gfx/Projectile02.bmp", DisplayDD.sysmem_flag_normal , 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: PROJECTILE_NAVAL120-1.BMP", (26.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_PROJ_NAVAL120].Init(&ClassAccess, ISURF_PROJ_NAVAL120, DisplayDD.pDD7, "./media/gfx/Projectile_naval120-1.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: HPBAR01.BMP", (27.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_HPBAR].Init(&ClassAccess, ISURF_HPBAR, DisplayDD.pDD7, "./media/gfx/HPBar01.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: MENU00.BMP", (28.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_MAINMENU].Init(&ClassAccess, ISURF_MAINMENU, DisplayDD.pDD7, "./media/gfx/Menu00.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: WHITE.BMP", (28.5/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_WHITE].Init(&ClassAccess, ISURF_WHITE, DisplayDD.pDD7, "./media/gfx/White.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
//Disp_LoadProgress("I: MENU01.BMP", (29.0/NUM_MAX_ISURFACES)*0.33);
//	Images[ISURF_MENUBUTTONS00].Init(&ClassAccess, ISURF_MENUBUTTONS00, DisplayDD.pDD7, "./media/gfx/Menu01.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: MENU03.BMP", (29.5/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_MENUBUTTONS01].Init(&ClassAccess, ISURF_MENUBUTTONS01, DisplayDD.pDD7, "./media/gfx/Menu03.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
//Disp_LoadProgress("I: MENU02.BMP", (30.0/NUM_MAX_ISURFACES)*0.33);
//	Images[ISURF_MENUBOXES00].Init(&ClassAccess, ISURF_MENUBOXES00, DisplayDD.pDD7, "./media/gfx/Menu02.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: MENU04.BMP", (30.25/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_MENUBOXES01].Init(&ClassAccess, ISURF_MENUBOXES01, DisplayDD.pDD7, "./media/gfx/Menu04.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: MENU05.BMP", (30.4/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_MENU05].Init(&ClassAccess, ISURF_MENU05, DisplayDD.pDD7, "./media/gfx/Menu05.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: ANIM-BLAST.BMP", (30.45/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_ANIM_BLAST].Init(&ClassAccess, ISURF_ANIM_BLAST, DisplayDD.pDD7, "./media/gfx/Anim-blast.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: STRUCT-EXTRACTOR.BMP", (30.46/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_STRUCT_EXTRACTOR].Init(&ClassAccess, ISURF_STRUCT_EXTRACTOR, DisplayDD.pDD7, "./media/gfx/Structure-Extractor-2.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: BLACK.BMP", (30.5/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_BLACK].Init(&ClassAccess, ISURF_BLACK, DisplayDD.pDD7, "./media/gfx/Black.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
	Images[ISURF_BLACK].SetFrameSize(33, 33, 0, 0);


//STATUS
	//special loading consideration for scrollbar(have it depend on load resolution
	int iX = 8;
	int iY = 48;
	if (DisplayDD.iXRes == 800) { iX = 10; iY = 60; }
	if (DisplayDD.iXRes == 1024) { iX = 12; iY = 72; }
	if (DisplayDD.iXRes == 1280) { iX = 14; iY = 84; }
	if (DisplayDD.iXRes == 1400) { iX = 16; iY = 96; }
Disp_LoadProgress("I: SCROLLBAR.BMP", (30.5/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_SCROLLBAR].Init(&ClassAccess, ISURF_SCROLLBAR, DisplayDD.pDD7, "./media/gfx/scrollbar.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, TRUE, 16, 96);
//STATUS
Disp_LoadProgress("I: CMDBUTTONS.BMP", (31.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_CMDBUTTONS].Init(&ClassAccess, ISURF_CMDBUTTONS, DisplayDD.pDD7, "./media/gfx/CmdButtons.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: CMDBUTTONS2.BMP", (31.5/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_CMDBUTTONS2].Init(&ClassAccess, ISURF_CMDBUTTONS2, DisplayDD.pDD7, "./media/gfx/CmdButtons2.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: CMDBUTTONSSQ.BMP", (31.75/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_CMDBUTTONS_SQ].Init(&ClassAccess, ISURF_CMDBUTTONS_SQ, DisplayDD.pDD7, "./media/gfx/cmdButtons_square.bmp", DisplayDD.sysmem_flag_normal, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: ANIM-TEST01.BMP", (32.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_ANIM_TEST01].Init(&ClassAccess, ISURF_ANIM_TEST01, DisplayDD.pDD7, "./media/gfx/Anim-Test01.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: ANIM-TEST02.BMP", (33.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_ANIM_TEST02].Init(&ClassAccess, ISURF_ANIM_TEST02,DisplayDD.pDD7, "./media/gfx/Anim-Test02.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: ANIM-TEST03.BMP", (34.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_ANIM_TEST03].Init(&ClassAccess, ISURF_ANIM_TEST03,DisplayDD.pDD7, "./media/gfx/Anim-Test03.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: ANIM-TEST04.BMP", (35.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_ANIM_TEST04].Init(&ClassAccess, ISURF_ANIM_TEST04, DisplayDD.pDD7, "./media/gfx/Anim-Test04.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: ANIM-UEXPL01.BMP", (36.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_ANIM_UEXPL01].Init(&ClassAccess, ISURF_ANIM_UEXPL01, DisplayDD.pDD7, "./media/gfx/Anim-uexpl01.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STATUS
Disp_LoadProgress("I: ANIM-MSLTRAIL1.BMP", (37.0/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_ANIM_MSLTRAIL1].Init(&ClassAccess, ISURF_ANIM_MSLTRAIL1, DisplayDD.pDD7, "./media/gfx/Anim-msltrail1.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, FALSE, 0, 0);
//STAUTS
Disp_LoadProgress("I: PIE.BMP", (37.1/NUM_MAX_ISURFACES)*0.33);
	Images[ISURF_PIE1].Init(&ClassAccess, ISURF_PIE1, DisplayDD.pDD7, "./media/gfx/Pie.bmp", DisplayDD.sysmem_flag_alpharender, 0, ColorTable.White, TRUE, 80, 120);
	Images[ISURF_PIE1].SetFrameSize(20, 20, 0, 0);
//STATUS
Disp_LoadProgress("INIT.: EVENT BUFFER", 0.34);
	Events.Init(&ClassAccess);
//STATUS
Disp_LoadProgress("INIT.: ANIMATIONS", 0.34);
	Animations.Init(&ClassAccess);
//STATUS
Disp_LoadProgress("INIT.: DIRECTINPUT", 0.35);
	//init DirectInput
	InputDI.Init(AppData.InstanceHandle, AppData.hMainWnd);
//STATUS
Disp_LoadProgress("INIT.: USER INPUT HANDLER", 0.36);
	GInput.Init(&ClassAccess);
//STATUS
Disp_LoadProgress("INIT.: DIRECTAUDIO", 0.37);
	AudioDA.Init(&ClassAccess, FALSE);
//STATUS
Disp_LoadProgress("SOUND: WXPD.WAV", 0.38 + 0.07*(1.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_TEST1].Init(&ClassAccess, NULL, NULL, "./media/sfx/WXPD.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: WEAPON_80MM.WAV", 0.38 + 0.07*(2.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_FIRE_80MM].Init(&ClassAccess, NULL, NULL, "./media/sfx/weapon_80mm.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: WEAPON_120MM.WAV", 0.38 + 0.07*(2.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_FIRE_120MM].Init(&ClassAccess, NULL, NULL, "./media/sfx/weapon_120mm.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: WEAPON_MISSILE.WAV", 0.38 + 0.07*(2.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_FIRE_MSL].Init(&ClassAccess, NULL, NULL, "./media/sfx/weapon_missile.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: WEAPON_NAVAL120.WAV", 0.38 + 0.07*(2.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_FIRE_NAVAL120].Init(&ClassAccess, NULL, NULL, "./media/sfx/weapon_naval120.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: SOUND_GUN1.WAV", 0.38 + 0.07*(3.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_GUN1].Init(&ClassAccess, NULL, NULL, "./media/sfx/sound_gun1.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: IMPACT1.WAV", 0.38 + 0.07*(3.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_IMPACT1].Init(&ClassAccess,NULL, NULL, "./media/sfx/impact1.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: IMPACT2.WAV", 0.38 + 0.07*(3.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_IMPACT2].Init(&ClassAccess, NULL, NULL, "./media/sfx/impact2.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: UEXPL01.WAV", 0.38 + 0.08*(3.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_UEXPL01].Init(&ClassAccess, NULL, NULL, "./media/sfx/uexpl01.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: AIRMSL.WAV", 0.38 + 0.09*(3.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_AIRMSL].Init(&ClassAccess, NULL, NULL, "./media/sfx/airmsl.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: ACK0.WAV", 0.38 + 0.09*(3.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_ACK0].Init(&ClassAccess, NULL, NULL, "./media/sfx/ack0.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: ACK1.WAV", 0.38 + 0.09*(3.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_ACK1].Init(&ClassAccess, NULL, NULL, "./media/sfx/ack1.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: ACK2.WAV", 0.38 + 0.09*(3.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_ACK2].Init(&ClassAccess, NULL, NULL, "./media/sfx/ack2.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: ACK10.WAV", 0.38 + 0.09*(3.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_ACK10].Init(&ClassAccess, NULL, NULL, "./media/sfx/ack10.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: ACK11.WAV", 0.38 + 0.09*(3.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_ACK11].Init(&ClassAccess, NULL, NULL, "./media/sfx/ack11.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: ACK12.WAV", 0.38 + 0.09*(3.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_ACK12].Init(&ClassAccess, NULL, NULL, "./media/sfx/ack12.wav", FALSE);
//STATUS
Disp_LoadProgress("SOUND: 2.MP3", 0.38 + 0.09*(3.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_M1].Init(&ClassAccess, NULL, NULL, "./media/sfx/2.mp3", FALSE);
//STATUS
Disp_LoadProgress("SOUND: 1.MP3", 0.38 + 0.09*(3.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_MENU].Init(&ClassAccess, NULL, NULL, "./media/sfx/1.mp3", FALSE);
//STATUS
Disp_LoadProgress("SOUND: CHIME.WAV", 0.38 + 0.09*(3.0/NUM_SOUNDSEGS));
	Sounds[SOUNDSEG_CHIME].Init(&ClassAccess, NULL, NULL, "./media/sfx/chime.wav", TRUE);

	//TEST
	Configuration.Load_Data("./config/additional_graphics.ini");
	Configuration.Load_Data("./config/additional_sounds.ini");

	FontInfo.SetFontColor(ColorTable.Green);


//STATUS
Disp_LoadProgress("INIT.: INTERFACE SYSTEM", 0.45);
	GInterface.Init(&ClassAccess);
	GInterface.Init_Console();

//STATUS
Disp_LoadProgress("INIT.: CHAT BUFFER", 0.5);
	ChatBuffer.Init(&ClassAccess, TEMP_CB_TICKCOUNT);

//STATUS
Disp_LoadProgress("INIT.: COMMAND QUEUE", 0.51);
	CommQueue.Init(&ClassAccess);
//STATUS
Disp_LoadProgress("INIT.: TIMER", 0.54);
	TimerControl.Init(30000, &ClassAccess);
	TimerControl.Set_CPT_FPS(60);  //set the effective number of ticks per second rate
	//I have to do this externally because I apparently can't control a thread from inside a class
			//the purpose of this thread is simply to repeatedly keep pinging the timer
		//it's thread affinity will be set to a single core, so that the timing values will be consistent
	    TimerControl.bTimerThread = Configuration.bTempStore_TimerThread;
		HANDLE hTimerThread;
		DWORD dwTimerThreadId;
		//create timer thread
	    bExitTimerThread = FALSE;
		if (TimerControl.bTimerThread == TRUE)
		{
			hTimerThread = CreateThread( 
				NULL,                        // default security attributes 
				0,                           // use default stack size  
				TimerThreadFunc,                  // thread function 
				NULL,						 // argument to thread function 
				0,                           // use default creation flags 
				&dwTimerThreadId);                // returns the thread identifier 
			SetThreadPriority(hTimerThread, THREAD_PRIORITY_HIGHEST);
			//setting thread affinity
			//set to run on processor 0 (since that is the only one we're guaranteed to have)
			SetThreadAffinityMask(hTimerThread, 1L);


		}
//STATUS
Disp_LoadProgress("INIT.: GAME OPTS", 0.55);
	GameOptions.Init(&ClassAccess, 0, 12);

//STATUS
Disp_LoadProgress("INIT.: MAIN MENU", 0.56);
	GInterface.Init_MainMenu();

//STATUS
Disp_LoadProgress("INIT.: DIRECTPLAY", 0.6);
	NetDP.Init(&ClassAccess);
//STATUS
Disp_LoadProgress("INIT.: STRINGS", 0.61);
	Strings.Init(&ClassAccess);
//STATUS
Disp_LoadProgress("INIT.: RAND NUM GEN", 0.65);
	RandGen.Init(NULL, &ClassAccess);

//init structure types

//STATUS
Disp_LoadProgress("INIT.: STRUCT CONTROL", 0.69);
	StructureTypes.Init(&ClassAccess);
	StructList.Init(&ClassAccess);
	StructIDTable.Init(&ClassAccess);


//init component types	
//STATUS
Disp_LoadProgress("INIT.: CMP CONTROL", 0.74);
	ComponentTypes.Init(&ClassAccess);
	CmpMasterIDTable.Init(&ClassAccess);


//init projectile types
//STATUS
Disp_LoadProgress("INIT.: PRJ CONTROL", 0.78);
	ProjTypeTable.Init(&ClassAccess);
		Configuration.Load_Data("./config/additional_projectile_types.ini");
	ProjList.Init(&ClassAccess);

//init material types
//STATUS
Disp_LoadProgress("INIT.: MATERIAL CONTROL", 0.78);
		MaterialTypes.Init(&ClassAccess);

//init unit types
//STATUS
Disp_LoadProgress("INIT.: UNIT CONTROL", 0.85);
	UnitTypeTable.Init(&ClassAccess);
	UnitList.Init(&ClassAccess);
	UnitIDTable.Init();



//init IPB(05.20.2003)
//STATUS
Disp_LoadProgress("INIT.: IPBUFFER", 0.91);
	InputPackets.Init(&ClassAccess);
	//initialize it's log file separately
if (DEBUG + DEBUG_IPB >= 1)
	InputPackets.Init_LogFile();

//STATUS

//STATUS
Disp_LoadProgress("DIAG: CMP CONTROL", 0.95);
	//do a quick diagnostic
	ComponentTypes.Diag_Output();
//STATUS
Disp_LoadProgress("TEST: UNIT DATA", 0.98);
	Test_Init_UnitStructCmp_Data();

Disp_LoadProgress("ACKTABLE", 0.981);
//acknowledgements
	Acknowledgements.Init(&ClassAccess);

//RandTest();
	Configuration.Load_Data("./config/Acknowledgements.ini");


//STATUS
Disp_LoadProgress("READY.", 1.00);
//play music test
AudioDA.Play_Secondary(SOUNDSEG_MENU);

//DEBUG (removed if not _DEBUG)
	//_ASSERTE( _CrtCheckMemory() );

//switch out splashgraphics
Swap_SplashGraphics("./media/gfx/BG1.bmp");


AppData.bRunStage = APP_RUNSTAGE_PRESTART;

//TEST: play midi intro music
//AudioDA.Play_Primary(SOUNDSEG_M1);

int framecount = 0;
int temp_alphaset = 9;

//TEST (test directory listing functions)
/*
WIN32_FIND_DATA DirInfo;
ZeroMemory(&DirInfo, sizeof(WIN32_FIND_DATA));
HANDLE hRet = FindFirstFile("*.txt", &DirInfo);
Write_Log(DirInfo.cFileName);
WriteLog_Enter();

delete( DirInfo.cFileName );
ZeroMemory(&DirInfo, sizeof(WIN32_FIND_DATA));

	while ( FindNextFile(hRet, &DirInfo) == TRUE)
	{
		Write_Log(DirInfo.cFileName);
		WriteLog_Enter();

		delete( DirInfo.cFileName );
		ZeroMemory(&DirInfo, sizeof(WIN32_FIND_DATA));
	}

FindClose(hRet);
*/

		//prime both surfaces
		DisplayDD.Colorfill2(DisplayDD.pPrimaryRenderTarget, 0);
		DisplayDD.Colorfill2(DisplayDD.pSecondaryRenderTarget, 0);

		//keep acquiring for safety
		InputDI.pDID8_Keyboard->Acquire();
		InputDI.pDID8_Mouse->Acquire();

AppData.iAIToken = 0; //controls which AI currently has the ability to process


int iframecount = 0;
//general program loop
while (AppData.bRunFlag == TRUE)
{

	//perform 
	if (AppData.bRunStage == APP_RUNSTAGE_PRESTART)
	{

		    
			bRet = PeekMessage(&AppData.WinMsg, NULL, 0, 0, PM_REMOVE);
			while (bRet != 0)
			{
				TranslateMessage(&AppData.WinMsg);
				DispatchMessage(&AppData.WinMsg);
				bRet = PeekMessage(&AppData.WinMsg, NULL, 0, 0, PM_REMOVE);
			} 

		iframecount++;
		//test music
		if (AudioDA.bIsPlaying(SOUNDSEG_MENU) == FALSE)
			AudioDA.Play_Secondary(SOUNDSEG_MENU);


Write_Log("Colorfill");
WriteLog_Enter();
		DisplayDD.Colorfill2(DisplayDD.pSecondaryRenderTarget, 0);
		//blit the BG
		DisplayDD.pSecondaryRenderTarget->Blt(&SplashRect, Images[ISURF_SPLASH].pDDSImage, &SplashSrcRect, 0, NULL);

Write_Log("Display Menu");
WriteLog_Enter();
		GDisplay.Disp_MainMenu(DisplayDD.pSecondaryRenderTarget, temp_alphaset);
		//for now, just display the chattext at the top of the screen
		//GDisplay.Disp_ChatText(DisplayDD.pDDSBack, 300, 0, 5); //disabled
Write_Log("Display Cursor");
WriteLog_Enter();
		GDisplay.Disp_Cursor(DisplayDD.pSecondaryRenderTarget);

				//TEST! when rendering with DDRM_SYSMEM, copy to screen at the very last moment
				if (DisplayDD.iDDRenderMode == DDRM_SYSMEM)
				{
					//DisplayDD.pDDSBack->BltFast(0, 0, DisplayDD.pSecondaryRenderTarget, NULL, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
Write_Log("Sysmem Blt to Main");
WriteLog_Enter();
					DisplayDD.pDDSMain->BltFast(0, 0, DisplayDD.pSecondaryRenderTarget, NULL, DDBLTFAST_WAIT);
				}

			if (DisplayDD.iDDRenderMode != DDRM_SYSMEM)
			{
Write_Log("Flip");
WriteLog_Enter();
				DisplayDD.pDDSMain->Flip(NULL, DDFLIP_NOVSYNC); //can unleash the frame rate(as of 01.03.2002 Thinkpad T22 pulled about 300 FPS), but didn't look too good
			}

Write_Log("Input");
WriteLog_Enter();
		//input functions
		InputDI.Get_InputState();
		GInput.Update_IGStates();

		//Network functions
		if (NetDP.bUPDRequired == TRUE)
		{
Write_Log("NetDP update connection state");
WriteLog_Enter();
			NetDP.Update_ConnectionState();
			NetDP.bUPDRequired = FALSE;
		}
		if (NetDP.bSetPeerInfoRequired == TRUE)
		{
Write_Log("Set Peer Info");
WriteLog_Enter();
			NetDP.pDP8->SetPeerInfo(&GameOptions.InfoPlayer_Local, NULL, NULL, DPNSETPEERINFO_SYNC);
			NetDP.bSetPeerInfoRequired = FALSE;
		}
		//HACK
		if (iframecount % 100 == 0)
		if (GInterface.IBoxes[IBOX_MAINMENU].iActiveGrid == TEMP_IGRID_MULTI3)
		{
			//if you have the map, rebroadcast that you have it
			if (GameOptions.bMapLocationFinalized == TRUE)
			{
				CommQueue.Issue_Command_Q2(CQ2TYPE_INDICATE_READY, (BYTE*)&GameOptions.DPNID_Local, sizeof(DPNID), TRUE);
				NetDP.Send_Command2(CQ2TYPE_INDICATE_READY, (BYTE*)&GameOptions.DPNID_Local, sizeof(DPNID), FALSE, TRUE);
			}
		}

		//pCA->pInterface->Update_MainMenu(TEMP_IGRID_SINGLE);
Write_Log("Update MainMenu");
WriteLog_Enter();
		GInterface.Update_MainMenu(GInterface.IBoxes[IBOX_MAINMENU].iActiveGrid);
		//interface processing
Write_Log("Process MainMenu");
WriteLog_Enter();

		GInterface.Process_MainMenu();
Write_Log("Process ChatBuffer");
WriteLog_Enter();
		ChatBuffer.Process_ChatBuffer();
Write_Log("Execute Commands");
WriteLog_Enter();
		CommQueue.Execute_Commands();
Write_Log("Execute Commands Q2");
WriteLog_Enter();
		CommQueue.Execute_Commands_Q2();
Write_Log("Process Static Buffer");
WriteLog_Enter();
		NetDP.Process_StaticBuffer();


		//exit control (disabled for now 05.16.2004)
		if (GInput.IGState_Keyboard[DIK_ESCAPE] == 2)
			AppData.bRunStage = APP_RUNSTAGE_EXIT;
	}

	//perform main cycle initializations
	if (AppData.bRunStage == APP_RUNSTAGE_MAININIT)
	{
		//stop main menu music

			AudioDA.Stop_Media(SOUNDSEG_MENU);
			AudioDA.Play_Secondary(SOUNDSEG_M1);

		//nations and objects
		Nations.MainInit(&ClassAccess, TEMP_DP_MAXPLAYERS);
		
		
		//if host, assign control IDs and synch random generators
		if (GameOptions.bAmHost == TRUE) 
		{
			NetDP.AssignIDs_Host();
			NetDP.Send_RandKeys(RandGen.RandKeys, 394);
		}

		//clear chatbuffer
		ChatBuffer.Clear_List();

		//do not start until:
		// 1. Randkeys have been received
		// 2. National Inits have been received for all nations playing
		// 3. Timer Inits have been received for 

		if (AppData.bRunFlag == 1 && GameOptions.bGameType == GAMETYPE_MULTI)
		{
			while (NetDP.bRandKeysReceived == FALSE || 
				   NetDP.bTotalPlayerCountReceived == FALSE || 
				   NetDP.bTotalTimerIDCountReceived == FALSE || 
				   NetDP.iNumTimerIDInitsReceived != NetDP.iTotalTimerIDCount)
			{
	
				NotifyBox("Waiting to receive data to start....", DisplayDD.pDDSMain);
				Write_Log("Waiting to receive data to start....");
				WriteLog_Enter();

				InputDI.Get_InputState();
				GInput.Update_IGStates();
				CommQueue.Execute_Commands();
				CommQueue.Execute_Commands_Q2();
				NetDP.Process_StaticBuffer();

				if (GInput.IGState_Keyboard[DIK_ESCAPE] != 0)
				{	
					AppData.bRunFlag = 0;
					AppData.bRunStage = APP_RUNSTAGE_EXIT;
					break;
				}
			}
		}

//DISABLED (nation inits now down in map file)
//		if (GameOptions.bGameType == GAMETYPE_SINGLE)
//		{
//			Nations.Init_Nation(0, "BLUE Nation", 11, FALSE, TRUE);	
//			Nations.Init_Nation(1, "RED Nation", 10, FALSE, TRUE);
//			Nations.Init_Nation(2, "PURPLE Nation", 13, FALSE, TRUE);
//			Nations.Init_Nation(3, "YELLOW Nation", 13, FALSE, TRUE);	
//		}

		//map
		SectorTable.Init(&ClassAccess); //the sector table could have also fallen under the jurisdiction of GMap(oh well) 05.28.2004
										//sector table needs to initialize before GMap because GMap will attempt to register sectors
		//pathfinder
		PathFinder.Init(&ClassAccess, 5);


		if (GameOptions.bGameType == GAMETYPE_MULTI)
		{
			Configuration.Load_Data(GameOptions.strMultiplayerMapPath);
			GMap.Init(GameOptions.strMultiplayerMapPath, &ClassAccess);
			//if multiplayer, disable tutorial option
			GameOptions.bTutorial = FALSE;
		} else
			{
				//disable tutorial by default unless enabled by the map
				GameOptions.bTutorial = FALSE;

				Configuration.Load_Data(AppData.achMapNames[GameOptions.iMapIndex]); //load configuration data on first pass (09.17.2004)
				GMap.Init(AppData.achMapNames[GameOptions.iMapIndex], &ClassAccess);
				
			}
		PathFinder.PostInit();
		//do partitioning
		PathFinder.Agg_Partition_Sectors();
		//Test_Init_Objects(); //now rely on maps to provide unit positions
		//rebuild link tables
		GMap.RebuildMTLinkLists();



		Minimap.Init(&ClassAccess);
		Minimap.Process_Minimap(TRUE);

		int c6 = 0;

		//for debug...
		QueryPerformanceCounter((LARGE_INTEGER *)&TimerControl.lStartTime);

		//kill keyedit
		GInterface.pKeyEdit = NULL;
		GInterface.pKeyEdit_Wide = NULL;

		//spawn "flipper" thread
		DWORD dwThreadId, dwThrdParam = 1; 
		HANDLE hThread; 
		char szMsg[80];
		if (DTHREAD_MODE == 1 )
		{
		hThread = CreateThread( 
			NULL,                        // default security attributes 
			0,                           // use default stack size  
			ThreadFunc,                  // thread function 
			NULL,						 // argument to thread function 
			0,                           // use default creation flags 
			&dwThreadId);                // returns the thread identifier 
		}
		

		//prepare the tutorial triggers and build display boxes
		GInterface.Init_Tutorial();

		//hide the main menu box, show the briefing box
		GInterface.Init_Briefing();
		GInterface.IBoxes[IBOX_MAINMENU].iActiveGrid = TEMP_IGRID_BRIEFING;

		

//switch out splashgraphics
Swap_SplashGraphics("./media/gfx/BG2.bmp");

		//test point to point
		//GMap.Link_Point_to_Point(1, 2020);


		AppData.bRunStage = APP_RUNSTAGE_BRIEFING;

	}
	if (AppData.bRunStage == APP_RUNSTAGE_BRIEFING)
	{
//DEBUG
Write_Log("Start BRIEFING loop");
WriteLog_Enter();

		iframecount++;
		//test music
		if (AudioDA.bIsPlaying(SOUNDSEG_M1))
			AudioDA.Play_Secondary(SOUNDSEG_M1);

		//HACK I guess
		//if we are in multiplay, broadcast a message identifying your icontrol
		if (GameOptions.bGameType == GAMETYPE_MULTI)
		{
			if (iframecount % 100 == 0)
			{
				int iretsize = 0;
				BYTE* pData = CommQueue.Assemble_CommData_Notify_iControl((DWORD)GameOptions.DPNID_Local, GameOptions.iControl, &iretsize);
				NetDP.Send_Command2(CQ2TYPE_NOTIFY_ICONTROL, pData, iretsize, FALSE, TRUE);
				//then issue it to yourself
				CommQueue.Issue_Command_Q2(CQ2TYPE_NOTIFY_ICONTROL, pData, iretsize, FALSE);
			}
		}

Write_Log("Colorfill");
WriteLog_Enter();
		DisplayDD.Colorfill2(DisplayDD.pSecondaryRenderTarget, 0);
		//blit the briefing screen
		DisplayDD.pSecondaryRenderTarget->Blt(&SplashRect, Images[ISURF_SPLASH].pDDSImage, &SplashSrcRect, 0, NULL);

Write_Log("Display Menu");
WriteLog_Enter();
		GDisplay.Disp_MainMenu(DisplayDD.pSecondaryRenderTarget, temp_alphaset);
		//for now, just display the chattext at the top of the screen
		//GDisplay.Disp_ChatText(DisplayDD.pDDSBack, 300, 0, 5); //disabled
Write_Log("Display Cursor");
WriteLog_Enter();


		GDisplay.Disp_Cursor(DisplayDD.pSecondaryRenderTarget);
		

				//TEST! when rendering with DDRM_SYSMEM, copy to screen at the very last moment
				if (DisplayDD.iDDRenderMode == DDRM_SYSMEM)
				{
					//DisplayDD.pDDSBack->BltFast(0, 0, DisplayDD.pSecondaryRenderTarget, NULL, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
Write_Log("Sysmem Blt to Main");
WriteLog_Enter();
					DisplayDD.pDDSMain->BltFast(0, 0, DisplayDD.pSecondaryRenderTarget, NULL, DDBLTFAST_WAIT);
				}

			if (DisplayDD.iDDRenderMode != DDRM_SYSMEM)
			{
Write_Log("Flip");
WriteLog_Enter();
				DisplayDD.pDDSMain->Flip(NULL, DDFLIP_NOVSYNC); //can unleash the frame rate(as of 01.03.2002 Thinkpad T22 pulled about 300 FPS), but didn't look too good
			}

Write_Log("Input");
WriteLog_Enter();
		//input functions
		InputDI.Get_InputState();
		GInput.Update_IGStates();

/*DISABLED FOR NOW (not necessary)
		//Network functions
//		if (NetDP.bUPDRequired == TRUE)
//		{
//Write_Log("NetDP update connection state");
//WriteLog_Enter();
			NetDP.Update_ConnectionState();
			NetDP.bUPDRequired = FALSE;
		}
		if (NetDP.bSetPeerInfoRequired == TRUE)
		{
Write_Log("Set Peer Info");
WriteLog_Enter();
			NetDP.pDP8->SetPeerInfo(&GameOptions.InfoPlayer_Local, NULL, NULL, DPNSETPEERINFO_SYNC);
			NetDP.bSetPeerInfoRequired = FALSE;
		}
		//pCA->pInterface->Update_MainMenu(TEMP_IGRID_SINGLE);
*/
Write_Log("Update MainMenu");
WriteLog_Enter();
		GInterface.Update_MainMenu(TEMP_IGRID_BRIEFING);



Write_Log("Process ChatBuffer");
WriteLog_Enter();
		ChatBuffer.Process_ChatBuffer();
Write_Log("Execute Commands");
WriteLog_Enter();
		CommQueue.Execute_Commands();
Write_Log("Execute Commands Q2");
WriteLog_Enter();
		CommQueue.Execute_Commands_Q2();
Write_Log("Processing Static Buffer");
WriteLog_Enter();
		NetDP.Process_StaticBuffer();
		//interface processing
Write_Log("Process MainMenu");
WriteLog_Enter();
			GInterface.Process_MainMenu();
		//exit control (disabled for now 05.16.2004)
		if (GInput.IGState_Keyboard[DIK_ESCAPE] == 2)
			AppData.bRunStage = APP_RUNSTAGE_EXIT;

	}
	if (AppData.bRunStage == APP_RUNSTAGE_POSTBRIEFING)
	{
Write_Log("Postbriefing: Processing Minimap");
WriteLog_Enter();
		Minimap.Process_Minimap(FALSE);

Write_Log("Postbriefing: Enabling AIs");
WriteLog_Enter();

		//JUST after the briefing is over (the only point of this is to enable AI for non-human players
		for (int lc1 = 0; lc1 < TEMP_DP_MAXPLAYERS; lc1++)
		{
			//for now only do this if we're in single mode
			if (GameOptions.bGameType == GAMETYPE_SINGLE)
			if (lc1 != GameOptions.iControl)
			if (Nations.ppNations[lc1] != NULL)
			{
				Nations.ppNations[lc1]->bAIFlag = TRUE;
			}
		}

Write_Log("Postbriefing: Clearing SBs");
WriteLog_Enter();

		//clear SB
		GInterface.ReleaseUnits_SB_all(GInterface.pUSB_First, GInterface.pUSB_Last);
		GInterface.ReleaseStructs_SB_all(GInterface.pSSB_First, GInterface.pSSB_Last);

Write_Log("Postbriefing: Random housecleaning");
WriteLog_Enter();

		//set Unit Selection Buffer to first unit 
		int iID_First = Nations.ppNations[GameOptions.iControl]->AINew.Find_First_ID(GameOptions.iControl); 
		cUnitA* pUnit_First = NULL;

			GInterface.map_xview = 0;
			GInterface.map_yview = 0;
		if (iID_First >= 0)
		{
			pUnit_First = UnitIDTable.pUnitTable[iID_First];
			GInterface.AddUnits_SB_singleB(iID_First, GInterface.pUSB_First,  GInterface.pUSB_Last);
			GInterface.pConsole_Unit_Select = GInterface.pUSB_First->pNext;
			//move map over this position
			int iX = pUnit_First->Pos_sqcurr.x - (GDisplay.MDRAW_NUMCOLS / 2);
			int iY = pUnit_First->Pos_sqcurr.y - (GDisplay.MDRAW_NUMROWS / 2);

					if (iX < 0)
						iX = 0;
					if (iY < 0)
						iY = 0;
					if (iX >= (GMap.iWidth - GDisplay.MDRAW_NUMCOLS))
						iX = GMap.iWidth - GDisplay.MDRAW_NUMCOLS - 1;
					if (iY >= (GMap.iHeight - GDisplay.MDRAW_NUMROWS))
						iY = GMap.iHeight - GDisplay.MDRAW_NUMROWS - 1;

			GInterface.map_xview = iX;
			GInterface.map_yview = iY;
		}
		
		iWinCount = 0;

		//clear splash graphic to save memory
		Clear_SplashGraphics();

Write_Log("Postbriefing: Hooking Sector Creates");
WriteLog_Enter();

		//now that we know what players are playing, create Sector AI entities for AIs
		SectorTable.Hook_SectorCreates();

Write_Log("Postbriefing: STARTING");
WriteLog_Enter();


		AppData.bRunStage = APP_RUNSTAGE_MAINCYCLE;
	}
	//actually running the game
	if (AppData.bRunStage == APP_RUNSTAGE_MAINCYCLE)
	{
			TimerControl.StartCount(TIMER_EXECT);

		if (GInterface.Tutorial.iActiveElement >= 0)
		{
			MainLoop_Tutorial();
			TimerControl.iCurrExecCount = 0; //suppress the timer so that it doesn't keep counting while tutorial is on

		} else if (TimerControl.iExecMode == EXECMODE_GTICK)
		{
			MainLoop_GTick();
		} else if (TimerControl.iExecMode == EXECMODE_GDRAW)
			MainLoop_GDraw();

		TimerControl.EndCount(TIMER_EXECT);

		int iWin = Temp_Determine_Winner();

		if (iWin >= 0)
		if (iWinCount <= 0)
		{
			iWinCount = TimerControl.iCurrTick;
		}

			if (iWin >= 0)
			if ( (TimerControl.iCurrTick - iWinCount) > 60)
				End_Game();  //note, as written, command will actually get sent iLatency times but this is OK

//DEBUG
char NText[50];
	TimerControl.Write_Log("Counts : ");
	itoa(TimerControl.iCurrExecCount, NText, 10);
	TimerControl.Write_Log(NText);
	TimerControl.Write_Log("EndCounts : ");
	_i64toa(TimerControl.lEndCounts[0], NText, 10);
	TimerControl.Write_Log(NText);
	TimerControl.Write_Log(" Counter per Tick : ");
	itoa(TimerControl.iCountsPerTick, NText, 10);
	TimerControl.Write_Log(NText);
	TimerControl.WriteLog_Enter();

		TimerControl.Process_Counts();   //figure how long everything took at the end of the execution loop 	

	} 	

	if (AppData.bRunStage == APP_RUNSTAGE_ENDGAME)
	{
			MainLoop_GDraw();
	}
	if (AppData.bRunStage == APP_RUNSTAGE_MAINEXIT)
	{

		//reload mainmenu splash graphics
		Load_SplashGraphics("./media/gfx/BG1.bmp");

		GInterface.IBoxes[IBOX_NOTIFY].bActiveBox = FALSE;

		//stop music
			AudioDA.Stop_Media(SOUNDSEG_M1);
		if(GameOptions.bGameType == GAMETYPE_MULTI)
		{
			while (NetDP.iNumMessagesInBuffer > 0)
			{
					NetDP.Process_StaticBuffer();
			}
			//DEBUG
			ChatBuffer.Add_CBNode("Ending DP Session", 1000);
			Write_Log("Ending DP Session"); WriteLog_Enter();

			NetDP.CloseSession();

			while (NetDP.iNumMessagesInBuffer > 0)
			{
					NetDP.Process_StaticBuffer();
			}

			//DEBUG
			Write_Log("Ending DP Session"); WriteLog_Enter();

		}

//DEBUG
Write_Log("Destroying all Units"); WriteLog_Enter();
		UnitList.Destroy_AllUnits();
		UnitList.ID_Last_Assigned = -1;
//DEBUG
Write_Log("Unregistering Units"); WriteLog_Enter();
		UnitIDTable.Remove_All();
//DEBUG
Write_Log("Destroying Structures"); WriteLog_Enter();
		StructList.Destroy_AllStructures();
		StructList.iID_Last_Assigned = -1;
//DEBUG
Write_Log("Unregistering Structures"); WriteLog_Enter();
		StructIDTable.Unregister_All();
//DEBUG
Write_Log("Exiting Nations"); WriteLog_Enter();
		Nations.Exit();

//DEBUG
Write_Log("Exiting Map"); WriteLog_Enter();		
		GMap.Exit();
		SectorTable.Exit();
//DEBUG
Write_Log("Exiting Minimap"); WriteLog_Enter();	
		Minimap.Exit();
//DEBUG
Write_Log("Exiting Pathfinder"); WriteLog_Enter();	
		PathFinder.Exit();

//DEBUG
Write_Log("Resetting Interface Boxes"); WriteLog_Enter();	
		GInterface.IBoxes[IBOX_MAINMENU].bActiveBox = TRUE;
		GInterface.IBoxes[IBOX_PRIMARY].bActiveBox = FALSE;
		GInterface.IBoxes[IBOX_MINIMAP].bActiveBox = FALSE;
		
		GInterface.IBoxes[IBOX_MINIMAPZOOM].bActiveBox = FALSE;
		GInterface.IBoxes[IBOX_CONSOLE_EQUIP].bActiveBox = FALSE;
		GInterface.IBoxes[IBOX_CONSOLE_LIST].bActiveBox = FALSE;
		GInterface.IBoxes[IBOX_CONSOLE_COMMAND].bActiveBox = FALSE;
		GInterface.IBoxes[IBOX_CONSOLE_INFO].bActiveBox = FALSE;
		GInterface.IBoxes[IBOX_CONSOLE_DISPLAY].bActiveBox = FALSE;
		GInterface.IBoxes[IBOX_CONSOLE_CHAT].bActiveBox = FALSE;

		//unloading graphics above index 200
		for (int lc88 = 200; lc88 < NUM_MAX_ISURFACES; lc88++)
		if (Images[lc88].bInit_Run == TRUE)
		{
			Images[lc88].Exit();
		}

//DEBUG
Write_Log("MAINEXIT complete"); WriteLog_Enter();

		//reset interface active grid
		GInterface.IBoxes[IBOX_MAINMENU].iActiveGrid = 0;

		AppData.bRunStage = APP_RUNSTAGE_PRESTART;
	}

	if (AppData.bRunStage == APP_RUNSTAGE_EXIT)
		AppData.bRunFlag = FALSE;

}


//	_CrtDumpMemoryLeaks();
	//prog exits
//DEBUG
Write_Log("[MAIN]Exit Timer");
WriteLog_Enter();
	TimerControl.Exit();
	bExitTimerThread = TRUE;

//DEBUG
Write_Log("[MAIN]Exit InputPackets");
WriteLog_Enter();

	InputPackets.Exit_LogFile();
	InputPackets.Exit();

//DEBUG
Write_Log("[MAIN]Exit Animation Types");
WriteLog_Enter();
	Animations.Exit();

//DEBUG
Write_Log("[MAIN]Exit Event Buffer");
WriteLog_Enter();
	Events.Exit();

//DEBUG
Write_Log("[MAIN]Exit Component Types");
WriteLog_Enter();
	ComponentTypes.Exit();
//DEBUG
Write_Log("[MAIN]Exit CmpMasterIDTable");
WriteLog_Enter();
	CmpMasterIDTable.Exit();
	
//DEBUG
Write_Log("[MAIN]Exit Structure Types");
WriteLog_Enter();
	StructureTypes.Exit();
//DEBUG
Write_Log("[MAIN]Exit StructList");
WriteLog_Enter();
	StructList.Exit();
//DEBUG
Write_Log("[MAIN]Exit StructIDTable");
WriteLog_Enter();
	StructIDTable.Exit();
//DEBUG
Write_Log("[MAIN]Exit String Table");
WriteLog_Enter();
	Strings.Exit();
//DEBUG
Write_Log("[MAIN]Exit UnitList");
WriteLog_Enter();
	UnitList.Exit();

//DEBUG
Write_Log("[MAIN]Exit NotifBuffer");
WriteLog_Enter();
	Notifications.Exit();

//DEBUG
Write_Log("[MAIN]Exit GMap");
WriteLog_Enter();
	GMap.Exit();
//DEBUG
Write_Log("[MAIN]Exit RandGen");
WriteLog_Enter();
	RandGen.Exit();
//DEBUG
Write_Log("[MAIN]Exit CommQueue");
WriteLog_Enter();
	CommQueue.Exit();
//DEBUG
Write_Log("[MAIN]Exit GInterface");
WriteLog_Enter();
	GInterface.Exit();
//DEBUG
Write_Log("[MAIN]Exit ChatBuffer");
WriteLog_Enter();
	ChatBuffer.Exit();
//DEBUG
Write_Log("[MAIN]Exit Surfaces");
WriteLog_Enter();
	for (int lc88 = 0; lc88 < NUM_MAX_ISURFACES; lc88++)
	if (Images[lc88].bInit_Run == TRUE)
	{
		Images[lc88].Exit();
	}

//DEBUG
Write_Log("[MAIN]Exit Sounds");
WriteLog_Enter();

	Sounds[SOUNDSEG_FIRE_MSL].Exit();
	Sounds[SOUNDSEG_FIRE_120MM].Exit();
	Sounds[SOUNDSEG_FIRE_80MM].Exit();
	Sounds[SOUNDSEG_FIRE_NAVAL120].Exit();
	Sounds[SOUNDSEG_IMPACT1].Exit();
	Sounds[SOUNDSEG_IMPACT2].Exit();
	Sounds[SOUNDSEG_TEST1].Exit();
	Sounds[SOUNDSEG_TEST2].Exit();
	Sounds[SOUNDSEG_GUN1].Exit();
	Sounds[SOUNDSEG_M1].Exit();
	Sounds[SOUNDSEG_MENU].Exit();

//DEBUG
Write_Log("[MAIN]Exit FontInfo");
WriteLog_Enter();
	FontInfo.Exit();
//DEBUG
Write_Log("[MAIN]Exit DirectAudio");
WriteLog_Enter();
	AudioDA.Exit();
//DEBUG
Write_Log("[MAIN]Exit NetDP");
WriteLog_Enter();
	NetDP.Exit();
//DEBUG
Write_Log("[MAIN]Exit InputDI");
WriteLog_Enter();
	InputDI.Exit();
//DEBUG
Write_Log("[MAIN]Exit InputPackets");
WriteLog_Enter();
	DisplayDD.Exit();
//DEBUG
Write_Log("[MAIN]Exit APPDATA");
WriteLog_Enter();
	AppData.Exit();

	

	return 0;
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
		{
		case WM_KEYDOWN:
			switch(wParam)
				{
				case VK_ESCAPE:
				//	if (AppData.bRunStage == 
				//	AppData.bRunFlag = 0;
					ChatBuffer.Add_CBNode("Escape message received.", 60);
					break;
				//more JUST FOR FUN(02.01.2003)
				case VK_F1:
					GameOptions.iControl = 0;
					break;
				case VK_F2:
					GameOptions.iControl = 1;
					break;
				case VK_F3:
					GameOptions.iControl = 2;
					break;
				case VK_F4:
					GameOptions.iControl = 3;
					break;
				case VK_F12:
					Nations.ppNations[GameOptions.iControl]->ddResources += 100000;
				}
			break;
		case WM_CLOSE:
			AppData.bRunFlag = 0;
		break;
		}

	//don't allow wm close to kill the window (sort of a trick that prevents windows from performing its default action if a wm_close message is sent)
//	if (uMsg != WM_CLOSE)
	  return DefWindowProc(hwnd, uMsg, wParam, lParam);
//	else
//	  return DefWindowProc(hwnd, WM_KEYDOWN, wParam, lParam);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
LRESULT CALLBACK WindowProc2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void Path_Test(int iIndex_Start, int iIndex_Goal)
{
	
	int c1;
	char text01[50];
	int xdraw, ydraw;
	SQCOORDSET Pos;

	cPATHENTRY* pPE_Trav;
	cPATHENTRY* pPE_Trav2;

	//get a path from the two nodes

TimerControl.StartCount(TIMER_PATHFIND);	
	//pPE_Trav = PathFinder.Find_Path(0,iIndex_Start, iIndex_Goal, 500);
TimerControl.EndCount(TIMER_PATHFIND);
	pPE_Trav2 = pPE_Trav;

	//clear the map of previous map courses
	for (c1 = 0; c1 <= GMap.iHeight * GMap.iWidth; c1++)
		GMap.pMap[c1].iDrawFrame_Layer2 = 0;

	//look at the pPathState array and see what squares the pather checked
/*	for (c1 = 0; c1 <= GMap.iHeight * GMap.iWidth; c1++)
	{
		if (PathFinder.pPathState[c1] != 0)
		{
			Pos.index = c1;
			Pos.Calc_xy(GMap.iWidth);
			_gcvt(PathFinder.pPathState[c1]->fEvalSum, 5, text01);
			GDisplay.Draw_Text(DisplayDD.pDDSMain, text01, (Pos.x - GInterface.map_xview) * TILE_SIZE, (Pos.y - GInterface.map_yview) * TILE_SIZE);
		}
			
		//GMap.pMap[c1].iDrawFrame_Layer2 = 4;
	}  */
	
	//now go through the list and change layer2 entries on the map as it finds them
	while (pPE_Trav2 != NULL)
	{
		//advance pPE_Trav2
		pPE_Trav2 = (cPATHENTRY*)pPE_Trav->pNext;

		//work on pPE_Trav
		GMap.pMap[pPE_Trav->Position.index].iDrawFrame_Layer2 = 16;

		//delete pPE_Trav
		delete(pPE_Trav);

		//set it to pPE_Trav2
		pPE_Trav = pPE_Trav2;

	}  

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
HRESULT WINAPI DP8_Handle_Message(PVOID pvUserContext,
											DWORD dwMessageID,
											PVOID pMessage
											)
{




	NetDP.iNumCallBackThreadsActive++;

	//DEBUG
	char NText[50];

	itoa(NetDP.iNumCallBackThreadsActive, NText, 10);
	Write_Log("DP Callback Initiated. Number of threads active: ");
	Write_Log(NText);
	WriteLog_Enter();
/*
	char Buffer[100];
	ZeroMemory(Buffer, sizeof(char)*100);
	strcat(Buffer, "DPN Callback: ");
	ultoa(dwMessageID, NText, 10);
	if (dwMessageID == DPN_MSGID_RECEIVE)
		strcat(NText, "DPN_MSGID_RECEIVE");
	strcat(Buffer, NText);
	ChatBuffer.Add_CBNode(Buffer, 60);
*/

//wait for ready
/*
while(NetDP.bCallBackInUse == TRUE)
{
	Write_Log("Waiting for DP Queue to free....");
	WriteLog_Enter();
}
NetDP.bCallBackInUse = TRUE;
*/

	HRESULT hr20 = S_OK;
		const char* KText;
			

	

	if (dwMessageID == DPN_MSGID_CREATE_PLAYER)
	{
		DPNMSG_CREATE_PLAYER* pMsg = (DPNMSG_CREATE_PLAYER*)pMessage;
		
	}
	if (dwMessageID ==  DPN_MSGID_ENUM_HOSTS_RESPONSE)
	{

//enter critical section
		//EnterCriticalSection(&ClassAccess.pDP_Multiplay->csCriticalSection);

		
		DPNMSG_ENUM_HOSTS_RESPONSE* pMsg = (DPNMSG_ENUM_HOSTS_RESPONSE*)pMessage;

		int DuplicateHost = 0;

		ClassAccess.pDP_Multiplay->Write_Log("Session Name: ");		
		ClassAccess.pDP_Multiplay->Write_Log_Special(reinterpret_cast<unsigned short*>(pMsg->pApplicationDescription->pwszSessionName));
		ClassAccess.pDP_Multiplay->WriteLog_Enter();

		//do a search through the existing list of hosts found
		for (int lc7 = 0; lc7 < TEMP_DP_MAXHOSTS; lc7++)
		{
			if (ClassAccess.pDP_Multiplay->HostsFound[lc7].pAppDesc != NULL)
			if (ClassAccess.pDP_Multiplay->HostsFound[lc7].pAppDesc->guidApplication == pMsg->pApplicationDescription->guidApplication)
				DuplicateHost = 1;
		}
		if (DuplicateHost == 0)
		{

		//ClassAccess.pDP_Multiplay->HostsFound[ClassAccess.pDP_Multiplay->wHostsFound] = pMsg->pApplicationDescription;
		//deep copy this information instead of trying to set pointers, which obviously doesn't work
		ClassAccess.pDP_Multiplay->HostsFound[ClassAccess.pDP_Multiplay->wHostsFound].pAppDesc = new DPN_APPLICATION_DESC;
		ZeroMemory(ClassAccess.pDP_Multiplay->HostsFound[ClassAccess.pDP_Multiplay->wHostsFound].pAppDesc, sizeof(DPN_APPLICATION_DESC));
		memcpy(ClassAccess.pDP_Multiplay->HostsFound[ClassAccess.pDP_Multiplay->wHostsFound].pAppDesc, pMsg->pApplicationDescription, sizeof(DPN_APPLICATION_DESC));

		//duplicate device and address objects
		pMsg->pAddressDevice->Duplicate(&ClassAccess.pDP_Multiplay->HostsFound[ClassAccess.pDP_Multiplay->wHostsFound].pDeviceAddr);
		pMsg->pAddressSender->Duplicate(&ClassAccess.pDP_Multiplay->HostsFound[ClassAccess.pDP_Multiplay->wHostsFound].pHostAddr);
		

		
		ClassAccess.pDP_Multiplay->Write_Log("Added Session Name in index ");
		itoa(ClassAccess.pDP_Multiplay->wHostsFound, NText, 10);
		ClassAccess.pDP_Multiplay->Write_Log(NText);
		ClassAccess.pDP_Multiplay->Write_Log(": ");
		ClassAccess.pDP_Multiplay->Write_Log_Special(reinterpret_cast<unsigned short*>(ClassAccess.pDP_Multiplay->HostsFound[ClassAccess.pDP_Multiplay->wHostsFound].pAppDesc->pwszSessionName));
		ClassAccess.pDP_Multiplay->WriteLog_Enter();

			ClassAccess.pDP_Multiplay->wHostsFound++;
		}
	//leave critical section
	//LeaveCriticalSection(&ClassAccess.pDP_Multiplay->csCriticalSection);

	}

	if (dwMessageID == DPN_MSGID_INDICATE_CONNECT && ClassAccess.pGameOptions->bAmHost == TRUE)
	{
		
		DPNMSG_INDICATE_CONNECT* pMsg = (DPNMSG_INDICATE_CONNECT*)pMessage;
		
		//send random number keys, iControl number here
		//also increment highest active playerID here
	
	//disabled 12.16.2002
	//ClassAccess.pGDisplay->Write_Log("ExportKeys");
		//RandGen.ExportKeys(&NetDP.DPCRD.RandKeys);
		//NetDP.DPCRD.wNumPlayers = GameOptions.bHighestActivePlayerID + 1;
		
		//set the reply data of the message
		//pMsg->pvReplyData = (VOID*)&NetDP.DPCRD;
		//pMsg->dwReplyDataSize = sizeof(DPCONNECTREPLYDATA);   //I don't have to worry about deleteing anything because this is static memory

	}
	if (dwMessageID == DPN_MSGID_SEND_COMPLETE)	
	{

		DPNMSG_SEND_COMPLETE* pMsg = (DPNMSG_SEND_COMPLETE*)pMessage;
	
		//DEBUG
		//const char* KText = DXGetErrorString9(pMsg->hResultCode);
		//ChatBuffer.Add_CBNode((char*)KText, 60);
	}
	if (dwMessageID == DPN_MSGID_HOST_MIGRATE)
	{
		ClassAccess.pChatBuffer->Add_CBNode("Host Changed.", 1000);
		Write_Log("Host Changed.");
		WriteLog_Enter();

	}
	if (dwMessageID == DPN_MSGID_CONNECT_COMPLETE && ClassAccess.pGameOptions->bAmHost == FALSE)
	{
//enter critical section
		//EnterCriticalSection(&ClassAccess.pDP_Multiplay->csCriticalSection);

		//receive random number keys, iControl number here
		DPNMSG_CONNECT_COMPLETE* pMsg = (DPNMSG_CONNECT_COMPLETE*)pMessage;		
		DPCONNECTREPLYDATA* pReplyData = (DPCONNECTREPLYDATA*)pMsg->pvApplicationReplyData;
	
		//capture own DPNID
		GameOptions.DPNID_Local = pMsg->dpnidLocal;

		//capture own name
		GameOptions.strName[0] = 0;
		char* strName =  GInterface.Conv_String_WideToReg2(reinterpret_cast<unsigned short*>(GameOptions.InfoPlayer_Local.pwszName));
		strcat(GameOptions.strName, strName);
		delete [] strName;

		//say it
		char NText[20];
		ultoa(GameOptions.DPNID_Local, NText, 10);
		char Text[100];
		Text[0] = 0;
		strcat(Text, GameOptions.strName);
		strcat(Text, " has discovered own ID as: ");
		strcat(Text, NText);
		NetDP.Send_ChatText(Text, GameOptions.strName, 100);

		//ask for host's DPNID
		//I hope this works
		DPNID MyDPNID = GameOptions.DPNID_Local;
		NetDP.Send_Command2(CQ2TYPE_SEND_HOSTDPNID, (BYTE*)&MyDPNID, sizeof(DWORD), FALSE, TRUE);

		//say it
		Text[0] = 0;
		strcat(Text, "Host, what is your DPNID?");
		NetDP.Send_ChatText(Text, GameOptions.strName, 100); 

	//Key reloading in random generator disabled 12.16.2002	
	//ClassAccess.pGDisplay->Write_Log("ReloadKeys");
		//reload keys of the rand generator
	//	RandGen.ReloadKeys(&pReplyData->RandKeys);
		//set iControl
	//	GameOptions.iControl = pReplyData->wNumPlayers;
		//update the player info
	//	((PLAYERDATA*)GameOptions.InfoPlayer_Local.pvData)->iControl = GameOptions.iControl;
		//set peer info
		//HRESULT hr21; //= ClassAccess.pDP_Multiplay->pDP8->SetPeerInfo(&GameOptions.InfoPlayer_Local, NULL, NULL, DPNSETPEERINFO_SYNC);
		ClassAccess.pDP_Multiplay->bSetPeerInfoRequired = TRUE;

	//KText = DXGetErrorString9(hr21);
	//GDisplay.Write_Log("SetPeerInfo: ");
	//GDisplay.Write_Log((char*)KText);
	//GDisplay.WriteLog_Enter();

	//leave critical section
//	LeaveCriticalSection(&ClassAccess.pDP_Multiplay->csCriticalSection);


	}

	if (dwMessageID == DPN_MSGID_DESTROY_PLAYER)
	{
		ClassAccess.pDP_Multiplay->bUPDRequired = TRUE;
		DPNMSG_DESTROY_PLAYER* pMsg = (DPNMSG_DESTROY_PLAYER*)pMessage;		
		DWORD dwDPNID = (DWORD)pMsg->dpnidPlayer;
		//now find that DPNID
		for (int lc55 = 0; lc55 < TEMP_DP_MAXPLAYERS; lc55++)
		{
			if ( TimerControl.dwDPNIDs[lc55] == dwDPNID)
			{
				//disconnect this player immediately! (why(rather than issuing a command)? because otherwise
				//all systems command queues will hang as they wait for synchronization from the aborted player
				//(and the drop command won't execute).
				TimerControl.iTickReportIDCodes[lc55] = -1;
				//
				ChatBuffer.Add_CBNode("Player disconnected.", 200);
			}

		}
	}

	if (dwMessageID == DPN_MSGID_PEER_INFO)
	{

		//NetDP.Update_ConnectionState();   //let's try not calling DP functions inside DP callbacks (07.20.2002)
		ClassAccess.pDP_Multiplay->bUPDRequired = TRUE;
	}

	if (dwMessageID ==  DPN_MSGID_RECEIVE)  //VERY TENTATIVE AT THE MOMENT ... just to see if it works (06.30.2002)
											//nature of the send changed 07.22.2002
	{


	//EnterCriticalSection(&ClassAccess.pDP_Multiplay->csCriticalSection);
		
		//do not process until we are ready to receive commands
		if (AppData.bRunStage == APP_RUNSTAGE_MAINCYCLE || AppData.bRunStage == APP_RUNSTAGE_BRIEFING || AppData.bRunStage == APP_RUNSTAGE_POSTBRIEFING || AppData.bRunStage == APP_RUNSTAGE_PRESTART || AppData.bRunStage == APP_RUNSTAGE_MAININIT)
		{
		
			DPNMSG_RECEIVE* pMsg = (DPNMSG_RECEIVE*)pMessage;
			itoa((int)pMsg, NText, 10);
			ClassAccess.pDP_Multiplay->Write_Log(NText);

			//DEBUG (output incoming message sizes)
			itoa(pMsg->dwReceiveDataSize, NText, 10);
			ClassAccess.pDP_Multiplay->Write_Log("MESSAGE RECEIVED. Data Receive Size: ");
			ClassAccess.pDP_Multiplay->Write_Log(NText);
			ClassAccess.pDP_Multiplay->Write_Log(" Total Structure Size: ");
			itoa(pMsg->dwSize, NText, 10);
			if (pMsg->dwSize >  TEMP_DP_SMBMAXSIZE)
			{
				Write_Log("Warning! Incoming message may potentially be too large!");
				WriteLog_Enter();
			}
			ClassAccess.pDP_Multiplay->Write_Log(NText);
			ClassAccess.pDP_Multiplay->WriteLog_Enter();

			BYTE* pHeader = (BYTE*)pMsg->pReceiveData;
			PCMD* pCommand = (PCMD*)(pHeader+1);
			char* pText = (char*)(pHeader+1);
			LONG* pKeys = (LONG*)pText;
			WORD* pwTrav = (WORD*)(pHeader+1);
			BYTE* pData;
			BYTE* pBuffer;



			if (*pHeader == DPSENDTYPE_COMMAND)
				{
				//issue the command right here...at least for now (06.30.2002)
				ClassAccess.pCommQueue->Issue_Command(pCommand->wDest, 
												pCommand->wType,
												pCommand->wParam1,
												pCommand->wParam2,
												pCommand->wParam3,
												pCommand->fParam4,
												pCommand->fParam5,
												pCommand->pParam6,
												pCommand->lExecTick);

				}

			if (*pHeader == DPSENDTYPE_COMMAND2)
				{
/*
			//DEBUG
				ClassAccess.pDP_Multiplay->Write_Log("Command2 Received. Tick: ");
				itoa(TimerControl.iCurrTick, NText, 10);
				ClassAccess.pDP_Multiplay->Write_Log(NText);
				ClassAccess.pDP_Multiplay->WriteLog_Enter();
*/
				//gather the type and data size from the message data
				WORD wType = *pwTrav;

			//DEBUG
			//itoa(*pHeader, NText, 10);
			//ChatBuffer.Add_CBNode(NText, 60);

				pwTrav++;
				WORD wDataSize = *pwTrav;
/*
			itoa(*pwTrav, NText, 10);
			ClassAccess.pDP_Multiplay->Write_Log("Command Data Size: ");
			ClassAccess.pDP_Multiplay->Write_Log(NText);
			ClassAccess.pDP_Multiplay->WriteLog_Enter();
*/
				pwTrav++;
				pData = (BYTE*)pwTrav;

			//DEBUG
			//itoa(*pData, NText, 10);
	//		ChatBuffer.Add_CBNode(NText, 60);
	//		BYTE* pInc = pData + 1;
	//		DWORD* pDW = (DWORD*)pInc;
	///		ultoa(*pDW, NText, 10);
	//		ChatBuffer.Add_CBNode(NText, 60);
			

					if (AppData.bRunStage == APP_RUNSTAGE_MAINCYCLE || AppData.bRunStage == APP_RUNSTAGE_PRESTART  || AppData.bRunStage == APP_RUNSTAGE_MAININIT || AppData.bRunStage == APP_RUNSTAGE_BRIEFING || AppData.bRunStage == APP_RUNSTAGE_MAINEXIT)
					{

					
						int lc2 = 0; 
						//block execution if the buffer is in use or there are any messages in the buffer
						while ( NetDP.bStaticBufferInUse == TRUE || NetDP.iNumMessagesInBuffer >= TEMP_DP_STATICBUFFERSIZE)
						{
								if (GInput.IGState_Keyboard[DIK_ESCAPE] != 0)
								{
									AppData.bRunStage = APP_RUNSTAGE_EXIT;
									return S_OK;
								}

								lc2++;
								if (lc2 > 100000)
								{
									if (wType == CQ2TYPE_REPORTFRAME)
										ChatBuffer.Add_CBNode("DP Callback has been in loop for long time. Loop broken. Incoming Command: REPORTFRAME", 1000);
									if (wType == CQ2TYPE_MOVE_B)
										ChatBuffer.Add_CBNode("DP Callback has been in loop for long time. Loop broken. Incoming Command: MOVE_B", 1000);

									break;
								}
								//wait...
								Sleep(0);

						}
						//then set lock
						NetDP.bStaticBufferInUse = TRUE;

						//do buffer activity
						//copy message
						NetDP.StaticMsgSizeBuffer[NetDP.iNumMessagesInBuffer] = wDataSize;
						NetDP.StaticMsgTypeBuffer[NetDP.iNumMessagesInBuffer] = wType;
						memcpy(NetDP.StaticMsgBuffer + TEMP_DP_SMBMAXSIZE*NetDP.iNumMessagesInBuffer, pData, wDataSize);
						
						//increment count
						NetDP.iNumMessagesInBuffer++;
					
						//unlock
						NetDP.bStaticBufferInUse = FALSE;

					} else
					{
								ClassAccess.pCommQueue->Issue_Command_Q2(wType, pData, wDataSize, TRUE);

					}   
				}


			if (*pHeader == DPSENDTYPE_CHATTEXT)
				{


					//add to chat text buffer here (07.22.2002)
					ClassAccess.pChatBuffer->Add_CBNode(pText, 50000);
	
				}

			if (*pHeader == DPSENDTYPE_RANDKEYS)
				{
					//reload keys here
					ClassAccess.pRandGen->ReloadKeys(pKeys);
					for (int lc6 = 0; lc6 < 200; lc6++)
					{
						itoa(pKeys[lc6], NText, 10);
						ClassAccess.pGDisplay->Write_Log(NText);
						ClassAccess.pGDisplay->Write_Log("^^^");
					}
					NetDP.bRandKeysReceived = TRUE;
					//DEBUG
					NetDP.Write_Log("Rand Keys Received. ");
					NetDP.WriteLog_Enter();
			
				}
		}

	}

	ClassAccess.pDP_Multiplay->iMessagesReceived++;
	itoa(ClassAccess.pDP_Multiplay->iMessagesReceived, NText, 10);
	ClassAccess.pDP_Multiplay->Write_Log(NText);
	ClassAccess.pDP_Multiplay->WriteLog_Enter();
	


	NetDP.iNumCallBackThreadsActive--;

	//DEBUG
	Write_Log("DP8 Callback Ended. Current Number of DP threads active: ");
	itoa(NetDP.iNumCallBackThreadsActive, NText, 10);
	Write_Log(NText);
	WriteLog_Enter();

	return S_OK;



}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void RandTest()  //test of the random generator's capabilities(08.01.2002)
{
	char NText[50];

//spit out what was in all the keys
for (int lc10 = 0; lc10 < 100; lc10++)
{
	itoa(RandGen.RandKeys[lc10], NText, 10);
//	GDisplay.Write_Log(NText);
//	GDisplay.Write_Log(" ");	
}


for (int c8 = 0; c8 <= 100; c8++)
{
	itoa(RandGen.rand2(), NText, 10);
//	GDisplay.Write_Log(NText);
//	GDisplay.Write_Log(" ");
}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//TEST lock the display surface (and report the color value of one of the pixels)
//	DDSURFACEDESC2 ddsd3;
//	ddsd3.dwSize = sizeof(DDSURFACEDESC2);
//	DisplayDD.pDDSBack->Lock(NULL, &ddsd3, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
//	WORD* pw = (WORD*)ddsd3.lpSurface;
//	pw = pw + GInput.IGState_Mouse.iXPos + (GInput.IGState_Mouse.iYPos * (ddsd3.lPitch / 2));
//	int scolor = (int)*pw;
//	DisplayDD.pDDSBack->Unlock(NULL);    
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void Init_LogFile()
{
//create the debug log file --> overwrite any files with the same name
	hLogFile = CreateFile("./debuglogs/Log_MAIN.txt", 
		GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_READ, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void Write_Log(LPSTR Text)
{
if (DEBUG + DEBUG_MAIN >= 1)
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
void WriteLog_Enter()
{
if (DEBUG + DEBUG_MAIN >= 1)
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
void Exit_LogFile()
{
	//just realized that I need to close the logfiles when I'm done with them! (01.31.2002)
	CloseHandle(hLogFile);

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
 void Test_Init_UnitStructCmp_Data()
 {

//DEBUG
Write_Log("Setting up structure and structure types");
WriteLog_Enter();

//TEMP 03.28.2003 create some temporary structure types
StructureTypes.Add_StructType_Simple("Storage", TEMP_STYPE_TANK, 0,
											ISURF_STRUCT_TANK, 
											48, 51,
											0,
											-3,
											500,
											10,
											4,
										  0, 1,
										  1, 1,
										  1, 0,
										  0, 0, 
										  0, 0, 
										  0, 0, 
										  0, 0, 
										  0, 0, 
										  0, 0,
										  0,
										  0, 0,
										  0, 0, 
										  0, 0, 
										  0, 0, 
										  2, 
										  CQ2TYPE_TANK_SET_TYPE,
										  CQ2TYPE_TANK_FLUSH,
										  0, 
										  0, 
										  0, 
										  0, 
										  0,
										  0, 
										  0,
										  0,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  NULL,
										  TRUE,
										  TRUE);
StructureTypes.Add_StructType_Simple("Extractor", TEMP_STYPE_EXTRACTOR, 1000, ISURF_STRUCT_EXTRACTOR, 
										 48, 58,
										 0, -10,
										 1000,
										 0,
										 4,
										 1,1,
										 0,1,
										 1,0,
										 0,0,
										 0,0,
										 0,0,
										 0,0,
										 0,0,
										 0,0,
										 0,
										 0, 0,
										 0, 0,
										 0, 0,
										 0, 0, 
										 0,
										 0,
										 0,
										 0,
										 0,
										 0,
										 0,
										 0,
										 0,
										 0,
										 -1,
										 -1,
										 -1,
										 -1,
										 -1,
										 -1,
										 -1,
										 -1,
										 -1,
										 -1,
										 "Nothing",
										 FALSE,
										 TRUE);
StructureTypes.Add_StructType_Simple("Outpost", TEMP_STYPE_OUTPOST, 1000,
											ISURF_STRUCT_TEST01, 
											54, 48,
											-3,
											0,
											500,
											0,
											4,
										  1, 1,
										  0, 1,
										  1, 0,
										  0, 0, 
										  0, 0, 
										  0, 0, 
										  0, 0, 
										  0, 0, 
										  0, 0,
										  0,
										  0, 0,
										  0, 0, 
										  0, 0, 
										  0, 0, 
										  1, 
										  CQ2TYPE_START_BUILD_UNIT_PRECONFIG,
										  0,
										  0, 
										  0, 
										  0, 
										  0, 
										  0,
										  0, 
										  0,
										  0,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  NULL,
										  TRUE,
										  TRUE);

StructureTypes.Add_StructType_Simple("PowerPlant", TEMP_STYPE_REACTOR, 2000,
										  ISURF_STRUCT_REACTOR,
										  48,
										  57,
										  0,
										  -9,
										  1000,
										  0,
										  4,
										  1, 0,
										  0, 1,
										  1, 1,
										  0, 0,
										  0, 0,
										  0,0,
										  0,0,
										  0,0,
										  0, 0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  10000 + MATERIAL_ELEMENT_A,
										  10000 + MATERIAL_ELEMENT_B,
										  10000 + MATERIAL_ELEMENT_C,
										  10000 + MATERIAL_ELEMENT_F,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  "Eng",
										  TRUE,
										  TRUE);
StructureTypes.Add_StructType_Simple("Refinery", TEMP_STYPE_REFINERY, 2000,
										  ISURF_STRUCT_REFINERY,
										  75,
										  76,
										  -TILE_SIZE - 2,
										  -TILE_SIZE - 4,
										  1000,
										  0,
										  9,
										  -1, -1,
										  0, -1,
										  1, -1,
										  -1, 0,
										  1, 0,
										  -1,1,
										  0,1,
										  1,1,
										  0, 0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  10000 + MATERIAL_ELEMENT_A,
										  10000 + MATERIAL_ELEMENT_B,
										  10000 + MATERIAL_ELEMENT_C,
										  10000 + MATERIAL_ELEMENT_F,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  "Plastics",
										  TRUE,
										  TRUE);
StructureTypes.Add_StructType_Simple("Shipyard", TEMP_STYPE_SHIPYARD, 2000,
										  ISURF_STRUCT_SHIPYARD,
										  72,
										  72,
										  -TILE_SIZE,
										  -TILE_SIZE,
										  1000,
										  0,
										  9,
										  -1, -1,
										  0, -1,
										  1, -1,
										  -1, 0,
										  1, 0,
										  -1,1,
										  0,1,
										  1,1,
										  0, 0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  1,
										  CQ2TYPE_START_BUILD_UNIT_PRECONFIG,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  10000 + MATERIAL_ELEMENT_C,
										  10000 + MATERIAL_ELEMENT_I,
										  10000 + MATERIAL_ELEMENT_S,
										  10000 + MATERIAL_ELEMENT_T,
										  PERMISSION_SEA1,
										  10000 + MATERIAL_ELEMENT_A,
										  10000 + MATERIAL_ELEMENT_P,
										  10000 + MATERIAL_ELEMENT_E,
										  -1,
										  NULL,
										  TRUE,
										  TRUE);
StructureTypes.Add_StructType_Simple("Forge", TEMP_STYPE_FORGE, 2000,
										  ISURF_STRUCT_FORGE,
										  72,
										  75,
										  -TILE_SIZE,
										  -TILE_SIZE - 3,
										  1000,
										  0,
										  9,
										  -1, -1,
										  0, -1,
										  1, -1,
										  -1, 0,
										  1, 0,
										  -1,1,
										  0,1,
										  1,1,
										  0, 0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  10000 + MATERIAL_ELEMENT_C,
										  10000 + MATERIAL_ELEMENT_I,
										  10000 + MATERIAL_ELEMENT_S,
										  10000 + MATERIAL_ELEMENT_P,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  "Steel",
										  TRUE,
										  TRUE);
StructureTypes.Add_StructType_Simple("Factory", TEMP_STYPE_FACTORY_ABC, 2000,
										  ISURF_STRUCT_FACTORY_ABC,
										  74,
										  72,
										  -TILE_SIZE - 1,
										  -TILE_SIZE,
										  1000,
										  0,
										  9,
										  -1, -1,
										  0, -1,
										  1, -1,
										  -1, 0,
										  1, 0,
										  -1,1,
										  0,1,
										  1,1,
										  0, 0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  1,
										  CQ2TYPE_START_BUILD_UNIT_PRECONFIG,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  0,
										  10000 + MATERIAL_ELEMENT_C,
										  10000 + MATERIAL_ELEMENT_I,
										  10000 + MATERIAL_ELEMENT_S,
										  10000 + MATERIAL_ELEMENT_P,
										  10000 + MATERIAL_ELEMENT_A,
										  10000 + MATERIAL_ELEMENT_T,
										  PERMISSION_LAND1,
										  PERMISSION_AIR1,
										  -1,
										  NULL,
										  TRUE,
										  TRUE);
	StructureTypes.Add_StructType_Simple( "CAPITAL", TEMP_STYPE_RSC, 20000,
										   ISURF_STRUCT_RSC01, 
										   140,
										   75,
										   -TILE_SIZE,
										   -TILE_SIZE,
										   2500,
										   0,
										   13,
										   1,-1,
										   1,0,
										   1,1,
										   0,-1,
										   0,0,
										   0,1,
										   -1,-1,
										   -1,0,
										   -1,1,
										   0,
										   0,0,
										   0,0,
										   0,0,
										   0,0,
										   2,
										   CQ2TYPE_START_BUILD_UNIT_PRECONFIG,
										   CQ2TYPE_LAY_LINK,
										   0,
										   0,
										   0,
										   0,
										   0,
										   0,
										   0,
										   0,
										  10000 + MATERIAL_ELEMENT_C,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  -1,
										  NULL,
										   TRUE,
										   TRUE);
	//add the other three tile slots
	StructureTypes.pTable[TEMP_STYPE_RSC]->Pos_offset_array[11].x = 4;
	StructureTypes.pTable[TEMP_STYPE_RSC]->Pos_offset_array[11].y = 0;
	StructureTypes.pTable[TEMP_STYPE_RSC]->Pos_offset_array[10].x = 3;
	StructureTypes.pTable[TEMP_STYPE_RSC]->Pos_offset_array[10].y = 0;
	StructureTypes.pTable[TEMP_STYPE_RSC]->Pos_offset_array[9].x = 2;
	StructureTypes.pTable[TEMP_STYPE_RSC]->Pos_offset_array[9].y = 0;


	StructureTypes.Reconstruct();

//add material costs
	//StructureTypes.pTable[TEMP_STYPE_TANK]->MaterialCost.Materials[MATERIAL_ELEMENT_B] = 1;
	StructureTypes.pTable[TEMP_STYPE_RSC]->MaterialCost.Materials[MATERIAL_ELEMENT_S] = 1;
	StructureTypes.pTable[TEMP_STYPE_RSC]->MaterialCost.Materials[MATERIAL_ELEMENT_L] = 1;

	StructureTypes.pTable[TEMP_STYPE_FORGE]->MaterialCost.Materials[MATERIAL_ELEMENT_I] = 1;
	StructureTypes.pTable[TEMP_STYPE_REFINERY]->MaterialCost.Materials[MATERIAL_ELEMENT_S] = 1;
	StructureTypes.pTable[TEMP_STYPE_SHIPYARD]->MaterialCost.Materials[MATERIAL_ELEMENT_S] = 1;
	StructureTypes.pTable[TEMP_STYPE_FACTORY_ABC]->MaterialCost.Materials[MATERIAL_ELEMENT_S] = 1;
	StructureTypes.pTable[TEMP_STYPE_OUTPOST]->MaterialCost.Materials[MATERIAL_ELEMENT_S] = 1;


//DEBUG
Write_Log("Setting up component types...");
WriteLog_Enter();


int iX, iY, c980;
//TEMP 03.21.2003 create some temporary components types

//

//DEBUG
Write_Log("Loading additional components");
WriteLog_Enter();

	Configuration.Load_Data("./config/additional_component_types.ini");

//DEBUG
Write_Log("Reconstructing");
WriteLog_Enter();


	ComponentTypes.Reconstruct();
	
//DEBUG
Write_Log("12.");
WriteLog_Enter();

//DEBUG
Write_Log("Setting up unit and unit types...");
WriteLog_Enter();


	Configuration.Load_Data("./config/additional_unit_types.ini");

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void Disp_LoadProgress(LPSTR InText, double Percent)
{
//STATUS
RECT DestRect;
DestRect.left = SplashRect.left + (SplashRect.right - SplashRect.left) * 0.305;
DestRect.top = SplashRect.top + (SplashRect.bottom - SplashRect.top) * 0.74;
DestRect.bottom = SplashRect.top + (SplashRect.bottom - SplashRect.top) * 0.76;


int iBarMaxWidth = (SplashRect.right - SplashRect.left) * 0.7 - DestRect.left;
RECT BlotRect;
BlotRect.top = SplashRect.top + (SplashRect.bottom - SplashRect.top) * 0.58;
BlotRect.bottom = SplashRect.top + (SplashRect.bottom - SplashRect.top) * 0.61;
BlotRect.left = SplashRect.left + (SplashRect.right - SplashRect.left) * 0.35;
BlotRect.right = SplashRect.left + (SplashRect.right - SplashRect.left) * 0.7;

RECT BlotSrcRect;
BlotSrcRect.top = SplashSrcRect.top + (SplashSrcRect.bottom - SplashSrcRect.top) * 0.58;
BlotSrcRect.bottom = SplashSrcRect.top + (SplashSrcRect.bottom - SplashSrcRect.top) * 0.61;
BlotSrcRect.left = SplashSrcRect.left + (SplashSrcRect.right - SplashSrcRect.left) * 0.35;
BlotSrcRect.right = SplashSrcRect.left + (SplashSrcRect.right - SplashSrcRect.left) * 0.7;

DDBLTFX BlotFX;
ZeroMemory(&BlotFX, sizeof(DDBLTFX));
BlotFX.dwSize = sizeof(DDBLTFX);
BlotFX.dwFillColor = ColorTable.Black;
DDBLTFX LBlitFX;
ZeroMemory(&LBlitFX, sizeof(DDBLTFX));
LBlitFX.dwSize = sizeof(DDBLTFX);
LBlitFX.dwFillColor = ColorTable.Green;

DisplayDD.pDDSMain->Blt(&BlotRect, Images[ISURF_SPLASH].pDDSImage, &BlotSrcRect, DDBLT_WAIT, NULL);


GDisplay.Draw_Text(DisplayDD.pDDSMain, InText, SplashRect.left + (SplashRect.right - SplashRect.left) * 0.39, SplashRect.top + (SplashRect.bottom - SplashRect.top) * 0.583, FONT_STANDARD1);
DestRect.right = DestRect.left + iBarMaxWidth * Percent;
DisplayDD.pDDSMain->Blt(&DestRect, NULL, NULL, DDBLT_COLORFILL, &LBlitFX);

Write_Log(InText);
WriteLog_Enter();

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void Test_Init_Objects()
{

StructList.Create_Structure( "BLUE Regional Sector Capital", 0, 33, 0, 6, 6); 
		StructList.Create_Structure("RED Regional Sector Capital", 0, 33, 1, GMap.iWidth - 6, 6); 
		StructList.Create_Structure("PURPLE Regional Sector Capital", 0, 33, 2, 6, GMap.iHeight - 6);
		StructList.Create_Structure("YELLOW Regional Sector Capital", 0, 33, 3, GMap.iWidth - 6, GMap.iHeight - 6);	

		cUnitA* pUnit;
		pUnit = UnitList.Create_Unit("BLUE 1st Armored Unit", UTYPE_NOTHING, 0, 2,2);
		pUnit->CmpAttached.Add_Component(4);
		pUnit->CmpAttached.Add_Component(5);		
		pUnit->CmpAttached.Add_Component(13);
		pUnit->CmpAttached.Add_Component(12);
		pUnit->CmpAttached.Add_Component(11);			
		pUnit->Recalc_Attributes();
	pUnit = UnitList.Create_Unit("BLUE 2nd Armored Unit", UTYPE_NOTHING, 0, 2,3);
		pUnit->CmpAttached.Add_Component(4);
		pUnit->CmpAttached.Add_Component(6);		
		pUnit->CmpAttached.Add_Component(13);
		pUnit->CmpAttached.Add_Component(12);
		pUnit->CmpAttached.Add_Component(11);		
		pUnit->Recalc_Attributes();
	UnitList.Create_Unit("BLUE 3rd Armored Unit", UTYPE_TEST00, 0, 3,2);
	UnitList.Create_Unit("BLUE 4th Armored Unit", UTYPE_TEST00,0, 3,3);
	UnitList.Create_Unit("RED 1st Armored Unit", UTYPE_TEST00,1, GMap.iWidth - 2,2);
	UnitList.Create_Unit("RED 2nd Armored Unit", UTYPE_TEST00,1, GMap.iWidth - 2,3);
	UnitList.Create_Unit("RED 3rd Armored Unit", UTYPE_TEST00,1, GMap.iWidth - 3,2);
	UnitList.Create_Unit("RED 4th Armored Unit", UTYPE_TEST00,1, GMap.iWidth - 3,3);
	UnitList.Create_Unit("PURPLE 1st Armored Unit", UTYPE_TEST00,2, 2,GMap.iHeight - 2);
	UnitList.Create_Unit("PURPLE 2nd Armored Unit", UTYPE_TEST00,2, 2,GMap.iHeight - 3);
	UnitList.Create_Unit("PURPLE 3rd Armored Unit", UTYPE_TEST00,2, 3,GMap.iHeight - 2);
	UnitList.Create_Unit("PURPLE 4th Armored Unit", UTYPE_TEST00,2, 3,GMap.iHeight - 3);
	UnitList.Create_Unit("YELLOW 1st Armored Unit", UTYPE_TEST00,3, GMap.iWidth - 2,GMap.iHeight - 2);
	UnitList.Create_Unit("YELLOW 2nd Armored Unit", UTYPE_TEST00,3, GMap.iWidth - 2,GMap.iHeight - 3);
	UnitList.Create_Unit("YELLOW 3rd Armored Unit", UTYPE_TEST00,3, GMap.iWidth - 3,GMap.iHeight - 2);
	UnitList.Create_Unit("YELLOW 4th Armored Unit", UTYPE_TEST00,3, GMap.iWidth - 3,GMap.iHeight - 3);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void Test_FakeAIModule()
{
	if (TimerControl.iCurrTick % 750 == 0)
{
for (int c27 = 0; c27 <= StructList.iID_Last_Assigned; c27++)
if (StructIDTable.pTable[c27] != NULL)
if (Nations.ppNations[StructIDTable.pTable[c27]->iControl]->bAIFlag == TRUE)
{
//DEBUG
//Write_Log("Commqueue - Faking AI:  ");
//Write_Log(StructIDTable.pTable[c27]->Name);
//WriteLog_Enter();
	int iBufferSize;
	BYTE* pData;
	int iRand = RandGen.rand2() % 7;
	if (iRand == 0 || iRand == 1 || iRand == 2)
	  pData = CommQueue.Assemble_CommData_Start_Build_Unit_Preconfig(TimerControl.iCurrTick + 1, c27, UTYPE_TEST00, &iBufferSize);
	if (iRand == 3 || iRand == 4 || iRand == 5)
	  pData = CommQueue.Assemble_CommData_Start_Build_Unit_Preconfig(TimerControl.iCurrTick + 1, c27, UTYPE_TEST01, &iBufferSize);
	if (iRand == 6)
	  pData = CommQueue.Assemble_CommData_Start_Build_Unit_Preconfig(TimerControl.iCurrTick + 1, c27, UTYPE_TEST04, &iBufferSize);
//DEBUG (removed if not _DEBUG)
	//_ASSERTE( _CrtCheckMemory() );
	
	CommQueue.Issue_Command_Q2(CQ2TYPE_START_BUILD_UNIT_PRECONFIG, pData, iBufferSize, FALSE);
//DEBUG (removed if not _DEBUG)
	//_ASSERTE( _CrtCheckMemory() );
}
}
if (TimerControl.iCurrTick % 5000 == 0)
{
for (int c27 = 0; c27 <= UnitList.ID_Last_Assigned; c27++)
if (UnitIDTable.pUnitTable[c27] != NULL)
{
	if ( Nations.ppNations[UnitIDTable.pUnitTable[c27]->iControl]->bAIFlag == TRUE) //UnitIDTable.pUnitTable[c27]->iControl != GameOptions.iControl)
	{
		int iX;
		int iY;
/*
		if (GameOptions.iControl == 0)
		{
			 iX = 15; iY = 15;
		}
		if (GameOptions.iControl == 1)
		{
			 iX = GMap.iWidth - 15; iY = 15;
		}
		if (GameOptions.iControl == 2)
		{
			 iX = 15; iY = GMap.iHeight - 15;
		}
		if (GameOptions.iControl == 3)
		{
			iX = GMap.iWidth - 15; iY = GMap.iHeight - 15;
		}
*/

		iX = GMap.iWidth / 2;
		iY = GMap.iHeight / 2;

		if (UnitIDTable.pUnitTable[c27]->iControl == 0)
		{
			iX -= 4;
			iY -= 4;
		}
		if (UnitIDTable.pUnitTable[c27]->iControl == 1)
		{
			iX += 4;
			iY -= 4;
		}
		if (UnitIDTable.pUnitTable[c27]->iControl == 2)
		{
			iY += 4;
			iX -= 4;
		}
		if (UnitIDTable.pUnitTable[c27]->iControl == 3)
		{
			iY += 4;
			iX += 4;
		}
		
		iX += RandGen.rand2() % 4 - 2;
		iY += RandGen.rand2() % 4 - 2;

		int iBufferSize;
		BYTE* pData = CommQueue.Assemble_CommData_Batch_Move2_Single_Unit(TimerControl.iCurrTick + 1, iY * GMap.iWidth + iX, c27, &iBufferSize);	
		CommQueue.Issue_Command_Q2(CQ2TYPE_MOVE_B, pData, iBufferSize, FALSE);
	}
}
}


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void Test_CheatModule()
{

	if (GInput.IGState_Keyboard[DIK_LSHIFT] == 1)
	if (GInput.IGState_Keyboard[DIK_RIGHT] == 3)
	{
		if (GInterface.Tutorial.iActiveElement < 34)
			GInterface.Tutorial.iActiveElement++;
	}
	if (GInput.IGState_Keyboard[DIK_LSHIFT] == 1)
	if (GInput.IGState_Keyboard[DIK_LEFT] == 3)
	{
		if (GInterface.Tutorial.iActiveElement > 0)
			GInterface.Tutorial.iActiveElement--;
	}


		//if (GInput.IGState_Keyboard[DIK_K] == 3)
	///	{
	//		End_Game();
	//	}

		if (GInput.IGState_Keyboard[DIK_S] == 3 &&
			GInput.IGState_Keyboard[DIK_LSHIFT] != 0)
		{
				GameOptions.bMusic = !GameOptions.bMusic;
				if (GameOptions.bMusic == FALSE)
				{
					AudioDA.Stop_Media(SOUNDSEG_M1);
					ChatBuffer.Add_CBNode("Music Disabled.", 500);
				}
				if (GameOptions.bMusic == TRUE)
				{
					AudioDA.Play_Secondary(SOUNDSEG_M1);
					ChatBuffer.Add_CBNode("Music Enabled.", 500);
				}

		}

		if (GInput.IGState_Keyboard[DIK_F5] != 0)
		if (GMap.pMap[GInterface.PosMap_Mouse.index].pOccupant_Unit == NULL)
		{
			if (UnitTypeTable.UnitTypes[101].Name[0] != 0)
				UnitList.Create_Unit(Strings.Supply_Random_String(GameOptions.iControl), 110, GameOptions.iControl, GInterface.PosMap_Mouse.x, GInterface.PosMap_Mouse.y);
		}
		if (GInput.IGState_Keyboard[DIK_F6] != 0)
		if (GMap.pMap[GInterface.PosMap_Mouse.index].pOccupant_Unit == NULL)
		{
			if (UnitTypeTable.UnitTypes[102].Name[0] != 0)
				UnitList.Create_Unit(Strings.Supply_Random_String(GameOptions.iControl), 102, GameOptions.iControl, GInterface.PosMap_Mouse.x, GInterface.PosMap_Mouse.y);
		}
		if (GInput.IGState_Keyboard[DIK_F7] != 0)
		if (GMap.pMap[GInterface.PosMap_Mouse.index].pOccupant_Unit == NULL)
		{
			if (UnitTypeTable.UnitTypes[103].Name[0] != 0)
				UnitList.Create_Unit(Strings.Supply_Random_String(GameOptions.iControl), 103, GameOptions.iControl, GInterface.PosMap_Mouse.x, GInterface.PosMap_Mouse.y);
		}


		if (GInput.IGState_Keyboard[DIK_F9] != 0)
		if (GMap.pMap[GInterface.PosMap_Mouse.index].pOccupant_Unit == NULL)
		{
			if (UnitTypeTable.UnitTypes[104].Name[0] != 0)
				UnitList.Create_Unit(Strings.Supply_Random_String(GameOptions.iControl), 104, GameOptions.iControl, GInterface.PosMap_Mouse.x, GInterface.PosMap_Mouse.y);
		}
		if (GInput.IGState_Keyboard[DIK_F10] != 0)
		if (GMap.pMap[GInterface.PosMap_Mouse.index].pOccupant_Unit == NULL)
		{
			if (UnitTypeTable.UnitTypes[105].Name[0] != 0)
				UnitList.Create_Unit(Strings.Supply_Random_String(GameOptions.iControl), 105, GameOptions.iControl, GInterface.PosMap_Mouse.x, GInterface.PosMap_Mouse.y);
		}

		//put a structure down on F8
		if (GInput.IGState_Keyboard[DIK_F8] != 0)
		if (GMap.pMap[GInterface.PosMap_Mouse.index].pOccupant_Structure == NULL)
		{
			 StructList.Create_Structure("Random Structure", 0, 33, GameOptions.iControl, GInterface.PosMap_Mouse.x, GInterface.PosMap_Mouse.y); 
		}

		//debug display toggle
		if (GInput.IGState_Keyboard[DIK_F11] != 0)
		{
			//GInterface.iConsoleDrawMode = 0;
			for (int lcy = 0; lcy < GMap.iHeight; lcy++)
			for (int lcx = 0; lcx < GMap.iWidth; lcx++)
			{
				GMap.pMap[lcy * GMap.iWidth + lcx].barrRevealed = 1 << (GameOptions.iControl);
			}

		}
		if (GInterface.IBoxes[IBOX_CONSOLE_CHAT].bActiveBox == FALSE)
		{
		if (GInput.IGState_Keyboard[DIK_Q] != 0)
			GInterface.iConsoleDrawMode = 1;
		if (GInput.IGState_Keyboard[DIK_W] != 0)
			GInterface.iConsoleDrawMode = 0;
		}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void Test_AdditModule()
{
	char DText[100];
	ZeroMemory(DText, sizeof(char)*100);

	FontInfo.SetFontColor(ColorTable.MM_Own);

	int iFontHeight = FontInfo.Fonts[FONT_STANDARD1].iHeight;

if (GInterface.mouse_currbox == IBOX_PRIMARY)
{
	MTile* MapTile = &GMap.pMap[GInterface.PosMap_Mouse.index];
	cUnitA* pUStackTrav = (cUnitA*)MapTile->pOccupant_Unit;

	strcat(DText, "Sector: ");
	strcat(DText, SectorTable.Sectors[MapTile->iSector]->Name);

	//write the name of the sector under the map cursor
	GDisplay.Draw_Text( DisplayDD.pSecondaryRenderTarget, DText, GInput.IGState_Mouse.iXPos, GInput.IGState_Mouse.iYPos + 20, FONT_STANDARD1);

	if (pUStackTrav != NULL)
	{

		GDisplay.Draw_Text( DisplayDD.pSecondaryRenderTarget, "Units:", GInput.IGState_Mouse.iXPos, GInput.IGState_Mouse.iYPos + 20 + (iFontHeight * 2), FONT_STANDARD1);

	}

	int lc774 = 0;

	//get information on units
	while (pUStackTrav != NULL)
	{
		GDisplay.Draw_Text( DisplayDD.pSecondaryRenderTarget, pUStackTrav->Name, GInput.IGState_Mouse.iXPos + 40, GInput.IGState_Mouse.iYPos + 20 + + (iFontHeight * 2) + (lc774*iFontHeight) , FONT_STANDARD1);
		lc774++;
		pUStackTrav = (cUnitA*)pUStackTrav->pNext_Stack;
	}

}
if (GInterface.mouse_currbox == IBOX_MINIMAP)
{
	strcat(DText, "Sector: ");
	strcat(DText, SectorTable.Sectors[GMap.pMap[GInterface.PosMinimap_Mouse.index].iSector]->Name);

	//write the name of the sector under the map cursor
	GDisplay.Draw_Text( DisplayDD.pSecondaryRenderTarget, DText, GInput.IGState_Mouse.iXPos, GInput.IGState_Mouse.iYPos + 20, FONT_STANDARD1);

}

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void MainLoop_Tutorial()
{

			CommQueue.Execute_Commands();

			CommQueue.Execute_Commands_Q2();

			if (GInterface.Tutorial.iActiveElement >= 0)
			{
				GInterface.Process_Interface_Elements_C( &GInterface.Tutorial.Elements[GInterface.Tutorial.iActiveElement] );
			}


			InputDI.Get_InputState();

			GInput.Update_IGStates();
			Test_CheatModule();

				if (GInput.IGState_Keyboard[DIK_ESCAPE] != 0)
				{	
					AppData.bRunStage = APP_RUNSTAGE_MAINEXIT;
				}



				GDisplay.Disp_Map(DisplayDD.pPrimaryRenderTarget);

				GDisplay.Disp_Units(DisplayDD.pPrimaryRenderTarget);

				GDisplay.Disp_Structures(DisplayDD.pPrimaryRenderTarget);

				Events.Process_Buffer();    //for now, events are DRAWN from insides the Process_Buffer() function...however, this leads to waste of blitting if many ticks occur to one GDRAW
											//in the future, GDisplay will probably be responsible for drawing the events

				GDisplay.UBlitQueue.Dispatch_BlitRequests(DisplayDD.pPrimaryRenderTarget, 0, NUM_ZLEVELS-1);

		if (GInput.IGState_Keyboard[DIK_TAB] > 0)
			GDisplay.Tint_MapControl(DisplayDD.pPrimaryRenderTarget);

		//do not allow flipping at any point between now and when we finish drawing on the secondary
		DisplayDD.bFlippable = FALSE;

				//TEST! before any secondary rendering is done, if DDRM_ALPHA, blit stuff from the render buffer to the video buffer
				if (DisplayDD.iDDRenderMode == DDRM_ALPHA)
				{
					DisplayDD.pSecondaryRenderTarget->BltFast(0, 0, DisplayDD.pPrimaryRenderTarget, NULL, DDBLTFAST_WAIT);
				}

				GDisplay.Disp_Projectiles(DisplayDD.pSecondaryRenderTarget);

				//GDisplay.Disp_Selection(DisplayDD.pSecondaryRenderTarget);


				if (GInterface.iConsoleDrawMode == 0)
				{

//blit over black on the main screen(to fill aspect-ratio areas)
if (DisplayDD.iYRes != DisplayDD.i_YResD || DisplayDD.iXRes != DisplayDD.i_XResD)
{
	DisplayDD.Colorfill3(DisplayDD.pSecondaryRenderTarget, 0,0, DisplayDD.iYRes, DisplayDD.i_XResD, DisplayDD.iXRes);
	DisplayDD.Colorfill3(DisplayDD.pSecondaryRenderTarget, 0,DisplayDD.i_YResD - 1, DisplayDD.iYRes, 0, DisplayDD.iXRes);

}

					GDisplay.Disp_Console(DisplayDD.pSecondaryRenderTarget);

				}
					else if (GInterface.iConsoleDrawMode == 1)
					{	
						GDisplay.Disp_Console(DisplayDD.pSecondaryRenderTarget);
						GDisplay.Disp_RunText(DisplayDD.pSecondaryRenderTarget);
					}
					if (GInput.IGState_Keyboard[DIK_LSHIFT] > 0)
						GDisplay.Disp_RunText(DisplayDD.pSecondaryRenderTarget);

				GDisplay.Disp_Minimap(DisplayDD.pSecondaryRenderTarget);
					
				ChatBuffer.Process_ChatBuffer();
				GDisplay.Disp_ChatText(DisplayDD.pSecondaryRenderTarget, 0, 40, 60);

				//DEBUG
				//GDisplay.Draw_Text(DisplayDD.pSecondaryRenderTarget, "TUTORIAL", 0, 0, FONT_STANDARD1);
				//char NText[50];
				//itoa(GInterface.Tutorial.iActiveElement, NText, 10);
				//GDisplay.Draw_Text(DisplayDD.pSecondaryRenderTarget, NText, 0, 20, FONT_STANDARD1);
				//itoa(GInterface.Tutorial.iTick_MostRecentCapitalStartConstruction, NText, 10);
				//GDisplay.Draw_Text(DisplayDD.pSecondaryRenderTarget, NText, 0, 40, FONT_STANDARD1);
				//itoa(GInterface.Tutorial.iTicksSince, NText, 10);
				//GDisplay.Draw_Text(DisplayDD.pSecondaryRenderTarget, NText, 0, 60, FONT_STANDARD1);


			NetDP.Process_StaticBuffer();	

			//notify box
			if (GameOptions.bGameType == GAMETYPE_MULTI)
			if (TimerControl.bLagging == TRUE)
			{
				NotifyBox("Waiting for players...", DisplayDD.pSecondaryRenderTarget);
			}
			//display the interface element here
			if (GInterface.Tutorial.iActiveElement >= 0)
			{
				GDisplay.Disp_IfcElems_C(&GInterface.Tutorial.Elements[GInterface.Tutorial.iActiveElement], 0, 0);
			}


			FontInfo.SetFontColor(ColorTable.Green);


				GDisplay.Disp_Cursor(DisplayDD.pSecondaryRenderTarget);

				//TEST! when rendering with DDRM_SYSMEM, copy to screen at the very last moment
				if (DisplayDD.iDDRenderMode == DDRM_SYSMEM)
				{
					//DisplayDD.pDDSBack->BltFast(0, 0, DisplayDD.pSecondaryRenderTarget, NULL, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
					DisplayDD.pDDSMain->BltFast(0, 0, DisplayDD.pSecondaryRenderTarget, NULL, 0);
				}

				DisplayDD.bFlippable = TRUE; //this will trigger the helper thread that it is ready to flip whenever the thread vsyncs...
				if (DTHREAD_MODE == 0)
					Test_Flip();

		
			bRet = PeekMessage(&AppData.WinMsg, NULL, 0, 0, PM_REMOVE);
			if (bRet != 0)
			{
				TranslateMessage(&AppData.WinMsg);
				DispatchMessage(&AppData.WinMsg);
			}  
			
			if (AppData.bRunFlag == FALSE)
				return;

TimerControl.EndCount(TIMER_GDRAW);


}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void MainLoop_GTick()
{

//DEBUG
Write_Log("Begin GTICK."); WriteLog_Enter();
			TimerControl.StartCount(TIMER_TEST1A);
			TimerControl.StartCount(TIMER_GTICK);
//DEBUG
Write_Log("Do Fake AI."); WriteLog_Enter();
			if (GameOptions.bGameType == GAMETYPE_SINGLE)
			{
				//Test_FakeAIModule(); //this module does not work as of 08.10.2004
			}

//DEBUG
Write_Log("Execute (real) AI"); WriteLog_Enter();
TimerControl.StartCount(TIMER_AI);


//if (TimerControl.iCurrTick % 4 == 0)
//{
		int currval = AppData.iAIToken;
		AppData.iAIToken++;
		if (AppData.iAIToken >= Nations.iNumNations)
			AppData.iAIToken = 0;
			if (Nations.ppNations[currval] != NULL)
			if (Nations.ppNations[currval]->bAIFlag == TRUE)
			if (Nations.ppNations[currval]->AINew.iAI_Override == 0) //for now, just don't process if there is any override
			{
				//do (new) AI
				Nations.ppNations[currval]->AINew.Run();

			}
//}	
TimerControl.EndCount(TIMER_AI);

TimerControl.StartCount(TIMER_MUSIC);
		//TEST check whether current piece is playing, play if not
		if (GameOptions.bMusic)
		if (TimerControl.iCurrTick % 60 == 0)
		if (AudioDA.bIsPlaying(SOUNDSEG_M1)  == FALSE)
				AudioDA.Play_Secondary(SOUNDSEG_M1);

TimerControl.EndCount(TIMER_MUSIC);
//DEBUG
Write_Log("Execute Commands 1"); WriteLog_Enter();

				CommQueue.Execute_Commands();

//DEBUG
Write_Log("Execute Commands 2"); WriteLog_Enter();

				CommQueue.Execute_Commands_Q2();  //01.01.2003

//DEBUG
Write_Log("Processing Tutorial (if active)"); WriteLog_Enter();
			if (GameOptions.bTutorial == TRUE)
			{
				GInterface.Process_Tutorial();  //01.01.2003
			}

//DEBUG
Write_Log("Process Units"); WriteLog_Enter();
TimerControl.StartCount(TIMER_PROCESS_UNITS);

				UnitList.Process_Units();

TimerControl.EndCount(TIMER_PROCESS_UNITS);
//DEBUG
Write_Log("Process Structures"); WriteLog_Enter();

				StructList.Process_Structures();

//DEBUG
Write_Log("Process Projectiles"); WriteLog_Enter();

				ProjList.Process_Projectiles();


				if (TimerControl.iCurrTick % GameOptions.iSectorTickCount == 0)
				{
//DEBUG
Write_Log("Process Sectors"); WriteLog_Enter();
				//	SectorTable.Process_Sectors();

					GMap.Assess();

					Nations.Deduct_SupportCosts();


				}
			
			if (TimerControl.iCurrTick % Configuration.iTempStore_MMUpdateTime == 0)
				Minimap.Process_Minimap(FALSE);

//DEBUG
Write_Log("Clearing Pending DP Messages"); WriteLog_Enter();
			NetDP.Process_StaticBuffer();

//DEBUG
Write_Log("End GTICK."); WriteLog_Enter();
				TimerControl.EndCount(TIMER_TEST1A);
			TimerControl.EndCount(TIMER_GTICK);

			TimerControl.iCurrTick++;
		

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void MainLoop_GDraw()
{


TimerControl.StartCount(TIMER_GDRAW);



//DEBUG
Write_Log("Begin GDRAW."); WriteLog_Enter();
			TimerControl.EndCount(TIMER_EXECF_GAP);
			TimerControl.StartCount(TIMER_EXECF);


			//execute commands during both GDRAW and GTICK cycles (GAMEWORLD destination commmands only processed inside GTick loop and at correct framecounts)
//DEBUG
Write_Log("Execute Commands 1"); WriteLog_Enter();
			CommQueue.Execute_Commands();

//DEBUG
Write_Log("Execute Commands 2"); WriteLog_Enter();
			CommQueue.Execute_Commands_Q2();



			TimerControl.StartCount(TIMER_MAPDRAW);


			//DEBUG
/* enable if you want to flood system with timing packets at inopportune times :-)
			int iRetAssemble = 0;
		BYTE* pBuffer = CommQueue.Assemble_CommData_ReportFrame2(GameOptions.iControl, TimerControl.iCurrTick, &iRetAssemble);
		NetDP.Send_Command2(CQ2TYPE_REPORTFRAME, pBuffer, iRetAssemble, TRUE, FALSE);
*/
//DEBUG
Write_Log("Get Inputstate"); WriteLog_Enter();
			InputDI.Get_InputState();

//DEBUG
Write_Log("Update IGStates"); WriteLog_Enter();
			GInput.Update_IGStates();

				if (GInput.IGState_Keyboard[DIK_ESCAPE] != 0)
				{	
					AppData.bRunStage = APP_RUNSTAGE_MAINEXIT;
				}
//DEBUG
Write_Log("Process Selection Buffers"); WriteLog_Enter();
			GInterface.Process_SBs();

//DEBUG
Write_Log("Process Chat Buffer"); WriteLog_Enter();
			ChatBuffer.Process_ChatBuffer();

//DEBUG
Write_Log("Process Input"); WriteLog_Enter();
			GInterface.Process_Input();
			Test_CheatModule();  
			if (GInterface.bTutorialPersistentMessageBoxActive == TRUE)
			{
				GInterface.Process_Interface_Elements_C( &GInterface.TutorialPersistentMessageBox );
			}


			//DEBUG
Write_Log("Process Console"); WriteLog_Enter();
		TimerControl.StartCount(TIMER_CONSOLE_PROCESS);

			GInterface.Process_Console();

Write_Log("Process Fake Hot Keys"); WriteLog_Enter();
			GInterface.Process_Fake_Hotkeys();

		TimerControl.EndCount(TIMER_CONSOLE_PROCESS);
		TimerControl.StartCount(TIMER_MAPDRAW);
//DEBUG
Write_Log("Drawing Map"); WriteLog_Enter();

				GDisplay.Disp_Map(DisplayDD.pPrimaryRenderTarget);
		TimerControl.EndCount(TIMER_MAPDRAW);


		//GDisplay.UBlitQueue.Dispatch_BlitRequests(DisplayDD.pPrimaryRenderTarget, 0, NUM_ZLEVELS-1);

		//GDisplay.Tint_MapControl(DisplayDD.pPrimaryRenderTarget);

//DEBUG
Write_Log("Drawing Units"); WriteLog_Enter();
		TimerControl.StartCount(TIMER_DISP_UNITS);

				GDisplay.Disp_Units(DisplayDD.pPrimaryRenderTarget);
		TimerControl.EndCount(TIMER_DISP_UNITS);

//DEBUG
Write_Log("Drawing Structures"); WriteLog_Enter();
		TimerControl.StartCount(TIMER_DISP_STRUCTS);

				GDisplay.Disp_Structures(DisplayDD.pPrimaryRenderTarget);
		TimerControl.EndCount(TIMER_DISP_STRUCTS);

//DEBUG
Write_Log("Process Events Buffer"); WriteLog_Enter();
		TimerControl.StartCount(TIMER_PROCESS_EVENTS);

				Events.Process_Buffer();    //for now, events are DRAWN from insides the Process_Buffer() function...however, this leads to waste of blitting if many ticks occur to one GDRAW
											//in the future, GDisplay will probably be responsible for drawing the events
		TimerControl.EndCount(TIMER_PROCESS_EVENTS);


//DEBUG
Write_Log("Dispatch Blit Requests"); WriteLog_Enter();
		TimerControl.StartCount(TIMER_BLITREQUESTS);

				GDisplay.UBlitQueue.Dispatch_BlitRequests(DisplayDD.pPrimaryRenderTarget, 0, NUM_ZLEVELS-1);




		if (GInput.IGState_Keyboard[DIK_TAB] > 0)
			GDisplay.Tint_MapControl(DisplayDD.pPrimaryRenderTarget);

		TimerControl.EndCount(TIMER_BLITREQUESTS);
		//do not allow flipping at any point between now and when we finish drawing on the secondary
		DisplayDD.bFlippable = FALSE;

				//TEST! before any secondary rendering is done, if DDRM_ALPHA, blit stuff from the render buffer to the video buffer
				if (DisplayDD.iDDRenderMode == DDRM_ALPHA)
				{
					DisplayDD.pSecondaryRenderTarget->BltFast(0, 0, DisplayDD.pPrimaryRenderTarget, NULL, DDBLTFAST_WAIT);
					
				}

//DEBUG
Write_Log("Drawing Projectiles"); WriteLog_Enter();
		TimerControl.StartCount(TIMER_DISP_PRJ);

				GDisplay.Disp_Projectiles(DisplayDD.pSecondaryRenderTarget);
		TimerControl.EndCount(TIMER_DISP_PRJ);

//DEBUG
Write_Log("Drawing Selection Graphics"); WriteLog_Enter();
	TimerControl.StartCount(TIMER_DISP_SELECT);

				GDisplay.Disp_Selection(DisplayDD.pSecondaryRenderTarget);
	TimerControl.EndCount(TIMER_DISP_SELECT);


//DEBUG
//Write_Log("Processing Console the second time"); WriteLog_Enter();

				GInterface.Process_Console();
	TimerControl.StartCount(TIMER_DISP_CONSOLE);


				if (GInterface.iConsoleDrawMode == 0)
				{

//blit over black on the main screen(to fill aspect-ratio areas)
if (DisplayDD.iYRes != DisplayDD.i_YResD || DisplayDD.iXRes != DisplayDD.i_XResD)
{
	DisplayDD.Colorfill3(DisplayDD.pSecondaryRenderTarget, 0,0, DisplayDD.iYRes, DisplayDD.i_XResD, DisplayDD.iXRes);
	DisplayDD.Colorfill3(DisplayDD.pSecondaryRenderTarget, 0,DisplayDD.i_YResD - 1, DisplayDD.iYRes, 0, DisplayDD.iXRes);

}

//DEBUG
Write_Log("Displaying Console"); WriteLog_Enter();

					GDisplay.Disp_Console(DisplayDD.pSecondaryRenderTarget);

				}
					else if (GInterface.iConsoleDrawMode == 1)
					{	

						GDisplay.Disp_Console(DisplayDD.pSecondaryRenderTarget);

						GDisplay.Disp_RunText(DisplayDD.pSecondaryRenderTarget);
					}
					if (GInput.IGState_Keyboard[DIK_LSHIFT] > 0)
						GDisplay.Disp_RunText(DisplayDD.pSecondaryRenderTarget);


				if (GInput.IGState_Keyboard[DIK_LSHIFT] > 0)
				for (int lc92 = 0; lc92 < TEMP_DP_MAXPLAYERS; lc92++)
				{
					if (Nations.ppNations[lc92] != NULL)
						Nations.ppNations[lc92]->AINew.Render_Entities_All(DisplayDD.pSecondaryRenderTarget);
				}
				//DEBUG
				//char NText[50];
				//itoa(GInterface.Tutorial.iActiveElement, NText, 10);
				//GDisplay.Draw_Text(DisplayDD.pSecondaryRenderTarget, NText, 0, 20, FONT_STANDARD1);
				//itoa(GInterface.Tutorial.iTick_StartProcessingSteel , NText, 10);
				//GDisplay.Draw_Text(DisplayDD.pSecondaryRenderTarget, NText, 0, 40, FONT_STANDARD1);
				//itoa(GInterface.Tutorial.iTicksSince, NText, 10);
				//GDisplay.Draw_Text(DisplayDD.pSecondaryRenderTarget, NText, 0, 60, FONT_STANDARD1);
				//itoa(GInterface.Tutorial.iMostRecentActiveElement, NText, 10);
				//GDisplay.Draw_Text(DisplayDD.pSecondaryRenderTarget, NText, 0, 100, FONT_STANDARD1);
			//display the interface element here
			if (GInterface.Tutorial.iActivePersistentElement >= 0)
			{
				GDisplay.Disp_IfcElems_C(&GInterface.Tutorial.Elements[GInterface.Tutorial.iActivePersistentElement], 0, 0);
			}

//if we have a ready button active display it here
if (GInterface.bTutorialPersistentMessageBoxActive == TRUE)
{
	GDisplay.Disp_IfcElems_C(&GInterface.TutorialPersistentMessageBox , 0, 0);
}

	TimerControl.EndCount(TIMER_DISP_CONSOLE);
	TimerControl.StartCount(TIMER_DISP_MINIMAP);

//DEBUG
Write_Log("Drawing Minimap"); WriteLog_Enter();

				GDisplay.Disp_Minimap(DisplayDD.pSecondaryRenderTarget);
	TimerControl.EndCount(TIMER_DISP_MINIMAP);
					
//DEBUG
Write_Log("Drawing ChatText"); WriteLog_Enter();

				GDisplay.Disp_ChatText(DisplayDD.pSecondaryRenderTarget, 0, 40, 60);

//DEBUG
Write_Log("Clearing Pending DP Messages"); WriteLog_Enter();

			NetDP.Process_StaticBuffer();	

//DEBUG
Write_Log("End GDRAW"); WriteLog_Enter();

			//notify box
			if (GameOptions.bGameType == GAMETYPE_MULTI)
			if (TimerControl.bLagging == TRUE)
			{
				NotifyBox("Waiting for players...", DisplayDD.pSecondaryRenderTarget);
			}

			FontInfo.SetFontColor(ColorTable.Green);

/*
			itoa(NetDP.iBufferTrace, NText, 10);
			GDisplay.Draw_Text(DisplayDD.pSecondaryRenderTarget, NText, 0, 30, FONT_STANDARD1);
			itoa( RandGen.RandKeys[RandGen.wIndex1] + RandGen.RandKeys[RandGen.wIndex2] + RandGen.RandKeys[RandGen.wIndex3] + RandGen.RandKeys[RandGen.wIndex4], NText, 10);
			GDisplay.Draw_Text(DisplayDD.pSecondaryRenderTarget, NText, 0, 50, FONT_STANDARD1);
*/			
			//GDisplay.Draw_Text(DisplayDD.pSecondaryRenderTarget, "Project Director - 12.19.2004", (int)(DisplayDD.i_XResD * 0.1), DisplayDD.i_YResD - 16, FONT_STANDARD1);

			//Test_AdditModule(); //draw the sector name underneath the cursor

			TimerControl.StartCount(TIMER_CURSORDRAW);		

				GDisplay.Disp_Cursor(DisplayDD.pSecondaryRenderTarget);

			TimerControl.EndCount(TIMER_CURSORDRAW);

				//TEST! when rendering with DDRM_SYSMEM, copy to screen at the very last moment
				if (DisplayDD.iDDRenderMode == DDRM_SYSMEM)
				{
					//DisplayDD.pDDSBack->BltFast(0, 0, DisplayDD.pSecondaryRenderTarget, NULL, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
					DisplayDD.pDDSMain->BltFast(0, 0, DisplayDD.pSecondaryRenderTarget, NULL, 0);
				}

 //FLIP NORMALLY GOES HERE!
				//Test_Flip();
				DisplayDD.bFlippable = TRUE; //this will trigger the helper thread that it is ready to flip whenever the thread vsyncs...
				if (DTHREAD_MODE == 0)
					Test_Flip();

			TimerControl.EndCount(TIMER_EXECF);
			TimerControl.StartCount(TIMER_EXECF_GAP);
		
			AppData.Secondary_Window_Processing();
			bRet = PeekMessage(&AppData.WinMsg, NULL, 0, 0, PM_REMOVE);
			while (bRet != 0)
			{
				TranslateMessage(&AppData.WinMsg);
				DispatchMessage(&AppData.WinMsg);
				bRet = PeekMessage(&AppData.WinMsg, NULL, 0, 0, PM_REMOVE);
			} 
			
			if (AppData.bRunFlag == FALSE)
				return;

TimerControl.EndCount(TIMER_GDRAW);
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
DWORD WINAPI TimerThreadFunc ( LPVOID lpParam)
{
	
	while (bExitTimerThread == FALSE)
	{
		//Cleared everything out. I presume this should be okay since the only thing writing to llSubCount is
		//the timer thread (all others just read it).
		//i disabled the mutex because I can't get it actually run smoothly.

		//wait for use to clear
///		while ((bExitTimerThread == FALSE) && (TimerControl.bSubCountInUse == TRUE))
//		{

//		}
		//set in use
//		TimerControl.bSubCountInUse = TRUE;
		//get count
		QueryPerformanceCounter((LARGE_INTEGER *)&TimerControl.llSubCount);
		Sleep(2);		


		//set out of use
//		TimerControl.bSubCountInUse = FALSE;
	}
	return 0;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

DWORD WINAPI ThreadFunc( LPVOID lpParam ) 
{

while(TRUE)
{
	
	if (AppData.bRunStage == APP_RUNSTAGE_MAINEXIT)
		return 0;
	if (DisplayDD.bFlippable == TRUE)
	{
		TimerControl.StartCount(TIMER_DDFLIP);
		DisplayDD.pDDSMain->Flip(NULL, DDFLIP_DONOTWAIT | DDFLIP_NOVSYNC);
		DisplayDD.bFlippable = FALSE;
			TimerControl.iCurrFrame++;
			TimerControl.EndCount(TIMER_DDFLIP);
	}
/*
	if (DisplayDD.bFlippable == TRUE)
	{
	DWORD dwScan = 0;
	BOOL bFlip = FALSE;
	DisplayDD.pDD7->GetVerticalBlankStatus(&bFlip);

	if (bFlip == TRUE)
	{
			TimerControl.StartCount(TIMER_DDFLIP);
			if (DisplayDD.iDDRenderMode != DDRM_SYSMEM)
				DisplayDD.pDDSMain->Flip(NULL, DDFLIP_DONOTWAIT | DDFLIP_NOVSYNC); //can unleash the frame rate(as of 01.03.2002 Thinkpad T22 pulled about 300 FPS), but didn't look too good
			TimerControl.EndCount(TIMER_DDFLIP);
			TimerControl.iCurrFrame++;
			DisplayDD.bFlippable = FALSE;
	}
	}
*/
}
}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////

void Test_Flip()
{
//	DWORD dwScan = 0;
///
//	if (dwScan > (DisplayDD.iYRes * 0.0))
//	{
			TimerControl.StartCount(TIMER_DDFLIP);
			if (DisplayDD.iDDRenderMode != DDRM_SYSMEM)
				DisplayDD.pDDSMain->Flip(NULL, DDFLIP_NOVSYNC); //can unleash the frame rate(as of 01.03.2002 Thinkpad T22 pulled about 300 FPS), but didn't look too good


			TimerControl.EndCount(TIMER_DDFLIP);
			TimerControl.iCurrFrame++;

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
void Launch_MusicService()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNA;

    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process. 
    CreateProcess( "./media/musicservice/musicservice.exe", // No module name (use command line). 
        "MyChildProcess", // Command line. 
        NULL,             // Process handle not inheritable. 
        NULL,             // Thread handle not inheritable. 
        FALSE,            // Set handle inheritance to FALSE. 
        HIGH_PRIORITY_CLASS,                
        NULL,             // Use parent's environment block. 
        NULL,             // Use parent's starting directory. 
        &si,              // Pointer to STARTUPINFO structure.
        &pi );             // Pointer to PROCESS_INFORMATION structure.

}
//////////**************************************************************************************************************************************///////////////////////
//////////**************************************************************************************************************************************///////////////////////
//evaluates victory condition.
//if one of the nations has won, return the ID of that nation.
//otherwise return -1
int Temp_Determine_Winner()
{
	int iTotalTerritory = 0;
	int iTotalUnits = 0;
	int iTotalStructures = 0;

	for (int lc1 = 0; lc1 < TEMP_DP_MAXPLAYERS; lc1++)
	{
		if (Nations.ppNations[lc1] != NULL)
		{
			iTotalTerritory += Nations.ppNations[lc1]->iPossession;
			iTotalUnits += Nations.ppNations[lc1]->iNumUnits;
			iTotalStructures += Nations.ppNations[lc1]->iNumStructures;
		}
	}
	for (int lc2 = 0; lc2 < TEMP_DP_MAXPLAYERS; lc2++)
	{
		if (Nations.ppNations[lc2] != NULL)
		{
			double ddRatio = double(Nations.ppNations[lc2]->iPossession)/iTotalTerritory;
			
			//territory win
			//if (ddRatio >= 0.7  && Nations.ppNations[lc2]->ddPossession > 0.1)
			//	return lc2;
			//kill all units and structures win
			if (Nations.ppNations[lc2]->iNumStructures == iTotalStructures)
			if (Nations.ppNations[lc2]->iNumUnits == iTotalUnits)
				return lc2;
		}
	}
return -1;

}
void End_Game()
{

		DWORD* pdwExecTick = new DWORD;
		*pdwExecTick = TimerControl.iCurrTick + GameOptions.iLatency + 1;

		CommQueue.Issue_Command_Q2(CQ2TYPE_INITIATE_ENDGAME, (BYTE*)pdwExecTick, sizeof(DWORD), TRUE);
		if (GameOptions.bGameType == GAMETYPE_MULTI)
			NetDP.Send_Command2(CQ2TYPE_INITIATE_ENDGAME, (BYTE*)pdwExecTick, sizeof(DWORD), FALSE, TRUE);

		delete pdwExecTick;
}
void NotifyBox(char* Text, LPDIRECTDRAWSURFACE7 pDrawSurf)
{
	int XRes = DisplayDD.i_XResD;
	int YRes = DisplayDD.i_YResD;
	RECT r; r.left = XRes * 0.4; r.right = XRes * 0.6; r.top = YRes * 0.4; r.bottom = YRes * 0.5;
	GDisplay.Draw_Interface_Panel(pDrawSurf, r);
	GDisplay.Draw_Text_Shadow(pDrawSurf, Text, XRes * 0.42, YRes*0.45, FONT_STANDARD1, ColorTable.White - 1, ColorTable.Black); 
}

void Calc_SplashRects()
{
		SplashRect.bottom = DisplayDD.i_YResD;
		SplashRect.top = 0; 
		SplashRect.left = 0;
		SplashRect.right = DisplayDD.i_XResD;

		SplashSrcRect.top = 0;
		SplashSrcRect.bottom = Images[ISURF_SPLASH].DDSDImage.dwHeight;
		SplashSrcRect.left = 0;
		SplashSrcRect.right = Images[ISURF_SPLASH].DDSDImage.dwWidth;

		double imageaspect = double(Images[ISURF_SPLASH].DDSDImage.dwWidth) / Images[ISURF_SPLASH].DDSDImage.dwHeight;
		double destaspect = double(DisplayDD.i_XResD) / double(DisplayDD.i_YResD);

		

		if (imageaspect > destaspect)
		{
			double xsrcfit = Images[ISURF_SPLASH].DDSDImage.dwHeight * destaspect;
			SplashSrcRect.left = 0.5 * (Images[ISURF_SPLASH].DDSDImage.dwWidth - xsrcfit);
			SplashSrcRect.right = Images[ISURF_SPLASH].DDSDImage.dwWidth - 0.5 * (Images[ISURF_SPLASH].DDSDImage.dwWidth - xsrcfit);
		}
		if (imageaspect < destaspect)
		{
			double ysrcfit = Images[ISURF_SPLASH].DDSDImage.dwWidth / destaspect;
			SplashSrcRect.top = 0.5 * (Images[ISURF_SPLASH].DDSDImage.dwHeight - ysrcfit);
			SplashSrcRect.bottom = Images[ISURF_SPLASH].DDSDImage.dwHeight - 0.5 * (Images[ISURF_SPLASH].DDSDImage.dwHeight - ysrcfit);
		}
}
void Swap_SplashGraphics(char* NewFileName)
{
	Clear_SplashGraphics();
	Load_SplashGraphics(NewFileName);
}

void Clear_SplashGraphics()
{
	//unload old splash screen
	Images[ISURF_SPLASH].Exit();
}
void Load_SplashGraphics(char* NewFileName)
{
	//load the new splash screen
	Images[ISURF_SPLASH].Init(&ClassAccess, ISURF_SPLASH, DisplayDD.pDD7, NewFileName, TRUE, FALSE, ColorTable.White, FALSE, 0, 0);	
	//recalc the appropriate rects
	Calc_SplashRects();
}