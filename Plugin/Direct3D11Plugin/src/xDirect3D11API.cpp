#include "stdafx.h"
#include <BaseLib/xEvol3DAPI.h>
#include <BaseLib/xXmlDocument.h>
#include <OperationSys/xOperationSys.h>
#include "xDirect3D11API.h"
#include "Image/PixelFormatAssit.h"
#include "fs/xFileSystem.h"
#include "xD3DTLEnviroment.h"
//#include "../xDirect3D11DllApi.h"

#include "xD3DInputBuffer.h"
#include "xD11Sampler.h"
#include "xD11RazState.h"
#include "xD11DepthStencilState.h"
#include "xD11BlendState.h"
#include "xD11FileTexture.h"
#include "xD11RenderTarget.h"
#include "xD11LockableTexture.h"

#include "xD11InputBuffer.h"
#include "xD11InputAssembler.h"

#include "xD11RenderView.h"
#include "xD11Shader.h"
#include "xD11GPUProgram.h"


BEGIN_NAMESPACE_XEVOL3D

#define GUID_SELECT_MODE(_value)  if( m_RenderMode == eRenderMode_Select ) return _value;

IRenderApi::eFeatureLevel xD3D11RenderApi::featureLevel() 
{ 
    switch( D3DFeatureLevel() )
    {
    case D3D_FEATURE_LEVEL_11_0:
        {
            return eFeature_ShaderModel5;
        }
    case D3D_FEATURE_LEVEL_10_1:
        {
            return eFeature_ShaderModel4;
        }
    case D3D_FEATURE_LEVEL_10_0:
        {
            return eFeature_ShaderModel4;
        }
    case D3D_FEATURE_LEVEL_9_3:
        {
            return eFeature_ShaderModel3;
            break;
        }
    case D3D_FEATURE_LEVEL_9_2: // Shader model 2 fits feature level 9_1
    case D3D_FEATURE_LEVEL_9_1:
        {
            return eFeature_ShaderModel2;
            break;
        }
    }
    return eFeature_ShaderModel2;
}
const char* xD3D11RenderApi::getShaderProfile(eShaderType _type)
{
	switch( D3DFeatureLevel() )
	{
	case D3D_FEATURE_LEVEL_11_0:
		{
			if(_type == eShader_VertexShader) return  "vs_5_0";
			if(_type == eShader_PixelShader)  return  "ps_5_0";
			break;
		}
	case D3D_FEATURE_LEVEL_10_1:
		{
			if(_type == eShader_VertexShader) return  "vs_4_1";
			if(_type == eShader_PixelShader)  return  "ps_4_1";
			break;
		}
	case D3D_FEATURE_LEVEL_10_0:
		{
			if(_type == eShader_VertexShader) return  "vs_4_0" ;
			if(_type == eShader_PixelShader)  return  "ps_4_0" ;
			break;
		}
	case D3D_FEATURE_LEVEL_9_3:
		{
			if(_type == eShader_VertexShader) return  "vs_4_0_level_9_3" ;
			if(_type == eShader_PixelShader)  return  "ps_4_0_level_9_3" ;
			break;
		}
	case D3D_FEATURE_LEVEL_9_2: // Shader model 2 fits feature level 9_1
	case D3D_FEATURE_LEVEL_9_1:
		{
			if(_type == eShader_VertexShader) return  "vs_4_0_level_9_1" ;
			if(_type == eShader_PixelShader)  return  "ps_4_0_level_9_1" ;
			break;
		}
	}
	return "vs_5_0";
}


xD3D11RenderApi::xD3D11RenderApi(ID3D11Device* pDevice , HWND hWnd , int w , int h)
{
	m_PrimType   = ePrimtiveType_Triangles;
	m_pGpuProgram = NULL;
	m_pInputAssembler = NULL;
	m_pVertStream  = NULL;
	//设备属性和参数
	m_hMainWnd = NULL;
	m_hInst    = NULL;
	m_driverType = D3D_DRIVER_TYPE_HARDWARE;	
	memset(&m_swapChainDesc , 0 , sizeof(DXGI_SWAP_CHAIN_DESC) );
	//m_pDef2DRect    = NULL;
	//m_pDef2DRectEnv  = NULL;
	m_pSwapChain        = NULL;
	m_pD3DDevice        = NULL;
	m_pD3DDeviceContext  = NULL;

	m_pBlendStateNoBlend = NULL;
	m_pRasterizerStateNoCull = NULL;

	m_pD3DDevice        = pDevice;
	m_hMainWnd          = hWnd;
	m_Width             = w;
	m_Height            = h;
    m_DebugDevice       = false;
}

xD3D11RenderApi::~xD3D11RenderApi()
{
	uninit();
}

ID3D11Device*  xD3D11RenderApi::d11Device()
{
	return m_pD3DDevice ; 
}

ID3D11DeviceContext* xD3D11RenderApi::d11DeviceContext()
{
	return m_pD3DDeviceContext;
}
bool xD3D11RenderApi::uninit( )
{
	XSAFE_DELETE(m_RenderWindow);
	XSAFE_RELEASE(m_pD3DDevice);
	XSAFE_RELEASE(m_pD3DDeviceContext);
	return xRenderApiBase::uninit();
}

