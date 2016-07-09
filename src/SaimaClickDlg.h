// SaimaClickDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CSaimaClickDlg dialog
class CSaimaClickDlg : public CDialog
{
// Construction
public:
	CSaimaClickDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SAIMACLICK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

public:
	CString m_strLog;
	CEdit m_ctlLog;


protected:
	void Log(LPCTSTR what);
	void StartCheck();

	bool DoFindWindow(const std::wstring & section);
	bool DoCheckPixelColor(const std::wstring & section);
	bool DoMouseClick(const std::wstring & section);
	bool DoPause(const std::wstring & section);
	bool DoQuit(const std::wstring & section);


protected:
	CWnd * m_foundWindow;
	bool m_bQuit;

protected:
	int m_nCount;
	COleDateTime m_dtLastCheck;
	FileUtils m_fileUtils;
};
