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
#include "ActionExporter.h"
#include "xXmlDocument.h"
#include <decomp.h> 

void     makBoneTrans2(INode* pNode , sBoneTrans_t& boneTrans , Matrix3& mat , bool& bAlert)
{
	Point3   Trans = mat.GetTrans();
	Matrix3  RotMat = mat;
	Matrix3  ScaleMat = mat;

    RotMat.NoScale();
    ScaleMat = mat * Inverse(RotMat);
    //算出Scale Matrix;

	RotMat.NoTrans();	
	Quat RotQuat(RotMat);

	//ScaleMat.NoRot();
	//ScaleMat.NoTrans();

	boneTrans.m_Rotate    = conv_type<sQuat_t , Quat>(RotQuat);
	boneTrans.m_Trans     = conv_type<sVector_t   ,Point3 >(Trans);
	boneTrans.m_Scale.x   = ScaleMat.GetRow(0).x;
	boneTrans.m_Scale.y   = ScaleMat.GetRow(1).y;
	boneTrans.m_Scale.z   = ScaleMat.GetRow(2).z;
	if( abs(boneTrans.m_Scale.x - boneTrans.m_Scale.y) > 0.000001 ||
		abs(boneTrans.m_Scale.y - boneTrans.m_Scale.z) > 0.000001 ||
		abs(boneTrans.m_Scale.z - boneTrans.m_Scale.x) > 0.000001 )
	{
            std::wstring _NodeName = INodeName(pNode);
            if(bAlert)
            {
                XEVOL_LOG(eXL_DEBUG_HIGH , L"   {警告} :  骨头[ %s ] 的上有NonUniformScale\r\n", _NodeName.c_str() );
                bAlert = false;
            }
	}
}

void     makBoneTrans(INode* pNode , sBoneTrans_t& boneTrans , Matrix3& mat , bool& bAlert)
{

    Point3   Scale ;
	Quat     Rot;
	Point3   Trans = mat.GetTrans();
	SpectralDecomp(mat,Scale ,Rot );

	makBoneTrans2(pNode , boneTrans,mat , bAlert);
	//boneTrans.m_Rotate    = conv_type<sQuat_t , Quat>(Rot);
	//boneTrans.m_Trans     = conv_type<sVector_t   ,Point3 >(Trans);
	//boneTrans.m_Scale     = conv_type<sVector_t   ,Point3 >(Scale);

}

Matrix3  CActionExporter::get_locale_tm(INode* pNode , bool hasParent,unsigned int iMaxTime)
{
     if(hasParent && pNode->GetParentNode())
     {
         Matrix3 parentTM = pNode->GetParentNode()->GetNodeTM(iMaxTime);
         return pNode->GetNodeTM(iMaxTime) * Inverse(parentTM);
     }
     return pNode->GetNodeTM(iMaxTime);
}


Matrix3 CActionExporter::get_bone_tm(CSkeletonExporter* pSkeleton,int boneIndex,unsigned int iMaxTime)
{
         sMaxBoneNode_t bone = pSkeleton->m_MaxBones[boneIndex];
         Matrix3        boneInitMTInv;
         if(CMaxEnv::singleton().m_bUseBeforeSkeletonPose)
             boneInitMTInv = bone.m_SkinInitMT;
         else
             boneInitMTInv = bone.m_InitNodeTM0;

         boneInitMTInv.Invert();
         INode* pNode = bone.m_pNode;

         Matrix3 BoneTM = boneInitMTInv * pNode->GetNodeTM(iMaxTime); //>GetNodeTM(iMaxTime);
         Point3 Trans = BoneTM.GetTrans();
         BoneTM.NoTrans();
         //对骨骼进行缩放
         Trans.x *= pSkeleton->m_fScale;
         Trans.y *= pSkeleton->m_fScale;
         Trans.z *= pSkeleton->m_fScale;
         BoneTM.SetTrans(Trans);

         return BoneTM;

}

CActionExporter::CActionExporter()
{
    m_bAlertStringSet = NULL;
    m_FlipYZMat.IdentityMatrix();
    m_FlipYZMat.SetRow(0 , Point3(1.0f,0.0f,0.0f) );
    m_FlipYZMat.SetRow(1 , Point3(0.0f,0.0f,1.0f) );
    m_FlipYZMat.SetRow(2 , Point3(0.0f,1.0f,0.0f) );
}

