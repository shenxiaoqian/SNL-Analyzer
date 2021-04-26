
// SNLCCDlg.h: 头文件
//

#pragma once

#include "TabSheet.h"
#include "CDlgScanner.h"
#include "CDlgRD.h"
#include "CDlgLL1.h"
#include "CDlgSemantic.h"
#include "CDlgMessage.h"


// CSNLCCDlg 对话框
class CSNLCCDlg : public CDialogEx
{
// 构造
public:
	CSNLCCDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SNLCC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CEdit m_code;
public:
	afx_msg void OnBnClickedSelectfile();
private:
	CStatic m_title;
public:
	afx_msg void OnBnClickedExecute();
private:
	CTabSheet m_result;

	CDlgScanner dlgScanner;
	CDlgRD dlgRD;
	CDlgLL1 dlgLL1;
	CDlgSemantic dlgSemantic;
	CDlgMessage dlgMessage;

};
