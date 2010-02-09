#include "../xStdPch.h"
#include "xRenderApiImp.h"
#include  "xBaseShader.h"
#include  "xShaderName.h"
#include  "xShaderManager.h"
#include  "xRenderView.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ xAlpahRefConstaBinder : public IShaderConstBinder
{
	xRenderApiMatContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xAlpahRefConstaBinder);
	float&                m_AlphaRefValue;
public:
	void     setDirty(){m_bDirty = true ;}
	xAlpahRefConstaBinder(float& _value) : m_AlphaRefValue(_value) {};
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst)
	{
		return pConst->setData( &m_AlphaRefValue , sizeof(m_AlphaRefValue) );
	}

	IShaderConstBinder*   createInstance()
	{
		return this;
	}
};


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
	virtual bool            drawRectf(IBaseTexture* pTexture, float vDestRect[4] , const xColor_4f& color);
	virtual bool            drawRectf(IBaseTexture* pTexture, float vDestRect[4] ,float vSrcRect[4]  , const xColor_4f& color);
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
	if(m_2DObject)
	{
		m_2DObject->setUVChanel( 1 );
	}
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

bool xFontRenderDevice::drawRectf(IBaseTexture* pTexture, float vDestRect[4] , const xColor_4f& color)
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

bool xFontRenderDevice::drawRectf(IBaseTexture* pTexture, float vDestRect[4] ,float vSrcRect[4]  , const xColor_4f& color)
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

class xRenderObjectSet: public IRenderObjectSets
{
public:
	void                  addRenderObject(IRenderApiObject* pObject);
	void                  removeRenderObject(IRenderApiObject* pObject);
	void                  dumpRenderObject();
	void                  KillRenderObject();
public:
	typedef std::map<IRenderApiObject* ,IRenderApiObject* , std::less<IRenderApiObject*> , dllsafe_alloc<IRenderApiObject*> > OBJECTSETS;
	OBJECTSETS            m_RenderObjectSet;
};

void  xRenderObjectSet::addRenderObject(IRenderApiObject* pObject)
{
	m_RenderObjectSet[pObject] = pObject;//.insert(pObject);
}

void  xRenderObjectSet::removeRenderObject(IRenderApiObject* pObject)
{
	m_RenderObjectSet.erase(pObject);
}

void  xRenderObjectSet::dumpRenderObject()
{
	OBJECTSETS::iterator pos = m_RenderObjectSet.begin();
	XEVOL_LOG(eXL_DEBUG_HIGH , "Begin RendererObject dumping......\n");
	for(; pos != m_RenderObjectSet.end() ; pos ++)
	{
		IRenderApiObject* pObject = pos->second;//*pos;
		int RefCount = pObject->RefCount();
		const xObjectClassID& clsID = pObject->classID();
		XEVOL_LOG(eXL_DEBUG_HIGH , L"   ->RenderApi Object not released: refcount=%d , name=%s POINTER=0x%XH\n" , pObject->RefCount() , clsID.name() , (int)pObject );
	}
	XEVOL_LOG(eXL_DEBUG_HIGH , "End RendererObject dumping......\n");   
	return ;
}

void  xRenderObjectSet::KillRenderObject()
{
	//while(m_RenderObjectSet.size())
	//{
	//    OBJECTSETS::iterator pos = m_RenderObjectSet.begin();
	//    IRenderApiObject* pObject = *pos;
	//    int RefCount = pObject->RefCount();
	//    const xObjectClassID& clsID = pObject->classID();
	//    XEVOL_LOG(eXL_DEBUG_HIGH , L"   ->RenderApi Object Released: refcount=%d , name=%s POINTER=0x%XH\n" , pObject->RefCount() , clsID.name() , (int)pObject );
	//    pObject->ReleaseObject();
	//    continue;
	//}
	return ;
}


