#pragma once
#include "afxwin.h"


// CInfoDlg 对话框

class CInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInfoDlg)

public:
	CInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInfoDlg();

// 对话框数据
	enum { IDD = IDD_INFO };

protected:
    BOOL is_scanning; // 是否正在扫描
    BOOL is_found; // 是否已经找到
    HICON icon_from; // origin icon
    HICON icon_blank; // blank icon
    HCURSOR cursor_scan; // 鼠标
    HWND saved_win_handle; // 保存的扫描句柄
    HWND parent_handle; // 主界面Spy句柄
    HWND prev_window; // 扫描的上一个窗口

// 扫描函数
    void SpyStartScanning(); // beginning of scan
    void SpyStopScanning(); // end of scan
    void SpyExecScanning(CPoint &pt); // process of scan
    HWND SpyFindSmallestWindow(const CPoint &pt); // find the smallest window
    void SpyInvertBorder(const HWND &hWnd); // invert the window border

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CEdit edit_pos;
    CStatic scan;
    afx_msg void OnClose();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
