// InfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Spy.h"
#include "InfoDlg.h"
#include "afxdialogex.h"
#include "SpyDlg.h"

// CInfoDlg 对话框

IMPLEMENT_DYNAMIC(CInfoDlg, CDialogEx)

    CInfoDlg::CInfoDlg(CWnd* pParent /*= Spy Window*/)
    : CDialogEx(CInfoDlg::IDD, pParent)
{
    // 初始化
    icon_from = AfxGetApp()->LoadIcon(IDI_FROM);
    icon_blank = AfxGetApp()->LoadIcon(IDI_SCAN);
    cursor_scan = AfxGetApp()->LoadCursor(IDC_SCAN);
    saved_win_handle = NULL;
    is_scanning = FALSE;
    prev_window = NULL;
}

CInfoDlg::~CInfoDlg()
{
    DestroyCursor(cursor_scan);
    DestroyIcon(icon_from);
    DestroyIcon(icon_blank);
}

void CInfoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_POS, edit_pos);
    DDX_Control(pDX, IDC_SPYPIC, scan);
}


BEGIN_MESSAGE_MAP(CInfoDlg, CDialogEx)
    ON_WM_CLOSE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CInfoDlg 消息处理程序


void CInfoDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if(saved_win_handle != INVALID_HANDLE_VALUE)
    {
        ::ShowWindowAsync(saved_win_handle, SW_SHOW); // 强制显示
    }
    CDialogEx::OnClose();
}


void CInfoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    // 鼠标所在窗口
    CWnd *pWnd = ChildWindowFromPoint(point);

    // 若单击Spy_Scan_Icon，启动扫描
    if(pWnd != NULL && pWnd->GetSafeHwnd() == scan.GetSafeHwnd())
    {
        SpyStartScanning();
    }

    CDialogEx::OnLButtonDown(nFlags, point);
}


void CInfoDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    // 停止扫描
    if(is_scanning)
    {
        SpyStopScanning();
    }

    CDialogEx::OnLButtonUp(nFlags, point);
}


void CInfoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    // 扫描中
    if(is_scanning)
    {
        SpyExecScanning(point);
    }

    CDialogEx::OnMouseMove(nFlags, point);
}


void CInfoDlg::SpyStartScanning()
{
    is_scanning = TRUE;
    scan.SetIcon(icon_blank);
    SetCursor(cursor_scan);

    SetCapture(); // 设置鼠标捕获
}


void CInfoDlg::SpyStopScanning()
{
    is_scanning = FALSE;
    scan.SetIcon(icon_from);
    SpyInvertBorder(saved_win_handle);
    ReleaseCapture();
    CSpyDlg* spy_dlg_ptr = (CSpyDlg*)m_pParentWnd;
    spy_dlg_ptr->spy_window = saved_win_handle;
    HCURSOR cursor_normal = LoadCursor(NULL, IDC_ARROW);
    SetCursor(cursor_normal);

    Sleep(100);
    SendMessage(WM_CLOSE);
}


void CInfoDlg::SpyExecScanning(CPoint &pt)
{
    ClientToScreen(&pt); // 转换到屏幕坐标

    {
        CString buf;
        buf.Format(L"(%d, %d)", pt.x, pt.y);
        edit_pos.SetWindowText(buf.GetBuffer());
        buf.ReleaseBuffer();
    }

    HWND current_window = SpyFindSmallestWindow(pt); //找到当前位置的最小窗口

    if(current_window != NULL)
    {
        // 确保找到的窗口不是自己的窗口，窗口进程不同
        if(GetWindowThreadProcessId(GetSafeHwnd(), NULL) != GetWindowThreadProcessId(current_window, NULL))
        {
            // 若是新窗口，就把旧窗口的边界去掉，画新窗口的边界
            if(current_window != prev_window)
            {	
                SpyInvertBorder(prev_window);
                prev_window = current_window;
                SpyInvertBorder(prev_window);
            }
        }
        else
        {
            // 窗口是自己的
            SpyInvertBorder(prev_window);
            prev_window = NULL;
        }
    }

    saved_win_handle = prev_window;

    UpdateData(FALSE); // 更新数据
}


HWND CInfoDlg::SpyFindSmallestWindow(const CPoint &pt)
{
    HWND hWnd = WindowFromPoint(pt)->GetSafeHwnd(); // 鼠标所在窗口

    if(hWnd != NULL)
    {
        // 得到本窗口大小和父窗口句柄，以便比较
        CRect rect;
        ::GetWindowRect(hWnd, &rect);
        HWND parent = ::GetParent(hWnd); // 父窗口

        // 只有该窗口有父窗口才继续比较
        if(parent != NULL)
        {
            // 按Z方向搜索
            HWND find = hWnd; // 递归调用句柄
            CRect rect_find;

            while(TRUE) // 循环
            {
                find = ::GetWindow(find, GW_HWNDNEXT); // 得到下一个窗口的句柄
                ::GetWindowRect(find, &rect_find); // 得到下一个窗口的大小

                if( ::PtInRect(&rect_find, pt) // 鼠标所在位置是否在新窗口里
                    && ::GetParent(find) == parent // 新窗口的父窗口是否是鼠标所在主窗口
                    && ::IsWindowVisible(find)) // 窗口是否可视
                {
                    // 比较窗口，看哪个更小
                    if(rect_find.Width() * rect_find.Height() < rect.Width() * rect.Height())
                    {
                        // 找到更小窗口
                        hWnd = find;

                        // 计算新窗口的大小
                        ::GetWindowRect(hWnd, &rect);
                    }
                }

                // hWnd的子窗口find为NULL，则hWnd为最小窗口
                if(find == NULL)
                {
                    break; // 退出循环
                }
            }
        }
    }

    return hWnd;
}


void CInfoDlg::SpyInvertBorder(const HWND &hWnd)
{
    // 若非窗口则返回
    if(!IsWindow(hWnd))
        return;

    CRect rect; // 窗口矩形

    // 得到窗口矩形
    ::GetWindowRect(hWnd, &rect);

    CDC dc; // 窗口设备上下文
    dc.Attach(::GetWindowDC(hWnd));

    // 设置窗口当前前景色的混合模式为R2_NOT
    // R2_NOT - 当前的像素值为屏幕像素值的取反，这样可以覆盖掉上次的绘图
    dc.SetROP2(R2_NOT);

    // 创建画笔
    CPen pen;

    // PS_INSIDEFRAME - 产生封闭形状的框架内直线，指定一个限定矩形
    // 3 * GetSystemMetrics(SM_CXBORDER) - 三倍边界粗细
    // RGB(0,0,0) - 黑色
    pen.CreatePen(PS_INSIDEFRAME, 3 * GetSystemMetrics(SM_CXBORDER), RGB(0, 0, 0));

    // 选择画笔
    HGDIOBJ old_pen = dc.SelectObject(pen);

    // 设定画刷
    HGDIOBJ old_brush = dc.SelectObject(GetStockObject(NULL_BRUSH));

    // 画矩形
    dc.Rectangle(0, 0, rect.Width(), rect.Height());

    // 恢复原来的设备环境
    dc.SelectObject(old_pen);
    dc.SelectObject(old_brush);

    pen.DeleteObject();
}