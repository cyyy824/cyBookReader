
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
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
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

	//
	bool iss = false;
	if( !m_env.Open("config.db"))
	{
		iss = m_env.Create("config.db");
		m_env.AddNew();
		m_env.m_cx = 0;
		m_env.m_cy = 0;
		m_env.m_vs = 0;
		m_env.m_font = "宋体";
		m_env.m_fontH = 15;
		m_env.m_color = RGB(255,255,255);
		m_env.m_fontColor = RGB(0,0,0);
		m_env.Update();
	}
	else
	{
		if( m_env.IsOpen())
		{
			int count = m_env.GetCount();
			m_env.MoveFirst();
		}
	}
	//
	// TODO: 在此添加额外的初始化代码
	sf=0;
	CFont* myFont = new CFont;
//	myFont->CreateFont(
    myFont->CreateFont( 
		m_env.m_fontH,                                              //   nHeight 
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
		CString(m_env.m_font));
	m_txReaderWnd.SetFont(myFont);
	m_txReaderWnd.SetVS(m_env.m_vs);
	myFont->Detach();
    delete myFont;

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
	
	if( m_env.m_cx >0 && m_env.m_cy >0)
	 {
	//	 ::Getcli
		 int sw = ::GetSystemMetrics(SM_CXSCREEN);
		 int sh = ::GetSystemMetrics(SM_CYSCREEN);
		 CRect r1;
		 r1.left = sw/2-m_env.m_cx/2;
		 r1.top = sh/2-m_env.m_cy/2;
	//	 r1.right = m_pSetting->m_cx;
	//	 r1.bottom = m_pSetting->m_cy;
		 this->SetWindowPos(NULL, r1.left,r1.top,m_env.m_cx,m_env.m_cy, SWP_SHOWWINDOW);
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

	
	std::wstring fnstr = strFile.GetBuffer();
	
	int a = fnstr.find(L".txt");
	int b = fnstr.find(L".epub");
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
	
	
	m_pBook->ParseFromFile(fnstr);


	m_dirLB.ResetContent();
	std::vector<std::wstring> snlist = m_pBook->GetAllSectionName();
	for(std::vector<std::wstring>::iterator it=snlist.begin();it!=snlist.end();it++)
	{
		m_dirLB.AddString( it->c_str());
	}

	int len1;
	const wchar_t* pstr = m_pBook->GetSectionContent(0,len1);
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
	sf=1;
	CDialogEx::OnNcLButtonDown(nHitTest, point);
}


void CcyBookReaderDlg::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( sf < 2)
	{
		sf = 0;
	}
	else
	{
		sf = 0;
		CRect r1;
		this->GetClientRect(&r1);
		int cx = r1.Width();
		int cy = r1.Height();
		ReSize(cx,cy);
	}
	CDialogEx::OnNcLButtonUp(nHitTest, point);
}


void CcyBookReaderDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(nType!=SIZE_MINIMIZED)  
	{
		if (sf == 1 || sf==2)
		{
			sf =2;
			return;
		}
		ReSize(cx,cy);
	}
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

		m_txReaderWnd.SetWindowPos(NULL,112,28,cx-112-119+20,cy-28-34-20-15,SWP_NOZORDER|SWP_NOMOVE);
		m_txReaderWnd.ReDraw();
		CString pnf;
		pnf.Format(L"1/%d",m_txReaderWnd.GetPageNum());
		m_pageF.SetWindowText(pnf);
		m_rateSC.SetRange(0, m_txReaderWnd.GetPageNum()-1);
		m_rateSC.SetPos(m_txReaderWnd.GetCurPageNum());

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
			pFont->Detach();
			delete pFont;

			m_txReaderWnd.ReDraw();

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
		m_txReaderWnd.ReDraw();
	}
}


void CcyBookReaderDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFont* pFont = m_txReaderWnd.GetFont();
	LOGFONT logf;
	pFont->GetLogFont(&logf);

	m_env.MoveFirst();
	m_env.Edit();

	m_env.m_fontH = logf.lfHeight;
	m_env.m_font =  CStringA(logf.lfFaceName);
	m_env.m_vs = m_txReaderWnd.GetVS();
	CRect r1;
		this->GetClientRect(&r1);
		m_env.m_cx = r1.Width();
		m_env.m_cy = r1.Height();
		m_env.m_color = RGB(255,255,255);
		m_env.m_fontColor = m_txReaderWnd.GetFontColor();

//	m_TxtPage.
		m_env.Update();




	CDialogEx::OnClose();
}
