#ifndef _XEVOL_OPENGL20_API_
#define _XEVOL_OPENGL20_API_
#include <d3d10.h>
#include <map>
#include <stack>
#include <RenderApi/xRenderApiImp.h>
#include <BaseLib/xStringHash.h>

#include "xGL2RenderApiBaseDecl.h"
#include "xGL2InputAssembler.h"
#include "xGL2HelperRender.h"
#include <RenderApi/xShaderManager.h>
#include <RenderAPI/xRenderStateObjManager.h>
#include "xGL2MatrixContext.h"
#include "xGL2Rect2D.h"
#define RENDER_NAME L"OpenGL2X"

BEGIN_NAMESPACE_XEVOL3D
class xGL2RenderView;
class xGL2RenderWindow;
class xGL2GpuProgram;
class xGL2VertexStream;
class xGL2BaseColorSelector;
//------------------------------------------------------------------
typedef std::map<int , IShaderConstBinder*> mapConstBinder;

class xGL2Locker
{
	CRITICAL_SECTION m_locker;
public:
	xGL2Locker()   
	{
		InitializeCriticalSection(&m_locker);
	}
	~xGL2Locker() 
	{ 
		DeleteCriticalSection(&m_locker); 
	}
	bool    lock()  
	{
		EnterCriticalSection(&m_locker);	
		return true;
	}
	bool    trylock()
	{ 
		return TryEnterCriticalSection(&m_locker) == TRUE;	
	}
	void    unlock() 
	{ 
		LeaveCriticalSection(&m_locker);
	}
};



template<typename TObject> class xObjectStack
{
	TObject*              m_pObject;
	std::stack<TObject*>  m_Stack;
public:
	xObjectStack(){ m_pObject = NULL; }
	TObject* operator ->() { return m_pObject ;}
	operator TObject*(){ return m_pObject ; }
	void operator = (TObject* rhv){m_pObject = rhv ; }
	bool operator ==(const TObject* rhv){return m_pObject == rhv ; } 
	bool operator !=(const TObject* rhv){return m_pObject != rhv ; } 
	template <typename TTypeCv> TTypeCv* dynamic_convert() { return dynamic_cast<TTypeCv> (m_pObject) ; }
	template <typename TTypeCv> TTypeCv* static_convert()  { return static_cast<TTypeCv> (m_pObject) ; }
public:
	bool push()
	{
		m_Stack.push(m_pObject);
		return true;
	}

	TObject* pop()
	{
		if(m_Stack.size() == 0 )
			return false;
		TObject* pObject = m_Stack.top();
		m_Stack.pop();
		return pObject;
	}
   
};


class xGL2RenderApi : public xRenderApiImp
{
    IMPL_REFCOUNT_OBJECT_INTERFACE(xGL2RenderApi);
public:
	xGL2RenderApi();
	~xGL2RenderApi();
	eRenderMode          renderMode() ;
	bool                 beginSelectMode();
	IColorSelector*      colorSelector();
	bool                 setColorSelector(IColorSelector* pColorSel);
	bool                 endSelectMode(int x , int y , int _id[2] );
	bool                 endSelectMode(int x , int y , int w , int h , void* _id);

//RenderApi的接口
	wchar_t*             name(){ return RENDER_NAME ; }
	bool                 create(ID3D10Device* pDevice , HWND hWnd , int w , int h);
	bool                 begin(xColor_4f& bkColor , float z = 1.0f , unsigned int stencil = 0);
	bool                 beginScene();
	bool                 endScene();
	bool                 swapBuffer();
	bool                 lock();
	bool                 unlock();
	bool                 enableThreadLocker(bool bEnable);
	bool                 onResize(int w , int h);
	bool                 init(xXmlNode* pxmlNode);
	bool                 uninit();
	//
	//Mem buffer
	IInputAssembler*     getInputAssembler(const wchar_t* name);
	IInputAssembler*     createInputAssembler(const wchar_t* name , xInputLayoutDesc& desc );
      IInputBuffer*        createInputBuffer(size_t nVert , size_t nBytePerVertex , const xInputBufferDesc* pBufferDesc , void* pData) ;
	IInputBuffer*        createBuffer(size_t iBufLen ,const xInputBufferDesc* pBufferDesc , void* pData);
	IInputBuffer*        findInputBuffer(const wchar_t* _name);
	bool                 removeInputBuffer(IInputBuffer* pConstBuffer);
	bool                 removeInputBuffer(const wchar_t* _name);
	bool                 setInputBuffer(const wchar_t* _name, IInputBuffer* pConstBuffer);
	IInputBuffer*        createConstBuffer(size_t iBufLen);
	IInputBuffer*        createInputBuffer(size_t iBufLen);
	ILightingEnv*        createLightingState(const wchar_t* _name);
	ILightingEnv*        findLightingState(const wchar_t* _name);
	//Object Manager
	xGL2ObjectManager& objectManager()    {return  m_objManager    ; }
	xGpuProgramManager*  gpuProgramManager(){return &m_GpuProgramMgr ; }

