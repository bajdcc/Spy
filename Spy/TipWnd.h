#pragma once
#include "afxwin.h"
class CTipWnd :
    public CWnd
{
public:
    CTipWnd(void);
    ~CTipWnd(void);

protected:
    CWnd *m_pParentWnd; // ¸¸´°¿Ú¾ä±ú
};

