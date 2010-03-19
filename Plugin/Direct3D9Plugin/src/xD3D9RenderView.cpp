#include "stdafx.h"
#include "xD3D9RenderView.h"
#include "xDIRECT3D9API.h"
#include "xD3D9RenderTarget.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xD3D9RenderView   , IRenderView);
IMPL_BASE_OBJECT_CLASSID(xD3D9RenderWindow , xD3D9RenderView);

xD3D9RenderView::xD3D9RenderView(xD3D9RenderApi* pRenderApi , const xRTSampleDesc& sampleDesc) : IRenderView(pRenderApi , sampleDesc)
{
	m_RefCount = 1;
	m_pD3D9RenderApi = pRenderApi;
	for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
	{
		m_RenderTargetsView[i] = NULL;
		m_RenderTargets[i] = NULL;
	}
	m_pDepthStencilView    = NULL;
	m_DepthBuffer          = NULL;

    pRenderApi->getWindowSize(m_widht , m_height );
    memset( &m_TextureDesc , 0 , sizeof(m_TextureDesc) );
    REGISTER_DEVICE_LOST_RESOURCE(m_pD3D9RenderApi);
}

xD3D9RenderView::~xD3D9RenderView()
{
    REMOVE_DEVICE_LOST_RESOURCE(m_pD3D9RenderApi);
	destory();
}

void  xD3D9RenderView::onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action)
{
    if(_Action == ID3D9DeviceLostListener::eReset_Begin)
    {
        XSAFE_RELEASE(m_pDepthStencilView);
        for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
        {
            XSAFE_RELEASE(m_RenderTargetsView[i]);
        }
    }

    if(_Action == ID3D9DeviceLostListener::eReset_Finish)
    {
        //重新安装Depth Buffer;
        if(m_DepthBuffer)
        {
            IDirect3DSurface9* pDSV = (IDirect3DSurface9*)m_DepthBuffer->handle();
            m_pDepthStencilView = pDSV;
            xTextureDesc desc;
            m_DepthBuffer->desc( desc);
            m_widht   = desc.m_width;
            m_height  = desc.m_height;
        }

        //重新安装RenderTarget;
        for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
        {
            if(m_RenderTargets[i])
            {
                IDirect3DSurface9* pRTV = (IDirect3DSurface9*)m_RenderTargets[i]->handle();
                if(pRTV == NULL) continue;
                m_RenderTargetsView[i] = pRTV;
            }
        }
        
    }
}

bool  xD3D9RenderView::destory()
{
    XSAFE_RELEASEOBJECT(m_DepthBuffer);
	for(size_t i = startIdx() ; i < MAX_RENDER_TARGET ; i ++ )
	{
		XSAFE_RELEASEOBJECT(m_RenderTargets[i]);
	}
	return true;
}

bool  xD3D9RenderView::createRenderTarget(int nRenderTarget , ePIXEL_FORMAT fmt , bool bLockable , bool bAsTexture)
{
	int w = m_widht;
	int h = m_height;
	for(size_t i = 0 ; i < (size_t)nRenderTarget ; i ++ )
	{
		IRenderTarget* pRenderTarget = m_pD3D9RenderApi->createRenderTarget(w , h ,fmt , bLockable , bAsTexture , m_RTSampleDesc);
		setRenderTarget(pRenderTarget , i);
        XSAFE_RELEASEOBJECT(pRenderTarget);
	}
	return setup();
}

int  xD3D9RenderView::nRenderTargetView()
{
	for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
	{
		if( m_RenderTargetsView[i] == NULL)
			return (int)i;
	}
	return MAX_RENDER_TARGET;
}

bool xD3D9RenderView::setRenderTarget(IBaseTexture*  pTexture , size_t rtIdx) 
{
	if( (int)rtIdx < startIdx() || rtIdx >= MAX_RENDER_TARGET )
		return false;

	if(pTexture == NULL)
		return false;
	return setRenderTarget(pTexture , rtIdx);
}

