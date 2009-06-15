#include <BaseLib/xEvol3DAPI.h>
#include <BaseLib/xXmlDocument.h>
#include <OperationSys/xOperationSys.h>
#include "xOpenGL2API.h"
#include "xGL2_InputAssembler.h"
#include "xGL2InputBuffer.h"
#include "xGL2Shader.h"
#include "xGL2GPUProgram.h"
#include "xGL2FileTexture.h"
#include "xGL2LockableTexture.h"
#include "xGL2Sampler.h"
#include "xGL2RazState.h"
#include "xGL2BlendState.h"
#include "xGL2DepthStencilState.h"
#include "xGL2RenderTarget.h"
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"dxgi.lib")
#ifdef _DEBUG
#pragma comment(lib,"d3dx10d.lib")
#else
#pragma comment(lib,"d3dx10.lib")
#endif

#include "xGL2HelperRender.h"
#include "xGL2RenderView.h"
#include "xGL2BaseColorSelector.h"
#include "Image/PixelFormatAssit.h"
#include "fs/xFileSystem.h"
#include "xGL2TLEnviroment.h"
BEGIN_NAMESPACE_XEVOL3D

#define GUID_SELECT_MODE(_value)  if( m_RenderMode == eRenderMode_Select ) return _value;
xGL2RenderApi::xGL2RenderApi()
:m_GpuProgramMgr(L"GpuProgramMgr" , 1)
,m_ShaderManager(L"ShaderManager" , 1)
,m_StateObjMgr(L"RdrStateManager"    , 1)
{
      m_RefCount = 1;
      for(int i = 0 ; i < Texture_StageMax ; i ++)
      {
            m_texLayer[i] = NULL;
      }
      m_pGL2Device  = NULL;
      m_PrimType   = ePrimtiveType_Triangles;
      m_pGpuProgram = NULL;
      m_pInputAssembler = NULL;
      m_pVertStream  = NULL;
      //设备属性和参数
      m_hMainWnd = NULL;
      m_Width    = 0 ;
      m_Height   = 0;
      m_hInst    = NULL;
      m_driverType = GL2_DRIVER_TYPE_HARDWARE;	
      m_pSwapChain = NULL;
      m_RenderWindow = NULL;
      memset(&m_swapChainDesc , 0 , sizeof(DXGI_SWAP_CHAIN_DESC) );
      m_pDef2DRect    = NULL;
      m_pDef2DRectEnv  = NULL;
      m_2DZValue      = 0.0f;

      m_GpuProgramMgr.setRenderApi(this , L".glsl");
      m_ShaderManager.setRenderApi(this , m_GpuProgramMgr.codeManager() , L".glsl");
      m_StateObjMgr.setRenderApi(this);
      m_hInst             = GetModuleHandle(NULL);
      m_driverType        = GL2_DRIVER_TYPE_NULL;
      m_pSwapChain        = NULL;
      m_pGL2Device      = NULL;
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
      m_LightingState = NULL;
      clearAllTexture();
      enableThreadLocker(true); 
}

IColorSelector* xGL2RenderApi::colorSelector()
{
      return m_pColorSelector;
}

bool xGL2RenderApi::setColorSelector(IColorSelector* pColorSel)
{
      if(pColorSel == NULL) 
      {
            m_pColorSelector = m_pBaseSelector;
            return true;
      }
      m_pColorSelector = pColorSel;
      return true;
}


eRenderMode xGL2RenderApi::renderMode()
{
      return m_RenderMode;
}


bool xGL2RenderApi::beginSelectMode()
{
      if(m_pColorSelector == NULL)
            return false;
      m_pColorSelector->begin();
      m_RenderMode = eRenderMode_Select;
      return true;
}

bool xGL2RenderApi::endSelectMode(int x , int y , int _id[2] )
{
      if(m_pColorSelector == NULL)
            return false ;
      m_RenderMode = eRenderMode_Render;
      m_pColorSelector->end();
      return m_pColorSelector->getSelectID(x , y , 1 , 1 , _id);
}

bool xGL2RenderApi::endSelectMode(int x , int y , int w , int h , void* _id)
{
      if(m_pColorSelector == NULL)
            return false ;
      m_RenderMode = eRenderMode_Render;
      m_pColorSelector->end();
      return m_pColorSelector->getSelectID(x , y , w , h , _id);
}

xGL2RenderApi::~xGL2RenderApi()
{
      uninit();
}

ID3D10Device*  xGL2RenderApi::d10Device()
{
      return m_pGL2Device ; 
}

bool xGL2RenderApi::uninit( )
{
      XSAFE_RELEASEOBJECT(m_LightingState);
      freeAllTextureManager();
      m_StateObjMgr.clear();
      m_FontManager.clear();
      m_GpuProgramMgr.clear();
      m_ShaderManager.clear();
      XSAFE_DELETE(m_pBaseSelector);
      XSAFE_DELETE(m_RenderWindow);
      KillRenderObject();
      XSAFE_RELEASE(m_pGL2Device);
      return true;
}
bool xGL2RenderApi::getWindowSize(int& w , int & h)
{
    RECT _Rect;
    GetClientRect(m_hMainWnd , &_Rect);
    w = _Rect.right  - _Rect.left;
    h = _Rect.bottom - _Rect.top;
    return true;
}

