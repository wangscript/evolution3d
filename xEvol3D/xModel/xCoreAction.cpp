#include "../xStdPch.h"
#include "xCoreAction.h"
#include "xCoreSkeleton.h"
#include "../xcomdoc/xdocfstream.h"
#include "../BaseLib/xI18N.h"
#include "xCoreMesh.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D 
IMPL_BASE_OBJECT_CLASSID(xBaseAction          , IBaseObject);
IMPL_BASE_OBJECT_CLASSID(xCoreAction          , xBaseAction);
IMPL_BASE_OBJECT_CLASSID(xEmbAction           , xCoreAction);
//骨架的一个Frame
xCoreActionFrame::xCoreActionFrame()
{
    m_nBone = 0;
    m_FrameMat   = NULL;
    m_WSMats     = NULL;
    m_vBoneTrans = NULL;
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

xMathLib::xmat4&  xCoreActionFrame::wsMat(int index)
{
    assert(index < m_nBone);
    assert(index >= 0);
    return m_WSMats[index];
}

xBoneTrans&  xCoreActionFrame::boneTrans(int index)
{
    assert(index < m_nBone);
    assert(index >= 0);
    return m_vBoneTrans[index];
}

bool xCoreActionFrame::load(int _nBone)
{
    unload();
    m_FrameMat   = new xMathLib::xmat4[_nBone];
    m_WSMats     = new xMathLib::xmat4[_nBone];
    m_vBoneTrans = new xBoneTrans[_nBone];
    assert(m_FrameMat);
    m_nBone = _nBone;
    return m_FrameMat != NULL;
}

void xCoreActionFrame::unload()
{
    XSAFE_DELETE_ARRAY(m_FrameMat);
    XSAFE_DELETE_ARRAY(m_WSMats);
    XSAFE_DELETE_ARRAY(m_vBoneTrans);
}

//////////////////////////////////////////////////////////////////////////
void xCoreActionAttr::init(int nBone , int Attri )
{
	m_BoneAttribtes.resize(nBone);
	for(int i = 0 ; i < nBone ; i ++)
	{
		m_BoneAttribtes[i] = Attri;
	}
}

bool xCoreActionAttr::boneUsed(int boneIndex)
{
	return m_BoneAttribtes[boneIndex] == eBoneInclude;
}

void xCoreActionAttr::setBoneAttribute(int attr )
{
	size_t nBone = m_BoneAttribtes.size();
	for(size_t i = 0 ; i < nBone ; i ++) m_BoneAttribtes[i] = attr;
}

void xCoreActionAttr::setBoneAttribute(int attr , int idx)
{
	m_BoneAttribtes[idx] = attr;
}

void xCoreActionAttr::setBoneAttribute(int* pAttributes , int nAttr , int iStart)
{
	for(int i = iStart ; i < iStart + nAttr ; i ++)
	{
		m_BoneAttribtes[i] = pAttributes[i - iStart];
	}
}

//////////////////////////////////////////////////////////////////////////
float xBaseAction::_getBlendFrameLoop(const xActionInfo* pInfo ,long _time_in_ms, int& frame1 , int& frame2)
{
    long t = _time_in_ms % pInfo->m_lTime;

    int nFrame = pInfo->m_iLastFrame - pInfo->m_iFirstFrame + 1;
    float fFrame = nFrame *  t / float( pInfo->m_lTime); 

    frame1 = (int)fFrame;
    frame2 = frame1 + 1;
    if(frame2 >= nFrame)
    {
        frame2 = frame2%nFrame;
    }
    if(frame1 > nFrame)
    {
        frame1 = frame1%nFrame;
    }
    return fFrame - frame1;
}

float xBaseAction::_getBlendFrameClamp(const xActionInfo* pInfo ,long _time_in_ms, int& frame1 , int& frame2)
{
    if(_time_in_ms > pInfo->m_lTime) _time_in_ms = pInfo->m_lTime;
    long t = _time_in_ms ;//tt % m_Info.m_lTime;

    int nFrame = pInfo->m_iLastFrame - pInfo->m_iFirstFrame + 1;
    float lTime = float( pInfo->m_lTime); 
    if(lTime == 0.0f ) lTime = 30.0f;
    float fFrame = nFrame *  t / float( pInfo->m_lTime); 

    frame1 = (int)fFrame;
    frame2 = frame1 + 1;
    if(frame2 >= nFrame)
    {
        frame2 = nFrame - 1;//frame2%nFrame;
    }
    if(frame1 >= nFrame)
    {
        frame1 = nFrame - 1; //frame1%nFrame;
    }
    return fFrame - frame1;
}

//////////////////////////////////////////////////////////////////////////
xCoreAction::xCoreAction()
{
    m_RefCount = 1;
	m_bLoopAction = true;
}
xCoreAction::~xCoreAction()
{

}

bool xCoreAction::setTime(float _time)
{
	m_Info.m_lTime = 1000 * _time;
	return true;
}

float xCoreAction::getDurTime()
{
    return m_Info.m_lTime/1000.0f;
}

const wchar_t* xCoreAction::typeID() const
{
	return L"xra";
}

bool xCoreAction::blend(long _time_in_ms, xCoreActionFrame* _skeletonFrame , xCoreSkeleton* pSkeleton)
{
	xCoreActionFrame& skeletonFrame = *_skeletonFrame;
    int frame1;
    int frame2;
	float tfloat = 0.0f;
    if(m_bLoopAction)
	{
		tfloat = _getBlendFrameLoop(info() , _time_in_ms , frame1 , frame2);
	}
	else
	{
		tfloat = _getBlendFrameClamp(info() ,_time_in_ms , frame1 , frame2);
	}
    pSkeleton->blendSlerp(this,skeletonFrame,tfloat,frame1,frame2);
    return true;
}

int xCoreAction::memUsage()
{
	return m_nBone * m_Info.m_nFrame * sizeof(xBoneData);
}

xBoneData* xCoreAction::getBoneData(int boneIndex, int frame)
{
	if(frame == -1 || frame >= m_Info.m_nFrame) 
		frame = m_Info.m_nFrame - 1;
	return &m_BoneFrame[boneIndex][frame];
}

bool  xCoreAction::load(xXmlNode* pCfgNode )
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

bool  xCoreAction::load(const wchar_t * _name , istream& stream)
{
	m_Name	     = _name;
	stream.read((char*)&m_SkeID , sizeof(m_SkeID) );
	stream.read((char*)&m_nBone , sizeof(m_nBone) );
	stream.read((char*)&m_Info  , sizeof(m_Info) );

	int nFrame = m_Info.m_iLastFrame - m_Info.m_iFirstFrame + 1;
	assert(m_Info.m_nFrame == nFrame );
	
	int dim[2];
	dim[0] = m_nBone;
	dim[1] = m_Info.m_nFrame;

	m_BoneFrame.create(dim);
	m_ActionAttr.init(m_nBone);
	for(int iFrame = 0 ; iFrame < nFrame ; iFrame ++ )
	{
		for(int iBone = 0 ; iBone < m_nBone ; iBone++)
		{
			xBoneData& boneData = m_BoneFrame[iBone][iFrame];
			stream.read((char*)&boneData , sizeof(xBoneData) );
		}
	}
	return true;
}

bool xCoreAction::load(const wchar_t * _name , xcomdocstream* stream)
{
    m_Name	     = _name;
    stream->read(m_SkeID);
    stream->read(m_nBone);
    stream->read(m_Info);

    int nFrame = m_Info.m_iLastFrame - m_Info.m_iFirstFrame + 1;
    assert(m_Info.m_nFrame == nFrame );
	
    int dim[2];
    dim[0] = m_nBone;
    dim[1] = nFrame;

    m_BoneFrame.create(dim);
	m_ActionAttr.init(m_nBone);
    for(int iFrame = 0 ; iFrame < nFrame ; iFrame ++ )
    {
        for(int iBone = 0 ; iBone < m_nBone ; iBone++)
        {

            xBoneData& boneData = m_BoneFrame[iBone][iFrame];
            if(stream->read(boneData) < sizeof(boneData) )
            {
                return false;
            }
            else
            {
                continue;
            }
        }
    }
	return true;
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
}
bool xCoreAction::load(xXmlNode* pCfgNode , xcomdoc& doc , const wchar_t* actionDir)
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

void xCoreAction::setBoneNumber(int _nBone)
{
    m_nBone = _nBone;
}

void xCoreAction::unload()
{
    m_BoneFrame.free();
}
//模型内置的动作
xEmbAction::xEmbAction()
{

}

xEmbAction::~xEmbAction()
{
	xCoreAction::unload();
}

void xEmbAction::unload()
{

}
END_NAMESPACE_XEVOL3D
