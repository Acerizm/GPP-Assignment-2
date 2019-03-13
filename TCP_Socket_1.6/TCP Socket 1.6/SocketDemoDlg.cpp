

#include "stdafx.h"
#include "SocketDemo.h"
#include "SocketDemoDlg.h"

void CSocketDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSocketDemoDlg)
	DDX_Control(pDX, IDC_COMBO_DEMO_MODE, mi_ComboDemoMode);
	DDX_Control(pDX, IDC_COMBO_BINDTO, mi_ComboBindTo);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, mi_Output);
	DDX_Control(pDX, IDC_COMBO_SENDTO, mi_ComboSendTo);
	DDX_Control(pDX, IDC_IPADDR, mi_IpAddr);
	DDX_Text(pDX, IDC_EDIT_SEND, ms_Send);
	DDX_Text(pDX, IDC_EDIT_PORT, ms32_Port);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSocketDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CSocketDemoDlg)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_BN_CLICKED(IDC_BTN_LISTEN, OnBtnListen)
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBtnClear)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_DEMO_MODE, OnSelchangeComboDemoType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Avoid that hitting Enter closes the window
void CSocketDemoDlg::OnOK()
{
}

// ----------------------------------------------------------

// Constructor
CSocketDemoDlg::CSocketDemoDlg(CWnd* pParent)
	: CDialog(CSocketDemoDlg::IDD, pParent),
	  mi_SocketList(64)
{
	//{{AFX_DATA_INIT(CSocketDemoDlg)
	ms_Send = _T("Hello World");
	ms32_Port = 2000;
	//}}AFX_DATA_INIT

	mb_DlgClosed    = FALSE;
	mb_RefreshCombo = FALSE;
	InitializeCriticalSection(&mk_Critical);
}

CSocketDemoDlg::~CSocketDemoDlg()
{
	DeleteCriticalSection(&mk_Critical);
}

BOOL CSocketDemoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText(TITLE);

	// Get all local IP addresses for each network adapter
	DWORD u32_Err = mi_Socket.GetLocalIPs(&mi_LocalIPs);
	if (u32_Err)
		Print(_T("Error retrieving Local IP: %s"), GetErrMsg(u32_Err));
	else
		mi_IpAddr.SetAddress(htonl(mi_LocalIPs.GetValueByIndex(0)));

	CString s_Text;
	#if _UNICODE
		s_Text = _T("Compiled as UNICODE,");
	#else
		s_Text = _T("Compiled as MBCS,");
	#endif

	#if _DEBUG
		s_Text += _T(" DEBUG,");
	#else
		s_Text += _T(" RELEASE,");
	#endif

	#if PROCESS_EVENTS_IN_GUI_THREAD
		s_Text += _T(" SingleThreaded,");
	#else
		s_Text += _T(" MultiThreaded,");
	#endif

	s_Text += _T(" Local IP = ");

	if (mi_LocalIPs.GetCount() > 1)
		mi_ComboBindTo.AddString(_T("All local IP's"));

	for (DWORD i=0; i<mi_LocalIPs.GetCount(); i++)
	{
		CString s_IP = FormatIP(mi_LocalIPs.GetValueByIndex(i));
		
		if (i>0) s_Text += _T(" + ");
		s_Text += s_IP;

		mi_ComboBindTo.AddString(s_IP);
	}

	mi_ComboBindTo.  SetCurSel(0);
	mi_ComboDemoMode.SetCurSel(0);
	me_DemoMode = E_NORMAL;

	Print(s_Text);

	// Refresh the Combobox and Output Editbox only in GUI thread
	SetTimer(ID_TIMER_UPDATE_GUI, 50, 0);
	return TRUE;
}

// When the application is started 3 times, position the windows on the screen without overlapping
void CSocketDemoDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	HANDLE h_Sema = CreateSemaphoreA(0, 0, 100, "ElmüSoft_SocketDemo");

	// For each application which is running at the same time, s32_Count is incremented by 1
	LONG s32_Count;
	ReleaseSemaphore(h_Sema, 1, &s32_Count);

	// Position the windows on the screen: one at the left and two at the right of the center
	// without overlapping. So the user can use the left window as server and the other two as clients.
	RECT  k_Rect;
	GetWindowRect(&k_Rect);

	LONG Xoff, Yoff;
	switch (s32_Count % 3)
	{
	case 0:
		Xoff = -238;
		Yoff = 0;
		break;
	case 1:
		Xoff = +238;
		Yoff = +204;
		break;
	case 2:
		Xoff = +238;
		Yoff = -204;
		break;
	}

	SetWindowPos(0, k_Rect.left + Xoff, k_Rect.top + Yoff, 0,0, SWP_NOZORDER | SWP_NOSIZE);
}

