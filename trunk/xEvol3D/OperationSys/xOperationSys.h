#ifndef _XEVOL_OPERATION_SYSTEM_H__
#define _XEVOL_OPERATION_SYSTEM_H__
#include "../BaseLib/xEvol3DAPI.h"
#include "../BaseLib/stl_dllalloc.h"
#include "../Application/xVirtualKey.h"
#ifdef WIN32
#include <windows.h>
#endif
#define XEVOL_MAX_PATH 512

BEGIN_NAMESPACE_XEVOL3D
//Operation System API
#ifdef _LINUX
unsigned long timeGetTime();
#endif
unsigned long _XEVOL_BASE_API_  xGetTickCount();
void          _XEVOL_BASE_API_  xSleep(unsigned long mSecond);



//MultiThread
class _XEVOL_BASE_API_  xThreadLocker :  public IRefCountObject
{
public:
	virtual bool    lock() = 0 ;
	virtual bool    trylock() = 0;
	virtual void    unlock() = 0;
};

class _XEVOL_BASE_API_  xThreadSignal : public IRefCountObject
{
public :
	virtual bool    wait(unsigned long mSecond) = 0;
	virtual bool    fire() = 0;
	virtual bool    reset() = 0;
};

class  _XEVOL_BASE_API_  xThreadRunner;

class  _XEVOL_BASE_API_  xThreadProc
{
public:
	virtual int  thread_proc() = 0;
public:
	xThreadProc();
	~xThreadProc();
	virtual void         run(xThreadRunner* pRunner);
	virtual bool         bExitThread();
	virtual void         exitThread(bool bWaited);
protected:
	bool           m_bExitThread;
	xThreadRunner* m_pRunner;
};

class _XEVOL_BASE_API_  xThreadRunner :  public IRefCountObject
{
public:
	virtual bool    run(xThreadProc* pThread) = 0;
	virtual void    wait_exit() = 0 ;//Wait for exit;
	virtual bool    exit_Thread(int exitCode)=0;
	virtual bool    suspend() = 0;
	virtual bool    resume() = 0;
};


//Dll Loader
class _XEVOL_BASE_API_  xDLLModule  : public IRefCountObject
{
public:
	virtual bool    load(const wchar_t* dllName) = 0;
	virtual bool    unload() = 0;
	virtual void*   getProcAddress(const wchar_t* funcName) = 0;
};

//Keyboard
class _XEVOL_BASE_API_  IKeyboard
{
public :
	virtual bool          isKeyPressed(VIRTUAL_KEY vKey)     = 0;
	virtual bool          isCharKey(VIRTUAL_KEY vKey)        = 0;
	virtual bool          isKeyToggle(VIRTUAL_KEY vKey)      = 0;
	virtual unsigned char convert2Char(VIRTUAL_KEY vKey)     = 0;
	virtual unsigned char getSysVirtualKey(VIRTUAL_KEY vKey) = 0;
};

class _XEVOL_BASE_API_  IIMEInput
{
public:
	virtual bool   setCompositionFont(int hImc, const wchar_t* strFamilly, int nHeight , bool bBold , bool bItalic , bool bUnderLine) = 0;
	virtual int    associateContext(int hWndHandle, int hImc) = 0;
	virtual bool   releaseContext(int hWndHandle, int hImc) = 0;
	virtual void*  getContextImc(int hWndHandle) = 0;
	virtual void   setCompositionWndPos(int hImc, int x, int y) = 0;
};

//Operation System Object
class _XEVOL_BASE_API_  xOperationSys
{

public:
    static xOperationSys* singleton();
public:
	//Implement OS independent;
	xOperationSys();
	~xOperationSys();
	bool              init(const wchar_t* appName);
	void			  unInit();
	std::ds_wstring   getAbsPath(const wchar_t* _relPath);
    std::ds_wstring   getAbsPath(const wchar_t* _relPath , const wchar_t* pRootPath);
	std::ds_wstring   getPath(const wchar_t* _filename);
	std::ds_wstring   getRelatePath(const wchar_t* _filename);
	std::ds_wstring   getRelatePath(const wchar_t* _filename , const wchar_t* basePath);
	const wchar_t*    getWorkPath();
	const wchar_t*    getAppPath();
	void			  setCursorStyle(int nStyle);
	void              enableConsole();
	void              convertSystemFileName(std::ds_wstring& _fileName);
	std::ds_wstring   getUserProfileFolder();
    const wchar_t*    name();
    void              setResourcePath(const wchar_t* resourePath);
    const wchar_t*    getResourcePath();
    std::ds_wstring   getAbsResourcePath(const wchar_t* relPath);
    bool              isFolder(const wchar_t* folderName);
#ifdef WIN32
    bool 			  getModuleFileName(HMODULE hModule , wchar_t* fileName , int len);
#endif
	//Implement OS dependent;
public:
	IIMEInput*     m_pImeInput;


public:
	
	xDLLModule*     createDLL(const wchar_t* dllName);
	xThreadLocker*  createThreadLocker();
	xThreadSignal*  createThreadSignal();
	xThreadRunner*  createThreadRunner();
	IIMEInput*      getImeInput();

	bool            clearClipboard();
	bool            copyToClipboard(const wchar_t* clipBox);
	std::ds_wstring getClipboardString();

	int             warnningBox(const wchar_t* message , const wchar_t* title);
	int             messageBox(const wchar_t* message , const wchar_t* title);

protected:
    std::wstring    m_AppPath;
    std::wstring    m_ResPath;
    std::ds_wstring m_LastRet;
};

#define _XEVOL_ABSPATH_(_path)       ( xOperationSys::singleton()->getAbsPath( (_path)).c_str() )
#define _RES_ABSPATH_(_path)    ( xOperationSys::singleton()->getAbsResourcePath( (_path)).c_str() )

#define _RELATEPATH(_path) ( xOperationSys::singleton()->getRelatePath((_path)).c_str() )
#define _RELATEPATH2(_path , _basePath) ( xOperationSys::singleton()->getRelatePath((_path) , (_basePath) ).c_str() )
END_NAMESPACE_XEVOL3D

#endif
