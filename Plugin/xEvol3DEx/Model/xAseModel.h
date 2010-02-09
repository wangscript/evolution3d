#ifndef __ASE_MODEL_H__
#define __ASE_MODEL_H__

BEGIN_NAMESPACE_XEVOL3D
class xAseMesh  : public xCoreMesh
{
public:
	IMPL_BASE_OBJECT_INTERFACE(xAseModel);
	xAseMesh(xBaseTextureMgr* pTexMgr , bool bSysMemCopy);
	~xAseMesh();
public:
	bool load(tMaterialInfo* pMat , t3DObject* pObject , xGeomLib::xaabb& _aabb);
};

class xAseModel : public xBaseModel
{
public:
	IMPL_BASE_OBJECT_INTERFACE(xAseModel);
	xAseModel(xBaseTextureMgr* pTexMgr , bool bSysMemCopy);
	~xAseModel();
public:
	virtual bool      load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	virtual bool      load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg);
public:
	bool              loadSkeleton(xcomdoc& doc , const wchar_t* _dir);
protected:
	bool              _loadModelMeshMateril(xCoreMesh* mesh , xXmlNode* pMeshNode);
	void              _setupMesh( xCoreMesh* pMesh, xXmlNode* pMeshNode );
	int               _findEmbMesh(const wchar_t* _name);

protected:
	bool              m_bSysMemCopy;
};

END_NAMESPACE_XEVOL3D
#endif
