#ifndef __XVRC_DUMMY_SCENE_NODE_H__
#define __XVRC_DUMMY_SCENE_NODE_H__

#include "vrcSceneNode.h"
USING_NS_XEVOL3D 

class _XEVOL_PLUGIN_API_ xVR_DummyNode : public xVR_SceneNode
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xVR_DummyNode);
public:
	xVR_DummyNode(XVR_SceneManager* pMgr);
	const wchar_t*  type(){ return L"DummyNode" ;}
	bool            load(xXmlNode * pNode);
	bool            save(xXmlNode* pXMLNode);
};

#endif

