/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "stdafx.h"
#include "MaxMeshNode.h"
#include "SkeletonExp.h"
#include <algorithm>
#include "BaseLib/xStringHash.h"
using namespace XEvol3DModel;

bool operator < (const MaxNormal& lhv , const MaxNormal& rhv)
{
    char buf[1024];
	sprintf(buf,"%g,%g,%g",lhv.vNormal.x ,lhv.vNormal.y,lhv.vNormal.z );
	int hash_lhv = xStringHash(buf);

	sprintf(buf,"%g,%g,%g",rhv.vNormal.x ,rhv.vNormal.y,rhv.vNormal.z );
	int hash_rhv = xStringHash(buf);

	if(hash_lhv < hash_rhv )
	{
		return true;
	}
	return false;
}

bool operator == (const MaxNormal& lhv , const MaxNormal& rhv)
{
	if(lhv.vNormal == rhv.vNormal )
		return true;
	return false;

	//是同一个点.只要smgroup相同，且不为0，那么就是法向量相同
	if(lhv.iVertex == rhv.iVertex)
	{
		//同一个RVertex顶点，指定了向量，一定相同
		if(lhv.bSpecified == true)
			return true;
		if(lhv.smgroup == rhv.smgroup && lhv.smgroup != 0)
			return true;
		return false;
	}
	//顶点都不相同了，那么一定不相同了。
	return false; 
}

void assign(Point3& point , const sVector_t& v)
{
    point.x = v.x;
    point.y = v.y;
    point.z = v.z;
}

void assign(sVector_t& v , const Point3& point)
{
    v.x = point.z;
    v.y = point.y;
    v.z = point.z;
}

struct sTempWeight_t
{
	sVector_t             m_InitPos;
	vector<sVertWeight_t> m_Weights;
	bool _sort()
	{
		std::sort(m_Weights.begin() , m_Weights.end() , std::greater<sVertWeight_t>() );
		return true;
	}
};

wstring getTextureName(std::wstring Name)
{
    std::wstring::size_type pos;
    std::wstring fileName ;
    pos = Name.find_last_of(L"\\");
    if(pos == std::wstring::npos) 
        fileName = Name;

    fileName = Name.substr(pos + 1);
    return fileName;
}

Point3 CalcNormal(const Point3& p1,const Point3& p2,const Point3& p3)
{
    Point3 e1 = p2-p1;
    Point3 e2 = p3-p1;

    Point3 n = CrossProd(e1,e2);
    n.Normalize();
    return n;
}

void CMaxMesh::__build_vertex_normals(bool isSkin )
{
    m_VertexNormals.clear();
    m_VertexNormals.resize(m_pMesh->numVerts);

    for(size_t i = 0 ; i < m_MeshData.m_IndexBuffers.size() ; ++i )
    {
        sFace_t* faces = m_MeshData.m_IndexBuffers[i];
        for(int iFace = 0 ; iFace < m_MeshData.m_pMeshDesc->m_SubMesh[i].m_nFace ; ++iFace)
        {
            sFace_t& face = faces[iFace];
            sVeretIndex_t  vi[3]; 
            Point3  normal;
            Point3  p[3];

            vi[0] = m_VertexIndexs[ face.vert[0] ];
            vi[1] = m_VertexIndexs[ face.vert[1] ];
            vi[2] = m_VertexIndexs[ face.vert[2] ];

            if(isSkin == false)
            {
                assign(p[0],m_MeshData.m_VertexData.m_Positons[face.vert[0]]);
                assign(p[1],m_MeshData.m_VertexData.m_Positons[face.vert[1]]);
                assign(p[2],m_MeshData.m_VertexData.m_Positons[face.vert[2]]);
            }
            else
            {
                assign(p[0],m_MeshData.m_VertexData.m_VertexWeights[face.vert[0]].m_InitPos);
                assign(p[1],m_MeshData.m_VertexData.m_VertexWeights[face.vert[1]].m_InitPos);
                assign(p[2],m_MeshData.m_VertexData.m_VertexWeights[face.vert[2]].m_InitPos);
            }
            Point3  e1 = p[1] - p[0];
            Point3  e2 = p[2] - p[0];

            normal = CrossProd(e1,e2);
            normal.Normalize();
            for(int j = 0 ; j < 3 ; j++)
            {
                m_VertexNormals[ vi[j].m_iPos ].x  += normal.x ;
                m_VertexNormals[ vi[j].m_iPos ].y  += normal.y ;
                m_VertexNormals[ vi[j].m_iPos ].z  += normal.z ;
            }    


        }
    }

    for(int i  = 0 ; i < m_pMesh->numVerts ; ++i)
    {
        m_VertexNormals[i].normalize();
    }

    int nVert  = (int)m_VertexIndexs.size();
    m_MeshData.m_VertexData.m_VNormals.resize(nVert);
    for(int i = 0 ; i < nVert ; ++i)
    {
        sVeretIndex_t& Vi = m_VertexIndexs[i];
        m_MeshData.m_VertexData.m_VNormals[i] = m_VertexNormals[Vi.m_iPos];
    }

}

void  CMaxMesh::__build_face_normals()
{
    m_FaceNormals.clear();
    Matrix3 nrmMat = m_pNode->GetObjTMAfterWSM(m_iMaxTime);
    nrmMat.NoTrans();
    for(size_t iIndexBuffer = 0 ; iIndexBuffer < m_MeshData.m_IndexBuffers.size() ; iIndexBuffer++ )
    {
        sFace_t* index_buffer  = m_MeshData.m_IndexBuffers[iIndexBuffer];
        int nFace = m_MeshData.m_pMeshDesc->m_SubMesh[iIndexBuffer].m_nFace;
        for(int iFace = 0 ; iFace < nFace ; ++iFace)
        {
            sFace_t& face = index_buffer[iFace];
            Point3 p1,p2,p3;
            p1.x = m_MeshData.m_VertexData.m_Positons[face.vert[0]].x;
            p1.y = m_MeshData.m_VertexData.m_Positons[face.vert[0]].y;
            p1.z = m_MeshData.m_VertexData.m_Positons[face.vert[0]].z;

            p2.x = m_MeshData.m_VertexData.m_Positons[face.vert[1]].x;
            p2.y = m_MeshData.m_VertexData.m_Positons[face.vert[1]].y;
            p2.z = m_MeshData.m_VertexData.m_Positons[face.vert[1]].z;

            p3.x = m_MeshData.m_VertexData.m_Positons[face.vert[2]].x;
            p3.y = m_MeshData.m_VertexData.m_Positons[face.vert[2]].y;
            p3.z = m_MeshData.m_VertexData.m_Positons[face.vert[2]].z;

            Point3 n = CalcNormal(p1,p2,p3);


            sVector_t nv;
            nv.x = n.x;nv.y = n.y;nv.z = n.z;

            m_FaceNormals.push_back(nv);
        }
    }
}



void  CMaxMesh::__build_skin_face_normals()
{
    m_FaceNormals.clear();
    Matrix3 nrmMat = m_pNode->GetObjTMAfterWSM(m_iMaxTime);
    nrmMat.NoTrans();
    for(size_t iIndexBuffer = 0 ; iIndexBuffer < m_MeshData.m_IndexBuffers.size() ; iIndexBuffer++ )
    {
         sFace_t* index_buffer  = m_MeshData.m_IndexBuffers[iIndexBuffer];
         int nFace = m_MeshData.m_pMeshDesc->m_SubMesh[iIndexBuffer].m_nFace;
         for(int iFace = 0 ; iFace < nFace ; ++iFace)
         {
             sFace_t& face = index_buffer[iFace];
             Point3 p1,p2,p3;
             p1.x = m_MeshData.m_VertexData.m_VertexWeights[face.vert[0]].m_InitPos.x;
             p1.y = m_MeshData.m_VertexData.m_VertexWeights[face.vert[0]].m_InitPos.y;
             p1.z = m_MeshData.m_VertexData.m_VertexWeights[face.vert[0]].m_InitPos.z;

             p2.x = m_MeshData.m_VertexData.m_VertexWeights[face.vert[1]].m_InitPos.x;
             p2.y = m_MeshData.m_VertexData.m_VertexWeights[face.vert[1]].m_InitPos.y;
             p2.z = m_MeshData.m_VertexData.m_VertexWeights[face.vert[1]].m_InitPos.z;

             p3.x = m_MeshData.m_VertexData.m_VertexWeights[face.vert[2]].m_InitPos.x;
             p3.y = m_MeshData.m_VertexData.m_VertexWeights[face.vert[2]].m_InitPos.y;
             p3.z = m_MeshData.m_VertexData.m_VertexWeights[face.vert[2]].m_InitPos.z;

             Point3 n = CalcNormal(p1,p2,p3);


             sVector_t nv;
             nv.x = n.x;nv.y = n.y;nv.z = n.z;

             m_FaceNormals.push_back(nv);
         }
    }
}

Point3   CMaxMesh::__get_smgrp_normal(int iNormal)
{
    MaxNormal& max_normal = m_MaxNormals[iNormal];
	return max_normal.vNormal;
    return  __get_smgrp_normal(max_normal);

}

