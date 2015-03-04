// RegExDemo.h : main header file for the REGEXDEMO application
//

#if !defined(AFX_REGEXDEMO_H__04E1ED84_618D_4E9A_AE79_D2C6C96CB709__INCLUDED_)
#define AFX_REGEXDEMO_H__04E1ED84_618D_4E9A_AE79_D2C6C96CB709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRegExDemoApp:
// See RegExDemo.cpp for the implementation of this class
//

class CRegExDemoApp : public CWinApp
{
public:
	CRegExDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegExDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRegExDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGEXDEMO_H__04E1ED84_618D_4E9A_AE79_D2C6C96CB709__INCLUDED_)
