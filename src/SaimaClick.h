// SaimaClick.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

// CSaimaClickApp:
// See SaimaClick.cpp for the implementation of this class
//

class CSaimaClickApp : public CWinApp
{
public:
	CSaimaClickApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSaimaClickApp theApp;