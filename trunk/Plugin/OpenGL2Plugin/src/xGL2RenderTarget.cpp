#include "xGL2LockableTexture.h"
#include "xGL2ConstLexer.h"
#include "xOpenGL2API.h"
#include <BaseLib/xI18N.h>
#include <BaseLib/xBaseMath.h>
#include <OperationSys/xOperationSys.h>
#include "Image/PixelFormatAssit.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xGL2RenderTarget   , xGL2BaseRenderTarget);
IMPL_BASE_OBJECT_CLASSID(xGL2RenderTexture  , xGL2UnkwonTexture);

IMPL_BASE_OBJECT_CLASSID(xGL2DepthBuffer   , xGL2BaseRenderTarget);
IMPL_BASE_OBJECT_CLASSID(xGL2DepthTexture  , xGL2UnkwonTexture);
xGL2RenderTarget::xGL2RenderTarget(xGL2RenderApi* pAPI):xGL2BaseRenderTarget(pAPI)
{
	m_RefCount = 1;
	m_pRenderTargetView = NULL;
	m_arraySlice = 0;
	m_mipLevel   = 0;
}
int xGL2RenderTarget::RefCount()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->RefCount();
	return xGL2BaseRenderTarget::RefCount();
}

int xGL2RenderTarget::AddRef()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->AddRef();
	return xGL2BaseRenderTarget::AddRef();
}

int xGL2RenderTarget::ReleaseObject()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->ReleaseObject();
	return xGL2BaseRenderTarget::ReleaseObject();
}

int xGL2RenderTarget::KillObject()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->KillObject();
	return xGL2BaseRenderTarget::KillObject();
}

xGL2RenderTarget::~xGL2RenderTarget()
{
	destory();   
}

bool xGL2RenderTarget::destory()
{
	XSAFE_RELEASE(m_pRenderTargetView);  
	return true;
}

bool xGL2RenderTarget::create(ID3D10Resource* pTexture , xGL2TexInfo& TexInfo, int iSlice , int mipLevel )
{
	if(m_pRenderTargetView && m_arraySlice == iSlice && m_mipLevel == mipLevel )
		return true;
	destory();
	m_arraySlice = iSlice;
	m_mipLevel   = mipLevel;

	XSAFE_RELEASE(m_pRenderTargetView);   
	m_pRenderTargetView = NULL;

	GL2_RENDER_TARGET_VIEW_DESC rtDesc;
	rtDesc.Format = TexInfo.m_RTViewFmt;

	if(TexInfo.m_ArraySize > 1)
	{
		rtDesc.Texture2DArray.MipSlice = 0;
		rtDesc.Texture2DArray.FirstArraySlice = 0;
		rtDesc.Texture2DArray.ArraySize       = (UINT)TexInfo.m_ArraySize;
		rtDesc.ViewDimension = GL2_RTV_DIMENSION_TEXTURE2DARRAY;
	}
	else if(TexInfo.m_ArraySize == 1)
	{
		rtDesc.ViewDimension = GL2_RTV_DIMENSION_TEXTURE2D;
		rtDesc.Texture2D.MipSlice = 0;
	}

	m_pD10Api->d10Device()->CreateRenderTargetView( pTexture, &rtDesc, &m_pRenderTargetView );
	return m_pRenderTargetView != NULL;
}

int xGL2DynamicRenderTarget::ReleaseObject()
{
    int RefCount = xGL2BaseRenderTarget::ReleaseObject();
    if( RefCount == 0  )
    {
        XSAFE_RELEASEOBJECT(m_pBaseTexture);
    }
    return RefCount;
}

int xGL2DynamicRenderTarget::KillObject()
{
    XSAFE_RELEASEOBJECT(m_pBaseTexture);
    return xGL2BaseRenderTarget::KillObject();
}


//用于Depth Buffer的 DepthTexture;
xGL2DepthBuffer::xGL2DepthBuffer(xGL2RenderApi* pAPI) : xGL2BaseRenderTarget(pAPI) 
{ 
	m_pDepthView = NULL ; 
	m_RefCount = 1;
}

xGL2DepthBuffer::~xGL2DepthBuffer() 
{
	destory() ;
}
bool xGL2DepthBuffer::destory()
{
	XSAFE_RELEASE(m_pDepthView);  
	return true;
}

