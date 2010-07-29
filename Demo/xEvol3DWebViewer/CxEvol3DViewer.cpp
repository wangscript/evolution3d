// CxEvol3DViewer.cpp : Implementation of CCxEvol3DViewer
#include "stdafx.h"
#include "OperationSys/xOperationSys.h"
#include "xEvol3D/xEvol3DEnv.h"
#include "CxEvol3DViewer.h"
USING_NS_XEVOL3D;
bool CCxEvol3DViewer::InitEvol3D(HWND hWnd , bool bCreateOwnWindow)
{
	   //初始化3D代码
    wchar_t appname[1024] = {0};
    xOperationSys::singleton()->getModuleFileName( (HMODULE)Global_hModuleHandle , appname , 1024);
    //开始决定将Resource目录放置在哪个位置
    //...开发过程中， 放置在引擎目录相对位置   
    /*
          xEvol3D
              +-----trunck
                      +---bin    [引擎目录]
          xGesture
              +-----resource
          因此相对目录为   ../../xGesture/resource/
    */
    //否则就是使用跟bin目录一样的目录
    m_Evol3D.init();
    if(bCreateOwnWindow)
       m_Evol3D.initRenderer(NULL , hWnd );
    else
        m_Evol3D.initRenderer(hWnd , NULL); 

    //app.loadScene(_XEVOL_ABSPATH_(L".\\WorldComposer\\scene\\demoScene.xml"));
    //m_Evol3D.loadScene(_RES_ABSPATH_(L".\\Web3D\\scene.xml"));

	m_bEvol3DInited = true;
    return true;
}
HRESULT CCxEvol3DViewer::OnDraw(ATL_DRAWINFO& di)
{
	return S_OK;
}

// CCxEvol3DViewer

LRESULT CCxEvol3DViewer::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	HWND hWnd = NULL;
	m_bEvol3DInited = false;
	ATL::IOleInPlaceActiveObjectImpl<CCxEvol3DViewer>::GetWindow(&hWnd);
	// TODO: Add your message handler code here and/or call default
	::MessageBox( hWnd, L"......." , L"test" , MB_OK);
	::SetTimer(hWnd , NULL , 20 , NULL);
	return 0;
}

LRESULT CCxEvol3DViewer::OnShowWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default

	return 0;
}

LRESULT CCxEvol3DViewer::OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    //URLDownloadToFile()
	// TODO: Add your message handler code here and/or call default
	static long lastTick = GetTickCount();
	long thisTickt = GetTickCount();
	long passedTime = thisTickt - lastTick;
	lastTick = thisTickt;
	if(m_bEvol3DInited == false ) 
	{
		HWND hWnd = NULL;
		ATL::IOleInPlaceActiveObjectImpl<CCxEvol3DViewer>::GetWindow(&hWnd);
		InitEvol3D(hWnd , true);
	}
	if(true)
	{
        m_Evol3D.lock();
		m_Evol3D.prevUpdateFrame(passedTime);
		m_Evol3D.updateFrame(passedTime);
		m_Evol3D.postUpdateFrame(passedTime);
        m_Evol3D.unlock();
	}
	return 0;
}

void WINAPI CCxEvol3DViewer::ObjectMain(bool bStarting)
{
    // TODO: Add your specialized code here and/or call the base class
    __super::ObjectMain(bStarting);
}

HRESULT CCxEvol3DViewer::FinalConstruct()
{
    return S_OK;
}

void CCxEvol3DViewer::FinalRelease()
{
    return ;
}

void CCxEvol3DViewer::OnDownloadFinish(const wchar_t* localFile)
{
    std::ds_wstring fileName = xFileSystem::singleton()->getFileName(localFile);
    std::ds_wstring pathName = xFileSystem::singleton()->getPathName(localFile);
    std::ds_wstring nameNoExt;
    xFileSystem::singleton()->fileNameWithoutExt(fileName.c_str() , nameNoExt );
    xcomdoc doc;
    if(doc.open(localFile , xcdm_read) == false )
        return ;

    std::ds_wstring resDir = pathName + nameNoExt;
    doc.extrac_to_dir(resDir.c_str());
    doc.close();

    std::ds_wstring sceneName = resDir + L"\\scene.xml";
    if(xFileSystem::singleton()->fileExist(sceneName.c_str()) == false )
    {
         sceneName = resDir + L"\\raytracer.xml";
    }
    if(xFileSystem::singleton()->fileExist(sceneName.c_str()) == false )
    {
        return ;
    }

    m_Evol3D.lock();
    std::ds_wstring modelPath = resDir + L"\\model\\";
    m_Evol3D.engine()->modelMgr()->addPath(modelPath.c_str());

    std::ds_wstring texturePath = resDir + L"\\texture\\";
    m_Evol3D.engine()->textureManager()->addPath(texturePath.c_str());

    m_Evol3D.loadScene( sceneName.c_str() );
    m_Evol3D.unlock();


    //delete m_pDownloader;   
} 

LRESULT CCxEvol3DViewer::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // TODO: Add your message handler code here and/or call default
    m_Evol3D.exit();
    return 0;
}

STDMETHODIMP CCxEvol3DViewer::OpenURL(BSTR _url)
{
    // TODO: Add your implementation code here
    xURLDownloader downloader(this);
    downloader.Download( (wchar_t*) _url );
    return S_OK;
}

xURLDownloader::xURLDownloader(CCxEvol3DViewer* pViewer)
{
   m_pViewer = pViewer;
}

bool xURLDownloader::Download(const wchar_t* _url)
{
    std::ds_wstring usrDir   = xOperationSys::singleton()->getUserProfileFolder();
    std::ds_wstring fileName = xFileSystem::singleton()->getFileName(_url);
    std::ds_wstring dlDir = usrDir + L"\\xEvolWeb3D\\";
    ::CreateDirectoryW(dlDir.c_str() , NULL);

    dlDir = dlDir + L"download\\";
    ::CreateDirectoryW(dlDir.c_str() , NULL);
    m_LocalFile = dlDir + fileName;

    if(xFileSystem::singleton()->fileExist(m_LocalFile.c_str() ) )
    {
        m_pViewer->OnDownloadFinish( m_LocalFile.c_str() );
        return true;
    }
    m_pViewer->Evol3DEnv()->lock();
    URLDownloadToFile(NULL , _url , m_LocalFile.c_str() , 0 , this );
    m_pViewer->Evol3DEnv()->unlock();
    return false;
}

HRESULT xURLDownloader::OnProgress ( ULONG ulProgress,   ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR wszStatusText )
{
    BINDSTATUS  _bindState = (BINDSTATUS)ulStatusCode;
    if(ulProgressMax !=0 && ulProgress >= ulProgressMax)
        this->m_pViewer->OnDownloadFinish(this->m_LocalFile.c_str() );

    wchar_t buf[256] = {0};
    if(ulProgressMax > 0)
    {
        swprintf(buf , L"downloading .... %%%d " , ulProgress * 100 /ulProgressMax );
    }
    else
    {
        swprintf(buf , L"downloading .... " );
    }
    m_pViewer->Evol3DEnv()->setInfoText(buf);
    return S_OK;
}