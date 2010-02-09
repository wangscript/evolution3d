#ifndef __XEVOL3D_DRAWELEMENT_RESOURCE_H__
#define __XEVOL3D_DRAWELEMENT_RESOURCE_H__
#include "xDrawElement.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ IDrawElementResource : public IDrawElement
{
protected:
	IDrawElementResource(IBaseRenderer* pRenderer);
	virtual ~IDrawElementResource();   
	DECL_BASE_OBJECT_DLLSAFE(IDrawElementResource);
public:
	virtual bool          load(const wchar_t* _fileName , void* buf , size_t len , int arg) = 0;
	virtual bool          load(xcomdoc& doc , const wchar_t* _name , int arg ) = 0;
	virtual bool          unload() = 0;
	virtual unsigned  int memUsage() = 0;
	virtual bool          isLoaded() = 0;
};

struct xDrawElementResName
{
public:
	xDrawElementResName(const wchar_t* typeName , const wchar_t* _resName);
	size_t hash_value() const;
	const std::ds_wstring& typeName() const { return _Type ; }
	const std::ds_wstring& resName() const { return _ResName ; }
protected:
	std::ds_wstring _Type;
	std::ds_wstring _ResName;
	size_t          m_hashValue;
};

inline bool operator == (const xDrawElementResName& lhv , const xDrawElementResName& rhv)
{
	return lhv.hash_value() == rhv.hash_value();
}

inline bool operator < (const xDrawElementResName& lhv , const xDrawElementResName& rhv)
{
	return lhv.hash_value() < rhv.hash_value();
}

inline size_t hash_value(const xDrawElementResName& _Keyval)
{
	return ( _Keyval.hash_value());
}

class  _XEVOL_BASE_API_  xDrawElementResLoader : public xResPathMgrProxy
{
protected:
	unsigned int      _getResSize(IDrawElementResource* pRes);
	virtual bool      _isResLoaded(IDrawElementResource* pRes);
	virtual bool      _postLoadResource(const xDrawElementResName resName , IDrawElementResource* & pRes , int& ResSize, unsigned int arg) { return true ; }
	virtual bool      _preLoadResource(const xDrawElementResName& resName , IDrawElementResource* & pRes , int& ResSize, unsigned int arg)  { return true ; }
	virtual bool      _loadResource  (const xDrawElementResName&  resName , IDrawElementResource* & pRes , int& ResSize, unsigned int arg);
	virtual bool      _unloadResource(const xDrawElementResName&  resName , IDrawElementResource* & pRes , unsigned int& TotalResSize);
	virtual void      _deleteResource(const xDrawElementResName&  resName , IDrawElementResource* pRes);
public:		
	void               setRenderer(IBaseRenderer* pRenderer);
	IBaseRenderer*     getRenderer();
	const wchar_t*     ext();
	xBaseTextureMgr*   getTextureMgr();
protected:
	bool               _loadFromFile(const std::ds_wstring& strResName , IDrawElementResource* pRes, unsigned int arg);
	bool               _loadFromPackage(xcomdoc& doc , const std::ds_wstring& _name , IDrawElementResource* pRes, unsigned int arg);
protected:
	xDrawElementResLoader();
	virtual ~xDrawElementResLoader();
protected:
	xBaseTextureMgr*  m_pTexMgr;
	IBaseRenderer*    m_pRenderer;
	std::ds_wstring   m_ext;
};
typedef TResHandle<IDrawElementResource       , xDrawElementResName , xDrawElementResLoader>    HDrawElementRes;
typedef TLRUPolicyResMgr<IDrawElementResource , xDrawElementResName , xDrawElementResLoader>    xDrawElementResMgr;
END_NAMESPACE_XEVOL3D
#endif