// -------------------------------------------------------------------------

// Switch this application into Server mode and listen for Client connections
void CSocketDemoDlg::OnBtnListen() 
{
	UpdateData(TRUE); // Load ms_Send and ms32_Port

	if (mi_Socket.GetSocketCount())
	{
		Print(_T("Socket already in use!"));
		return;
	}

	DWORD u32_BindIP = 0;
	int s32_Sel = mi_ComboBindTo.GetCurSel();
	if (s32_Sel > 0)
		u32_BindIP = mi_LocalIPs.GetValueByIndex(s32_Sel -1);

	CString s_BindIP;
	mi_ComboBindTo.GetWindowText(s_BindIP);

	DWORD u32_EventTimeout = (PROCESS_EVENTS_IN_GUI_THREAD) ? 50 : INFINITE;

	DWORD u32_Err = mi_Socket.Listen(u32_BindIP, ms32_Port, u32_EventTimeout, MAX_SERVER_IDLE_TIME);
	
	if (u32_Err) Print(_T("Listen Error %s"), GetErrMsg(u32_Err));
	else
	{
		Print(_T("Listening (%s) on Port %d.... (waiting for FD_ACCEPT)."), s_BindIP, ms32_Port);

		if (MAX_SERVER_IDLE_TIME > 0)
			Print(_T("Maximum idle time per client: %d seconds"), MAX_SERVER_IDLE_TIME);
	}
	
	if (u32_Err) 
	{
		CloseSockets();
		return;
	}

	// runs until an error occurred or all sockets have closed
	#if PROCESS_EVENTS_IN_GUI_THREAD
		ProcessEvents();
	#else
		DWORD u32_ID;
		mh_Thread = CreateThread(0, 0, ProcessEventThread, this, 0, &u32_ID);
	#endif
}

// Switch this application into Client mode and connect to a server
void CSocketDemoDlg::OnBtnConnect() 
{
	UpdateData(TRUE); // Load ms_Send and ms32_Port

	if (mi_Socket.GetSocketCount())
	{
		Print(_T("Socket already in use!"));
		return;
	}

	DWORD u32_IP;
	mi_IpAddr.GetAddress(u32_IP);
	u32_IP = htonl(u32_IP);

	DWORD u32_EventTimeout = (PROCESS_EVENTS_IN_GUI_THREAD) ? 50 : INFINITE;

	DWORD u32_Err = mi_Socket.ConnectTo(u32_IP, ms32_Port, u32_EventTimeout, MAX_CLIENT_IDLE_TIME);
	
	if (u32_Err) Print(_T("Connect Error %s"), GetErrMsg(u32_Err));
	else
	{
		Print(_T("Connecting to Server (%s) on Port %d.... (waiting for FD_CONNECT)"), FormatIP(u32_IP), ms32_Port);

		if (MAX_CLIENT_IDLE_TIME > 0)
			Print(_T("Maximum idle time: %d seconds"), MAX_CLIENT_IDLE_TIME);
	}

	if (u32_Err) 
	{
		CloseSockets();
		return;
	}

	// runs until an error occurred or all sockets have closed
	#if PROCESS_EVENTS_IN_GUI_THREAD
		ProcessEvents();
	#else
		DWORD u32_ID;
		mh_Thread = CreateThread(0, 0, ProcessEventThread, this, 0, &u32_ID);
	#endif
}

// static
ULONG WINAPI CSocketDemoDlg::ProcessEventThread(void* p_Param)
{
	CSocketDemoDlg* p_This = (CSocketDemoDlg*)p_Param;
	p_This->ProcessEvents();
	CloseHandle(p_This->mh_Thread);
	return 0;
}

