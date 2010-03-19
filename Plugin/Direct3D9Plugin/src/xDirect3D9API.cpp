#include "stdafx.h"
#include <BaseLib/xEvol3DAPI.h>
#include <BaseLib/xXmlDocument.h>
#include <OperationSys/xOperationSys.h>
#include "xDIRECT3D9API.h"
#include "Image/PixelFormatAssit.h"
#include "fs/xFileSystem.h"
#include "xD3DTLEnviroment.h"
//#include "../xDIRECT3D9DllApi.h"

#include "xD3DInputBuffer.h"
#include "xD3D9Sampler.h"
#include "xD3D9RazState.h"
#include "xD3D9DepthStencilState.h"
#include "xD3D9BlendState.h"
#include "xD3D9FileTexture.h"
#include "xD3D9RenderTarget.h"
#include "xD3D9Texture2D.h"
#include "xD3D9Texture3D.h"
#include "xD3D9TextureCube.h"
#include "xD3D9InputBuffer.h"
#include "xD3D9InputAssembler.h"

#include "xD3D9RenderView.h"
#include "xD3D9Shader.h"
#include "xD3D9GPUProgram.h"
#include "xD3DBaseColorSelector.h"

BEGIN_NAMESPACE_XEVOL3D

#define GUID_SELECT_MODE(_value)  if( m_RenderMode == eRenderMode_Select ) return _value;

IRenderApi::eFeatureLevel xD3D9RenderApi::featureLevel() 
{ 
    D3DCAPS9 Caps;
    getDevice()->GetDeviceCaps(&Caps);
    int MaxVertexShaderConst = Caps.MaxVertexShaderConst;
    int VsVersion    = (LOWORD(Caps.VertexShaderVersion) & 0xff00) >> 8;
    int PsVersion    = (LOWORD(Caps.PixelShaderVersion ) & 0xff00) >> 8;
    int VsSubVersion = LOWORD(Caps.VertexShaderVersion)  & 0x00ff;
    int PsSubVersion = LOWORD(Caps.PixelShaderVersion )  & 0x00ff;


    if(VsVersion >= 3 && PsVersion >= 3 && MaxVertexShaderConst >= 248)
        return eFeature_ShaderModel3;
    if(VsVersion >= 2 && PsVersion >= 2 )
        return eFeature_ShaderModel2;

    return eFeature_ShaderModel1;
}

const char* xD3D9RenderApi::getShaderProfile(eShaderType _type)
{
    D3DCAPS9 Caps;
    getDevice()->GetDeviceCaps(&Caps);

    int VsVersion    = (LOWORD(Caps.VertexShaderVersion) & 0xff00) >> 8;
    int PsVersion    = (LOWORD(Caps.PixelShaderVersion ) & 0xff00) >> 8;
    int VsSubVersion = LOWORD(Caps.VertexShaderVersion)  & 0x00ff;
    int PsSubVersion = LOWORD(Caps.PixelShaderVersion )  & 0x00ff;

    static char buf[16] = {0};
    if(_type == eShader_VertexShader)
    {
        sprintf(buf , "vs_%d_%d" , VsVersion , VsSubVersion);
        return buf;
    }
    else if(_type == eShader_PixelShader)
    {
        sprintf(buf , "ps_%d_%d" , PsVersion , PsSubVersion);
        return buf;
    }
	return NULL;
}

void xD3D9RenderApi::getRTSampleDesc(xRTSampleDesc & rtSample)
{
    rtSample = xRTSampleDesc::None;
}

int xD3D9RenderApi::intCapsValue(const wchar_t* cfgName , int defValue)
{
    xXmlValue* pValue = m_RenderCaps.findValue(cfgName);
    if(pValue == NULL) return defValue;
    return pValue->int_value();
}

xD3D9RenderApi::xD3D9RenderApi(IDirect3DDevice9* pDevice , HWND hWnd , int w , int h)
{
    m_RenderCaps.setValue(L"MaxGpuBone" , 72 );

	m_PrimType   = ePrimtiveType_Triangles;
	m_pGpuProgram = NULL;
	m_pInputAssembler = NULL;
	m_pVertStream  = NULL;
	//设备属性和参数
	m_hMainWnd = NULL;
	m_hInst    = NULL;

	//m_pDef2DRect    = NULL;
	//m_pDef2DRectEnv  = NULL;
	m_pD3DDevice        = pDevice;

    m_hMainWnd          = hWnd;
	m_Width             = w;
	m_Height            = h;
    m_DebugDevice       = false;
    m_pDirect3D         = NULL;
    m_NumVerticesInVB   = 0;
}

