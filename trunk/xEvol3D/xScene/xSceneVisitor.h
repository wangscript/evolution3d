#ifndef _XSCENE_VISITOR_H_
#define _XSCENE_VISITOR_H_
#include "../RenderAPI/xRenderAPI.h"
#include "../Renderer/xMaterial.h"
#include "../Renderer/xDrawElement.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ ISceneNode;
class _XEVOL_BASE_API_ ISceneGraph;
class _XEVOL_BASE_API_ ISceneVisitor : public IBaseObject
{
public:
	DECL_BASE_OBJECT_DLLSAFE(ISceneVisitor);
public:
    virtual bool visit(ISceneNode* pNode , ISceneGraph* pScne) = 0;
};


END_NAMESPACE_XEVOL3D

#endif
