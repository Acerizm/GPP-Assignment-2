// SocketDemo.h : main header file for the SOCKETTEST application
//

#if !defined(AFX_SOCKETTEST_H__F404F878_0E73_44A0_BE52_5E57088808D0__INCLUDED_)
#define AFX_SOCKETTEST_H__F404F878_0E73_44A0_BE52_5E57088808D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSocketDemoApp:
// See SocketDemo.cpp for the implementation of this class
//

class CSocketDemoApp : public CWinApp
{
public:
	CSocketDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSocketDemoApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETTEST_H__F404F878_0E73_44A0_BE52_5E57088808D0__INCLUDED_)
