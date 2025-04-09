//TODO:
//(05.15.2002) 
//-Try making the maximum number of units dynamic by re-implementing the unitIDtable as a tvector

//NOTE: as of 08.09.2003 the /GF (pool and ROM duplicate string literals) option was added to the compile options.
//this has apparently served to end the random crash-upon-creating-some-units problem which could not
//be traced to any specific line of code (it had been traced to some random memory allocations)
//08.10.2003: /ML flag changed to MLd for ssame reason

//disable "conversion from xxx to yyy: possible loss of data" warning
#pragma warning(disable: 4244)
//disable "xxx: unreferenced local variable" warning
#pragma warning(disable: 4101)
//disable "xxx: signed/unsigned" warning
#pragma warning(disable: 4018)
//disable "local variable is initialized but not referenced: warning
#pragma warning(disable: 4189)
//disable "conditional expression is constant" warning
#pragma warning(disable: 4127)
//disable "strcat is unsafe" warning
#pragma warning(disable: 4996)

#define DIRECTINPUT_VERSION 0x0800
/*
#define DIRECTSOUND_VERSION 0x0800
#define DIRECTPLAY_VERSION 0x0800
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <limits.h>

#include <ddraw.h>
#include <dinput.h>
//#include <d3d9.h>
#define INITGUID
#include <dplay.h>
#include <dplay8.h>
#include <dpaddr.h>
#include <dmusici.h>
#include <dsound.h>
//#include <dxerr8.h>   //disabled 09.15.2003  //re-enabled 05.28.2004
#include <dxerr9.h>
#include <math.h>
#include <new.h>
#include <assert.h>
#include <time.h>
#include <process.h>
#include <stdio.h>

//testing fmod
#include <fmod.h>

//#define DXGetErrorString9 DXGetErrorString8

#define nNoMansLandSize	4

#ifdef _DEBUG
typedef struct _CrtMemBlockHeader
{
        struct _CrtMemBlockHeader * pBlockHeaderNext;
        struct _CrtMemBlockHeader * pBlockHeaderPrev;
        char *                      szFileName;
        int                         nLine;
        size_t                      nDataSize;
        int                         nBlockUse;
        long                        lRequest;
        unsigned char               gap[nNoMansLandSize];
        /* followed by:
         *  unsigned char           data[nDataSize];
         *  unsigned char           anotherGap[nNoMansLandSize];
         */
} _CrtMemBlockHeader;

#define pbData(pblock) ((unsigned char *)((_CrtMemBlockHeader *)pblock + 1))
#define pHdr(pbData) (((_CrtMemBlockHeader *)pbData)-1)

//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


//D3D stuff (disable. these features will not be in Director Project at this late stage) (05.28.2004)
//#define D3D9T_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
//#define D3D9T_CUSTOMVERTEX2 (D3DFVF_XYZ|D3DFVF_TEX1)

//attempt to map get error 8 to get error 9 (temp?) 09.15.2003
//#define DXGetErrorString8 DXGetErrorString9A
#define DXGetErrorString9A DXGetErrorString8A

//just a small hack so that I sneak a converter program with minimal effort(05.11.2004)
#define HACK_MODE 0

//experimental change 12.26.2003
//#define DX9_DD_NEW_MODE	
			
#ifdef DX9_DD_NEW_MODE
	#define DDNM
	#define LPDIRECTDRAW7			LPDIRECTDRAW
	#define LPDIRECTDRAWSURFACE7	LPDIRECTDRAWSURFACE
	#define CLSID_DirectDraw7		CLSID_DirectDraw
	#define IID_IDirectDraw7		IID_IDirectDraw
	#define DDSURFACEDESC2			DDSURFACEDESC
	#define DDSCAPS2				DDSCAPS
#endif


#define DDRM_NORMAL		0
#define DDRM_ALPHA		1	//this mode conceived as way to make the alpha-blending method feasible.
							//it renders all "alpha-critical" sections entirely in system memory, and
							//then blits it in one shot to the display memory (01.16.2004)
#define DDRM_SYSMEM		2	//force the system to render EVERYTHING in system memory and copy it to screen at the end (09.17.2004)

#define DTHREAD_MODE	0   //set to 1 to enable "helper thread"

//rendering methods (moved to configuration file setup 07.21.2004)
//#define DD_RENDER_MODE	DDRM_ALPHA

//this color is the color index used for "emblem-drawing" (drawing a nation's color onto a unit) purposes
#define EMBLEM_REPLACE_COLOR	12

//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
#define IDI_ICON1	101

//12.23.2003
#define DEBUG					0	//set to 1 to start all debugging flags, 0 for off
#define DEBUG_MAIN				1
#define DEBUG_APPDATA			0
#define DEBUG_PATHFINDER		0		//overrides general debug information (-1 for no debug, 0 for no override, 1 for debug)
#define DEBUG_COMMQUEUE			0		
#define DEBUG_COMPONENT_A		0
#define DEBUG_D3D				0
#define DEBUG_DAUDIO			0
#define DEBUG_DISPLAYDD			1
#define DEBUG_INPUTDI			0
#define DEBUG_DP_MULTIPLAY		0
#define DEBUG_GDISPLAY			0
#define DEBUG_GAMEWORLD_A		0
#define DEBUG_IMAGESURFACE		0
#define DEBUG_INTERFACE			0
#define DEBUG_NOTIFBUFFER		0
#define DEBUG_PROJECTILE_A		0
//#define DEBUG_STRUCTURE_A		0	// currently no debug control (multiple instances)
#define DEBUG_STRUCTLIST		0
#define DEBUG_TIMER				0
#define DEBUG_UNITLIST			0
#define DEBUG_UNIT_A			0
#define DEBUG_IPB				0
#define DEBUG_EVENTBUFFER		0
#define DEBUG_ANIMINFOTABLE		0
#define DEBUG_MEM				0
#define DEBUG_SECTORTABLE		0	//05.28.2004
#define DEBUG_CONFIG			0
#define DEBUG_CSRCGRAPHICS		0
#define DEBUG_AI				0  //01.01.2005
//#define DEBUG_STRUCTINFO		0	//currrently no debug control (12.23.2003)
//#define DEBUG_MINIMAP			0       //currently no debug control


#define VAL_INVALID		-6994

#define MAX_MAPS		 50  //simply put some cap on the number of maps so that we can simply have a static array
#define MAX_SECTORS		 65600	//the maximum number of sectors on a given map. The number here is simply to be safe.
								//The planned implementation to the map format will result in a theoretical limit of 65536.(05.28.2004)
								//(06.28.2008) no longer true. the number of sectors now depends on how fragmented the map is...
								//we've got to figure out what to do about this.

#define NUM_MAX_ACKS	 5000
#define NUM_ACK_TYPES      26
#define ACK_TYPE_HELLO	            1
#define ACK_TYPE_CMDRECV_UNIT_GENERIC 2
#define ACK_TYPE_CMDRECV_UNIT_GENERIC_CANT	3  //if the unit has decided it can't complete the command
#define ACK_TYPE_CMDRECV_UNIT_MOVE  4
#define ACK_TYPE_CMDRECV_UNIT_MOVE_CANT 5
#define ACK_TYPE_CMDRECV_UNIT_ATK	6
#define ACK_TYPE_CMDRECV_UNIT_ATK_CANT 7
#define ACK_TYPE_CMDRECV_UNIT_BUILD 8
#define ACK_TYPE_CMDRECV_UNIT_BUILD_CANT 9
#define ACK_TYPE_CMDRECV_UNIT_LOAD  10
#define ACK_TYPE_CMDRECV_UNIT_LOAD_CANT 11
#define ACK_TYPE_CMDRECV_UNIT_UNLOAD 12
#define ACK_TYPE_CMDRECV_UNIT_UNLOAD_CANT 13
#define ACK_TYPE_CMDRECV_UNIT_FILL 14
#define ACK_TYPE_CMDRECV_UNIT_FILL_CANT 15
#define ACK_TYPE_CMDRECV_UNIT_EMPTY 16
#define ACK_TYPE_CMDRECV_UNIT_EMPTY_CANT 17
#define ACK_TYPE_STRUCT_TURN_OFF 18
#define ACK_TYPE_STRUCT_TURN_ON 19
#define ACK_TYPE_STATUS_UNIT_DIE 20
#define ACK_TYPE_STATUS_UNIT_ALERT 21
#define ACK_TYPE_STATUS_UNIT_BORN 22
#define ACK_TYPE_UNIT_SPECIAL1	23
#define ACK_TYPE_MENTAT_CONSTRUCTION_COMPLETE 24
#define ACK_TYPE_MENTAT_GAME_START 25


#define TILE_SIZE        24
#define CURSOR_SIZE		 24
#define EMBLEM_SIZE		 8    //05.15.2002
#define BLIT_FPCOL       16   //"frames per column" in the files
#define MBUTTON_SIZE_X   100 //07.08.2002
#define MBUTTON_SIZE_Y   24  //07.08.2002
#define CMDBUTTON_SIZE_X 24	
#define CMDBUTTON_SIZE_Y 24
#define CMDBUTTON2_SIZE_X 72
#define CMDBUTTON2_SIZE_Y 26
#define SQCMDBUTTON_SIZE_X 24
#define SQCMDBUTTON_SIZE_Y 24

//range of error anticipated/allowed in floating point routines (01.07.2004)
#define FP_TOLERANCE	 0.007
#define FP_TOLERANCE_MOV 0.9
#define RAD_DEG_CONV	 57.295779513082320876798154814105
#define FP_TOLERANCE_LOW 0.8 //radians

//"tilt" angle of perspective
#define TILT_ANGLE		28.125  //degrees

//this parameter currently determines how much "slip" the aiming algorithm will tolerate before attempting to re-aim a weapon
//this is necessary because otherwise weapons will continually hesitate to fire on a moving target (01.08.2004)
#define ALLOW_AIM_DEVIATION		5		//degrees

//definitions for the "fineness" of rotations, the number of possible unit directions
#define UNIT_NUMDIRS			64     //hopefully we can make this obsolete today (01.07.2004)
//.....same for projectile counterparts....just in case we don't want to be as "fine" with projectiles (06.16.2002)
//#define PROJ_NUMDIRS			64	  //obsolete! (07.22.2004)

#define NUM_ZLEVELS		10   //01.07.2004
#define ZLEVEL_MIDBASE	2	//specifies the "middle ZLevel" that is treated as 0 for non-frame components that need to draw more than 1 image
#define ZLEVEL_MAIN		0	//HACK...a Z-level that all frames must currently have and specifies a "base" that "offset" components tack to (01.07.2004) 

//number of isurfaces to have available arrayed
#define NUM_DEF_ISURFACES		61    //"already defined isurfaces" (those that are predefined in the program)
#define NUM_MAX_ISURFACES       300  //user defined isurfaces can go up to this number


//set numbers for the image surfaces each of these tile graphics are loaded to
#define ISURF_GRASSLAND		0
#define ISURF_OCEAN			1
#define ISURF_MOUNTAIN		2
#define ISURF_HILL			3
#define ISURF_RIVER			4
#define ISURF_FOREST		5
#define ISURF_CURSOR		6
#define ISURF_MLAYER2		7
#define ISURF_FONT01		8
#define ISURF_FRAME_TEST01	9
#define ISURF_FRAME_02		26	//12.26.2003
#define ISURF_FRAME_03		29
#define ISURF_FRAME_04		33
//#define ISURF_TANKGUN_TEST01 10
#define ISURF_TANKGUN_03	27	//12.26.2003
#define ISURF_TANKGUN_04	28  //12.28.2003
#define ISURF_NAVALGUN_01	30
#define ISURF_SELECT		11  //general selection box(03.29.2003)
#define ISURF_EMBLEMS		12	//national emblems (05.15.2002)
#define ISURF_PROJ_TEST01	13  //first test projectile graphics (06.07.2002)
#define ISURF_HPBAR			14  //a HP bar
#define ISURF_SPLASH		15	//splash screen
#define ISURF_MAINMENU		16  //main menu screen
#define ISURF_MENU05		46
//#define ISURF_MENUBUTTONS00	17 //menu buttons
#define ISURF_MENUBUTTONS01 43
//#define ISURF_MENUBOXES00	18
#define ISURF_MENUBOXES01	44
#define ISURF_FONT02		19
//#define ISURF_CONSOLE_BOTTOM 20
//#define ISURF_CONSOLE_RIGHT	 21
#define ISURF_PROJ_TEST02	22
#define ISURF_TANKGUN_TEST02 23
#define ISURF_STRUCT_TEST01	24
#define ISURF_PROJ_NAVAL120	31
#define ISURF_STRUCT_RSC01	32
#define ISURF_ANIM_TEST01	34
#define ISURF_ANIM_TEST02	35
#define ISURF_ANIM_TEST03	36
#define ISURF_ANIM_TEST04	37
#define ISURF_ANIM_UEXPL01	38
#define ISURF_SCROLLBAR		39
#define ISURF_BLACK			40
#define ISURF_CMDBUTTONS	25  
#define ISURF_CMDBUTTONS2	41	//on the info display
#define ISURF_CMDBUTTONS_SQ 45
#define ISURF_ANIM_MSLTRAIL1 	42
#define ISURF_STRUCT_TANK	47
#define ISURF_ALPHA_FONT03	48
#define ISURF_STRUCT_FACTORY_ABC 49
#define ISURF_STRUCT_FORGE	50
#define ISURF_CRANE			51
#define ISURF_STRUCT_REFINERY	52
#define ISURF_STRUCT_REACTOR	53
#define ISURF_STRUCT_SHIPYARD	54
#define ISURF_ICONS 55
#define ISURF_PIE1  56
#define ISURF_WHITE 57 //this is going to be a WTF someday...
#define ISURF_LINKS 58
#define ISURF_ANIM_BLAST	59
#define ISURF_STRUCT_EXTRACTOR  60


//define how to sort out the stock images
#define NUM_FRAMESET_TYPES	4 
#define FRAMESET_DEFAULT 0      //07.26.2004
	

//define what flags are what in the bExBufferFlags
#define EXBUFFER_KEY		0x00	//the key/alpha 1-bit channel used to draw shadows and faux alpha
#define EXBUFFER_MASK_KEY	0x01	//mask to isolate this bit
#define EXBUFFER_COLORMARK	0x01	//channel used to indicate that this pixel is to be replaced with the color of it's icontrol
#define EXBUFFER_MASK_COLORMARK 0x02 //mask to isolate this bit

//MAP ID blocks in the version 2.00 map files
#define MAP_V2_IDBLOCK_BASIC	19
#define MAP_V2_IDBLOCK_UCR		20	//create unit  
#define MAP_V2_IDBLOCK_SCR		21  //create structure 
#define MAP_V2_IDBLOCK_SECTOR	22	//define sector associations for all tiles on the map
#define MAP_V2_IDBLOCK_SECTREG	23	//register sector properties to a sector ID
#define MAP_V2_IDBLOCK_BRIEFTEXT 24   //defines the briefing text
#define MAP_V2_IDBLOCK_TUTORIAL 25 //enables/disables tutorial mode on this map
#define MAP_V2_IDBLOCK_NATION	26 //initializes a nation
#define MAP_V2_IDBLOCK_FILLUNIT 27 //fill's a unit material storage with a specific amount of materials
#define MAP_V2_IDBLOCK_SETLAYER2	28  //sets the value of a tile's Layer2 draw(used to lay down "specials" on the map)
#define MAP_V2_IDBLOCK_NOTHING	0

//CONFIG ID blocks in configuration files
#define CONFIG_IDBLOCK_NOTHING	0
#define CONFIG_IDBLOCK_LGRAPHIC	24  //
#define CONFIG_IDBLOCK_LSOUND	25
#define CONFIG_IDBLOCK_ACK_STRUCT 26
#define CONFIG_IDBLOCK_ACK_UNIT	27
#define CONFIG_IDBLOCK_ACK_MENTAT 28
#define CONFIG_IDBLOCK_LTCMP_FR	29  //load type - component - frame (separate block types are needed for each component, because defining all five types with one block would be very complicated
#define CONFIG_IDBLOCK_LTCMP_EN 30	//engine
#define CONFIG_IDBLOCK_LTCMP_WP 31	//weapon
#define CONFIG_IDBLOCK_LTCMP_DF 32	//defense
#define CONFIG_IDBLOCK_LTCMP_TL 33	//tool
#define CONFIG_IDBLOCK_LTCMP_ST 34
#define CONFIG_IDBLOCK_LTCMP_GR 35	//graphics for a component type
#define CONFIG_IDBLOCK_LTCMP_GR_STA 36 //load the "standard A" set of graphics for the component type
#define CONFIG_IDBLOCK_LTCMP_MAT 37  //add a resource cost to a component type
#define CONFIG_IDBLOCK_LTPRJ	38 //load type - projectile (07.20.2004)
#define CONFIG_IDBLOCK_LTPRJ_GR	39  //load the graphics for a projectile type (07.20.2004)
#define CONFIG_IDBLOCK_LTUNIT	40  //load type - unit (07.20.2004)
#define CONFIG_IDBLOCK_DDRM		41	//set DD rendermode
#define CONFIG_IDBLOCK_MMUPDATET 42 //set the amount of ticks between minimap refreshes
#define CONFIG_IDBLOCK_SETRES	43	//set xres, yres, bpp
#define CONFIG_IDBLOCK_TEXTZERO 45  //true or false
#define CONFIG_IDBLOCK_TIMERTHREAD 46 //true or false
#define CONFIG_IDBLOCK_CORE0	47	//force core 0

//DEDICATED terrain type indexes (02.11.2003) (for now, matches the old ISURF indexing)
#define NUM_MAPTILE_TYPES	7

#define MAPTILE_GRASSLAND	0
#define MAPTILE_OCEAN		1
#define MAPTILE_MOUNTAIN	2
#define MAPTILE_HILL		3
#define MAPTILE_RIVER		4
#define MAPTILE_FOREST		5
#define MAPTILE_AIR			6	//air is defined as a "maptile", thus, aircraft frames would be defined with this flag enabled

//map overlay indexes into the barrOverlay member of MTile
#define MAPOVERLAY_LINK_MASK   1  //0x000000001 (bit)		

//mapfile symbol, for version 2
#define MAPFILE_SYM_GRASSLAND	'G'
#define MAPFILE_SYM_OCEAN		'O'
#define MAPFILE_SYM_MOUNTAIN	'M'
#define MAPFILE_SYM_RIVER		'R'
#define MAPFILE_SYM_FOREST		'F'
#define MAPFILE_SYM_HILL		'H'
//some default sector assignments
#define MAPFILE_SCT_NOSECTOR	0    //this is the default sector ID for an "unclaimed" maptile (06.11.2004)

//define Animation TYPES
#define	ANIMTYPE_SPARKLE01	1
#define ANIMTYPE_TRAIL01	2
#define ANIMTYPE_EXPL01		3
#define ANIMTYPE_EXPL02		4   //"red' explosion
#define ANIMTYPE_UEXPL01	5	//unit explosion
#define ANIMTYPE_MSLTRAIL1	6
#define ANIMTYPE_BLAST		7   //large blast

//define Component Class Numbers(03.01.2003)
#define NUM_COMPONENT_CLASSES		6   //just in case (used. 08.20.2003)

#define CMPCLASS_FRAME		0
#define CMPCLASS_ENGINE		1
#define CMPCLASS_DEFENSE	2
#define CMPCLASS_WEAPON		3
#define CMPCLASS_TOOL		4
#define CMPCLASS_STORAGE	5

//define Component draw characteristics (03.21.2003)
#define CMPDRAW_DRAWNORMAL	0   //this name might change, just implies that it is draw
#define CMPDRAW_SINGLEFRAME	1	//draw only single frame(because only one frame is there)
#define CMPDRAW_NODRAW		2	//don't draw this component

//define Component directional characteristics(03.26.2003)
#define CMPDIR_LOCK			0
#define CMPDIR_FREE			1

//(01.08.2004)
//define UNIT MULTI-FIRE WEAPON PATTERNS (as in, when a unit wishes to fire multiple weapons, how to fire them (all cannons? all machine guns? all missiles? or simply all weapons?)
#define MULTIFIRE_ALL		0

//define Weapon attack procedure characteristics (03.21.2003)
#define ATTACK_NOTHING		0
#define ATTACK_PROJECTILE	1		//standard projectile creation method

//define WEAPON pre-attack procedure characteristics( 03.26.2003)
#define PREATTACK_NOTHING	0
#define PREATTACK_AIM		1

//define Tool use procedure characteristics (10.11.2003)
#define TOOL_BUILD			1		//a little unsure about using the iUseProcedure to specify how tools work(10.11.2003)
#define TOOL_NOTHING		0

//define Tool pre-use procedure characteristics(10.11.2003)
#define PRETOOL_NOTHING		0

//define PERMISSION TYPES
#define PERMISSION_SEA1		17
#define PERMISSION_LAND1    18
#define PERMISSION_AIR1		19

//unit blit queue stuff
#define BQU_BLITTYPE_NORMAL		0		//blit the graphic only
#define BQU_BLITTYPE_OVERLAY	1		//blit the graphic, then blit the overlay over of the prescribed color
#define BQU_BLITTYPE_OV_ONLY	2		//blit the overlay only
#define BQU_BLITTYPE_SHADOW		3		//this is actually currently the same as OV_ONLY, except it forces blitting at the front(01.17.2004)
#define BQU_BLITTYPE_OBJECT_01	4		//first blitting scheme for object....blits the unit and then blits it's "emblem" colorreplace markings on it
#define BQU_BLITTYPE_ALPHA		6		//full alpha the source image

//FMOD specific
#define FMOD_MEDIA_TYPE_MIDI	12
#define FMOD_MEDIA_TYPE_SAMPLE	13
#define FMOD_MEDIA_TYPE_STREAM	14

//....do something analogous with sound segments(02.08.2003)
#define NUM_SOUNDSEGS		200
#define SOUNDSEG_TEST1		1
#define SOUNDSEG_TEST2		2
#define SOUNDSEG_GUN1		3
#define SOUNDSEG_FIRE_80MM	4
#define SOUNDSEG_FIRE_120MM	5
#define SOUNDSEG_FIRE_MSL	6
#define SOUNDSEG_FIRE_NAVAL120 7
#define SOUNDSEG_IMPACT1	8
#define SOUNDSEG_IMPACT2	9
#define SOUNDSEG_UEXPL01	10
#define SOUNDSEG_AIRMSL		11
#define SOUNDSEG_ACK0		12
#define SOUNDSEG_ACK1		13
#define SOUNDSEG_ACK2		14
#define SOUNDSEG_ACK10		15
#define SOUNDSEG_ACK11		16
#define SOUNDSEG_ACK12		17
#define SOUNDSEG_M1			18
#define SOUNDSEG_CHIME		19
#define SOUNDSEG_MENU		20

//assign numbers to game types(really only two at the moment)
#define GAMETYPE_SINGLE		0
#define GAMETYPE_MULTI		1


#define	GEN_PADBYTES		4
//define a number of "pad" bytes to be placed at the end of each PCMD2, and each pData in PCMD2, to better prevent against memory corruption(03.10.2004)
#define CQ2_ENDPADBYTES		4

//assign numbers for commqueue commands (02.09.2002)
#define CQDEST_GAMEWORLD    0
#define CQDEST_INTERFACE	1

#define CQTYPE_MOVE			0    //(for a move command, Param1: unit ID, Param2: target index)
#define CQTYPE_ATTACK		1	 //(for an attack command, Param1: unit ID, Param2: target type, Param3: target ID, Param4: Pos_ABSx, Param5: Pos_ABSy)

//new command format, 01.01.2003 (reduces command size, overhead, allows for "batch" commands(move unit group, etc))
#define CQ2TYPE_INVALID			0
#define CQ2TYPE_MOVE			1	//format: LONG(execution tick), WORD(Unit ID), LONG(target index) 
#define CQ2TYPE_ATTACK_UNIT		2		//format: LONG(execution tick), WORD(Unit ID), WORD(enemy ID)
#define CQ2TYPE_REPORTFRAME		3		//format: BYTE(control ID), LONG(reported tick count)
#define CQ2TYPE_BATCH_MOVE		4		//format: LONG(execution tick), LONG(target index), WORD(number of IDs issued to) ... then all WORDs afterwards: Unit IDs
#define CQ2TYPE_BATCH_ATTACK_UNIT 5	//format: LONG(execution tick), WORD(EnemyID), WORD(number of IDs issued to) ... then all WORDs afterwards: Unit IDs
#define CQ2TYPE_BATCH_ATTACK_STRUCT 6 //format: LONG(execution tick), WORD(EnemyID), WORD(number of IDs issued to) .... then all WORDs afterwards: Unit IDs
#define CQ2TYPE_BATCH_GUARD		7		//format: LONG(execution tick), LONG(guard index), WORD(number of IDs issued), WORDs(IDs of units thereafter)

//the NEW formats for command2 commands(sigh....) these will start out as copies of the old format code but upon proof that it works will eventually supersede the old format
//05.30.2003
#define CQ2TYPE_MOVE_B			8		//format: DWORD(execution tick), SET(OBJTYPE_UNIT, PLURAL), SET(PARAM_DWORD, SINGLE)
#define CQ2TYPE_ATTACK_B		9		//format: DWORD(execution tick), SET(OBJTYPE_UNIT, PLURAL), SET(OBJTYPE_UNIT/OBJTYPE_STRUCTURE, SINGLE)
										//NOTE: this "ATTACK" command is named as the "standard" attack command. Different modes of attacking(varying degrees of aggression) will receive different command numbers(07.30.2003)
#define CQ2TYPE_START_BUILD_UNIT_PRECONFIG	10	//format: DWORD(execution tick), SET(OBJTYPE_STRUCTURE, SINGLE), SET(OBJTYPE_UTYPE, SINGLE) (08.07.2003)
#define CQ2TYPE_START_BUILD_UNIT_CUSTOM		11  //format: DWORD(execution tick), SET(OBJTYPE_STRUCTURE, SINGLE), SET(OBJTYPE_CTYPEGROUP, SINGLE/PLURAL) 08.20.2003
#define CQ2TYPE_START_BUILD_COMPONENT		12	//format: DWORD(execution tick), SET(OBJTYPE_STRUCTURE, SINGLE), SET(OBJTYPE_CTYPE, SINGLE) (08.07.2003)
#define CQ2TYPE_BUILD_STRUCTURE				13  //format: DWORD(execution tick), SET(OBJTYPE_UNIT, PLURAL(IDs of units to be issued to), SET(OBJTYPE_STYPE, SINGLE), SET(OBJTYPE_MAPTILE, SINGLE)  (10.11.2003)
#define CQ2TYPE_LOAD						14  //format: DWORD(execution tick), SET(OBJTYPE_UNIT, PLURAL(IDs of units to be issued to), SET(OBJTYPE_UNIT, SINGLE) (id of unit to load onto)
#define CQ2TYPE_UNLOAD					    15  //format: DWORD(execution tick), SET(OBJTYPE_UNIT, PLURAL(IDs of debarking units), SET(OBJTYPE_UNIT, SINGLE) (ID of units unloading), SET(PARAM_DWORD, SINGLE) (target maptile to unload to)
#define CQ2TYPE_UNLOAD_ALL					16  //format: DWORD(exeuction tick), SET(OBJTYPE_UNIT, PLURAL (IDs of unloading units), SET(PARAM_DWORD, SINGLE) (maptile of unloading location)  //basically just a wrapper around the more general CQ2TYPE_UNLOAD
#define CQ2TYPE_TRANSFER_MATERIAL			17	//format: DWORD(execution tick), SET(OBJTYPE_UNIT, SINGLE (ID of unit transferring), SET(OBJTYPE_MAPTILE, SINLE) (maptile to pickup from), SET(OBJTYPE_MAPTILE, SINLE) (maptile to drop off at)
#define CQ2TYPE_LOAD_MATERIAL				18	//format: DWORD(execution tick), SET(OBJTYPE_UNIT, SINGLE (ID of loading unit), SET(OBJTYPE_MAPTILE, SINGLE) (maptile of where the "stuff" is)
#define CQ2TYPE_UNLOAD_MATERIAL				19  //format: DWORD(exeuction tick), SET(OBJTYPE_UNIT, SINGLE (ID of unloading unit), SET(OBJTYPE_MAPTILE, SINGLE) (maptile of where the "stuff" will be unloaded to (whether it be a tank or another tanker)
#define CQ2TYPE_STRUCT_TURN_OFF				20  //format: DWORD(execution tick), SET(OBJTYPE_STRUCT, SINGLE (ID of structure))
#define CQ2TYPE_STRUCT_TURN_ON				21  //format: DWORD(execution tick), SET(OBJTYPE_STRUCT, SINGLE (ID of structure))
#define CQ2TYPE_TANK_SET_TYPE				22	//format: DWORD(execution tick), SET(OBJTYPE_STRUCT, SINGLE (ID of structure)), SET(OBJTYPE_MATTYPE, SINGLE(type to set to))
#define CQ2TYPE_TANK_FLUSH					23  //format: DWORD(execution tick), SET(OBJTYPE_STRUCT, SINGLE (ID of structure))
#define CQ2TYPE_CANCEL_PROGORDER_TOP		24  //format: DWORD(execution tick), SET(OBJTYPE_STRUCT, SINGLE (ID of structure))
#define CQ2TYPE_LAY_LINK					25  //format: DWORD(execution tick), SET(OBJTYPE_MAPTILE_SINGLE (target location)
#define CQ2TYPE_ARM_MATERIAL				26  //format: DWORD(execution tick), SET(OBJTYPE_UNIT, PLURAL) (IDs of units attempting to arm)

//upper limit of arrays allocated to service all possible commands
#define NUM_CQ2COMMANDS			27///////

//08.07.2003 note that the following CQ2 commands start at far over the CQ2COMMAND limit because they are actually used internally, and the user can never call them(through the proper interface)
//I cannot think of a reason why it is necessary to use the new formatting with these commands, other than it works using the format anyway
#define CQ2TYPE_BUILD_PROGRESS_UNIT			200		//format: POINTER(the calling ProgOrder), DWORD(Structure ID), DOUBLE(resource cost?), DOUBLE(construction difficulty), DWORD(type ID, if it exists, otherwise send -1)		(in this case, the first is the Structure constructing and the second is the iType of the unit under construction
													//this command simply increases the progress of the PO specified based on the building capacity of the specified structure, the corresponding construction cost(deducts from national reserve) and the difficulty of construction
#define CQ2TYPE_BUILD_PROGRESS_COMPONENT	201		//format: POINTER(the calling ProgOrder), DWORD(Structure ID), DWORD(Component iType)
													//command increases the progress of the PO specified based on the building capacity of the structure and the cost and difficulty of construction of the specified component
#define CQ2TYPE_EARMARK_COMPONENTS			202		//format: POINTER(the calling ProgOrder), DWORD(Structure ID), SET(OBJTYPE_COMPONENT, PLURAL) //format change 08.17.2003 (if optionally, the last parameters are type OBJTYPE_CTYPE, the command will search national inventory for available components
	#define OWNER_NATIONAL_INVENTORY		-6996	//the above command is planned to be used for a build ProgOrder beginning, to earmark components for use in construction so that no other buildings can use them. this sets the owner (thus the reason for the #define OWNER_NATIONAL_INVENTORY )
#define CQ2TYPE_CREATE_UNIT_A				203		//format: POINTER(the calling ProgOrder), DWORD(Structure ID), DWORD(Unit Type Hint), SET(OBJTYPE_COMPONENT, PLURAL) //format change 08.17.2003 (if optionally, the last parameters are type OBJTYPE_CTYPE, the command will search national inventory for available components
													//the above command will actually create the unit of specified makeup of component IDs around the specified structure
#define CQ2TYPE_CREATE_UNIT_B				204		//format: DWORD(maptile index), DWORD(nation ID), DWORD(utype)
#define CQ2TYPE_CREATE_COMPONENT_A			205		//format: POINTER(the calling ProgOrder), DWORD(Structure ID), DWORD(Component iType)
#define CQ2TYPE_INIT_NATION					206		//this command tells a computer to initialize a nation.
													//format: DWORD(iControl Index), DWORD(length of nation name), CHARs(the name)
#define CQ2TYPE_INITIATE_ENDGAME			207		//format: DWORD(execution tick)
#define CQ2TYPE_INIT_TIMERID				208     //format: DWORD(iTimerID), DWORD(corresponding DPNID), DWORD(name length), chars(name)  //this command tells the system to keep track of this ID during synchronization checking
#define CQ2TYPE_SEND_HOSTDPNID				209		//format: DWORD(DPNID to send to)
#define CQ2TYPE_SET_HOSTDPNID				210		//format: DWORD(DPNID of host)
#define CQ2TYPE_SEND_UMAPID					211		//format: DWORD(DPNID to send to)
#define CQ2TYPE_SET_UMAPID					212		//format: DWORD(map ID)
#define CQ2TYPE_SEND_MAPPATH				213		//format: DWORD(DPNID to send to)
#define CQ2TYPE_SET_MAPPATH					214		//format: chars(path)
#define CQ2TYPE_INDICATE_READY				215		//format; DWORD(DPNID of player that is ready)
#define CQ2TYPE_NOTIFY_ICONTROL				216		//format: DWORD(DPNID of player), int (iControl)
#define CQ2TYPE_SET_IDCONTROLANDTIMER		217     //parameter int(control index), int(timer index)

//THESE commands are used for embedded interface functions
#define CQ2TYPE_BLIT						400     //format: DWORD(render target code), DWORD(image surface index), DWORD(destination left), DWORD(destination top), DWORD(source left), DWORD(source right), DWORD(source top), DWORD(source bottom)
#define CQ2TYPE_BLITFIT						401		//format: DWORD(render target code), DWORD(image surface index), DWORD(destination left), DWORD(destination right), DWORD(destination top), DWORD(destination bottom), DWORD(source left), DWORD(source right), DWORD(source top), DWORD(source bottom)
#define CQ2TYPE_BLITBYSURFACE				402		//format: DWORD(render target code), SURFACEPOINTER (target surface, DWORD(destination left), DWORD(destination right), DWORD(destination top), DWORD(destination bottom), DWORD(source left), DWORD(source right), DWORD(source top), DWORD(source bottom)
#define CQ2TYPE_DRAWIFCPANEL				403		//format: DWORD(render target code), DWORD(target left), DWORD(target right), DWORD(target top), DWORD(target bottom) 
#define CQ2TYPE_DRAWIFCLISTBOX				404		//format: DWORD(render target code), DWORD(target left), DWORD(target right), DWORD(target top), DWORD(target bottom) 
#define CQ2TYPE_DRAWIFCSECONDARYPANEL		405		//format: DWORD(render target code), DWORD(target left), DWORD(target right), DWORD(target top), DWORD(target bottom), DWORD ColorIndex 


