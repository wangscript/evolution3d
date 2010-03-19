#include "../xStdPch.h"
#include "xCoreSkeleton.h"
#include "../xcomdoc/xdocfstream.h"
#include "../BaseLib/xI18N.h"
#include "xCoreMesh.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xCoreSkeleton  , IBaseObject);
//骨架层次类
xBoneHiberarchy::xBoneHiberarchy()
{
    m_nChildren = 0;
}
xBoneHiberarchy::~xBoneHiberarchy()
{
    m_nChildren = 0 ;
    m_Childrens.clear();
}
void xBoneHiberarchy::clear()
{
    m_nChildren = 0 ;
    m_Childrens.clear();
}
bool xBoneHiberarchy::add_child(int index )
{
	size_t _nChild = m_Childrens.size() ;
    for(size_t i = 0 ; i < _nChild ;  ++i)
    {
        if(m_Childrens[i] == index)
            return false;
    }
    m_Childrens.push_back(index);
    m_nChildren ++;
    return true;
}
void xBoneHiberarchy::push_back(int index )
{
    m_Childrens.push_back(index);
    m_nChildren ++;   
}

//骨架类
xCoreSkeleton::xCoreSkeleton()
{
    m_RefCount   = 1;
}

int  xCoreSkeleton::findBoneIndex(const wchar_t* boneName)
{
	if(boneName == NULL)
		return 0;
	std::wstring _boneName = boneName;
	for(int i = 0 ; i < m_nBone ; i ++)
	{
		if(m_Bones[i].m_BoneName == _boneName)
		{
			return i;
		}
	}
	return 0;
}

xCoreSkeleton::~xCoreSkeleton()
{
    unload();
}

bool xCoreSkeleton::unload()
{
    m_RootHibers.clear();
    m_BoneHibers.clear();

    //骨架信息
    m_nBone = 0;
    m_Bones.clear();
    return true;
}

bool xCoreSkeleton::load(xcomdoc& doc,const wchar_t* skeleton_dir)
{
    //Load skeleton
    {
        std::ds_wstring _name = std::ds_wstring(skeleton_dir ) + L"skeleton/skeleton";
        xcomdocstream* boneinfo_stream = doc.open_stream(_name.c_str(),true);
        if(boneinfo_stream == NULL)
            return false;
        xcdstream _in(boneinfo_stream);
        bool bRet = loadBoneList(_in);
        _in.close();
        doc.close_stream(boneinfo_stream);	
        if(bRet == false)
            return false;
    }

    //加载骨架信息
    {
        std::ds_wstring _name = std::ds_wstring(skeleton_dir ) + L"skeleton/hiberarchy";
        xcomdocstream* actdata_hiberarchy = doc.open_stream(_name.c_str(),true);

        if(actdata_hiberarchy == NULL)
            return false;

        xcdstream _in(actdata_hiberarchy);
        bool bRet = loadHiberarchy(_in);
        _in.close();
        doc.close_stream(actdata_hiberarchy);		
        if(bRet == false)
            return false;

        _name = std::ds_wstring(skeleton_dir ) + L"skeleton/hiberarchy.xml";
        xcomdocstream* actdata_hiberarchy_xml = doc.open_stream(_name.c_str(),true);
        if(actdata_hiberarchy_xml == NULL)
        {
            actdata_hiberarchy_xml = doc.open_stream(_name.c_str(),false);
            if(actdata_hiberarchy_xml)
            {
                saveHiberarchy(actdata_hiberarchy_xml);
                doc.close_stream(actdata_hiberarchy_xml);	
            }

        }
    }

    return true;
}

