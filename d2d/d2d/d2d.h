
// d2d.h : d2d 應用程式的主標頭檔
//
#pragma once

#ifndef __AFXWIN_H__
	#error "對 PCH 包含此檔案前先包含 'stdafx.h'"
#endif

#include "resource.h"       // 主要符號


// Cd2dApp:
// 請參閱實作此類別的 d2d.cpp
//

class Cd2dApp : public CWinAppEx
{
public:
	Cd2dApp();


// 覆寫
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 程式碼實作

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cd2dApp theApp;
