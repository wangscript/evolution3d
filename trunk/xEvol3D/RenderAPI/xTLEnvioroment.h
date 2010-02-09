#ifndef __XEVOL_BASE_TLENV_H__
#define __XEVOL_BASE_TLENV_H__
#include "xRenderApiObject.h"
#include "xRenderApiFwDecl.h"
#include "../XMathLib/xMathLib.h"
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
struct xLightDesc
{
	  xMathLib::xvec4 m_Ambient;
      xMathLib::xvec4 m_Diffuse;
      xMathLib::xvec4 m_Speculer;
      xMathLib::xvec4 m_position;
      xMathLib::xvec4 m_dir;
};

class _XEVOL_BASE_API_ ILightingEnv : public IRenderApiObject
{
      IMPL_BASE_OBJECT_CLASSID_INTERFACE(ILightingEnv);
public:
      virtual ~ILightingEnv();
      ILightingEnv(IRenderApi* pRenderApi);
public:
      virtual xLightDesc*    lightDesc(size_t idx)     = 0;
      virtual void           enableLight(size_t nLight)= 0;
      virtual size_t         nLightEnabled() = 0;
      virtual const wchar_t* name() = 0;
      virtual void           applyChange() = 0;
};


//Transformϵͳ
class _XEVOL_BASE_API_ xTransformContext
{
public:
	xMathLib::xmat4         m_matWorld;
	xMathLib::xmat4         m_matView;
	xMathLib::xmat4         m_matProject;
	xMathLib::xmat4         m_matTexture;
public:
	void*         data();
	int           dataLen();

};

typedef ds_vector(xMathLib::xmat4) vecXMatrixs;
class _XEVOL_BASE_API_ IMatrixContext 
{
protected:
	//-------------------------------------
	vecXMatrixs   m_stackWorldMatrix;
	vecXMatrixs   m_stackViewMatrix;
	vecXMatrixs   m_stackProjectMatrix;
	vecXMatrixs        m_stackTextureMatrix;
	xTransformContext* m_Context;
public:
	bool          multiMatrix(const float* mat , eMatrixMode matMode);
	bool          setMatrix(const float* mat   , eMatrixMode matMode);
	bool          identityMatrix(eMatrixMode matMode)          ;
	bool          getMatrix(float* mat , eMatrixMode)          ;
	bool          pushMatrix(eMatrixMode matMode)              ;
	bool          popMatrix(eMatrixMode matMode)               ;

	virtual xTransformContext* transContext() = 0;
	virtual void               onMatrixChange(eMatrixMode )  = 0;
	virtual bool               install()   = 0;
	virtual bool               uninstall() = 0;
};

END_NAMESPACE_XEVOL3D
#endif
