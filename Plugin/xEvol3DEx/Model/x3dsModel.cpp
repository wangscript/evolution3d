#include "../stdafx.h"
#include "x3dsModel.h"
#include "lib3ds_helper.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(x3dsModel , xBaseModel);
IMPL_BASE_OBJECT_CLASSID(x3dsMesh  , xCoreMesh );
IMPL_OBJECT_FACTORY(x3dsModel , xBaseModel , x3dsModelCreator , xBaseModelCreator , xBaseModelCreatorMgr , L"3ds"    , L"3ds model" , xBaseTextureMgr* , bool)

bool operator<(const _3dsVertIndex& lhv , const _3dsVertIndex& rhv)
{
	//如果SmoothGroup大。那肯定就是false
	if(lhv._smthGroup > rhv._smthGroup)
		return false;

	//如果小那就是小了。
	if(lhv._smthGroup < rhv._smthGroup)
		return true;

	return lhv._vert < rhv._vert;
}
typedef std::set<_3dsVertIndex> v3dsVertIndexSet;
size_t AddVertexIndex(v3dsVertIndex& vis , v3dsVertIndexSet& viset , _3dsVertIndex& _vi)
{
	size_t nVertex = vis.size();
	v3dsVertIndexSet::iterator pos = viset.find(_vi);
	if(pos == viset.end() )
	{
		vis.push_back(_vi);
		return nVertex;
	}
	else
	{
		_3dsVertIndex& ret = *pos;
		return ret._idx;
	}
	//----------------
	for(size_t i = 0 ; i < nVertex ; i ++)
	{
		 _3dsVertIndex& _vi2 = vis[i];
		 if(_vi2._vert == _vi._vert && _vi2._smthGroup == _vi._smthGroup)
			 return i;
	}
	vis.push_back(_vi);
    return nVertex;
}


Lib3DsSubMesh& AddSubMesh(vLib3DsSubMesh& subMeshList , int matIdx)
{
	size_t nSubMesh = subMeshList.size();
	for(size_t i = 0 ; i < nSubMesh ; i ++)
	{
		Lib3DsSubMesh& subMesh = subMeshList[i];
		if(subMesh.matIdx == matIdx)
			return subMesh;
	}

	Lib3DsSubMesh subMesh;
	subMesh.matIdx = matIdx;
	subMeshList.push_back(subMesh);
	return subMeshList[nSubMesh];
}

x3dsMesh::x3dsMesh(xBaseTextureMgr* pTexMgr , bool bSysMemCopy) : xCoreMesh(pTexMgr , NULL)
{
    m_nFrame = 1;
}

x3dsMesh::~x3dsMesh()
{

}

static bool __cacl_tbn(xvec3* pTangents , v3dsVertIndex& VertexIdxs , Lib3dsMesh* pMesh , xvec3* pNormals , xvec3i& face)
{
	xvec3  normal[3];
	xvec3  Tangent; 
	xvec3  p[3];

	_3dsVertIndex vertIndex[3];
	vertIndex[0] = VertexIdxs[face.x];
	vertIndex[1] = VertexIdxs[face.y];
	vertIndex[2] = VertexIdxs[face.z];

	normal[0] = (pNormals[ vertIndex[0]._norm ]);
	normal[1] = (pNormals[ vertIndex[1]._norm ]);
	normal[2] = (pNormals[ vertIndex[2]._norm ]);


	p[0] = *(xvec3*)(&pMesh->vertices[ vertIndex[0]._vert ]);
	p[1] = *(xvec3*)(&pMesh->vertices[ vertIndex[1]._vert ]);
	p[2] = *(xvec3*)(&pMesh->vertices[ vertIndex[2]._vert ]);



	xvec2 uv[3];
	uv[0] = *(xvec2*)(&pMesh->texcos[ vertIndex[0]._texc ]);
	uv[1] = *(xvec2*)(&pMesh->texcos[ vertIndex[1]._texc ]);
	uv[2] = *(xvec2*)(&pMesh->texcos[ vertIndex[2]._texc ]);

	xvec3 e1 = p[1] - p[0];
	xvec3 e2 = p[2] - p[0];
	xvec2 u1 ( uv[1].s - uv[0].s , uv[1].t - uv[0].t );
	xvec2 u2 ( uv[2].s - uv[0].s , uv[2].t - uv[0].t );

	float det =  ( u1.s * u2.t - u2.s * u1.t);
	if(det == 0.0f)
	{
		Tangent = e1;
	}
	else
	{
		Tangent = u2.t * e1 - u1.t * e2;
	}

	//从Normal 和 Tangent里重新计算出Tangent,因为面的Tangent和顶点的Normal可能不垂直
	xvec3 final_tangent;
	for(int i = 0 ;i < 3 ; ++i)
	{
		xvec3 binormal  = normal[i].cp(Tangent);
		final_tangent   = binormal.cp(normal[i]);
		final_tangent.normalize();
		pTangents[ face.v[i] ].x += final_tangent.x;
		pTangents[ face.v[i] ].y += final_tangent.y;
		pTangents[ face.v[i] ].z += final_tangent.z;

	}
	return true;
}