bool CActionExporter::load_action_list(sActionInfos_t& actions,xcomdoc& comdoc , std::wstring action_list_name)
{

	xcomdocstream* pactionsstreamTXT = comdoc.open_stream((action_list_name).c_str());
	xXmlDocument  xmlDoc;
	if(pactionsstreamTXT->data_len())
	{
		int dataLen = (int)pactionsstreamTXT->data_len()  ;
		char* buf = new char[dataLen];
		pactionsstreamTXT->stream_seekr(xcdsd_beg,0);
		pactionsstreamTXT->read(buf,dataLen);
		xmlDoc.load(buf,dataLen,true);
		delete buf;
	}

	comdoc.close_stream(pactionsstreamTXT);
	xXmlNode* rootNode = xmlDoc.root();
	if(rootNode == NULL)
		return true;

    for(int i = 0 ;  i < rootNode->countNode() ; i ++)
	{
		xXmlNode* actNode = rootNode->findNode(i);
		if( actNode )
		{

			sActionInfo_t action_info;
			wcsncpy(action_info.m_Name,actNode->name(),32);
			action_info.m_ActionType  = actNode->int_value(L"ActionType");
			action_info.m_iFirstFrame = actNode->int_value(L"FirstFrame");
			action_info.m_iLastFrame  = actNode->int_value(L"LastFrame");
			action_info.m_nFrame      = actNode->int_value(L"nFrame");
			action_info.m_lTime       = actNode->int_value(L"DurTime");
			actions.push_back(action_info);
		}
	}
	return true;
}

void CActionExporter::writeActionList(sActionInfos_t& actions, xcomdoc&  comdoc, std::wstring action_list_name)
{
	xcomdocstream* pactionsstream = comdoc.open_stream(action_list_name.c_str(),false);
	pactionsstream->stream_seekr(xcdsd_beg,0);
	xXmlDocument xmlDoc;

	if(pactionsstream->data_len())
	{
		int dataLen = (int)pactionsstream->data_len();
		char* buf = new char[dataLen];
		pactionsstream->stream_seekr(xcdsd_beg,0);
		pactionsstream->read(buf,dataLen);
		xmlDoc.load(buf,dataLen,true);
		delete buf;
	}
	if(xmlDoc.countNode() == 0)
	{
		xmlDoc.insertNode(L"actions");
	}
    xXmlNode* rootNode = xmlDoc.root();

	for(size_t i = 0 ;  i < actions.size(); i ++)
	{
		actions[i].m_nFrame = GetActNumFrame(actions[i]);
		if(rootNode->findNode(actions[i].m_Name))
		{
             XEVOL_LOG(eXL_DEBUG_HIGH , L"{错误} :  名字为 <  %s > 的动作已经存在\r\n" , actions[i].m_Name );
			continue;
		}
		else
		{
			xXmlNode* actionNode = rootNode->insertNode( actions[i].m_Name );
			actionNode->setValue(L"Name" , actions[i].m_Name);
            actionNode->setValue(L"ActionType",  actions[i].m_ActionType);
			actionNode->setValue(L"FirstFrame",  actions[i].m_iFirstFrame);
			actionNode->setValue(L"LastFrame" ,  actions[i].m_iLastFrame);
			actionNode->setValue(L"nFrame"    ,  actions[i].m_nFrame);
			actionNode->setValue(L"DurTime"   ,  actions[i].m_lTime);
		}
	}
	pactionsstream->stream_seekw(xcdsd_beg,0);

	//write xml file to the stringstream
	xcdstream out(pactionsstream);
	xmlDoc.setXMLStyle(true);
	xmlDoc.save(out);
	out.close();
	comdoc.close_stream(pactionsstream);
	xmlDoc.unload();
	
}

void CActionExporter::export(CSkeletonExporter* pSkeleton, sActionInfos_t& actions, xcomdoc&  comdoc, std::wstring action_dir)
{
    //初始化AlertString列表
    if(m_bAlertStringSet) delete [] m_bAlertStringSet;
    m_bAlertStringSet = new bool[pSkeleton->m_MaxBones.size()];
    for(int i = 0 ; i < (int) pSkeleton->m_MaxBones.size() ; i ++)
    {
        m_bAlertStringSet[i] = true;
    }

    //写入动作列表
    std::wstring action_list_name = action_dir + L"actions.xml";
	writeActionList(actions,comdoc,action_list_name);

	//
	int nBone  = (int)pSkeleton->m_MaxBones.size();

    pSkeleton->setUniform(TRUE);
	//分动作，把所有的Bone的Data写入到文件中
    for(size_t iAction = 0 ; iAction < actions.size() ; ++iAction)
    {
        sActionInfo_t action = actions[iAction];
        if(action.m_ActionType == eActType_Keyframe)
            WriteActionDataInKeyFrame(action, comdoc, action_dir, pSkeleton);
        else
            WriteActionData(action, comdoc, action_dir, pSkeleton);
	}
    pSkeleton->setUniform(FALSE);
}

