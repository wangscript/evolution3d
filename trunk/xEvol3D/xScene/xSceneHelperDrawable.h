#ifndef _XSCENE_HELPERDRAWABLE_H_
#define _XSCENE_HELPERDRAWABLE_H_
#include "xSceneObject.h"
#include "xSceneDrawable.h"
#include "xSceneNode.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_  xSceneHelperDrawable : public xSceneEffectObject
{
	DECL_BASE_OBJECT_DLLSAFE(xSceneHelperDrawable);
public:
	
	bool           drawImm(IBaseRenderer* pRenderer, unsigned int passedTime , xGeomLib::xCamera* pCamer);
	bool           draw(IBaseRenderer* pRenderer , xGeomLib::xCamera* pCamer);
	IDrawElement*  drawElement(size_t idx);
	size_t         nDrawElement(){ return 1 ; }
	bool           setDrawElement(IDrawElement* pDrawElement , size_t idx);
	virtual bool   updateFrame(unsigned long passedTime, IRenderCamera* pCamera);
	virtual bool   load(xXmlNode* pXml);
	virtual bool   save(xXmlNode* pNode);
	xSceneHelperDrawable(ISceneGraph*  pScene , int arg);
	virtual ~xSceneHelperDrawable();
public:
	xRenderPass    m_Pass;
	IDrawElement*  m_pElement;
};

class _XEVOL_BASE_API_  xSceneHelperDrawableNode : public xSceneBasicNode 
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xSceneHelperDrawableNode);
	xSceneHelperDrawableNode(ISceneGraph*  pScene , ISceneNode* pParent = NULL);
	virtual ~xSceneHelperDrawableNode();
    virtual bool attachDrawElement(IDrawElement* pDrawElement);
	virtual bool detachDrawElement(IDrawElement* pDrawElement);
	virtual bool load(xXmlNode* pNode);
	virtual bool save(xXmlNode* pNode);
public:
};

END_NAMESPACE_XEVOL3D

#endif