bool xCoreSkeleton::loadHiberarchy(std::istream& _in)
{
    m_BoneHibers.clear();
    int32 nRootNode = -1;
    xSkeletonID id ;
    Ts_Read(_in , id);
    Ts_Read(_in,nRootNode );
    for(int i = 0 ; i < nRootNode ; ++i)
    {
        int child = - 1;
        _in.read( (char*)&child , sizeof(child) );

        if(child == -1)
            return false;

        m_RootHibers.add_child(child);
    }
    int nBone = -1;
    Ts_Read(_in,nBone);
    m_BoneHibers.resize(nBone);
    for(int i = 0 ; i < nBone ; ++i)
    {

        xBoneHiberarchy& boneH = m_BoneHibers[i];
        int nChild = 0;

        Ts_Read(_in, nChild );
        for(int j = 0 ; j < nChild ; ++j)
        {
            int child = - 1;
            Ts_Read(_in,child);

            if(child == -1)
                return false;

            boneH.add_child(child);
        }
    }
    return true;
}

xBoneHiberarchy&  xCoreSkeleton::boneHiberarchy(int iBone)
{
	return m_BoneHibers[iBone];
}

bool xCoreSkeleton::saveHiberarchy(xcomdocstream* boneinfo_stream)
{
    xXmlDocument _xml;
    xcdstream _in(boneinfo_stream);

    int nBone = (int) m_Bones.size();
    xXmlNode* pRootNode = _xml.insertNode(L"hiberarchy");
    pRootNode->setValue(L"nBone" , nBone);
    pRootNode->setValue(L"scale" , 1.0f);
    pRootNode->setValue(L"beforeSkelPose" , false );

    //骨架类
    xXmlNode* pIDNode = pRootNode->insertNode(L"id");
    pIDNode->setHex(L"LoID" , m_SkeletonID.m_LoWord);
    pIDNode->setHex(L"HiID" , m_SkeletonID.m_HiWord);

	size_t _nChildres = m_RootHibers.m_Childrens.size() ;
    for(size_t i = 0 ; i <  _nChildres ; i ++)
    {
        int index = m_RootHibers.m_Childrens[i];
        xXmlNode* BoneNode = pRootNode->insertNode(L"bone");
        BoneNode->setValue(L"name"  , m_Bones[index].m_BoneName );
        BoneNode->setValue(L"index" , index);
        saveHiberarchy(BoneNode , index);
    }
    _xml.setXMLStyle(true);
    _xml.save(_in);
    _in.close();
    return true;
}

bool xCoreSkeleton::saveHiberarchy(xXmlNode* pRootNode , int rootindex)
{
    xBoneHiberarchy& boneH = m_BoneHibers[rootindex];
    int nChild = (int)boneH.m_Childrens.size();
    for(int j = 0 ; j < nChild ; ++j)
    {
        int index = boneH.m_Childrens[j];
        xXmlNode* BoneNode = pRootNode->insertNode(L"bone");
        BoneNode->setValue(L"name"  , m_Bones[index].m_BoneName );
        BoneNode->setValue(L"index" , index);
        saveHiberarchy(BoneNode , index);
    }
    return true;
}

bool xCoreSkeleton::loadBoneList(std::istream& _in)
{
    int nBone = -1;
    Ts_Read(_in , nBone);
    if(nBone <= 0)
        return false;



    //首次读入Skeleton的ID.以后每次加载数据的时候要和这个ID进行验证
    //需要验证的数据是和Skeleton有绑定关系的数据。
    //主要数据如下: 一): 动作数据 二) 
    Ts_Read(_in , m_SkeletonID);
    bool bUseBeforeSkeletonPose;
    Ts_Read(_in , bUseBeforeSkeletonPose);


    for(int i = 0 ; i < nBone ; i ++)
    {
        xSkinBone bone;
        //判断wchar_t的长度
        if(sizeof(wchar_t) == 2)
        {
            Ts_Read(_in , bone);
        }
        else
        {
            unsigned short _name[32];
            Ts_Read(_in , _name, (std::streamsize)32);
            XEvol_UCS2ToUCS4(_name, (unsigned int*)bone.m_BoneName , 32 , 32);

            size_t Len = sizeof(xSkinBone) - sizeof(wchar_t) * 32;
            Ts_Read(_in , (char* )&bone.m_UC4AddStart , (std::streamsize)Len );

        }
        m_Bones.push_back(bone);
    }
    m_nBone = (int)m_Bones.size();
    float fScale = 0.0f;
    Ts_Read(_in , fScale);
    return true;
}

