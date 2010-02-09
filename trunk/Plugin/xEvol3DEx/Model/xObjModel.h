#ifndef __OBJ_MODEL_H__
#define __OBJ_MODEL_H__

BEGIN_NAMESPACE_XEVOL3D
class xOBJMesh  : public xCoreMesh
{
public:
	IMPL_BASE_OBJECT_INTERFACE(xOBJModel);
	xOBJMesh(xBaseTextureMgr* pTexMgr , bool bSysMemCopy);
	~xOBJMesh();
public:
	bool load(tMaterialInfo* pMat , t3DObject* pObject , xGeomLib::xaabb& _aabb);
};

class xOBJModel : public xBaseModel
{
public:
	IMPL_BASE_OBJECT_INTERFACE(xOBJModel);
	xOBJModel(xBaseTextureMgr* pTexMgr , bool bSysMemCopy);
	~xOBJModel();
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
