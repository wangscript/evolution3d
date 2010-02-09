#ifndef _XSCENE_SIMPLE_VISITOR_H_
#define _XSCENE_SIMPLE_VISITOR_H_
#include "xSceneVisitor.h"
#include "../Renderer/xRenderer.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_  xSceneRenderVisitor : public ISceneVisitor
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xSceneRenderVisitor);
public:
	xSceneRenderVisitor(IBaseRenderer* pRenderer,IRenderCamera* pCamera);
	virtual bool visit(ISceneNode* pNode , ISceneGraph* pScne);
protected:
	IBaseRenderer*           m_pRenderer;
	IRenderCamera*           m_pCamera;
};

class _XEVOL_BASE_API_  xSceneColorSelectVisitor : public ISceneVisitor
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xSceneColorSelectVisitor);
public:
	xSceneColorSelectVisitor(IBaseRenderer* pRenderer,IRenderCamera* pCamera);
	virtual bool visit(ISceneNode* pNode , ISceneGraph* pScne);
protected:
	IBaseRenderer*          m_pRenderer;
	IRenderCamera*          m_pCamera;
};


class _XEVOL_BASE_API_  xSceneUpdateVisitor : public ISceneVisitor
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xSceneUpdateVisitor);
public:
	xSceneUpdateVisitor();
	virtual bool visit(ISceneNode* pNode , ISceneGraph* pScne);
public:
	virtual bool setFrameTime(unsigned long passedTime) ;
protected:
	unsigned long m_PassedTime;

};


class _XEVOL_BASE_API_  xTerrainHeightSelVisitor : public ISceneVisitor
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xTerrainHeightSelVisitor);
public:
	xTerrainHeightSelVisitor(IBaseRenderer* pRenderer, IRenderCamera* pCamera);
	virtual bool visit(ISceneNode* pNode , ISceneGraph* pScene);
	virtual bool bQueue(){ return m_bQueue  ; }
	virtual void bQueue(bool bQue) { m_bQueue = bQue ; }
protected:
	IBaseRenderer*          m_pRenderer;
	IRenderCamera*          m_pCamera;
	bool                    m_bQueue;
};

END_NAMESPACE_XEVOL3D

#endif
