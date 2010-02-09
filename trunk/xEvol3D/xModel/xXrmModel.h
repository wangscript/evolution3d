#ifndef __XRM_MODEL_H__
#define __XRM_MODEL_H__

#include "xBaseModel.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ xXrmModel : public xBaseModel
{
public:
	IMPL_BASE_OBJECT_INTERFACE(xXrmModel);
	xXrmModel(xBaseTextureMgr* pTexMgr , bool bSysMemCopy);
	~xXrmModel();
public:
	virtual bool      load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	virtual bool      load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg);
public:
	bool              loadSkeleton(xcomdoc& doc , const wchar_t* _dir);
	bool              loadAction(xcomdoc& doc , const wchar_t* _dir);
	bool              loadEmbMeshs(xcomdoc& doc , const wchar_t* _dir , xXmlNode* pRootNode);
	bool              loadEmbSkinGroup(xcomdoc& doc , const wchar_t* _dir , xXmlNode* pRootNode);
protected:
	bool              _loadModelMeshMateril(xCoreMesh* mesh , xXmlNode* pMeshNode);
	void              _setupMesh( xCoreMesh* pMesh, xXmlNode* pMeshNode );
	int               _findEmbMesh(const wchar_t* _name);
};

END_NAMESPACE_XEVOL3D
#endif
