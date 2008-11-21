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
#include "SkeletonExp.h"
#include "xStringHash.h"

CSkeletonExporter& GetRoleSkeletonExp()
{
    static CSkeletonExporter gs_SkelExp;
    return gs_SkelExp;
}

void CSkeletonExporter::__findSkinMeshModifers(INode* pNode)
{
    __getSkinMeshModifers(pNode);
    int nChildNode = pNode->NumberOfChildren();
    for(int i = 0 ; i < nChildNode ; ++i)
    {
        __findSkinMeshModifers(pNode->GetChildNode(i));
    }
    return ;
}

int CSkeletonExporter::__findBonePhysiqueIndex(INode* pNode)
{
    size_t nPhysique = m_PhysiqueExports.size();
    Matrix3 mt;
    for(size_t i = 0 ; i < nPhysique ; ++i)
    {
        IPhysiqueExport* Physique = m_PhysiqueExports[i];
        if(Physique->GetInitNodeTM(pNode,mt) == MATRIX_RETURNED)
            return (int)i;
    }
    return -1;
}

int CSkeletonExporter::__findBoneSkinIndex(INode* pNode)
{
    size_t nSkin = m_SkinModifiers.size();
    Matrix3 mt;
    for(size_t i = 0 ; i < nSkin ; ++i)
    {
        ISkin* Skin = m_SkinModifiers[i];
        if(Skin->GetBoneInitTM(pNode,mt,true)  == SKIN_OK)
            return (int)i;
    }
    return -1;
}

int CSkeletonExporter::__getSkinMeshModifers(INode* pNode)
{
    if(pNode == NULL)
        return -1;


    Modifier* pModify = CMaxEnv::FindPhysiqueModifier( pNode );
    if( pModify == NULL )
    {

        //检测Skin
        pModify = CMaxEnv::FindSkinModifier(pNode);
        
        if(pModify == NULL )
            return -1;

        ISkin* pSkin = (ISkin*)pModify->GetInterface(I_SKIN);
        if(pSkin == NULL)
            return -1;

        int nSkin = (int)m_SkinModifiers.size();
        for(int i = 0 ;i < nSkin ; ++i)
        {
            ISkin* old_skin = m_SkinModifiers[i] ;
            if(old_skin == pSkin)
            {
                return i;
            }
        }  
        m_SkinModifiers.push_back(pSkin);
        m_ModifiersForSkin.push_back(pModify);
        return nSkin;

    }
    IPhysiqueExport *phyExport = (IPhysiqueExport *)pModify->GetInterface(I_PHYINTERFACE);
    int nPhyExp = (int)m_PhysiqueExports.size();
    //添加这个PhysiqueExport到PhysiqueExport列表中．不重复添加
    if(phyExport)
    {
        for(int i = 0 ;i < nPhyExp ; ++i)
        {
            IPhysiqueExport* old_phy = m_PhysiqueExports[i] ;
            if(old_phy == phyExport)
            {
                return i;
            }
        }
    }
    m_PhysiqueExports.push_back(phyExport);
    m_ModifiersForPy.push_back(pModify);
    return nPhyExp;
}


int CSkeletonExporter::build_from_scene()
{
    return 1;
}

bool CSkeletonExporter::init_max_scene()
{
    CMaxEnv::singleton().Init_scene_bone_id();
    return true;
}

int   CSkeletonExporter::find_bone(INode* pBone)
{
    size_t nBone = m_MaxBones.size() ; 
    for(size_t i = 0 ; i < nBone ; ++ i)
    {
        sMaxBoneNode_t& BoneNode = m_MaxBones[i];
        if(BoneNode.m_pNode == pBone)
            return (int)i;
    }
    return -1;
}

