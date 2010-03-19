#ifndef _XSURF_MATERAIL_H__
#define _XSURF_MATERAIL_H__
#include "../RenderAPI/xRenderAPI.h"
#include "xRenderObject.h"
BEGIN_NAMESPACE_XEVOL3D

/*

*/
class _XEVOL_BASE_API_ IDrawElement;
class _XEVOL_BASE_API_ IBaseRenderer;
class _XEVOL_BASE_API_ xMaterial : public IRenderObject
{
	DECL_BASE_OBJECT_DLLSAFE(xMaterial);
public:
	wchar_t            m_Name[32];
	int                m_iName;
public:
	xMaterial(IBaseRenderer* pRenderer);
	~xMaterial();
	const wchar_t*       name(){return m_Name;}
	int                  name_id(){return m_iName;}
public:
	//Resource ID;
	virtual bool         load(const wchar_t* matName , const int8* buf , int len);
	virtual bool         isLoad();
	virtual int          getResSize();
	virtual void         unload();
public:
	virtual bool         begin(IGpuProgramParamTable* pConstTable = NULL);
	virtual bool         end(IGpuProgramParamTable* pConstTable = NULL);
	virtual bool         render(IDrawElement* pObject ,unsigned long passedTime);
    virtual IGpuProgram* gpuProgram() ;
    virtual bool         setGpuProgram(xGpuProgramName& _name);
             
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
class _XEVOL_BASE_API_ IRenderEffect : public IRenderObject
{
public:
	DECL_BASE_OBJECT_DLLSAFE(IRenderEffect);
	IRenderEffect( IBaseRenderer* pRenderer); 
	virtual  ~IRenderEffect();
public:
	virtual bool draw(IDrawElement* pObject , unsigned int passedTime) = 0;
	virtual bool draw(IDrawElement* pObject) = 0;
	virtual bool setMaterial(xMaterial* pMaterial) = 0;
	virtual bool load(xXmlNode* pXml) = 0;
	virtual bool save(xXmlNode* pXml) = 0;
};

END_NAMESPACE_XEVOL3D

#endif
