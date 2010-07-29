#ifndef __XEVOL3D_SCENEOBJECT_RESOURCE_H__
#define __XEVOL3D_SCENEOBJECT_RESOURCE_H__
#include "xSceneObject.h"
BEGIN_NAMESPACE_XEVOL3D

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
	unsigned int      _getResSize(ISceneObject* pRes);
	virtual bool      _isResLoaded(ISceneObject* pRes);
	virtual bool      _postLoadResource(const xSceneObjectResName resName , ISceneObject* & pRes , int& ResSize, unsigned int arg) { return true ; }
	virtual bool      _preLoadResource(const xSceneObjectResName& resName , ISceneObject* & pRes , int& ResSize, unsigned int arg)  { return true ; }
	virtual bool      _loadResource  (const xSceneObjectResName&  resName , ISceneObject* & pRes , int& ResSize, unsigned int arg);
	virtual bool      _unloadResource(const xSceneObjectResName&  resName , ISceneObject* & pRes , unsigned int& TotalResSize);
	virtual void      _deleteResource(const xSceneObjectResName&  resName , ISceneObject* pRes);
public:		
	const wchar_t*     ext();
	ISceneGraph*       sceneGraph();
	void               sceneGraph(ISceneGraph* pSceneGraph);
protected:
	bool               _loadFromFile(const std::ds_wstring& strResName , ISceneObject* pRes, unsigned int arg);
	bool               _loadFromPackage(xcomdoc& doc , const std::ds_wstring& _name , ISceneObject* pRes, unsigned int arg);
protected:
	xSceneObjectResLoader();
	virtual ~xSceneObjectResLoader();
protected:
	xBaseTextureMgr*  m_pTexMgr;
	ISceneGraph*      m_pSceneGraph;
	std::ds_wstring   m_ext;
};
typedef TResHandle<ISceneObject       , xSceneObjectResName , xSceneObjectResLoader>    HDrawElementRes;
typedef TLRUPolicyResMgr<ISceneObject , xSceneObjectResName , xSceneObjectResLoader>    xDrawElementResMgr;
END_NAMESPACE_XEVOL3D
#endif
