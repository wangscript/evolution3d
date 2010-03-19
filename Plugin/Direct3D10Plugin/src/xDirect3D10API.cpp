#include "stdafx.h"
#include <BaseLib/xEvol3DAPI.h>
#include <BaseLib/xXmlDocument.h>
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
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxgi.lib")
#ifdef _DEBUG
#pragma comment(lib,"d3dx10d.lib")
#else
#pragma comment(lib,"d3dx10.lib")
#endif

#include "xD10HelperRender.h"
#include "xD10RenderView.h"
#include "xD3DBaseColorSelector.h"
#include "Image/PixelFormatAssit.h"
#include "fs/xFileSystem.h"
#include "xD3DTLEnviroment.h"
BEGIN_NAMESPACE_XEVOL3D

#define GUID_SELECT_MODE(_value)  if( m_RenderMode == eRenderMode_Select ) return _value;
xD3D10RenderApi::xD3D10RenderApi(ID3D10Device* pDevice , HWND hWnd , int w , int h)
{
      m_PrimType   = ePrimtiveType_Triangles;
      m_pGpuProgram = NULL;
      m_pInputAssembler = NULL;
      m_pVertStream  = NULL;
      //设备属性和参数
      m_hMainWnd = NULL;
       m_hInst    = NULL;
      m_driverType = D3D10_DRIVER_TYPE_HARDWARE;	
      m_pDef2DRect    = NULL;
      m_pDef2DRectEnv  = NULL;
      m_driverType        = D3D10_DRIVER_TYPE_NULL;
      m_pD3DDevice      = NULL;

      m_pD3DDevice      = pDevice;
      m_hMainWnd          = hWnd;
      m_Width             = w;
      m_Height            = h;
      m_DebugDevice       = false;
      m_RenderCaps.setValue(L"MaxGpuBone" , 256 );

      m_RenderWindow = NULL;

}

xD3D10RenderApi::~xD3D10RenderApi()
{
      uninit();
}

ID3D10Device*  xD3D10RenderApi::d10Device()
{
      return m_pD3DDevice ; 
}

bool xD3D10RenderApi::GetDXGISampleDesc(DXGI_SAMPLE_DESC& SampleDesc)
{
    if(m_RenderWindow == NULL)
        return false;

    return m_RenderWindow->GetDXGISampleDesc(SampleDesc);
}

bool xD3D10RenderApi::uninit( )
{
      XSAFE_DELETE(m_RenderWindow);
      XSAFE_RELEASE(m_pD3DDevice);
	  return xRenderApiBase::uninit();
}