bool xD3D11RenderApi::init(xXmlNode* pSysNode)
{
	DXGI_SAMPLE_DESC sampleDes;
	xXmlNode* pRendererNode = pSysNode->findNode( name() , 0 );

	sampleDes.Count   = pRendererNode->value(L"Sample.Count")   ? pRendererNode->int_value(L"Sample.Count") : 1;
	sampleDes.Quality = pRendererNode->value(L"Sample.Quality") ? pRendererNode->int_value(L"Sample.Quality") : 0;
    m_DebugDevice = pRendererNode->bool_value(L"DebugDevice");
	if(false == create(sampleDes) )
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

	return true;
}
bool xD3D11RenderApi::GetDXGISampleDesc(DXGI_SAMPLE_DESC& SampleDesc)
{
    SampleDesc = m_swapChainDesc.SampleDesc;
    return true;
}
bool xD3D11RenderApi::create(DXGI_SAMPLE_DESC sampleDesc)
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
		ret = __createD11Device(sampleDesc);
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

bool xD3D11RenderApi::onResize(int width , int height)
{
	TAutoLocker<xRenderApiLocker> aLocker(m_pDevLocker);
	TAutoLocker<xRenderApiLocker> aRenderLocker(&m_RenderLocker);
	xD11RenderView* pRenderView = m_RenderView.type_case<xD11RenderView*>(); //dynamic_cast<xD11RenderView*>(m_RenderView.operator TObject*());
	if(false == __needResize(width , height) )
	{
		pRenderView->install();
		return false;
	}
	if(m_pBaseSelector)
	{
		m_pBaseSelector->resize(m_Width , m_Height );
	}
	//XEVOL_WARNNING_NOT_IMPLEMENT_INFO("在删除这些对象前应该判断是不是当前的RenderTarget");
	bool ret = m_RenderWindow->resize(m_Width  , m_Height);
	__resetViewPort();
	return pRenderView->install();
}

bool xD3D11RenderApi::__needResize(int width , int height)
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

bool xD3D11RenderApi::__resetViewPort()
{
	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width  = (FLOAT)m_Width;
	vp.Height = (FLOAT)m_Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pD3DDeviceContext->RSSetViewports(1, &vp );
	return true;
}

bool xD3D11RenderApi::__createD11Device(DXGI_SAMPLE_DESC sampleDes)
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
	D3D11_CREATE_DEVICE_FLAG createDeviceFlags = (D3D11_CREATE_DEVICE_FLAG)0;