xRenderApiImp::xRenderApiImp()
:m_FontManager(L"XFontManager" , 1)
{
	m_FontRenderDevice = new xFontRenderDevice(this);
	m_FontManager.setFontRenderDevice( m_FontRenderDevice );
	m_pRenderObjectSets = new xRenderObjectSet;
	//m_RenderObjectSet.reserve(10000);

}

xRenderApiImp::~xRenderApiImp()
{
    XSAFE_DELETE(m_pRenderObjectSets);
	XSAFE_DELETE(m_FontRenderDevice);
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
	return m_FontManager.findFont(family , fontName , false);
}

HFontRender xRenderApiImp::findFont(const wchar_t* fontName)
{
	return m_FontManager.findFont(fontName , false);
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
	size_t nTexLoaders = m_TextureLoaders.size() ;
	for(size_t i = 0 ; i < nTexLoaders ; i ++)
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


//=====Basic RenderApi===========
#define GUID_SELECT_MODE(_value)  if( m_RenderMode == eRenderMode_Select ) return _value;
xRenderApiBase::xRenderApiBase()
:m_GpuProgramMgr(L"GpuProgramMgr" , 1)
,m_ShaderManager(L"ShaderManager" , 1)
,m_StateObjMgr(L"RdrStateManager"    , 1)
{
	m_RefCount = 1;
	for(int i = 0 ; i < Texture_StageMax ; i ++)
	{
		m_texLayer[i] = NULL;
	}

	m_PrimType   = ePrimtiveType_Triangles;
	m_pGpuProgram = NULL;
	m_pInputAssembler = NULL;
	m_pVertStream  = NULL;
	//设备属性和参数
	m_hMainWnd = NULL;
	m_Width    = 0 ;
	m_Height   = 0;

	m_2DZValue      = 0.0f;

	m_GpuProgramMgr.setRenderApi(this , L".hlsl");
	m_ShaderManager.setRenderApi(this , m_GpuProgramMgr.codeManager() , L".hlsl");
	m_StateObjMgr.setRenderApi(this);
	m_hMainWnd          = NULL;
	m_Width             = 0;
	m_Height            = 0;

	//上下文状态
	m_PrimType         = ePrimtiveType_Triangles;
	m_pGpuProgram      = NULL;
	m_pInputAssembler  = NULL;

	m_pDevLocker       = NULL;
	m_RenderMode       = eRenderMode_Render;

	m_BlenderState      = NULL;
	m_RasterizerState   = NULL;
	m_DepthStencilState = NULL;

	m_LightingState = NULL;
	clearAllTexture();
	enableThreadLocker(true);
	m_MatrixCtx = new xRenderApiMatContext(this);

	m_pBaseSelector = NULL;
	m_pColorSelector = NULL;
	m_pCallback = NULL;

	m_AlphaRef = 0.0f;
	m_pAlphaRefValueConstBinder = NULL;
}

IColorSelector* xRenderApiBase::colorSelector()
{
	return m_pColorSelector;
}

bool xRenderApiBase::setColorSelector(IColorSelector* pColorSel)
{
	if(pColorSel == NULL) 
	{
		m_pColorSelector = m_pBaseSelector;
		return true;
	}
	m_pColorSelector = pColorSel;
	return true;
}


eRenderMode xRenderApiBase::renderMode()
{
	return m_RenderMode;
}


bool xRenderApiBase::beginSelectMode()
{
	if(m_pColorSelector == NULL)
		return false;
	m_pColorSelector->begin();
	m_RenderMode = eRenderMode_Select;
	return true;
}

bool xRenderApiBase::endSelectMode(int x , int y , int _id[2] )
{
	if(m_pColorSelector == NULL)
		return false ;
	m_RenderMode = eRenderMode_Render;
	m_pColorSelector->end();
	return m_pColorSelector->getSelectID(x , y , 1 , 1 , _id);
}

bool xRenderApiBase::endSelectMode(int x , int y , int w , int h , void* _id)
{
	if(m_pColorSelector == NULL)
		return false ;
	m_RenderMode = eRenderMode_Render;
	m_pColorSelector->end();
	return m_pColorSelector->getSelectID(x , y , w , h , _id);
}

xRenderApiBase::~xRenderApiBase()
{
	uninit();
}


bool xRenderApiBase::uninit( )
{
	XSAFE_DELETE(m_pAlphaRefValueConstBinder);
	XSAFE_RELEASEOBJECT(m_LightingState);
	XSAFE_DELETE(m_pBaseSelector);
	freeAllTextureManager();
	m_StateObjMgr.clear();
	m_FontManager.clear();
	m_GpuProgramMgr.clear();
	m_ShaderManager.clear();
	KillRenderObject();
	return true;
}


bool xRenderApiBase::getWindowSize(int& w , int & h)
{
#ifdef _WIN32
	RECT _Rect;
	GetClientRect(m_hMainWnd , &_Rect);
	w = _Rect.right  - _Rect.left;
	h = _Rect.bottom - _Rect.top;
	return true;
#endif
	return false;	
}

bool xRenderApiBase::getRenderViewSize(int& w , int & h)
{
	if(m_RenderView == NULL)
		return false;

	xTextureDesc _desc;
	m_RenderView->desc(_desc);
	w = _desc.m_width;
	h = _desc.m_height;
	return true;
}

bool xRenderApiBase::init(xXmlNode* pSysNode)
{
	//========建立光照环境====================
	m_LightingState = createLightingState(L"xLightState");
	//初始化矩阵
	m_MatrixCtx->init(this);

	//------------------------------------------
	//初始化配置文件里的东西
	//------------------------------------------
	//IRenderApi::init(pSysNode);
	TAutoLocker<xRenderApiLocker> aLocker(m_pDevLocker);
	xXmlNode* pRendererNode = pSysNode->findNode( name() , 0 );
	xXmlNode* pShaderMgrNode = pRendererNode->findNode(L"ShaderManager",0);

	//Load Shader Manager configuration
	if(pShaderMgrNode)
	{
		xXmlNode::XmlNodes pathNodes ;
		pShaderMgrNode->findNode(L"path",pathNodes);
		size_t _nPathNodes =  pathNodes.size() ;
		for(size_t i = 0 ; i < _nPathNodes ; i ++)
		{
			xXmlNode* pPathNode = pathNodes[i];
			const wchar_t* strPath =  pPathNode->value(L"path");
			const wchar_t* strPkg  =  pPathNode->value(L"file");
			if(strPkg)
			{
				m_GpuProgramMgr.addPackage( _XEVOL_ABSPATH_(strPkg) , strPath?strPath:L"");

			}
			else if(strPath)
			{
				m_GpuProgramMgr.addPath( _XEVOL_ABSPATH_(strPath) );
			}
		}

		xXmlNode* pPreGenNode = pShaderMgrNode->findNode(L"PreGen");
		if(pPreGenNode)
		{
			const wchar_t* strPath =  pPreGenNode->value(L"path");
			bool           bPreGen =  pPreGenNode->bool_value(L"enable");
			if(strPath) m_ShaderManager.setOutputForlder( _XEVOL_ABSPATH_ (strPath) );
			m_ShaderManager.enablePreGenerator(bPreGen);
		}
	}

	xXmlNode* pStateMgrNode = pRendererNode->findNode(L"StateManager",0);

	//Load State Manager configuration
	if(pStateMgrNode)
	{
		xXmlNode::XmlNodes pathNodes ;
		pStateMgrNode->findNode(L"path",pathNodes);
		size_t _nPathNodes =  pathNodes.size() ;
		for(size_t i = 0 ; i < _nPathNodes ; i ++)
		{
			xXmlNode* pPathNode = pathNodes[i];
			const wchar_t* strPath =  pPathNode->value(L"path");
			const wchar_t* strPkg  =  pPathNode->value(L"file");
			if(strPkg)
			{
				m_StateObjMgr.addPackage( _XEVOL_ABSPATH_(strPkg) , strPath?strPath:L"");

			}
			else
			{
				m_StateObjMgr.addPath( _XEVOL_ABSPATH_(strPath) );
			}
		}
	}

	xXmlNode* pFontNode = pSysNode->findNode(L"fontconfig");
	if(pFontNode)
	{
		m_FontManager.load(pFontNode);
	}
	//load other configuration

	m_FontRenderDevice->init();

	XSAFE_DELETE(m_pAlphaRefValueConstBinder);
	m_pAlphaRefValueConstBinder = new xAlpahRefConstaBinder(m_AlphaRef);
	registeShaderConstBinder(L"AlphaRefValue"      , m_pAlphaRefValueConstBinder );
	registeShaderConstBinder(L"AlphaRef"           , m_pAlphaRefValueConstBinder );
	return true;
}



bool xRenderApiBase::enableThreadLocker(bool bEnable)
{
	if(bEnable == false) 
		m_pDevLocker = NULL;
	else
		m_pDevLocker = &m_RenderLocker;//&m_devLocker;

	return true;
}



static int g_iLocked  = 0;
bool xRenderApiBase::lock()
{
	m_RenderLocker.lock();
	g_iLocked ++;
	return true;
}

bool xRenderApiBase::unlock()
{
	g_iLocked --;
	m_RenderLocker.unlock();
	return true;
}

bool xRenderApiBase::begin(xColor_4f& bkColor , float z , unsigned int stencil)
{
	lock();
	TAutoLocker<xRenderApiLocker> aLocker(m_pDevLocker);
	return m_RenderView->clear(bkColor , z , stencil );
}

bool xRenderApiBase::beginScene()
{
	//XEVOL_WARNNING_NOT_IMPLEMENT
	lock();
	return true;
}

bool xRenderApiBase::endScene()
{
	//XEVOL_WARNNING_NOT_IMPLEMENT
	unlock();
	return true;
}


ISamplerState* xRenderApiBase::createSamplerState(const wchar_t* name)
{
	std::wstring full_name = name + std::wstring(L".sampler");
	HRenderState hState = this->m_StateObjMgr.add(full_name.c_str() , (int)(eRenderState_Sampler) , true);
	return (ISamplerState*) hState.getResource();
}

IBlenderState* xRenderApiBase::createBlendState(const wchar_t* name)
{
	std::wstring full_name = name + std::wstring(L".blend");
	HRenderState hState = this->m_StateObjMgr.add(full_name.c_str() , (int)(eRenderState_Blend) , true);
	return (IBlenderState*) hState.getResource();
}

IRasterizerState* xRenderApiBase::createRasterizerState(const wchar_t* name)
{
	std::wstring full_name = name + std::wstring(L".ras");
	HRenderState hState = this->m_StateObjMgr.add(full_name.c_str() , (int)(eRenderState_Rasterizer) , true);
	return (IRasterizerState*) hState.getResource();
}

IDepthStencilState*  xRenderApiBase::createDepthStencilState(const wchar_t* name)
{
	std::wstring full_name = name + std::wstring(L".zs");
	HRenderState hState = this->m_StateObjMgr.add(full_name.c_str() , (int)(eRenderState_DepthStencil) , true);
	return (IDepthStencilState*) hState.getResource();
}

IBlenderState*  xRenderApiBase::getBlendState()
{
	return m_BlenderState;
}

IRasterizerState* xRenderApiBase::getRasterizerState()
{
	return m_RasterizerState;
}

IDepthStencilState*  xRenderApiBase::getDepthStencilState()
{
	return m_DepthStencilState;
}


IInputAssembler* xRenderApiBase::getInputAssembler(const wchar_t* name)
{
	//First create a assembler
	IInputAssembler* assembler = objectManager().inputAssemblerMgr().find(name);
	if(assembler) 
	{
		assembler->AddRef();
		return assembler ;
	}
	return NULL;
}

IInputBuffer* xRenderApiBase::findInputBuffer(const wchar_t* _name)
{
	IInputBuffer* pBuffer = m_objManager.InputBufferMgr().find(_name);
	return pBuffer;
}

bool xRenderApiBase::removeInputBuffer(IInputBuffer* pConstBuffer)
{
	if(pConstBuffer)
	{
		m_objManager.InputBufferMgr().remove(pConstBuffer );
		pConstBuffer->ReleaseObject();
		return true;
	}
	else
	{
		return false;
	}
	return true;
}

bool xRenderApiBase::removeInputBuffer(const wchar_t* _name)
{
	if(_name)
	{
		IInputBuffer* pOldBuffer = m_objManager.InputBufferMgr().find(_name);
		if(pOldBuffer)
		{
			removeInputBuffer(pOldBuffer);
			pOldBuffer->ReleaseObject();			
		}
		return true;
	}
	else
	{
		return false;		
	}
}

bool xRenderApiBase::setInputBuffer(const wchar_t* _name, IInputBuffer* pConstBuffer)
{
	if(pConstBuffer)
	{
		IInputBuffer* pOldBuffer = m_objManager.InputBufferMgr().find(_name);
		if(pOldBuffer)
		{
			pOldBuffer->ReleaseObject();
			removeInputBuffer(pOldBuffer);
		}
		m_objManager.InputBufferMgr().add(xStringHash(_name) , pConstBuffer );
		pConstBuffer->AddRef();
	}
	else
	{
		m_objManager.InputBufferMgr().remove(_name );
	}
	return true;
}

IInputBuffer* xRenderApiBase::createInputBuffer(size_t iBufLen)
{
	xInputBufferDesc cbDesc;
	cbDesc.m_bindtype    = BIND_AS_SHADER_RESOURCE ;
	cbDesc.m_accessFlage = RESOURCE_ACCESS_WRITE;
	cbDesc.m_usage       = RESOURCE_USAGE_DYNAMIC;
	return createBuffer(iBufLen , &cbDesc, NULL);
}

IInputBuffer* xRenderApiBase::createConstBuffer(size_t iBufLen)
{
	xInputBufferDesc cbDesc;
	cbDesc.m_bindtype    = BIND_AS_CONSTANT_BUFFER ;
	cbDesc.m_accessFlage = RESOURCE_ACCESS_WRITE;
	cbDesc.m_usage       = RESOURCE_USAGE_DYNAMIC;
	return createBuffer(iBufLen , &cbDesc, NULL);
}


IInputAssembler*  xRenderApiBase::getInputAssembler()
{
	return m_pInputAssembler;
}


bool xRenderApiBase::beginDrawPrimitive()
{
	if(m_pGpuProgram==NULL)
	{
		XEVOL_LOG(eXL_ERROR_FALT,L"Direct3D10中,不设置Shader是不能进行绘制的\n");
		return false;
	}
	//安装Shader;
	m_pGpuProgram->commit();

	//如果是ColorSelect 模式，则需要安装Color选择PixelShader
	if(m_RenderMode == eRenderMode_Select && m_pColorSelector)
	{
		m_pColorSelector->beginPrimitive();
	}
	return true;
}
bool xRenderApiBase::endDrawPrimitive()
{
	if(m_RenderMode == eRenderMode_Select && m_pColorSelector)
	{
		m_pColorSelector->endPrimitive();
	}
	return true;
}

bool xRenderApiBase::draw(IInputBuffer* pIdxBuffer , size_t nVertex , size_t iStartVertex, ePrimtiveType pt)
{
	TAutoLocker<xRenderApiLocker> aLocker(m_pDevLocker);
	setIndexBuffer(pIdxBuffer);

	if(false == beginDrawPrimitive() ) 
		return false;

	bool bDrawRet = this->drawPrimitive(nVertex , iStartVertex , pt);

	bool bRet = endDrawPrimitive();

	return bDrawRet&bRet;
}

bool xRenderApiBase::draw(size_t nVertex , size_t iStartVertex, ePrimtiveType pt)
{
	TAutoLocker<xRenderApiLocker> aLocker(m_pDevLocker);
	if(false == beginDrawPrimitive() ) return false;

	bool bDrawRet = drawPrimitive(nVertex , iStartVertex , pt);
	bool bRet = endDrawPrimitive();

	return bDrawRet&bRet;
}

bool xRenderApiBase::set2DZValue(float zValue)
{
	m_2DZValue = zValue;
	return true;
}

float xRenderApiBase::get2DZValue()
{
	return m_2DZValue;
}

bool xRenderApiBase::draw2DRect(I2DRectObject* p2DRect)
{
	p2DRect->commit();
	return p2DRect->draw();
}

HBaseShader xRenderApiBase::createShader(const wchar_t* shaderName , eShaderType type)
{
	HBaseShader hShader = m_ShaderManager.add(shaderName,(unsigned int)type , true );
	return hShader;
}

xBaseShaderMgr* xRenderApiBase::shaderManager()
{
	return &m_ShaderManager;
}

bool xRenderApiBase::pushGpuProgram(IGpuProgram* pGpuProgram)
{
	m_pGpuProgram.push();
	setGpuProgram(pGpuProgram);
	return true;
}

bool xRenderApiBase::popGpuProgram()
{
	IGpuProgram* pGpuProg = m_pGpuProgram.pop();
	if( pGpuProg ==  NULL)
		return false;
	setGpuProgram(pGpuProg);
	return true;
}

IGpuProgram* xRenderApiBase::getGpuProgram( )
{
	return m_pGpuProgram;
}

bool  xRenderApiBase::setTexture(const int iTexName      , IBaseTexture* pTexture)
{
	if(iTexName < Texture_StageMax)
	{
		m_texLayer[iTexName] = pTexture;
		return true;
	}
	m_mapTexLayers[iTexName] = pTexture;
	return true;
}

bool  xRenderApiBase::setTexture(const wchar_t* iTexName , IBaseTexture* pTexture)
{
	if(iTexName == NULL)
		return false ;

	int iName = xStringHash(iTexName);

	if(iName < Texture_StageMax)
		return false;

	m_mapTexLayers[iName] = pTexture;
	return true;
}

IBaseTexture*  xRenderApiBase::getTexture(const int iTexName)
{
	if(iTexName < Texture_StageMax)
	{
		return m_texLayer[iTexName];
	}
	vTexLayers::iterator pos = m_mapTexLayers.find(iTexName);
	if(pos != m_mapTexLayers.end() )
		return pos->second;
	return NULL;
}

IBaseTexture*  xRenderApiBase::getTexture(const wchar_t* iTexName)
{
	if(iTexName == NULL)
		return  NULL;

	int iName = xStringHash(iTexName);
	if(iName < Texture_StageMax)
	{
		return NULL;
	}
	vTexLayers::iterator pos = m_mapTexLayers.find(iName);
	if(pos != m_mapTexLayers.end() )
		return pos->second;
	return NULL;
}

bool  xRenderApiBase::clearAllTexture()
{
	for(int i = 0 ; i < Texture_StageMax ; i ++)
	{
		m_texLayer[i] = NULL;
	}
	m_mapTexLayers.clear();
	return true;
}

IShaderConstBinder*  xRenderApiBase::getShaderConstBinder(const wchar_t* constName)
{
	int hashKey = xStringHash2(constName);
	mapConstBinder::iterator pos  = m_constNameBinder.find(hashKey);
	if(pos != m_constNameBinder.end() )
	{
		return pos->second;
	}
	return NULL;
}

void xRenderApiBase::registeShaderConstBinder(const wchar_t* constName , IShaderConstBinder* pBinder)
{
	int hashKey = xStringHash2(constName);

	if(getShaderConstBinder(constName) != NULL)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH,L"你注册的Shader绑定器已经存在! name=%s\n" , constName);
		return ;
	}
	m_constNameBinder.insert(mapConstBinder::value_type(hashKey , pBinder) ) ;

	return ;
}

