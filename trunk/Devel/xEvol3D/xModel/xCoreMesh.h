#ifndef __XBASE_MESH_H__
#define __XBASE_MESH_H__
#include "xCoreMeshDef.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ xCoreMesh : public IRenderOtherRes
{
	IMPL_BASE_OBJECT_INTERFACE(xCoreMesh);
public:
	xCoreMesh(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr, bool bSusMemCopy = false);
	bool              load(const wchar_t* fileName , unsigned long  arg);
	bool              load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool              isLoaded();
	bool              unload();
	unsigned long     memUsage();

public:
	bool              load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg);
	bool              load_dir(const wchar_t* _dir , unsigned int arg);
    bool              draw();

protected:
	bool              loadMaterial(xcomdoc& doc ,const wchar_t* _dir);
protected:
	struct xSubMesh
	{
		int           m_nFace;
		int           m_iMatIdx;
		IInputBuffer* m_pIdxBuffer;
	};
    typedef std::vector<xSubMesh, dllsafe_alloc<xSubMesh> > vSubMeshs; 

	int               m_nFace;
	int               m_nVertex;
	ds_wstring        m_name;
	int               m_nSubMesh;
	int               m_nUVMaps;
	xSkeletonID       m_SkeltonID;
	bool              m_bSkin;

	vMeshMaterials    m_Materials;
	vSubMeshs         m_SubMeshs;
	IInputBuffer*     m_pVertexBuffer;
	IInputAssembler*  m_pInputAss;
	IVertexStream*    m_pVertexStream;
protected:
	IRenderApi*       m_pRenderApi;
	xBaseTextureMgr*  m_pTexMgr;
};

END_NAMESPACE_XEVOL3D
#endif
