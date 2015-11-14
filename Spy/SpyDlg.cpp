// SpyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Spy.h"
#include "SpyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpyDlg 对话框

HWND spyWnd = NULL;


CSpyDlg::CSpyDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CSpyDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDI_MAINFRAME);
    m_pInfoDlg = NULL;
    spy_window = NULL;
    m_hThreadRS = NULL;
}

void CSpyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE1, tree);
    DDX_Control(pDX, IDC_EDIT_CLASSNAME, edit_class_name);
    DDX_Control(pDX, IDC_EDIT_CLASSSTYLE, edit_class_style);
    DDX_Control(pDX, IDC_EDIT_EXSTYLE, edit_exstyle);
    DDX_Control(pDX, IDC_EDIT_HANDLE, edit_handle);
    DDX_Control(pDX, IDC_EDIT_STYLE, edit_style);
    DDX_Control(pDX, IDC_EDIT_TITLE, edit_title);
    DDX_Control(pDX, IDC_LIST_CLASS, list_class);
    DDX_Control(pDX, IDC_LIST_EXSTYLE, list_exstyle);
    DDX_Control(pDX, IDC_LIST_STYLE, list_style);
    DDX_Control(pDX, IDC_EDIT_CLASSSTYLE, edit_class_style);
    DDX_Control(pDX, IDC_EDIT_PID, edit_pid);
    DDX_Control(pDX, IDC_EDIT_TID, edit_tid);
    DDX_Control(pDX, IDC_EDIT_CLIENT, edit_client);
    DDX_Control(pDX, IDC_EDIT_SCREEN, edit_screen);
    DDX_Control(pDX, IDC_EDIT_SEARCH, edit_search);
    DDX_Control(pDX, IDC_SEARCH, btn_search);
    DDX_Control(pDX, IDC_REFRESH, btn_refresh);
    DDX_Control(pDX, IDC_SCAN, btn_scan);
    DDX_Control(pDX, IDC_EDIT_ATTRIBUTION, edit_attr);
    DDX_Control(pDX, IDC_EDIT_TREEID, edit_treeid);
    DDX_Control(pDX, IDC_EDIT_IMAGENAME, edit_img_name);
    DDX_Control(pDX, IDC_COMBO_IMGINFO, combo_img_info);
    DDX_Control(pDX, IDC_COMBO_MODULE, combo_img_module);
    DDX_Control(pDX, IDC_COMBO_THREAD, combo_img_thread);
    DDX_Control(pDX, IDC_SPY_ICON, m_icon);
	DDX_Control(pDX, IDC_SLIDER1, m_slide);
	DDX_Control(pDX, IDC_CHECK1, m_checkbox);
}

BEGIN_MESSAGE_MAP(CSpyDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BUTTON1, &CSpyDlg::OnRefreshWindows)
ON_NOTIFY(NM_CLICK, IDC_TREE1, &CSpyDlg::OnShowInfo)
ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CSpyDlg::OnShowMenu)
ON_COMMAND(ID__ABOUT, &CSpyDlg::OnTVAbout)
ON_COMMAND(ID_W_CLOSE, &CSpyDlg::OnTVWndClose)
ON_COMMAND(ID_W_HIDE, &CSpyDlg::OnTVWndHide)
ON_COMMAND(ID_W_SHOW, &CSpyDlg::OnTVWndShow)
ON_COMMAND(ID_W_DESTROY, &CSpyDlg::OnTVWndDestroy)
ON_COMMAND(ID_W_QUIT, &CSpyDlg::OnTVWndQuit)
ON_BN_CLICKED(IDC_SEARCH, &CSpyDlg::OnSearchWindow)
ON_BN_CLICKED(IDC_SCAN, &CSpyDlg::OnSpy)
ON_COMMAND(ID_W_VISIBLE, &CSpyDlg::OnTVWndVisible)
ON_COMMAND(ID_W_INVISIBLE, &CSpyDlg::OnTVWndInvisible)
ON_COMMAND(ID_W_ENABLED, &CSpyDlg::OnTVWndEnabled)
ON_COMMAND(ID_W_DISABLED, &CSpyDlg::OnTVWndDisabled)
ON_COMMAND(ID_W_ICONIC, &CSpyDlg::OnTVWndIconic)
ON_COMMAND(ID_W_ZOOMED, &CSpyDlg::OnTVWndZoomed)
ON_COMMAND(ID_T_COLLAPSE, &CSpyDlg::OnTVTreeCollapse)
ON_COMMAND(ID_T_EXPAND, &CSpyDlg::OnTVTreeExpand)
ON_COMMAND(ID_T_TOGGLE, &CSpyDlg::OnTVTreeToggle)
ON_COMMAND(ID_P_SUSPENDED, &CSpyDlg::OnProcSuspended)
ON_COMMAND(ID_P_RESUME, &CSpyDlg::OnProcResume)
ON_COMMAND(ID_P_TERMINATE, &CSpyDlg::OnProcTerminate)
ON_COMMAND(ID_W_TOPMOST, &CSpyDlg::OnWTopmost)
ON_COMMAND(ID_W_UNTOPMOST, &CSpyDlg::OnWUntopmost)
ON_COMMAND(ID_W_TRANSPARENT, &CSpyDlg::OnWTransparent)
ON_COMMAND(ID_W_BLOCK, &CSpyDlg::OnWBlock)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CSpyDlg::OnNMCustomdrawSlider1)
ON_BN_CLICKED(IDC_CHECK1, &CSpyDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CSpyDlg 消息处理程序

BOOL CSpyDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    
    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标
    m_icon.SetIcon(m_hIcon);

    // TODO: 在此添加额外的初始化代码

    selected_wnd = NULL;
	spyWnd = GetSafeHwnd();

    static CImageList image_list; // 图象列表
    image_list.Create(16, 16, ILC_COLOR, 2, 2);
    image_list.SetBkColor(RGB(255,255,255));
    image_list.Add(AfxGetApp()->LoadIcon(IDI_WNDICON));   // 0 代表可视
    image_list.Add(AfxGetApp()->LoadIcon(IDI_WNDICON_H)); // 1 代表不可视
    tree.SetImageList(&image_list, LVSIL_NORMAL); //设置图象列表

	m_slide.SetRange(0, 255);
	m_slide.SetPos(0);
	m_slide.SetLineSize(1);
	m_slide.SetPageSize(10);
	m_slide.EnableWindow(FALSE);
    
    HANDLE hToken;
    BOOL flag = OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken );
    if ( flag )
    {
        SetPrivilege( hToken, SE_DEBUG_NAME, TRUE );
        CloseHandle( hToken );
    }
    else
    {
        GetLastErrorStr( L"OpenProcessToken failed." );
    }
    OnRefreshWindows();

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSpyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSpyDlg::OnPaint()
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
HCURSOR CSpyDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

