#include "xCoreSkeleton.h"
#include "../xcomdoc/xdocfstream.h"
#include "../BaseLib/xI18N.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xCoreAction    , IBaseObject);
IMPL_BASE_OBJECT_CLASSID(xCoreSkeleton  , IBaseObject);
//骨架的一个Frame
xCoreActionFrame::xCoreActionFrame()
{
    m_nBone = 0;
    m_FrameMat = NULL;
}

xCoreActionFrame::~xCoreActionFrame()
{
    unload();
}

int    xCoreActionFrame::nBone()
{
    return m_nBone;
}

xmat4& xCoreActionFrame::operator[](int index)
{
    assert(index < m_nBone);
    assert(index >= 0);
    return m_FrameMat[index];
}

bool xCoreActionFrame::load(int nBone)
{
    unload();
    m_FrameMat = new xmat4[nBone];
    assert(m_FrameMat);
    m_nBone = nBone;
    return m_FrameMat != NULL;
}

void xCoreActionFrame::unload()
{
    if(m_FrameMat)
        delete m_FrameMat;
    m_FrameMat = NULL;
}

xCoreAction::xCoreAction()
{
    m_RefCount = 1;
}
xCoreAction::~xCoreAction()
{

}

bool xCoreAction::blend(long _time_in_ms, xCoreActionFrame& skeletonFrame , xCoreSkeleton* pSkeleton)
{
    long t = _time_in_ms % m_Info.m_lTime;

    int nFrame = m_Info.m_iLastFrame - m_Info.m_iFirstFrame;
    float fFrame = nFrame *  t / float(m_Info.m_lTime); 

    int frame1 = (int)fFrame;
    int frame2 = frame1 + 1;
    if(frame2 >= nFrame)
    {
        frame2 = frame2%nFrame;
    }
    if(frame1 > nFrame)
    {
        frame1 = frame1%nFrame;
    }
    float tfloat = fFrame - frame1;
    pSkeleton->blendSlerp(this,skeletonFrame,tfloat,frame1,frame2);
    return true;
}

xBoneData& xCoreAction::getBoneData(int boneIndex, int frame)
{
    return m_BoneFrame[boneIndex][frame];
}


bool xCoreAction::load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir)
{
    std::ds_wstring fullName = std::ds_wstring(actionDir) + pCfgNode->name();
    m_Info.m_lTime       = pCfgNode->int_value(L"DurTime")    ;//"5000"
    m_Info.m_iLastFrame  = pCfgNode->int_value(L"LastFrame")  ;//"77"
    m_Info.m_iFirstFrame = pCfgNode->int_value(L"FirstFrame") ;//"50"
    m_Info.m_nFrame      = pCfgNode->int_value(L"nFrame") ;//"nFrame"
    m_Info.m_eActType    = (eActionType)pCfgNode->int_value(L"ActionType") ;//"1"

    m_Name	     = pCfgNode->value(L"Name");
    xcomdocstream* stream =doc.open_stream(fullName.c_str() , true);
    if(stream == NULL)
        return false;

    stream->read(m_SkeID);
    stream->read(m_nBone);
    stream->read(m_Info);

    int nFrame = m_Info.m_iLastFrame - m_Info.m_iFirstFrame + 1;

    int dim[2];
    dim[0] = m_nBone;
    dim[1] = nFrame;

    m_BoneFrame.create(dim);
    for(int iFrame = 0 ; iFrame < nFrame ; iFrame ++ )
    {
        for(int iBone = 0 ; iBone < m_nBone ; iBone++)
        {

            xBoneData& boneData = m_BoneFrame[iBone][iFrame];
            if(stream->read(boneData) < sizeof(boneData) )
            {
                doc.close_stream(stream);
                return false;
            }
            else
            {
                continue;
            }
        }
    }
    doc.close_stream(stream);


    /*
    //write;
    fullName = fullName + L".new";
    xcomdocstream* newstream =doc.open_stream(fullName.c_str() ,false);
    if(newstream == NULL)
    return false;

    newstream->write(m_SkeID);
    newstream->write(m_nBone);
    newstream->write(m_Info);
    for(int iBone = 0 ; iBone < m_nBone ; iBone++)
    {
    for(int iFrame = 0 ; iFrame <= m_Info.m_iLastFrame ; iFrame ++ )
    {
    xBoneData& boneData = m_BoneFrame[iBone][iFrame];
    newstream->write(boneData);
    }
    }
    doc.close_stream(newstream);
    */
    return true;
}