Point3   CMaxMesh::__get_smgrp_normal(const MaxNormal& max_normal)
{
    int vertexId = max_normal.iVertex;
    int faceId   = max_normal.iFace;
    // get the "rendered" vertex
    RVertex *pRVertex;
    pRVertex = m_pMesh->getRVertPtr(vertexId);

    // get the face
    Face *pFace;
    pFace = &m_pMesh->faces[faceId];

    // get the smoothing group of the face
    DWORD smGroup;
    smGroup = pFace->smGroup;

    // get the number of normals
    int normalCount;
    normalCount = pRVertex->rFlags & NORCT_MASK;

    // check if the normal is specified ...
    if(pRVertex->rFlags & SPECIFIED_NORMAL)
    {
        return pRVertex->rn.getNormal();
    }
    // ... otherwise, check for a smoothing group
    else if((normalCount > 0) && (smGroup != 0))
    {
        // If there is only one vertex is found in the rn member.
        if(normalCount == 1)
        {
            return pRVertex->rn.getNormal();
        }
        else
        {
            int normalId;
            for(normalId = 0; normalId < normalCount; normalId++)
            {
                if(pRVertex->ern[normalId].getSmGroup() & smGroup)
                {
                    return pRVertex->ern[normalId].getNormal();
                }
            }
        }
    }

    // if all fails, return the face normal
    return m_pMesh->getFaceNormal(faceId);
}

void CMaxMesh::__clear_index_buf()
{
    size_t nIB = m_MeshData.m_IndexBuffers.size();
    for(size_t i = 0 ; i < nIB ;  ++i)
    {
        IndexBuffer_t pIndexBuffer = m_MeshData.m_IndexBuffers[i];
        if(pIndexBuffer)
        {
            delete [] pIndexBuffer;
        }
    }
    m_MeshData.m_IndexBuffers.clear();
    nIB = m_MeshData.m_IndexBuffers.size();
}



int  CMaxMesh::__add_smgrp_normal(const MaxNormal& max_normal , MaxNormalSet_t& nSet)
{
	MaxNormalSet_t::iterator pos = nSet.find(max_normal);
	if(pos != nSet.end() )
	{
		return pos->second;
	}
	else
	{
		int n = (int)m_MaxNormals.size();
		m_MaxNormals.push_back(max_normal);
		nSet[max_normal] = n;
		return n ;
	}

    for(size_t i = 0 ; i < m_MaxNormals.size() ; ++i)
    {
        MaxNormal& o_normal = m_MaxNormals[i];
        if(o_normal == max_normal)
            return (int)i;
    }
	//MaxNormalIndexs::iterator pos =  m_MaxNormalIndex.find(max_normal);
	//if(pos != m_MaxNormalIndex.end() )
	//{
	//	return pos->second;
	//}
    int n = (int)m_MaxNormals.size();
    m_MaxNormals.push_back(max_normal);
	//m_MaxNormalIndex.insert( MaxNormalIndexs::value_type(max_normal , n) );
    return n;
}

void CMaxMesh::__build_smgrp_normal()
{
    m_MaxNormals.clear();
	m_MaxNormalIndex.clear();
    m_NormalFaces.clear();
	MaxNormalSet_t MaxNormalSet;
    for(int i = 0 ; i < m_pMesh->numFaces; ++i)
    {
        Face& face = m_pMesh->faces[i];
        NFace NormalFace ;
        for(int iv = 0 ; iv < 3 ; ++iv)
        {
            MaxNormal mmormal;
            RVertex *pRVertex = m_pMesh->getRVertPtr(face.v[iv]);
            if(pRVertex->rFlags & SPECIFIED_NORMAL)
            {
                mmormal.bSpecified = true;
            }
            else
            {
                mmormal.bSpecified = false;
            }
            mmormal.iFace = i;
            mmormal.iVertex = face.v[iv];
            mmormal.smgroup = face.smGroup; 
			mmormal.vNormal = __get_smgrp_normal(mmormal);
            NormalFace.normal[iv] = __add_smgrp_normal(mmormal , MaxNormalSet);
        }
        m_NormalFaces.push_back(NormalFace);         
    }
	MaxNormalSet.clear();
}

void CMaxMesh::_free_face_info()
{
	//free face data
    m_MeshData.m_BoundBox.clear(100000000.0f);
    m_MeshData.m_VertexData.clear();
	m_MeshData.m_Materials.clear();

    //clear other data;
    m_VertexIndexs.clear();
    m_vStdMtls.clear();
    m_UsedMapChannels.clear();

    m_MaxNormals.clear();
    m_NormalFaces.clear();
	m_MaxNormalIndex.clear();
    __clear_index_buf(); 
	m_State = eFaceNotCreated;
    if(m_MeshData.m_pMeshDesc)
    {
        ::free(m_MeshData.m_pMeshDesc);
        m_MeshData.m_pMeshDesc = NULL;
    }
}

void CMaxMesh::_init_data()
{
    m_pMesh = NULL;
    m_pNode = NULL;
    m_MeshDataFlag._default();
    m_triObjNeedDelete = NULL;
    m_iMaxTime  = 0;
    m_iFrame    = 0;
    m_bMirrored = false;

    m_MeshData.m_pMeshDesc = NULL;
    m_MeshData.m_VertexData.clear();
    m_MeshData.m_Materials.clear();
	m_MeshData.m_IndexBuffers.clear();


    m_vStdMtls.clear();
	m_VertexIndexs.clear();
    m_UsedMapChannels.clear();
    

	m_MeshDataFlag.m_fColorScale[0] = 1.0f;
	m_MeshDataFlag.m_fColorScale[1] = 1.0f;
	m_MeshDataFlag.m_fColorScale[2] = 1.0f;
	m_MeshDataFlag.m_fColorScale[3] = 1.0f;
	m_MeshData.m_fFps  = CMaxEnv::singleton().getFps();
}

int CMaxMesh::__count_used_channel(sMaxMatrial_t& mat)
{
    vector<int> v;
    int sum = 0;
    for(size_t i = 0 ; i < mat.m_TexLayers.size() ; ++i)
    {
        for(size_t j = 0 ; j < v.size() ; ++j)
        {
            if(v[j] == mat.m_TexLayers[i].m_MTL.m_UVChannel)
                goto _countinue;
        }
        v.push_back(mat.m_TexLayers[i].m_MTL.m_UVChannel);
_countinue:
        ;
    }
    return (int)v.size();
}

bool CMaxMesh::__is_mat_used(int matID)
{
    matID = matID % (int)m_vStdMtls.size();
    for(int i = 0 ; i < m_pMesh->getNumFaces(); ++i)
    {
        Face& face = m_pMesh->faces[i];
        int faceMatID = face.getMatID();
        faceMatID = faceMatID % (int)m_vStdMtls.size();
        if(matID == faceMatID)
            return true;
    }
    return false;
}

/***
删除没有用到的材质
遍历所有的材质，把没有使用到过的材质删除掉
***/
void CMaxMesh::__remove_unused_mat()
{
    size_t nMat = m_MeshData.m_Materials.size();
    Materials_t::iterator it = m_MeshData.m_Materials.begin();
    for( ; it !=  m_MeshData.m_Materials.end() ;  )
    {
        sMaxMatrial_t& mat =  *it;
        //判断这个ID的材质是否被任何一面使用到过
        if(!__is_mat_used(mat.m_matID))
        {
            it = m_MeshData.m_Materials.erase(it);
        }
        else
        {
            ++it;
        }
    }

}

/*
检测两个材质使用的MapChannel是不是相同
*/
bool CMaxMesh::__is_mat_tex_layer_map_same(sMaxMatrial_t& lhv,sMaxMatrial_t& rhv)
{
	/*
    if(lhv.m_TexLayers.size() != rhv.m_TexLayers.size())
        return false;
    for(size_t i = 0 ; i < lhv.m_TexLayers.size() ; ++i)
    {
        if(lhv.m_TexLayers[i].m_MTL.m_UVChannel != rhv.m_TexLayers[i].m_MTL.m_UVChannel)
            return false;
    }
	*/
    return true;
}

void CMaxMesh::_freeMeshData()
{
    _free_face_info();

	m_MeshData.m_UsedBones.clear();
    m_MeshData.m_VertexData.clear();
    if(m_triObjNeedDelete)
    {
        m_triObjNeedDelete->DeleteMe();
        m_triObjNeedDelete = NULL;
    }
    m_pMesh = NULL;
    if(m_MeshData.m_pMeshDesc)
    {
        ::free((void*)m_MeshData.m_pMeshDesc);
        m_MeshData.m_pMeshDesc = NULL;
    }
}