bool xGL2RenderApi::getRenderViewSize(int& w , int & h)
{
    if(m_RenderView == NULL)
        return false;

    xTextureDesc _desc;
    m_RenderView->desc(_desc);
    w = _desc.m_width;
    h = _desc.m_height;
    return true;
}

bool xGL2RenderApi::init(xXmlNode* pSysNode)
{
      //IRenderApi::init(pSysNode);
      TAutoLocker<xGL2Locker> aLocker(m_pDevLocker);
      xXmlNode* pRendererNode = pSysNode->findNode( name() , 0 );
      xXmlNode* pShaderMgrNode = pRendererNode->findNode(L"ShaderManager",0);

      //Load Shader Manager configuration
      if(pShaderMgrNode)
      {
            xXmlNode::XmlNodes pathNodes ;
            pShaderMgrNode->findNode(L"path",pathNodes);
            for(size_t i = 0 ; i < pathNodes.size() ; i ++)
            {
                  xXmlNode* pPathNode = pathNodes[i];
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

            xXmlNode* pPreGenNode = pShaderMgrNode->findNode(L"PreGen");
            if(pPreGenNode)
            {
                  const wchar_t* strPath =  pPreGenNode->value(L"path");
                  bool           bPreGen =  pPreGenNode->bool_value(L"enable");
                  if(strPath) m_ShaderManager.setOutputForlder( _ABSPATH (strPath) );
                  m_ShaderManager.enablePreGenerator(bPreGen);
            }
      }

      xXmlNode* pStateMgrNode = pRendererNode->findNode(L"StateManager",0);

      //Load State Manager configuration
      if(pStateMgrNode)
      {
            xXmlNode::XmlNodes pathNodes ;
            pStateMgrNode->findNode(L"path",pathNodes);
            for(size_t i = 0 ; i < pathNodes.size() ; i ++)
            {
                  xXmlNode* pPathNode = pathNodes[i];
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

      xXmlNode* pFontNode = pSysNode->findNode(L"fontconfig");
      if(pFontNode)
      {
            m_FontManager.load(pFontNode);
      }
      //load other configuration

      m_FontRenderDevice->init();

      //现在来初始化选择Buffer;

      xXmlNode* pSelNode = pRendererNode->findNode(L"PixelSelect");//  pSysNode->bool_value(L"pixel_select");
      if(pSelNode && pSelNode->bool_value(L"enable"))
      {
            m_pBaseSelector = new xGL2BaseColorSelector(this);
            m_pBaseSelector->create(pSelNode ) ;
            m_pColorSelector = m_pBaseSelector;
      }

      //========建立光照环境====================
      m_LightingState = createLightingState(L"xLightState");
      return true;
}



bool xGL2RenderApi::enableThreadLocker(bool bEnable)
{
      if(bEnable == false) 
            m_pDevLocker = NULL;
      else
            m_pDevLocker = &m_RenderLocker;//&m_devLocker;

      return true;
}


bool xGL2RenderApi::create(ID3D10Device* pDevice , HWND hWnd , int w , int h)
{
      TAutoLocker<xGL2Locker> aLocker(m_pDevLocker);
      //MessageBoxA(NULL,"开始创建GL2 Api","错误",MB_OK);
      m_pGL2Device      = pDevice;
      m_hMainWnd          = hWnd;
      m_Width             = w;
      m_Height            = h;
      bool ret = false;
      if(m_pGL2Device) 
      { 
            ret = __initByDevice();
      }
      else
      {
            ret = __createD10Device();
      }
      //MessageBoxA(NULL,"创建GL2 Api完成","错误",MB_OK);
      if(ret == false)
      {
            XEVOL_LOG(eXL_ERROR_FALT,"Create Direct3D 10 Failed\n");
      }

      __afterCreateDevice();
      enableThreadLocker(true);
      return ret;
}

bool xGL2RenderApi::onResize(int width , int height)
{
      TAutoLocker<xGL2Locker> aLocker(m_pDevLocker);
      TAutoLocker<xGL2Locker> aRenderLocker(&m_RenderLocker);
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

bool xGL2RenderApi::__needResize(int width , int height)
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

bool xGL2RenderApi::__resetViewPort()
{
      // Setup the viewport
      GL2_VIEWPORT vp;
      vp.Width  = m_Width;
      vp.Height = m_Height;
      vp.MinDepth = 0.0f;
      vp.MaxDepth = 1.0f;
      vp.TopLeftX = 0;
      vp.TopLeftY = 0;
      m_pGL2Device->RSSetViewports( 1, &vp );
      return true;
}

bool xGL2RenderApi::__createD10Device()
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
      createDeviceFlags |= GL2_CREATE_DEVICE_DEBUG;
#endif

      GL2_DRIVER_TYPE driverTypes[] = 
      {
            GL2_DRIVER_TYPE_HARDWARE,            
            GL2_DRIVER_TYPE_REFERENCE,
            GL2_DRIVER_TYPE_REFERENCE,
            GL2_DRIVER_TYPE_REFERENCE,
            GL2_DRIVER_TYPE_REFERENCE,
            GL2_DRIVER_TYPE_REFERENCE,
      };
      HMODULE hDriverModule[]=
      {
            NULL,
            LoadLibraryA( "GL2WARP_beta.DLL" ),
            LoadLibraryW( _ABSPATH(L"GL2WARP_beta.DLL") ),
            LoadLibraryA( "GL2WARP.DLL" ),
            LoadLibraryW( _ABSPATH(L"GL2WARP.DLL" ) ),
            NULL,
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

      //Find all Adapter
      IDXGIAdapter * pAdapter;
      std::vector<IDXGIAdapter*> vAdapters;
      IDXGIFactory* pDXGIFactory = NULL;
       hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pDXGIFactory) );

      while(pDXGIFactory->EnumAdapters(vAdapters.size(), &pAdapter) != DXGI_ERROR_NOT_FOUND)
      {
            vAdapters.push_back(pAdapter);
      }


      for(int i = 0 ; i < vAdapters.size() ; i ++)
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

            for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
            {
                  m_driverType = driverTypes[driverTypeIndex];
                  HMODULE hModule = hDriverModule[driverTypeIndex];
                  hr = GL2CreateDeviceAndSwapChain( pAdapter, m_driverType, hModule , createDeviceFlags, GL2_SDK_VERSION, &m_swapChainDesc, &m_pSwapChain, &m_pGL2Device );
                  if( SUCCEEDED( hr ) )
                        break;
            }
            if(m_pGL2Device && m_pSwapChain)
            {
                  break;
            }
      }
   


      if( FAILED(hr) )
            return false;


      m_RenderWindow = new xGL2RenderWindow(m_hMainWnd , this);
      m_RenderWindow->create(m_pSwapChain , width , height);
      __resetViewPort();
      setRenderView(m_RenderWindow);

      GL2_BLEND_DESC BlendState;
      ZeroMemory(&BlendState, sizeof(GL2_BLEND_DESC));
      BlendState.BlendEnable[0] = FALSE;
      BlendState.RenderTargetWriteMask[0] = GL2_COLOR_WRITE_ENABLE_ALL;

      ID3D10BlendState*       g_pBlendStateNoBlend = NULL;
      ID3D10RasterizerState*	g_pRasterizerStateNoCull = NULL;
      m_pGL2Device->CreateBlendState(&BlendState, &g_pBlendStateNoBlend) ;

      // Create a rasterizer state to disable culling
      GL2_RASTERIZER_DESC RSDesc;
      RSDesc.FillMode = GL2_FILL_SOLID;
      RSDesc.CullMode = GL2_CULL_NONE;
      RSDesc.FrontCounterClockwise = TRUE;
      RSDesc.DepthBias = 0;
      RSDesc.DepthBiasClamp = 0;
      RSDesc.SlopeScaledDepthBias = 0;
      RSDesc.ScissorEnable = FALSE;
      RSDesc.MultisampleEnable = TRUE;
      RSDesc.AntialiasedLineEnable = FALSE;
      m_pGL2Device->CreateRasterizerState(&RSDesc, &g_pRasterizerStateNoCull) ;

      m_pGL2Device->OMSetBlendState(g_pBlendStateNoBlend, 0, 0xffffffff);
      m_pGL2Device->RSSetState(g_pRasterizerStateNoCull);
      return true;
}

bool xGL2RenderApi::__destory()
{
      XSAFE_RELEASEOBJECT(m_RenderWindow);
      XSAFE_RELEASE(m_pSwapChain);
      XSAFE_RELEASE(m_pGL2Device);
      XSAFE_RELEASE(m_defDepthStencilState);
      XSAFE_RELEASE(m_defBlendState);
      XSAFE_RELEASE(m_defpRasState);
      return true;

}
bool xGL2RenderApi::__afterCreateDevice()
{
      m_pGL2Device->OMGetDepthStencilState( &m_defDepthStencilState , &m_DefStencilRef);
      m_pGL2Device->OMGetBlendState(&m_defBlendState , m_DefBlendFactor , &m_DefSampleMask);
      m_pGL2Device->RSGetState(&m_defpRasState);
      m_2DZValue = 0.0f;
      m_pDef2DRectEnv = new xGL2Rect2DEnv(this);
      m_pDef2DRect    = new xGL2Rect2D(m_pDef2DRectEnv);
      return true;   
}

bool xGL2RenderApi::__initByDevice()
{
      XEVOL_WARNNING_NOT_IMPLEMENT
            return false;
}

static int g_iLocked  = 0;
bool xGL2RenderApi::lock()
{
      m_RenderLocker.lock();
      g_iLocked ++;
      return true;
}

bool xGL2RenderApi::unlock()
{
      g_iLocked --;
      m_RenderLocker.unlock();
      return true;
}

bool xGL2RenderApi::begin(xColor_4f& bkColor , float z , unsigned int stencil)
{
      lock();
      TAutoLocker<xGL2Locker> aLocker(m_pDevLocker);
      return m_RenderView->clear(bkColor , z , stencil );
}

bool xGL2RenderApi::beginScene()
{
      //XEVOL_WARNNING_NOT_IMPLEMENT
      lock();
      return true;
}

bool xGL2RenderApi::endScene()
{
      //XEVOL_WARNNING_NOT_IMPLEMENT
      unlock();
      return true;
}

bool xGL2RenderApi::swapBuffer()
{
      TAutoLocker<xGL2Locker> aLocker(m_pDevLocker);
      m_pSwapChain->Present( 0, 0 );
      unlock();
      return true;
}

ISamplerState* xGL2RenderApi::createSamplerState(const wchar_t* name)
{
      std::wstring full_name = name + std::wstring(L".sampler");
      HRenderState hState = this->m_StateObjMgr.add(full_name.c_str() , (int)(eRenderState_Sampler) , true);
      return (ISamplerState*) hState.getResource();
}

IBlenderState* xGL2RenderApi::createBlendState(const wchar_t* name)
{
      std::wstring full_name = name + std::wstring(L".blend");
      HRenderState hState = this->m_StateObjMgr.add(full_name.c_str() , (int)(eRenderState_Blend) , true);
      return (IBlenderState*) hState.getResource();
}

IRasterizerState* xGL2RenderApi::createRasterizerState(const wchar_t* name)
{
      std::wstring full_name = name + std::wstring(L".ras");
      HRenderState hState = this->m_StateObjMgr.add(full_name.c_str() , (int)(eRenderState_Rasterizer) , true);
      return (IRasterizerState*) hState.getResource();
}

IDepthStencilState*  xGL2RenderApi::createDepthStencilState(const wchar_t* name)
{
      std::wstring full_name = name + std::wstring(L".zs");
      HRenderState hState = this->m_StateObjMgr.add(full_name.c_str() , (int)(eRenderState_DepthStencil) , true);
      return (IDepthStencilState*) hState.getResource();
}

IBlenderState*  xGL2RenderApi::getBlendState()
{
      return m_BlenderState;
}

IRasterizerState* xGL2RenderApi::getRasterizerState()
{
      return m_RasterizerState;
}

IDepthStencilState*  xGL2RenderApi::getDepthStencilState()
{
      return m_DepthStencilState;
}

bool xGL2RenderApi::setBlendState(IBlenderState* pState)
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

bool xGL2RenderApi::setRasterizerState(IRasterizerState* pState)
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


bool xGL2RenderApi::setDepthStencilState(IDepthStencilState* pState)
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

IRenderState* xGL2RenderApi::createRenderState(eRenderStateType _type)
{
      switch(_type)
      {
      case eRenderState_Sampler:
            return new xGL2Sampler(this);
      case eRenderState_Blend:
            return new xGL2BlendState(this);
            return NULL;
      case eRenderState_Rasterizer:
            return new xGL2RazState(this);
            return NULL;
      case eRenderState_DepthStencil:
            return new xGL2ZStencilState(this);
      }
      return NULL;
}

IInputAssembler* xGL2RenderApi::getInputAssembler(const wchar_t* name)
{
      //First create a assembler
      xGL2InputAssembler* assembler = objectManager().inputAssemblerMgr().find(name);
      if(assembler) 
      {
            assembler->AddRef();
            return assembler ;
      }
      return NULL;
}


IInputAssembler* xGL2RenderApi::createInputAssembler(const wchar_t* name , xInputLayoutDesc& desc )
{
      //First create a assembler
      xGL2InputAssembler* assembler = objectManager().inputAssemblerMgr().find(name);
      if(assembler) 
      {
            assembler->AddRef();
            return assembler ;
      }

      //Not found
      assembler = new xGL2InputAssembler(this , name , desc);
      if( assembler->create() )
      {
            objectManager().inputAssemblerMgr().add(name , assembler);
            return assembler;
      }
      assembler->KillObject();
      return NULL;
}

IInputBuffer*    xGL2RenderApi::createInputBuffer(size_t nVertex , size_t nBytePerVertex , const xInputBufferDesc* pBufferDesc , void* pData)
{
      xGL2InputBuffer* vBuffer = new xGL2InputBuffer(this);

      bool ret  = vBuffer->create(nVertex ,  nBytePerVertex , pBufferDesc , pData) ;
      if(ret == false )
      {
            XSAFE_KILLOBJECT( vBuffer );
            return NULL;
      }
      return vBuffer;
}

IInputBuffer*  xGL2RenderApi::createBuffer(size_t iBufLen ,const xInputBufferDesc* pBufferDesc , void* pData)
{
      xGL2InputBuffer* vBuffer = new xGL2InputBuffer(this);

      bool ret  = vBuffer->create(iBufLen , pBufferDesc , pData) ;
      if(ret == false )
      {
            XSAFE_KILLOBJECT( vBuffer );
            return NULL;
      }
      return vBuffer;
}

IInputBuffer* xGL2RenderApi::findInputBuffer(const wchar_t* _name)
{
      IInputBuffer* pBuffer = m_objManager.InputBufferMgr().find(_name);
      return pBuffer;
}

bool xGL2RenderApi::removeInputBuffer(IInputBuffer* pConstBuffer)
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

bool xGL2RenderApi::removeInputBuffer(const wchar_t* _name)
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

bool xGL2RenderApi::setInputBuffer(const wchar_t* _name, IInputBuffer* pConstBuffer)
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

IInputBuffer* xGL2RenderApi::createInputBuffer(size_t iBufLen)
{
      xInputBufferDesc cbDesc;
      cbDesc.m_bindtype    = BIND_AS_SHADER_RESOURCE ;
      cbDesc.m_accessFlage = RESOURCE_ACCESS_WRITE;
      cbDesc.m_usage       = RESOURCE_USAGE_DYNAMIC;
      return createBuffer(iBufLen , &cbDesc, NULL);
}

IInputBuffer* xGL2RenderApi::createConstBuffer(size_t iBufLen)
{
      xInputBufferDesc cbDesc;
      cbDesc.m_bindtype    = BIND_AS_CONSTANT_BUFFER ;
      cbDesc.m_accessFlage = RESOURCE_ACCESS_WRITE;
      cbDesc.m_usage       = RESOURCE_USAGE_DYNAMIC;
      return createBuffer(iBufLen , &cbDesc, NULL);
}

bool xGL2RenderApi::setPrimitiveType(ePrimtiveType type)
{
      m_PrimType = type;
      GL2_PRIMITIVE_TOPOLOGY d3d10PtLogicy = (GL2_PRIMITIVE_TOPOLOGY)m_PrimType;
      m_pGL2Device->IASetPrimitiveTopology( d3d10PtLogicy );
      return false;
}

ePrimtiveType xGL2RenderApi::getPrimitiveType()
{
      GL2_PRIMITIVE_TOPOLOGY type;
      m_pGL2Device->IAGetPrimitiveTopology( &type );
      return (ePrimtiveType)type;
}

bool xGL2RenderApi::setInputAssembler(IInputAssembler* pAssembler)
{
    m_pInputAssembler = pAssembler;
    xGL2InputAssembler* assembler = dynamic_cast<xGL2InputAssembler*>(pAssembler);
    ID3D10InputLayout* pILayout = assembler->d10Layout();
    m_pGL2Device->IASetInputLayout( pILayout );
    return true;
}

IInputAssembler*  xGL2RenderApi::getInputAssembler()
{
      return m_pInputAssembler;
}

bool xGL2RenderApi::setIndexBuffer(IInputBuffer* pVertexBuffer , size_t iOffset)
{
      xGL2InputBuffer* d10IB = dynamic_cast<xGL2InputBuffer*>(pVertexBuffer);
      UINT Offset  = (UINT)iOffset;
      UINT uBPB = (UINT)d10IB->m_nBytePerVertex;	
      DXGI_FORMAT df = (uBPB == 2 ? DXGI_FORMAT_R16_UINT  : DXGI_FORMAT_R32_UINT );
      m_pGL2Device->IASetIndexBuffer(d10IB->m_pBuffer , df , Offset);
      return true;
}

bool xGL2RenderApi::setVertexBuffer(IInputBuffer* pVertexBuffer ,   size_t bufIdx , size_t stride , size_t iOffset )
{
      xGL2InputBuffer* d10IB = dynamic_cast<xGL2InputBuffer*>(pVertexBuffer);
      UINT Stride  = (UINT)stride;
      UINT Offset  = (UINT)iOffset;
      if(stride == 0 ) Stride = (UINT)d10IB->m_nBytePerVertex;	
      m_pGL2Device->IASetVertexBuffers((UINT)bufIdx , 1 , &d10IB->m_pBuffer , &Stride , &Offset );
      return true;
}

bool xGL2RenderApi::setVertexStream(IVertexStream* vertexStream)
{
      xGL2VertexStream* d10VertStream = dynamic_cast<xGL2VertexStream*>(vertexStream);
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

bool xGL2RenderApi::beginDrawPrimitive()
{
      if(m_pGpuProgram==NULL)
      {
            XEVOL_LOG(eXL_ERROR_FALT,L"OpenGL2中,不设置Shader是不能进行绘制的\n");
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
bool xGL2RenderApi::endDrawPrimitive()
{
      if(m_RenderMode == eRenderMode_Select && m_pColorSelector)
      {
            m_pColorSelector->endPrimitive();
      }
      return true;
}

bool xGL2RenderApi::drawPrimitive(size_t nVertex , size_t iStartVertex , ePrimtiveType pt)
{
      setPrimitiveType(pt);
      m_pGL2Device->DrawIndexed( (UINT)nVertex , (UINT)iStartVertex , 0 );
      return true;
}

bool xGL2RenderApi::draw(IInputBuffer* pIdxBuffer , size_t nVertex , size_t iStartVertex, ePrimtiveType pt)
{
      TAutoLocker<xGL2Locker> aLocker(m_pDevLocker);
      setIndexBuffer(pIdxBuffer);

      if(false == beginDrawPrimitive() ) 
            return false;

      bool bDrawRet = drawPrimitive(nVertex , iStartVertex , pt);

      bool bRet = endDrawPrimitive();

      return bDrawRet&bRet;
}

bool xGL2RenderApi::draw(size_t nVertex , size_t iStartVertex, ePrimtiveType pt)
{
      TAutoLocker<xGL2Locker> aLocker(m_pDevLocker);
      if(false == beginDrawPrimitive() ) return false;

      bool bDrawRet = drawPrimitive(nVertex , iStartVertex , pt);
      bool bRet = endDrawPrimitive();

      return bDrawRet&bRet;
}

bool xGL2RenderApi::set2DZValue(float zValue)
{
      m_2DZValue = zValue;
      return true;
}

float xGL2RenderApi::get2DZValue()
{
      return m_2DZValue;
}

bool  xGL2RenderApi::drawRectf(IBaseTexture* pTexture, float vDestRect[4] , xColor_4f& color)
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

bool xGL2RenderApi::drawRectf(IBaseTexture* pTexture, float vDestRect[4] ,float vSrcRect[4]  , xColor_4f& color)
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

void xGL2RenderApi::set2DTexture(int iStage , IBaseTexture* pTexture)
{
      m_pDef2DRect->setTexture(iStage , pTexture);
}

void xGL2RenderApi::set2DTexture(int iStage , IBaseTexture* pTexture , float _srcRect[4])
{
      xvec4& srcRect =  *((xvec4*)_srcRect);;
      m_pDef2DRect->setTexture(iStage , pTexture, srcRect);
}

I2DRectObject* xGL2RenderApi::create2DRectObject()
{
      return new xGL2Rect2D(m_pDef2DRectEnv);
}

bool xGL2RenderApi::draw2DRect(I2DRectObject* p2DRect)
{
      p2DRect->commit();
      return p2DRect->draw();
}

IGpuProgram* xGL2RenderApi::createGpuProgram()
{
      return new xGL2GpuProgram(this);
}

IBaseShader* xGL2RenderApi::createShader(const wchar_t* shaderName , eShaderType type)
{
      HBaseShader hShader = m_ShaderManager.add(shaderName,(unsigned int)type , true );
      hShader.lock();
      return hShader.getResource();
}


IBaseShader* xGL2RenderApi::createShader(eShaderType type)
{
      if(type == eShader_VertexShader)
      {
            return new xGL2VertexShader(this);
      }

      if(type == eShader_PixelShader)
      {
            return new xGL2PixelShader(this);
      }

      if(type == eShader_GeometryShader)
      {
            return new xGL2GeomShader(this);
      }

      return NULL;
}

bool xGL2RenderApi::pushGpuProgram(IGpuProgram* pGpuProgram)
{
      xGL2GpuProgram* pGpuProg = (xGL2GpuProgram*)pGpuProgram;
      m_pGpuProgram.push();
      setGpuProgram(pGpuProgram);
      return true;
}

bool xGL2RenderApi::popGpuProgram()
{
      xGL2GpuProgram* pGpuProg = m_pGpuProgram.pop();
      if( pGpuProg ==  NULL)
            return false;
      setGpuProgram(pGpuProg);
      return true;
}

IGpuProgram* xGL2RenderApi::getGpuProgram( )
{
      return m_pGpuProgram;
}

bool xGL2RenderApi::setGpuProgram(IGpuProgram* pGpuProgram)
{
      if(m_pGpuProgram == pGpuProgram)
            return false;

      m_pGpuProgram = (xGL2GpuProgram*)pGpuProgram;
      if(m_pGpuProgram)
      {
            setVertexShader(m_pGpuProgram->m_pVertexShader);
            setPixelShader(m_pGpuProgram->m_pPixelShader);
            setGeomShader(m_pGpuProgram->m_pGeomShader);
      }
      return true;
}

bool xGL2RenderApi::setVertexShader(IBaseShader* pVertShader)
{
      if( pVertShader == NULL || pVertShader->isLoaded() == false)
      {
            m_pGL2Device->VSSetShader( NULL );
            return true;
      }
#ifdef _DEBUG
      xGL2VertexShader* pVS = dynamic_cast<xGL2VertexShader*> (pVertShader);
#else
      xGL2VertexShader* pVS = (xGL2VertexShader*) pVertShader;
#endif
      m_pGL2Device->VSSetShader( pVS->m_pVertexShader );
      return true;
}

bool xGL2RenderApi::setSamplerState(eShaderType _st , int iSlot , ISamplerState* pSampler)
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

bool xGL2RenderApi::setPixelShader(IBaseShader* pPixelShader)
{
      if(pPixelShader == NULL || pPixelShader->isLoaded() == false)
      {
            m_pGL2Device->PSSetShader( NULL );
            return true;
      }
#ifdef _DEBUG
      xGL2PixelShader* pPS = dynamic_cast<xGL2PixelShader*> (pPixelShader);
#else
      xGL2PixelShader* pPS = (xGL2PixelShader*) pPixelShader;
#endif
      m_pGL2Device->PSSetShader( pPS->m_pPixelShader );
      return true;
}

bool xGL2RenderApi::setGeomShader(IBaseShader* pGeomShader)
{
      if(pGeomShader == NULL || pGeomShader->isLoaded() == false)
      {
            m_pGL2Device->GSSetShader( NULL );
            return true;
      }
#ifdef _DEBUG
      xGL2GeomShader* pGS = dynamic_cast<xGL2GeomShader*> (pGeomShader);
#else
      xGL2GeomShader* pGS = (xGL2GeomShader*) pGeomShader;
#endif
      m_pGL2Device->GSSetShader( pGS->m_pGeomShader );
      return true;
}

bool  xGL2RenderApi::setShaderResource(eShaderType _st , int iSlot , IBaseTexture* pTexture)
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



bool  xGL2RenderApi::setTexture(const int iTexName      , IBaseTexture* pTexture)
{
      if(iTexName < Texture_StageMax)
      {
            m_texLayer[iTexName] = pTexture;
            return true;
      }
      m_mapTexLayers[iTexName] = pTexture;
      return true;
}

bool  xGL2RenderApi::setTexture(const wchar_t* iTexName , IBaseTexture* pTexture)
{
      if(iTexName == NULL)
            return false ;

      int iName = xStringHash(iTexName);

      if(iName < Texture_StageMax)
            return false;

      m_mapTexLayers[iName] = pTexture;
      return true;
}

IBaseTexture*  xGL2RenderApi::getTexture(const int iTexName)
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

IBaseTexture*  xGL2RenderApi::getTexture(const wchar_t* iTexName)
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

bool  xGL2RenderApi::clearAllTexture()
{
      for(int i = 0 ; i < Texture_StageMax ; i ++)
      {
            m_texLayer[i] = NULL;
      }
      m_mapTexLayers.clear();
      return true;
}
bool  xGL2RenderApi::setShaderResource(eShaderType _st , int iSlot ,IInputBuffer* pBuffer)
{
      return false;
}

bool xGL2RenderApi::isTextureSupport(ePIXEL_FORMAT fmt , bool lockable)
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

IShaderConstBinder*  xGL2RenderApi::getShaderConstBinder(const wchar_t* constName)
{
      int hashKey = xStringHash2(constName);
      mapConstBinder::iterator pos  = m_constNameBinder.find(hashKey);
      if(pos != m_constNameBinder.end() )
      {
            return pos->second;
      }
      return NULL;
}

void xGL2RenderApi::registeShaderConstBinder(const wchar_t* constName , IShaderConstBinder* pBinder)
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

IShaderConstBinder* xGL2RenderApi::getShaderConstBinder(eShaderParamSemantic _semantic)
{
      mapConstBinder::iterator pos  = m_semanticBinder.find( (int)_semantic);
      if(pos != m_semanticBinder.end() )
      {
            return pos->second;
      }
      return NULL;
}

void xGL2RenderApi::registeShaderConstBinder(eShaderParamSemantic _semantic , IShaderConstBinder* pBinder)
{
      if(getShaderConstBinder(_semantic) != NULL)
      {
            XEVOL_LOG(eXL_DEBUG_HIGH,L"你注册的Shader绑定器已经存在! name=%s\n" , _semantic);
            return ;
      }
      m_semanticBinder.insert(mapConstBinder::value_type( (int)_semantic , pBinder) ) ;
      return ;
}

IBaseTexture* xGL2RenderApi::createFileTexture(const wchar_t* texFile , const unsigned int8* buf , unsigned int bufLen, unsigned int arg)
{
      xGL2FileTexture* pTex = new xGL2FileTexture(this);
      if(pTex->load(texFile , buf , bufLen , arg ) )
      {
            return pTex;
      }
      pTex->KillObject();
      return NULL;
}

IBaseTexture* xGL2RenderApi::createFileTexture(const wchar_t* extFile)
{
      return new xGL2FileTexture(this);
}

IBaseTexture* xGL2RenderApi::createLockableTexture(int w , int h , int depth , ePIXEL_FORMAT fmt , bool bReadable , int nMipMap, int nArraySize)
{
      xGL2LockableTexture* pTexture = new xGL2LockableTexture(this , bReadable);
      if(pTexture->create(w , h , depth , fmt , nMipMap , nArraySize) )
      {
            return pTexture;
      }
      return NULL;	
}

xBaseTextureMgr* xGL2RenderApi::createTextureManager(const wchar_t* _name , bool nonRefKeep)
{
      xBaseTextureMgr* pMgr = new xBaseTextureMgr(_name , nonRefKeep ? 1 : 0 );
      pMgr->setRenderApi(this);
      m_TextureMagrs.push_back(pMgr);
      return pMgr;
}

void xGL2RenderApi::deleteTextureManager(xBaseTextureMgr* pTextureMgr)
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

void xGL2RenderApi::freeAllTextureManager()
{
      for(size_t i = 0 ; i < m_TextureMagrs.size() ; i ++)
      {
            m_TextureMagrs[i]->clear();
            XSAFE_DELETE(m_TextureMagrs[i]);
      }
      m_TextureMagrs.clear();
}

void xGL2RenderApi::unloadAllTextureManager()
{
      for(size_t i = 0 ; i < m_TextureMagrs.size() ; i ++)
      {
            m_TextureMagrs[i]->unload();
      }
}

bool xGL2RenderApi::enter2DMode(int x , int y , int w , int h , bool bTextMode)
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

bool xGL2RenderApi::leave2DMode()
{
      m_MatrixCtx.popMatrix(MATRIXMODE_Project);
      m_MatrixCtx.popMatrix(MATRIXMODE_View);
      return true;
}

bool xGL2RenderApi::multiMatrix(float* mat , eMatrixMode matMode)
{
      return m_MatrixCtx.multiMatrix( mat , matMode);
}

bool xGL2RenderApi::setMatrix(float* mat   , eMatrixMode matMode)
{
      return m_MatrixCtx.setMatrix( mat , matMode);
}

bool xGL2RenderApi::identityMatrix(eMatrixMode matMode)
{
      return m_MatrixCtx.identityMatrix(matMode);
}

bool xGL2RenderApi::getMatrix(float* mat , eMatrixMode matMode)
{
      return m_MatrixCtx.getMatrix( mat , matMode);
}

bool xGL2RenderApi::pushMatrix(eMatrixMode matMode)
{
      return m_MatrixCtx.pushMatrix(matMode );
}

bool xGL2RenderApi::popMatrix(eMatrixMode matMode)
{
      return m_MatrixCtx.popMatrix( matMode);
}

IRenderCamera* xGL2RenderApi::createCamera(const wchar_t* cameraName)
{
      return m_MatrixCtx.createCamera(cameraName);
}

bool xGL2RenderApi::applyCamera()
{
      return m_MatrixCtx.applyCamera();
}

bool xGL2RenderApi::applyCamera(IRenderCamera* pCamera)
{
      return m_MatrixCtx.applyCamera(pCamera);
}

IRenderCamera* xGL2RenderApi::getCamera()
{
      return m_MatrixCtx.getCamera();
}

IBaseTexture *xGL2RenderApi::createRenderableTexture(int w , int h , int depth , ePIXEL_FORMAT fmt , bool bReadable ,  int nMipMap, int nArraySize )
{
      if(depth != 1 && bReadable)
      {
            XEVOL_LOG(eXL_DEBUG_HIGH,"3D Render To Texture cann't created as readable\n");
            bReadable = false;
      }
      xGL2RenderTexture* pRenderTexture = new xGL2RenderTexture(bReadable , true , this);
      if( false == pRenderTexture->create(w , h , depth , fmt) )
      {
            delete pRenderTexture;
            return NULL;
      }
      return pRenderTexture;
}

IRenderTarget* xGL2RenderApi::createRenderTarget(int w , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture)
{
      xGL2RenderTexture* pRenderTexture = new xGL2RenderTexture(bLockable , bAsTexture , this);
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

IRenderTarget* xGL2RenderApi::createDepthBuffer(int w  , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture)
{
      xGL2DepthTexture* pDepthTexture = new xGL2DepthTexture(bLockable , bAsTexture , this);
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

IRenderTarget*  xGL2RenderApi::DefDepthBuffer()
{
      if(m_RenderWindow == NULL) return NULL;
      return m_RenderWindow->depthBuffer();
}

ID3D10DepthStencilView*  xGL2RenderApi::DefDepthStencilView()
{
      if(m_RenderWindow == NULL) return NULL;
      return m_RenderWindow->GetDepthStencilView();
}

IRenderView* xGL2RenderApi::createRenderView(int w , int h , bool bCreateDepth )
{
      xGL2RenderView* pRenderView = new xGL2RenderView(this);
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

IRenderView* xGL2RenderApi::createRenderView(int w , int h ,void* hWnd ,bool bCreateDepth )
{
      return NULL;
}
IRenderView* xGL2RenderApi::getRenderView()
{
      return m_RenderView;
}

bool xGL2RenderApi::setRenderView(IRenderView* renderView)
{
      GUID_SELECT_MODE(false);
      xGL2RenderView* pD10RV = (xGL2RenderView*) renderView;
      m_RenderView = pD10RV;
      return pD10RV->install();
}

bool xGL2RenderApi::pushRenderView(IRenderView* renderView)
{
      GUID_SELECT_MODE(false);
      xGL2RenderView* pD10RV = (xGL2RenderView*) renderView;
      m_RenderView.push(  );
      setRenderView(renderView);
      return false;
}

bool xGL2RenderApi::popRenderView()
{
      GUID_SELECT_MODE(false);
      xGL2RenderView* renderView = m_RenderView.pop();
      if(renderView == NULL)
            return false;
      setRenderView(renderView);
      return true;
}

ILightingEnv* xGL2RenderApi::createLightingState(const wchar_t* _name)
{
      xGL2LightingEnv* pLightingEnv = new xGL2LightingEnv(this , _name);
      m_objManager.LightEnvMgr().add(_name , pLightingEnv);
      return pLightingEnv;
}

ILightingEnv* xGL2RenderApi::findLightingState(const wchar_t* _name)
{
      ILightingEnv* pLightingEnv = m_objManager.LightEnvMgr().find(_name );
      return pLightingEnv;
}

bool xGL2RenderApi::removeLightingState(const wchar_t* _name)
{
      m_objManager.LightEnvMgr().remove(_name);
      return true;
}
END_NAMESPACE_XEVOL3D