bool xD3D10RenderApi::init(xXmlNode* pSysNode)
{
	DXGI_SAMPLE_DESC sampleDes;
	xXmlNode* pRendererNode = pSysNode->findNode( name() , 0 );

	sampleDes.Count   = pRendererNode->value(L"Sample.Count")   ? pRendererNode->int_value(L"Sample.Count") : 1;
	sampleDes.Quality = pRendererNode->value(L"Sample.Quality") ? pRendererNode->int_value(L"Sample.Quality") : 0;
    m_DebugDevice     = pRendererNode->bool_value(L"DebugDevice");
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

bool xD3D10RenderApi::create(DXGI_SAMPLE_DESC sampleDes)//)
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
            ret = __createD10Device(sampleDes);
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
      TAutoLocker<xRenderApiLocker> aLocker(m_pDevLocker);
      TAutoLocker<xRenderApiLocker> aRenderLocker(&m_RenderLocker);
	  xD10RenderView* pRenderView = m_RenderView.type_case<xD10RenderView*>(); //dynamic_cast<xD10RenderView*>(m_RenderView.operator TObject*());
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

int xD3D10RenderApi::intCapsValue(const wchar_t* cfgName , int defValue)
{
    xXmlValue* pValue = m_RenderCaps.findValue(cfgName);
    if(pValue == NULL) return defValue;
    return pValue->int_value();
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
      if( m_RenderWindow->NeedResize(width , height ) == false)
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
      m_pD3DDevice->RSSetViewports( 1, &vp );
      return true;
}

bool xD3D10RenderApi::__createD10Device(DXGI_SAMPLE_DESC sampleDesc)
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
      if(m_DebugDevice)
          createDeviceFlags = D3D10_CREATE_DEVICE_DEBUG;
      D3D10_DRIVER_TYPE driverTypes[] = 
      {
            D3D10_DRIVER_TYPE_HARDWARE,            
            //D3D10_DRIVER_TYPE_REFERENCE,
            //D3D10_DRIVER_TYPE_REFERENCE,
            //D3D10_DRIVER_TYPE_REFERENCE,
            //D3D10_DRIVER_TYPE_REFERENCE,
            //D3D10_DRIVER_TYPE_REFERENCE,
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
      DXGI_SWAP_CHAIN_DESC swapChainDesc;
      ZeroMemory( &swapChainDesc, sizeof(swapChainDesc) );
      swapChainDesc.BufferCount = 1;
      swapChainDesc.BufferDesc.Width = width;
      swapChainDesc.BufferDesc.Height = height;
      swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
      swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
      swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      swapChainDesc.OutputWindow = m_hMainWnd;
      swapChainDesc.SampleDesc = sampleDesc;
      swapChainDesc.Windowed = TRUE;

      //Find all Adapter
      IDXGIAdapter * pAdapter;
      std::vector<IDXGIAdapter*> vAdapters;
      IDXGIFactory* pDXGIFactory = NULL;
       hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pDXGIFactory) );

      while(pDXGIFactory->EnumAdapters((UINT)vAdapters.size(), &pAdapter) != DXGI_ERROR_NOT_FOUND)
      {
            vAdapters.push_back(pAdapter);
      }

      IDXGISwapChain* pSwapChain = NULL;
	  size_t _nAdapters = vAdapters.size() ;
      for(size_t i = 0 ; i < _nAdapters ; i ++)
      {
            IDXGIAdapter* pAdapter = vAdapters[i];
            DXGI_ADAPTER_DESC desc;
            pAdapter->GetDesc(&desc);

            std::vector<IDXGIOutput*> vOutputers;
            IDXGIOutput* pOutputer = NULL;
            while(pAdapter->EnumOutputs((UINT)vOutputers.size(), &pOutputer) != DXGI_ERROR_NOT_FOUND)
            {
                  vOutputers.push_back(pOutputer);
                  DXGI_OUTPUT_DESC odesc;
                  pOutputer->GetDesc(&odesc);
                  continue;
            }
RECREATE:
            for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
            {
				m_driverType    = driverTypes[driverTypeIndex];
				HMODULE hModule = hDriverModule[driverTypeIndex];
				hr = D3D10CreateDevice(pAdapter , m_driverType, hModule , createDeviceFlags , D3D10_SDK_VERSION , &m_pD3DDevice);

				//m_pD3DDevice-
				if( SUCCEEDED( hr ) )
				{
					UINT _Quality = 0;
					while(_Quality == 0)
					{
						m_pD3DDevice->CheckMultisampleQualityLevels(swapChainDesc.BufferDesc.Format , swapChainDesc.SampleDesc.Count , &_Quality);
						if(_Quality > 0)
							break;
						swapChainDesc.SampleDesc.Count --;
					}

					if(swapChainDesc.SampleDesc.Quality > (_Quality - 1) )
					{
						swapChainDesc.SampleDesc.Quality = (_Quality - 1) ;
					}

					IDXGIDevice * pDXGIDevice = NULL;
					hr = m_pD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);

					IDXGIAdapter * pDXGIAdapter = NULL;
					hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);

					IDXGIFactory * pIDXGIFactory = NULL;
					pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);


					hr = pIDXGIFactory->CreateSwapChain(m_pD3DDevice , &swapChainDesc  , &pSwapChain);
					XSAFE_RELEASE(pIDXGIFactory);
					XSAFE_RELEASE(pDXGIAdapter);
					XSAFE_RELEASE(pDXGIDevice);
					if( SUCCEEDED( hr ) )
						break;
					XSAFE_RELEASE(m_pD3DDevice);
				}
            }
			if(m_pD3DDevice == NULL && createDeviceFlags!=0)
			{
				createDeviceFlags = 0;
				goto RECREATE;
			}
            if(m_pD3DDevice && pSwapChain)
            {
                  break;
            }
      }
   


      if( FAILED(hr) )
            return false;


      m_RenderWindow = new xD10RenderWindow(m_hMainWnd , this);
      m_RenderWindow->create(pSwapChain , width , height);
      __resetViewPort();
      setRenderView(m_RenderWindow);

      D3D10_BLEND_DESC BlendState;
      ZeroMemory(&BlendState, sizeof(D3D10_BLEND_DESC));
      BlendState.BlendEnable[0] = FALSE;
      BlendState.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

      ID3D10BlendState*       g_pBlendStateNoBlend = NULL;
      ID3D10RasterizerState*	g_pRasterizerStateNoCull = NULL;
      m_pD3DDevice->CreateBlendState(&BlendState, &g_pBlendStateNoBlend) ;

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
      m_pD3DDevice->CreateRasterizerState(&RSDesc, &g_pRasterizerStateNoCull) ;

      m_pD3DDevice->OMSetBlendState(g_pBlendStateNoBlend, 0, 0xffffffff);
      m_pD3DDevice->RSSetState(g_pRasterizerStateNoCull);
      return true;
}

