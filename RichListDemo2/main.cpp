#include "StdAfx.h"
#include "MenuWnd.h"


struct CustomListItem
{
	wstring title;
	wstring icon;
	wstring appbid;
	wstring appversion;
	wstring backuptime;
	wstring filesize;
	wstring filepath;
	
	CustomListItem() 
	{ 
		memset(this, 0, sizeof(CustomListItem));
	}
	~CustomListItem()
	{
	}
};

// 控件key
const TCHAR* const kTitleControlName = _T("title");
const TCHAR* const kIconControlName = _T("icon");
const TCHAR* const kAppVersionControlName = _T("appversion");
const TCHAR* const kAppBIDControlName = _T("appbid");
const TCHAR* const kFileSizeControlName = _T("filesize");
const TCHAR* const kBackupTimeControlName = _T("backuptime");
const TCHAR* const kFilePathControlName =_T("filepath");


const TCHAR* const kRowOptionCheckBoxControlName = _T("rowOptionBackup");
const TCHAR* const kRecoveryButtonControlName = _T("recovery");
const TCHAR* const kGifRecoveryingButtonControlName = _T("btnGifRecoverying");
const TCHAR* const kRecoveryingLabelControlName = _T("lblRecoverying");
const TCHAR* const kDeleteButtonControlName = _T("delete");
const TCHAR* const kCheckButtonControlName = _T("checkbackup");
class CCustomListUI:public CListUI ,
	public INotifyUI 
{
public:
	CCustomListUI(CPaintManagerUI& PM):m_PM(PM)
	{
		m_bStartRect = FALSE;
		LoadData();
	}
	~CCustomListUI(void){}

bool Add(CControlUI* pControl)
{
	if (!pControl)
		return false;

	return CListUI::Add(pControl);
}

bool AddAt(CControlUI* pControl, int iIndex)
{
	if (!pControl)
		return false;


	return CListUI::AddAt(pControl, iIndex);
}

void AddNode(const CustomListItem& item)
{
	TCHAR szBuf[MAX_PATH] = {0};

	CListContainerElementUI* pListElement = NULL;

	// 需要传入回调接口this，以识别自定义控件ButtonGif
	if( !m_dlgBuilder.GetMarkup()->IsValid() ) {

		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("ListItem_Custom.xml"), (UINT)0, NULL, &m_PM));
	}
	else {
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(NULL, &m_PM));
	}
	if (pListElement == NULL)
		return;

	CControlUI* icon_control = static_cast<CControlUI*>(m_PM.FindSubControlByName(pListElement, kIconControlName));
	if (icon_control != NULL)
	{
		icon_control->SetBkImage(item.icon.c_str());
		icon_control->SetTag((UINT_PTR)pListElement);
		//log_button->OnEvent += MakeDelegate(&OnLogoButtonEvent);
	}

	

	CLabelUI* title = static_cast<CLabelUI*>(m_PM.FindSubControlByName(pListElement, kTitleControlName));
	if (title != NULL)
	{
		title->SetShowHtml(true);
		title->SetText(item.title.c_str());
	}

	CLabelUI* appbid = static_cast<CLabelUI*>(m_PM.FindSubControlByName(pListElement, kAppBIDControlName));
	if (appbid != NULL)
	{
		appbid->SetShowHtml(true);
		appbid->SetText(item.appbid.c_str());
	}

	{
		CLabelUI* appversion = static_cast<CLabelUI*>(m_PM.FindSubControlByName(pListElement, kAppVersionControlName));
		if (appversion != NULL)
		{
			appversion->SetShowHtml(true);
			appversion->SetText(item.appversion.c_str());
		}
	}

	{
		CLabelUI* backuptime = static_cast<CLabelUI*>(m_PM.FindSubControlByName(pListElement, kBackupTimeControlName));
		if (backuptime != NULL)
		{
			backuptime->SetShowHtml(true);
			backuptime->SetText(item.backuptime.c_str());
		}
	}

	CLabelUI* filesize = static_cast<CLabelUI*>(m_PM.FindSubControlByName(pListElement, kFileSizeControlName));
	if (filesize != NULL)
	{
		filesize->SetShowHtml(true);
		filesize->SetText(item.filesize.c_str());
	}

	CLabelUI* filepath = static_cast<CLabelUI*>(m_PM.FindSubControlByName(pListElement, kFilePathControlName));
	if (filepath != NULL)
	{
		filepath->SetShowHtml(true);
		filepath->SetText(item.filepath.c_str());
	}


	CButtonUI* recovery_button = static_cast<CButtonUI*>(m_PM.FindSubControlByName(pListElement, kRecoveryButtonControlName));
	if (recovery_button != NULL)
	{
		recovery_button->SetTag((UINT_PTR)pListElement);
	}

	CButtonUI* delete_button = static_cast<CButtonUI*>(m_PM.FindSubControlByName(pListElement, kDeleteButtonControlName));
	if (delete_button != NULL)
	{
		delete_button->SetTag((UINT_PTR)pListElement);
	}

	CButtonUI* check_button = static_cast<CButtonUI*>(m_PM.FindSubControlByName(pListElement, kCheckButtonControlName));
	if (check_button != NULL)
	{
		check_button->SetTag((UINT_PTR)pListElement);
	}

	if (!CListUI::Add(pListElement))
	{
		delete pListElement;
	}
}

