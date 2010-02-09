#ifndef _XEVOL_RENDER_API_IMPL_H_
#define _XEVOL_RENDER_API_IMPL_H_
#include "xRenderAPI.h"
#include "xMatrixContext.h"
#include "xRenderApiHelper.h"
#include "xShaderManager.h"
#include "xRenderStateObjManager.h"
#include <set>
BEGIN_NAMESPACE_XEVOL3D
class IRenderObjectSets
{
public:
	virtual void                  addRenderObject(IRenderApiObject* pObject) = 0;
	virtual void                  removeRenderObject(IRenderApiObject* pObject) = 0;
	virtual void                  dumpRenderObject() = 0;
	virtual void                  KillRenderObject() = 0;
};

class _XEVOL_BASE_API_  xRenderApiImp : public IRenderApi
{
public:
	xRenderApiImp();
	virtual ~xRenderApiImp();
    void                  addRenderObject(IRenderApiObject* pObject)     {return m_pRenderObjectSets->addRenderObject(pObject)    ; } 
	void                  removeRenderObject(IRenderApiObject* pObject)  {return m_pRenderObjectSets->removeRenderObject(pObject) ; }
	void                  dumpRenderObject()  {return m_pRenderObjectSets->dumpRenderObject() ; } 
	void                  KillRenderObject()  {return m_pRenderObjectSets->KillRenderObject() ; }
	xFontManager*         fontManager() ;
	HFontRender           findFont(const wchar_t* family , const wchar_t* fontName);
	HFontRender           findFont(const wchar_t* fontName);
	IFontRenderDevice*    fontRenderDevice();
	IRenderObjectSets*    renderObjects(){return m_pRenderObjectSets ; }
protected:
	void                  __updateTextureLoaders();
	xFontManager          m_FontManager;
	IFontRenderDevice*    m_FontRenderDevice;
	IRenderObjectSets*    m_pRenderObjectSets;

};


class xTextureLdrMgrProxy
{
public:
	typedef std::vector<ITextureLoader*>               vTextureLoaders;
	typedef std::map<std::ds_wstring, ITextureLoader*> TextureLoaderMap;
public:
	ITextureLoader*               findTextureLoader(const wchar_t* texExt);
	void                          addTextureLoader(ITextureLoader* pTextureLoader);
	void                          removeTextureLoader(ITextureLoader* pTextureLoader);
protected:
	void                          __updateTextureLoaders();
	vTextureLoaders               m_TextureLoaders;
	TextureLoaderMap              m_TextureLoaderMap;
};


class _XEVOL_BASE_API_ xRenderApiBase : public xRenderApiImp
{
public:
	xRenderApiBase();
	~xRenderApiBase();
	virtual eRenderMode                 renderMode() ;
	virtual bool                        beginSelectMode();
	virtual IColorSelector*             colorSelector();
	virtual bool                        setColorSelector(IColorSelector* pColorSel);
	virtual bool                        endSelectMode(int x , int y , int _id[2] );
	virtual bool                        endSelectMode(int x , int y , int w , int h , void* _id);

	//RenderApi的接口
	virtual bool                        begin(xColor_4f& bkColor , float z = 1.0f , unsigned int stencil = 0);
	virtual bool                        beginScene();
	virtual bool                        endScene();
	virtual bool                        lock();
	virtual bool                        unlock();
	virtual bool                        enableThreadLocker(bool bEnable);
	virtual bool                        init(xXmlNode* pxmlNode);
	virtual bool                        uninit();