#ifdef _DEBUG
	createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif
    if(m_DebugDevice)
        createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;

	D3D_DRIVER_TYPE driverTypes[] = 
	{
		D3D_DRIVER_TYPE_HARDWARE,            
		//D3D11_DRIVER_TYPE_REFERENCE,
		//D3D11_DRIVER_TYPE_REFERENCE,
		//D3D11_DRIVER_TYPE_REFERENCE,
		//D3D11_DRIVER_TYPE_REFERENCE,
		//D3D11_DRIVER_TYPE_REFERENCE,
	};
	HMODULE hDriverModule[]=
	{
		NULL,
		//LoadLibraryA( "D3D10WARP_beta.DLL" ),
		//LoadLibraryW( _XEVOL_ABSPATH_(L"D3D10WARP_beta.DLL") ),
		//LoadLibraryA( "D3D10WARP.DLL" ),
		//LoadLibraryW( _XEVOL_ABSPATH_(L"D3D10WARP.DLL" ) ),
		//NULL,
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
	m_swapChainDesc.SampleDesc = sampleDes;
	m_swapChainDesc.Windowed = TRUE;

	//Find all Adapter
	IDXGIAdapter * pAdapter;
	std::vector<IDXGIAdapter*> vAdapters;
	IDXGIFactory1* pDXGIFactory = NULL;
	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&pDXGIFactory) );

	while(pDXGIFactory->EnumAdapters(vAdapters.size(), &pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		vAdapters.push_back(pAdapter);
	}

    
	size_t _nAdapters = vAdapters.size() ;
	for(size_t i = 0 ; i < _nAdapters ; i ++)
	{
		IDXGIAdapter* pAdapter = vAdapters[i];
		DXGI_ADAPTER_DESC desc;
		pAdapter->GetDesc(&desc);

		std::vector<IDXGIOutput*> vOutputers;
		IDXGIOutput* pOutputer = NULL;
		while(pAdapter->EnumOutputs(vOutputers.size(), &pOutputer) != DXGI_ERROR_NOT_FOUND)
		{
			vOutputers.push_back(pOutputer);
			DXGI_OUTPUT_DESC odesc;
			pOutputer->GetDesc(&odesc);
			continue;
		}

CREATE_DEVICE:

		D3D_FEATURE_LEVEL FeatureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};
		UINT NumFeatureLevels = ARRAYSIZE( FeatureLevels );
		m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

		if(vAdapters.size() == 1) pAdapter = NULL;

		for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
		{
			m_driverType    = driverTypes[driverTypeIndex];
			HMODULE hModule = hDriverModule[driverTypeIndex];
			hr = D3D11CreateDevice(pAdapter , m_driverType, hModule , createDeviceFlags , FeatureLevels , NumFeatureLevels , D3D11_SDK_VERSION , &m_pD3DDevice, &m_FeatureLevel , &m_pD3DDeviceContext);
			
			//m_pD3DDevice-
			if( SUCCEEDED( hr ) )
			{
				UINT _Quality = 0;
				while(_Quality == 0)
				{
					m_pD3DDevice->CheckMultisampleQualityLevels(m_swapChainDesc.BufferDesc.Format , m_swapChainDesc.SampleDesc.Count , &_Quality);
				    if(_Quality > 0)
						break;
					m_swapChainDesc.SampleDesc.Count --;
				}

				if(m_swapChainDesc.SampleDesc.Quality > (_Quality - 1) )
				{
					m_swapChainDesc.SampleDesc.Quality = (_Quality - 1) ;
				}

				IDXGIDevice * pDXGIDevice = NULL;
				hr = m_pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);

				IDXGIAdapter * pDXGIAdapter = NULL;
				hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);

				IDXGIFactory1 * pIDXGIFactory = NULL;
				pDXGIAdapter->GetParent(__uuidof(IDXGIFactory1), (void **)&pIDXGIFactory);


				hr = pIDXGIFactory->CreateSwapChain(m_pD3DDevice , &m_swapChainDesc  , &m_pSwapChain);
				XSAFE_RELEASE(pIDXGIFactory);
				XSAFE_RELEASE(pDXGIAdapter);
				XSAFE_RELEASE(pDXGIDevice);
				if( SUCCEEDED( hr ) )
					break;
				XSAFE_RELEASE(m_pD3DDevice);
			}
			//hr = D3D11CreateDeviceAndSwapChain( pAdapter, m_driverType, hModule , createDeviceFlags,
			//	FeatureLevels , NumFeatureLevels , D3D11_SDK_VERSION, &m_swapChainDesc, &m_pSwapChain, &m_pD3DDevice ,
			//	&m_FeatureLevel , &m_pD3DDeviceContext);
			//if( SUCCEEDED( hr ) )
			//	break;
		}
		//重新创建
		if(m_pD3DDevice == NULL && createDeviceFlags != 0)
		{
			createDeviceFlags = (D3D11_CREATE_DEVICE_FLAG)0;
			goto CREATE_DEVICE;
		}

		size_t _nOutput = vOutputers.size();
		for(size_t iOp = 0 ; iOp < _nOutput ; iOp ++)
		{
			XSAFE_RELEASE(vOutputers[iOp]);
		}
		if(m_pD3DDevice && m_pSwapChain)
		{
			break;
		}
	}


	XSAFE_RELEASE(pDXGIFactory);
	_nAdapters = vAdapters.size();
	for(size_t iAdp = 0 ; iAdp < _nAdapters ; iAdp ++)
	{
		XSAFE_RELEASE(vAdapters[iAdp]);
	}

	if( FAILED(hr) )
		return false;


	m_RenderWindow = new xD11RenderWindow(m_hMainWnd , this);
	m_RenderWindow->create(m_pSwapChain , width , height);
	__resetViewPort();
	setRenderView(m_RenderWindow);

	//Blend Desc;
	D3D11_RENDER_TARGET_BLEND_DESC RTBlendDesc;
	ZeroMemory(&RTBlendDesc, sizeof(D3D11_RENDER_TARGET_BLEND_DESC));
	RTBlendDesc.BlendEnable = FALSE ;
	RTBlendDesc.SrcBlend = D3D11_BLEND_ONE ;
	RTBlendDesc.DestBlend = D3D11_BLEND_ZERO ;
	RTBlendDesc.BlendOp = D3D11_BLEND_OP_ADD ;
	RTBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE ;
	RTBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO ;
	RTBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD ;
	RTBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL ;

	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
	BlendState.AlphaToCoverageEnable  = FALSE;
	BlendState.IndependentBlendEnable = FALSE;
	for(int i = 0 ; i < 8 ; i ++)
		BlendState.RenderTarget[i] = RTBlendDesc;

	XSAFE_RELEASE(m_pBlendStateNoBlend);
	XSAFE_RELEASE(m_pRasterizerStateNoCull);
	m_pD3DDevice->CreateBlendState(&BlendState, &m_pBlendStateNoBlend) ;

	// Create a rasterizer state to disable culling
	D3D11_RASTERIZER_DESC RSDesc;
	RSDesc.FillMode = D3D11_FILL_SOLID;
	RSDesc.CullMode = D3D11_CULL_NONE;
	RSDesc.FrontCounterClockwise = TRUE;
	RSDesc.DepthBias = 0;
	RSDesc.DepthBiasClamp = 0;
	RSDesc.SlopeScaledDepthBias = 0;
	RSDesc.DepthClipEnable = TRUE;
	RSDesc.ScissorEnable = FALSE;
	RSDesc.MultisampleEnable = TRUE;
	RSDesc.AntialiasedLineEnable = FALSE;
	m_pD3DDevice->CreateRasterizerState(&RSDesc, &m_pRasterizerStateNoCull) ;

	m_pD3DDeviceContext->OMSetBlendState(m_pBlendStateNoBlend, 0, 0xffffffff);
	m_pD3DDeviceContext->RSSetState(m_pRasterizerStateNoCull);
	return true;
}