// 枚举子窗口过程回调函数
BOOL CALLBACK EnumChildWindowsProc(
    HWND hWnd,   // Enum Child Windows: HWND
	LPARAM lParam // (HTREEITEM_Plus_ptrCTreeCtrl*)
    )
{
	if (hWnd == spyWnd)
	{
		return TRUE;
	}

    HPC parameter; // 目录参数 - 窗口
    ZeroMemory(&parameter, sizeof(parameter));

    static WCHAR szWindowText[MAX_BUFFER_LENGTH];
    ZeroMemory(szWindowText, sizeof(szWindowText));

    static WCHAR szClassName[MAX_BUFFER_LENGTH];
    ZeroMemory(szClassName, sizeof(szClassName));

    ::GetWindowText(hWnd, szWindowText, MAX_BUFFER_LENGTH);
    ::GetClassName(hWnd, szClassName, MAX_BUFFER_LENGTH);

	if (_tcscmp(szClassName, L"SWT_Window0") == 0)
	{
		return TRUE;
	}

    CString buf;
    buf.Format(L"子窗口 %p \"%s\" <%s>", hWnd, szWindowText, szClassName);

    TV_INSERTSTRUCT tvChild; // 子目录 - 窗口
    tvChild.hParent = ((lpHPC)lParam)->hItem;
    tvChild.hInsertAfter = TVI_LAST;
    tvChild.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvChild.item.iImage = ::IsWindowVisible(hWnd) == 0;  // 窗口是否可见
    tvChild.item.iSelectedImage = tvChild.item.iImage;
    tvChild.item.pszText = buf.GetBuffer();
    tvChild.item.lParam = (LPARAM)hWnd;

    parameter.hItem = ((lpHPC)lParam)->lpTree->InsertItem(&tvChild);
    parameter.lpTree = ((lpHPC)lParam)->lpTree;

    // 保存hDesktop数据
    parameter.lpTree->SetItemData(parameter.hItem, (DWORD_PTR)hWnd);

    // 枚举子窗口
    EnumChildWindows(hWnd, WNDENUMPROC(EnumChildWindowsProc), (LPARAM)(&parameter));

    // 展开目录
    // ((lpHPC)lParam)->lpTree->Expand(parameter.hItem, TVE_EXPAND);

    return TRUE;
}


// 枚举窗口过程回调函数
BOOL CALLBACK EnumWindowsProc(
    HWND hWnd,   // Enum Windows: HWND
	LPARAM lParam // (HTREEITEM_Plus_ptrCTreeCtrl*)lParam
    )
{
	if (hWnd == spyWnd)
	{
		return TRUE;
	}

    // 提权
    DWORD dwProdessId = 0;
    GetWindowThreadProcessId(hWnd, &dwProdessId);

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProdessId);
    if(hProcess == INVALID_HANDLE_VALUE)
    {
        GetLastErrorStr(L"OpenProcess failed.");
    }

    HPC parameter; // 目录参数 - 窗口
    ZeroMemory(&parameter, sizeof(parameter));

    static WCHAR szWindowText[MAX_BUFFER_LENGTH];
    ZeroMemory(szWindowText, sizeof(szWindowText));

    static WCHAR szClassName[MAX_BUFFER_LENGTH];
    ZeroMemory(szClassName, sizeof(szClassName));

    ::GetWindowText(hWnd, szWindowText, MAX_BUFFER_LENGTH);
    ::GetClassName(hWnd, szClassName, MAX_BUFFER_LENGTH);

    CString buf;
    buf.Format(L"窗口 %p \"%s\" <%s>", hWnd, szWindowText, szClassName);

    TV_INSERTSTRUCT tvWnd; // 目录 - 窗口
    tvWnd.hParent = ((lpHPC)lParam)->hItem;
    tvWnd.hInsertAfter = TVI_LAST;
    tvWnd.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvWnd.item.iImage = ::IsWindowVisible(hWnd) == 0;  // 窗口是否可见
    tvWnd.item.iSelectedImage = tvWnd.item.iImage;
    tvWnd.item.pszText = buf.GetBuffer();
    tvWnd.item.lParam = (LPARAM)hWnd;

    parameter.hItem = ((lpHPC)lParam)->lpTree->InsertItem(&tvWnd);
    parameter.lpTree = ((lpHPC)lParam)->lpTree;

    // 保存hDesktop数据
    parameter.lpTree->SetItemData(parameter.hItem, (DWORD_PTR)hWnd);

    // 枚举窗口
    EnumChildWindows(hWnd, WNDENUMPROC(EnumChildWindowsProc), (LPARAM)(&parameter));

    // 展开目录
    // ((lpHPC)lParam)->lpTree->Expand(parameter.hItem, TVE_EXPAND);

    CloseHandle(hProcess);

    return TRUE;
}