int   CSkeletonExporter::push_bone(INode* pBone)
{ 
      int boneIndex = -1;
      boneIndex = find_bone(pBone);
      if(boneIndex != -1)
          return boneIndex;

      if(m_isLoaded == true)
          return -1;


      int pyIndex = __findBonePhysiqueIndex(pBone);
      int psIndex = __findBoneSkinIndex(pBone);
      IPhysiqueExport* py = NULL;
      ISkin*           pS = NULL;
      if(pyIndex != -1)
      {
          py = m_PhysiqueExports[pyIndex];
      }
      if(psIndex != -1)
      {
          pS = m_SkinModifiers[psIndex];
      }

      if(pS == NULL && py == NULL)
      {
             XEVOL_LOG(eXL_DEBUG_HIGH , L"   +++++++++>这个Bone没有找到对应的Skin或者Physique \n   BoneNode=[%s]\n" , INodeName(pBone).c_str() );
      }
      sMaxBoneNode_t BoneData;

      int nBone = (int)m_MaxBones.size();
      if( pBone->GetUserPropInt("BoneID",BoneData.m_Bone.m_BoneID) == FALSE)
      {
            MessageBoxW(GetActiveWindow(),(wstring(L"这个Bone没有找到BoneID \n   MaxNode: ") + INodeName(pBone ) ).c_str(),L"错误",MB_OK);
            BoneData.m_Bone.m_BoneID = -1;
      }


      INode* pParentNode = pBone->GetParentNode();
      if(pParentNode)
      {
          if( pParentNode->GetUserPropInt("BoneID",BoneData.m_Bone.m_ParentBoneID) == FALSE)
          {
              MessageBoxW(GetActiveWindow(),(wstring(L"这个Bone没有找到BoneID \n   MaxNode: ")+ INodeName(pParentNode) ).c_str(),L"错误",MB_OK);
              BoneData.m_Bone.m_ParentBoneID = -1;
          }
      }
      else
      {
           BoneData.m_Bone.m_ParentBoneID = 0;
      }
      BoneData.m_pNode     = pBone;
      
      if(py)
      {
          py->GetInitNodeTM(pBone,BoneData.m_SkinInitMT);
      }
      else if(pS)
      {
          pS->GetBoneInitTM(pBone,BoneData.m_SkinInitMT);
      }
      else //假的骨头
      {
          BoneData.m_SkinInitMT = pBone->GetNodeTM(0);
      }
      BoneData.m_InitNodeTM0 = pBone->GetNodeTM(0);


      if(CMaxEnv::singleton().m_bUseBeforeSkeletonPose)
      {
          BoneData.m_Bone.m_InitMTInv = conv_type<sMatrix4x3_t,Matrix3>(Inverse(BoneData.m_SkinInitMT));
          BoneData.m_Bone.m_InitMT = conv_type<sMatrix4x3_t,Matrix3>(BoneData.m_SkinInitMT);
      }
      else
      {
          BoneData.m_Bone.m_InitMT = conv_type<sMatrix4x3_t,Matrix3>(BoneData.m_InitNodeTM0);
          BoneData.m_Bone.m_InitMTInv = conv_type<sMatrix4x3_t,Matrix3>(Inverse(BoneData.m_InitNodeTM0));
      }

	  wcsncpy( BoneData.m_Bone.m_BoneName ,  INodeName(BoneData.m_pNode).c_str() ,32);
      m_MaxBones.push_back(BoneData);
      return nBone;

}

void CSkeletonExporter::get_bone_nodes(sMaxNodes_t& bones)
{
    for(size_t i = 0 ; i < m_MaxBones.size() ;  i++)
	{
		bones.push_back(m_MaxBones[i].m_pNode);
	}
}

void CSkeletonExporter::buildHiberarchys()
{
    m_BoneHiberarchys.clear();
    m_RootHiberarchys.clear();

    m_BoneHiberarchys.resize(m_MaxBones.size());

    //找到所有的根节点
    for(int i = 0 ;i < (int)m_MaxBones.size() ; ++ i)
    {
          sMaxBoneNode_t& bone = m_MaxBones[i];
          bool isRoot = true;
          for(int j = 0 ; j < (int)m_MaxBones.size() ; j++ )
          {
              sMaxBoneNode_t& bone2 = m_MaxBones[j];
              
              if(bone.m_Bone.m_ParentBoneID == bone2.m_Bone.m_BoneID)
              {
                  //有父亲
                  isRoot = false;
                  break;
              }
          }
          if(isRoot)
          {
              //他是个根节点
              bone.m_Bone.m_ParentIndex = -1;
              m_RootHiberarchys.add_child(i);
          }
    }

    //找到所有节点的父节点
    for(int i = 0 ;i < (int)m_MaxBones.size() ; ++ i)
    {
          sMaxBoneNode_t& bone = m_MaxBones[i];

          for(int j = 0 ; j < (int)m_MaxBones.size() ; j++ )
          {
              sMaxBoneNode_t& bone2 = m_MaxBones[j];
              if( (bone2.m_Bone.m_ParentBoneID != -1) && 
                  (bone2.m_Bone.m_ParentBoneID == bone.m_Bone.m_BoneID) )
              {
                   //找到父节点
                   bone2.m_Bone.m_ParentIndex = i;
                   //父节点多了个子节点
                   m_BoneHiberarchys[i].add_child(j);
              }
          }
   }
}
void   CSkeletonExporter::push_bones(sMaxNodes_t Nodes)
{
     size_t nNode = Nodes.size();
     for(size_t i = 0 ; i < nNode ; ++i)
     {
          push_bone(Nodes[i]);  
     }
}


