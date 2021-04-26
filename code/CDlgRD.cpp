// CDlgRD.cpp: 实现文件
//

#include "pch.h"
#include "SNLCC.h"
#include "CDlgRD.h"
#include "afxdialogex.h"


// CDlgRD 对话框

IMPLEMENT_DYNAMIC(CDlgRD, CDialogEx)

CDlgRD::CDlgRD(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RD, pParent)
{

}

CDlgRD::~CDlgRD()
{
}

void CDlgRD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(CDlgRD, CDialogEx)
END_MESSAGE_MAP()


// CDlgRD 消息处理程序
