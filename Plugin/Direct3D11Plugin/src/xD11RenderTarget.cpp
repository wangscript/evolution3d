#include "stdafx.h"
#include "xD11LockableTexture.h"
#include "xD11ConstLexer.h"
#include "xDirect3D11API.h"
#include <BaseLib/xI18N.h>
#include <BaseLib/xBaseMath.h>
#include <OperationSys/xOperationSys.h>
#include "Image/PixelFormatAssit.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xD11RenderTarget   , xD11BaseRenderTarget);
IMPL_BASE_OBJECT_CLASSID(xD11RenderTexture  , xD11UnkwonTexture);

IMPL_BASE_OBJECT_CLASSID(xD11DepthBuffer   , xD11BaseRenderTarget);
IMPL_BASE_OBJECT_CLASSID(xD11DepthTexture  , xD11UnkwonTexture);
xD11RenderTarget::xD11RenderTarget(xD3D11RenderApi* pAPI):xD11BaseRenderTarget(pAPI)
{
	m_RefCount = 1;
	m_pRenderTargetView = NULL;
	m_arraySlice = 0;
	m_mipLevel   = 0;
}
int xD11RenderTarget::RefCount()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->RefCount();
	return xD11BaseRenderTarget::RefCount();
}

int xD11RenderTarget::AddRef()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->AddRef();
	return xD11BaseRenderTarget::AddRef();
}

int xD11RenderTarget::ReleaseObject()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->ReleaseObject();
	return xD11BaseRenderTarget::ReleaseObject();
}

int xD11RenderTarget::KillObject()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->KillObject();
	return xD11BaseRenderTarget::KillObject();
}

xD11RenderTarget::~xD11RenderTarget()
{
	destory();   
}

bool xD11RenderTarget::destory()
{
	XSAFE_RELEASE(m_pRenderTargetView);  
	return true;
}

bool xD11RenderTarget::saveToFile(const wchar_t* fileName)
{
	ID3D11Resource* pResource = NULL;

	m_pRenderTargetView->GetResource(&pResource);
	if(pResource == NULL)
		return false;
	if(saveResourceToFile(fileName , pResource) == true)
	{
		pResource->Release();
		return true;
	}
	pResource->Release();
	return false;
}

bool xD11RenderTarget::create(ID3D11Resource* pTexture , xD11TexInfo& TexInfo , DXGI_SAMPLE_DESC& sampleDesc , int iSlice , int mipLevel)
{
	if(m_pRenderTargetView && m_arraySlice == iSlice && m_mipLevel == mipLevel )
		return true;
	destory();
	m_arraySlice = iSlice;
	m_mipLevel   = mipLevel;

	XSAFE_RELEASE(m_pRenderTargetView);   
	m_pRenderTargetView = NULL;

	D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
	rtDesc.Format = TexInfo.m_RTViewFmt;

	if(TexInfo.m_ArraySize > 1)
	{
		rtDesc.Texture2DArray.MipSlice = 0;
		rtDesc.Texture2DArray.FirstArraySlice = 0;
		rtDesc.Texture2DArray.ArraySize       = (UINT)TexInfo.m_ArraySize;
		rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	}
	else if(TexInfo.m_ArraySize == 1)
	{
		if(sampleDesc.Count > 1)
			rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
		else
		    rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtDesc.Texture2D.MipSlice = 0;
	}

	m_pD11RenderApi->d11Device()->CreateRenderTargetView( pTexture, &rtDesc, &m_pRenderTargetView );
	return m_pRenderTargetView != NULL;
}

int xD11DynamicRenderTarget::ReleaseObject()
{
    int RefCount = xD11BaseRenderTarget::ReleaseObject();
    if( RefCount == 0  )
    {
        XSAFE_RELEASEOBJECT(m_pBaseTexture);
    }
    return RefCount;
}

int xD11DynamicRenderTarget::KillObject()
{
    XSAFE_RELEASEOBJECT(m_pBaseTexture);
    return xD11BaseRenderTarget::KillObject();
}


//用于Depth Buffer的 DepthTexture;
xD11DepthBuffer::xD11DepthBuffer(xD3D11RenderApi* pAPI) : xD11BaseRenderTarget(pAPI) 
{ 
	m_pDepthView = NULL ; 
	m_RefCount = 1;
}

