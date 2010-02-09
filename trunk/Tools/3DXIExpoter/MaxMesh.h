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

#ifndef __MAX_NODE_MESH_H__
#define __MAX_NODE_MESH_H__
#include "MaxEnv.h"
#include "Math.h"
#include "DataDefine.h"
using namespace XEvol3DModel;
#include <vector>
#include <string>
#include <hash_map>
using namespace std;
using namespace stdext;
#include "MaxMeshDefine.h"

class CMaxSubMesh : public sSubMeshDesc_t
{
public:
	typedef sFace_t*  sFaces_t;
public:
	CMaxSubMesh() 
	{
		m_nFace = 0 ;
		m_Faces = NULL;
	}
	~CMaxSubMesh()
	{
		
	}
	void           Free()
	{
		if(m_Faces) 
		{
			delete [] m_Faces;
			m_Faces = NULL;
		}
	}

	sSubMeshDesc_t GetSubMeshDesc()
	{
		sSubMeshDesc_t desc;
		desc.m_nFace = m_nFace;
		desc.m_nMatIndex = m_nMatIndex;
		return desc;
	}
	IGameMaterial* m_pGameMaterial;
	sFaces_t       m_Faces;
	int            m_MatID;
};

class xMeshData
{
public:
	typedef std::vector<CMaxSubMesh> vMaxSubMeshs;
	sVertexData_t                    m_VertexData;
	Materials_t                      m_MatsExport;
	sMeshDesc_t                      m_MeshDesc;
	sBoundBox_t                      m_BoundBox;
	INode*                           m_pNode;
	xMeshBones_t                     m_UsedBones;
	wstring                          m_MatName;
	vMaxSubMeshs                     m_vSubMeshs;
};

class CMaxMesh : public xMeshData
{
public:
	typedef std::map<int , IGameMaterial*>  vGameMaterials;
	
    typedef std::map<CMaxVertexIndex , int> vMaxVertexIndexMap;
    typedef std::vector<CMaxVertexIndex>    vMaxVertexIndexs;
	vector<sMaxMorphTarget_t>      m_MorphTargets;
	IGameNode*                     m_pGameNode;
	IGameMesh*                     m_pGameMesh;
	IGameObject*                   m_pGameObject;
	IGameMorpher*                  m_pMorpModifer;
	IGameSkin*                     m_pSkinModifer;
	IGameMaterial*                 m_pGameMaterial;
	vGameMaterials                 m_vGameMaterials;
	
	bool                           m_bFaceInCreated;
	Tab<int>                       m_ActiveChannels;
	int                            m_CurrentTime;
	int                            m_CurrentFrame;

	//顶点索引
	vMaxVertexIndexs               m_MaxVertexIndexs;
	//
	Mesh*                          m_pMaxMesh;
	bool                           m_bMirrored;
public:
	~CMaxMesh();
	CMaxMesh();
	CMaxMesh(INode* pNode , CSkeletonExporter* pSkeleton);
	void            SetScale(float scale);
	void            SetColorScale(float r_s, float g_s , float b_s , float a_s);
	bool            Init(INode* pNode , CSkeletonExporter* pSkeleton);
	bool            ParseGameMaterial(IGameMaterial* pGameMaterial , int matID);
	bool            CreateFaces();
	bool            CreateMesh(unsigned int iMaxTime , CSkeletonExporter* pSkeleton);
	bool            IsMeshMirror();	
	int             AddMaxVertexIndex(CMaxVertexIndex& indx , vMaxVertexIndexMap& _map);
	int             MapChannel2UVIndex(int mapChannel);
	int             MapMatID2MatIndex(IGameMaterial* pMaterial);
	bool            BuildMaterialExport();
	bool            BuildMeshDesc();
	bool            Free();
	bool            IsExportable();
	Matrix3         Transpose(Matrix3& matrix);
	IGameMaterial*  FindGameMaterialByMatID(int matID);
	Matrix3         FindMapChannelTM(int chIdx, IGameMaterial* pMaterial);
	bool            BuildVertexData(CSkeletonExporter* pSkeleton);
	bool            BuildVertexWeights(CSkeletonExporter* pSkeleton);
	bool            ExtractVertexWeights(CSkeletonExporter* pSkeleton);
	bool            ExtractVertexWeightsFromModifier(IGameSkin* pGameSkin ,CSkeletonExporter* pSkeleton);
	bool            ExtractVertexWeightsFromLink(INode* pFakeBone, CSkeletonExporter* pSkeleton);
	bool            CalcTBN(sFace_t& face,bool isSkin);
	void            CalcTangents(bool isSkin);
	bool            BuildMorphTarget();
	void            FindAllUsedBone(CSkeletonExporter* pSkeleton);
	void            InitSkeleton(CSkeletonExporter* pSkeleton);
protected:
	void _InitData();
private:
	/*和Mesh数据相关的东西*/
	sMeshDataFlag m_MeshDataFlag;
public:
	void addUsedBone(xMeshBone_t& bone)
	{
		for(size_t i = 0 ; i < m_UsedBones.size(); ++i)
		{
			if(m_UsedBones[i].m_pBone == bone.m_pBone)
				return ;
		}
		m_UsedBones.push_back(bone);
	}

private:
	sVectors_t                       m_VertexNormals;
	sVectors_t                       m_FaceNormals;	
protected:
	//骨骼动画相关的数据
	sWeightedVertexs_t               m_VertexWeights;
};

typedef std::vector<CMaxMesh> MaxMeshs;
#include "MaxMeshBuilder.h"

#endif

