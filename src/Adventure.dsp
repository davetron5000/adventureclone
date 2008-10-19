# Microsoft Developer Studio Project File - Name="Adventure" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Adventure - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Adventure.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Adventure.mak" CFG="Adventure - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Adventure - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Adventure - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Adventure", ECAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Adventure - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "ReleaseObjects"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O1 /I "." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D DIRECTINPUT_VERSION=0x0500 /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ddraw.lib dinput.lib dxguid.lib dsound.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /machine:I386 /libpath:"d:\davec\Programming\GameLib\source\Release\lib"
# SUBTRACT LINK32 /profile
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy *.bmp Release	copy *.wav Release
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Adventure - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "DebugObjects"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "." /D "_DEBUG" /D LOGGING=1 /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D DIRECTINPUT_VERSION=0x0500 /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dsound.lib ddraw.lib dinput.lib dxguid.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /debug /machine:I386 /libpath:"d:\davec\Programming\GameLib\source\Debug\lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy *.bmp Debug	copy *.wav Debug
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Adventure - Win32 Release"
# Name "Adventure - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Bat.cpp
# End Source File
# Begin Source File

SOURCE=.\Bmp8BitFileReader.cpp
# End Source File
# Begin Source File

SOURCE=.\Bmp8BitFileWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\ConnectionTestWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\Dragon.cpp
# End Source File
# Begin Source File

SOURCE=.\EventDispatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\Game1World.cpp
# End Source File
# Begin Source File

SOURCE=.\Game2World.cpp
# End Source File
# Begin Source File

SOURCE=.\Game3World.cpp
# End Source File
# Begin Source File

SOURCE=.\Game4World.cpp
# End Source File
# Begin Source File

SOURCE=.\Game5World.cpp
# End Source File
# Begin Source File

SOURCE=.\GameObject.cpp
# End Source File
# Begin Source File

SOURCE=.\GameSelectWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\Gate.cpp
# End Source File
# Begin Source File

SOURCE=.\Holder.cpp
# End Source File
# Begin Source File

SOURCE=.\InputManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Logger.cpp
# End Source File
# Begin Source File

SOURCE=.\Magnet.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectTestWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\Player.cpp
# End Source File
# Begin Source File

SOURCE=.\Properties.cpp
# End Source File
# Begin Source File

SOURCE=.\RandomGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\RectangularImage.cpp
# End Source File
# Begin Source File

SOURCE=.\Room.cpp
# End Source File
# Begin Source File

SOURCE=.\RoomConnector.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenImage.cpp
# End Source File
# Begin Source File

SOURCE=.\ScreenshotManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleArray.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Wall.cpp
# End Source File
# Begin Source File

SOURCE=.\WinMain.cpp
# End Source File
# Begin Source File

SOURCE=.\World.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Bat.h
# End Source File
# Begin Source File

SOURCE=.\Bmp8BitFileReader.h
# End Source File
# Begin Source File

SOURCE=.\Bmp8BitFileWriter.h
# End Source File
# Begin Source File

SOURCE=.\ConnectionTestWorld.h
# End Source File
# Begin Source File

SOURCE=.\Dragon.h
# End Source File
# Begin Source File

SOURCE=.\EventDispatcher.h
# End Source File
# Begin Source File

SOURCE=.\EventListener.h
# End Source File
# Begin Source File

SOURCE=.\Game.h
# End Source File
# Begin Source File

SOURCE=.\Game1World.h
# End Source File
# Begin Source File

SOURCE=.\Game2World.h
# End Source File
# Begin Source File

SOURCE=.\Game3World.h
# End Source File
# Begin Source File

SOURCE=.\Game4World.h
# End Source File
# Begin Source File

SOURCE=.\Game5World.h
# End Source File
# Begin Source File

SOURCE=.\GameObject.h
# End Source File
# Begin Source File

SOURCE=.\GameSelectWorld.h
# End Source File
# Begin Source File

SOURCE=.\Gate.h
# End Source File
# Begin Source File

SOURCE=.\GlowingScreenImage.h
# End Source File
# Begin Source File

SOURCE=.\Holder.h
# End Source File
# Begin Source File

SOURCE=.\ImageFileReader.h
# End Source File
# Begin Source File

SOURCE=.\ImageFileWriter.h
# End Source File
# Begin Source File

SOURCE=.\InputManager.h
# End Source File
# Begin Source File

SOURCE=.\Key.h
# End Source File
# Begin Source File

SOURCE=.\Logger.h
# End Source File
# Begin Source File

SOURCE=.\Magnet.h
# End Source File
# Begin Source File

SOURCE=.\NumberObject.h
# End Source File
# Begin Source File

SOURCE=.\ObjectTestWorld.h
# End Source File
# Begin Source File

SOURCE=.\Player.h
# End Source File
# Begin Source File

SOURCE=.\Properties.h
# End Source File
# Begin Source File

