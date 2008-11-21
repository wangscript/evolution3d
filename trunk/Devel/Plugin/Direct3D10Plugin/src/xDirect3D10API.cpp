#include <BaseLib/xEvol3DAPI.h>
#include <BaseLib/xCfgParser.h>
#include <OperationSys/xOperationSys.h>
#include "xDirect3D10API.h"
#include "xD10_InputAssembler.h"
#include "xD10InputBuffer.h"
#include "xD10Shader.h"
#include "xD10GPUProgram.h"
#include "xD10FileTexture.h"
#include "xD10LockableTexture.h"
#include "xD10Sampler.h"
#include "xD10RazState.h"
#include "xD10BlendState.h"
#include "xD10DepthStencilState.h"
#include "xD10RenderTarget.h"
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10d.lib")
#include "xD10HelperRender.h"
#include "xD10RenderView.h"
#include "xD10BaseColorSelector.h"
#include "Image/PixelFormatAssit.h"
#include "fs/xFileSystem.h"
BEGIN_NAMESPACE_XEVOL3D

#define GUID_SELECT_MODE(_value)  if( m_RenderMode == eRenderMode_Select ) return _value;
xD3D10RenderApi::xD3D10RenderApi()
:m_GpuProgramMgr(L"GpuProgramMgr" , 1)
,m_ShaderManager(L"ShaderManager" , 1)
,m_StateObjMgr(L"RdrStateManager"    , 1)
{
	m_RefCount = 1;
	for(int i = 0 ; i < Texture_StageMax ; i ++)
	{
		m_texLayer[i] = NULL;
	}
	m_pD3D10Device  = NULL;
	m_PrimType   = ePrimtiveType_Triangles;
	m_pGpuProgram = NULL;
	m_pInputAssembler = NULL;
	m_pVertStream  = NULL;
	//设备属性和参数
	m_hMainWnd = NULL;
	m_Width    = 0 ;
	m_Height   = 0;
	m_hInst    = NULL;
	m_driverType = D3D10_DRIVER_TYPE_HARDWARE;	
	m_pSwapChain = NULL;
	m_RenderWindow = NULL;
	memset(&m_swapChainDesc , 0 , sizeof(DXGI_SWAP_CHAIN_DESC) );
	m_pDef2DRect    = NULL;
	m_pDef2DRectEnv  = NULL;
	m_2DZValue      = 0.0f;

	m_GpuProgramMgr.setRenderApi(this , L".hlsl");
	m_ShaderManager.setRenderApi(this , m_GpuProgramMgr.codeManager() , L".hlsl");
	m_StateObjMgr.setRenderApi(this);
	m_hInst             = GetModuleHandle(NULL);
	m_driverType        = D3D10_DRIVER_TYPE_NULL;
	m_pSwapChain        = NULL;
	m_pD3D10Device      = NULL;
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
	//初始化矩阵
	m_MatrixCtx.init(this);
	clearAllTexture();
	enableThreadLocker(true); 
}

IColorSelector* xD3D10RenderApi::colorSelector()
{
	return m_pColorSelector;
}

bool xD3D10RenderApi::setColorSelector(IColorSelector* pColorSel)
{
	if(pColorSel == NULL) 
	{
		m_pColorSelector = m_pBaseSelector;
		return true;
	}
	m_pColorSelector = pColorSel;
	return true;
}


eRenderMode xD3D10RenderApi::renderMode()
{
	return m_RenderMode;
}


bool xD3D10RenderApi::beginSelectMode()
{
	if(m_pColorSelector == NULL)
		return false;
    m_pColorSelector->begin();
	m_RenderMode = eRenderMode_Select;
	return true;
}

bool xD3D10RenderApi::endSelectMode(int x , int y , int _id[2] )
{
	if(m_pColorSelector == NULL)
		return false ;
	m_RenderMode = eRenderMode_Render;
	m_pColorSelector->end();
	return m_pColorSelector->getSelectID(x , y , 1 , 1 , _id);
}

bool xD3D10RenderApi::endSelectMode(int x , int y , int w , int h , void* _id)
{
	if(m_pColorSelector == NULL)
		return false ;
	m_RenderMode = eRenderMode_Render;
    m_pColorSelector->end();
	return m_pColorSelector->getSelectID(x , y , w , h , _id);
}

xD3D10RenderApi::~xD3D10RenderApi()
{
	uninit();
}

ID3D10Device*  xD3D10RenderApi::d10Device()
{
	return m_pD3D10Device ; 
}

bool xD3D10RenderApi::uninit( )
{
	freeAllTextureManager();
	m_StateObjMgr.clear();
	m_FontManager.clear();
	m_GpuProgramMgr.clear();
	m_ShaderManager.clear();

    XSAFE_DELETE(m_pBaseSelector);
	XSAFE_DELETE(m_RenderWindow);
	return true;
}

