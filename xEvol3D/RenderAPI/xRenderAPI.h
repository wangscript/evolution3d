#ifndef _XEVOL_RENDER_API_H_
#define _XEVOL_RENDER_API_H_
#include "xRenderApiDef.h"
#include "xRenderApiFwDecl.h"
#include "xRenderView.h"
#include "../font/xFontManager.h"

#include "xMemBuffer.h"
#include "xTextureManager.h"

#include <XMathLib/xGeomLib.h>
#include <XMathLib/xcamera.h>
#include "BaseLib/xProperties.h"

BEGIN_NAMESPACE_XEVOL3D


class _XEVOL_BASE_API_  IColorSelector : public IRefCountObject
{
public:
	IColorSelector(IRenderApi* pApi){};
	virtual IRenderApi* renderApi() = 0;
	virtual ~IColorSelector(){};
	virtual bool setRenderObjectID(int id1 , int id2) = 0;
	virtual bool setRenderObjMasterID(int _id) = 0;
	virtual bool setRenderObjSlaveID(int _id) = 0;
	virtual bool begin() = 0;
	virtual bool end() = 0;
	virtual bool beginPrimitive() = 0;
	virtual bool endPrimitive() = 0;
	virtual bool getSelectID(int x , int y , int w , int h, void* _data) = 0;
	virtual bool resize(int w , int h) = 0;
};

class _XEVOL_BASE_API_ IRenderApiEventCallback
{
public:
	virtual bool preDrawPrimitive(IRenderApi* pSender , eRenderMode renderMode)  = 0;
	virtual bool postDrawPrimitive(IRenderApi* pSender , eRenderMode renderMode) = 0;
};

class _XEVOL_BASE_API_ ICameraPropertySet: public xPropertySet
{
public:
	ICameraPropertySet(const wchar_t* name , const wchar_t* _desc) : xPropertySet(name , _desc)
	{

	}
};

class _XEVOL_BASE_API_  IRenderCamera : public IRefCountObject , public xGeomLib::xCamera
{
private:
	ICameraPropertySet*         m_pPropertySet;
    float                       m_fStepLen;
public:
    IRenderCamera();
	virtual ~IRenderCamera();
	virtual ICameraPropertySet*  getPropertySet();
	virtual void                 initPropertySet();
    float                        step();
    void                         setStep(float step);
	void                         toTopView();
	void                         toLeftView();
	void                         toRightView();
	void                         toBackView();
	void                         toFrontView();
	virtual        bool          onChange(){ return true ; }
};

class IRenderApiDeviceLostListener 
{
public:
    enum eResetAction
    {
        eReset_Begin,
        eReset_Finish,
    };
public:
    virtual void onDeviceReset(bool bNewDevice , eResetAction _Action) = 0;
};

