#pragma once
#include<string>
#include<vector>
// CTextReaderWnd


class CTextReaderWnd : public CWnd
{
	DECLARE_DYNAMIC(CTextReaderWnd)

public:
	CTextReaderWnd();
	virtual ~CTextReaderWnd();

protected:
	DECLARE_MESSAGE_MAP()
	void OnPaint();

public:
	void SetText(const wchar_t* ptext,int length);
	int CalculatePage(std::string text);
	void ShowText(int pageNum);
	int GetPageNum(){return m_pageNum;}
	int GetCurPageNum(){return m_curPage;}
	void SetVS(int vs){m_vs=vs;}
	int GetVS(){return m_vs;}
	void Init();
	void ReDraw();

	COLORREF GetFontColor(){return m_fontColor;}
	void SetFontColor(COLORREF color){m_fontColor=color;}
	
private:
	void split();


public:
	enum TShowMode
	{
		EListMode,
		EPageMode,
	};
	struct STR_LOC
	{
		int start;
		int length;
	};
private:
	int m_fontH;
	int m_chiW;
	int m_ascW;
	int m_pageNum;
	int m_curPage;
	int m_vs;//ÐÐ¾à
	COLORREF m_color;
	COLORREF m_fontColor;
	CDC m_MemDC;
	CBitmap m_MemBitmap;
	CBitmap *m_pOldBitmap;
	const wchar_t* m_ptext;
	int m_textLen;
	std::vector<STR_LOC> m_slList;

public:

};


