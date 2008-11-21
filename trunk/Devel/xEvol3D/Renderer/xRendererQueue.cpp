#include "xRendererQueue.h"
#include "xDrawElement.h"
#include <algorithm>
BEGIN_NAMESPACE_XEVOL3D

class  drawElementCmp
{
	IRenderObjectCmp* m_cmp;
public:
	drawElementCmp(IRenderObjectCmp* cmp)
	{
		m_cmp = cmp;
	}
	bool operator()(IDrawElement* lhv, IDrawElement* rhv)
	{
		return m_cmp->operator ()(lhv,rhv);
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

void xRendererQueue::setName(const wchar_t* _name)
{
	wcsncpy(m_Name,_name , 32);
}


size_t        xRendererQueue::size()
{
    return m_vecElement.size();
}

size_t        xRendererQueue::capacity()
{
    return m_vecElement.capacity();
}

bool          xRendererQueue::reserve(int maxObject)
{
     m_vecElement.reserve(maxObject);
	 return true;
}

IDrawElement* xRendererQueue::operator[](size_t index)
{
	return m_vecElement[index];
}

size_t           xRendererQueue::insert(IDrawElement* drawObject)
{
	 m_vecElement.push_back( drawObject );
	 m_bNeedSorted = true;
     return m_vecElement.size() - 1;
}

void          xRendererQueue::clear()
{
    for(size_t i = 0 ; i < m_vecElement.size() ; i ++)
	{
		m_vecElement[i]->ReleaseObject() ;
	}
	m_vecElement.clear();
}

bool          xRendererQueue::setComparer(IRenderObjectCmp* cmp)
{
	this->m_Cmper = cmp;
	return true;
}


void xRendererQueue::sort()
{
	if(m_Cmper == NULL) return ;

	drawElementCmp cmp(m_Cmper);
	std::sort(m_vecElement.begin() , m_vecElement.end() , cmp );
	m_bNeedSorted = false;

}

END_NAMESPACE_XEVOL3D