bool xD3D10RenderApi::__destory()
{
      XSAFE_RELEASEOBJECT(m_RenderWindow);
      XSAFE_RELEASE(m_pD3DDevice);
      XSAFE_RELEASE(m_defDepthStencilState);
      XSAFE_RELEASE(m_defBlendState);
      XSAFE_RELEASE(m_defpRasState);
      return true;

}
bool xD3D10RenderApi::__afterCreateDevice()
{
      m_pD3DDevice->OMGetDepthStencilState( &m_defDepthStencilState , &m_DefStencilRef);
      m_pD3DDevice->OMGetBlendState(&m_defBlendState , m_DefBlendFactor , &m_DefSampleMask);
      m_pD3DDevice->RSGetState(&m_defpRasState);
      m_2DZValue = 0.0f;
      m_pDef2DRectEnv = new xD3D2DRectEnv(this);
      m_pDef2DRect    = new xD3D2DRect(m_pDef2DRectEnv);
      return true;   
}

bool xD3D10RenderApi::__initByDevice()
{
      XEVOL_WARNNING_NOT_IMPLEMENT
            return false;
}

bool xD3D10RenderApi::swapBuffer()
{
      TAutoLocker<xRenderApiLocker> aLocker(m_pDevLocker);
      m_RenderWindow->Present( 0, 0 );
      unlock();
      return true;
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

IRenderApiState* xD3D10RenderApi::createRenderState(eRenderStateType _type)
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

IInputAssembler* xD3D10RenderApi::createInputAssembler(const wchar_t* name , xInputLayoutDesc& desc )
{
      //First create a assembler
      xD10InputAssembler* assembler = (xD10InputAssembler*)objectManager().inputAssemblerMgr().find(name);
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

bool xD3D10RenderApi::setPrimitiveType(ePrimtiveType type)
{
      m_PrimType = type;
      D3D10_PRIMITIVE_TOPOLOGY d3d10PtLogicy = (D3D10_PRIMITIVE_TOPOLOGY)m_PrimType;
      m_pD3DDevice->IASetPrimitiveTopology( d3d10PtLogicy );
      return false;
}

ePrimtiveType xD3D10RenderApi::getPrimitiveType()
{
      D3D10_PRIMITIVE_TOPOLOGY type;
      m_pD3DDevice->IAGetPrimitiveTopology( &type );
      return (ePrimtiveType)type;
}

bool xD3D10RenderApi::setInputAssembler(IInputAssembler* pAssembler)
{
    m_pInputAssembler = pAssembler;
    xD10InputAssembler* assembler = dynamic_cast<xD10InputAssembler*>(pAssembler);
    ID3D10InputLayout* pILayout = assembler->d10Layout();
    m_pD3DDevice->IASetInputLayout( pILayout );
    return true;
}

bool xD3D10RenderApi::setIndexBuffer(IInputBuffer* pVertexBuffer , size_t iOffset)
{
      xD10InputBuffer* d10IB = dynamic_cast<xD10InputBuffer*>(pVertexBuffer);
      UINT Offset  = (UINT)iOffset;
      UINT uBPB = (UINT)d10IB->m_nBytePerVertex;	
      DXGI_FORMAT df = (uBPB == 2 ? DXGI_FORMAT_R16_UINT  : DXGI_FORMAT_R32_UINT );
      m_pD3DDevice->IASetIndexBuffer(d10IB->m_pBuffer , df , Offset);
      return true;
}

bool xD3D10RenderApi::setVertexBuffer(IInputBuffer* pVertexBuffer ,   size_t bufIdx , size_t stride , size_t iOffset )
{
      xD10InputBuffer* d10IB = dynamic_cast<xD10InputBuffer*>(pVertexBuffer);
      UINT Stride  = (UINT)stride;
      UINT Offset  = (UINT)iOffset;
      if(stride == 0 ) Stride = (UINT)d10IB->m_nBytePerVertex;	
      m_pD3DDevice->IASetVertexBuffers((UINT)bufIdx , 1 , &d10IB->m_pBuffer , &Stride , &Offset );
      return true;
}

bool xD3D10RenderApi::setVertexStream(IVertexStream* vertexStream)
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

bool xD3D10RenderApi::drawPrimitive(size_t nVertex , size_t iStartVertex , ePrimtiveType pt)
{
	if(m_pCallback)
	{
		m_pCallback->preDrawPrimitive(this , m_RenderMode);
		setPrimitiveType(pt);
		m_pD3DDevice->DrawIndexed( (UINT)nVertex , (UINT)iStartVertex , 0 );
		m_pCallback->preDrawPrimitive(this , m_RenderMode);
	}
	else
	{
		setPrimitiveType(pt);
		m_pD3DDevice->DrawIndexed( (UINT)nVertex , (UINT)iStartVertex , 0 );
	}
	return true;
}

bool  xD3D10RenderApi::drawRectf(IBaseTexture* pTexture, float vDestRect[4] , const xColor_4f& color)
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

bool xD3D10RenderApi::drawRectf(IBaseTexture* pTexture, float vDestRect[4] ,float vSrcRect[4]  , const xColor_4f& color)
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
      return new xD3D2DRect(m_pDef2DRectEnv);
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

IShaderCodeGenerator* xD3D10RenderApi::createShaderCodeGenerator(const unsigned char * _code)
{
	return NULL; 
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

bool xD3D10RenderApi::setGpuProgram(IGpuProgram* pGpuProgram)
{
      if( m_pGpuProgram.equal(pGpuProgram) )
            return false;

	  xD10GpuProgram* pD10GpuProgram = (xD10GpuProgram*)pGpuProgram;
      m_pGpuProgram = pGpuProgram;//pD10GpuProgram;
      if(pD10GpuProgram)
      {		    
            setVertexShader(pD10GpuProgram->m_pVertexShader);
            setPixelShader(pD10GpuProgram->m_pPixelShader);
            setGeomShader(pD10GpuProgram->m_pGeomShader);
      }
      return true;
}

bool xD3D10RenderApi::setShader(IBaseShader* pShader , eShaderType _type)
{
	if(pShader)
        _type = pShader->type();
	if(_type == eShader_PixelShader)
		return setPixelShader(pShader);
	if(_type == eShader_VertexShader)
		return setVertexShader(pShader);
	if(_type == eShader_GeometryShader)
		return setGeomShader(pShader);
	return false;
}

bool xD3D10RenderApi::setVertexShader(IBaseShader* pVertShader)
{
      if( pVertShader == NULL || pVertShader->isLoaded() == false)
      {
            m_pD3DDevice->VSSetShader( NULL );
            return true;
      }
#ifdef _DEBUG
      xD10VertexShader* pVS = dynamic_cast<xD10VertexShader*> (pVertShader);
#else
      xD10VertexShader* pVS = (xD10VertexShader*) pVertShader;
#endif
      m_pD3DDevice->VSSetShader( pVS->m_pVertexShader );
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
            m_pD3DDevice->PSSetShader( NULL );
            return true;
      }
#ifdef _DEBUG
      xD10PixelShader* pPS = dynamic_cast<xD10PixelShader*> (pPixelShader);
#else
      xD10PixelShader* pPS = (xD10PixelShader*) pPixelShader;
#endif
      m_pD3DDevice->PSSetShader( pPS->m_pPixelShader );
      return true;
}

bool xD3D10RenderApi::setGeomShader(IBaseShader* pGeomShader)
{
      if(pGeomShader == NULL || pGeomShader->isLoaded() == false)
      {
            m_pD3DDevice->GSSetShader( NULL );
            return true;
      }
#ifdef _DEBUG
      xD10GeomShader* pGS = dynamic_cast<xD10GeomShader*> (pGeomShader);
#else
      xD10GeomShader* pGS = (xD10GeomShader*) pGeomShader;
#endif
      m_pD3DDevice->GSSetShader( pGS->m_pGeomShader );
      return true;
}

bool  xD3D10RenderApi::setConstantBuffers(eShaderType shType , int iSlot , int nResource , ID3D10Buffer** pConstBuffer)
{
	if(shType == eShader_VertexShader )
	{
		d10Device()->VSSetConstantBuffers( iSlot , nResource , pConstBuffer);
	}
	else if(shType == eShader_PixelShader)
	{
		d10Device()->PSSetConstantBuffers( iSlot , nResource , pConstBuffer);
	}
	else if(shType == eShader_GeometryShader)
	{
		d10Device()->GSSetConstantBuffers( iSlot , nResource , pConstBuffer);  
	}

	return true;
}

bool  xD3D10RenderApi::setShaderResource(eShaderType _st , int iSlot , int nResource , ID3D10ShaderResourceView** pResourceView)
{
	HRESULT hRet = E_FAIL;
	switch(_st)
	{
	case eShader_VertexShader:
		d10Device()->VSSetShaderResources(iSlot , 1 , pResourceView );
		return true;
		break;
	case eShader_PixelShader:
		d10Device()->PSSetShaderResources(iSlot , 1 , pResourceView );
		return true;
		break;
	case eShader_GeometryShader:
		d10Device()->GSSetShaderResources(iSlot , 1 , pResourceView );
		return true;


		return true;

		break;
	}
	return SUCCEEDED(hRet);
}

bool  xD3D10RenderApi::setShaderResource(eShaderType _st , int iSlot , IBaseTexture* pTexture)
{
	  ID3D10ShaderResourceView* pResourceView = pTexture?(ID3D10ShaderResourceView*)pTexture->handle() : NULL;
      return setShaderResource(_st , iSlot , 1 , &pResourceView);
}


bool  xD3D10RenderApi::setShaderResource(eShaderType _st , int iSlot ,IInputBuffer* pBuffer , ePIXEL_FORMAT fmt)
{
	xD10InputBuffer* pD11InputBuffer = dynamic_cast<xD10InputBuffer*>(pBuffer);
	ID3D10ShaderResourceView* pResourceView = pD11InputBuffer?(ID3D10ShaderResourceView*)pD11InputBuffer->toShaderResourceView( fmt ) : NULL;
	return setShaderResource(_st , iSlot , 1 , &pResourceView);
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

IBaseTexture* xD3D10RenderApi::createFileTexture(const wchar_t* texFile , const unsigned int8* buf , unsigned int bufLen, unsigned int arg, const xTextureInitDesc* texInitDesc)
{
      xD10FileTexture* pTex = new xD10FileTexture(this);
      if(pTex->load(texFile , buf , bufLen , arg ) )
      {
            return pTex;
      }
      pTex->KillObject();
      return NULL;
}

const wchar_t* xD3D10RenderApi::texCoordStyle()
{
     return L"Direct3D";
}

IBaseTexture*  xD3D10RenderApi::createFileTexture(const wchar_t* extFile, const xTextureInitDesc* texInitDesc)
{
      return new xD10FileTexture(this);
}


IBaseTexture* xD3D10RenderApi::createTexture(const xTextureInitDesc& initDesc , xTextureInitData* pInitData , int nInitData )
{
      bool bReadable =  initDesc.m_bReadable ;
      xD10LockableTexture* pTexture = new xD10LockableTexture(this , bReadable);
      if(pTexture->create(initDesc , pInitData , nInitData) )
      {
            return pTexture;
      }
      return NULL;	
}

IRenderCamera* xD3D10RenderApi::createCamera(const wchar_t* cameraName)
{
      return m_MatrixCtx->createCamera(cameraName);
}

IBaseTexture *xD3D10RenderApi::createRenderableTexture(int w , int h , int depth , ePIXEL_FORMAT fmt , bool bReadable ,  int nMipMap, int nArraySize  , const xRTSampleDesc& sampleQulity )
{
      if(depth != 1 && bReadable)
      {
            XEVOL_LOG(eXL_DEBUG_HIGH,"3D Render To Texture cann't created as readable\n");
            bReadable = false;
      }
      xD10RenderTexture* pRenderTexture = new xD10RenderTexture(bReadable , true , this , GetDXGISampleDesc(sampleQulity)  );
      
      xTextureInitDesc  initDesc(w , h , fmt , depth);
      initDesc.m_bReadable = bReadable;
      initDesc.m_access = RESOURCE_ACCESS_NONE;
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

IRenderTarget* xD3D10RenderApi::createRenderTarget(int w , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture , const xRTSampleDesc& sampleQulity)
{
      xD10RenderTexture* pRenderTexture = new xD10RenderTexture(bLockable , bAsTexture , this , GetDXGISampleDesc(sampleQulity) );
      xTextureInitDesc initDesc(w , h , fmt);
      initDesc.m_bReadable = bLockable;
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

IRenderTarget* xD3D10RenderApi::createDepthBuffer(int w  , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture , const xRTSampleDesc& sampleQulity)
{
      xD10DepthTexture* pDepthTexture = new xD10DepthTexture(bLockable , bAsTexture , this ,  GetDXGISampleDesc(sampleQulity)  );
       xTextureInitDesc initDesc(w , h , fmt);
       initDesc.m_bReadable = bLockable;
      if( false == pDepthTexture->create(initDesc , NULL , 0 ) )
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

IRenderView* xD3D10RenderApi::createRenderView(int w , int h , bool bCreateDepth , const xRTSampleDesc& sampleDesc)
{
      xD10RenderView* pRenderView = new xD10RenderView(this , sampleDesc);
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

void xD3D10RenderApi::getRTSampleDesc(xRTSampleDesc& _desc)
{
    if(m_RenderWindow == NULL)
        return ;

	m_RenderWindow->getRTSampleDesc(_desc);
}

DXGI_SAMPLE_DESC xD3D10RenderApi::GetDXGISampleDesc(const xRTSampleDesc& sampleDesc)
{
	DXGI_SAMPLE_DESC SampleDesc = { sampleDesc.m_SampleCount , sampleDesc.m_SampleQulity };
	if(sampleDesc.m_SampleCount == - 1)
	{
		GetDXGISampleDesc(SampleDesc);
	}
	return SampleDesc;
}

IRenderView* xD3D10RenderApi::createRenderView(int w , int h ,void* hWnd ,bool bCreateDepth , const xRTSampleDesc& sampleDesc)
{
      return NULL;
}


bool xD3D10RenderApi::setRenderView(IRenderView* renderView)
{
      GUID_SELECT_MODE(false);
      xD10RenderView* pD10RV = (xD10RenderView*) renderView;
      m_RenderView = pD10RV;
      return pD10RV->install();
}

ILightingEnv* xD3D10RenderApi::createLightingState(const wchar_t* _name)
{
      xD1xLightingEnv* pLightingEnv = new xD1xLightingEnv(this , _name);
      m_objManager.LightEnvMgr().add(_name , pLightingEnv);
      return pLightingEnv;
}

END_NAMESPACE_XEVOL3D