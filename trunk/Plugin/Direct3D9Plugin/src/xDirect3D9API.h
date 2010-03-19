#ifndef _XEVOL_DIRECT3D9_API_
#define _XEVOL_DIRECT3D9_API_
#include <map>
#include <stack>
#include <RenderApi/xRenderApiImp.h>
#include <BaseLib/xStringHash.h>
#include <RenderApi/xShaderManager.h>
#include <RenderAPI/xRenderStateObjManager.h>
#include "RenderAPI/xMatrixContext.h"
#include "xDirect3D9APIResource.h"
#include "xD3D2DRect.h"
#include "xD3D9Sampler.h"
#include "xD3D9RazState.h"
#include "xD3D9BlendState.h"
#include "xD3D9DepthStencilState.h"

#define RENDER_NAME L"Direct3D9"
#define MAX_TEXTURES 8
BEGIN_NAMESPACE_XEVOL3D
class xD3D9RenderView;
class xD3D9RenderWindow;
class xD3D9GpuProgram;
class xD3DVertexStream;
class xD3DBaseColorSelector;
//------------------------------------------------------------------
typedef std::map<int , IShaderConstBinder*> mapConstBinder;

class xD3D9RenderApi : public xRenderApiBase
{
	IMPL_REFCOUNT_OBJECT_INTERFACE(xD3D9RenderApi);
public:
	xD3D9RenderApi(IDirect3DDevice9* pDevice , HWND hWnd , int w , int h);
	~xD3D9RenderApi();
    eFeatureLevel              featureLevel();
    int                        intCapsValue(const wchar_t* cfgName , int defValue);
	bool                       uninit( );
	bool                       init(xXmlNode* pSysNode);
	bool                       create();//DXGI_SAMPLE_DESC sampleDes);
    xD3D9RenderWindow*         renderWindow(){ return m_RenderWindow; }
    IDirect3DSurface9*         DefDepthStencilView();
    void                       getRTSampleDesc(xRTSampleDesc & rtSample);
    xDeviceLostResManager*     GetDeviceLostResMgr();

	bool                       onResize(int width , int height);
	bool                       swapBuffer();
	bool                       setBlendState(IBlenderState* pState);
	bool                       setRasterizerState(IRasterizerState* pState);
	bool                       setDepthStencilState(IDepthStencilState* pState);
	IRenderApiState*           createRenderState(eRenderStateType _type);
	IInputAssembler*           createInputAssembler(const wchar_t* name , xInputLayoutDesc& desc );
	IInputBuffer*              createInputBuffer(size_t nVertex , size_t nBytePerVertex , const xInputBufferDesc* pBufferDesc , void* pData);
	IInputBuffer*              createBuffer(size_t iBufLen ,const xInputBufferDesc* pBufferDesc , void* pData);
	bool                       setPrimitiveType(ePrimtiveType type);
	ePrimtiveType              getPrimitiveType();
	bool                       setInputAssembler(IInputAssembler* pAssembler);
	bool                       setIndexBuffer(IInputBuffer* pVertexBuffer , size_t iOffset);
	bool                       setVertexBuffer(IInputBuffer* pVertexBuffer ,   size_t bufIdx , size_t stride , size_t iOffset );
	bool                       setVertexStream(IVertexStream* vertexStream);
	bool                       drawPrimitive(size_t nVertex , size_t iStartVertex , ePrimtiveType pt);
	bool                       drawRectf(IBaseTexture* pTexture, float vDestRect[4] , const xColor_4f& color);
	bool                       drawRectf(IBaseTexture* pTexture, float vDestRect[4] ,float vSrcRect[4]  , const xColor_4f& color);
	void                       set2DTexture(int iStage , IBaseTexture* pTexture);
	void                       set2DTexture(int iStage , IBaseTexture* pTexture , float _srcRect[4]);
	I2DRectObject*             create2DRectObject();
	bool                       draw2DRect(I2DRectObject* p2DRect);
    bool                       beginScene();
    bool                       endScene();
	//Shader Function
	IGpuProgram*               createGpuProgram();
	IBaseShader*               createShader(eShaderType type);
	bool                       setGpuProgram(IGpuProgram* pGpuProgram);
	
	bool                       setSamplerState(eShaderType _st , int iSlot , ISamplerState* pSampler);
	bool                       setPixelShader(IBaseShader* pPixelShader);
	bool                       setGeomShader(IBaseShader* pGeomShader);
	bool                       setVertexShader(IBaseShader* pVertShader);
	bool                       setHullShader(IBaseShader* pVertShader);
    bool                       setDomainShader(IBaseShader* pVertShader);
	bool                       setComputeShader(IBaseShader* pVertShader);
	bool                       setShader(IBaseShader* pVertShader , eShaderType _type);