bool Remove(CControlUI* pControl)
{
	if (!pControl)
		return false;

	return CListUI::Remove(pControl);
}

bool RemoveAt(int iIndex)
{
	CControlUI* pControl = GetItemAt(iIndex);
	if (!pControl)
		return false;

	return CListUI::RemoveAt(iIndex);
}

void RemoveAll()
{
	CListUI::RemoveAll();
}

bool SelectItem(int iIndex, bool bTakeFocus)
{

	if (!__super::SelectItem(iIndex, bTakeFocus))
		return false;
	
	return true;
}
bool HotItem(int iIndex, bool bHot)
{
	return true;
}
bool ClickItem(int iIndex, bool bTakeFocus)
{
	BOOL bCtrl = (GetKeyState(VK_CONTROL) & 0x8000);
	BOOL bShift = (GetKeyState(VK_SHIFT) & 0x8000);
	// 取消之前的选择
	if(!bCtrl && !bShift){
		for(int i=0;i<GetCount();i++){
			CControlUI* pControl = GetItemAt(i);
			if(i!=iIndex && pControl != NULL) {
				CListContainerElementUI* pItem = static_cast<CListContainerElementUI*>(pControl);
				COptionUI* pCheckBox = static_cast<COptionUI*>(m_PM.FindSubControlByName(pItem, kRowOptionCheckBoxControlName));
				if(pCheckBox->IsSelected()==true){
					pCheckBox->Selected(false);
				}
			}
		}
	}else if(bShift){
		int i=0;
		int iFirst = m_iCurSel;
		int iLast = iIndex;
		int iCount = GetCount();
		
		if(iIndex == m_iCurSel){
			CControlUI* pControl = GetItemAt(iIndex);
			if( pControl != NULL) {
				CListContainerElementUI* pItem = static_cast<CListContainerElementUI*>(pControl);
				COptionUI* pCheckBox = static_cast<COptionUI*>(m_PM.FindSubControlByName(pItem, kRowOptionCheckBoxControlName));
				pCheckBox->Selected(pItem->IsSelected());
			}
			return true;

		}
		//locate (and select) either first or last
		// (so order is arbitrary)
		while(i<iCount){
			if(i==iFirst || i == iLast){
				i++;
				break;
			}

			CControlUI* pControl = GetItemAt(i);
			if( pControl != NULL) {
				CListContainerElementUI* pItem = static_cast<CListContainerElementUI*>(pControl);
				COptionUI* pCheckBox = static_cast<COptionUI*>(m_PM.FindSubControlByName(pItem, kRowOptionCheckBoxControlName));
				if(pCheckBox != NULL){
					pCheckBox->Selected(false);
				}
			}
			i++;
		}

		// select rest of range
		while(i<iCount){
			CControlUI* pControl = GetItemAt(i);
			if( pControl != NULL) {
				CListContainerElementUI* pItem = static_cast<CListContainerElementUI*>(pControl);
				COptionUI* pCheckBox = static_cast<COptionUI*>(m_PM.FindSubControlByName(pItem, kRowOptionCheckBoxControlName));
				if(pCheckBox != NULL){
					pCheckBox->Selected(true);
				}
			}
			if(i==iFirst || i == iLast){
				i++;
				break;
			}
			i++;
		}

		// unselect rest of range
		while(i<iCount){
			CControlUI* pControl = GetItemAt(i);
			if( pControl != NULL) {
				CListContainerElementUI* pItem = static_cast<CListContainerElementUI*>(pControl);
				COptionUI* pCheckBox = static_cast<COptionUI*>(m_PM.FindSubControlByName(pItem, kRowOptionCheckBoxControlName));
				if(pCheckBox != NULL){
					pCheckBox->Selected(false);
				}
			}
			i++;
		}
	}

    CControlUI* pControl = GetItemAt(iIndex);
	if( pControl != NULL) {
		CListContainerElementUI* pItem = static_cast<CListContainerElementUI*>(pControl);
		COptionUI* pCheckBox = static_cast<COptionUI*>(m_PM.FindSubControlByName(pItem, kRowOptionCheckBoxControlName));
		pCheckBox->Selected(pItem->IsSelected());
	}
	return true;
}