IShaderConstBinder* xRenderApiBase::getShaderConstBinder(eShaderParamSemantic _semantic)
{
	mapConstBinder::iterator pos  = m_semanticBinder.find( (int)_semantic);
	if(pos != m_semanticBinder.end() )
	{
		return pos->second;
	}
	return NULL;
}

void xRenderApiBase::registeShaderConstBinder(eShaderParamSemantic _semantic , IShaderConstBinder* pBinder)
{
	if(getShaderConstBinder(_semantic) != NULL)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH,L"你注册的Shader绑定器已经存在! name=%s\n" , _semantic);
		return ;
	}
	m_semanticBinder.insert(mapConstBinder::value_type( (int)_semantic , pBinder) ) ;
	return ;
}


xBaseTextureMgr* xRenderApiBase::createTextureManager(const wchar_t* _name , bool nonRefKeep)
{
	xBaseTextureMgr* pMgr = new xBaseTextureMgr(_name , nonRefKeep ? 1 : 0 );
	pMgr->setRenderApi(this);
	m_TextureMagrs.push_back(pMgr);
	return pMgr;
}

void xRenderApiBase::deleteTextureManager(xBaseTextureMgr* pTextureMgr)
{
	std::vector<xBaseTextureMgr*>::iterator pos = m_TextureMagrs.begin();
	for(;pos != m_TextureMagrs.end() ; pos ++)
	{
		xBaseTextureMgr* pTexMgr = *pos;
		if(pTexMgr == pTextureMgr)
		{
			pTextureMgr->unload();
			delete pTextureMgr;
			m_TextureMagrs.erase(pos);
			return ;
		}
	}
}