bool CMaxMesh::_buildIMesh(unsigned int iMaxTime)
{

    class NullView: public View 
    {
    public:
        Point2 ViewToScreen(Point3 p) 
        { 
            return Point2(p.x,p.y); 
        }
        NullView() 
        { 
            worldToView.IdentityMatrix(); 
            screenW=640.0f; screenH = 480.0f; 
        }

    };

    Object* pObj = NULL;
    Mesh *pMesh;
    BOOL  bNeedDelete = false;
    //测试是不是一个Mesh

    ObjectState os = m_pNode->EvalWorldState(iMaxTime);
    pObj = os.obj;
    //这个地方有泄露．参加，discreet唯一的一个教程的代码
    TriObject * triObject = (TriObject *)pObj->ConvertToType(iMaxTime, triObjectClassID);
    m_triObjNeedDelete = NULL;

    if(triObject == NULL)
    {
        NullView view;
        GeomObject* pGeoObj = (GeomObject*)pObj;
        pMesh = pGeoObj->GetRenderMesh( iMaxTime , m_pNode , view , bNeedDelete );
    }
    else
    {
        //pMesh = &triObject->GetMesh();
        //this->m_bDelete = false;
        NullView view;
        GeomObject* pGeoObj = (GeomObject*)pObj;
        pMesh = pGeoObj->GetRenderMesh( iMaxTime , m_pNode , view , bNeedDelete );
        //this->m_bDelete = false;
    }

    //需要删除的指针
    if(triObject != pObj)
        m_triObjNeedDelete = triObject;

    m_pMesh = pMesh;

    return m_pMesh!=NULL;
}

/***
通过一个MatID，
去查找它在导出Material列表中的索引
***/
int CMaxMesh::__find_mat_by_matid(int matID)
{
    matID = matID % (int)m_vStdMtls.size();
    size_t nMat = m_MeshData.m_Materials.size();
    for(size_t i = 0 ; i < nMat ; ++i)
    {
        sMaxMatrial_t& mat = m_MeshData.m_Materials[i]; 
        if(mat.m_matID == matID)
            return (int)i;
    }
    return -1;
}

bool CMaxMesh::_buildMaterialInfo(sMaxMatrial_t& mat, StdMat* pMat)
{
    if(pMat == NULL)return false;
    if(pMat->ClassID() != Class_ID(DMTL_CLASS_ID, 0) && pMat->ClassID() != Class_ID(MULTI_CLASS_ID, 0) )
        return false;
    for(int tex_layer = 0 ; tex_layer < tex_usage_max ; ++tex_layer)
    {
        Texmap* pTexMap = pMat->GetSubTexmap(tex_layer);
        //贴图是不是被激活
        if(pTexMap != NULL && pMat->MapEnabled(tex_layer))
        {
            if(pTexMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
            {
                // get the full filepath
                sMaxTexLayer texLayer;
                std::wstring strFilename;
                BitmapTex *pBMPMap = (BitmapTex *)pTexMap;
                strFilename = getTextureName( ToWCHAR(pBMPMap->GetMapName()) );
                wcsncpy(texLayer.m_MTL.m_TexName,strFilename.c_str(),64);
                texLayer.m_MTL.m_Usage = (eTexLayerUsage)tex_layer;

                //先将其设置为
                int ch = pBMPMap->GetMapChannel();
                texLayer.m_MTL.m_UVChannel = __find_used_map_channel_index(ch);

                pTexMap->GetUVTransform(texLayer.m_UVTransMat);                            
                mat.m_TexLayers.push_back(texLayer);
            }
        }
    }
    mat.m_nTexLayer = (int)mat.m_TexLayers.size();
    if(mat.m_matID == -1 && mat.m_nTexLayer == 0)
        return false;
    return true;
}
int CMaxMesh::__find_all_used_map_channel()
{
	m_UsedMapChannels.clear();
	int nMapVert = 0;
	for(int i = 1 ;i < 100 ; i++)
	{
		int nimvert = m_pMesh->getNumMapVerts(i);
		if(nimvert != 0)
		{
			/*
			if(nMapVert == 0)
				nMapVert = nimvert;
			if(nMapVert != nimvert)
			{
			    XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error}[ %s ] 使用的MapChannel有不同的顶点个数\n" , INodeName(pNode).c_str() );
				return false;
			}
			*/
			m_UsedMapChannels.push_back(i);
		}
	}
	return (int)m_UsedMapChannels.size();
}

/***
找到一个Channel在需要导出的Channel列表中的索引
比如: 一共有使用了　7, 9 .13这三个Channel.
他们在m_UsedMapChannels中的顺序是　9 .13 . 7
那么13的channel 对应的index就是　1.
***/
int  CMaxMesh::__find_used_map_channel_index(int ch)
{
    //检查m_UsedMapChannels中是不是有这个channel了．
    for(size_t i = 0 ; i < m_UsedMapChannels.size() ; ++i)
    {
        int used_ch = m_UsedMapChannels[i];
        if(used_ch == ch)
            return (int)i;
    }
    //没有．则加入一个新的。
    //m_UsedMapChannels.push_back(ch);
	assert(0);
    return -1;//(int)(m_UsedMapChannels.size() - 1);
}

/***
把一个材质以及所有的子材质的信息提取出来．
***/
void set_clor(sColor_t& cl, Color max_cl)
{
    cl.r = max_cl.r;
    cl.g = max_cl.g;
    cl.b = max_cl.b;
    cl.a = 255;
}
bool CMaxMesh::_BuildMaterials(Mtl *pMtl)
{
	
    if(pMtl == NULL)
    {
        return false;
    }

	m_MeshData.m_MatName = ToWCHAR( pMtl->GetName() );
    // 检查是不是一个有效的常规的Material
    if(pMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
    {
        sMaxMatrial_t mat;
        // 添加一个Material
        m_vStdMtls.push_back((StdMat*)pMtl);
        if(_buildMaterialInfo(mat,(StdMat*)pMtl) )
        {
            StdMat* pStdMtl = (StdMat*)pMtl;
            Color amb = pStdMtl->GetAmbient(0);
            Color dif = pStdMtl->GetDiffuse(0);
            Color spe = pStdMtl->GetSpecular(0);

            set_clor(mat.m_BaseMat.m_Ambient , amb);
            set_clor(mat.m_BaseMat.m_Diffuse , dif);
            set_clor(mat.m_BaseMat.m_Speculer , spe);

            mat.m_BaseMat.m_fOpactiy = pStdMtl->GetOpacity(0);
            mat.m_BaseMat.m_fShiness = pStdMtl->GetShininess(0);
            wcsncpy(mat.m_BaseMat.m_MatName, ToWCHAR(pStdMtl->GetFullName()).c_str() ,64);
            mat.m_BaseMat.m_MatName[63] = 0;
            mat.m_BaseMat.m_bTwoSide = pStdMtl->GetTwoSided() != FALSE;
            
            mat.m_Mtl = pMtl;
            mat.m_matID =(int) m_vStdMtls.size()-1;
            m_MeshData.m_Materials.push_back(mat);
        }

    }

    // 处理所有的子ID;
    int subId;
    for(subId = 0; subId < pMtl->NumSubMtls(); subId++)
    {
        _BuildMaterials(pMtl->GetSubMtl(subId));
    }

    return true;

}

/***
加入一个新的VertexIndex(通过这个VertexIndex里的索引，去Mesh里得到实际数据)
返回这个VertexIndex在m_VertexIndexs列表里的索引
这样可以避免重复顶点
***/
int      CMaxMesh::__insert_vertex_index(sVeretIndex_t& new_vi , VertexIndexSet_t& viset)
{
	size_t nVertex = m_VertexIndexs.size();
	VertexIndexSet_t::iterator pos = viset.find(new_vi);
	if(pos == viset.end() )
	{
		m_VertexIndexs.push_back(new_vi);
		viset[new_vi] = nVertex;
		return nVertex;
	}
	else
	{
		return pos->second;
	}

    //查找这个顶点是不是已经在m_VertexIndexs列表里了。
    //size_t nVertex = m_VertexIndexs.size();
    for(size_t i = 0 ; i < nVertex ; ++i )
    {
        sVeretIndex_t& vi = m_VertexIndexs[i];
        if(new_vi ==  vi)
            return (int)i;
    }
    //不在，则加入到列表的末尾
    m_VertexIndexs.push_back(new_vi);
    return (int)nVertex;
}

/**
得到一个面的材质
**/
int   CMaxMesh::__find_face_mat(Face* face)
{
    return __find_mat_by_matid(face->getMatID()%(int)m_vStdMtls.size());
}

/**
加入一个Face．返回这个Face中三个顶点在VertexIndex的索引.
即建立VertexBuffer和IndexBuffer之间的对应关系，
(VertexBuffer通过m_VertexIndexs列表并在Mesh中取得实际数据来建立)
add_face返回的sFace_t用来构造IndexBuffer

输入：MaxFace_t, 包含了这个面在MAX_Mesh里的各种数据的索引.
输出：sFace_t .  包含了这个面在IndexBuffer里的面的三个index
**/
sFace_t  CMaxMesh::__add_face(sMaxFace_t& face)
{
    sFace_t retFace;
    int matIndex = __find_mat_by_matid(face.matID);//__find_mat_by_matid(face->matID%m_vStdMtls.size());
    sMaxMatrial_t& mat =  m_MeshData.m_Materials[matIndex];
    sVeretIndex_t Vi;
    Vi.m_nUV = (int)m_UsedMapChannels.size();
    for(int i = 0 ; i < 3 ; ++i)
    {
        //得到所有Channel的Index
        for(int iMapCh = 0 ; iMapCh < Vi.m_nUV ; ++iMapCh)
        {
            int mapCh = m_UsedMapChannels[iMapCh];
            TVFace* pFace = m_pMesh->mapFaces(mapCh);
            Vi.m_UVs[iMapCh] = pFace[face.faceIndex].t[i];
        }
		TVFace* pAlphaFace = m_pMesh->mapFaces(-VDATA_ALPHA);
		if(pAlphaFace)
		{
			Vi.m_iAlpha = pAlphaFace[face.faceIndex].t[i];
		}
		else
		{
			Vi.m_iAlpha = -1;
		}
        //其他的Index
        Vi.m_iDiffuse = face.color[i];
        Vi.m_iPos     = face.vert[i];
        Vi.m_iNormal  = face.normal[i];

        retFace.vert[i] = __insert_vertex_index(Vi , m_VertexIndexSet );
    }

    //交换位置
    if(m_bMirrored)
    {
        unsigned short temp = retFace.vert[0];
        retFace.vert[0] = retFace.vert[1];
        retFace.vert[1] = temp;

    }
    return retFace;
}


bool CMaxMesh::_build_face_info()
{
    assert(m_pMesh);
    int nMat = (int)m_MeshData.m_Materials.size();
    int faceSum = m_pMesh->getNumFaces();

	m_VertexIndexSet.clear(); 
    typedef std::vector<sMaxFace_t> FaceList;
    FaceList* pFaceList = new FaceList[nMat];
    if(m_pMesh->faces == NULL)
    {
        XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error}[ %s ] faces指针为空 \n" , INodeName(m_pNode).c_str() );
        return false;
    }
    if(m_pMesh->tvFace == NULL)
    {
        XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error}[ %s ] tvfaces指针为空 \n" , INodeName(m_pNode).c_str() );
        return false;
    }

    //构造一个分组，就是把面按材质分组
    for(int iFace = 0 ; iFace < faceSum ; iFace ++)
    {

        Face&   vface =   m_pMesh->faces[iFace];
        TVFace& tface =   m_pMesh->tvFace[iFace];
        NFace&  nrlface = m_NormalFaces[iFace];

        sMaxFace_t max_face;


        max_face.normal[0] = nrlface.normal[0];
        max_face.normal[1] = nrlface.normal[1];
        max_face.normal[2] = nrlface.normal[2];

        if(m_pMesh->vcFace)
        {
            TVFace& cface = m_pMesh->vcFace[iFace];
            max_face.color[0] = cface.t[0];
            max_face.color[1] = cface.t[1];
            max_face.color[2] = cface.t[2];
        }
        else
        {
            max_face.color[0] = 0;
            max_face.color[1] = 0;
            max_face.color[2] = 0;
        }


        max_face.uv[0] = tface.t[0];
        max_face.uv[1] = tface.t[1];
        max_face.uv[2] = tface.t[2];

        max_face.vert[0] = vface.v[0];
        max_face.vert[1] = vface.v[1];
        max_face.vert[2] = vface.v[2];

        max_face.faceIndex = iFace;
        max_face.matID = vface.getMatID()%(int)m_vStdMtls.size();

        //找到这个Face用的是第几个mat
        int matIndex = __find_face_mat(&vface);//__find_mat_by_matid(face->matID%m_vStdMtls.size());
        //把面加入到对应的材质分组里
        pFaceList[matIndex].push_back(max_face);
    }


    //遍历所有的材质．
    for(int i = 0 ; i < nMat; ++i)
    {
        sMaxMatrial_t& mat = m_MeshData.m_Materials[i];
        FaceList& faceList = pFaceList[i];
        //取出这个材质的面的信息，
        //并构造IndexBuffer
        int nface = (int)faceList.size();
        IndexBuffer_t pIndexBuffer = new sFace_t[nface];
        for(int j = 0 ; j <  nface; ++j)
        {
            sMaxFace_t&  face = faceList[j];
            pIndexBuffer[j] = __add_face(face);
        }
        //第i个SubMesh和第i个Material是对应的
        //这里我们填写这个Material对应的SubMesh的信息
        m_MeshData.m_pMeshDesc->m_SubMesh[i].m_nFace = nface;
        m_MeshData.m_pMeshDesc->m_SubMesh[i].m_nMatIndex = i;//第几个Material
        //把构造好的IndexBuffer加入到IndexBuffer列表中
        //m_IndexBuffers中的第i个IndexBuffer和第i个SubMesh或者第i个Material都是相对应的．
        m_MeshData.m_IndexBuffers.push_back(pIndexBuffer);

        //构造第i个Material的一些附加信息：Face的个数，TextureLayer的个数
        m_MeshData.m_Materials[i].m_nTexLayer = (int)m_MeshData.m_Materials[i].m_TexLayers.size();
    }
    m_MeshData.m_pMeshDesc->m_nFace   = faceSum;
    m_MeshData.m_pMeshDesc->m_nVertex = (int)m_VertexIndexs.size();
    delete [] pFaceList;
	m_VertexIndexSet.clear(); 
    return true;
}
bool CMaxMesh::__find_channel_tm(int ch_index,Matrix3& chTM)
{
    for(size_t iMat = 0 ; iMat < m_MeshData.m_Materials.size() ; ++iMat)
    {
        sMaxMatrial_t& mat = m_MeshData.m_Materials[iMat];
        for(size_t iTL = 0 ; iTL < mat.m_TexLayers.size(); ++iTL)
        {
            sMaxTexLayer& MTL = mat.m_TexLayers[iTL];
            if(MTL.m_MTL.m_UVChannel == ch_index)
            {
                chTM =  MTL.m_UVTransMat;
                return true;
            }
        }
    }
    return false;
}
/**
检测使用到的ChannelMap是不是都被Enable了
(也就是ChannelMap中是不是有顶点)
**/
bool CMaxMesh::__test_used_mapchannel_active()
{
    return true;
}

