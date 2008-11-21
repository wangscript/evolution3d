#ifndef __XVRC_DUMMY_SCENE_NODE_H__
#define __XVRC_DUMMY_SCENE_NODE_H__

#include "vrcSceneNode.h"
USING_NS_XEVOL3D 

class  xVR_DummyNode : public xVR_SceneNode
{
	IMPL_BASE_OBJECT_INTERFACE(xVR_DummyNode)
public:
	xVR_DummyNode(XVR_SceneManager* pMgr);
	const wchar_t*  type(){ return L"DummyNode" ;}
	bool            load(xCfgNode * pNode);
};

#endif

