#include "../stdafx.h"
#include "ModelLoader.h"
#include "xDxfModel.h"
#include "DxfParser.h"
#include "HelpFunction.h"

using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xDxfModel , xBaseModel);
IMPL_BASE_OBJECT_CLASSID(xDxfMesh  , xCoreMesh );
IMPL_OBJECT_FACTORY(xDxfModel , xBaseModel , xDxfModelCreator , xBaseModelCreator , xBaseModelCreatorMgr , L"dxf"    , L"Dxf model" , xBaseTextureMgr* , bool)

xDxfMesh::xDxfMesh(xBaseTextureMgr* pTexMgr , bool bSysMemCopy) : xCoreMesh(pTexMgr , NULL)
{

}

xDxfMesh::~xDxfMesh()
{

}
bool xDxfMesh::load(tMaterialInfo* pMat , t3DObject* pObject , xGeomLib::xaabb& _aabb)
{ 
    m_aabb.Init(10000000000.0f);
	xCoreMesh::unload();
	wchar_t _name[256] = {0};
	XEvol_LocaleToUnicode(pObject->strName , _name , 256);
	m_name     = _name;
	m_nFace    = pObject->numOfFaces;
	m_nVertex  = pObject->numOfVerts;
	m_nSubMesh = 1;
	m_nUVMaps  = 1; //pObject->numTexVertex > 0 ? 1:0 ;
    m_nFrame   = 1;
	m_VertexBuffers.resize(m_nFrame);
	m_SkeltonID.m_HiWord = 0;
	m_SkeltonID.m_LoWord = 0;

	m_SubMeshs.resize(m_nSubMesh);
	m_bSkin  = false;

	m_pInputAss = xCoreMesh::CreateInputAssembler(m_nUVMaps , m_pRenderApi , m_bSkin);
	m_pVertexStream = m_pInputAss->createVertexStream();
	xvec3i* _faceList = new xvec3i[m_nFace];
    xvec3* pTangents = NULL;

	//计算Tangent;
	if(pObject->numTexVertex > 0)
	{
		pTangents = new xvec3[m_nVertex];
		for(int i = 0 ; i < m_nVertex ; i ++)
		{
			pTangents[i].x = 0.0f;
			pTangents[i].y = 0.0f;
			pTangents[i].z = 0.0f;
		}
		for(int i = 0 ; i < m_nFace ; i ++)
		{
			tFace& fc = pObject->pFaces[i];
			__cacl_tbn(pTangents , pObject , fc);
		}
		for(int i = 0 ; i < m_nVertex ; i ++)
		{
			pTangents[i].normalize();
		}
	}

 
	std::vector<_VertexIndex> VIArray;
    std::set<_VertexIndex>    VISet;
	for(int i = 0 ; i < m_nFace ; i ++)
	{
		tFace& fc = pObject->pFaces[i];
        xvec3i& ib = _faceList[i];
		for(int j = 0 ; j < 3 ; j ++)
		{
			_VertexIndex _vi;
			_vi.vi = fc.vertIndex[j];
			_vi.ti = 0;

			if(pObject->numTexVertex > 0)
				_vi.ti = fc.coordIndex[j];

			//找到了
			if(VISet.find(_vi) != VISet.end())
			{
				_vi.idx =  find(VIArray , _vi);
				ib.v[j] =  _vi.idx;
			}
			else
			{
				//不存在，加到里面去
				_vi.idx = (int)VIArray.size();
				ib.v[j] = _vi.idx;
				VISet.insert(_vi);
				VIArray.push_back( _vi );
			}
		}
	}
	//所有的Face构造好了。
	//开始构造VB

	m_nVertex = (int)VIArray.size();
	int  vertSize = sizeof(xStaticMeshVertex) + (m_nUVMaps - 1) * sizeof(xvec2);
	char*  pData = new char[  (vertSize) * (m_nVertex )];
	for(int i = 0 ; i < m_nVertex ; i ++)
	{
		xStaticMeshVertex* pVertex =  (xStaticMeshVertex*)(pData + i * vertSize);
		_VertexIndex& _vi = VIArray[i];

		CModelVector3 pPos = pObject->pVerts[_vi.vi];
		CModelVector3 pNor = pObject->pNormals[_vi.vi];
		xvec3         pTan = xvec3(pNor.x  , pNor.y , pNor.z);
		if( pObject->numTexVertex > 0)
			pTan = pTangents[_vi.vi];
		

		m_aabb.AddPoint( * (xvec3*)(&pPos) );
		pVertex->m_Position = xvec4(pPos.x  , pPos.y , pPos.z  , 1.0f);
		pVertex->m_Normal   = xvec4(pNor.x  , pNor.y , pNor.z  , 1.0f);
		pVertex->m_Tangent  = xvec4(pTan.x  , pTan.y , pTan.z  , 1.0f);
		pVertex->m_Diffuse  = xvec4(1.0f     , 1.0f    , 1.0f     , 1.0f);
		for(int iUV = 0 ; iUV < m_nUVMaps ; iUV ++)
		{
			if(pObject->numTexVertex > 0 )
			{
				CModelVector2 pTexCoord = pObject->pTexVerts[_vi.ti];
				pVertex->m_uv[iUV] = xvec2(pTexCoord.x , pTexCoord.y);
			}
			else
			{
				pVertex->m_uv[iUV] = xvec2(0.5f , 0.5f);
			}

		}
	}
	m_VertexBuffers[0] = m_pVertexStream->createInputBuffer(0 , m_nVertex , pData , NULL);


	xInputBufferDesc idxBufDesc;
	idxBufDesc.m_usage         = RESOURCE_USAGE_DEFAULT;
	idxBufDesc.m_accessFlage   = RESOURCE_ACCESS_NONE;
	idxBufDesc.m_bindtype      = BIND_AS_INDEX_BUFFER;
	m_SubMeshs[0].m_pIdxBuffer = m_pRenderApi->createInputBuffer( m_nFace *3  , 4 , &idxBufDesc, _faceList);
	m_SubMeshs[0].m_iMatIdx    = 0;
	m_SubMeshs[0].m_nFace      = m_nFace;

	xMeshMaterial mat;
	xMeshTexture tex;
	tex.m_iChannel = 0;
    tex.m_TexName  = L"";
	if(pMat)
	{
		XEvol_LocaleToUnicode(pMat->strFile , _name , 256);
		tex.m_TexName  = _name;
	}	
	if(tex.m_TexName.length() == 0 )
	{
		tex.m_TexName = L"white.tga";
	}
	tex.m_hTexture = m_pTexMgr->add(tex.m_TexName.c_str() , 0 , true);
	mat.m_vTextures.push_back(tex);
	m_Materials.push_back(mat);
	_aabb.AddPoint(m_aabb.m_Min);
	_aabb.AddPoint(m_aabb.m_Max);
	delete [] pTangents;
	delete [] _faceList;
	delete [] pData;
	return true;
}

