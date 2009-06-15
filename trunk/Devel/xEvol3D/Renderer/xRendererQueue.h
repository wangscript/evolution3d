#ifndef __XR_RENDERER_QUEUE_H__
#define __XR_RENDERER_QUEUE_H__

#include <vector>
#include <list>

#include "xRenderer.h"

BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ IDrawElement;
class _XEVOL_BASE_API_ xRenderPass;
class _XEVOL_BASE_API_ IRenderObjectCmp
{
public:
	virtual bool operator()(IDrawElement* lhv, IDrawElement* rhv) = 0;
};


class _XEVOL_BASE_API_ xRendererQueue
{

public:
	xRendererQueue(bool bOptForSequnce = false);
	~xRendererQueue();
	const  wchar_t*  name();
	int              idName();
	void             setName(const wchar_t* _name);
	size_t           size();
	size_t           capacity();
	bool             reserve(int maxObject);
	xRenderPass*     operator[](size_t index);
	size_t           insert(xRenderPass* drawObject);
    void             clear();
	bool             setComparer(IRenderObjectCmp* cmp);
	void             sort();
	int              priority() const { return m_priority ; }
	void             setPriority(int p ){ m_priority = p ; }

public:
	bool operator    < (const xRendererQueue& rhv) 
	{
		return  priority() < rhv.priority();
	}
public:
	typedef ds_vector(xRenderPass*)    vRenderPass;
	protected:
	IRenderObjectCmp*          m_Cmper;
	vRenderPass                m_vRenderPass;
	bool                       m_bNeedSorted;
	wchar_t                    m_Name[32];
	int                        m_priority;
};



DECL_OBJECT_FACTORY_AND_MGR(IRenderObjectCmp , IRenderObjCmpFactory ,  xRenderObjCmpFactoryMgr , int , int );
END_NAMESPACE_XEVOL3D

#endif
