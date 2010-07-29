#include "stdafx.h"
#include "xD11RenderView.h"
#include "xDirect3D11API.h"
#include "xD11RenderTarget.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xD11RenderView   , IRenderView);
IMPL_BASE_OBJECT_CLASSID(xD11RenderWindow , xD11RenderView);

xD11RenderView::xD11RenderView(xD3D11RenderApi* pRenderApi , const xRTSampleDesc& sampleDesc) : IRenderView(pRenderApi , sampleDesc)
{
	m_RefCount = 1;
	m_pD11RenderApi = pRenderApi;
	for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
	{
		m_RenderTargetsView[i] = NULL;
		m_RenderTargets[i] = NULL;
	}
	m_DepthTexture      = NULL;
	m_pDepthStencilView    = NULL;
	m_DepthBuffer          = NULL;

    pRenderApi->getWindowSize(m_widht , m_height );
    memset( &m_TextureDesc , 0 , sizeof(m_TextureDesc) );
    
	m_DXGISampleDesc = pRenderApi->GetDXGISampleDesc(m_RTSampleDesc);
}

xD11RenderView::~xD11RenderView()
{
	destory();
}
bool  xD11RenderView::destory()
{
    XSAFE_RELEASEOBJECT(m_DepthBuffer);
	XSAFE_RELEASEOBJECT(m_DepthTexture);
	for(size_t i = startIdx() ; i < MAX_RENDER_TARGET ; i ++ )
	{
		XSAFE_RELEASEOBJECT(m_RenderTargets[i]);
	}
	return true;
}

bool  xD11RenderView::createRenderTarget(int nRenderTarget , ePIXEL_FORMAT fmt , bool bLockable , bool bAsTexture)
{
	int w = m_widht;
	int h = m_height;
	for(size_t i = 0 ; i < (size_t)nRenderTarget ; i ++ )
	{
		IRenderTarget* pRenderTarget = m_pD11RenderApi->createRenderTarget(w , h ,fmt , bLockable , bAsTexture , m_RTSampleDesc);
		setRenderTarget(pRenderTarget , i);
        XSAFE_RELEASEOBJECT(pRenderTarget);
	}
	return setup();
}

int  xD11RenderView::nRenderTargetView()
{
	for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
	{
		if( m_RenderTargetsView[i] == NULL)
			return (int)i;
	}
	return MAX_RENDER_TARGET;
}

bool xD11RenderView::setRenderTarget(IBaseTexture*  pTexture , size_t rtIdx, size_t iSlice , size_t iMipMapLevel) 
{
	if( (int)rtIdx < startIdx() || rtIdx >= MAX_RENDER_TARGET )
		return false;

	if(pTexture == NULL)
		return setRenderTarget( (IRenderTarget*)NULL , rtIdx );
	return setRenderTarget(pTexture->toRenderTarget(iSlice , iMipMapLevel) , rtIdx);
}

bool xD11RenderView::setRenderTarget(IRenderTarget* pRenderTarget , size_t rtIdx)
{
	if( (int)rtIdx < startIdx() || rtIdx >= MAX_RENDER_TARGET )
		return false;

	if(pRenderTarget == NULL)
	{
		if(m_RenderTargets[rtIdx]) m_RenderTargets[rtIdx]->ReleaseObject();
		m_RenderTargets[rtIdx] = NULL;
		m_RenderTargetsView[rtIdx] = NULL;
		return true;
	}
	
	ID3D11RenderTargetView* pRTV = (ID3D11RenderTargetView*)pRenderTarget->handle();
	if(pRTV == NULL)
	   return false ; 

	//增加引用计数
	pRenderTarget->AddRef();
	if(m_RenderTargets[rtIdx]) m_RenderTargets[rtIdx]->ReleaseObject();
	m_RenderTargets[rtIdx] = pRenderTarget;
	m_RenderTargetsView[rtIdx] = pRTV;
	return true;
}
bool xD11RenderView::setupDepthView(int w , int h , bool bSet)
{
    xTextureInitDesc initDesc(w , h , PIXELFORMAT_DEPTH32);
    if( false == m_DepthTexture->create( initDesc , NULL , 0 ) )
    {
        return false;
    }
    if(bSet)
    {
        return setDepthBuffer(m_DepthTexture);
    }
    return true;
}

