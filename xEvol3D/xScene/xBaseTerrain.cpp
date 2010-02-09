#include "../xStdPch.h"
#include "xBaseTerrain.h"

BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_DLLSAFE(xTerrainBase , ISceneGraph)
xTerrainBase::xTerrainBase(ISceneGraph*  pScene , ISceneNode* pParent)
:ISceneGraph(pScene , pParent)
{
	m_TypeName = L"xTerrainBase";
}

xTerrainBase::~xTerrainBase()
{

}

END_NAMESPACE_XEVOL3D
