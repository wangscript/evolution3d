#pragma  once
#include <vector>
#include <RenderAPI/xBaseShader.h>
#include <XMathLib/XMathLib.h>
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
typedef ds_vector(xmat4) vecXMatrixs;
class xD3D10MatrixContext ;
class xD3D10RenderApi;

class xWorldMatConstBinder  ;
class xTextureMatConstBinder;
class xProjectMatConstBinder;
class xViewMatConstBinder   ;

class xMatrixConstBinder: public IShaderConstBinder
{
public:
	IShaderConstBinder*   createInstance()
	{
		AddRef(); 
		return this;
	}
};
class xWorldMatConstBinder : public xMatrixConstBinder
{
	xD3D10MatrixContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xWorldMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
	xWorldMatConstBinder(xD3D10MatrixContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};

class xViewMatConstBinder : public xMatrixConstBinder
{
	xD3D10MatrixContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xViewMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
	xViewMatConstBinder(xD3D10MatrixContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};

class xProjectMatConstBinder : public xMatrixConstBinder
{
	xD3D10MatrixContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xProjectMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
	xProjectMatConstBinder(xD3D10MatrixContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};

class xTextureMatConstBinder : public xMatrixConstBinder
{
	xD3D10MatrixContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xTextureMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
    xTextureMatConstBinder(xD3D10MatrixContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};

//D3D10用到的相机
class xD3D10Camera : public IRenderCamera
{
public:

};


class xD3D10MatrixContext
{
	friend class xWorldMatConstBinder  ;
	friend class xTextureMatConstBinder;
	friend class xProjectMatConstBinder;
	friend class xViewMatConstBinder   ;

//-------------------------------------
	vecXMatrixs   m_stackWorldMatrix;
	vecXMatrixs   m_stackViewMatrix;
	vecXMatrixs   m_stackProjectMatrix;
	vecXMatrixs   m_stackTextureMatrix;

	xmat4         m_matWorld;
	xmat4         m_matView;
	xmat4         m_matProject;
	xmat4         m_matTexture;


	xD3D10Camera* m_pCamera;
private:

	xWorldMatConstBinder   m_WorldConstBinder;
	xTextureMatConstBinder m_textureMatBinder;
	xProjectMatConstBinder m_projMatBinder;
	xViewMatConstBinder    m_viewMatBinder;

public:
	xD3D10MatrixContext();
	~xD3D10MatrixContext();
    bool                 init(xD3D10RenderApi* pRenderApi);
	bool                 multiMatrix(float* mat , eMatrixMode matMode);
	bool                 setMatrix(float* mat   , eMatrixMode matMode);
	bool                 identityMatrix(eMatrixMode matMode)          ;
	bool                 getMatrix(float* mat , eMatrixMode)          ;
	bool                 pushMatrix(eMatrixMode matMode)              ;
	bool                 popMatrix(eMatrixMode matMode)               ;
	IRenderCamera*       createCamera(const wchar_t* cameraName);
	bool                 applyCamera(IRenderCamera* pCamera);
	bool                 applyCamera();
	IRenderCamera*       getCamera();
};
END_NAMESPACE_XEVOL3D
