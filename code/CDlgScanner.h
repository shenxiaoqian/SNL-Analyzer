#pragma once


// CDlgScanner 对话框

class CDlgScanner : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgScanner)

public:
	CDlgScanner(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgScanner();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCANNER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit;
	virtual BOOL OnInitDialog();
};