xD3D9RenderApi::~xD3D9RenderApi()
{
	uninit();
}

IDirect3DDevice9*  xD3D9RenderApi::getDevice()
{
	return m_pD3DDevice ; 
}

bool xD3D9RenderApi::uninit( )
{
    __unloadAllResource();
	XSAFE_DELETE(m_RenderWindow);
	XSAFE_RELEASE(m_pD3DDevice);
    XSAFE_RELEASE(m_pDirect3D);
	return xRenderApiBase::uninit();
}

bool xD3D9RenderApi::init(xXmlNode* pSysNode)
{
	xRTSampleDesc sampleDes;
	xXmlNode* pRendererNode = pSysNode->findNode( name() , 0 );
    sampleDes.m_SampleCount   = 1 ;
    sampleDes.m_SampleQulity  = 0 ;
    m_DebugDevice = false;
    if(pRendererNode)
    {
        sampleDes.m_SampleCount   = pRendererNode->value(L"Sample.Count")   ? pRendererNode->int_value(L"Sample.Count") : 1;
        sampleDes.m_SampleQulity  = pRendererNode->value(L"Sample.Quality") ? pRendererNode->int_value(L"Sample.Quality") : 0;
        m_DebugDevice             = pRendererNode->bool_value(L"DebugDevice");
    }

    m_vSync = pSysNode->bool_value(L"vSync");
	if(false == create() )
		return false;

	if(false ==  xRenderApiBase::init(pSysNode) )
		return false;
	
	//现在来初始化选择Buffer;

	xXmlNode* pSelNode = pRendererNode->findNode(L"PixelSelect");//  pSysNode->bool_value(L"pixel_select");
	if(pSelNode && pSelNode->bool_value(L"enable"))
	{
		xD3DBaseColorSelector* pD10Selector = new xD3DBaseColorSelector(this);
		pD10Selector->create(pSelNode ) ;
		m_pBaseSelector = pD10Selector;
		m_pColorSelector = m_pBaseSelector;
	}

    D3DCAPS9 Caps;
    getDevice()->GetDeviceCaps(&Caps);
    int MaxVertexShaderConst = Caps.MaxVertexShaderConst;
    int VsVersion    = (LOWORD(Caps.VertexShaderVersion) & 0xff00) >> 8;
    int PsVersion    = (LOWORD(Caps.PixelShaderVersion ) & 0xff00) >> 8;
    int VsSubVersion = LOWORD(Caps.VertexShaderVersion)  & 0x00ff;
    int PsSubVersion = LOWORD(Caps.PixelShaderVersion )  & 0x00ff;

    m_MaxGpuBone = (MaxVertexShaderConst-40)/3 ;
    XEVOL_LOG(eXL_DEBUG_HIGH , "-------------------------------------------------\n");
    XEVOL_LOG(eXL_DEBUG_HIGH , "       >>Chooseing DX9 RenderApi\n");
    XEVOL_LOG(eXL_DEBUG_HIGH , "       >>Shader Model = %d_%d\n"          , PsVersion , PsSubVersion);
    XEVOL_LOG(eXL_DEBUG_HIGH , "       >>Support Max Gpu Bone count=%d\n" , m_MaxGpuBone );
    XEVOL_LOG(eXL_DEBUG_HIGH , "       >>VSync=%s\n" , m_vSync?"On":"Off" );
    XEVOL_LOG(eXL_DEBUG_HIGH , "-------------------------------------------------\n");
    
	return true;
}


bool xD3D9RenderApi::create()
{
	TAutoLocker<xRenderApiLocker> aLocker(m_pDevLocker);
	//MessageBoxA(NULL,"开始创建D3D10 Api","错误",MB_OK);
	bool ret = false;
	if(m_pD3DDevice) 
	{ 
		ret = __initByDevice();
	}
	else
	{
		ret = __createDevice();
	}
	//MessageBoxA(NULL,"创建D3D11 Api完成","错误",MB_OK);
	if(ret == false)
	{
		XEVOL_LOG(eXL_ERROR_FALT,"Create Direct3D 11 Failed\n");
	}

	__afterCreateDevice();
	enableThreadLocker(true);
	return ret;
}

