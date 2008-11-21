#ifndef __XREAL_VRC_SCENE_MANAGER_H__
#define __XREAL_VRC_SCENE_MANAGER_H__
#include "vrcSceneNode.h"


class  xVR_SceneNodeFactory
{
public:
	virtual xVR_SceneNode* createNode(const wchar_t* _type , const wchar_t* _name , XVR_SceneManager* pSceneMgr) = 0;
	virtual bool           isNodeTypeSupport(const wchar_t* _type) = 0;
};

class  XVR_SceneManager
{
	
public:
	XVR_SceneManager();
	~XVR_SceneManager();
	void              destory();
	void              deleteNode(xVR_SceneNode* pNode);
	xVR_SceneNode*    createNode( const wchar_t* _typeName , const wchar_t* _Name , xCfgNode* pNode, xVR_SceneNode* pParentNode = NULL);
	bool              update(unsigned long passedTime);
	bool              render(unsigned long passedTim);
	xVR_SceneNode*    getSelected(int x , int y );
	IRenderApi*       renderApi(){return m_pRenderApi ; }

public:
	static bool       registeNodeFactory(xVR_SceneNodeFactory* pFactry);
	static bool       unregisteNodeFactory(xVR_SceneNodeFactory* pFactory);

protected:
	vector<xVR_SceneNode*>    m_RootNodes;
	IRenderApi*               m_pRenderApi;
};

#endif