//render target codes
	#define RTC_PRIMARY			0
	#define RTC_SECONDARY		1
	#define RTC_BACK				2
#define CQ2TYPE_COLORFILL					406		//format: DWORD(render target code), DWORD(target color), DWORD(left), DWORD(right), DWORD(top), DWORD(bottom) -->note, despite the fact we don't know beforehand what the color format is, one can use cColorTable's DetColor to acquire the necessary color value
#define CQ2TYPE_COLORFILL2					407		//format: DWORD(R component --> 0 to 255), DWORD(G component --> 0 to 255), DWORD(B component --> 0 to 255)
#define CQ2TYPE_DRAWTEXT					408		//format: DWORD(render target code), DWORD(X-position, pixels), DWORD(Y-position, pixels), DWORD(XLimit, pixels),DWORD(font type), DWORD(font color), DWORD(number of characters), from here on unicode SHORTS for each of the characters, terminated with a zero(not included in character count)
#define CQ2TYPE_DRAWTEXT2					409		//format: DWORD(unicode flag), DWORD(render target code), DWORD(X-position, pixels), DWORD(XLimit, pixels), DWORD(Y-position, pixels), DWORD(font type), DWORD(font color), pointer LPSTR to a string
#define CQ2TYPE_DRAWTEXT3					410		//format: DWORD(unicode flag), DWORD(render target code), DWORD(X-position, pixels), DWORD(XLimit, pixels), DWORD(Y-position, pixels), DWORD(font type), DWORD(font color), pointer LPSTR to a pointer to a string (!)
#define CQ2TYPE_DRAWTEXT4					411		// <special> encapsulated in cCQ2TYPE_DRAWTEXT4_DATA structure
#define CQ2TYPE_SELECT_GAMETYPE_SINGLE		412     // no parameters
#define CQ2TYPE_SELECT_GAMETYPE_MULTI		413		// no parameters
#define CQ2TYPE_SELECT_QUIT					414		// no parameters
#define CQ2TYPE_SELECT_START				415		// no parameters
#define CQ2TYPE_SELECT_PROCEED				416		// no parameters
#define CQ2TYPE_SELECT_MAP					417		//1 parameter, DWORD map ID
#define CQ2TYPE_SET_ACTIVEPROVIDER			418		//1 parameter, DWORD provider ID
#define CQ2TYPE_SET_ACTIVEDEVICE			419		//2 parameters, DWORD provider ID, DWORD device ID
#define CQ2TYPE_SET_INPUTSTRING				420		//2 parameters, DWORD buffersize, pointer to string
#define CQ2TYPE_SET_INPUTSTRING_WIDE		421		//2 parameter, DWORD buffersize, pointer to string
#define CQ2TYPE_SET_PDWORD					422		//2 parameters, DWORD set value, pointer to dword
#define CQ2TYPE_COPY_STRING_CTOC			423		//(char to char)2 parameters, pointer to source string, pointer to destination string (assumes that string buffer sizes are compatible)
#define CQ2TYPE_COPY_STRING_CTOW			424		//(char to wide)2 parameters, pointer to source string, pointer to destination string 
#define CQ2TYPE_COPY_STRING_WTOW			425	//(wide to wide)2 parameters, pointer to source string, pointer to destination string 
#define CQ2TYPE_COPY_STRING_WTOC			426		//(wide to char)2 parameters, pointer to source string, pointer to destination string 
#define CQ2TYPE_SETMAINMENUMODE				427		//format: DWORD(mode number)
#define CQ2TYPE_FINALIZE_MP_MENUS			428		//no parameters (this command tells the system to rebuild the Service-Provider specific interface panels after the user has specified the SP and the device that will be used...)
#define CQ2TYPE_HOST_STD					429		//no parameters 
#define CQ2TYPE_SELECT_DISCONNECT			430		//no parameters
#define CQ2TYPE_ENUMHOSTS_STD				431		//no params, pulls connection information from activedevice, activeprovider info
#define CQ2TYPE_SET_ACTIVEHOST				432		//1 param, active provider ID
#define CQ2TYPE_CONNECT_STD					433		//no parameters
#define CQ2TYPE_SENDTEXT					434		//DWORD string length, BUFFER of CHARS corresponding to name, DWORD string length, BUFFER of CHARS corresponding to chat text
#define CQ2TYPE_SENDTEXT2					435		//WCHAR* input name, char* text, DWORD buffer limit
#define CQ2TYPE_DISPCHATTEXT				436		//DWORD Xpos, DWORD Ypos, DWORD Numlines
#define CQ2TYPE_CLEARCHATBUFFER				437		//no params
#define CQ2TYPE_LISTBOX_EXECUTE_IFC			438		//(see assemble_commdata function)
#define CQ2TYPE_LISTBOX_ARROW_UP			439		//pointer(parent listbox ifcelement)
#define CQ2TYPE_LISTBOX_ARROW_DOWN			440		//pointer(parent listbox ifcelemtnt)
#define CQ2TYPE_LISTBOX_CONDITIONAL_BLIT	441		//format: pointer(parent listbox ifcelement), DWORD(render target code), DWORD(image surface index), DWORD(destination left), DWORD(destination top), DWORD(source left), DWORD(source right), DWORD(source top), DWORD(source bottom)1
#define CQ2TYPE_LISTBOX_DISP_SLIDER			442		//format: pointer (parent listbix ifcelement), DWORD render target
#define CQ2TYPE_SLIDER_INPUT				443		//format: pointer (parent listbox ifcelement)
#define CQ2TYPE_START_PENDINGCOMMAND		444		//format: DWORD holds command2 type
#define CQ2TYPE_CANCEL_PENDINGCOMMAND		445		//no parameters
#define CQ2TYPE_SET_ACTIVEIGRID				446		//format: DWORD(IBox ID), DWORD(active IGrid to set to)
#define CQ2TYPE_SEND_INPUTPACKET			447		//format: DWORD(PacketType OBJREF/PARAM), DWORD(SubType), DWORD(NumItems), VOID*(Array)
#define CQ2TYPE_SET_MAPVIEW					448		//format; DWORD(xtop), DWORD(ytop)
#define CQ2TYPE_SELECT_UNIT					449		//format: DWORD(unit ID)
#define CQ2TYPE_SELECT_STRUCT				450		//format: DWORD(struct ID)
#define CQ2TYPE_SET_CONSOLE_UNIT_SELECT		451		//format: DWORD(unit ID)
#define CQ2TYPE_DISABLE_ISSUE_PP			452		//format: DWORD(type of group packet to issue)
#define CQ2TYPE_FLUSH_PP					453		//no parameters
#define CQ2TYPE_END_GAME					454		//no parameters
#define CQ2TYPE_RETURN_FROM_TUTORIAL		455		//no parameters
#define CQ2TYPE_SET_IDCONTROL				456		//format: DWORD(icontrol)
#define CQ2TYPE_SET_TUTORIAL_PAGE           457		//format: int(Tutorial Page)
#define CQ2TYPE_PERSBOXCLICK				458		//no parameters


//(interface)
#define CQTYPE_REPORTFRAME	      2	 //parameter 1 is the iControl ID, parameter2 is the reported tick count
#define CQTYPE_SET_MAINMENUMODE   3  //parameter 1 is the new Main Menu IBox mode
#define CQTYPE_SELECT_GAMETYPE_SINGLE   4     //no parameters, this is an extremely specialized command because interface grid cells currently only hold ONE command each //07.08.2002
#define CQTYPE_SELECT_GAMETYPE_MULTI	5	  //no parameters, same as above
#define CQTYPE_SELECT_QUIT				6	  //07.09.2002
#define CQTYPE_SET_ACTIVEPROVIDER		7	  //parameter 1 is provider index
#define CQTYPE_SET_ACTIVEDEVICE			8	  //parameter 1 is device index
#define CQTYPE_SET_ACTIVEHOST			25	  //parameter 1 is host index
#define CQTYPE_SET_INPUTSTRING			9	  //pParam6 is the pointer to the string (to cInputState's input string), wParam1 holds maxbuffersize
#define CQTYPE_SET_INPUTSTRING_WIDE		10	  //pParam6 is the pointer to the string, wParam1 holds max buffersize
     //and now a bunch of SET_SERIAL_MODES
#define CQTYPE_SETSERIAL_FLOWCONTROL_RTSDTR		11	  //no params
#define CQTYPE_SETSERIAL_FLOWCONTROL_XONXOFF	12
#define CQTYPE_SETSERIAL_FLOWCONTROL_RTS		13
#define CQTYPE_SETSERIAL_FLOWCONTROL_DTR		14
#define CQTYPE_SETSERIAL_FLOWCONTROL_NONE		15
#define CQTYPE_SETSERIAL_PARITY_NONE			16	
#define CQTYPE_SETSERIAL_PARITY_EVEN			17
#define CQTYPE_SETSERIAL_PARITY_ODD				18
#define CQTYPE_SETSERIAL_PARITY_MARK			19
#define CQTYPE_SETSERIAL_PARITY_SPACE			20
#define CQTYPE_SETSERIAL_STOPBITS_ONE			21
#define CQTYPE_SETSERIAL_STOPBITS_ONEFIVE		22
#define CQTYPE_SETSERIAL_STOPBITS_TWO			23
#define CQTYPE_SETSERIAL_BAUD					24 //wParam1 holds BAUDRATE
#define CQTYPE_ENUMHOSTS_STD					26  //no params, pulls connection information from activedevice, activeprovider info
#define CQTYPE_HOST_STD							27  //no params
#define CQTYPE_CONNECT_STD						28
//more 
#define CQTYPE_SELECT_DISCONNECT				29
#define CQTYPE_SENDTEXT							30 //pParam6 is the pointer to the string, wParam1 holds length of string
#define CQTYPE_SELECT_START						31 //only the host may start

//set control ID command(nominally from host)
#define CQTYPE_SET_IDCONTROLANDTIMER			32 //parameter 1 is iControl index, 2 is Timer index
//interface
#define CQTYPE_SET_CONSOLE_UNIT					33 //pParam6 holds LPSBE pointer
#define CQTYPE_START_PENDINGCOMMAND				34 //parameter 1 holds the command2 type to start, pParam6, if NOT NULL, is a pointer to command information to be placed initially into the data buffer.
#define CQTYPE_CANCEL_PENDINGCOMMAND			35 //no params
#define CQTYPE_SET_ACTIVE_IGRID					36 //param1 holds the IBOX index, param2 holds the IGRID index to set active
#define CQTYPE_SET_TOTAL_PLAYER_COUNT			37 //param1 holds the count (includes both AI and human players)
#define CQTYPE_ENABLE_PACKETPROCESSOR			38//param1 holds the object to have it begin "sniffing"
#define CQTYPE_DISABLE_PACKETPROCESSOR			39 //no parameters
#define CQTYPE_DISABLE_ISSUE_PP					40  //disables packet processor and issues. (param1 holds group type) 
#define CQTYPE_FLUSH_PP							41
#define CQTYPE_SELECT_MAP						42	//wParam1 holds map index
#define CQTYPE_SELECT_MAP_MULTI					43  //select this map, but additionally broadcast a request to other players for whether they have this map
#define CQTYPE_REQUEST_MAP						44  //broadcast a request for whether the other players have the requested map. first parameter holds map ID
#define CQTYPE_RETURN_REQUEST_MAP				45  //broadcast a confirmation/rejection of a map request back to the host, first parameter holds map ID, second parameter holds 1 or 0, depending on whether the sender has the map
#define CQTYPE_SET_MAP_MULTI					46  //actually set the map
#define CQTYPE_SELECT_UNIT						47  //wParam1 holds unit ID
#define CQTYPE_SELECT_STRUCT					48	//wParam1 holds struct ID
#define CQTYPE_POST_ACK_U						49  //wParam1 holds the ack type, wParam2 holds the unit ID
#define CQTYPE_SET_TOTAL_TIMERID_COUNT			50  //param1 holds the total number of timers to keep track of (essentialy # of physical connected computers participating in the game)


//types of targets in the attack command
#define CQATTACKTYPE_NOTHING	0
#define CQATTACKTYPE_UNIT		1
#define CQATTACKTYPE_STRUCT		2
#define CQATTACKTYPE_MAPTILE	3


//define percentage of screen to draw the map
#define MDRAW_XSCR		0.80
#define	MDRAW_YSCR		0.80
#define	MMDRAW_XSCR_START	0.82
#define MMDRAW_YSCR_START	0.021
#define MMDRAW_XSCR			0.16
#define MMDRAW_YSCR			0.2133 //keep in mind, 4:3
//for the "zoomed" minimap
#define MMZDRAW_XSCR_START	0.81
#define MMZDRAW_YSCR_START	0.25
#define MMZDRAW_XSCR			0.18
#define MMZDRAW_YSCR			0.2

//percentage screen position for each of the console boxes
//equip
#define CONSOLE_EQUIP_TOP		0.82
#define CONSOLE_EQUIP_BOTTOM	0.98
#define CONSOLE_EQUIP_LEFT		0.02
#define CONSOLE_EQUIP_RIGHT		0.15
//list access
#define CONSOLE_LIST_TOP		0.82
#define CONSOLE_LIST_BOTTOM		0.98
#define CONSOLE_LIST_LEFT		0.57
#define CONSOLE_LIST_RIGHT		0.79
//command
#define CONSOLE_COMMAND_TOP		0.81
#define CONSOLE_COMMAND_BOTTOM	0.98
#define CONSOLE_COMMAND_LEFT	0.81
#define CONSOLE_COMMAND_RIGHT	0.98
//information
#define CONSOLE_INFO_TOP		0.32
#define CONSOLE_INFO_BOTTOM		0.79
#define CONSOLE_INFO_LEFT		0.82
#define CONSOLE_INFO_RIGHT		0.98
//information
#define CONSOLE_DISPLAY_TOP		0.82
#define CONSOLE_DISPLAY_BOTTOM		0.98
#define CONSOLE_DISPLAY_LEFT		0.17
#define CONSOLE_DISPLAY_RIGHT		0.55
//chat
#define CONSOLE_CHAT_TOP		0.70
#define CONSOLE_CHAT_BOTTOM		0.80
#define CONSOLE_CHAT_LEFT		0.0
#define CONSOLE_CHAT_RIGHT		0.30
//notify window
#define CONSOLE_NOTIFY_TOP		0.30
#define CONSOLE_NOTIFY_BOTTOM	0.50
#define CONSOLE_NOTIFY_LEFT		0.30
#define CONSOLE_NOTIFY_RIGHT	0.50

//*****//


//interface element command categories
#define IFC_ELEM_CAT_NUMELEMS	11
#define IFC_ELEM_CAT_DEFAULT	0
#define IFC_ELEM_CAT_ONHOVER	1
#define IFC_ELEM_CAT_ONLDOWN    2
#define IFC_ELEM_CAT_ONLON		3
#define IFC_ELEM_CAT_ONLUP		4
#define IFC_ELEM_CAT_DEFAULT_DRAW  5
#define IFC_ELEM_CAT_ONHOVER_DRAW  6
#define IFC_ELEM_CAT_ONLDOWN_DRAW  7
#define IFC_ELEM_CAT_ONLON_DRAW	   8
#define IFC_ELEM_CAT_ONLUP_DRAW	   9

//define the text size of FONT01 to be 8x15
//#define FONT01_HEIGHT		15	//obsolete ... tabled 03.02.2003
//#define FONT01_WIDTH		8   //obsolete ... tabled 03.02.2003
//define "names" for fonts
#define FONT_NUMFONTS		3
#define FONT_TEST01			0
#define FONT_STANDARD1		1
#define FONT_ALPHA_STANDARD1 2 //this is a version of the text that is loaded in the alphabuffer region so that it can be drawn directly on the alpharender area

//define timer count numbers
#define TIMER_EXECT				0  //"execute tick"  //execute everything that goes on in one tick
#define TIMER_EXECF				1  //"execute frame" 
#define TIMER_MAPDRAW			2
#define TIMER_CURSORDRAW		3
#define TIMER_DDFLIP			4
#define TIMER_ALLPROG			5
#define TIMER_PATHFIND			6  //time pathfinder took to find one path
#define TIMER_TEST1A			7
#define TIMER_PF_A				8
#define TIMER_PF_B				9
#define TIMER_EXECF_GAP			10
#define TIMER_COUNTER1			11
#define TIMER_COUNTER2			12
#define TIMER_COUNTER3			13
#define TIMER_AI				14
#define TIMER_PROCESS_UNITS		15
#define TIMER_CONSOLE_DRAW		16
#define TIMER_CONSOLE_PROCESS	17
#define TIMER_GDRAW				18
#define TIMER_GTICK				19
#define TIMER_DISP_UNITS		20
#define TIMER_DISP_STRUCTS		21
#define TIMER_PROCESS_EVENTS	22
#define TIMER_BLITREQUESTS		23
#define TIMER_DISP_PRJ			24
#define TIMER_DISP_SELECT		25
#define TIMER_DISP_CONSOLE		26
#define TIMER_DISP_MINIMAP		27
#define TIMER_MUSIC				28

//definitions for app run stages(07.01.2002)
#define APP_RUNSTAGE_INIT		0
#define APP_RUNSTAGE_PRESTART	1
#define APP_RUNSTAGE_MAINCYCLE	2
#define APP_RUNSTAGE_MAININIT	3	//mode which signals main loop to perform initialization (05.10.2004)
#define APP_RUNSTAGE_BRIEFING	7
#define APP_RUNSTAGE_POSTBRIEFING 8
#define APP_RUNSTAGE_MAINEXIT	4	//mode which signals main loop to deconstruct 
#define APP_RUNSTAGE_EXIT		5
#define APP_RUNSTAGE_ENDGAME	6   //game has ended, show a message to that effect but don't allow the game to continue

//define execution modes
#define EXECMODE_GTICK			0  //tick processes, game functions that occur in one tick
#define EXECMODE_GDRAW			1  //draw the display

//input packet types(05.20.2003)
#define IPACKETTYPE_OBJREF		0
#define IPACKETTYPE_PARAM		1
#define IPACKETTYPE_INVALID		6

//object and parameter types (05.20.2003)
//NOTE: these are the same as the flags specifying valid input packet types for command info requests
//(the values, there, specify the number of shifts right(>>) needed to access the bitflag specifying that particular field)
//(except for CDR_BITFLAG_PLURAL, which is needed for the a command data request to specify whether it accepts multiple instances of data in its request)
#define	OBJTYPE_UNIT			0
#define	OBJTYPE_STRUCTURE		1
#define OBJTYPE_COMPONENT		2
#define	OBJTYPE_MAPTILE			3
#define OBJTYPE_UTYPE			4   //08.12.2003  //needed to prevent clashing of parameters....yeah
#define OBJTYPE_STYPE			5	//08.12.2003
#define OBJTYPE_CTYPE			6	//08.12.2003
#define OBJTYPE_CTYPEGROUP		7	//08.20.2003
#define OBJTYPE_MATTYPE			8
#define IPARAMTYPE_FLOATS		9
#define IPARAMTYPE_DWORDS		10
#define IPARAMTYPE_LONGS		11	//signed as opposed to unsigned (05.30.2003)
#define IPARAMTYPE_STRING		12
#define CDR_BITFLAG_PLURAL		13
#define CDR_BITFLAG_INVALID		-1

//link types for the objectreport structure (01.01.2005 (sigh...))
#define OR_NUM_LINKTYPES		3
#define OR_LINKTYPE_SECTOR		0
#define OR_LINKTYPE_FULLR		1
#define OR_LINKTYPE_ATTACH		2

//task types
//LEVEL 1 (top level)
//1 Destroy Enemy 
//2 Defend Self
//3 Increase Infrastructure
//4 Gain Information
//LEVEL 2 
//5 Destroy Enemy Unit  (bottom)
//6 Destroy Enemy Structure (bottom)
//7 Guard Sector
//8 Guard Location (bottom)
//9 Capture Sector 
//10 Scout Sector (bottom)
//LEVEL 3
//8 Guard Location (bottom)
//6 Destroy Enemy Structure (bottom)
//11 Build Structure (bottom)

//AI task types
//new new
//rules: 1) number of roles needed by the task is in the 5th digit (ex. 3 roles = 30000)
//       2) a task that is merely intermediate (pre-cursor to choosing other tasks)
//          should be divisible by 10 (1st digit 0) (ex. 110)
//NOTE: I tried using "00100" and such to emphasize the
//tasks for which there were no roles (0 in the 5th digit),
//but this was interpreted as "0x100" (i.e. 64). Possible fix:
//just use hex for the numbers
#define AI_TASKTYPE_ATTACK_SECTOR	10101
#define AI_TASKTYPE_DEFEND_SECTOR	10102
#define AI_TASKTYPE_DEVELOP_SECTOR	100
    #define AI_TASKTYPE_ESTABLISH_CAPITAL	30105
    #define AI_TASKTYPE_ESTABLISH_OUTPOST   30106
#define AI_TASKTYPE_EXPLORE_SECTOR	10104
#define AI_TASKTYPE_ATTACK_SEED		10111
#define AI_TASKTYPE_DEFEND_SEED     10112
#define AI_TASKTYPE_DEVELOP_SEED    110
   #define AI_TASKTYPE_BUILD_STORAGE    11001
   #define AI_TASKTYPE_BUILD_FORGE      11002
   #define AI_TASKTYPE_BUILD_FACTORY    11003
   #define AI_TASKTYPE_BUILD_SHIPYARD   11004
   #define AI_TASKTYPE_BUILD_REFINERY   11005
   #define AI_TASKTYPE_BUILD_EXTRACTOR  11006
#define AI_TASKTYPE_EXPLORE_SEED	10113
#define AI_TASKTYPE_BUILD_UNIT 120
	#define AI_TASKTYPE_BUILD_UNIT_BASE	121	//build the simplest possible unit
	#define AI_TASKTYPE_BUILD_TANKER_A	122 //build a tanker (path A)

//AI Entity types
#define AI_ENTITY_TYPE_NONE			0
#define AI_ENTITY_TYPE_UNIT			1
#define AI_ENTITY_TYPE_STRUCT		2
#define AI_ENTITY_TYPE_SECTOR		3
#define AI_ENTITY_TYPE_SEED			4
#define AI_ENTITY_TYPE_TOPLEVEL		5

//define AI (new) decision threshold matrix indexes
#define AI_DEC_THR_NONE		0	 //doing nothing
#define AI_DEC_THR_HOST		1	 //hosting a task
#define AI_DEC_THR_JOIN		2	 //joined a task
#define AI_DECISION_THRESHOLD_RAISE_TASK	0.500 //(NONE->HOST)entity raises a task
#define AI_DECISION_THRESHOLD_JOIN_TASK		0.500 //(NONE->JOIN)entity joins a task
#define AI_DECISION_THRESHOLD_DISBAND_TASK	0.100 //(HOST->NONE)entity closes down the task it raised
#define AI_DECISION_THRESHOLD_LEAVE_TASK	0.100 //(JOIN->NONE)entity leaves the task it's joined
#define AI_DECISION_THRESHOLD_RENEGE_TASK	0.700 //(HOST->HOST)entity closes down the task it raised and raises a new one
#define AI_DECISION_THRESHOLD_SWITCH_TASK	0.700 //(JOIN->JOIN)entity switches to join a different task
#define AI_DECISION_THRESHOLD_HOST_TO_JOIN	0.700 //(HOST->JOIN)
#define AI_DECISION_THRESHOLD_JOIN_TO_HOST	0.700 //(JOIN->HOST)

#define AI_MODESTACK_MAX_MODES	5

//AI top-level modes
   //zero-parameter mode
#define AI_TOPLEVEL_MODE_NOTHING	0
#define AI_TOPLEVEL_MODE_DEFAULT	1
  //1-parameter modes
#define AI_TOPLEVEL_MODE_INTRASTRUCTURE  2 //focus resources on creating buildings parameter 1 is intensity of focus
  //2-parameter modes
#define AI_TOPLEVEL_MODE_DEVELOP_SECTOR  3  //parameter 1: intensity, parameter 2: intensity


#define AI_SECTOR_MODE_NOTHING	0

//number of IDs to sustain in the unitIDtable
#define UNIT_NUMIDS				5000
//03.28.2003
#define STRUCT_NUMIDS			5000
#define STRING_NUMIDS			5000	 //sigh, I'm giving IDs to STRINGS now?
#define CMP_NUMIDS				50000    //sigh, gotta do something about this

//definitions for unit "multitasking"
#define MODES_SIMUL_L1			3   //for now, allow only two modes(01.16.2004)

//definitions for unit mode numbers
#define UMODE_L1_IDLE			0
#define UMODE_L1_TRANSLATE		1
#define UMODE_L1_TURN			2
#define UMODE_L1_SCAN			3
#define UMODE_L1_ASCEND			4
#define UMODE_L1_DESCEND		5
#define UMODE_L1_FIRE_PROJECTILE			6  //06.21.2002  //differentiated 03.21.2003
#define UMODE_L1_AIM			7  //06.21.2002 (moves the unit towards attack target direction instead of course target)
#define UMODE_L1_ADD_FCMP		8	//base "build", to add fCompletion to the designated target index
#define UMODE_L1_INIT_CONSTRUCT 9	//initialize the construction with fCompletion 0
#define UMODE_L1_AIM_MULTI		10	//L1 modes for the aiming and firing of multiple weapons (01.08.2004)
#define UMODE_L1_FIRE_MULTI		11
#define UMODE_L1_TRANSLATE_AIR	12
#define UMODE_L1_TURN_SLOW		13	 //TEMP mode! to simulate the an airplane idling lazily around it's target
#define UMODE_L1_RETRAIN_ALL	14	//return weapons to their default position
#define UMODE_L1_NOTHING		15
#define UMODE_L2_IDLE			0
#define UMODE_L2_MOVE		    1
#define UMODE_L2_ATTACK_PROJECTILE		2  //differentiated attack procedures(03.21.2003)
#define UMODE_L2_ATTACK_MULTI			3  //a fundamental limitation of the Director Engine is the lack of individual "AI" for components
#define UMODE_L2_ATTACK_MULTI_AIR		4	
#define UMODE_L2_MOVE_AIR		5			   //special "air" mode reflecting the planned different approach to air movement (01.16.2004)
#define UMODE_L2_HOLD			6			  //what a unit does if it cannot stop 
#define UMODE_L2_NOTHING		7			   //consequently, the only way to operate multiple weapons on a unit simultaneously is to fire them all at once, and so this a very special mode designed to do that (01.07.2004)	
#define UMODE_L3_IDLE			0
#define UMODE_L3_MOVE			1
#define UMODE_L3_MOVE_AIR		2		//special "air" mode reflecting the planned different approach to air movement (01.16.2004)
#define UMODE_L3_ATTACK			3
#define UMODE_L3_ATTACK_MULTI			4	//as of now, this mode simply hands the ATTACK MULTI mode to L2 (01.08.2004)
#define UMODE_L3_ATTACK_MULTI_NAVAL		5	//this is kind of a "special" mode(okay, a HACK), that orients a ship + or - 90 degrees vs direction to target (thus it looks a bit more realistic)
#define UMODE_L3_APPROACH		6
#define UMODE_L3_NOTHING		7
#define UMODE_L4_IDLE			0
#define UMODE_L4_ATTACK			1
#define UMODE_L4_ATTACK_MULTI	2	//why there is a "multi" version all the way up to L4 is because L4 does range-checking--and this is dependent on the weapon used
#define UMODE_L4_BUILD			3  //Order to move into position to begin building, and then to begin building
#define UMODE_L4_LOAD			4  //order to load into a another unit
#define UMODE_L4_UNLOAD			5	//order to unload a group of units
#define UMODE_L4_LOAD_MATERIAL	6   //order to load material from the specified location
#define UMODE_L4_UNLOAD_MATERIAL 7  //order to unload material to the specificed location
#define UMODE_L4_NOTHING		8  //L4 mode which literally does nothing, including not trying to set the L3 Mode (06.22.2002)
#define UMODE_L5_IDLE			0
#define UMODE_L5_GUARD			1
#define UMODE_L5_TRANSFER_MATERIAL 2
#define UMODE_L5_NOTHING		3
#define UMODE_L6_IDLE			0
#define UMODE_L6_DISTRACTED		1
#define UMODE_L6_NOTHING		2

// a note on the UMODE_L2_ATTACK_MULTI_WEAPON define above....
//the unit will have a data structure, probably an array of pointers to all available weapons on the unit...
// at some point a user might be able to toggle which weapons to use in multi weapons fire, but right now we'll just fire all of them.
// in this mode a unit will approach to the range at which all weapons can hit their target, then begin training weapons to target
// we might have each weapon begin firing as soon as they are ready, or we might wait until all weapons are ready to being firing,
// don't know.

//definitions for unit types (relegated to allowing for "predefined" unit component configurations)
#define UTYPE_NUMTYPES			1000	//number of unit types
#define UTYPE_NOTHING			0   //defines "attach no components" (03.21.2003)
#define UTYPE_TEST00			1   //06.14.2002
#define UTYPE_TEST01			2
#define UTYPE_TEST02			3
#define UTYPE_TEST03			4   //01.06.2004
#define UTYPE_TEST04			5

//definitions for projectile types
#define PTYPE_NUMTYPES			500	//number of projectile types
#define PTYPE_TEST00			0	//06.14.2002
#define PTYPE_TEST01			1	//03.21.2003
#define PTYPE_TEST02			2	//03.26.2003
#define PTYPE_NAVAL120			3	//"Naval 120mm" "shell"  (01.08.2004)
#define PTYPE_AIRGUN			4  //a test "airgun" projectile

//definition for raw materials types
#define NUM_MATERIAL_TYPES		40
#define MATERIAL_ELEMENT_A		1
#define MATERIAL_ELEMENT_B		2
#define MATERIAL_ELEMENT_C		3
#define MATERIAL_ELEMENT_D		4
#define MATERIAL_ELEMENT_E		5
#define MATERIAL_ELEMENT_F		6
#define MATERIAL_ELEMENT_G		7
#define MATERIAL_ELEMENT_H		8
#define MATERIAL_ELEMENT_I		9
#define MATERIAL_ELEMENT_J		10
#define MATERIAL_ELEMENT_K		11
#define MATERIAL_ELEMENT_L		12
#define MATERIAL_ELEMENT_M		13
#define MATERIAL_ELEMENT_N		14
#define MATERIAL_ELEMENT_O		15
#define MATERIAL_ELEMENT_P		16
#define MATERIAL_ELEMENT_Q		17
#define MATERIAL_ELEMENT_R		18
#define MATERIAL_ELEMENT_S		19
#define MATERIAL_ELEMENT_T		20
#define MATERIAL_ELEMENT_U		21
#define MATERIAL_ELEMENT_V		22
#define MATERIAL_ELEMENT_W		23
#define MATERIAL_ELEMENT_X		24
#define MATERIAL_ELEMENT_Y		25
#define MATERIAL_ELEMENT_Z		26

//definitions for pathfinding lengths
#define PF_SEARCHDEPTH			1000
#define PF_SEARCHDEPTH_SHORT	20      //05.15.2002 //shortened even more aggressively until maybe some kind of target adjustment scheme can be set up(06.27.2002)

//definitions for interface modes (03.20.2002)
#define IFCM_SELECTION			0		//awaiting selection of units to command
#define IFCM_UCOMMAND			1		//awaiting commands to be issued to selected 
#define IFCM_SCOMMAND			2
#define IFCM_SELECT_EXTENDED	3		//(04.03.2003 awaiting supply of secondary information to pending command)
#define IFCM_VIEW				4		//viewing information 

//definitions for interface grids(05.06.2002)
#define IBOX_NUMBOXES			14   //the number of boxes ... i'm not entirely sure if this necessitates an entire define
#define IBOX_PRIMARY			0	//the mapscreen itself
#define IBOX_BOTTOM				1
#define IBOX_RIGHT				2
#define IBOX_CORNER				3
#define IBOX_MAINMENU			4   //main menu (07.04.2002)
#define IBOX_NOWHERE			-1	//just in case the mouse ends up anywhere that isn't covered by an interface grid
#define IBOX_MINIMAP			5
#define IBOX_MINIMAPZOOM		6
#define IBOX_CONSOLE_EQUIP		7   //lower left, the "equip" box (03.12.2003)
#define IBOX_CONSOLE_LIST		8	//lower right, the "list access" box (03.12.2003)
#define IBOX_CONSOLE_COMMAND	9	//corner lower right, the general command box (03.12.2003)
#define IBOX_CONSOLE_INFO		10  //just above command box, the general information box
#define IBOX_CONSOLE_DISPLAY    11  //bottom information display
#define IBOX_CONSOLE_CHAT		12
#define IBOX_NOTIFY				13

#define NUM_IFCRECTS			9
#define IFCRECT_TOPLEFT			0
#define IFCRECT_TOPRIGHT		1
#define IFCRECT_BOTTOMRIGHT		2
#define IFCRECT_BOTTOMLEFT		3
#define IFCRECT_LEFT			4
#define IFCRECT_RIGHT			5
#define IFCRECT_TOP				6
#define IFCRECT_BOTTOM			7
#define IFCRECT_CENTER			8

//definitions for IGRIDCELL drawtypes(07.08.2002)
#define IGRIDCELL_DRAWTYPE_TEXT		0
#define IGRIDCELL_DRAWTYPE_TEXTHDC	1
#define IGRIDCELL_DRAWTYPE_BLIT		2
#define IGRIDCELL_DRAWTYPE_BLITTEXT	3   //draws both text and blit(blit first)
#define IGRIDCELL_DRAWTYPE_TEXTWIDE 4   //draws wide text
#define IGRIDCELL_DRAWTYPE_BLITFIT	5   //06.03.2003
//definitions for IGRIDCELL datasource types
#define IGRIDCELL_DSOURCE_ALLCELL	0
//with the addition of a COMMAND2 command storage type for IGRIDCELLs, DEFINES for their data sources needed
//too(03.28.2003)
#define IGRIDCELL_DSOURCE2_ALLCELL	0	//the PCMD2 command issued uses a direct copy from the data in the buffer