bool CMaxMesh::__build_mesh_desc()
{

    //建立Mesh的
    int nVert = m_pMesh->getNumVerts();
    if(nVert <= 0)
    {
		XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} Node Mesh: [%s]  顶点个数小于0 \n" , INodeName(m_pNode).c_str() );
        return false;
    }

    size_t usedChNumber = m_UsedMapChannels.size();
    for(size_t i = 0 ; i < usedChNumber ; ++i)
    {
        int nMapVert = m_pMesh->getNumMapVerts(m_UsedMapChannels[i]);
        if(nMapVert <=0)
        {
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Error} Mesh: [%s]  channel map [index=%d]  nMapVert < 0 \n" , INodeName(m_pNode).c_str() , m_UsedMapChannels[i]);
            return false;
        }
    }

    if(m_MeshData.m_pMeshDesc)
    {
        ::free((void*)m_MeshData.m_pMeshDesc);
    }
    int nMat = (int)m_MeshData.m_Materials.size();
    int size_md = sizeof(sMeshDesc_t) + (nMat - 1) * sizeof(sSubMeshDesc_t);
    //sMeshDesc_t是个变长结构体.
    m_MeshData.m_pMeshDesc = (sMeshDesc_t*)::malloc(size_md);
    wcsncpy(m_MeshData.m_pMeshDesc->m_Name,INodeName(m_pNode).c_str() ,32);
    m_MeshData.m_pMeshDesc->m_nSubMesh = (int)m_MeshData.m_Materials.size();
    m_MeshData.m_pMeshDesc->m_nUVMaps = (int)usedChNumber;
    m_MeshData.m_pMeshDesc->m_FurInfo = m_MeshDataFlag.m_FurInfo;
    return true;
}



