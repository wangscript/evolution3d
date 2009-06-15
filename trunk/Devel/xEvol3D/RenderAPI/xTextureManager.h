#ifndef _XBASETEXTURE_MANAGER_H_
#define _XBASETEXTURE_MANAGER_H_
#include <vector>

#include "../BaseLib/xEvol3DBaseInc.h"
#include "../BaseLib/xResPackageMgr.h"
#include "../BaseLib/xStringHash.h"
#include "xRenderResManager.h"
#include "xBaseTexture.h"

BEGIN_NAMESPACE_XEVOL3D
class  _XEVOL_BASE_API_  xBaseTextureLoader;
struct _XEVOL_BASE_API_  xTextureName
{
	std::ds_wstring m_Name;
	int             m_Width;
	int             m_Height;
	int             m_Depth;
	ePIXEL_FORMAT   m_fmt;
	int             m_ArraySize;
	bool            m_bR2Texture;
	eResourceType   m_Type;
	bool            m_bLockable;
public:
	xTextureName();
	xTextureName(const wchar_t* fileName);
	xTextureName(const wchar_t* _Name , ePIXEL_FORMAT fmt , eResourceType type ,bool rtt , bool bLockable, int w , int h , int depth , int arraySize);
};

inline bool operator == (const xTextureName& lhv , const xTextureName& rhv)
{
	return xStringHash(lhv.m_Name.c_str() )  == xStringHash(rhv.m_Name.c_str() );
}

inline bool operator < (const xTextureName& lhv , const xTextureName& rhv)
{
	return xStringHash(lhv.m_Name.c_str() )  < xStringHash(rhv.m_Name.c_str() );
}

inline size_t hash_value(const xTextureName& _Keyval)
{
	return ( xStringHash(_Keyval.m_Name.c_str() ));
}


typedef TResHandle      <IBaseTexture , xTextureName , xBaseTextureLoader> HBaseTexture;
typedef TLRUPolicyResMgr<IBaseTexture , xTextureName , xBaseTextureLoader> xBaseTextureMgr;

class  _XEVOL_BASE_API_  xBaseTextureLoader : public xResPathManager
{
	IRenderApi*          m_pRenderApi;
public:	
	xBaseTextureLoader();
	IRenderApi*          renderApi(){return m_pRenderApi ;}
	void                 setRenderApi(IRenderApi* pRenderApi);
	HBaseTexture         load(const xTextureName& texName , bool bLoadImm  , bool arg);
	HBaseTexture         loadTexture(const wchar_t* texName, bool bLoadImm  , bool arg);
	HBaseTexture         loadTexture(const wchar_t* texName , ePIXEL_FORMAT fmt , int w , int h , int depth = 1 , eResourceType type = RESOURCE_TEXTURE2D , int arraySize = 1,  bool rtt  = false , bool bLockable = false) ;
protected:
	virtual unsigned int _getResSize(IBaseTexture* pRes);
	virtual bool         _isResLoaded(IBaseTexture* pRes);
	virtual bool         _loadResource(const xTextureName& strResName   , IBaseTexture* & pRes , int& ResSize, unsigned int arg);
	virtual bool         _unloadResource(const xTextureName& strResName , IBaseTexture* & pRes , unsigned int& TotalResSize);
	virtual void         _deleteResource(const xTextureName& strResName , IBaseTexture* pRes);
protected:
	virtual bool    _loadTexture( const xTextureName& _texName  , IBaseTexture*& pTexture , int& ResSize , unsigned int arg);
	IBaseTexture*   _loadTexture( const xTextureName& _texName , const wchar_t* fileName ,unsigned char* buf , int bufLen , unsigned int arg);
};




END_NAMESPACE_XEVOL3D
#endif