class _XEVOL_BASE_API_  IRenderApi : public IRefCountObject
{
public:
    enum eFeatureLevel
    {
        eFeature_ShaderModel1  = 0, //DirectX 9 目前不支持
        eFeature_ShaderModel2  ,    //DirectX 9 目前准备支持     OpenGL ES 2.0 目前准备支持
        eFeature_ShaderModel3  ,    //DirectX 9c 目前准备支持    OpenGL 2.0 目前准备支持
        eFeature_ShaderModel4  ,    //DirectX 10.0 目前已经支持  OpenGL 3.0 没有计划支持
        eFeature_ShaderModel5  ,    //DirectX 11   目前已经支持  
    };
public:
	IRenderApi();
	virtual ~IRenderApi();
    virtual eFeatureLevel            featureLevel() = 0;
    virtual int                      intCapsValue(const wchar_t* cfgName , int defValue) = 0; 
    virtual void                     AddDeviceLostListener(IRenderApiDeviceLostListener* pResource) = 0;
    virtual void                     RemoveDeviceLostListener(IRenderApiDeviceLostListener* pResource) = 0;
	//选择模式
    virtual eRenderMode              renderMode() = 0;
    virtual void                     addRenderObject(IRenderApiObject* pObject) = 0;
    virtual void                     removeRenderObject(IRenderApiObject* pObject) = 0;
    virtual void                     dumpRenderObject() = 0;
	virtual bool                     beginSelectMode() = 0;
	virtual IColorSelector*          colorSelector() = 0;
	virtual bool                     setColorSelector(IColorSelector* pColorSel) = 0;
	virtual bool                     endSelectMode(int x , int y , int _id[2] ) = 0;
	virtual bool                     endSelectMode(int x , int y , int w , int h , void* _id) = 0;
	virtual bool                     setEventCallback(IRenderApiEventCallback* pCallback) = 0;
    virtual IRenderApiEventCallback* getEventCallback() = 0;
	virtual wchar_t*                 name() = 0;
	virtual bool                     init(xXmlNode* pxmlNode) = 0;
	virtual bool                     uninit() = 0;
	virtual bool                     lock() = 0;
	virtual bool                     unlock() = 0;
	virtual bool                     enableThreadLocker(bool bEnable) = 0;
	virtual void                     getRTSampleDesc(xRTSampleDesc& _desc) = 0;
	//==============================================
	//Base Render Function	         
	virtual bool                     begin(xColor_4f& bkColor , float z = 1.0f , unsigned int stencil = 0) = 0;
	virtual bool                     beginScene() = 0;
	virtual bool                     endScene() = 0;
	virtual bool                     swapBuffer() = 0;
    virtual bool                     onResize(int w , int h) = 0;
    virtual bool                     getWindowSize(int& w , int & h) = 0 ;
    virtual bool                     getRenderViewSize(int& w , int & h) =0 ;
    //Transform Matrix			  
	virtual bool                     multiMatrix(const float* mat , eMatrixMode matMode) = 0;
	virtual bool                     setMatrix(const float* mat   , eMatrixMode matMode) = 0;
	virtual bool                     identityMatrix(eMatrixMode matMode)           = 0;
	virtual bool                     getMatrix(float* mat , eMatrixMode)           = 0;
	virtual bool                     pushMatrix(eMatrixMode matMode)               = 0;
	virtual bool                     popMatrix(eMatrixMode matMode)                = 0;
	virtual bool                     enter2DMode(int x , int y , int w , int h , bool bTextMode) = 0;
	virtual bool                     leave2DMode() = 0;
    virtual IRenderCamera*           createCamera(const wchar_t* cameraName) = 0;
	virtual ILightingEnv*            createLightingState(const wchar_t* _name) = 0;
	virtual ILightingEnv*            findLightingState(const wchar_t* _name) = 0;
	//使得相机改变的参数起作用	  
	virtual bool                     applyCamera(IRenderCamera* pCamera) = 0;
	virtual bool                     applyCamera() = 0;
	virtual IRenderCamera*           getCamera() = 0;
	//Draw function				  
	virtual bool                     setPrimitiveType(ePrimtiveType type) = 0;
	virtual ePrimtiveType            getPrimitiveType() = 0;
	virtual bool                     setInputAssembler(IInputAssembler* pAssembler) = 0;
	virtual IInputAssembler*         getInputAssembler() = 0;
	virtual bool                     setIndexBuffer(IInputBuffer* pVertexBuffer , size_t iOffset = 0) = 0;
	virtual bool                     setVertexBuffer(IInputBuffer* pVertexBuffer , size_t bufIdx = 0 , size_t stride = 0 , size_t iOffset = 0) = 0;
	virtual bool                     setVertexStream(IVertexStream* vertexStream) = 0;
	virtual bool                     draw(IInputBuffer* pIdxBuffer , size_t nVertexIndex , size_t iStartVertexIndex = 0 , ePrimtiveType pt = ePrimtiveType_Triangles ) = 0;
	virtual bool                     draw(size_t nVertexIndex , size_t iStartVertexIndex = 0 , ePrimtiveType pt = ePrimtiveType_Triangles) = 0;
	virtual bool                     set2DZValue(float zValue) = 0;
	virtual float                    get2DZValue() = 0;
	virtual bool                     drawRectf(IBaseTexture* pTexture, float vDestRect[4] , const xColor_4f& color) = 0;
	virtual bool                     drawRectf(IBaseTexture* pTexture, float vDestRect[4] ,float vSrcRect[4]  , const xColor_4f& color) = 0;
	virtual void                     set2DTexture(int iStage , IBaseTexture* pTexture) = 0;
	virtual void                     set2DTexture(int iStage , IBaseTexture* pTexture , float srcRect[4]) = 0;
	virtual I2DRectObject*           create2DRectObject() = 0;
    virtual I2DRectObject*           def2DObject() = 0;
	virtual bool                     draw2DRect(I2DRectObject* p2DRect) = 0;
	//Vertex /Pixel / Geometry Shader
	virtual IShaderConstBinder*      getShaderConstBinder(eShaderParamSemantic _semantic) = 0;
	virtual IShaderConstBinder*      getShaderConstBinder(const wchar_t* pShaderConstName) = 0;
	virtual void                     registeShaderConstBinder(const wchar_t* constName , IShaderConstBinder* pBinder) = 0;
	virtual void                     registeShaderConstBinder(eShaderParamSemantic _semantic , IShaderConstBinder* pBinder) = 0;
    virtual void                     registeShaderConstBinder(const wchar_t* _name , const wchar_t* _alisName) = 0;
								  
