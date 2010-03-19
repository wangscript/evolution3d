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

int CSkeletonExporter::AddSkinModifier(IGameSkin* pSkinModifer)
{
	if(pSkinModifer == NULL)
		return 0;
	size_t nMod = m_GameSkins.size() ;
    for(size_t i = 0 ; i < nMod ; i ++)
	{
		if(m_GameSkins[i] == pSkinModifer)
			return 0;
	}
	m_GameSkins.push_back(pSkinModifer);
	return 1;
}

void CSkeletonExporter::AddAllSkinModifers(INode* pNode)
{
    AddSkinModifier(pNode);
    int nChildNode = pNode->NumberOfChildren();
    for(int i = 0 ; i < nChildNode ; ++i)
    {
        AddAllSkinModifers(pNode->GetChildNode(i));
    }
    return ;
}

int CSkeletonExporter::AddSkinModifier(INode* pNode)
{
	if(pNode == NULL)
		return -1;
	int nAdd = 0;
	IGameNode* pGameNode = G_GameScene()->GetIGameNode(pNode);
	if(pGameNode == NULL)
        return -1;
    IGameObject* pGameObject = pGameNode->GetIGameObject();
	int nIModifier = pGameNode->GetIGameObject() ->GetNumModifiers();
	for(int i = 0 ; i < nIModifier ; i ++)
	{
		IGameModifier* pModifier = pGameObject->GetIGameModifier(i);
		if(pModifier->GetModifierType() == IGameModifier::IGAME_SKINNING)
		{
			IGameSkin* pSkinModifer = (IGameSkin*)pModifier;
			nAdd += AddSkinModifier(pSkinModifer);
		}
	}
	return nAdd;
}

int CSkeletonExporter::FindSkinModifierIndx(INode* pNode)
{
	size_t nMod = m_GameSkins.size() ;
	for(size_t i = 0 ; i < nMod ; i ++)
	{
		IGameSkin* pSkinModifer = m_GameSkins[i];
		GMatrix intMat;
		if( pSkinModifer->GetInitBoneTM(pNode , intMat) == true )
		{
			//找到了
			return (int)i;
		}
	}
	return -1;
}

IGameSkin* CSkeletonExporter::FindSkinModifier(INode* pNode)
{
	size_t nMod = m_GameSkins.size() ;
	for(size_t i = 0 ; i < nMod ; i ++)
	{
		IGameSkin* pSkinModifer = m_GameSkins[i];
		GMatrix intMat;
		if( pSkinModifer && pSkinModifer->GetInitBoneTM(pNode , intMat) == true )
		{
			//找到了
			return pSkinModifer;
		}
	}
	return NULL;
}

bool CSkeletonExporter::InitMaxScene()
{
    G_MaxEnv().InitSceneBoneID();
    return true;
}
void CSkeletonExporter::ensureHiberarchys()
{
    checkBoneLinked( CMaxEnv::Inst().m_pInterace->GetRootNode() );
}

bool CSkeletonExporter::isChildInSkeleton(INode* pNode)
{
    int nChild = pNode->NumberOfChildren();

    //没有子节点
    if(nChild == 0)
        return false;

    for(int i = 0 ; i < nChild ; i ++)
    {
        INode* pChild = pNode->GetChildNode(i);
        //某个子节点正在使用。返回TRUE;
        if(this->find_bone(pChild) != -1 )
            return true;
    }

    for(int i = 0 ; i < nChild ; i ++)
    {
        INode* pChild = pNode->GetChildNode(i);
        if(isChildInSkeleton(pChild) == true)
            return true;
    }
    return false;     
}

bool CSkeletonExporter::isParentInSkeleton(INode* pNode)
{
    INode* pParent = pNode->GetParentNode();
    if(pParent == NULL)
        return false;

    //如果父节点在使用，直接返回.
    if(this->find_bone(pParent) != -1 )
        return true;

    return isParentInSkeleton(pParent);
}

