#include "../xStdPch.h"
#include "xBaseAction.h"
#include "xCoreSkeleton.h"
#include "../xcomdoc/xdocfstream.h"
#include "../BaseLib/xI18N.h"
#include "xCoreMesh.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D 
IMPL_BASE_OBJECT_CLASSID(xBaseAction          , IBaseObject);
IMPL_OBJECT_FACTORY_MGR(xBaseAction ,xBaseActionCreator , xBaseActionFactoryMgr )

xBoneArray::xBoneArray()
{
    m_Data = NULL;
}

xBoneArray::~xBoneArray()
{
    free();
}


void xBoneArray::create(int Row , int Col)
{
    int size = Col * Row;
    m_Col = Col ;
    m_Row = Row ;
    m_Data = new xBoneTrans[size];
}

void xBoneArray::create(int* DimSize)
{
    create(DimSize[0],DimSize[1]);
}
#ifndef _DEBUG
xBoneTrans* xBoneArray::operator[](int index)
{
    assert(index < m_Row && index >= 0);
    return &m_Data[index * m_Col];
}
#else
xBoneArray::xBoneArray1D xBoneArray::operator[](int index)
{
    assert(index < m_Row && index >= 0);
    return xBoneArray1D(&m_Data[index * m_Col] , m_Col );
}
#endif
void xBoneArray::free()
{
    if( m_Data)
    {
        delete [] m_Data;
        m_Data = NULL;
    }
}


//¹Ç¼ÜµÄÒ»¸öFrame
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
void xBaseAction::_getBlendFrameLoop(const xActionInfo* pInfo  , long _time_in_ms , xBaseActionState& _as)
{
	int& frame1 = _as.m_Frame1 ;
	int& frame2 = _as.m_Frame2 ;

	int lTime = pInfo->m_lTime;
	if(lTime <=0 ) lTime = 1;
    long t = _time_in_ms % lTime;

    int nFrame = pInfo->m_iLastFrame - pInfo->m_iFirstFrame + 1;
    float fFrame = (nFrame ) *  t / float( lTime); 

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
    _as.m_fTime =  fFrame - frame1;
}

void xBaseAction::_getBlendFrameClamp(const xActionInfo* pInfo  , long _time_in_ms , xBaseActionState& _as)
{
	int& frame1 = _as.m_Frame1 ;
	int& frame2 = _as.m_Frame2 ;
    if(_time_in_ms > pInfo->m_lTime) _time_in_ms = pInfo->m_lTime;
    long t = _time_in_ms ;//tt % m_Info.m_lTime;

    int nFrame = pInfo->m_iLastFrame - pInfo->m_iFirstFrame + 1;
    float lTime = float( pInfo->m_lTime); 
    if(lTime == 0.0f ) lTime = 30.0f;
    float fFrame = (nFrame ) *  t / float( pInfo->m_lTime); 

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
    _as.m_fTime =  fFrame - frame1;
}

void xBaseAction::updateState(long _time_in_ms , xBaseActionState& _as , bool bLoop)
{
	if(bLoop)
	{
		_getBlendFrameLoop(info() , _time_in_ms , _as);
	}
	else
	{
		_getBlendFrameClamp(info() ,_time_in_ms , _as);
	}
}

void xBaseAction::updateState(long _time_in_ms , xBaseActionState& _as)
{
    if(m_bLoopAction)
    {
        _getBlendFrameLoop(info() , _time_in_ms , _as);
    }
    else
    {
        _getBlendFrameClamp(info() ,_time_in_ms , _as);
    }
}

xBaseAction::xBaseAction(xCoreSkeleton* pSkeleton , int param)
{

}


xBaseAction* xBaseAction::createInstance(eActionType _type , xCoreSkeleton* pSkeleton  , int param  )
{
    std::wstring _className = L"";
     switch(_type)
     {
     case eActType_Skeleton   : _className = L"xCoreAction"      ;  break;
     case eActType_Keyframe   : _className = L"xKeyFrameAction"  ;  break;
     case eActType_Frame      : assert(0)                        ;  break;
     case eActType_Blend      : assert(0)                        ;  break;
     case eActType_Transition : _className = L"xTransitionAction";  break;
     case eActType_Mix        : _className = L"xMixedAction"     ;  break;
     }
     return createInstance(_className.c_str() , pSkeleton , param);
}

xBaseAction* xBaseAction::createInstance(const wchar_t*  _class , xCoreSkeleton* pSkeleton  , int param )
{
    return xBaseActionFactoryMgr::singleton()->createInstance(_class , pSkeleton , param);
}

//
//bool xBaseAction::getBoneData(xBoneData& boneData , int boneIndex, long _time )
//{
//	xBaseActionState _ActionState;
//	updateState(_time , _ActionState , m_bLoopAction);
//	xBoneTrans boneTrans;
//	getBoneTrans(boneIndex , &_ActionState , boneTrans);
//	boneData.fromBoneTrans(boneTrans);
//	return true;
//}

END_NAMESPACE_XEVOL3D
