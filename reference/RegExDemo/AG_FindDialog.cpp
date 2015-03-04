// AG_FindDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RegExDemo.h"
#include "AG_FindDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAG_FindDialog dialog


CAG_FindDialog::CAG_FindDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAG_FindDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAG_FindDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAG_FindDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAG_FindDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAG_FindDialog, CDialog)
	//{{AFX_MSG_MAP(CAG_FindDialog)
	ON_BN_CLICKED(IDC_CLOSE_BUTTON, OnCloseButton)
	ON_BN_CLICKED(IDC_FIND_NEXT, OnFindNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAG_FindDialog message handlers

void CAG_FindDialog::OnCloseButton() 
{
	ShowWindow(SW_HIDE);
}

void CAG_FindDialog::OnFindNext() 
{
	if(AfxGetMainWnd()->SendMessage(WM_FIND_NEXT, 0, 0) == -1)
		AfxMessageBox(_T("Done!"));
}