void xRenderApiBase::freeAllTextureManager()
{
	size_t _nTexMgr = m_TextureMagrs.size() ;
	for(size_t i = 0 ; i < _nTexMgr ; i ++)
	{
		m_TextureMagrs[i]->clear();
		XSAFE_DELETE(m_TextureMagrs[i]);
	}
	m_TextureMagrs.clear();
}

void xRenderApiBase::unloadAllTextureManager()
{
	size_t _nTexMgr = m_TextureMagrs.size() ;
	for(size_t i = 0 ; i < _nTexMgr ; i ++)
	{
		m_TextureMagrs[i]->unload();
	}
}

bool xRenderApiBase::enter2DMode(int x , int y , int w , int h , bool bTextMode)
{
	m_MatrixCtx->pushMatrix(MATRIXMODE_Project);
	m_MatrixCtx->pushMatrix(MATRIXMODE_View);
	if( w ==  0 || h == 0)
	{
		xTextureDesc _desc;
		getRenderView()->desc(_desc);
		w = _desc.m_width;
		h = _desc.m_height;
	}
	//D3DXMATRIX mat;
	//D3DXMatrixOrthoOffCenterRH(&mat, (float)0, (float)w , (float)h , (float)0 ,-100000.0f,100000.0f);
	xmat4 mat;
	XM_Ortho3D(mat , -100000.0f , 100000.0f , 0.0f , (float)h , 0.0f , (float)w );
	setMatrix( (float *)mat.m , MATRIXMODE_Project);
	identityMatrix(MATRIXMODE_View);
	return true;
}