void UpdateSelectionForRect(RECT rect)
{
	RECT rcTemp = { 0 };
	for (int iIndex = 0; iIndex<GetCount();iIndex++)
	{
		CControlUI * pControl = GetItemAt(iIndex);
		if( pControl == NULL) { continue;}
		CListContainerElementUI* pListElement = static_cast<CListContainerElementUI*>(pControl);
		COptionUI * pOption = static_cast<COptionUI*>(pListElement->FindSubControl(kRowOptionCheckBoxControlName));
		if(pOption == NULL) return;
		RECT rectItem = pListElement->GetPos();

		if( ::IntersectRect(&rcTemp, &rect, &pListElement->GetPos()) )
		{//item in rect
			pListElement->Select(true,false);
			pOption->Selected(true);
		}else{//item not in rect
			pListElement->Select(false,false);
			pOption->Selected(false);
		}
	}
}

void DoEvent(TEventUI& event){
	BOOL bShift = (GetKeyState(VK_SHIFT) & 0x8000);
	BOOL bCtrl = (GetKeyState(VK_CONTROL) & 0x8000);
	if(bShift || bCtrl){
		CListUI::DoEvent(event);
		return;
	}

	switch( event.Type ) {
	case UIEVENT_BUTTONDOWN://左键
	case UIEVENT_RBUTTONDOWN://右键
		{
			m_bStartRect = true;
			m_startPoint = event.ptMouse; //记录开始点
			SetCapture(m_PM.GetPaintWindow());
		}
		break;
	case UIEVENT_MOUSEMOVE:
		//if(GetCapture() == m_PM.GetPaintWindow()){
		if(m_bStartRect){
			HWND hwnd = m_PM.GetPaintWindow();
			RECT rcClient;
			POINT point = event.ptMouse;
			RECT rect = {0};
			//有效防止点击事件，进入滑动事件处理
			if((point.x >= m_startPoint.x-20 && point.x <= m_startPoint.x+20 ) 
				&& (point.y >= m_startPoint.y-20 && point.y <= m_startPoint.y+20)) break;
			::GetClientRect(hwnd, &rcClient);
			::InvalidateRect(hwnd, &rcClient, FALSE);
			
			
			if(point.x<m_startPoint.x && point.y < m_startPoint.y){
				RECT rectTemp = {point.x,point.y,m_startPoint.x,m_startPoint.y};
				rect  = rectTemp;
			}else if(point.x>m_startPoint.x && point.y < m_startPoint.y){
				RECT rectTemp = {m_startPoint.x,point.y,point.x,m_startPoint.y};
				rect  = rectTemp;
			}else if(point.x<m_startPoint.x && point.y > m_startPoint.y){
				RECT rectTemp = {point.x,m_startPoint.y,m_startPoint.x,point.y};
				rect  = rectTemp;
			}else {
				RECT rectTemp = {m_startPoint.x,m_startPoint.y,point.x,point.y};
				rect  = rectTemp;
			}

			// 滚动条
			/*RECT rcList = GetPos();
			TEventUI eventScrollWheel = { 0 };
			eventScrollWheel.Type = UIEVENT_SCROLLWHEEL;
			if (point.y < rcList.top)
				eventScrollWheel.wParam = MAKELPARAM(SB_LINEUP, 0);
			else if (point.y >= rcList.bottom)
				eventScrollWheel.wParam = MAKELPARAM(SB_LINEDOWN, 0);

			eventScrollWheel.lParam = event.lParam;
			eventScrollWheel.dwTimestamp = ::GetTickCount();
			CListUI::DoEvent(eventScrollWheel);
			EnsureVisible(GetCurSel());*/

			DWORD bkColor2 = this->GetItemBkColor();
			UpdateSelectionForRect(rect);
			CRenderEngine::DrawRect(m_PM.GetPaintDC(),rect,1,bkColor2);//draw
		}
		break;
	case UIEVENT_BUTTONUP:
		{
			m_bStartRect = false;

			if(GetCapture() == m_PM.GetPaintWindow()){
				ReleaseCapture();

				HWND hwnd = m_PM.GetPaintWindow();
				InvalidateRect(hwnd,NULL,TRUE);
			}
		}
		break;
	case UIEVENT_RBUTTONUP:
		{
			m_bStartRect = false;

			if(GetCapture() == m_PM.GetPaintWindow()){
				ReleaseCapture();

				HWND hwnd = m_PM.GetPaintWindow();
				InvalidateRect(hwnd,NULL,TRUE);
			}
		}
		
	case UIEVENT_CONTEXTMENU:
		if( IsContextMenuUsed() ) {
			m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
			return;
		}
		break;
	}
	CListUI::DoEvent(event);
}
virtual void Notify(TNotifyUI& msg){
	if(msg.sType == _T("menu")) 
	{
		//if( msg.pSender->GetName() != _T("domainlist") ) return;
		CMenuWnd* pMenu = new CMenuWnd();
		if( pMenu == NULL ) { return; }
		POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
		::ClientToScreen(m_PM.GetPaintWindow(), &pt);
		pMenu->Init(msg.pSender, pt);
		}
}
void LoadData(){
	CustomListItem item;
	item.appbid = L"";
	item.appversion = L"1.0.0";
	item.backuptime = L"2014-01-01 10:20";
	item.filepath = L"";
	item.filesize = L"30M";
	item.icon = L"";
	item.title = L"网易新闻";
	
	AddNode(item);
	AddNode(item);
	AddNode(item);
	AddNode(item);
	AddNode(item);
	AddNode(item);
	AddNode(item);
	AddNode(item);
}
private:
	CPaintManagerUI& m_PM;
    CDialogBuilder m_dlgBuilder;
	bool m_bStartRect;
	POINT m_startPoint; //矩形框开始点
};