void CActionExporter::WriteActionData( sActionInfo_t& action , xcomdoc &comdoc, std::wstring action_dir, CSkeletonExporter* pSkeleton ) 
{
    xcomdocstream* pactiondatastream = comdoc.create_stream((action_dir + action.m_Name + L".xra").c_str(),xcddt_common,XCOMDOC_COMPRESS_BLEND);

    if(pactiondatastream == NULL)
    {
        XEVOL_LOG(eXL_DEBUG_HIGH , L"{error}: Action已经存在或者非法: < %s > \n" , (action_dir + action.m_Name).c_str() );
    }

    WriteActionDesc(pSkeleton, pactiondatastream, action);

    //获取信息
    int nFrame     = GetActNumFrame(action);
    int firstFrame = action.m_iFirstFrame;
    int nBone      = (int)pSkeleton->m_MaxBones.size();
    for(int i = 0 ; i < nFrame ; ++i)
    {
        int iFrame = firstFrame + i ;
        for(int iBone = 0 ; iBone < nBone ; ++iBone)
        {
            sBoneData_t  BoneData;
            MakeBoneData(BoneData, (int)iBone , iFrame, pSkeleton);
            pactiondatastream->write( BoneData.m_lTranform );
        }
    }
    comdoc.close_stream(pactiondatastream);	
}

void AddKeyFrame(int iFrame , std::vector<int>& vKeys)
{
    for(size_t i = 0 ; i < vKeys.size() ; i ++)
    {
        if(vKeys[i] == iFrame)
            return ;
    }
    vKeys.push_back(iFrame);
}

void CActionExporter::WriteActionDataInKeyFrame( sActionInfo_t& action , xcomdoc &comdoc, std::wstring action_dir, CSkeletonExporter* pSkeleton )
{
    xcomdocstream* pactiondatastream = comdoc.create_stream((action_dir + action.m_Name + L".xra").c_str(),xcddt_common,XCOMDOC_COMPRESS_BLEND);

    if(pactiondatastream == NULL)
    {
        XEVOL_LOG(eXL_DEBUG_HIGH , L"{error}: Action已经存在或者非法: < %s > \n" , (action_dir + action.m_Name).c_str() );
    }

    WriteActionDesc(pSkeleton, pactiondatastream, action);

    //获取信息
    int nFrame     = GetActNumFrame(action);
    int firstFrame = action.m_iFirstFrame;
    int lastFrame  = firstFrame + nFrame - 1;
    int nBone      = (int)pSkeleton->m_MaxBones.size();

    for(int iBone = 0 ; iBone < nBone ; ++iBone)
    {
        sBoneData_t  BoneData;
        sMaxBoneNode_t bone = pSkeleton->m_MaxBones[iBone];
        bool           hasParentBone = (bone.m_Bone.m_ParentIndex != -1);
        INode* pNode = bone.m_pNode;

        std::vector<int> vKeys;
        AddKeyFrame(firstFrame , vKeys);
        AddKeyFrame(lastFrame  , vKeys);

        //位置
        {
            Control *  pCtrl  = pNode->GetTMController()->GetPositionController();
            IKeyControl *pKeyControl = GetKeyControlInterface(pCtrl);
            int NumKey = pKeyControl->GetNumKeys();
            for(int i = 0 ; i < NumKey ; i ++)
            {
                ILinPoint3Key Key;
                pKeyControl->GetKey(i , &Key);
                TimeValue _time = Key.time;
                int iFrame = _time / GetTicksPerFrame();
                AddKeyFrame(iFrame  , vKeys);
            }
        }

        //旋转
        {
            Control *  pCtrl  = pNode->GetTMController()->GetRotationController();
            IKeyControl *pKeyControl = GetKeyControlInterface(pCtrl);
            int NumKey = pKeyControl->GetNumKeys();
            for(int i = 0 ; i < NumKey ; i ++)
            {
                ILinRotKey Key;
                pKeyControl->GetKey(i , &Key);
                TimeValue _time = Key.time;
                int iFrame = _time / GetTicksPerFrame();
                AddKeyFrame(iFrame  , vKeys);
            }
        }

        //缩放
        {
            Control *  pCtrl  = pNode->GetTMController()->GetScaleController();
            IKeyControl *pKeyControl = GetKeyControlInterface(pCtrl);
            int NumKey = pKeyControl->GetNumKeys();
            for(int i = 0 ; i < NumKey ; i ++)
            {
                ILinScaleKey Key;
                pKeyControl->GetKey(i , &Key);
                TimeValue _time = Key.time;
                int iFrame = _time / GetTicksPerFrame();
                AddKeyFrame(iFrame  , vKeys);
            }
        }

        int32 nKey = (int)vKeys.size();
        pactiondatastream->write( nKey );
        for(int i = 0 ; i < nKey ; ++i)
        {
            int iFrame = vKeys[i];
            sBoneData_t  BoneData;
            MakeBoneData(BoneData, (int)iBone , iFrame, pSkeleton);
            int32 iFrameZoroBase = iFrame - firstFrame;
            pactiondatastream->write( iFrameZoroBase );
            pactiondatastream->write( BoneData.m_lTranform );
        }
        //INode* pNode = MakeBoneData(BoneData, (int)iBone , iFrame, pSkeleton);
        //pactiondatastream->write( BoneData.m_lTranform );
    }
    comdoc.close_stream(pactiondatastream);	
}