bool xD3D10RenderApi::init(xCfgNode* pSysNode)
{
	//IRenderApi::init(pSysNode);
	TAutoLocker<xD3D10Locker> aLocker(m_pDevLocker);
	xCfgNode* pRendererNode = pSysNode->findNode( name() , 0 );
	xCfgNode* pShaderMgrNode = pRendererNode->findNode(L"ShaderManager",0);
	
	//Load Shader Manager configuration
	if(pShaderMgrNode)
	{
		xCfgNode::CfgNodes pathNodes ;
		pShaderMgrNode->findNode(L"path",pathNodes);
		for(size_t i = 0 ; i < pathNodes.size() ; i ++)
		{
			xCfgNode* pPathNode = pathNodes[i];
			const wchar_t* strPath =  pPathNode->value(L"path");
			const wchar_t* strPkg  =  pPathNode->value(L"file");
			if(strPkg)
			{
				m_GpuProgramMgr.addPackage( _ABSPATH(strPkg) , strPath?strPath:L"");

			}
			else if(strPath)
			{
				m_GpuProgramMgr.addPath( _ABSPATH(strPath) );
			}
		}

		xCfgNode* pPreGenNode = pShaderMgrNode->findNode(L"PreGen");
		if(pPreGenNode)
		{
			const wchar_t* strPath =  pPreGenNode->value(L"path");
			bool           bPreGen =  pPreGenNode->bool_value(L"enable");
			if(strPath) m_ShaderManager.setOutputForlder( _ABSPATH (strPath) );
			m_ShaderManager.enablePreGenerator(bPreGen);
		}
	}

	xCfgNode* pStateMgrNode = pRendererNode->findNode(L"StateManager",0);

	//Load State Manager configuration
	if(pStateMgrNode)
	{
		xCfgNode::CfgNodes pathNodes ;
		pStateMgrNode->findNode(L"path",pathNodes);
		for(size_t i = 0 ; i < pathNodes.size() ; i ++)
		{
			xCfgNode* pPathNode = pathNodes[i];
			const wchar_t* strPath =  pPathNode->value(L"path");
			const wchar_t* strPkg  =  pPathNode->value(L"file");
			if(strPkg)
			{
				m_StateObjMgr.addPackage( _ABSPATH(strPkg) , strPath?strPath:L"");

			}
			else
			{
				m_StateObjMgr.addPath( _ABSPATH(strPath) );
			}
		}
	}

	xCfgNode* pFontNode = pSysNode->findNode(L"fontconfig");
	if(pFontNode)
	{
		m_FontManager.load(pFontNode);
	}
	//load other configuration

	m_FontRenderDevice->init();

	//现在来初始化选择Buffer;

	xCfgNode* pSelNode = pRendererNode->findNode(L"PixelSelect");//  pSysNode->bool_value(L"pixel_select");
	if(pSelNode && pSelNode->bool_value(L"enable"))
	{
		m_pBaseSelector = new xD10BaseColorSelector(this);
		m_pBaseSelector->create(pSelNode ) ;
		m_pColorSelector = m_pBaseSelector;
	}
	return true;
}



bool xD3D10RenderApi::enableThreadLocker(bool bEnable)
{
    if(bEnable == false) 
		m_pDevLocker = NULL;
	else
		m_pDevLocker = &m_RenderLocker;//&m_devLocker;

	return true;
}


bool xD3D10RenderApi::create(ID3D10Device* pDevice , HWND hWnd , int w , int h)
{
	TAutoLocker<xD3D10Locker> aLocker(m_pDevLocker);
	//MessageBoxA(NULL,"开始创建D3D10 Api","错误",MB_OK);
	m_pD3D10Device      = pDevice;
	m_hMainWnd          = hWnd;
	m_Width             = w;
	m_Height            = h;
	bool ret = false;
	if(m_pD3D10Device) 
	{ 
		ret = __initByDevice();
	}
	else
	{
		ret = __createD10Device();
	}
	//MessageBoxA(NULL,"创建D3D10 Api完成","错误",MB_OK);
	if(ret == false)
	{
		XEVOL_LOG(eXL_ERROR_FALT,"Create Direct3D 10 Failed\n");
	}
	
	__afterCreateDevice();
	enableThreadLocker(true);
	return ret;
}

bool xD3D10RenderApi::onResize(int width , int height)
{
	TAutoLocker<xD3D10Locker> aLocker(m_pDevLocker);
	TAutoLocker<xD3D10Locker> aRenderLocker(&m_RenderLocker);
	if(false == __needResize(width , height) )
	{
		m_RenderView->install();
		return false;
	}
	if(m_pBaseSelector)
	{
		m_pBaseSelector->resize(m_Width , m_Height );
	}
	//XEVOL_WARNNING_NOT_IMPLEMENT_INFO("在删除这些对象前应该判断是不是当前的RenderTarget");
	bool ret = m_RenderWindow->resize(m_Width  , m_Height);
	__resetViewPort();
	return m_RenderView->install();
}

bool xD3D10RenderApi::__needResize(int width , int height)
{
	if(width == 0 || height == 0)
	{
		RECT rc;
		GetClientRect( m_hMainWnd, &rc );
		width  = rc.right  - rc.left;
		height = rc.bottom - rc.top;
	}
	if(width == 0 || height == 0)
	{
		return false;
	}
	if(m_swapChainDesc.BufferDesc.Width == width && m_swapChainDesc.BufferDesc.Height == height)
	{
		return false;
	}
	m_Width  = width;
	m_Height = height;
	return  true;
}

bool xD3D10RenderApi::__resetViewPort()
{
	// Setup the viewport
	D3D10_VIEWPORT vp;
	vp.Width  = m_Width;
	vp.Height = m_Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pD3D10Device->RSSetViewports( 1, &vp );
	return true;
}

