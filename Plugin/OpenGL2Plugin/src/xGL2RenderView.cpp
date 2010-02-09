#include "xGL2RenderView.h"
#include "xOpenGL2API.h"
#include "xGL2RenderTarget.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xGL2RenderView   , IRenderView);
IMPL_BASE_OBJECT_CLASSID(xGL2RenderWindow , xGL2RenderView);

xGL2RenderView::xGL2RenderView(xGL2RenderApi* pRenderApi) : IRenderView(pRenderApi)
{
	m_RefCount = 1;
	m_pD10Api = pRenderApi;
	for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
	{
		m_RenderTargetsView[i] = NULL;
		m_RenderTargets[i] = NULL;
	}
	m_DepthTexture      = NULL;
	m_pDepthStencilView    = NULL;
	m_DepthBuffer          = NULL;

    pRenderApi->getWindowSize(m_widht , m_height );
}

xGL2RenderView::~xGL2RenderView()
{
	destory();
}
bool  xGL2RenderView::destory()
{
    XSAFE_RELEASEOBJECT(m_DepthBuffer);
	XSAFE_RELEASEOBJECT(m_DepthTexture);
	for(size_t i = startIdx() ; i < MAX_RENDER_TARGET ; i ++ )
	{
		XSAFE_RELEASEOBJECT(m_RenderTargets[i]);
	}
	return true;
}

bool  xGL2RenderView::createRenderTarget(int nRenderTarget , ePIXEL_FORMAT fmt , bool bLockable , bool bAsTexture)
{
	int w = m_widht;
	int h = m_height;
	for(size_t i = 0 ; i < (size_t)nRenderTarget ; i ++ )
	{
		IRenderTarget* pRenderTarget = m_pD10Api->createRenderTarget(w , h ,fmt , bLockable , bAsTexture);
		setRenderTarget(pRenderTarget , i);
        XSAFE_RELEASEOBJECT(pRenderTarget);
	}
	return setup();
}

int  xGL2RenderView::nRenderTargetView()
{
	for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
	{
		if( m_RenderTargetsView[i] == NULL)
			return (int)i;
	}
	return MAX_RENDER_TARGET;
}

bool xGL2RenderView::setRenderTarget(IBaseTexture*  pTexture , size_t rtIdx) 
{
	if( (int)rtIdx < startIdx() || rtIdx >= MAX_RENDER_TARGET )
		return false;

	if(pTexture == NULL)
		return false;
	return setRenderTarget(pTexture , rtIdx);
}

bool xGL2RenderView::setRenderTarget(IRenderTarget* pRenderTarget , size_t rtIdx)
{
	if( (int)rtIdx < startIdx() || rtIdx >= MAX_RENDER_TARGET )
		return false;

	if(pRenderTarget == NULL)
		return false;
	
	ID3D10RenderTargetView* pRTV = (ID3D10RenderTargetView*)pRenderTarget->handle();
	if(pRTV == NULL)
	   return false ; 

	//增加引用计数
	pRenderTarget->AddRef();
	if(m_RenderTargets[rtIdx]) m_RenderTargets[rtIdx]->ReleaseObject();
	m_RenderTargets[rtIdx] = pRenderTarget;
	m_RenderTargetsView[rtIdx] = pRTV;
	return true;
}
bool xGL2RenderView::setupDepthView(int w , int h , bool bSet)
{
    if( false == m_DepthTexture->create(w , h , PIXELFORMAT_DEPTH32) )
    {
        return false;
    }
    if(bSet)
    {
        return setDepthBuffer(m_DepthTexture);
    }
    return true;
}

bool xGL2RenderView::createDepthView(int w , int h)
{
    HRESULT hr = S_OK;
    ID3D10Device* pDevice = m_pD10Api->d10Device();
    XSAFE_RELEASEOBJECT(m_DepthTexture);
    m_DepthTexture = new xGL2DepthTexture(false , false , m_pD10Api);
    if(setupDepthView( w , h , true) == false)
    {
         XSAFE_RELEASEOBJECT(m_DepthTexture);
         return false;
    }
    return true;
}
bool xGL2RenderView::setDepthBuffer(IBaseTexture* pDepthTexture)
{
    if(pDepthTexture == NULL)
        return false;

    IRenderTarget* pRenderTarget = pDepthTexture->toRenderTarget();
    bool bRet = setDepthBuffer( pRenderTarget );
    XSAFE_RELEASEOBJECT(pRenderTarget);
    return bRet;
}

