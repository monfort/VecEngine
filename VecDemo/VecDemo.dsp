# Microsoft Developer Studio Project File - Name="VecDemo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=VecDemo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VecDemo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VecDemo.mak" CFG="VecDemo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VecDemo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "VecDemo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VecDemo - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /WX /GR /GX /Zi /Gf /I "../VecEngine" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40d /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40d /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /map /debug /debugtype:both /machine:I386 /out:"..\Out\Release/VecDemo.exe"

!ELSEIF  "$(CFG)" == "VecDemo - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /WX /Gm /GR /GX /ZI /Od /I "../VecEngine" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40d /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x40d /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /map /debug /machine:I386 /out:"..\Out\Debug/VecDemo.exe"

!ENDIF 

# Begin Target

# Name "VecDemo - Win32 Release"
# Name "VecDemo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "GUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GridPropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LineTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewPagePropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PageBackgroundDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PicTransparentDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PictureSizeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RotateFreeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShadowDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShapeFillDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ShapesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TextEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VecDemoDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\VecDemoView.cpp
# End Source File
# End Group
# Begin Group "FrameWork"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VecDemo.cpp
# End Source File
# Begin Source File

SOURCE=.\VecDemo.rc
# ADD BASE RSC /l 0x40d
# ADD RSC /l 0x40d /i "../VecEngine"
# End Source File
# End Group
# Begin Group "General"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GobalBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\MemBuff.cpp
# End Source File
# Begin Source File

SOURCE=.\NodedArchive.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "GUI."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GridPropDlg.h
# End Source File
# Begin Source File

SOURCE=.\LineTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewPagePropDlg.h
# End Source File
# Begin Source File

SOURCE=.\PageBackgroundDlg.h
# End Source File
# Begin Source File

SOURCE=.\PicTransparentDlg.h
# End Source File
# Begin Source File

SOURCE=.\PictureSizeDlg.h
# End Source File
# Begin Source File

SOURCE=.\RotateFreeDlg.h
# End Source File
# Begin Source File

SOURCE=.\ShadowDlg.h
# End Source File
# Begin Source File

SOURCE=.\ShapeFillDlg.h
# End Source File
# Begin Source File

SOURCE=.\ShapesDlg.h
# End Source File
# Begin Source File

SOURCE=.\TextEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\VecDemoDoc.h
# End Source File
# Begin Source File

SOURCE=.\VecDemoView.h
# End Source File
# End Group
# Begin Group "FrameWork."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VecDemo.h
# End Source File
# End Group
# Begin Group "General."

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GobalBuffer.h
# End Source File
# Begin Source File

SOURCE=.\MemBuff.h
# End Source File
# Begin Source File

SOURCE=.\NodedArchive.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\VecEngine\res\Copy4way.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\create_c.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\crop_cur.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\guide_ho.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\guide_ve.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\locked_o.bmp
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\my_move_.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\my_set_s.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\pan.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\pan_act.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\pic_move.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\pic_resi.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\pic_rota.cur
# End Source File
# Begin Source File

SOURCE=.\res\shapes_b.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_align.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_Object.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar_page.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbox.bmp
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\Trck4way.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\Trcknesw.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\Trckns.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\Trcknwse.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\Trckwe.cur
# End Source File
# Begin Source File

SOURCE=.\res\VecDemo.ico
# End Source File
# Begin Source File

SOURCE=.\res\VecDemo.rc2
# End Source File
# Begin Source File

SOURCE=.\res\VecDemoDoc.ico
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\zoom_in.cur
# End Source File
# Begin Source File

