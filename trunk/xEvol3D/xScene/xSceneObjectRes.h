#ifndef __XEVOL3D_SCENEOBJECT_RESOURCE_H__
#define __XEVOL3D_SCENEOBJECT_RESOURCE_H__
#include "xSceneObject.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ ISceneObjectRes : public ISceneObject
{
protected:
	ISceneObjectRes(ISceneGraph*  pScene , int arg);
	virtual ~ISceneObjectRes();   
	DECL_BASE_OBJECT_DLLSAFE(ISceneObjectRes);
public:
	virtual bool          load(const wchar_t* _fileName , void* buf , size_t len , int arg) = 0;
	virtual bool          load(xcomdoc& doc , const wchar_t* _name , int arg ) = 0;
	virtual bool          unload() = 0;
	virtual unsigned  int memUsage() = 0;
	virtual bool          isLoaded() = 0;
};

struct xSceneObjectResName
{
public:
	xSceneObjectResName(const wchar_t* typeName , const wchar_t* _resName);
	size_t hash_value() const;
	const std::ds_wstring& typeName() const { return _Type ; }
	const std::ds_wstring& resName() const { return _ResName ; }
protected:
	std::ds_wstring _Type;
	std::ds_wstring _ResName;
	size_t          m_hashValue;
};

inline bool operator == (const xSceneObjectResName& lhv , const xSceneObjectResName& rhv)
{
	return lhv.hash_value() == rhv.hash_value();
}

inline bool operator < (const xSceneObjectResName& lhv , const xSceneObjectResName& rhv)
{
	return lhv.hash_value() < rhv.hash_value();
}

inline size_t hash_value(const xSceneObjectResName& _Keyval)
{
	return ( _Keyval.hash_value());
}

class  _XEVOL_BASE_API_  xSceneObjectResLoader : public xResPathMgrProxy
{
protected:
	unsigned int      _getResSize(ISceneObjectRes* pRes);
	virtual bool      _isResLoaded(ISceneObjectRes* pRes);
	virtual bool      _postLoadResource(const xSceneObjectResName resName , ISceneObjectRes* & pRes , int& ResSize, unsigned int arg) { return true ; }
	virtual bool      _preLoadResource(const xSceneObjectResName& resName , ISceneObjectRes* & pRes , int& ResSize, unsigned int arg)  { return true ; }
	virtual bool      _loadResource  (const xSceneObjectResName&  resName , ISceneObjectRes* & pRes , int& ResSize, unsigned int arg);
	virtual bool      _unloadResource(const xSceneObjectResName&  resName , ISceneObjectRes* & pRes , unsigned int& TotalResSize);
	virtual void      _deleteResource(const xSceneObjectResName&  resName , ISceneObjectRes* pRes);
public:		
	const wchar_t*     ext();
	ISceneGraph*       sceneGraph();
	void               sceneGraph(ISceneGraph* pSceneGraph);
protected:
	bool               _loadFromFile(const std::ds_wstring& strResName , ISceneObjectRes* pRes, unsigned int arg);
	bool               _loadFromPackage(xcomdoc& doc , const std::ds_wstring& _name , ISceneObjectRes* pRes, unsigned int arg);
protected:
	xSceneObjectResLoader();
	virtual ~xSceneObjectResLoader();
protected:
	xBaseTextureMgr*  m_pTexMgr;
	ISceneGraph*      m_pSceneGraph;
	std::ds_wstring   m_ext;
};
typedef TResHandle<ISceneObjectRes       , xSceneObjectResName , xSceneObjectResLoader>    HDrawElementRes;
typedef TLRUPolicyResMgr<ISceneObjectRes , xSceneObjectResName , xSceneObjectResLoader>    xDrawElementResMgr;
END_NAMESPACE_XEVOL3D
#endif
