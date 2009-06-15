#include "xRenderApiImp.h"
#include  "xBaseShader.h"
#include  "xShaderName.h"
#include  "xShaderManager.h"
BEGIN_NAMESPACE_XEVOL3D


//===========================================================
class xFontRenderDevice : public IFontRenderDevice
{
	IRenderApi*            m_pRenderApi;
	eFontFilter            m_filter;
	IBlenderState*         m_BlendState;
	ISamplerState*         m_pPointSampler;
	ISamplerState*         m_pLinearSampler;
	IGpuProgram*           m_pDefFontShader;
	IGpuProgram*           m_pUsrFontShader;
	IDepthStencilState*    m_pStencilState;
	I2DRectObject*         m_2DObject;
public:
	xFontRenderDevice(IRenderApi* pRenderApi);
	~xFontRenderDevice();
    IRenderApi*             renderApi(){return m_pRenderApi ; }
public:
	virtual I2DRectObject*  get2DObject();
	virtual bool            setUVLayer(int nUVLayer);
	virtual bool            setShaderProgram(IGpuProgram* pProgram);
	virtual bool            init();
	virtual bool            setFontFilter(eFontFilter filter);
	virtual bool            beginFontRender();
	virtual bool            endFontRender();
	virtual bool            drawRectf(IBaseTexture* pTexture, float vDestRect[4] , xColor_4f& color);
	virtual bool            drawRectf(IBaseTexture* pTexture, float vDestRect[4] ,float vSrcRect[4]  , xColor_4f& color);
	virtual IBaseTexture*   createLockableTexture(int w , int h , ePIXEL_FORMAT fmt , bool bReadable , int nMipMap = 1, int nArraySize = 1 );
	virtual bool            isTextureSupport(ePIXEL_FORMAT fmt , bool lockable = true);
};

I2DRectObject* xFontRenderDevice::get2DObject()
{
	return m_2DObject;
}

bool xFontRenderDevice::setUVLayer(int nUVLayer)
{
	m_2DObject->setUVChanel(nUVLayer);
	return true;
}

bool xFontRenderDevice::setShaderProgram(IGpuProgram* pProgram)
{
	m_pUsrFontShader = pProgram;
	return true;
}

bool xFontRenderDevice::init()
{
	m_2DObject = m_pRenderApi->create2DRectObject();
	m_2DObject->setUVChanel( 1 );
	m_filter = eFontFilter_Point;

	if(m_pDefFontShader == NULL)
	{
		HGpuProgram hGpuProgram = m_pRenderApi->gpuProgramManager()->load(L"font.vertex", L"font.pixel" ,NULL);
		m_pDefFontShader = hGpuProgram.getResource();
	}

	if(m_pStencilState == NULL)	 { m_pStencilState = m_pRenderApi->createDepthStencilState(L"Overlay");}
	if(m_pPointSampler == NULL)	 { m_pPointSampler = m_pRenderApi->createSamplerState(L"FontPoint");	}
	if(m_pLinearSampler == NULL) { m_pLinearSampler = m_pRenderApi->createSamplerState(L"FontLinear");	}
	if(m_BlendState == NULL)     { m_BlendState = m_pRenderApi->createBlendState(L"Font.Blend"); }
	return true;
}

bool xFontRenderDevice::setFontFilter(eFontFilter filter)
{
	m_filter = filter;
	if(m_filter == eFontFilter_Point)
	{
		return m_pRenderApi->setSamplerState(eShader_PixelShader , 0 , m_pPointSampler );
	}
	else if(m_filter == eFontFilter_Linear)
	{
		return m_pRenderApi->setSamplerState(eShader_PixelShader , 0 , m_pLinearSampler );
	}
	return false;
}

xFontRenderDevice::xFontRenderDevice(IRenderApi* pRenderApi)
{
	m_filter         = eFontFilter_Point;
	m_pStencilState  = NULL;
	m_pDefFontShader = NULL;
	m_pRenderApi     = pRenderApi;
	m_pPointSampler  = NULL;
	m_pLinearSampler = NULL;
	m_BlendState     = NULL;
	m_pUsrFontShader = NULL;
}

