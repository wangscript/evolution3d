#ifndef __XRENDERER_GEOM_ELEMENT_OBJECT_H__
#define __XRENDERER_GEOM_ELEMENT_OBJECT_H__
#include "xDrawElement.h"
#include "BaseLib/xRect.h"
BEGIN_NAMESPACE_XEVOL3D


class _XEVOL_BASE_API_ x2DQuadElement : public IDrawElement
{
public:
	XMathLib::xmat4 m_matPos;
	x2DRect_f       m_Rectf;
	xColor_4f       m_fColor;
	IBaseTexture*   m_hMainTexture;

	IMPL_BASE_OBJECT_CLASSID_INTERFACE(x2DQuadElement)
public:

	x2DQuadElement(IRenderApi* pApi);
	~x2DQuadElement();

	void             center(XMathLib::xvec3&   _center);
	void             aabb(xGeomLib::xaabb&     _aabb);
	void             shpere(xGeomLib::xshpere& _shpere);

	IDrawElement*    clone();
	void             render();
	bool             prepare();
	void             setupTransform();
	bool             unprepare();

public:
	void setTexture(IBaseTexture* hTexture , int stage = 0);
	void setRect(float x , float y , float w , float h);
	void setFullScreenRect();
};


END_NAMESPACE_XEVOL3D

#endif
