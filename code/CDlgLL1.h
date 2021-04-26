#pragma once


// CDlgLL1 对话框

class CDlgLL1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgLL1)

public:
	CDlgLL1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgLL1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LL1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit;
};