xFontRenderDevice::~xFontRenderDevice()
{

}

bool xFontRenderDevice::beginFontRender()
{
	if(m_pDefFontShader == NULL)
		return false;
	m_pRenderApi->setDepthStencilState(m_pStencilState);
	m_pRenderApi->setBlendState(m_BlendState);
	if(m_pUsrFontShader) 
	{
		m_pRenderApi->pushGpuProgram(m_pUsrFontShader);
	}
	else 
	{
		m_pRenderApi->pushGpuProgram(m_pDefFontShader);
	}
	return true;
}

bool xFontRenderDevice::endFontRender()
{
	m_pRenderApi->popGpuProgram();
	return true;
}

bool xFontRenderDevice::drawRectf(IBaseTexture* pTexture, float vDestRect[4] , xColor_4f& color)
{
	if(m_pRenderApi)
	{
		xvec4& dstRect =  *((xvec4*)vDestRect);
		m_2DObject->setColor(color);
		m_2DObject->setTexture(0,pTexture);
		m_2DObject->setRect(dstRect);
		return m_pRenderApi->draw2DRect(m_2DObject);
	}
	return false;
}

bool xFontRenderDevice::drawRectf(IBaseTexture* pTexture, float vDestRect[4] ,float vSrcRect[4]  , xColor_4f& color)
{
	if(m_pRenderApi)
	{
		xvec4& dstRect =  *((xvec4*)vDestRect);
		xvec4& srcRect =  *((xvec4*)vSrcRect);
		m_2DObject->setColor(color);
		m_2DObject->setTexture(0,  pTexture , srcRect);
		m_2DObject->setRect(dstRect);
		return m_pRenderApi->draw2DRect(m_2DObject);
	}
	return false;
}

IBaseTexture* xFontRenderDevice::createLockableTexture(int w , int h , ePIXEL_FORMAT fmt , bool bReadable, int nMipMap, int nArraySize)
{
	if(m_pRenderApi)
	{
		return m_pRenderApi->createLockableTexture(w , h , fmt , bReadable,  nMipMap , nArraySize);
	}
	return false; 
}

bool xFontRenderDevice::isTextureSupport(ePIXEL_FORMAT fmt , bool lockable)
{
	//if(fmt == PIXELFORMAT_ALPHA8)
	//	return false;
	if(m_pRenderApi)
	{
		return m_pRenderApi->isTextureSupport(fmt , lockable);
	}
	return false; 
}

//==========================================


xRenderApiImp::xRenderApiImp()
:m_FontManager(L"XFontManager" , 1)
{
	m_FontRenderDevice = new xFontRenderDevice(this);
	m_FontManager.setFontRenderDevice( m_FontRenderDevice );

}

xRenderApiImp::~xRenderApiImp()
{
	XSAFE_DELETE(m_FontRenderDevice);
}
void  xRenderApiImp::addRenderObject(IRenderObject* pObject)
{
    m_RenderObjectSet.insert(pObject);
}

void  xRenderApiImp::removeRenderObject(IRenderObject* pObject)
{
    m_RenderObjectSet.erase(pObject);
}

void  xRenderApiImp::dumpRenderObject()
{
    OBJECTSETS::iterator pos = m_RenderObjectSet.begin();
    XEVOL_LOG(eXL_DEBUG_HIGH , "Begin RendererObject dumping......\n");
    for(; pos != m_RenderObjectSet.end() ; pos ++)
    {
        IRenderObject* pObject = *pos;
        int refCount = pObject->RefCount();
        const xObjectClassID& clsID = pObject->classID();
        XEVOL_LOG(eXL_DEBUG_HIGH , L"   ->RenderApi Object not released: refcount=%d , name=%s POINTER=0x%XH\n" , pObject->RefCount() , clsID.name() , (int)pObject );
    }
    XEVOL_LOG(eXL_DEBUG_HIGH , "End RendererObject dumping......\n");   
    return ;
}

