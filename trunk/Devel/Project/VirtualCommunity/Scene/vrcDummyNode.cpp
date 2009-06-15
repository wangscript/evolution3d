#include "vrcDummyNode.h"
IMPL_BASE_OBJECT_CLASSID(xVR_DummyNode , xVR_SceneNode);
xVR_DummyNode::xVR_DummyNode(XVR_SceneManager* pMgr)
:xVR_SceneNode(pMgr)
{
	m_RefCount = 1;
}

bool xVR_DummyNode::load(xXmlNode * pNode)
{

	return true;
}

bool xVR_DummyNode::save(xXmlNode* pXMLNode)
{
	XEVOL_WARNNING_NOT_IMPLEMENT;
	return true;
}