// CDlgScanner.cpp: 实现文件
//

#include "pch.h"
#include "SNLCC.h"
#include "CDlgScanner.h"
#include "afxdialogex.h"


// CDlgScanner 对话框

IMPLEMENT_DYNAMIC(CDlgScanner, CDialogEx)

CDlgScanner::CDlgScanner(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCANNER, pParent)
{

}

CDlgScanner::~CDlgScanner()
{
}

void CDlgScanner::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(CDlgScanner, CDialogEx)
END_MESSAGE_MAP()


// CDlgScanner 消息处理程序


BOOL CDlgScanner::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//m_edit.SetWindowTextW(TEXT("词法分析结果"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