// Process all events which occur on one of the open sockets
void CSocketDemoDlg::ProcessEvents()
{
	BOOL b_Server = (mi_Socket.GetState() & TCP::cSocket::E_Server);

	if (b_Server) SetWindowText(TITLE + _T(" - Server"));
	else          SetWindowText(TITLE + _T(" - Client"));

	while (TRUE) // Loop runs until the main window was closed or a severe error occurred
	{
		#if PROCESS_EVENTS_IN_GUI_THREAD
			PumpMessages();
		#endif

		TCP::cSocket::cMemory* pi_RecvMem;
		SOCKET  h_Socket;
		DWORD u32_Event, u32_IP, u32_Read, u32_Sent;
		DWORD u32_Err = mi_Socket.ProcessEvents(&u32_Event, &u32_IP, &h_Socket, &pi_RecvMem, &u32_Read,  &u32_Sent);
		
		// Main Dialog was closed -> !Immediately! stop all output and printing into GUI.
		// Otherwise the application will not shut down correctly and the EXE keeps running. (only visible in Task Manager)
		// There may appear a lot of other strange things when the Events thread still runs while the GUI thread already finished!
		if (mb_DlgClosed) 
			return;  // return NOT break!

		if (u32_Err == ERROR_TIMEOUT) // 50 ms interval has elapsed
			continue;

		CString s_Msg, s_Events;
		if (u32_Event) // ATTENTION: u32_Event may be == 0 -> do nothing.
		{
			if (b_Server) s_Events.Format(_T("Client %X (%s) --> "), h_Socket, FormatIP(u32_IP));
			else          s_Events.Format(_T("Server (%s) --> "), FormatIP(u32_IP));

			char s8_Events[200];
			mi_Socket.FormatEvents(u32_Event, s8_Events);
			s_Events += s8_Events;
		
			if (u32_Event & FD_READ)  s_Msg.Format(_T(" %d Bytes received."), u32_Read);
			if (u32_Event & FD_WRITE) s_Msg.Format(_T(" %d Bytes sent"),      u32_Sent);

			Print(s_Events + s_Msg);

			if (u32_Event & FD_READ && pi_RecvMem) // pi_RecvMem may be NULL if an error occurred!!
			{
				switch (me_DemoMode)
				{
					case E_NORMAL:   ProcessReceivedDataNormal(pi_RecvMem); break;
					case E_PREFIXED: ProcessReceivedDataPrefix(pi_RecvMem); break;
					case E_TELNET:   ProcessReceivedDataTelnet(pi_RecvMem); break;
				}
			}
		}

		// It is NOT necessary to update the Combobox after FD_READ or FD_WRITE
		mb_RefreshCombo |= (u32_Event & (FD_ACCEPT | FD_CONNECT | FD_CLOSE) || u32_Err);

		if (u32_Err)
		{
			// mi_Socket.Close() has been called -> don't print this error message
			if (u32_Err == WSAENOTCONN)
				break;

			// Print all the other error messages
			Print(_T("ProcessEvent Error %s"), GetErrMsg(u32_Err));
			
			// An error normally means that the socket has a problem -> abort the loop.
			// A few errors should not abort the processing:
			if (u32_Err != WSAECONNABORTED && // e.g. after the other side was killed in TaskManager 
				u32_Err != WSAECONNRESET   && // Connection reset by peer.
				u32_Err != WSAECONNREFUSED && // FD_ACCEPT with already 62 clients connected
				u32_Err != WSAESHUTDOWN)      // Sending data to a socket just in the short timespan 
				break;                        //   between shutdown() and closesocket()
		}
	}; // end loop

	CloseSockets();

	SetWindowText(TITLE);

	if (b_Server) Print(_T("Stop Listening.\r\n"));
	else          Print(_T("Connection abandoned.\r\n"));
}

// ##################################################################################################
//                                  PROCESS RECEIVED DATA
// ##################################################################################################

// Mode NORMAL:
// This simple "data processor" prints the data blocks immediately and unchanged as they arrive from the network
void CSocketDemoDlg::ProcessReceivedDataNormal(TCP::cSocket::cMemory* pi_RecvMem)
{
	char*  s8_Buf = pi_RecvMem->GetBuffer();
	DWORD u32_Len = pi_RecvMem->GetLength();

	CString s_String = CopyToString(s8_Buf, u32_Len);
	Print(_T("Received: '%s'"), s_String);

	// Delete all received data from the receive memory
	pi_RecvMem->DeleteLeft(u32_Len);
}

