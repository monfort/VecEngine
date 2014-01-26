# Microsoft Developer Studio Project File - Name="VecEngineLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=VecEngineLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VecEngineLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VecEngineLib.mak" CFG="VecEngineLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VecEngineLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "VecEngineLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VecEngineLib - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /WX /GR /GX /I "..\.." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x40d /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40d /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "VecEngineLib - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /WX /Gm /GR /GX /ZI /Od /I "..\.." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x40d /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40d /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "VecEngineLib - Win32 Release"
# Name "VecEngineLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "VecClasses"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\VecBase.cpp
# End Source File
# Begin Source File

SOURCE=..\VecBitmapBase.cpp
# End Source File
# Begin Source File

SOURCE=..\VecContainer.cpp
# End Source File
# Begin Source File

SOURCE=..\VecEllipse.cpp
# End Source File
# Begin Source File

SOURCE=..\VecEllipsePic.cpp
# End Source File
# Begin Source File

SOURCE=..\VecEllipseText.cpp
# End Source File
# Begin Source File

SOURCE=..\VecFrameContainer.cpp
# End Source File
# Begin Source File

SOURCE=..\VecLine.cpp
# End Source File
# Begin Source File

SOURCE=..\VecPointsBase.cpp
# End Source File
# Begin Source File

SOURCE=..\VecPolygon.cpp
# End Source File
# Begin Source File

SOURCE=..\VecPolygonPic.cpp
# End Source File
# Begin Source File

SOURCE=..\VecPolygonText.cpp
# End Source File
# Begin Source File

SOURCE=..\VecPolyline.cpp
# End Source File
# Begin Source File

SOURCE=..\VecRectangle.cpp
# End Source File
# Begin Source File

SOURCE=..\VecRectanglePic.cpp
# End Source File
# Begin Source File

SOURCE=..\VecRectangleText.cpp
# End Source File
# Begin Source File

SOURCE=..\VecTextBase.cpp
# End Source File
# End Group
# Begin Group "GenClasses"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\BitmapBaseEx.cpp
# End Source File
# Begin Source File

SOURCE=..\DragMan.cpp
# End Source File
# Begin Source File

SOURCE=..\G_Routines.cpp
# End Source File
# Begin Source File

SOURCE=..\Guide.cpp
# End Source File
# Begin Source File

SOURCE=..\MemBuffEx.cpp
# End Source File
# Begin Source File

SOURCE=..\MHookWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\MRulerOrganizer.cpp
# End Source File
# Begin Source File

SOURCE=..\SelectionManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Shapes.cpp
# End Source File
# Begin Source File

SOURCE=..\Snapper.cpp
# End Source File
# Begin Source File

SOURCE=..\Text_Routines.cpp
# End Source File
# Begin Source File

SOURCE=..\TextEditRect.cpp
# End Source File
# Begin Source File

SOURCE=..\UndoManager.cpp
# End Source File
# Begin Source File

SOURCE=..\VecController.cpp
# End Source File
# Begin Source File

SOURCE=..\VecTypes.cpp
# End Source File
# Begin Source File

SOURCE=..\VecView.cpp
# End Source File
# Begin Source File

SOURCE=..\VecWorkspace.cpp
# End Source File
# End Group
# Begin Group "Trackers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\BaseTracker.cpp
# End Source File
# Begin Source File

SOURCE=..\EllipseTracker.cpp
# End Source File
# Begin Source File

SOURCE=..\LineTracker.cpp
# End Source File
# Begin Source File

SOURCE=..\MRectTracker.cpp
# End Source File
# Begin Source File

SOURCE=..\PicTracker.cpp
# End Source File
# Begin Source File

SOURCE=..\PolygonTracker.cpp
# End Source File
# Begin Source File

SOURCE=..\PolylineCreatorTracker.cpp
# End Source File
# Begin Source File

SOURCE=..\RectangleTracker.cpp
# End Source File
# Begin Source File

SOURCE=..\SuperRotateTracker.cpp
# End Source File
# Begin Source File

SOURCE=..\SuperTracker.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\VecEngine.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "VecClasses."

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\VecBase.h
# End Source File
# Begin Source File

SOURCE=..\VecBitmapBase.h
# End Source File
# Begin Source File

SOURCE=..\VecContainer.h
# End Source File
# Begin Source File

SOURCE=..\VecEllipse.h
# End Source File
# Begin Source File

SOURCE=..\VecEllipsePic.h
# End Source File
# Begin Source File

SOURCE=..\VecEllipseText.h
# End Source File
# Begin Source File

SOURCE=..\VecFrameContainer.h
# End Source File
# Begin Source File

