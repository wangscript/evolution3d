#include "stdafx.h"
#include "xD3D9BaseTexture.h"
#include "xD3D9RenderTarget.h"
#include "xD3D9ConstLexer.h"
#include "xDIRECT3D9API.h"
#include <BaseLib/xI18N.h>
#include <BaseLib/xBaseMath.h>
#include <OperationSys/xOperationSys.h>
#include "Image/PixelFormatAssit.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xD3D9RenderTarget        , xD3D9BaseRenderTarget);
IMPL_BASE_OBJECT_CLASSID(xD3D9DepthBuffer         , xD3D9BaseRenderTarget);
IMPL_BASE_OBJECT_CLASSID(xD3D9DynamicRenderTarget , xD3D9BaseRenderTarget);
xD3D9RenderTarget::xD3D9RenderTarget(xD3D9RenderApi* pAPI , int arraySlice , int mipmapLevel):xD3D9BaseRenderTarget(pAPI,arraySlice , mipmapLevel)
{
	m_RefCount = 1;
	m_pRenderTargetView = NULL;
    m_bResourceLosted = false;
}
int xD3D9RenderTarget::RefCount()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->RefCount();
	return xD3D9BaseRenderTarget::RefCount();
}

int xD3D9RenderTarget::AddRef()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->AddRef();
	return xD3D9BaseRenderTarget::AddRef();
}

int xD3D9RenderTarget::ReleaseObject()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->ReleaseObject();
	return xD3D9BaseRenderTarget::ReleaseObject();
}

int xD3D9RenderTarget::KillObject()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->KillObject();
	return xD3D9BaseRenderTarget::KillObject();
}

xD3D9RenderTarget::~xD3D9RenderTarget()
{
	destory();   
}

bool xD3D9RenderTarget::destory()
{
	XSAFE_RELEASE(m_pRenderTargetView);  
	return true;
}

bool xD3D9RenderTarget::saveToFile(const wchar_t* fileName)
{
	if(m_pRenderTargetView == NULL)
		return false;
	if(saveResourceToFile(fileName , m_pRenderTargetView) == true)
	{
		return true;
	}
	return false;
}

void* xD3D9RenderTarget::handle()
{ 
    if(m_bResourceLosted == true)
    {      
        m_pD3D9BaseTexture->onDeviceReset(false , ID3D9DeviceLostListener::eReset_Finish);
        //create(m_pD3D9BaseTexture , m_ArraySlice , m_MipmapLevel);
    }

    if(m_pRenderTargetView) 
        m_pRenderTargetView->AddRef() ;  
    return m_pRenderTargetView ;
}

void xD3D9RenderTarget::onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action)
{
    XSAFE_RELEASE(m_pRenderTargetView);
    m_bResourceLosted = true;
}

bool xD3D9RenderTarget::create(xD3D9BaseTexture* pBaseTexture , int iSlice , int mipMapLevel)
{
    m_bResourceLosted = false;
    XSAFE_RELEASE(m_pRenderTargetView);
    m_ArraySlice        = iSlice;
    m_MipmapLevel       = mipMapLevel;
    m_pBaseTexture      = pBaseTexture;
    m_pD3D9BaseTexture  = pBaseTexture;
    xD3D9TexInfo& TexInfo = pBaseTexture->texInfo();

    IDirect3DBaseTexture9* pTexture = pBaseTexture->BaseDXTexture();

    IDirect3DTexture9* p2DTexture = NULL;
    pTexture->QueryInterface(IID_IDirect3DTexture9 , (void**) &p2DTexture);
    if(p2DTexture) 
    {
        p2DTexture->GetSurfaceLevel(mipMapLevel , &m_pRenderTargetView);
        p2DTexture->Release();
        return true;
    }

    IDirect3DCubeTexture9* pCubeTexture = NULL;
    pTexture->QueryInterface(IID_IDirect3DCubeTexture9 , (void**) &pCubeTexture);
    if(pCubeTexture )
    {
        pCubeTexture->GetCubeMapSurface((D3DCUBEMAP_FACES)iSlice , mipMapLevel, &m_pRenderTargetView);
        pCubeTexture->Release();
        return true;
    }
    return false;
}

