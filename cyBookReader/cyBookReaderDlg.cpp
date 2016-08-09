
// cyBookReaderDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "cyBookReader.h"
#include "cyBookReaderDlg.h"
#include "afxdialogex.h"

#include "VSDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
END_MESSAGE_MAP()


// CcyBookReaderDlg 对话框




CcyBookReaderDlg::CcyBookReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcyBookReaderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcyBookReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DIR, m_dirLB);
	DDX_Control(pDX, IDC_STATIC_TXT, m_txReaderWnd);
	DDX_Control(pDX, IDC_SLIDER_RATE, m_rateSC);
	DDX_Control(pDX, IDC_STATIC_PAGENUM, m_pageF);
}

BEGIN_MESSAGE_MAP(CcyBookReaderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CcyBookReaderDlg::OnBnClickedButtonOpen)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_PRE, &CcyBookReaderDlg::OnBnClickedButtonPre)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CcyBookReaderDlg::OnBnClickedButtonNext)
	ON_LBN_DBLCLK(IDC_LIST_DIR, &CcyBookReaderDlg::OnLbnDblclkListDir)
	ON_BN_CLICKED(IDC_BUTTON_FONT, &CcyBookReaderDlg::OnBnClickedButtonFont)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_VS, &CcyBookReaderDlg::OnBnClickedButtonVs)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_BG, &CcyBookReaderDlg::OnBnClickedButtonBg)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_CHECK_DIR, &CcyBookReaderDlg::OnBnClickedCheckDir)
END_MESSAGE_MAP()


// CcyBookReaderDlg 消息处理程序

BOOL CcyBookReaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_config.Read();

	//
	bool iss = false;

	m_isShowDir = true;
	((CButton*)GetDlgItem(IDC_CHECK_DIR))->SetCheck(m_config.m_issplit);
	if (m_config.m_issplit)
	{
		ShowDir();
	}
	else
	{
		HideDir();
	}
	
	//
	// TODO: 在此添加额外的初始化代码
	sf=0;
	OnSizeState = SIZE_RESTORED;
	
	m_txReaderWnd.SetFontColor(m_config.m_fontColor);
	
	CFont* myFont = new CFont;
//	myFont->CreateFont(
    myFont->CreateFont( 
		m_config.m_fontH,                                              //   nHeight 
        0,                                                         //   nWidth 
        0,                                                         //   nEscapement 
        0,                                                         //   nOrientation 
        FW_NORMAL,                                                  //   nWeight 
        FALSE,                                                   //   bItalic 
        FALSE,                                                    //   bUnderline 
        0,                                                   //   cStrikeOut 
        GB2312_CHARSET,                             //   nCharSet 
        OUT_DEFAULT_PRECIS,                 //   nOutPrecision 
        CLIP_DEFAULT_PRECIS,               //   nClipPrecision 
        CLEARTYPE_QUALITY,                       //   nQuality 
        DEFAULT_PITCH   |   FF_SWISS,     //   nPitchAndFamily 
		CString(m_config.m_font));
	m_txReaderWnd.SetFont(myFont);
	m_txReaderWnd.SetVS(m_config.m_vs);
	myFont->Detach();
    delete myFont;

	m_txReaderWnd.SetBgColor(m_config.m_color);

	m_pageF.SetWindowText(L"");
	m_txReaderWnd.Init();

	CRect rect;
	CRect rect1;
	CRect rect2;
	CRect rect3;
	CRect rect4;
	GetClientRect(&rect);
	m_dirLB.GetClientRect(&rect1);
	m_txReaderWnd.GetClientRect(&rect2);
	m_rateSC.GetClientRect(&rect3);
	m_pageF.GetClientRect(&rect4);
	
	if( m_config.m_cx >0 && m_config.m_cy >0)
	 {
	//	 ::Getcli
		 int sw = ::GetSystemMetrics(SM_CXSCREEN);
		 int sh = ::GetSystemMetrics(SM_CYSCREEN);
		 CRect r1;
		 r1.left = sw/2-m_config.m_cx/2;
		 r1.top = sh/2-m_config.m_cy/2;
	//	 r1.right = m_pSetting->m_cx;
	//	 r1.bottom = m_pSetting->m_cy;
		 this->SetWindowPos(NULL, r1.left,r1.top,m_config.m_cx,m_config.m_cy, SWP_SHOWWINDOW);
	//	 this->ReSize(m_pSetting->m_cx, m_pSetting->m_cy);
	//	this->GetClientRect(&r1);
	//	this->SetWindowPos(NULL, r1.left, r1.
	 }



	m_pBook = NULL;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CcyBookReaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
		if( (nID & 0xFFF0) == SC_SIZE || (nID & 0xFFF0) == SC_MOVE)
            PostMessage(WM_NCLBUTTONUP, HTCAPTION, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcyBookReaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcyBookReaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CcyBookReaderDlg::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFile = _T("");

    CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.txt;*.epub)|*.txt;*.epub|All Files (*.*)|*.*||"), NULL);
    if (dlgFile.DoModal()!= IDOK)
    {
		return;
    }
	strFile = dlgFile.GetPathName();
	((CEdit*)GetDlgItem(IDC_EDIT_PATH))->SetWindowText(strFile);

	
	CString fnstr = strFile.GetBuffer();
	
	int a = fnstr.Find(L".txt");
	int b = fnstr.Find(L".epub");
	if( a > -1 )
	{
		if( m_pBook !=NULL) delete m_pBook;
		m_pBook = new CTxtBookMark();
	}
	else if(b > -1 )
	{
		if( m_pBook !=NULL) delete m_pBook;
		m_pBook = new CEpubBookMark();
	}
	m_pBook->SetIsSplit(((CButton*)GetDlgItem(IDC_CHECK_DIR))->GetCheck());
	m_pBook->ParseFromFile(fnstr);


	m_dirLB.ResetContent();
	std::vector<CString> snlist = m_pBook->GetAllSectionName();
	for(std::vector<CString>::iterator it=snlist.begin();it!=snlist.end();it++)
	{
		m_dirLB.AddString( it->GetBuffer());
	}

	int len1;
	const wchar_t* pstr = m_pBook->GetSectionContent(0,len1);
