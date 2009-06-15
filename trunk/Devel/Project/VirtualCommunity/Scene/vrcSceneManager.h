#ifndef __XREAL_VRC_SCENE_MANAGER_H__
#define __XREAL_VRC_SCENE_MANAGER_H__
#include "vrcSceneNode.h"


class _XEVOL_PLUGIN_API_ xVR_SceneNodeFactory
{
public:
	virtual xVR_SceneNode* createNode(const wchar_t* _type , const wchar_t* _name , XVR_SceneManager* pSceneMgr) = 0;
	virtual bool           isNodeTypeSupport(const wchar_t* _type) = 0;
};

class _XEVOL_PLUGIN_API_ XVR_SceneManager : public xVR_SceneNode
{
	
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(XVR_SceneManager);
public:
	XVR_SceneManager(IRenderApi* pRenderApi);
	~XVR_SceneManager();
	void              destory();
	void              deleteNode(xVR_SceneNode* pNode);
	xVR_SceneNode*    createNode( const wchar_t* _typeName , const wchar_t* _Name , xXmlNode* pNode, xVR_SceneNode* pParentNode = NULL);
	bool              update(unsigned long passedTime);
	bool              render(unsigned long passedTim);
	xVR_SceneNode*    getSelected(int x , int y );
	IRenderApi*       renderApi(){return m_pRenderApi ; }

	bool              load(xXmlNode* pNode) ;
	bool              save(xXmlNode* pXMLNode);
	bool              load(const wchar_t* sceneFile);
	bool              save(const wchar_t* );
	HBaseModel        addModel(const wchar_t * file);
	const wchar_t*    type() {return L"SimpleSceneManager" ; }

	xBaseModelMgr*    modelManager(){return m_pModelMager ; }
	xBaseTextureMgr*  textureManager() {return m_pTextureManager; }

public:
	static bool               registeNodeFactory(xVR_SceneNodeFactory* pFactry);
	static bool               unregisteNodeFactory(xVR_SceneNodeFactory* pFactory);
	static XVR_SceneManager*  createSceneManager(const wchar_t* scenManer , IRenderApi* pRenderApi);

protected:
	IRenderApi*               m_pRenderApi;
	xBaseModelMgr*            m_pModelMager;
	xBaseTextureMgr*          m_pTextureManager;
	IDepthStencilState*       m_ZStencil;
};

#endif

