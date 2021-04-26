
// SNLCCDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "SNLCC.h"
#include "SNLCCDlg.h"
#include "afxdialogex.h"


//#include "global.h"
extern int Main();



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFont cfont;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSNLCCDlg 对话框



CSNLCCDlg::CSNLCCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SNLCC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSNLCCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CODE, m_code);
	DDX_Control(pDX, IDC_TITLE, m_title);
	DDX_Control(pDX, IDC_RESULT, m_result);
}

BEGIN_MESSAGE_MAP(CSNLCCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SELECTFILE, &CSNLCCDlg::OnBnClickedSelectfile)
	ON_BN_CLICKED(IDC_EXECUTE, &CSNLCCDlg::OnBnClickedExecute)
END_MESSAGE_MAP()


// CSNLCCDlg 消息处理程序

BOOL CSNLCCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	/*ShowWindow(SW_MAXIMIZE);

	ShowWindow(SW_MINIMIZE);*/
	int m_Width = GetSystemMetrics(SM_CXSCREEN);  //屏幕宽度    
	int m_Height = GetSystemMetrics(SM_CYSCREEN); //屏幕高度
	CRect rectDlg;
	rectDlg.SetRect(m_Width * 0.15, m_Height * 0.1, m_Width * 0.85, m_Height * 0.9);
	TRACE("%d,%d,%d,%d\n", rectDlg);
	ShowWindow(SW_SHOWNORMAL);

	// TODO: 在此添加额外的初始化代码

	cfont.CreatePointFont(150, _T("黑体"), NULL);
	GetDlgItem(IDC_TITLE)->SetFont(&cfont);

	//添加标签页
	m_result.AddPage(TEXT("词法分析"),& dlgScanner, IDD_SCANNER);
	m_result.AddPage(TEXT("递归下降"), &dlgRD, IDD_RD);
	m_result.AddPage(TEXT("LL1分析"), &dlgLL1, IDD_LL1);
	m_result.AddPage(TEXT("语义分析"), &dlgSemantic, IDD_SEMANTIC);
	m_result.AddPage(TEXT("错误信息"), &dlgMessage, IDD_MESSAGE);
	//初始化标签页
	/*m_result.GetItemRect(0, &rectDlg);
	TRACE("%d,%d,%d,%d\n", rectDlg);*/

	m_result.Show();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSNLCCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSNLCCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSNLCCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSNLCCDlg::OnBnClickedSelectfile()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePathName;
	CString FileText = TEXT("");
	CString temp = TEXT("");
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		FilePathName = dlg.GetPathName(); 
	}
	CStdioFile file;
	if (!file.Open(FilePathName, CFile::modeRead))
	{
	//::AfxMessageBox(_T("文件打开失败。"));
		return;
	}
	while (file.ReadString(temp))
	{
		FileText += temp + TEXT("\r\n"); 
	}
	m_code.SetWindowText(FileText);
}

int count = 0;
void CSNLCCDlg::OnBnClickedExecute()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CStdioFile file;
	CString FileText = TEXT("");
	CString temp = TEXT("");

	//程序执行
	if (!file.Open(TEXT("./result/source.txt"), CFile::modeCreate | CFile::modeWrite))
	{
		//TRACE("OK!!\n");
		::AfxMessageBox(_T("source.txt文件打开失败。"));
		return;
	}
	m_code.GetWindowText(FileText);
	file.WriteString(FileText);
	file.Close();
	//::AfxMessageBox(_T("source.txt文件已打开!"));
	Main();
	

	//词法分析
	FileText = TEXT("");
	if (!file.Open(TEXT("./result/scannerResult.txt"), CFile::modeRead))
	{
		//TRACE("OK!!\n");
		::AfxMessageBox(_T("scannerResult.txt文件打开失败。"));
		return;
	}
	while (file.ReadString(temp))
	{
		FileText += temp + TEXT("\r\n");
	}
	/*CString str;
	str.Format(_T("count = % d"), count++);
	dlgScanner.m_edit.SetWindowText(str);*/
	dlgScanner.m_edit.SetWindowText(FileText);
	file.Close();


	//递归下降
	FileText = TEXT("");
	if (!file.Open(TEXT("./result/RDResult.txt"), CFile::modeRead))
	{
		::AfxMessageBox(_T("RDResult.txt文件打开失败。"));
		return;
	}
	while (file.ReadString(temp))
	{
		FileText += temp + TEXT("\r\n");
	}
	dlgRD.m_edit.SetWindowText(FileText);
	file.Close();

	//LL1
	FileText = TEXT("");
	if (!file.Open(TEXT("./result/LL1Result.txt"), CFile::modeRead))
	{
		::AfxMessageBox(_T("LL1Result.txt文件打开失败。"));
		return;
	}
	while (file.ReadString(temp))
	{
		FileText += temp + TEXT("\r\n");
	}
	dlgLL1.m_edit.SetWindowText(FileText);
	dlgRD.m_edit.SetWindowText(FileText);
	/*str.Format(_T("count = % d"), count++);
	dlgLL1.m_edit.SetWindowText(str);*/
	file.Close();

	//语义分析
	FileText = TEXT("");
	if (!file.Open(TEXT("./result/semanticResult.txt"), CFile::modeRead))
	{
		::AfxMessageBox(_T("semanticResult.txt文件打开失败。"));
		return;
	}
	while (file.ReadString(temp))
	{
		FileText += temp + TEXT("\r\n");
	}
	dlgSemantic.m_edit.SetWindowText(FileText);
	file.Close();

	//报错信息
	FileText = TEXT("");
	if (!file.Open(TEXT("./result/message.txt"), CFile::modeRead))
	{
		::AfxMessageBox(_T("message.txt文件打开失败。"));
		return;
	}
	while (file.ReadString(temp))
	{
		FileText += temp + TEXT("\r\n");
	}
	dlgMessage.m_edit.SetWindowText(FileText);
	file.Close();

	//CStdioFile file;
	//CString FileText = TEXT("");
	//CString temp = TEXT("");
	//file.Open(TEXT("./result/source.txt"), CFile::modeCreate | CFile::modeWrite);
	//file.WriteString("This is a test"); 
	//TRACE("%d\n", count++);
	////file.Close();
	//file.Open(TEXT("./result/source.txt"), CFile::modeRead);
	//while (file.ReadString(temp))
	//{
	//	FileText += temp + TEXT("\r\n");
	//}
	//file.Close();
	//dlgScanner.m_edit.SetWindowText(FileText);

}
