
// cyBookReader.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CcyBookReaderApp:
// �йش����ʵ�֣������ cyBookReader.cpp
//

class CcyBookReaderApp : public CWinApp
{
public:
	CcyBookReaderApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CcyBookReaderApp theApp;