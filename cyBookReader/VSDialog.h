#pragma once


// CVSDialog 对话框

class CVSDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CVSDialog)

public:
	CVSDialog(CWnd* pParent = NULL);   // 标准构造函数
	CVSDialog(int vs,CWnd* pParent = NULL);
	virtual ~CVSDialog();

// 对话框数据
	enum { IDD = IDD_VS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
	int GetVS(){return m_vs;}
private:
	int m_vs;

};