void xCoreAction::unload()
{
    m_BoneFrame.free();
}

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
    for(size_t i = 0 ; i < m_Childrens.size() ; ++i)
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
    int nRootNode = -1;
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

    for(size_t i = 0 ; i < m_RootHibers.m_Childrens.size() ; i ++)
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

bool xCoreSkeleton::blendLerp(xCoreAction* pAction,xCoreActionFrame& skeletonFrame,float t, int frame1, int frame2)
{
    for(int boneIndex = 0 ; boneIndex < m_nBone ; boneIndex++)
    {
        xSkinBone* pBone = &m_Bones[boneIndex];
        xBoneData& bone1 = pAction->getBoneData(boneIndex,frame1);
        xBoneData& bone2 = pAction->getBoneData(boneIndex,frame2);
        xmat4& mat = skeletonFrame.m_FrameMat[boneIndex];
        XM_Lerp(bone1.m_Matrix , bone2.m_Matrix , t , mat);
    }
    return true;
}

bool xCoreSkeleton::blendHiberarchy(xCoreAction* pAction,xCoreActionFrame& skeletonFrame,float t, int frame1, int frame2,int parent)
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

        xBoneData& boneData1 = pAction->getBoneData(boneIndex,frame1);
        xBoneData& boneData2 = pAction->getBoneData(boneIndex,frame2);

        XM_Lerp(boneData1.m_LocaleTM , boneData2.m_LocaleTM , t , localeMT);

        xmat4& mat = skeletonFrame.m_FrameMat[boneIndex];
        //      mat = boneData1.m_Matrix;
        //XM_Lerp(boneData1.m_Matrix , boneData2.m_Matrix , t , mat);

        if(parent == -1)
        {
            mat = pBone->m_InitMTInv * localeMT;
        }
        else
        {
            xmat4 mat1 ,mat2;
            XM_Mul(pBone->m_InitMTInv , localeMT, mat1);
            XM_Mul(mat1 , m_Bones[parent].m_InitMT , mat2);
            XM_Mul(mat2 , parentMT , mat);
            //mat = pBone->m_InitMTInv * localeMT * m_Bones[parent]->m_InitMT * parentMT;
        }

        blendHiberarchy(pAction,skeletonFrame,t,frame1,frame2,boneIndex);
    }
    return true;
}

bool makeLocaleTM(xmat4& mat , xBoneData& boneData1 , xBoneData& boneData2 , float t)
{
    //插值旋转
    xquat q1 = boneData1.m_BoneTrans.m_Rotate;
    xquat q2 = boneData2.m_BoneTrans.m_Rotate;
    xquat q = q1.slerp(t,q2);
    q.toMatrix(mat);

    //插值平移
    xvec3& t1 = boneData1.m_BoneTrans.m_Trans;
    xvec3& t2 = boneData2.m_BoneTrans.m_Trans;
    xvec3 tt = t1*(1.0f - t) + t2*t;

    mat.setTransform(tt.x,tt.y,tt.z);
    return true;
}

bool xCoreSkeleton::blendSlerp(xCoreAction* pAction,xCoreActionFrame& skeletonFrame,float t, int frame1, int frame2,int parent)
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

        xBoneData& boneData1 = pAction->getBoneData(boneIndex,frame1);
        xBoneData& boneData2 = pAction->getBoneData(boneIndex,frame2);

        makeLocaleTM(localeMT, boneData1 , boneData2 , t);

        xmat4& mat = skeletonFrame.m_FrameMat[boneIndex];
        if(parent == -1)
        {
            XM_Lerp(boneData1.m_LocaleTM , boneData2.m_LocaleTM , t , localeMT);
            mat = pBone->m_InitMTInv * localeMT;
        }
        else
        {
            xmat4 mat1 ,mat2;
            XM_Mul(pBone->m_InitMTInv , localeMT, mat1);
            XM_Mul(mat1 , m_Bones[parent].m_InitMT , mat2);
            XM_Mul(mat2 , parentMT , mat);
            //mat = pBone->m_InitMTInv * localeMT * m_Bones[parent]->m_InitMT * parentMT;
        }
        blendSlerp(pAction, skeletonFrame , t,frame1,frame2,boneIndex);
    }
    return true;
}


END_NAMESPACE_XEVOL3D
