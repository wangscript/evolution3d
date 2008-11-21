#ifndef __XR_RENDERER_QUEUE_H__
#define __XR_RENDERER_QUEUE_H__

#include <vector>
#include <list>

#include "xRenderer.h"

BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ IDrawElement;
class _XEVOL_BASE_API_ IRenderObjectCmp
{
public:
	virtual bool operator()(IDrawElement* lhv, IDrawElement* rhv) = 0;
};


class _XEVOL_BASE_API_ xRendererQueue
{
	IRenderObjectCmp*          m_Cmper;
	std::vector<IDrawElement*> m_vecElement;
	bool                       m_bNeedSorted;
	wchar_t                    m_Name[32];
public:
	xRendererQueue(bool bOptForSequnce = false);
	~xRendererQueue();
	const  wchar_t*  name();
	void             setName(const wchar_t* _name);
	size_t           size();
	size_t           capacity();
	bool             reserve(int maxObject);
	IDrawElement*    operator[](size_t index);
	size_t           insert(IDrawElement* drawObject);
    void             clear();
	bool             setComparer(IRenderObjectCmp* cmp);
	void             sort();
};
END_NAMESPACE_XEVOL3D

#endif
