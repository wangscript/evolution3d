#include "../xStdPch.h"
#include "xTransitionAction.h"
#include "xCoreSkeleton.h"
#include "../BaseLib/xI18N.h"
#include "xCoreMesh.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D 
IMPL_BASE_OBJECT_CLASSID(xTransitionAction    , xCoreAction);
IMPL_ACTION_FACTORY(xTransitionAction);
//两个动作之间做插值的过渡动作
xTransitionAction::xTransitionAction(xCoreSkeleton* pSkeleton , int param) : xCoreAction(pSkeleton , param)
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
   m_Info.m_lTime = (int)(_time * 1000);//毫秒记
}

float xTransitionAction::getDurTime()
{
    return m_Info.m_lTime/1000.0f;
}

bool xTransitionAction::setAction(xBaseAction* pStart , xBaseAction* pEnd , float _time)
{
    int nBone = pStart->nBone(); //
	if(pStart->skeletonID() != pEnd->skeletonID())
		return false;

	m_BoneFrame.free();

	m_Info.m_lTime = (int)(_time * 1000);
	m_nBone = pStart->nBone();
	int dim[2];
	dim[0] = m_nBone;
	dim[1] = m_Info.m_nFrame;

	m_SkeID = pStart->skeletonID();
	m_Name  = MakeName(pStart->name() , pEnd->name() );
	m_nBone = pStart->nBone();
	m_BoneFrame.create(dim);
	int nLastFrame = pStart->info()->m_nFrame - 1;
	m_ActionAttr.init(m_nBone);
	xBaseActionState _State1;
	xBaseActionState _State2;
	_State1.m_lTime = pStart->info()->m_lTime;
	_State2.m_lTime = 0;
    
	xBoneTrans boneTrans;
	for(int iBone = 0 ; iBone < m_nBone ; iBone++)
	{
        xCoreActionAttr* pStartAttri = pStart->attribute();
		m_ActionAttr[iBone] = pStartAttri ? pStartAttri->boneAttribute(iBone) : xCoreActionAttr::eBoneInclude;
		pStart->updateState(_State1.m_lTime , _State1);
		pEnd  ->updateState(_State2.m_lTime , _State2);

        pStart->getBoneTrans(iBone , &_State1 , m_BoneFrame[iBone][0] ); //第一帧就是前一个动作的最后一帧
		pEnd  ->getBoneTrans(iBone , &_State2 , m_BoneFrame[iBone][1] ); //第二帧就是后一个动作的第一帧
	}
	return true;
}

bool xTransitionAction::setAction(xCoreSkeleton* pSkeleton , xCoreActionFrame* pStart , xBaseAction* pEnd , float _time)
{
    int nBone = pEnd->nBone(); //
    if(pEnd->skeletonID() != pSkeleton->id())
        return false;

    m_BoneFrame.free();

    m_Info.m_lTime = (int)(_time * 1000);
    m_nBone = pStart->nBone();
    int dim[2];
    dim[0] = m_nBone;
    dim[1] = m_Info.m_nFrame;

    m_SkeID = pEnd->skeletonID();
    m_Name  = MakeName(L"LastFrame" , pEnd->name() );
    m_nBone = pStart->nBone();
    m_BoneFrame.create(dim);
    int nLastFrame = pEnd->info()->m_nFrame - 1;
    m_ActionAttr.init(m_nBone);
	xBaseActionState _State2;
	_State2.m_lTime = 0;

    for(int iBone = 0 ; iBone < m_nBone ; iBone++)
    {
        xCoreActionAttr* pEndAttri = pEnd->attribute();
        m_ActionAttr[iBone] = pEndAttri ? pEndAttri->boneAttribute(iBone) : xCoreActionAttr::eBoneInclude;

        const xSkinBone& SkinBone = pSkeleton->getBone(iBone);
        xBoneTrans&      trans    = pStart->boneTrans( iBone );

        m_BoneFrame[iBone][0] = trans ;// , pStart->wsMat(iBone) , SkinBone.m_InitMTInv );
		pEnd  ->getBoneTrans(iBone , &_State2 , m_BoneFrame[iBone][1] );
        //m_BoneFrame[iBone][1] = *pEnd->getBoneData(iBone   ,  0         ) ; //第二帧就是后一个动作的第一帧
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
//////////////////////////////////////////////////////////////////////////
IMPL_BASE_OBJECT_CLASSID(xPauseAction    , xCoreAction);
IMPL_ACTION_FACTORY(xPauseAction);
//两个动作之间做插值的过渡动作
xPauseAction::xPauseAction(xCoreSkeleton* pSkeleton , int param) : xCoreAction(pSkeleton , param)
{
    m_Info.m_eActType = eActType_Freeze;
    m_Info.m_iFirstFrame = 0;
    m_Info.m_iLastFrame = 1;
    m_Info.m_lTime = 500; //0.5秒
    m_Info.m_nFrame = 2;
}

xPauseAction::~xPauseAction()
{

}

void xPauseAction::setTransitTime(float _time)
{
    m_Info.m_lTime = _time * 1000;//毫秒记
}

float xPauseAction::getDurTime()
{
    return m_Info.m_lTime/1000.0f;
}

bool xPauseAction::setAction(xCoreAction* pAction , float _time)
{
    int nBone = pAction->nBone(); //

    m_BoneFrame.free();

    m_Info.m_lTime = (int)(_time * 1000);
    m_nBone = pAction->nBone();
    int dim[2];
    dim[0] = m_nBone;
    dim[1] = m_Info.m_nFrame;

    m_SkeID = pAction->skeletonID();
    m_Name  = MakeName(pAction->name() , _time );
    m_nBone = pAction->nBone();
    m_BoneFrame.create(dim);
    int nLastFrame = pAction->info()->m_nFrame - 1;
    m_ActionAttr.init(m_nBone);
    xBaseActionState _State1;
    _State1.m_lTime = pAction->info()->m_lTime;

    xBoneTrans boneTrans;
    for(int iBone = 0 ; iBone < m_nBone ; iBone++)
    {
        xCoreActionAttr* pStartAttri = pAction->attribute();
        m_ActionAttr[iBone] = pStartAttri ? pStartAttri->boneAttribute(iBone) : xCoreActionAttr::eBoneInclude;
        pAction->updateState(_State1.m_lTime , _State1);

        pAction->getBoneTrans(iBone , &_State1 , m_BoneFrame[iBone][0] ); //一个动作的最后一帧
        pAction->getBoneTrans(iBone , &_State1 , m_BoneFrame[iBone][1] ); //一个动作的最后一帧
    }
    return true;
}

std::ds_wstring  xPauseAction::MakeName(const wchar_t* _action , float _t)
{
    wchar_t buf [128] = {0};
    swprintf_x(buf , 128 , L"%s@%g" , _action , _t);
    return buf;
}

std::ds_wstring   xPauseAction::GetActionName(const wchar_t*  _action  , float& _t)
{
    //格式 ActionName@time
    wchar_t buf [128] = {0};
    wcsncpy_x(buf , _action , 128);
    for(int i = 0 ; i < 128 ; i ++)
    {
        if(buf[i] == '@')
        {
            buf[i] = 0;
            swscanf(& buf[i + 1] , L"%f" , &_t);
            return buf;
        }
    }
    return L"";
}

END_NAMESPACE_XEVOL3D
