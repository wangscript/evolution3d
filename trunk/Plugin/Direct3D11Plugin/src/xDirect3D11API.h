#ifndef _XEVOL_DIRECT3D11_API_
#define _XEVOL_DIRECT3D11_API_
#include <map>
#include <stack>
#include <RenderApi/xRenderApiImp.h>
#include <BaseLib/xStringHash.h>
#include <RenderApi/xShaderManager.h>
#include <RenderAPI/xRenderStateObjManager.h>
#include "RenderAPI/xMatrixContext.h"
#include "xD3D2DRect.h"
#include "xD3DBaseColorSelector.h"
#define  MAX_SHASER_RES_SLOT 64
#define  RENDER_NAME L"Direct3D11"

BEGIN_NAMESPACE_XEVOL3D
class xD11RenderView;
class xD11RenderWindow;
class xD11GpuProgram;
class xD3DVertexStream;
class xD3DBaseColorSelector;
//------------------------------------------------------------------
typedef std::map<int , IShaderConstBinder*> mapConstBinder;

class xD3D11RenderApi : public xRenderApiBase
{
	IMPL_REFCOUNT_OBJECT_INTERFACE(xD3D11RenderApi);
public:
	xD3D11RenderApi(ID3D11Device* pDevice , HWND hWnd , int w , int h);
	~xD3D11RenderApi();
    eFeatureLevel              featureLevel();
    int                        intCapsValue(const wchar_t* cfgName , int defValue);
	bool                       uninit( );
	bool                       init(xXmlNode* pSysNode);
	bool                       create(DXGI_SAMPLE_DESC sampleDes);

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
	bool                       drawPrimitiveIndex(size_t nVertexIndex , size_t iStartVertexIndex , ePrimtiveType pt);
    bool                       drawPrimitive(size_t nVertexIndex , size_t iStartVertexIndex , ePrimtiveType pt);
	bool                       drawRectf(IBaseTexture* pTexture, float vDestRect[4] , const xColor_4f& color);
	bool                       drawRectf(IBaseTexture* pTexture, float vDestRect[4] ,float vSrcRect[4]  , const xColor_4f& color);
	void                       set2DTexture(int iStage , IBaseTexture* pTexture);
	void                       set2DTexture(int iStage , IBaseTexture* pTexture , float _srcRect[4]);
	I2DRectObject*             create2DRectObject();
    I2DRectObject*             def2DObject(){ return m_pDef2DRect ; }
	bool                       draw2DRect(I2DRectObject* p2DRect);

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
    bool                       setRenderWindow(IRenderView* pRenderWindow);
	//Resource Function
	bool                       isTextureSupport(ePIXEL_FORMAT fmt , bool lockable);
	IBaseTexture*              createFileTexture(const wchar_t* texFile , const unsigned int8* buf , unsigned int bufLen, unsigned int arg, const xTextureInitDesc* texInitDesc);
	const wchar_t*             texCoordStyle();
	IBaseTexture*              createFileTexture(const wchar_t* extFile, const xTextureInitDesc* texInitDesc);
	IBaseTexture*              createTexture(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData = NULL, int nInitData = 0);

    IRenderCamera*             createCamera(const wchar_t* cameraName);
    IBaseTexture *             createRenderableTexture(int w , int h , int depth , ePIXEL_FORMAT fmt , bool bReadable ,  int nMipMap, int nArraySize , const xRTSampleDesc& sampleDesc);
	IRenderTarget*             createRenderTarget(int w , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture , const xRTSampleDesc& sampleDesc);
	IRenderTarget*             createDepthBuffer(int w  , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture , const xRTSampleDesc& sampleDesc);
	IRenderTarget*             DefDepthBuffer();
	ID3D11DepthStencilView*    DefDepthStencilView();
	IRenderView*               createRenderView(int w , int h , bool bCreateDepth , const xRTSampleDesc& sampleDes);
	IRenderView*               createRenderView(int w , int h ,void* hWnd ,bool bCreateDepth , const xRTSampleDesc& sampleDes);
	bool                       setRenderView(IRenderView* renderView);
	ILightingEnv*              createLightingState(const wchar_t* _name);
	IShaderCodeGenerator*      createShaderCodeGenerator(const unsigned char * _code);

	ID3D11Device*              d11Device();
	ID3D11DeviceContext*       d11DeviceContext();
	D3D_FEATURE_LEVEL          D3DFeatureLevel() { return m_pD3DDevice->GetFeatureLevel() ; }
	const char*                getShaderProfile(eShaderType _type);
	bool                       setShaderResource(eShaderType _st , int iSlot , int nResource , ID3D11ShaderResourceView** pResourceView);
    bool                       setConstantBuffers(eShaderType shType , int iSlot , int nResource , ID3D11Buffer** pConstBuffer);
	bool                       GetDXGISampleDesc(DXGI_SAMPLE_DESC& SampleDesc);
	void                       getRTSampleDesc(xRTSampleDesc& _desc);   
    DXGI_SAMPLE_DESC           GetDXGISampleDesc(const xRTSampleDesc& sampleDesc);
protected:
	bool           __needResize(int width , int height);
	bool           __resetViewPort();
	bool           __createD11Device(DXGI_SAMPLE_DESC sampleDes);
	bool           __destory();
	bool           __afterCreateDevice();
	bool           __initByDevice();

public:
	wchar_t*             name(){ return RENDER_NAME ; }
protected:
	ID3D11Device*             m_pD3DDevice;
	ID3D11DeviceContext*      m_pD3DDeviceContext;
	D3D_FEATURE_LEVEL         m_FeatureLevel;
	//设备属性和参数
	HINSTANCE                 m_hInst;
	D3D_DRIVER_TYPE           m_driverType;	

    bool                      m_DebugDevice;
    xXmlNode                  m_RenderCaps;
    xD11RenderWindow*         m_RenderWindow;
    ID3D11ShaderResourceView* m_vShaderResourceViews[MAX_SHASER_RES_SLOT * eShader_Max];
protected:

    xD11RenderWindow*        m_DefRenderWindow;
    //2D绘制
    xD3D2DRect*              m_pDef2DRect;
    xD3D2DRectEnv*           m_pDef2DRectEnv;

  
	ID3D11DepthStencilState* m_defDepthStencilState;
	ID3D11RasterizerState*   m_defpRasState;
	ID3D11BlendState*        m_defBlendState;

	ID3D11BlendState*        m_pBlendStateNoBlend;
	ID3D11RasterizerState*	 m_pRasterizerStateNoCull;

	float                    m_DefBlendFactor[4];
	UINT                     m_DefSampleMask;
	UINT                     m_DefStencilRef;



};


END_NAMESPACE_XEVOL3D

#endif