bool xD3D9RenderView::setRenderTarget(IRenderTarget* pRenderTarget , size_t rtIdx)
{
	if( (int)rtIdx < startIdx() || rtIdx >= MAX_RENDER_TARGET )
		return false;

	if(pRenderTarget == NULL)
		return false;
	
	IDirect3DSurface9* pRTV = (IDirect3DSurface9*)pRenderTarget->handle();
	if(pRTV == NULL)
	   return false ; 

	//增加引用计数
	pRenderTarget->AddRef();
	if(m_RenderTargets[rtIdx]) m_RenderTargets[rtIdx]->ReleaseObject();
	m_RenderTargets[rtIdx] = pRenderTarget;
	m_RenderTargetsView[rtIdx] = pRTV;
	return true;
}
bool xD3D9RenderView::setupDepthView(int w , int h , bool bSet)
{
    if(m_DepthBuffer == NULL)
        return false;

    xTextureInitDesc initDesc(w , h , PIXELFORMAT_DEPTH32);
    if( false == m_DepthBuffer->create( w , h , D3DFMT_D24S8 ) )
    {
        return false;
    }
    if(bSet)
    {
        xD3D9DepthBuffer* pDepthBuffer = m_DepthBuffer;
        m_DepthBuffer = NULL;
        return setDepthBuffer(pDepthBuffer);
    }
    return true;
}

bool xD3D9RenderView::createDepthView(int w , int h)
{
    HRESULT hr = S_OK;
    IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
    XSAFE_RELEASEOBJECT(m_DepthBuffer);
    m_DepthBuffer = new xD3D9DepthBuffer(m_pD3D9RenderApi);
    if(setupDepthView( w , h , true) == false)
    {
         XSAFE_RELEASEOBJECT(m_DepthBuffer);
         return false;
    }
    return true;
}

bool xD3D9RenderView::setDepthBuffer(IBaseTexture* pDepthTexture)
{
    return false;
}

bool xD3D9RenderView::setDepthBuffer(IRenderTarget* pDepthBuffer) 
{
    XSAFE_RELEASE(m_pDepthStencilView);
	IDirect3DSurface9* pDSV = (IDirect3DSurface9*)pDepthBuffer->handle();
	XSAFE_RELEASEOBJECT(m_DepthBuffer) ;
	pDepthBuffer->AddRef();
	m_DepthBuffer = dynamic_cast<xD3D9DepthBuffer*>(pDepthBuffer);
	m_pDepthStencilView = pDSV;

	xTextureDesc desc;
	m_DepthBuffer->desc( desc);
	m_widht   = desc.m_width;
	m_height  = desc.m_height;
	return m_pDepthStencilView != NULL ; 
}

bool xD3D9RenderView::resize(int w , int h  , bool bDestorRT)
{
	TAutoLocker<IRenderApi> aLocker(m_pD3D9RenderApi);
	if(bDestorRT)
	{
		for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
		{
			XSAFE_RELEASEOBJECT( m_RenderTargets[i] );
		}
	}
	resize(w , h);
	return true;
}

bool xD3D9RenderView::resize(int w , int h)
{
	TAutoLocker<IRenderApi> aLocker(m_pD3D9RenderApi);
	return setupDepthView( w , h , true);
}

IDirect3DSurface9* xD3D9RenderView::depthView()
{
    IDirect3DSurface9* pDepthStencilView = m_pDepthStencilView;
    if(pDepthStencilView == NULL) 
        pDepthStencilView = m_pD3D9RenderApi->DefDepthStencilView();

    return pDepthStencilView;
}

int xD3D9RenderView::addRenderTarget(IBaseTexture*  pTexture)
{ 
	for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
	{
		if( m_RenderTargetsView[i] == NULL)
		{
			if(setRenderTarget(pTexture , i) == false)
				return (int)i ;
			return (int)i + 1;
		}
	}
	return MAX_RENDER_TARGET;
}

int xD3D9RenderView::addRenderTarget(IRenderTarget* pRenderTarget) 
{ 
	for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
	{
		if( m_RenderTargetsView[i] == NULL)
		{
			if(setRenderTarget(pRenderTarget , i) == false) 
				return (int)i ;
			return (int)i + 1;
		}
	}
	return MAX_RENDER_TARGET;; 
}

IRenderTarget*  xD3D9RenderView::depthBuffer()
{ 
	if(m_DepthBuffer) 
	{
        m_DepthBuffer->AddRef();
		return m_DepthBuffer;
	}
    return NULL;
}

bool  xD3D9RenderView::depthBufferDesc(D3DSURFACE_DESC& _desc )
{
	IDirect3DSurface9* pDepthView = depthView();
	if(pDepthView == NULL)
		return false;
	pDepthView->GetDesc( &_desc );
	return true;
}

