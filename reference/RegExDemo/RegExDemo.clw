; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CRegExDemoDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "regexdemo.h"
LastPage=0

ClassCount=4
Class1=CAG_FindDialog
Class2=CRegExDemoApp
Class3=CAboutDlg
Class4=CRegExDemoDlg

ResourceCount=4
Resource1=IDD_REGEXDEMO_DIALOG
Resource2=IDD_FIND_DIALOG
Resource3=IDD_ABOUTBOX
Resource4=IDR_MAIN_MENU

[CLS:CAG_FindDialog]
Type=0
BaseClass=CDialog
HeaderFile=AG_FindDialog.h
ImplementationFile=AG_FindDialog.cpp
LastObject=IDC_FIND_NEXT
Filter=D
VirtualFilter=dWC

[CLS:CRegExDemoApp]
Type=0
BaseClass=CWinApp
HeaderFile=RegExDemo.h
ImplementationFile=RegExDemo.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=RegExDemoDlg.cpp
ImplementationFile=RegExDemoDlg.cpp
LastObject=CAboutDlg

[CLS:CRegExDemoDlg]
Type=0
BaseClass=CDialog
HeaderFile=RegExDemoDlg.h
ImplementationFile=RegExDemoDlg.cpp
LastObject=ID_HELP_ABOUT
Filter=D
VirtualFilter=dWC

[DLG:IDD_FIND_DIALOG]
Type=1
Class=CAG_FindDialog
ControlCount=3
Control1=IDC_FIND_NEXT,button,1342242816
Control2=IDC_REG_EX_EDIT,edit,1350631552
Control3=IDC_CLOSE_BUTTON,button,1342242816

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_REGEXDEMO_DIALOG]
Type=1
Class=CRegExDemoDlg
ControlCount=1
Control1=IDC_RICHEDIT1,RICHEDIT,1353781444

[MNU:IDR_MAIN_MENU]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_EXIT
Command3=ID_EDIT_FIND
Command4=ID_EDIT_TESTMENU
Command5=ID_HELP_ABOUT
CommandCount=5

