#ifndef _XSURF_MATERAIL_H__
#define _XSURF_MATERAIL_H__
#include "../RenderAPI/xRenderAPI.h"
BEGIN_NAMESPACE_XEVOL3D

/*

*/
class _XEVOL_BASE_API_ IDrawableObject;
class _XEVOL_BASE_API_ xMaterial : public IRenderObject
{
	DECL_BASE_OBJECT_DLLSAFE(xMaterial);
public:
	wchar_t            m_Name[32];
	int                m_iName;
public:
	xMaterial(IRenderApi* pRenderApi);
	~xMaterial();
	const wchar_t*     name(){return m_Name;}
	int                name_id(){return m_iName;}
public:
	//Resource ID;
	virtual bool       load(const wchar_t* matName , const int8* buf , int len);
	virtual bool       isLoad();
	virtual int        getResSize();
	virtual void       unload();
public:
	virtual bool       begin(IGpuProgramParamTable* pConstTable = NULL);
	virtual bool       end(IGpuProgramParamTable* pConstTable = NULL);

	virtual bool       render(IDrawableObject* pObject ,unsigned long passedTime);
             
};

class _XEVOL_BASE_API_ xMaterialMgr
{
public:
	virtual const wchar_t*     matName(size_t idx ) = 0;
	virtual xMaterial*         loadMaterial(const wchar_t* _name) = 0;
	virtual void               addPath(const wchar_t* path) = 0;
	virtual void               addPackage(const wchar_t* _pkgName , const wchar_t* path) = 0;
	virtual void               addPackage(xcomdoc& doc , const wchar_t* path) = 0;
};

//=================
//-----------------
class _XEVOL_BASE_API_ xRenderEffect : public IRenderObject
{
public:
      DECL_BASE_OBJECT_DLLSAFE(xRenderEffect);
      xRenderEffect(IRenderApi* pRenderApi);
      ~xRenderEffect();
};

END_NAMESPACE_XEVOL3D

#endif
