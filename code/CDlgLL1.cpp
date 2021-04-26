// CDlgLL1.cpp: 实现文件
//

#include "pch.h"
#include "SNLCC.h"
#include "CDlgLL1.h"
#include "afxdialogex.h"


// CDlgLL1 对话框

IMPLEMENT_DYNAMIC(CDlgLL1, CDialogEx)

CDlgLL1::CDlgLL1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LL1, pParent)
{

}

CDlgLL1::~CDlgLL1()
{
}

void CDlgLL1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(CDlgLL1, CDialogEx)
END_MESSAGE_MAP()


// CDlgLL1 消息处理程序
