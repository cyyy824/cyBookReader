#pragma once


// CVSDialog �Ի���

class CVSDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CVSDialog)

public:
	CVSDialog(CWnd* pParent = NULL);   // ��׼���캯��
	CVSDialog(int vs,CWnd* pParent = NULL);
	virtual ~CVSDialog();

// �Ի�������
	enum { IDD = IDD_VS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
	int GetVS(){return m_vs;}
private:
	int m_vs;

};