SOURCE=.\RandomGenerator.h
# End Source File
# Begin Source File

SOURCE=.\RectangularImage.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Room.h
# End Source File
# Begin Source File

SOURCE=.\RoomConnector.h
# End Source File
# Begin Source File

SOURCE=.\ScreenImage.h
# End Source File
# Begin Source File

SOURCE=.\ScreenshotManager.h
# End Source File
# Begin Source File

SOURCE=.\SimpleArray.h
# End Source File
# Begin Source File

SOURCE=.\SoundManager.h
# End Source File
# Begin Source File

SOURCE=.\SoundPlayer.h
# End Source File
# Begin Source File

SOURCE=.\TransmolecularDot.h
# End Source File
# Begin Source File

SOURCE=.\Wall.h
# End Source File
# Begin Source File

SOURCE=.\WallImage.h
# End Source File
# Begin Source File

SOURCE=.\World.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Adventure.ico
# End Source File
# Begin Source File

SOURCE=.\Adventure.rc
# End Source File
# End Group
# Begin Group "Design Docs"

# PROP Default_Filter "vsd;doc"
# Begin Source File

SOURCE=.\Adventure.doc
# End Source File
# Begin Source File

SOURCE=.\Adventure.vsd
# End Source File
# Begin Source File

SOURCE=.\TODO.doc
# End Source File
# Begin Source File

SOURCE=.\WorldFileFormat.doc
# End Source File
# End Group
# Begin Group "Media"

# PROP Default_Filter ""
# Begin Group "Images"

# PROP Default_Filter "bmp"
# Begin Source File

SOURCE=.\0.bmp
# End Source File
# Begin Source File

SOURCE=.\1.bmp
# End Source File
# Begin Source File

SOURCE=.\2.bmp
# End Source File
# Begin Source File

SOURCE=.\3.bmp
# End Source File
# Begin Source File

SOURCE=.\4.bmp
# End Source File
# Begin Source File

SOURCE=.\5.bmp
# End Source File
# Begin Source File

SOURCE=.\6.bmp
# End Source File
# Begin Source File

SOURCE=.\7.bmp
# End Source File
# Begin Source File

SOURCE=.\8.bmp
# End Source File
# Begin Source File

SOURCE=.\9.bmp
# End Source File
# Begin Source File

SOURCE=.\armour.bmp
# End Source File
# Begin Source File

SOURCE=.\bat00.bmp
# End Source File
# Begin Source File

SOURCE=.\bat01.bmp
# End Source File
# Begin Source File

SOURCE=.\bg.bmp
# End Source File
# Begin Source File

SOURCE=.\bgdark.bmp
# End Source File
# Begin Source File

SOURCE=.\bridge.bmp
# End Source File
# Begin Source File

SOURCE=.\candle.bmp
# End Source File
# Begin Source File

SOURCE=.\davetron.bmp
# End Source File
# Begin Source File

SOURCE=.\difficult.bmp
# End Source File
# Begin Source File

SOURCE=.\dot.bmp
# End Source File
# Begin Source File

SOURCE=".\dragon-attack.bmp"
# End Source File
# Begin Source File

SOURCE=".\dragon-dead.bmp"
# End Source File
# Begin Source File

SOURCE=".\dragon-normal.bmp"
# End Source File
# Begin Source File

SOURCE=.\easteregg.bmp
# End Source File
# Begin Source File

SOURCE=.\easy.bmp
# End Source File
# Begin Source File

SOURCE=.\extraitems.bmp
# End Source File
# Begin Source File

SOURCE=.\gate00.bmp
# End Source File
# Begin Source File

SOURCE=.\gate01.bmp
# End Source File
# Begin Source File

SOURCE=.\gate02.bmp
# End Source File
# Begin Source File

SOURCE=.\gate03.bmp
# End Source File
# Begin Source File

SOURCE=.\gate04.bmp
# End Source File
# Begin Source File

SOURCE=.\key.bmp
# End Source File
# Begin Source File

SOURCE=.\magnet.bmp
# End Source File
# Begin Source File

SOURCE=.\moreselect.bmp
# End Source File
# Begin Source File

SOURCE=.\setup.bmp
# End Source File
# Begin Source File

SOURCE=.\sword.bmp
# End Source File
# Begin Source File

SOURCE=.\thanks.bmp
# End Source File
# End Group
# Begin Group "Sounds"

# PROP Default_Filter "wav"
# Begin Source File

SOURCE=.\alldeath.wav
# End Source File
# Begin Source File

SOURCE=.\attack.wav
# End Source File
# Begin Source File

SOURCE=.\death.wav
# End Source File
# Begin Source File

SOURCE=.\drop.wav
# End Source File
# Begin Source File

SOURCE=.\ending.wav
# End Source File
# Begin Source File

SOURCE=.\pickup.wav
# End Source File
# Begin Source File

SOURCE=.\swallow.wav
# End Source File
# End Group
# End Group
# End Target
# End Project
