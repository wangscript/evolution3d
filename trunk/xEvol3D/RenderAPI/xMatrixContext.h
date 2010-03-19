#pragma  once
#include <vector>
#include <RenderAPI/xBaseShader.h>
#include <RenderAPI/xTLEnvioroment.h>
#include <XMathLib/xMathLib.h>
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
//D3D10用到的相机

class _XEVOL_BASE_API_ xBasicRenderCamera;
class _XEVOL_BASE_API_ xCameraEyeInfo
{
public:
	XMathLib::xvec4 m_Up;
	XMathLib::xvec4 m_EyePos;
	XMathLib::xvec4 m_EyeDir;
    union
    {
        struct 
        {
            float           m_fNear;
            float           m_fFar;
            float           m_fFov;
            float           m_fAspect;
        };
        float       m_CameraArg[4];
    };
};
class _XEVOL_BASE_API_ xConstantValueDataBinder;
class _XEVOL_BASE_API_ xRenderApiMatContext : public IMatrixContext
{
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

    
private:

    xConstantValueDataBinder* m_pWorldConstBinder;
    //ViewMatrix
    xConstantValueDataBinder* m_pViewMatBinder;
    //ProjectMatrix
    xConstantValueDataBinder* m_pProjMatBinder;
    //Texture Matrix
    xConstantValueDataBinder* m_pTextureMatBinder;

    xConstantValueDataBinder* m_pCameraUpBinder ;
    xConstantValueDataBinder* m_pCameraPosBinder;

    xConstantValueDataBinder* m_pCameraDirBinder;
    xConstantValueDataBinder* m_pCameraArgBinder;

protected:
    xBasicRenderCamera*      m_pCamera;
    IRenderApi*              m_pRenderApi;
    xCameraEyeInfo*          m_pCameraInfo;
    IInputBuffer*            m_pConstBuffer;
    IInputBufferReflection*  m_pReflection;
    std::wstring             m_name;
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
