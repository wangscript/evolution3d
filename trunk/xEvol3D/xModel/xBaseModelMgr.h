#ifndef __XBASE_MODEL_MANANGER_H__
#define __XBASE_MODEL_MANANGER_H__


#include "xModelFWDecl.h"
#include "../BaseLib/xResPackageMgr.h"
#include "../BaseLib/xResourceMgr.h"
#include "xCoreMeshMgr.h"

BEGIN_NAMESPACE_XEVOL3D


class  _XEVOL_BASE_API_  xBaseModelLoader : public xResPathMgrProxy
{
public:  
	HCoreMesh           loadMesh(const wchar_t*  _meshName  , const wchar_t* _ownModelName = NULL , bool bLoadImm = true);
	const wchar_t*      getModelExtName();
	xBaseTextureMgr*    getTextureMgr();

public:
	void                KillObject();
	static              xBaseModelMgr* createInstance(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr , const wchar_t* _name);
protected:
	unsigned int       _getResSize(xBaseModel* pRes);
	virtual bool       _isResLoaded(xBaseModel* pRes);
	virtual bool       _postLoadResource  (const std::ds_wstring& strResName , xBaseModel* & pRes , int& ResSize, unsigned int arg) { return true ; }
	virtual bool       _preLoadResource  (const std::ds_wstring& strResName , xBaseModel* & pRes , int& ResSize, unsigned int arg)  { return true ; }
	virtual bool       _loadResource  (const std::ds_wstring& strResName , xBaseModel* & pRes , int& ResSize, unsigned int arg);
	virtual bool       _unloadResource(const std::ds_wstring& strResName , xBaseModel* & pRes , unsigned int& TotalResSize);
	virtual void       _deleteResource(const std::ds_wstring& strResName , xBaseModel* pRes);
public:			       
	const wchar_t*     ext();
protected:
	bool         _loadFromFile(const std::ds_wstring& strResDir , const std::ds_wstring& strResName , xBaseModel* pRes, unsigned int arg);
	bool         _loadFromPackage(xResPkgPathItem& item , const std::ds_wstring& _name , xBaseModel* pRes, unsigned int arg);
protected:
	xBaseModelLoader();
	virtual ~xBaseModelLoader();
protected:
	IRenderApi*       m_pRenderApi;
	xBaseTextureMgr*  m_pTexMgr;
	xCoreMeshMgr      m_MeshMgr;
	std::ds_wstring   m_ext;
};
END_NAMESPACE_XEVOL3D

#endif
