// CxEvol3DViewer.h : Declaration of the CCxEvol3DViewer
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "xEvol3DWebViewer_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

class xURLDownloader;
// CCxEvol3DViewer
class ATL_NO_VTABLE CCxEvol3DViewer :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<ICxEvol3DViewer, &IID_ICxEvol3DViewer, &LIBID_xEvol3DWebViewerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IPersistStreamInitImpl<CCxEvol3DViewer>,
	public IOleControlImpl<CCxEvol3DViewer>,
	public IOleObjectImpl<CCxEvol3DViewer>,
	public IOleInPlaceActiveObjectImpl<CCxEvol3DViewer>,
	public IViewObjectExImpl<CCxEvol3DViewer>,
	public IOleInPlaceObjectWindowlessImpl<CCxEvol3DViewer>,
	public ISupportErrorInfo,
	public IPersistStorageImpl<CCxEvol3DViewer>,
	public ISpecifyPropertyPagesImpl<CCxEvol3DViewer>,
	public IQuickActivateImpl<CCxEvol3DViewer>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<CCxEvol3DViewer>,
#endif
	public IProvideClassInfo2Impl<&CLSID_CxEvol3DViewer, NULL, &LIBID_xEvol3DWebViewerLib>,
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
	public IObjectSafetyImpl<CCxEvol3DViewer, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
#endif
	public CComCoClass<CCxEvol3DViewer, &CLSID_CxEvol3DViewer>,
	public CComControl<CCxEvol3DViewer>
{
public:


	CCxEvol3DViewer()
	{
		m_bWindowOnly  =  TRUE; 
		m_bEvol3DInited = false;
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_CXEVOL3DVIEWER)


BEGIN_COM_MAP(CCxEvol3DViewer)
	COM_INTERFACE_ENTRY(ICxEvol3DViewer)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
#ifndef _WIN32_WCE
	COM_INTERFACE_ENTRY(IDataObject)
#endif
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
#ifdef _WIN32_WCE // IObjectSafety is required on Windows CE for the control to be loaded correctly
	COM_INTERFACE_ENTRY_IID(IID_IObjectSafety, IObjectSafety)
#endif
END_COM_MAP()

BEGIN_PROP_MAP(CCxEvol3DViewer)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY_TYPE("Property Name", dispid, clsid, vtType)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CCxEvol3DViewer)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    CHAIN_MSG_MAP(CComControl<CCxEvol3DViewer>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] =
		{
			&IID_ICxEvol3DViewer,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// ICxEvol3DViewer
public:
	HRESULT OnDraw(ATL_DRAWINFO& di);
    CEvolEnviroment* Evol3DEnv(){ return &m_Evol3D ; };
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void    FinalRelease();
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnShowWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    void    OnDownloadFinish(const wchar_t* localFile);
protected:
	bool InitEvol3D(HWND hWnd , bool bCreateOwnWindow);
	CEvolEnviroment m_Evol3D;
	bool            m_bEvol3DInited;
public:
	LRESULT   OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    static void WINAPI ObjectMain(bool bStarting);
    LRESULT   OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    STDMETHOD(OpenURL)(BSTR _url);
};


class xURLDownloader : public IBindStatusCallback 
{
public:
    std::ds_wstring     m_LocalFile;
    std::ds_wstring     m_URL;
    CCxEvol3DViewer*    m_pViewer;
public:
    xURLDownloader(CCxEvol3DViewer* pViewer);
    bool Download(const wchar_t* _url);
    STDMETHOD(OnProgress)( ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR wszStatusText);

    STDMETHOD(OnStartBinding)( DWORD dwReserved,  IBinding __RPC_FAR *pib)  { return E_NOTIMPL; }
    STDMETHOD(GetPriority)   (  LONG __RPC_FAR *pnPriority)  { return E_NOTIMPL; }
    STDMETHOD(OnLowResource) ( DWORD reserved)   { return E_NOTIMPL; }
    STDMETHOD(OnStopBinding)( HRESULT hresult,  LPCWSTR szError)   { return E_NOTIMPL; }
    STDMETHOD(GetBindInfo)(DWORD __RPC_FAR *grfBINDF,  BINDINFO __RPC_FAR *pbindinfo)   { return E_NOTIMPL; }
    STDMETHOD(OnDataAvailable)(DWORD grfBSCF,DWORD dwSize,FORMATETC __RPC_FAR *pformatetc,STGMEDIUM __RPC_FAR *pstgmed)   { return E_NOTIMPL; }
    STDMETHOD(OnObjectAvailable)(REFIID riid,IUnknown __RPC_FAR *punk)   { return E_NOTIMPL; }
    STDMETHOD_(ULONG,AddRef)()  { return 0; }
    STDMETHOD_(ULONG,Release)()  { return 0; }
    STDMETHOD(QueryInterface)( REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject)   { return E_NOTIMPL; }

};
OBJECT_ENTRY_AUTO(__uuidof(CxEvol3DViewer), CCxEvol3DViewer)
