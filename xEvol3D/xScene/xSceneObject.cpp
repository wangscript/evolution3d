#include "../xStdPch.h"
#include "xSceneObject.h"

BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(ISceneObject     ,  IBaseObject);
IMPL_OBJECT_FACTORY_MGR(ISceneObject      , xSceneObjectCreator , xSceneObjectFactoryMgr )
ISceneObject::ISceneObject(ISceneGraph*  pScene , int )
{
	m_pSceneGraph = pScene;
}
END_NAMESPACE_XEVOL3D