IPhysiqueExport* CSkeletonExporter::findMeshPhysiqueExport(INode* pNode)
{
    Modifier* pModify = CMaxEnv::FindPhysiqueModifier( pNode );
    if( pModify == NULL )
    {
        return NULL;
    }
    IPhysiqueExport *phyExport = (IPhysiqueExport *)pModify->GetInterface(I_PHYINTERFACE);
    return phyExport;
}

void  CSkeletonExporter::release_modifiers()
{
    for(size_t i = 0 ; i < m_ModifiersForPy.size() ; ++i)
    {
        m_ModifiersForPy[i]->ReleaseInterface(I_PHYINTERFACE,m_PhysiqueExports[i]); 
    }
    for(size_t i = 0 ; i < m_ModifiersForSkin.size() ; ++i)
    {
        m_ModifiersForSkin[i]->ReleaseInterface(I_SKIN,m_SkinModifiers[i]); 
    }

    m_PhysiqueExports.clear();
    m_ModifiersForPy.clear();
    m_ModifiersForSkin.clear();
    m_SkinModifiers.clear();
}
int  CSkeletonExporter::build_modifiers(sMaxNodes_t nodes)
{
	m_PhysiqueExports.clear();
	m_SkinModifiers.clear();
	m_ModifiersForPy.clear();
	m_ModifiersForSkin.clear();
	for(size_t i = 0 ; i < nodes.size() ; i++)
	{
		__findSkinMeshModifers(nodes[i]);
	}
	return (int)m_PhysiqueExports.size();  
}
int  CSkeletonExporter::build_modifiers(bool selected)
{
    INode* pRootNode = CMaxEnv::singleton().m_pInterace->GetRootNode();
    m_PhysiqueExports.clear();
    m_SkinModifiers.clear();
    m_ModifiersForPy.clear();
    m_ModifiersForSkin.clear();
    if(selected)
    {
        int nSelNode = CMaxEnv::singleton().m_pInterace->GetSelNodeCount();
        for(int i = 0 ; i < nSelNode ; ++i)
        {
             __findSkinMeshModifers(CMaxEnv::singleton().m_pInterace->GetSelNode(i));
        }
    }
    else
    {
        __findSkinMeshModifers(pRootNode);
    }

    if(m_PhysiqueExports.size() == 0 && m_SkinModifiers.size() == 0)
    {
        MessageBoxA(GetActiveWindow(),"年轻人,别玩我...\n场景中没有有效的Skin/Physique修改器，\n你的模型一定不是个骨骼动画!!","错误",MB_OK);
    }
    return (int)m_PhysiqueExports.size();     
} 

bool  CSkeletonExporter::save_skeleton(xcomdoc& doc,wstring skeleton_dir)
{
	makeID();
	xcomdocstream* pHiberarchyStream =  doc.open_stream((skeleton_dir + L"hiberarchy").c_str(),false);
	if(pHiberarchyStream)
	{
		save_hiberarchys(*pHiberarchyStream);
		doc.close_stream(pHiberarchyStream);
	}
   
	xcomdocstream* pHiberarchyXMLStream =  doc.open_stream((skeleton_dir + L"skeleton.xml").c_str(),false);
	if(pHiberarchyXMLStream)
	{
		xcdstream _out(pHiberarchyXMLStream);
		save_hiberarchys_xml(_out);
		_out.close();
		 doc.close_stream(pHiberarchyXMLStream);
	}

	xcomdocstream* pSkeletonStream =  doc.open_stream((skeleton_dir + L"skeleton").c_str(),false);
	if(pSkeletonStream)
	{
		bool ret= save_base_info(*pSkeletonStream);
		doc.close_stream(pSkeletonStream);
		return ret;
	}
	return false;

}

sSkeletonID_t CSkeletonExporter::makeID()
{
	__int64 ID = 0;
	wstring  name = L"";
	for(size_t i = 0 ; i < m_MaxBones.size() ; ++i)
	{
		ID += m_MaxBones[i].m_Bone.m_BoneID;
		name += INodeName(m_MaxBones[i].m_pNode);
	}

	int hashVA = xStringHash ( name.c_str() );
	int hashVB = xStringHash2( name.c_str() );

	m_SkeletonID.m_ID = ID;
	m_SkeletonID.m_IDHW = hashVA;
	m_SkeletonID.m_IDLW = hashVB;

	sSkeletonID_t sid;
	sid.m_HiWord = m_SkeletonID.m_IDHW ;
	sid.m_LoWord = m_SkeletonID.m_IDLW;
	return sid;
}

sSkeletonID_t CSkeletonExporter::getID()
{
	sSkeletonID_t sid;
	sid.m_HiWord = m_SkeletonID.m_IDHW ;
	sid.m_LoWord = m_SkeletonID.m_IDLW;
	return sid;
}