bool xD3D10RenderApi::__createD10Device()
{
	HRESULT hr = S_OK;;

	UINT width  = m_Width;
	UINT height = m_Height;

	if(width == 0 || height == 0)
	{
		RECT rc;
		GetClientRect( m_hMainWnd, &rc );
		width = rc.right - rc.left;
		height = rc.bottom - rc.top;
		m_Width  = width;
		m_Height = height;

	}
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

	D3D10_DRIVER_TYPE driverTypes[] = 
	{
		D3D10_DRIVER_TYPE_HARDWARE,
		D3D10_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

	
	m_Width   = width;
	m_Height  = height;
	ZeroMemory( &m_swapChainDesc, sizeof(m_swapChainDesc) );
	m_swapChainDesc.BufferCount = 1;
	m_swapChainDesc.BufferDesc.Width = width;
	m_swapChainDesc.BufferDesc.Height = height;
	m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_swapChainDesc.OutputWindow = m_hMainWnd;
	m_swapChainDesc.SampleDesc.Count = 1;
	m_swapChainDesc.SampleDesc.Quality = 0;
	m_swapChainDesc.Windowed = TRUE;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		m_driverType = driverTypes[driverTypeIndex];
		hr = D3D10CreateDeviceAndSwapChain( NULL, m_driverType, NULL, createDeviceFlags, D3D10_SDK_VERSION, &m_swapChainDesc, &m_pSwapChain, &m_pD3D10Device );
		if( SUCCEEDED( hr ) )
			break;
	}
	if( FAILED(hr) )
		return false;


    m_RenderWindow = new xD10RenderWindow(m_hMainWnd , this);
	m_RenderWindow->create(m_pSwapChain , width , height);
	__resetViewPort();
	setRenderView(m_RenderWindow);

	D3D10_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D10_BLEND_DESC));
	BlendState.BlendEnable[0] = FALSE;
	BlendState.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
	
	ID3D10BlendState*       g_pBlendStateNoBlend = NULL;
	ID3D10RasterizerState*	g_pRasterizerStateNoCull = NULL;
	m_pD3D10Device->CreateBlendState(&BlendState, &g_pBlendStateNoBlend) ;

	// Create a rasterizer state to disable culling
	D3D10_RASTERIZER_DESC RSDesc;
	RSDesc.FillMode = D3D10_FILL_SOLID;
	RSDesc.CullMode = D3D10_CULL_NONE;
	RSDesc.FrontCounterClockwise = TRUE;
	RSDesc.DepthBias = 0;
	RSDesc.DepthBiasClamp = 0;
	RSDesc.SlopeScaledDepthBias = 0;
	RSDesc.ScissorEnable = FALSE;
	RSDesc.MultisampleEnable = TRUE;
	RSDesc.AntialiasedLineEnable = FALSE;
	m_pD3D10Device->CreateRasterizerState(&RSDesc, &g_pRasterizerStateNoCull) ;

	m_pD3D10Device->OMSetBlendState(g_pBlendStateNoBlend, 0, 0xffffffff);
	m_pD3D10Device->RSSetState(g_pRasterizerStateNoCull);
	return true;
}

bool xD3D10RenderApi::__destory()
{
    XSAFE_RELEASEOBJECT(m_RenderWindow);
	XSAFE_RELEASE(m_pSwapChain);
	XSAFE_RELEASE(m_pD3D10Device);
	XSAFE_RELEASE(m_defDepthStencilState);
	XSAFE_RELEASE(m_defBlendState);
	XSAFE_RELEASE(m_defpRasState);
	return true;

}
bool xD3D10RenderApi::__afterCreateDevice()
{
	m_pD3D10Device->OMGetDepthStencilState( &m_defDepthStencilState , &m_DefStencilRef);
	m_pD3D10Device->OMGetBlendState(&m_defBlendState , m_DefBlendFactor , &m_DefSampleMask);
	m_pD3D10Device->RSGetState(&m_defpRasState);
	m_2DZValue = 0.0f;
	m_pDef2DRectEnv = new xD102DRectEnv(this);
	m_pDef2DRect    = new xD102DRect(m_pDef2DRectEnv);
     return true;   
}

bool xD3D10RenderApi::__initByDevice()
{
	XEVOL_WARNNING_NOT_IMPLEMENT
    return false;
}

static int g_iLocked  = 0;
bool xD3D10RenderApi::lock()
{
	m_RenderLocker.lock();
	g_iLocked ++;
	return true;
}

bool xD3D10RenderApi::unlock()
{
	g_iLocked --;
	m_RenderLocker.unlock();
	return true;
}

bool xD3D10RenderApi::begin(xColor_4f& bkColor , float z , unsigned int stencil)
{
   lock();
   TAutoLocker<xD3D10Locker> aLocker(m_pDevLocker);
   return m_RenderView->clear(bkColor , z , stencil );
}

bool xD3D10RenderApi::beginScene()
{
	//XEVOL_WARNNING_NOT_IMPLEMENT
	lock();
	return true;
}

bool xD3D10RenderApi::endScene()
{
	//XEVOL_WARNNING_NOT_IMPLEMENT
	unlock();
	return true;
}

bool xD3D10RenderApi::swapBuffer()
{
	TAutoLocker<xD3D10Locker> aLocker(m_pDevLocker);
	m_pSwapChain->Present( 0, 0 );
	unlock();
	return true;
}

ISamplerState* xD3D10RenderApi::createSamplerState(const wchar_t* name)
{
	std::wstring full_name = name + std::wstring(L".sampler");
	HRenderState hState = this->m_StateObjMgr.add(full_name.c_str() , (int)(eRenderState_Sampler) , true);
	return (ISamplerState*) hState.getResource();
}

IBlenderState* xD3D10RenderApi::createBlendState(const wchar_t* name)
{
	std::wstring full_name = name + std::wstring(L".blend");
	HRenderState hState = this->m_StateObjMgr.add(full_name.c_str() , (int)(eRenderState_Blend) , true);
	return (IBlenderState*) hState.getResource();
}

IRasterizerState* xD3D10RenderApi::createRasterizerState(const wchar_t* name)
{
	std::wstring full_name = name + std::wstring(L".ras");
	HRenderState hState = this->m_StateObjMgr.add(full_name.c_str() , (int)(eRenderState_Rasterizer) , true);
	return (IRasterizerState*) hState.getResource();
}

IDepthStencilState*  xD3D10RenderApi::createDepthStencilState(const wchar_t* name)
{
	std::wstring full_name = name + std::wstring(L".zs");
	HRenderState hState = this->m_StateObjMgr.add(full_name.c_str() , (int)(eRenderState_DepthStencil) , true);
	return (IDepthStencilState*) hState.getResource();
}

IBlenderState*  xD3D10RenderApi::getBlendState()
{
   return m_BlenderState;
}

