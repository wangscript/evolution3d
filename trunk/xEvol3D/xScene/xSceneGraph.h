#ifndef _XSCENE_MANAGEMENT_H_
#define _XSCENE_MANAGEMENT_H_
#include "../xModel/xBaseModelMgr.h"
#include "../xModel/xCoreMeshMgr.h"
#include "xSceneNode.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ ISceneVisitor;

struct xSceneInitArg
{
	IBaseRenderer*       m_pRenderer;
	xBaseTextureMgr*     m_pTextureMgr;
	xBaseModelMgr*       m_pModelMgr;
	xResPathManager*     m_pResPathMgr;
};

class _XEVOL_BASE_API_ ISceneGraph  : public xSceneBasicNode
{
public:
	enum eVisitOrder
	{
		eVO_PreOrder,
		eVO_PostOrder
	};
public:
	DECL_BASE_OBJECT_DLLSAFE(ISceneGraph);
public:
	ISceneGraph(ISceneGraph*  pScene , ISceneNode* pParent = NULL);
	virtual ~ISceneGraph();
    virtual void setRenderer(IBaseRenderer* pRenderer);
	virtual IBaseRenderer* renderer();

	virtual xBaseModelMgr*     getModelManager();
	virtual bool               setModelManager(xBaseModelMgr* pMgr) ; 
	virtual xBaseTextureMgr*   getTextureManger() ;
	virtual bool               setTextureManager(xBaseTextureMgr* pTextureManagerpMgr);
	//所有物体的操作，对Scene来说是不需要的
	virtual bool               attachObject(ISceneObject* pObject) { return false ; }
	virtual bool               detachObject(ISceneObject* pObject) { return false ; }
	virtual bool               detachAllObject() { return false ; }
	virtual size_t             nObjects() { return 0 ; }
	virtual ISceneObject*      getObject(size_t idx ) { return NULL ; }
	virtual bool               save(const wchar_t * fileName );
	virtual bool               save(xXmlNode* pNode) = 0;
	virtual bool               load(xXmlNode* pNode) = 0;
	virtual bool               unload();
	virtual IResManager*       findResManager(const wchar_t* _name);
	virtual bool               registeResManager(IResManager* _pResManager);
	virtual eSceneNodeType     type(){ return eSNT_SceneGraph ; }
public:
	//绘制操作
	virtual bool       visit(ISceneVisitor* pVisitor , eVisitOrder  _order = eVO_PreOrder, IRenderCamera* pCamera = NULL) = 0;
	virtual bool       updateFrame(unsigned long passedTime, bool bRecursive) = 0;

	virtual bool       draw(ISceneVisitor* pVisitor , IRenderCamera* pCamera = NULL)   { return visit(pVisitor , eVO_PreOrder , pCamera) ; }
	virtual bool       update(ISceneVisitor* pVisitor , IRenderCamera* pCamera = NULL) { return visit(pVisitor , eVO_PreOrder , pCamera) ; }
	
	void               setPathManager(xResPathManager* pathMgr) { m_pResPathMgr = pathMgr; }
	xResPathManager*   getPathManager(){ return m_pResPathMgr; }
	ISceneGraph*       sceneGraph();

public:
	virtual bool       initPropertySet(){ return xSceneBasicNode::initPropertySet() ; }
protected:
	typedef ds_map(int , IResManager* )   mapResManager;

	IBaseRenderer*       m_pRenderer;
	xBaseTextureMgr*     m_pTextureMgr;
	xBaseModelMgr*       m_pModelMgr;
	mapResManager        m_vResManagers;
	xResPathManager*     m_pResPathMgr;

};

class _XEVOL_BASE_API_ xBasicScene  : public ISceneGraph
{
public:
	DECL_BASE_OBJECT_DLLSAFE(xBasicScene);
public:
	xBasicScene(ISceneGraph*  pScene , ISceneNode* pParent = NULL);
	virtual ~xBasicScene();

public:
	virtual bool       initPropertySet(){ return ISceneGraph::initPropertySet() ; }
	virtual bool       load(xXmlNode* pNode);
	virtual bool       save(xXmlNode* pNode);
	virtual bool       visit(ISceneVisitor* pVisitor , eVisitOrder  _order = eVO_PreOrder, IRenderCamera* pCamera = NULL);
	virtual bool       updateFrame(unsigned long passedTime, bool bRecursive);
protected:
	bool               _visit_preorder(ISceneNode* pSceneNode , ISceneVisitor* pVisitor ,  IRenderCamera* pCamera = NULL);
	bool               _visit_postorder(ISceneNode* pSceneNode , ISceneVisitor* pVisitor ,  IRenderCamera* pCamera = NULL);
};



inline  ISceneNode* XEvol_CreateSceneNode(const wchar_t* _name , ISceneGraph*  pScene , ISceneNode* pParent )
{
	return xSceneNodeFactoryMgr::singleton()->createInstance(_name , pScene , pParent);
}

_XEVOL_BASE_API_ ISceneGraph*    XEvol_CreateScene(const wchar_t* _sceneFile , const xSceneInitArg& arg);

//Object Factory defination
class  _XEVOL_BASE_API_ xSceneCreator;
class  _XEVOL_BASE_API_ xSceneFactoryMgr;
DECL_OBJECT_FACTORY_AND_MGR(ISceneGraph ,xSceneCreator , xSceneFactoryMgr , ISceneGraph* , ISceneNode*);

#define IMPL_SCENE_FACTORY(NodeClass , NodeName   )             \
namespace nsScene_##NodeClass {                                 \
IMPL_OBJECT_FACTORY(NodeClass ,ISceneGraph ,  NodeClass##Factory , xSceneCreator , xSceneFactoryMgr ,  _TEXT_(NodeName),  _TEXT_(NodeName), ISceneGraph* , ISceneNode*);   \
}                                                               \



END_NAMESPACE_XEVOL3D

#endif
