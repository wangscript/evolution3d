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


template <typename ISTREAM_T> bool CSkeletonExporter::load_base_info(ISTREAM_T& stream)
{
	release_modifiers();
	this->build_modifiers(false);
	int nBone;
	m_isLoaded = false;
	stream.read( (char*)&nBone , sizeof(int) );
	sSkeletonID_t sid;
	stream.read( (char*)&sid,sizeof(sSkeletonID_t) );
	m_SkeletonID.m_IDHW  = sid.m_HiWord;
	m_SkeletonID.m_IDLW  = sid.m_LoWord;


	//读取这个骨架到底是不是使用了绑定前的姿势。
	bool bUseBeforeSkeletonPose ;
	stream.read( (char*)&bUseBeforeSkeletonPose,sizeof(bool) );
	CMaxEnv::singleton().UseBeforeSkeletonPose(bUseBeforeSkeletonPose);

	for(int i = 0 ;i < nBone ; ++i)
	{
		sMaxBoneNode_t MaxBone;
		stream.read( (char*)&MaxBone.m_Bone,sizeof(sBone_t)  );
		INode* pNode = CMaxEnv::FindNodeByBoneID(MaxBone.m_Bone.m_BoneID);
		if(pNode == NULL)
		{
			char buf[128];
			printf(buf,"骨头没找到%s,你加载了一个错误的骨架",MaxBone.m_Bone.m_BoneName);
			MessageBox(GetActiveWindow(),buf,"错误",MB_OK);
			return false;
		}
		int iBone = push_bone(pNode);
		//m_MaxBones[iBone].m_InitNodeTM0 = MaxBone.m_InitNodeTM0;
		//m_MaxBones[iBone].m_SkinInitMT  = MaxBone.m_SkinInitMT;

	}

	//读取骨架的缩放
	stream.read( (char*)&m_fScale,sizeof(float) );
    buildHiberarchys();
	m_isLoaded = true;
	return true;
}

template <typename OSTREAM_T> bool  CSkeletonExporter::save_base_info(OSTREAM_T& stream)
{
	int nBone = (int)m_MaxBones.size();
	stream.write( (const char*)&nBone , sizeof(int) );

	sSkeletonID_t sid;
	sid.m_HiWord = m_SkeletonID.m_IDHW ;
	sid.m_LoWord = m_SkeletonID.m_IDLW;
	stream.write( (const char*)&sid,sizeof(sSkeletonID_t) );

	XEVOL_LOG(eXL_DEBUG_HIGH , L"  =====开始写入action data==========\r\n");
	XEVOL_LOG(eXL_DEBUG_HIGH , L"        一共有 %d 个骨骼\r\n" , nBone );
	XEVOL_LOG(eXL_DEBUG_HIGH , L"   骨架ID=[%d + %d]\n",sid.m_HiWord,sid.m_LoWord );
	
	//写入这个骨架到底是不是使用了 帮定前的姿势。
	bool bUseBeforeSkeletonPose = CMaxEnv::singleton().m_bUseBeforeSkeletonPose;
	stream.write( (const char*)&bUseBeforeSkeletonPose,sizeof(bool) );

	Matrix3 FlipYZMat;
	FlipYZMat.IdentityMatrix();
	FlipYZMat.SetRow(0 , Point3(1.0f,0.0f,0.0f) );
	FlipYZMat.SetRow(1 , Point3(0.0f,0.0f,1.0f) );
	FlipYZMat.SetRow(2 , Point3(0.0f,1.0f,0.0f) );

	for(size_t iBone = 0 ; iBone <  (size_t)nBone ; ++iBone)
	{
		sMaxBoneNode_t bone =  m_MaxBones[iBone];
		Matrix3 InitTM,InitTMInv;
		if(CMaxEnv::singleton().m_bUseBeforeSkeletonPose)
			InitTM = bone.m_SkinInitMT;
		else
			InitTM = bone.m_InitNodeTM0 ;
		Point3 Trans = InitTM.GetTrans();
		InitTM.NoTrans();
		Trans.x *= m_fScale;
		Trans.y *= m_fScale;
		Trans.z *= m_fScale;
		InitTM.SetTrans(Trans);
		InitTMInv = Inverse(InitTM);
		bone.m_Bone.m_InitMTInv = conv_type<sMatrix4x3_t,Matrix3>(InitTMInv);
		bone.m_Bone.m_InitMT    = conv_type<sMatrix4x3_t,Matrix3>(InitTM);

		if(CMaxEnv::singleton().m_bInvertYZCoord == true)
		{
			Matrix3 fMat ;
			fMat = FlipYZMat * InitTM * FlipYZMat;
			//世界空间中的变换矩阵
			bone.m_Bone.m_InitMT    = conv_type<sMatrix4x3_t,Matrix3>(fMat);

			fMat = FlipYZMat * InitTMInv * FlipYZMat;
			bone.m_Bone.m_InitMTInv  = conv_type<sMatrix4x3_t,Matrix3>(fMat);
		}

		//Bone名字
		
		wcsncpy(bone.m_Bone.m_BoneName,INodeName(bone.m_pNode).c_str() ,32);
		stream.write( (char*)&bone.m_Bone,sizeof(bone.m_Bone));//先写入Bone的基本信息
	}

	//保存骨架的缩放
	stream.write( (const char*)&m_fScale,sizeof(float) );

	return true;
}