IRasterizerState* xD3D10RenderApi::getRasterizerState()
{
   return m_RasterizerState;
}

IDepthStencilState*  xD3D10RenderApi::getDepthStencilState()
{
   return m_DepthStencilState;
}

bool xD3D10RenderApi::setBlendState(IBlenderState* pState)
{

	GUID_SELECT_MODE(false);
	if(pState == NULL)
	{
		d10Device()->OMSetBlendState(m_defBlendState , m_DefBlendFactor , m_DefSampleMask );
		return true;
	}
    ID3D10BlendState* pBlendState = (ID3D10BlendState*)pState->handle();
	d10Device()->OMSetBlendState(pBlendState , pState->factor() , pState->samplemask() );
	m_BlenderState      = pState;
	return true;
}

bool xD3D10RenderApi::setRasterizerState(IRasterizerState* pState)
{
	if(pState == NULL)
	{
        d10Device()->RSSetState( m_defpRasState );
		return true;
	}
	ID3D10RasterizerState* pRasState = (ID3D10RasterizerState*)pState->handle();
	d10Device()->RSSetState(pRasState );
	m_RasterizerState   = pState; 
	return true;
}


bool xD3D10RenderApi::setDepthStencilState(IDepthStencilState* pState)
{
	if(pState == NULL)
	{
		d10Device()->OMSetDepthStencilState( m_defDepthStencilState , m_DefStencilRef );
		return true;
	}
	ID3D10DepthStencilState* pZSState = (ID3D10DepthStencilState*)pState->handle();
	d10Device()->OMSetDepthStencilState( pZSState , pState->stencil_ref() );
	m_DepthStencilState = pState;
	return true;
}

IRenderState* xD3D10RenderApi::createRenderState(eRenderStateType _type)
{
    switch(_type)
	{
	case eRenderState_Sampler:
		return new xD10Sampler(this);
	case eRenderState_Blend:
		return new xD10BlendState(this);
		return NULL;
	case eRenderState_Rasterizer:
		return new xD10RazState(this);
		return NULL;
	case eRenderState_DepthStencil:
		return new xD10ZStencilState(this);
	}
	return NULL;
}

IInputAssembler* xD3D10RenderApi::getInputAssembler(const wchar_t* name)
{
	//First create a assembler
	xD10InputAssembler* assembler = objectManager().inputAssemblerMgr().find(name);
	if(assembler) 
	{
		assembler->AddRef();
		return assembler ;
	}
	return NULL;
}


IInputAssembler* xD3D10RenderApi::createInputAssembler(const wchar_t* name , xInputLayoutDesc& desc )
{
	//First create a assembler
	xD10InputAssembler* assembler = objectManager().inputAssemblerMgr().find(name);
	if(assembler) 
	{
		assembler->AddRef();
		return assembler ;
	}
    
	//Not found
	assembler = new xD10InputAssembler(this , name , desc);
	if( assembler->create() )
	{
        objectManager().inputAssemblerMgr().add(name , assembler);
		return assembler;
	}
	assembler->KillObject();
	return NULL;
}

IInputBuffer*    xD3D10RenderApi::createInputBuffer(size_t nVertex , size_t nBytePerVertex , const xInputBufferDesc* pBufferDesc , void* pData)
{
	xD10InputBuffer* vBuffer = new xD10InputBuffer(this);

	bool ret  = vBuffer->create(nVertex ,  nBytePerVertex , pBufferDesc , pData) ;
	if(ret == false )
	{
		XSAFE_KILLOBJECT( vBuffer );
		return NULL;
	}
	return vBuffer;
}

IInputBuffer*  xD3D10RenderApi::createBuffer(size_t iBufLen ,const xInputBufferDesc* pBufferDesc , void* pData)
{
	xD10InputBuffer* vBuffer = new xD10InputBuffer(this);

	bool ret  = vBuffer->create(iBufLen , pBufferDesc , pData) ;
	if(ret == false )
	{
		XSAFE_KILLOBJECT( vBuffer );
		return NULL;
	}
	return vBuffer;
}

IInputBuffer* xD3D10RenderApi::findInputBuffer(const wchar_t* _name)
{
     IInputBuffer* pBuffer = m_objManager.InputBufferMgr().find(_name);
	 return pBuffer;
}

bool xD3D10RenderApi::setInputBuffer(const wchar_t* _name, IInputBuffer* pConstBuffer)
{
     m_objManager.InputBufferMgr().add(xStringHash(_name) , pConstBuffer );
	 return true;
}

IInputBuffer* xD3D10RenderApi::createInputBuffer(size_t iBufLen)
{
	xInputBufferDesc cbDesc;
	cbDesc.m_bindtype    = BIND_AS_SHADER_RESOURCE ;
	cbDesc.m_accessFlage = RESOURCE_ACCESS_WRITE;
	cbDesc.m_usage       = RESOURCE_USAGE_DYNAMIC;
	return createBuffer(iBufLen , &cbDesc, NULL);
}

IInputBuffer* xD3D10RenderApi::createConstBuffer(size_t iBufLen)
{
	xInputBufferDesc cbDesc;
	cbDesc.m_bindtype    = BIND_AS_CONSTANT_BUFFER ;
	cbDesc.m_accessFlage = RESOURCE_ACCESS_WRITE;
	cbDesc.m_usage       = RESOURCE_USAGE_DYNAMIC;
	return createBuffer(iBufLen , &cbDesc, NULL);
}

bool xD3D10RenderApi::setPrimitiveType(ePrimtiveType type)
{
	m_PrimType = type;
	D3D10_PRIMITIVE_TOPOLOGY d3d10PtLogicy = (D3D10_PRIMITIVE_TOPOLOGY)m_PrimType;
    m_pD3D10Device->IASetPrimitiveTopology( d3d10PtLogicy );
	return false;
}

