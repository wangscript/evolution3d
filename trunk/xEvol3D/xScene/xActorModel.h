#ifndef _XSCENE_ACTOR_H_
#define _XSCENE_ACTOR_H_
#include "../Renderer/xActorDrawElement.h"
#include "../Renderer/xDrawElement.h"
#include "../Renderer/xMaterial.h"
#include "xSceneDrawable.h"

BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_  xSceneActor : public xSceneEffectObject
{
	DECL_BASE_OBJECT_DLLSAFE(xSceneActor);
public:
	xSceneActor(ISceneGraph*  pScene , int arg);
	~xSceneActor();
	virtual bool           load(xXmlNode* pXml);
	virtual bool           save(xXmlNode* pXml);
	virtual bool           unload();
	virtual IDrawElement*  drawElement( size_t iEl) ;
    virtual size_t         nDrawElement() { return 1; }
	virtual bool           setDrawElement(IDrawElement* pDrawElement , size_t iEl);
	virtual bool           updateFrame(unsigned long passedTime);
protected:
	xActorElement*         m_pModelElement;
	std::ds_wstring        m_strModelFile;
	std::ds_wstring        m_effectName;
	std::ds_wstring        m_matName;
};

END_NAMESPACE_XEVOL3D

#endif