// Mode PREFIX:
// Each datablock comes prefixed with a DWORD which contains the total length of the datablock.
// So it is easy to determine if a block has been received completely.
// The data is accumulated in pi_RecvMem which works like a FIFO memory.
// This is the recommended principle for transmitting binary data.
// To test this mode set SEND_LARGE_DATA to 100 and set READ_BUFFER_SIZE to 30
void CSocketDemoDlg::ProcessReceivedDataPrefix(TCP::cSocket::cMemory* pi_RecvMem)
{
	while (TRUE) // There may arrive multiple datablocks at once -> loop until FIFO is empty
	{
		char*  s8_Buf = pi_RecvMem->GetBuffer();
		DWORD u32_Len = pi_RecvMem->GetLength();
		if (u32_Len < 4)
			return; // There must always be at least 1 Dword

		DWORD u32_Blocksize = ((DWORD*)s8_Buf)[0];
		if (u32_Blocksize > u32_Len)
		{
			Print(_T("%d Bytes in RecvMemory (Blocksize= %d Byte) Waiting for more data..."), u32_Len, u32_Blocksize);
			return; // The block is not yet complete -> accumulate more data in pi_RecvMem
		}

		CString s_String = CopyToString(s8_Buf+4, u32_Blocksize-4);
		Print(_T("Received entire datablock (%d Bytes): '%s'"), u32_Blocksize, s_String);

		// Only delete the data that has been processed and leave the rest.
		// ATTENTION: DeleteLeft(u32_Len) would result in data loss!!
		pi_RecvMem->DeleteLeft(u32_Blocksize);
	}
}

// Mode TELNET:
// This function demonstrates how single characters received from a Telnet client
// are accumulated in pi_RecvMem, which works like a FIFO memory, until a line feed is found.
// When a line is complete it is printed to the screen and deleted from pi_RecvMem.
void CSocketDemoDlg::ProcessReceivedDataTelnet(TCP::cSocket::cMemory* pi_RecvMem)
{
	#if _UNICODE
		Print(_T("Telnet does not use Unicode. Please compile the Telnet demo as MBCS!"));
		return;
	#endif;

	// If you send "Hello\nWorld\n" from SocketDemo instead of using a real Telnet client this requires a loop
	while (TRUE) 
	{
		char*  s8_Buf = pi_RecvMem->GetBuffer();
		DWORD u32_Len = pi_RecvMem->GetLength();

		CString s_String = CopyToString(s8_Buf, u32_Len);
		int s32_Pos = s_String.Find('\n');
		if (s32_Pos < 0)
		{
			Print(_T("%d Bytes in RecvMemory. Waiting for linefeed..."), u32_Len);
			return; // The line is not yet complete -> accumulate more characters
		}

		s_String = s_String.Left(s32_Pos);
		s_String.Replace(_T("\r"),_T(""));

		// Print all characters up to the "\n"
		Print(_T("Received entire line: '%s'"), s_String);

		// Delete all characters including the "\n" itself from the receive memory
		// but leave all characters in RecvMem which follow the "\n"
		pi_RecvMem->DeleteLeft(s32_Pos+1);
	};
}

// ##################################################################################################
//                                            END
// ##################################################################################################

// Close all open sockets
void CSocketDemoDlg::OnBtnClose() 
{
	if (!mi_Socket.GetSocketCount())
		Print(_T("No Socket open!"));
	else
		CloseSockets();
}

// Close all open sockets (if any)
void CSocketDemoDlg::CloseSockets() 
{
	if (mi_Socket.GetSocketCount())
	{
		mi_Socket.Close();
		Print(_T("Socket(s) closed."));

		mi_ComboSendTo.ResetContent();
	}
}

// Send a text string to one or multiple destinations
void CSocketDemoDlg::OnBtnSend() 
{
	UpdateData(TRUE); // Load ms_Send and ms32_Port

	if (!mi_ComboSendTo.GetCount())
	{
		Print(_T("Not connected!"));
		return;
	}

	#if SEND_LARGE_DATA > 0
		// SEND_LARGE_DATA = 100000 -> send a 100 Kilobyte string "AAAAAAAA...", each time with another character
		static TCHAR t_Chr = 'A';
		CString s_SendData(t_Chr++, SEND_LARGE_DATA/sizeof(TCHAR));
		if (t_Chr > 'Z') t_Chr = 'A';
	#else
		// send the string that the user has entered
		CString s_SendData = ms_Send;
	#endif

	if (!s_SendData.GetLength())
	{
		Print(_T("Error: You must enter a text!"));
		return;
	}

	int  s32_Sel = mi_ComboSendTo.GetCurSel();
	// Get the socket handle which is stored invisibly in the Combobox
	SOCKET h_Socket = (SOCKET)mi_ComboSendTo.GetItemData(s32_Sel);

	// Combobox index=0 on server -> Send to all connected Clients
	if (h_Socket==0 && (mi_Socket.GetState() & TCP::cSocket::E_Server))
	{
		for (DWORD i=0; i<mi_SocketList.GetCount(); i++)
		{
			h_Socket = mi_SocketList.GetKeyByIndex(i);
			if (!SendTo(h_Socket, s_SendData))
				break;
		}
	}
	else
	{
		SendTo(h_Socket, s_SendData);
	}
}

