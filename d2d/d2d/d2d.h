
// d2d.h : d2d ���ε{�����D���Y��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"       // �D�n�Ÿ�


// Cd2dApp:
// �аѾ\��@�����O�� d2d.cpp
//

class Cd2dApp : public CWinAppEx
{
public:
	Cd2dApp();


// �мg
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �{���X��@

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cd2dApp theApp;