SOURCE=..\VecLine.h
# End Source File
# Begin Source File

SOURCE=..\VecPointsBase.h
# End Source File
# Begin Source File

SOURCE=..\VecPolygon.h
# End Source File
# Begin Source File

SOURCE=..\VecPolygonPic.h
# End Source File
# Begin Source File

SOURCE=..\VecPolygonText.h
# End Source File
# Begin Source File

SOURCE=..\VecPolyline.h
# End Source File
# Begin Source File

SOURCE=..\VecRectangle.h
# End Source File
# Begin Source File

SOURCE=..\VecRectanglePic.h
# End Source File
# Begin Source File

SOURCE=..\VecRectangleText.h
# End Source File
# Begin Source File

SOURCE=..\VecTextBase.h
# End Source File
# End Group
# Begin Group "GenClasses."

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\BitmapBaseEx.h
# End Source File
# Begin Source File

SOURCE=..\DragMan.h
# End Source File
# Begin Source File

SOURCE=..\G_Routines.h
# End Source File
# Begin Source File

SOURCE=..\Guide.h
# End Source File
# Begin Source File

SOURCE=..\MemBuffEx.h
# End Source File
# Begin Source File

SOURCE=..\MHookWnd.h
# End Source File
# Begin Source File

SOURCE=..\MRulerOrganizer.h
# End Source File
# Begin Source File

SOURCE=..\SelectionManager.h
# End Source File
# Begin Source File

SOURCE=..\Shapes.h
# End Source File
# Begin Source File

SOURCE=..\Snapper.h
# End Source File
# Begin Source File

SOURCE=..\Text_Routines.h
# End Source File
# Begin Source File

SOURCE=..\TextEditRect.h
# End Source File
# Begin Source File

SOURCE=..\UndoManager.h
# End Source File
# Begin Source File

SOURCE=..\VecController.h
# End Source File
# Begin Source File

SOURCE=..\VecTypes.h
# End Source File
# Begin Source File

SOURCE=..\VecView.h
# End Source File
# Begin Source File

SOURCE=..\VecWorkspace.h
# End Source File
# End Group
# Begin Group "Trackers."

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\BaseTracker.h
# End Source File
# Begin Source File

SOURCE=..\EllipseTracker.h
# End Source File
# Begin Source File

SOURCE=..\LineTracker.h
# End Source File
# Begin Source File

SOURCE=..\MRectTracker.h
# End Source File
# Begin Source File

SOURCE=..\PicTracker.h
# End Source File
# Begin Source File

SOURCE=..\PolygonTracker.h
# End Source File
# Begin Source File

SOURCE=..\PolylineCreatorTracker.h
# End Source File
# Begin Source File

SOURCE=..\RectangleTracker.h
# End Source File
# Begin Source File

SOURCE=..\SuperRotateTracker.h
# End Source File
# Begin Source File

SOURCE=..\SuperTracker.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\res\Copy4way.cur
# End Source File
# Begin Source File

SOURCE=..\res\create_c.cur
# End Source File
# Begin Source File

SOURCE=..\res\crop_cur.cur
# End Source File
# Begin Source File

SOURCE=..\res\guide_ho.cur
# End Source File
# Begin Source File

SOURCE=..\res\guide_ve.cur
# End Source File
# Begin Source File

SOURCE=..\res\locked.bmp
# End Source File
# Begin Source File

SOURCE=..\res\locked_o.bmp
# End Source File
# Begin Source File

SOURCE=..\res\my_move_.cur
# End Source File
# Begin Source File

SOURCE=..\res\my_set_s.cur
# End Source File
# Begin Source File

SOURCE=..\res\pan.cur
# End Source File
# Begin Source File

SOURCE=..\res\pan_act.cur
# End Source File
# Begin Source File

SOURCE=..\res\pic_move.cur
# End Source File
# Begin Source File

SOURCE=..\res\pic_resi.cur
# End Source File
# Begin Source File

SOURCE=..\res\pic_rota.cur
# End Source File
# Begin Source File

SOURCE=..\res\Trck4way.cur
# End Source File
# Begin Source File

SOURCE=..\res\Trcknesw.cur
# End Source File
# Begin Source File

SOURCE=..\res\Trckns.cur
# End Source File
# Begin Source File

SOURCE=..\res\Trcknwse.cur
# End Source File
# Begin Source File

SOURCE=..\res\Trckwe.cur
# End Source File
# Begin Source File

SOURCE=..\res\zoom_in.cur
# End Source File
# Begin Source File

SOURCE=..\res\zoom_out.cur
# End Source File
# End Group
# End Target
# End Project