//------------------------
xDxfModel::xDxfModel(xBaseTextureMgr* pTexMgr , bool bSysMemCopy)
:xBaseModel(pTexMgr , bSysMemCopy)
{
	 m_bSysMemCopy = bSysMemCopy;
}

xDxfModel::~xDxfModel()
{

}

//====================
bool xDxfModel::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	m_aabb.Init(100000000.0f);
	std::ds_wstring _ext = xFileSystem::singleton()->getFileExtName( fileName );
	t3DModel model;
	if(_ext == L"dxf")
	{
        CLoadDxf loader;
		if( false == loader.ImportDxf( &model , fileName) )
			return false;
	}
	for(int i = 0 ; i < model.numOfObjects ; i ++)
	{
          xDxfMesh * pMesh = new xDxfMesh(m_pTexMgr , m_bSysMemCopy);
		  t3DObject* pObject = &model.pObject[i];
		  tMaterialInfo* pMat = &model.pMaterials[pObject->materialID] ;
		  if( pMesh->load( pMat , pObject , m_aabb) == false)
		  {
			  delete pMesh;
			  continue;
		  }
		  _setupMesh(pMesh , NULL);
		  delete [] pObject->pFaces;
		  delete [] pObject->pNormals;
		  delete [] pObject->pTexVerts;
		  delete [] pObject->pVerts;

	}
	
	model.pMaterials.clear();
	model.pObject.clear();
	return true;
}

bool xDxfModel::_loadModelMeshMateril( xCoreMesh* mesh , xXmlNode* pMeshNode)
{
	return false;
}

bool xDxfModel::load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg)
{
    //xcomdoc[_dir.3ds] 目前放在xcomdoc里的模型不支持。
	return false;
}

void xDxfModel::_setupMesh( xCoreMesh* pMesh, xXmlNode*pMeshNode ) 
{
	_loadModelMeshMateril(pMesh,pMeshNode);
	m_Meshs.push_back(pMesh);
}

bool xDxfModel::loadSkeleton(xcomdoc& doc , const wchar_t* _dir)
{
	//没有骨骼动画的啦
	m_pSkeleton = NULL;    
	return false;
}



END_NAMESPACE_XEVOL3D