class ListMainForm : public CWindowWnd, public INotifyUI, public IDialogBuilderCallback
{
public:
    ListMainForm() {
    };

    LPCTSTR GetWindowClassName() const 
    { 
        return _T("ScanMainForm"); 
    };

    UINT GetClassStyle() const
    { 
        return CS_DBLCLKS; 
    };

    void OnFinalMessage(HWND /*hWnd*/) 
    { 
        delete this;
    };

    void Init() 
    {
        m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
        m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
        m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
        m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
        m_pSearch = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn")));
    }

    void OnPrepare(TNotifyUI& msg) 
    {

    }
    //static DWORD WINAPI Search(LPVOID lpParameter)
    //{
    //    try
    //    {
    //        struct Prama* prama = (struct Prama *)lpParameter;
    //        CListUI* pList = prama->pList;
    //        CButtonUI* pSearch = prama->pSearch;
    //        CDuiString tDomain = prama->tDomain;
    //        //-------------------------------------
    //        /*
    //        * 添加数据循环
    //        */
    //        for(int i=0; i<100; i++)
    //        {
    //            std::stringstream ss;
    //            ss << "www." << i << ".com";
    //            domain.push_back(ss.str());
    //            ss.clear();
    //            ss << "it's " << i;
    //            desc.push_back(ss.str());
    //            CListTextElementUI* pListElement = new CListTextElementUI;
    //            pListElement->SetTag(i);
    //            if (pListElement != NULL)
    //            {
    //                ::PostMessage(prama->hWnd, WM_ADDLISTITEM, 0L, (LPARAM)pListElement);
    //            }
    //            /*
    //            *	Sleep 为了展示添加的动态效果，故放慢了添加速度，同时可以看到添加过程中界面仍然可以响应
    //            */
    //            ::Sleep(100);
    //        }
    //        //------------------------------------------
    //        delete prama;

    //        pSearch->SetEnabled(true);
    //        return 0;
    //    }
    //    catch(...)
    //    {
    //        return 0;
    //    }
    //}
    //void OnSearch()
    //{
    //    struct Prama *prama = new Prama;