//definitions for DirectPlay send data types    //07.22.2002
#define DPSENDTYPE_COMMAND			168
#define DPSENDTYPE_COMMAND2			169	//01.01.2003
#define DPSENDTYPE_CHATTEXT			141
#define DPSENDTYPE_RANDKEYS			136  //a message sent by host to synchronize randkeys 
										 //all connected computers

//add conditions for the commqueue(01.02.2003)
#define COMM_ADDCONDITIONS_ICONTROL_ONLY	1


//define the number of pixels a drag can go before it becomes a group-select (mouse....temp?) 05.13.2002
#define MSELECT_DRPIXELS	    8

#define FINDTANKMODE_PUTINTO	0
#define FINDTANKMODE_TAKEFROM	1

//define the maximum number of info requests possible for a pending command
#define MAX_COMMINFO_REQUESTS	5

//04.03.2003
#define NUM_CI_REQTYPES			1


//string indexing!
#define STRI_BUILD_UNIT			0
#define STRI_BLANK				1
#define STRI_BUILD_COMPONENT	2
//just an idea for "fun" ... starting at index 4000 and above are ten categories of random names for the purpose of naming things
//for now, each nation has access to a name group corresponding to their national ID

//tutorial messages
#define TUTORIAL_MSG_WELCOME 0


//TIMER de-sync retry
#define TIMER_DESYNC_RETRY  100 //1 million times


//TEMP lockset for unit speeds
//obsolete 03.21.2003
//#define TEMP_USPEED				1  //temporary until unit type information structures can be created (06.13.2002)
//#define TEMP_PSPEED				5  //temporary until projectile type information is used (06.21.2002)
//other TEMPS (mainly/probably because I don't feel like filling out the type tables yet (06.21.2002)
//#define TEMP_PREMRANGE			120
//#define TEMP_FIRETIME			100	//fire time, 100 ticks(12.14.2002) //obsolete
#define TEMP_SCANTIME			30 //180 ticks
// #define TEMP_UHP				200 //obsolete
//#define TEMP_PDAMAGE			10
//#define TEMP_UGRAPHICSIZE		24   //temporary spec for the size of projectile and unit graphics...will be held individually by  // obsolete!
#define TEMP_PGRAPHICSIZE		24	 //each unit and projectile type once the info tables are up(06.26.2002)
#define TEMP_DP_MAXHOSTS		10   //the maximum number of hosts that can be found...(not likely to be a problem)
#define TEMP_DP_MAXPLAYERS		8
#define TEMP_IGRID_SINGLE		1
#define TEMP_IGRID_MULTI		2
#define TEMP_IGRID_MULTI2A		3						
#define TEMP_IGRID_OPTIONS		4
#define TEMP_IGRID_MULTI2		5    //the 2nd multiplayer screen
#define TEMP_IGRID_MULTI2B		6
#define TEMP_IGRID_SERIAL		7
#define TEMP_IGRID_MODEM		8
#define TEMP_IGRID_TCPIP		9
#define TEMP_IGRID_MULTI3		10
#define TEMP_IGRID_BRIEFING		11
#define TEMP_IGRID_MULTI4		12   //the map select screen for multiplayer
#define TEMP_STD_STRLEN			80
#define TEMP_CB_TICKCOUNT		300	// = 5 seconds
#define TEMP_DEFAULT_LATENCY	17
#define TEMP_DEFAULT_TICKREPORT_INTERVAL	5 
#define TEMP_DEFAULT_MAXLAG		15
#define TEMP_SCANRANGE			10   //10 squares in each direction
#define TEMP_UCREATETIME		100	//5000 ticks
#define TEMP_UROTATESPEED		5.625 // 5.625 degrees (temp until we put this information in a table, which i'm afraid to do right now ) 01.07.2004
#define TEMP_DEFAULT_STC		100	  //default "sector tick count"
#define TEMP_ASSEMBLECOSTRATIO  0.2   //how much it costs to assemble a group of components, as a percentage of the total component cost (06.11.2004)
#define TEMP_STYPE_RSC			33
#define TEMP_STYPE_OUTPOST		34
#define TEMP_STYPE_TANK			35
#define TEMP_STYPE_FACTORY_ABC	36
#define TEMP_STYPE_REACTOR		37
#define TEMP_STYPE_REFINERY		38
#define TEMP_STYPE_FORGE		39
#define TEMP_STYPE_SHIPYARD		40
#define TEMP_STYPE_EXTRACTOR	41
#define TEMP_MAXALPHASIZE		100 //the maximum size of an alpha blit
#define TEMP_SCROLLBARINWIDTHS	3
#define TEMP_CMPDEFOFFSET		1
#define TEMP_SIMULSOUNDS_MAX	5  //12.25.2004
#define TEMP_BUILDRATE			0.01
#define TEMP_TICKSJAMMED_LIMIT	60
#define TEMP_TIMER_ATTACK_RESCAN 100
#define TEMP_DP_STATICBUFFERSIZE	50
#define TEMP_DP_SMBMAXSIZE 1000
#define TEMP_BLITQUEUE_SLOTS	4000
#define TEMP_CLAIMRADIUS_UNITS	2
#define TEMP_CLAIMRADIUS_CAPITAL 12
#define TEMP_CLAIMRADIUS_OUTPOST 6
#define TEMP_UNIT_REPATH_INTERVAL  120  //ticks
#define TEMP_PATHFINDER_CRAPPY_PATH_THRESHOLD 5.0
#define TEMP_MATERIAL_ACCUMULATE_MASTER_RATE  100000.0
#define TEMP_DISPERSE_MATERIALS_INTERVAL  5
#define TEMP_ATTACKED_UNIT_DISTRACT_TICKS 600
#define TEMP_AITASK_MAX_ROLES	10
#define TEMP_SECTOR_TACTICAL_INFO_UPDATE_RATE_BASE    60   //1 seconds
//define a GUID (added 06.27.2002)
//DEFINE_GUID(APP_ST_GUID, 0xca761230, 0xed42, 0x11ce, 0xba, 0xcd, 0x0, 0xaa, 0x0, 0x57, 0xb2, 0x23);

// {BF9BBB5D-54D0-453b-A4B2-0082CAB59598}  -> (06.27.2002) created from GUIDGEN
static const GUID APP_A1_GUID = 
{ 0xbf9bbb5d, 0x54d0, 0x453b, { 0xa4, 0xb2, 0x0, 0x82, 0xca, 0xb5, 0x95, 0x98 } };


//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//

//EXREMELY puzzled as to why this works, as opposed to commenting it out doesn't work (03.08.2004)
//oh well....it's for debugging anyway
//12.17.2004 not sure what the above is about...it won't compile because this class is passed to cClassAccess
class cDbgMem
{
public:
	void Write_Log_H(LPSTR Text, HANDLE hLog);
	void WriteLog_Enter_H(HANDLE hLog);
	void MemCheck(void* ptr, HANDLE hLog);

};
//################################################
//################################################

//structures
typedef struct _COORDSET //a structure containing coordinates (I'm not sure whether or not to use this - 01.19.2002)
{
	float  x;
	float  y;
	float  z; //yes, z (01.16.2004)

} COORDSET, * LPCOORDSET;

class SQCOORDSET //a special coordset class containing coordinates and mapindex information(I'm not sure whether or not to use this - 01.19.2002)
{
	public:
		int	x;
		int y;
		int index;
		void Calc_Index(int iMapWidth) { index = (y * iMapWidth) + x; }                   //calculates the index based on the x and y
		void Calc_xy(int iMapWidth) {  x = index % iMapWidth;  y = index / iMapWidth; }    //calculates the x and y based the index
		void Set_xy(int ix, int iy) { x = ix; y = iy; } //sets the x and y values
	private:
};

class SQCOORDSET_L //extension of the square coordset class that allows linking into lists(currently only used for cStructureA class
{
public:
		int	x;
		int y;
		int z; //01.16.2004  (floating structures? haha)
		int index;
		void Calc_Index(int iMapWidth) { index = (y * iMapWidth) + x; }                   //calculates the index based on the x and y
		void Calc_xy(int iMapWidth) {  x = index % iMapWidth;  y = index / iMapWidth; }    //calculates the x and y based the index
		void Set_xy(int ix, int iy) { x = ix; y = iy; } //sets the x and y values
	
		void*	pPrev;
		void*	pNext;
	
private:


};

typedef struct MTILE  //maptile 
{
	SQCOORDSET Pos;  //it's own psoition

	int		iType;         //tile type
	int		iSector;	   //the sector this maptile belongs to
	int     iPossessingStructID; 
	
	//int     iLastParticipatedJobNumber; //temporary value that is used externally to find connected tanks
	int		iLastParticipatedJobNumber;
	
	DWORD		barrRevealed;	//

	DWORD   barrOverlay;   //bit-array of possible overlays on this tile 

	VOID*	pOccupant_Unit;   //pointer to the linked list of units in this tile (the type is VOID because this dec is before the cUnitA definition, it can just be cast later)
	VOID*   pOccupant_Structure; //pointer to the linked list of structures in....
	int		iDrawFrame;	   //frame of graphic to draw for this tile
	int		iDrawFrame_Layer2;    //surface for layer2 overdraw graphics on the tile
	int		iDrawFrame_LinkGraphic; //determine which frame draws the correct link graphic
	
	char	bIMTargeted_Nation;		//the numerical index of the nation whose units have IM-targeted this tile (05.20.2002)
	WORD	wIMTargeted_Count;		//the number of units that have IMtargeted this tile (05.20.2002)
	int		iControl; //the group that controls this tile (09.20.2006)
	int		iControlLevel; //the priority of the hold that is being exerted on this tile by the controller (09.20.2006)

	MTILE* pParent;
	MTILE*	pPrev;
	MTILE*  pNext;

	MTILE* pSectorList_Prev;  //link to next maptile in the same sector
	MTILE* pSectorList_Next;  //link to previous maptile in the same sector

	double fEval;
	double fDistFromHome;
	double fDistFromTarget;

	MTILE() { iControl = -1; iControlLevel = -1;  pOccupant_Unit = NULL; pOccupant_Structure = NULL; iLastParticipatedJobNumber = 0; pSectorList_Prev = NULL; pSectorList_Next = NULL;}	

} MTile, *LPMTile;

class cPermission
{
public:
	int iPermission;
	cPermission* pPrev;
	cPermission* pNext;

private:


};
class cPermissionBlock
{
public:
	cPermission* pP_First;
	cPermission* pP_Last;

	cPermissionBlock() 
	{
		pP_First = new cPermission;
		pP_Last = new cPermission;
		pP_First->pPrev = NULL;
		pP_First->pNext = pP_Last;
		pP_Last->pPrev = pP_First;
		pP_Last->pNext;
	}
	~cPermissionBlock()
	{
			Clear(); delete pP_First; delete pP_Last;
	}
	void Clear()
	{
		cPermission* pTrav = pP_First->pNext;
		cPermission* pTrav2;
		while (pTrav != pP_Last)
		{
			pTrav2 = pTrav->pNext;
			delete pTrav;
			pTrav = pTrav2;
		}
		pP_First->pNext = pP_Last;
		pP_Last->pPrev = pP_First;
 	}
	BOOL Check_Permissions(cPermissionBlock* pRequired_Permissions)
	{
		//this is a double loop returns TRUE if this permission block has
		//all the required permission
		//returns FALSE if it doesn't
		cPermission* pTrav_Have = pP_First->pNext;
		cPermission* pTrav_Need = pRequired_Permissions->pP_First->pNext;

		while (pTrav_Need != pRequired_Permissions->pP_Last)
		{
			BOOL bFoundFlag = FALSE;

			pTrav_Have = pP_First->pNext;
			while(pTrav_Have != pP_Last)
			{
				//if we found something we can break out of the loop
				if (pTrav_Have->iPermission == pTrav_Need->iPermission)
					bFoundFlag = TRUE;

				pTrav_Have = pTrav_Have->pNext;
			}
			//if we didn't find anything here, we can give up
			if (bFoundFlag == FALSE)
				return FALSE;

			pTrav_Need = pTrav_Need->pNext;
		}
			
		//if we get here, we're in the clear, we found everything
		return TRUE;
	}
	int Remove_Permission(int iPermission)
	{
		cPermission* pTrav = pP_First->pNext;
		while (pTrav != pP_Last)
		{
			if (pTrav->iPermission == iPermission)
			{
				cPermission* pPrev = pTrav->pPrev;
				cPermission* pNext = pTrav->pNext;
				pPrev->pNext = pNext;
				pNext->pPrev = pPrev;
				delete pTrav;
				return 0;
			}
			pTrav = pTrav->pNext;
		}
		//otherwise we didnt find it
		return -1;
	}
	void Add_Permission(int iPermission)
	{
		cPermission* pNew = new cPermission;
		pNew->iPermission = iPermission;
		cPermission* pNext = pP_Last;
		cPermission* pPrev = pP_Last->pPrev;
		pNext->pPrev = pNew;
		pPrev->pNext = pNew;
		pNew->pNext = pNext;
		pNew->pPrev = pPrev;
	}

private:

};
class cMaterialCollection
{
public:
	cMaterialCollection() 
	{ for (int lc1 = 0; lc1 < NUM_MATERIAL_TYPES; lc1++)
	{
		Materials[lc1] = 0;
	}
	}
	double Materials[NUM_MATERIAL_TYPES];

	void Add(cMaterialCollection Other)
	{
		for (int lc1 = 0; lc1 < NUM_MATERIAL_TYPES; lc1++)
			Materials[lc1] += Other.Materials[lc1];
	}
	void Subtract(cMaterialCollection Other)
	{
		for (int lc1 = 0; lc1 < NUM_MATERIAL_TYPES; lc1++)
			Materials[lc1] -= Other.Materials[lc1];
	}

private:
};

typedef struct _PCMD    //"program command"
{
	 WORD		wDest;    //destination of command(interface, gameworld, sound etc.)
	 WORD		wType;	  //type of command(ex. move, scroll, etc.)
	 LONG		wParam1;  //parameter 1 of issued command
	 WORD		wParam2;  //parameter 2 of issued command
	 WORD		wParam3;  //parameter 3 of issued command
	 double		fParam4;  //parameter 4 of issued command
	 double		fParam5;  //parameter 5 of issued command
	 void*		pParam6;  //parameter 6 of issued command...a pointer, for local commands only
	 LONG		lExecTick;  //if a GAMEWORLD command, the tick count at which this command will be executed
	 VOID*		pNext;	  //next node in command chain
	 VOID*		pPrev;	  //previous node in command chain 

} PCMD, * LPPCMD;

struct CommDataDrawText2
{
	DWORD dwUnicodeFlag;
	DWORD dwRenderTargetCode;
	DWORD dwXPos;
	DWORD dwYPos; 
	DWORD dwXLimit;
	DWORD dwFontType;
	DWORD dwFontColor;
	char* Text;
};

class PCMD2   //"program command, second version"  (needed to reduce command size to reduce multiplayer network congestion(?) 01.01.2003
{
public:
//	WORD		wType;
//	BYTE*		pData;
//	WORD		wDataSize;	//size of the data pointed to by pData, in bytes 
//	VOID*		pNext;
//	VOID*		pPrev;
	//WORDs changed to shorts (03.05.2004)
	short		wType;
	BYTE*		pData;
	short		wDataSize;	//size of the data pointed to by pData, in bytes 
	PCMD2*		pNext;
	PCMD2*		pPrev;

//	BYTE		PadBlock[CQ2_ENDPADBYTES]; //03.10.2004

private:


};
//note: using 


//08.07.2003
//structure currently used by structures(double entendre noticed) to queue build/repair orders
//this is on top of any bMode abilities a structure might have
class cProgressiveOrder
{
public:
	PCMD2*	pCmd2_Init;	//Command (or even list of commands) to execute upon initialization of the PO
	PCMD2*	pCmd2_Prog;	//The "progressive" part of the order. The command(or list of commands) that is executed each time the caller attempts
						//to add "progress" to the completion of the order.
	PCMD2*	pCmd2_Exit;	//Command ( or even list of commands) to execute upon completion of the PO

	double	fCompletion; //"completion" of this order

	char*	pName;		//needs a name! :-)  08.12.2003

	void*	pCAccess;
	
	void*	pPrev;
	void*	pNext;

	void	Init( VOID* pCA );  //Exit() and Init() do not issue Cmd_Init and Cmd_Exit commands, as it might seem
	void	Flush_List(int iDest);  // 0 = INIT, 1 = PROG, 2 = EXIT
	void	Exit();   //Exit() and Init() do not issue Cmd_Init and Cmd_Exit commands, as it might seem
	void	Add_Cmd2(PCMD2* pCmd2, BOOL bCopyFlag, int iDest); //0 = INIT, 1 = PROG, 2 = EXIT

};

//08.07.2003
//simply a class to store a list of ProgressiveOrders
class cProgOrderBuffer
{
public:
	cProgressiveOrder*	pPO_First;
	cProgressiveOrder*	pPO_Last;
	
	void*	pCAccess;

	int iNumOrders;

	void Init(VOID* pCA);
	void Flush_Buffer();
	void Add_PO(cProgressiveOrder* pPO_New, BOOL bCopyFlag);
	void Delete_PO(cProgressiveOrder* pPO_Kill);
	void Exit();
};

typedef struct _RANDGENKEYDATA   //self-explanatory    (data to load to the random number generator) //07.15.2002
{
	LONG		lGroup1[100];    //100 numbers in the first group
	LONG		lGroup2[99];		//99 numbers in the second group
	LONG		lGroup3[98];			//98 numbers in the third group
	LONG		lGroup4[97];		//97 numbers in the 4th group

} RANDGENKEYDATA, * PRANDGENKEYDATA;

//an abstraction of a command to draw something to screen
//instead of flat-out calling draw functions, code can queue a draw order
//to be executed later.
//further, this will allow one to select which graphics library(DirectDraw or Direct3D, or other),
//and easily interchange them with minimal coding or option setting
//(in other words, this is to add one further layer of abstraction to the drawing process)
//ACK! won't completely work in it's current implementation! (09.26.2003)
//(because GDisplay currently uses some handling of DD7 surfaces in order to draw some stuff
//to screen)
//specifically, map draw and text draw functions use intermediate surfaces which defy the 
//ability of the "DrawIssue" idea to replicate in any simple fashion
//also, they use colorkeying effects which I currently don't know how to replicate
//in D3D at all....
class cDrawIssue
{

public:
	
	int iType;		//ex. BltFast, Blt, D3DTexture ... etc.

	//general 
	DWORD	dwParameters1;
	DWORD	dwParameters2;

	//blt/bltfast
	RECT SourceRect;
	int iXDest;
	int iYDest;
	LPDIRECTDRAWSURFACE7  pDDS_Source;
	LPDIRECTDRAWSURFACE7  pDDS_Dest; 
	RECT DestRect;
	
	int	iSourceIndex;
	int iSourceType;
	DWORD dwColorKey;

	void* pPrev;
	void* pNext;

private:

};

class cDrawIssueBuffer
{

public:

	cDrawIssue* pDIB_First;
	cDrawIssue* pDIB_Last;

	void Init();
	void Exit();
	cDrawIssue* Create_Add_Node(RECT DestRect,
								RECT SourceRect,
								int iSourceIndex,
								int iSourceType,
								DWORD dwColorKey);
	void Add_Node(cDrawIssue* pNodeNew, BOOL bCopyFlag);
	void Clear_List();
	void Delete_Node(cDrawIssue* pNode);

private:

};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//cconfig holds tools for loading things from configuration files (07.07.2004)
class cConfig
{
public:

	//some configuration options (it's responsibility accidentally overlap a bit over cGameOptions (:-/)
	int iTempStore_DDRenderMode;
	int iTempStore_MMUpdateTime;
	BOOL bTempStore_TimerThread;
	BOOL bTempStore_ForceCore0;

	BOOL bTextZero;

	void*	pCAccess;
	HANDLE	hInfile;
	HANDLE	hLogFile;

	void Init(void* pCA);
	void Load_Data(char* LoadFileName);
	void Read_To_Stop(char* LoadBuffer, char stopchar);

	void Init_LogFile();
	void Write_Log(LPSTR Text);
	void WriteLog_Enter();
	

private:


};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
class cFontInfo
{
	public:
		int	iFontISurfIndex;    //image surface where the font graphics are
		int	iHeight;			//height of this font
		int iWidth;				//width of this font
	
		int CharacterOffsetTable[256];

		DDSURFACEDESC2			BlitMaskDesc;   //:-)  03.24.2003
		DDCOLORKEY				DDCK_BlitMask;
		LPDIRECTDRAWSURFACE7	pDDS_BlitMask;
		LPDIRECTDRAWSURFACE7    pDDS_BlitMask_Colorstore;  // 04.17.2003				
												 //the goal here is to attempt
												//to cut down the number of 
												//colorfills attempted to the
												//minimum possible

	private:

};

class cFontInfoTable
{

	public:
		cFontInfo	Fonts[FONT_NUMFONTS];  //might as well simply set this as 100, i don't expect any problems with allocation

		VOID*		pCAccess;

		int TextLen_Pixels(char* Text, int iStrlen, int iFont);
		void SetFontColor(DWORD dwColor);
		void Add_Font(int iImageSurf, int iFontIndex, int iHeight, int iWidth, BOOL bSysMemFlag, LPDIRECTDRAW7 pDD7);
		void Init(VOID* pCA);
		void Exit();
		void Trim1(int* pOffsetTable);

	private:

};

//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
class cDD_Display
{

	public:
		BOOL bFlippable;  //we can flip right now
		LPDIRECTDRAW7			pDD7;
		LPDIRECTDRAWSURFACE7	pDDSMain;
		LPDIRECTDRAWSURFACE7	pDDSBack;
		LPDIRECTDRAWSURFACE7	pRenderBuffer; //system memory surface (01.16.2004)
		
		LPDIRECTDRAWSURFACE7	pPrimaryRenderTarget;
		LPDIRECTDRAWSURFACE7	pSecondaryRenderTarget;

		LPDIRECTDRAWSURFACE7	pDDS_Scratch; //scratch surface used for miscellaneous purposes (for now, simply be 1x1 pixel!)
		DDSURFACEDESC2			DDSDscratch;

		LPDIRECTDRAWSURFACE7	pDDS_Alpha;
		DDSURFACEDESC2			DDSDalpha;

		DDCOLORKEY				DDCKEmblem;	//DESTINATION color key used to draw emblems! (06.23.2004)

		LPDIRECTDRAWSURFACE7	pDDSBack2;  //back drawing surface for alternate "straight blit" display 02.02.2002

		DDSURFACEDESC2			DDSDmain;
		DDSCAPS2				DDSCAPSback;
		DDSURFACEDESC2			DMDesc; //structure used to store display mode

		int						iDDRenderMode;

		//store for surfaces to look at
		int						sysmem_flag_normal;
		int						sysmem_flag_alpharender;
		int						sysmem_flag_menu;

		DWORD					AMask;
		DWORD					wRBitMask;
		DWORD					wBBitMask;
		DWORD					wGBitMask;
		BYTE					bRShift;
		BYTE					bBShift;
		BYTE					bGShift;

		int						iXRes;
		int						iYRes;
		int						i_XResD; // (10.02.2004) if necessary, ignore part of the screen to preserve aspect ratio!
		int						i_YResD; // (10.02.2004) if necessary, ignore part of the screen to preserve aspect ratio!
		int						iBPP;
		int						iResProduct; //i_XResD * i_YResD

		//temporary holds for values if user puts in resolution values
		int						iXR_TempStore;
		int						iYR_TempStore;
		int						iBPP_TempStore;

		HDC						hdc00;
		DDBLTFX					ddblitfx;

		DDCAPS_DX7				SystemCaps;

		VOID*					pCAccess;

		void Init(HWND hMainWnd, VOID* pCA);
		void Exit();
		~cDD_Display();
		cDD_Display();

		void Draw_Text_HDC(LPDIRECTDRAWSURFACE7 pDDS, char* Text, int xpos, int ypos);
		void Colorfill(LPDIRECTDRAWSURFACE7 pDDS, BYTE bRcolor, BYTE bGcolor, BYTE bBcolor);
		void Colorfill2(LPDIRECTDRAWSURFACE7 pDDS, DWORD dwColorIndex);
		void Colorfill3(LPDIRECTDRAWSURFACE7 pDDS, DWORD dwColorIndex, int iTop, int iBottom, int iLeft, int iRight);

		void Change_Resolution(int xr, int yr, int bpp);

		void Init_LogFile();
		void Write_Log(LPSTR Text);
		void WriteLog_Enter();
		void Exit_LogFile();

	private:
		HANDLE					hLogFile; //handle for the debugging logfile	
		int						c1, c2;   //counters
		RECT					TextSrcRect; //rect for text blitting purposes
		int						text_xdraw, text_ydraw;  //vars for text blitting purposes 
		int						iTextDrawFrame; //int which holds drawframe for text (1.03.2002)

};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
/*

//Vertex format (09.20.2003)
struct d3dvertex{
    FLOAT x, y, z, rhw; // The transformed position for the vertex.
    DWORD color;        // The vertex color.
};

//2nd texture format (9.20.2003)
struct d3dvertex2{
    FLOAT x, y, z;   // The untransformed position for the vertex.
    FLOAT tu, tv;    // The texture coordinates
};



class cD3D_Display
{


public:
	IDirect3D9*			pD3D9;
	IDirect3DDevice9*	pDevice;
	
	D3DPRESENT_PARAMETERS*  pPP;
	
	D3DFORMAT			Format;

	D3DDISPLAYMODE		CurrDisplayMode;

	d3dvertex2*				VertexData;	//array of vertices to put into vertex buffer
	IDirect3DVertexBuffer9*	pVertexBuffer;   //the vertex buffer...

	BYTE*					pBuffer;	//pointer to point to locked vertex buffer

	void*				pCAccess;

	void	Init(VOID* pCA, HWND hMainWnd);

	void	Exit();

	HANDLE	hLogFile;


		void Init_LogFile();
		void Write_Log(LPSTR Text);
		void WriteLog_Enter();
		void Exit_LogFile();



private:







};

*/
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
class cDI_Input
{

public:
	LPDIRECTINPUT8			pDI8;
	LPDIRECTINPUTDEVICE8	pDID8_Mouse;
	LPDIRECTINPUTDEVICE8	pDID8_Keyboard;

	DIMOUSESTATE2			DIState_Mouse_Curr;
	DIMOUSESTATE2			DIState_Mouse_Prev;
	BYTE					DIState_Keyboard_Curr[256];
	BYTE					DIState_Keyboard_Prev[256];

	void Init(HINSTANCE hInst_App, HWND hMainWnd);
	void Exit();
	void Get_MouseState(); //updates mouse states
	void Get_KeyboardState(); //updates keyboard states
	void Get_InputState(); //updates both mouse and keyboard states
	~cDI_Input();

	void Init_LogFile();
	void Write_Log(LPSTR Text);
	void WriteLog_Enter();

private:

		int						c1, c2; //counters
		HANDLE					hLogFile; //handle for the debugging logfile
	
};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//02.08.2003
class cDA_Audio
{
	public:

		VOID*		pCAccess;
		

		void Test_Play();
		void Play_Primary(int iSoundSegIndex, long fVolume);
		void Play_Primary(int iSoundSegIndex);
		void Play_Secondary(int iSoundSegIndex);  //? 02.08.2003
		void Play_Secondary(int iSoundSegIndex, long fVolume);  //? 02.08.2003
		void Stop_Media(int iSoundSegIndex);
		
		BOOL bIsPlaying(int iSoundSegIndex);

		int Determine_Standard_Volume(int iX, int iY);

		void Init(VOID* pCA, BOOL bSecByPass);
		void Exit();
		
		void WriteLog_Enter();
		void Init_LogFile();
		void Write_Log(LPSTR Text);

	private:

		HANDLE					hLogFile; //handle for the debugging logfile		

};


//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//

//can't put in a class....(06.27.2002)
HRESULT WINAPI DP8_Handle_Message(PVOID pvUserContext, DWORD dwMessageID, PVOID pMessage);

//obsolete 12.16.2002
typedef struct _DPCONNECTREPLYDATA  //data transferred from host to peer when peer joins a session
{
	RANDGENKEYDATA	RandKeys;
	WORD			wNumPlayers;   //number of players currently in play(excluding the joining peer), which
								   //effectively can become the control ID of the joining peer
} DPCONNECTREPLYDATA, * PDPCONNECTREPLYDATA;

typedef struct _DPHOSTENUMINFO //create a custom host enum info structure (06.30.2002)
{
	DPN_APPLICATION_DESC*	pAppDesc;
    IDirectPlay8Address*	pHostAddr;
    IDirectPlay8Address*	pDeviceAddr;

} DPHOSTENUMINFO, * PDPHOSTENUMINFO;

typedef struct _DPDEVICEINFOARRAY
{
		DPN_SERVICE_PROVIDER_INFO*  SPDevices;
		int							wActiveDevice;
		DWORD						dwNumDevices;
		DWORD						dwDeviceBufferSize;
} DPDEVICEINFOARRAY, * LPDPDEVICEINFOARRAY;

typedef struct _DPSPCONNECTINFO   //connect info for service providers(baud, conn. flow, IP address, etc.) 07.14.2002
{ //an array of these, one per SP, with so each SP will know how to connect
	//the address to attach these to
	IDirectPlay8Address*	pDeviceAddr;
	//name of session
	WCHAR wcaSessionName[80]; //might not be used yet (07.14.2002)
	//TCP/IP infos
	WCHAR wcaIP[80];
	DWORD dwPortIP;  //
	WCHAR wcaPortIP[80]; //stores port string which is later converted to dwIP
	//IPX infos
	DWORD dwPortIPX;
	//Modem Infos
	WCHAR wcaPhoneNumber[80];//WCHAR* wcsPhoneNumber;  //they have to be WC arrays instead of pointers becaues they have to be edited(and my edit-string routine can only work with arrays at the moment) 07.14.2002
	//serial infos
	DWORD dwBaud;
	LPWSTR wcsFlowControl;
	LPWSTR wcsParity;
	LPWSTR wcsStopBits;

} DPSPCONNECTINFO, * PDPSPCONNECTINFO; 

class cDP_Multiplay
{
	public:
		//BOOL bCallBackInUse;
		int iNumCallBackThreadsActive;

		PDIRECTPLAY8PEER			pDP8;
	
		DPN_SERVICE_PROVIDER_INFO*	ServiceProviders;
		int							iActiveProvider;
		DWORD						dwNumProviders;
		DWORD						dwSPBufferSize;
		DPSPCONNECTINFO*			pSPConnectInfo;

		BOOL						bConnected;

		//parameters for startup sequences 08.12.2003
		BOOL						bRandKeysReceived;
		BOOL						bTotalPlayerCountReceived;
		BOOL						bTotalTimerIDCountReceived;
		int							iTotalPlayerCount;
		int							iTotalTimerIDCount;
		int							iNumNationInitsReceived;
		int							iNumTimerIDInitsReceived;

		DPDEVICEINFOARRAY*			pDeviceArrays;

		//PCMD						UserContextType; //user context value 06.27.2002 (?)
		WORD						wUserContext;

		IDirectPlay8Address*		pAddrHost;  //stores address if this system is hosting
		DPN_APPLICATION_DESC		AppDesc;

		DPHOSTENUMINFO			    HostsFound[TEMP_DP_MAXHOSTS];
		WORD						wHostsFound;
		WORD						wSelectedHost;

		DPCONNECTREPLYDATA			DPCRD;   //used to transfer reply data

		int							iMessagesReceived;

		BOOL						bUPDRequired;   //flag signalling that an update of the connection state is needed (07.20.2002)
		BOOL						bSetPeerInfoRequired;   //flag signalling that the peer info needs to be updated/set (07.20.2002)
		DPN_PLAYER_INFO				TestBuffer;  //grrr...07.20.2002

		CRITICAL_SECTION			csCriticalSection;

		DPNID						DPN_Static_ID_Array[TEMP_DP_MAXPLAYERS + 10];

		DPNID						DPN_List_of_Ready_IDs[2000];
		int							iLowestEmptyIndex;

		//static DP message control (06.03.2005)
		BYTE						StaticMsgBuffer[(TEMP_DP_STATICBUFFERSIZE+2)*TEMP_DP_SMBMAXSIZE];
		int							StaticMsgTypeBuffer[TEMP_DP_STATICBUFFERSIZE+2];
		int							StaticMsgSizeBuffer[TEMP_DP_STATICBUFFERSIZE+2];
		int							iNumMessagesInBuffer;
		BOOL						bStaticBufferInUse;
		int							iBufferTrace;

		void Process_StaticBuffer();

		VOID* pCAccess;

		void Send_Command(WORD wDest, WORD wType, LONG wParam1, WORD wParam2, WORD wParam3, double fParam4, double fParam5, void* pParam6, LONG lExecTick, BOOL LoopBackFlag, BOOL ReliableFlag);
		void Send_Command_Directed(DPNID iDPNID, WORD wDest, WORD wType, LONG wParam1, WORD wParam2, WORD wParam3, double fParam4, double fParam5, void* pParam6, LONG lExecTick, BOOL ReliableFlag);
		void Send_ChatText(char* pBuffer, WCHAR* wcsName, DWORD dwBufferSize);
		void Send_ChatText(char* pBuffer, char* Name, DWORD dwBufferSize);

		void Send_RandKeys(LONG* pKeys, DWORD dwBufferSize);
		//second type of command sending(01.01.2003)
		void Send_Command2(WORD wType, BYTE* pData, WORD wDataSize, BOOL LoopBackFlag, BOOL ReliableFlag);
		void Send_Command2(DPNID DPNIDTarget, WORD wType, BYTE* pData, WORD wDataSize, BOOL LoopBackFlag, BOOL ReliableFlag);


		void Init(VOID* pCA);
		void Exit();
		void EnumHosts();
		HRESULT	HostSession(WCHAR* pSessionName);
		HRESULT ConnectSession();
		HRESULT CloseSession();

		void AssignIDs_Host(); //function called by host to give IDs to other itself and other players

		~cDP_Multiplay();

		void Update_ConnectionState();

		void Init_LogFile();		
		void Write_Log(LPSTR Text);
		void WriteLog_Enter();
		char* Write_Log_Special(unsigned short* Text);

