#pragma once
#include<string>
#include<vector>
#include<map>
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

	void SetBgColor(COLORREF color) { m_color = color; }
	COLORREF GetBGColor() { return m_color; }
	COLORREF GetFontColor(){return m_fontColor;}
	void SetFontColor(COLORREF color){m_fontColor=color;}
	
private:
	void split();
	void split1();
	LONG GetAscW(USHORT ch);

public:
	enum TShowMode
	{
		EListMode,
		EPageMode,
	};
	enum ELocType 
	{
		ELocNone=0,
		ELocEnter,
	};
	struct STR_LOC
	{
		ELocType type;
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
	int m_preHeight;
	int m_preFontH;
	int m_preVs;
	COLORREF m_color;
	COLORREF m_fontColor;
	CDC m_MemDC;
	CBitmap m_MemBitmap;
	CBitmap *m_pOldBitmap;
	const wchar_t* m_ptext;
	int m_textLen;
	std::vector<STR_LOC> m_slList;
	std::map<char, LONG> m_ascMap;

public:

};