bool xD3D11RenderApi::__destory()
{
	XSAFE_RELEASEOBJECT(m_RenderWindow);
	XSAFE_RELEASE(m_pSwapChain);
	XSAFE_RELEASE(m_pD3DDevice);
	XSAFE_RELEASE(m_pD3DDeviceContext);
	XSAFE_RELEASE(m_defDepthStencilState);
	XSAFE_RELEASE(m_defBlendState);
	XSAFE_RELEASE(m_defpRasState);
	return true;

}
bool xD3D11RenderApi::__afterCreateDevice()
{
	m_pD3DDeviceContext->OMGetDepthStencilState( &m_defDepthStencilState , &m_DefStencilRef);
	m_pD3DDeviceContext->OMGetBlendState(&m_defBlendState , m_DefBlendFactor , &m_DefSampleMask);
	m_pD3DDeviceContext->RSGetState(&m_defpRasState);
	m_2DZValue = 0.0f;
	m_pDef2DRectEnv = new xD3D2DRectEnv(this);
	m_pDef2DRect    = new xD3D2DRect(m_pDef2DRectEnv);
	return true;   
}

bool xD3D11RenderApi::__initByDevice()
{
	XEVOL_WARNNING_NOT_IMPLEMENT
		return false;
}

bool xD3D11RenderApi::swapBuffer()
{
	TAutoLocker<xRenderApiLocker> aLocker(m_pDevLocker);
	m_pSwapChain->Present( 0, 0 );
	unlock();
	return true;
}

bool xD3D11RenderApi::setBlendState(IBlenderState* pState)
{

	GUID_SELECT_MODE(false);
    //d11DeviceContext()->OMSetBlendState(m_pBlendStateNoBlend , m_DefBlendFactor , m_DefSampleMask );
    //return true;

	if(pState == NULL)
	{
		d11DeviceContext()->OMSetBlendState(m_defBlendState , m_DefBlendFactor , m_DefSampleMask );
		return true;
	}
	ID3D11BlendState* pBlendState = (ID3D11BlendState*)pState->handle();
	d11DeviceContext()->OMSetBlendState(pBlendState , pState->factor() , pState->samplemask() );
	m_BlenderState      = pState;
	return true;
}

bool xD3D11RenderApi::setRasterizerState(IRasterizerState* pState)
{
    d11DeviceContext()->RSSetState( m_pRasterizerStateNoCull );
    return true;
	if(pState == NULL)
	{
		d11DeviceContext()->RSSetState( m_defpRasState );
		return true;
	}
	ID3D11RasterizerState* pRasState = (ID3D11RasterizerState*)pState->handle();
	d11DeviceContext()->RSSetState(pRasState );
	m_RasterizerState   = pState; 
	return true;
}


bool xD3D11RenderApi::setDepthStencilState(IDepthStencilState* pState)
{
	if(pState == NULL)
	{
		d11DeviceContext()->OMSetDepthStencilState( m_defDepthStencilState , m_DefStencilRef );
		return true;
	}
	ID3D11DepthStencilState* pZSState = (ID3D11DepthStencilState*)pState->handle();
	d11DeviceContext()->OMSetDepthStencilState( pZSState , pState->stencil_ref() );
	m_DepthStencilState = pState;
	return true;
}

IRenderApiState* xD3D11RenderApi::createRenderState(eRenderStateType _type)
{
	switch(_type)
	{
	case eRenderState_Sampler:
		return new xD11Sampler(this);
	case eRenderState_Blend:
		return new xD11BlendState(this);
		return NULL;
	case eRenderState_Rasterizer:
		return new xD11RazState(this);
		return NULL;
	case eRenderState_DepthStencil:
		return new xD11ZStencilState(this);
	}
	return NULL;
}

IInputAssembler* xD3D11RenderApi::createInputAssembler(const wchar_t* name , xInputLayoutDesc& desc )
{
	//First create a assembler
	xD11InputAssembler* assembler = (xD11InputAssembler*)objectManager().inputAssemblerMgr().find(name);
	if(assembler) 
	{
		assembler->AddRef();
		return assembler ;
	}

	//Not found
	assembler = new xD11InputAssembler(this , name , desc);
	if( assembler->create() )
	{
		objectManager().inputAssemblerMgr().add(name , assembler);
		return assembler;
	}
	assembler->KillObject();
	return NULL;
}

IInputBuffer*    xD3D11RenderApi::createInputBuffer(size_t nVertex , size_t nBytePerVertex , const xInputBufferDesc* pBufferDesc , void* pData)
{
	xD11InputBuffer* vBuffer = new xD11InputBuffer(this);

	bool ret  = vBuffer->create(nVertex ,  nBytePerVertex , pBufferDesc , pData) ;
	if(ret == false )
	{
		XSAFE_KILLOBJECT( vBuffer );
		return NULL;
	}
	return vBuffer;
}

