#ifndef _XSCENE_DRAWABLE_H_
#define _XSCENE_DRAWABLE_H_
#include "xSceneObject.h"
#include "../Renderer/xDrawElement.h"
#include "../Renderer/xMaterial.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ ISceneDrawable : public ISceneObject
{
	DECL_BASE_OBJECT_DLLSAFE(ISceneDrawable);
public:
     virtual IRenderEffect* effect(size_t idx)    = 0 ;
	 virtual bool           setEffect(IRenderEffect* _effect) = 0 ;
	 virtual bool           load(xXmlNode* pXml) = 0;
	 virtual bool           save(xXmlNode* pNode) = 0;
	 virtual size_t         nDrawElement() = 0;
	 virtual IDrawElement*  drawElement(size_t idx) = 0 ;
	 virtual bool           setDrawElement(IDrawElement* pDrawElement , size_t idx) = 0;
	 virtual bool           draw(IBaseRenderer* pRenderer , xGeomLib::xCamera* pCamera) = 0;
	 virtual bool           draw(IBaseRenderer* pRenderer, unsigned int passedTime , xGeomLib::xCamera* pCamera) = 0;
	 virtual bool           isVisible(xGeomLib::xCamera* pCamera);
public:
	ISceneDrawable(ISceneGraph*  pScene , int arg);
	virtual ~ISceneDrawable();
	bool isDrawable()      { return true ; }
};

class _XEVOL_BASE_API_ xSceneEffectObject :  public ISceneDrawable
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xSceneEffectObject);
public:
	xSceneEffectObject(ISceneGraph*  pScene , int arg);
	virtual ~xSceneEffectObject();
	virtual bool           load(xXmlNode* pXml) ;
	virtual bool           unload();
	virtual IRenderEffect* effect(size_t idx) ;
	virtual bool           setEffect(IRenderEffect* _effect) ;
	virtual bool           draw(IBaseRenderer* pRenderer , xGeomLib::xCamera* pCamera);
	virtual bool           draw(IBaseRenderer* pRenderer, unsigned int passedTime , xGeomLib::xCamera* pCamera);
protected:
	IRenderEffect* m_pEffect;
};


END_NAMESPACE_XEVOL3D

#endif
