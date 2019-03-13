
#pragma once

// PROCESS_EVENTS_IN_GUI_THREAD = TRUE  -> process the events in the GUI  thread
// PROCESS_EVENTS_IN_GUI_THREAD = FALSE -> process the events in an extra thread
#define PROCESS_EVENTS_IN_GUI_THREAD   FALSE

// SEND_LARGE_DATA = 0      -> Send the text string which the user has entered
// SEND_LARGE_DATA = 100000 -> Send a 100kB string "AAAAAAAA...", each time with another character
// Note: When sending large data and the Output Edit box becomes very full, the output becomes very slow.
// The application may even crash if you write 100 Megabyte into an Edit box.
// Then hit the "Clear" button to reset the Editbox!
#define SEND_LARGE_DATA   0

// The time in seconds after which a server disconnects his idle clients
// Set 0 to disable the timeout
#define MAX_SERVER_IDLE_TIME  5*60 // 5 minutes

// The time in seconds after which a client disconnects from his idle server
// Set 0 to disable the timeout
#define MAX_CLIENT_IDLE_TIME  0 // off

// The titlebar of the window
#define TITLE  CString("ElmüSoft TCP Socket 1.5")


#include "Socket.h"

class CSocketDemoDlg : public CDialog
{
	// Study the different demo modes to see how to work with pi_RecvMem!
	enum eDemoMode
	{
		// Receive data is printed immediately exactly as it comes from Winsock
		E_NORMAL = 0,
		
		// Sends all data prefixed with a DWORD which contains the length of the entire datablock.
		// Even if the data arrives in multiple parts the receiver knows in advance 
		// how many data he has to read into pi_RecvMem
		E_PREFIXED = 1,
		
		// Accumulate characters sent from a Telnet client (Port 23)
		// and print them as entire lines rather than single characters.
		// You can send linefeeds from SocketDemo if you enter "Hello\nWorld\n"
		E_TELNET = 2
	};

public:
	 CSocketDemoDlg(CWnd* pParent = NULL);
	~CSocketDemoDlg();

	//{{AFX_DATA(CSocketDemoDlg)
	enum { IDD = IDD_SOCKETTEST_DIALOG };
	CComboBox	mi_ComboDemoMode;
	CComboBox	mi_ComboBindTo;
	CEdit	mi_Output;
	CComboBox	mi_ComboSendTo;
	CIPAddressCtrl	mi_IpAddr;
	CString	ms_Send;
	int		ms32_Port;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSocketDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	void    PumpMessages();
	CString GetErrMsg(DWORD u32_Error);
	CString FormatIP (DWORD u32_IP);
	void    ProcessEvents();
	void    Print(CString s_Format, ...);
	CString CopyToString(char* s8_Buf, DWORD u32_Bytes);
	void    CloseSockets();
	BOOL    SendTo(SOCKET h_Socket, CString s_SendData);
	CString FormatDisplayName(SOCKET h_Socket);
	void    ProcessReceivedDataNormal(TCP::cSocket::cMemory* pi_RecvMem);
	void    ProcessReceivedDataPrefix(TCP::cSocket::cMemory* pi_RecvMem);
	void    ProcessReceivedDataTelnet(TCP::cSocket::cMemory* pi_RecvMem);

	static ULONG WINAPI ProcessEventThread(void* p_Param);

	//{{AFX_MSG(CSocketDemoDlg)
	afx_msg void OnBtnSend();
	afx_msg void OnBtnListen();
	afx_msg void OnBtnConnect();
	afx_msg void OnClose();
	afx_msg void OnBtnClose();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtnClear();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR u32_TimerID);
	afx_msg void OnSelchangeComboDemoType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	TCP::cSocket      mi_Socket;
	BOOL              mb_DlgClosed;
	BOOL              mb_RefreshCombo;
	CString           ms_Output;
	CRITICAL_SECTION  mk_Critical;
	HANDLE            mh_Thread;
	eDemoMode         me_DemoMode;

	TCP::cSocket::cHash<DWORD, DWORD> mi_LocalIPs;
	TCP::cSocket::cHash<SOCKET,DWORD> mi_SocketList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

