#include "../xStdPch.h"
#include "xKeyFrameAction.h"
#include "xCoreSkeleton.h"
#include "../BaseLib/xI18N.h"
#include "xCoreMesh.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D 
IMPL_BASE_OBJECT_CLASSID(xKeyFrameAction    , xSkeletonAction);
IMPL_ACTION_FACTORY(xKeyFrameAction);



void xTransKeyFrameTrack::init(int nKeyFrame , const xActionInfo& info)
{
    m_vKeyframs.resize(nKeyFrame);
    m_Info = info;
}

float xTransKeyFrameTrack::getBoneTrans(int lTime , int& iKey1 , int& iKey2 , bool bLoop)
{
    int actTime = m_Info.m_lTime;
    if(actTime <= 0 ) actTime = 1;
    if(bLoop)
    {
        if(lTime < 0)
        {
            int nActionTime = abs(lTime)/(float)actTime + 0.5f;
            lTime += nActionTime * actTime;
        }

        if(lTime > actTime)
        {
            lTime %= actTime;         
        }
    }
    else
    {
        if(lTime < 0      ) lTime = 0;
        if(lTime > actTime) lTime = actTime;
    }

    int nKey = (int)m_vKeyframs.size();
    for(int i = 0 ; i < nKey - 1 ; i ++)
    {
        xTransKeyFrame& Key = m_vKeyframs[i];
        if(Key.m_iTime >= lTime)
        {
            xTransKeyFrame& KeyNext = m_vKeyframs[ i + 1];
            int KeyStepTime = KeyNext.m_iTime - Key.m_iTime;
            iKey1 = i;
            iKey2 = i + 1;
            float t = (lTime - Key.m_iTime)/ (float) KeyStepTime;
            return t;            
        }
    }
    return -1.0f;
}

xTransKeyFrame& xTransKeyFrameTrack::keyFrame(int iKey)
{
    return m_vKeyframs[iKey];
}

xTransKeyFrameTrack::xTransKeyFrameTrack()
{

}


//两个动作之间做插值的过渡动作
xKeyFrameAction::xKeyFrameAction(xCoreSkeleton* pSkeleton , int param) : xSkeletonAction(pSkeleton , param)
{
	m_Info.m_eActType = eActType_Keyframe;
	m_Info.m_iFirstFrame = 0;
	m_Info.m_iLastFrame = 1;
	m_Info.m_lTime = 500; //0.5秒
	m_Info.m_nFrame = 2;
}

xKeyFrameAction::~xKeyFrameAction()
{

}

bool xKeyFrameAction::getBoneTrans(int boneIndex, xBaseActionState* pActionState , xBoneTrans& boneTrans)
{
    float t = pActionState->m_fTime;
    int frame1 = pActionState->m_Frame1;
    int frame2 = pActionState->m_Frame2;
    int lTime = (float)(frame1 + t )* m_Info.m_lTime / (float)m_Info.m_nFrame;

    xTransKeyFrameTrack& BoneTrack = m_vBoneTracks[boneIndex];
    int iKey1 = 0;
    int iKey2 = 0;

    t = BoneTrack.getBoneTrans(lTime , iKey1 , iKey2 , m_bLoopAction);


    xBoneTrans& boneData1 = BoneTrack.keyFrame(iKey1).m_Trans;
    xBoneTrans& boneData2 = BoneTrack.keyFrame(iKey2).m_Trans;

    //插值旋转
    xquat q1 = boneData1.m_Rotate;
    xquat q2 = boneData2.m_Rotate;
    boneTrans.m_Rotate = q1.slerp(t,q2);

    //插值平移
    xvec3& t1 = boneData1.m_Trans;
    xvec3& t2 = boneData2.m_Trans;
    boneTrans.m_Trans = t1*(1.0f - t) + t2*t;

    //插值平移
    xvec3& s1 = boneData1.m_Scale;
    xvec3& s2 = boneData2.m_Scale;
    boneTrans.m_Scale = s1*(1.0f - t) + s2*t;
    return true;
}


bool xKeyFrameAction::blend(long _time_in_ms, xCoreActionFrame* _skeletonFrame , xCoreSkeleton* pSkeleton)
{
    xCoreActionFrame& skeletonFrame = *_skeletonFrame;
    m_ActionState.m_lTime = _time_in_ms;
    updateState(_time_in_ms , m_ActionState , m_bLoopAction );
    pSkeleton->blendSlerp(this, &m_ActionState , attribute() ,  skeletonFrame );
    return true;
}


