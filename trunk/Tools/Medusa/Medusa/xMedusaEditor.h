#pragma once 
#include <Windows.h>
#include "BaseLib/xBaseObject.h"
#include "BaseLib/xObjectFactory.h"
#include "Application/xListener.h"
#ifdef MEDUSA_EXPORT 
#define MEDUSA_API _declspec(dllexport)
#else
#define MEDUSA_API _declspec(dllimport)
#endif
class MEDUSA_API CEvolEnviroment;
namespace nsMedusaEditor
{
	//ID 管理。定义每一个插件可以使用的ID范围
	enum 
	{
		ID_VIEW_DOCKPANE      = 10000,
		ID_VIEW_DOCKPANE_MAX  =  1000, //最大支持10000个DockPane, 地球人应该够用了。

		ID_PLUGIN_ID_0        =  35000, //插件可以用的动态ID,由这里开始
		ID_PLUGIN_ID_STEP     =    500, //每个插件可以用的ID数量
		ID_PLUGIN_ID_MAX      =  15000

	};

	class     IMEdUICommandReciver
	{
	public:
		virtual bool            OnCommand(int ctrlID , int ctrlIdx) = 0;
	};


	//状态栏对象
	class IMEdUIStatusBarItem
	{
	public:
		enum StatusBarItemType
		{
			SBIT_Label,
			SBIT_Button,
			SBIT_Checkbok,
			SBIT_Combox,
		};
	protected:
		nsMedusaEditor::IMEdUICommandReciver* m_pCallBack;
	};

	class MEDUSA_API IMEdUIStatusBar
	{
	public:
		virtual IMEdUIStatusBarItem* FindItem(const wchar_t* pName) = 0;
		virtual bool                 DeleteItem(IMEdUIStatusBarItem* pItem) = 0;
		virtual IMEdUIStatusBarItem* InsertItem(const wchar_t* pName , IMEdUIStatusBarItem::StatusBarItemType _type) =0;
		virtual bool                 InsertSeperator() = 0;
	};

	//Redo/Undo就靠这个了
	class MEDUSA_API IMEdUICommand : public NS_XEVOL3D::IRefCountObject
	{
	public:
		virtual bool Redo() = 0;
		virtual bool Undo() = 0;
	};
	//ToolBar对象
	struct MEDUSA_API CMEdUiToolBarInfo
	{
	public:
        enum ToolBarType
        {
            eToolBarTypeNone = 0 ,
            eToolBar,
            eMenuBar,
            eRibbonBar,
        };
        class MEDUSA_API CMEdUiCmdUi
        {
        public:
            virtual void Enable(BOOL bOn = TRUE) = 0;
            virtual void SetCheck(int nCheck = 1) = 0;   // 0, 1 or 2 (indeterminate)
            virtual void SetRadio(BOOL bOn = TRUE) = 0;
            virtual void SetText(LPCTSTR lpszText) = 0;
        };
		class MEDUSA_API MEdUIToolbarCallback : public IMEdUICommandReciver
		{
		public:
			virtual bool             OnUpdateCommandUI(int ctrlID , int ctrlIdx , CMEdUiCmdUi* pCmdUi) = 0;
			virtual bool             OnCommand(int ctrlID , int ctrlIdx) = 0;
            virtual bool             OnExit(){ return true; }
		};
	public:
		void                         ProcessToolbarID(int startID);
        void                         ProcessMenuID(int startID);
        void*                        ProcessRibbonBarData(int startID);
        void                         FreeData(void* pData);

		bool                         IsToolbarCmd(int cmdId);
		int                          GetOriginID(int cmdID);
        int                          MapOriginID2CtrlID(int originID);
        
        ToolBarType                  GetType(); 
        void                         SetType(ToolBarType _type) { m_Type = _type ; } 
		CMEdUiToolBarInfo();
		~CMEdUiToolBarInfo();
	public:
		MEdUIToolbarCallback* m_funcCallback;
		wchar_t               m_name[64];
		wchar_t               m_title[256];
		HINSTANCE             m_hDll;
		UINT                  m_ResID;
		void*                 m_hToolbar;
        

		int                   m_startID;
		int                   m_nButton;
		int*                  m_OriginBtnIDs;
    protected:
       ToolBarType             m_Type;
	};


	enum eMEUIEvent 
	{
       eMEUI_DockedPane_Attach,
	   eMEUI_DockedPane_Detach,
	   eMEUI_SceneChanged,
	   eMEUI_SceneInsertNode,
	   eMEUI_SceneRemoveNode,
	   eMEUI_SceneLoaded ,
	   eMEUI_SceneUnload ,
	   eMEUI_SceneSelChanged, //场景选择被改变了
	   eMEUI_CameraChange   ,
	   eMEUI_ScenePathChange, //场景的资源路径改变了
	   eMEUI_PrepaireInsertNode    , //准备插入一个模型 , 这个消息只能由一个面板响应，如果多个响应。则会插入多个模型
	   eMEUI_PrepaireInsertFile    , //插入一个文件
	};