bool xD3D9RenderApi::onResize(int width , int height)
{
   // return false;

	TAutoLocker<xRenderApiLocker> aLocker(m_pDevLocker);
	TAutoLocker<xRenderApiLocker> aRenderLocker(&m_RenderLocker);
	xD3D9RenderView* pRenderView = m_RenderView.type_case<xD3D9RenderView*>(); //dynamic_cast<xD3D9RenderView*>(m_RenderView.operator TObject*());
	if(false == __needResize(width , height) )
	{
		pRenderView->install();
		return false;
	}

    __beforeResetDevice();
	//XEVOL_WARNNING_NOT_IMPLEMENT_INFO("在删除这些对象前应该判断是不是当前的RenderTarget");
	bool ret = m_RenderWindow->resize(m_Width  , m_Height);
    __afterDeviceRest();
    __resetViewPort();
    
    if(m_pBaseSelector)
    {
        m_pBaseSelector->resize(m_Width , m_Height );
    }

    ((xD3D9ZStencilState*)m_DepthStencilState)->Install(NULL);
    ((xD3D9BlendState*)m_BlenderState)->Install(NULL);
    ((xD3D9RazState*)m_RasterizerState)->Install(NULL);
	return true;// pRenderView->install();
}

bool xD3D9RenderApi::__needResize(int width , int height)
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
	if(false == m_RenderWindow->NeedResize(width , height))
	{
		return false;
	}
	m_Width  = width;
	m_Height = height;
	return  true;
}

bool xD3D9RenderApi::__resetViewPort()
{
	// Setup the viewport
	D3DVIEWPORT9 vp;
    vp.Width  = (DWORD)m_Width;
    vp.Height = (DWORD)m_Height;
    vp.MinZ = 0.0f;
    vp.MaxZ = 1.0f;
    vp.X = 0;
    vp.Y = 0;
    getDevice()->SetViewport(&vp );
	return true;
}

bool xD3D9RenderApi::__createDevice()
{
    if(m_pDirect3D == NULL)
    {
        m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
    }
    if(NULL == m_pDirect3D)
        return false;

    RECT rect ;
    GetClientRect(m_hMainWnd , &rect);
    int w = m_Width;
    int h = m_Height;

    if(w == 0 || h == 0)
    {
        h  = rect.bottom; GetSystemMetrics(SM_CYSCREEN);   // rect.bottom;
        w  = rect.right ; GetSystemMetrics(SM_CXSCREEN);   // rect.right ;
    }

    if(w == 0 || h == 0)
    {
        h  = w = 20;
    }

    D3DFORMAT DepthStencilFormat = D3DFMT_D24S8;

    m_hDevAdapter       = D3DADAPTER_DEFAULT;
    m_hDevDeviceType    = D3DDEVTYPE_HAL;
    m_hDevAdapterFormat = D3DFMT_X8R8G8B8;

    D3DDISPLAYMODE d3dm;
    if(FAILED(m_pDirect3D->GetAdapterDisplayMode(m_hDevAdapter,&d3dm) ))
    {
        return false;
    }

    //Now Check the depth - Stencil buffer format
    HRESULT hr = m_pDirect3D->CheckDeviceFormat( m_hDevAdapter,    m_hDevDeviceType,      m_hDevAdapterFormat,     D3DUSAGE_DEPTHSTENCIL,    D3DRTYPE_SURFACE,  DepthStencilFormat);

    if(FAILED(hr))
        return false;

    hr = m_pDirect3D->CheckDepthStencilMatch(m_hDevAdapter, m_hDevDeviceType, m_hDevAdapterFormat, m_hDevAdapterFormat, DepthStencilFormat) ;


    if( !SUCCEEDED(hr))
        return false;

    //Now prepaire to create the D3D Device;

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.BackBufferCount = 1;

    d3dpp.hDeviceWindow    = m_hMainWnd;
    d3dpp.BackBufferWidth  = w;
    d3dpp.BackBufferHeight = h;
    d3dpp.BackBufferFormat = m_hDevAdapterFormat;
    d3dpp.AutoDepthStencilFormat  = DepthStencilFormat;
    d3dpp.EnableAutoDepthStencil  = TRUE;
    d3dpp.Windowed = true;
    d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
    d3dpp.PresentationInterval = m_vSync?D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
    //All parameter is filled Check the device type first
    //Now I can create the device
    HWND hPWnd = GetParent(m_hMainWnd);
    //SetParent(hWnd,NULL);
    if(FAILED(m_pDirect3D->CreateDevice(m_hDevAdapter,m_hDevDeviceType, m_hMainWnd , D3DCREATE_PUREDEVICE | D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED ,&d3dpp,&m_pD3DDevice) ) )
    {
        //SetParent(hWnd,hPWnd);
        return false;
    }

    m_RenderWindow = new xD3D9RenderWindow(0 , m_hMainWnd , this);
    m_RenderWindow->create(m_Width , m_Height);
    setRenderView(m_RenderWindow);
    __afterCreateDevice();
	return true;
}



