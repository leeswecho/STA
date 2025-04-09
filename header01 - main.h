#define WINDOWS_EXTRA_LEAN
#include <windows.h>
#include <ddraw.h>
#include <dinput.h>

#include "classes.h"

cAPPDATA      AppData;
cDD_Display   DisplayDD;
//cD3D_Display	DisplayD3D;
cDI_Input     InputDI;
cDP_Multiplay  NetDP;
cDA_Audio		AudioDA;
cInputState   GInput;
cImageSurface Images[NUM_MAX_ISURFACES];
cSoundSegment Sounds[NUM_SOUNDSEGS];
cUnitTT		  UnitTypeTable;
cProjTT		  ProjTypeTable;
cGameWorldA   GMap;
cGDisplay     GDisplay;
cInterface    GInterface;
cCommQueue    CommQueue;
cTimer		  TimerControl;
cClassAccess  ClassAccess;
cUnitListA	  UnitList;
cUnitTableA	  UnitIDTable;
cProjListA	  ProjList;
cPathFinder   PathFinder;
cGameOptions  GameOptions;
cRandGen	  RandGen;
cChatBuffer	  ChatBuffer;
cColorTable	  ColorTable;
cMinimap	  Minimap;
cCmpInfoTable ComponentTypes;
cCmpIDTable	  CmpMasterIDTable;
cCmpOffsetTable CmpOffsetReference;
cFontInfoTable FontInfo;
cStructListA	StructList;
cStructIDTable	StructIDTable;
cStructInfoTable	StructureTypes;
cCmdDataReqTable	CmdDataReference;
cInputPacketBuffer	InputPackets;
cCmdInfoTable		CmdInfoReference;
cNationManager		Nations;
cNotifBuffer		Notifications;
cAnimInfoTable		Animations;
cEventBuffer		Events;
cDbgMem				MemDebugger;
cStringTable		Strings;
cBM					MapConv;
cSectorTable		SectorTable;
cConfig				Configuration;
cAckTable			Acknowledgements;
cMaterialInfoTable  MaterialTypes;


BOOL bRet;    //boolean used for holding the return value of PeekMessage()
char Text01[50], Text02[50], Text03[50]; //TEST var for checking purposes
char NText[50];
int iWinCount;

BOOL bExitTimerThread;

RECT SplashRect;
RECT SplashSrcRect;