IInputBuffer*  xD3D11RenderApi::createBuffer(size_t iBufLen ,const xInputBufferDesc* pBufferDesc , void* pData)
{
	xD11InputBuffer* vBuffer = new xD11InputBuffer(this);

	bool ret  = vBuffer->create(iBufLen , pBufferDesc , pData) ;
	if(ret == false )
	{
		XSAFE_KILLOBJECT( vBuffer );
		return NULL;
	}
	return vBuffer;
}

bool xD3D11RenderApi::setPrimitiveType(ePrimtiveType type)
{
	m_PrimType = type;
	D3D11_PRIMITIVE_TOPOLOGY d3d11PtLogicy = (D3D11_PRIMITIVE_TOPOLOGY)m_PrimType;
	d11DeviceContext()->IASetPrimitiveTopology( d3d11PtLogicy );
	return true;
}

ePrimtiveType xD3D11RenderApi::getPrimitiveType()
{
	D3D11_PRIMITIVE_TOPOLOGY type;
	d11DeviceContext()->IAGetPrimitiveTopology( &type );
	return (ePrimtiveType)type;
}

bool xD3D11RenderApi::setInputAssembler(IInputAssembler* pAssembler)
{
	m_pInputAssembler = pAssembler;
	xD11InputAssembler* assembler = dynamic_cast<xD11InputAssembler*>(pAssembler);
	ID3D11InputLayout* pILayout = assembler->d11Layout();
	d11DeviceContext()->IASetInputLayout( pILayout );
	return true;
}

bool xD3D11RenderApi::setIndexBuffer(IInputBuffer* pVertexBuffer , size_t iOffset)
{
	xD11InputBuffer* d10IB = dynamic_cast<xD11InputBuffer*>(pVertexBuffer);
	UINT Offset  = (UINT)iOffset;
	UINT uBPB = (UINT)d10IB->m_nBytePerVertex;	
	DXGI_FORMAT df = (uBPB == 2 ? DXGI_FORMAT_R16_UINT  : DXGI_FORMAT_R32_UINT );
	d11DeviceContext()->IASetIndexBuffer(d10IB->m_pBuffer , df , Offset);
	return true;
}

bool xD3D11RenderApi::setVertexBuffer(IInputBuffer* pVertexBuffer ,   size_t bufIdx , size_t stride , size_t iOffset )
{
	xD11InputBuffer* d10IB = dynamic_cast<xD11InputBuffer*>(pVertexBuffer);
	UINT Stride  = (UINT)stride;
	UINT Offset  = (UINT)iOffset;
	if(stride == 0 ) Stride = (UINT)d10IB->m_nBytePerVertex;	
	m_pD3DDeviceContext->IASetVertexBuffers((UINT)bufIdx , 1 , &d10IB->m_pBuffer , &Stride , &Offset );
	return true;
}

bool xD3D11RenderApi::setVertexStream(IVertexStream* vertexStream)
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

bool xD3D11RenderApi::drawPrimitive(size_t nVertex , size_t iStartVertex , ePrimtiveType pt)
{
	if(m_pCallback)
	{
		m_pCallback->preDrawPrimitive(this , m_RenderMode);
		setPrimitiveType(pt);
		d11DeviceContext()->DrawIndexed( (UINT)nVertex , (UINT)iStartVertex , 0 );
		m_pCallback->preDrawPrimitive(this , m_RenderMode);
	}
	else
	{
		setPrimitiveType(pt);
		d11DeviceContext()->DrawIndexed( (UINT)nVertex , (UINT)iStartVertex , 0 );
	}
	return true;
}

bool  xD3D11RenderApi::drawRectf(IBaseTexture* pTexture, float vDestRect[4] , const xColor_4f& color)
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

bool xD3D11RenderApi::drawRectf(IBaseTexture* pTexture, float vDestRect[4] ,float vSrcRect[4]  , const xColor_4f& color)
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

void xD3D11RenderApi::set2DTexture(int iStage , IBaseTexture* pTexture)
{
	m_pDef2DRect->setTexture(iStage , pTexture);
}

void xD3D11RenderApi::set2DTexture(int iStage , IBaseTexture* pTexture , float _srcRect[4])
{
	xvec4& srcRect =  *((xvec4*)_srcRect);;
	m_pDef2DRect->setTexture(iStage , pTexture, srcRect);
}

I2DRectObject* xD3D11RenderApi::create2DRectObject()
{
	return new xD3D2DRect(m_pDef2DRectEnv);
}

bool xD3D11RenderApi::draw2DRect(I2DRectObject* p2DRect)
{
	p2DRect->commit();
	return p2DRect->draw();
}

IGpuProgram* xD3D11RenderApi::createGpuProgram()
{
	return new xD11GpuProgram(this);
}

IShaderCodeGenerator* xD3D11RenderApi::createShaderCodeGenerator(const unsigned char * _code)
{
	return NULL; 
}

IBaseShader* xD3D11RenderApi::createShader(eShaderType type)
{
	if(type == eShader_VertexShader)
	{
		return new xD11VertexShader(this);
	}

	if(type == eShader_PixelShader)
	{
		return new xD11PixelShader(this);
	}

	if(type == eShader_GeometryShader)
	{
		return new xD11GeomShader(this);
	}
	if(type == eShader_HullShader)
	{
		return new xD11HullShader(this);
	}

	if(type == eShader_DomainShader)
	{
		return new xD11DomainShader(this);
	}

	if(type == eShader_ComputeShader)
	{
		return new xD11ComputeShader(this);
	}
	return NULL;
}