xD11DepthBuffer::~xD11DepthBuffer() 
{
	destory() ;
}
bool xD11DepthBuffer::destory()
{
	XSAFE_RELEASE(m_pDepthView);  
	return true;
}

int xD11DepthBuffer::RefCount()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->RefCount();
	return xD11BaseRenderTarget::RefCount();
}

int xD11DepthBuffer::AddRef()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->AddRef();
	return xD11BaseRenderTarget::AddRef();
}

int xD11DepthBuffer::ReleaseObject()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->ReleaseObject();
	return xD11BaseRenderTarget::ReleaseObject();
}

int xD11DepthBuffer::KillObject()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->KillObject();
	return xD11BaseRenderTarget::KillObject();
}

bool xD11DepthBuffer::saveToFile(const wchar_t* fileName)
{
	ID3D11Resource* pSource = NULL;

	m_pDepthView->GetResource(&pSource);
	if(pSource == NULL)
		return false;
    if(saveResourceToFile(fileName , pSource) == true)
	{
		pSource->Release();
		return true;
	}
	pSource->Release();
	return false;
}

bool xD11DepthBuffer::create(ID3D11Resource* pTexture , xD11TexInfo& TexInfo , DXGI_SAMPLE_DESC& sampleDesc)
{
    destory();
	ID3D11Device* pDevice = m_pD11RenderApi->d11Device();
	//创建深度缓冲
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	memset(&descDSV , 0 , sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC) );
	descDSV.Format = TexInfo.m_RTViewFmt;
	if(sampleDesc.Count > 1)
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	else
	    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	HRESULT hr = pDevice->CreateDepthStencilView( pTexture, &descDSV, &m_pDepthView );
	return !FAILED(hr);
}


//=======可渲染的Texture=============================================
xD11RenderTexture::xD11RenderTexture(bool lockAble ,  bool canUseTexture , xD3D11RenderApi* pRenderApi , DXGI_SAMPLE_DESC SampleDesc )
:xD11UnkwonTexture(pRenderApi) , m_RenderTarget(pRenderApi)
{
	 m_bLockable = lockAble;
	 m_bCanUseTexute = canUseTexture;
	 m_pSysTexture   = NULL;
	 m_RenderTarget.setTexture(this);
	 m_SampleDesc = SampleDesc;
}
bool xD11RenderTexture::unload()
{
    m_RenderTarget.destory();
    XSAFE_RELEASE(m_pSysTexture);
    return xD11UnkwonTexture::unload();
}
xD11RenderTexture::~xD11RenderTexture()
{
	m_RenderTarget.destory();
    unload();
}

bool  xD11RenderTexture::grabRenderTagetData(int x , int y , int w , int h , void* pData)
{
	if(m_bLockable == false || m_pSysTexture == NULL)
		return false;

	ID3D11DeviceContext* pDeviceContext = m_pD11RenderApi->d11DeviceContext();
	//pDeviceContext->CopyResource(m_pSysTexture , m_pTexture);
	xTextureLockArea lockInfo;
	lock(eLock_Read , lockInfo);
	const char* pSrcLine = (const char*)lockInfo.m_pixels + x * m_TexInfo.m_nBytePerPixel;
	pSrcLine += lockInfo.m_picth * y;
	char*       pDstLine = (char*)pData;
	int dstPitch = m_TexInfo.m_nBytePerPixel * w;
	int line_len =  dstPitch;
	int max_len  =  lockInfo.m_picth - x * m_TexInfo.m_nBytePerPixel;
	if(dstPitch > max_len ) line_len = max_len;

	for(int _y = 0 ; _y < (int)h ; _y ++ )
	{
        memcpy(pDstLine , pSrcLine , line_len );
		pDstLine += dstPitch;
		pSrcLine += lockInfo.m_picth;
	}
	unlock(lockInfo);
	return true;
}

