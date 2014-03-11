#include "StdAfx.h"
#include "UIRichList.h"


CRichListUI::CRichListUI(void)
{
}


CRichListUI::~CRichListUI(void)
{
}

void CRichListUI::setDelegate(CRichListUIDelegate* pDelegate)
{
	m_pDelegate = pDelegate;
}

CRichListUIDelegate *CRichListUI::getDelegate(void)
{
	return m_pDelegate;
}

void CRichListUI::DrawItemBk(HDC hDC, const RECT& rcItem)
{
    ASSERT(m_pOwner);
    if( m_pOwner == NULL ) return;
    TListInfoUI* pInfo = m_pOwner->GetListInfo();
    DWORD iBackColor = 0;
    if( !pInfo->bAlternateBk || m_iIndex % 2 == 0 ) iBackColor = pInfo->dwBkColor;
    if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        iBackColor = pInfo->dwHotBkColor;
    }
    if( IsSelected() ) {
        iBackColor = pInfo->dwSelectedBkColor;
    }
    if( !IsEnabled() ) {
        iBackColor = pInfo->dwDisabledBkColor;
    }

    if ( iBackColor != 0 ) {
        CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(iBackColor));
    }

    if( !IsEnabled() ) {
        if( !pInfo->sDisabledImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)pInfo->sDisabledImage) ) pInfo->sDisabledImage.Empty();
            else return;
        }
    }
    if( IsSelected() ) {
        if( !pInfo->sSelectedImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)pInfo->sSelectedImage) ) pInfo->sSelectedImage.Empty();
            else return;
        }
    }
    if( (m_uButtonState & UISTATE_HOT) != 0 ) {
        if( !pInfo->sHotImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)pInfo->sHotImage) ) pInfo->sHotImage.Empty();
            else return;
        }
    }

    if( !m_sBkImage.IsEmpty() ) {
        if( !pInfo->bAlternateBk || m_iIndex % 2 == 0 ) {
            if( !DrawImage(hDC, (LPCTSTR)m_sBkImage) ) m_sBkImage.Empty();
        }
    }

    if( m_sBkImage.IsEmpty() ) {
        if( !pInfo->sBkImage.IsEmpty() ) {
            if( !DrawImage(hDC, (LPCTSTR)pInfo->sBkImage) ) pInfo->sBkImage.Empty();
            else return;
        }
    }

    if ( pInfo->dwLineColor != 0 ) {
        RECT rcLine = { m_rcItem.left, m_rcItem.bottom - 1, m_rcItem.right, m_rcItem.bottom - 1 };
        CRenderEngine::DrawLine(hDC, rcLine, 1, GetAdjustColor(pInfo->dwLineColor));
    }
}