ePrimtiveType xD3D10RenderApi::getPrimitiveType()
{
	D3D10_PRIMITIVE_TOPOLOGY type;
	m_pD3D10Device->IAGetPrimitiveTopology( &type );
	return (ePrimtiveType)type;
}

bool xD3D10RenderApi::setInputAssembler(IInputAssembler* pAssembler)
{
    m_pInputAssembler = pAssembler;
	xD10InputAssembler* assembler = dynamic_cast<xD10InputAssembler*>(pAssembler);
	ID3D10InputLayout* pILayout = assembler->d10Layout();
	m_pD3D10Device->IASetInputLayout( pILayout );
	return true;
}

IInputAssembler*  xD3D10RenderApi::getInputAssembler()
{
	return m_pInputAssembler;
}

bool xD3D10RenderApi::setIndexBuffer(IInputBuffer* pVertexBuffer , size_t iOffset)
{
	xD10InputBuffer* d10IB = dynamic_cast<xD10InputBuffer*>(pVertexBuffer);
	UINT Offset  = (UINT)iOffset;
	UINT uBPB = (UINT)d10IB->m_nBytePerVertex;	
	DXGI_FORMAT df = (uBPB == 2 ? DXGI_FORMAT_R16_UINT  : DXGI_FORMAT_R32_UINT );
	m_pD3D10Device->IASetIndexBuffer(d10IB->m_pBuffer , df , Offset);
	return true;
}

bool xD3D10RenderApi::setVertexBuffer(IInputBuffer* pVertexBuffer ,   size_t bufIdx , size_t stride , size_t iOffset )
{
	xD10InputBuffer* d10IB = dynamic_cast<xD10InputBuffer*>(pVertexBuffer);
	UINT Stride  = (UINT)stride;
	UINT Offset  = (UINT)iOffset;
	if(stride == 0 ) Stride = (UINT)d10IB->m_nBytePerVertex;	
	m_pD3D10Device->IASetVertexBuffers((UINT)bufIdx , 1 , &d10IB->m_pBuffer , &Stride , &Offset );
	return true;
}

bool xD3D10RenderApi::setVertexStream(IVertexStream* vertexStream)
{
	xD10VertexStream* d10VertStream = dynamic_cast<xD10VertexStream*>(vertexStream);
	size_t nInputSlot = d10VertStream->nInputSlot();
	const xInputLayoutDesc& desc = d10VertStream->getAssembler()->desc();
	IInputAssembler* pAssembler = d10VertStream->getAssembler();
	setInputAssembler(pAssembler);

	for(size_t i = 0 ; i < nInputSlot ; i ++)
	{
		IInputBuffer*    pIB = d10VertStream->getInputBuffer( i );
		setVertexBuffer(pIB , i , 0 , 0 );
		
	}
	m_pVertStream = d10VertStream;
	return false;
}

bool xD3D10RenderApi::beginDrawPrimitive()
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
bool xD3D10RenderApi::endDrawPrimitive()
{
	if(m_RenderMode == eRenderMode_Select && m_pColorSelector)
	{
		m_pColorSelector->endPrimitive();
	}
	return true;
}

bool xD3D10RenderApi::drawPrimitive(size_t nVertex , size_t iStartVertex , ePrimtiveType pt)
{
	setPrimitiveType(pt);
	m_pD3D10Device->DrawIndexed( (UINT)nVertex , (UINT)iStartVertex , 0 );
	return true;
}

bool xD3D10RenderApi::draw(IInputBuffer* pIdxBuffer , size_t nVertex , size_t iStartVertex, ePrimtiveType pt)
{
	TAutoLocker<xD3D10Locker> aLocker(m_pDevLocker);
	setIndexBuffer(pIdxBuffer);

	if(false == beginDrawPrimitive() ) 
		return false;

	bool bDrawRet = drawPrimitive(nVertex , iStartVertex , pt);

	bool bRet = endDrawPrimitive();

	return bDrawRet&bRet;
}

bool xD3D10RenderApi::draw(size_t nVertex , size_t iStartVertex, ePrimtiveType pt)
{
	TAutoLocker<xD3D10Locker> aLocker(m_pDevLocker);
    if(false == beginDrawPrimitive() ) return false;

	bool bDrawRet = drawPrimitive(nVertex , iStartVertex , pt);
	bool bRet = endDrawPrimitive();

	return bDrawRet&bRet;
}

bool xD3D10RenderApi::set2DZValue(float zValue)
{
    m_2DZValue = zValue;
	return true;
}

float xD3D10RenderApi::get2DZValue()
{
    return m_2DZValue;
}

bool  xD3D10RenderApi::drawRectf(IBaseTexture* pTexture, float vDestRect[4] , xColor_4f& color)
{
	setTexture(0 , pTexture);
	xvec4& dstRect =  *((xvec4*)vDestRect);
	m_pDef2DRect->setColor(color);
	m_pDef2DRect->setTexture(0,pTexture);
	m_pDef2DRect->setRect(dstRect);
	m_pDef2DRect->commit();
	m_pDef2DRect->draw();
	return true;
}

bool xD3D10RenderApi::drawRectf(IBaseTexture* pTexture, float vDestRect[4] ,float vSrcRect[4]  , xColor_4f& color)
{
     setTexture(0 , pTexture);
	 xvec4& dstRect =  *((xvec4*)vDestRect);
	 xvec4& srcRect =  *((xvec4*)vSrcRect);
	 m_pDef2DRect->setTexture(0,pTexture,srcRect);
	 m_pDef2DRect->setRect(dstRect);
	 m_pDef2DRect->commit();
	 m_pDef2DRect->draw();
	 return true;
}

void xD3D10RenderApi::set2DTexture(int iStage , IBaseTexture* pTexture)
{
	m_pDef2DRect->setTexture(iStage , pTexture);
}