bool xD11RenderTexture::lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel, int arraySlice)
{
	if(m_bLockable == false || m_pSysTexture == NULL)
		return false;

	if(lockPolicy != eLock_Read )
		return false;

	ID3D11DeviceContext* pDeviceContext = m_pD11RenderApi->d11DeviceContext();
	if(lockPolicy != eLock_WriteDiscard)
	{
		pDeviceContext->CopyResource(m_pSysTexture , m_pTexture);
	}
	D3D11_MAPPED_SUBRESOURCE mappedTex;
	mappedTex.pData = NULL;
	mappedTex.RowPitch = 0;
	D3D11_MAP  mapType = D3D11_MAP_READ;

	UINT lockResource = D3D11CalcSubresource((UINT)mipmapLevel , (UINT)arraySlice, (UINT)m_TexInfo.m_MipmapLevel);
	lockInfo.m_lockResource = (long)lockResource;

	pDeviceContext->Map(m_pSysTexture , lockResource , (D3D11_MAP)lockPolicy , 0 , &mappedTex);
	if(mappedTex.pData == NULL)
		return false;
	lockInfo.m_width       = (int)m_TexInfo.m_TexWidth;
	lockInfo.m_height      = (int)m_TexInfo.m_TexHeight;
	lockInfo.m_depth       = 1;
	lockInfo.m_picth       = (int)mappedTex.RowPitch ;
	lockInfo.m_slice_pitch = (int)mappedTex.DepthPitch;
	lockInfo.m_pixels      = (char*)mappedTex.pData;
    return true;
}

bool xD11RenderTexture::unlock(xTextureLockArea& lockInfo)
{
	if(m_bLockable == false || m_pSysTexture == NULL)
		return false;

	ID3D11DeviceContext* pDeviceContext = m_pD11RenderApi->d11DeviceContext();
    pDeviceContext->Unmap(m_pSysTexture , lockInfo.m_lockResource );
	pDeviceContext->CopyResource(m_pTexture , m_pSysTexture );
	return true;
}

bool xD11RenderTexture::__createSysTexture(int w , int h , DXGI_FORMAT fmt)
{
	XD3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width            = (UINT)w;
	desc.Height           = (UINT)h;
	desc.MipLevels        = (UINT)1;
	desc.ArraySize        = (UINT)1;
	desc.Format           = fmt ; //DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc       = m_SampleDesc;
	desc.BindFlags        =  0; 
	desc.Usage            = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags   = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	ID3D11Texture2D* pTexture = NULL;
	m_pD11RenderApi->d11Device()->CreateTexture2D( &desc, NULL, &m_pSysTexture );
    return m_pSysTexture != NULL;
}

bool xD11RenderTexture::create(int w , int h , ePIXEL_FORMAT fmt, int mipMapLevels , int arraySize )
{
	unload();
	xD11GIFormatInfo* pFmtInfo = xD11ConstLexer::singleton()->GetPixelFormat(fmt);
    m_TexInfo.m_RTViewFmt = pFmtInfo->m_dxfmt;
    m_TexInfo.m_ResFmt    = pFmtInfo->m_dxfmt;
    m_TexInfo.m_ShaderViewFmt= pFmtInfo->m_dxfmt;

	XD3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width            = (UINT)w;
	desc.Height           = (UINT)h;
	desc.MipLevels        = (UINT)mipMapLevels;
	desc.ArraySize        = (UINT)arraySize;
	desc.Format           = m_TexInfo.m_ResFmt ; //DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc       = m_SampleDesc;
	desc.BindFlags        =  D3D11_BIND_RENDER_TARGET; 
	if(m_bCanUseTexute)
		desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	desc.Usage            = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags   = 0;
	m_TexInfo.m_Usage     = desc.Usage;

	ID3D11Texture2D* pTexture = NULL;
	m_pD11RenderApi->d11Device()->CreateTexture2D( &desc, NULL, &pTexture );
	if(pTexture == NULL)
		return false;
	m_pTextureView = NULL;
	_load(pTexture , m_bCanUseTexute );
	m_TexInfo.m_xfmt = fmt;

	if(m_bLockable)
	{
		return __createSysTexture(w , h , desc.Format);
	}
	return true;
}