	private:
		HANDLE					hLogFile;


};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//a component of game's "sound library" directly analogous to the image surface(02.08.2003)
class cSoundSegment
{
	public:
		IDirectMusicLoader8*	pLoader;
		IDirectMusicSegment8*	pSegment;
	
		LPSTR					FileName;
		BOOL					bInit_Run;

		VOID*					pCAccess;

		//fmod specific
		FMUSIC_MODULE*			pFMOD_handle; //
		FSOUND_SAMPLE*			pFMOD_handle_sample;
		FSOUND_STREAM*			pFMOD_handle_stream;
		int						iFModMediaType;

		void Init(VOID* pCA, IDirectMusicPerformance8* pDM8Perf, IDirectMusicLoader8* pDM8Loader, LPSTR FName, BOOL bMIDIflag);
		void Exit();

	private:


};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
class cImageSurface
{

	public:
	
		LPDIRECTDRAW7			pDD7;  //experimental change 12.26.2003
		LPDIRECTDRAWSURFACE7	pDDSImage;
		DDSURFACEDESC2			DDSDImage;
		DDSURFACEDESC2			DDSDLock;    //07.05.2002
		
		int iXFrameSize;
		int iYFrameSize;		//kluges, temporary, I lack an information system to get data on frame sizes for images...I suppose this will suffice for the moment
		int iXCenterOffset;		//more kluging :-( specifies how many pixels in x the image should be offset from true center when drawn
		int iYCenterOffset;		//specifies how many pixels in y the image should be offset from true center when drawn

		DDCOLORKEY				DDCKImage;
		LPSTR					FileName;

		int						iSurfIndex;

		HANDLE					hLogFile;
		
		BOOL					bInit_Run;

		VOID*					pCAccess;

			
//		BOOL					bAlphaFlag;  //specifies whether an alpha buffer should be created for this Image (07.05.2002)  //consolidated 06.24.2004
		BOOL					bSysMemFlag;	//stores whether or not this surface resides in main memory
				
		BYTE*					pExBuffer;	//"extra information buffer" buffer is used to store extra associated image information such as (at the moment)
											//a 1-bit "alpha/key" channel, and a 1-bit "emblem mark" channel
		BYTE					bExtraBufferFlags; //saves what flags are currently active regarding the exbuffer

		cImageSurface();
		//added parameters to "force" a particular dimension of final image surface
		void Init(VOID* pCA, int iSurfIdx, LPDIRECTDRAW7 PDD7, LPSTR FName, BOOL bSysMemFlag, BYTE bExBufferFlags, DWORD dwColorKey, BOOL bForceDimensions, int iForceX, int iForceY);
		
		void SetFrameSize(int iX, int iY, int iXOff, int iYOff) { iXFrameSize = iX; iYFrameSize = iY; iXCenterOffset = iXOff; iYCenterOffset = iYOff; } 

		void Exit();  //07.03.2002
		DWORD Find_ColorKeyIndex(LPDIRECTDRAWSURFACE7 pImageSurf);
		~cImageSurface();

		void Write_Log(LPSTR Text);
		void WriteLog_Enter();

	private:

};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//02.01.2003
class cMinimap
{

public:
		VOID*				pCAccess;

		cImageSurface		MapSurface;
		cImageSurface		ProcessSurface;

//not sure what to do with these(03.24.2003)
//		SQCOORDSET			sqMinimapOffset;
		SQCOORDSET			sqMMZOffset;  //stores offset view value from map_xview and map_yview(always negative)
		BOOL				MMZDrawFlag;	//determines whether minimap ZOOM features should be enabled or not

		void Process_Minimap(BOOL bRevealAll);
		void Init(VOID* pCA);
		void Exit();
		


private:


};

//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
class cColorTable
{

public:
		DWORD				MM_Terrain[NUM_MAX_ISURFACES+1];
		DWORD				MM_Players[TEMP_DP_MAXPLAYERS+1]; //just to be safe 02.01.2003
		DWORD				MM_Players_ObjectMark[TEMP_DP_MAXPLAYERS+1]; //01.17.2004 (to mark national objects
		DWORD				MM_Players_Light[TEMP_DP_MAXPLAYERS+1]; //01.17.2004 muted version of national colors
		DWORD				MM_Players_Background[TEMP_DP_MAXPLAYERS+1]; //temp measure to color system backgrounds
		DWORD				MM_Players_Console[TEMP_DP_MAXPLAYERS+1]; //temp measure to color system backgrounds

		DWORD				MM_Own; //color indicating player's own
		DWORD				MM_Enemy; //color indicating enemy(in the case of Starcraft-like shift+tab)

		DWORD				Black;
		DWORD				White;
		DWORD				Green;
		DWORD				Yellow;
		DWORD				Red;
		DWORD				DarkRed;
		DWORD				DarkGreen;
		DWORD				Gray;
		DWORD				WinBlue1;
		DWORD				Blue;
		DWORD				DarkGray;

		DWORD				RMask;
		DWORD				GMask;
		DWORD				BMask;
		int					iRShift;
		int					iGShift;
		int					iBShift;

		DWORD				Std_ReplaceMarkerColor; //standard color that a certain function will replace another color on an image with

		DDPIXELFORMAT		PixelFormat;

		void Init(VOID* pCA);
		DWORD DetColor(double Rratio, double Gratio, double Bratio);
		int Det_HPGreenCmp(double ddHealthRatio); //determines the RGB green component for a bar color representing a unit's health
		int Det_HPRedCmp(double ddHealthRatio);  //determines the RGB red component...
		VOID*					pCAccess;

private:



};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//

//the idea for the graphics source handling structure is that, it will allow a unit's graphics to change based
//on two general aspects, 1) it's direction, and then 2) anything else. In other words, a unit/structure/projectile
//has a source graphic for each direction of each type of behavior that the user specifies that the object will have.
//That's two dimensions, and then because each graphic will allowably be a series of frames(a "frame cycle"),
//the structure ultimately handling all the graphics for each object must have organization in three dimensions for
//the frames it contains. 
//However, we are not hard coding a maximum value for ANY of the three directions(dynamic directions, dynamic
//cycle animation length, dynamic number of framecyclesets), which necessitates dynamic allocation of everything.
//Further, most instances of cSrcGraphicsInfo will likely be sparsely populated, thus the decision to use arrays
//to pointers to objects rather than arrays directly to objects. Finally, to simplify the task of inputting
//all this data in various forms into the cSrcGraphicsInfo structure, a "temp" linked list is used to 
//first gather all the entries that the user wishes to add to the structure, and then the entries are
//redistributed to their final destination, once information about how many there are and their extent
//is known.(07.11.2004)

//base of system implemented to robustly handle graphics for various things (07.06.2004)
class SRCIMGFRAME
{

public:

	RECT Imgcoords;
	int	 iImageSurfIndex;

	int iDuration; //the duration, in ticks, to display, if part of an animation
	int iStartTime; //the time, in ticks for the sequence to start

	void Init(int iLeft, int iRight, int iTop, int iBottom, int iImgIndex, int iDur) { Imgcoords.left = iLeft; Imgcoords.right = iRight; Imgcoords.top = iTop; Imgcoords.bottom = iBottom; iImageSurfIndex = iImgIndex; iDuration = iDur; }

};
class SRCIMGFRAME_L    //used to hold the entries inside the temporary list, instead of putting the pointers directly onto SRCIMAGEFRAME to save some memory....yeah....
{
public:
	void* pPrev;
	void* pNext;
	SRCIMGFRAME* pCurr;
	
	int iSetI;
	int iCycleI;
	int iFrameI;

};

class SRCIMGFRAMECYCLE    //set of frames (07.06.2004)
{

public:
	
	SRCIMGFRAME**	ppImgFrames;
	int				iNumFrames;
	int				iTotalDuration; //the total running time of all the frames in this cycle(zero on init)

};

class SRCIMGFRAMESET
{
public:
	SRCIMGFRAMECYCLE**	ppImgFrameCycles;
	int	iNumFrameCycles;
};


class cSrcGraphicsInfo
{

public:

	SRCIMGFRAMESET**	ppFrameSets;
	//more flags may go here
	int iNumFrameSets;

	SRCIMGFRAME_L* pTempList_First;
	SRCIMGFRAME_L* pTempList_Last;

	SRCIMGFRAME ImgFrameDefault; //default return

	VOID*	pCAccess;
	HANDLE hLogFile;

	void Init(VOID* pCA, HANDLE hLF);
	void Exit();

	void Flush_Arrays();
	void Flush_List();

	void Add_Entry(int iSet,         //which set of frame cycles
		           int iDirection,   //which frame cycle
				   int iFrameIndex,  //which frame in the frame cycle
				   int iSurfIndex,   //the isurf containing the source data
				   int SrcRect_left, 
				   int SrcRect_right,
				   int SrcRect_top,
				   int SrcRect_bottom,
				   int iDuration);	 //duration of frame

	//this is the first "get_entry" function. you specify the exact set, direction and frame,
	SRCIMGFRAME* Get_Entry(int iSet, int iDirection, int iFrame, int* piRetCode);
	//this is the second "get_entry" function. you specify the set and direction,
	//and the time at which this current type of animation "started". 
	//the function then calculates and returns the frame that is "showing" at that time
	SRCIMGFRAME* Get_Entry2(int iSet, int iDirection, int iStartTick, int* piRetCode);

	void Setup_Graphics_DefaultA(int iDirections, int iXFrameSize, int iYFrameSize, int iSurfSource);


	void Update();

		HANDLE Init_LogFile();
		void Exit_LogFile();
		void Write_Log(LPSTR Text);
		void WriteLog_Enter();



};


//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//node for queue below
class cBQUNode
{
public:
	void* pNext;
	void* pPrev;

	int iZLevel;  //just in case

	//these params exactly match the ones for IDDS::BltFast
	int	dwX;
	int	dwY;
	LPDIRECTDRAWSURFACE7 pDDS;
	LPRECT	lpSrcRect;
	RECT	StoreRect;
	DWORD	dwTrans;
	WORD	wFrameHeight;
	BYTE*	pExBuffer; //if needed

	int		iBQU_BlitType;
	DWORD	dwOverlayColor;

};

//"unit drawing specialized blit queue"
//class designed to more easily handle the necessity of having "Z-levels" when drawing a unit (01.07.2004)
//we are appropriating the UBlitQueue for other uses
class cBlitQueue_USP
{
public:
/*
	//anchors
	cBQUNode*	pBQU_First[NUM_ZLEVELS];
	cBQUNode*	pBQU_Last[NUM_ZLEVELS];
*/
	//alternate queueing scheme
	cBQUNode*   Levels[NUM_ZLEVELS];
	int			iHighestQueued[NUM_ZLEVELS];

	VOID*	pCAccess;
	//
	void	Init(VOID* pCA);
	void	Exit();
	void	Submit_BlitRequest(int iZLevel, DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE7 pDDS, LPRECT lpSrcRect, DWORD dwTrans, int iBQU_BlitType, DWORD dwOverlayColor, BYTE* pOpacityBuffer);
	void	Flush_Buffers(int iIndex_Low, int iIndex_High);
	void	Dispatch_BlitRequests(LPDIRECTDRAWSURFACE7 pDrawSurf, int iIndex_Low, int iIndex_High);
};

class cGDisplay
{

	public:
	
		//VOID*			pImages[10];
		//VOID*			pGMap;
		//VOID*			pIState;
		//VOID*			pInterface;
		//VOID*			pDD_Display;
		VOID*			pCAccess;
		DDBLTFX			ddblitfx;
		HANDLE			hLogFile;

		DWORD			dwFontColor;

		cDrawIssueBuffer	DrawQueue;   //might be obsolete as of 01.07.2004....maybe not, might be part of the failed integrated D3D 9 project

		cBlitQueue_USP		UBlitQueue;  //01.07.2004

		int	 MDRAW_NUMROWS;
		int	 MDRAW_NUMCOLS;

		DDSURFACEDESC2		LockDescSource; 
		DDSURFACEDESC2		LockDescDest;

		void	AlphaBlt(LPDIRECTDRAWSURFACE7 pDDS_Dest, int dwX, int dwY, LPDIRECTDRAWSURFACE7 pDDS_Source, LPRECT lpSrcRect, DWORD dwColorKey);
		void	AlphaBltF(LPDIRECTDRAWSURFACE7 pDDS_Dest, int dwX, int dwY, LPDIRECTDRAWSURFACE7 pDDS_Source, LPRECT lpSrcRect, DWORD dwColorKey); //06.24.2004 ("Fake" alpha blitting)
		void	ShadowBlt(LPDIRECTDRAWSURFACE7 pDDS_Dest, int dwX, int dwY, LPDIRECTDRAWSURFACE7 pDDS_Source, LPRECT lpSrcRect, DWORD dwColorKey, DWORD dwRefColor, BYTE* pExBuffer);
		void	ShadowBltF(LPDIRECTDRAWSURFACE7 pDDS_Dest, int dwX, int dwY, LPDIRECTDRAWSURFACE7 pDDS_Source, LPRECT lpSrcRect, DWORD dwColorKey, DWORD dwRefColor, BYTE* pExBuffer);
		void	EmBlt(LPDIRECTDRAWSURFACE7 pDDS_Dest, int dwX, int dwY, LPDIRECTDRAWSURFACE7 pDDS_Source, LPRECT lpSrcRect, DWORD dwColorKey, DWORD dwRefColor);		
		void	EmBltF(LPDIRECTDRAWSURFACE7 pDDS_Dest, int dwX, int dwY, LPDIRECTDRAWSURFACE7 pDDS_Source, LPRECT lpSrcRect, DWORD dwColorKey, DWORD dwRefColor, BYTE* pExBuffer);
		void	AlphaTintRect(LPDIRECTDRAWSURFACE7 pDDS_Dest, RECT Target, DWORD dwAlphaClr);
		void	AlphaTintRectF(LPDIRECTDRAWSURFACE7 pDDS_Dest, RECT Target, DWORD dwAlphaClr);


		void    Tint_MapControl(LPDIRECTDRAWSURFACE7 pDrawSurf);


		//the marker replacing function
		void	ProcessImage_MarkerReplace(int iImageSurf);

		//store the draw position and size of the minimap
		RECT MinimapDrawRect;
		RECT MinimapZoomDrawRect; //rectangle for the "zoomed" map

		//store the rectangle positions needed to draw interface panels (06.20.2005)
		RECT RectIfcPanels[NUM_IFCRECTS];
		RECT RectIfcListBoxes[NUM_IFCRECTS];
		RECT RectIfcSecondaryPanels[NUM_IFCRECTS];

/*
		RECT RectPanelTopLeft;
		RECT RectPanelTopRight;
		RECT RectPanelBottomLeft;
		RECT RectPanelBottomRight;
		RECT RectPanelTop;
		RECT RectPanelBottom;
		RECT RectPanelLeft;
		RECT RectPanelRight;
		RECT RectPanelCenter;
*/
		int iMinimumWidth;
		int iMinimumHeight;

		void Init(VOID* pCA);
		void Exit();
		void Disp_Map(LPDIRECTDRAWSURFACE7 pDrawSurf);
		void Disp_Cursor(LPDIRECTDRAWSURFACE7 pDrawSurf);
		void Disp_RunText(LPDIRECTDRAWSURFACE7 pDrawSurf);   //temporary 01.03.2002
		void Disp_Units(LPDIRECTDRAWSURFACE7 pDrawSurf);    //temporary(for now) as it was written on 01.18.2002
		void Disp_Structures(LPDIRECTDRAWSURFACE7 pDrawSurf);
		void Disp_Selection(LPDIRECTDRAWSURFACE7 pDrawSurf);  //written(or started 05.14.2002)

		void Disp_Pie(double ddFraction, int iX, int iY);

		void Disp_IfcElems(void* _pIGrid, int iXOffset, int iYOffset);
		void Disp_IfcElems_B(void* pHead_, int iXOffset, int iYOffset);
		void Disp_IfcElems_C(void* pElem_, int iXOffset, int iYOffset);

		//Draw_Text functions extended to include Font control(03.02.2003)
		int Draw_Text(LPDIRECTDRAWSURFACE7 pDDS, char* Text, int xpos, int ypos, int iFont);  //this function is now under GDisplay because it's no longer Windows GDI-ish... 01.03.2002
		int Draw_Text(LPDIRECTDRAWSURFACE7 pDDS, char* Text, int xpos, int ypos, int iFont, int iXLimit);  //this function is now under GDisplay because it's no longer Windows GDI-ish... 01.03.2002
		int Draw_Text_Wide(LPDIRECTDRAWSURFACE7 pDDS, unsigned short* Text, int xpos, int ypos, int iFont);		
		int Draw_Text_Shadow(LPDIRECTDRAWSURFACE7 pDDS, char* Text, int xpos, int ypos, int iFont, DWORD dwTextColor, DWORD dwShadowColor);
		int Draw_Text_BlitRequest(LPDIRECTDRAWSURFACE7 pDDS, char* Text, int xpos, int ypos, int iFont);


		void Draw_Interface_Panel(LPDIRECTDRAWSURFACE7 pDDS, RECT RectDest);
		void Draw_Interface_ListBox(LPDIRECTDRAWSURFACE7 pDDS, RECT RectDest);
		void Draw_Interface_Secondary_Panel(LPDIRECTDRAWSURFACE7 pDDS, RECT RectDest, int iColorIndex);
		void Draw_Interface_Special_Square(LPDIRECTDRAWSURFACE7 pDDS, RECT RectDest, int iPanelFlag);

		void Draw_TextCursor(LPDIRECTDRAWSURFACE7 pDDS, int iFontType, int iX, int iY);

		void Disp_Projectiles(LPDIRECTDRAWSURFACE7 pDrawSurf);
		void Disp_MainMenu(LPDIRECTDRAWSURFACE7 pDrawSurf, int iInc);
		void Disp_ChatText(LPDIRECTDRAWSURFACE7 pDrawSurf, int iXOff, int iYOff, int iEntries);  //07.23.2002
		void Disp_Notifications(LPDIRECTDRAWSURFACE7 pDrawSurf, int iXOff, int iYOff, int iEntries);  //almost identical to Disp_ChatText (06.18.2004)
		void Disp_Minimap(LPDIRECTDRAWSURFACE7 pDrawSurf);
		void Disp_Console(LPDIRECTDRAWSURFACE7 pDrawSurf);
		void Disp_Scrollbar(LPDIRECTDRAWSURFACE7 pDrawSurf, void* pIG_Act, void* pIB);
		~cGDisplay();  //obsolete 09.26.2003
		void Init_LogFile();
		void Write_Log(LPSTR Text);
		void WriteLog_Enter();

		void Execute_Command2_Draw(PCMD2* pQ2Tr, int iXOffset, int iYOffset); //for draw specific commands only(this function exists because the overhead on draw commands must be low, so we cannot waste time in the general function)

	private:
		//DO NOT EVER DECLARE GLOBAL COUNTERS FOR CLASSES AS LARGE AS GDISPLAY!
		RECT					TextSrcRect; //rect for text blitting purposes
		int						text_xdraw, text_ydraw;  //vars for text blitting purposes 
		int						iTextDrawFrame; //int which holds drawframe for text (1.03.2002)


};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
class cAnimFrame
{
public:
		RECT		SourceRect;
		int			iImageSurf;
		int			iBlittype;
		int			iOverlayColor;
		
		void*		pPrev;
		void*		pNext;


		void Init(RECT Rect, int iIS, int iBT, int iOVC);


private:

};

class cAnimType
{
public:
		cAnimFrame*		pAF_First;
		cAnimFrame*		pAF_Last;
	
		cAnimFrame**	pTable;

		int			iNumFrames;
		int			iType;

		void		Init();
		void		Add_Frame(RECT Rect, int iIS, int iBlittype, int iOverlayColor);
		void		Reconstruct();
		void		Flush_Buffer();
		void		Exit();

		void*			pPrev;
		void*			pNext;

		//note that this structure has the dubious honor of being a list-within-a-list(01.18.2004)

private:

};

class cAnimInfoTable
{

public:
		cAnimType*		pAT_First;
		cAnimType*		pAT_Last;

		cAnimType**		pTable;

		void*			pCAccess;

		void			Init(void* pCA);
		void			Exit();

		void			Add_EmptyType(int iType);

		void			Reconstruct();

		HANDLE			hLogFile;

		void Init_LogFile();
		void Write_Log(LPSTR Text);
		void WriteLog_Enter();
		void Exit_LogFile();


private:

};

class cAnimation
{
public:
		
		int				iType;
		int				iCurrFrame;
		int				iZLevel;
		int				iX;	//absolute coords
		int				iY; //absolute coords

		void			Init()  {iCurrFrame = 0; }

private:

};

class cEvent
{
public:
		cAnimation*		pAnimation;
		LONG			lAnimStartTick;  //when to start the animation
		BOOL			bSoundPlayed;	 //whether the sound has been played
		//possibly like... other structures for sounds, commands, and other such stuff (01.18.2004)
		int				iSoundSegIndex;
		int				iVolume;

		VOID*			pCAccess;

		void			Init_Simple(void* pCA, int iAnimType, LONG lAnimSTick, int _iSoundSegIndex, int _iVolume, int iZLev, int iAnimX, int iAnimY);  //negative types to specify NONE
		void			Exit();
		BOOL			Calc_Completion(); //returns whether this event is done occurring (and thus whether to remove)
		BOOL			Calc_Execute();		//returns whether this event has started(and thus execute)

		void*			pPrev;
		void*			pNext;
private:

};

class cEventBuffer
{
public:
		//another list...this time of events
		cEvent*			pEV_First;
		cEvent*			pEV_Last;

		HANDLE			hLogFile;


		void*			pCAccess;

		void			Init(VOID* pCA);
		void			Add_Event(int iAnimType, LONG lAnimTick, int iSoundSegIndex, int iVolume, int iZLevel, int iAnimX, int iAnimY);
		void			Flush_Buffer();
		int				Execute_Event(cEvent* pEvent);   //returns int to indicate it's destruction
		void			Process_Buffer();  //go through instructions in buffer
		void			Exit();

		
		int iSoundsPlayed; //keep track of the number of sounds played in a frame (so we don't play a million of them at once)

		void Init_LogFile();
		void Write_Log(LPSTR Text);
		void WriteLog_Enter();
		void Exit_LogFile();


private:



};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//mouse state
typedef struct _IGMSTATE
{

	BYTE		Buttons[3];		//0 = left button, 1 = right button
	int			iXPos;
	int			iYPos;
	int			iWheel;  //12.27.2004

} IGMSTATE, * LPIGMSTATE;
//inputstate will hold the input data in a form that the main program is more able to use
class cInputState
{

public:
	VOID*				pCAccess;

//the keyboard array will be an array of 256 bytes just like the DI Keyboard state array was, except more of the
//values will be used: ex. 0 = nothing; 1 = key on; 2 = key down; 3 = key up
	BYTE				IGState_Keyboard[256];
	IGMSTATE			IGState_Mouse;

	char				DI_ANSI_Table[256];  //holds indexes to string value for each DIK value
	char				DI_ANSI_Table_Shift[256];  //holds indexes for shifted DIK values

	int					iDrawFrame; //frame specifying which cursor graphic to show

	void Init(VOID* pCA);
	void Update_IGStates();
	~cInputState();

private:
	int					c1, c2; //counters


};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//from Greg7 AFRL (08.11.2003)
class cNotifNode
{
public:	
	LPSTR Text;
	DWORD	dwActiveColor;
	DWORD	dwPassiveColor;
	
	BOOL bFreeTextFlag;

	void*	pPrev;
	void*	pNext;
private:

};

class cNotifBuffer
{
public:
	cNotifNode*	pNB_First;
	cNotifNode* pNB_Last;

	void*	pCAccess;

	HANDLE	hLogFile;

	void	Init(VOID* pCA);
	void	Exit();

	void	Create_Add_Node(LPSTR NewText, DWORD dwActiveColor, DWORD dwPassiveColor, BOOL bFreeTextFlag);

		void Init_LogFile();
		void Write_Log(LPSTR Text);
		void WriteLog_Enter();
		void Exit_LogFile();


private:
	
};


//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
typedef struct _CBNODE   //"chat buffer node" 07.22.2002
{
//	LPSTR	Name;   //name of entity which posted this message
	LPSTR	Text;	//message text
	int		iTicksRemaining;   //number of ticks remaining for this message to stay in the buffer
	void*	pNext;
	void*	pPrev;

} CBNODE, * LPCBNODE;


class cChatBuffer  //simple class to handle chat messaging....similar concept to most other game structures....will
					//hold linked list of chat messages posted....to be removed when framecount expires(07.22.2002)
{
public:

	VOID*	pCAccess;

	CBNODE*		pCBL_First;
	CBNODE*		pCBL_Last;

	int		iPersistTickCount;

	void Add_CBNode(LPSTR Text, int iStayTime);
	void Remove_CBNode(CBNODE* pNode);

	void Process_ChatBuffer();
	void Clear_List();

	void Init(VOID* pCA, int iPersistTicks);
	void Exit();


private:
	CBNODE* pCBL_Trav;
	CBNODE* pCBL_Trav2;
	CBNODE* pCBL_Trav3;
	CBNODE* pCBL_Trav4;


};

//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//01.18.2005
class cAcknowledgement
{
public:

	char* AckText;
	int	 iSndSeg;
private:

};

class cAckTable
{
public:
	cAcknowledgement* Table;
	int iNumAcks;
		//ACK is split like this:
		// a CYCLE: 3 sounds(or links to sounds)
		// a PERSONALITY: 1 cycle for EACH of the ACK_TYPE_ types except for the _MENTAT_ ones 
		// a UNIT TYPE: 3 personalities
		// a STRUCTURE TYPE: 1 personality
		// MENTAT: special index located 

	void* pCAccess;

	void Init(void* pCA);
	void Exit();
	void Set_Ack(BOOL bStruct, int iObjectType, int iAckType, int iPersonality, int iCycleIndex, int iSoundSegToSetTo, char* AckText, int iAckTextLength);
	int Get_Ack(BOOL bStruct, int iObjectType, int iAckType, int iPersonality, int iCycleIndex);

	int Supply_Random_Ack();
	int Supply_Random_Ack2();

private:

};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//INTERFACE CLASSES

//to be implemented as planned 05.17.2003
typedef struct _CMDDATAREQUESTSEQUENCE
{
	ULONGLONG Sequence[MAX_COMMINFO_REQUESTS];	//unsigned 64-bit, treated as 64 BOOLs, 32 for parameter fields and 32 for object reference type fields
	LPSTR RequestText[MAX_COMMINFO_REQUESTS];	//contains text to be written to the IFC display text for each sequence

	
} CMDDATAREQUESTSEQUENCE, * PCMDDATAREQUESTSEQUENCE;

//stores the type and order of queries 
class cCmdDataReqTable
{
public:

	LPSTR					RequestText[NUM_CI_REQTYPES];

	CMDDATAREQUESTSEQUENCE	CommandTypes[NUM_CQ2COMMANDS];   //index to match command IDs

	void Init();

	ULONGLONG CompileAttributeValue(int Flag1, int Flag2, int Flag3, int Flag4, int Flag5, int Flag6, int Flag7, int Flag8, int Flag9, int Flag10);

private:

};

//"packet" to keep track of user parameter and object reference inputs(05.17.2003)
class INPUTPACKET
{

public:
	DWORD	dwTick;			//tick time that is pertinent tot his input packet. this will probably be the creation time of the packet
	DWORD	dwPacketType;	// {IPACKETTYPE_OBJREF, IPACKETTYPE_PARAM
	
	DWORD	dwObjectsType;	//{OBJTYPE_UNIT, OBJTYPE_STRUCTURE, OBJTYPE_COMPONENT, OBJTYPE_MAPTILE, etc.}
	DWORD	dwNumObjects;	
	VOID**	pObjectArray;	//{cast items to cUnitA*, cStructureA*, cComponentA*, Maptile*, etc.)
	DWORD*	pdwIDs;			//{matching table IDs(or maptile index coordinates) or units in array

	DWORD	dwParameterType;	//{IPARAMTYPE_FLOATS, IPARAMTYPE_INTS, IPARAMTYPE_STRING}
	DWORD	dwNumParamItems;
	VOID*	pItemArray;

	VOID*	pNext;
	VOID*	pPrev;

private:
	
};
//packet handler list. keeps track of input packets 
class cInputPacketBuffer
{
public:
	INPUTPACKET*	pIPB_First;
	INPUTPACKET*	pIPB_Last;

	HANDLE			hLogFile;

	VOID*			pCAccess;

	void	Init(VOID* pCA);
	void	Exit();

	//debuglog functions
	void Write_Log(LPSTR Text);
	void WriteLog_Enter();
	void Init_LogFile();
	void Exit_LogFile();

	//all INTs changed to DWORDs to maintain constant data type size(05.30.2003)
	INPUTPACKET*	Add_Packet(INPUTPACKET*	pInputPacket);
	void			Compile_Add_Packet( DWORD dwPacketType, 
								DWORD dwObjectsType, 
								DWORD dwNumObjects, 
								VOID** pObjectArray, 
								DWORD* pdwIDs, 
								DWORD dwParameterType,
								DWORD dwNumParamItems,
								VOID* pItemArray,
								BOOL bCopyFlag);
	void			Delete_Packet(INPUTPACKET* pInputPacket);
	void	Flush_Buffer(INPUTPACKET* pIPB_Start, INPUTPACKET* pIPB_End);  //if pIPB_Start and pIPB_End are NOT linked in some way to each other, we'll crash into infinite loop here(05.17.2003)
	void	Flush_Buffer2(INPUTPACKET* pIPB_Start, INPUTPACKET* pIPB_End); //differs from the first in that it does not get rid of start and end(08.20.2003)
	
	//the buffer itself is receiving its own IPB searching function so that it can scan other packet buffers and pull 
	//packets from them. The current intended use for this is for building a custom-component unit.
	//this will allow an IPB to acquire a group of CTYPE packets, then re-issue them together as a CTYPEGROUP packet
	//the CTYPEGROUP packet will be the one that is picked up by the Pending Command and issued(08.20.2003)
	BOOL	PeekAcquire_IPB_Specific(INPUTPACKET* pIPB_Start, INPUTPACKET* pIPB_End, int iObjType, LONGLONG* pllCutoffTime, BOOL bCopyFlag);
	void	Cull_ComponentPackets_By_Class(INPUTPACKET* pIPB_Start, INPUTPACKET* pIPB_End, BOOL bFrameOnlyFlag); //08.20.2003 function intended to cull any buffer to contain only one of each class of component
	void	Issue_Group_Packet(void* pIPB/*cast to cInputPacketBuffer*)*/, int iObjectGroupType); //08.20.2003



private:

};

//05.20.2003
//data structure for a pending command
class cPendingCommand
{
public:

	PCMD2*	pCommand2; //command data will be assembled into this structure
	PCMD*	pCommand;

	cInputPacketBuffer	AcquiredData;		//stores the input packets that have been acquired as input data for this pending command
	int		iRequestIndex;	//if calling from the request table, the index of the entry specifying the current sequence of data queries
	int		iCurrentSequenceNumber;	//the entry within the current sequence we are currently at(ex. the
									//sequence requires we specify a unit, a component, and a unit, and we are at
									//2nd number, we need to select a component). the type of data being requested
									//is held in the request table itself(05.13.2003)
	int		iCStore1;				//"convenient store".... yeah.....the reason for this is that the cursor needs to know
									//what structure type was selected to show a grayed out region corresponding to the structure's size
									//to do this any other way it would have to go through and extract that information from
									//the acquired data, which is clunky, so just waste a little memory and save that here (06.18.2004)
	LONGLONG	llCutoffTime;	//the tick time beyond which to search for matching Input Packets in the IPB

	VOID*	pCAccess;

	void	Init(VOID* pCA, WORD wType);
	void	Compile_Command2();		//compiles a command2 command from data in AcquiredData
	BOOL	PeekAcquire_SB();	//check in the Selection Buffer for matching object references and acquire data
	BOOL	PeekAcquire_IPB(BOOL bCopyFlag);		//check in the IPB for matching Input Packets, and acquire the data if so

	void	Exit();

	void*	pPrev; //don't know why I need these(05.13.2003)
	void*	pNext; //don't know why I need these(05.13.2003)

private:
};

typedef struct _SBE     //selection buffer entry (03.20.2002) At the moment I'm figuring that trying to run the selection buffer through the unit list itself will be too cumbersome.
{
	int		iID;   //
	int		iObjectClass; //whether the selection is a unit, a structure, a projectile, or what(03.28.2003)
	int		iCommandFlag; //the class of command TO BE ORDERED to this unit (01.02.2003)
	VOID*	pPrev; //previous entry
	VOID*	pNext; //next entry
} SBE, * LPSBE;

typedef struct _IGRIDCELL   //interface grid cell (05.06.2002) ... tiny little "command" box that holds data which can be used to issue commands
{
	//this will probably be modified later as I figure out exactly what this will do(05.06.2002)
	//WORD	wDest;	//destination of command
	//WORD	wType;	//command type
	PCMD		ProgCommand;  //for the moment, store a whole command to be mixed with any data to be issued(07.04.2002) -->Independence Day!
	PCMD2*		pProgCommand2;  //new command format with information to execute command2 commands(03.12.2003)
	BOOL		bDrawCell;  //specifies whether to draw this cell
	BOOL		bActiveCommand;
	BOOL		bActiveCommand2;  //specifies whether to execute the Command2 information(03.12.2003)
	BYTE		bDataSources;  //specifies whether each field of the prog command is receiving its data from the unit buffer, the grid cell, or what (07.04.2002)
	BYTE		bDataSources2;  //ditto for the command2 command
	BYTE		DrawType;
	
	//drawtype PICTURE
	WORD		wISurfIndex;
	WORD		wFrame;
	RECT		SurfRect;

	//drawtype TEXT or TEXTHDC
//	LPSTR		Text;
	char*		Text;
	LPSTR		TextExtended; // in the case that a cell needs to hold text longer than 80 characters (not yet implemented) 08.12.2003
	int			iFont;
	int			ColorR;  //additional color flags (HACK, really...if not used(default), set to -1 or VAL_INVALID)
	int			ColorG;
	int			ColorB;	

	//extending a gridcell to hold object references if so desired(05.20.2003)
	DWORD		dwNumObjRefs;
	DWORD		dwObjRefType;
	VOID**		pObjRefArray; //array of pointers to objects
	DWORD*		pdwIDArray;		   //array of IDs

	//doesn't hold other parameters yet because the other parameters are held in other places(ex. mouse's position, unit buffer, etc.)
} IGRIDCELL, *LPIGRIDCELL;