void xD3D10RenderApi::set2DTexture(int iStage , IBaseTexture* pTexture , float _srcRect[4])
{
	xvec4& srcRect =  *((xvec4*)_srcRect);;
    m_pDef2DRect->setTexture(iStage , pTexture, srcRect);
}

I2DRectObject* xD3D10RenderApi::create2DRectObject()
{
     return new xD102DRect(m_pDef2DRectEnv);
}

bool xD3D10RenderApi::draw2DRect(I2DRectObject* p2DRect)
{
	p2DRect->commit();
	return p2DRect->draw();
}

IGpuProgram* xD3D10RenderApi::createGpuProgram()
{
	return new xD10GpuProgram(this);
}

IBaseShader* xD3D10RenderApi::createShader(const wchar_t* shaderName , eShaderType type)
{
	HBaseShader hShader = m_ShaderManager.add(shaderName,(unsigned int)type , true );
	hShader.lock();
	return hShader.getResource();
}


IBaseShader* xD3D10RenderApi::createShader(eShaderType type)
{
	if(type == eShader_VertexShader)
	{
		return new xD10VertexShader(this);
	}

	if(type == eShader_PixelShader)
	{
		return new xD10PixelShader(this);
	}

	if(type == eShader_GeometryShader)
	{
		return new xD10GeomShader(this);
	}

	return NULL;
}

bool xD3D10RenderApi::pushGpuProgram(IGpuProgram* pGpuProgram)
{
	xD10GpuProgram* pGpuProg = (xD10GpuProgram*)pGpuProgram;
	m_pGpuProgram.push();
	setGpuProgram(pGpuProgram);
	return true;
}

bool xD3D10RenderApi::popGpuProgram()
{
    xD10GpuProgram* pGpuProg = m_pGpuProgram.pop();
	if( pGpuProg ==  NULL)
		return false;
	setGpuProgram(pGpuProg);
	return true;
}

IGpuProgram* xD3D10RenderApi::getGpuProgram( )
{
	return m_pGpuProgram;
}

bool xD3D10RenderApi::setGpuProgram(IGpuProgram* pGpuProgram)
{
	if(m_pGpuProgram == pGpuProgram)
		return false;

	m_pGpuProgram = (xD10GpuProgram*)pGpuProgram;
	if(m_pGpuProgram)
	{
		setVertexShader(m_pGpuProgram->m_pVertexShader);
		setPixelShader(m_pGpuProgram->m_pPixelShader);
		setGeomShader(m_pGpuProgram->m_pGeomShader);
	}
	return true;
}

bool xD3D10RenderApi::setVertexShader(IBaseShader* pVertShader)
{
	if( pVertShader == NULL || pVertShader->isLoaded() == false)
	{
		m_pD3D10Device->VSSetShader( NULL );
		return true;
	}
#ifdef _DEBUG
	xD10VertexShader* pVS = dynamic_cast<xD10VertexShader*> (pVertShader);
#else
	xD10VertexShader* pVS = (xD10VertexShader*) pVertShader;
#endif
	m_pD3D10Device->VSSetShader( pVS->m_pVertexShader );
	return true;
}

bool xD3D10RenderApi::setSamplerState(eShaderType _st , int iSlot , ISamplerState* pSampler)
{
	if(pSampler == NULL)
		return false;

   ID3D10SamplerState * pD10Sampler = (ID3D10SamplerState *)pSampler->handle();
   switch(_st)
   {
   case eShader_VertexShader:
	   d10Device()->VSSetSamplers(iSlot , 1 , &pD10Sampler);
	  return true;
	   break;
   case eShader_PixelShader:
	   d10Device()->PSSetSamplers(iSlot , 1 , &pD10Sampler);
	   return true;
	   break;
   case eShader_GeometryShader:
	   d10Device()->GSSetSamplers(iSlot , 1 , &pD10Sampler);
	   return true;
	   break;
   }
   return false;
}

bool xD3D10RenderApi::setPixelShader(IBaseShader* pPixelShader)
{
	if(pPixelShader == NULL || pPixelShader->isLoaded() == false)
	{
		m_pD3D10Device->PSSetShader( NULL );
		return true;
	}
#ifdef _DEBUG
	xD10PixelShader* pPS = dynamic_cast<xD10PixelShader*> (pPixelShader);
#else
	xD10PixelShader* pPS = (xD10PixelShader*) pPixelShader;
#endif
	m_pD3D10Device->PSSetShader( pPS->m_pPixelShader );
	return true;
}

bool xD3D10RenderApi::setGeomShader(IBaseShader* pGeomShader)
{
	if(pGeomShader == NULL || pGeomShader->isLoaded() == false)
	{
		m_pD3D10Device->GSSetShader( NULL );
		return true;
	}
#ifdef _DEBUG
	xD10GeomShader* pGS = dynamic_cast<xD10GeomShader*> (pGeomShader);
#else
	xD10GeomShader* pGS = (xD10GeomShader*) pGeomShader;
#endif
	m_pD3D10Device->GSSetShader( pGS->m_pGeomShader );
	return true;
}

bool  xD3D10RenderApi::setShaderResource(eShaderType _st , int iSlot , IBaseTexture* pTexture)
{
	ID3D10ShaderResourceView* pResourceView = (ID3D10ShaderResourceView*)pTexture->handle();
	if(pResourceView == NULL)
		return false;

	HRESULT hRet = E_FAIL;
    switch(_st)
	{
	case eShader_VertexShader:
		d10Device()->VSSetShaderResources(iSlot , 1 , &pResourceView );
		return true;
		break;
	case eShader_PixelShader:
		d10Device()->PSSetShaderResources(iSlot , 1 , &pResourceView );
		return true;
		break;
	case eShader_GeometryShader:
		d10Device()->GSSetShaderResources(iSlot , 1 , &pResourceView );
		return true;
		break;
	}
	return SUCCEEDED(hRet);
}



