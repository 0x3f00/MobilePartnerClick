// SaimaClickDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "SaimaClick.h"
#include "SaimaClickDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define CHECK_PERIOD 10
#define CHECK_TIMER 444

// CSaimaClickDlg dialog


CSaimaClickDlg::CSaimaClickDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaimaClickDlg::IDD, pParent)
	, m_strLog(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nCount = 0;
	m_dtLastCheck = COleDateTime::GetCurrentTime();
	m_foundWindow = nullptr;
	m_bQuit = false;
}

void CSaimaClickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strLog);
	DDX_Control(pDX, IDC_EDIT1, m_ctlLog);
}

BEGIN_MESSAGE_MAP(CSaimaClickDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSaimaClickDlg message handlers

BOOL CSaimaClickDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetTimer(CHECK_TIMER, CHECK_PERIOD, nullptr);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	

	return TRUE;  // return TRUE  unless you set the focus to a control
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSaimaClickDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSaimaClickDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSaimaClickDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == CHECK_TIMER)
	{
		COleDateTime dtNow = COleDateTime::GetCurrentTime();
		COleDateTimeSpan diff = dtNow - m_dtLastCheck;
		if(diff.GetTotalSeconds() < CHECK_PERIOD)
			return;

		m_dtLastCheck = dtNow;

		StartCheck();
		m_dtLastCheck = COleDateTime::GetCurrentTime();

		if (m_bQuit)
			CDialog::OnOK();

		return;
	}

	CDialog::OnTimer(nIDEvent);
}


void CSaimaClickDlg::StartCheck()
{
	Log(L"Start Checking...");


	std::wstring commands = m_fileUtils.GetIniValue(L"commands");
	std::vector<std::wstring> arr = my_utils::wtokenize(commands, L",;");

	for (size_t i = 0; i < arr.size(); i++)
	{
		bool bSuccess = false;

		std::wstring cmd = m_fileUtils.GetIniValue(arr[i] + L"_Cmd");

		if (cmd == L"findWindow")
		{
			bSuccess = DoFindWindow(arr[i]);
		}
		else if (cmd == L"checkPixel")
		{
			bSuccess = DoCheckPixelColor(arr[i]);
		}
		else if (cmd == L"mouseClick")
		{
			bSuccess = DoMouseClick(arr[i]);
		}
		else if (cmd == L"pause")
		{
			bSuccess = DoPause(arr[i]);
		}
		else if (cmd == L"quit")
		{
			bSuccess = DoQuit(arr[i]);
		}

		if (!bSuccess)
			break;
	}
}

void CSaimaClickDlg::Log(LPCTSTR what)
{
	m_nCount++;
	CString dummy;
	dummy.Format(L"%d: ", m_nCount);
	m_strLog += dummy;
	m_strLog += what;
	m_strLog += "\r\n";

	UpdateData(FALSE);
	m_ctlLog.LineScroll(100000, 0);
}



bool CSaimaClickDlg::DoFindWindow(const std::wstring & section)
{
	bool bRes = true;
	
	std::wstring name = m_fileUtils.GetIniValue(section + L"_Name");

	m_foundWindow = FindWindow(NULL, name.c_str());
	if (!m_foundWindow)
	{
		Log(L"Not Found!");
		bRes = false;
	}

	return bRes;
}