bool CMaxMesh::__cacl_tbn(sFace_t& face,bool isSkin)
{
    Point3  normal[3];
    Point3  Tangent; 
    Point3  p[3];

    assign(normal[0],m_MeshData.m_VertexData.m_Normals[face.vert[0]]);
    assign(normal[1],m_MeshData.m_VertexData.m_Normals[face.vert[1]]);
    assign(normal[2],m_MeshData.m_VertexData.m_Normals[face.vert[2]]);

    if(isSkin == false)
    {
        assign(p[0],m_MeshData.m_VertexData.m_Positons[face.vert[0]]);
        assign(p[1],m_MeshData.m_VertexData.m_Positons[face.vert[1]]);
        assign(p[2],m_MeshData.m_VertexData.m_Positons[face.vert[2]]);
    }
    else
    {
        assign(p[0],m_MeshData.m_VertexData.m_VertexWeights[face.vert[0]].m_InitPos);
        assign(p[1],m_MeshData.m_VertexData.m_VertexWeights[face.vert[1]].m_InitPos);
        assign(p[2],m_MeshData.m_VertexData.m_VertexWeights[face.vert[2]].m_InitPos);
    }


    sUVCoord_t uv[3];
    uv[0] = m_MeshData.m_VertexData.m_UVChannels[0][face.vert[0]];
    uv[1] = m_MeshData.m_VertexData.m_UVChannels[0][face.vert[1]];
    uv[2] = m_MeshData.m_VertexData.m_UVChannels[0][face.vert[2]];

    Point3  e1 = p[1] - p[0];
    Point3  e2 = p[2] - p[0];
    sUVCoord_t u1 = { uv[1].u - uv[0].u , uv[1].v - uv[0].v};
    sUVCoord_t u2 = { uv[2].u - uv[0].u , uv[2].v - uv[0].v};

    float det =  ( u1.u * u2.v - u2.u * u1.v);
    if(det == 0.0f)
    {
        Tangent = e1;
    }
    else
    {
        Tangent = u2.v * e1 - u1.v * e2;
    }

    //从Normal 和 Tangent里重新计算出Tangent,因为面的Tangent和顶点的Normal可能不垂直
    Point3 final_tangent;
    for(int i = 0 ;i < 3 ; ++i)
    {
        Point3 binormal  = CrossProd(normal[i],Tangent);
        final_tangent = CrossProd(binormal,normal[i]);
        final_tangent.Normalize();
        m_MeshData.m_VertexData.m_Tangents[face.vert[i]].x += final_tangent.x;
        m_MeshData.m_VertexData.m_Tangents[face.vert[i]].y += final_tangent.y;
        m_MeshData.m_VertexData.m_Tangents[face.vert[i]].z += final_tangent.z;

    }
    return true;
}

void CMaxMesh::__build_normals(bool isSkin)
{
    for(size_t i = 0 ; i < m_MeshData.m_IndexBuffers.size() ; ++i )
    {
        sFace_t* faces = m_MeshData.m_IndexBuffers[i];
        for(int iFace = 0 ; iFace < m_MeshData.m_pMeshDesc->m_SubMesh[i].m_nFace ; ++iFace)
        {
            __cacl_tbn(faces[iFace],isSkin);
        }
    }

    //计算好以后，把tangent重新单位化
    for(size_t i = 0 ; i < m_MeshData.m_VertexData.m_Tangents.size() ; ++i)
    {
        m_MeshData.m_VertexData.m_Tangents[i].normalize();
    }
}

//找到这个Mesh需要用到的所有骨骼
void CMaxMesh::find_all_used_bone(CSkeletonExporter* pSkeleton)
{
	bool ret =  false;
	Modifier* pModify = CMaxEnv::FindPhysiqueModifier(m_pNode);
	if( pModify != NULL )
	{
		IPhysiqueExport *phyExp = (IPhysiqueExport *)pModify->GetInterface(I_PHYINTERFACE);
		if(phyExp == NULL)
		{
			ret =  false;
			//goto error_line;
		}
		IPhyContextExport *mcExport = (IPhyContextExport *)phyExp->GetContextInterface(m_pNode);
		if(mcExport == NULL)
		{
			ret =  false;
			//goto error_line;
		}
		ret = __find_all_bone_phy(mcExport,pSkeleton);
		phyExp->ReleaseContextInterface(mcExport);
		pModify->ReleaseInterface(I_PHYINTERFACE, phyExp);
		return ;
	}
	pModify = CMaxEnv::FindSkinModifier(m_pNode);
	if(pModify != NULL)
	{
		ISkin *pSkin = (ISkin*)pModify->GetInterface(I_SKIN);
		if(pSkin == NULL)
		{
			ret =  false;
			//goto error_line;
		}
		ISkinContextData* pSkinContext = pSkin->GetContextInterface(m_pNode);
		if(pSkinContext == NULL)
		{
			ret =  false;
			//goto error_line;
		}
		ret = __find_all_bone_skin(pSkin,pSkinContext,pSkeleton);
		pModify->ReleaseInterface(I_SKIN, pSkin);
		return ;
	}
	INode* pParent = m_pNode->GetParentNode();
	if(pParent && pParent != CMaxEnv::singleton().m_pInterace->GetRootNode())
		 pSkeleton->push_bone(m_pNode->GetParentNode());
	else
		 pSkeleton->push_bone(m_pNode);
}
//给一个Physique修改器，把所有的这个修改器绑定的骨骼，全部都提取到pSkeleton里。
bool CMaxMesh::__find_all_bone_phy(IPhyContextExport *mcExport,CSkeletonExporter* pSkeleton)
{
	mcExport->ConvertToRigid( TRUE );
	int nNumVerts = m_pMesh->numVerts;

	XEVOL_LOG(eXL_DEBUG_HIGH , L"  ------>Mesh: [%s] 开始从Physique修改器构造Vertex Weights<--------\n" , INodeName(m_pNode).c_str() );
	if( mcExport->GetNumberVertices()!=nNumVerts )
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s] 得到的顶点数目和IPhyContextExport得到的顶点数目不相等\n" , INodeName(m_pNode).c_str() );
		return false;
	}
	for( int iVert =0 ; iVert<nNumVerts ; iVert++ )
	{
		IPhyVertexExport *vtxExport = mcExport->GetVertexInterface(iVert);
		int type = vtxExport->GetVertexType();
		if( (type & RIGID_NON_BLENDED_TYPE) ||
			(type & RIGID_BLENDED_TYPE)      )
		{}
		else if( (type & DEFORMABLE_NON_BLENDED_TYPE) ||
			(type & DEFORMABLE_BLENDED_TYPE) )
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s] DEFORMABLE_VERTEX_TYPE Not supported iVertex = %d\n" , INodeName(m_pNode).c_str() , iVert);
		}
		else if( (type & FLOATING_TYPE))
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s] FLOATING_VERTEX_TYPE Not supported iVertex = %d\n" , INodeName(m_pNode).c_str() , iVert);
		}
		if( vtxExport )
		{
			Point3 vOffset;
			Point3 ptUse;
			if( vtxExport->GetVertexType() & BLENDED_TYPE )
			{
				IPhyBlendedRigidVertex *vtxBlend = (IPhyBlendedRigidVertex *)vtxExport;
				vector<Point3> OffsetArray;
				for( int j=0 ; j<vtxBlend->GetNumberNodes() ; j++ )
				{
					INode* pBone = vtxBlend->GetNode( j );
					if(pBone == NULL)
					{
						XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s] Mesh找不到骨骼\n" , INodeName(m_pNode).c_str() );
						mcExport->ReleaseVertexInterface(vtxExport);
						return false;
					}
					int iBone = pSkeleton->push_bone(pBone);
					if( iBone<0 )
					{
						XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s] Skeleton找不到指定骨骼 Bone=[]\n" , INodeName(m_pNode).c_str() , INodeName(pBone).c_str() );
						mcExport->ReleaseVertexInterface(vtxExport);
						return false;
					}
				}
			}
			else
			{
				IPhyRigidVertex *vtxNoBlend = (IPhyRigidVertex *)vtxExport;
				INode *pBone = vtxNoBlend->GetNode();
				vOffset = vtxNoBlend->GetOffsetVector();
				int iBone = pSkeleton->push_bone( pBone );
				if( iBone<0 )
				{
					XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s] 找不到骨骼 Bone=[]\n" , INodeName(m_pNode).c_str() , INodeName(pBone).c_str() );
					mcExport->ReleaseVertexInterface(vtxExport);
					return false;
				}
			}
			//Physique里的数据都取出来了
			mcExport->ReleaseVertexInterface(vtxExport);
		}
	}
	XEVOL_LOG(eXL_DEBUG_HIGH , L"   ------>Mesh: [%s] 的Vertex Weight已经成功从Physique里构造完毕<--------\n" , INodeName(m_pNode).c_str()  );
	return true;
}

//给一个Skin修改器，把所有的这个修改器绑定的骨骼，全部都提取到pSkeleton里。
bool CMaxMesh::__find_all_bone_skin(ISkin* pSkin,ISkinContextData* pSkinContext , CSkeletonExporter* pSkeleton)
{
	int nNumVerts = m_pMesh->numVerts;
	XEVOL_LOG(eXL_DEBUG_HIGH , L"   ------>Mesh: [%s] 开始从Skin修改器构造Vertex Weights<--------\n" , INodeName(m_pNode).c_str()  );
    if(pSkinContext->GetNumPoints() !=nNumVerts )
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s] 得到的顶点数目和ISkin得到的顶点数目不相等\n" , INodeName(m_pNode).c_str() );
		return false;
	}

	for(int ivert = 0 ; ivert  < nNumVerts ; ++ivert)
	{
		int nBones = pSkinContext->GetNumAssignedBones(ivert);
		if(nBones <= 0 )
		{

			wchar_t buf[128];
			swprintf(buf, L"谁做的模型啊？竟然有个点没绑骨骼 iVertex = %d , MeshNodeName = %s 修改器为Skin", ivert,INodeName(m_pNode));
			MessageBoxW(GetActiveWindow(),buf,L"错误",MB_OK);
			XEVOL_LOG(eXL_DEBUG_HIGH , "   {error}  [%s] \n", buf );
		}
		for(int jBone = 0; jBone < nBones; jBone ++)
		{

			INode* pBone = pSkin->GetBone(pSkinContext->GetAssignedBone(ivert,jBone));
			int iBoneIndex = pSkeleton->push_bone(pBone);
			if( iBoneIndex<0 )
			{
				XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s] 找不到指定骨骼 Bone=[%s]\n" , INodeName(m_pNode).c_str() , INodeName(pBone).c_str());
				return false;
			}
		}
	}

	XEVOL_LOG(eXL_DEBUG_HIGH , L"   ------>Mesh:[%s] 的Vertex Weight已经成功从ISkin里构造完毕<--------\n" , INodeName(m_pNode).c_str() );
	return true;
}

