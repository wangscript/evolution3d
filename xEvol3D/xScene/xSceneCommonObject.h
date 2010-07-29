#ifndef _XSCENE_COMMONOBJECT_H_
#define _XSCENE_COMMONOBJECT_H_
#include "xSceneObject.h"
#include "xSceneDrawable.h"
#include "xSceneNode.h"
BEGIN_NAMESPACE_XEVOL3D



class _XEVOL_BASE_API_  xSceneSerializeObject : public ISceneDrawable
{
	DECL_BASE_OBJECT_DLLSAFE(xSceneSerializeObject);
public:
	
	bool                   drawImm(IBaseRenderer* pRenderer, unsigned int passedTime , xGeomLib::xCamera* pCamer);
	bool                   draw(IBaseRenderer* pRenderer , xGeomLib::xCamera* pCamer);
	IDrawElement*          drawElement(size_t idx);
	size_t                 nDrawElement(){ return 1 ; }
	bool                   setDrawElement(IDrawElement* pDrawElement , size_t idx);
	virtual bool           updateFrame(unsigned long passedTime, IRenderCamera* pCamera);
	virtual bool           load(xXmlNode* pXml);
    virtual bool           unload();
	virtual bool           save(xXmlNode* pNode);
	virtual IRenderEffect* effect(size_t idx = 0){return NULL ; }
	virtual bool           setEffect(IRenderEffect* _effect , size_t idx = 0){return false; }

	xSceneSerializeObject(ISceneGraph*  pScene , int arg);
	virtual ~xSceneSerializeObject();
public:
	xRenderPass             m_Pass;
	xSerializeDrawElement*  m_pElement;
};


END_NAMESPACE_XEVOL3D

#endif