//new, experimental implementation of an interface system... started 06.10.2005
//to add desperately needed new functionality to the interface system
//to be programmed towards the possibility of re-use with future programs
class cIfcElement
{
public:
	void* pListBoxInfo;

	RECT Rect_Pos; //the position of the interface element

	PCMD2* pCmds[IFC_ELEM_CAT_NUMELEMS];
	
	void Init(int left, int right, int top, int bottom);
	void Exit();
	
	void Add_Cmd(int iCategory, PCMD2* pCommand, BOOL bCopyFlag);
	void Clear_Cmds(int iCategory);	

	void Add_IfcElem(cIfcElement* pElem);
	void Add_IfcElem_Front(cIfcElement* pElem);

	void Clear_IfcElems();

	int iNumChildren; //number of children elements
	//links
	cIfcElement* pNext; //next command in the list
	cIfcElement* pPrev; //prev command in the list
	
	//not used currently 06.11.2005
	cIfcElement* pParent; //parent(if any)

	//use 07.22.2005
	cIfcElement* pChildren; //pointer to children(if any). It is not possible to create anchors due to the recursive properties of this class
	cIfcElement* pTail;

	cIfcElement* pAttachedElement;  //a storage location for extra elements

private:

};

//ListBoxINfo class for use with listboxes in cIFcElements
class cIfcListBoxInfo
{
public:
	cIfcElement* pListBox;
	int iListWindowSize;
	int iPrimaryOffset;
	cIfcElement* pZeroOffsetElement;

private:



};

class cIGrid	//grid class for an interface grid(redundant! I know!) (05.06.2002)
{
	
public:
	//NEW interface elements (06.11.2005)
	//anchors
	cIfcElement* pIfcElem_First;
	cIfcElement* pIfcElem_Last;
	
	void Add_IfcElem(cIfcElement* pElem);
	void Clear_IfcElems();
	cIfcElement* Get_IfcElem(int iElemNumber);	

	IGRIDCELL*	pGrid;		//pointer to the grid array when it is allocated
	RECT		GridPos;	//position of the grid on the screen
	int			iElements_x;	//width in terms of entries horizontally
	int			iElements_y;	//height in terms of entries vertically
	int			iHeight_pixels; //height of the grid in pixels
	int			iWidth_pixels;	//width of the grid in pixels
	int			iCellX_pixels;  //width of each cell in pixels
	int			iCellY_pixels;	//height of each cell in pixels  //ALL OF THESE VALUES CAN BE CALCULATED FROM EACH OTHER AND SO THEY MUST ALL BE CHANGED AT ONCE IF THEY ARE CHANGED

	//this set of variables implements a scroll bar functionality to IGrids
	BOOL bScrollbarFlag; //indicates whether to have a scrollbar for this grid
	int iScrollVal;		 //this is the current "position" of the scrollbar in the range of the full document
	int iScrollMax;		 //this is the maximum value the scrollval can have
	int iScrollMin;		 //this is the minimum value the scrollval can have


	void Set_Scrollbar(BOOL bEnabled, int iScrVal, int iScrMax, int iScrMin); 

	int	Calc_GridIndex(int xpos_scr, int ypos_scr);	//calculates the entry index based on the screen coordinates
 
	RECT Calc_GridcellRect(int iGridIndex); 
	void Init(int iXOff, int iYOff, int iElemX, int iElemY, int iCellX, int iCellY);
	void Exit(); 

private:

};

class cIBox    //class for interface "box" --> collection of interface grids representing a consolidated interface for a certain area (07.04.2002)
{
	
public:
	cIGrid*		pIGrids;
	int			iNumGrids;  //number of grids active

	RECT		BoxPos;
	int			iActiveGrid;   //representing which Interface Grid is currently active
	BOOL		bActiveBox;	   //whether this box is currently active or not

	VOID*		pCAccess;

	void Init(VOID* pCA);
	void Exit();

private:
	int			c1, c2; //counters

};

class cTutorial
{

public:
	cIfcElement Elements[300];
	int iNum_Times_Element_Has_Been_Called[300];

	int iActiveElement;
	int iMostRecentActiveElement;
	int iTicksSince;
	int iActivePersistentElement;

	//gameworld monitoring hooks
	int iTick_MostRecentCapitalStartConstruction;
	int iTick_MostRecentCapitalFinishConstruction;
	int iTick_MostRecentTABKeyUp;
	int iNumStoragesCompleted;
	int iTick_SecondStorageCompleted;
	int iTick_AllStoragesSet;
	int iTick_MostRecentConstructionVehicleSelect;
	int iTick_MostRecentForgeFinishConstruction;
	int iTick_MostRecentFactoryFinishConstruction;
	int iTick_MostRecentUraniumVehicleSelect;
	int iTick_MostRecentVehicleArmed;
	double ddAmtSteel;
	double ddAmtUranium;
	int iTick_StartProcessingSteel;
	int iTick_FirstMechInfCreated;
	void* pCV;
	void* pTanker;



	cTutorial() 
	{  iActiveElement = 0; 
		for (int lc1 = 0; lc1 < 30; lc1++)
			iNum_Times_Element_Has_Been_Called[lc1] = 0;

		iMostRecentActiveElement = -1;
		iTicksSince = 0;

		iTick_MostRecentCapitalStartConstruction = -1;

	}

	//void Init(VOID* pCA);  //let cInterface handle initialization because it is better equipped.
	void Exit() 
	{
		for (int lc1 = 0; lc1 < 30; lc1++)
		{
			Elements[lc1].Exit();

		}
	}
private:

};


class cInterface
{

public:
		int				map_xview;    //the new places where the position of the map is going to be held
		int				map_yview;

		//mouse selection control vars
		int				mouse_currbox;		//the current interface box the mouse is moving over
		SQCOORDSET		PosMap_Mouse;		//the current position of the mouse on the map
		COORDSET		PosMap_Mouse_ABS;	//position of the mouse on the GW in absolute coordinates (05.13.2002)
		SQCOORDSET		PosMinimap_Mouse;
		
		RECT			MSelectBox_GW;		//the gameworld coordinates of the selectionbox
		RECT			MSelectBox_GW_ABS;	//absolute gameworld coordinates of the selectionbox (05.13.2002)
	
		BYTE			iMode;
		
		cIGrid*			pScrollGrid; //07.27.2004
		cIBox*			pScrollBox;	 //07.27.2004 //why these are BOTH here is because of the weird way the interface system was implemented(the Box and the Grid are not necessarily of the same dimensions)

		LPSBE			pUSB_First;		//first in the unit selection buffer(void because cUnitA has not been defined yet) 03.20.2002
		LPSBE			pUSB_Last;		//last in the unit selection buffer(void because cUnitA has not been defined yet)
		LPSBE			pSSB_First;		//structure selection buffer(03.29.2003)
		LPSBE			pSSB_Last;
		void			Reset_CommandFlags(WORD wType); //resets all Command Flags in the USB with the specified type (01.02.2003)

		cIBox			IBoxes[IBOX_NUMBOXES];	//pointers to the interface grids

		char*			pKeyEdit;   //stores pointer to string that is being edited by key presses  //NULL if no pointer set
		DWORD			dwKeyEditMaxBuffer;  //holds the size of the buffer(in charactres) being edited
		WCHAR*			pKeyEdit_Wide;   //ditto, except for wide strings
		DWORD			dwKeyEditWideMaxBuffer; //ditto, except for wide strings(in WCHARs)

		void*			pConsole_Unit_Select;  //pointer to selected unit for console(cast to LPSBE)
		void*			pConsole_Struct_Select; //03.28.2003

		cPendingCommand* pPendingCommand;	//05.20.2003

		cInputPacketBuffer PacketProcessor; //08.20.2003
		int				iScanType; //specifies what type of objrefs the Packet Processor is currently "sniffing" for(-6996 if not active) (08.20.2003)
		LONGLONG		llCutOffTime; //the current cutoff time of the scan

		char			caChatText[1000];  //limit to a thousand characters
		char			caTestBox[1000];
		char			caTestBox2[1000];
		WCHAR			wcaTestBox[1000];
		char			StandingTextBuffer[TEMP_STD_STRLEN];	//to hold text for, say, displaying the cost of a component or component type

		char			TutorialPersistentMessageBuffer[1000];
		cIfcElement		TutorialPersistentMessageBox;
		BOOL			bTutorialPersistentMessageBoxActive;
		int				iLastPersistentButtonClick;

		int				iiSupportedCommandsBuffer[200];

		//I realize this is a very not-elegant-quick-fix but I will currently use it for now and implement something else later if necessary (06.03.2003)
		//try this...hierarchical display system. (06.03.2003)
		char*			pDisplayText;		//simply puts text to be drawn to screen at display time
		
		//
		char*			pMapNameSelect;	//pointer to the text name of the currently selected map
		
		char*			pSPNameSelect; //pointer to the text name of the currently selected service provider(this is easier than rearranging the interface element)
		char*			pDeviceNameSelect;

		WCHAR**			ppPhoneNumberText;
		WCHAR**			ppIPText;

		cIfcElement*	pSPMapsListBox;
		cIfcElement*	pSPMapsListBoxFrontEnd;
		cIfcElement*	pMPMapsListBox; //currently the same thing as its SP counterpart
		cIfcElement*	pMPMapsListBoxFrontEnd;
		cIfcElement*	pServiceProviderListBox;
		cIfcElement*	pDeviceListBox;
		cIfcElement*	pHostsListBox;
		cIfcElement*	pGameLaunchUpdateRgn;
		cIfcElement*	pMPStartButton;  //keep track of this button so as to enable it only for the host
		
		cIfcElement*	pConsoleCommandListBox;
		cIfcElement*	pConsoleInfoListBox;
		cIfcElement*    pConsoleDisplayUpdate;
		cIfcElement*	pConsoleGroupList;
		cIfcElement*	pConsoleEquipListBox;

		cIfcElement*	pBriefingNationSelectListBox;
		cIfcElement*    pBriefingNationSelectListBoxFrontEnd; //the interface element in charge of "displaying" the listbox

		cIfcElement*	pBriefingPlayerListBox;
		cIfcElement*	pBriefingPlayerListBoxFrontEnd;

		cIfcElement*	pTestList;
		DWORD*			pdwSPDest;

		int				iConsoleDrawMode;   //0 for standard, 1 for debug mode

		cTutorial		Tutorial;

		VOID*			pCAccess;
		//VOID*			pInputState;
		//VOID*			pCommQueue;
		//VOID*			pGameWorld;

		HANDLE		hLogFile;
		void Write_Log(LPSTR Text);
		void WriteLog_Enter();
		void Init_LogFile();
		void Exit_LogFile();

		void Init(VOID* pCA);
		void Exit();

		void Init_Tutorial();
		void Process_Tutorial();

		void Process_Input();
			void Process_Fake_Hotkeys();  //just a quick and dirty "hotkey" enabling funciton
		

		void Signal_Chime(char* Text); //play chime, then flash notification

		void Fill_Listboxes();
			void Fill_ListBox_Command();
				void Fill_ListBox_Command_NoPending();
				void Fill_ListBox_Command_Pending();
			void Fill_ListBox_Info();
				void Fill_ListBox_Info_Units();
				void Fill_ListBox_Info_UnitTypes();
				void Fill_ListBox_Info_Structures();
				void Fill_ListBox_Info_StructTypes();
				void Fill_ListBox_Info_MaterialTypes();
				void Fill_ListBox_Info_Components();
				void Fill_ListBox_Info_CmpTypes();
				void Fill_ListBox_Info_Sectors();
			void Fill_ListBox_Display();
				void Fill_ListBox_Display_Unit();
				void Fill_ListBox_Display_Structure();
			void Fill_ListBox_List();
				void Fill_ListBox_List_Group();
				void Fill_ListBox_List_OrderQueue();
				void Fill_ListBox_List_CustBuild();
			void Fill_ListBox_Equip();
				void Fill_ListBox_Equip_Unit();

		void Process_Interface_Elements(cIfcElement* pFirst, cIfcElement* pLast); //06.11.2005 (to avoid confusion, put all the new interface functionality in one function
		void Process_Interface_Elements_B(cIfcElement* pHead); //07.22.2005
		void Process_Interface_Elements_C(cIfcElement* pElem); //10.30.2005 

		cIfcElement* Create_Button(LPSTR ButtonText, int iX, int iY);
		cIfcElement* Create_TextInputBox(int iX, int iY, int iWidth_Pixels, int iFont, BOOL bUnicodeFlag, void* pText, int iStringLimit);
		cIfcElement* Create_IfcPanel(int iLeft, int iRight, int iTop, int iBottom);
		cIfcElement* Create_IfcSecondaryPanel(int iLeft, int iRight, int iTop, int iBottom, int iColorIndex);
		cIfcElement* Create_IfcListBox(int iLeft, int iRight, int iTop, int iBottom);
		cIfcElement* Create_IfcListBox2(int iLeft, int iRight, int iTop, int iBottom, cIfcElement* pDataList);
		cIfcElement* Create_IfcListBox2Ex(int iLeft, int iRight, int iTop, int iBottom, cIfcElement* pDataList, int iUniformListingHeight);   //extended version
		cIfcElement* Create_StaticTextElem(LPSTR Text, int iFont, int iColor, int iX, int iY, DWORD dwXLimit);
		cIfcElement* Create_StaticTextElem(LPSTR Text, int iFont, int iColor, int iX, int iY);
		cIfcElement* Create_SquareButton(int iX, int iY, int iColorIndex);
		cIfcElement* Create_ObjectListing(int iX, int iY, int iControl, LPSTR DispText);
		cIfcElement* Create_SecondaryButton(int iLeft, int iRight, int iTop, int iBottom, LPSTR DispText, int iFont,  int iColorIndex);
		cIfcElement* Create_TutorialPage(int iLeft, int iRight, int iTop, int iBottom, int iXText, int iYText, int iXTextLimit, LPSTR MsgText, int iXButton, int iYButton, LPSTR ButtonText, int iFont, int iButtonJumpTo);

		void Add_ListBoxListing(cIfcElement* pParentElem, int iOffset, int Left, int iRight, int iTop, int iBottom);


		PCMD2* Add_StaticTextElem(cIfcElement* pElem, int iCategory, LPSTR Text, int iFont, int iColor, int iX, int iY);


		void Clear_SBs();  //clear both the SSB and the USB

		void AddUnits_SB_stack(int iMapIndex, BOOL ControlFlag, LPSBE pFirst, LPSBE pLast); //adds all of the units in the stack at that map coordinate (05.14.2002)
		int AddStructs_SB_single(int iMapIndex, LPSBE pFirst, LPSBE pLast); //03.29.2003
		int AddStructs_SB_singleB(int iID, LPSBE pFirst, LPSBE pLast);
		int AddUnits_SB_single(int iMapIndex, LPSBE pFirst, LPSBE pLast); //adds only the top unit in the stack at that map coordinate(05.14.2002)
		int AddUnits_SB_singleB(int iID, LPSBE pFirst, LPSBE pLast);
		void ReleaseUnits_SB_all(LPSBE pFirst, LPSBE pLast);	//removes all units from the unit selection buffer(USB) 05.14.2002
		void ReleaseStructs_SB_all(LPSBE pFirst, LPSBE pLast);
		void ReleaseUnits_SB_single(LPSBE pUS_Entry); // 06.26.2002
		void Process_SBs();	//performs operations during the game tick
		void Process_PendingCommand(); //performs operations regarding the pending command whenever requested(such as when the inputpacketbuffer receives a new packet)
		void Initiate_PendingCommand(DWORD dwType); //handles the initiation of a pending command
		SQCOORDSET Calc_PosMap(int iXPos, int iYPos);
		COORDSET   Calc_PosMap_ABS(int iXPos, int iYPos); //05.13.2002
		
		SQCOORDSET Calc_PosMinimap(int iXPos, int iYPos);
		SQCOORDSET Calc_PosMinimapZoom(int iXPos, int iYPos);

		LPRECT Fix_RectBox(LPRECT pInRect);	//fixes a RECT box so that the values can actually be drawn as a rectangle(the top , bottom, etc. values actually make sense)

		void Init_Console();	 //initialize the interface boxes in the console
			void Init_InfoGrids();
		void Init_MainMenu();    //maybe temp procedure to initialize main menu Interface Box(07.04.2002)
		void Init_Briefing();
		void Update_MainMenu(int iGridNum); //update parts of the main menu (05.10.2004)
		void MPSP_RunOnce(); //08.09.2005

		void Process_Console();  //function to process interaction with the console (03.12.2003)
		void Process_MainMenu(); //function to process interaction with the main menu (07.04.2002)

		void Set_IGridCell_AllFlags(IGRIDCELL* pCell, BOOL bDrawCell, BYTE bActiveCommand, BYTE bDataSources, BYTE bDrawType,
									WORD wISurfIndex, WORD wFrame, WORD wSurfRect_left, WORD wSurfRect_right, WORD wSurfRect_top,
									WORD wSurfRect_bottom, char* Text, int iFont, WORD PC_wDest, WORD PC_wType, LONG PC_wParam1, WORD PC_wParam2, WORD PC_wParam3,
									double PC_fParam4, double PC_fParam5, void* PC_pParam6, LONG PC_lExecTick, 
									DWORD dwNumObjRefs, DWORD dwObjRefType, VOID** pObjRefArray, DWORD* pdwIDArray);
		void Set_IGridCell_DrawFlags(IGRIDCELL* pCell, BOOL bDrawCell, BYTE bDrawType,
									WORD wISurfIndex, WORD wFrame, WORD wSurfRect_left, WORD wSurfRect_right, WORD wSurfRect_top,
									WORD wSurfRect_bottom, char* Text, int iFont);
		void Set_IGridCell_CmdFlags(IGRIDCELL* pCell, BYTE bActiveCommand, BYTE bDataSources, WORD PC_wDest, WORD PC_wType, LONG PC_wParam1, WORD PC_wParam2, WORD PC_wParam3,
									double PC_fParam4, double PC_fParam5, void* PC_pParam6, LONG PC_lExecTick);
		void Set_IGridCell_CmdFlags_Box(IGRIDCELL* pCellArray, WORD wStartIndex, WORD wWidth, WORD wHeight, WORD wElements_Array_Width,
									BYTE bActiveCommand, BYTE bDataSources, WORD PC_wDest, WORD PC_wType, LONG PC_wParam1, WORD PC_wParam2, WORD PC_wParam3,
									double PC_fParam4, double PC_fParam5, void* PC_pParam6, LONG PC_lExecTick);
		void Set_IGridCell_ObjRefFlags(IGRIDCELL* pCell, DWORD dwNumObjRefs, DWORD iObjRefType, VOID** pObjRefArray, DWORD* pdwIDArray); //05.20.2003
		void Update_MPConnect_Display(int iDispStringLimit);  //07.12.2002 -> refreshes text displays in the interface boxes
															  //to the correct connect information
		void Conv_String_WideToReg(unsigned short* Text);
		char* Conv_String_WideToReg2(unsigned short* Text);  //I'm not sure what the first one does (08.09.2003)
		
		void Edit_String(int iVirtualKey, char* TextEdit, WORD wMaxBufferSize);
		void Edit_String_Wide(int iVirtualKey, WCHAR* TextEdit, WORD wMaxBufferSize);

		~cInterface();
private:	
	//	LPSBE			pUSB_Trav;      //USB Traverser
	//	LPSBE			pUSB_Trav2;		//second traverser
	//	int				c1, c2, c3, c4;			//counters

};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//this following type "casts" the entire pData member for the CQ2TYPE_DRAWTEXT4 command as a single data structure
//so that the stuff inside(particularly the TEXT part) can be easily accessed
class cCQ2TYPE_DRAWTEXT4_DATA
{
public:
	BOOL bUnicodeFlag;
	DWORD dwRenderTargetCode;
	DWORD dwX;
	DWORD dwY;
	DWORD dwXLimit;
	DWORD dwFont;
	DWORD dwColor;
	void* Text;

	void Set(BOOL _bUnicodeFlag, DWORD _dwRenderTargetCode, DWORD _dwX, DWORD _dwY, DWORD _dwXLimit, DWORD _dwFont, DWORD _dwColor, void* _Text)
	{	bUnicodeFlag = _bUnicodeFlag; dwRenderTargetCode = _dwRenderTargetCode; dwX = _dwX; dwY = _dwY; dwXLimit = _dwXLimit; dwFont = _dwFont, dwColor = _dwColor; Text = _Text; }
};

//provides information about the classes(as of 05.30.2003, mainly what the formal name of the command is)
class cCmdInfoTable
{

public:

	//NOTE: "+1" is here because we are saving one for the "CANCEL" command, which is currently a CQ1 command (06.03.2003)
	LPSTR	Names[NUM_CQ2COMMANDS + 1];
	RECT	SourceButtonFrameRects[NUM_CQ2COMMANDS + 1];	//06.03.2003 specifies where on CmdButtons.bmp the button picture for this command is

	VOID*	pCAccess;

	void Init(VOID* pCA);

private:

};


class cCommQueue //"command queue"
{
public:
	
	LONG lNumCommands;

	LPPCMD		pQFirst; //anchor of the queue
	LPPCMD		pQLast;  //anchor of the queue

	//01.01.2003 (redundancy currently necessary because I do not want to reprogram the entire interface again)
	PCMD2*		pQ2First;  //anchor of type 2 queue
	PCMD2*		pQ2Last;   //anchor of type 2 queue

	VOID*		pCAccess; //pointer to the game world class so that commands can be executed

	HANDLE		hLogFile;

	BOOL		bQueueInUse;
	BOOL		bQueue2InUse;
	
	void Issue_Command(WORD wDest, WORD wType, LONG wParam1, WORD wParam2, WORD wParam3, double fParam4, double fParam5, void* pParam6, LONG lExecTick);
	void Execute_Commands();

	void Issue_Command_Q2(WORD wType, BYTE* pData, WORD wDataSize, BOOL bCopyFlag);  //extended to include copy option(05.20.2003)
	void Execute_Commands_Q2();

	int Execute_Command2(PCMD2* pCommand2);	 //08.07.2003 Function to execute a SINGLE command2 command, even if it is not in the queue

	//"time-saving" commands to assemble buffers for certain functions
	BYTE* Assemble_CommData_Notify_iControl(DWORD dwDPNID, int iControl, int* iRetBufferSize);
	BYTE* Assemble_CommData_Init_TimerID(DWORD dwTimerID, DWORD dwDPNID, DWORD dwNameLength, char* pName, int *iRetBufferSize);
	BYTE* Assemble_CommData_Start_Build_Unit_Preconfig(LONG lExecTick, DWORD dwStructID, DWORD dwUnitType, int* iRetBufferSize);
	BYTE* Assemble_CommData_Move2(LONG lExecTick, WORD wUnitID, LONG lTargetIndex, int* iRetBufferSize);
	BYTE* Assemble_CommData_Attack2_Unit(LONG lExecTick, WORD wUnitID, WORD wEnemyID, int* iRetBufferSize);
	BYTE* Assemble_CommData_ReportFrame2(BYTE bControlID, LONG lTickCount, int* iRetBufferSize);
	BYTE* Assemble_CommData_Batch_Move2(LONG lExecTick, LONG lTargetIndex, WORD* retUnitsIssue, VOID* USB_Buffer_List_Begin, VOID* USB_Buffer_List_End, int* iRetBufferSize); //draws list of unit IDs from a USB buffer list(does not add the anchors themselves
	BYTE* Assemble_CommData_Batch_Move2_Single_Unit(LONG lExecTick, DWORD dwTargetIndex, DWORD dwUnitID, int* iRetBufferSize);
	BYTE* Assemble_CommData_Batch_Attack2_Unit(LONG lExecTick, WORD wEnemyID, WORD* retUnitsIssue, VOID* USB_Buffer_List_Begin, VOID* USB_Buffer_List_End, int* iRetBufferSize);
	BYTE* Assemble_CommData_Batch_Attack2_Struct(LONG lExecTick, WORD wEnemyID, WORD* retUnitsIssue, VOID* USB_Buffer_List_Begin, VOID* USB_Buffer_List_End, int* iRetBufferSize);
	BYTE* Assemble_CommData_Batch_Guard(LONG lExecTick, LONG lGuardIndex, WORD* retUnitsIssue, VOID* USB_Buffer_List_Begin, VOID* USB_Buffer_List_End, int* iRetBufferSize);
	BYTE* Assemble_CommData_Build_Progress_Unit(VOID* pProgOrder, DWORD dwStructID, double ddResourceCost, double ddConstructionDifficulty, int iUnitType, int* iRetBufferSize);
	BYTE* Assemble_CommData_Attack_B_single(DWORD lExecTick, WORD wUnitID, WORD wEnemyID, WORD wEnemyObjType, int* iRetBufferSize);
	//note that these two following commands have the same format
	BYTE* Assemble_CommData_Build_Progress_Component(VOID* pProgOrder, DWORD dwStructID, DWORD dwCmpType, int* iRetBufferSize);
	BYTE* Assemble_CommData_Create_Component_A(VOID* pProgOrder, DWORD dwStructID, DWORD dwCmpType, int* iRetBufferSize);
	//that that these two following ALSO have the same format (no longer. 08.20.2003)
	BYTE* Assemble_CommData_Earmark_Components(VOID* pProgOrder, DWORD dwStructID, DWORD* pdwCmpIDs, int iNumIDs, int* iRetBufferSize);
	//note, the reason why we are receiving arrays of INTs is so that we can accept arrays with some invalid entries(ex. -6996) 08.20.2003
	BYTE* Assemble_CommData_Create_Unit_A(VOID* pProgOrder, DWORD dwStructID, DWORD utypehint, int* piCmpIDs, int iNumIDs, int* piTypes, int iTypes, int* iRetBufferSize);
	BYTE* Assemble_CommData_Init_Nation(int iCtrl, char* pName, int iNameLength, BOOL bAIFlag, int* iRetBufferSize);
	BYTE* Assemble_CommData_Blit(int iRenderTargetCode, int iImageSurface, int idestleft, int idesttop, int isrcleft, int isrcright, int isrctop, int isrcbottom, int* iRetBufferSize);
	BYTE* Assemble_CommData_BlitFit(int iRenderTargetCode, int iImageSurface, int idestleft, int idestright, int idesttop, int idestbottom, int isrcleft, int isrcright, int isrctop, int isrcbottom, int* iRetBufferSize);
	BYTE* Assemble_CommData_BlitBySurface(int iRenderTargetCode, LPDIRECTDRAWSURFACE7 pDDS, int idestleft, int idesttop, int isrcleft, int isrcright, int isrctop, int isrcbottom, int* iRetBufferSize);
	BYTE* Assemble_CommData_SetMainMenuMode(int iMode, int* iRetBufferSize);
	BYTE* Assemble_CommData_Colorfill(DWORD dwRenderTargetCode, DWORD dwTargetColor, DWORD dwLeft, DWORD dwRight, DWORD dwTop, DWORD dwBottom, int* iRetBufferSize);
	BYTE* Assemble_CommData_DrawText(DWORD dwRenderTargetCode, DWORD dwXPos, DWORD dwYPos, DWORD dwXLimit, DWORD dwFontType, DWORD dwFontColor, DWORD dwNumChars, LPSTR Text, int* iRetBufferSize);
	BYTE* Assemble_CommData_DrawText(DWORD dwRenderTargetCode, DWORD dwXPos, DWORD dwYPos, DWORD dwFontType, DWORD dwFontColor, DWORD dwNumChars, LPSTR Text, int* iRetBufferSize);
	BYTE* Assemble_CommData_DrawText2(BOOL bUnicodeFlag, DWORD dwRenderTargetCode, DWORD dwXPos, DWORD dwYPos, DWORD dwXLimit, DWORD dwFontType, DWORD dwFontColor, char* Text, int* iRetBufferSize);
	int Assemble_CommData_DrawText2ex(CommDataDrawText2* buffer, BOOL bUnicodeFlag, DWORD dwRenderTargetCode, DWORD dwXPos, DWORD dwYPos, DWORD dwXLimit, DWORD dwFontType, DWORD dwFontColor, char* Text);
	int Pack_CommData_DrawText2(PCMD2* pCmd, BOOL bUnicodeFlag, DWORD dwRenderTargetCode, DWORD dwXPos, DWORD dwYPos, DWORD dwXLimit, DWORD dwFontType, DWORD dwFontColor, char* Text);
	BYTE* Assemble_CommData_DrawText2(BOOL bUnicodeFlag, DWORD dwRenderTargetCode, DWORD dwXPos, DWORD dwYPos, DWORD dwFontType, DWORD dwFontColor, char* Text, int* iRetBufferSize);	
	BYTE* Assemble_CommData_DrawText3(BOOL bUnicodeFlag, DWORD dwRenderTargetCode, DWORD dwXPos, DWORD dwYPos, DWORD dwXLimit, DWORD dwFontType, DWORD dwFontColor, char** ppText, int* iRetBufferSize);
	BYTE* Assemble_CommData_DrawText3(BOOL bUnicodeFlag, DWORD dwRenderTargetCode, DWORD dwXPos, DWORD dwYPos, DWORD dwFontType, DWORD dwFontColor, char** ppText, int* iRetBufferSize);
	BYTE* Assemble_CommData_DrawIfcPanel(DWORD dwRenderTargetCode, DWORD dwLeft, DWORD dwRight, DWORD dwTop, DWORD dwBottom, int* iRetBufferSize);
	BYTE* Assemble_CommData_DrawIfcSecondaryPanel(DWORD dwRenderTargetCode, DWORD dwLeft, DWORD dwRight, DWORD dwTop, DWORD dwBottom, int iColorIndex, int* iRetBufferSize);
	BYTE* Assemble_CommData_DrawIfcListBox(DWORD dwRenderTargetCode, DWORD dwLeft, DWORD dwRight, DWORD dwTop, DWORD dwBottom, int* iRetBufferSize);
	BYTE* Assemble_CommData_Set_ActiveDevice(DWORD dwProviderID, DWORD dwDeviceID, int* iRetBufferSize);
	BYTE* Assemble_CommData_Set_Inputstring(char* pText, DWORD BufferLength, int* iRetBufferSize);
	BYTE* Assemble_CommData_Set_Inputstring_Wide(WCHAR* pText, DWORD BufferLength, int* iRetBufferSize);
	BYTE* Assemble_CommData_Set_Pdword(DWORD Val, DWORD* pdwPoint, int* iRetBufferSize);
	BYTE* Assemble_CommData_Copy_String(void* pString_Source, void* pString_Dest, int* iRetBufferSize);
	BYTE* Assemble_CommData_Send_Text(WCHAR* pString_Name, int iNameLength, LPSTR pString_Text, int iTextLength, int iStayTime, int* iRetBufferSize);
	BYTE* Assemble_CommData_Send_Text2(WCHAR* pName, char* pText, int iTextBufferSize, int iStayTime, int* iRetBufferSize);
	BYTE* Assemble_CommData_DispChatText(DWORD dwRenderTargetCode, int iX, int iY, int iLines, int* iRetBufferSize);
	BYTE* Assemble_CommData_ListBox_Execute_Ifc(cIfcElement* pParent, cIfcElement* pListing, DWORD dwOffset, DWORD dwElemCat,int* iRetBufferSize);
	BYTE* Assemble_CommData_ListBox_Conditional_Blit(cIfcElement* pParent, int iRenderTargetCode, int iImageSurface, int idestleft, int idesttop, int isrcleft, int isrcright, int isrctop, int isrcbottom, int* iRetBufferSize);
	BYTE* Assemble_CommData_ListBox_Disp_Slider(cIfcElement* pParent, int iRenderTargetCode, int iDispCode, int* iRetBufferSize);
	BYTE* Assemble_CommData_Set_ActiveIGrid(DWORD dwIBox, DWORD dwActive, int* iRetBufferSize);
	BYTE* Assemble_CommData_Send_InputPacket_ObjRef(DWORD dwPacketType, DWORD dwObjectsType, DWORD dwNumObjects, DWORD* pdwIDs, VOID** pObjectArray, int* iRetBufferSize);
	BYTE* Assemble_CommData_Send_InputPacket_Param(DWORD dwPacketType, DWORD dwParameterType, DWORD dwNumParamItems, VOID** pItemArray, int* iRetBufferSize);
	BYTE* Assemble_CommData_Two_Dword(DWORD dword1, DWORD dword2, int* iRetBufferSize);
	BYTE* Assemble_CommData_Unload_A(LONG lExecTick, int iTargetUnitID, int iTargetLocation, int* iRetBufferSize); //10.15.2006
	BYTE* Assemble_CommData_Unload_B(LONG lExecTick, int iCargoUnitID, int iTargetUnitID, int iTargetLocation, int* iRetBufferSize); //10.15.2006	
	BYTE* Assemble_CommData_Unload_All(LONG lExecTick, int iTargetUnit, int iTargetLocation, int* iRetBufferSize); //10.16.2006
	BYTE* Assemble_CommData_Load_Unload_Material(LONG lExecTick, int iUnitID, int iMapIndex, int* iRetBufferSize);  //the parameters are the same for both commands... 10.19.2006
	BYTE* Assemble_CommData_Struct_Turn_Off_On(LONG lExecTick, int iStructID, int* iRetBufferSize);
	BYTE* Assemble_CommData_Tank_Set_Type(LONG lExecTick, int iStructID, int iMaterialType, int* iRetBufferSize);
	BYTE* Assemble_CommData_Build_Structure_ONE(LONG lExecTick, int iID_Unit, int iStructType, int iMapIndex, int* iRetBufferSize);
	BYTE* Assemble_CommData_Cancel_Progorder_Top(LONG lExecTick, int iStructID, int* iRetBufferSize);
	BYTE* Assemble_CommData_Move_B_Single(LONG lExecTick, WORD wUnitID, LONG lTargetIndex, int* iRetBufferSize);


	int	Scan_Inventory_A(int iNationControl, int iOwnerType, int iOwnerID, int* pdwTypes, int* pdwReturnIDs, int iNumEntries, BOOL bMarkFlag, BOOL bSelectOwnFlag);
	double Calc_ResourceCost(int* pdwAssembleTypes, int* pdwHaveIDs, int iArraySize); //calculates the cost of a unit built with the list of components in pdwAssembleTypes, with the components in pdwHaveIDs already in possession. Returns the result

