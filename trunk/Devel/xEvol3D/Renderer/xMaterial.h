#ifndef _XSURF_MATERAIL_H__
#define _XSURF_MATERAIL_H__
#include "../RenderAPI/xRenderAPI.h"
BEGIN_NAMESPACE_XEVOL3D

/*
*/
class _XEVOL_BASE_API_ xMaterial : public IRenderObject
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xMaterial);
public:
	wchar_t            m_Name[32];
	int                m_iName;
public:
	xMaterial(IRenderApi* pRenderApi);
	~xMaterial();
	bool               load(const wchar_t* matName , const int8* buf , int len);
	bool               isLoad();
	int                getResSize();
	void               unload();
	const wchar_t*     name(){return m_Name;}
	int                name_id(){return m_iName;}
};

END_NAMESPACE_XEVOL3D

#endif
