
// SpyDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "InfoDlg.h"


typedef struct _HTREEITEM_Plus_ptrCTreeCtrl
{
    HTREEITEM  hItem;
    CTreeCtrl* lpTree;
}HPC, *lpHPC;


// CSpyDlg 对话框
class CSpyDlg : public CDialogEx
{
// 构造
public:
	CSpyDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SPY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
    HICON m_hIcon;
    CInfoDlg *m_pInfoDlg; // 信息对话框指针
    void ShowAttrbution(const HWND &hWnd); // 显示属性
    void ShowData(const HWND &hWnd); // 显示信息
    void OnTreeExpand(BOOL is_expanded); // 递归收展节点
    void TreeItemExpand(HTREEITEM item, const UINT &code); // 递归收展节点
    void ShowImageInfo(const HWND &hWnd); // 显示映像信息


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CTreeCtrl tree;
    afx_msg void OnRefreshWindows();
    afx_msg void OnShowInfo(NMHDR *pNMHDR, LRESULT *pResult);
    CEdit edit_class_name;
    CEdit edit_exstyle;
    CEdit edit_handle;
    CEdit edit_style;
    CEdit edit_title;
    CListBox list_class;
    CListBox list_exstyle;
    CListBox list_style;
    CEdit edit_class_style;
    CEdit edit_pid;
    CEdit edit_tid;
    CEdit edit_client;
    CEdit edit_screen;
    afx_msg void OnShowMenu(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTVAbout();
    HWND selected_wnd;
    afx_msg void OnTVWndClose();
    afx_msg void OnTVWndHide();
    afx_msg void OnTVWndShow();
    afx_msg void OnTVWndDestroy();
    afx_msg void OnTVWndQuit();
    afx_msg void OnSearchWindow();
    CEdit edit_search;
    CButton btn_search;
    CButton btn_refresh;
    afx_msg void OnSpy();
    HWND spy_window;
    CButton btn_scan;
    afx_msg void OnTVWndVisible();
    afx_msg void OnTVWndInvisible();
    CEdit edit_attr;
    afx_msg void OnTVWndEnabled();
    afx_msg void OnTVWndDisabled();
    afx_msg void OnTVWndIconic();
    afx_msg void OnTVWndZoomed();
    afx_msg void OnTVTreeCollapse();
    afx_msg void OnTVTreeExpand();
    afx_msg void OnTVTreeToggle();
    CEdit edit_treeid;
    CEdit edit_img_name;
    CComboBox combo_img_info;
    CComboBox combo_img_module;
    CComboBox combo_img_thread;
    CStatic m_icon;
	CSliderCtrl m_slide;
	CButton m_checkbox;
    afx_msg void OnProcSuspended();
    afx_msg void OnProcResume();
    afx_msg void OnProcTerminate();

private:
    HANDLE m_hThreadRS;
public:
    afx_msg void OnWTopmost();
    afx_msg void OnWUntopmost();
    afx_msg void OnWTransparent();
    afx_msg void OnWBlock();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheck1();
};

extern BOOL CALLBACK EnumChildWindowsProc(
    HWND hWnd,   // Enum Child Windows: HWND
    DWORD lParam // (HTREEITEM_Plus_ptrCTreeCtrl*)
    );

extern BOOL CALLBACK EnumWindowsProc(
    HWND hWnd,   // Enum Windows: HWND
    DWORD lParam // (HTREEITEM_Plus_ptrCTreeCtrl*)lParam
    );

extern BOOL SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
    );

extern DWORD GetLastErrorStr(
    PWCHAR str);

extern void ListWindowStyle(
    CListBox *list, // List控件
    DWORD syle);   // (DWORD) Style

extern void ListWindowExStyle(
    CListBox *list, // List控件
    DWORD style);   // (DWORD) Style

extern void ListClassStyle(
    CListBox *list, // List控件
    DWORD style);   // (DWORD) Style

extern HTREEITEM FindTheWindow(
    CTreeCtrl*tree, // 控件指针
    HTREEITEM item, // 递归调用指针
    HWND hWnd);     // 要查找的窗口句柄

int const MAX_BUFFER_LENGTH = 255;