	void Write_Log(LPSTR Text);
	void WriteLog_Enter();
	void Init_LogFile();
	void Exit_LogFile();
	void Init(VOID* pCA);
	void Exit();
	~cCommQueue();

private:
	LPPCMD		pQTr, pQTrNext, pQTrPrev, pQTr2;	 //traversers for the command list

};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
class cBriefing
{
public:
	char BriefText[5000];

	cBriefing() 
	{
		char etr[3];
		etr[0] = 13;
		etr[1] = 10;
		etr[2] = 0;
		ZeroMemory(BriefText, sizeof(char)*5000);

		strcat(BriefText, "--MISSION BRIEFING--");
		strcat(BriefText, etr);
		strcat(BriefText, etr);
		strcat(BriefText, etr);
		strcat(BriefText, "Secure Control of the Planet");
		strcat(BriefText, etr);
		strcat(BriefText, "Terminate all enemy forces");
		strcat(BriefText, etr);
		strcat(BriefText, etr);
		strcat(BriefText, "End of Message.");
	}

private:

};

class cTileTypeInfo
{
public:
	double ResourcePayout[NUM_MATERIAL_TYPES];
	cTileTypeInfo()
	{
		for (int lc1 = 0; lc1 < NUM_MATERIAL_TYPES; lc1++)
			ResourcePayout[NUM_MATERIAL_TYPES] = 0;
	}
private:

};

class cGameWorldA
{
	public:
		cBriefing	Briefing;
		cTileTypeInfo	TileInfos[NUM_MAPTILE_TYPES];

		int			iHeight;
		int			iWidth;
		int			iVersion;
		int			iID;
		//int			iMap_xview;  //DO NOT USE THIS VAR ANYMORE (cInterface now holds it 12.28.2001)
		//int			iMap_yview;  //DO NOT USE THIS VAR ANYMORE (cInterface now holds it 12.28.2001)
		HANDLE		hMapFile;   // handle of the file containing the map data
		HANDLE		hLogFile;   // handle of the file where log information will be written
		LPMTile		pMap;
		DWORD		dwNumBytesRead;

		VOID*		pCAccess;
		
		void Write_Log(LPSTR Text);
		void WriteLog_Enter();
		void Init(LPSTR FileName, VOID* pCA);
		void Exit();			//TEST to see if it can fix a memory leak
		void Load_Map_101(LPSTR FileName); //load map with version 1.01 format
		void Load_Map_200(LPSTR FileName); //load map with version 2.00 format
		void Modify_Map();
		void Modify_MTile_4Way(int MapIndex);
		void Modify_MTile_8Way(int MapIndex);
		void Modify_MTile_Ocean(int MapIndex);
		void Modify_MTile_River(int MapIndex);

		int Determine_Link_DrawFrame(int iMapIndex);
		void Add_Link(int iMapIndex);

		BOOL isNextToOcean(int iIndex);

		~cGameWorldA();

		int UniqueMapIDOf(char* FileName);
		void Register_Unit(SQCOORDSET Pos_square, VOID* pUnitA);
		void Unregister_Unit(SQCOORDSET Pos_square, VOID* pUnitA);
		void Register_Structure(SQCOORDSET Pos_square, VOID* pStructureA);
		void Unregister_Structure(SQCOORDSET Pos_square, VOID* pStructureA);
		void Reveal(int iX_tile, int iY_tile, int iRad, int iControl);
		void Possess(int iStructID, int iX_tile, int iY_tile, int iRad, int iControl, int iControlLevel);
		void cGameWorldA::Assess(); //09.20.2006
		void Read_To_Stop(char* LoadBuffer, char stopchar);
		void Read_To_Stop(char* LoadBuffer, char stopchar, int iBufferLimit);
		
		int Link_Point_to_Point(int iMapIndex1, int iMapIndex2);

		SQCOORDSET Select_Scouting_Location(int iControl);

		BOOL IsValidBuildLocation(SQCOORDSET sqLocation, int iStructType);
		BOOL IsValidBuildLocation(int iX_Square, int iY_Square, int iStructType);
		//this one checks all of the offsets of the square
		BOOL BigIsValidBuildLocation(int iX_Square, int iY_Square, int iStructType);

		BOOL IsValidLinkLocation(SQCOORDSET sqLocation);
		BOOL IsValidLinkLocation(int iX_Square, int iY_Square);

		void RebuildMTLinkLists();

	private:
		int		c1, c2;  //counters
		BYTE*	pByteRead;
		void* pLoadBuffer;

		void Init_LogFile();

		DWORD   dwErrorCode;
		char	Text[50];

		int		ABOVE, ABOVE_LEFT, ABOVE_RIGHT, LEFT, RIGHT, BELOW, BELOW_LEFT, BELOW_RIGHT;  //used for mapmodify


};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
#define ID_EDITBOX1	1050
#define ID_EDITBOX2	1051
#define ID_EDITBOX3	1052
#define ID_EDITBOX4	1053
#define ID_EDITBOX5	1054
#define ID_EDITBOX6	1055
#define ID_EDITBOX7	1056



class cAPPDATA
	{
	private:
	public:
			HINSTANCE InstanceHandle;
			HINSTANCE PrevInstanceHandle;
			LPSTR	  CommandLine;
			HWND	  hMainWnd;
			HWND	  hSecondWnd;
			BOOL	  bRunFlag;			//set to 1, if set to 0, exit program
			MSG		  WinMsg;			//holds windows messages
			HDC		  hdc00;
			

			VOID* pCAccess;

			//HANDLES on Secondary Window
			HWND	  hEditBox;
			HWND	  hEditBox2;
			HWND      hEditBox3;
			HWND	  hEditBox4;
			HWND	  hEditBox5;
			HWND      hEditBox6;
			HWND	  hEditBox7;
			HWND	  hText1;
			void Secondary_Window_Processing();
			char BoxBuffer[10000];

			WIN32_FIND_DATA	DirFileInfo;
				
			char*	  achMapNames[MAX_MAPS];
			int		  achMapIDs[MAX_MAPS];
			int		  iHighestMapIndex;

			
			HANDLE	  hMapFile;
			HANDLE	  hLogFile;

			BYTE	  bRunStage;		//07.01.2002
			int			iAIToken;

		void Init(HINSTANCE IHandle, HINSTANCE PIHandle, LPSTR CLine, LPSTR Windowname, WNDPROC CallBackProc, WNDPROC CallBackProc2, VOID* pCA);
		void Find_Maps();
		void Read_To_Stop(char* LoadBuffer, char stopchar);
		int Check_Map(LPSTR FileName); //05.15.2004, returns ID of map
		
		void Init_LogFile();
		void Write_Log(LPSTR Text);
		void WriteLog_Enter();
		void Exit_LogFile();

		void Create_Secondary_Window();

		
		void Exit();
		~cAPPDATA();

	};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
class cTimer
	{	
	
	public:
		LONGLONG	lFrequency;
		LONGLONG	lStartCounts[300];
		LONGLONG	lEndCounts[300];
		LONGLONG	lTimeElapsed[300];
		LONGLONG	lStartTime;	 //the time the maincycle started(for debug purposes 07.01.2002)
		LONGLONG	lCurrTime;   //the current count/time

		int			iFPS;
		int			iCountsPerTick;

		int	iCurrExecCount; //stores how long since the last process execution
		int			iExecMode;     //determines which portion of the program executes, based on the time...
		int			iOldMode;

		int			iCurrTick;	   //the current tick count (07.01.2002)
		int			iCurrFrame;		//the current frame count (04.17.2003) graphics draws only
		int			iTickReports[TEMP_DP_MAXPLAYERS]; //reports of the latest reported tick counts from remote players, indexed by their iTimerIDs (07.01.2002)
		int			iTickReportIDCodes[TEMP_DP_MAXPLAYERS]; //identifies whether a given iTimerID is actually being updated by an active player (nonnegative if yes, negative if no)
		DWORD		dwDPNIDs[TEMP_DP_MAXPLAYERS]; //receives the DPNID corresponding to this iTimerID during the INIT_TIMERID command
		char		Names[TEMP_DP_MAXPLAYERS][TEMP_STD_STRLEN];
		int			iControls[TEMP_DP_MAXPLAYERS]; 

		int			iContiguousDraws;  //records the number of times the GDRAW has occurred in a row, uninterrupted by a TICK command

		BOOL		bLagging;
		int			iLaggingPlayer;

		VOID*		pCAccess;  
		HANDLE		hLogFile;

		//for timer thread
		BOOL		bTimerThread; //use or not (set externally)
		LONGLONG	llSubCount; //
		BOOL	    bSubCountInUse;

		void Exit();

		void Init(int iCPT, VOID* pCA);
		void StartCount(int CountIndex);
		void EndCount(int CountIndex);
		void Set_CPT_FPS(int iFPS);
		void Process_Counts();

		int Ret_Rand();

		void Init_LogFile();
		void Write_Log(LPSTR Text);
		void Exit_LogFile();
		void WriteLog_Enter();
	private:


	};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
typedef struct _CMPDRAWOFFSETARRAY
{

	int		XOffsets[UNIT_NUMDIRS];
	int		YOffsets[UNIT_NUMDIRS];

} CMPDRAWOFFSETARRAY, * PCMPDRAWOFFSETARRAY;

//contains reference information for drawing component offsets
class cCmpOffsetTable
{
public:
	CMPDRAWOFFSETARRAY		StdDist_From_Center[40];  //
	CMPDRAWOFFSETARRAY		StdDist_From_Center8[40]; //same, but only with 8 directions

	void Init();

private:

};

typedef struct _CMPFRAMEDRAWINFO   //structure with additional information for drawing on frames
{
	CMPDRAWOFFSETARRAY*			pOffsets;	//identifies what offsets to draw off the unit for each rotation of the unit
	int							iZLevel;	//determines in what order to draw the components on (a sort of simple "painter's algorithm" 01.07.2004)
	int							iZLevelBase;	//the level to which any component here is attached
	int							iStartingIndex;     //determines which of the entries should be remapped to Direction 0, and so forth

} CMPFRAMEDRAWINFO, * PCMPFRAMEDRAWINFO;

//01.16.2004
/*
typedef struct _OBJECTFLIGHTINFO
{
	double			fPos_z;
	double			fAirAcceleration;
	double			fAirDeceleration;

} OBJECTFLIGHTINFO, * POBJECTFLIGHTINFO;
*/ //disabled in favor of a more "integrated" approach 

//info on components(02.11.2003)
typedef struct _CMPFRAMEINFO	//information block for frame-specific component information
{
		int iIntegrity; //some combination of mass and integrity may determine the frame's BASE hit points in the unit

		int Capacity; //array of ints, denoting the number and size of components
					   //that can be fit into this frame
		int iNumVisibleHardPoints; //the number of visible points
		CMPFRAMEDRAWINFO*	VisHardPoints;  //allocate array of hard point draw information 

		//01.16.2004
		float	fAccelBonus;		//any special attributes(such as nitros?) that enabled special acceleration bonus over what the engine alone can achieve(01.16.2004)
		float	fDecelBonus;		//any special attributes(such as special air-brakes this frame might have), that increase it's deceleration capable over what is determined by the engine(01.16.2004)

		float MoveCharacteristics[NUM_MAPTILE_TYPES];	//defines movement of this frame across different types of terrain
		BYTE  MoveExtCharacteristics;	//"extended characteristics array" ... defines more things about a frame(such as whether it can stop in the air --> ex. aircraft)
		//NOTE: dictates percentage of speed that unit can achieve on terrain....negative value indicates impassable(03.21.2003)

} CMPFRAMEINFO, * PCMPFRAMEINFO;
//NOTE: for now, we depart from reality by appointing the "frame" as the arbiter
//of terrain traversal capabilities, as opposed to possibly the engine.(02.11.2003)

typedef struct _CMPENGINEINFO //information block for "engine"(read powerplant) components
{
		int iPower; //the amount of power that this engine delivers

} CMPENGINEINFO, * PCMPENGINEINFO;

typedef struct _CMPSTORAGEINFO
{
	int iStorageBonus;    //unit storage volume
	double ddMaterialStorageBonus;  //raw materials

} CMPSTORAGEINFO, * PCMPSTORAGEINFO;

typedef struct _CMPDEFENSEINFO	//information block for "defense" components(read armor, possibly something else)
{
		int iDefenseBonus;  //possibly the number of hit points the component directly adds to the unit

} CMPDEFENSEINFO, * PCMPDEFENSEINFO;

typedef struct _CMPWEAPONINFO
{
		int iFireRate;
		int iAttackProcedure;		//flag to indicate how this weapon is fired
		float fRange;			
		int iProjectileType;		//in the case that the attack procedure is ATTACK_PROJECTILE, define the type created
		int iPreAttackProcedure;	 //flag to indicate what this weapon needs to do before firing

		double fDmgRate; //calculated on load
			
} CMPWEAPONINFO, * PCMPWEAPONINFO;

typedef struct _CMPTOOLINFO  //weapon and tool COULD have been generalized to one class. oh well.
{
		int iUseRate;
		float fRange;
		int iUseProcedure;		//flag to indicate how this tool is used
		int iPreUseProcedure;	//flag to indicate what this tool needs to do before using(03.26.2003)

} CMPTOOLINFO, * PCMPTOOLINFO;


//new class, general draw info for components (01.07.2004)
typedef struct _CMPGENDRAWINFO
{
	
		BOOL	bValid;				  //is the information in this structure valid?
		BYTE	bDrawCharacteristics; //how we draw this component
		BYTE	bDirCharacteristics;  //how this component "moves" with the unit
		int iRotateFrames;	    //the number of frame rotations that are available for drawing
		BOOL 	bEmFlag; //whether to "mark" the unit with its national colors or not

		cSrcGraphicsInfo	SourceGraphics; //07.11.2004

} CMPGENDRAWINFO, * PCMPGENDRAWINFO;


//I am noticing more and more that "component" is something of an artificial categorization
//I seem to need to separate out component classes in everything I do anyway :-(
class cCmpInfo
{
	public:
		cCmpInfo(); //constructor

		int		iClass; //component class(ex. engine, weapon, frame, etc.)
		int		iType;	//unique type ID
		int		iSize;	//some measure of the size of the frame, may be useful later
		int		iMass;	//self explanatory, will calculate unit's maneuverability and speed characteristics

		double	ddResourceCost;	//how much this component costs
		cMaterialCollection MaterialCost; //how much this component costs
		double	ddSupport;		//how much this component requires to support inside an active unit, every "turn"
		LPSTR	Name;	//official name

		//put in new class CMPGENDRAWINFO 01.07.2004
//		BYTE	bDrawCharacteristics; //how we draw this component
//		BYTE	bDirCharacteristics;  //how this component "moves" with the unit
//		int iImageSurfIndex;	//the image surface from which the graphic is drawn
//		int	iXFrameSize;		//size of the image frame, x pixels
//		int iYFrameSize;		//size of the image frame, y pixels 
//		int iImageFramesFormat; //the layout of the image frames on the image surface
		CMPGENDRAWINFO	DrawSchemes[NUM_ZLEVELS];	

		VOID*	pNext_cmplist;
		VOID*	pPrev_cmplist;
		VOID*	pNext_classlist;
		VOID*	pPrev_classlist;
					

		//type-specific component information blocks
		CMPFRAMEINFO*	pFrameInfo;
		CMPENGINEINFO*	pEngineInfo;
		CMPDEFENSEINFO* pDefenseInfo;
		CMPWEAPONINFO*	pWeaponInfo;
		CMPTOOLINFO*	pToolInfo;
		CMPSTORAGEINFO* pStoreInfo;
		

	private:

};

class cCmpInfoTable
{
	//the table functions like this....when components are added,
	//they are added as nodes to the complist.
	//to prepare the table for use, the table is reconstructed,
	//such that the ComponentsTable gets the correct allocation pointers,
	//and the class lists are rebuilt correctly.
	//anytime a component is added, the whole table must be reconstructed
	//before it can used correctly
	public:	
		cCmpInfo**		pCmpTable;	//array of CompInfo POINTERS 
	
		cCmpInfo*			pCTL_First;	//list anchors
		cCmpInfo*			pCTL_Last;		

		//anchors for individual component classes
		cCmpInfo*			pEngineTypes_First;
		cCmpInfo*			pEngineTypes_Last;

		cCmpInfo*			pFrameTypes_First;
		cCmpInfo*			pFrameTypes_Last;

		cCmpInfo*			pDefenseTypes_First;
		cCmpInfo*			pDefenseTypes_Last;

		cCmpInfo*			pWeaponTypes_First;
		cCmpInfo*			pWeaponTypes_Last;	
		
		cCmpInfo*			pToolTypes_First;
		cCmpInfo*			pToolTypes_Last;

		cCmpInfo*			pStorageTypes_First;
		cCmpInfo*			pStorageTypes_Last;

		int					iHighestTypeID;
	
		VOID*					pCAccess;

		HANDLE					hLogFile;

		//limited support for up to 5 add "hardpoints"  (and two "Z" levels - 01.07.2004)
		void Add_FrameType_Simple(int iType, 
							 LPSTR Name, 
							 int iSize,
							 int iMass,
							 double ddResourceCost,
							 double ddSupportCost,
							 BOOL bMarkFlag,
							 int iIntegrity,
							 int iVisHardPoints,
							 float fAccelBonus, //negative accel/decel to indicate "infinity"(instant) accel /decel
							 float fDecelBonus,
							 int iCapacity,
							 CMPDRAWOFFSETARRAY* pOffsetArray1,
							 int iStartingIndex1,
							 int iZLevel1,     //clarification: iZLevel, the level at which the attached component is drawn
							 int iZLevelBase1, //clarification: iZLevelBase, the frame level to which that component is attached	
							 CMPDRAWOFFSETARRAY* pOffsetArray2,
							 int iStartingIndex2,
							 int iZLevel2,
							 int iZLevelBase2, //clarification: iZLevelBase, the frame level to which that component is attached	
							 CMPDRAWOFFSETARRAY* pOffsetArray3,
							 int iStartingIndex3,
							 int iZLevel3,
							 int iZLevelBase3, //clarification: iZLevelBase, the frame level to which that component is attached	
							 CMPDRAWOFFSETARRAY* pOffsetArray4,
							 int iStartingIndex4,
							 int iZLevel4,
							 int iZLevelBase4, //clarification: iZLevelBase, the frame level to which that component is attached	
							 CMPDRAWOFFSETARRAY* pOffsetArray5,
							 int iStartingIndex5,
							 int iZLevel5,
							 int iZLevelBase5, //clarification: iZLevelBase, the frame level to which that component is attached	
							 float fTrav_Grassland,
							 float fTrav_Ocean,
							 float fTrav_Mountain,
							 float fTrav_Hill,
							 float fTrav_River,
							 float fTrav_Forest,
							 float fTrav_Air,
							 BYTE bMoveExtCharacteristics,
							 BYTE bDrawCharacteristics_z0,
							 BYTE bDirCharacteristics_z0,
							 int iRotateFrames_z0,
							 BOOL bValid_z0,
							 BYTE bDrawCharacteristics_z1,
							 BYTE bDirCharacteristics_z1,
							 int iRotateFrames_z1,
							 BOOL bValid_z1,
							 BOOL bReconstructFlag);
		void Add_EngineType_Simple(int iType, 
							 LPSTR Name, 
							 int iSize,
							 int iMass,
							 double ddResourceCost,
							 double ddSupportCost,
							 BOOL bMarkFlag,
							 int iPower,
							 BYTE bDrawCharacteristics,
							 BYTE bDirCharacteristics,
							 int iRotateFrames,
							 BOOL bValid,
							 BOOL bReconstructFlag);
		void Add_DefenseType_Simple(int iType, 
							 LPSTR Name, 
							 int Size,
							 int iMass,
							 double ddResourceCost,
							 double ddSupportCost,
							 BOOL bMarkFlag,
							 int iDefenseBonus,
							 BYTE bDrawCharacteristics,
							 BYTE bDirCharacteristics,
							 int iRotateFrames,
							 BOOL bValid,
							 BOOL bReconstructFlag);
		void Add_StorageType_Simple(int iType, 
							 LPSTR Name, 
							 int iSize,
							 int iMass,
							 double ddResourceCost,
							 double ddSupportCost,
							 BOOL bMarkFlag,
							 int iStorageBonus,
							 double ddMaterialStorageBonus,
							 BYTE bDrawCharacteristics,
							 BYTE bDirCharacteristics,
							 int iRotateFrames,
							 BOOL bValid,
							 BOOL bReconstructFlag);
		void Add_WeaponType_Simple(int iType, 
							 LPSTR Name, 
							 int iSize,
							 int iMass,
							 double ddResourceCost,
							 double ddSupportCost,
							 BOOL bMarkFlag,
							 int iProjectileType,
							 int iFireRate,
							 int iPreAttackProcedure,
							 int iAttackProcedure,
							 float fRange,
							 BYTE bDrawCharacteristics,
							 BYTE bDirCharacteristics,
							 int iRotateFrames,
							 BOOL bValid,
							 BOOL bReconstructFlag);		
		void Add_ToolType_Simple(int iType, 
							 LPSTR Name, 
							 int iSize,
							 int iMass,
							 double ddResourceCost,
							 double ddSupportCost,
							 BOOL bMarkFlag, 
							 int iUseRate,
							 int iPreUseProcedure,
							 int iUseProcedure,
							 float fRange,
							 BYTE bDrawCharacteristics,
							 BYTE bDirCharacteristics,
							 int iRotateFrames,
							 BOOL bValid,
							 BOOL bReconstructFlag);	
		void Init(VOID* pCA);
		void Exit();

		void Diag_Output(); //report on the state of the class

		void Init_LogFile();
		void Write_Log(LPSTR Text);
		void Exit_LogFile();
		void WriteLog_Enter();
		
		void Reconstruct();

	private:

};

//02.12.2003
//as for the "physical" components themselves, there will be no lists or tables
//to contain them. they will be allocated as created by the player, and
//"attached" to cUnitA units as units are assembled.....
//CORRECTION. there will be a cComponentListA class that will serve as component
//storages for the various structures that will bandy the components around
//(units, national inventory queues, etc.), and will be able to create or insert
//components
class cComponentA
{

	public:
		int			iID;		//unique ID, in the case we need it(we do need it! 03.27.2003)
		int			iClass;		//hold the class here, should it be needed
		int			iType;		//the real reference to type data

		void		Set_OwnerType(int iOT) { iOwnerType = iOT; }
		void		Set_OwnerID(int iID) { iOwnerID = iID; }

		int			iOwnerType;	//OBJTYPE_UNIT or OBJTYPE_STRUCTURE
		int			iOwnerID;	 //the ID of the unit or structure holding ownership. 

		double		fEfficiency;  //(ostensibly) between 0 and 1, determines
								  //what percentage of the "benefits" of this component
								  //are still usable(in the case of swapping out damaged
								  //armor, stuff like that)
//		int			iDir_Offset;  //the direction this component is facing, offset from the unit's direction
		double		fDir_Offset;  //offset from the main...in degrees
		double		fDir_Offset_Default; //the "default" offset of the unit when it is not doing anything

		CMPDRAWOFFSETARRAY* pOffsetTable;	  //in the case when a component needs to know exactly where IT's at
		int			iStartingIndex;
		int			iZLevel_attached;		//the Z-level of the frame where this component is attached (01.08.2004) sigh....

		LONGLONG	llNextUse;		//specific for WEAPON and TOOL types only(last use time for this component)

		

		//sigh
		void		Set_PrevList(VOID* pP) { pPrev_list = pP; }
		void		Set_NextList(VOID* pP) { pNext_list = pP; }
		void		Set_PrevClassList(VOID* pP) { pPrev_classlist = pP; }
		void		Set_NextClassList(VOID* pP) { pNext_classlist = pP; }
		void		Set_PrevExtLink(VOID* pP) { pPrev_extlink = pP; }
		void		Set_NextExtLink(VOID* pP) { pNext_extlink = pP; }
			
		VOID*		pPrev_list;
		VOID*		pNext_list;
		VOID*		pPrev_classlist;
		VOID*		pNext_classlist;
		VOID*		pPrev_extlink;  //08.11.2003
		VOID*		pNext_extlink;	//08.11.2003 a third set of links for linking in multiple queues at once (currently
									// being used for linking 


	private:

};

class cComponentListA
{
	public:			

		cComponentA*	pCmpList_First;
		cComponentA*	pCmpList_Last;

		//anchors for individual component classes
//abandoned in favor of indexed sort 10.11.2003
/*		cComponentA*			pEngines_First;
		cComponentA*			pEngines_Last;

		cComponentA*			pFrames_First;
		cComponentA*			pFrames_Last;

		cComponentA*			pDefenses_First;
		cComponentA*			pDefenses_Last;

		cComponentA*			pWeapons_First;
		cComponentA*			pWeapons_Last;	
		
		cComponentA*			pTools_First;
		cComponentA*			pTools_Last;
*/

		cComponentA*			pLByClass_First[NUM_COMPONENT_CLASSES];
		cComponentA*			pLByClass_Last[NUM_COMPONENT_CLASSES];


		int					iNumComponents; //keeps tabs on number of components in list (08.09.2003)

		VOID*				pCAccess;
		HANDLE				hLogFile;
		
		cComponentA* Add_Component(int iType);
		void Insert_Component(cComponentA* pComponent); //automatically extracts it from its source
		void Export_Component(cComponentA* pComponent, cComponentA* pLinkTarget, BOOL bLocalFlag);
		void Destroy_Component_byID(int iID);
		void Destroy_Component_byaddr(cComponentA* pComponent, BOOL bLocalFlag);  //flag to make sure component is local

		void Exit();
		void Init(VOID* pCA, LPSTR LogFileName);
		void Init_LogFile(LPSTR LogFileName);
		void Write_Log(LPSTR Text);
		void Exit_LogFile();
		void WriteLog_Enter();


	private:


};

//class with extended functionality over cComponentListA, but with a larger memory footprint(is that the right word?) so
//as to make it undesirable to use it in every unit and structure (08.11.2003)
//currently intended for use in national inventories
//this is the first time I am using an inherited class.
//as a result of needing compatibility between ListB and ListA instances, the "Type Lists"(sorting of components into lists
//by ID, is forced to be implemented by constantly recompiling all the TL's in one shot rather than updating
//the list incrementally.
//however, the upshot is that this allows cComponentListB to be implemented as an inherited class from cComponentListA.
//the speed at which all TLs can be recompiled is critical to how widespread the use of cComponentListB will be
class cComponentListB : public cComponentListA
{
public:
		
		cComponentA**			ppCmpTL_First;  // for allocation of an array of pointers(to form lists)
														// to link the list's components by type
		cComponentA**			ppCmpTL_Last;

		int						iNumCmpTypes;

		LONGLONG				llLastRecompiledTick;	//the tick the TL was recompiled on

		//it appears we are forced to implement TLs this way(as opposed to keeping a running 
		void Link_TL(cComponentA* pComponent); //used to link a component into 
		void Recompile_TL(); //repopulate the TL using extlinks to see what components are available by what type
							

		void Exit_B();
		void Init_B(VOID* pCA, LPSTR LogFileName, int NumComponentTypes); //call this instead of the inherited INIT

private:

};  

class cCmpIDTable
{


public:
		cComponentA**	pTable;	//to allocate an array of pointers

		VOID*	pCAccess;

		int iID_Last_Assigned;   //only to be used by the MASTER component table(03.28.2003)

		void Init(VOID* pCA);
		void Exit();

		void Register_Component(cComponentA* pComponent, int iID);
		void Unregister_Component(int iID);

private:


};



//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
class UNITATTACKINFO  //structure that holds information about attack targets....used during attacking (06.07.2002)
{
public:
	double		fAttackDirection;  //direction to target
	int			iEnemyID;		   //ID of target
	int			iTargetType;		//type of target being attack(ground, unit, structure, etc.)
	COORDSET	Pos_ABSTarget;
//	VOID*	pEnemy;				//pointer to enemy (use this? I dunno)
private:


};

class UNITGUARDINFO	//structure that holds information during guard mode (06.07.2002)
{

public:
	SQCOORDSET	Pos_guard;
private:

};

class UNITBUILDINFO //structure that holds information during build mode (10.11.2003)
{
public:
	SQCOORDSET  Pos_build;
	COORDSET	Pos_ABSbuild;
	int			iBuildOrderNumber;
	int			iBuildType;
	//void*		pStructure;
	int iStructureID;

private:

};

class UNITOVERLOADINFO //holds information about overloads being applied to this unit
{
public:
	int iOverLoadType; //the material type that is being overloaded on this unit
	BOOL bActive; //whether it is active or not
private:
};

//info structure to specify the exact drawing of components on a unit(03.01.2003)
typedef struct _UNITCMPDRAWINFO
{
	cComponentA*	pComponent;		//pointer to component to draw;
	CMPDRAWOFFSETARRAY*	pOffsets;	//holds the offset information
	int					iZLevel;	//ZLevel to draw this component
	int					iZLevelBase;   //Frame Base level to which this component is attached
	int					iStartingIndex;		//the remapping index value
	//int					iImageSurfIndex;	//for speed, simply have the value here also (disabled, obsolete 07.13.2004)
	

} UNITCMPDRAWINFO, * PUNITCMPDFRAWINFO;

//01.08.2004
typedef struct _UNITMULTIFIREINFO
{
	cComponentA**	ppSelectedComponents;
	int				iNumSelectedComponents;		//keep track of the size of the array
	
	double*			arrAttackDirections; //local attack directions for all components in ppSelectedComponents
	float			fMinRange;		     //the range at which all weapons are able to fire				
	
	int				iMultiFireMode;      //store it here just in case

} UNITMULTIFIREINFO, * PUNITMULTIFIREINFO;

//10.15.2006
class cStorageModule
{
public:
	cStorageModule() 
	{
		for (int lc1 = 0; lc1 < 10; lc1++)
			iUnitIDs[lc1] = -1;
		iNumFree = 10;
		iNumStored = 0;

		ddMaterialCapacity = 0;
		ddMaterialStored = 0;
		iMaterialType = -1;

		iCarrierID = -1;
		bIsCarrierStructure = FALSE;
		}

	int iCarrierID;
	BOOL bIsCarrierStructure;
	
    int iUnitIDs[10];
	int iNumFree;
	int iNumStored;

	double ddMaterialCapacity; //how much raw material can this module store
	double ddMaterialStored;   //how much raw material is currently stored
	int iMaterialType;         //the type of material stored here



	int Add_Unit(int iID)
	{
		if (iID < 0)
			return -1;
		//find open slot and stick
		for (int lc1 = 0; lc1 < 10; lc1++)
		{
			if ( iUnitIDs[lc1] == -1)
			{
				iUnitIDs[lc1] = iID;
				iNumFree--;
				iNumStored++;
				return lc1;
			}
		}
		//otherwise return -1
		return -1;
	}
	int Remove_Unit(int iID)
	{
		if (iID < 0)
			return -1;
		//find unit
		for (int lc1 = 0; lc1 < 10; lc1++)
		{
			if ( iUnitIDs[lc1] == iID)
			{
				iUnitIDs[lc1] = -1;
				iNumFree++;
				iNumStored--;
				return lc1;
			}
		}
		//otherwise return -1
		return -1;
	}
	void Clear()
	{
		for (int lc1 = 0; lc1 < 10; lc1++)
		{
				iUnitIDs[lc1] = -1;
		}
		iNumFree = 10;
		iNumStored = 0;
	}
	int Get_UnitID_byOrder(int iOrder)
	{
		int iOrderCount = 0;
		for (int lc1 = 0; lc1 < 10; lc1++)
		{
			if (iUnitIDs[lc1] >= 0)
			{
				if (iOrder == iOrderCount)
					return iUnitIDs[lc1];
				iOrderCount++;
			}
		}
		return -1;
	}
	int Get_UnitOrder_byID(int iID)
	{
		if (iID < 0)
			return -1;
		int iOrderCount = 0;
		for (int lc1 = 0; lc1 < 10; lc1++)
		{
			if (iUnitIDs[lc1] >= 0)
			{
				if (iUnitIDs[lc1] == iID)
					return iOrderCount;
				iOrderCount++;
			}
		}
		return -1;			
	}

private:



};


class cUnitA     //I have cUnitA and cGameWorldA because I can't decide if its appropriate to just have one type of unit and one gameworld class....hence the A (01.17.2002)
{
	public:

		int AI_Misc_Use_Counter;
/*		int  iXPos_ABS;  //absolute map_coord position (all of the other positions are calculated from this)
		int  iYPos_ABS;  //absolute map_coord position (all of the other positions are calculated from this)

		int	 iXPos_offset;  //offset position from the home position of a square(this might not be used)
		int	 iYPos_offset;  //offset position from the home position of a square(this might not be used)
		
		int  iXPos_square;  //square position of the unit on the map
		int  iYPos_square;  
		int	 iIndexPos_curr;

		int	 iXPos_target;  //square positions of a unit's target
		int  iYPos_target;
		int  iIndexPos_target;

		int  iXPos_imtarget; //square positions of a unit's IMtarget(it's next immediate square to travel)
		int  iYPos_imtarget; 
		int	 iIndexPos_imtarget;  */   //changed to use coordset structures 01.19.2002

		//unit status
		BOOL bNoPathPossible;  //unit flags this if it has performed a full path and simply can't reach the target

		int iPersonality; //for characterization purposes

		int	 iLoadTarget_ID;	//ID of the loading target is stored here
		BOOL bLoaded;

		SQCOORDSET sqLoadMaterialLocation;
		SQCOORDSET sqUnloadMaterialLocation;
		
		cStorageModule Storage;
		int UnloadBuffer[10];
		int iStorageVolume;

		//data storage for UMODE_L6_DISTRACTED mode
		int L6_LastFlagged;
		SQCOORDSET L6_sqOriginalTarget;

		cComponentA* pFrame;  //gets a pointer to a the Unit's frame in the CmpAttached array
		cCmpInfo* pFrameType; //quick access (01.06.2004)
		cComponentA* pPrimaryWeapon;  //gets a pointer to a unit's primary weapon
		cComponentA* pCurrentWeapon;  //weapon that the unit is currently firing with

		UNITMULTIFIREINFO	MultiFireInfo;   //information for keeping track of firing with multiple weapons ... hopefully to supersede pPrimaryWeapon and pCurrentWeapon?
		UNITOVERLOADINFO	OverloadInfo;

		cComponentA* pBuilder;  //unit's primary builder component(10.12.2003)
		cComponentListA	CmpAttached;  //list of components attached to this unit