bool xD3D9RenderApi::__initByDevice()
{
	XEVOL_WARNNING_NOT_IMPLEMENT
		return false;
}

bool xD3D9RenderApi::swapBuffer()
{
	TAutoLocker<xRenderApiLocker> aLocker(m_pDevLocker);
	m_RenderWindow->Present(NULL , NULL , NULL , NULL , 0 );
	unlock();
	return true;
}

bool xD3D9RenderApi::setBlendState(IBlenderState* pState)
{
    GUID_SELECT_MODE(false);

    xD3D9BlendState* pD9Blender = dynamic_cast<xD3D9BlendState*>(pState);
	if(pD9Blender == NULL)
	{
        pD9Blender = m_pDefBlender;
	}
	pD9Blender->Install((xD3D9BlendState*)m_BlenderState);
	m_BlenderState      = pD9Blender;
	return true;
}

bool xD3D9RenderApi::setRasterizerState(IRasterizerState* pState)
{
    xD3D9RazState* pD9RazState = dynamic_cast<xD3D9RazState*>(pState);
	if(pD9RazState == NULL)
	{
        pD9RazState = m_pDefRazState;
	}
    pD9RazState->Install( (xD3D9RazState*)m_RasterizerState );
	m_RasterizerState   = pD9RazState; 
	return true;
}


bool xD3D9RenderApi::setDepthStencilState(IDepthStencilState* pState)
{
    xD3D9ZStencilState* pD9ZStencilState = dynamic_cast<xD3D9ZStencilState*>(pState);
	if(pD9ZStencilState == NULL)
	{
		pD9ZStencilState = m_pDefZStencilState;
	}
	pD9ZStencilState->Install( (xD3D9ZStencilState*)m_DepthStencilState );
	m_DepthStencilState = pD9ZStencilState;
	return true;
}

IRenderApiState* xD3D9RenderApi::createRenderState(eRenderStateType _type)
{
	switch(_type)
	{
	case eRenderState_Sampler:
		return new xD3D9Sampler(this);
	case eRenderState_Blend:
		return new xD3D9BlendState(this);
		return NULL;
	case eRenderState_Rasterizer:
		return new xD3D9RazState(this);
		return NULL;
	case eRenderState_DepthStencil:
		return new xD3D9ZStencilState(this);
	}
	return NULL;
}

IInputAssembler* xD3D9RenderApi::createInputAssembler(const wchar_t* name , xInputLayoutDesc& desc )
{
	//First create a assembler
	xD3D9InputAssembler* assembler = (xD3D9InputAssembler*)objectManager().inputAssemblerMgr().find(name);
	if(assembler) 
	{
		assembler->AddRef();
		return assembler ;
	}

	//Not found
	assembler = new xD3D9InputAssembler(this , name , desc);
	if( assembler->create() )
	{
		objectManager().inputAssemblerMgr().add(name , assembler);
		return assembler;
	}
	assembler->KillObject();
	return NULL;
}

IInputBuffer*    xD3D9RenderApi::createInputBuffer(size_t nVertex , size_t nBytePerVertex , const xInputBufferDesc* pBufferDesc , void* pData)
{
    IInputBuffer* pInputBuffer = NULL;
    if(pBufferDesc->m_bindtype & BIND_AS_CONSTANT_BUFFER )
    {
        pInputBuffer = new xD3D9ConstantBuffer(this);
    }
    else if(pBufferDesc->m_bindtype & BIND_AS_VERTEX_BUFFER)
    {
        pInputBuffer = new xD3D9VertexBuffer(this);
    }
    else if(pBufferDesc->m_bindtype & BIND_AS_INDEX_BUFFER)
    {
        pInputBuffer = new xD3D9IndexBuffer(this);
    }

	bool ret  = pInputBuffer->create(nVertex ,  nBytePerVertex , pBufferDesc , pData) ;
	if(ret == false )
	{
		XSAFE_KILLOBJECT( pInputBuffer );
		return NULL;
	}
	return pInputBuffer;
}