SOURCE=..\VecEngine\res\zoom_out.cur
# End Source File
# End Group
# End Target
# End Project
# Section VecDemo : {8E3867A3-8586-11D1-B16A-00C0F0283628}
# 	2:21:DefaultSinkHeaderFile:statusbar.h
# 	2:16:DefaultSinkClass:CStatusBar1
# End Section
# Section VecDemo : {1BE65FA9-CBF9-11D2-BBC7-00104B9E0792}
# 	2:5:Class:CSSComboBox
# 	2:10:HeaderFile:sscombobox.h
# 	2:8:ImplFile:sscombobox.cpp
# End Section
# Section VecDemo : {1BE65FAB-CBF9-11D2-BBC7-00104B9E0792}
# 	2:5:Class:CSSMenu
# 	2:10:HeaderFile:ssmenu.h
# 	2:8:ImplFile:ssmenu.cpp
# End Section
# Section VecDemo : {8E3867AA-8586-11D1-B16A-00C0F0283628}
# 	2:5:Class:CPanel
# 	2:10:HeaderFile:panel.h
# 	2:8:ImplFile:panel.cpp
# End Section
# Section VecDemo : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section VecDemo : {8C3D4AA7-2599-11D2-BAF1-00104B9E0792}
# 	2:21:DefaultSinkHeaderFile:sssplitter.h
# 	2:16:DefaultSinkClass:CSSSplitter
# End Section
# Section VecDemo : {1BE65FAF-CBF9-11D2-BBC7-00104B9E0792}
# 	2:5:Class:CSSToolBarControl
# 	2:10:HeaderFile:sstoolbarcontrol.h
# 	2:8:ImplFile:sstoolbarcontrol.cpp
# End Section
# Section VecDemo : {1BE65FA3-CBF9-11D2-BBC7-00104B9E0792}
# 	2:5:Class:CSSToolBarsCollection
# 	2:10:HeaderFile:sstoolbarscollection.h
# 	2:8:ImplFile:sstoolbarscollection.cpp
# End Section
# Section VecDemo : {8C3D4AA1-2599-11D2-BAF1-00104B9E0792}
# 	2:5:Class:CPane
# 	2:10:HeaderFile:pane.h
# 	2:8:ImplFile:pane.cpp
# End Section
# Section VecDemo : {1BE65FA7-CBF9-11D2-BBC7-00104B9E0792}
# 	2:5:Class:CSSToolsCollection
# 	2:10:HeaderFile:sstoolscollection.h
# 	2:8:ImplFile:sstoolscollection.cpp
# End Section
# Section VecDemo : {8E3867A1-8586-11D1-B16A-00C0F0283628}
# 	2:5:Class:CStatusBar1
# 	2:10:HeaderFile:statusbar.h
# 	2:8:ImplFile:statusbar.cpp
# End Section
# Section VecDemo : {8C3D4AA5-2599-11D2-BAF1-00104B9E0792}
# 	2:5:Class:CSSSplitter
# 	2:10:HeaderFile:sssplitter.h
# 	2:8:ImplFile:sssplitter.cpp
# End Section
# Section VecDemo : {1BE65FA1-CBF9-11D2-BBC7-00104B9E0792}
# 	2:5:Class:CSSToolBar
# 	2:10:HeaderFile:sstoolbar.h
# 	2:8:ImplFile:sstoolbar.cpp
# End Section
# Section VecDemo : {1BE65FAD-CBF9-11D2-BBC7-00104B9E0792}
# 	2:5:Class:CSSEdit
# 	2:10:HeaderFile:ssedit.h
# 	2:8:ImplFile:ssedit.cpp
# End Section
# Section VecDemo : {1BE65FB1-CBF9-11D2-BBC7-00104B9E0792}
# 	2:21:DefaultSinkHeaderFile:sstoolbarcontrol.h
# 	2:16:DefaultSinkClass:CSSToolBarControl
# End Section
# Section VecDemo : {8E3867A4-8586-11D1-B16A-00C0F0283628}
# 	2:5:Class:CPanels
# 	2:10:HeaderFile:panels.h
# 	2:8:ImplFile:panels.cpp
# End Section
# Section VecDemo : {1BE65FA5-CBF9-11D2-BBC7-00104B9E0792}
# 	2:5:Class:CSSTool
# 	2:10:HeaderFile:sstool.h
# 	2:8:ImplFile:sstool.cpp
# End Section
# Section VecDemo : {766BA581-6514-11D3-9BEB-0020182C5402}
# 	2:23:CLASS: CPersistentFrame:CPersistentFrame
# 	2:11:Persist.cpp:Persist.cpp
# 	2:9:Persist.h:Persist.h
# 	2:19:Application Include:VecDemo.h
# End Section
# Section VecDemo : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section VecDemo : {8C3D4AA3-2599-11D2-BAF1-00104B9E0792}
# 	2:5:Class:CPanesCollection
# 	2:10:HeaderFile:panescollection.h
# 	2:8:ImplFile:panescollection.cpp
# End Section
