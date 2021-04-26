// CDlgSemantic.cpp: 实现文件
//

#include "pch.h"
#include "SNLCC.h"
#include "CDlgSemantic.h"
#include "afxdialogex.h"


// CDlgSemantic 对话框

IMPLEMENT_DYNAMIC(CDlgSemantic, CDialogEx)

CDlgSemantic::CDlgSemantic(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SEMANTIC, pParent)
{

}

CDlgSemantic::~CDlgSemantic()
{
}

void CDlgSemantic::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(CDlgSemantic, CDialogEx)
END_MESSAGE_MAP()


// CDlgSemantic 消息处理程序
