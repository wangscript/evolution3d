#ifndef _XSCENE_NODE_H_
#define _XSCENE_NODE_H_
#include "../RenderAPI/xRenderAPI.h"
#include "../Renderer/xMaterial.h"
#include "../Renderer/xDrawElement.h"
#include "../BaseLib/xProperties.h"
#include "../XMathLib/XPlacement.h"
#include "xSceneObject.h"
#include "xSceneDrawable.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ ISceneNodePropertySet: public xPropertySet
{
	DECL_REFCOUNT_OBJECT_INTERFACE(ISceneNodePropertySet);
public:
	ISceneNodePropertySet(const wchar_t* name , const wchar_t* _desc) : xPropertySet(name , _desc) {};
};
class _XEVOL_BASE_API_ ISceneNode : public IBaseObject
{
public:
	DECL_BASE_OBJECT_DLLSAFE(ISceneNode);
public:
	typedef std::vector<ISceneNode* , dllsafe_alloc<ISceneNode*> > vSceneNodes;
    enum eSceneNodeType
	{
		eSNT_Normal,
		eSNT_Dummy,
		eSNT_SceneGraph,
	};
public:
	ISceneNode(ISceneGraph*  pScene , ISceneNode* pParent = NULL);
	virtual ~ISceneNode();
	virtual const wchar_t*          name();
	virtual void                    setName(const wchar_t* _name);
	virtual void                    setParent(ISceneNode* pNode );
	virtual ISceneNode*             getParent();
	virtual IBaseRenderer*          renderer();
	virtual void                    attachToScene(ISceneGraph* pScene);
    virtual eSceneNodeType          type() = 0;
	virtual const wchar_t*          type_name(){ return m_TypeName.c_str(); }
	virtual void                    type_name(const wchar_t* _name) {m_TypeName = _name ; }
	//子类管理
	virtual size_t                  nChildren() = 0 ;
    virtual bool                    beginEnumChildren() = 0;
	virtual ISceneNode*             nextChild() = 0;
	virtual void                    endEnumChildren() = 0;
	virtual void                    insertChild(ISceneNode* pNode) = 0;
	virtual void                    insertChild(ISceneNode* pNode , const wchar_t* _name) = 0;
	virtual ISceneNode*             findChild(const wchar_t* _name, bool bRecursive = false) = 0;
	virtual bool                    findChildren(ISceneNode::vSceneNodes& _ret) = 0;
	virtual bool                    findChildren(ISceneNode::vSceneNodes& _ret , const wchar_t* _name, bool bRecursive = false) = 0;
	virtual bool                    removeChild(ISceneNode* pNode , bool bRecursive = false) = 0;
    virtual bool                    findChildren(ISceneNode::vSceneNodes& _ret , const xObjectClassID&  classID   ,bool inclChildren = false) = 0; 

    //悬挂的物体操作
	virtual bool                    attachObject(ISceneObject* pObject) = 0;
	virtual bool                    detachObject(ISceneObject* pObject) = 0;
	virtual bool                    detachAllObject() = 0;
    virtual size_t                  nObjects() = 0;
	virtual ISceneObject*           getObject(size_t idx ) = 0;
//public
	virtual bool                    load(xXmlNode* pNode) = 0;
	virtual bool                    unload() = 0;
	virtual bool                    save(xXmlNode* pNode) = 0;
	virtual bool                    updateFrame(unsigned long passedTime, bool bRecursive = false) = 0;
    virtual bool                    bInScene();
	virtual void                    setInScene(bool _bInScene);
	virtual ISceneGraph*            sceneGraph(){return m_pScene ; }
public:
	virtual bool                    loadPlacement(xXmlNode* pNode);
	virtual bool                    savePlacement(xXmlNode* pNode);
	virtual xMathLib::xPlacement*   placement();
	virtual const xMathLib::xmat4&  tansMatrix();
	virtual bool                    updateChildrenTrans();
	virtual bool                    invalidatePlacement();
	virtual bool                    isVisible();
	virtual void                    setVisible(bool bVisible);
	virtual ISceneNodePropertySet*  getPropertySet();
	virtual bool                    initPropertySet();
public:
	template<typename T>  T*        getObject()
	{
		size_t nObj =  nObjects();
		for(size_t i = 0 ; i < nObj ; i ++)
		{
			ISceneObject* pObj = getObject(i);
			T* pRet = dynamic_cast<T*>(pObj);
			if(pRet) return pRet;
		}
		return NULL;
	}

	template<typename T>  T*        getObject(int i )
	{
			ISceneObject* pObj = getObject(i);
			T* pRet = dynamic_cast<T*>(pObj);
			return pRet;
	}