bool xCoreSkeleton::blendLerp(xBaseAction* pAction, xCoreActionAttr* pAttr ,xCoreActionFrame& skeletonFrame,float t, int frame1, int frame2)
{
    for(int boneIndex = 0 ; boneIndex < m_nBone ; boneIndex++)
    {
        xSkinBone* pBone = &m_Bones[boneIndex];
        xBoneData* bone1 = pAction->getBoneData(boneIndex,frame1);
        xBoneData* bone2 = pAction->getBoneData(boneIndex,frame2);
		if(bone1 == NULL || bone2 == NULL || pAttr->boneUsed(boneIndex) == false )
		{
			continue;
		}

		xmat4& mat = skeletonFrame.m_FrameMat[boneIndex];
        XM_Lerp(bone1->m_Matrix , bone2->m_Matrix , t , mat);
    }
    return true;
}

bool xCoreSkeleton::blendHiberarchy(xBaseAction* pAction, xCoreActionAttr* pAttr , xCoreActionFrame& skeletonFrame,float t, int frame1, int frame2,int parent)
{
    xmat4 parentMT;
    xmat4 parentMTInv;
    xBoneHiberarchy* pBoneH = NULL;
    if(parent != -1)
    {
        parentMT = skeletonFrame.m_FrameMat[parent];
        pBoneH = &m_BoneHibers[parent];
    }
    else
    {
        parentMT.identity();
        pBoneH = &m_RootHibers;

    }
    xmat4 localeMT;
    xBoneHiberarchy& boneH = *pBoneH;
    for(int i = 0 ; i < boneH.m_nChildren; ++i)
    {
        int      boneIndex = boneH.m_Childrens[i];
        xSkinBone* pBone = &m_Bones[boneIndex];

        xBoneData* boneData1 = pAction->getBoneData(boneIndex,frame1);
        xBoneData* boneData2 = pAction->getBoneData(boneIndex,frame2);
		if(boneData1 == NULL || boneData2 == NULL || pAttr->boneUsed(boneIndex) == false )
		{
			blendHiberarchy(pAction,pAttr , skeletonFrame,t,frame1,frame2,boneIndex);
			continue;
		}

        XM_Lerp(boneData1->m_LocaleTM , boneData2->m_LocaleTM , t , localeMT);

        xmat4& transMat   = skeletonFrame.m_FrameMat[boneIndex];
        xmat4& wsMat      = skeletonFrame.m_WSMats[boneIndex];
        //      mat = boneData1.m_Matrix;
        //XM_Lerp(boneData1.m_Matrix , boneData2.m_Matrix , t , mat);

        if(parent == -1)
        {
            wsMat      = localeMT;
            XM_Mul(pBone->m_InitMTInv , wsMat , transMat);
            //XM_Lerp(boneData1.m_Matrix , boneData2.m_Matrix , t , transMat);
            //XM_Mul(pBone->m_InitMT , transMat , wsMat);
        }
        else
        {
            /*
            xmat4 mat1 ,mat2;            
            XM_Mul(pBone->m_InitMTInv , localeMT, mat1);
            XM_Mul(mat1 , m_Bones[parent].m_InitMT , mat2);
            XM_Mul(mat2 , parentMT , transMat);
            //mat = pBone->m_InitMTInv * localeMT * m_Bones[parent]->m_InitMT * parentMT;
            */

            //优化
            XM_Mul(localeMT , skeletonFrame.m_WSMats[parent] , wsMat);
            XM_Mul(pBone->m_InitMTInv , wsMat , transMat);

            int a  = 0;
            //mat = pBone->m_InitMTInv * wsMat --> wsMat = localeMT * parentWsMT;

        }

        blendHiberarchy(pAction,pAttr , skeletonFrame,t,frame1,frame2,boneIndex);
    }
    return true;
}