	//
	//Mem buffer
	virtual IInputAssembler*            getInputAssembler(const wchar_t* name);
	virtual IInputBuffer*               findInputBuffer(const wchar_t* _name);
	virtual bool                        removeInputBuffer(IInputBuffer* pConstBuffer);
	virtual bool                        removeInputBuffer(const wchar_t* _name);
	virtual bool                        setInputBuffer(const wchar_t* _name, IInputBuffer* pConstBuffer);
	virtual ILightingEnv*               findLightingState(const wchar_t* _name);
	//Object Manager
	virtual xRenderApiObjectManager&    objectManager()    {return  m_objManager    ; }
	virtual xGpuProgramManager*         gpuProgramManager(){return &m_GpuProgramMgr ; }
	virtual xBaseShaderMgr*             shaderManager();

	virtual bool                        multiMatrix(const float* mat , eMatrixMode matMode) ;
	virtual bool                        setMatrix(const float* mat   , eMatrixMode matMode) ;
	virtual bool                        identityMatrix(eMatrixMode matMode)           ;
	virtual bool                        getMatrix(float* mat , eMatrixMode)           ;
	virtual bool                        pushMatrix(eMatrixMode matMode)               ;
	virtual bool                        popMatrix(eMatrixMode matMode)                ;
	virtual bool                        enter2DMode(int x ,int y , int w , int h , bool bTextMode);
	virtual bool                        leave2DMode();
	virtual bool                        applyCamera(IRenderCamera* pCamera);
	virtual bool                        applyCamera();
	virtual IRenderCamera*              getCamera();
	virtual bool                        beginDrawPrimitive();
	virtual bool                        endDrawPrimitive();
	virtual bool                        draw(IInputBuffer* pIdxBuffer , size_t nVertex , size_t iStartVertex, ePrimtiveType pt);
	virtual bool                        draw(size_t nVertex , size_t iStartVertex, ePrimtiveType pt);
	virtual bool                        draw2DRect(I2DRectObject* p2DRect);
	//-----------------
	virtual IInputAssembler*            getInputAssembler();
	virtual bool                        set2DZValue(float zValue);
	virtual float                       get2DZValue();

	virtual IShaderConstBinder*         getShaderConstBinder(const wchar_t* pShaderConstName);
	virtual void                        registeShaderConstBinder(const wchar_t* constName , IShaderConstBinder* pBinder);
	virtual IShaderConstBinder*         getShaderConstBinder(eShaderParamSemantic _semantic);
	virtual void                        registeShaderConstBinder(eShaderParamSemantic _semantic , IShaderConstBinder* pBinder);

	virtual bool                        pushGpuProgram(IGpuProgram* pGpuProgram);
	virtual bool                        popGpuProgram();
	virtual IGpuProgram*                getGpuProgram( ) ;

	virtual HBaseShader                 createShader(const wchar_t* shaderName , eShaderType type);
	//
	virtual bool                        setTexture(const int iTexName      , IBaseTexture* pTexture);
	virtual bool                        setTexture(const wchar_t* iTexName , IBaseTexture* pTexture);
	virtual IBaseTexture*               getTexture(const int iTexName);
	virtual IBaseTexture*               getTexture(const wchar_t* iTexName);
	virtual bool                        clearAllTexture();
public:
	virtual IInputBuffer*               createInputBuffer(size_t iBufLen);
	virtual IInputBuffer*               createConstBuffer(size_t iBufLen);
	virtual ISamplerState*              createSamplerState(const wchar_t* name);
	virtual IBlenderState*              createBlendState(const wchar_t* name);
	virtual IDepthStencilState*         createDepthStencilState(const wchar_t* name);
	virtual IRasterizerState*           createRasterizerState(const wchar_t* name);

	virtual IBlenderState*              getBlendState();
	virtual IRasterizerState*           getRasterizerState() ;
	virtual IDepthStencilState*         getDepthStencilState();

	virtual xBaseTextureMgr*            createTextureManager(const wchar_t* _name, bool nonRefKeep = true);
	virtual void                        deleteTextureManager(xBaseTextureMgr* pTextureMgr);
	virtual void                        freeAllTextureManager();
	virtual void                        unloadAllTextureManager();