	//所有UI元素的基类
	class  MEDUSA_API IMEdUIElement : public NS_XEVOL3D::IRefCountObject
	{
	public:
		virtual bool             onDetach() = 0;
		virtual bool             onAttach() = 0;
		virtual bool             onMEdUIEvent(eMEUIEvent _event , int param, int param2 = 0) = 0;
		virtual bool             createMEdUI(HWND hMainWnd) = 0;
		virtual bool             destroyMEdUI() = 0 ;
		virtual bool             HideMEdUI()=0;
		virtual bool             ShowMEdUI()=0;
        virtual bool             IsVisible()=0;
		virtual HWND             hWnd() = 0 ;
		virtual const wchar_t*   title() = 0;
		virtual const wchar_t*   name() = 0;
		virtual const wchar_t*   typeName() = 0 ;

		virtual bool             AttachUIElement(IMEdUIElement* pPane) { return false ; }
		virtual bool             DetachUIElement(IMEdUIElement* pPane) { return false ; }
		virtual bool             DetachUIElement() { return false ; }

	};
              
	//可以停靠的UI基类，必须带一个hIcon, title 和 HWND .
	class MEDUSA_API IMEdDockPane : public IMEdUIElement
	{
	public :
		virtual HICON            hIcon() = 0;
		virtual   void           setID(UINT id) = 0;
		virtual UINT             getID() = 0;
		
	};
    
    class MEDUSA_API CMEdUiBaseToolbarElement : public IMEdUIElement
    {
     public:
         virtual BOOL    IsChecked(int ctrlIdx) = 0;
         virtual int     GetSliderPos(int ctrlIdx) = 0;
         virtual void    SetChecked(int ctrlIdx , BOOL bFlag) = 0;
         virtual void    SetSliderPos(int ctrlIdx  , int pos) = 0;
    } ;
    
	class MEDUSA_API IMEdUIMessageListenner : public NS_XEVOL3D::IMsgListener
	{
	public:
		enum eActiveReason
		{
			eAR_DeActive ,
			eAR_Active,
			eAR_Attach,
			eAR_Detach,
		};
	public:
		virtual bool onMessage( NS_XEVOL3D::xWindowMsg& msg) = 0;
		virtual bool onActive(eActiveReason _Reason ) = 0 ;
	};

	//编辑器的主UI。
	class MEDUSA_API IMEdMainUI
	{
	public:
        virtual IMEdUIElement*     FindUIElement(const wchar_t* _name , bool recursive = true) = 0 ;
		virtual void               AttachUIElement(IMEdUIElement* pPane) = 0 ;
		virtual void               DetachUIElement(IMEdUIElement* pPane) = 0 ;
		virtual bool               DetachUIElement() = 0;
	public:
		virtual void               RegisteToolbar(CMEdUiToolBarInfo* _toolbar) = 0;
		virtual void               UnregisteToolbar(const wchar_t* _tbName) = 0;
		virtual CMEdUiToolBarInfo* FindToolbar(const wchar_t* _tbName)= 0;
		virtual bool               CreateToolbar(const wchar_t* _tbName) = 0;
		virtual bool               DestroyToolbar() =0;
		virtual bool               DestroyToolbar(const wchar_t* _tbName) = 0;
		virtual CMEdUiToolBarInfo* FindToolbarByCmdID(int cmdID) = 0;
		virtual bool               PushMessageListenner(IMEdUIMessageListenner* pListener) = 0;
		virtual bool               PopMessageListenner() = 0;
        virtual IMEdUIMessageListenner* GetMessageListenner() = 0;
		virtual bool               fireMEdUIEvent(eMEUIEvent _event , int param , int param2 = 0) = 0;
		virtual IMEdUIStatusBar*   GetStatusBar() = 0;
        virtual bool               Redo() = 0 ;
        virtual bool               Undo() = 0 ;
        virtual bool               CanRedo() = 0 ;
        virtual bool               CanUndo() = 0 ;
        virtual bool               InsertCommand(IMEdUICommand* pCommand) = 0;

	public:
		virtual int                AllocUIID(int nId) = 0;

	};



	//编辑器对象本身
	class MEDUSA_API IMedusaEditor
	{
	public :
		virtual IMEdMainUI*      GetUI() = 0 ;
        virtual CEvolEnviroment* GetEvol3DEnv() = 0;
	public:
	};


	//-------------------------------------
	//对象管理函数
	//-------------------------------------
	class MEDUSA_API  CMEdUIElFactoryMgr;
	class   IMEdUIElementFactory ;
	class MEDUSA_API CMEdUIElFactoryMgr
	{
	public:
		size_t                           nFactories();
		IMEdUIElement*                   createInstance(const wchar_t* name );
		IMEdUIElementFactory*            find(size_t idx);
		IMEdUIElementFactory*            find(const wchar_t* name);
		bool                             registe(IMEdUIElementFactory* object);
		bool                             unregiste(IMEdUIElementFactory* object);
        static CMEdUIElFactoryMgr*       singleton();
	protected:
		void                            _enusureSingleton();
	};

	DECL_OBJECT_FACTORY(IMEdUIElement , IMEdUIElementFactory , CMEdUIElFactoryMgr , int , int );
	//---------------------------------------------------
	extern "C"
	{
		MEDUSA_API IMedusaEditor* GetMedusaEditor();
	}
};

#define  NS_MDED    nsMedusaEditor
#define  USING_NS_MDED  using namespace nsMedusaEditor;



