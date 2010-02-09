#include "../xStdPch.h"
#include "xModelElement.h"
#include "xRenderer.h"
#include "xMaterial.h"
#include "../xModel/xCoreMesh.h"
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_DLLSAFE(xModelElement ,  IDrawElement);

IMPL_OBJECT_FACTORY(xModelElement , IDrawElement , xModelElementFactory , IDrawElementCreator , xDrawElementFactoryMgr , L"ModelElement", L"Model Element" , IBaseRenderer* , int);

xModelElement::xModelElement(IBaseRenderer* pRenderer , int v):IDrawElement(pRenderer)
{

	m_actIdx = 0;
	m_actTime = 0;
	m_pActionFrame = NULL;
}
xModelElement::~xModelElement()
{
    m_Model.unload();
}
void xModelElement::center(XMathLib::xvec3&   _center)
{

}
void xModelElement::aabb(xGeomLib::xaabb&     _aabb)
{
	xBaseModel* pModel = m_Model.getResource();
	if(pModel)
	{
		pModel->aabb(_aabb);
	}
}

void xModelElement::shpere(xGeomLib::xshpere& _shpere)
{

}


bool xModelElement::render(unsigned long passedTime)
{
	xBaseModel* pModel = m_Model.getResource();
	if(pModel)
	{
		if(m_pActionFrame == NULL) // ²»ÊÇ¹Ç÷À¶¯»­
		{
			pModel->setupAnimationTime(m_actIdx , (FLOAT)m_actTime);
		}
		pModel->setupAnimation(m_pActionFrame);
		size_t nMesh = pModel->nMesh();
		for(size_t i = 0 ; i < nMesh ; i ++)
		{
			xCoreMesh* pMesh = pModel->getMesh(i);
			pMesh->draw(pModel->skeleton() , m_pActionFrame);
		}
	}
	return true;
}

bool xModelElement::update(unsigned long passedTime)
{
	xBaseModel* pModel = m_Model.getResource();
	if(pModel)
	{
		m_actTime += passedTime;
		pModel->updateFrame(m_pActionFrame , m_actIdx , m_actTime);
	}
	return true;
}
const wchar_t*  xModelElement::modelName()
{
    xBaseModel* pModel = m_Model.getResource();
    if(pModel)
    {
        return pModel->name();
    }
    return NULL;  
}

bool xModelElement::setModel(HBaseModel hModel)
{
	xBaseModel* pModel = hModel.getResource();
	if(pModel)
	{	
		m_actIdx = 0;
		m_actTime = 0;
		m_pActionFrame = NULL;
		if(m_pActionFrame && m_Model.getResource() )
		{
			m_Model->destoryActionFrame(m_pActionFrame);
		}
		this->m_Model = hModel;
		m_pActionFrame = m_Model->createActionFrame();
		return true;
	}
	return false;
}

//---Mesh Element


xMeshElement::xMeshElement(IBaseRenderer* pRenderer , int v) : IDrawElement(pRenderer)
{
}

xMeshElement::~xMeshElement()
{
    m_hMesh.unload();
}

void xMeshElement::setMesh(HCoreMesh hMesh)
{
	m_hMesh = hMesh;
}

void xMeshElement::center(XMathLib::xvec3&   _center)
{

}

void xMeshElement::aabb(xGeomLib::xaabb&     _aabb)
{
	return ;
}

void xMeshElement::shpere(xGeomLib::xshpere& _shpere)
{
	return ;
}


bool xMeshElement::render(unsigned long passedTime)
{
	if(m_hMesh.getResource() == NULL) 
		return false;
	return m_hMesh->draw(NULL , NULL);
}

bool xMeshElement::update(unsigned long passedTime)
{
	return true;
}

END_NAMESPACE_XEVOL3D