	virtual ITextureLoader*             findTextureLoader(const wchar_t* texExt);
	virtual void                        addTextureLoader(ITextureLoader* pTextureLoader);
	virtual void                        removeTextureLoader(ITextureLoader* pTextureLoader);
	//RenderTarget支持
	virtual IRenderView*                getRenderView();
	virtual bool                        pushRenderView(IRenderView* renderView);
	virtual bool                        popRenderView();
	virtual bool                        getWindowSize(int& w , int & h);
	virtual bool                        getRenderViewSize(int& w , int & h);

    //RenderState管理
	virtual bool                        pushBlendState(IBlenderState* pState);
	virtual bool                        pushRasterizerState(IRasterizerState* pState);
	virtual bool                        pushDepthStencilState(IDepthStencilState* pState);

	virtual float                       alphaTestRef() { return m_AlphaRef ; }
	virtual void                        alphaTestRef(float _value) { m_AlphaRef = _value ; }
	virtual bool                        popBlendState();
	virtual bool                        popRasterizerState();
	virtual bool                        popDepthStencilState();
	virtual bool                        setEventCallback(IRenderApiEventCallback* pCallback) { m_pCallback = pCallback ; return true; }
	virtual IRenderApiEventCallback*    getEventCallback(){return m_pCallback ; }
	//D3D10RenderApi特有的
public:
	virtual bool                        removeLightingState(const wchar_t* _name);
protected:
	typedef std::map<int , IBaseTexture*>            vTexLayers;
	typedef std::map<int , IShaderConstBinder*>      mapConstBinder;
	typedef std::vector<IBlenderState*      , dllsafe_alloc<IBlenderState*>      > vBlenderState;
	typedef std::vector<IRasterizerState*   , dllsafe_alloc<IRasterizerState*>   > vRasterizerStates;
	typedef std::vector<IDepthStencilState* , dllsafe_alloc<IDepthStencilState*> > vDepthStencilStates;

	IBaseTexture*                       m_texLayer[Texture_StageMax];
	vTexLayers                          m_mapTexLayers;
	xRenderApiLocker                    m_RenderLocker;
	xRenderApiLocker*                   m_pDevLocker;
	vBlenderState                       m_vStackBlenderState;
	vRasterizerStates                   m_vStackRasterizerStates;
    vDepthStencilStates                 m_vStackDepthStencilStates;
protected:
	mapConstBinder                      m_constNameBinder;
	mapConstBinder                      m_semanticBinder;

	//状态
	ePrimtiveType                       m_PrimType;
	xRenderApiMatContext*               m_MatrixCtx;
	xObjectStack<IGpuProgram>           m_pGpuProgram;
	IInputAssembler*                    m_pInputAssembler;
	IVertexStream*                      m_pVertStream;
	float                               m_AlphaRef;
	IShaderConstBinder*                 m_pAlphaRefValueConstBinder;

	//设备属性和参数
#ifdef _WIN32
	HWND                                m_hMainWnd;
#endif
	int                                 m_Width;
	int                                 m_Height;

	xObjectStack<IRenderView>           m_RenderView;
	//资源
	xRenderApiObjectManager         m_objManager;
	xGpuProgramMgrImp                  m_GpuProgramMgr;
	xBaseShaderMgr                     m_ShaderManager;
	xRenderStateObjMgr                 m_StateObjMgr;
	std::vector<xBaseTextureMgr*>      m_TextureMagrs;

	float                              m_2DZValue;

	//光照系统
	ILightingEnv*                      m_LightingState;
	//像素选择    
	eRenderMode                        m_RenderMode;
	IColorSelector*                    m_pColorSelector;
	IColorSelector*                    m_pBaseSelector;
protected:
	IBlenderState*                     m_BlenderState;
	IRasterizerState*                  m_RasterizerState;  
	IDepthStencilState*                m_DepthStencilState;
	IRenderApiEventCallback*           m_pCallback;

};


END_NAMESPACE_XEVOL3D
#endif
