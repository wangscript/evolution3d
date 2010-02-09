#ifndef _XSCENE_MODEL_H_
#define _XSCENE_MODEL_H_
#include "xSceneDrawable.h"
#include "../Renderer/xModelElement.h"
#include "../Renderer/xDrawElement.h"
#include "../Renderer/xMaterial.h"
BEGIN_NAMESPACE_XEVOL3D



class _XEVOL_BASE_API_ xSceneModel : public xSceneEffectObject
{
	DECL_BASE_OBJECT_DLLSAFE(xSceneModel);
public:
	xSceneModel(ISceneGraph*  pScene , int arg);
	~xSceneModel();
	virtual bool           load(xXmlNode* pXml);
	virtual bool           save(xXmlNode* pXml);
	virtual bool           unload();
	virtual size_t         nDrawElement(){ return 1 ;}
	virtual IDrawElement*  drawElement(size_t idx) ;
	virtual bool           setDrawElement(IDrawElement* pDrawElement , size_t idx);
	virtual bool           updateFrame(unsigned long passedTime);
protected:
	xModelElement*         m_pModelElement;
	std::ds_wstring        m_strModelFile;
	std::ds_wstring        m_effectName;
	std::ds_wstring        m_matName;
};

class _XEVOL_BASE_API_ xSceneMesh : public xSceneEffectObject
{
	DECL_BASE_OBJECT_DLLSAFE(xSceneModel);
public:
	xSceneMesh(ISceneGraph*  pScene , int arg);
	~xSceneMesh();
	virtual bool           load(xXmlNode* pXml);
	virtual bool           unload();
	virtual bool           save(xXmlNode* pXml);
	virtual size_t         nDrawElement(){ return 1 ;}
	virtual IDrawElement*  drawElement(size_t idx) ;
	virtual bool           setDrawElement(IDrawElement* pDrawElement , size_t idx);
	virtual bool           updateFrame(unsigned long passedTime);
};


END_NAMESPACE_XEVOL3D

#endif
