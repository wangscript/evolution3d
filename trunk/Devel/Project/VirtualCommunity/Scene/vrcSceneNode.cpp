#include "vrcSceneNode.h"
#include "vrcSceneManager.h"
IMPL_BASE_OBJECT_CLASSID(xVR_SceneNode , IDrawableObject);

bool   loadPlacement(xXmlNode* pXMLNode , xPlacement* placeMent)
{
    const wchar_t* _strRot   = pXMLNode->value(L"rotate");
	const wchar_t* _strTrans = pXMLNode->value(L"trans");
	if(_strRot)
	{
		xquat _quat;
		swscanf(_strRot , L"[%f,%f,%f,%f]" , &_quat.x , &_quat.y , &_quat.z , &_quat.w);
		placeMent->setRotate(_quat);
	}
   
	if(_strTrans)
	{
		xvec3 _trans;
		swscanf(_strTrans , L"[%f,%f,%f]"    , &_trans.x , &_trans.y , &_trans.z );
		placeMent->setPosition(_trans);
	}
	return true;
}

bool   savePlacement(xXmlNode* pXMLNode , xPlacement* placeMent)
{
    wchar_t buf[256];
	xquat& _quat  = placeMent->m_Rotation.m_Quat ;
	xvec3& _trans = placeMent->m_Position;
	swprintf_x(buf,256,L"[%f,%f,%f,%f]" ,_quat.x , _quat.y , _quat.z , _quat.w);
	pXMLNode->setValue(L"rotate" , buf);

	swprintf_x(buf,256,L"[%f,%f,%f]" ,_trans.x , _trans.y , _trans.z);
	pXMLNode->setValue(L"trans" , buf);
	return true;
}

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
bool xVR_SceneNode::save(xXmlNode* pXMLNode)
{
	//º”‘ÿPlacement
	savePlacement(pXMLNode , &m_Placement);
    for(size_t i = 0 ; i < m_Children.size() ; i ++)
	{
		xXmlNode* pChildXML = pXMLNode->insertNode(L"node");
		m_Children[i]->save(pChildXML);
	}
	return true;
}

bool xVR_SceneNode::load(xXmlNode* pXMLNode)
{
	//º”‘ÿAABB

	//º”‘ÿPlacement
    loadPlacement(pXMLNode , &m_Placement);
	xXmlNode::XmlNodes childXmlNodes ;
	pXMLNode->findNode(L"node" , childXmlNodes);
	for(size_t i = 0 ; i < childXmlNodes.size() ; i ++)
	{
          xXmlNode* pChildXML = childXmlNodes[i];
		  const wchar_t* _type = pChildXML->value(L"type");
		  const wchar_t* _name = pChildXML->value(L"name");
		  xVR_SceneNode* pChildNode = m_pSceneManager->createNode(_type,_name,pChildXML,this);
		  pChildNode->load(pChildXML);
		  insertChild(pChildNode);
	} 
	return true;
}

bool xVR_SceneNode::render(unsigned long passedTime)
{
	for(int i = 0 ; i < numOfChildren() ; ++i)
	{
		m_Children[i]->render(passedTime);
	}
	return true;
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



