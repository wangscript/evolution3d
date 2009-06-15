#ifndef __XBASE_MODEL_H__
#define __XBASE_MODEL_H__

#include "../BaseLib/xXmlDocument.h"
#include "../RenderAPI/xRenderAPI.h"
#include "../RenderAPI/xBaseTexture.h"
#include "../RenderAPI/xTextureManager.h"
#include "../xcomdoc/xcomdoc.h"
#include "../XMathLib/XGVolume.h"

#include "../BaseLib/xResPackageMgr.h"
#include "../BaseLib/xResourceMgr.h"


#include "xCoreMesh.h"
#include "xCoreSkeleton.h"

BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ xBaseModel;
class _XEVOL_BASE_API_ xBaseModelLoader;

typedef TResHandle<xBaseModel , std::ds_wstring , xBaseModelLoader>       HBaseModel;
typedef TLRUPolicyResMgr<xBaseModel , std::ds_wstring , xBaseModelLoader> xBaseModelMgr;

class  _XEVOL_BASE_API_  xBaseModelLoader : public xResPathManager
{
public:
	void   KillObject();
	static xBaseModelMgr* createInstance(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr , const wchar_t* _name);
protected:
	unsigned int _getResSize(xBaseModel* pRes);
	virtual bool _isResLoaded(xBaseModel* pRes);
	virtual bool _loadResource  (const std::ds_wstring& strResName , xBaseModel* & pRes , int& ResSize, unsigned int arg);
	virtual bool _unloadResource(const std::ds_wstring& strResName , xBaseModel* & pRes , unsigned int& TotalResSize);
	virtual void _deleteResource(const std::ds_wstring& strResName , xBaseModel* pRes);
protected:
	bool  _loadFromFile(const std::ds_wstring& strResName , xBaseModel* pRes, unsigned int arg);
	bool  _loadFromPackage(xcomdoc& doc , const std::ds_wstring& _name , xBaseModel* pRes, unsigned int arg);
protected:
	xBaseModelLoader();
	virtual ~xBaseModelLoader();
protected:
	IRenderApi*       m_pRenderApi;
	xBaseTextureMgr*  m_pTexMgr;
};

class _XEVOL_BASE_API_ xBaseModel : public IRenderOtherRes
{
    IMPL_BASE_OBJECT_INTERFACE(xBaseModel);
public:
    xBaseModel(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr , bool bSysMemCopy = false);
	bool              load(const wchar_t* fileName , unsigned long  arg);
	bool              load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool              isLoaded();
	bool              unload();
	unsigned long     memUsage();
public:
	bool              load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg);
    bool              draw(); 
	bool              updateFrame(unsigned long passedTime);
	xGeomLib::xaabb&  boundBox();
	xCoreSkeleton*    skeleton(){return m_pSkeleton ; }
	bool              setAction(size_t idx);

public:
	bool              loadSkeleton(xcomdoc& doc , const wchar_t* _dir);
	bool              loadEmbMeshs(xcomdoc& doc , const wchar_t* _dir , xXmlNode* pRootNode);
	bool              loadEmbSkinGroup(xcomdoc& doc , const wchar_t* _dir , xXmlNode* pRootNode);
	bool              createBoneFrameBuffer();
protected:
	struct xModelMesh
	{
		xCoreMesh* m_pMesh;
		//材质等信息放这里
	};
	typedef std::vector<xModelMesh   , std::_dllsafe_alloc<xModelMesh>   > xModelMeshs;
	typedef std::vector<xCoreAction* , std::_dllsafe_alloc<xCoreAction*> > vCoreActions;
	xGeomLib::xaabb   m_aabb;
	std::ds_wstring   m_name;
	xModelMeshs       m_Meshs;	
	vCoreActions      m_Actions;

	xCoreSkeleton*    m_pSkeleton;
	IInputBuffer*     m_pBoneFrameBuffer;
	xCoreActionFrame  m_ActionFrame;
	size_t            m_iActTime;
	size_t            m_iAction;
protected:
	bool              loadModelMeshMateril(xModelMesh& mesh , xXmlNode* pMeshNode);
protected:
	IRenderApi*       m_pRenderApi;
	xBaseTextureMgr*  m_pTexMgr;
};

END_NAMESPACE_XEVOL3D
#endif