bool CSaimaClickDlg::DoCheckPixelColor(const std::wstring & section)
{
	bool bRes = false;
	if (!m_foundWindow)
		return bRes;

	int x;
	int y;
	COLORREF color;

	std::wstring str;
	str = m_fileUtils.GetIniValue(section + L"_X");
	swscanf_s(str.c_str(), L"%d", &x);

	str = m_fileUtils.GetIniValue(section + L"_Y");
	swscanf_s(str.c_str(), L"%d", &y);

	CString strCoords; strCoords.Format(L"Coords = %d,%d", x, y);
	Log(strCoords);

	RECT rc;
	HWND hwnd = m_foundWindow->GetSafeHwnd();
	::BringWindowToTop(hwnd);
	::GetClientRect(hwnd, &rc);

	//create
	HDC hdcScreen = ::GetDC(NULL);
	HDC hdc = ::CreateCompatibleDC(hdcScreen);
	HBITMAP hbmp = ::CreateCompatibleBitmap(hdcScreen,
		rc.right - rc.left, rc.bottom - rc.top);
	::SelectObject(hdc, hbmp);
	::PrintWindow(hwnd, hdc, PW_CLIENTONLY);

	color = GetPixel(hdc, x, y);
	CString strColor; 
	strColor.Format(L"%02X%02X%02X", GetRValue(color), GetGValue(color), GetBValue(color));
	Log(strColor);


	int max_r;
	int min_r;
	int max_g;
	int min_g;
	int max_b;
	int min_b;

	str = m_fileUtils.GetIniValue(section + L"_MaxR");
	swscanf_s(str.c_str(), L"%d", &max_r);
	str = m_fileUtils.GetIniValue(section + L"_MinR");
	swscanf_s(str.c_str(), L"%d", &min_r);
	str = m_fileUtils.GetIniValue(section + L"_MaxG");
	swscanf_s(str.c_str(), L"%d", &max_g);
	str = m_fileUtils.GetIniValue(section + L"_MinG");
	swscanf_s(str.c_str(), L"%d", &min_g);
	str = m_fileUtils.GetIniValue(section + L"_MaxB");
	swscanf_s(str.c_str(), L"%d", &max_b);
	str = m_fileUtils.GetIniValue(section + L"_MinB");
	swscanf_s(str.c_str(), L"%d", &min_b);


	if ((GetRValue(color) > min_r) &&
		(GetRValue(color) < max_r) &&
		(GetGValue(color) > min_g) &&
		(GetGValue(color) < max_g) &&
		(GetBValue(color) > min_b) &&
		(GetBValue(color) < max_b))
	{
		bRes = true;
	}

	return bRes;
}

bool CSaimaClickDlg::DoMouseClick(const std::wstring & section)
{
	bool bRes = false;
	if (!m_foundWindow)
		return bRes;

	int dx;
	int dy;

	CString dummy;
	std::wstring str;
	str = m_fileUtils.GetIniValue(section + L"_DX");
	swscanf_s(str.c_str(), L"%d", &dx);
	str = m_fileUtils.GetIniValue(section + L"_DY");
	swscanf_s(str.c_str(), L"%d", &dy);
	dummy.Format(L"Click = %d %d", dx, dy);
	Log(dummy);

	RECT rc;
	HWND hwnd = m_foundWindow->GetSafeHwnd();
	::GetWindowRect(hwnd, &rc);
	dummy.Format(L"%d %d %d %d", rc.left, rc.top, rc.right, rc.bottom);
	Log(dummy);
	INPUT arr[3];
	memset(arr, 0, sizeof(arr));

	DWORD x = rc.left + dx;
	DWORD y = rc.top + dy;
	arr[0].type = INPUT_MOUSE;
	arr[0].mi.dx = x*(65536 / GetSystemMetrics(SM_CXSCREEN));
	arr[0].mi.dy = y*(65536 / GetSystemMetrics(SM_CYSCREEN));
	arr[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

	arr[1].type = INPUT_MOUSE;
	arr[1].mi.dx = x*(65536 / GetSystemMetrics(SM_CXSCREEN));
	arr[1].mi.dy = y*(65536 / GetSystemMetrics(SM_CYSCREEN));
	arr[1].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;


	arr[2].type = INPUT_MOUSE;
	arr[2].mi.dx = x*(65536 / GetSystemMetrics(SM_CXSCREEN));
	arr[2].mi.dy = y*(65536 / GetSystemMetrics(SM_CYSCREEN));
	arr[2].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;

	UINT res = SendInput(3, arr, sizeof(INPUT));

	bRes = (res == 3);
	return bRes;
}

bool CSaimaClickDlg::DoPause(const std::wstring & section)
{
	bool bRes = false;

	int n = 0;

	std::wstring str;
	str = m_fileUtils.GetIniValue(section + L"_N");
	swscanf_s(str.c_str(), L"%d", &n);
	CString dummy; dummy.Format(L"Sleep = %d ms", n);
	Log(dummy);

	Sleep(n);
	bRes = true;

	return bRes;
}


bool CSaimaClickDlg::DoQuit(const std::wstring & section)
{
	bool bRes = false;
	m_bQuit = true;
	bRes = true;
	return bRes;
}