	bool                       setShaderResource(eShaderType _st , int iSlot , IBaseTexture* pTexture);
	bool                       setShaderResource(eShaderType _st , int iSlot ,IInputBuffer* pBuffer , ePIXEL_FORMAT fmt);

	//Resource Function
	bool                       isTextureSupport(ePIXEL_FORMAT fmt , bool lockable);
	IBaseTexture*              createFileTexture(const wchar_t* texFile , const unsigned int8* buf , unsigned int bufLen, unsigned int arg, const xTextureInitDesc* texInitDesc);
	const wchar_t*             texCoordStyle();
	IBaseTexture*              createFileTexture(const wchar_t* extFile , const xTextureInitDesc* texInitDesc);
	IBaseTexture*              createTexture(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData = NULL, int nInitData = 0);

    IRenderCamera*             createCamera(const wchar_t* cameraName);
    IBaseTexture *             createRenderableTexture(int w , int h , int depth , ePIXEL_FORMAT fmt , bool bReadable ,  int nMipMap, int nArraySize , const xRTSampleDesc& sampleDesc);
	IRenderTarget*             createRenderTarget(int w , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture , const xRTSampleDesc& sampleDesc);
	IRenderTarget*             createDepthBuffer(int w  , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture , const xRTSampleDesc& sampleDesc);
	IRenderTarget*             DefDepthBuffer();
	IRenderView*               createRenderView(int w , int h , bool bCreateDepth , const xRTSampleDesc& sampleDes);
	IRenderView*               createRenderView(int w , int h ,void* hWnd ,bool bCreateDepth , const xRTSampleDesc& sampleDes);
	bool                       setRenderView(IRenderView* renderView);
	ILightingEnv*              createLightingState(const wchar_t* _name);
	IShaderCodeGenerator*      createShaderCodeGenerator(const unsigned char * _code);

	IDirect3DDevice9*          getDevice();
	const char*                getShaderProfile(eShaderType _type);
    void                       AddDeviceLostListener(IRenderApiDeviceLostListener* pResource);
    void                       RemoveDeviceLostListener(IRenderApiDeviceLostListener* pResource);
	//bool                       setShaderResource(eShaderType _st , int iSlot , int nResource , ID3D11ShaderResourceView** pResourceView);
    //bool                       setConstantBuffers(eShaderType shType , int iSlot , int nResource , ID3D11Buffer** pConstBuffer);
	//bool                       GetDXGISampleDesc(DXGI_SAMPLE_DESC& SampleDesc);
	//void                       getRTSampleDesc(xRTSampleDesc& _desc);   
    //DXGI_SAMPLE_DESC           GetDXGISampleDesc(const xRTSampleDesc& sampleDesc);
protected:
	bool           __needResize(int width , int height);
	bool           __resetViewPort();
	bool           __createDevice();//DXGI_SAMPLE_DESC sampleDes);
	bool           __destory();

	bool           __initByDevice();

    bool          __beforeResetDevice();
    bool          __afterDeviceRest();
    bool          __resetDevice(int w, int h);
    bool          __unloadAllResource();
    bool          __afterCreateDevice();

public:
	wchar_t*             name(){ return RENDER_NAME ; }
protected:
	IDirect3DDevice9*       m_pD3DDevice;
    UINT                    m_hDevAdapter;
    D3DDEVTYPE              m_hDevDeviceType;
    D3DFORMAT               m_hDevAdapterFormat;
    IDirect3D9*             m_pDirect3D;
    int                     m_MaxGpuBone;
    bool                    m_vSync;
	//设备属性和参数
	HINSTANCE               m_hInst;
	xD3D9RenderWindow*      m_RenderWindow;

    xD3D9BlendState*        m_pDefBlender;
    xD3D9ZStencilState*     m_pDefZStencilState;
    xD3D9RazState*          m_pDefRazState;
    xD3D9Sampler*           m_pDefSampler;
    xD3D9Sampler*           m_Samplers[Texture_StageMax];

    //2D绘制
    xD3D2DRect*              m_pDef2DRect;
    xD3D2DRectEnv*           m_pDef2DRectEnv;
    xDeviceLostResManager    m_DeviceLostResManager;

protected:
    bool                     m_DebugDevice;
    xXmlNode                 m_RenderCaps;
    UINT                     m_NumVerticesInVB;

};


END_NAMESPACE_XEVOL3D

#endif