bool  xD3D10RenderApi::setTexture(const int iTexName      , IBaseTexture* pTexture)
{
	if(iTexName < Texture_StageMax)
	{
		m_texLayer[iTexName] = pTexture;
		return true;
	}
	m_mapTexLayers[iTexName] = pTexture;
	return true;
}

bool  xD3D10RenderApi::setTexture(const wchar_t* iTexName , IBaseTexture* pTexture)
{
	if(iTexName == NULL)
		return false ;

     int iName = xStringHash(iTexName);

	 if(iName < Texture_StageMax)
		 return false;

	 m_mapTexLayers[iName] = pTexture;
	 return true;
}

IBaseTexture*  xD3D10RenderApi::getTexture(const int iTexName)
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

IBaseTexture*  xD3D10RenderApi::getTexture(const wchar_t* iTexName)
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

bool  xD3D10RenderApi::clearAllTexture()
{
	for(int i = 0 ; i < Texture_StageMax ; i ++)
	{
		m_texLayer[i] = NULL;
	}
	m_mapTexLayers.clear();
	return true;
}
bool  xD3D10RenderApi::setShaderResource(eShaderType _st , int iSlot ,IInputBuffer* pBuffer)
{
     return false;
}

bool xD3D10RenderApi::isTextureSupport(ePIXEL_FORMAT fmt , bool lockable)
{
	switch(fmt)
	{
	case  PIXELFORMAT_R8G8B8   :
	case  PIXELFORMAT_R8G8B8X8 :
	case  PIXELFORMAT_B8G8R8   :
	case  PIXELFORMAT_B8G8R8X8 :

	//Depth												   	
	case  PIXELFORMAT_DEPTH16  :
	case  PIXELFORMAT_DEPTH24  :
	case  PIXELFORMAT_DEPTH32  :

	//不可以操作的
	case  PIXELFORMAT_DXT1  :
	case  PIXELFORMAT_DXT2  :
	case  PIXELFORMAT_DXT3  :
	case  PIXELFORMAT_DXT4  :
	case  PIXELFORMAT_DXT5  :
		return false;
	default:
		return true;
	}
	return true;
}

IShaderConstBinder*  xD3D10RenderApi::getShaderConstBinder(const wchar_t* constName)
{
	int hashKey = xStringHash2(constName);
	mapConstBinder::iterator pos  = m_constNameBinder.find(hashKey);
	if(pos != m_constNameBinder.end() )
	{
		return pos->second;
	}
	return NULL;
}

void xD3D10RenderApi::registeShaderConstBinder(const wchar_t* constName , IShaderConstBinder* pBinder)
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

IShaderConstBinder* xD3D10RenderApi::getShaderConstBinder(eShaderParamSemantic _semantic)
{
	mapConstBinder::iterator pos  = m_semanticBinder.find( (int)_semantic);
	if(pos != m_semanticBinder.end() )
	{
		return pos->second;
	}
	return NULL;
}

void xD3D10RenderApi::registeShaderConstBinder(eShaderParamSemantic _semantic , IShaderConstBinder* pBinder)
{
	if(getShaderConstBinder(_semantic) != NULL)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH,L"你注册的Shader绑定器已经存在! name=%s\n" , _semantic);
		return ;
	}
	m_semanticBinder.insert(mapConstBinder::value_type( (int)_semantic , pBinder) ) ;
	return ;
}

IBaseTexture* xD3D10RenderApi::createFileTexture(const wchar_t* texFile , const unsigned int8* buf , unsigned int bufLen, unsigned int arg)
{
	xD10FileTexture* pTex = new xD10FileTexture(this);
	if(pTex->load(texFile , buf , bufLen , arg ) )
	{
		return pTex;
	}
	pTex->KillObject();
	return NULL;
}

IBaseTexture* xD3D10RenderApi::createFileTexture(const wchar_t* extFile)
{
	return new xD10FileTexture(this);
}

IBaseTexture* xD3D10RenderApi::createLockableTexture(int w , int h , int depth , ePIXEL_FORMAT fmt , bool bReadable , int nMipMap, int nArraySize)
{
     xD10LockableTexture* pTexture = new xD10LockableTexture(this , bReadable);
	 if(pTexture->create(w , h , depth , fmt , nMipMap , nArraySize) )
	 {
		 return pTexture;
	 }
	 return NULL;	
}

xBaseTextureMgr* xD3D10RenderApi::createTextureManager(const wchar_t* _name , bool nonRefKeep)
{
  xBaseTextureMgr* pMgr = new xBaseTextureMgr(_name , nonRefKeep ? 1 : 0 );
  pMgr->setRenderApi(this);
  m_TextureMagrs.push_back(pMgr);
  return pMgr;
}

void xD3D10RenderApi::deleteTextureManager(xBaseTextureMgr* pTextureMgr)
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

void xD3D10RenderApi::freeAllTextureManager()
{
	for(size_t i = 0 ; i < m_TextureMagrs.size() ; i ++)
	{
		m_TextureMagrs[i]->clear();
		XSAFE_DELETE(m_TextureMagrs[i]);
	}
	m_TextureMagrs.clear();
}

void xD3D10RenderApi::unloadAllTextureManager()
{
	for(size_t i = 0 ; i < m_TextureMagrs.size() ; i ++)
	{
		m_TextureMagrs[i]->unload();
	}
}