bool xD3D11RenderApi::setGpuProgram(IGpuProgram* pGpuProgram)
{
	if( m_pGpuProgram.equal(pGpuProgram) )
		return false;

	xD11GpuProgram* pD11GpuProgram = (xD11GpuProgram*)pGpuProgram;
	m_pGpuProgram = pGpuProgram;//pD10GpuProgram;
	if(pD11GpuProgram)
	{		    
		setVertexShader (pD11GpuProgram->m_pShader[eShader_VertexShader  ]);
		setPixelShader  (pD11GpuProgram->m_pShader[eShader_PixelShader   ]);
		setGeomShader   (pD11GpuProgram->m_pShader[eShader_GeometryShader]);
		setHullShader   (pD11GpuProgram->m_pShader[eShader_HullShader    ]);
		setDomainShader (pD11GpuProgram->m_pShader[eShader_DomainShader  ]);
		setComputeShader(pD11GpuProgram->m_pShader[eShader_ComputeShader ]);
	}
	return true;
}


bool xD3D11RenderApi::setShader(IBaseShader* pShader , eShaderType _type)
{
	if(pShader)
		_type = pShader->type();
	if(_type == eShader_PixelShader)
		return setPixelShader(pShader);
	if(_type == eShader_VertexShader)
		return setVertexShader(pShader);
	if(_type == eShader_GeometryShader)
		return setGeomShader(pShader);

	if(_type == eShader_HullShader)
		return setHullShader(pShader);
	if(_type == eShader_ComputeShader)
		return setComputeShader(pShader);
	if(_type == eShader_DomainShader)
		return setDomainShader(pShader);

	return false;

}

bool xD3D11RenderApi::setHullShader(IBaseShader* pHullShader)
{
		if( pHullShader == NULL || pHullShader->isLoaded() == false)
		{
			m_pD3DDeviceContext->HSSetShader( NULL  , NULL , 0);
			return true;
		}
	#ifdef _DEBUG
		xD11HullShader* pHS = dynamic_cast<xD11HullShader*> (pHullShader);
	#else
		xD11HullShader* pHS = (xD11HullShader*) pHullShader;
	#endif
		m_pD3DDeviceContext->HSSetShader( pHS->m_pHullShader , pHS->DynamicLinkClass() , pHS->nDynamicLinkClas() );
	return true;
}

bool xD3D11RenderApi::setDomainShader(IBaseShader* pDomainShader)
{
	if( pDomainShader == NULL || pDomainShader->isLoaded() == false)
	{
		m_pD3DDeviceContext->DSSetShader( NULL , NULL , 0);
		return true;
	}
#ifdef _DEBUG
	xD11DomainShader* pDS = dynamic_cast<xD11DomainShader*> (pDomainShader);
#else
	xD11DomainShader* pDS = (xD11DomainShader*) pDomainShader;
#endif
	m_pD3DDeviceContext->DSSetShader( pDS->m_pDomainShader , pDS->DynamicLinkClass() , pDS->nDynamicLinkClas() );
	return true;
}

bool xD3D11RenderApi::setComputeShader(IBaseShader* pComputeShader)
{
	if( pComputeShader == NULL || pComputeShader->isLoaded() == false)
	{
		m_pD3DDeviceContext->CSSetShader( NULL , NULL , 0 );
		return true;
	}
#ifdef _DEBUG
	xD11ComputeShader* pCS = dynamic_cast<xD11ComputeShader*> (pComputeShader);
#else
	xD11ComputeShader* pCS = (xD11ComputeShader*) pComputeShader;
#endif
	m_pD3DDeviceContext->CSSetShader( pCS->m_pComputeShader , pCS->DynamicLinkClass() , pCS->nDynamicLinkClas() );
	return true;
}

bool xD3D11RenderApi::setVertexShader(IBaseShader* pVertShader)
{
	if( pVertShader == NULL || pVertShader->isLoaded() == false)
	{
		m_pD3DDeviceContext->VSSetShader( NULL , NULL , 0 );
		return true;
	}
#ifdef _DEBUG
	xD11VertexShader* pVS = dynamic_cast<xD11VertexShader*> (pVertShader);
#else
	xD11VertexShader* pVS = (xD11VertexShader*) pVertShader;
#endif
	m_pD3DDeviceContext->VSSetShader( pVS->m_pVertexShader , pVS->DynamicLinkClass() , pVS->nDynamicLinkClas() );
	return true;
}

bool xD3D11RenderApi::setPixelShader(IBaseShader* pPixelShader)
{
	if(pPixelShader == NULL || pPixelShader->isLoaded() == false)
	{
		m_pD3DDeviceContext->PSSetShader( NULL , NULL , 0 );
		return true;
	}
#ifdef _DEBUG
	xD11PixelShader* pPS = dynamic_cast<xD11PixelShader*> (pPixelShader);
#else
	xD11PixelShader* pPS = (xD11PixelShader*) pPixelShader;
#endif
	m_pD3DDeviceContext->PSSetShader( pPS->m_pPixelShader , pPS->DynamicLinkClass() , pPS->nDynamicLinkClas() );
	return true;
}