void CSkeletonExporter::checkBoneLinked(INode* pBone)
{
    if( isChildInSkeleton(pBone) && isParentInSkeleton(pBone) )
    {
        this->push_bone(pBone);
    }
    int nChild = pBone->NumberOfChildren();
    for(int i = 0 ; i < nChild ; i ++)
    {
        INode* pChild = pBone->GetChildNode(i);
        checkBoneLinked(pChild);
    }
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


	  IGameSkin* pGameSkin = FindSkinModifier(pBone);

      if( pGameSkin = NULL)
      {
             XEVOL_LOG(eXL_DEBUG_HIGH , L"   +++++++++>这个Bone没有找到对应的Skin或者Physique \n   BoneNode=[%s]\n" , INodeName(pBone).c_str() );
      }
      sMaxBoneNode_t BoneData;

      int nBone = (int)m_MaxBones.size();
      if( pBone->GetUserPropInt("BoneID",BoneData.m_Bone.m_BoneID) == FALSE)
      {
            MessageBoxW(GetActiveWindow(),(std::wstring(L"这个Bone没有找到BoneID \n   MaxNode: ") + INodeName(pBone ) ).c_str(),L"错误",MB_OK);
            BoneData.m_Bone.m_BoneID = -1;
      }


      INode* pParentNode = pBone->GetParentNode();
      if(pParentNode)
      {
          if( pParentNode->GetUserPropInt("BoneID",BoneData.m_Bone.m_ParentBoneID) == FALSE)
          {
              MessageBoxW(GetActiveWindow(),(std::wstring(L"这个Bone没有找到BoneID \n   MaxNode: ")+ INodeName(pParentNode) ).c_str(),L"错误",MB_OK);
              BoneData.m_Bone.m_ParentBoneID = -1;
          }
      }
      else
      {
           BoneData.m_Bone.m_ParentBoneID = 0;
      }
      BoneData.m_pNode     = pBone;
      

	  //SkinInitMT，可以认为是动画模型没有加Skin修改器前的那个姿势。
	  //因为这个矩阵，我们会用到。
	  //当从Physique里取出来的数据时候，我们得到的是应用修改后的姿势，就是手已经不是平举着的了。
	  //所以，如果我们算回到手是平举的那个姿势的时候，将会用到这个矩阵。
      if(pGameSkin)
      {
 		  GMatrix InitMat;
          pGameSkin->GetInitBoneTM(pBone,InitMat);
		  BoneData.m_SkinInitMT = InitMat.ExtractMatrix3();
      }
      else //假的骨头
      {
          BoneData.m_SkinInitMT = pBone->GetNodeTM(0);
      }
      BoneData.m_InitNodeTM0 = pBone->GetNodeTM(0);

      if(G_MaxEnv().m_bUseBeforeSkeletonPose)
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



void  CSkeletonExporter::RemoveModifiers()
{
    for(size_t i = 0 ; i < m_GameSkins.size() ; ++i)
    {
        IGameSkin* pGameSkin  = m_GameSkins[i]; 
    }
 
    m_GameSkins.clear();

}
int  CSkeletonExporter::AddSkinModifiers(sMaxNodes_t nodes)
{
	m_GameSkins.clear();
	for(size_t i = 0 ; i < nodes.size() ; i++)
	{
		AddSkinModifier( (INode*)nodes[i]);
	}
	return (int)m_GameSkins.size();  
}

int  CSkeletonExporter::AddSkinModifiers(bool selected)
{
    INode* pRootNode = G_MaxEnv().m_pInterace->GetRootNode();
    m_GameSkins.clear();
    if(selected)
    {
        int nSelNode = G_MaxEnv().m_pInterace->GetSelNodeCount();
        for(int i = 0 ; i < nSelNode ; ++i)
        {
             AddSkinModifier( (INode*) G_MaxEnv().m_pInterace->GetSelNode(i));
        }
    }
    else
    {
        AddAllSkinModifers( (INode*)pRootNode);
    }

    if(m_GameSkins.size() == 0 )
    {
        MessageBoxA(GetActiveWindow(),"年轻人,别玩我...\n场景中没有有效的Skin/Physique修改器，\n你的模型一定不是个骨骼动画!!","错误",MB_OK);
    }
    return (int)m_GameSkins.size();     
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
