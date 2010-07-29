#ifndef _XEVOL_DIRECT3D10_API_
#define _XEVOL_DIRECT3D10_API_
#include <d3d10.h>
#include <map>
#include <stack>
#include <RenderApi/xRenderApiImp.h>
#include <BaseLib/xStringHash.h>

#include "xD10_InputAssembler.h"
#include "xD10HelperRender.h"
#include "RenderAPI/xRenderApiImp.h"
#include <RenderApi/xShaderManager.h>
#include <RenderAPI/xRenderStateObjManager.h>
#include "RenderAPI/xMatrixContext.h"
#include "xD3D2DRect.h"
#include "RenderAPI/xRenderApiHelper.h"
#define RENDER_NAME L"Direct3D10"

BEGIN_NAMESPACE_XEVOL3D
class xD10RenderView;
class xD10RenderWindow;
class xD10GpuProgram;
class xD3DVertexStream;
class xD3DBaseColorSelector;
//------------------------------------------------------------------

class xD3D10RenderApi : public xRenderApiBase
{
	IMPL_REFCOUNT_OBJECT_INTERFACE(xD3D10RenderApi);
public:
	xD3D10RenderApi(ID3D10Device* pDevice , HWND hWnd , int w , int h);
	~xD3D10RenderApi();
    eFeatureLevel              featureLevel() { return eFeature_ShaderModel4 ; }
    int                        intCapsValue(const wchar_t* cfgName , int defValue);
	bool                       uninit( );
	bool                       init(xXmlNode* pSysNode);
	bool                       create(DXGI_SAMPLE_DESC SampleDes);

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

	bool                       setShader(IBaseShader* pPixelShader , eShaderType _type);
	bool                       setPixelShader(IBaseShader* pPixelShader);
	bool                       setGeomShader(IBaseShader* pGeomShader);
	bool                       setVertexShader(IBaseShader* pVertShader);

	bool                       setShaderResource(eShaderType _st , int iSlot , IBaseTexture* pTexture);
	bool                       setShaderResource(eShaderType _st , int iSlot ,IInputBuffer* pBuffer , ePIXEL_FORMAT fmt);

	//Resource Function
	bool                       isTextureSupport(ePIXEL_FORMAT fmt , bool lockable);
	IBaseTexture*              createFileTexture(const wchar_t* texFile , const unsigned int8* buf , unsigned int bufLen, unsigned int arg, const xTextureInitDesc* texInitDesc = NULL);
	const wchar_t*             texCoordStyle();
	IBaseTexture*              createFileTexture(const wchar_t* extFile, const xTextureInitDesc* texInitDesc = NULL);
	IBaseTexture*              createTexture(const xTextureInitDesc& initDesc , xTextureInitData* pInitData = NULL, int nInitData = 0);

	IRenderCamera*             createCamera(const wchar_t* cameraName);
    IBaseTexture *             createRenderableTexture(int w , int h , int depth , ePIXEL_FORMAT fmt , bool bReadable ,  int nMipMap, int nArraySize  , const xRTSampleDesc& sampleQulity);
	IRenderTarget*             createRenderTarget(int w , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture , const xRTSampleDesc& sampleQulity);
	IRenderTarget*             createDepthBuffer(int w  , int h , ePIXEL_FORMAT fmt , bool bLockable, bool bAsTexture , const xRTSampleDesc& sampleQulity);
	IRenderTarget*             DefDepthBuffer();
	ID3D10DepthStencilView*    DefDepthStencilView();
	IRenderView*               createRenderView(int w , int h , bool bCreateDepth , const xRTSampleDesc& sampleDesc);
	IRenderView*               createRenderView(int w , int h ,void* hWnd ,bool bCreateDepth , const xRTSampleDesc& sampleDesc);
	bool                       setRenderView(IRenderView* renderView);
	ILightingEnv*              createLightingState(const wchar_t* _name);
	IShaderCodeGenerator*      createShaderCodeGenerator(const unsigned char * _code);

	ID3D10Device*              d10Device();
	bool                       setShaderResource(eShaderType _st , int iSlot , int nResource , ID3D10ShaderResourceView** pResourceView);
	bool                       setConstantBuffers(eShaderType _st , int iSlot , int nResource , ID3D10Buffer** pConstBuffer);
	bool                       GetDXGISampleDesc(DXGI_SAMPLE_DESC& SampleDesc);
	void                       getRTSampleDesc(xRTSampleDesc& _desc);   
	DXGI_SAMPLE_DESC           GetDXGISampleDesc(const xRTSampleDesc& sampleDesc);
protected:
	bool           __needResize(int width , int height);
	bool           __resetViewPort();
	bool           __createD10Device(DXGI_SAMPLE_DESC sampleDes);
	bool           __destory();
	bool           __afterCreateDevice();
	bool           __initByDevice();

public:
	wchar_t*             name(){ return RENDER_NAME ; }
protected:
	ID3D10Device*           m_pD3DDevice;
	//设备属性和参数
	HINSTANCE               m_hInst;
	D3D10_DRIVER_TYPE       m_driverType;	
	xD10RenderWindow*       m_RenderWindow;
	//2D绘制
	xD3D2DRect*             m_pDef2DRect;
	xD3D2DRectEnv*          m_pDef2DRectEnv;
protected:
	ID3D10DepthStencilState* m_defDepthStencilState;
	ID3D10RasterizerState*   m_defpRasState;
	ID3D10BlendState*        m_defBlendState;

	float                    m_DefBlendFactor[4];
	UINT                     m_DefSampleMask;
	UINT                     m_DefStencilRef;
    bool                     m_DebugDevice;
    xXmlNode                 m_RenderCaps;

};

END_NAMESPACE_XEVOL3D

#endif