void xBoneTrans::toMatrix(xMathLib::xmat4& mat)
{
    //暂时忽略了缩放
    xMathLib::xmat4 scaleMat;
    xMathLib::xmat4 rotMat;
    m_Rotate.toMatrix(rotMat);
    scaleMat.identity();
    scaleMat.scale(m_Scale.x , m_Scale.y , m_Scale.y);
    XM_Mul(scaleMat , rotMat  , mat);
    
    //m_Rotate.toMatrix(mat);
    //mat.scale(this->m_Scale.x , m_Scale.y , m_Scale.y );

    mat.setTransform(m_Trans.x , m_Trans.y , m_Trans.z);
    return ;
}


bool makeLocaleTM(xBoneTrans& boneTrans , xmat4& mat , xBoneData& boneData1 , xBoneData& boneData2 , float t)
{
    //插值旋转
    xquat q1 = boneData1.m_BoneTrans.m_Rotate;
    xquat q2 = boneData2.m_BoneTrans.m_Rotate;
    boneTrans.m_Rotate = q1.slerp(t,q2);

    //插值平移
    xvec3& t1 = boneData1.m_BoneTrans.m_Trans;
    xvec3& t2 = boneData2.m_BoneTrans.m_Trans;
    boneTrans.m_Trans = t1*(1.0f - t) + t2*t;

    //插值平移
    xvec3& s1 = boneData1.m_BoneTrans.m_Scale;
    xvec3& s2 = boneData2.m_BoneTrans.m_Scale;
    boneTrans.m_Scale = s1*(1.0f - t) + s2*t;

    boneTrans.toMatrix(mat);
    return true;
}

bool makeTransData(xBoneData& boneData)
{
     xmat4 wsMat = boneData.m_LocaleTM ; //pBone->m_InitMT * boneData.m_Matrix;
     xeuler _euler;
     _euler.fromMatrix(wsMat);
     xquat quat ;
     quat.fromEuler(_euler);
     boneData.m_BoneTrans.m_Rotate.fromEuler(_euler);
     boneData.m_BoneTrans.m_Scale = boneData.m_BoneTrans.m_Scale;
     boneData.m_BoneTrans.m_Trans.x = wsMat.m30;
     boneData.m_BoneTrans.m_Trans.y = wsMat.m31;
     boneData.m_BoneTrans.m_Trans.z = wsMat.m32;
     return true;
}

bool xCoreSkeleton::blendSlerp(xBaseAction* pAction,xCoreActionAttr* pAttr ,xCoreActionFrame& skeletonFrame,float t, int frame1, int frame2,int parent)
{
    xmat4 parentMT;
    xmat4 parentMTInv;
    xBoneHiberarchy* pBoneH = NULL;
    if(parent != -1)
    {
        parentMT = skeletonFrame.m_FrameMat[parent];
        pBoneH = &m_BoneHibers[parent];
    }
    else
    {
        parentMT.identity();
        pBoneH = &m_RootHibers;

    }
    xmat4 localeMT;
    xBoneHiberarchy& boneH = *pBoneH;
    for(int i = 0 ; i < boneH.m_nChildren; ++i)
    {
        int     boneIndex = boneH.m_Childrens[i];
        xSkinBone*  pBone = &m_Bones[boneIndex];

        xBoneData* boneData1 = pAction->getBoneData(boneIndex,frame1);
        xBoneData* boneData2 = pAction->getBoneData(boneIndex,frame2);


		//骨骼不属于这个动作
        if(boneData1 == NULL || boneData2 == NULL || pAttr->boneUsed(boneIndex) == false )
		{
			blendSlerp(pAction, pAttr , skeletonFrame , t,frame1,frame2,boneIndex);
			continue;
		}

        xmat4& transMat       = skeletonFrame.m_FrameMat[boneIndex];
        xmat4& wsMat          = skeletonFrame.m_WSMats[boneIndex];
        xBoneTrans& boneTrans = skeletonFrame.boneTrans(boneIndex);

        makeLocaleTM(boneTrans , localeMT, *boneData1 , *boneData2 , t);

        //mat = boneData1.m_Matrix;
        //XM_Lerp(boneData1.m_Matrix , boneData2.m_Matrix , t , mat);

        if(parent == -1)
        {
            //使用记录的变量插值
            wsMat      = localeMT;
            XM_Mul(pBone->m_InitMTInv , localeMT , transMat);
            
            //直接使用夫节点
            //XM_Lerp(boneData1.m_Matrix , boneData2.m_Matrix , t , transMat);
            //XM_Mul(pBone->m_InitMT , transMat , wsMat);
           
        }
        else
        {
            /*
            xmat4 mat1 ,mat2;
            XM_Mul(pBone->m_InitMTInv , localeMT, mat1);
            XM_Mul(mat1 , m_Bones[parent].m_InitMT , mat2);
            XM_Mul(mat2 , parentMT , transMat);
            //因为顶点已经带有了 pBone->m_InitMT。 所以要先乘上 pBone->m_InitMTInv 。
            //另外，夫节点上带了 pBone->m_InitMTInv。 所以在parentMT前要乘上m_Bones[parent]->m_InitMT
            //mat = pBone->m_InitMTInv * localeMT * m_Bones[parent]->m_InitMT * parentMT;
            */

            //优化
            XM_Mul(localeMT , skeletonFrame.m_WSMats[parent] , wsMat);
            XM_Mul(pBone->m_InitMTInv , wsMat , transMat);
            //mat = pBone->m_InitMTInv * wsMat --> wsMat = localeMT * parentWsMT;

        }
        blendSlerp(pAction, pAttr , skeletonFrame , t,frame1,frame2,boneIndex);
    }
    return true;
}