//	m_txReaderWnd.ShowText(0);
	m_txReaderWnd.SetText(pstr, len1);

	m_txReaderWnd.ShowText(0);
	m_rateSC.SetRange(0, m_txReaderWnd.GetPageNum()-1);
	TRACE(L"%d-%d",0, m_txReaderWnd.GetPageNum());
	m_rateSC.SetPos(0);
	CString pnf;
	pnf.Format(L"1/%d",m_txReaderWnd.GetPageNum());
	m_pageF.SetWindowText(pnf);
}


void CcyBookReaderDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect r1;
	this->GetClientRect(&r1);
	int cx = r1.Width();
	int cy = r1.Height();
	CString ab;
	ab.Format(_T("\n OnNcLButtonDown----left:%d,top=%d,w=%d,h=%d;px=%d,py=%d"), r1.left, r1.top, cx, cy,point.x,point.y);
	OutputDebugString(ab);
	sf=1;
	CDialogEx::OnNcLButtonDown(nHitTest, point);
}


void CcyBookReaderDlg::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect r1;
	this->GetClientRect(&r1);
	int cx = r1.Width();
	int cy = r1.Height();
	CString ab;
	ab.Format(_T("\n OnNcLButtonUp----left:%d,top=%d,w=%d,h=%d;px=%d,py=%d"), r1.left, r1.top, cx, cy, point.x, point.y);
	OutputDebugString(ab);
	if( sf < 2)
	{
		sf = 0;
	}
	else
	{
		sf = 0;

		ReSize(cx,cy);
	}
	CDialogEx::OnNcLButtonUp(nHitTest, point);
}


void CcyBookReaderDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	

	CString ab;
	ab.Format(_T("\nType:%d:x=%d,y=%d"), nType,cx,cy);
	OutputDebugString(ab);
	// TODO: 在此处添加消息处理程序代码
	if (nType == SIZE_RESTORED)
	{
		if (OnSizeState == SIZE_MINIMIZED)
		{

		}
		else if (OnSizeState == SIZE_MAXIMIZED)
		{
			ReSize(cx, cy);
		}
		else if (sf == 1 || sf == 2)
		{
			sf = 2;
		}
		else
		{
			ReSize(cx, cy);
		}
	}
	else if (nType == SIZE_MAXIMIZED)
	{
		sf = 0;
		ReSize(cx, cy);
	}

	OnSizeState = nType;
}


