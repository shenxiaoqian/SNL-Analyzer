// CDlgMessage.cpp: 实现文件
//

#include "pch.h"
#include "SNLCC.h"
#include "CDlgMessage.h"
#include "afxdialogex.h"


// CDlgMessage 对话框

IMPLEMENT_DYNAMIC(CDlgMessage, CDialogEx)

CDlgMessage::CDlgMessage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MESSAGE, pParent)
{

}

CDlgMessage::~CDlgMessage()
{
}

void CDlgMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(CDlgMessage, CDialogEx)
END_MESSAGE_MAP()


// CDlgMessage 消息处理程序
