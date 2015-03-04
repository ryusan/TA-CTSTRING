// RegExDemoDlg.h : header file
//

#if !defined(AFX_REGEXDEMODLG_H__85624EB8_426C_4F32_B10B_6D460C99F025__INCLUDED_)
#define AFX_REGEXDEMODLG_H__85624EB8_426C_4F32_B10B_6D460C99F025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CRegExDemoDlg dialog

#include "AG_FindDialog.h"
#include "AG_RegEx.h"

class CRegExDemoDlg : public CDialog
{
// Construction
public:
	CRegExDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRegExDemoDlg)
	enum { IDD = IDD_REGEXDEMO_DIALOG };
	CRichEditCtrl	m_rich_edit;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegExDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Main Menu 
	CMenu m_MainMenu;

	// Find Dialog
	CAG_FindDialog m_FindDialog;

	// Regular Expression Parser
	CAG_RegEx m_RegEx;

	// Rich Edit Font
	CFont m_Font;

	// Generated message map functions
	//{{AFX_MSG(CRegExDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFileExit();
	afx_msg void OnFileOpen();
	afx_msg void OnEditFind();
	afx_msg LRESULT OnFindNext(WPARAM wParam, LPARAM lParam);
	virtual void OnOK() {};
	afx_msg void OnEditTestmenu();
	afx_msg void OnHelpAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGEXDEMODLG_H__85624EB8_426C_4F32_B10B_6D460C99F025__INCLUDED_)