void CSpyDlg::OnRefreshWindows()
{
    // TODO: 在此添加控件通知处理程序代码
    
    tree.DeleteAllItems();

    // parameter - 根目录参数 - root
    HPC parameter;
    ZeroMemory(&parameter, sizeof(parameter));

    static WCHAR szWindowText[MAX_BUFFER_LENGTH];
    ZeroMemory(szWindowText, sizeof(szWindowText));

    static WCHAR szClassName[MAX_BUFFER_LENGTH];
    ZeroMemory(szClassName, sizeof(szClassName));

    HWND hDesktop = GetDesktopWindow()->GetSafeHwnd();
    ::GetWindowText(hDesktop, szWindowText, MAX_BUFFER_LENGTH);
    ::GetClassName(hDesktop, szClassName, MAX_BUFFER_LENGTH);

    CString buf;
    buf.Format(L"Desktop(桌面) %p \"%s\" <%s>", hDesktop, szWindowText, szClassName);       

    TV_INSERTSTRUCT root; // 根目录
    ZeroMemory(&root, sizeof(root));
    root.hParent = TVI_ROOT;
    root.hInsertAfter = TVI_LAST;
    root.item.mask = TVIF_TEXT | TVIF_IMAGE;
    root.item.pszText = buf.GetBuffer();
    root.item.iImage = 0;
    parameter.hItem = tree.InsertItem(&root);
    parameter.lpTree = &tree;

    // 保存hDesktop数据
    tree.SetItemData(parameter.hItem, (DWORD_PTR)hDesktop);

    // 枚举窗口
    EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)(&parameter));


    // 展开目录
    tree.Expand(parameter.hItem, TVE_EXPAND);

    // 选中root
    tree.SelectItem(parameter.hItem);
}

BOOL SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
    )
{
    TOKEN_PRIVILEGES tp;
    LUID luid;

    CString buf;

    if ( !LookupPrivilegeValue( 
        NULL,            // lookup privilege on local system
        lpszPrivilege,   // privilege to lookup 
        &luid ) )        // receives LUID of privilege
    {
        GetLastErrorStr( L"LookupPrivilegeValue failed." );
        return FALSE; 
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if ( bEnablePrivilege )
    {
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    }
    else
    {
        tp.Privileges[0].Attributes = 0;
    }

    // Enable the privilege or disable all privileges.

    if ( !AdjustTokenPrivileges(
        hToken,
        FALSE,
        &tp,
        sizeof(TOKEN_PRIVILEGES),
        (PTOKEN_PRIVILEGES) NULL,
        (PDWORD) NULL) )
    {
        GetLastErrorStr( L"AdjustTokenPrivileges failed." );
        return FALSE; 
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
    {
        GetLastErrorStr( L"The token does not have the specified privilege.\n");
        return FALSE;
    }

    return TRUE;
}

DWORD GetLastErrorStr(
    PWCHAR str) // Error Notification
{
    CString buf; // display string
    LPWSTR format_string = NULL; // format string

    DWORD dwRet = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,    
        NULL,
        GetLastError(),
        MAKELANGID( LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED ),
        (LPWSTR) &format_string,
        0,
        NULL );

    buf.Format( L"Error:\n%s%s\n", str, format_string );
    AfxMessageBox( buf.GetBuffer() );
    buf.ReleaseBuffer();

    if ( dwRet )
    {
        LocalFree( format_string );
    }

    return dwRet;
}

void CSpyDlg::OnShowInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
    //TODO: 在此添加控件通知处理程序代码

    CPoint pos; // 坐标
    GetCursorPos(&pos); // 获取屏幕鼠标坐标
    tree.ScreenToClient(&pos); // 转化成客户坐标

    UINT flag = 0; // 标志
    CString buf; // 缓存
    // 获取当前选择项item句柄
    HTREEITEM item_selected = tree.HitTest(pos, &flag); // 当前选择的item

    tree.SelectItem(item_selected);

    selected_wnd = (HWND)tree.GetItemData(item_selected); // 窗口句柄

    if(selected_wnd == NULL || selected_wnd == GetSafeHwnd() || !IsWindow(selected_wnd))
    {
        AfxMessageBox(L"窗口句柄无效/非法！");
        goto End;
    }

    ShowData(selected_wnd);

End:
    *pResult = 0;
}

void CSpyDlg::ShowData(const HWND &hWnd)
{
    CString buf;
    buf.Format(L"%p", hWnd);
    edit_handle.SetWindowText(buf);

    static WCHAR szWindowText[MAX_BUFFER_LENGTH]; // 窗口标题
    ZeroMemory(szWindowText, sizeof(szWindowText));

    static WCHAR szClassName[MAX_BUFFER_LENGTH]; // 窗口类名
    ZeroMemory(szClassName, sizeof(szClassName));

    // 写标题
    ::GetWindowText(hWnd, szWindowText, MAX_BUFFER_LENGTH);
    if(lstrlen(szWindowText))
    {
        edit_title.SetWindowText(szWindowText);
    }
    else
    {
        edit_title.SetWindowText(L"无");
    }

    // 写类名
    ::GetClassName(hWnd, szClassName, MAX_BUFFER_LENGTH);
    edit_class_name.SetWindowText(szClassName);

    LONG lStyle = GetWindowLong(hWnd, GWL_STYLE);  // 窗口风格  
    buf.Format(L"%p", lStyle);
    edit_style.SetWindowText(buf);

    LONG lExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);  // 窗口扩展风格  
    buf.Format(L"%p", lExStyle);
    edit_exstyle.SetWindowText(buf);

    LONG lClassStyle = GetClassLong(hWnd, GCL_STYLE);  // 窗口类风格  
    buf.Format(L"%p", lClassStyle);
    edit_class_style.SetWindowText(buf);

    DWORD dwProcessId, // 进程标识符
          dwThreadId;  // 线程标识符
    dwThreadId = GetWindowThreadProcessId(hWnd, &dwProcessId);

    buf.Format(L"%p", dwProcessId);
    edit_pid.SetWindowText(buf);

    buf.Format(L"%p", dwThreadId);
    edit_tid.SetWindowText(buf);
    
    // List Window Style
    ListWindowStyle(&list_style, lStyle);

    // List Window Extended Style
    ListWindowExStyle(&list_exstyle, lExStyle);

    // List Class Style
    ListClassStyle(&list_class, lClassStyle);

	// Transparent
	LONG lTrans = ::GetWindowLong(hWnd, GWL_EXSTYLE);
	BOOL bTrans = lTrans & WS_EX_LAYERED;
	m_checkbox.SetCheck(bTrans);
	m_slide.EnableWindow(bTrans);
	if (bTrans)
	{
		BYTE transChannel;
		::GetLayeredWindowAttributes(hWnd, NULL, &transChannel, NULL);
		m_slide.SetPos(transChannel);
	}
	else
	{
		m_slide.SetPos(0);
	}

    // Screen Coordinate
    CRect screen;
    ::GetWindowRect(hWnd, &screen);
    buf.Format(L"(%d, %d)-(%d, %d), %dx%d", screen.left, screen.top,
        screen.right, screen.bottom, screen.Width(), screen.Height());
    edit_screen.SetWindowText(buf);

    // Client Cordinate
    CRect client;
    ::GetClientRect(hWnd, &client);
    buf.Format(L"(%d, %d)-(%d, %d), %dx%d", client.left, client.top,
        client.right, client.bottom, client.Width(), client.Height());
    edit_client.SetWindowText(buf);

    // 窗口重要属性
    ShowAttrbution(hWnd);
}