// Sends data to the given socket
// A "\r\n" in the input string is replaced with a linebreak
// returns FALSE when the sockets have been closed due to a severe error
BOOL CSocketDemoDlg::SendTo(SOCKET h_Socket, CString s_SendData) 
{
	CString s_Text = s_SendData;
	if (s_Text.GetLength() > 50)
		s_Text = s_Text.Left(50) + "...<cut>";

	s_SendData.Replace(_T("\\n"), _T("\n"));
	s_SendData.Replace(_T("\\r"), _T("\r"));

	// If Unicode: 1 character = 2 Bytes!
	DWORD u32_Len = s_SendData.GetLength() * sizeof(TCHAR);

	Print(_T("Sending %d Bytes to %s: '%s'"), u32_Len, FormatDisplayName(h_Socket), s_Text);

	// Insert a DWORD at the begin which contains the total length of the sent data
	if (me_DemoMode == E_PREFIXED)
	{
		// We need always 4 BYTES (=2 characters if Unicode, =4 characters if MBCS)
		CString s_Prefix('x', 4 / sizeof(TCHAR));
		s_SendData.Insert(0, s_Prefix); // insert "xx" or "xxxx"
	}

	char* s8_Data = (char*)(const TCHAR*)s_SendData; // get buffer AFTER Insert() !!!

	if (me_DemoMode == E_PREFIXED)
	{
		u32_Len += 4;                   // set to total length of datablock
		((DWORD*)s8_Data)[0] = u32_Len; // replace "xxxx" with the length of the send data block
	}

	DWORD u32_Err = mi_Socket.SendTo(h_Socket, s8_Data, u32_Len);

	switch (u32_Err)
	{
	case 0:
		return TRUE;

	case WSAEWOULDBLOCK:
		Print(_T("WSAEWOULDBLOCK -> The data will be send after the next FD_WRITE event."));
		return TRUE;

	case WSA_IO_PENDING:
		Print(_T("WSA_IO_PENDING -> Error: A previous Send operation is still pending. This data will not be sent."));
		return TRUE;

	default:
		Print(_T("%s"), _T(" -> Error ") + GetErrMsg(u32_Err));
		// Severe error -> abort event loop
		CloseSockets();
		return FALSE; 
	};
}

// Clear the content of the Output Editbox
void CSocketDemoDlg::OnBtnClear() 
{
	mi_Output.SetWindowText(_T(""));
}


