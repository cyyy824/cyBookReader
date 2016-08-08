#pragma once

#define CONFIGFILE "./config.ini"

class CcyBookReaderConfig
{
public:
	CcyBookReaderConfig();
	~CcyBookReaderConfig();
public:
	bool Update(CString filename = _T(CONFIGFILE));
	bool Read(CString filename = _T(CONFIGFILE));

	void GetSize(CSize &size);
	void SetSize(CSize size);

private:

public:
	int m_cx;
	int m_cy;
	int m_fontH;
	int m_vs;//ÐÐ¾à
	CString m_font;
	int m_color;
	int m_fontColor;
	int m_issplit;
};