bool x3dsMesh::_createIndxBuffer(int nFrame , vMeshMaterial& materials  , Lib3dsMesh* mesh , Lib3dsMeshInstanceNode * node)
{
	m_VertexIndexs.clear();
	m_subMeshList.clear();
	wchar_t _name[256] = {0};
	XEvol_LocaleToUnicode(mesh->name , _name , 256);
	m_name     = _name;
	m_nFace    = mesh->nfaces;
	m_nVertex  = mesh->nfaces ;
	m_nSubMesh = 1;
	m_nUVMaps  = 1; //pObject->numTexVertex > 0 ? 1:0 ;
    m_nFrame   = nFrame;
	m_SkeltonID.m_HiWord = 0;
	m_SkeltonID.m_LoWord = 0;

	v3dsVertIndexSet VertexIndexSet;
	//VertexIndexSet.reserve(m_nFace * 3 );
	m_VertexIndexs.reserve(m_nFace * 3 );
	m_bSkin  = false;

	m_pInputAss = xCoreMesh::CreateInputAssembler(m_nUVMaps , m_pRenderApi , m_bSkin);

	//构造Face. 也就是Index buffer.
	//根据Face的Smoothing Group进行检重
	for(int i = 0 ; i < m_nFace ; i ++)
	{
		xvec3i ib;
		//建立索引
		Lib3dsFace& face = mesh->faces[i];
        int matIdx = face.material;
		if(matIdx < 0) 
		{
			matIdx = materials.size() - 1;
		}

		if(matIdx >= (int)materials.size() )
		{
			if(materials.size() > 1)
			    matIdx %= materials.size()-1;
			else
				matIdx = 1;
		}
		for(int j = 0  ; j < 3 ; j ++)
		{
			_3dsVertIndex _vi;
			_vi._vert = face.index[j];
			_vi._smthGroup = face.smoothing_group;

			int vertIdx = (int)AddVertexIndex(m_VertexIndexs ,VertexIndexSet , _vi);
			_3dsVertIndex& _newvi = m_VertexIndexs[vertIdx];

			_newvi._texc = face.index[j];
			_newvi._norm = i * 3 + j;
			_newvi._matIdx = matIdx;
			_newvi._idx = vertIdx;
			VertexIndexSet.insert(_newvi);
			ib.v[j] = vertIdx;
		}

		Lib3DsSubMesh& subMesh = AddSubMesh(m_subMeshList , matIdx);
		subMesh.Faces.push_back( ib );
	}
    VertexIndexSet.clear();
	m_nSubMesh = (int)m_subMeshList.size();
	m_nVertex  = (int)m_VertexIndexs.size();
    m_SubMeshs.resize(m_nSubMesh);
	//SubMesh的输出
	for(int i = 0 ; i < m_nSubMesh ; i ++)
	{
		Lib3DsSubMesh& subMesh = m_subMeshList[i];

		size_t nFace = subMesh.Faces.size();
		xvec3i* _faceList = new xvec3i[ nFace ];
		for(size_t iFace = 0 ; iFace < nFace ; iFace ++)
		{
			_faceList[iFace] = subMesh.Faces[iFace];
		}
		//Index Buffer.
		xInputBufferDesc idxBufDesc;
		idxBufDesc.m_usage         = RESOURCE_USAGE_DEFAULT;
		idxBufDesc.m_accessFlage   = RESOURCE_ACCESS_NONE;
		idxBufDesc.m_bindtype      = BIND_AS_INDEX_BUFFER;
		m_SubMeshs[i].m_pIdxBuffer = m_pRenderApi->createInputBuffer( nFace *3  , 4 , &idxBufDesc, _faceList);
		m_SubMeshs[i].m_iMatIdx    = i;
		m_SubMeshs[i].m_nFace      = (int)nFace;
		m_Materials.push_back(materials[ subMesh.matIdx % materials.size() ]);
		delete [] _faceList;
	}

	m_VertexBuffers.resize(m_nFrame);

	return true;


}