	bool                 multiMatrix(float* mat , eMatrixMode matMode) ;
	bool                 setMatrix(float* mat   , eMatrixMode matMode) ;
	bool                 identityMatrix(eMatrixMode matMode)           ;
	bool                 getMatrix(float* mat , eMatrixMode)           ;
	bool                 pushMatrix(eMatrixMode matMode)               ;
	bool                 popMatrix(eMatrixMode matMode)                ;
	IRenderCamera*       createCamera(const wchar_t* cameraName);
	bool                 enter2DMode(int x ,int y , int w , int h , bool bTextMode);
	bool                 leave2DMode();
	bool                 applyCamera(IRenderCamera* pCamera);
	bool                 applyCamera();
	IRenderCamera*       getCamera();

	//-----------------
	bool                 setPrimitiveType(ePrimtiveType type);
	ePrimtiveType        getPrimitiveType();
	bool                 setInputAssembler(IInputAssembler* pAssembler);
	IInputAssembler*     getInputAssembler();
	bool                 setIndexBuffer(IInputBuffer* pVertexBuffer , size_t iOffset = 0);
	bool                 setVertexBuffer(IInputBuffer* pVertexBuffer , size_t bufIdx  , size_t stride , size_t iOffset = 0);
	bool                 setVertexStream(IVertexStream* vertexStream);
	bool                 draw(IInputBuffer* pIdxBuffer , size_t nVertex , size_t iStartVertex = 0, ePrimtiveType pt = ePrimtiveType_Triangles);
	bool                 draw(size_t nVertex , size_t iStartVertex = 0, ePrimtiveType pt = ePrimtiveType_Triangles);
	bool                 set2DZValue(float zValue);
	float                get2DZValue();
	bool                 drawRectf(IBaseTexture* pTexture, float vDestRect[4] , xColor_4f& color);
	bool                 drawRectf(IBaseTexture* pTexture, float vDestRect[4] ,float vSrcRect[4]  , xColor_4f& color);
	void                 set2DTexture(int iStage , IBaseTexture* pTexture);
	void                 set2DTexture(int iStage , IBaseTexture* pTexture , float srcRect[4]);
	I2DRectObject*       create2DRectObject();
	bool                 draw2DRect(I2DRectObject* p2DRect);

	IShaderConstBinder*  getShaderConstBinder(const wchar_t* pShaderConstName);
	void                 registeShaderConstBinder(const wchar_t* constName , IShaderConstBinder* pBinder);
	IShaderConstBinder*  getShaderConstBinder(eShaderParamSemantic _semantic);
	void                 registeShaderConstBinder(eShaderParamSemantic _semantic , IShaderConstBinder* pBinder);
	IBaseShader*         createShader(eShaderType type);
	IBaseShader*         createShader(const wchar_t* shaderFile , eShaderType type);
	IGpuProgram*         createGpuProgram();
	bool                 pushGpuProgram(IGpuProgram* pGpuProgram);
	bool                 popGpuProgram();
	IGpuProgram*         getGpuProgram( ) ;
	bool                 setGpuProgram(IGpuProgram* pGpuProgram);
	bool                 setVertexShader(IBaseShader* pVertShader);
	bool                 setPixelShader(IBaseShader* pPixelShader);
	bool                 setGeomShader(IBaseShader* pGeomShader);

	bool                 setShaderResource(eShaderType _st , int iSlot , IBaseTexture* pTexture) ;
	bool                 setShaderResource(eShaderType _st , int iSlot , IInputBuffer* pBuffer)  ;

	//
	bool                 setTexture(const int iTexName      , IBaseTexture* pTexture);
	bool                 setTexture(const wchar_t* iTexName , IBaseTexture* pTexture);
	IBaseTexture*        getTexture(const int iTexName);
	IBaseTexture*        getTexture(const wchar_t* iTexName);
	bool                 clearAllTexture();
public:
	IBaseShader*         createShader(wchar_t* shaderFile , eShaderType type);
	bool                 beginDrawPrimitive();
	bool                 endDrawPrimitive();
	bool                 drawPrimitive(size_t nVertex , size_t iStartVertex, ePrimtiveType pt);

public:
	ISamplerState*       createSamplerState(const wchar_t* name);
	IBlenderState*       createBlendState(const wchar_t* name);
	IRasterizerState*    createRasterizerState(const wchar_t* name);
	IDepthStencilState*  createDepthStencilState(const wchar_t* name) ;

	IRenderState*        createRenderState(eRenderStateType _type);
	bool                 setSamplerState(eShaderType _st , int iSlot , ISamplerState* pState); 
	bool                 setBlendState(IBlenderState* pState);
	bool                 setRasterizerState(IRasterizerState* pState);
	bool                 setDepthStencilState(IDepthStencilState* pState);

