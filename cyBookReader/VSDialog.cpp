// VSDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "cyBookReader.h"
#include "VSDialog.h"
#include "afxdialogex.h"


// CVSDialog 对话框

IMPLEMENT_DYNAMIC(CVSDialog, CDialogEx)

CVSDialog::CVSDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVSDialog::IDD, pParent)
{
	m_vs = 0;
}

CVSDialog::CVSDialog(int vs,CWnd* pParent /*=NULL*/)
	: CDialogEx(CVSDialog::IDD, pParent)
{
	m_vs = vs;
}

CVSDialog::~CVSDialog()
{
}

void CVSDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVSDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CVSDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CVSDialog 消息处理程序


void CVSDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_vs = GetDlgItemInt(IDC_EDIT_VS);
	CDialogEx::OnOK();
}


BOOL CVSDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetDlgItemInt(IDC_EDIT_VS, m_vs);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