void  xRenderApiImp::KillRenderObject()
{
    //while(m_RenderObjectSet.size())
    //{
    //    OBJECTSETS::iterator pos = m_RenderObjectSet.begin();
    //    IRenderObject* pObject = *pos;
    //    int refCount = pObject->RefCount();
    //    const xObjectClassID& clsID = pObject->classID();
    //    XEVOL_LOG(eXL_DEBUG_HIGH , L"   ->RenderApi Object Released: refcount=%d , name=%s POINTER=0x%XH\n" , pObject->RefCount() , clsID.name() , (int)pObject );
    //    pObject->ReleaseObject();
    //    continue;
    //}
    return ;
}

xFontManager* xRenderApiImp::fontManager()
{
	return &m_FontManager;
}

IFontRenderDevice* xRenderApiImp::fontRenderDevice()
{
	return m_FontRenderDevice;
}

HFontRender xRenderApiImp::findFont(const wchar_t* family , const wchar_t* fontName)
{
	return m_FontManager.findFont(family , fontName);
}

HFontRender xRenderApiImp::findFont(const wchar_t* fontName)
{
	return m_FontManager.find(fontName);
}

//=======================================
ITextureLoader* xTextureLdrMgrProxy::findTextureLoader(const wchar_t* texExt)
{
	TextureLoaderMap::iterator pos = m_TextureLoaderMap.find(texExt);
	if(pos != m_TextureLoaderMap.end() )
		return pos->second;
	return NULL;
}

void xTextureLdrMgrProxy::addTextureLoader(ITextureLoader* pTextureLoader)
{
	pTextureLoader->AddRef();
    m_TextureLoaders.push_back(pTextureLoader);
	__updateTextureLoaders();
}

void xTextureLdrMgrProxy::removeTextureLoader(ITextureLoader* pTextureLoader)
{
	vTextureLoaders::iterator pos = m_TextureLoaders.begin();
	for(; pos != m_TextureLoaders.end() ; pos ++)
	{
		if(*pos == pTextureLoader)
		{
			pTextureLoader->ReleaseObject();
			m_TextureLoaders.erase(pos);
			__updateTextureLoaders();
			return ;
		}
	}
}

void xTextureLdrMgrProxy::__updateTextureLoaders()
{
	for(size_t i = 0 ; i < m_TextureLoaders.size() ; i ++)
	{
         ITextureLoader* pTextureLoader = m_TextureLoaders[i];
		 for(size_t iExt = 0 ; iExt < pTextureLoader->nExt() ; iExt ++)
		 {
			 const wchar_t* _ext = pTextureLoader->ext(iExt);
			 m_TextureLoaderMap.insert(TextureLoaderMap::value_type(_ext , pTextureLoader) );
		 }
	}
}

static xTextureLdrMgrProxy* g_pTextureLdrMgrProxy = NULL;

xTextureLdrMgr::xTextureLdrMgr()
{
	if(g_pTextureLdrMgrProxy == NULL)
		g_pTextureLdrMgrProxy = new xTextureLdrMgrProxy;

}
xTextureLdrMgr::~xTextureLdrMgr()
{
	XSAFE_DELETE(g_pTextureLdrMgrProxy);
}

//自定义纹理加载器
ITextureLoader* xTextureLdrMgr::findTextureLoader(const wchar_t* texExt)
{
	return g_pTextureLdrMgrProxy->findTextureLoader(texExt);
}
void            xTextureLdrMgr::addTextureLoader(ITextureLoader* pTextureLoader)
{
	g_pTextureLdrMgrProxy->addTextureLoader(pTextureLoader);
	
}
void            xTextureLdrMgr::removeTextureLoader(ITextureLoader* pTextureLoader)
{
	g_pTextureLdrMgrProxy->removeTextureLoader(pTextureLoader);
}

xTextureLdrMgr* xTextureLdrMgr::singleton()
{
	static xTextureLdrMgr texLdrMgr;
	return &texLdrMgr;
}

END_NAMESPACE_XEVOL3D