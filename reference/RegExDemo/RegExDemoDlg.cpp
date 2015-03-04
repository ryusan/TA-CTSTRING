// RegExDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegExDemo.h"
#include "RegExDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegExDemoDlg dialog

CRegExDemoDlg::CRegExDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegExDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegExDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRegExDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegExDemoDlg)
	DDX_Control(pDX, IDC_RICHEDIT1, m_rich_edit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRegExDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CRegExDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_MESSAGE(WM_FIND_NEXT, OnFindNext)
	ON_COMMAND(ID_EDIT_TESTMENU, OnEditTestmenu)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegExDemoDlg message handlers

BOOL CRegExDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Create main menu
	if(m_MainMenu.LoadMenu(IDR_MAIN_MENU))
		SetMenu(&m_MainMenu);
	else AfxMessageBox(_T("Could not create main menu."));

	// Create Find Dialog
	m_FindDialog.Create(IDD_FIND_DIALOG, this);
	m_FindDialog.ShowWindow(SW_HIDE);

	// Set Font to be courier new
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));					// zero out structure
	lf.lfHeight = 14;									// request a 12-pixel-height font
	strcpy(lf.lfFaceName, "Courier New");				// request a face name "Arial"
	m_Font.CreateFontIndirect(&lf);		// create the font
	m_rich_edit.SetFont(&m_Font);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRegExDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRegExDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRegExDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRegExDemoDlg::OnFileExit() 
{ EndDialog(0); }

void CRegExDemoDlg::OnFileOpen() 
{
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		CFile f;
		if(f.Open(dlg.GetPathName(), CFile::modeRead | CFile::typeBinary))
		{
			char *pBuff = new char[f.GetLength()+1];
			if(f.Read(pBuff, f.GetLength()) != f.GetLength())
				AfxMessageBox(_T("Did not read complete file."));
			pBuff[f.GetLength()] = 0;
			m_rich_edit.SetWindowText(pBuff);
			delete pBuff;
			f.Close();
		}
		else AfxMessageBox(_T("Could not open file " + dlg.GetPathName()));
	}
}

void CRegExDemoDlg::OnEditFind() 
{
	m_FindDialog.ShowWindow(SW_SHOW);
}

LRESULT CRegExDemoDlg::OnFindNext(WPARAM wParam, LPARAM lParam)
{
	static CString strRegEx = _T("");
	
	// return values
	int nPos;
	string strPattern;

	// get the regular expression
	CString strRegExText;
	m_FindDialog.GetDlgItemText(IDC_REG_EX_EDIT, strRegExText);

	// if regular expression was changed then parse it
	if(strRegExText.Compare(strRegEx) != 0)
	{
		if(!m_RegEx.SetRegEx(string(strRegExText)))
			AfxMessageBox(_T("Error parsing regular expression."));

		// Save Tables for debuging purposes
		m_RegEx.SaveNFATable();
		m_RegEx.SaveDFATable();
		m_RegEx.SaveNFAGraph();
		m_RegEx.SaveDFAGraph();
		
		CString strTextToSearch;
		m_rich_edit.GetWindowText(strTextToSearch);

		// now call m_RegEx.FindFirst(...);
		if(m_RegEx.FindFirst(string(strTextToSearch), nPos, strPattern))
		{
			int nStartPos = nPos;
			int nEndPos	= nStartPos + strPattern.size();

			m_rich_edit.SetSel(nStartPos, nEndPos);
			m_rich_edit.HideSelection(FALSE, TRUE);
			strRegEx = strRegExText;
			return 0;
		}
		else 
		{
			strRegEx = strRegExText;
			return -1;
		}
	}
	else
	{
		// Call find next
		if(m_RegEx.FindNext(nPos, strPattern))
		{	
			int nStartPos = nPos;
			int nEndPos	= nStartPos + strPattern.size();

			m_rich_edit.SetSel(nStartPos, nEndPos);
			m_rich_edit.HideSelection(FALSE, TRUE);
		}
		else return -1;
	}
	
	return 0;
}

void CRegExDemoDlg::OnEditTestmenu() 
{
}

void CRegExDemoDlg::OnHelpAbout() 
{
	CAboutDlg dlg;
	dlg.DoModal();
}