IRenderApiObject*  xD3D9RenderView::renderTarget(size_t rtIdx )
{ 
	if( (int)rtIdx < startIdx() || rtIdx >= MAX_RENDER_TARGET )
		return NULL;
	return m_RenderTargets[rtIdx] ; 
}



bool xD3D9RenderView::install()
{
	IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
	int nRV = nRenderTargetView();
	if(nRV == 0)
		return false;
    for(int i = 0 ; i < nRV ; i ++)
    {
        if(m_RenderTargetsView[i])
        {
            pDevice->SetRenderTarget(i , m_RenderTargetsView[i] );
        }
    }
    pDevice->SetDepthStencilSurface( depthView() );
 	
	D3DVIEWPORT9 vp;
	xTextureDesc _desc;
	desc(_desc);
	vp.Width  = (DWORD)_desc.m_width;
	vp.Height = (DWORD)_desc.m_height;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	vp.X = 0;
	vp.Y = 0;
	pDevice->SetViewport(&vp );
	return true;
}

bool xD3D9RenderView::setup()
{
	int nRT  = nRenderTargetView();
	D3DSURFACE_DESC _desc;
    ZeroMemory(&_desc , sizeof(_desc) );
	if(false == depthBufferDesc(_desc) )
		return false;

	for(size_t i = 0 ; i < (size_t)nRT ; i ++)
	{
		if(m_RenderTargets[i]) 
        {
            m_RenderTargets[i]->desc( m_TextureDesc );
		    if( (int)_desc.Width < m_TextureDesc.m_width || (int)_desc.Height < m_TextureDesc.m_height)
			   return false;
        }
	}
	m_TextureDesc.m_nArraySize = nRT;
	return true;
}

bool xD3D9RenderView::clear(xColor_4f& bkColor)
{
	TAutoLocker<IRenderApi> aLocker(m_pD3D9RenderApi);
	IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
    xColor_4ub cl = bkColor;
    cl.swapEndian();
    pDevice->Clear(0 , NULL , D3DCLEAR_TARGET ,cl.color , 1.0f , 0 );
	return true;
}

bool xD3D9RenderView::clearRenderTarget(xColor_4f& bkColor, int idx)
{
    TAutoLocker<IRenderApi> aLocker(m_pD3D9RenderApi);
    IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
    xColor_4ub cl = bkColor;
    pDevice->Clear(0 , NULL , D3DCLEAR_TARGET ,cl.color , 1.0f , 0 );
    return true;
}

bool xD3D9RenderView::clear(xColor_4f& bkColor , float z , unsigned int stencil)
{
    TAutoLocker<IRenderApi> aLocker(m_pD3D9RenderApi);
    IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
    xColor_4ub cl = bkColor;
    cl.swapRB();
    pDevice->Clear(0 , NULL , D3DCLEAR_TARGET |  D3DCLEAR_ZBUFFER,cl.color , z , stencil );
    return true;
}

bool xD3D9RenderView::clear(xColor_4f& bkColor, int nClear)
{
    TAutoLocker<IRenderApi> aLocker(m_pD3D9RenderApi);
    IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
    xColor_4ub cl = bkColor;
    cl.swapRB();
    pDevice->Clear(0 , NULL , D3DCLEAR_TARGET ,cl.color , 1.0f , 0 );
    return true;
}

bool xD3D9RenderView::clear(xColor_4f& bkColor ,  float z , unsigned int stencil, int nClear )
{
    TAutoLocker<IRenderApi> aLocker(m_pD3D9RenderApi);
    IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
    xColor_4ub cl = bkColor;
    cl.swapRB();
    pDevice->Clear(0 , NULL , D3DCLEAR_TARGET |  D3DCLEAR_ZBUFFER,cl.color , z , stencil );
    return true;
}

//==================窗口化的====================================================
xD3D9RenderWindow::xD3D9RenderWindow(int iSwapChain , HWND hWnd , xD3D9RenderApi* pRenderApi) : xD3D9RenderView(pRenderApi , xRTSampleDesc::Defualt),m_WindowRT(pRenderApi ,m_TextureDesc )
{
	m_hWnd = hWnd;
    m_pD3D9RenderApi = pRenderApi;
    m_iSwapChain = iSwapChain;

    for(int i = 0 ; i < MAX_RENDER_TARGET ; i ++)
    {
        m_RenderTargetsView[i] = NULL;
    }

    IDirect3DDevice9* pDevice = pRenderApi->getDevice();
    
    pDevice->GetSwapChain(iSwapChain , &m_pSwapChain);
    m_pSwapChain->GetPresentParameters(&m_d3dpp);

}