// This function is called every 50 ms
// 1.) Fill the combobox with the currently possible destinations for a SendTo operation.
// 2.) Write ms_Output into the Output Edit box.
// ATTENTION:
// This function uses several SendMessage() (in GetCurSel(), ResetContent(), AddString(), SetCurSel(), SetWindowText())
// This function MUST be called ALWAYS from the GUI thread otherwise it deadlocks the worker thread!
// (SendMessage() would switch the thread context if the calling thread is not the GUI thread!)
void CSocketDemoDlg::OnTimer(UINT_PTR u32_TimerID) 
{
	CDialog::OnTimer(u32_TimerID);
	
	if (u32_TimerID != ID_TIMER_UPDATE_GUI)
		return;
	
	// --------- Update Combobox ----------

	if (mb_RefreshCombo)
	{
		mb_RefreshCombo = FALSE;

		int s32_Sel = mi_ComboSendTo.GetCurSel();

		mi_ComboSendTo.ResetContent();

		if (mi_Socket.GetState() & TCP::cSocket::E_Connected)
		{
			DWORD u32_Err = mi_Socket.GetAllConnectedSockets(&mi_SocketList);
			
			if (u32_Err) Print(_T("Error getting connected Sockets: %s"), GetErrMsg(u32_Err));

			DWORD u32_Count = mi_SocketList.GetCount();
			for (DWORD i=0; i<u32_Count; i++)
			{
				SOCKET h_Socket = mi_SocketList.GetKeyByIndex(i);
				mi_ComboSendTo.AddString(FormatDisplayName(h_Socket));
				// Store the socket handle invisibly in the combobox item's data
				mi_ComboSendTo.SetItemData(i, h_Socket);
			}

			if (mi_Socket.GetState() & TCP::cSocket::E_Server) 
			{
				mi_ComboSendTo.InsertString(0, _T("All Clients"));
				// Socket handle = 0 -> Send to all
				mi_ComboSendTo.SetItemData(0, 0);
			}

			// Maintain the current selection if possible
			mi_ComboSendTo.SetCurSel(max(0, min((int)u32_Count-1, s32_Sel)));
		}
	}

	// --------- Update Output Editbox ----------

	// The variable ms_Output is manipulated from two threads
	// The critical section assures thread safety
	EnterCriticalSection(&mk_Critical);
	
	CString s_Append = ms_Output;
	ms_Output.Empty();
	
	LeaveCriticalSection(&mk_Critical);
	
	if (s_Append.GetLength())
	{
		CString s_Text;
		mi_Output.GetWindowText(s_Text);

		s_Text += s_Append;

		mi_Output.SetWindowText(s_Text);

		// Scroll to the last line
		mi_Output.SetSel(s_Text.GetLength(), s_Text.GetLength());
	}
}

// Allows to update the GUI from within an endless loop without needing an extra thread
void CSocketDemoDlg::PumpMessages()
{
	MSG k_Msg;
	while (PeekMessage(&k_Msg, NULL, NULL, NULL, PM_NOREMOVE))
	{
		AfxGetThread()->PumpMessage();
	}
}

// When the main dialog is closed: set the mb_DlgClosed flag to abort the ProcessEvents() Thread!
void CSocketDemoDlg::OnClose()
{
	mb_DlgClosed = TRUE;
	mi_Socket.Close();
	CDialog::OnClose();
}

// About the demo modes read the comment for eDemoMode!
void CSocketDemoDlg::OnSelchangeComboDemoType() 
{
	mi_Output.SetWindowText(_T(""));

	me_DemoMode = (eDemoMode) mi_ComboDemoMode.GetCurSel();
	mi_Socket.Close();

	if (me_DemoMode == E_TELNET)
	{
		UpdateData(TRUE);
		ms32_Port = 23;
		UpdateData(FALSE);
	}
}

// ----------------------------------------------------------------
// -------------------------- HELPER ------------------------------
// ----------------------------------------------------------------

// Appends formatted text to the string ms_Output which is later written to the Output Editbox in the GUI thread
void CSocketDemoDlg::Print(CString s_Format, ...)
{
	va_list  args;
	va_start(args, s_Format);

	int BUFLEN = 50000;

	CString s_Out;
	TCHAR*  t_Out = s_Out.GetBuffer(BUFLEN+1);

	_vsntprintf(t_Out, BUFLEN, s_Format, args);

	// If the new line should be longer than BUFLEN it is cropped.
	t_Out[BUFLEN] = 0;
	s_Out.ReleaseBuffer();
	
	if (s_Out.GetLength() == BUFLEN) 
		s_Out += _T("...<cut>");

	// The edit box does not display a single "\n" correctly. It requires always "\r\n"
	s_Out.Replace(_T("\r"), _T(""));
	s_Out.Replace(_T("\n"), _T("\r\n"));
	s_Out += _T("\r\n");

	// The variable ms_Output is manipulated from two threads
	// The critical section assures thread safety
	EnterCriticalSection(&mk_Critical);
	ms_Output += s_Out;
	LeaveCriticalSection(&mk_Critical);
}

// Copies the not zero terminated data in s8_Buf into a CString
// u32_Bytes always specifies the length in bytes no matter if compiled as Unicode or MBCS
CString CSocketDemoDlg::CopyToString(char* s8_Buf, DWORD u32_Bytes)
{
	DWORD u32_StrLen = u32_Bytes / sizeof(TCHAR);
	
	CString s_String;
	char*  s8_String = (char*)s_String.GetBuffer(u32_StrLen+1);
	memcpy(s8_String, s8_Buf, u32_Bytes);
	s_String.ReleaseBuffer(u32_StrLen);

	return s_String;
}