bool xRenderApiBase::leave2DMode()
{
	m_MatrixCtx->popMatrix(MATRIXMODE_Project);
	m_MatrixCtx->popMatrix(MATRIXMODE_View);
	return true;
}

bool xRenderApiBase::multiMatrix(const float* mat , eMatrixMode matMode)
{
	return m_MatrixCtx->multiMatrix( mat , matMode);
}

bool xRenderApiBase::setMatrix(const float* mat   , eMatrixMode matMode)
{
	return m_MatrixCtx->setMatrix( mat , matMode);
}

bool xRenderApiBase::identityMatrix(eMatrixMode matMode)
{
	return m_MatrixCtx->identityMatrix(matMode);
}

bool xRenderApiBase::getMatrix(float* mat , eMatrixMode matMode)
{
	return m_MatrixCtx->getMatrix( mat , matMode);
}

bool xRenderApiBase::pushMatrix(eMatrixMode matMode)
{
	return m_MatrixCtx->pushMatrix(matMode );
}

bool xRenderApiBase::popMatrix(eMatrixMode matMode)
{
	return m_MatrixCtx->popMatrix( matMode);
}

bool xRenderApiBase::applyCamera()
{
	return m_MatrixCtx->applyCamera();
}

bool xRenderApiBase::applyCamera(IRenderCamera* pCamera)
{
	return m_MatrixCtx->applyCamera(pCamera);
}

