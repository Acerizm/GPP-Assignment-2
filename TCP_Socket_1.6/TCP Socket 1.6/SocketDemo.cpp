// SocketDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SocketDemo.h"
#include "SocketDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSocketDemoApp

BEGIN_MESSAGE_MAP(CSocketDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CSocketDemoApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSocketDemoApp construction

CSocketDemoApp::CSocketDemoApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSocketDemoApp object

CSocketDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSocketDemoApp initialization

BOOL CSocketDemoApp::InitInstance()
{
	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CSocketDemoDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