	virtual IBaseShader*             createShader(eShaderType type) = 0;
    virtual HBaseShader              createShader(const wchar_t* shaderName , eShaderType type)= 0;
	virtual IGpuProgram*             createGpuProgram() = 0;
	virtual bool                     setGpuProgram(IGpuProgram* pGpuProgram) = 0;
	virtual IGpuProgram*             getGpuProgram( ) = 0;
	virtual bool                     pushGpuProgram(IGpuProgram* pGpuProgram) = 0;
	virtual bool                     popGpuProgram() = 0;
	virtual bool                     setShader(IBaseShader* pVertShader , eShaderType _type = eShader_None) = 0;
	virtual IShaderCodeGenerator*    createShaderCodeGenerator(const unsigned char * _code) = 0;

	//
	virtual bool                     setShaderResource(eShaderType _st ,int iSlot , IBaseTexture* pTexture) = 0;
	virtual bool                     setShaderResource(eShaderType _st ,int iSlot , IInputBuffer* pBuffer , ePIXEL_FORMAT fmt)  = 0;
	virtual bool                     setTexture(const int iTexName      , IBaseTexture* pTexture) = 0;
	virtual bool                     setTexture(const wchar_t* iTexName , IBaseTexture* pTexture) = 0;
	virtual IBaseTexture*            getTexture(const int iTexName) = 0;
	virtual IBaseTexture*            getTexture(const wchar_t* iTexName) = 0;
	virtual bool                     clearAllTexture() = 0;
									
	//Texture Funciton				
	//文件类型的纹理				
	//通过资源来加载。				
	virtual IBaseTexture*            createFileTexture(const wchar_t* texFile , const unsigned int8* buf , unsigned int bufLen , unsigned int arg = 0, const xTextureInitDesc* texInitDesc = NULL) = 0 ;
	//通过扩展名,创建对象后，        需要自己调用load
	virtual IBaseTexture*            createFileTexture(const wchar_t* extFile, const xTextureInitDesc* texInitDesc = NULL) = 0 ;
    virtual IBaseTexture*            createTexture(int w , int h , ePIXEL_FORMAT fmt , bool bLockable = false ,int nMipMap = 1, int nArraySize = 1 ,  eResourceUsage usage = RESOURCE_USAGE_DYNAMIC , eResourceAccessFlage access = RESOURCE_ACCESS_WRITE , eResourceBindType bindType = BIND_AS_SHADER_RESOURCE) = 0;
	virtual IBaseTexture*            createTexture(int w , int h , int depth , ePIXEL_FORMAT fmt ,  bool bLockable = false , int nMipMap = 1, int nArraySize = 1 , eResourceUsage usage = RESOURCE_USAGE_DYNAMIC , eResourceAccessFlage access = RESOURCE_ACCESS_WRITE , eResourceBindType bindType = BIND_AS_SHADER_RESOURCE) = 0 ;
    virtual IBaseTexture*            createTexture(const xTextureInitDesc& initDesc , xTextureInitData* pInitData = NULL, int nInitData = 0) = 0;
	virtual IBaseTexture*            createRenderableTexture(int w , int h , int depth , ePIXEL_FORMAT fmt , bool bReadable ,  int nMipMap = 1, int nArraySize = 1 , const xRTSampleDesc& sampleQulity = xRTSampleDesc::None) = 0 ;
	virtual bool                     isTextureSupport(ePIXEL_FORMAT fmt , bool lockable = true) = 0;
    virtual xBaseTextureMgr*         createTextureManager(const wchar_t* _name, bool nonRefKeep = false) = 0;
	virtual void                     freeAllTextureManager() = 0;
	virtual void                     unloadAllTextureManager() = 0;
	virtual void                     deleteTextureManager(xBaseTextureMgr* pTextureMgr) = 0;
									
									
	//RenderTarget的支持系统		
	virtual IRenderTarget*           createRenderTarget(int w , int h , ePIXEL_FORMAT fmt , bool bLockable , bool bAsTexture = false, const xRTSampleDesc& sampleQulity = xRTSampleDesc::None) = 0; 
	virtual IRenderTarget*           createDepthBuffer(int w  , int h , ePIXEL_FORMAT fmt , bool bLockable , bool bAsTexture = false, const xRTSampleDesc& sampleQulity = xRTSampleDesc::None) = 0; 
	virtual IRenderView*             createRenderView(int w , int h , bool bCreateDepth = false , const xRTSampleDesc& sampleDesc = xRTSampleDesc::None) = 0;
	virtual IRenderView*             createRenderView(int w , int h , void* hWnd ,bool bCreateDepth = true , const xRTSampleDesc& sampleDesc= xRTSampleDesc::None) = 0;
	virtual IRenderView*             getRenderView() = 0;
	virtual bool                     setRenderView(IRenderView* renderView) = 0;
	virtual bool                     pushRenderView(IRenderView* renderView) = 0;
	virtual bool                     popRenderView() = 0;
									