	template<typename T> T*         drawElement()
	{
		size_t nObj =  nObjects();
		for(size_t i = 0 ; i < nObj ; i ++)
		{
			ISceneDrawable* pDrawable = dynamic_cast<ISceneDrawable*>(getObject(i));
			if(pDrawable)
			{
                size_t nEl = pDrawable->nDrawElement();
                for(size_t j = 0 ; j < nEl ; j ++)
                {
                    T* pRet = dynamic_cast<T*>(pDrawable->drawElement(j) );
                    if(pRet)
                        return pRet;
                }
			}
		}
		return NULL;		
	}

	template<typename T> T*         drawElement(size_t iObject , size_t iElement = 0)
	{
		ISceneDrawable* pDrawable = dynamic_cast<ISceneDrawable*>(getObject(iObject));
		if(pDrawable)
		{
			T* pRet = dynamic_cast<T*>(pDrawable->drawElement(iElement) );
			return pRet;
		}
		return NULL;
	}

public:
	//Internal function;
	virtual bool                    onDetached() = 0;
	virtual bool                    onAttached() = 0;
protected:
	ISceneNode*                     m_pParent;
	ISceneGraph*                    m_pScene;
	xMathLib::xPlacement            m_Placement;
	xMathLib::xmat4                 m_trans;
	std::ds_wstring                 m_Name;
	std::ds_wstring                 m_TypeName;
	bool                            m_bInTheScene;
	bool                            m_bVisible;
	bool                            m_bPropertySetInited;
	ISceneNodePropertySet*          m_pPropertySet;
protected:
	static xMathLib::xmat4          m_idTransMat;
};
//-------------------------------------------------------

class _XEVOL_BASE_API_ xSceneBasicNode : public ISceneNode
{
public:
	virtual size_t                   nChildren();
	virtual bool                     beginEnumChildren();
	virtual ISceneNode*              nextChild();
	virtual void                     endEnumChildren();
	virtual void                     insertChild(ISceneNode* pNode);
	virtual void                     insertChild(ISceneNode* pNode , const wchar_t* _name);
	virtual ISceneNode*              findChild(const wchar_t* _name, bool bRecursive);
	virtual bool                     findChildren(ISceneNode::vSceneNodes& _ret);
	virtual bool                     findChildren(ISceneNode::vSceneNodes& _ret , const wchar_t* _name, bool bRecursive = false);
	virtual bool                     removeChild(ISceneNode* pNode , bool bRecursive = false);
							         
	virtual bool                     attachObject(ISceneObject* pObject);
	virtual bool                     detachObject(ISceneObject* pObject);
	virtual bool                     detachAllObject();
	virtual size_t                   nObjects();
	virtual ISceneObject*            getObject(size_t idx );
	//Loading				         
	virtual bool                     load(xXmlNode* pNode);
	virtual bool                     unload();
	virtual bool                     save(xXmlNode* pNode);
	virtual bool                     updateFrame(unsigned long passedTime, bool bRecursive);
	virtual bool                     updateChildrenTrans();
	virtual bool                     initPropertySet(){ return ISceneNode::initPropertySet() ; }
	virtual ISceneNodePropertySet*   getPropertySet();
	virtual bool                     findChildren(ISceneNode::vSceneNodes& _ret , const xObjectClassID&   classID  ,bool inclChildren = false);
protected:					         
	virtual bool                     onDetached();
	virtual bool                     onAttached();
	virtual eSceneNodeType           type(){return eSNT_Normal ;}
public:						         
	DECL_BASE_OBJECT_DLLSAFE(xSceneBasicNode);
	xSceneBasicNode(ISceneGraph*  pScene , ISceneNode* pParent = NULL);
	virtual ~xSceneBasicNode();
protected:
	vSceneNodes                 m_Childrens;	
	int                         m_iCurrentIdx;
	ISceneObject::vSceneObjects m_vObjects;
};


_XEVOL_BASE_API_ ISceneNode* XEvol_InsertSceneNode( xXmlNode* pChildXML , ISceneGraph* pScene , ISceneNode* pParentNode ) ;
//SceneNode Factory defination
class  _XEVOL_BASE_API_ xSceneNodeCreator;
class  _XEVOL_BASE_API_ xSceneNodeFactoryMgr;
DECL_OBJECT_FACTORY_AND_MGR(ISceneNode ,xSceneNodeCreator , xSceneNodeFactoryMgr , ISceneGraph* , ISceneNode* );

#define IMPL_SCENENODE_FACTORY_2(NodeClass , NodeName) IMPL_OBJECT_FACTORY(NodeClass ,ISceneNode ,  NodeClass##Factory , xSceneNodeCreator , xSceneNodeFactoryMgr ,  _TEXT_(NodeName)      ,  _TEXT_(NodeName)      , ISceneGraph* , ISceneNode* );
#define IMPL_SCENENODE_FACTORY(NodeClass) IMPL_OBJECT_FACTORY(NodeClass ,ISceneNode ,  NodeClass##Factory , xSceneNodeCreator , xSceneNodeFactoryMgr ,  _WIDETEXT_(#NodeClass), _WIDETEXT_(#NodeClass) , ISceneGraph* , ISceneNode* );

END_NAMESPACE_XEVOL3D

#endif