IInputBuffer*  xD3D9RenderApi::createBuffer(size_t iBufLen ,const xInputBufferDesc* pBufferDesc , void* pData)
{
    IInputBuffer* pInputBuffer = NULL;
    if(pBufferDesc->m_bindtype & BIND_AS_CONSTANT_BUFFER)
    {
        pInputBuffer = new xD3D9ConstantBuffer(this);
    }
    else if(pBufferDesc->m_bindtype & BIND_AS_VERTEX_BUFFER)
    {
        pInputBuffer = new xD3D9VertexBuffer(this);
    }
    else if(pBufferDesc->m_bindtype & BIND_AS_VERTEX_BUFFER)
    {
        pInputBuffer = new xD3D9IndexBuffer(this);
    }

	bool ret  = pInputBuffer->create(iBufLen , pBufferDesc , pData) ;
	if(ret == false )
	{
		XSAFE_KILLOBJECT( pInputBuffer );
		return NULL;
	}
	return pInputBuffer;
}

bool xD3D9RenderApi::setPrimitiveType(ePrimtiveType type)
{
	m_PrimType = type;
	return true;
}

ePrimtiveType xD3D9RenderApi::getPrimitiveType()
{
	return m_PrimType;
}

bool xD3D9RenderApi::setInputAssembler(IInputAssembler* pAssembler)
{
	m_pInputAssembler = pAssembler;
	xD3D9InputAssembler* assembler = dynamic_cast<xD3D9InputAssembler*>(pAssembler);
	IDirect3DVertexDeclaration9* pVDecl = assembler->d3d9Layout();
	getDevice()->SetVertexDeclaration(pVDecl);
	return true;
}

bool xD3D9RenderApi::setIndexBuffer(IInputBuffer* pVertexBuffer , size_t iOffset)
{
	xD3D9IndexBuffer* d9IB = dynamic_cast<xD3D9IndexBuffer*>(pVertexBuffer);
	UINT Offset  = (UINT)iOffset;
	UINT uBPB = (UINT)d9IB->m_nBytePerVertex;	
	getDevice()->SetIndices( (IDirect3DIndexBuffer9*)d9IB->handle() );
	return true;
}

bool xD3D9RenderApi::setVertexBuffer(IInputBuffer* pVertexBuffer ,   size_t bufIdx , size_t stride , size_t iOffset )
{
    if(pVertexBuffer == NULL)
    {
        getDevice()->SetStreamSource((UINT)bufIdx , NULL , (UINT)iOffset , 0 );
        return true;
    }
	xD3D9VertexBuffer* d9VB = dynamic_cast<xD3D9VertexBuffer*>(pVertexBuffer);
	UINT Stride  = (UINT)stride;
	UINT Offset  = (UINT)iOffset;
	if(stride == 0 ) Stride = (UINT)d9VB->m_nBytePerVertex;	
    getDevice()->SetStreamSource( (UINT)bufIdx , (IDirect3DVertexBuffer9*)d9VB->handle() , (UINT)iOffset , Stride );
    m_NumVerticesInVB = (UINT)d9VB->nElement();
	return true;
}