//////////////////////////////////////////////////////////////

// 宏定义消息显示 - 开始

#define MacroAddString(_style, _str) \
{\
    if ( style & _style)\
    {\
        list->AddString(L##_str);\
    }\
}

#define MacroListStyleBegin(_list) \
{\
    _list->ShowWindow(SW_HIDE);\
    _list->Invalidate(FALSE);\
    _list->ResetContent();\
}

#define MacroListStyleEnd(_list) \
{\
    _list->Invalidate(TRUE);\
    _list->ShowWindow(SW_SHOW);\
    _list->UpdateWindow();\
}

void ListWindowStyle(
    CListBox *list, // List控件
    DWORD style)   // (DWORD) Style
{
    MacroListStyleBegin(list);

    MacroAddString( WS_OVERLAPPED, "WS_OVERLAPPED" );
    MacroAddString( WS_POPUP, "WS_POPUP" );
    MacroAddString( WS_CHILD, "WS_CHILD" );
    MacroAddString( WS_MINIMIZE, "WS_MINIMIZE" );
    MacroAddString( WS_VISIBLE, "WS_VISIBLE" );
    MacroAddString( WS_DISABLED, "WS_DISABLED" );
    MacroAddString( WS_CLIPSIBLINGS, "WS_CLIPSIBLINGS" );
    MacroAddString( WS_CLIPCHILDREN, "WS_CLIPCHILDREN" );
    MacroAddString( WS_MAXIMIZE, "WS_MAXIMIZE" );
    MacroAddString( WS_CAPTION, "WS_CAPTION" );
    MacroAddString( WS_BORDER, "WS_BORDER" );
    MacroAddString( WS_DLGFRAME, "WS_DLGFRAME" );
    MacroAddString( WS_VSCROLL, "WS_VSCROLL" );
    MacroAddString( WS_HSCROLL, "WS_HSCROLL" );
    MacroAddString( WS_SYSMENU, "WS_SYSMENU" );
    MacroAddString( WS_THICKFRAME, "WS_THICKFRAME" );
    MacroAddString( WS_GROUP, "WS_GROUP" );
    MacroAddString( WS_TABSTOP, "WS_TABSTOP" );
    MacroAddString( WS_MINIMIZEBOX, "WS_MINIMIZEBOX" );
    MacroAddString( WS_MAXIMIZEBOX, "WS_MAXIMIZEBOX" );

    MacroListStyleEnd(list);
}

void ListWindowExStyle(
    CListBox *list, // List控件
    DWORD style)   // (DWORD) Style
{
    MacroListStyleBegin(list);

    MacroAddString( WS_EX_DLGMODALFRAME, "WS_EX_DLGMODALFRAME" );
    MacroAddString( WS_EX_NOPARENTNOTIFY, "WS_EX_NOPARENTNOTIFY" );
    MacroAddString( WS_EX_TOPMOST, "WS_EX_TOPMOST" );
    MacroAddString( WS_EX_ACCEPTFILES, "WS_EX_ACCEPTFILES" );
    MacroAddString( WS_EX_TRANSPARENT, "WS_EX_TRANSPARENT" );
    MacroAddString( WS_EX_MDICHILD, "WS_EX_MDICHILD" );
    MacroAddString( WS_EX_TOOLWINDOW, "WS_EX_TOOLWINDOW" );
    MacroAddString( WS_EX_WINDOWEDGE, "WS_EX_WINDOWEDGE" );
    MacroAddString( WS_EX_CLIENTEDGE, "WS_EX_CLIENTEDGE" );
    MacroAddString( WS_EX_CONTEXTHELP, "WS_EX_CONTEXTHELP" );
    MacroAddString( WS_EX_RIGHT, "WS_EX_RIGHT" );
    MacroAddString( WS_EX_LEFT, "WS_EX_LEFT" );
    MacroAddString( WS_EX_RTLREADING, "WS_EX_RTLREADING" );
    MacroAddString( WS_EX_LTRREADING, "WS_EX_LTRREADING" );
    MacroAddString( WS_EX_LEFTSCROLLBAR, "WS_EX_LEFTSCROLLBAR" );
    MacroAddString( WS_EX_RIGHTSCROLLBAR, "WS_EX_RIGHTSCROLLBAR" );
    MacroAddString( WS_EX_CONTROLPARENT, "WS_EX_CONTROLPARENT" );
    MacroAddString( WS_EX_STATICEDGE, "WS_EX_STATICEDGE" );
    MacroAddString( WS_EX_APPWINDOW, "WS_EX_APPWINDOW" );

    MacroListStyleEnd(list);
}

void ListClassStyle(
    CListBox *list, // List控件
    DWORD style)   // (DWORD) Style
{
    MacroListStyleBegin(list);

    MacroAddString( CS_VREDRAW, "CS_VREDRAW" );
    MacroAddString( CS_HREDRAW, "CS_HREDRAW" );
    MacroAddString( CS_DBLCLKS, "CS_DBLCLKS" );
    MacroAddString( CS_OWNDC, "CS_OWNDC" );
    MacroAddString( CS_CLASSDC, "CS_CLASSDC" );
    MacroAddString( CS_PARENTDC, "CS_PARENTDC" );
    MacroAddString( CS_NOCLOSE, "CS_NOCLOSE" );
    MacroAddString( CS_SAVEBITS, "CS_SAVEBITS" );
    MacroAddString( CS_BYTEALIGNCLIENT, "CS_BYTEALIGNCLIENT" );
    MacroAddString( CS_BYTEALIGNWINDOW, "CS_BYTEALIGNWINDOW" );
    MacroAddString( CS_GLOBALCLASS, "CS_GLOBALCLASS" );
    MacroAddString( CS_IME, "CS_IME" );
    MacroAddString( CS_DROPSHADOW, "CS_DROPSHADOW" );

    MacroListStyleEnd(list);
}

#undef MacroAddString
#undef MacroListStyleBegin
#undef MacroListStyleEnd

// 宏定义消息显示 - 结束

//////////////////////////////////////////////////////////////

void CSpyDlg::OnShowMenu(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 在此添加控件通知处理程序代码

    CPoint pos; // 坐标
    GetCursorPos(&pos); // 获取屏幕鼠标坐标

    CPoint client(pos); // 客户坐标
    tree.ScreenToClient(&client); // 转化成客户坐标

    UINT flag = 0; // 标志
    // 获取当前选择项item句柄
    HTREEITEM item_selected = tree.HitTest(client, &flag); // 当前选择的item
    tree.SelectItem(item_selected); // 选中
    selected_wnd = (HWND)tree.GetItemData(item_selected);

    if(selected_wnd == NULL || selected_wnd == GetSafeHwnd() || !IsWindow(selected_wnd))
    {
        AfxMessageBox(L"窗口句柄无效/非法！");
        goto End;
    }

    {
        CMenu menu; // 菜单
        VERIFY(menu.LoadMenu(IDR_TVMENU));

        CMenu *pop_menu = menu.GetSubMenu(0); // 根菜单指针
        ASSERT(pop_menu);
        CWnd *pop_owner = this; // 菜单父窗口

        while(pop_owner->GetStyle() & WS_CHILD)
        {
            pop_owner = pop_owner->GetParent();
        }

        pop_menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pos.x, pos.y, pop_owner);
    }

End:
    *pResult = 0;
}


