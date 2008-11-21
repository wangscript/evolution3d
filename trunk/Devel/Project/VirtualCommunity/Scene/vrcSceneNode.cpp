#include "Stdafx.h"
#include "vrcSceneNode.h"
#include "vrcSceneManager.h"
IMPL_BASE_OBJECT_CLASSID(xVR_SceneNode , IDrawableObject);
xVR_SceneNode::xVR_SceneNode(XVR_SceneManager* pSceneManager)
{
	 m_pRenderApi    = pSceneManager->renderApi();
	 m_pSceneManager = pSceneManager;
	 m_MatrixCached.identity();
	 m_Trans.identity();
     m_bAbsulotedMatrixVilidate = false;
	 m_pParent = NULL;
}

xVR_SceneNode::~xVR_SceneNode()
{
	for(int i = 0 ; i < numOfChildren() ; ++i)
	{
		XSAFE_RELEASEOBJECT(m_Children[i]);
	}
	m_Children.clear(); 
}

void xVR_SceneNode::center(XMathLib::xvec3&   _center)
{
    _center.x = m_Trans.m[3][0];
	_center.y = m_Trans.m[3][1];
	_center.z = m_Trans.m[3][2];
}

void xVR_SceneNode::aabb(xGeomLib::xaabb&     _aabb)
{
	_aabb.m_Max = xvec3(0.0f,0.0f,0.0f);
	_aabb.m_Min = xvec3(0.0f,0.0f,0.0f);
}

void xVR_SceneNode::shpere(xGeomLib::xshpere& _shpere)
{
	center(_shpere.m_Orign);
	_shpere.m_R = 0;
}
void xVR_SceneNode::setname(const wchar_t* _name)
{
	wcsncpy(m_Name,_name,32);
}

bool xVR_SceneNode::update(unsigned long passedTime)
{
	for(int i = 0 ; i < numOfChildren() ; ++i)
	{
		m_Children[i]->update(passedTime);
	}
	return true;
}

void xVR_SceneNode::render(unsigned long passedTime)
{
	for(int i = 0 ; i < numOfChildren() ; ++i)
	{
		m_Children[i]->render(passedTime);
	}
}

bool xVR_SceneNode::_invilidateTransMatrix()
{
	 m_bAbsulotedMatrixVilidate = false;
     for(int i = 0 ; i < numOfChildren() ; ++i)
	 {
		 m_Children[i]->_invilidateTransMatrix();
	 }
	 return true;
}

bool xVR_SceneNode::insertChild(xVR_SceneNode* pNode)
{
    m_Children.push_back(pNode);
	return true;
}


void xVR_SceneNode::setTrans(const XMathLib::xmat4& Trans)
{
    m_Trans = Trans;
	_invilidateTransMatrix();
}

void xVR_SceneNode::getTrans(XMathLib::xmat4& trans)
{
	if(m_pParent && m_bAbsulotedMatrixVilidate == false)
	{
          XMathLib::xmat4 mat;
		  m_pParent->getTrans(mat);
		  m_MatrixCached = mat * m_Trans;
		  trans = m_MatrixCached;
		  m_bAbsulotedMatrixVilidate = true;
	}
	if(m_pParent)
	{
		trans = m_MatrixCached;
	}
	else
	{
		trans = m_Trans;
	}    
}

void xVR_SceneNode::getLocalTrans(XMathLib::xmat4& trans)
{
	trans = m_Trans;
}

int xVR_SceneNode::numOfChildren()
{
    return (int)m_Children.size();
}

void xVR_SceneNode::removeChild(xVR_SceneNode* pNode_)
{
	for(vSceneNodes::iterator pos = m_Children.begin() ; pos != m_Children.end() ; pos ++ )
	{
		xVR_SceneNode* pNode = *pos ;
		if(pNode == pNode_)
		{
			m_Children.erase(pos);
			return ;
		}
	}
	return ;    
}

xVR_SceneNode*  xVR_SceneNode::getChild(int index)
{
    if(index >= numOfChildren() || index <0)
		return NULL;
	return m_Children[index];
}

const wchar_t*  xVR_SceneNode::name()
{
    return m_Name;
}

const wchar_t*  xVR_SceneNode::type()
{
    return L"BaseNode";
}

bool  xVR_SceneNode::load(xCfgNode * pNode)
{
	XEVOL_WARNNING_NOT_IMPLEMENT;
	return true;
}


