#if !defined(AFX_AG_FINDDIALOG_H__2CE336BF_3085_45BA_9E62_668EB0B5CF74__INCLUDED_)
#define AFX_AG_FINDDIALOG_H__2CE336BF_3085_45BA_9E62_668EB0B5CF74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AG_FindDialog.h : header file
//

#define WM_FIND_NEXT	WM_USER+1

/////////////////////////////////////////////////////////////////////////////
// CAG_FindDialog dialog

class CAG_FindDialog : public CDialog
{
// Construction
public:
	CAG_FindDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAG_FindDialog)
	enum { IDD = IDD_FIND_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAG_FindDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAG_FindDialog)
		virtual void OnOK() {};
	afx_msg void OnCloseButton();
	afx_msg void OnFindNext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AG_FINDDIALOG_H__2CE336BF_3085_45BA_9E62_668EB0B5CF74__INCLUDED_)
