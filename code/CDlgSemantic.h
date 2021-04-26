#pragma once


// CDlgSemantic 对话框

class CDlgSemantic : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSemantic)

public:
	CDlgSemantic(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgSemantic();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEMANTIC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit;
};