bool x3dsMesh::loadFrameData(int iFrame , vMeshMaterial& materials  , Lib3dsMesh* mesh , Lib3dsMeshInstanceNode * node , xGeomLib::xaabb& _aabb)
{
	//基准点变换,
	{
		float inv_matrix[4][4], M[4][4];
		float tmp[3];
		int i;

		lib3ds_matrix_copy(M, node->base.matrix);
		lib3ds_matrix_translate(M, -node->pivot[0], -node->pivot[1], -node->pivot[2]);
		lib3ds_matrix_copy(inv_matrix, mesh->matrix);
		lib3ds_matrix_inv(inv_matrix);
		lib3ds_matrix_mult(M, M, inv_matrix);
		for (i = 0; i < mesh->nvertices; ++i) 
		{
			lib3ds_vector_transform(tmp, M, mesh->vertices[i]);
			lib3ds_vector_copy(mesh->vertices[i], tmp);
		}
	}
	bool export_texcos = (mesh->texcos != 0);
	bool export_normals = (mesh->faces != 0);
	if(export_normals == false)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"3ds loader error . Mesh has no faces \n");
		return false;
	}


	m_pVertexStream = m_pInputAss->createVertexStream();



	xvec3* pTangents = NULL;
	xvec3* pNormals  = new xvec3[m_nFace * 3];
	typedef float (*LIB_3DS_NORMAL)[3];
	lib3ds_mesh_calculate_vertex_normals(mesh, (LIB_3DS_NORMAL)pNormals);




	//计算Tangent;
	if(mesh->texcos != NULL)
	{
		pTangents = new xvec3[m_nVertex];
		for(int i = 0 ; i < m_nVertex ; i ++)
		{
			pTangents[i].x = 1.0f;
			pTangents[i].y = 0.0f;
			pTangents[i].z = 0.0f;
		}

		for(int i = 0 ; i < m_nSubMesh ; i ++)
		{
			Lib3DsSubMesh& subMesh = m_subMeshList[i];
			size_t nFace = subMesh.Faces.size();
			for(size_t j = 0 ; j < nFace ; j ++)
			{
				__cacl_tbn(pTangents , m_VertexIndexs , mesh , pNormals , subMesh.Faces[j] );
			}
		}

		for(int i = 0 ; i < m_nVertex ; i ++)
		{
			pTangents[i].normalize();
		}
	}

	//所有的Face构造好了。
	//开始构造VB
	m_aabb.Init(10000000000.0f);
	int  vertSize = sizeof(xStaticMeshVertex) + (m_nUVMaps - 1) * sizeof(xvec2);
	char*  pData = new char[  (vertSize) * (m_nVertex )];
	for(int i = 0 ; i < m_nVertex ; i ++)
	{
		xStaticMeshVertex* pVertex =  (xStaticMeshVertex*)(pData + i * vertSize);
		_3dsVertIndex& _vi = m_VertexIndexs[i];
		xvec4 _vertexColor = xvec4(1.0f , 1.0f , 1.0f , 1.0f);
		if((int)materials.size() > _vi._matIdx)
		{
			xMeshMaterial& mat = materials[_vi._matIdx];
			_vertexColor = xvec4(mat.m_Diffuse.r , mat.m_Diffuse.g , mat.m_Diffuse.b , mat.m_Diffuse.a);
		}
		xvec3&  pPos  = *(xvec3*)mesh->vertices[_vi._vert];
		xvec3&  pNor  = pNormals[ _vi._norm ];
		xvec3   pTan  = xvec3(pNor.x  , pNor.y , pNor.z);
		if( pTangents != NULL)
			pTan = pTangents[ i ]; //只有Tangent的顺序跟Vertex的顺序是一致的


		m_aabb.AddPoint( * (xvec3*)(&pPos) );
		pVertex->m_Position = xvec4(pPos.x  , pPos.y , pPos.z  , 1.0f);
		pVertex->m_Normal   = xvec4(pNor.x  , pNor.y , pNor.z  , 1.0f);
		pVertex->m_Tangent  = xvec4(pTan.x  , pTan.y , pTan.z  , 1.0f);
		pVertex->m_Diffuse  = _vertexColor;
		for(int iUV = 0 ; iUV < m_nUVMaps ; iUV ++)
		{
			if(mesh->texcos != NULL )
			{
				pVertex->m_uv[iUV] = *(xvec2*)mesh->texcos[_vi._texc];
			}
			else
			{
				pVertex->m_uv[iUV] = xvec2(0.5f , 0.5f);
			}

		}
	}
	m_VertexBuffers[iFrame] = m_pVertexStream->createInputBuffer(0 , m_nVertex , pData , NULL);

	_aabb.AddPoint(m_aabb.m_Min);
	_aabb.AddPoint(m_aabb.m_Max);
	delete [] pTangents;
	delete [] pData;
	return true;
}