void CcyBookReaderDlg::OnBnClickedButtonPre()
{
	// TODO: 在此添加控件通知处理程序代码
	int pn = m_txReaderWnd.GetPageNum();
	int curp = m_txReaderWnd.GetCurPageNum();
	if( pn<=0)return;
	if(curp<=0)
	{
		int len1;
		int cb = this->m_pBook->GetPreGetBlock();
		const wchar_t* pstr=m_pBook->GetSectionContent(cb-1,len1);
		if( pstr!=NULL)
		{
			m_txReaderWnd.SetText(pstr,len1);
			int pn = m_txReaderWnd.GetPageNum();
			curp=pn-1;
			m_rateSC.SetRange(0, m_txReaderWnd.GetPageNum()-1);
		}
	}
	else
	{
		curp=curp-1;
	}
	TRACE(L"%d",curp);

	m_txReaderWnd.ShowText(curp);
	m_rateSC.SetPos(curp);
	TRACE(L",%d\n",m_rateSC.GetPos());
	CString pnf;
	pnf.Format(L"%d/%d",curp+1,m_txReaderWnd.GetPageNum());
	m_pageF.SetWindowText(pnf);
}


void CcyBookReaderDlg::OnBnClickedButtonNext()
{
	// TODO: 在此添加控件通知处理程序代码
	int pn = m_txReaderWnd.GetPageNum();
	int curp = m_txReaderWnd.GetCurPageNum();
	if( pn<=0)return;
	if(curp>=pn-1)
	{
		int len1;
		int cb = this->m_pBook->GetPreGetBlock();
		const wchar_t* pstr=m_pBook->GetSectionContent(cb+1,len1);
		if( pstr!=NULL)
		{
		//	m_txReaderWnd.set
			m_txReaderWnd.SetText(pstr,len1);
			int pn = m_txReaderWnd.GetPageNum();

			m_rateSC.SetRange(0, m_txReaderWnd.GetPageNum()-1);
			curp=0;
		}
	}
	else
	{
		curp++;
	}

	TRACE(L"%d",curp);

	m_txReaderWnd.ShowText(curp);
	m_rateSC.SetPos(curp);
	TRACE(L",%d\n",m_rateSC.GetPos());
	CString pnf;
	pnf.Format(L"%d/%d",curp+1,m_txReaderWnd.GetPageNum());
	m_pageF.SetWindowText(pnf);
}


void CcyBookReaderDlg::OnLbnDblclkListDir()
{
	// TODO: 在此添加控件通知处理程序代码
	int len1;
	int i = m_dirLB.GetCurSel();
	
	const wchar_t* pstr = m_pBook->GetSectionContent(i,len1);
	m_txReaderWnd.SetText(pstr, len1);
	m_txReaderWnd.ShowText(0);
	m_rateSC.SetRange(0, m_txReaderWnd.GetPageNum()-1);
	m_rateSC.SetPos(0);

	CString pnf;
	pnf.Format(L"1/%d",m_txReaderWnd.GetPageNum());
	m_pageF.SetWindowText(pnf);
}

void CcyBookReaderDlg::ReSize(int cx, int cy)
{
	if( m_txReaderWnd)
	{

	//	SetWindowPos(m_txReaderWnd.m_hWnd,112,27,cx-119,cy-34,SWP_NOZORDER|SWP_NOMOVE);
		m_dirLB.SetWindowPos(NULL,7,27,175,cy-64,SWP_NOZORDER|SWP_NOMOVE);
		m_rateSC.SetWindowPos(NULL,196,cy-40,cx-330,26, SWP_NOZORDER);
		m_pageF.SetWindowPos(NULL,cx-135,cy-34,119,21,SWP_NOZORDER);

		if( m_isShowDir) 
		{
			m_txReaderWnd.SetWindowPos(NULL, 200, 47, cx - 112 - 119 + 20, cy - 28 - 34 - 20 - 15, SWP_NOZORDER);
		}
		else
		{
			m_txReaderWnd.SetWindowPos(NULL, 17, 47, cx - 112 - 119 + 20+183, cy - 28 - 34 - 20 - 15, SWP_NOZORDER);
		}

		m_txReaderWnd.ReDraw();
		CString pnf;
		int pn = m_txReaderWnd.GetPageNum();
		int curp = m_txReaderWnd.GetCurPageNum();
		pnf.Format(L"%d/%d",curp+1,pn);
		m_pageF.SetWindowText(pnf);
		m_rateSC.SetRange(0, pn-1);
		m_rateSC.SetPos(curp);

	}
}