bool  xKeyFrameAction::load(xXmlNode* pCfgNode )
{
    m_Info.m_lTime       = pCfgNode->int_value(L"DurTime")    ;//"5000"
    m_Info.m_iLastFrame  = pCfgNode->int_value(L"LastFrame")  ;//"77"
    m_Info.m_iFirstFrame = pCfgNode->int_value(L"FirstFrame") ;//"50"
    m_Info.m_nFrame      = pCfgNode->int_value(L"nFrame") ;//"nFrame"
    m_Info.m_eActType    = (eActionType)pCfgNode->int_value(L"ActionType") ;//"1"
    m_Name	     = pCfgNode->value(L"Name");
    m_SkeID.m_HiWord = 0;
    m_SkeID.m_LoWord = 0;
    m_nBone = 0;
    if(m_Info.m_nFrame == 0)
    {
        m_Info.m_nFrame      = m_Info.m_iLastFrame - m_Info.m_iFirstFrame + 1;  
    }
    return true;
}

bool  xKeyFrameAction::load(const wchar_t * _name , istream& stream)
{
    m_Name	     = _name;
    stream.read((char*)&m_SkeID , sizeof(m_SkeID) );
    stream.read((char*)&m_nBone , sizeof(m_nBone) );
    stream.read((char*)&m_Info  , sizeof(m_Info) );
    istream::pos_type cur_pos = stream.tellg();
    stream.seekg(0 , ios::end );
    int len = (int)(stream.tellg() - cur_pos );
    stream.seekg(cur_pos , ios::beg);

    int nFrame = m_Info.m_iLastFrame - m_Info.m_iFirstFrame + 1;
    assert(m_Info.m_nFrame == nFrame );

    m_vBoneTracks.resize(m_nBone);
    m_ActionAttr.init(m_nBone);

    
    for(int i = 0  ; i < m_nBone ; i ++)
    {
        xTransKeyFrameTrack& track = m_vBoneTracks[i];
        int32 nKey = 0;
        stream.read((char*)&nKey , sizeof(nKey) );

        track.init(nKey, m_Info);
        for(int i = 0 ; i < nKey ; i ++)
        {
            xTransKeyFrame& KeyFrame = track.keyFrame(i);
            stream.read((char*)&KeyFrame.m_iFrame , sizeof(KeyFrame.m_iFrame) );
            stream.read((char*)&KeyFrame.m_Trans  , sizeof(KeyFrame.m_Trans)  );
            KeyFrame.m_iTime = ((float)KeyFrame.m_iFrame / m_Info.m_nFrame) * m_Info.m_lTime;
        }
    }
    return true;
}

bool xKeyFrameAction::load(const wchar_t * _name , xcomdocstream* stream)
{
    m_Name	     = _name;
    stream->read(m_SkeID);
    stream->read(m_nBone);
    stream->read(m_Info);

    int cur_pos = stream->stream_tellr();
    stream->stream_seekr(xcdsd_end , 0 );
    int len = (int)(stream->stream_tellr() - cur_pos );
    stream->stream_seekr(xcdsd_beg , cur_pos );

    int nFrame = m_Info.m_iLastFrame - m_Info.m_iFirstFrame + 1;
    assert(m_Info.m_nFrame == nFrame );

    m_vBoneTracks.resize(m_nBone);
    m_ActionAttr.init(m_nBone);


    for(int i = 0  ; i < m_nBone ; i ++)
    {
        xTransKeyFrameTrack& track = m_vBoneTracks[i];
        int32 nKey = 0;
        stream->read(nKey );

        track.init(nKey, m_Info);
        for(int i = 0 ; i < nKey ; i ++)
        {
            xTransKeyFrame& KeyFrame = track.keyFrame(i);
            stream->read(KeyFrame.m_iFrame  );
            stream->read(KeyFrame.m_Trans   );
            KeyFrame.m_iTime = ((float)KeyFrame.m_iFrame / m_Info.m_nFrame) * m_Info.m_lTime;
        }
    }
    return true;
}
bool xKeyFrameAction::load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir)
{
    std::ds_wstring fullName = std::ds_wstring(actionDir) + pCfgNode->name() + L".xra";
    m_Info.m_lTime       = pCfgNode->int_value(L"DurTime")    ;//"5000"
    m_Info.m_iLastFrame  = pCfgNode->int_value(L"LastFrame")  ;//"77"
    m_Info.m_iFirstFrame = pCfgNode->int_value(L"FirstFrame") ;//"50"
    m_Info.m_nFrame      = pCfgNode->int_value(L"nFrame") ;//"nFrame"
    m_Info.m_eActType    = (eActionType)pCfgNode->int_value(L"ActionType") ;//"1"

    xcomdocstream* stream =doc.open_stream(fullName.c_str() , true);
    if(stream == NULL)
    {
        //找没扩展名的
        fullName = std::ds_wstring(actionDir) + pCfgNode->name();
        stream = doc.open_stream(fullName.c_str() , true);
    }

    if(stream == NULL)
        return false;

    load(pCfgNode->value(L"Name") , stream );
    doc.close_stream(stream);
    return true;
}



void xKeyFrameAction::unload()
{
    m_vBoneTracks.clear();
}


END_NAMESPACE_XEVOL3D