bool x3dsMesh::load(int nFrame ,  vMeshMaterial& materials  , Lib3dsMesh* mesh , Lib3dsMeshInstanceNode * node , xGeomLib::xaabb& _aabb)
{ 
	xCoreMesh::unload();
	if(false == _createIndxBuffer(nFrame , materials , mesh , node) )
		return false;

	return loadFrameData(0 , materials , mesh , node , _aabb);
}

//------------------------
x3dsModel::x3dsModel(xBaseTextureMgr* pTexMgr , bool bSysMemCopy)
:xBaseModel(pTexMgr , bSysMemCopy)
{
	 m_bSysMemCopy = bSysMemCopy;
	 m_aabb.Init(100000000.0f);
}

x3dsModel::~x3dsModel()
{

}

void x3dsModel::_addTexture(const char* _texfile , xMeshMaterial& mat , int iChannel)
{
	wchar_t _name[256] = {0};
	XEvol_LocaleToUnicode(_texfile , _name , 256);
	xMeshTexture  meshTexture;
	meshTexture.m_TexName  = _name;
	meshTexture.m_iChannel = iChannel;
	if(meshTexture.m_TexName.length() > 0 )
	{
		meshTexture.m_hTexture = m_pTexMgr->add(meshTexture.m_TexName.c_str() , 0 , true);
		mat.m_vTextures.push_back( meshTexture );
	}
}
size_t x3dsModel::_extractMaterials(vMeshMaterial& materials , Lib3dsFile* p3dsFile)
{
	for (int i = 0; i < p3dsFile->nmaterials; ++i)
	{
		Lib3dsMaterial *m = p3dsFile->materials[i];
		xMeshMaterial mat;
		_addTexture(m->texture1_map.name   , mat , 0 );
		_addTexture(m->bump_map.name       , mat , 1 );
		_addTexture(m->opacity_map.name    , mat , 2 );
		_addTexture(m->reflection_map.name , mat , 3 );
		_addTexture(m->specular_map.name   , mat , 4 );
		mat.m_Ambient  = xColor_4f(m->ambient[0] , m->ambient[1] , m->ambient[2]  , 1.0f);
		mat.m_Diffuse  = xColor_4f(m->diffuse[0] , m->diffuse[1] , m->diffuse[2]  , 1.0f);
		mat.m_Speculer = xColor_4f(m->specular[0], m->specular[1], m->specular[2] , 1.0f);
        mat.m_fShiness = m->shininess;
		mat.m_fOpactiy = m->transparency;

		if(mat.m_vTextures.size() == 0 )
		{
			_addTexture("white.tga"   , mat , 0 );
		}
	    if(mat.m_vTextures.size() > 1)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"More than 1 texture map used in the 3ds file\n");
		}
		materials.push_back( mat );
        /*
		fprintf(mtl, "newmtl %s\n", m->name);
		fprintf(mtl, "Ka %f %f %f\n", m->ambient[0], m->ambient[1], m->ambient[2]);
		fprintf(mtl, "Kd %f %f %f\n", m->diffuse[0], m->diffuse[1], m->diffuse[2]);
		fprintf(mtl, "Ks %f %f %f\n", m->specular[0], m->specular[1], m->specular[2]);
		fprintf(mtl, "illum 2\n");
		fprintf(mtl, "Ns %f\n", pow(2, 10 * m->shininess + 1));
		fprintf(mtl, "d %f\n", 1.0 - m->transparency);
		fprintf(mtl, "map_Kd %s\n"  , m->texture1_map.name);
		fprintf(mtl, "map_bump %s\n", m->bump_map.name);
		fprintf(mtl, "map_d %s\n"   , m->opacity_map.name);
		fprintf(mtl, "refl %s\n"    , m->reflection_map.name);
		fprintf(mtl, "map_KS %s\n"  , m->specular_map.name);
		fprintf(mtl, "\n");
		*/
	}

	xMeshMaterial mat;
	_addTexture("white.tga"   , mat , 0 );
	materials.push_back( mat );
	return materials.size();
}
//====================
bool x3dsModel::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	std::ds_wstring _ext = xFileSystem::singleton()->getFileExtName( fileName );
	if(_ext != L"3ds")
	{
		return false;
	}
	Lib3dsFile* p3dsFile = lib3ds_file_wopen(fileName);
	if(p3dsFile == NULL)
		return false;

	vMeshMaterial materials;
	_extractMaterials( materials , p3dsFile);

	if (!p3dsFile->nodes)
		lib3ds_file_create_nodes_for_meshes(p3dsFile);
	
	if(p3dsFile->frames > 1)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"3ds has [%d] frame , but we only use one frame=[] file=%S" , p3dsFile->frames , p3dsFile->current_frame , fileName);
	}
	//如果要做动画的话，在这里下手脚
	lib3ds_file_eval(p3dsFile, 0);
	_parseNodes(materials , p3dsFile, p3dsFile->nodes);
	lib3ds_file_free(p3dsFile);
	return true;
}