void CActionExporter::MakeBoneData(sBoneData_t& BoneData , int iBone , int iFrame, CSkeletonExporter* pSkeleton) 
{
    sMaxBoneNode_t bone = pSkeleton->m_MaxBones[iBone];
    bool           hasParentBone = (bone.m_Bone.m_ParentIndex != -1);
    INode* pNode = bone.m_pNode;
    //pNode->GetTMController()->

    unsigned int iMaxTime = iFrame * GetTicksPerFrame();
    //因为我们在算WeightedVertx的时候乘了InitMT，
    //所以我们这里要把它逆乘回去，好抵消掉
    Matrix3 BoneTM = get_bone_tm(pSkeleton,(int)iBone,iMaxTime);

    //现在开始计算出相对于父节点的相对矩阵
    Matrix3 localTM = get_locale_tm(pNode,hasParentBone,iMaxTime);//Inverse(parentBoneTM) * BoneTM;
    if(hasParentBone == false)
        localTM = pNode->GetNodeTM(iMaxTime);
    Point3 lTrans = localTM.GetTrans();
    localTM.NoTrans();
    //对骨骼进行缩放
    lTrans.x *= pSkeleton->m_fScale;
    lTrans.y *= pSkeleton->m_fScale;
    lTrans.z *= pSkeleton->m_fScale;
    localTM.SetTrans(lTrans);


    //世界空间中的变换矩阵
    BoneData.m_Matrix    = conv_type<sMatrix4x3_t,Matrix3>(BoneTM);
    //局部空间中的变换矩阵
    BoneData.m_LocaleTM  = conv_type<sMatrix4x3_t,Matrix3>(localTM);

    if(CMaxEnv::singleton().m_bInvertYZCoord )
    {
        Matrix3 fMat ;
        fMat = m_FlipYZMat * BoneTM * m_FlipYZMat;
        //世界空间中的变换矩阵
        BoneData.m_Matrix    = conv_type<sMatrix4x3_t,Matrix3>(fMat);
        fMat =  m_FlipYZMat * localTM * m_FlipYZMat;
        BoneData.m_LocaleTM  = conv_type<sMatrix4x3_t,Matrix3>(fMat);
    } 
    //从LocalTM里构造出旋转，平移和缩放矩阵
    makBoneTrans2(pNode , BoneData.m_lTranform , localTM , m_bAlertStringSet[iBone] );
    return ;
}

void  CActionExporter::WriteActionDesc( CSkeletonExporter* pSkeleton, xcomdocstream* pactiondatastream, sActionInfo_t & _action ) 
{
    //写入ID;
    sActionInfo_t action = _action;
    pSkeleton->makeID();
    sSkeletonID_t sid = pSkeleton->getID();
    pactiondatastream->write(sid);

    int nBone      = (int)pSkeleton->m_MaxBones.size();
    pactiondatastream->write(nBone);

    int nFrame   = GetActNumFrame(action);
    int firstFrame = action.m_iFirstFrame;

    action.m_iLastFrame  -= firstFrame;
    action.m_iFirstFrame -= firstFrame;
    action.m_nFrame      = nFrame;
    pactiondatastream->write( (const char*)&action.m_lTime , sizeof(sActionInfo_t) - sizeof(wchar_t) * 32 );
    XEVOL_LOG(eXL_DEBUG_HIGH , L"  动作名字=%s 动作时间=%d 动作帧数=%d \n",action.m_Name,action.m_lTime,nFrame );
}