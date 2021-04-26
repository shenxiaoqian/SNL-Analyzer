#pragma once


// CDlgRD 对话框

class CDlgRD : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRD)

public:
	CDlgRD(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgRD();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit;
};