HRESULT  xD3D9RenderWindow::Present(CONST RECT * pSourceRect,    CONST RECT * pDestRect, HWND hDestWindowOverride, CONST RGNDATA * pDirtyRegion, DWORD dwFlags)
{
   return  m_pSwapChain->Present(pSourceRect,pDestRect, hDestWindowOverride, pDirtyRegion, dwFlags);
}

bool xD3D9RenderWindow::NeedResize(int width , int height)
{
     if(m_d3dpp.BackBufferWidth== width && m_d3dpp.BackBufferHeight == height)
         return false;
     return true;
}

xD3D9RenderWindow::~xD3D9RenderWindow()
{
	destory();
}


bool xD3D9RenderWindow::resize(int w , int h)
{
    m_pSwapChain->GetPresentParameters(&m_d3dpp);
    m_pSwapChain->Release();
    IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
    for(int i = 0 ; i < MAX_RENDER_TARGET ; i ++)
    {
        XSAFE_RELEASE(m_RenderTargetsView[i]);
    }
    XSAFE_RELEASE(m_pDepthStencilView);
    while(1)
    {
        m_d3dpp.BackBufferWidth = w;
        m_d3dpp.BackBufferHeight = h;
        HRESULT hr = pDevice->Reset(&m_d3dpp);
        if(hr == D3DERR_DEVICELOST)
        {
            continue;
        }
        else if(hr == D3DERR_DEVICELOST)
        {
            MessageBoxW(NULL,L"D3D Driver Error Exit the application!",L"Error",MB_OK);
            //exit(0);
            break;
        }
        else if(hr == D3DERR_OUTOFVIDEOMEMORY)
        {
            MessageBoxW(NULL,L"D3D Driver Error Exit the application!",L"Error",MB_OK);
            exit(0);
            break;
        }
        else if(hr == S_OK)
        {
            break;
        }
    }    

    pDevice->GetSwapChain(m_iSwapChain , &m_pSwapChain);
    m_pSwapChain->GetPresentParameters(&m_d3dpp);
    return create(w , h);
}

bool xD3D9RenderWindow::_createRenderTargets()
{

	return false;
}

bool xD3D9RenderWindow::desc(xTextureDesc& _desc)
{
	_desc = m_WindowRT.m_TexDesc;
	return true;
}

bool xD3D9RenderWindow::create(int width , int height)
{
    m_pSwapChain->GetPresentParameters(&m_d3dpp);

    IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();

    pDevice->GetDepthStencilSurface(&m_pDepthStencilView);
    IDirect3DSurface9* pRT = NULL;
    pDevice->GetRenderTarget(0 , &pRT);
    m_RenderTargetsView[0] = pRT;

	m_WindowRT.m_TexDesc.m_fmt = PIXELFORMAT_R8G8B8A8;
	m_widht  = width;
	m_height = height;
	m_WindowRT.m_TexDesc.m_width  = m_d3dpp.BackBufferWidth;
	m_WindowRT.m_TexDesc.m_height = m_d3dpp.BackBufferHeight;
	m_WindowRT.m_TexDesc.m_nArraySize = 1;
	m_WindowRT.m_TexDesc.m_depth  = 1;
    m_WindowRT.m_pRenderTargetView = pRT;

	m_RenderTargets[0] = &m_WindowRT;
	return _createRenderTargets();
}

bool  xD3D9RenderWindow::destory()
{
	m_RenderTargets[0] = NULL;
    XSAFE_RELEASE(m_pSwapChain);
    XSAFE_RELEASE(m_pDepthStencilView);
	XSAFE_RELEASE(m_RenderTargetsView[0]);
	return xD3D9RenderView::destory();
}

IDirect3DSurface9*  xD3D9RenderWindow::GetDepthStencilView()
{ 
    XEVOL_LOG(eXL_DEBUG_TIPS , L"xD3D9RenderWindow::GetDepthStencilView()   ---> AddRef() \n");
    m_pDepthStencilView->AddRef();
    return m_pDepthStencilView ; 
}

void  xD3D9RenderWindow::onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action)
{

}

END_NAMESPACE_XEVOL3D
