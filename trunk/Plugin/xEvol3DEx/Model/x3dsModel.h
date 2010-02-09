#ifndef __3DS_MODEL_H__
#define __3DS_MODEL_H__
#include "./lib3ds/lib3ds.h"
BEGIN_NAMESPACE_XEVOL3D

struct _3dsVertIndex
{
	int _vert;
	int _texc;
	int _norm;
	int _smthGroup;
	int _matIdx;
	int _idx;
};
struct Lib3DsSubMesh
{
	int                    matIdx;
	std::vector<xvec3i>    Faces;
};
typedef std::vector<Lib3DsSubMesh> vLib3DsSubMesh;
typedef std::vector<_3dsVertIndex> v3dsVertIndex;
typedef std::vector<xMeshMaterial> vMeshMaterial;

class x3dsMesh  : public xCoreMesh
{
public:
	IMPL_BASE_OBJECT_INTERFACE(x3dsModel);
	x3dsMesh(xBaseTextureMgr* pTexMgr , bool bSysMemCopy);
	~x3dsMesh();
public:
	bool load(int nFrame , vMeshMaterial& materials  , Lib3dsMesh* pMesh , Lib3dsMeshInstanceNode *pNode , xGeomLib::xaabb& _aabb);
    bool loadFrameData(int iFrame , vMeshMaterial& materials  , Lib3dsMesh* mesh , Lib3dsMeshInstanceNode * node , xGeomLib::xaabb& _aabb);
protected:
    bool _createIndxBuffer(int nFrame , vMeshMaterial& materials  , Lib3dsMesh* mesh , Lib3dsMeshInstanceNode * node);
protected:
	v3dsVertIndex   m_VertexIndexs;
	vLib3DsSubMesh  m_subMeshList;
};

class x3dsModel : public xBaseModel
{
public:
	IMPL_BASE_OBJECT_INTERFACE(x3dsModel);
	x3dsModel(xBaseTextureMgr* pTexMgr , bool bSysMemCopy);
	~x3dsModel();
public:
	virtual bool      load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	virtual bool      load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg);
public:
	bool              loadSkeleton(xcomdoc& doc , const wchar_t* _dir);
protected:
	void              _setupMesh( xCoreMesh* pMesh);
	int               _findEmbMesh(const wchar_t* _name);
	size_t            _extractMaterials(vMeshMaterial& materials , Lib3dsFile* p3dsFile);
	void              _addTexture(const char* _texfile , xMeshMaterial& mat , int iChannel);
	void              _addMesh(vMeshMaterial& materials , Lib3dsFile *f, Lib3dsMeshInstanceNode *node);
	void              _parseNodes(vMeshMaterial& materials , Lib3dsFile *f, Lib3dsNode *first_node) ;

protected:
	bool              m_bSysMemCopy;
};

END_NAMESPACE_XEVOL3D
#endif
