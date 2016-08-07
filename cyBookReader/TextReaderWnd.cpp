// TextReaderWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "cyBookReader.h"
#include "TextReaderWnd.h"


// CTextReaderWnd

IMPLEMENT_DYNAMIC(CTextReaderWnd, CWnd)

CTextReaderWnd::CTextReaderWnd()
{
	m_vs = 0;
	m_fontH = 0;
	m_ascW = 0;
	m_chiW = 0;
	m_pageNum=0;
	m_textLen=0;
	m_curPage = 0;
	m_preHeight = 0;
	m_preFontH = 0;
	m_preVs = 0;
	m_color = RGB(255,255,255);
	m_fontColor = RGB(0,0,0);
	
}

CTextReaderWnd::~CTextReaderWnd()
{
	m_MemBitmap.DeleteObject();
	m_MemDC.DeleteDC();
}


BEGIN_MESSAGE_MAP(CTextReaderWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CTextReaderWnd 消息处理程序

void CTextReaderWnd::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	dc.BitBlt(0,0,rect.Width(),rect.Height(),&m_MemDC,0,0,SRCCOPY);

}

void CTextReaderWnd::ShowText(int pageNum)
{
	CRect rect;
	GetClientRect(&rect);

	m_MemDC.FillSolidRect(0,0,rect.Width(),rect.Height(),m_color);
 
	CRect lineRect;
	lineRect.top = 5;
	lineRect.left = 5;
	lineRect.bottom = m_fontH+5+m_vs;
	lineRect.right = rect.Width()-5;

	//计算页位置
	int PageLineNum = (rect.Height()-8) / (m_fontH+m_vs);
	int sline = pageNum*PageLineNum;

	m_MemDC.SetTextColor(m_fontColor);
	
	for(int i=0;i<PageLineNum;i++)
	{
		CString ab;
		if( i+sline >=m_slList.size()) break;

		if (m_slList[i + sline].type == ELocEnter)
		{
			m_MemDC.DrawTextExW(_T("\n"), &lineRect, DT_LEFT, NULL);

			//		m_MemDC.Rectangle(&lineRect);

			lineRect.OffsetRect(0, (m_fontH + m_vs));
		}
//		wchar_t* pstr = new wchar_t[m_slList.at(i+sline).length+1];
//		wcsncpy(pstr, m_ptext+m_slList.at(i+sline).start, m_slList.at(i+sline).length);
		wchar_t* pstr = new wchar_t[m_slList[i + sline].length + 1];
		wcsncpy(pstr, m_ptext+m_slList[i+sline].start, m_slList[i+sline].length);
		*(pstr+m_slList[i+sline].length)=L'\0';
		ab= pstr;
		CSize s2 = m_MemDC.GetTextExtent(ab);
		m_MemDC.DrawTextExW(ab, &lineRect, DT_LEFT, NULL);

//		m_MemDC.Rectangle(&lineRect);

		lineRect.OffsetRect(0,(m_fontH+m_vs));
		delete []pstr;
	}

	m_curPage = pageNum;
//	m_MemDC.SelectObject(m_pOldBitmap);
	Invalidate();
}

void CTextReaderWnd::split1()
{
	CRect rect;
	GetClientRect(&rect);
	if (!m_preHeight) m_preHeight = rect.Height();
	if (!m_preFontH) m_preFontH = this->m_fontH;
	if (!m_preVs) m_preVs = m_vs;
	int tPageLineNum = (m_preHeight - 8) / (m_preFontH + m_vs);
	int tsline = m_curPage*tPageLineNum;
	int slen=0;
	if(m_curPage) slen=m_slList.at(tsline).start;

	int i = 0;
	int len = m_textLen;
	int nline = 0;
	int linew = 0;

	
	int width = rect.Width() - 5 - 5;
	int height = rect.Height();
	int PageLineNum = (height - 8) / (m_fontH + m_vs);
	
	const wchar_t* pContent = m_ptext;
	const wchar_t* pStr = pContent;

	m_slList.clear();

	for (i=0; i<slen-1; i++)
	{
		USHORT ch = *((USHORT*)(pContent + i));
		wchar_t ch1 = *((USHORT*)(pContent + i));
		if (ch == 0x0a)
		{// '\n'
			int len1 = pContent + i - pStr + 1;
			//	CStringW str;
			STR_LOC sl;
			sl.start = pStr - pContent;
			sl.length = len1;
			this->m_slList.push_back(sl);

			pStr = pContent + i + 1;


			nline++;
			linew = 0;
		}
		else if (ch == 0x0d)
		{// '\r'

		}
		else if (ch <= 255)
		{//ASCII
		 //	linew += m_ascW;
			linew += GetAscW(ch);
		}
		else
		{//CHINESE
			linew += m_chiW;
		}

		if (linew >= width - m_chiW)
		{
			int len1 = pContent + i - pStr + 1;
			//	CStringW str;
			STR_LOC sl;
			sl.start = pStr - pContent;
			sl.length = len1;
			this->m_slList.push_back(sl);

			pStr = pContent + i + 1;

			linew = 0;
			nline++;
		}

	}
	if (linew > 0)
	{
		int len1 = pContent + i - pStr + 1;
		STR_LOC sl;
		sl.type = ELocNone;
		sl.start = pStr - pContent;
		sl.length = len1;
		this->m_slList.push_back(sl);

		pStr = pContent + i + 1;

		linew = 0;
		nline++;
	}

	int cline = m_slList.size();
	int mln = cline==0?0:PageLineNum - m_slList.size() % PageLineNum;
	std::vector<STR_LOC>::iterator it = m_slList.begin();
	STR_LOC sl;
	sl.type = ELocEnter;
	sl.start = 0;
	sl.length = 0;
	m_slList.insert(it,mln,sl);

	m_pageNum = m_slList.size() / PageLineNum + (m_slList.size() % PageLineNum>0 ? 1 : 0);

	for (i = slen; i<len; i++)
	{
		USHORT ch = *((USHORT*)(pContent + i));
		wchar_t ch1 = *((USHORT*)(pContent + i));
		if (ch == 0x0a)
		{// '\n'
			int len1 = pContent + i - pStr + 1;
			//	CStringW str;
			STR_LOC sl;
			sl.type = ELocNone;
			sl.start = pStr - pContent;
			sl.length = len1;
			this->m_slList.push_back(sl);

			pStr = pContent + i + 1;


			nline++;
			linew = 0;
		}
		else if (ch == 0x0d)
		{// '\r'

		}
		else if (ch <= 255)
		{//ASCII
		 //	linew += m_ascW;
			linew += GetAscW(ch);
		}
		else
		{//CHINESE
			linew += m_chiW;
		}

		if (linew >= width - m_chiW)
		{
			int len1 = pContent + i - pStr + 1;
			//	CStringW str;
			STR_LOC sl;
			sl.type = ELocNone;
			sl.start = pStr - pContent;
			sl.length = len1;
			this->m_slList.push_back(sl);

			pStr = pContent + i + 1;

			linew = 0;
			nline++;
		}

	}
	if (linew > 0)
	{
		int len1 = pContent + i - pStr;
		STR_LOC sl;
		sl.type = ELocNone;
		sl.start = pStr - pContent;
		sl.length = len1;
		this->m_slList.push_back(sl);

		pStr = pContent + i + 1;

		linew = 0;
		nline++;
	}

	m_curPage = m_pageNum;
	m_pageNum = m_slList.size() / PageLineNum + (m_slList.size() % PageLineNum>0 ? 1 : 0);
	m_preHeight = rect.Height();
	m_preFontH = m_fontH;
	m_preVs = m_vs;
	
}