void CSpyDlg::OnTVAbout()
{
    // TODO: 在此添加命令处理程序代码
    AfxMessageBox(L"Powered by Bajdcc.\n           2014.9");
}


void CSpyDlg::OnTVWndClose()
{
    // TODO: 在此添加命令处理程序代码
    ::CloseWindow(selected_wnd);
}


void CSpyDlg::OnTVWndHide()
{
    // TODO: 在此添加命令处理程序代码
    ::ShowWindow(selected_wnd, SW_HIDE);
}


void CSpyDlg::OnTVWndShow()
{
    // TODO: 在此添加命令处理程序代码
    ::ShowWindow(selected_wnd, SW_SHOWNORMAL);
}


void CSpyDlg::OnTVWndDestroy()
{
    // TODO: 在此添加命令处理程序代码
    ::DestroyWindow(selected_wnd);
}


void CSpyDlg::OnTVWndQuit()
{
    // TODO: 在此添加命令处理程序代码
    ::PostMessage(selected_wnd, WM_QUIT, 0, 0);
}


void CSpyDlg::OnSearchWindow()
{
    // TODO: 在此添加控件通知处理程序代码

    tree.SetFocus();

    WCHAR buf[20];
    edit_search.GetWindowText(buf, 20);
    HWND hWnd; // 要查到的窗口句柄
    try
    {
#ifdef _WIN64
        hWnd = (HWND)wcstoll(buf, NULL, 16);
#else
		hWnd = (HWND)wcstol(buf, NULL, 16);
#endif
    }
    catch(...)
    {
        AfxMessageBox(L"输入错误！");
    }
   
    HTREEITEM root = tree.GetRootItem(); // 根节点
    HTREEITEM result = FindTheWindow(&tree, root, hWnd);

    if(result != NULL)
    {
        // 找到
        tree.SelectItem(result);
        HWND hWnd = (HWND)tree.GetItemData(result); // 窗口句柄
        ShowData(hWnd);
    }
    else
    {
        // 未找到
        AfxMessageBox(L"没有找到！");
    }
}

HTREEITEM FindTheWindow(
    CTreeCtrl*tree, // 控件指针
    HTREEITEM item, // 递归调用指针
    HWND hWnd)      // 要查找的窗口句柄
{
    HTREEITEM find; // 结果

    // 空树返回NULL
    if(item == NULL)
    {
        return NULL;
    }

    // 遍历查找
    while(item != NULL)
    {
        HWND data = (HWND)tree->GetItemData(item);

        // 当前节点即所要查找节点
        if(hWnd == data)
        {
            return item;
        }

        // 查找子节点
        if(tree->ItemHasChildren(item))
        {
            item = tree->GetChildItem(item);

            // 递归调用查找子节点下一个节点
            find = FindTheWindow(tree, item, hWnd);

            if(find)
            {
                return find;
            }
            else
            {
                // 子节点未发现，查找兄弟节点
                item = tree->GetNextSiblingItem(tree->GetParentItem(item));
            }
        }
        else
        {
            // 若无子节点，查找兄弟节点
            item = tree->GetNextSiblingItem(item);
        }
    }

    return item;
}


