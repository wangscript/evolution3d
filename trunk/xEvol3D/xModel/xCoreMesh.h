#ifndef __XBASE_MESH_H__
#define __XBASE_MESH_H__
#include "xCoreMeshDef.h"
#include "xModelFWDecl.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ xCoreMesh : public IRenderOtherRes
{
	IMPL_BASE_OBJECT_INTERFACE(xCoreMesh);
public:
	xCoreMesh(xBaseTextureMgr* pTexMgr, xCoreSkeleton* pSkeleton/* = NULL*/);
	bool                    load(const wchar_t* fileName , unsigned long  arg);
	bool                    load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool                    isLoaded();
	bool                    unload();
	unsigned long           memUsage();
public:
	bool                    load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg);
	bool                    load_dir(const wchar_t* _dir , unsigned int arg);
    virtual bool            draw(xCoreSkeleton* pSkeleton , xCoreActionFrame* pActionFrame);
	virtual bool            setCurrentFrame(int iFrame);
    static IInputAssembler* CreateInputAssembler(int nUVChanel , IRenderApi* pRenderapi , bool bSkin);
 protected:
	bool                    loadMaterial(xcomdoc& doc ,const wchar_t* _dir);
	bool                    readSingleFrame(xcomdoc& doc , const wchar_t* _dir , unsigned int arg , bool bInvertVCoord);

   
    bool                    readAnimationFrame(xcomdoc& doc , const wchar_t* _dir , unsigned int arg , bool bInvertVCoord);

protected:
    bool                    doSkin(xCoreSkeleton* pSkeleton , xCoreActionFrame* pActionFrame);
    void                    PrepareNonGpuSkinData() ;
protected:
	struct xSubMesh
	{
		int           m_nFace;
		int           m_iMatIdx;
		IInputBuffer* m_pIdxBuffer;
	};
    typedef std::vector<xSubMesh     , dllsafe_alloc<xSubMesh>      > vSubMeshs; 
	typedef std::vector<IInputBuffer*, dllsafe_alloc<IInputBuffer*> > vInputBuffers;  

	int               m_nFace;
	int               m_nVertex;
	ds_wstring        m_name;
	int               m_nSubMesh;
	int               m_nUVMaps;
	xSkeletonID       m_SkeltonID;
	bool              m_bSkin;

	vMeshMaterials    m_Materials;
	vSubMeshs         m_SubMeshs;
	vInputBuffers     m_VertexBuffers;
	IInputAssembler*  m_pInputAss;
	IVertexStream*    m_pVertexStream;
	int               m_nFrame;
	xGeomLib::xaabb   m_aabb;
    bool              m_bGpuSkin;

    //只有GpuSkin为false的时候，这两个Buffer才有内容
    char*             m_pSkinVertexData;
    char*             m_pStaticVertexData;
    xCoreSkeleton*    m_pSkeleton;
protected:
	xBaseTextureMgr*  m_pTexMgr;
};

END_NAMESPACE_XEVOL3D
#endif