	IBlenderState*        getBlendState();
	IRasterizerState*     getRasterizerState() ;
	IDepthStencilState*   getDepthStencilState();
public:
	IBaseTexture*        createFileTexture(const wchar_t* texFile , const unsigned int8* buf , unsigned int bufLen, unsigned int arg = 0);
	IBaseTexture*        createFileTexture(const wchar_t* extFile);
	IBaseTexture*        createLockableTexture(int w , int h , int depth , ePIXEL_FORMAT fmt , bool bReadable ,  int nMipMap = 1, int nArraySize = 1 ) ;
	IBaseTexture*        createLockableTexture(int w , int h , ePIXEL_FORMAT fmt , bool bReadable ,  int nMipMap, int nArraySize)
	{
		return createLockableTexture(w , h , 1 , fmt , bReadable , nMipMap , nArraySize);
	}

	IBaseTexture*        createRenderableTexture(int w , int h , int depth , ePIXEL_FORMAT fmt , bool bReadable ,  int nMipMap = 1, int nArraySize = 1 ) ;
	bool                 isTextureSupport(ePIXEL_FORMAT fmt , bool lockable = true);
	xBaseTextureMgr*     createTextureManager(const wchar_t* _name, bool nonRefKeep = true);
	void                 deleteTextureManager(xBaseTextureMgr* pTextureMgr);
	void                 freeAllTextureManager();
	void                 unloadAllTextureManager();

	ITextureLoader*      findTextureLoader(const wchar_t* texExt);
	void                 addTextureLoader(ITextureLoader* pTextureLoader);
	void                 removeTextureLoader(ITextureLoader* pTextureLoader);
	//RenderTarget支持
	IRenderTarget*       createRenderTarget(int w , int h , ePIXEL_FORMAT fmt , bool bLockable , bool bAsTexture = false); 
	IRenderTarget*       createDepthBuffer(int w  , int h , ePIXEL_FORMAT fmt , bool bLockable , bool bAsTexture = false); 
	IRenderView*         createRenderView(int w , int h ,bool bCreateDepth = false );
	IRenderView*         createRenderView(int w , int h ,void* hWnd ,bool bCreateDepth = true );
	IRenderView*         getRenderView();
	bool                 setRenderView(IRenderView* renderView);
	bool                 pushRenderView(IRenderView* renderView);
	bool                 popRenderView();
    bool                 getWindowSize(int& w , int & h);
    bool                 getRenderViewSize(int& w , int & h);
	//GL2RenderApi特有的
public:
	IRenderTarget*           DefDepthBuffer();
	bool                     removeLightingState(const wchar_t* _name);
protected:
	bool           __needResize(int width , int height);
	bool           __resetViewPort();
	bool           __createD10Device();
	bool           __afterCreateDevice();
	bool           __initByDevice();
	bool           __destory();
protected:
	typedef map<int , IBaseTexture*>  vTexLayers;
	IBaseTexture*          m_texLayer[Texture_StageMax];
	vTexLayers             m_mapTexLayers;
	//xGL2Locker           m_devLocker;
	xGL2Locker             m_RenderLocker;
	xGL2Locker*            m_pDevLocker;
protected:
	GLuint                  m_GLContext;
	mapConstBinder          m_constNameBinder;
	mapConstBinder          m_semanticBinder;

	//状态
	ePrimtiveType                m_PrimType;
	xGL2MatrixContext          m_MatrixCtx;
	xObjectStack<xGL2GpuProgram> m_pGpuProgram;
	IInputAssembler*             m_pInputAssembler;
	xGL2VertexStream*            m_pVertStream;

	//设备属性和参数
	HWND                    m_hMainWnd;
	int                     m_Width;
	int                     m_Height;
	HINSTANCE               m_hInst;
    xGL2RenderWindow*       m_RenderWindow;
	
	DXGI_SWAP_CHAIN_DESC    m_swapChainDesc;
	xObjectStack<xGL2RenderView> m_RenderView;
	//资源
	xGL2ObjectManager     m_objManager;
	xGpuProgramMgrImp       m_GpuProgramMgr;
	xBaseShaderMgr          m_ShaderManager;
	xRenderStateObjMgr      m_StateObjMgr;
	std::vector<xBaseTextureMgr*> m_TextureMagrs;

	//2D绘制
	xGL2Rect2D*             m_pDef2DRect;
	xGL2RectEnv2D*          m_pDef2DRectEnv;
	float                   m_2DZValue;

	//光照系统
	ILightingEnv*           m_LightingState;
	//像素选择
	eRenderMode                   m_RenderMode;
    IColorSelector*               m_pColorSelector;
	xGL2BaseColorSelector*        m_pBaseSelector;
	
	
protected:
   
	float                    m_DefBlendFactor[4];
	UINT                     m_DefSampleMask;
	UINT                     m_DefStencilRef;
	
};

END_NAMESPACE_XEVOL3D

#endif

