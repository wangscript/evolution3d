#include "../xStdPch.h"
#include "xCoreAction.h"
#include "xCoreSkeleton.h"
#include "../xcomdoc/xdocfstream.h"
#include "../BaseLib/xI18N.h"
#include "xCoreMesh.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xCoreAction          , IBaseObject);
IMPL_BASE_OBJECT_CLASSID(xTransitionAction    , xCoreAction);
IMPL_BASE_OBJECT_CLASSID(xEmbAction           , xCoreAction);
//骨架的一个Frame
xCoreActionFrame::xCoreActionFrame()
{
    m_nBone = 0;
    m_FrameMat = NULL;
    m_WSMats   = NULL;
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

bool xCoreActionFrame::load(int nBone)
{
    unload();
    m_FrameMat = new xMathLib::xmat4[nBone];
    m_WSMats   = new xMathLib::xmat4[nBone];
    assert(m_FrameMat);
    m_nBone = nBone;
    return m_FrameMat != NULL;
}

void xCoreActionFrame::unload()
{
    XSAFE_DELETE_ARRAY(m_FrameMat);
    XSAFE_DELETE_ARRAY(m_WSMats);
}

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
float xCoreAction::_getBlendFrameLoop(long _time_in_ms, int& frame1 , int& frame2)
{
	long t = _time_in_ms % m_Info.m_lTime;

	int nFrame = m_Info.m_iLastFrame - m_Info.m_iFirstFrame + 1;
	float fFrame = nFrame *  t / float(m_Info.m_lTime); 

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
float xCoreAction::_getBlendFrameClamp(long _time_in_ms, int& frame1 , int& frame2)
{
	if(_time_in_ms > m_Info.m_lTime) _time_in_ms = m_Info.m_lTime;
	long t = _time_in_ms ;//tt % m_Info.m_lTime;

	int nFrame = m_Info.m_iLastFrame - m_Info.m_iFirstFrame + 1;
	float fFrame = nFrame *  t / float(m_Info.m_lTime); 

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

bool xCoreAction::blend(long _time_in_ms, xCoreActionFrame* _skeletonFrame , xCoreSkeleton* pSkeleton)
{
	xCoreActionFrame& skeletonFrame = *_skeletonFrame;
    int frame1;
    int frame2;
	float tfloat = 0.0f;
    if(m_bLoopAction)
	{
		tfloat = _getBlendFrameLoop(_time_in_ms , frame1 , frame2);
	}
	else
	{
		tfloat = _getBlendFrameClamp(_time_in_ms , frame1 , frame2);
	}
    pSkeleton->blendSlerp(this,skeletonFrame,tfloat,frame1,frame2);
    return true;
}

int xCoreAction::memUsage()
{
	return m_nBone * m_Info.m_nFrame * sizeof(xBoneData);
}

xBoneData& xCoreAction::getBoneData(int boneIndex, int frame)
{
    return m_BoneFrame[boneIndex][frame];
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



//两个动作之间做插值的过渡动作
xTransitionAction::xTransitionAction()
{
	m_Info.m_eActType = eActType_Transition;
	m_Info.m_iFirstFrame = 0;
	m_Info.m_iLastFrame = 1;
	m_Info.m_lTime = 500; //0.5秒
	m_Info.m_nFrame = 2;
}

xTransitionAction::~xTransitionAction()
{

}

void xTransitionAction::setTransitTime(float _time)
{
   m_Info.m_lTime = _time * 1000;//毫秒记
}

bool xTransitionAction::setAction(xCoreAction* pStart , xCoreAction* pEnd , float _time)
{
    int nBone = pStart->nBone(); //
	if(pStart->skeletonID() != pEnd->skeletonID())
		return false;

	m_Info.m_lTime = _time * 1000;
	m_nBone = pStart->nBone();
	int dim[2];
	dim[0] = m_nBone;
	dim[1] = m_Info.m_nFrame;

	m_SkeID = pStart->skeletonID();
	m_Name  = MakeName(pStart->name() , pEnd->name() );
	m_nBone = pStart->nBone();
	m_BoneFrame.create(dim);
	int nLastFrame = pStart->info().m_nFrame - 1;
	for(int iBone = 0 ; iBone < m_nBone ; iBone++)
	{
		xBoneData& boneData = m_BoneFrame[iBone][0];
		m_BoneFrame[iBone][0] = pStart->getBoneData(iBone , nLastFrame) ; //第一帧就是前一个动作的最后一帧。
		m_BoneFrame[iBone][1] = pEnd->getBoneData(iBone,0)           ; //第二帧就是后一个动作的第一帧
	}
	return true;
}

bool xTransitionAction::IsTransitionName(const std::ds_wstring& _name , std::ds_wstring& _first , std::ds_wstring& _second)
{
	if(_name.find(L"->") == std::ds_wstring::npos)
		return false;
	int pos = (int)_name.find(L"->");
	wchar_t buf[1024] = {0};
	wcsncpy(buf , _name.c_str() , 1024);
	buf[pos] = 0;
    _first = buf;
	_second = buf + pos + 2;
	return true;
}

std::ds_wstring  xTransitionAction::MakeName(const wchar_t* _first , const wchar_t* _second)
{
	return std::ds_wstring(_first) + L"->" + _second;
}

END_NAMESPACE_XEVOL3D
