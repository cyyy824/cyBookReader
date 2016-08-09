#include "stdafx.h"
#include "CyBookReaderConfig.h"


CcyBookReaderConfig::CcyBookReaderConfig()
{
	m_cx=800;
	m_cy=600;
	m_fontH=14;
	m_vs=14;//ÐÐ¾à
	m_font = "ËÎÌå";
	m_color= RGB(255,255,255);
	m_fontColor= RGB(0,0,0);
	m_issplit=0;
}


CcyBookReaderConfig::~CcyBookReaderConfig()
{
}

bool CcyBookReaderConfig::Update(CString filename)
{
	CString wstr;
	CString hstr;
	CString fhstr;
	CString fcstr;
	CString bcstr;
	CString fstr;
	CString splitstr;
	CString vsstr;

	wstr.Format(_T("%d"), m_cx);
	hstr.Format(_T("%d"), m_cy);
	fhstr.Format(_T("%d"), m_fontH);
	fcstr.Format(_T("%d,%d,%d"), GetRValue(m_fontColor), GetGValue(m_fontColor), GetBValue(m_fontColor));
	bcstr.Format(_T("%d,%d,%d"), GetRValue(m_color), GetGValue(m_color), GetBValue(m_color));
	//	fstr.Format(_T("%d"), m_cx);
	splitstr.Format(_T("%d"), m_issplit);
	vsstr.Format(_T("%d"), m_vs);

	WritePrivateProfileString(_T("Config"), _T("width"), wstr, filename);
	WritePrivateProfileString(_T("Config"), _T("height"), hstr, filename);
	WritePrivateProfileString(_T("Config"), _T("fontH"), fhstr, filename);
	WritePrivateProfileString(_T("Config"), _T("rowspace"), vsstr, filename);
	WritePrivateProfileString(_T("Config"), _T("split"), splitstr, filename);
	WritePrivateProfileString(_T("Config"), _T("fontColor"), fcstr, filename);
	WritePrivateProfileString(_T("Config"), _T("BGColor"), bcstr, filename);
	WritePrivateProfileString(_T("Config"), _T("font"), m_font, filename);
	WritePrivateProfileString(_T("Config"), _T("split"), splitstr, filename);
	return 0;
}

bool CcyBookReaderConfig::Read(CString filename)
{
	CString fcstr;
	CString bcstr;
	CString wstr;
	int r, g, b;

	GetPrivateProfileString(_T("Config"), _T("width"), _T(""), wstr.GetBuffer(5), 5, filename);
	if (wstr==_T(""))
	{
		Update(filename);
//		GetPrivateProfileString(_T("Config"), _T("width"), _T("0"), wstr.GetBuffer(5), 5, filename);
	}
	m_cx = GetPrivateProfileInt(_T("Config"), _T("width"), 0, filename);
	m_cy = GetPrivateProfileInt(_T("Config"), _T("height"), 0, filename);
	m_fontH = GetPrivateProfileInt(_T("Config"), _T("fontH"), 0, filename);
	m_vs = GetPrivateProfileInt(_T("Config"), _T("rowspace"), 0, filename);
	GetPrivateProfileString(_T("Config"), _T("fontColor"), _T("0"), fcstr.GetBuffer(14), 14, filename);
	GetPrivateProfileString(_T("Config"), _T("BGColor"), _T("0"), bcstr.GetBuffer(14), 14, filename);
	GetPrivateProfileString(_T("Config"), _T("font"), _T("ËÎÌå"), m_font.GetBuffer(20), 20, filename);
	m_issplit = GetPrivateProfileInt(_T("Config"), _T("split"), 0, filename); 
	swscanf(fcstr, _T("%d,%d,%d"), &r, &g, &b);
	m_fontColor = RGB(r, g, b);

	swscanf(bcstr, _T("%d,%d,%d"), &r, &g, &b);
	m_color = RGB(r, g, b);

	return 0;
}

void CcyBookReaderConfig::GetSize(CSize &size)
{
	size.cx = this->m_cx;
	size.cy = this->m_cy;
}
void CcyBookReaderConfig::SetSize(CSize size)
{
	this->m_cx = size.cx;
	this->m_cy = size.cy;
}