bool xD3D11RenderApi::setGeomShader(IBaseShader* pGeomShader)
{
	if(pGeomShader == NULL || pGeomShader->isLoaded() == false)
	{
		m_pD3DDeviceContext->GSSetShader( NULL , NULL , 0 );
		return true;
	}
#ifdef _DEBUG
	xD11GeomShader* pGS = dynamic_cast<xD11GeomShader*> (pGeomShader);
#else
	xD11GeomShader* pGS = (xD11GeomShader*) pGeomShader;
#endif
	m_pD3DDeviceContext->GSSetShader( pGS->m_pGeomShader , pGS->DynamicLinkClass() , pGS->nDynamicLinkClas() );
	return true;
}

bool xD3D11RenderApi::setSamplerState(eShaderType _st , int iSlot , ISamplerState* pSampler)
{
	if(pSampler == NULL)
		return false;

	ID3D11SamplerState * pD10Sampler = (ID3D11SamplerState *)pSampler->handle();
	switch(_st)
	{
	case eShader_VertexShader:
		d11DeviceContext()->VSSetSamplers(iSlot , 1 , &pD10Sampler);
		return true;
		break;
	case eShader_PixelShader:
		d11DeviceContext()->PSSetSamplers(iSlot , 1 , &pD10Sampler);
		return true;
		break;
	case eShader_GeometryShader:
		d11DeviceContext()->GSSetSamplers(iSlot , 1 , &pD10Sampler);
		return true;

	case eShader_HullShader:
		d11DeviceContext()->HSSetSamplers(iSlot , 1 , &pD10Sampler);
		return true;

	case eShader_DomainShader:
		d11DeviceContext()->DSSetSamplers(iSlot , 1 , &pD10Sampler);
		return true;

	case eShader_ComputeShader:
		d11DeviceContext()->CSSetSamplers(iSlot , 1 , &pD10Sampler);
		return true;
		break;
	}
	return false;
}

bool  xD3D11RenderApi::setConstantBuffers(eShaderType shType , int iSlot , int nResource , ID3D11Buffer** pConstBuffer)
{
	if(shType == eShader_VertexShader )
	{
		d11DeviceContext()->VSSetConstantBuffers( iSlot , nResource , pConstBuffer);
	}
	else if(shType == eShader_PixelShader)
	{
		d11DeviceContext()->PSSetConstantBuffers( iSlot , nResource , pConstBuffer);
	}
	else if(shType == eShader_GeometryShader)
	{
		d11DeviceContext()->GSSetConstantBuffers( iSlot , nResource , pConstBuffer);  
	}
	else if(shType == eShader_HullShader )
	{
		d11DeviceContext()->HSSetConstantBuffers( iSlot , nResource , pConstBuffer);
	}
	else if(shType == eShader_DomainShader)
	{
		d11DeviceContext()->DSSetConstantBuffers( iSlot , nResource , pConstBuffer);
	}
	else if(shType == eShader_ComputeShader)
	{
		d11DeviceContext()->CSSetConstantBuffers( iSlot , nResource , pConstBuffer);  
	}

	return true;
}

bool  xD3D11RenderApi::setShaderResource(eShaderType _st , int iSlot , int nResource , ID3D11ShaderResourceView** pResourceView)
{
	HRESULT hRet = E_FAIL;
	switch(_st)
	{
	case eShader_VertexShader:
		d11DeviceContext()->VSSetShaderResources(iSlot , 1 , pResourceView );
		return true;
		break;
	case eShader_PixelShader:
		d11DeviceContext()->PSSetShaderResources(iSlot , 1 , pResourceView );
		return true;
		break;
	case eShader_GeometryShader:
		d11DeviceContext()->GSSetShaderResources(iSlot , 1 , pResourceView );
		return true;

	case eShader_HullShader:
		d11DeviceContext()->HSSetShaderResources(iSlot , 1 , pResourceView );
		return true;
		break;
	case eShader_DomainShader:
		d11DeviceContext()->DSSetShaderResources(iSlot , 1 , pResourceView );
		return true;
		break;
	case eShader_ComputeShader:
		d11DeviceContext()->CSSetShaderResources(iSlot , 1 , pResourceView );
		return true;

		break;
	}
	return SUCCEEDED(hRet);
}

bool  xD3D11RenderApi::setShaderResource(eShaderType _st , int iSlot , IBaseTexture* pTexture)
{
	ID3D11ShaderResourceView* pResourceView = pTexture?(ID3D11ShaderResourceView*)pTexture->handle() : NULL;
	return setShaderResource(_st , iSlot , 1 , &pResourceView);
}


bool  xD3D11RenderApi::setShaderResource(eShaderType _st , int iSlot ,IInputBuffer* pBuffer , ePIXEL_FORMAT fmt)
{
	xD11InputBuffer* pD11InputBuffer = dynamic_cast<xD11InputBuffer*>(pBuffer);
	ID3D11ShaderResourceView* pResourceView = pD11InputBuffer?(ID3D11ShaderResourceView*)pD11InputBuffer->toShaderResourceView( fmt ) : NULL;
	return setShaderResource(_st , iSlot , 1 , &pResourceView);
}