// Format the display string for the given socket
// returns "Server (192.168.1.100)" or "Client 71C (192.168.1.100)"
CString CSocketDemoDlg::FormatDisplayName(SOCKET h_Socket)
{
	CString s_IP = FormatIP(mi_SocketList.GetValueByKey(h_Socket));
	CString s_Disp;
	if (mi_Socket.GetState() & TCP::cSocket::E_Server) 
		s_Disp.Format(_T("Client %X (%s)"), h_Socket, s_IP);
	else
		s_Disp.Format(_T("Server (%s)"), s_IP);

	return s_Disp;
}

// Formats an IP address "192.168.1.100"
CString CSocketDemoDlg::FormatIP(DWORD u32_IP)
{
	BYTE* pu8_Addr = (BYTE*)&u32_IP;

	CString s_IP;
	s_IP.Format(_T("%d.%d.%d.%d"), pu8_Addr[0],pu8_Addr[1],pu8_Addr[2],pu8_Addr[3]);
	return s_IP;
}

// Get a human readable error message for an API error code
CString CSocketDemoDlg::GetErrMsg(DWORD u32_Error)
{
	// Some translations of error codes are really stupid --> show the original error code.
	CString s_Code;
	switch (u32_Error)
	{
		case WSAEINTR:                s_Code = _T("WSAEINTR"); break;
		case WSAEBADF:                s_Code = _T("WSAEBADF"); break;
		case WSAEACCES:               s_Code = _T("WSAEACCES"); break;
		case WSAEFAULT:               s_Code = _T("WSAEFAULT"); break;
		case WSAEINVAL:               s_Code = _T("WSAEINVAL"); break;
		case WSAEMFILE:               s_Code = _T("WSAEMFILE"); break;
		case WSAEWOULDBLOCK:          s_Code = _T("WSAEWOULDBLOCK"); break;
		case WSAEINPROGRESS:          s_Code = _T("WSAEINPROGRESS"); break;
		case WSAEALREADY:             s_Code = _T("WSAEALREADY"); break;
		case WSAENOTSOCK:             s_Code = _T("WSAENOTSOCK"); break;
		case WSAEDESTADDRREQ:         s_Code = _T("WSAEDESTADDRREQ"); break;
		case WSAEMSGSIZE:             s_Code = _T("WSAEMSGSIZE"); break;
		case WSAEPROTOTYPE:           s_Code = _T("WSAEPROTOTYPE"); break;
		case WSAENOPROTOOPT:          s_Code = _T("WSAENOPROTOOPT"); break;
		case WSAEPROTONOSUPPORT:      s_Code = _T("WSAEPROTONOSUPPORT"); break;
		case WSAESOCKTNOSUPPORT:      s_Code = _T("WSAESOCKTNOSUPPORT"); break;
		case WSAEOPNOTSUPP:           s_Code = _T("WSAEOPNOTSUPP"); break;
		case WSAEPFNOSUPPORT:         s_Code = _T("WSAEPFNOSUPPORT"); break;
		case WSAEAFNOSUPPORT:         s_Code = _T("WSAEAFNOSUPPORT"); break;
		case WSAEADDRINUSE:           s_Code = _T("WSAEADDRINUSE"); break;
		case WSAEADDRNOTAVAIL:        s_Code = _T("WSAEADDRNOTAVAIL"); break;
		case WSAENETDOWN:             s_Code = _T("WSAENETDOWN"); break;
		case WSAENETUNREACH:          s_Code = _T("WSAENETUNREACH"); break;
		case WSAENETRESET:            s_Code = _T("WSAENETRESET"); break;
		case WSAECONNABORTED:         s_Code = _T("WSAECONNABORTED"); break;
		case WSAECONNRESET:           s_Code = _T("WSAECONNRESET"); break;
		case WSAENOBUFS:              s_Code = _T("WSAENOBUFS"); break;
		case WSAEISCONN:              s_Code = _T("WSAEISCONN"); break;
		case WSAENOTCONN:             s_Code = _T("WSAENOTCONN"); break;
		case WSAESHUTDOWN:            s_Code = _T("WSAESHUTDOWN"); break;
		case WSAETOOMANYREFS:         s_Code = _T("WSAETOOMANYREFS"); break;
		case WSAETIMEDOUT:            s_Code = _T("WSAETIMEDOUT"); break;
		case WSAECONNREFUSED:         s_Code = _T("WSAECONNREFUSED"); break;
		case WSAELOOP:                s_Code = _T("WSAELOOP"); break;
		case WSAENAMETOOLONG:         s_Code = _T("WSAENAMETOOLONG"); break;
		case WSAEHOSTDOWN:            s_Code = _T("WSAEHOSTDOWN"); break;
		case WSAEHOSTUNREACH:         s_Code = _T("WSAEHOSTUNREACH"); break;
		case WSAENOTEMPTY:            s_Code = _T("WSAENOTEMPTY"); break;
		case WSAEPROCLIM:             s_Code = _T("WSAEPROCLIM"); break;
		case WSAEUSERS:               s_Code = _T("WSAEUSERS"); break;
		case WSAEDQUOT:               s_Code = _T("WSAEDQUOT"); break;
		case WSAESTALE:               s_Code = _T("WSAESTALE"); break;
		case WSAEREMOTE:              s_Code = _T("WSAEREMOTE"); break;
		case WSASYSNOTREADY:          s_Code = _T("WSASYSNOTREADY"); break;
		case WSAVERNOTSUPPORTED:      s_Code = _T("WSAVERNOTSUPPORTED"); break;
		case WSANOTINITIALISED:       s_Code = _T("WSANOTINITIALISED"); break;
		case WSAEDISCON:              s_Code = _T("WSAEDISCON"); break;
		case WSAENOMORE:              s_Code = _T("WSAENOMORE"); break;
		case WSAECANCELLED:           s_Code = _T("WSAECANCELLED"); break;
		case WSAEINVALIDPROCTABLE:    s_Code = _T("WSAEINVALIDPROCTABLE"); break;
		case WSAEINVALIDPROVIDER:     s_Code = _T("WSAEINVALIDPROVIDER"); break;
		case WSAEPROVIDERFAILEDINIT:  s_Code = _T("WSAEPROVIDERFAILEDINIT"); break;
		case WSASYSCALLFAILURE:       s_Code = _T("WSASYSCALLFAILURE"); break;
		case WSASERVICE_NOT_FOUND:    s_Code = _T("WSASERVICE_NOT_FOUND"); break;
		case WSATYPE_NOT_FOUND:       s_Code = _T("WSATYPE_NOT_FOUND"); break;
		case WSA_E_NO_MORE:           s_Code = _T("WSA_E_NO_MORE"); break;
		case WSA_E_CANCELLED:         s_Code = _T("WSA_E_CANCELLED"); break;
		case WSAEREFUSED:             s_Code = _T("WSAEREFUSED"); break;
		case WSAHOST_NOT_FOUND:       s_Code = _T("WSAHOST_NOT_FOUND"); break;
		case WSATRY_AGAIN:            s_Code = _T("WSATRY_AGAIN"); break;
		case WSANO_RECOVERY:          s_Code = _T("WSANO_RECOVERY"); break;
		case WSANO_DATA:              s_Code = _T("WSANO_DATA"); break;
		case WSA_IO_PENDING:          s_Code = _T("WSA_IO_PENDING"); break;
		case WSA_IO_INCOMPLETE:       s_Code = _T("WSA_IO_INCOMPLETE"); break;
		case WSA_INVALID_HANDLE:      s_Code = _T("WSA_INVALID_HANDLE"); break;
		case WSA_INVALID_PARAMETER:   s_Code = _T("WSA_INVALID_PARAMETER"); break;
		case WSA_NOT_ENOUGH_MEMORY:   s_Code = _T("WSA_NOT_ENOUGH_MEMORY"); break;
		case WSA_OPERATION_ABORTED:   s_Code = _T("WSA_OPERATION_ABORTED"); break;
		
		default:
			s_Code.Format(_T("Code %u"), u32_Error); 
			break;
	}

	CString s_Out;
	const DWORD BUFLEN = 1000;
	TCHAR t_Buf[BUFLEN];

	if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, u32_Error, 0, t_Buf, BUFLEN, 0))
		s_Out.Format(_T("%s: %s"), s_Code, t_Buf);
	else 
		s_Out.Format(_T("%s: Windows has no explanation for this error"), s_Code);

	s_Out.TrimRight(); // some messages end with useless Linefeeds
	return s_Out;
}