IRenderCamera* xRenderApiBase::getCamera()
{
	return m_MatrixCtx->getCamera();
}

IRenderView* xRenderApiBase::getRenderView()
{
	return m_RenderView;
}

bool xRenderApiBase::pushRenderView(IRenderView* renderView)
{
	GUID_SELECT_MODE(false);
	m_RenderView.push(  );
	setRenderView(renderView);
	return false;
}

bool xRenderApiBase::popRenderView()
{
	GUID_SELECT_MODE(false);
	IRenderView* renderView = m_RenderView.pop();
	if(renderView == NULL)
		return false;
	setRenderView(renderView);
	return true;
}


ILightingEnv* xRenderApiBase::findLightingState(const wchar_t* _name)
{
	ILightingEnv* pLightingEnv = m_objManager.LightEnvMgr().find(_name );
	return pLightingEnv;
}

bool xRenderApiBase::removeLightingState(const wchar_t* _name)
{
	m_objManager.LightEnvMgr().remove(_name);
	return true;
}

ITextureLoader* xRenderApiBase::findTextureLoader(const wchar_t* texExt)
{
	return xTextureLdrMgr::singleton()->findTextureLoader(texExt);
}

void xRenderApiBase::addTextureLoader(ITextureLoader* pTextureLoader)
{
	xTextureLdrMgr::singleton()->addTextureLoader(pTextureLoader);
}

