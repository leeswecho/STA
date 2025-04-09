# Microsoft Developer Studio Project File - Name="Sam37" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Sam37 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Sam37.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Sam37.mak" CFG="Sam37 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Sam37 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Sam37 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Sam37 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /Zp16 /W3 /vmg /vd0 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib ddraw.lib dinput8.lib dxerr9.lib dplay.lib dsound.lib fmodvc.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Sam37 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib dxguid.lib ddraw.lib dinput8.lib dxerr9.lib dplayx.lib dsound.lib  fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Sam37 - Win32 Release"
# Name "Sam37 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\cAINew.cpp
# End Source File
# Begin Source File

SOURCE=.\cAppData.cpp
# End Source File
# Begin Source File

SOURCE=.\cBM.cpp
# End Source File
# Begin Source File

SOURCE=.\cChatBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\cClassAccess.cpp
# End Source File
# Begin Source File

SOURCE=.\cColorTable.cpp
# End Source File
# Begin Source File

SOURCE=.\cCommQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\cComponentA.cpp
# End Source File
# Begin Source File

SOURCE=.\cConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\cDA_Audio.cpp
# End Source File
# Begin Source File

SOURCE=.\cDbgMem.cpp
# End Source File
# Begin Source File

SOURCE=.\cDD_Display.cpp
# End Source File
# Begin Source File

SOURCE=.\cDI_Input.cpp
# End Source File
# Begin Source File

SOURCE=.\cDP_Multiplay.cpp
# End Source File
# Begin Source File

SOURCE=.\cDrawIssue.cpp
# End Source File
# Begin Source File

SOURCE=.\cEventBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\cFontInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\cGameOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\cGameWorldA.cpp
# End Source File
# Begin Source File

SOURCE=.\cGDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\cImageSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\cInputState.cpp
# End Source File
# Begin Source File

SOURCE=.\cInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\cMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\cMinimap.cpp
# End Source File
# Begin Source File

SOURCE=.\cNation.cpp
# End Source File
# Begin Source File

SOURCE=.\cNotifBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\cPathFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\cProjectileA.cpp
# End Source File
# Begin Source File

SOURCE=.\cRandGen.cpp
# End Source File
# Begin Source File

SOURCE=.\cSector.cpp
# End Source File
# Begin Source File

SOURCE=.\cSoundSegment.cpp
# End Source File
# Begin Source File

SOURCE=.\cSrcGraphicsInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\cStringTable.cpp
# End Source File
# Begin Source File

SOURCE=.\cStructureA.cpp
# End Source File
# Begin Source File

SOURCE=.\cTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\cUnitA.cpp
# End Source File
# Begin Source File

SOURCE=".\source01 - main.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\classes.h
# End Source File
# Begin Source File

SOURCE=".\header01 - main.h"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
