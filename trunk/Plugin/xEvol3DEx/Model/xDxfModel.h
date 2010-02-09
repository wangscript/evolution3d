#ifndef __DXF_MODEL_H__
#define __DXF_MODEL_H__

BEGIN_NAMESPACE_XEVOL3D

class xDxfMesh  : public xCoreMesh
{
public:
	IMPL_BASE_OBJECT_INTERFACE(xDxfModel);
	xDxfMesh(xBaseTextureMgr* pTexMgr , bool bSysMemCopy);
	~xDxfMesh();
public:
	bool load(tMaterialInfo* pMat , t3DObject* pObject , xGeomLib::xaabb& _aabb);
};

class xDxfModel : public xBaseModel
{
public:
	IMPL_BASE_OBJECT_INTERFACE(xDxfModel);
	xDxfModel(xBaseTextureMgr* pTexMgr , bool bSysMemCopy);
	~xDxfModel();
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