bool CMaxMesh::__extract_vertex_weight_fromphysique(IPhyContextExport *mcExport,CSkeletonExporter* pSkeleton)
{
    mcExport->ConvertToRigid( TRUE );
    int nNumVerts = m_pMesh->numVerts;


	XEVOL_LOG(eXL_DEBUG_HIGH , L"   ------>Mesh:[%s] 开始从Physique修改器构造Vertex Weights<--------\n" , INodeName(m_pNode).c_str() );
    if( mcExport->GetNumberVertices()!=nNumVerts )
    {
		XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s] 得到的顶点数目和IPhyContextExport得到的顶点数目不相等\n" , INodeName(m_pNode).c_str()  );
        return false;
    }

    // 得到将导出顶点的数据
    sWeightedVertex_t exportVertex;
    m_VertexWeights.resize(nNumVerts);
    for( int iVert =0 ; iVert<nNumVerts ; iVert++ )
    {
        IPhyVertexExport *vtxExport = mcExport->GetVertexInterface(iVert);
        int type = vtxExport->GetVertexType();
        if( (type & RIGID_NON_BLENDED_TYPE) || (type & RIGID_BLENDED_TYPE)      )
        {}
        else if( (type & DEFORMABLE_NON_BLENDED_TYPE) || (type & DEFORMABLE_BLENDED_TYPE) )
        {
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s] DEFORMABLE_VERTEX_TYP iVertex = %d\n" , INodeName(m_pNode).c_str() , iVert );
        }
        else if( (type & FLOATING_TYPE))
        {
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s] FLOATING_VERTEX_TYPE iVertex = %d\n" , INodeName(m_pNode).c_str() , iVert );
        }

        if( vtxExport )
        {
            sTempWeight_t formerVertex;
			sTempWeight_t zeroWeightVertex;
            Point3 vOffset;
            Point3 ptUse;
            if( vtxExport->GetVertexType() & BLENDED_TYPE )
            {
                IPhyBlendedRigidVertex *vtxBlend = (IPhyBlendedRigidVertex *)vtxExport;
                vector<Point3> OffsetArray;
                for( int j=0 ; j<vtxBlend->GetNumberNodes() ; j++ )
                {
                    INode* pBone = vtxBlend->GetNode( j );
                    if(pBone == NULL)
                    {
						XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s]  Mesh找不到骨骼\n" , INodeName(m_pNode).c_str()  );
                        mcExport->ReleaseVertexInterface(vtxExport);
                        return false;
                    }
                    int iBone = pSkeleton->push_bone(pBone);
                    if( iBone<0 )
                    {
						XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s]  Skeleton找不到指定骨骼 Bone=[%s]\n" , INodeName(m_pNode).c_str() , INodeName(pBone).c_str());
                        mcExport->ReleaseVertexInterface(vtxExport);
                        return false;
                    }

					//用到了这块骨骼
					xMeshBone_t usedBone;
					usedBone.m_BoneIndex = iBone;
					usedBone.m_pBone     = pBone;
					m_MeshData.addUsedBone(usedBone);

                    sMaxBoneNode_t& boneNode = pSkeleton->m_MaxBones[iBone];

                    vOffset = vtxBlend->GetOffsetVector(j);
                    Matrix3& initMT = boneNode.m_SkinInitMT;

                    ptUse = vOffset*initMT;
                    OffsetArray.push_back( ptUse );
                    if( 0.0f == vtxBlend->GetWeight( j ))
					{
						sVertWeight_t boneWeight;
						boneWeight.m_BoneID = boneNode.m_Bone.m_BoneID;
						boneWeight.m_BoneIndex = iBone;
						boneWeight.m_fWeight = 0.0f;
						zeroWeightVertex.m_Weights.push_back(boneWeight);
						continue;
					}
                    // 如果有重复的骨头，则累加权重
                    bool bRepeat = false;
                    for( size_t indexB = 0; indexB < formerVertex.m_Weights.size(); ++indexB )
                    {
                        if( formerVertex.m_Weights[indexB].m_BoneIndex == (WORD)iBone )
                        {
                            formerVertex.m_Weights[indexB].m_fWeight += vtxBlend->GetWeight( j );
                            bRepeat = true;
                            break;
                        }
                    }
                    // 如果没有重复的骨头
                    if( bRepeat == false )
                    {
                        sVertWeight_t boneWeight;
                        boneWeight.m_BoneID = boneNode.m_Bone.m_BoneID;
                        boneWeight.m_BoneIndex = iBone;
                        boneWeight.m_fWeight = vtxBlend->GetWeight( j );
                        formerVertex.m_Weights.push_back(boneWeight);
                    }

                }

                
				//求取平均顶点位置
                Point3 initPos;
                initPos.Set(0.f, 0.f, 0.f );
                for( size_t j=0 ; j<OffsetArray.size() ; j++ )
                {
                    initPos += OffsetArray[j];
                }
                Point3 avgPos = initPos/(float)(OffsetArray.size());

				//得到初始位置
				exportVertex.m_InitPos = conv_type<sVector_t,Point3>(avgPos);


				//如果权重和为0
				if(zeroWeightVertex.m_Weights.size() >0 && formerVertex.m_Weights.size() == 0)
				{
					XEVOL_LOG(eXL_DEBUG_HIGH , L"   {waring} Mesh: [%s]  有顶点的weight 和为 0 (iVert = %d) 强制平均weight\n" , INodeName(m_pNode).c_str() , iVert );
                    exportVertex.m_nEffBone = 1;
					exportVertex.m_BoneWeights[0].m_BoneID = zeroWeightVertex.m_Weights[0].m_BoneID;
					exportVertex.m_BoneWeights[0].m_BoneIndex = zeroWeightVertex.m_Weights[0].m_BoneIndex;
					exportVertex.m_BoneWeights[0].m_fWeight = zeroWeightVertex.m_Weights[0].m_fWeight;
					continue;
				}

				// sort
				formerVertex._sort();
                //去掉超过4块骨骼的影响
				for(int i = MAX_INFLUENCE_BONE ; i < (int)formerVertex.m_Weights.size() ; i ++)
				{
                      formerVertex.m_Weights[i].m_fWeight = 0.0f;
				}

				//规格化权重
				{
					float fTWeight = 0.0f;
					for(size_t iW = 0 ; iW < formerVertex.m_Weights.size(); ++iW)
					{
						fTWeight += formerVertex.m_Weights[iW].m_fWeight;
					}
					if(fTWeight != 0)
					{
						float scale = 1.0f/fTWeight;
						for(size_t iW = 0 ; iW < formerVertex.m_Weights.size(); ++iW)
						{
							formerVertex.m_Weights[iW].m_fWeight *= scale;
						}
					}
				}

                // 将formerVertex 的值赋给 exportVertex
                if( formerVertex.m_Weights.size() > MAX_INFLUENCE_BONE )
                {
                    exportVertex.m_nEffBone = MAX_INFLUENCE_BONE;
					XEVOL_LOG(eXL_DEBUG_HIGH , L"   {waring} Mesh: [%s]  有顶点受到超过4块骨头影响，忽略多余的骨骼 (iVert = %d) \n" , INodeName(m_pNode).c_str() , iVert );
                }
                else
                {
                    exportVertex.m_nEffBone = (int)formerVertex.m_Weights.size();
                }

                

                //防止顶点权重为0的情况
                float fWeigtAll = 0.0f;
                for( int jBone = 0; jBone < exportVertex.m_nEffBone; ++jBone )
                {
                    exportVertex.m_BoneWeights[jBone].m_BoneID    = formerVertex.m_Weights[jBone].m_BoneID;
                    exportVertex.m_BoneWeights[jBone].m_BoneIndex = formerVertex.m_Weights[jBone].m_BoneIndex;
                    exportVertex.m_BoneWeights[jBone].m_fWeight   = formerVertex.m_Weights[jBone].m_fWeight;
                    fWeigtAll += formerVertex.m_Weights[jBone].m_fWeight;
                }
                if( fabs(fWeigtAll-1.f)>0.0001f )
                {
					XEVOL_LOG(eXL_DEBUG_HIGH , L"   {waring} Mesh: [%s]  有顶点的weight 和为 0 (iVert = %d) 强制平均weight :" , INodeName(m_pNode).c_str() , iVert );
                    float weight = 1.0f/float(exportVertex.m_nEffBone);
                    for( int jBone = 0; jBone < exportVertex.m_nEffBone; ++jBone )
                    {
						XEVOL_LOG(eXL_DEBUG_HIGH , L"<Wight iBone=%d, weight=%f>",jBone ,exportVertex.m_BoneWeights[jBone].m_fWeight);
                        exportVertex.m_BoneWeights[jBone].m_fWeight   = weight;
                    }
                    XEVOL_LOG(eXL_DEBUG_HIGH , L"\n");
                    //return false ;
                }
            }
            else
            {
                IPhyRigidVertex *vtxNoBlend = (IPhyRigidVertex *)vtxExport;
                INode *pBone = vtxNoBlend->GetNode();
                vOffset = vtxNoBlend->GetOffsetVector();
                exportVertex.m_nEffBone = 1;
                int iBone = pSkeleton->push_bone( pBone );
                if( iBone<0 )
                {
					XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s]  找不到骨骼 Bone=[%s]\n" , INodeName(m_pNode).c_str(), INodeName(pBone).c_str() );
                    mcExport->ReleaseVertexInterface(vtxExport);
                    return false;
                }

				//用到了这块骨骼
				xMeshBone_t usedBone;
				usedBone.m_BoneIndex = iBone;
				usedBone.m_pBone     = pBone;
				m_MeshData.addUsedBone(usedBone);

                sMaxBoneNode_t& boneNode = pSkeleton->m_MaxBones[iBone];
                Matrix3& initMT = boneNode.m_SkinInitMT;

                ptUse = vOffset*initMT;
                exportVertex.m_nEffBone = 1;
                exportVertex.m_InitPos = conv_type<sVector_t,Point3>(ptUse) ;
                exportVertex.m_BoneWeights[0].m_BoneIndex = iBone;
                exportVertex.m_BoneWeights[0].m_BoneID = boneNode.m_Bone.m_BoneID;
                exportVertex.m_BoneWeights[0].m_fWeight = 1.f;
            }

            //Physique里的数据都取出来了
            mcExport->ReleaseVertexInterface(vtxExport);

            //算出蒙皮的初始位置
            if(CMaxEnv::singleton().m_bCalcInitPosMySelf && CMaxEnv::singleton().m_bUseBeforeSkeletonPose )
            {
				//获取第0帧的数据. 从第0帧算回到手平举着的时候的模型初始位置（第i帧）。
				//假设，第0帧的时候：位置是v0, 骨骼变换矩阵是TM(0,k)
				//手平举着的时候,    位置是vi, 骨骼变换矩阵式TM(i,k). ---这个就是Bone InitTM
				//四个权重对应位置为 W(k);

				//在第0帧的时候，顶点相对四个骨骼的的位移应该为 v0 * inv[TM(0,k)]. 
				//（这个值在Physique里取出来的就是vtxBlend->GetOffsetVector(j), 
				//所以。其实我们并不需要这个Offset，从第0帧，加上i帧的Bone InitTM就可以算出这个offset）
				//那么从第0帧，算到第i帧（初始帧）, 根据骨骼动画公式。
				//vi = SIGMA< v0 * inv[TM(0,k)] * TM(i,k) > (k = 0, 4).
                Point3 v0 = m_pMesh->getVert(iVert) * m_pNode->GetObjTMAfterWSM(0);
                Point3 vInit;
                vInit.Set(0.0f,0.0f,0.0f);
                for(int i = 0 ; i < exportVertex.m_nEffBone ; i++)
                {
                    Matrix3 matMeshInit;
                    int boneIndex = exportVertex.m_BoneWeights[i].m_BoneIndex ;
                    float fWeight = exportVertex.m_BoneWeights[i].m_fWeight;
                    sMaxBoneNode_t& boneNode = pSkeleton->m_MaxBones[boneIndex];
                    assert(exportVertex.m_BoneWeights[i].m_BoneID == boneNode.m_Bone.m_BoneID);
                    matMeshInit = Inverse(boneNode.m_InitNodeTM0) * boneNode.m_SkinInitMT;
                    Point3 pt = v0 * matMeshInit;
                    vInit.x = vInit.x + fWeight * pt.x;
                    vInit.y = vInit.y + fWeight * pt.y;
                    vInit.z = vInit.z + fWeight * pt.z;
                }
                exportVertex.m_InitPos.x = vInit.x;
                exportVertex.m_InitPos.y = vInit.y;
                exportVertex.m_InitPos.z = vInit.z;
            }

            m_VertexWeights[iVert] = exportVertex;
        }
    }

	
	XEVOL_LOG(eXL_DEBUG_HIGH , L"   ------>Mesh:[%s] 的Vertex Weight已经成功从Physique里构造完毕<--------\n" , INodeName(m_pNode).c_str()  );
    return true;

}