bool xGL2RenderView::setDepthBuffer(IRenderTarget* pDepthBuffer) 
{
	if(pDepthBuffer == NULL) 
	{
		if(m_DepthTexture == NULL)
			return false;
        return setDepthBuffer(m_DepthTexture);
	}

	ID3D10DepthStencilView* pDSV = (ID3D10DepthStencilView*)pDepthBuffer->handle();
	XSAFE_RELEASEOBJECT(m_DepthBuffer) ;
	pDepthBuffer->AddRef();
	m_DepthBuffer = pDepthBuffer;
	m_pDepthStencilView = pDSV;

	xTextureDesc desc;
	m_DepthBuffer->desc( desc);
	m_widht   = desc.m_width;
	m_height  = desc.m_height;
	return m_pDepthStencilView != NULL ; 
}

bool xGL2RenderView::resize(int w , int h  , bool bDestorRT)
{
	TAutoLocker<IRenderApi> aLocker(m_pD10Api);
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

bool xGL2RenderView::resize(int w , int h)
{
	TAutoLocker<IRenderApi> aLocker(m_pD10Api);
	if( m_DepthTexture )
	{
		m_DepthTexture->unload();
        bool bSet = m_DepthTexture->isSameInstance(m_DepthBuffer);
		return setupDepthView( w , h , bSet);
	}
	return false;
}

ID3D10DepthStencilView* xGL2RenderView::depthView()
{
    ID3D10DepthStencilView* pDepthStencilView = m_pDepthStencilView;
    if(pDepthStencilView == NULL) pDepthStencilView = m_pD10Api->DefDepthStencilView();
    return pDepthStencilView;
}

int xGL2RenderView::addRenderTarget(IBaseTexture*  pTexture)
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

int xGL2RenderView::addRenderTarget(IRenderTarget* pRenderTarget) 
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

IRenderTarget*  xGL2RenderView::depthBuffer()
{ 
	if(m_DepthBuffer) 
	{
        m_DepthBuffer->AddRef();
		return m_DepthBuffer;
	}
	return m_DepthTexture->toRenderTarget();
}

bool  xGL2RenderView::depthBufferDesc(xTextureDesc& _desc )
{
    if(m_DepthBuffer) 
    {
        return m_DepthBuffer->desc(_desc);
    }
    return m_DepthTexture->desc(_desc);
}
IRenderApiObject*  xGL2RenderView::renderTarget(size_t rtIdx )
{ 
	if( (int)rtIdx < startIdx() || rtIdx >= MAX_RENDER_TARGET )
		return NULL;
	return m_RenderTargets[rtIdx] ; 
}



bool xGL2RenderView::install()
{
	ID3D10Device* pDevice = m_pD10Api->d10Device();
	int nRV = nRenderTargetView();
	if(nRV == 0)
		return false;
	pDevice->OMSetRenderTargets(nRV , m_RenderTargetsView , depthView() );
	
	GL2_VIEWPORT vp;
	xTextureDesc _desc;
	desc(_desc);
	vp.Width  = _desc.m_width;
	vp.Height = _desc.m_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDevice->RSSetViewports( 1, &vp );
	return true;
}

bool xGL2RenderView::setup()
{
	int nRT  = nRenderTargetView();
    if(m_TextureDesc.m_nArraySize == 0)
		return false;
	xTextureDesc _desc;
	depthBufferDesc(_desc);
	for(size_t i = 0 ; i < (size_t)nRT ; i ++)
	{
		if(m_RenderTargets[i]) 
        {
            m_RenderTargets[i]->desc( m_TextureDesc );
		    if(_desc.m_fmt != m_TextureDesc.m_fmt || _desc.m_width != m_TextureDesc.m_width || _desc.m_height != m_TextureDesc.m_height)
			   return false;
        }
	}
	m_TextureDesc.m_nArraySize = nRT;
	return true;
}
bool xGL2RenderView::clear(xColor_4f& bkColor)
{
	TAutoLocker<IRenderApi> aLocker(m_pD10Api);
	ID3D10Device* pDevice = m_pD10Api->d10Device();
	pDevice->ClearRenderTargetView( m_RenderTargetsView[0]  , &bkColor.r );
	return true;
}

bool xGL2RenderView::clear(xColor_4f& bkColor , float z , unsigned int stencil)
{
    TAutoLocker<IRenderApi> aLocker(m_pD10Api);
	ID3D10Device* pDevice = m_pD10Api->d10Device();
	pDevice->ClearRenderTargetView( m_RenderTargetsView[0]  , &bkColor.r );
	pDevice->ClearDepthStencilView( depthView() , GL2_CLEAR_DEPTH | GL2_CLEAR_STENCIL , z, stencil );
	return true;
}
bool xGL2RenderView::clear(xColor_4f& bkColor, int nClear)
{
	TAutoLocker<IRenderApi> aLocker(m_pD10Api);
	ID3D10Device* pDevice = m_pD10Api->d10Device();
	for(int i = 0 ;  i < nClear ; i ++)
	{
		if(m_RenderTargetsView[i]) pDevice->ClearRenderTargetView( m_RenderTargetsView[i]  , &bkColor.r );
	}
	return true;
}
bool xGL2RenderView::clear(xColor_4f& bkColor ,  float z , unsigned int stencil, int nClear )
{
	TAutoLocker<IRenderApi> aLocker(m_pD10Api);
	ID3D10Device* pDevice = m_pD10Api->d10Device();
	for(int i = 0 ;  i < nClear ; i ++)
	{
		if(m_RenderTargetsView[i]) pDevice->ClearRenderTargetView( m_RenderTargetsView[i]  , &bkColor.r );
	}
	pDevice->ClearDepthStencilView( depthView() , GL2_CLEAR_DEPTH | GL2_CLEAR_STENCIL , z, stencil );
	return true;
}

//==================窗口化的====================================================
xGL2RenderWindow::xGL2RenderWindow(HWND hWnd , xGL2RenderApi* pRenderApi) : xGL2RenderView(pRenderApi),m_WindowRT(pRenderApi ,m_TextureDesc )
{
	m_hWnd = hWnd;
	m_pSwapChain = NULL;
	m_pD10Api = pRenderApi;
}

xGL2RenderWindow::~xGL2RenderWindow()
{
	destory();
}

bool xGL2RenderWindow::resize(int w , int h)
{
    TAutoLocker<IRenderApi> aLocker(m_pD10Api);
	if(m_widht == w && m_height == h )
		return true; 
	m_pD10Api->d10Device()->OMSetRenderTargets(0 , NULL , NULL );

    XSAFE_RELEASEOBJECT(m_DepthBuffer);
	XSAFE_UNLOAD(m_DepthTexture);
	XSAFE_RELEASE(m_RenderTargetsView[0]);
	DXGI_SWAP_CHAIN_FLAG swcFlag = (DXGI_SWAP_CHAIN_FLAG)0;
	m_widht  = w;
	m_height = h;
	DXGI_SWAP_CHAIN_DESC _desc;
	m_pSwapChain->GetDesc(&_desc);
	HRESULT hr = m_pSwapChain->ResizeBuffers(_desc.BufferCount , m_widht , m_height , _desc.BufferDesc.Format , swcFlag);
    return _createRenderTargets();
}

bool xGL2RenderWindow::_createRenderTargets()
{
	ID3D10Device* pDevice = m_pD10Api->d10Device();
	HRESULT hr = S_OK;
	// Create a render target view
	ID3D10Texture2D *pBackBuffer;
	hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (LPVOID*)&pBackBuffer );
	if( FAILED(hr) )
		return false;

	hr = pDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_RenderTargetsView[0] );
	pBackBuffer->Release();
	if( FAILED(hr) )
		return false;

	m_WindowRT.m_TexDesc.m_width  = m_widht;
	m_WindowRT.m_TexDesc.m_height = m_height;
	//重新创建Depth Stencil view
    if( createDepthView(m_widht , m_height) )
	{
		return 	setup();
	}
	return false;
}