int xGL2DepthBuffer::RefCount()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->RefCount();
	return xGL2BaseRenderTarget::RefCount();
}

int xGL2DepthBuffer::AddRef()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->AddRef();
	return xGL2BaseRenderTarget::AddRef();
}

int xGL2DepthBuffer::ReleaseObject()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->ReleaseObject();
	return xGL2BaseRenderTarget::ReleaseObject();
}

int xGL2DepthBuffer::KillObject()
{
	if(m_pBaseTexture)
		return m_pBaseTexture->KillObject();
	return xGL2BaseRenderTarget::KillObject();
}

bool xGL2DepthBuffer::create(ID3D10Resource* pTexture , xGL2TexInfo& TexInfo)
{
    destory();
	ID3D10Device* pDevice = m_pD10Api->d10Device();
	//创建深度缓冲
	GL2_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = TexInfo.m_RTViewFmt;
	descDSV.ViewDimension = GL2_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	HRESULT hr = pDevice->CreateDepthStencilView( pTexture, &descDSV, &m_pDepthView );
	return !FAILED(hr);
}


//=======可渲染的Texture=============================================
xGL2RenderTexture::xGL2RenderTexture(bool lockAble ,  bool canUseTexture , xGL2RenderApi* pRenderApi)
:xGL2UnkwonTexture(pRenderApi) , m_RenderTarget(pRenderApi)
{
	 m_bLockable = lockAble;
	 m_bCanUseTexute = canUseTexture;
	 m_pSysTexture   = NULL;
	 m_RenderTarget.setTexture(this);
}
bool xGL2RenderTexture::unload()
{
    m_RenderTarget.destory();
    XSAFE_RELEASE(m_pSysTexture);
    return xGL2UnkwonTexture::unload();
}
xGL2RenderTexture::~xGL2RenderTexture()
{
	m_RenderTarget.destory();
    unload();
}

bool  xGL2RenderTexture::grabRenderTagetData(int x , int y , int w , int h , void* pData)
{
	if(m_bLockable == false || m_pSysTexture == NULL)
		return false;

	ID3D10Device* pDevice = m_pD10Api->d10Device();
	pDevice->CopyResource(m_pSysTexture , m_pTexture);
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

bool xGL2RenderTexture::lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel, int arraySlice)
{
	if(m_bLockable == false || m_pSysTexture == NULL)
		return false;

	if(lockPolicy != eLock_Read )
		return false;

	GL2_MAPPED_TEXTURE2D mappedTex;
	mappedTex.pData = NULL;
	mappedTex.RowPitch = 0;
	GL2_MAP  mapType = GL2_MAP_READ;

	UINT lockResource = GL2CalcSubresource((UINT)mipmapLevel , (UINT)arraySlice, (UINT)m_TexInfo.m_MipmapLevel);
	lockInfo.m_lockResource = (long)lockResource;
	m_pSysTexture->Map( lockResource  , mapType, 0, &mappedTex );
	if(mappedTex.pData == NULL)
		return false;
	lockInfo.m_width       = (int)m_TexInfo.m_TexWidth;
	lockInfo.m_height      = (int)m_TexInfo.m_TexHeight;
	lockInfo.m_depth       = 1;
	lockInfo.m_picth       = (int)mappedTex.RowPitch ;
	lockInfo.m_slice_pitch = (int)(lockInfo.m_picth * m_TexInfo.m_TexHeight);
	lockInfo.m_pixels      = (char*)mappedTex.pData;
    return true;
}

bool xGL2RenderTexture::unlock(xTextureLockArea& lockInfo)
{
	if(m_bLockable == false || m_pSysTexture == NULL)
		return false;

	m_pSysTexture->Unmap( lockInfo.m_lockResource);
	return true;
}

bool xGL2RenderTexture::__createSysTexture(int w , int h , DXGI_FORMAT fmt)
{
	GL2_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width            = (UINT)w;
	desc.Height           = (UINT)h;
	desc.MipLevels        = (UINT)1;
	desc.ArraySize        = (UINT)1;
	desc.Format           = fmt ; //DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.BindFlags        =  0; 
	desc.Usage            = GL2_USAGE_STAGING;
	desc.CPUAccessFlags   = GL2_CPU_ACCESS_READ;
	ID3D10Texture2D* pTexture = NULL;
	m_pD10Api->d10Device()->CreateTexture2D( &desc, NULL, &m_pSysTexture );
    return m_pSysTexture != NULL;
}