void xRenderApiBase::removeTextureLoader(ITextureLoader* pTextureLoader)
{
	xTextureLdrMgr::singleton()->removeTextureLoader(pTextureLoader);
}

bool xRenderApiBase::pushBlendState(IBlenderState* pState)
{
    IBlenderState* pOldState = this->getBlendState();
	m_vStackBlenderState.push_back(pOldState);
	return setBlendState(pState);
}

bool xRenderApiBase::pushRasterizerState(IRasterizerState* pState)
{
	IRasterizerState* pOldState = this->getRasterizerState();
	m_vStackRasterizerStates.push_back(pOldState);
	return setRasterizerState(pState);
}

bool xRenderApiBase::pushDepthStencilState(IDepthStencilState* pState)
{
	IDepthStencilState* pOldState = this->getDepthStencilState();
	m_vStackDepthStencilStates.push_back(pOldState);
	return setDepthStencilState(pState);
}

bool xRenderApiBase::popBlendState()
{
     size_t iLast = m_vStackBlenderState.size();
	 IBlenderState* pTopState = NULL;
	 if(iLast != 0)
	 {
		 pTopState = m_vStackBlenderState[iLast - 1];
		 m_vStackBlenderState.pop_back();
	 }
	 return setBlendState(pTopState);
}

bool xRenderApiBase::popRasterizerState()
{
	size_t iLast = m_vStackRasterizerStates.size();
	IRasterizerState* pTopState = NULL;
	if(iLast != 0)
	{
		pTopState = m_vStackRasterizerStates[iLast - 1];
		m_vStackRasterizerStates.pop_back();
	}
	return setRasterizerState(pTopState);
}

bool xRenderApiBase::popDepthStencilState()
{
	size_t iLast = m_vStackDepthStencilStates.size();
	IDepthStencilState* pTopState = NULL;
	if(iLast != 0)
	{
		pTopState = m_vStackDepthStencilStates[iLast - 1];
		m_vStackDepthStencilStates.pop_back();
	}
	return setDepthStencilState(pTopState);
}


END_NAMESPACE_XEVOL3D