#ifndef __XEVOL_BASE_TLENV_H__
#define __XEVOL_BASE_TLENV_H__
#include "xRenderObject.h"
#include "xRenderApiFwDecl.h"
#include "../XMathLib/XMathLib.h"
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
struct xLightDesc
{
      xMathLib::xvec4 m_Diffuse;
      xMathLib::xvec4 m_Speculer;
      xMathLib::xvec4 m_position;
      xMathLib::xvec4 m_dir;
};

class _XEVOL_BASE_API_ ILightingEnv : public IRenderObject
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

END_NAMESPACE_XEVOL3D
#endif