void CSpyDlg::OnSpy()
{
    // TODO: 在此添加控件通知处理程序代码
    if(m_pInfoDlg != NULL)
    {
        AfxMessageBox(L"已打开Spy窗口！");
        return;
    }
    ShowWindow(SW_HIDE);
    m_pInfoDlg = new CInfoDlg(this);
    try
    {
        m_pInfoDlg->DoModal();
    }
    catch(...)
    {
        AfxMessageBox(L"创建窗口失败！");
        goto End;
    }
    ShowWindow(SW_SHOW); // 显示窗口
    if(m_pInfoDlg != NULL)
    {
        {
            CString buf;
            buf.Format(L"%p", spy_window);
            edit_search.SetWindowText(buf.GetBuffer());
            buf.ReleaseBuffer();
            OnSearchWindow();
            ShowData(spy_window);
        }
        delete m_pInfoDlg;
        goto End;
    }
End:
    spy_window = NULL;
    m_pInfoDlg = NULL;
}

void CSpyDlg::ShowAttrbution(const HWND& hWnd)
{
    LPCWSTR info_str[] = {L"no", L"yes"};
    BOOL is_visable = ::IsWindowVisible(selected_wnd),
         is_unicode = ::IsWindowUnicode(selected_wnd),
         is_enabled = ::IsWindowEnabled(selected_wnd),
         is_iconic = ::IsIconic(selected_wnd),
         is_zoomed = ::IsZoomed(selected_wnd);
    CString buf;
    buf.Format(L"Visable: %s; Unicode: %s; Enabled: %s; Iconic: %s; Zoomed %s;",
        info_str[is_visable],
        info_str[is_unicode],
        info_str[is_enabled],
        info_str[is_iconic],
        info_str[is_zoomed]);
    edit_attr.SetWindowText(buf.GetBuffer()); // 属性
    buf.ReleaseBuffer();

    HTREEITEM selected = tree.GetSelectedItem();
    buf.Format(L"%p", selected);
    edit_treeid.SetWindowText(buf.GetBuffer()); // tree item handle
    buf.ReleaseBuffer();

    ShowImageInfo(hWnd);
}


void CSpyDlg::OnTVWndVisible()
{
    // TODO: 在此添加命令处理程序代码
    LONG lStyle = ::GetWindowLong(selected_wnd, GWL_STYLE); // 窗口风格
    lStyle |= WS_VISIBLE;
    ::SetWindowLong(selected_wnd, GWL_STYLE, lStyle);

    if(::IsWindowVisible(selected_wnd))
    {
        HTREEITEM item_selected = tree.GetSelectedItem(); // 当前选择的item
        tree.SetItemImage(item_selected, 0, 0);
        ShowAttrbution(selected_wnd);
    }
}


void CSpyDlg::OnTVWndInvisible()
{
    // TODO: 在此添加命令处理程序代码
    LONG lStyle = ::GetWindowLong(selected_wnd, GWL_STYLE); // 窗口风格
    lStyle &= ~WS_VISIBLE;
    ::SetWindowLong(selected_wnd, GWL_STYLE, lStyle);

    if(!::IsWindowVisible(selected_wnd))
    {
        HTREEITEM item_selected = tree.GetSelectedItem(); // 当前选择的item
        tree.SetItemImage(item_selected, 1, 1);
        ShowAttrbution(selected_wnd);
    }
}

void CSpyDlg::OnTVWndEnabled()
{
    // TODO: 在此添加命令处理程序代码
    ::EnableWindow(selected_wnd, TRUE);

    if(::IsWindowEnabled(selected_wnd))
    {
        ShowAttrbution(selected_wnd);
    }
}


void CSpyDlg::OnTVWndDisabled()
{
    // TODO: 在此添加命令处理程序代码
    ::EnableWindow(selected_wnd, FALSE);

    if(!::IsWindowEnabled(selected_wnd))
    {
        ShowAttrbution(selected_wnd);
    }
}



void CSpyDlg::OnTVWndIconic()
{
    // TODO: 在此添加命令处理程序代码
    ::ShowWindow(selected_wnd, SW_MINIMIZE);

    if(::IsIconic(selected_wnd))
    {
        ShowAttrbution(selected_wnd);
    }
}


void CSpyDlg::OnTVWndZoomed()
{
    // TODO: 在此添加命令处理程序代码
    ::ShowWindow(selected_wnd, SW_MAXIMIZE);

    if(::IsZoomed(selected_wnd))
    {
        ShowAttrbution(selected_wnd);
    }
}


void CSpyDlg::OnTVTreeCollapse()
{
    // TODO: 在此添加命令处理程序代码
    OnTreeExpand(FALSE);
}


void CSpyDlg::OnTVTreeExpand()
{
    // TODO: 在此添加命令处理程序代码
    OnTreeExpand(TRUE);
}