bool xD3D9RenderApi::setVertexStream(IVertexStream* vertexStream)
{
	xD3DVertexStream* d10VertStream = dynamic_cast<xD3DVertexStream*>(vertexStream);
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
D3DPRIMITIVETYPE GetD3DPrimitiveType(ePrimtiveType _pt , int nVertex , int& nPrimitive)
{
    switch(_pt)  
    {
    case ePrimtiveType_Points           : { nPrimitive = nVertex     ; return D3DPT_POINTLIST    ; }
    case ePrimtiveType_Lines            : { nPrimitive = nVertex/2   ; return D3DPT_LINELIST     ; }
    case ePrimtiveType_LineStrip        : { nPrimitive = nVertex - 1 ; return D3DPT_LINESTRIP    ; }
    case ePrimtiveType_Triangles        : { nPrimitive = nVertex/3   ; return D3DPT_TRIANGLELIST ; }
    case ePrimtiveType_TriangleStrip    : { nPrimitive = nVertex - 2 ; return D3DPT_TRIANGLESTRIP; }
    default:                              { nPrimitive = nVertex     ; return D3DPT_POINTLIST    ; }
    }
}
bool xD3D9RenderApi::beginScene()
{
    getDevice()->BeginScene();
    lock();
    return true;
}

bool xD3D9RenderApi::endScene()
{
    getDevice()->EndScene();
    unlock();
    return true;
}
bool xD3D9RenderApi::drawPrimitive(size_t nVertex , size_t iStartVertex , ePrimtiveType pt)
{
    int nPrimitive = 0;
    D3DPRIMITIVETYPE d3dPt = GetD3DPrimitiveType(pt , (int)nVertex , nPrimitive);
	if(m_pCallback)
	{
		m_pCallback->preDrawPrimitive(this , m_RenderMode);
		setPrimitiveType(pt);
		getDevice()->DrawIndexedPrimitive(d3dPt,0 , 0 , m_NumVerticesInVB , (UINT)iStartVertex , (UINT)nPrimitive);
		m_pCallback->preDrawPrimitive(this , m_RenderMode);
	}
	else
	{
		setPrimitiveType(pt);
		getDevice()->DrawIndexedPrimitive(d3dPt,0 , 0 , m_NumVerticesInVB , (UINT)iStartVertex , nPrimitive);
	}
	return true;
}

bool  xD3D9RenderApi::drawRectf(IBaseTexture* pTexture, float vDestRect[4] , const xColor_4f& color)
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

bool xD3D9RenderApi::drawRectf(IBaseTexture* pTexture, float vDestRect[4] ,float vSrcRect[4]  , const xColor_4f& color)
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

void xD3D9RenderApi::set2DTexture(int iStage , IBaseTexture* pTexture)
{
	m_pDef2DRect->setTexture(iStage , pTexture);
}

void xD3D9RenderApi::set2DTexture(int iStage , IBaseTexture* pTexture , float _srcRect[4])
{
	xvec4& srcRect =  *((xvec4*)_srcRect);;
	m_pDef2DRect->setTexture(iStage , pTexture, srcRect);
}

I2DRectObject* xD3D9RenderApi::create2DRectObject()
{
	return new xD3D2DRect(m_pDef2DRectEnv);
}

bool xD3D9RenderApi::draw2DRect(I2DRectObject* p2DRect)
{
	p2DRect->commit();
	return p2DRect->draw();
}

IGpuProgram* xD3D9RenderApi::createGpuProgram()
{
	return new xD3D9GpuProgram(this);
}

IShaderCodeGenerator* xD3D9RenderApi::createShaderCodeGenerator(const unsigned char * _code)
{
	return NULL; 
}

IBaseShader* xD3D9RenderApi::createShader(eShaderType type)
{
	if(type == eShader_VertexShader)
	{
		return new xD3D9VertexShader(this);
	}

	if(type == eShader_PixelShader)
	{
		return new xD3D9PixelShader(this);
	}
	return NULL;
}

bool xD3D9RenderApi::setGpuProgram(IGpuProgram* pGpuProgram)
{
	if( m_pGpuProgram.equal(pGpuProgram) )
		return false;

	xD3D9GpuProgram* pD9GpuProgram = (xD3D9GpuProgram*)pGpuProgram;
	m_pGpuProgram = pGpuProgram;//pD10GpuProgram;
	if(pD9GpuProgram)
	{		    
		setVertexShader (pD9GpuProgram->m_pShader[eShader_VertexShader  ]);
		setPixelShader  (pD9GpuProgram->m_pShader[eShader_PixelShader   ]);
	}
	return true;
}


bool xD3D9RenderApi::setShader(IBaseShader* pShader , eShaderType _type)
{
	if(pShader)
		_type = pShader->type();
	if(_type == eShader_PixelShader)
		return setPixelShader(pShader);
	if(_type == eShader_VertexShader)
		return setVertexShader(pShader);
	return false;

}

bool xD3D9RenderApi::setHullShader(IBaseShader* pHullShader)
{
    return false;
}

bool xD3D9RenderApi::setDomainShader(IBaseShader* pDomainShader)
{
    return false;
}

bool xD3D9RenderApi::setComputeShader(IBaseShader* pComputeShader)
{
    return false;
}

bool xD3D9RenderApi::setVertexShader(IBaseShader* pVertShader)
{
	if( pVertShader == NULL || pVertShader->isLoaded() == false)
	{
        getDevice()->SetVertexShader(NULL);
		return true;
	}
#ifdef _DEBUG
	xD3D9VertexShader* pVS = dynamic_cast<xD3D9VertexShader*> (pVertShader);
#else
	xD3D9VertexShader* pVS = (xD3D9VertexShader*) pVertShader;
#endif
	getDevice()->SetVertexShader( pVS->m_pVertexShader );
	return true;
}

bool xD3D9RenderApi::setPixelShader(IBaseShader* pPixelShader)
{
	if(pPixelShader == NULL || pPixelShader->isLoaded() == false)
	{
		getDevice()->SetPixelShader(NULL);
		return true;
	}
#ifdef _DEBUG
	xD3D9PixelShader* pPS = dynamic_cast<xD3D9PixelShader*> (pPixelShader);
#else
	xD3D9PixelShader* pPS = (xD3D9PixelShader*) pPixelShader;
#endif
	getDevice()->SetPixelShader(pPS->m_pPixelShader);
	return true;
}

bool xD3D9RenderApi::setGeomShader(IBaseShader* pGeomShader)
{
     return false;
}

bool xD3D9RenderApi::setSamplerState(eShaderType _st , int iSlot , ISamplerState* pSampler)
{
	if(pSampler == NULL)
		return false;

	xD3D9Sampler * pD9Sampler = (xD3D9Sampler *)pSampler;
    if(pD9Sampler == NULL) pD9Sampler = m_pDefSampler;

    pD9Sampler->Install(iSlot , m_Samplers[iSlot] );
    m_Samplers[iSlot] = pD9Sampler;
	return false;
}


bool  xD3D9RenderApi::setShaderResource(eShaderType _st , int iSlot , IBaseTexture* pTexture)
{
	 IDirect3DBaseTexture9* pDxTexture = (IDirect3DBaseTexture9*)pTexture->handle();
     getDevice()->SetTexture(iSlot , pDxTexture);
     return true;
}


bool  xD3D9RenderApi::setShaderResource(eShaderType _st , int iSlot ,IInputBuffer* pBuffer , ePIXEL_FORMAT fmt)
{
	return false;
}

bool xD3D9RenderApi::isTextureSupport(ePIXEL_FORMAT fmt , bool lockable)
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

IBaseTexture* xD3D9RenderApi::createFileTexture(const wchar_t* texFile , const unsigned int8* buf , unsigned int bufLen, unsigned int arg, const xTextureInitDesc* texInitDesc)
{
	xD3D9FileTexture* pTex = new xD3D9FileTexture(this , texInitDesc);
	if(pTex->load(texFile , buf , bufLen , arg ) )
	{
		return pTex;
	}
	pTex->KillObject();
	return NULL;
}

const wchar_t* xD3D9RenderApi::texCoordStyle()
{
	return L"Direct3D";
}

IBaseTexture*  xD3D9RenderApi::createFileTexture(const wchar_t* extFile, const xTextureInitDesc* texInitDesc)
{
	return new xD3D9FileTexture(this , texInitDesc);
}

IBaseTexture* xD3D9RenderApi::createTexture(const xTextureInitDesc& initDesc , xTextureInitData* pInitData , int nInitData )
{
    xD3D9BaseTexture* pTexture = NULL;
    if(initDesc.m_TextureDesc.m_depth > 1)
    {
        pTexture = new xD3D9Texture3D(this , initDesc.m_bReadable);
    }
    else if(initDesc.m_TextureDesc.m_nArraySize == 6)
    {
        pTexture = new xD3D9TextureCube(this , initDesc.m_bReadable);
    }
    else if(initDesc.m_TextureDesc.m_nArraySize == 1)
    {
        pTexture = new xD3D9Texture2D(this , initDesc.m_bReadable);
    }
    
    if(pTexture == NULL)
        return NULL;

    if(pTexture->create(initDesc , pInitData , nInitData) )
    {
        return pTexture;
    }
    delete pTexture;
    return NULL;	
}

IRenderCamera* xD3D9RenderApi::createCamera(const wchar_t* cameraName)
{
	return m_MatrixCtx->createCamera(cameraName);
}

IBaseTexture *xD3D9RenderApi::createRenderableTexture(int w , int h , int depth , ePIXEL_FORMAT fmt , bool bReadable ,  int nMipMap, int nArraySize  , const xRTSampleDesc& sampleQulity)
{
    if(depth != 1)
        return false;

    xD3D9BaseTexture* pRenderTexture = NULL;
    if(nArraySize == 6)
    {
        pRenderTexture = new xD3D9TextureCube(this , bReadable);
    }
    else if(nArraySize)
    {
        pRenderTexture = new xD3D9Texture2D  (this , bReadable);
    }

    if(pRenderTexture == NULL)
        return NULL;

    xTextureInitDesc  initDesc(w , h , fmt , depth);
    initDesc.m_access = RESOURCE_ACCESS_NONE;
    initDesc.m_bReadable = bReadable;
    initDesc.m_nMipmap = nMipMap;
    initDesc.m_TextureDesc.m_nArraySize = nArraySize;
    initDesc.m_usage = RESOURCE_USAGE_DEFAULT;
    initDesc.m_bindType = eResourceBindType (BIND_AS_RENDER_TARGET | BIND_AS_SHADER_RESOURCE);
    if( false == pRenderTexture->create(initDesc , NULL , 0) )
    {
        delete pRenderTexture;
        return NULL;
    }
    return pRenderTexture;
}

IRenderTarget* xD3D9RenderApi::createRenderTarget(int w , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture , const xRTSampleDesc& sampleQulity)
{
    xD3D9BaseTexture* pRenderTexture = new xD3D9Texture2D(this , bLockable);

    xTextureInitDesc initDesc(w , h , fmt);
    initDesc.m_bReadable = bLockable;
    initDesc.m_usage = RESOURCE_USAGE_DEFAULT;
    initDesc.m_bindType = eResourceBindType (BIND_AS_RENDER_TARGET | BIND_AS_SHADER_RESOURCE);
    initDesc.m_access = RESOURCE_ACCESS_NONE;
    if( false == pRenderTexture->create(initDesc , NULL , 0) )
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
	pRenderTexture->ReleaseObject();
	return pRT;
}

IRenderTarget* xD3D9RenderApi::createDepthBuffer(int w  , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture , const xRTSampleDesc& sampleQulity)
{
    xD3D9GIFormatInfo* pDX9Fmt = xD3D9ConstLexer::singleton()->GetPixelFormat(fmt);
    if(pDX9Fmt == NULL)
        return NULL;

	xD3D9DepthBuffer* pDepthBuffer = new xD3D9DepthBuffer(this);

    if( false == pDepthBuffer->create(w , h , pDX9Fmt->m_dxfmt) )
	{
		delete pDepthBuffer;
		return NULL;
	}
	return pDepthBuffer;
}

IRenderTarget*  xD3D9RenderApi::DefDepthBuffer()
{
	if(m_RenderWindow == NULL) return NULL;
	return m_RenderWindow->depthBuffer();
}

IDirect3DSurface9*  xD3D9RenderApi::DefDepthStencilView()
{
	if(m_RenderWindow == NULL) return NULL;
	return m_RenderWindow->GetDepthStencilView();
}

IRenderView* xD3D9RenderApi::createRenderView(int w , int h , bool bCreateDepth , const xRTSampleDesc& sampleDesc)
{
	xD3D9RenderView* pRenderView = new xD3D9RenderView(this , sampleDesc);
	if(w == 0 || h == 0)
	{
		RECT rect;
		GetClientRect(m_hMainWnd , &rect);
		w = rect.right - rect.left;
		h = rect.bottom - rect.top;
	}
	if(bCreateDepth) 
	{
		pRenderView->createDepthView(w , h );
	}

	return pRenderView;
}

IRenderView* xD3D9RenderApi::createRenderView(int w , int h ,void* hWnd ,bool bCreateDepth , const xRTSampleDesc& sampleDesc)
{
	return NULL;
}


bool xD3D9RenderApi::setRenderView(IRenderView* renderView)
{
	GUID_SELECT_MODE(false);
	xD3D9RenderView* pD10RV = (xD3D9RenderView*) renderView;
	m_RenderView = pD10RV;
	return pD10RV->install();
}

ILightingEnv* xD3D9RenderApi::createLightingState(const wchar_t* _name)
{
	xD1xLightingEnv* pLightingEnv = new xD1xLightingEnv(this , _name);
	m_objManager.LightEnvMgr().add(_name , pLightingEnv);
	return pLightingEnv;
}

END_NAMESPACE_XEVOL3D