bool xCoreSkeleton::blendSlerp(xBaseAction* pAction, xBaseActionState* _pInputActionState ,xCoreActionFrame& skeletonFrame , int parent)
{
    xmat4 parentMT;
    xmat4 parentMTInv;
    xBoneHiberarchy* pBoneH = NULL;
    if(parent != -1)
    {
        parentMT = skeletonFrame.m_FrameMat[parent];
        pBoneH = &m_BoneHibers[parent];
    }
    else
    {
        parentMT.identity();
        pBoneH = &m_RootHibers;

    }
    xmat4 localeMT;
    float tTime = 0.0f;
    xBoneHiberarchy& boneH = *pBoneH;
    for(int i = 0 ; i < boneH.m_nChildren; ++i)
    {
        int     boneIndex = boneH.m_Childrens[i];
        xSkinBone*  pBone = &m_Bones[boneIndex];


        xBaseActionState* pActionState = pAction->getActionState(boneIndex , _pInputActionState);

        int frame1 = pActionState->m_Frame1;
        int frame2 = pActionState->m_Frame2;
        xBoneData* boneData1 = pAction->getBoneData(boneIndex, frame1);
        xBoneData* boneData2 = pAction->getBoneData(boneIndex, frame2);

        xmat4& transMat       = skeletonFrame.m_FrameMat[boneIndex];
        xmat4& wsMat          = skeletonFrame.m_WSMats[boneIndex];
        xBoneTrans& boneTrans = skeletonFrame.boneTrans(boneIndex);

        makeLocaleTM(boneTrans , localeMT, *boneData1 , *boneData2 , pActionState->m_fTime);

        //      mat = boneData1.m_Matrix;
        //XM_Lerp(boneData1.m_Matrix , boneData2.m_Matrix , t , mat);

        if(parent == -1)
        {
            //使用记录的变量插值
            wsMat      = localeMT;
            XM_Mul(pBone->m_InitMTInv , localeMT , transMat);
        }
        else
        {
            //优化
            XM_Mul(localeMT , skeletonFrame.m_WSMats[parent] , wsMat);
            XM_Mul(pBone->m_InitMTInv , wsMat , transMat);
        }
        blendSlerp(pAction, _pInputActionState , skeletonFrame ,boneIndex);
    }
    return true;
}

END_NAMESPACE_XEVOL3D