void CSpyDlg::OnTreeExpand(BOOL is_expanded)
{
    // 收展指令
    UINT code = is_expanded ? TVE_EXPAND : TVE_COLLAPSE;
    int scroll_pos = tree.GetScrollPos(SB_VERT);
    // 选中的句柄
    HTREEITEM item = tree.GetSelectedItem();
    tree.ShowWindow(SW_HIDE);
    tree.Invalidate(FALSE);
    __try
    {
        TreeItemExpand(item, code);
    }
    __except( (GetExceptionCode() == EXCEPTION_STACK_OVERFLOW)
        ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
    {
        AfxMessageBox(L"异常:\n堆栈溢出！");
        int reset = _resetstkoflw();
        if(reset)
        {
            AfxMessageBox(L"堆栈恢复成功！");
        }
        else
        {
            AfxMessageBox(L"堆栈恢复失败！");
        }
    }
    tree.Invalidate(TRUE);
    tree.ShowWindow(SW_SHOW);
    tree.UpdateWindow();
    tree.SetScrollPos(SB_VERT, scroll_pos);
    tree.SetFocus();
    tree.SelectItem(item);
}


void CSpyDlg::OnTVTreeToggle()
{
    // TODO: 在此添加命令处理程序代码
    HTREEITEM item = tree.GetSelectedItem();
    tree.Expand(item, TVE_TOGGLE);
    tree.SelectItem(item);
}


void CSpyDlg::TreeItemExpand(HTREEITEM item, const UINT &code)
{
    // 没有子节点则返回
    if(!tree.ItemHasChildren(item))
    {
        return;
    }

        HTREEITEM kk=tree.GetRootItem();


    // 得到子节点
    HTREEITEM child = tree.GetChildItem(item);

    // 循环遍历，没有下一个节点则返回
    while(child != NULL)
    {
        if(tree.ItemHasChildren(child))
        {
            // 递归收缩/展开子节点
            TreeItemExpand(child, code);
        }

        // 得到下一个节点
        child = tree.GetNextSiblingItem(child);
    }

    // 收缩/展开
    tree.Expand(item, code);
}


void CSpyDlg::ShowImageInfo(const HWND &hWnd)
{
    // 进程快照
    HANDLE snapshot= CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if(INVALID_HANDLE_VALUE == snapshot) // 失败
    {
        GetLastErrorStr(L"CreateToolhelp32Snapshot failed.");
        return;
    }
    else
    {
        PROCESSENTRY32 proc_entry32 = {0};
        proc_entry32.dwSize = sizeof(PROCESSENTRY32);

        BOOL success = Process32First(snapshot, &proc_entry32);
        DWORD pid = 0;
        GetWindowThreadProcessId(hWnd, &pid);
        while(success)
        {
            // 找到进程
            if(proc_entry32.th32ProcessID == pid)
            {
                // 清空
                combo_img_info.ResetContent();
                combo_img_module.ResetContent();
                combo_img_thread.ResetContent();

                edit_img_name.SetWindowText(proc_entry32.szExeFile); // 映像名
                CString buf;
                buf.Format(L"进程ID: %u(0x%p)", proc_entry32.th32ProcessID, proc_entry32.th32ProcessID);
                combo_img_info.AddString(buf.GetBuffer()); // pid
                buf.Format(L"父进程ID: %u(0x%p)", proc_entry32.th32ParentProcessID, proc_entry32.th32ParentProcessID);
                combo_img_info.AddString(buf.GetBuffer()); // parent's pid
                buf.Format(L"线程数: %u", proc_entry32.cntThreads, proc_entry32.cntThreads);
                combo_img_info.AddString(buf.GetBuffer()); // thread counts
                buf.Format(L"基本优先级: %u", proc_entry32.pcPriClassBase, proc_entry32.pcPriClassBase);
                combo_img_info.AddString(buf.GetBuffer()); // thread counts

                // 打开进程
                HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,\
                    FALSE, proc_entry32.th32ProcessID);

                // 无效句柄
                if(INVALID_HANDLE_VALUE == process)
                {
                    GetLastErrorStr(L"OpenProcess failed.");
                    return;
                }

                // 模块句柄数组
                static HMODULE mod_handle[MAX_BUFFER_LENGTH];
                ZeroMemory(&mod_handle, sizeof(mod_handle));

                // 模块名称数组
                static WCHAR mod_name[MAX_PATH];
			    ZeroMemory(&mod_name, sizeof(mod_name));

                // 内存使用计数
                static PROCESS_MEMORY_COUNTERS process_memory_counters;
			    ZeroMemory(&process_memory_counters, sizeof(process_memory_counters));
                process_memory_counters.cb = sizeof(PROCESS_MEMORY_COUNTERS);

                DWORD mod_needed = 0;
                // 枚举进程模块信息
                EnumProcessModules(process, mod_handle, sizeof(mod_handle), &mod_needed);

                // 取得主模块全名，每个进程的第一模块即为进程主模块
                GetModuleFileNameEx(process, mod_handle[0], mod_name, sizeof(mod_name));

                // 取得进程的内存使用信息
                GetProcessMemoryInfo(process, &process_memory_counters, sizeof(process_memory_counters));
                
                BOOL bSrc = FALSE;
                // 路径
                if(lstrlen(mod_name) == 0) lstrcpy(mod_name, L"空");
                else bSrc = TRUE;
                buf.Format(L"路径: \"%s\"", mod_name);
                combo_img_info.AddString(buf.GetBuffer());
                buf.ReleaseBuffer();

                // 获取程序图标
                if (bSrc)
                {
                    SHFILEINFO shfi = {0};
                    SHGetFileInfo(mod_name, 0, &shfi, sizeof(SHFILEINFO),
                        SHGFI_DISPLAYNAME | SHGFI_TYPENAME | SHGFI_ICON);
                    m_icon.SetIcon(shfi.hIcon);

                    buf.Format(L"显示名: \"%s\"", shfi.szDisplayName);
                    combo_img_info.AddString(buf.GetBuffer());
                    buf.ReleaseBuffer();

                    buf.Format(L"类型名: \"%s\"", shfi.szTypeName);
                    combo_img_info.AddString(buf.GetBuffer());
                    buf.ReleaseBuffer();
                }
                else
                {
                    m_icon.SetIcon(m_hIcon);
                }

                ZeroMemory(mod_name, sizeof(mod_name));

                GetProcessImageFileName(process, mod_name, sizeof(mod_name));
                buf.Format(L"设备路径: \"%s\"", mod_name);
                combo_img_info.AddString(buf.GetBuffer());
                buf.ReleaseBuffer();
                ZeroMemory(mod_name, sizeof(mod_name));

                // PageFaultCount
                buf.Format(L"缺页中断次数: %u", process_memory_counters.PageFaultCount);
                combo_img_info.AddString(buf.GetBuffer());

                // PeakWorkingSetSize
                buf.Format(L"使用内存高峰: %u", process_memory_counters.PeakWorkingSetSize);
                combo_img_info.AddString(buf.GetBuffer());

                // WorkingSetSize
                buf.Format(L"工作设置内存: %u", process_memory_counters.WorkingSetSize);
                combo_img_info.AddString(buf.GetBuffer());

                // QuotaPeakPagedPoolUsage
                buf.Format(L"使用页面缓存池高峰: %u", process_memory_counters.QuotaPeakPagedPoolUsage);
                combo_img_info.AddString(buf.GetBuffer());

                // QuotaPagedPoolUsage
                buf.Format(L"使用页面缓存池: %u", process_memory_counters.QuotaPagedPoolUsage);
                combo_img_info.AddString(buf.GetBuffer());

                // QuotaPeakNonPagedPoolUsage
                buf.Format(L"使用非分页缓存池高峰: %u", process_memory_counters.QuotaPeakNonPagedPoolUsage);
                combo_img_info.AddString(buf.GetBuffer());

                // QuotaNonPagedPoolUsage
                buf.Format(L"使用非分页缓存池: %u", process_memory_counters.QuotaNonPagedPoolUsage);
                combo_img_info.AddString(buf.GetBuffer());

                // PagefileUsage
                buf.Format(L"使用分页文件: %u", process_memory_counters.PagefileUsage);
                combo_img_info.AddString(buf.GetBuffer());

                // PeakPagefileUsage
                buf.Format(L"使用分页文件高峰: %u", process_memory_counters.PeakPagefileUsage);
                combo_img_info.AddString(buf.GetBuffer());

                for(DWORD i = 0; i < mod_needed; i++)
                {
                    GetModuleFileNameEx(process, mod_handle[i], mod_name, sizeof(mod_name));
                    if(lstrlen(mod_name) == 0) lstrcpy(mod_name, L"空");
                    combo_img_module.AddString(mod_name);
                    ZeroMemory(mod_name, sizeof(mod_name));
                }

                buf.ReleaseBuffer();

                // 关闭进程句柄
                CloseHandle(process);

                // 线程枚举
                HANDLE thread_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
                if(INVALID_HANDLE_VALUE == snapshot) // 失败
                {
                    GetLastErrorStr(L"CreateToolhelp32Snapshot failed.");
                    break;
                }

                static THREADENTRY32 thread_entry32;
                ZeroMemory(&thread_entry32, sizeof(thread_entry32));
                thread_entry32.dwSize = sizeof(THREADENTRY32);

                BOOL td_success = ::Thread32First(thread_snapshot, &thread_entry32);

                DWORD count_threads = 0;
                while(td_success)
                {
                    // 找到
                    if(thread_entry32.th32OwnerProcessID == pid)
                    {
                        buf.Format(L"TID: %6u (0x%p) BasePri: %u", thread_entry32.th32ThreadID,
                            thread_entry32.th32ThreadID, thread_entry32.tpBasePri);
                        combo_img_thread.AddString(buf.GetBuffer());
                        buf.ReleaseBuffer();
                        count_threads++;
                    }
                    if(count_threads >= proc_entry32.cntThreads)
                    {
                        break;
                    }
                    td_success = Thread32Next(thread_snapshot, &thread_entry32);
                }

                CloseHandle(thread_snapshot);
                break;
            }
            else
            {
                success = Process32Next(snapshot, &proc_entry32);
            }
        }
    }
}