bool xD3D11RenderApi::isTextureSupport(ePIXEL_FORMAT fmt , bool lockable)
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

IBaseTexture* xD3D11RenderApi::createFileTexture(const wchar_t* texFile , const unsigned int8* buf , unsigned int bufLen, unsigned int arg)
{
	xD11FileTexture* pTex = new xD11FileTexture(this);
	if(pTex->load(texFile , buf , bufLen , arg ) )
	{
		return pTex;
	}
	pTex->KillObject();
	return NULL;
}

const wchar_t* xD3D11RenderApi::texCoordStyle()
{
	return L"Direct3D";
}

IBaseTexture*  xD3D11RenderApi::createFileTexture(const wchar_t* extFile)
{
	return new xD11FileTexture(this);
}

IBaseTexture* xD3D11RenderApi::createLockableTexture(int w , int h , int depth , ePIXEL_FORMAT fmt , bool bReadable , int nMipMap, int nArraySize)
{
	xD11LockableTexture* pTexture = new xD11LockableTexture(this , bReadable);
	if(pTexture->create(w , h , depth , fmt , nMipMap , nArraySize) )
	{
		return pTexture;
	}
	return NULL;	
}

IRenderCamera* xD3D11RenderApi::createCamera(const wchar_t* cameraName)
{
	return m_MatrixCtx->createCamera(cameraName);
}

IBaseTexture *xD3D11RenderApi::createRenderableTexture(int w , int h , int depth , ePIXEL_FORMAT fmt , bool bReadable ,  int nMipMap, int nArraySize  , const xRTSampleDesc& sampleQulity)
{
	if(depth != 1 && bReadable)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH,"3D Render To Texture cann't created as readable\n");
		bReadable = false;
	}
	xD11RenderTexture* pRenderTexture = new xD11RenderTexture(bReadable , true , this ,  GetDXGISampleDesc(sampleQulity) );
	if( false == pRenderTexture->create(w , h , depth , fmt) )
	{
		delete pRenderTexture;
		return NULL;
	}
	return pRenderTexture;
}

IRenderTarget* xD3D11RenderApi::createRenderTarget(int w , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture , const xRTSampleDesc& sampleQulity)
{
	xD11RenderTexture* pRenderTexture = new xD11RenderTexture(bLockable , bAsTexture , this,  GetDXGISampleDesc(sampleQulity) );
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
	pRenderTexture->ReleaseObject();
	return pRT;
}

IRenderTarget* xD3D11RenderApi::createDepthBuffer(int w  , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture , const xRTSampleDesc& sampleQulity)
{
	xD11DepthTexture* pDepthTexture = new xD11DepthTexture(bLockable , bAsTexture , this,  GetDXGISampleDesc(sampleQulity) );
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
	pDepthTexture->ReleaseObject();
	return pRT;
}

IRenderTarget*  xD3D11RenderApi::DefDepthBuffer()
{
	if(m_RenderWindow == NULL) return NULL;
	return m_RenderWindow->depthBuffer();
}

ID3D11DepthStencilView*  xD3D11RenderApi::DefDepthStencilView()
{
	if(m_RenderWindow == NULL) return NULL;
	return m_RenderWindow->GetDepthStencilView();
}

IRenderView* xD3D11RenderApi::createRenderView(int w , int h , bool bCreateDepth , const xRTSampleDesc& sampleDesc)
{
	xD11RenderView* pRenderView = new xD11RenderView(this , sampleDesc);
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

IRenderView* xD3D11RenderApi::createRenderView(int w , int h ,void* hWnd ,bool bCreateDepth , const xRTSampleDesc& sampleDesc)
{
	return NULL;
}

void xD3D11RenderApi::getRTSampleDesc(xRTSampleDesc& _desc)
{
	DXGI_SWAP_CHAIN_DESC swap_desc;
	m_pSwapChain->GetDesc(&swap_desc);
	_desc.m_SampleCount = swap_desc.SampleDesc.Count;
	_desc.m_SampleQulity= swap_desc.SampleDesc.Quality;
}

DXGI_SAMPLE_DESC xD3D11RenderApi::GetDXGISampleDesc(const xRTSampleDesc& sampleDesc)
{
	DXGI_SAMPLE_DESC SampleDesc = { sampleDesc.m_SampleCount , sampleDesc.m_SampleQulity };
	if(sampleDesc.m_SampleCount == - 1)
	{
		GetDXGISampleDesc(SampleDesc);
	}
	return SampleDesc;
}

bool xD3D11RenderApi::setRenderView(IRenderView* renderView)
{
	GUID_SELECT_MODE(false);
	xD11RenderView* pD10RV = (xD11RenderView*) renderView;
	m_RenderView = pD10RV;
	return pD10RV->install();
}

ILightingEnv* xD3D11RenderApi::createLightingState(const wchar_t* _name)
{
	xD1xLightingEnv* pLightingEnv = new xD1xLightingEnv(this , _name);
	m_objManager.LightEnvMgr().add(_name , pLightingEnv);
	return pLightingEnv;
}

END_NAMESPACE_XEVOL3D