bool xD11RenderTexture::create(int w , int h , int depth , ePIXEL_FORMAT fmt, int mipMapLevels , int arraySize)
{
	if(depth == 1)
		return create(w , h , fmt , mipMapLevels , arraySize);

	unload();
	xD11GIFormatInfo* pFmtInfo = xD11ConstLexer::singleton()->GetPixelFormat(fmt);
    m_TexInfo.m_RTViewFmt = pFmtInfo->m_dxfmt;
    m_TexInfo.m_ResFmt    = pFmtInfo->m_dxfmt;
    m_TexInfo.m_ShaderViewFmt= pFmtInfo->m_dxfmt;
	D3D11_TEXTURE3D_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width            = (UINT)w;
	desc.Height           = (UINT)h;
	desc.MipLevels        = (UINT)mipMapLevels;
	desc.Depth            = (UINT)depth;
	desc.Format           = m_TexInfo.m_ResFmt ; //DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.BindFlags        = D3D11_BIND_RENDER_TARGET; 
	if(m_bCanUseTexute)
		desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	desc.Usage            = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags   = 0;
	m_TexInfo.m_Usage     = desc.Usage;

	ID3D11Texture3D* pTexture = NULL;
	m_pD11RenderApi->d11Device()->CreateTexture3D( &desc, NULL, &pTexture );
	if(pTexture == NULL)
		return false;
	m_pTextureView = NULL;
	_load(pTexture , m_bCanUseTexute);
	m_TexInfo.m_xfmt = fmt;
	if(m_bLockable)
	{
		return __createSysTexture(w , h , desc.Format);
	}
	return true;
}
bool xD11RenderTexture::isSameInstance(IRenderTarget* pRenderTarget)
{
    if(&m_RenderTarget == pRenderTarget)
        return true;
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return false;
}

IRenderTarget* xD11RenderTexture::toRenderTarget(size_t iSlice , size_t iMipMapLevel )
{
    if(iSlice == 0 && iMipMapLevel == 0)
	{
		m_RenderTarget.create(m_pTexture,m_TexInfo , m_SampleDesc ,  0 , 0 );
        AddRef();
		return &m_RenderTarget;
	}
	else
	{
		xD11DynamicRenderTarget* pRenderTarget = new xD11DynamicRenderTarget(m_pD11RenderApi);
		pRenderTarget->setTexture(this);
		pRenderTarget->create(m_pTexture,m_TexInfo , m_SampleDesc , (int)iSlice , (int)iMipMapLevel );
        AddRef();
		return pRenderTarget;
	}
    return NULL;
}

//===========可渲染的深度纹理
xD11DepthTexture::xD11DepthTexture(bool lockAble ,  bool canUseTexture , xD3D11RenderApi* pRenderApi ,  DXGI_SAMPLE_DESC SampleDes) 
:xD11UnkwonTexture(pRenderApi) , m_RenderTarget(pRenderApi)
{
	m_bLockable     = lockAble;
	m_bCanUseTexute = canUseTexture;
	m_RenderTarget.setTexture(this);
	m_SampleDesc = SampleDes;
}

bool xD11DepthTexture::unload()
{
    m_RenderTarget.destory();
    return xD11UnkwonTexture::unload();
}

xD11DepthTexture::~xD11DepthTexture()
{
    unload();
	m_RenderTarget.destory();
    XSAFE_RELEASE(m_pTexture);
}