bool xD3D10RenderApi::enter2DMode(int x , int y , int w , int h , bool bTextMode)
{
	m_MatrixCtx.pushMatrix(MATRIXMODE_Project);
	m_MatrixCtx.pushMatrix(MATRIXMODE_View);
	if( w ==  0 || h == 0)
	{
		xTextureDesc _desc;
		getRenderView()->desc(_desc);
		w = _desc.m_width;
		h = _desc.m_height;
	}
	D3DXMATRIX mat;
	D3DXMatrixOrthoOffCenterRH(&mat, (float)0, (float)w , (float)h , (float)0 ,-100000.0f,100000.0f);
	setMatrix( (float *)mat.m , MATRIXMODE_Project);
	identityMatrix(MATRIXMODE_View);
	return true;
}

bool xD3D10RenderApi::leave2DMode()
{
	m_MatrixCtx.popMatrix(MATRIXMODE_Project);
	m_MatrixCtx.popMatrix(MATRIXMODE_View);
	return true;
}

bool xD3D10RenderApi::multiMatrix(float* mat , eMatrixMode matMode)
{
	return m_MatrixCtx.multiMatrix( mat , matMode);
}

bool xD3D10RenderApi::setMatrix(float* mat   , eMatrixMode matMode)
{
	return m_MatrixCtx.setMatrix( mat , matMode);
}

bool xD3D10RenderApi::identityMatrix(eMatrixMode matMode)
{
	return m_MatrixCtx.identityMatrix(matMode);
}

bool xD3D10RenderApi::getMatrix(float* mat , eMatrixMode matMode)
{
	return m_MatrixCtx.getMatrix( mat , matMode);
}

bool xD3D10RenderApi::pushMatrix(eMatrixMode matMode)
{
	return m_MatrixCtx.pushMatrix(matMode );
}

bool xD3D10RenderApi::popMatrix(eMatrixMode matMode)
{
	return m_MatrixCtx.popMatrix( matMode);
}

IRenderCamera* xD3D10RenderApi::createCamera(const wchar_t* cameraName)
{
     return m_MatrixCtx.createCamera(cameraName);
}

bool xD3D10RenderApi::applyCamera()
{
	return m_MatrixCtx.applyCamera();
}

bool xD3D10RenderApi::applyCamera(IRenderCamera* pCamera)
{
	return m_MatrixCtx.applyCamera(pCamera);
}

IRenderCamera* xD3D10RenderApi::getCamera()
{
	return m_MatrixCtx.getCamera();
}

IBaseTexture *xD3D10RenderApi::createRenderableTexture(int w , int h , int depth , ePIXEL_FORMAT fmt , bool bReadable ,  int nMipMap, int nArraySize )
{
	if(depth != 1 && bReadable)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH,"3D Render To Texture cann't created as readable\n");
		bReadable = false;
	}
	xD10RenderTexture* pRenderTexture = new xD10RenderTexture(bReadable , true , this);
	if( false == pRenderTexture->create(w , h , depth , fmt) )
	{
		delete pRenderTexture;
		return NULL;
	}
	return pRenderTexture;
}

IRenderTarget* xD3D10RenderApi::createRenderTarget(int w , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture)
{
	xD10RenderTexture* pRenderTexture = new xD10RenderTexture(bLockable , bAsTexture , this);
	if( false == pRenderTexture->create(w , h , fmt) )
	{
		delete pRenderTexture;
		return NULL;
	}
	IRenderTarget* pRT = pRenderTexture->toRenderTarget();
	if(pRT == NULL)
	{
		delete pRenderTexture;
		return NULL;
	}
	return pRT;
}

IRenderTarget* xD3D10RenderApi::createDepthBuffer(int w  , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture)
{
	xD10DepthTexture* pDepthTexture = new xD10DepthTexture(bLockable , bAsTexture , this);
	if( false == pDepthTexture->create(w , h , fmt) )
	{
		delete pDepthTexture;
		return NULL;
	}
	IRenderTarget* pRT = pDepthTexture->toRenderTarget();
	if(pRT == NULL)
	{
		delete pDepthTexture;
		return NULL;
	}
	return pRT;
}

IRenderTarget*  xD3D10RenderApi::DefDepthBuffer()
{
	if(m_RenderWindow == NULL) return NULL;
    return m_RenderWindow->depthBuffer();
}

ID3D10DepthStencilView*  xD3D10RenderApi::DefDepthStencilView()
{
	if(m_RenderWindow == NULL) return NULL;
     return m_RenderWindow->GetDepthStencilView();
}

IRenderView* xD3D10RenderApi::createRenderView(int w , int h , bool bCreateDepth )
{
	xD10RenderView* pRenderView = new xD10RenderView(this);
	if(w == 0 || h == 0)
	{
		RECT rect;
		GetClientRect(m_hMainWnd , &rect);
		w = rect.right - rect.left;
		h = rect.bottom - rect.top;
	}
	if(bCreateDepth) 
		pRenderView->createDepthView(w , h);

	return pRenderView;
}

IRenderView* xD3D10RenderApi::createRenderView(int w , int h ,void* hWnd ,bool bCreateDepth )
{
	return NULL;
}
IRenderView* xD3D10RenderApi::getRenderView()
{
	return m_RenderView;
}

bool xD3D10RenderApi::setRenderView(IRenderView* renderView)
{
	GUID_SELECT_MODE(false);
	xD10RenderView* pD10RV = (xD10RenderView*) renderView;
	m_RenderView = pD10RV;
	return pD10RV->install();
}

bool xD3D10RenderApi::pushRenderView(IRenderView* renderView)
{
	GUID_SELECT_MODE(false);
	xD10RenderView* pD10RV = (xD10RenderView*) renderView;
	m_RenderView.push(  );
	setRenderView(renderView);
	return false;
}

bool xD3D10RenderApi::popRenderView()
{
	GUID_SELECT_MODE(false);
	xD10RenderView* renderView = m_RenderView.pop();
	if(renderView == NULL)
		return false;
	setRenderView(renderView);
	return true;
}
END_NAMESPACE_XEVOL3D