		//unit properties(determined from attached components)
		int iSize;
		int iMass;
		double fHP; 
		double fHPMax; //calculated ... somehow
		double fCurrSpeed; 
		double fMaxSpeed; //calculated
		double fAccel; //calculated acceleration capability, pixels/tick^2    //01.16.2004
		double fDecel; //calculated deceleration capability, pixels/tick^2
		double fCompletion;	//degree of unit completion (03.28.2003)
		double fArmor;   
		UNITCMPDRAWINFO*	VisibleComponents; //allocated array of draw information
		int				iNumDrawVisibleComponents;	//to keep track of the size of the array(03.26.2003)
									//NOTE: a unit might have more visible components than
									//what are drawn, because the unit frame does not have
									//enough visible hard points to put them

		int	iTimeLastL1Change;	//the time, in ticks, of the last L1 mode change(to facilitate in animating the unit (07.26.2004) )

		LONGLONG		iLastUpdateWeaponTargets; //10.03.2004

		double ddMainRange;

		COORDSET	Pos_ABS;
		COORDSET	Pos_offset;
		COORDSET    Heading;

		SQCOORDSET  Pos_sqcurr;
		SQCOORDSET	Pos_sqtarget;
			double ddAllowableBackOffDistance; //allows the pather to come up with paths that don't actually hit the target
		SQCOORDSET  Pos_sqimtarget;

		SQCOORDSET	Pos_sqfail_start;	//start coords of the last failed pathfind (05.15.2002)
		SQCOORDSET	Pos_sqfail_target;   //end coords of the lsat failed pathfind

		//int		iDir_Current;
		//int	    iDir_Target;   //changed 01.07.2004
		double		fDir_Current;  //direction, in degrees (0/360 is up)
		double		fDir_Target;

		int  iID;        
		LPSTR Name;
		char* ShortName;
		int  iType;      //not obsolete! (01.08.2004)
		int  iDrawFrame; //OBSOLETE 03.01.2003

		int	 iControl;	 //the "nation"/player which controls this unit (05.15.2002)
		double ddSupportCost; //support cost of unit

		BOOL  bBroadsideFlag; //indicates whether this unit should attempt to fire with a broadside(if unit is a naval frame and all of it's weapons are FREE)
		BOOL  bExternalOrderCompleted; //indication that an externally issued order has been carried out
		WORD  wExternalOrderIssued;    //name of the external order that was issued

		BOOL  bPathDeferredFlag;  //indicator that the pathfinder deferred processing 
								  //the unit's last path(it did not genuinely find no viable paths),
								  //and so the unit should not consider this path "failed" 02.02.2003
//		BYTE  bMode_L1;	//level1(bottom level mode for the unit)
		BYTE  bModes_L1[MODES_SIMUL_L1];	//modification to allow a unit to execute multiple L1 modes in one cycle (01.16.2004)
		BYTE  bMode_L2; //level2(unit mode which\ determines level1 mode)
		BYTE  bMode_L3; //level3(unit mode which determines level2 mode)
		BYTE  bMode_L4; //level4(unit mode which determines level3 mode)
		BYTE  bMode_L5; //level5(unit mode which determines level4 mode) //01.02.2003
		BYTE  bMode_L6; //level6 (unit mode which determines level5 mode) //07.18.2007
		
		VOID* pPrev_UL;      //address of previous unit node connected to it (on the UnitList)
		VOID* pNext_UL;	     //address of next unit node connected to it (on the UnitList)

		VOID* pPrev_Stack;   //address of another node that is in the same unit access stack 
		VOID* pNext_Stack;   //address of another node that is in the same unit access stack

		VOID* pCAccess;   //access to the other classes 

		VOID* pUnitPath;	//pointer to unitpath

		//(05.30.2003)
		int		iiNumSupportedCommands;
		int*  piSupportedCommands;	//array of commands supported by this unit (05.30.2003)
	
		UNITATTACKINFO*	pAttackInfo;	//pointer to a structure containing attack information when the unit is in attack mode(06.07.2002)
		UNITGUARDINFO*	pGuardInfo;		//pointer to a structure containing guard information when the unit is in guard mode
		UNITBUILDINFO*  pBuildInfo;		//pointer to a structure containing build information when the unit is in build mode (10.11.2003)

		//timer nextfire is obsolete(03.21.2003)
		//LONGLONG Timer_NextFire;	//the next time before the unit is able to fire(06.26.2002) (goes on game ticks instead of timer counts (12.23.2002)
		LONGLONG Timer_NextScan;	//the next time before the unit is able to scan(01.02.2003)
		int		 Timer_TicksJammed; //stores the number of ticks the unit has spent trying to execute a failed path search
		int		iTick_LastRepath;

		//used for AI purposes(indicate whether this unit has been marked by an enemy unit for a "destroy unit" task
		int	arrDestroyEnemyMarked[TEMP_DP_MAXPLAYERS];	
		void*	pOR;  //the object report corresponding to this unit

		void Init(VOID* pCA);
		void Set_MultiFirePattern(int iPattern); //01.08.2004
		void Update_Position();
		void Update_Targets(cUnitA* pTarget); //updates target data based on the target pointer
		void Update_LocalWeaponTargets(double PosABStargetx, double PosABStargety, UNITMULTIFIREINFO* pMFI);  //does so for the individual weapons trained on the target, for the time being, separate from update_targets(01.08.2004)
		void Update_Heading_Bearing();
		void Update_Heading_Bearing_B(double ABSTargetX, double ABSTargetY); //alternate method for calculating heading and bearing which adjusts in response to a unit's speed capabilities(more suited for Real-Time stuff) 06.13.2002
		//special version for air units :-(
		void Update_Heading_Bearing_C(double ABSTargetX, double ABSTargetY); //alternate method for calculating heading and bearing which adjusts in response to a unit's speed capabilities(more suited for Real-Time stuff) 06.13.2002
		void* Repath(void* pOldPath, int searchdepth);
		int Calc_DrawFrame(double Angle, int iNumDirs);
		int Run_Cycle();  //does what the unit does for in each running loop cycle
		void Init_LogFile();

		COORDSET Calc_Position_CmpOffset(cComponentA* pComponent, cCmpInfo* pFrameInfo);
		double Calc_Distance(float xstart, float ystart, float xend, float yend);
		double Calc_Direction(float fXHeading, float fYHeading); //function essentially copied from its equivalent in the cProjectileA class(06.22.2002)
		~cUnitA();

		void Recalc_Attributes();  //recalculates the capabilities of a unit based on the components attached ot it
		void Update_SupportedCommands(); //recalculates the list of commands that the unit is capable of.

		int Guard_Scan(BYTE bScanMode, WORD wScanRange);  //scans for, well, right now, just hostiles, and sets unit's attack information when found

		HANDLE	hLogFile;
		void Write_Log(LPSTR Text);
		void WriteLog_Enter();


	private:
		double clockwise, counterclockwise; //counters storing how long it will take to turn toward the target going in either direction

};

class cUnitListA //class which will hold the linked list of Unit instances
{
	public:
		cUnitA*		pUL_First; //list anchor
		cUnitA*     pUL_Last;  //list anchor

		HANDLE		hLogFile;

		VOID* pCAccess;

		int			ID_Last_Assigned;

		void Init(VOID* pCA);
		void Exit();  
		~cUnitListA();  //functions moved to Exit() 03.28.2003

		//	create_unit modified(iType obsolescent) 03.04.2003
		// iType now specifies predefined bill of components (03.21.2003)
		cUnitA* Create_Unit(LPSTR Name, int iType, int iControl, int iXPos_square, int iYPos_square);   //doesn't use the coordset because it would be cumbersome(01.19.2002)
		void Destroy_Unit_byID(int ID);    //might use to search through the list to destroy a unit by its ID, probably won't use the unit list to call this procedure
		void Destroy_Unit_byaddr(cUnitA* pUA); //logically goes here, as something you could use, but could be called from any class since the cUnitA can delete itself itself
		void Destroy_AllUnits();

		void Init_LogFile();
		void Write_Log(LPSTR Text);
		void WriteLog_Enter();

		void Process_Units();   //the best name I can think of at the moment to describe allowing their units to do their things during a process cycle
		

	private:
		cUnitA*		pUL_Trav;	//list traverser
		cUnitA*		pUL_Trav2;	//list traverser

		DWORD numbyteswritten;


};

class cUnitTableA //class which will simply hold addresses of units based on their IDs....an alternate and expedient way to access the units
{

	public:
		cUnitA**     pUnitTable;    //will allocate on init a block of memory that will be used as an array of cUnitA addresses
									//wow, I never thought I would ever have to use a double-pointer...(01.17.2002)

		void Init();
		~cUnitTableA();
		void Add_Unit(cUnitA* pUA, int iID);
		void Remove_Unit(int iID);
		void Remove_All();

	private:

};

//discontinued in favor of a "build units out of components" model (02.11.2003)
//renewed - unit types are not sort of "bills of components" now (03.21.2003)
class UNITTYPEINFO //changed to class 08.09.2003
{
public:
	char			   Name[TEMP_STD_STRLEN];	//08.09.2003 
	int				   iNameLength;
	char			   ShortName[TEMP_STD_STRLEN]; //12.27.2004
	int				   iShortNameLength;
	cComponentListA	   CmpAttached;
	double			   ddCost;		//05.29.2004
	cMaterialCollection MaterialCost; //10.21.2006    //materials required to build this unit
	int					iInstances[TEMP_DP_MAXPLAYERS];
	cPermissionBlock	Permissions_Required_Build;  //permissions required to build this unit

	cAcknowledgement	AckTable[10]; //01.19.2005
	int					iNumAcks;

	void Set_Name(char* pName, int _iNameLength); //08.09.2003
	void Set_ShortName(char* pName, int _iNameLength);
	void Update_Cost(VOID* pCA);
	void Update_Permissions(VOID* pCAccess);
};

class cUnitTT  //"unit Type Table" (06.16.2002) 
{
public:
		//may be extended to list/table hybrid functionality in the future(03.21.2003)
		UNITTYPEINFO	UnitTypes[UTYPE_NUMTYPES];

		int iHighestTypeID;
		VOID* pCAccess;
		
		void Init(VOID* pCA);

private:
};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
class cProjectileA //(06.07.2002) 
{
	public:
		VOID*		pNext;
		VOID*		pPrev;
		
		COORDSET	Pos_ABS;
		double		fPos_Z; //01.16.2004
		SQCOORDSET	Pos_square;
		COORDSET	Pos_offset;
		COORDSET	Heading;

		int iDrawFrame;
		int iType;
		float fDirection;
		int iID;

		float fRemrange;	//remaining range this projectile hsa left to travel
		int   iDamage;		//amount of damage to HP this projectile inflicts (not sure how damage scheme will work out yet - 06.14.2002)
							//for now, to save writing an entire new(and only minimally useful) projectile information table,
							//have the firing unit pass the draw and damage information to the projectile on creation(03.21.2003)
		float	fSpeed;	    
		float   fAccel;
		float	fMaxSpeed;  //use if the unit accelerates to a maximum speed. leave -1 otherwise
		

		int iControl;	//the nation that fired this projectile


		COORDSET	Pos_target; //position  of target (not sure how to use this yet)
		float		fTrack_cap; //"tracking capabilities" - ability of a projectile to track its target (not sure how to use this yet)		

		VOID*		pCAccess;

		int Run_Cycle();	//return type changed so that projectile can return whether to destroy itself or not (06.21.2002)
		//void Calc_DrawFrame();
		void Init(VOID* pCA);
		void Update_Position();
		float Calc_Direction(float fXHeading, float fYHeading);	//calculates direction number from heading coords.... 06.16.2002 
																//passes and returns values so that it can have future uses
		double Calc_Distance(float xstart, float ystart, float xend, float yend);
		int Calc_DrawFrame(double Angle, int iNumDirs); //07.22.2004 copied from cUnitA
		void RegisterHitOnUnit(cUnitA* pUnit, double ddDamage, double contactpoint_abs_x, double contactpoint_abs_y);



	private:

};

class cProjListA //(06.07.2002)
{
	public:
		cProjectileA*  pPRL_First;
		cProjectileA*  pPRL_Last;

		int iID_Last_Assigned;
		VOID*	pCAccess;

		void Init(VOID* pCA);
		void Create_Projectile(int iType, int iControl, float fXPos_ABS, float fYPos_ABS, float fXPos_Target, float fYPos_Target, int iID_target); //iDirection used to calculate heading and bearing	
		void Destroy_Projectile_byaddr(cProjectileA* pPA);
		void Process_Projectiles(); //corresponding sister projectusre to cUnitListA::Process_Units (06.13.2002)
		~cProjListA();
		
		HANDLE hLogFile;
		void Init_LogFile();
		void Exit_LogFile();
		void Write_Log(LPSTR Text);
		void WriteLog_Enter();

	private:
		cProjectileA*	pPRL_Trav;
		cProjectileA*	pPRL_Trav2;
};

class PROJTYPEINFO //(06.14.2002)	//default projectile characteristics for a type of projectile
{
public:
	float	fRemrange;
	int		iDamage;
	float	fSpeed;
	float	fAccel;
	float	fMaxSpeed;
//	int		iImageSurfaceIndex;		//image surface index holding graphics for this unit //hopefully obsolete 07.20.2004

	int		iSoundSeg_Fire;			//the sound to make when it is fired (when it is "born")  (negative number fo invalid index)
	int		iSoundSeg_Detonate;		//the sound to make when it detonates (when it "dies violently")

	int		iAnimType_Trail;		//perhaps temporary
	int		iAnimType_Detonate;		//the animation that might play when the projectile reaches it's target

	int		iRotateFrames;		    //07.22.2004

	BOOL	bLineOfFire;  //indicates whether this thing detonates on things in the line of fire, or only on the target itself

	COORDSET	Pos_secondarydamage[50];
	double		ddDamageRatios[50];
	int			iNumSecondarySquares;

	PROJTYPEINFO () 
	{
		Clear_Secondaries();
		bLineOfFire = TRUE;
	}
	void Clear_Secondaries()
	{
		for (int lc1 = 0; lc1 < 50; lc1++)
		{
			Pos_secondarydamage[lc1].x = -1;
			Pos_secondarydamage[lc1].y = -1;
			Pos_secondarydamage[lc1].z = -1;
			ddDamageRatios[lc1] = 0;
		}
		iNumSecondarySquares = 0;
	}
	//
	void Set_1TileSplash(double ddTier1, double ddTier2);

	cSrcGraphicsInfo	SourceGraphics;  //07.15.2004
private:
};

class cProjTT //"projectile Type Table" (06.16.2002)
{
public:

	PROJTYPEINFO ProjTypes[PTYPE_NUMTYPES];
	
	VOID*	pCAccess;
	
	void Init(VOID* pCA);

private:


};

//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
class cStructInfo
{

public:
	LPSTR GeneralName;			//needed?  (I think, yes. 12.28.2004)


	int		iHP;
	int		iType;
	double	ddResourceCost;
	cMaterialCollection MaterialCost; //how much this structure costs

	double ddMaterialStorageCapacity;

	cPermissionBlock	Permissions;
	int			iOccMaptiles;		//the number of maptiles that the structure occupies
	COORDSET*	Pos_offset_array; //array of position offsets from the central position
								  //that this structure will occupy


	int			iAttachedComponents;	//the number of components that can be
										//attached to this structure
	COORDSET*	AC_Offsets;			//array of x/y offsets specifying the offset positions
										//for the attached components
										//note that components on structures 

	int			iImageSurfIndex;
	int			iXFramesize;
	int			iYFramesize;
	int			iXDrawOffset;
	int			iYDrawOffset;

	char ProcessText[80];

//replace this with an array setup (because structure information is not dynamic)
	int					iNumCommCaps;
	int*				CommCaps_Array;	//try going with just an int instead of a COMMISSUEINFOs and see what happens(05.20.2003)
	
	BOOL				bActiveOrderQueue;	//sort of a HACK to indicate whether to enable order queue functionality for this structure
		
//	COMMISSUEINFO*		pCommCaps_First;	//buffer to put list of supported commands for this structure type
//	COMMISSUEINFO*		pCommCaps_Last;		//(sort of redundant with the information in the structure itself)

	void*		pPrev;
	void*		pNext;

private:



};

class cStructInfoTable
{

public:
	cStructInfo**	pTable;

	cStructInfo*	pStructs_First;
	cStructInfo*	pStructs_Last;

	VOID*			pCAccess;

	int				iHighestTypeID;

	//these are just for reference to make it easy to input new structure types into the table
	//what are these for? commented out 08.07.2003 to see what happens
//	PCMD2*			pCQ2Reference[200]; //enough to accomodate all of the CQ2TYPE IDs
//	PCMD*			pCQReference[200];	//enough to accomodate all of the CQTYPE IDs

	void Init(VOID* pCA);
	void Exit();
	void Add_StructType_Simple(LPSTR GeneralName,
							   int iType, 
							   double ddResourceCost,
							   int iImageSurfIndex,
							   int iXFramesize,
							   int iYFramesize,
							   int iXDrawOffset,
							   int iYDrawOffset,
							   int iHP,
							   double ddMaterialStorageCapacity,
							   int iOccMaptiles,
							   int iXOcc_offset1,
							   int iYOcc_offset1,
							   int iXOcc_offset2,
							   int iYOcc_offset2,							   
							   int iXOcc_offset3,
							   int iYOcc_offset3,
							   int iXOcc_offset4,
							   int iYOcc_offset4,
							   int iXOcc_offset5,
							   int iYOcc_offset5,
							   int iXOcc_offset6,
							   int iYOcc_offset6,
							   int iXOcc_offset7,
							   int iYOcc_offset7,
							   int iXOcc_offset8,
							   int iYOcc_offset8,
							   int iXOcc_offset9,
							   int iYOcc_offset9,
							   int iAttachedComponents,
							   int iXAC_offset1,
							   int iYAC_offset1,
							   int iXAC_offset2,
							   int iYAC_offset2,
							   int iXAC_offset3,
							   int iYAC_offset3,
							   int iXAC_offset4,
							   int iYAC_offset4,
							   int iNumCommCaps,
							   int iComm2_1,
							   int iComm2_2,
							   int iComm2_3,
							   int iComm2_4,
							   int iComm2_5,
							   int iComm2_6,
							   int iComm2_7,
							   int iComm2_8,
							   int iComm2_9,
							   int iComm2_10,
							   int iPermission1,
							   int iPermission2,
							   int iPermission3,
							   int iPermission4,
							   int iPermission5,
							   int iPermission6,
							   int iPermission7,
							   int iPermission8,
							   int iPermission9,
							   char* pProcessText,
							   BOOL bActiveOrderQueue,
							   BOOL bReconstructFlag);
			void Reconstruct();

private:


};
class cExtractionInfo
{
public:
	int iExtractionMaterialType;

private:

};

//03.27.2003
class cStructureA
{

public:

	BOOL		bOn; //indicates whether this structure is operating

	SQCOORDSET	Pos_primary;  //the primary or, I guess, "base" position occupied by the structure
	SQCOORDSET*	Pos_array;	//will contain an array of positions other than the primary position that this
							//structure occupies
	int			iNumArrayEntries;
	
	SQCOORDSET_L*	Pos_list_first;	//sort of redundant, anchor for a linked list of coordinates 
	SQCOORDSET_L*	Pos_list_last;	//that this structure occupies(an addition to the
									//ones in the Pos_array)...just in case this is needed(03.27.2003)

	cExtractionInfo ExtractionInfo;

	int iLastMaterialDeposited;
	int iLastMaterialWithdrawn;
	int iLastDumped;

	int iLastAward[NUM_MATERIAL_TYPES];

	char ProcessText[80];  //ingame "pie" name of whatever the structure does

	int			iBuildOrderNumber; //10.12.2003
	int			iType;
	LPSTR		Name;
	int			iID;
	int			iSector;	//the sector this structure is located in
	int			iControl;

	int			iHPMax;
	int			iHP;
	float		fCompletion;	//determines how complete this structure is in its construction...

	int			bMode_L1;		//currently anticipated that a structure will not use more than 1 mode level

	LPSBE		pUSB_First;		//unit selection anchor
	LPSBE		pUSB_Last;		//unit selection anchor

	cProgOrderBuffer	OrderQueue;	//stores build/repair orders that this structure has  (08.07.2003)
									//note that ALL structure possess an order queue but only structures supporting BUILD/REPAIR commands (RSCs and attachments) will actually be able to use it

	cComponentListA* CmpInventory;	//probably used by only RSCs and CSCs  (used for holding earmarked components 08.11.2003)

	cStorageModule Storage;        //can be used for storing 

	double* pResourceCollection;
	int iCollectionModifiedLast;

	cComponentA**	CmpAttached;	//components directly used by this structure
									//arrayed as opposed to listed.
									//components placed and operated according to 
									//their positions on the iAC_Offsets data
	int			iComponentSlots;	//the number of components that are in the cmpattached array
								

//	COMMISSUEINFO*	pCommCaps_First;	//anchor for list to hold list of commands this structure is capable of issuing
//	COMMISSUEINFO*	pCommCaps_Last;	
	//for now, simply storing it as an array(05.20.2003)
	int			iNumSupportedCommands;   //05.20.2003
	int*		pSupportedCommands;
	//during a process cycle, the interface will query the list of supported commands
	//of the selected structure and set gridcell command pointers to the commands in the
	//commissueinfo structures(03.28.2003)

	VOID*		pCAccess;

	void*		pOR;   //link to structure's object report

	void*		pPrev;
	void*		pNext;

	int	Run_Cycle();	//will a structure need this? (03.28.2003)
	void Process_Reactor();
	void Process_Refinery();
	void Process_Forge();
	void Process_Extractor();
	void Process_ResourceProcessing_Simple(int iTakeMaterial1, 
													int iTakeMaterial1Amt,
													int iTakeMaterial2, 
													int iTakeMaterial2Amt,
													double ddTakeEnergy, 
													int iProduceMaterial1,
													int iProduceMaterial1Amt,
													int iProduceMaterial2,
													int iProduceMaterial2Amt,
													double ddProduceEnergy,
													int iProcessTime);

	cStructureA* GetStructureFromID(int iID);

	void Disperse_Materials();  //disperse the units resource collection array

	void Init_Extractor();

	void Init(VOID* pCA);
	void Exit();

	

private:

};

//the Struct List and the Struct Table are master lists containing all structures currently in the game
class cStructListA
{

public:

	cStructureA*	pSL_First;
	cStructureA*	pSL_Last;

	int		iID_Last_Assigned;
	int		iBuildOrder_Last_Assigned;
	
	VOID*		pCAccess;
	
	HANDLE		hLogFile;			
			
	void Write_Log(LPSTR Text);
	void WriteLog_Enter();
	void Init_LogFile();
	void Exit_LogFile();

	void Init(VOID* pCA);
	void Exit();
	
	void Process_Structures();

	cStructureA* Create_Structure(LPSTR Name, int iSector, int iType, int iControl, int iXPos_square, int iYPos_square);
	void Destroy_Structure_byaddr(cStructureA* pStruct);
	void Destroy_Structure_byID(int iID);
	void Destroy_AllStructures();



private:




};

class cStructIDTable
{

public:

	cStructureA**	pTable;

	VOID*	pCAccess;

	void Init(VOID* pCA);
	void Exit();

	void Register_Struct(cStructureA* pStruct, int iID);
	void Unregister_Struct(int iID);
	void Unregister_All();

private:

};

//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//to classify different types of raw materials (10.19.2006)
class cMaterialType
{
public:
	char Name[40];
	char ShortName[10];
	char RefChar;
	double ddDensity;  //quantity/unit volume
	int iOverloadProjectileType; //the projectile type that this material overloads into should the user try to "arm" it.
	double ddSpeedMultiplier;
	double ddArmorMultiplier;
	double ddArmorBonus;
private:
};

class cMaterialInfoTable
{
public:
	cMaterialType Types[NUM_MATERIAL_TYPES];
	
	void Init(VOID* pCA);
	int FindType_ByRefChar(char InRefChar);
	VOID* pCAccess;

private:


};

//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//this table is to help reduce the sheer amount of strings that are thrown around(and possibly corrupted), if there are multiple structures that need
//using a single string(for example--> "build unit"), then in the future they could simply be specified with a single index to the table
//(03.10.2004)
class cStringTable
{

public:

	VOID*		pCAccess;

	char*		pTable[STRING_NUMIDS];	
	
	int			iRandomStringCounter[10];		//random string counter
	int			iOverStringCounter[10];
												//
	char*		Register_String(LPSTR pString, int iIndex, int iStrLen);	//for now, this function ALWAYS copies the incoming string
	void		Unregister_String(int iIndex);
	
	char*		Supply_Random_String(int iNameType);  //just a "fun" function for now, to supply random string names



	void		Init(VOID* pCA);
	void		Exit();

};

//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//indicates that a link exists from one sector to another
class cSectorLinkEntry
{
public:
	int iSectorLinkedTo;

	cSectorLinkEntry* pPrev;
	cSectorLinkEntry* pNext;

};

//06.28.2008 Updates with information about what is in the sector
//
class cSectorTacticalInfo
{
public:
	int iLastUpdatedTick;
	int arrNumUnits[TEMP_DP_MAXPLAYERS];
	int iTotalUnits;
	int arrNumStructs[TEMP_DP_MAXPLAYERS];
	int iTotalStructs;
	int arrNumCapitals[TEMP_DP_MAXPLAYERS];
	int iTotalCapitals;


};

//a sector is a region on the map. It is a game structure that players contend over, 
//for power and ultimately for victory. Each tile on the map holds a sector value
//(05.25.2004)
class cSector
{

public:
	int				iID;	//sector ID. corresponds to it's index in the sector table
	char			Name[TEMP_STD_STRLEN];	//holds the name as a static array(for the moment)
	int				iResourceBonus;
	int				iControl;
	cStructureA*	pCapital;		//pointer to the capital of this sector(NULL if no capital)

	int				iLastParticipatedJobNumber;

	cSectorTacticalInfo	TacticalInfo;
	void Init_TacticalInfo();
		

	MTILE* pRepresentativeTile;
	MTILE* pMTList_First; //for holding a linked list of maptiles that are of this same sector
	MTILE* pMTList_Last; 
	void Init_MTList();
	void Exit_MTList();
	void Add_MTile(MTILE* pTile);
	int iNumMapTiles;

	cSector* pPrev;
	cSector* pNext;

	cSectorLinkEntry* pSectorLink_First;
	cSectorLinkEntry* pSectorLink_Last;

	void* pCAccess;

	int iNumLinks;

	void Init_LinkQueue()
	{
		iNumLinks = 0;

		pSectorLink_First = new cSectorLinkEntry;
		pSectorLink_Last = new cSectorLinkEntry;
		pSectorLink_First->iSectorLinkedTo = VAL_INVALID;
		pSectorLink_Last->iSectorLinkedTo = VAL_INVALID;

		pSectorLink_First->pPrev = NULL;
		pSectorLink_First->pNext = pSectorLink_Last;
		pSectorLink_Last->pPrev = pSectorLink_First;
		pSectorLink_Last->pNext = NULL;
	}
	void Exit_LinkQueue()
	{
		cSectorLinkEntry* pTrav = pSectorLink_First;

		while (pTrav != NULL)
		{
			cSectorLinkEntry* pTrav2 = pTrav->pNext;
			delete pTrav;
			pTrav = pTrav2;
		}
		pSectorLink_First = NULL;
		pSectorLink_Last = NULL;

		iNumLinks = 0;
	}
	void Add_Link(int iSectorLinkedTo)
	{
		if (IsLinkAlreadyHere(iSectorLinkedTo) == TRUE)
			return;

		iNumLinks++;

		cSectorLinkEntry* pNew = new cSectorLinkEntry;
		pNew->iSectorLinkedTo = iSectorLinkedTo;
		cSectorLinkEntry* pNext = pSectorLink_Last;
		cSectorLinkEntry* pPrev = pSectorLink_Last->pPrev;

		pNew->pNext = pNext;
		pNew->pPrev = pPrev;
		pPrev->pNext = pNew;
		pNext->pPrev = pNew;
	}
	BOOL IsLinkAlreadyHere(int iSectorLinkedTo)
	{
		cSectorLinkEntry* pTrav = pSectorLink_First->pNext;

		while (pTrav != pSectorLink_Last)
		{
			if (pTrav->iSectorLinkedTo == iSectorLinkedTo)
				return TRUE;
			pTrav = pTrav->pNext;
		}
		return FALSE;
	}


private:




};

//this class holds information for all the sectors currently in the game, and is in charge of their
//management(another appropriate name for this class would have been cSectorManager) (05.28.2004) <-- my 21st birthday
class cSectorTable
{
public:
	cSector*	Sectors[MAX_SECTORS];
	
	void*		pCAccess;

	void		Init(void* pCA);

	void		Register_Sector(int iID, char* Name, int iResourceBonus);
	void		Unregister_Sector(int iID);

	void		Process_Sectors();	//05.29.2004

	void Hook_SectorCreates();

	void		Exit();

	HANDLE		hLogFile;

		void Init_LogFile();
		void Write_Log(LPSTR Text);
		void Exit_LogFile();
		void WriteLog_Enter();

private:


};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
class cPATHENTRY     //an part of the final constructed path that is released by the pathfinder
{
	public:
		SQCOORDSET	 Position;
		
		VOID*	pPrev;
		VOID*   pNext;
	private:

};

class cPLNODE   //"pathlist node"    nodes which the pathfinder uses to generate paths (i could have probably inherited it from cPATHENTRY ... but I don't know how :-)) 01.21.2002
{
	public:
		
		SQCOORDSET	Pos_node;
		SQCOORDSET  Pos_goal;
		float	fDist_from_home;
		float	fEvalSum; //the evaluated value of this node that determines how "good" it is toward the goal
		int		iObjectType;  //NOT USED YET but will hold the type of unit that is plotting the path so that the pather creates a viable path (01.31.2002)
		
		VOID*	pPrev;
		VOID*	pNext;
		VOID*	pParent;
		//VOID*   darn! problem with the evalsums....^&*^*&&^*^ 02.01.2002  (problem fixed - 02.02.2002)

	private:
};

class cSimpleNode   //bare minimum set of pointers needed to execute a search through connected tanks and pipes
{
public:

	int iMapIndex;

	cSimpleNode* pNext;
	cSimpleNode* pPrev;
};

class cPathFinder //class which can generates viable paths in the form of linked lists of "path entries" to guide units along their paths
{

	public:	
		VOID*			pCAccess;
		MTILE*			pMap;

		cSimpleNode*	pS_First;
		cSimpleNode*	pS_Last;
		int iJobNumber;
		void Clear_SimpleList();
		void Add_SimpleNode(cSimpleNode* pNode);
		cSimpleNode* Add_SimpleNode(int iMapIndex);
		void Remove_SimpleNode(cSimpleNode* pNode);
		int Find_ConnectedTank(int iMapIndex, int iMaterialType, double ddMinValue, int iMode);
		double arePointsConnected(int iMapIndex1, int iMapIndex2);
		cStorageModule* Find_ConnectedTank(int iMapIndex, int iMaterialType, double ddMinValue, int iMode, int iDummy); //overloaded
		int EvalTankSquare_TakeFrom(int iMapIndex, int iMaterialType, int iCurrJobNumber, double ddMinValue);
		int EvalTankSquare_PutInto(int iMapIndex, int iMaterialType, int iCurrJobNumber, double ddMinValue);

		double Calc_Distance(float xstart, float ystart, float xend, float yend);

		cPLNODE*		pOL_First;
		cPLNODE*		pOL_Last;
		MTILE*			pMOL_First;
		MTILE*			pMOL_Last;

		cPLNODE*		pCL_First;
		cPLNODE*		pCL_Last;
		MTILE*			pMCL_First;
		MTILE*			pMCL_Last;

		cPLNODE*		pTL_First;
		cPLNODE*		pTL_Last;
		MTILE*			pMTL_First;
		MTILE*			pMTL_Last;

		//used in the sector pathing scheme
		//there is no heuristic, it essentially just "blots" the map until it finds the target
		cSector*		pSecSchemeOL_First;
		cSector*		pSecSchemeOL_Last;
		cSector*		pSecSchemeCL_First;
		cSector*		pSecSchemeCL_Last;
		//sector scheme functions
		void SCS_Clear_Lists_All();
		void SCS_Spawn_New_OpenNodes(int iSector, int iUnitID);
		void SCS_Detach_Reattach_to_Closed_List(int iSector);
		void SCS_Insert_OpenNode(int iSector);
		BOOL CanUnitTraverse(int iUnitID, int iStartIndex, int iEndIndex);


		int				iID_Unit;  //ID of the last unit which requested a path (06.05.2002)
		cUnitA*			pUnit;		//direct pointer to the unit which made the request
									//makes it faster(03.21.2003)
		float			PassCharacteristics[NUM_MAPTILE_TYPES]; //makes it even faster(03.21.2003)
			
		LONGLONG		llCurrRunningTick;
		//int				PathsExecuted_Tick;
		//int				MaxPathsExecute_Tick;
		LONGLONG		MaxIterations_Tick;
		LONGLONG		IterationsExecuted_Tick;

		BOOL			bConsiderUnits;

		float			A_Star_Heuristic;
		
		HANDLE			hLogFile;


		void Init(VOID* pCA, float ASHeuristic);
		void PostInit(); //to initialize AFTER the map has been initialized
		void Exit();
		~cPathFinder();

		cPATHENTRY* Find_Path(int IDUnit, int iIndex_Start, int iIndex_Goal, int iSearchDepth, BOOL* pbDeferFlag_Ret, BOOL bUnitConsiderFlag, double ddAllowableBackOffDistance);
																											  //DeferFlag added 02.02.2003
		cPATHENTRY* Find_Path2(int IDUnit, int iIndex_Start, int iIndex_Goal, int iSearchDepth, BOOL* pbDeferFlag_Ret, BOOL bUnitConsiderFlag, BOOL bExhaustFlag, double ddAllowableBackOffDistance);
		cPATHENTRY* Find_Path3(int IDUnit, int iIndex_Start, int iIndex_Goal, int iSearchDepth, BOOL* pbDeferFlag_Ret, BOOL bUnitConsiderFlag, BOOL bExhaustFlag, double ddAllowableBackOffDistance, BOOL bReturn_Path);
			int iFP2_Ret;
		cPATHENTRY*	Find_Path_Link(	int iIndex_Start, int iIndex_Goal, int iSearchDepth);																									  
		
