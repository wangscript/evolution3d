#ifndef __xCoreActionMgr_h__
#define __xCoreActionMgr_h__

#include "../BaseLib/xXmlDocument.h"
#include "../xcomdoc/xcomdoc.h"
#include "../BaseLib/xResPackageMgr.h"
#include "../BaseLib/xResourceMgr.h"
#include "xCoreAction.h"
#include "xModelFWDecl.h"


BEGIN_NAMESPACE_XEVOL3D

//======Define of the MeshManager
class  _XEVOL_BASE_API_ xCoreActionName
{
public:
	ds_wstring   m_Name    ;    //每个动作有自己的名字. 这个名字可能会挂一个组别
	HBaseModel   m_hModel  ;    //动作隶属的模型
	int          m_strHash    ;
public:
	bool operator == (const xCoreActionName& rhv) const;
	bool operator <  (const xCoreActionName& rhv) const;
	xCoreActionName();
	xCoreActionName(HBaseModel hBaseModel , const wchar_t* actionName = NULL, const wchar_t* skelName = NULL);
	~xCoreActionName();
#ifdef _MSC_VER
	size_t hash_value() const;
#endif
};

#ifdef _MSC_VER
inline size_t hash_value(const xCoreActionName& _Keyval)
{
	return ( _Keyval.hash_value() ^ _HASH_SEED);
}
#endif

/*
动作保存的路径
1. 目录里的
    【PATH】/SkeletonName/skeleton/[ACTIONNAME]
	【PATH】/SkeletonName/[ACTIONNAME]

2. Pkg里的
   【PAKAGE.xrm】【PATH】/SkeletonName/skeleton/[ACTIONNAME]
   【PAKAGE.xrm】【PATH】/SkeletonName/[ACTIONNAME]

*/

class  _XEVOL_BASE_API_  xCoreActionLoader  : public xResPathManager
{
public:
	void   KillObject();
	static xCoreActionMgr* createInstance(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr , const wchar_t* _name);
    void   setActionLoop(bool bLoop) { m_bActionLoop = bLoop ; }
    bool   isActionLoop(){return m_bActionLoop ; }
protected:
	unsigned int  _getResSize(xCoreAction* pRes);
	virtual  bool _isResLoaded(xCoreAction* pRes);
	virtual  bool _postLoadResource  (const xCoreActionName& strResName , xCoreAction* & pRes , int& ResSize, unsigned int arg){ return true ; }
	virtual  bool _preLoadResource  (const xCoreActionName& strResName , xCoreAction* & pRes , int& ResSize, unsigned int arg) { return true ; }
	virtual  bool _loadResource  (const xCoreActionName& strResName , xCoreAction* & pRes , int& ResSize, unsigned int arg);
	virtual  bool _unloadResource(const xCoreActionName& strResName , xCoreAction* & pRes , unsigned int& TotalResSize);
	virtual  void _deleteResource(const xCoreActionName& strResName , xCoreAction* pRes);
protected:
	bool __loadFromDirectory(const xCoreActionName& strResName,xCoreAction* & pCoreMesh , xResPkgPathItem& resPkgItem, int i);
	bool __loadFromPackage(const xCoreActionName& strResName,xCoreAction* & pCoreMesh , xResPkgPathItem& item , int i);
	bool newInstance(const xCoreActionName& name , xCoreAction* & pCoreAction);
protected:
	xCoreActionLoader();
	virtual ~xCoreActionLoader();
protected:
    bool     m_bActionLoop;
};


END_NAMESPACE_XEVOL3D
#endif