template <typename OSTREAM_T>
bool  CSkeletonExporter::save_hiberarchys(OSTREAM_T& stream)
{
	int nRootNode = (int)m_RootHiberarchys.m_Childrens.size() ;
	stream.write((const char*)&m_SkeletonID , sizeof(m_SkeletonID) );
	stream.write((const char*)&nRootNode,sizeof(int));
	for(int i = 0 ; i < nRootNode ; ++i)
	{
		int child = m_RootHiberarchys.m_Childrens[i];
		stream.write((const char*)&child,sizeof(int));
	}

	int nBone = (int) m_MaxBones.size();
	stream.write((const char*)&nBone,sizeof(int));
	for(int i = 0 ; i < nBone ; ++i)
	{
		sBoneHiberarchy_t& boneH = m_BoneHiberarchys[i];
		int nChild = (int)boneH.m_Childrens.size();
		stream.write((const char*)&nChild,sizeof(int));
		for(int j = 0 ; j < nChild ; ++j)
		{
			int child = boneH.m_Childrens[j];
			stream.write((const char*)&child,sizeof(int));
		}
	}
	return true;
}

inline bool  CSkeletonExporter::save_hiberarchys_xml_node(xXmlNode* pRootNode , int rootBoneIndex)
{
	sBoneHiberarchy_t& boneH = m_BoneHiberarchys[rootBoneIndex];
	int nChild = (int)boneH.m_Childrens.size();
	for(int j = 0 ; j < nChild ; ++j)
	{
		int index = boneH.m_Childrens[j];
		xXmlNode* BoneNode = pRootNode->insertNode(L"bone");
		BoneNode->setValue(L"name"  , INodeName(m_MaxBones[index].m_pNode).c_str() );
		BoneNode->setValue(L"index" , index);
		save_hiberarchys_xml_node(BoneNode , index);
	}
	return true;
}

template <typename OSTREAM_T>
bool  CSkeletonExporter::save_hiberarchys_xml(OSTREAM_T& asciistream)
{
	xXmlDocument doc;

	sSkeletonID_t sid;
	sid.m_HiWord = m_SkeletonID.m_IDHW ;
	sid.m_LoWord = m_SkeletonID.m_IDLW;

	int nBone = (int) m_MaxBones.size();
    xXmlNode* pRootNode = doc.insertNode(L"hiberarchy");
	pRootNode->setValue(L"nBone" , nBone);
	pRootNode->setValue(L"scale" , m_fScale);
	pRootNode->setValue(L"beforeSkelPose" , CMaxEnv::singleton().m_bUseBeforeSkeletonPose );

	//骨架类
	xXmlNode* pIDNode = pRootNode->insertNode(L"id");
	pIDNode->setHex(L"LoID" , m_SkeletonID.m_IDLW);
	pIDNode->setHex(L"HiID" , m_SkeletonID.m_IDHW);

	for(size_t i = 0 ; i < m_RootHiberarchys.m_Childrens.size() ; i ++)
	{
		int index = m_RootHiberarchys.m_Childrens[i];
		xXmlNode* BoneNode = pRootNode->insertNode(L"bone");
		BoneNode->setValue(L"name"  , INodeName(m_MaxBones[index].m_pNode).c_str() );
		BoneNode->setValue(L"index" , index);
		save_hiberarchys_xml_node(BoneNode , index);
	}
	doc.save(asciistream);
	return true;
}
