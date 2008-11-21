#ifndef __XEVOL3D_DRAABLEOBJECT_H__
#define __XEVOL3D_DRAABLEOBJECT_H__
#include "../RenderAPI/xRenderAPI.h"
#include "../XMathLib/XGeomLib.h"
#include "../XMathLib/XGVolume.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ xMaterial;
struct sDrawElementInfo_t
{
     char m_queueIndex;
	 int  m_queueName;
};

class _XEVOL_BASE_API_ IDrawElement : public IRenderObject
{
protected:
	xMaterial*       m_pMaterial;
	IGpuProgram*     m_pShader;
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IDrawElement);
    IDrawElement(IRenderApi* pRenderApi);
	virtual ~IDrawElement() = 0{};    

	//Virtual Attribute
	virtual void             center(XMathLib::xvec3&   _center) = 0;
	virtual void             aabb(xGeomLib::xaabb&     _aabb) = 0;
	virtual void             shpere(xGeomLib::xshpere& _shpere) = 0;

	//Attribute
	virtual void             setMaterial(xMaterial* pMaterial);
	virtual void             setShader(IGpuProgram* pShader);

	virtual xMaterial*       material();
	virtual IGpuProgram*     shader() ;

	//For Rendering
	virtual IDrawElement*    clone() = 0;
	virtual void             render() = 0;
	virtual bool             prepare() = 0;
	virtual bool             unprepare() = 0;
	virtual void             setupTransform() = 0;
   
};

END_NAMESPACE_XEVOL3D
#endif