void xD3D9DynamicRenderTarget::onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action)
{
    XSAFE_RELEASE(m_pRenderTargetView);
    m_bResourceLosted = true;
}
void* xD3D9DynamicRenderTarget::handle()
{ 
    if(m_bResourceLosted == true)
    {      
        m_pD3D9BaseTexture->onDeviceReset(false , ID3D9DeviceLostListener::eReset_Finish);
        //create(m_pD3D9BaseTexture , m_ArraySlice , m_MipmapLevel);
    }

    if(m_pRenderTargetView) 
        m_pRenderTargetView->AddRef() ;  
    return m_pRenderTargetView ;
}

//ÓÃÓÚDepth BufferµÄ DepthTexture;
xD3D9DepthBuffer::xD3D9DepthBuffer(xD3D9RenderApi* pAPI) : xD3D9BaseRenderTarget(pAPI , 0 , 0) 
{ 
    m_pBaseTexture = NULL;
	m_pRenderTargetView = NULL ; 
	m_RefCount = 1;
    m_Width  = 0  ;
    m_Height = 0 ;
    m_Format = D3DFMT_D24S8;
}
bool xD3D9DepthBuffer::desc(xTextureDesc& desc)
{
    if(m_pRenderTargetView == NULL)
        return false;
    D3DSURFACE_DESC _sDesc;
    m_pRenderTargetView->GetDesc(&_sDesc);
    desc.m_depth = 1;
    desc.m_nArraySize = 1;
    desc.m_width = _sDesc.Width;
    desc.m_height = _sDesc.Height;

    xD3D9GIFormatInfo* pFmt = xD3D9ConstLexer::singleton()->GetPixelFormat(_sDesc.Format);
    desc.m_fmt = pFmt->m_fmt;
    return true;
}
xD3D9DepthBuffer::~xD3D9DepthBuffer() 
{
	destory() ;
}
bool xD3D9DepthBuffer::destory()
{
	XSAFE_RELEASE(m_pRenderTargetView);  
	return true;
}

int xD3D9DepthBuffer::RefCount()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->RefCount();
	return xD3D9BaseRenderTarget::RefCount();
}

int xD3D9DepthBuffer::AddRef()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->AddRef();
	return xD3D9BaseRenderTarget::AddRef();
}

int xD3D9DepthBuffer::ReleaseObject()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->ReleaseObject();
	return xD3D9BaseRenderTarget::ReleaseObject();
}

int xD3D9DepthBuffer::KillObject()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->KillObject();
	return xD3D9BaseRenderTarget::KillObject();
}

bool xD3D9DepthBuffer::saveToFile(const wchar_t* fileName)
{

	if(m_pRenderTargetView == NULL)
		return false;
    if(saveResourceToFile(fileName , m_pRenderTargetView) == true)
	{
		return true;
	}
	return false;
}


void xD3D9DepthBuffer::onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action)
{
    if(_Action == ID3D9DeviceLostListener::eReset_Begin)
        XSAFE_RELEASE(m_pRenderTargetView);

    if(_Action == ID3D9DeviceLostListener::eReset_Finish)
    {
        create( m_Width , m_Height , m_Format);
    }
}
void* xD3D9DepthBuffer::handle()
{ 
    if(m_pRenderTargetView) 
        m_pRenderTargetView->AddRef() ;  
    return m_pRenderTargetView ;
}
bool xD3D9DepthBuffer::create( int w , int h , D3DFORMAT fmt )
{
    destory();
    m_Width  = w  ;
    m_Height = h  ;
    m_Format = fmt;
	IDirect3DDevice9* pDevice = m_pD3D9RenderApi->getDevice();
    D3DMULTISAMPLE_TYPE _MsType = D3DMULTISAMPLE_NONE;
    DWORD MultisampleQuality = 1;
	HRESULT hr = pDevice->CreateDepthStencilSurface(w , h , fmt , _MsType , MultisampleQuality , TRUE , &m_pRenderTargetView , NULL);
	return !FAILED(hr);
}


END_NAMESPACE_XEVOL3D