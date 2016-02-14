// VSDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "cyBookReader.h"
#include "VSDialog.h"
#include "afxdialogex.h"


// CVSDialog �Ի���

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


// CVSDialog ��Ϣ�������


void CVSDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_vs = GetDlgItemInt(IDC_EDIT_VS);
	CDialogEx::OnOK();
}


BOOL CVSDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetDlgItemInt(IDC_EDIT_VS, m_vs);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