bool CMaxMesh::__extract_vertex_weight_fromskin(ISkin* pSkin,ISkinContextData* pSkinContext ,CSkeletonExporter* pSkeleton)
{
    int nNumVerts = m_pMesh->numVerts;

	XEVOL_LOG(eXL_DEBUG_HIGH , L"   ------>Mesh:[%s] 开始从Skin修改器构造Vertex Weights<--------\n" , INodeName(m_pNode).c_str()  );


    
    if(pSkinContext->GetNumPoints() !=nNumVerts )
    {
		XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s]  得到的顶点数目和ISkin得到的顶点数目不相等\n" , INodeName(m_pNode).c_str()  );
        return false;
    }


    // 得到将导出顶点的数据
    sWeightedVertex_t exportVertex;
    m_VertexWeights.clear();
    m_VertexWeights.resize(nNumVerts);
    for(int ivert = 0 ; ivert  < nNumVerts ; ++ivert)
    {
        sTempWeight_t formerVertex;
        int nBones = pSkinContext->GetNumAssignedBones(ivert);
        if(nBones <= 0 )
        {
			wchar_t buf[128]={0};
            swprintf(buf, L"谁做的模型啊？竟然有个点没绑骨骼 iVertex = %d , MeshNodeName = %s 修改器为Skin", ivert,INodeName(m_pNode));
            MessageBoxW(GetActiveWindow(),buf,L"错误",MB_OK);
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {Warning} [%s] \n" , buf  );
        }
        for(int jBone = 0; jBone < nBones; jBone ++)
        {

            INode* pBone = pSkin->GetBone(pSkinContext->GetAssignedBone(ivert,jBone));
            int iBoneIndex = pSkeleton->push_bone(pBone);
            if( iBoneIndex<0 )
            {
				XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s]   Skeleton找不到指定骨骼 Bone=[%s]\n" , INodeName(m_pNode).c_str() , INodeName(pBone).c_str() );
                return false;
            }

			//用到了这块骨骼
			xMeshBone_t usedBone;
			usedBone.m_BoneIndex = iBoneIndex;
			usedBone.m_pBone     = pBone;
			m_MeshData.addUsedBone(usedBone);

            sMaxBoneNode_t& boneNode = pSkeleton->m_MaxBones[iBoneIndex];
            float fWeight = pSkinContext->GetBoneWeight(ivert,jBone);
            bool bRepeat = false;
            for( size_t indexB = 0; indexB < formerVertex.m_Weights.size(); ++indexB )
            {
                if( formerVertex.m_Weights[indexB].m_BoneIndex == (WORD)iBoneIndex )
                {
                    formerVertex.m_Weights[indexB].m_fWeight += fWeight;
                    bRepeat = true;
                    break;
                }
            }
            // 如果没有重复的骨头
            if( bRepeat == false )
            {
                sVertWeight_t boneWeight;
                boneWeight.m_BoneID = boneNode.m_Bone.m_BoneID;
                boneWeight.m_BoneIndex = iBoneIndex;
                boneWeight.m_fWeight = fWeight;
                //if( fabs( boneWeight.m_fWeight ) > 1.0e-3 )
                {
                    formerVertex.m_Weights.push_back(boneWeight);
                }
            }

        }


		// sort
		formerVertex._sort();
		//去掉超过4块骨骼的影响
		for(int i = MAX_INFLUENCE_BONE ; i < (int)formerVertex.m_Weights.size() ; i ++)
		{
			formerVertex.m_Weights[i].m_fWeight = 0.0f;
		}

		//规格化权重

		float fTWeight = 0.0f;
		for(size_t iW = 0 ; iW < formerVertex.m_Weights.size(); ++iW)
		{
			fTWeight += formerVertex.m_Weights[iW].m_fWeight;
		}
		if(fTWeight != 0)
		{
			float scale = 1.0f/fTWeight;
			for(size_t iW = 0 ; iW < formerVertex.m_Weights.size(); ++iW)
			{
				formerVertex.m_Weights[iW].m_fWeight *= scale;
			}
		}


        // 将formerVertex 的值赋给 exportVertex
        if( formerVertex.m_Weights.size() > MAX_INFLUENCE_BONE )
        {
            exportVertex.m_nEffBone = MAX_INFLUENCE_BONE;
			XEVOL_LOG(eXL_DEBUG_HIGH , L"   {waring} Mesh: [%s]   有顶点受到超过4块骨头影响，忽略多余的骨骼: iVert= [%d]\n" , INodeName(m_pNode).c_str() , ivert );
        }
        else
        {
            exportVertex.m_nEffBone = (int)formerVertex.m_Weights.size();
        }

        //复制数据到导出的顶点中并检测权重之和
        float fWeigtAll = 0.0f;
        for( int jBone = 0; jBone < exportVertex.m_nEffBone; ++jBone )
        {
            exportVertex.m_BoneWeights[jBone].m_BoneID    = formerVertex.m_Weights[jBone].m_BoneID;
            exportVertex.m_BoneWeights[jBone].m_BoneIndex = formerVertex.m_Weights[jBone].m_BoneIndex;
            exportVertex.m_BoneWeights[jBone].m_fWeight   = formerVertex.m_Weights[jBone].m_fWeight;
            fWeigtAll += formerVertex.m_Weights[jBone].m_fWeight;
        }
        //权重之和为0的话，我们就强制平均
        if( fabs(fWeigtAll-1.f)>0.01f )
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"   {waring} Mesh: [%s]  有顶点的weight 和为 0 (iVert = %d) 强制平均weight :" , INodeName(m_pNode).c_str() , ivert );
            float weight = 1.0f/float(exportVertex.m_nEffBone);
            for( int jBone = 0; jBone < exportVertex.m_nEffBone; ++jBone )
            {
                XEVOL_LOG(eXL_DEBUG_HIGH , L"<Wight iBone=%d, weight=%f>",jBone ,exportVertex.m_BoneWeights[jBone].m_fWeight);
                exportVertex.m_BoneWeights[jBone].m_fWeight   = weight;
            }
            XEVOL_LOG(eXL_DEBUG_HIGH , L"\n");
        }

        //算出蒙皮的初始位置-----就是角色手平举着的那个姿势
        {
			//获取第0帧的数据. 从第0帧算回到手平举着的时候的模型初始位置（第i帧）。
			//假设，第0帧的时候：位置是v0, 骨骼变换矩阵是TM(0,k)
			//手平举着的时候,    位置是vi, 骨骼变换矩阵式TM(i,k). ---这个就是Bone InitTM
			//四个权重对应位置为 W(k);

			//在第0帧的时候，顶点相对四个骨骼的的位移应该为 v0 * inv[TM(0,k)]. 
			//（这个值在Physique里取出来的就是vtxBlend->GetOffsetVector(j), 但是Skin只能自己算了
			//所以。其实我们并不需要这个Offset，从第0帧，加上i帧的Bone InitTM就可以算出这个offset）
			//那么从第0帧，算到第i帧（初始帧）, 根据骨骼动画公式。
			//vi = SIGMA< v0 * inv[TM(0,k)] * TM(i,k) > (k = 0, 4).
            Point3 v0 = m_pMesh->getVert(ivert) * m_pNode->GetObjTMAfterWSM(0);
            Point3 vInit;
            vInit.Set(0.0f,0.0f,0.0f);
            for(int i = 0 ; i < exportVertex.m_nEffBone ; i++)
            {
                Matrix3 matMeshInit;
                int boneIndex = exportVertex.m_BoneWeights[i].m_BoneIndex ;
                float fWeight = exportVertex.m_BoneWeights[i].m_fWeight;
                sMaxBoneNode_t& boneNode = pSkeleton->m_MaxBones[boneIndex];
                assert(exportVertex.m_BoneWeights[i].m_BoneID == boneNode.m_Bone.m_BoneID);
                matMeshInit = Inverse(boneNode.m_InitNodeTM0) * boneNode.m_SkinInitMT;
                Point3 pt = v0 * matMeshInit;
                vInit.x = vInit.x + fWeight * pt.x;
                vInit.y = vInit.y + fWeight * pt.y;
                vInit.z = vInit.z + fWeight * pt.z;
            }
            exportVertex.m_InitPos.x = vInit.x;
            exportVertex.m_InitPos.y = vInit.y;
            exportVertex.m_InitPos.z = vInit.z;
        }

        m_VertexWeights[ivert] = exportVertex;
    }
	XEVOL_LOG(eXL_DEBUG_HIGH , L"   ------>Mesh:[%s] 的Vertex Weight已经成功从ISkin里构造完毕<--------\n" , INodeName(m_pNode).c_str()  );
    return true;
}
bool CMaxMesh::__extract_vertex_weight_fromlinkednode(INode* pFakeBone, CSkeletonExporter* pSkeleton)
{
    int nNumVerts = m_pMesh->numVerts;

    std::wstring BoneName = INodeName( pFakeBone );
	XEVOL_LOG(eXL_DEBUG_HIGH , L"   ------>Mesh:[%s] 开始从Linked Node里Vertex Weights<--------\n" , INodeName(m_pNode).c_str()  );

    // 得到将导出顶点的数据
    sWeightedVertex_t exportVertex;
    m_VertexWeights.clear();
    m_VertexWeights.resize(nNumVerts);
    
    int boneIndex  = pSkeleton->push_bone(pFakeBone);

	if(boneIndex < 0)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH , L"   {error} Mesh: [%s]  伪骨头 找不到BondeIndex，骨架错误 FakeBone=[%s] \n" , INodeName(m_pNode).c_str() , INodeName(pFakeBone).c_str() );
		return false;
	}

	//这个Mesh只用了一个mesh的。
	xMeshBone_t usedBone;
	usedBone.m_BoneIndex = boneIndex;
	usedBone.m_pBone     = pFakeBone;
	m_MeshData.addUsedBone(usedBone);

    sMaxBoneNode_t& boneNode = pSkeleton->m_MaxBones[boneIndex];
    for(int ivert = 0 ; ivert < nNumVerts ; ++ivert)
    {

        exportVertex.m_nEffBone = 1.0f;
        exportVertex.m_BoneWeights[0].m_BoneID = boneNode.m_Bone.m_BoneID;
        exportVertex.m_BoneWeights[0].m_BoneIndex = boneIndex;
        exportVertex.m_BoneWeights[0].m_fWeight = 1.0f;

        //算出蒙皮的初始位置
        {
            Point3 v0 = m_pMesh->getVert(ivert) * m_pNode->GetObjTMAfterWSM(0);

            Matrix3 matMeshInit = Inverse(boneNode.m_InitNodeTM0) * boneNode.m_SkinInitMT;

            Point3 vInit = v0 * matMeshInit;
            exportVertex.m_InitPos.x = vInit.x;
            exportVertex.m_InitPos.y = vInit.y;
            exportVertex.m_InitPos.z = vInit.z;
        }

        m_VertexWeights[ivert] = exportVertex;
    }

	XEVOL_LOG(eXL_DEBUG_HIGH , L"   ------>Mesh:[%s] 从Linked Node里构造Vertex Weights完毕\n" , INodeName(m_pNode).c_str()  );
    return true;
}

