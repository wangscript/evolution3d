#ifndef __XVRC_SCENE_NODE_H__
#define __XVRC_SCENE_NODE_H__
#include "../../../Plugin/xPluginBaseInc.h"
#include "xModel/xBaseModel.h"
#include "xModel/xCoreMesh.h"
#include "Renderer/xRenderer.h"
#include "Renderer/xDrawableObject.h"
#include "XMathLib/XPlacement.h"
USING_NS_XEVOL3D 

class  xVR_SceneNode;
class  XVR_SceneManager;

class _XEVOL_PLUGIN_API_ xVR_SceneNode : public IDrawableObject
{
	IMPL_BASE_OBJECT_INTERFACE(xVR_SceneNode)
	friend class XVR_SceneManager;
	typedef std::vector<xVR_SceneNode* , dllsafe_alloc<xVR_SceneNode*> >  vSceneNodes;
public:
	xVR_SceneNode(XVR_SceneManager* pSceneManager);
	virtual ~xVR_SceneNode();
	virtual void           center(XMathLib::xvec3&   _center);
	virtual void           aabb(xGeomLib::xaabb&     _aabb);
	virtual void           shpere(xGeomLib::xshpere& _shpere);
	
	virtual bool           render(unsigned long passedTime) = 0;
	virtual bool           update(unsigned long passedTime) = 0 ;
    virtual bool           load(xXmlNode* pNode) = 0 ;
	virtual bool           save(xXmlNode* pXMLNode) = 0;
public:
	virtual IRenderApi*    renderApi(){ return m_pRenderApi ; };
	virtual const wchar_t* type();
public:
	void                   setTrans(const xMathLib::xmat4& Trans);
	void                   getTrans(xMathLib::xmat4& trans);
    
	void                   getLocalTrans(xMathLib::xmat4& trans);
	int                    numOfChildren();
	xVR_SceneNode*         getChild(int index);
	void                   removeChild(xVR_SceneNode* pNode);
	const wchar_t*         name();
	xVR_SceneNode*         getParent() {return m_pParent ; }
	void                   setname(const wchar_t* _name);
	bool                   insertChild(xVR_SceneNode* pNode);
	
protected:

	bool                   _invilidateTransMatrix();
protected:
	xMathLib::xmat4          m_Trans;
	xMathLib::xmat4          m_MatrixCached;
	xMathLib::xPlacement     m_Placement;

	xVR_SceneNode*           m_pParent;
	wchar_t                  m_Name[32];
	bool                     m_bAbsulotedMatrixVilidate;
	vSceneNodes              m_Children;
	IRenderApi*              m_pRenderApi;
	XVR_SceneManager*        m_pSceneManager;
};
#endif

