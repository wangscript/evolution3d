#include "../xStdPch.h"
#include "xRendererQueue.h"
#include "xDrawElement.h"
#include <algorithm>
BEGIN_NAMESPACE_XEVOL3D

class  xRenderPassCompare
{
	IRenderObjectCmp* m_cmp;
public:
	xRenderPassCompare(IRenderObjectCmp* cmp)
	{
		m_cmp = cmp;
	}
	bool operator()(xRenderPass* lhv, xRenderPass* rhv)
	{
		return m_cmp->operator ()(lhv->getDrawable() , rhv->getDrawable());
	}

};

xRendererQueue::xRendererQueue(bool bOptForSequnce)
{
	m_Cmper = NULL;
	m_bNeedSorted = false;
}

xRendererQueue::~xRendererQueue()
{
    clear();
}

const  wchar_t* xRendererQueue::name()
{
	return m_Name;
}
int xRendererQueue::idName()
{
	return xStringHash(m_Name);
}

void xRendererQueue::setName(const wchar_t* _name)
{
	wcsncpy(m_Name,_name , 32);
}


size_t xRendererQueue::size()
{
    return m_vRenderPass.size();
}

size_t xRendererQueue::capacity()
{
    return m_vRenderPass.capacity();
}

bool xRendererQueue::reserve(int maxObject)
{
     m_vRenderPass.reserve(maxObject);
	 return true;
}

xRenderPass* xRendererQueue::operator[](size_t index)
{
	return m_vRenderPass[index];
}

size_t xRendererQueue::insert(xRenderPass* drawObject)
{
	 drawObject->AddRef();
	 m_vRenderPass.push_back( drawObject );
	 m_bNeedSorted = true;
     return m_vRenderPass.size() - 1;
}

void xRendererQueue::clear()
{
	size_t nPass = m_vRenderPass.size() ;
    for(size_t i = 0 ; i < nPass; i ++)
	{
		m_vRenderPass[i]->ReleaseObject() ;
	}
	m_vRenderPass.clear();
}

bool xRendererQueue::setComparer(IRenderObjectCmp* cmp)
{
	this->m_Cmper = cmp;
	return true;
}

void xRendererQueue::sort()
{
	if(m_Cmper == NULL && m_bNeedSorted) 
		return ;
	xRenderPassCompare cmp(m_Cmper);
	std::sort(m_vRenderPass.begin() , m_vRenderPass.end() , cmp );
	m_bNeedSorted = false;
}

IMPL_OBJECT_FACTORY_MGR(IRenderObjectCmp , IRenderObjCmpFactory ,  xRenderObjCmpFactoryMgr);

END_NAMESPACE_XEVOL3D