bool CMaxMesh::__calc_skinmesh_vertex_weights(CSkeletonExporter* pSkeleton)
{
    bool ret =  false;
    m_VertexWeights.clear();
    Modifier* pModify = CMaxEnv::FindPhysiqueModifier(m_pNode);
    if( pModify != NULL )
    {
        IPhysiqueExport *phyExp = (IPhysiqueExport *)pModify->GetInterface(I_PHYINTERFACE);
        if(phyExp == NULL)
        {
            ret =  false;
            goto error_line;
        }
        IPhyContextExport *mcExport = (IPhyContextExport *)phyExp->GetContextInterface(m_pNode);
        if(mcExport == NULL)
        {
            ret =  false;
            goto error_line;
        }
        ret = __extract_vertex_weight_fromphysique(mcExport,pSkeleton);
        phyExp->ReleaseContextInterface(mcExport);
        pModify->ReleaseInterface(I_PHYINTERFACE, phyExp);
        return ret;
    }
    pModify = CMaxEnv::FindSkinModifier(m_pNode);
    if(pModify != NULL)
    {
        ISkin *pSkin = (ISkin*)pModify->GetInterface(I_SKIN);
        if(pSkin == NULL)
        {
            ret =  false;
            goto error_line;
        }
        ISkinContextData* pSkinContext = pSkin->GetContextInterface(m_pNode);
        if(pSkinContext == NULL)
        {
            ret =  false;
            goto error_line;
        }
        ret = __extract_vertex_weight_fromskin(pSkin,pSkinContext,pSkeleton);
        pModify->ReleaseInterface(I_SKIN, pSkin);
        return ret;
    }
    INode* pParent = m_pNode->GetParentNode();
    if(pParent && pParent != CMaxEnv::singleton().m_pInterace->GetRootNode())
        return __extract_vertex_weight_fromlinkednode(m_pNode->GetParentNode(),pSkeleton);
    else
        return __extract_vertex_weight_fromlinkednode(m_pNode , pSkeleton);
error_line:

    return ret;
}