bool xGL2RenderTexture::create(int w , int h , ePIXEL_FORMAT fmt, int mipMapLevels , int arraySize )
{
	unload();
	xGL2GIFormatInfo* pFmtInfo = xGL2ConstLexer::singleton()->GetPixelFormat(fmt);
    m_TexInfo.m_RTViewFmt = pFmtInfo->m_dxfmt;
    m_TexInfo.m_ResFmt    = pFmtInfo->m_dxfmt;
    m_TexInfo.m_ShaderViewFmt= pFmtInfo->m_dxfmt;

	GL2_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width            = (UINT)w;
	desc.Height           = (UINT)h;
	desc.MipLevels        = (UINT)mipMapLevels;
	desc.ArraySize        = (UINT)arraySize;
	desc.Format           = m_TexInfo.m_ResFmt ; //DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.BindFlags        =  GL2_BIND_RENDER_TARGET; 
	if(m_bCanUseTexute)
		desc.BindFlags |= GL2_BIND_SHADER_RESOURCE;
	desc.Usage            = GL2_USAGE_DEFAULT;
	desc.CPUAccessFlags   = 0;

	ID3D10Texture2D* pTexture = NULL;
	m_pD10Api->d10Device()->CreateTexture2D( &desc, NULL, &pTexture );
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

bool xGL2RenderTexture::create(int w , int h , int depth , ePIXEL_FORMAT fmt, int mipMapLevels , int arraySize)
{
	if(depth == 1)
		return create(w , h , fmt , mipMapLevels , arraySize);

	unload();
	xGL2GIFormatInfo* pFmtInfo = xGL2ConstLexer::singleton()->GetPixelFormat(fmt);
    m_TexInfo.m_RTViewFmt = pFmtInfo->m_dxfmt;
    m_TexInfo.m_ResFmt    = pFmtInfo->m_dxfmt;
    m_TexInfo.m_ShaderViewFmt= pFmtInfo->m_dxfmt;
	GL2_TEXTURE3D_DESC desc;
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width            = (UINT)w;
	desc.Height           = (UINT)h;
	desc.MipLevels        = (UINT)mipMapLevels;
	desc.Depth            = (UINT)depth;
	desc.Format           = m_TexInfo.m_ResFmt ; //DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.BindFlags        = GL2_BIND_RENDER_TARGET; 
	if(m_bCanUseTexute)
		desc.BindFlags |= GL2_BIND_SHADER_RESOURCE;
	desc.Usage            = GL2_USAGE_DEFAULT;
	desc.CPUAccessFlags   = 0;

	ID3D10Texture3D* pTexture = NULL;
	m_pD10Api->d10Device()->CreateTexture3D( &desc, NULL, &pTexture );
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
bool xGL2RenderTexture::isSameInstance(IRenderTarget* pRenderTarget)
{
    if(&m_RenderTarget == pRenderTarget)
        return true;
    XEVOL_WARNNING_NOT_IMPLEMENT;
    return false;
}

IRenderTarget* xGL2RenderTexture::toRenderTarget(size_t iSlice , size_t iMipMapLevel )
{
    if(iSlice == 0 && iMipMapLevel == 0)
	{
		m_RenderTarget.create(m_pTexture,m_TexInfo , 0 , 0 );
        AddRef();
		return &m_RenderTarget;
	}
	else
	{
		xGL2DynamicRenderTarget* pRenderTarget = new xGL2DynamicRenderTarget(m_pD10Api);
		pRenderTarget->setTexture(this);
		pRenderTarget->create(m_pTexture,m_TexInfo , (int)iSlice , (int)iMipMapLevel );
        AddRef();
		return pRenderTarget;
	}
    return NULL;
}

//===========可渲染的深度纹理
xGL2DepthTexture::xGL2DepthTexture(bool lockAble ,  bool canUseTexture , xGL2RenderApi* pRenderApi) 
:xGL2UnkwonTexture(pRenderApi) , m_RenderTarget(pRenderApi)
{
	m_bLockable     = lockAble;
	m_bCanUseTexute = canUseTexture;
	m_RenderTarget.setTexture(this);
}

bool xGL2DepthTexture::unload()
{
    m_RenderTarget.destory();
    return xGL2UnkwonTexture::unload();
}

xGL2DepthTexture::~xGL2DepthTexture()
{
    unload();
	m_RenderTarget.destory();
    XSAFE_RELEASE(m_pTexture);
}

bool xGL2DepthTexture::create(int w , int h , ePIXEL_FORMAT fmt, int mipMapLevels  , int arraySize)
{
	unload();
	xGL2GIFormatInfo* pFmtInfo = xGL2ConstLexer::singleton()->GetPixelFormat(fmt);
	GL2_TEXTURE2D_DESC desc;
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
            XEVOL_LOG(eXR_LOG_LEVEL::eXL_ERROR_FALT , L"Depthbuffer format not support bind as texture [fmt=%s]\n" , _name );
        }

    }
	ZeroMemory( &desc, sizeof(desc) );
	desc.Width            = (UINT)w;
	desc.Height           = (UINT)h;
	desc.MipLevels        = (UINT)mipMapLevels;
	desc.ArraySize        = (UINT)arraySize;
	desc.Format           = m_TexInfo.m_ResFmt; //DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BindFlags        = GL2_BIND_DEPTH_STENCIL;

	if(m_bCanUseTexute)
		desc.BindFlags |= GL2_BIND_SHADER_RESOURCE;

	desc.Usage            = GL2_USAGE_DEFAULT;
	desc.CPUAccessFlags   = 0;

	ID3D10Texture2D* pTexture = NULL;
	m_pD10Api->d10Device()->CreateTexture2D( &desc, NULL, &pTexture );
	if(pTexture == NULL)
		return false;

	m_pTexture = pTexture;

    XSAFE_RELEASE(m_pTextureView);
	GL2_RESOURCE_DIMENSION type;
	m_pTexture->GetType( &type );
	if( type ==  GL2_RESOURCE_DIMENSION_TEXTURE2D)
	{
		GL2_TEXTURE2D_DESC desc;
		ID3D10Texture2D *pTexture2D = (ID3D10Texture2D*)m_pTexture;
		pTexture2D->GetDesc( &desc );

		m_pTextureView = NULL;


        pTexture2D->GetDesc( &desc );
        if(m_bCanUseTexute)
        {
            GL2_SHADER_RESOURCE_VIEW_DESC srvDesc;
            srvDesc.Format = m_TexInfo.m_ShaderViewFmt;
            srvDesc.ViewDimension = GL2_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MostDetailedMip = 0;
            srvDesc.Texture2D.MipLevels       = desc.MipLevels;
            ID3D10ShaderResourceView *pSRView = NULL;
            m_pD10Api->d10Device()->CreateShaderResourceView( m_pTexture, &srvDesc, &pSRView );
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
		xGL2GIFormatInfo* pFormat = xGL2ConstLexer::singleton()->GetPixelFormat( m_TexInfo.m_ShaderViewFmt );
		m_TexInfo.m_nBytePerPixel = pFormat->m_compont * pFormat->m_byte;
		m_TexInfo.m_xfmt = pFormat->m_fmt;
		m_TexInfo.m_Pitch = xFloorToPower2( (unsigned int)(m_TexInfo.m_TexWidth  * pFormat->m_compont * pFormat->m_byte) );
		m_TexInfo.m_MemSize = int( m_TexInfo.m_Pitch * m_TexInfo.m_ArraySize * m_TexInfo.m_TexDepth * m_TexInfo.m_TexHeight );
		return true;
	}
	return false;
}
bool xGL2DepthTexture::isSameInstance(IRenderTarget* pRenderTarget)
{
    if(&m_RenderTarget == pRenderTarget)
        return true;
    return false;
}
IRenderTarget* xGL2DepthTexture::toRenderTarget(size_t iSlice , size_t iMipMapLevel)
{
	if(iSlice == 0 && iMipMapLevel == 0)
	{
		m_RenderTarget.create(m_pTexture,m_TexInfo);
        AddRef();
		return &m_RenderTarget;
	}
	return NULL;
}

END_NAMESPACE_XEVOL3D