		void Clear_Lists_All();
		void Clear_Lists_All2();
		void Clear_List_Specify(cPLNODE* pPL_Start, cPLNODE* pPL_End);

		BOOL Eval_NewNodeSpawn(int iIndex_new, cPLNODE* pParent);
		BOOL Eval_NewNodeSpawn2(int iIndex_new, MTILE* pParent);
		BOOL Eval_NewNodeSpawn_Link(int iIndex_new, MTILE* pParent);

		float Calc_Evalsum(float fDist_Home, SQCOORDSET Pos_Node, SQCOORDSET Pos_Goal);

		cPLNODE**	pPathState;		//will hold an array of pathstate pointers corresponding to the map which point to the current path state of various squares on the map
		
		BOOL Insert_TempNode(cPLNODE* pParent, int iIndex_node, int iIndex_Goal);
		BOOL Insert_TempNode2(MTILE* pParent, int iIndex_node);
		BOOL Insert_TempNode_Link(MTILE* pParent, int iIndex_node);
		void Process_TempList();
		void Process_TempList2();
		void Process_TempList_Link();
		void Process_OpenNode(cPLNODE* pOpenNode);
		void Process_OpenNode2(MTILE* pOpenNode);
		void Process_OpenNode_Link(MTILE* pOpenNode);
		void Scan_List(int iMapIndex, cPLNODE* pStart, cPLNODE* pEnd);
		void Clear_PathArray();   //a manual function to clear out the path state array
	
		//for the terrain aggregation
		void Agg_Spawn_Nodes_SameTerrain(int iNode);
		void Agg_Insert_OpenNode(int iIndex_node);
		void Agg_Sector_Blot(int iIndex_node, int iSectorID);
		void Agg_Detach_Reattach_To_Closed_List(int iIndex_node);
		void Agg_Partition_Sectors();

		cPATHENTRY* Generate_Path(cPLNODE* pNode_End);
		cPATHENTRY* Generate_Path2(MTILE* pNode_End);
		void Destroy_Path(cPATHENTRY* pPathEntry);

		int Adjust_Target(int iMapIndex, int iIDUnit); //06.27.2002
		int Adjust_Target2(int iIndex_Start, int iIDUnit); //10.16.2006
		void AT_Insert_TempNode(cPLNODE* pParent, int iIndex_node);
		cPLNODE* AT_Process_OpenNode(cPLNODE* pOpenNode); //06.27.2002
		BOOL AT_EvalNewNode_Spawn(int iMapIndex, cPLNODE* pParent); //06.27.2002
		void Init_LogFile();
		void Write_Log(LPSTR Text);
		void Exit_LogFile();
		void WriteLog_Enter();

		void Path_Diag(SQCOORDSET* pSqCurr, cPATHENTRY* pPath); //diagnostic to find source of messed up paths problem(09.05.2002)

		void Alter_Parent(cPLNODE* pParent_Old, cPLNODE* pParent_New);

	private:
		cPLNODE*		pPLTrav;     //pathlist traverser
		cPLNODE*		pPLTrav2;    //second traverser, since I always seem to need two
		cPLNODE*		pOL_Trav;	 //traverser for open list
		cPLNODE*		pOL_Trav2;	 //second traverser used for insertion functions
		cPLNODE*		pTL_Trav;	 //traverser for temp list
		cPLNODE*		pTL_Trav2;	 //second traverser used for insertion functions
		cPLNODE*		pTL_Trav3;	 //third traverser used for insertion functions
		cPLNODE*		pCL_Trav;	 //traverser for closed list
		cPLNODE*		pCL_Trav2;   //traverser for closed list

		SQCOORDSET		Pos_goal;

		int UP; 
		int DOWN;
		int LEFT;
		int RIGHT;
		int UPRIGHT;
		int UPLEFT;
		int DOWNLEFT;
		int DOWNRIGHT;
		
		int				c1,c2;		 //counters

};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//

typedef struct _PLAYERDATA   //the data type of the data in the pvData member of DPN_PLAYER_INFO to be passed in peer infos (07.15.2002)
{
	int		iControl_Requested;		//control index of this player
	int		iTeam_Requested;		//"team" index of this player (NOTE: iControl_Requested and iTeam_Requested used when needed) 12.23.2002
	DPNID	iDPNID;					//the DPNID of this player

} PLAYERDATA, * PPLAYERDATA;

//PLAYERDATA NODE, manual store for player data (12.23.2002)
typedef struct _PDNODE
{
	int		iControl_Requested;
	int		iTeam_Requested;
	DPNID	iDPNID;
	BOOL	bHaveCurrentMap; //05.12.2004 (flagged if player has the current map selected by the host) (currrnetly not used)
	WCHAR*	wcsName;
	int		iNameLength;
	BOOL	bAIFlag;
	void*	pNext;
	void*	pPrev;
} PDNODE, * PPDNODE;

//final playerdata, to store FINAL(after control IDs have been decided by the host) information about the participating players(03.14.2004)
/*
typedef struct _PLAYERDATA_FINAL
{
	BOOL	bValid;
	BOOL	bAIFlag; //only thing I can think of so far

} PDFNODE, * PPDFNODE;
*/

class cGameOptions //contains information about the controller/player (06.27.2002) (under modification 12.23.2002)
{
public:
	int		iControl;	//control of the local player
	int		iTimerID;   //the timing ID of the local player (used to ensure all players are in sync)
	BOOL	bAmHost;	//flag indicating whether this is the host or not
	int		iLatency;	//the number of ticks commands are delayed from the time of issue
	
	char    strName[80]; //regular-character name

	int		iSectorTickCount; //the number of ticks that goes by between each "appraisal" of sector control to see how resources are produced(05.29.2004)

	BOOL	bMusic; //enable/disable music

	BOOL	bTutorial; //enable/disable tutorial assistance messages

//	PLAYERDATA_FINAL	PDF_Table[TEMP_DP_MAXPLAYERS];	//array of final playerdata's(03.14.2004) (redundant)

	PDNODE*	pPD_First;  //playerdata list anchors
	PDNODE* pPD_Last;	
	PDNODE* pPD_AIFirst;	//list for AI Players
	PDNODE* pPD_AILast;

	int		iMapIndex;
	//TEMP until DP routines can be rewritten again(05.15.2004)
	int		iNumPlayersWithMap;
	//path of map for multiplayer (because the specific map path may be different between players(just the uMapID must be the same)
	char    strMultiplayerMapPath[256];
	int     iMapUniqueID; //stores the map unique ID
	BOOL    bMapLocationFinalized; //this identifies that the player is ready to go

	DPNID  DPNID_Local; //DPNID of the local player
	DPNID  DPNID_Host;

	PDNODE** ScratchInfoTable;	//08.09.2003 just a scratch table to hold information just so we don't have to call the really difficult GetPeerInfo everytime we need stuff
	BOOL bMPStatusesInUse;

	void Add_Player(WCHAR* wcsName, int iNameLength, int iControl_Requested, int iTeam_Requested, DPNID iDPNID); //12.23.2002
	void Clear_PlayerList(PDNODE* pFirst, PDNODE* pLast);
	void Add_AIPlayer(int iControl_Requested, int iTeam_Requested); //12.23.2002 (no name or DPNID currently needed)
	BOOL Remove_AIPlayer_Team(int iTeam_Requested); //12.23.2002 (removes the first AI player with indicated team ID, returns FALSE if none found)
	BOOL Remove_AIPlayer_Control(int iControl_Requested); //12.23.2002 (removes the first AI player with indicated control ID, returns FALSE if none found)

	DPN_PLAYER_INFO InfoPlayer_Local; //information on local player(to set)
	int		iNumPlayers; //total players, AI and Human
	BYTE	bHighestActivePlayerID;	
	BYTE	bGameType;

	VOID* pCAccess;
	
	
	void Init(VOID* pCA, int iCtrl, int iLtcy);
	void Exit();

private:

};

//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
class cRandGen    //programmed "random" number generator so that random numbers can be synchronized on all systems   //07.15.2002
{

public:
	//RANDGENKEYDATA*		pRandKeys;
	LONG				RandKeys[394];  //simpler random system
	BOOL				bLocalGen; //were the keys self-generated? (07.18.2002)

	WORD				wIndex1;     
	WORD				wIndex2;
	WORD				wIndex3;
	WORD				wIndex4;

	VOID* pCAccess;

	void Init(LONG* pRK, VOID* pCA); //if pRK is NULL, then system will generate its own pRandKeys
	void ReloadKeys(LONG* pRK);
	void ExportKeys(LONG* pRKReceiveBuffer);    //exports the current key data to another destination (12.16.2002)
	void GenKeyData();
	
	
	unsigned long rand2();   //returns a "random" number
	void Exit();


private:



};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//

//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//

//may contain duplicates of information already inside the seed class itself.
//the reason is to keep the "new" AI implementation separate for simplicity
class cSeedTacticalInfo
{
public:
		

private:

};

//specifies a single location where the AI can try to "fan out" to get a build location
class cBuildLocationSeed
{
public:
	cBuildLocationSeed() { bFudged = FALSE; }
	SQCOORDSET sqLocation;
	BOOL bFudged; //indicates whether this seed was fudged

	cBuildLocationSeed* pNextSeed;
	cBuildLocationSeed* pPrevSeed;

	void* pSeedGroup; // void(uncasted) pointer to the parent seed group

};
//specifies a group of locations that the AI "intended" to be contiguous, that have common access to the same
//resource pool (note the word "intended": it is possible for enemies to destroy buildings to break continuity.)
//(most likely the computer's response to this will be to initially re-check the group of seeds for continuity, "orphan" 
//the ones that do not have continuity (remove them completely from the seeding process), and then after some extended period of
//time, perform a more expensive procedure to determine what is connected to what, to "recover" the lost buildings.)
class cBuildLocationSeedGroup
{
public:
	//internally
	cBuildLocationSeed* pSeed_First;
	cBuildLocationSeed* pSeed_Last;
	
	int iID;

	BOOL bContainsCapital; 
	BOOL bContainsForge;
	BOOL bContainsFactory;
	BOOL bContainsRefinery;
	BOOL bContainsShipyard;
	BOOL bContainsStorageFor[NUM_MATERIAL_TYPES];
	BOOL bContainsProductionFor[NUM_MATERIAL_TYPES];
	double fContains[NUM_MATERIAL_TYPES];
	BOOL bAdequateStorage;
	cBuildLocationSeed* pCapital; //holds pointer to capital (if it exists, otherwise NULL)

	cSeedTacticalInfo TacticalInfo;

	//it's own links for prev and next seed groups
	cBuildLocationSeedGroup* pGroup_Prev;
	cBuildLocationSeedGroup* pGroup_Next;

	void* pCAccess;

	void Delete_Seed(cBuildLocationSeed* pSeed);
	cBuildLocationSeed* Create_Add_Seed(int iMapIndex);

	void Clear_Gather_Info();

	void Init(void* pCA, cBuildLocationSeedGroup* pPrev, cBuildLocationSeedGroup* pNext);
	void Exit();

	cBuildLocationSeed* Find_Seed(int iMapIndex);
};

class cLocationsManager
{
public:

	int iControl;

	cBuildLocationSeedGroup* pGroup_First;
	cBuildLocationSeedGroup* pGroup_Last;

	void* pCAccess;

	void Delete_BuildLocationSeedGroup(cBuildLocationSeedGroup* pGroup);

	cBuildLocationSeedGroup* Create_Add_BuildLocationSeedGroup(int iMapIndex);

	cBuildLocationSeed* Find_Seed(int iMapIndex);
	cBuildLocationSeedGroup* Find_SeedGroup(int iMapIndex);

	void Init(void* pCA, int _iCtrl);
	void Exit();

	int iNumSeedGroups;

private:


};

class cAIMode
{
public:
	int iMode;
	int iParam1;
	int iParam2;
	int iParam3;
	double fEfficacy;
};

class cAIModeStack
{
public:
	cAIMode	Modes[AI_MODESTACK_MAX_MODES];
	
	void Clear();
	void Push(int iMode, double fEfficacy);

};
//
class cAITopInfo
{
public:
	double fOwnHP;
	double fOwnFirepower;
	double fOwnInfrastructure;
	double fEnemyHP[TEMP_DP_MAXPLAYERS];
	double fEnemyAverageHP;
	double fEnemyFirePower[TEMP_DP_MAXPLAYERS];
	double fEnemyAverageFirePower;
	double fEnemyInfrastructure[TEMP_DP_MAXPLAYERS];
	double fEnemyAverageInfrastructure;
    int iNumEnemies;
	double fRevealedMapFraction;

	double fAttackDisposition;
	double fDefenseDisposition;
	double fDevelopDisposition;
	double fExploreDisposition;

	int iNumCapitals;
	int iNumBuildings;
	int iNumUnits;
	int iNumSeedGroups;

	double MaterialDemand[NUM_MATERIAL_TYPES];

};
class cAISectorInfo
{
public:
	int iSize; //number of tiles
	int iType; //terrain type

};


class cAIInfo
{
public:
	cAITopInfo* pTopInfo;
    cAISectorInfo* pSectorInfo;

	void Init() 
		{
			pTopInfo = NULL;
			pSectorInfo = NULL;

		}
	void Exit()
		{
			if (pTopInfo != NULL)
				delete pTopInfo;
			if (pSectorInfo != NULL)
				delete pSectorInfo;
		    pTopInfo = NULL;
			pSectorInfo = NULL;
		}
		cAIInfo() { Init(); }
		~cAIInfo() { Exit(); }

};

//06.28.2008
//AITaskHostInfo is a structure that the task host uses to communicate with its task subordinates
//for now it is a static member in cAIEntity, but it may have to go dynamic if we run into problems with running out of memory!
class cAITaskHostInfo
{
public:
	int iTaskStep; //which step of the task we are currently at
	SQCOORDSET MapTileLocation; //whatever map location the task needs
	cStructureA* pStructure; //whatever structure pointer we need
	int iCommenceTick;  //whatever start time is meaningful

	//
	int iUnitType; //
};

//11.17.2008
//The purpose of this scheme is to even out the task of performing terrain analysis over time, instead of
//having to do it all at once when it is needed. The Location Lists continuously examine tiles in anticipation
//of their future use, as their selections change as time elapses, the AI's priorities change, etc, old selections
//are retired after a certain time. Thus when the AI must actually make a decision based on terrain, possible
//choices have already been narrowed down to a few likely choices.
//This approach is chosen based on its similarity to how a human player might think when making terrain-based decisions.
class cAILocation
{
public:
	SQCOORDSET	sqPosition;
	int	iTick_Evaluated;
	double	ddEfficacy;

	cAILocation* pPrev;
	cAILocation* pNext;

private:

};

//02.10.2008
//new approach to AI. This entity represents a single AI entity, a unit, a building, a seed, a sector.
class cAIEntity
{
public:

	//AI
	cAIModeStack Modes;
	cAIInfo Info;
	cAITaskHostInfo HostInfo; //only when an entity is "hosting" a task does this become used
	int iControl; //note that this is not the iControl of the game element this represents, but rather the iControl of this AI

    //
    cBuildLocationSeedGroup* pSeed; //pointer to the seed, if entity is seed type. a hack because a seed type isn't easy to access using just the ID

	//What it's considering doing
	cAIModeStack Modes_Consider;

	//Running procedures
	void Tasking();	//determines whether to withdraw from the current task, remain where it is, or start/join a new one
		void Tasking_Unit();
		void Tasking_Struct();
		void Tasking_Seed();
		void Tasking_Sector();
	void Execute(); //performs whatever tasks are appropriate for the state of the entity
	
	cAIModeStack Choose_Modes();
		cAIModeStack Choose_Modes_Unit();
		cAIModeStack Choose_Modes_Struct();
		cAIModeStack Choose_Modes_Seed();
		cAIModeStack Choose_Modes_Sector();
		

	VOID* pCAccess; 

	//type and ID of the entity this represents
	int iID;
	int iType;
	

	//this flag represents the entity raising its hand and saving "I have a task to accomplish"
	BOOL bTaskRaised;
	int iTaskType;   //the task type only encapsulates what the unit is doing, not what it is planning
	double fImmediacy; //how loudly the entity wants to raise the task

	cAIEntity* pSubQueue_Head;
	cAIEntity* pSubQueue_Tail;

	cAIEntity* pPrev;
	cAIEntity* pNext;

	//establish its position in the 
	//status queue.
	//it's position is determined by its priority (how "loud" it is shouting)
	cAIEntity* pStatusQueue_Prev;
	cAIEntity* pStatusQueue_Next;
	int iPriority;

	//this pointer keeps track of who is assisting it
	cAIEntity* pAsstRecv_Head;
	int iNumAsstRecv;
	//
	int arrNumInRole[TEMP_AITASK_MAX_ROLES];
	void Reset_RoleNums();

	//
	cAIEntity* pAsstGive_Target; //the actual target it is assisting
	cAIEntity* pAsstGive_Prev; //its position in the list, if there are multiple assistants
	cAIEntity* pAsstGive_Next;
	//it's role in assisting
	int iRole;
	
	int iNumSubEntities;

	void Init(VOID* pCA, int iCtrl);
	void Set_Asst(cAIEntity* pAsst);
	void Init_SubQueue();
	void Init_Asst();
	void Clear_Assistants();
	void Clear_Assisting();
	void Add_SubEntity( cAIEntity* pAdd);

	//Tasking functions
	void Raise_Task(int iTaskType_, double fImmediacy_);
	void Disband_Task();
	void Join_Task(cAIEntity* pEnt_Join, int iRole_);
	void Leave_Task();
	void Switch_Task(cAIEntity* pEnt_Leave, cAIEntity* pEnt_Switch, int iRole);
	void Renege_Task(int iTaskType_, double fImmediacy_);

	//
	double Compute_Renege_Threshold(cAIModeStack Stack_A, cAIModeStack Stack_B);
	double Compute_RoleChange_Threshold(int iCurRole, int iNewRole);


private:

};


class cAINew
{
public:

	int iControl;

	cAITopInfo AITopLevel;

	//intended as a debugging tool to detect errors when they happen
	void AI_Integrity_Check();

	//keep track of entities by their priorities
	cAIEntity*		pRequest_First;
	cAIEntity*		pRequest_Last;
	//keep track of the entities in their...normal order....
	//yeah, I'm not sure why I need this
	cAIEntity*		pEntities_First;
	cAIEntity*		pEntities_Last;

	int Find_First_ID(int iControl);

	int iAI_Override;

	void Init(void* pCA, HANDLE hLF, int _iControl);
	void Exit();

	//runs the AI
	void Run();
	//update top level
	void Update_TopLevel();
	//update sector tactical information
	void Update_Sector_Tactical_Info(int iSector);
	//update seed tactical information
	void Update_Seed_Tactical_Info(cAIEntity* pEnt_Seed);

	//meant to be used internally, as part of create_entity
	cAIEntity* Create_Entity(int iID, int iType);
	void Add_Entity(cAIEntity* pNew);
	cAIEntity* Create_Add_Entity(int iID, int iType);
	//find entity
	cAIEntity* Find_Entity(int iID, int iType);
	//init entity queues
	void Init_Entity_Queues();
	//remove entity
	void Clear_Entities();
	void Remove_Entity(int iID, int iType);

	HANDLE			hLogFile;
	void*			pCAccess;

	cLocationsManager LocationsManager;

	//hooks into the gameworld
	void Event_Structure_Created(cStructureA* pStruct);
	void Event_Structure_Destroyed(cStructureA* pStruct);

	void Event_Unit_Created(cUnitA* pUnit);
	void Event_Unit_Destroyed(cUnitA* pUnit);

	void Event_Sector_Created(cSector* pSector);
	void Event_Sector_Destroyed(cSector* pSector);

	void Event_Seed_Created(cBuildLocationSeedGroup* pSeedGroup);
	void Event_Seed_Destroyed(cBuildLocationSeedGroup* pSeedGroup);

	//continuous status-queue sorting
	void Sort_Status_Queue();

	//decision-making functions
	double Evaluate_Create_Task(cAIEntity* Entity, int iTask);
		double Evaluate_Create_Task_Attack_Sector(cAIEntity* Entity);
		double Evaluate_Create_Task_Defend_Sector(cAIEntity* Entity);
		double Evaluate_Create_Task_Develop_Sector(cAIEntity* Entity);
		double Evaluate_Create_Task_Develop_Seed(cAIEntity* Entity);
		double Evaluate_Create_Task_Establish_Capital(cAIEntity* Entity);
		double Evaluate_Create_Task_Establish_Outpost(cAIEntity* Entity);
		double Evaluate_Create_Task_Explore_Sector(cAIEntity* Entity);
		double Evaluate_Create_Task_Build_Storage(cAIEntity* Entity);
		double Evaluate_Create_Task_Build_Shipyard(cAIEntity* Entity);
		double Evaluate_Create_Task_Build_Forge(cAIEntity* Entity);
		double Evaluate_Create_Task_Build_Refinery(cAIEntity* Entity);
		double Evaluate_Create_Task_Build_Extractor(cAIEntity* Entity);
		double Evaluate_Create_Task_Build_Factory(cAIEntity* Entity);
		double Evaluate_Create_Task_Explore_Seed(cAIEntity* Entity);
		double Evaluate_Create_Task_Attack_Seed(cAIEntity* Entity);
		double Evaluate_Create_Task_Defend_Seed(cAIEntity* Entity);
		double Evaluate_Create_Task_Build_Unit(cAIEntity* Entity);
		double Evaluate_Create_Task_Build_Unit_Base(cAIEntity* Entity);
		double Evaluate_Create_Task_Build_Tanker_A(cAIEntity* Entity);

	double Evaluate_Unit_Join_Task(cAIEntity* Entity, cAIEntity* Entity_Join, int iTask, int iRoleNum);
	double Evaluate_Unit_Join_Task_Attack_Sector(cAIEntity* Entity, cAIEntity* Ent_Sector, int iRoleNum);
	double Evaluate_Unit_Join_Task_Defend_Sector(cAIEntity* Entity, cAIEntity* Ent_Sector, int iRoleNum);
	double Evaluate_Unit_Join_Task_Develop_Sector(cAIEntity* Entity, cAIEntity* Ent_Sector, int iRoleNum);
		double Evaluate_Unit_Join_Task_Establish_Capital(cAIEntity* Entity, cAIEntity* Ent_Sector, int iRoleNum);
		double Evaluate_Unit_Join_Task_Establish_Outpost(cAIEntity* Entity, cAIEntity* Ent_Sector, int iRoleNum);
	double Evaluate_Unit_Join_Task_Explore_Sector(cAIEntity* Entity, cAIEntity* Ent_Sector, int iRoleNum);
	double Evaluate_Unit_Join_Task_Attack_Seed(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum);
		double Evaluate_Unit_Join_Task_Build_Storage(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum);
		double Evaluate_Unit_Join_Task_Build_Shipyard(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum);
		double Evaluate_Unit_Join_Task_Build_Forge(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum);
		double Evaluate_Unit_Join_Task_Build_Refinery(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum);
		double Evaluate_Unit_Join_Task_Build_Extractor(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum);
		double Evaluate_Unit_Join_Task_Build_Factory(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum);
	double Evaluate_Unit_Join_Task_Defend_Seed(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum);
	double Evaluate_Unit_Join_Task_Explore_Seed(cAIEntity* Entity, cAIEntity* Ent_Seed, int iRoleNum);

	//for debugging purposes
	void Render_Entity(cAIEntity* Entity, LPDIRECTDRAWSURFACE7 pDrawSurf);
	void cAINew::Render_Entities_All(LPDIRECTDRAWSURFACE7 pDrawSurf);

	void Execute_Task(cAIEntity* Entity, int iTaskType, int iRole);
	
		void Execute_Task_Establish_Capital_Role0(cAIEntity* Entity);//Hosting Entity Role
		void Execute_Task_Establish_Capital_Role1(cAIEntity* Entity);//Construction Vehicle Role
		void Execute_Task_Establish_Capital_Role2(cAIEntity* Entity);//Tanker (Steel) role
		void Execute_Task_Establish_Capital_Role3(cAIEntity* Entity); //Tanker (Silicon) role

		void Execute_Task_Build_Storage_Role0(cAIEntity* Entity); //Hosting Entity Role
		void Execute_Task_Build_Storage_Role1(cAIEntity* Entity); //Construction Vehicle Role

		void Execute_Task_Build_Forge_Role0(cAIEntity* Entity); //Hosting Entity Role
		void Execute_Task_Build_Forge_Role1(cAIEntity* Entity); //Construction Vehicle Role

		void Execute_Task_Build_Refinery_Role0(cAIEntity* Entity); //Hosting Entity Role
		void Execute_Task_Build_Refinery_Role1(cAIEntity* Entity); //Construction Vehicle Role

		void Execute_Task_Build_Factory_Role0(cAIEntity* Entity); //Hosting Entity Role
		void Execute_Task_Build_Factory_Role1(cAIEntity* Entity); //Construction Vehicle Role

		void Execute_Task_Build_Shipyard_Role0(cAIEntity* Entity); //Hosting Entity Role
		void Execute_Task_Build_Shipyard_Role1(cAIEntity* Entity); //Construction Vehicle Role

		void Execute_Task_Build_Unit_Base_Role0(cAIEntity* Entity); //Hosting Entity Role

		void Execute_Task_Build_Tanker_A_Role0(cAIEntity* Entity); //Hosting Entity Role

	//Determine good location to put capital
	SQCOORDSET Compute_Capital_Location(int iSector);
	//Determine good location to put a storage
	SQCOORDSET Compute_Storage_Location(cAIEntity* pEnt_Seed, cAIEntity* pBuilderUnit);
	//Determine good location to put a forge
	SQCOORDSET Compute_Forge_Location(cAIEntity* pEnt_Seed, cAIEntity* pBuilderUnit);
	//Determine good location to put a refinery
	SQCOORDSET Compute_Refinery_Location(cAIEntity* pEnt_Seed, cAIEntity* pBuilderUnit);
	//Determine good location to put a factory
	SQCOORDSET Compute_Factory_Location(cAIEntity* pEnt_Seed, cAIEntity* pBuilderUnit);
	//Determine good location to put a shipyard
	SQCOORDSET Compute_Shipyard_Location(cAIEntity* pEnt_Seed, cAIEntity* pBuilderUnit);


	//Determine unit type to build
	int Compute_Build_Unit_Base_Type(cAIEntity* pEnt_Struct);
	int Find_Tanker_Type(cAIEntity* pEnt_Struct);

	int Decide_Most_Urgent_Storage(cAIEntity* pEnt_Seed);

	//utility function
	void Get_Task_Name(int iTaskType, char* Text);

	//list of potential capital locations 
	cAILocation*	pLoc_Cap_First; 
	cAILocation*	pLoc_Cap_Last;
	//list of potential outpost locations
	cAILocation*	pLoc_Outpost_First;
	cAILocation*	pLoc_Outpost_Last;
	//init location lists
	void Init_Location_Lists();
	void Exit_Location_Lists();
	//Add
	void Create_Add_Location(int iIndex, double _ddEfficacy, int iTick_Evaluated, int iQueue);
	void Add_Location(cAILocation* pAdd, int iQueue);
	//Delete
	void Delete_Location(cAILocation* pDelete);
	//processing
	void Location_Processing();
	double Evaluate_Location_Capital(int iIndex);
	double Evaluate_Location_Outpost(int iIndex);




private:



};

//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//08.09.2003
//this class represents a player's "Nation" (well, duh)
//this class represents "heads" all units, structures, and components bound under a certain iControl ID
//for now, this class contains information about the nation's name, the resources that it has available, and 
//the inventory of components that it possesses
class cNation
{
public:
	char	Name[80]; //80 character maxname
	int		iID; //just to be safe

	cAINew	AINew; //sigh...

	double	ddResources;  //energy this nation has (piggybacking the old variable with a new definition!)
						//I'll do something about this eventually :-/ !!!! 10.23.2006
	double  ddMoney;   //amount of money this nation has
	double  ddPossession;
	int iPossession;

	//for recording purposes
	int iNumUnits;
	int iNumStructures;

	int		iCapitalID; //structure ID of the nation's capital (negative if nation doesn't have one)

	BOOL    bAIFlag; //03.14.2004
	BOOL    bHumanPlayable;

	cComponentListB		NationalInventory;


private:

};

//08.09.2003 
//class just to make accessing nations info easier by binding them all up in a single class
class cNationManager
{
public:
	cNation**	ppNations;	//pointer to array of pointers to nations
	int			iNumNations;
	
	void*	pCAccess;

	void	MainInit(VOID* pCA, int iMaxNations); //allocates pointer array
	void	Init_Nation(int iID, LPSTR pName, int iNameLength, BOOL bAIFlag, BOOL bHumanPlayable); //note that the name is ALWAYS copied
	void	Deduct_SupportCosts();


	void	Exit();

private:

};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
//bitmap-to-map converter (2.00 version) (05.11.2004)
//modified to additionally accept sector bitmap information 06.12.2004)

//class to aid in the sector-bitmap loading process
class cSectorAssignment
{
public:
	char	IDPair[3];  //two-character string to be used to identify the sector in the mapfile (the third should just be 0 so we can just write this out as a sting)
	

private:

};

class cBM
{

public:
	

	LPSTR				InFileName, OutFileName, InFileName2;
	HANDLE				hBitmap, hBitmap2, hMapFile, hLogFile;
	
	BYTE*				ImageBuffer;
	BYTE*				ImageBuffer2;
	BYTE*				MapBuffer;
	BITMAPFILEHEADER	ImageHeader;
	BITMAPFILEHEADER	ImageHeader2;
	LPBITMAPINFO		ImageInfo;
	LPBITMAPINFO		ImageInfo2;

	DWORD				dwNumBytesRead;
	DWORD				dwNumBytesWritten;

	BOOL				rb;

	BOOL				bSector;		//indicates whether we are writing sector block information
	BOOL				bMain;			//incidates whether we actually have a main map 

	void Init(LPSTR IFN, LPSTR IFN2, LPSTR OFN);
	void Init_LogFile();
	void Write_Log(LPSTR Text);
	void WriteLog_Enter();
	void Write_MapFile_101(); //version 1.01
	void Write_MapFile_200(); //version 2.00
	
	WORD Conv_24bit_to_16bit(BYTE bR, BYTE bG, BYTE bB);

	~cBM();

private:
	int  c1, c2;  //counters

};


//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//
class cClassAccess   //while the rest don't have to be in any special order, this must go last 
{

public:
	cDD_Display*		pDD_Display;
	//cD3D_Display*		pD3D_Display;
	cDI_Input*			pDI_Input;
	cDP_Multiplay*		pDP_Multiplay;
	cDA_Audio*			pDA_Audio;
	cImageSurface*		pImages[NUM_MAX_ISURFACES];
	cSoundSegment*		pSounds[NUM_SOUNDSEGS];
	cGDisplay*			pGDisplay;
	cInputState*		pInputState;
	cInterface*         pInterface;
	cGameWorldA*		pGameWorld;
	cCommQueue*			pCommQueue;
	cAPPDATA*			pAppData;
	cTimer*				pTimer;
	cUnitListA*			pUnitList;
	cProjListA*			pProjList;
	cUnitTableA*		pUnitTable;
	cPathFinder*		pPathFinder;
	cUnitTT*			pUTypeTable;
	cProjTT*			pPTypeTable;
	cGameOptions*		pGameOptions;
	cRandGen*			pRandGen;
	cChatBuffer*		pChatBuffer;
	cMinimap*			pMinimap;
	cColorTable*		pColorTable;
	cCmpInfoTable*		pCmpInfoTable;
	cFontInfoTable*		pFontInfoTable;
	cCmpOffsetTable*	pCmpOffsetTable;
	cStructListA*		pStructList;
	cStructIDTable*		pStructIDTable;
	cStructInfoTable*	pStructInfoTable;
	cCmpIDTable*		pCmpIDTable;
	cCmdDataReqTable*	pCmdReqTable;
	cInputPacketBuffer*	pInputPacketBuffer;
	cCmdInfoTable*		pCmdInfoTable;
	cNationManager*		pNationManager;
	cNotifBuffer*		pNotifBuffer;
	cAnimInfoTable*		pAnimInfoTable;
	cEventBuffer*		pEventBuffer;
	cDbgMem*			pDbgMem;
	cStringTable*		pStringTable;
	cSectorTable*		pSectorTable;
	cConfig*			pConfig;
	cAckTable*			pAckTable;
	cMaterialInfoTable*	pMaterialInfoTable;

	void Init(cDD_Display* pDD, 
		cSectorTable* pSCT,
		cDI_Input* pDI,
		cDP_Multiplay* pDP,
		cDA_Audio* pDA,
		cImageSurface* pIM, 
		cSoundSegment* pSS,
		cGDisplay* pGD, 
		cInputState* pIS, 
		cInterface* pIF, 
		cGameWorldA* pGW, 
		cCommQueue* pCQ, 
		cAPPDATA* pAD, 
		cTimer* pTM, 
		cUnitListA* pUL,
		cUnitTableA* pUT, 
		cProjListA* pPL,
		cPathFinder* pPF,
		cUnitTT* pUTT,
		cProjTT* pPTT,
		cGameOptions* pGO,
		cRandGen* pRG,
		cChatBuffer* pCB,
		cMinimap* pMM,
		cColorTable* pCT,
		cCmpInfoTable* pCIT,
		cFontInfoTable* pFIT,
		cCmpOffsetTable* pCOT,
		cStructListA* pSLA,
		cStructIDTable* pSIDT,
		cStructInfoTable* pSIT,
		cCmpIDTable* pCIDT,
		cCmdDataReqTable* pCDRT,
		cInputPacketBuffer* pIPB,
		cCmdInfoTable* pCMIT,
		cNationManager* pNM,
		cNotifBuffer* pNB,
		cAnimInfoTable* pAIT,
		cEventBuffer* pEB,
		cDbgMem* pDM,
		cStringTable* pSRT,
		cConfig* pCFG,
		cAckTable* pAckT,
		cMaterialInfoTable* pMIT);    //bwahahaha MIT
   
		void Init_LogFile();
		void Write_Log(LPSTR Text);
		void Exit_LogFile();
		void WriteLog_Enter();


private:
	int					c1, c2;  //counters

};
//############################################################################################################################################//
//############################################################################################################################################//
//############################################################################################################################################//