void x3dsModel::_addMesh(vMeshMaterial& materials , Lib3dsFile *f, Lib3dsMeshInstanceNode *node) 
{
	 Lib3dsMesh * p3dsMesh = NULL;
	 p3dsMesh = lib3ds_file_mesh_for_node(f, (Lib3dsNode*)node);
	 if (!p3dsMesh || !p3dsMesh->vertices) return;

	 int nFrame = f->frames;
	 if(nFrame < 1) nFrame = 1;
     if(nFrame > 1 )
	 {
          XEVOL_LOG(eXL_DEBUG_HIGH , L"3ds file has animations \n");
	 }
	  x3dsMesh * pMesh = new x3dsMesh(m_pTexMgr , m_bSysMemCopy );
	 if(pMesh->load(nFrame , materials , p3dsMesh , node , m_aabb) == false)
	 {
		 delete pMesh;
		 return ;
	 }
	 _setupMesh(pMesh);
}

void x3dsModel::_parseNodes(vMeshMaterial& materials ,Lib3dsFile *f, Lib3dsNode *first_node) 
{
	Lib3dsNode *p;
	for (p = first_node; p; p = p->next)
	{
		if (p->type == LIB3DS_NODE_MESH_INSTANCE)
		{
			_addMesh(materials , f, (Lib3dsMeshInstanceNode*)p);
			_parseNodes(materials , f, p->childs);
		}
	}
	return ;
}

bool x3dsModel::load(xcomdoc& doc , const wchar_t* _dir , unsigned int arg)
{
    //xcomdoc[_dir.3ds] 目前放在xcomdoc里的模型不支持。
	return false;
}

void x3dsModel::_setupMesh( xCoreMesh* pMesh) 
{
	m_Meshs.push_back(pMesh);
}

bool x3dsModel::loadSkeleton(xcomdoc& doc , const wchar_t* _dir)
{
	//没有骨骼动画的啦
	m_pSkeleton = NULL;    
	return false;
}



END_NAMESPACE_XEVOL3D