void CcyBookReaderDlg::OnBnClickedButtonFont()
{
	// TODO: 在此添加控件通知处理程序代码
	CFont *pFont = m_txReaderWnd.GetFont();
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	CFontDialog dlg(&lf,CF_EFFECTS   |   CF_SCREENFONTS,NULL,this); 
	dlg.m_cf.rgbColors = m_txReaderWnd.GetFontColor();

        if(IDOK==dlg.DoModal())
        {
			CFont *pOldFont = m_txReaderWnd.GetFont();
			CFont *pFont = new CFont;
			pFont->CreateFontIndirectW(dlg.m_cf.lpLogFont);
			m_txReaderWnd.SetFontColor(dlg.m_cf.rgbColors);
		//	font.CreateFontIndirectW(dlg.m_cf.lpLogFont);
			m_txReaderWnd.SetFont(pFont);

			if (dlg.m_cf.lpLogFont->lfHeight != m_config.m_vs)
			{
				m_config.m_fontH = dlg.m_cf.lpLogFont->lfHeight;
				m_config.m_vs = m_txReaderWnd.AutoSetVS();
			}
			pFont->Detach();
			delete pFont;

			CRect r1;
			this->GetClientRect(&r1);
			int cx = r1.Width();
			int cy = r1.Height();
			ReSize(cx, cy);
		//	m_txReaderWnd.ReDraw();

        }
}

//由于WM_KEY消息只能捕捉控件的，所以用这个
BOOL CcyBookReaderDlg::PreTranslateMessage(MSG* pMsg)
{
//	if( pMsg->hwnd == m_rateSC.m_hWnd)
//	{
//	}
//	else
//	{
	if( ((CEdit*)GetDlgItem(IDC_EDIT_PATH))->m_hWnd == pMsg->hwnd )
	{
	}
	else if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RIGHT)
	{
		OnBnClickedButtonNext();
		return TRUE;
	}
	else if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_LEFT)
	{
		OnBnClickedButtonPre();
		return TRUE;
	}
//	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CcyBookReaderDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	switch(pScrollBar->GetDlgCtrlID())
	{
	case IDC_SLIDER_RATE:
		
		TRACE("OnHScroll:nSBCode=%d,nPos=%d\n",nSBCode,nPos);
		if(m_rateSC.GetPos()>=0 && m_rateSC.GetPos() < m_txReaderWnd.GetPageNum())
		{
			m_txReaderWnd.ShowText(m_rateSC.GetPos());

			CString pnf;
			pnf.Format(L"%d/%d",m_rateSC.GetPos()+1,m_txReaderWnd.GetPageNum());
			m_pageF.SetWindowText(pnf);
		}
		
	//	m_FileContent.GetCurFrame(nPos);
		break;
	}


	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}




void CcyBookReaderDlg::OnBnClickedButtonVs()
{
	// TODO: 在此添加控件通知处理程序代码
	int n = m_txReaderWnd.GetVS();
	CVSDialog vsdlg(n);
	if(IDOK==vsdlg.DoModal())
	{
		m_txReaderWnd.SetVS(vsdlg.GetVS());
	//	m_txReaderWnd.ReDraw();
		CRect r1;
		this->GetClientRect(&r1);
		int cx = r1.Width();
		int cy = r1.Height();
		ReSize(cx, cy);
	}
}


void CcyBookReaderDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFont* pFont = m_txReaderWnd.GetFont();
	LOGFONT logf;
	pFont->GetLogFont(&logf);
	

	m_config.m_fontH = logf.lfHeight;
	m_config.m_font =  CStringA(logf.lfFaceName);
	m_config.m_vs = m_txReaderWnd.GetVS();
	CRect r1;
	this->GetClientRect(&r1);
	m_config.m_cx = r1.Width();
	m_config.m_cy = r1.Height();
	m_config.m_color = m_txReaderWnd.GetBGColor();
	m_config.m_fontColor = m_txReaderWnd.GetFontColor();
	m_config.m_issplit = ((CButton*)GetDlgItem(IDC_CHECK_DIR))->GetCheck();

//	m_TxtPage.
	m_config.Update();




	CDialogEx::OnClose();
}


void CcyBookReaderDlg::OnBnClickedButtonBg()
{
	// TODO: 在此添加控件通知处理程序代码
	/*
	CFont *pFont = m_txReaderWnd.GetFont();
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	CFontDialog dlg(&lf, CF_EFFECTS | CF_SCREENFONTS, NULL, this);
	dlg.m_cf.rgbColors = m_txReaderWnd.GetFontColor();
	*/
	COLORREF color = m_txReaderWnd.GetBGColor();
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_RGBINIT | CC_FULLOPEN;
	if (IDOK == dlg.DoModal())
	{
		m_config.m_color = dlg.m_cc.rgbResult;
		m_txReaderWnd.SetBgColor(m_config.m_color);
		m_txReaderWnd.ReDraw();

	}
}


void CcyBookReaderDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect drc;
	CRect crc;
	m_dirLB.GetWindowRect(&drc);
	ScreenToClient(&drc);
	m_txReaderWnd.GetWindowRect(&crc);
	ScreenToClient(&crc);
	
	if (m_isShowDir)
	{
		CRect trc;
		trc.top = drc.top;
		trc.bottom = drc.bottom;
		trc.left = drc.right;
		trc.right = crc.left;
		if (trc.PtInRect(point))
		{
			HideDir();
			
		}
	}
	else
	{
		CRect trc;
		trc.top = crc.top;
		trc.bottom = crc.bottom;
		trc.left = 0;
		trc.right = crc.left;
		if (trc.PtInRect(point))
		{
			ShowDir();
			
		}
	}



	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CcyBookReaderDlg::HideDir()
{
	CRect r1;
	this->GetClientRect(&r1);
	int cx = r1.Width();
	int cy = r1.Height();
	
	if (!m_isShowDir)
		return;

	m_dirLB.ShowWindow(FALSE);
	m_isShowDir = false;
	ReSize(cx, cy);
}

void CcyBookReaderDlg::ShowDir()
{
	CRect r1;
	this->GetClientRect(&r1);
	int cx = r1.Width();
	int cy = r1.Height();

	if (m_isShowDir)
		return;
	m_dirLB.ShowWindow(TRUE);
	m_isShowDir = true;
	ReSize(cx, cy);
}



void CcyBookReaderDlg::OnBnClickedCheckDir()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect r1;
	this->GetClientRect(&r1);
	int cx = r1.Width();
	int cy = r1.Height();

	int cs = ((CButton*)GetDlgItem(IDC_CHECK_DIR))->GetCheck();
	
	
	if (((CButton*)GetDlgItem(IDC_CHECK_DIR))->GetCheck() == BST_CHECKED)
	{
		OutputDebugString(L"check");
		ShowDir();
		if (m_pBook == NULL) return;
		if (m_pBook->SetIsSplit(true))
		{
			m_pBook->ParseFromString();
			m_dirLB.ResetContent();
			std::vector<CString> snlist = m_pBook->GetAllSectionName();
			for (std::vector<CString>::iterator it = snlist.begin(); it != snlist.end(); it++)
			{
				m_dirLB.AddString(it->GetBuffer());
			}

			int len1;
			const wchar_t* pstr = m_pBook->GetSectionContent(0, len1);
			m_txReaderWnd.SetText(pstr, len1);
			m_txReaderWnd.ShowText(0);
			m_rateSC.SetRange(0, m_txReaderWnd.GetPageNum() - 1);
			TRACE(L"%d-%d", 0, m_txReaderWnd.GetPageNum());
			m_rateSC.SetPos(0);
			CString pnf;
			pnf.Format(L"1/%d", m_txReaderWnd.GetPageNum());
			m_pageF.SetWindowText(pnf);
//			ReSize(cx, cy);
		}
	}
	else
	{
		OutputDebugString(L"nocheck");
		HideDir();
		if (m_pBook == NULL) return;
		if (m_pBook->SetIsSplit(false))
		{
			m_pBook->ParseFromString();
			m_dirLB.ResetContent();
			std::vector<CString> snlist = m_pBook->GetAllSectionName();
			for (std::vector<CString>::iterator it = snlist.begin(); it != snlist.end(); it++)
			{
				m_dirLB.AddString(it->GetBuffer());
			}

			int len1;
			const wchar_t* pstr = m_pBook->GetSectionContent(0, len1);
			m_txReaderWnd.SetText(pstr, len1);
			m_txReaderWnd.ShowText(0);
			m_rateSC.SetRange(0, m_txReaderWnd.GetPageNum() - 1);
			TRACE(L"%d-%d", 0, m_txReaderWnd.GetPageNum());
			m_rateSC.SetPos(0);
			CString pnf;
			pnf.Format(L"1/%d", m_txReaderWnd.GetPageNum());
			m_pageF.SetWindowText(pnf);
		}
	}
	
}


void CAboutDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
//	if (wcscmp(pNMLink->item.szUrl, _T("http:\/\/weibo.com\/lisonglisong")) == 0)
//	{
		// 主要执行语句 
		ShellExecuteW(NULL, L"open", pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
//	}
	*pResult = 0;

}