    //    CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(_T("domainlist")));
    //    CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("input")));
    //    pEdit->SetEnabled(false);
    //    CDuiString input = pEdit->GetText();
    //    m_pSearch->SetEnabled(false);
    //    pList->RemoveAll();
    //    domain.empty();
    //    domain.resize(0);
    //    desc.empty();
    //    desc.resize(0);
    //    DWORD dwThreadID = 0;
    //    pList->SetTextCallback(this);//[1]

    //    prama->hWnd = GetHWND();
    //    prama->pList = pList;
    //    prama->pSearch = m_pSearch;
    //    prama->tDomain = input;

    //    HANDLE hThread = CreateThread(NULL,0,&ListMainForm::Search, (LPVOID)prama,  0,&dwThreadID);
    //}
    /*
    * 关键的回调函数，IListCallbackUI 中的一个虚函数，渲染时候会调用,在[1]中设置了回调对象
    */
    /*LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
    {
        TCHAR szBuf[MAX_PATH] = {0};
        switch (iSubItem)
        {
        case 0:
            _stprintf(szBuf, _T("%d"), iIndex);
            break;
        case 1:
            {
#ifdef _UNICODE		
            int iLen = domain[iIndex].length();
            LPWSTR lpText = new WCHAR[iLen + 1];
            ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
            ::MultiByteToWideChar(CP_ACP, 0, domain[iIndex].c_str(), -1, (LPWSTR)lpText, iLen) ;
            _stprintf(szBuf, lpText);
            delete[] lpText;
#else
            _stprintf(szBuf, domain[iIndex].c_str());
#endif
            }
            break;
        case 2:
            {
#ifdef _UNICODE		
            int iLen = desc[iIndex].length();
            LPWSTR lpText = new WCHAR[iLen + 1];
            ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
            ::MultiByteToWideChar(CP_ACP, 0, desc[iIndex].c_str(), -1, (LPWSTR)lpText, iLen) ;
            _stprintf(szBuf, lpText);
            delete[] lpText;
#else
            _stprintf(szBuf, desc[iIndex].c_str());
#endif
            }
            break;
        }
        pControl->SetUserData(szBuf);
        return pControl->GetUserData();
    }*/

    void Notify(TNotifyUI& msg)
    {
        if( msg.sType == _T("windowinit") ) 
            OnPrepare(msg);
        else if( msg.sType == _T("click") ) 
        {
            if( msg.pSender == m_pCloseBtn ) 
            {
                PostQuitMessage(0);
                return; 
            }
            else if( msg.pSender == m_pMinBtn ) 
            { 
                SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
                return; 
            }
            else if( msg.pSender == m_pMaxBtn ) 
            { 
                SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; 
            }
            else if( msg.pSender == m_pRestoreBtn ) 
            { 
                SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; 
            }
            /*else if(msg.pSender == m_pSearch)
            {
                OnSearch();
            }*/
        }
        else if(msg.sType==_T("setfocus"))
        {
        }
        else if( msg.sType == _T("itemclick") ) 
        {
        }
        /*else if( msg.sType == _T("itemactivate") ) 
        {
            int iIndex = msg.pSender->GetTag();
            CDuiString sMessage = _T("Click: ");;
#ifdef _UNICODE		
            int iLen = domain[iIndex].length();
            LPWSTR lpText = new WCHAR[iLen + 1];
            ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
            ::MultiByteToWideChar(CP_ACP, 0, domain[iIndex].c_str(), -1, (LPWSTR)lpText, iLen) ;
            sMessage += lpText;
            delete[] lpText;
#else
            sMessage += domain[iIndex].c_str();

#endif
            ::MessageBox(NULL, sMessage.GetData(), _T("提示(by tojen)"), MB_OK);
        }*/
        else if(msg.sType == _T("menu")) 
        {
            //if( msg.pSender->GetName() != _T("domainlist") ) return;
            CMenuWnd* pMenu = new CMenuWnd();
            if( pMenu == NULL ) { return; }
            POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
            ::ClientToScreen(*this, &pt);
            pMenu->Init(msg.pSender, pt);
        }
       /* else if( msg.sType == _T("menu_Delete") ) {
            CListUI* pList = static_cast<CListUI*>(msg.pSender);
            int nSel = pList->GetCurSel();
            if( nSel < 0 ) return;
            pList->RemoveAt(nSel);
            domain.erase(domain.begin() + nSel);
            desc.erase(desc.begin() + nSel);   
        }*/
    }