void CTextReaderWnd::split()
{
	int i=0;
	int len = m_textLen;
	int nline = 0;
	int linew = 0;
	
	CRect rect;
	GetClientRect(&rect);
	int width=rect.Width()-5-5;
	int height=rect.Height();
	int PageLineNum = (height-8) / (m_fontH+m_vs);
	m_slList.clear();
	const wchar_t* pContent = m_ptext;
	const wchar_t* pStr = pContent;


	for(;i<len;i++)
	{
		USHORT ch = *((USHORT*)(pContent+i));
		wchar_t ch1 = *((USHORT*)(pContent+i));
		if( ch == 0x0a )
		{// '\n'
			int len1 = pContent+i-pStr+1;
			//	CStringW str;
			STR_LOC sl;
			sl.start = pStr-pContent;
			sl.length = len1;
			this->m_slList.push_back(sl);

			pStr = pContent+i+1;


			nline++;
			linew = 0;
		}
		else if( ch == 0x0d)
		{// '\r'

		}
		else if( ch <= 255 )
		{//ASCII
		//	linew += m_ascW;
			linew += GetAscW(ch);
		}
		else
		{//CHINESE
			linew += m_chiW;
		}

		if( linew >= width-m_chiW )
		{
			int len1 = pContent+i-pStr+1;
			//	CStringW str;
			STR_LOC sl;
			sl.start = pStr-pContent;
			sl.length = len1;
			this->m_slList.push_back(sl);

			pStr = pContent+i+1;

			linew = 0;
			nline++;
		}

	}

	m_curPage = 0;
	m_pageNum = m_slList.size()/PageLineNum+(m_slList.size()%PageLineNum>0?1:0);
}

LONG CTextReaderWnd::GetAscW(USHORT ch)
{
	return m_ascMap[ch];
}

void CTextReaderWnd::SetText(const wchar_t* ptext,int length)
{
	m_textLen = length;
	m_ptext = ptext;
	this->m_curPage = 0;
	
	split1();
}

void CTextReaderWnd::ReDraw()
{
	Init();

	split1();
	ShowText(this->m_curPage);
}

void  CTextReaderWnd::Init()
{
	CPaintDC dc(this);
	CRect rect1;
	GetClientRect(&rect1);
	

	m_ascMap.clear();

	m_MemBitmap.DeleteObject();
	m_MemDC.DeleteDC();

	m_MemDC.CreateCompatibleDC(NULL);
	m_MemBitmap.CreateCompatibleBitmap(&dc,rect1.Width(),rect1.Height());
	m_pOldBitmap  = m_MemDC.SelectObject(&m_MemBitmap);
	m_MemDC.SelectObject( this->GetFont());
	m_MemDC.FillSolidRect(0,0,rect1.Width(),rect1.Height(),m_color);

//	CSize s1 = m_MemDC.GetTextExtent(_T("f"));
	CSize s2 = m_MemDC.GetTextExtent(_T("网"));


	for (char i = ' '; i <= '~'; i++)
	{
		CString te;
		te.Format(_T("%c"), i);
		CSize s1 = m_MemDC.GetTextExtent(te);
		m_ascMap[i] = s1.cx;
	}



	m_fontH = s2.cy;
//	m_ascW = s1.cx;
	m_chiW = s2.cx;
	
	
	
	
}

