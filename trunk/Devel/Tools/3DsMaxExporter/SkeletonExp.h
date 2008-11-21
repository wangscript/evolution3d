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

#pragma  once
#include "MaxEnv.h"
#include "Math.h"
#include "DataDefine.h"
using namespace XEvol3DModel;
#include <vector>
#include <string>
#include <iostream>

using namespace std;

struct sMaxBoneNode_t
{
	sBone_t          m_Bone;
	INode*           m_pNode;
	//骨头第一帧的矩阵
	Matrix3          m_InitNodeTM0;
	//蒙皮的初始化矩阵
	Matrix3          m_SkinInitMT;
};

template <typename T, typename T2> T& conv_type(T2& data)
{
	return  *((T*)&data);
}

template <typename T, typename T2> T& conv_type(T2* data)
{
	return  *((T*)data);
}

class CSkeletonExporter
{
public:
	bool        m_isLoaded ;
	float       m_fScale;
	union
	{
		struct 
		{
			long        m_IDHW;
			long        m_IDLW;
		};
		__int64 m_ID;
	}m_SkeletonID;



	int         __getSkinMeshModifers(INode* pNode);
	void        __findSkinMeshModifers(INode* pNode);
	int         __findBonePhysiqueIndex(INode* pNode);
	int         __findBoneSkinIndex(INode* pNode);
public:

	CSkeletonExporter()
	{
		m_fScale   = 1.0f;
		m_isLoaded = false; //如果是Loaded的，就不允许添加Bone，PushBone一个不存在的Bone会失败
	}
	typedef vector<IPhysiqueExport*> sPhysiqueExports_t;
	typedef vector<ISkin*>           sSkins_t;
	typedef vector<Modifier*>        sModifiers_t;
	typedef vector<sMaxBoneNode_t>   sMaxBoneNodes_t;

public:
	IPhysiqueExport* findMeshPhysiqueExport(INode* pNode);
	sSkeletonID_t    makeID();
	sSkeletonID_t    getID();

	template <typename ISTREAM_T> bool  load_base_info(ISTREAM_T& stream);
	template <typename OSTREAM_T> bool  save_base_info(OSTREAM_T& stream);
	template <typename OSTREAM_T> bool  save_hiberarchys(OSTREAM_T& stream);
	template <typename OSTREAM_T> bool  save_hiberarchys_xml(OSTREAM_T& asciistream);
	
public:
	bool  save_hiberarchys_xml_node(xCfgNode* pRootNode , int boneIndex);

	bool        save_skeleton(xcomdoc& doc,wstring skeleton_dir = L"skeleton/");
	//bool        save_bone_inf(xcomdoc& comdoc,wstring skeleton_dir);

	int         build_modifiers(sMaxNodes_t nodes);
	int         build_modifiers(bool selected);
	void        release_modifiers();
	int         build_from_scene();
	bool        init_max_scene();
	int         find_bone(INode* pBone);
	int         push_bone(INode* pBone);
	void        push_bones(sMaxNodes_t Nodes);
	void        buildHiberarchys();

	void        get_bone_nodes(sMaxNodes_t& bones);
public:
	sPhysiqueExports_t m_PhysiqueExports;
	sSkins_t           m_SkinModifiers;
	sModifiers_t       m_ModifiersForPy;
	sModifiers_t       m_ModifiersForSkin;
	sMaxBoneNodes_t    m_MaxBones;

	sBoneHiberarchy_t  m_RootHiberarchys;
	sBoneHiberarchys_t m_BoneHiberarchys;

};

#include "Skeleton_Helper.h"

CSkeletonExporter& GetRoleSkeletonExp();