	virtual CControlUI* CreateControl(LPCTSTR pstrClass){
		CDialogBuilder builder;

		if (_tcsicmp(pstrClass, _T("customList")) == 0)
		{
			return new CCustomListUI(m_pm);
		}

		return NULL;
	}

    LRESULT OnAddListItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        CListTextElementUI* pListElement = (CListTextElementUI*)lParam;
        CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(_T("domainlist")));
        if( pList ) pList->Add(pListElement);
        return 0;
    }

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
        styleValue &= ~WS_CAPTION;
        ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
        m_pm.Init(m_hWnd);
        //m_pm.SetTransparent(100);
        CDialogBuilder builder;
        CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0, this, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");
        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);
        Init();
        return 0;
    }

    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        ::PostQuitMessage(0L);
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if( ::IsIconic(*this) ) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
    }

    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
        ::ScreenToClient(*this, &pt);

        RECT rcClient;
        ::GetClientRect(*this, &rcClient);

        if( !::IsZoomed(*this) ) {
            RECT rcSizeBox = m_pm.GetSizeBox();
            if( pt.y < rcClient.top + rcSizeBox.top ) {
                if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
                if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
                return HTTOP;
            }
            else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
                if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
                if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
                return HTBOTTOM;
            }
            if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
        }

        RECT rcCaption = m_pm.GetCaptionRect();
        if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
            && pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
                if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
                    _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
                    _tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
                    return HTCAPTION;
        }

        return HTCLIENT;
    }

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        SIZE szRoundCorner = m_pm.GetRoundCorner();
        if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
            CDuiRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            RECT rc = { rcWnd.left, rcWnd.top + szRoundCorner.cx, rcWnd.right, rcWnd.bottom };
            HRGN hRgn1 = ::CreateRectRgnIndirect( &rc );
            HRGN hRgn2 = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom - szRoundCorner.cx, szRoundCorner.cx, szRoundCorner.cy);
            ::CombineRgn( hRgn1, hRgn1, hRgn2, RGN_OR );
            ::SetWindowRgn(*this, hRgn1, TRUE);
            ::DeleteObject(hRgn1);
            ::DeleteObject(hRgn2);
        }

        bHandled = FALSE;
        return 0;
    }

    LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        CDuiRect rcWork = oMonitor.rcWork;
		rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

        LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
        lpMMI->ptMaxPosition.x	= rcWork.left;
        lpMMI->ptMaxPosition.y	= rcWork.top;
        lpMMI->ptMaxSize.x		= rcWork.right;
        lpMMI->ptMaxSize.y		= rcWork.bottom;

        bHandled = FALSE;
        return 0;
    }

    LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        // 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
        if( wParam == SC_CLOSE ) {
            ::PostQuitMessage(0L);
            bHandled = TRUE;
            return 0;
        }
        BOOL bZoomed = ::IsZoomed(*this);
        LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
        if( ::IsZoomed(*this) != bZoomed ) {
            if( !bZoomed ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
                if( pControl ) pControl->SetVisible(false);
                pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
                if( pControl ) pControl->SetVisible(true);
            }
            else {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
                if( pControl ) pControl->SetVisible(true);
                pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
                if( pControl ) pControl->SetVisible(false);
            }
        }
        return lRes;
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch( uMsg ) {
            //case WM_ADDLISTITEM:   lRes = OnAddListItem(uMsg, wParam, lParam, bHandled); break;
            case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
            case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
            case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
            case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
            case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
            case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
            case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
            case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
            case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
            case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
            default:
                bHandled = FALSE;
        }
        if( bHandled ) return lRes;
        if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }
public:
    CPaintManagerUI m_pm;

private:
    CButtonUI* m_pCloseBtn;
    CButtonUI* m_pMaxBtn;
    CButtonUI* m_pRestoreBtn;
    CButtonUI* m_pMinBtn;
    CButtonUI* m_pSearch;

    //...
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin\\RichListRes2\\"));
    //CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
    //CPaintManagerUI::SetResourceZip(_T("ListRes.zip"));

    ListMainForm* pFrame = new ListMainForm();
    if( pFrame == NULL ) return 0;
    pFrame->Create(NULL, _T("ListDemo"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW , 0, 0, 600, 320);
    pFrame->CenterWindow();
    ::ShowWindow(*pFrame, SW_SHOW);

    CPaintManagerUI::MessageLoop();

	return 0;
}