bool xD11RenderView::createDepthView(int w , int h)
{
    HRESULT hr = S_OK;
    ID3D11Device* pDevice = m_pD11RenderApi->d11Device();
    XSAFE_RELEASEOBJECT(m_DepthTexture);
    m_DepthTexture = new xD11DepthTexture(false , false , m_pD11RenderApi , m_DXGISampleDesc );
    if(setupDepthView( w , h , true) == false)
    {
         XSAFE_RELEASEOBJECT(m_DepthTexture);
         return false;
    }
    return true;
}

bool xD11RenderView::setDepthBuffer(IBaseTexture* pDepthTexture)
{
    if(pDepthTexture == NULL)
        return false;

    IRenderTarget* pRenderTarget = pDepthTexture->toRenderTarget();
    bool bRet = setDepthBuffer( pRenderTarget );
    XSAFE_RELEASEOBJECT(pRenderTarget);
    return bRet;
}

bool xD11RenderView::setDepthBuffer(IRenderTarget* pDepthBuffer) 
{
	if(pDepthBuffer == NULL) 
	{
		if(m_DepthTexture == NULL)
			return false;
        return setDepthBuffer(m_DepthTexture);
	}

	ID3D11DepthStencilView* pDSV = (ID3D11DepthStencilView*)pDepthBuffer->handle();
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

bool xD11RenderView::resize(int w , int h  , bool bDestorRT)
{
	TAutoLocker<IRenderApi> aLocker(m_pD11RenderApi);
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

bool xD11RenderView::resize(int w , int h)
{
	TAutoLocker<IRenderApi> aLocker(m_pD11RenderApi);
	if( m_DepthTexture )
	{
		m_DepthTexture->unload();
        bool bSet = m_DepthTexture->isSameInstance(m_DepthBuffer);
		return setupDepthView( w , h , bSet);
	}
	return false;
}

ID3D11DepthStencilView* xD11RenderView::depthView()
{
    ID3D11DepthStencilView* pDepthStencilView = m_pDepthStencilView;
    if(pDepthStencilView == NULL) pDepthStencilView = m_pD11RenderApi->DefDepthStencilView();
    return pDepthStencilView;
}

int xD11RenderView::addRenderTarget(IBaseTexture*  pTexture, size_t iSlice , size_t iMipMapLevel)
{ 
	for(size_t i = 0 ; i < MAX_RENDER_TARGET ; i ++ )
	{
		if( m_RenderTargetsView[i] == NULL)
		{
			if(setRenderTarget(pTexture , i , iSlice , iMipMapLevel) == false)
				return (int)i ;
			return (int)i + 1;
		}
	}
	return MAX_RENDER_TARGET;
}

int xD11RenderView::addRenderTarget(IRenderTarget* pRenderTarget) 
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

IRenderTarget*  xD11RenderView::depthBuffer()
{ 
	if(m_DepthBuffer) 
	{
        m_DepthBuffer->AddRef();
		return m_DepthBuffer;
	}
	return m_DepthTexture->toRenderTarget();
}

bool  xD11RenderView::depthBufferDesc(XD3D11_TEXTURE2D_DESC& _desc )
{
	ID3D11DepthStencilView* pDepthView = depthView();
	if(pDepthView == NULL)
		return false;

	ID3D11Resource* pResource = NULL;
	pDepthView->GetResource(&pResource);
	ID3D11Texture2D* pDepthTexture = NULL;

	pResource->QueryInterface(IID_ID3D11Texture2D , (void**)&pDepthTexture);
	if(pDepthTexture == NULL)
	{
		pResource->Release();
		return false;
	}

	pDepthTexture->GetDesc( &_desc );
	pDepthTexture->Release();
	pResource->Release();
	return true;
}

IRenderApiObject*  xD11RenderView::renderTarget(size_t rtIdx )
{ 
	if( (int)rtIdx < startIdx() || rtIdx >= MAX_RENDER_TARGET )
		return NULL;
	return m_RenderTargets[rtIdx] ; 
}



bool xD11RenderView::install()
{
	ID3D11DeviceContext* pDeviceContext = m_pD11RenderApi->d11DeviceContext();
	int nRV = nRenderTargetView();
	if(nRV == 0)
		return false;
	pDeviceContext->OMSetRenderTargets(nRV , m_RenderTargetsView , depthView() );
	
	D3D11_VIEWPORT vp;
	xTextureDesc _desc;
	desc(_desc);
	vp.Width  = (FLOAT)_desc.m_width;
	vp.Height = (FLOAT)_desc.m_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDeviceContext->RSSetViewports( 1, &vp );
	return true;
}

bool xD11RenderView::setup()
{
	int nRT  = nRenderTargetView();
	XD3D11_TEXTURE2D_DESC _desc;
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
bool xD11RenderView::clear(xColor_4f& bkColor)
{
	TAutoLocker<IRenderApi> aLocker(m_pD11RenderApi);
	ID3D11DeviceContext* pDeviceContext = m_pD11RenderApi->d11DeviceContext();
	pDeviceContext->ClearRenderTargetView( m_RenderTargetsView[0]  , &bkColor.r );
	return true;
}

bool xD11RenderView::clearRenderTarget(xColor_4f& bkColor, int idx)
{
	TAutoLocker<IRenderApi> aLocker(m_pD11RenderApi);
	ID3D11DeviceContext* pDeviceContext = m_pD11RenderApi->d11DeviceContext();
	ID3D11Device* pDevice = m_pD11RenderApi->d11Device();
	if(m_RenderTargetsView[idx]) pDeviceContext->ClearRenderTargetView( m_RenderTargetsView[idx]  , &bkColor.r );
	return true;
}

bool xD11RenderView::clear(xColor_4f& bkColor , float z , unsigned int stencil)
{
    TAutoLocker<IRenderApi> aLocker(m_pD11RenderApi);
	ID3D11DeviceContext* pDeviceContext = m_pD11RenderApi->d11DeviceContext();
	pDeviceContext->ClearRenderTargetView( m_RenderTargetsView[0]  , &bkColor.r );
	pDeviceContext->ClearDepthStencilView( depthView() , D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL , z, stencil );
	return true;
}

bool xD11RenderView::clear(xColor_4f& bkColor, int nClear)
{
	TAutoLocker<IRenderApi> aLocker(m_pD11RenderApi);
	ID3D11DeviceContext* pDeviceContext = m_pD11RenderApi->d11DeviceContext();
	ID3D11Device* pDevice = m_pD11RenderApi->d11Device();
	for(int i = 0 ;  i < nClear ; i ++)
	{
		if(m_RenderTargetsView[i]) pDeviceContext->ClearRenderTargetView( m_RenderTargetsView[i]  , &bkColor.r );
	}
	return true;
}

bool xD11RenderView::clear(xColor_4f& bkColor ,  float z , unsigned int stencil, int nClear )
{
	TAutoLocker<IRenderApi> aLocker(m_pD11RenderApi);
	ID3D11Device* pDevice = m_pD11RenderApi->d11Device();
	ID3D11DeviceContext* pDeviceContext = m_pD11RenderApi->d11DeviceContext();
	for(int i = 0 ;  i < nClear ; i ++)
	{
		if(m_RenderTargetsView[i]) pDeviceContext->ClearRenderTargetView( m_RenderTargetsView[i]  , &bkColor.r );
	}
	pDeviceContext->ClearDepthStencilView( depthView() , D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL , z, stencil );
	return true;
}

//==================窗口化的====================================================
xD11RenderWindow::xD11RenderWindow(HWND hWnd , xD3D11RenderApi* pRenderApi) : xD11RenderView(pRenderApi , xRTSampleDesc::Defualt),m_WindowRT(pRenderApi ,m_TextureDesc )
{
	m_hWnd = hWnd;
	m_pSwapChain = NULL;
	m_pD11RenderApi = pRenderApi;
}

xD11RenderWindow::~xD11RenderWindow()
{
	destory();
}

bool xD11RenderWindow::resize(int w , int h)
{
    TAutoLocker<IRenderApi> aLocker(m_pD11RenderApi);
	if(m_widht == w && m_height == h )
		return true; 
	m_pD11RenderApi->d11DeviceContext()->OMSetRenderTargets(0 , NULL , NULL );

    XSAFE_RELEASEOBJECT(m_DepthBuffer);
	XSAFE_UNLOAD(m_DepthTexture);
	XSAFE_RELEASE(m_RenderTargetsView[0]);
	DXGI_SWAP_CHAIN_FLAG swcFlag = (DXGI_SWAP_CHAIN_FLAG)0;
	m_widht  = w;
	m_height = h;
	DXGI_SWAP_CHAIN_DESC _desc;
	m_pSwapChain->GetDesc(&_desc);
	m_pD11RenderApi->getRTSampleDesc(m_RTSampleDesc);
	HRESULT hr = m_pSwapChain->ResizeBuffers(_desc.BufferCount , m_widht , m_height , _desc.BufferDesc.Format , swcFlag);
    return _createRenderTargets();
}

bool xD11RenderWindow::_createRenderTargets()
{
	ID3D11Device* pDevice = m_pD11RenderApi->d11Device();
	HRESULT hr = S_OK;
	// Create a render target view
	ID3D11Texture2D *pBackBuffer;
	hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer );
	if( FAILED(hr) )
		return false;

	hr = pDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_RenderTargetsView[0] );
	pBackBuffer->Release();
	if( FAILED(hr) )
		return false;

	m_WindowRT.m_TexDesc.m_width  = m_widht;
	m_WindowRT.m_TexDesc.m_height = m_height;
    DXGI_SAMPLE_DESC DXGISampleDesc ;
    m_pD11RenderApi->GetDXGISampleDesc(DXGISampleDesc);
	m_pD11RenderApi->getRTSampleDesc(m_RTSampleDesc);
    //重新创建Depth Stencil view
    if( createDepthView(m_widht , m_height) )
	{
		return 	setup();
	}
	return false;
}

bool xD11RenderWindow::desc(xTextureDesc& _desc)
{
	_desc = m_WindowRT.m_TexDesc;
	return true;
}

bool xD11RenderWindow::create(IDXGISwapChain* pSwapChain , int width , int height)
{
	m_pSwapChain = pSwapChain;
	m_pSwapChain->GetDesc(&m_SwapChainDesc);
    m_DXGISampleDesc = m_pD11RenderApi->GetDXGISampleDesc(m_RTSampleDesc);
	xD11GIFormatInfo* pFmtInfo = xD11ConstLexer::singleton()->GetPixelFormat(m_SwapChainDesc.BufferDesc.Format);
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

bool  xD11RenderWindow::destory()
{
	m_RenderTargets[0] = NULL;
	XSAFE_RELEASE(m_RenderTargetsView[0]);
	return xD11RenderView::destory();
}

bool xD11RenderWindow::NeedResize(int width , int height )
{
    if(m_SwapChainDesc.BufferDesc.Width == width && m_SwapChainDesc.BufferDesc.Height == height)
    {
        return false;
    }
    return true;
}
bool xD11RenderWindow::Present(UINT syncInterval , UINT Flags)
{
    m_pSwapChain->Present( 0, 0 );
    return true;
}

bool xD11RenderWindow::GetSwapChainDesc( DXGI_SWAP_CHAIN_DESC& swap_desc)
{
    m_pSwapChain->GetDesc(&swap_desc);
    return true;
}

DXGI_SWAP_CHAIN_DESC& xD11RenderWindow::SwapChainDesc()
{
    return m_SwapChainDesc;
}

END_NAMESPACE_XEVOL3D
