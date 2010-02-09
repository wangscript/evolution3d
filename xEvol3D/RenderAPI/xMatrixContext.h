#pragma  once
#include <vector>
#include <RenderAPI/xBaseShader.h>
#include <RenderAPI/xTLEnvioroment.h>
#include <XMathLib/xMathLib.h>
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ xRenderApiMatContext ;
class _XEVOL_BASE_API_ xWorldMatConstBinder  ;
class _XEVOL_BASE_API_ xTextureMatConstBinder;
class _XEVOL_BASE_API_ xProjectMatConstBinder;
class _XEVOL_BASE_API_ xViewMatConstBinder   ;
class _XEVOL_BASE_API_ xMatrixConstBinder: public IShaderConstBinder
{
public:
	IShaderConstBinder*   createInstance()
	{
		AddRef(); 
		return this;
	}
};

class _XEVOL_BASE_API_ xWorldMatConstBinder : public xMatrixConstBinder
{
	xRenderApiMatContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xWorldMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
	xWorldMatConstBinder(xRenderApiMatContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};

class _XEVOL_BASE_API_ xViewMatConstBinder : public xMatrixConstBinder
{
	xRenderApiMatContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xViewMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
	xViewMatConstBinder(xRenderApiMatContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};

class _XEVOL_BASE_API_ xProjectMatConstBinder : public xMatrixConstBinder
{
	xRenderApiMatContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xProjectMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
	xProjectMatConstBinder(xRenderApiMatContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};

class _XEVOL_BASE_API_ xTextureMatConstBinder : public xMatrixConstBinder
{
	xRenderApiMatContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xTextureMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
    xTextureMatConstBinder(xRenderApiMatContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};


//Camera的属性
class _XEVOL_BASE_API_ xCameraPosConstBinder : public xMatrixConstBinder
{
	xRenderApiMatContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xWorldMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
	xCameraPosConstBinder(xRenderApiMatContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};

class _XEVOL_BASE_API_ xCameraUpConstBinder : public xMatrixConstBinder
{
	xRenderApiMatContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xWorldMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
	xCameraUpConstBinder(xRenderApiMatContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};

class _XEVOL_BASE_API_ xCameraDirConstBinder : public xMatrixConstBinder
{
	xRenderApiMatContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xWorldMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
	xCameraDirConstBinder(xRenderApiMatContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};

class _XEVOL_BASE_API_ xCameraArgConstBinder : public xMatrixConstBinder
{
	xRenderApiMatContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xWorldMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
	xCameraArgConstBinder(xRenderApiMatContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};

//D3D10用到的相机

class _XEVOL_BASE_API_ xBasicRenderCamera;
class _XEVOL_BASE_API_ xCameraEyeInfo
{
public:
	XMathLib::xvec4 m_Up;
	XMathLib::xvec4 m_EyePos;
	XMathLib::xvec4 m_EyeDir;
	float           m_fNear;
	float           m_fFar;
	float           m_fFov;
	float           m_fAspect;
};

class _XEVOL_BASE_API_ xRenderApiMatContext : public IMatrixContext
{
	friend class xWorldMatConstBinder  ;
	friend class xTextureMatConstBinder;
	friend class xProjectMatConstBinder;
	friend class xViewMatConstBinder   ;
	friend class xCameraUpConstBinder  ;
	friend class xCameraPosConstBinder ;
	friend class xCameraDirConstBinder ;
	friend class xCameraArgConstBinder ;
	xBasicRenderCamera*      m_pCamera;
private:

	xWorldMatConstBinder     m_WorldConstBinder;
	xTextureMatConstBinder   m_textureMatBinder;
	xProjectMatConstBinder   m_projMatBinder;
	xViewMatConstBinder      m_viewMatBinder;
	xCameraUpConstBinder     m_CameraUpBinder;
	xCameraPosConstBinder    m_CameraPosBinder;
	xCameraDirConstBinder    m_CameraDirBinder;
	xCameraArgConstBinder    m_CameraArgBinder;

	IRenderApi*              m_pRenderApi;
	xCameraEyeInfo*          m_pCameraInfo;

	//
	IInputBuffer*           m_pConstBuffer;
	IInputBufferReflection* m_pReflection;
	std::wstring            m_name;

public:
	xRenderApiMatContext(IRenderApi* pRenderApi);
	~xRenderApiMatContext();
    bool                         init(IRenderApi* pRenderApi);

	bool                         onCameraChange(IRenderCamera* pCamera);
	virtual IRenderCamera*       createCamera(const wchar_t* cameraName);
	virtual bool                 applyCamera(IRenderCamera* pCamera);
	virtual bool                 applyCamera();
	IRenderCamera*               getCamera();
	void                         onMatrixChange(eMatrixMode );
	xTransformContext*           transContext() { return m_Context ; }
	bool                         install()   ;
	bool                         uninstall() ;
};

class _XEVOL_BASE_API_ xBasicRenderCamera : public IRenderCamera
{
public:
    DECL_REFCOUNT_OBJECT_INTERFACE(xBasicRenderCamera);
	xRenderApiMatContext* m_pMatContext;
	xBasicRenderCamera(xRenderApiMatContext* pContext) { m_pMatContext = pContext ; }
	 bool          onChange()
	 {
		 return m_pMatContext->onCameraChange(dynamic_cast<IRenderCamera*>(this));
	 }
};

END_NAMESPACE_XEVOL3D