void CSpyDlg::OnProcSuspended()
{
    // TODO: 在此添加命令处理程序代码
    DWORD dwProdessId = 0;
    DWORD dwThreadId = GetWindowThreadProcessId(selected_wnd, &dwProdessId);
    m_hThreadRS = OpenThread(THREAD_SUSPEND_RESUME, FALSE, dwThreadId);
    SuspendThread(m_hThreadRS);
}


void CSpyDlg::OnProcResume()
{
    // TODO: 在此添加命令处理程序代码
    ResumeThread(m_hThreadRS);
}


void CSpyDlg::OnProcTerminate()
{
    // TODO: 在此添加命令处理程序代码
    DWORD dwProdessId = 0;
    GetWindowThreadProcessId(selected_wnd, &dwProdessId);
    HANDLE process = OpenProcess(PROCESS_TERMINATE, FALSE, dwProdessId);
    TerminateProcess(process, 0);
}


void CSpyDlg::OnWTopmost()
{
    // TODO: 在此添加命令处理程序代码
    ::SetWindowPos(selected_wnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}


void CSpyDlg::OnWUntopmost()
{
    // TODO: 在此添加命令处理程序代码
    ::SetWindowPos(selected_wnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}


void CSpyDlg::OnWTransparent()
{
    // TODO: 在此添加命令处理程序代码
    ::SetWindowLong(selected_wnd, GWL_EXSTYLE, GetWindowLong(selected_wnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    ::SetLayeredWindowAttributes(selected_wnd, 0, 128, LWA_ALPHA);
}


void CSpyDlg::OnWBlock()
{
    // TODO: 在此添加命令处理程序代码
    ::SetLayeredWindowAttributes(selected_wnd, 0, 0, 0);
    ::SetWindowLong(selected_wnd, GWL_EXSTYLE, GetWindowLong(selected_wnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
}


void CSpyDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	::SetLayeredWindowAttributes(selected_wnd, 0, (BYTE)m_slide.GetPos() , LWA_ALPHA);
	*pResult = 0;
}


void CSpyDlg::OnBnClickedCheck1()
{
	// TODO:  在此添加控件通知处理程序代码
	BOOL bTrans = m_checkbox.GetCheck();
	m_slide.EnableWindow(bTrans);
	if (bTrans)
	{
		::SetWindowLong(selected_wnd, GWL_EXSTYLE, GetWindowLong(selected_wnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		::SetLayeredWindowAttributes(selected_wnd, 0, 128, LWA_ALPHA);
		m_slide.SetPos(128);
		m_slide.EnableWindow(TRUE);
	}
	else
	{
		::SetLayeredWindowAttributes(selected_wnd, 0, 0, 0);
		::SetWindowLong(selected_wnd, GWL_EXSTYLE, GetWindowLong(selected_wnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
		m_slide.SetPos(0);
		m_slide.EnableWindow(FALSE);
	}
}
