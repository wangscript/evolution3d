#ifndef __XEVOL3D_MODELELEMENT_H__
#define __XEVOL3D_MODELELEMENT_H__
#include "xDrawElement.h"
#include "../xModel/xBaseModelMgr.h"
#include "../xModel/xBaseModel.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ xMeshElement : public IDrawElement
{
public:
	xMeshElement(IBaseRenderer* pRenderer , int v);
	virtual ~xMeshElement();   
	virtual void           setMesh(HCoreMesh hMesh);
	virtual void           center(XMathLib::xvec3&   _center);
	virtual void           aabb(xGeomLib::xaabb&     _aabb);
	virtual void           shpere(xGeomLib::xshpere& _shpere);
	virtual bool           render(unsigned long passedTime);
	virtual bool           update(unsigned long passedTime);
protected:
	HCoreMesh              m_hMesh;
};

typedef std::vector<xMeshElement* , dllsafe_alloc<xMeshElement*> > vMeshElements;

class _XEVOL_BASE_API_ xModelElement : public IDrawElement
{
public:
	xModelElement(IBaseRenderer* pRenderer , int v);
	virtual ~xModelElement();    
	virtual void           center(XMathLib::xvec3&   _center);
	virtual void           aabb(xGeomLib::xaabb&     _aabb);
	virtual void           shpere(xGeomLib::xshpere& _shpere);
	virtual bool           render(unsigned long passedTime);
	virtual bool           update(unsigned long passedTime);
    virtual const wchar_t* modelName();
public:
	bool    setModel(HBaseModel hModel);
public:
	DECL_BASE_OBJECT_DLLSAFE(xModelElement);
protected:
	HBaseModel               m_Model;
	int                      m_actIdx;
	int                      m_actTime;
	xCoreActionFrame*        m_pActionFrame;
};


END_NAMESPACE_XEVOL3D
#endif