bool xD11DepthTexture::create(int w , int h , ePIXEL_FORMAT fmt, int mipMapLevels  , int arraySize)
{
	unload();
	xD11GIFormatInfo* pFmtInfo = xD11ConstLexer::singleton()->GetPixelFormat(fmt);
	XD3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc , sizeof(desc) );
    m_TexInfo.m_RTViewFmt = pFmtInfo->m_dxfmt;
    m_TexInfo.m_ResFmt    = pFmtInfo->m_dxfmt;
    m_TexInfo.m_ShaderViewFmt= pFmtInfo->m_dxfmt;
    if(m_bCanUseTexute == true)
    {
        if(m_TexInfo.m_ResFmt == DXGI_FORMAT_D32_FLOAT ) 
        { 
            m_TexInfo.m_ResFmt     = DXGI_FORMAT_R32_TYPELESS; 
            m_TexInfo.m_ShaderViewFmt = DXGI_FORMAT_R32_FLOAT; 
        } 
        else if(m_TexInfo.m_ResFmt == DXGI_FORMAT_D16_UNORM ) 
        { 
            m_TexInfo.m_ResFmt     = DXGI_FORMAT_R16_TYPELESS; 
            m_TexInfo.m_ShaderViewFmt = DXGI_FORMAT_R16_UNORM; 
        } 
        else
        {
            const wchar_t* _name  = xPixelFormatAssit::singleton()->GetPixelFormatName(fmt);
            XEVOL_LOG(eXL_ERROR_FALT , L"Depthbuffer format not support bind as texture [fmt=%s]\n" , _name );
        }

    }
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width            = (UINT)w;
	desc.Height           = (UINT)h;
	desc.MipLevels        = (UINT)mipMapLevels;
	desc.ArraySize        = (UINT)arraySize;
	desc.Format           = m_TexInfo.m_ResFmt; //DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc       = m_SampleDesc;
	desc.BindFlags        = D3D11_BIND_DEPTH_STENCIL;

	if(m_bCanUseTexute)
		desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

	desc.Usage            = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags   = 0;
	m_TexInfo.m_Usage     = desc.Usage;

	ID3D11Texture2D* pTexture = NULL;
	m_pD11RenderApi->d11Device()->CreateTexture2D( &desc, NULL, &pTexture );
	if(pTexture == NULL)
		return false;

	m_pTexture = pTexture;

    XSAFE_RELEASE(m_pTextureView);
	D3D11_RESOURCE_DIMENSION type;
	m_pTexture->GetType( &type );
	if( type ==  D3D11_RESOURCE_DIMENSION_TEXTURE2D)
	{
		ZeroMemory(&desc , sizeof(desc) );
		ID3D11Texture2D *pTexture2D = (ID3D11Texture2D*)m_pTexture;
		pTexture2D->GetDesc( &desc );

		m_pTextureView = NULL;


        pTexture2D->GetDesc( &desc );
        if(m_bCanUseTexute)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
            srvDesc.Format = m_TexInfo.m_ShaderViewFmt;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MostDetailedMip = 0;
            srvDesc.Texture2D.MipLevels       = desc.MipLevels;
            ID3D11ShaderResourceView *pSRView = NULL;
            m_pD11RenderApi->d11Device()->CreateShaderResourceView( m_pTexture, &srvDesc, &pSRView );
            if(pSRView != NULL)
            {
                m_pTextureView = pSRView;
            }
        }      

        //填充
		m_TexInfo.m_ArraySize   = desc.ArraySize;
		m_TexInfo.m_TexWidth    = desc.Width;
		m_TexInfo.m_TexHeight   = desc.Height;
		m_TexInfo.m_TexDepth    = 1;
		m_TexInfo.m_MipmapLevel = desc.MipLevels;
		m_TexInfo.m_Type        = RESOURCE_DEPTHBUFFER;
		//===================================
		xD11GIFormatInfo* pFormat = xD11ConstLexer::singleton()->GetPixelFormat( m_TexInfo.m_ShaderViewFmt );
		m_TexInfo.m_nBytePerPixel = pFormat->m_compont * pFormat->m_bytePerComponent;
		m_TexInfo.m_xfmt = pFormat->m_fmt;
		m_TexInfo.m_Pitch = xFloorToPower2( (unsigned int)(m_TexInfo.m_TexWidth  * pFormat->m_compont * pFormat->m_bytePerComponent) );
		m_TexInfo.m_MemSize = int( m_TexInfo.m_Pitch * m_TexInfo.m_ArraySize * m_TexInfo.m_TexDepth * m_TexInfo.m_TexHeight );
		return true;
	}
	return false;
}
bool xD11DepthTexture::isSameInstance(IRenderTarget* pRenderTarget)
{
    if(&m_RenderTarget == pRenderTarget)
        return true;
    return false;
}
IRenderTarget* xD11DepthTexture::toRenderTarget(size_t iSlice , size_t iMipMapLevel)
{
	if(iSlice == 0 && iMipMapLevel == 0)
	{
		m_RenderTarget.create(m_pTexture,m_TexInfo , m_SampleDesc);
        AddRef();
		return &m_RenderTarget;
	}
	return NULL;
}

END_NAMESPACE_XEVOL3D