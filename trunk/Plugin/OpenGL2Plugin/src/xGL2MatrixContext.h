#pragma  once
#include <vector>
#include <RenderAPI/xBaseShader.h>
#include <XMathLib/xMathLib.h>
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
typedef ds_vector(xmat4) vecXMatrixs;
class xGL2MatrixContext ;
class xGL2RenderApi;

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
	xGL2MatrixContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xWorldMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
	xWorldMatConstBinder(xGL2MatrixContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};

class xViewMatConstBinder : public xMatrixConstBinder
{
	xGL2MatrixContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xViewMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
	xViewMatConstBinder(xGL2MatrixContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};

class xProjectMatConstBinder : public xMatrixConstBinder
{
	xGL2MatrixContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xProjectMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
	xProjectMatConstBinder(xGL2MatrixContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};

class xTextureMatConstBinder : public xMatrixConstBinder
{
	xGL2MatrixContext* matContext;
	bool                 m_bDirty;
	IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(xTextureMatConstBinder);
public:
	void     setDirty(){m_bDirty = true ;}
    xTextureMatConstBinder(xGL2MatrixContext* mc){matContext = mc; m_bDirty = true ; };
	//返回true，表示变量被更新了
	bool     updateConstant(IShaderConstantReflection* pConst);
};

//GL2用到的相机
class xGL2Camera : public IRenderCamera
{
public:

};


class xGL2MatrixContext
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


	xGL2Camera* m_pCamera;
private:

	xWorldMatConstBinder   m_WorldConstBinder;
	xTextureMatConstBinder m_textureMatBinder;
	xProjectMatConstBinder m_projMatBinder;
	xViewMatConstBinder    m_viewMatBinder;

public:
	xGL2MatrixContext();
	~xGL2MatrixContext();
    bool                 init(xGL2RenderApi* pRenderApi);
	bool                 multiMatrix(const float* mat , eMatrixMode matMode);
	bool                 setMatrix(const float* mat   , eMatrixMode matMode);
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
