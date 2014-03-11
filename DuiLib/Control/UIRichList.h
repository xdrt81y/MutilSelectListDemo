#ifndef __UIRICHLIST_H__
#define __UIRICHLIST_H__

#pragma once
#include "Layout/UIVerticalLayout.h"
#include "Layout/UIHorizontalLayout.h"

namespace DuiLib {
/////////////////////////////////////////////////////////////////////////////////////
//
class CRichListHeaderUI;

#define UILIST_MAX_COLUMNS 32

/////////////////////////////////////////////////////////////////////////////////////
//



class CRichListUI;

class CRichListUIDelegate
{
public:
    virtual LPCTSTR GetItemText(CControlUI* pList, int iItem, int iSubItem) = 0;
};

class CRichListUI :	public CListUI
{
public:
	CRichListUI(void);
	~CRichListUI(void);

	void setDelegate(CRichListUIDelegate*);
	CRichListUIDelegate *getDelegate(void);

	void DrawItemBk(HDC hDC, const RECT& rcItem);
private:
	CRichListUIDelegate *m_pDelegate;
};

}
#endif