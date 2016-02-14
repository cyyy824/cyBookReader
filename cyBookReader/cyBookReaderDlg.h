
// cyBookReaderDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "TextReaderWnd.h"
#include "TxtBookMark.h"
#include "EpubBookMark.h"
#include "afxcmn.h"
#include "EnvDb.h"

// CcyBookReaderDlg 对话框
class CcyBookReaderDlg : public CDialogEx
{
// 构造
public:
	CcyBookReaderDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CYBOOKREADER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void ReSize(int cx, int cy);

public:
	CListBox m_dirLB;
//	CTxtBookMark m_bm;
	MBookMark* m_pBook;
	CEnvDb m_env;
	int sf;
	CTextReaderWnd m_txReaderWnd;

	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonPre();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnLbnDblclkListDir();
	afx_msg void OnBnClickedButtonFont();
	CSliderCtrl m_rateSC;
	CStatic m_pageF;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnBnClickedButtonVs();
	afx_msg void OnClose();
};