	//Mem Buffer function			
	virtual IInputAssembler*         getInputAssembler(const wchar_t* name) = 0;
	virtual IInputAssembler*         createInputAssembler(const wchar_t* name , xInputLayoutDesc& desc ) = 0;
	virtual IInputBuffer*            createInputBuffer(size_t nVert , size_t nBytePerVertex ,const xInputBufferDesc* pBufferDesc , void* pData = NULL)  = 0;
	virtual IInputBuffer*            createBuffer(size_t iBufLen ,const xInputBufferDesc* pBufferDesc , void* pData = NULL)  = 0;
    virtual IInputBuffer*            findInputBuffer(const wchar_t* _name) = 0;
	virtual bool                     setInputBuffer(const wchar_t* _name, IInputBuffer* pConstBuffer) = 0;
	virtual bool                     removeInputBuffer(IInputBuffer* pConstBuffer) = 0;
	virtual bool                     removeInputBuffer(const wchar_t* _name) = 0;
	virtual IInputBuffer*            createConstBuffer(size_t iBufLen) = 0;
	virtual IInputBuffer*            createInputBuffer(size_t iBufLen) = 0;
	//State Object create			
	virtual IRenderApiState*         createRenderState(eRenderStateType _type) = 0;
	virtual ISamplerState*           createSamplerState(const wchar_t* name) = 0;
	virtual IRasterizerState*        createRasterizerState(const wchar_t* name) = 0;
	virtual IBlenderState*           createBlendState(const wchar_t* name) = 0;
	virtual IDepthStencilState*      createDepthStencilState(const wchar_t* name) = 0;
	virtual float                    alphaTestRef() = 0;
	virtual void                     alphaTestRef(float _value) = 0;
									
	virtual bool                     setSamplerState(eShaderType _st , int iSlot , ISamplerState* pState) = 0;
	virtual bool                     setBlendState(IBlenderState* pState) = 0;
	virtual bool                     setRasterizerState(IRasterizerState* pState) = 0;
	virtual bool                     setDepthStencilState(IDepthStencilState* pState) = 0;
									
	virtual bool                     pushBlendState(IBlenderState* pState) = 0;
	virtual bool                     pushRasterizerState(IRasterizerState* pState) = 0;
	virtual bool                     pushDepthStencilState(IDepthStencilState* pState) = 0;
									
	virtual bool                     popBlendState() = 0;
	virtual bool                     popRasterizerState() = 0;
	virtual bool                     popDepthStencilState() = 0;
									
	virtual IBlenderState*           getBlendState() = 0;
	virtual IRasterizerState*        getRasterizerState() = 0;
	virtual IDepthStencilState*      getDepthStencilState() = 0;
									
	virtual xFontManager*            fontManager() = 0;
	virtual HFontRender              findFont(const wchar_t* family , const wchar_t* fontName) = 0;
	virtual HFontRender              findFont(const wchar_t* fontName) = 0;
	virtual IFontRenderDevice*       fontRenderDevice() = 0;
	virtual const wchar_t*           texCoordStyle() = 0;
	//Help funtion;					
	virtual xGpuProgramManager*      gpuProgramManager() = 0;
	virtual xBaseShaderMgr*          shaderManager() = 0;
	virtual float                    globalAlpha() = 0;
	virtual void                     globalAlpha(float _value) = 0;
    virtual void                     setGlobalWireFrame(bool bFlag) = 0;
    virtual bool                     isGlobalWireFrame() = 0;

protected:
	virtual  bool                    drawPrimitiveIndex(size_t nVertexIndex , size_t iStartVertexIndex, ePrimtiveType pt) = 0;
    virtual  bool                    drawPrimitive(size_t nVertexIndex , size_t iStartVertexIndex , ePrimtiveType pt) = 0;
};

END_NAMESPACE_XEVOL3D
#endif
