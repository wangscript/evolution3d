#ifndef __XVRC_MODEL_SCENE_NODE_H__
#define __XVRC_MODEL_SCENE_NODE_H__

#include "vrcSceneNode.h"
USING_NS_XEVOL3D 
class  xVR_ModelNode : public xVR_SceneNode
{
	friend class           XVR_SceneManager;
	HBaseModel             m_hModel;
public:
	xVR_ModelNode(XVR_SceneManager* pSceneManager);
	~xVR_ModelNode();
	void                   setModel(HBaseModel hModel) { m_hModel = hModel ; }
	HBaseModel             getModel()                  { return m_hModel ;}
	void                   render(unsigned long passedTime);
	bool                   update(unsigned long passedTime);
	const wchar_t*         type(){return L"ModelNode" ; }
	bool                   load(xCfgNode *);
};
#endif

