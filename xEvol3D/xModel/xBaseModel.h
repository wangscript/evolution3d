#ifndef __XBASE_MODEL_H__
#define __XBASE_MODEL_H__

#include "../BaseLib/xXmlDocument.h"
#include "../RenderAPI/xRenderAPI.h"
#include "../RenderAPI/xBaseTexture.h"
#include "../RenderAPI/xTextureManager.h"
#include "../xcomdoc/xcomdoc.h"
#include "../XMathLib/xvolume.h"
#include "../BaseLib/xObjectFactory.h"
#include "xModelFWDecl.h"
#include "xCoreSkeleton.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ xBaseModel : public IRenderOtherRes
{
    IMPL_BASE_OBJECT_INTERFACE(xBaseModel);
public:
    xBaseModel(xBaseTextureMgr* pTexMgr , bool bSysMemCopy = false);
	virtual bool           load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;
	virtual bool           load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg) = 0;
	virtual bool           load(const wchar_t* fileName , unsigned long  arg);
	virtual bool           unload();
	virtual unsigned long  memUsage();
	virtual bool           isLoaded();
public:
	
    bool                   draw(xCoreActionFrame* pActionFrame = NULL); 
	bool                   drawMeshs(xCoreActionFrame* pActionFrame = NULL); 
    xCoreActionFrame*      createActionFrame();
	void                   destoryActionFrame(xCoreActionFrame* pAcFrame);
	bool                   updateFrame(xCoreActionFrame* pAcFrame, int actIdx , unsigned long actTime);
	bool                   updateFrame(unsigned long passedTime);
	xGeomLib::xaabb&       boundBox();
	xCoreSkeleton*         skeleton(){ return m_pSkeleton ; }
	bool                   setAction(size_t idx);
    xCoreMesh*             getMesh(size_t idx);
	HCoreMesh              getMeshHandle(size_t idx);
	xCoreMesh*             getMeshPtr(size_t idx);
	size_t                 nMeshPtr();
	size_t                 nMeshHandle();
	size_t                 nMesh();
	bool                   setupAnimationTime(int actIdx , float actTime);
	bool                   setupAnimation();
	bool                   setupAnimation(xCoreActionFrame* pAcFrame);
	bool                   confirmMesh();
	xBaseAction*           coreAction(size_t idx );
	size_t                 nAction();

	bool                   attachMesh(HCoreMesh hMesh);
	bool                   detachMesh(HCoreMesh hMesh);
	bool                   attachMesh(xCoreMesh* pMesh);
	bool                   detachMesh(xCoreMesh* pMesh);

	bool                   createBoneFrameBuffer();
	void                   aabb(xGeomLib::xaabb& _aabb){ _aabb = m_aabb ; }
	const xGeomLib::xaabb& aabb(){return m_aabb ;}
	const wchar_t*         name();
	void                   setName(const wchar_t* _name);

public:	
	typedef ds_vector(xBaseAction*) vCoreActions;
	typedef ds_map(int , int)       mapMeshNameToIdx;
protected:
	xGeomLib::xaabb   m_aabb;
	std::ds_wstring   m_name;
	vCoreActions      m_Actions;
	xCoreSkeleton*    m_pSkeleton;
	IInputBuffer*     m_pBoneFrameBuffer;
	xCoreActionFrame  m_ActionFrame;
	size_t            m_iActTime;
	size_t            m_iAction;

	vCoreMeshs        m_Meshs;	
	vHCoreMeshs       m_hCoreMeshs; //外部加入的Mesh
	mapMeshNameToIdx  m_EmbMeshNameIdx;
	xBaseTextureMgr*  m_pTexMgr;
};

DECL_OBJECT_FACTORY_AND_MGR(xBaseModel, xBaseModelCreator , xBaseModelCreatorMgr , xBaseTextureMgr* , bool);
END_NAMESPACE_XEVOL3D
#endif