bool xGL2RenderWindow::desc(xTextureDesc& _desc)
{
	_desc = m_WindowRT.m_TexDesc;
	return true;
}

bool xGL2RenderWindow::create(IDXGISwapChain* pSwapChain , int width , int height)
{
	m_pSwapChain = pSwapChain;
	DXGI_SWAP_CHAIN_DESC _desc;
	m_pSwapChain->GetDesc(&_desc);
	xGL2GIFormatInfo* pFmtInfo = xGL2ConstLexer::singleton()->GetPixelFormat(_desc.BufferDesc.Format);
	if(pFmtInfo == NULL)
	{
		m_WindowRT.m_TexDesc.m_fmt = PIXELFORMAT_R8G8B8A8;
	}
	else 
	{
		m_WindowRT.m_TexDesc.m_fmt = pFmtInfo->m_fmt;
	}

	m_widht  = width;
	m_height = height;
	m_WindowRT.m_TexDesc.m_width  = m_widht;
	m_WindowRT.m_TexDesc.m_height = m_height;
	m_WindowRT.m_TexDesc.m_nArraySize = 1;
	m_WindowRT.m_TexDesc.m_depth  = 1;

	m_RenderTargets[0] = &m_WindowRT;
	return _createRenderTargets();
}

bool  xGL2RenderWindow::destory()
{
	m_RenderTargets[0] = NULL;
	XSAFE_RELEASE(m_RenderTargetsView[0]);
	return xGL2RenderView::destory();
}

END_NAMESPACE_XEVOL3D
