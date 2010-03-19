#include "../xStdPch.h"
#include "xTransitionAction.h"
#include "xCoreSkeleton.h"
#include "../BaseLib/xI18N.h"
#include "xCoreMesh.h"
using namespace xMathLib;
BEGIN_NAMESPACE_XEVOL3D 
IMPL_BASE_OBJECT_CLASSID(xTransitionAction    , xCoreAction);

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

float xTransitionAction::getDurTime()
{
    return m_Info.m_lTime/1000.0f;
}

bool xTransitionAction::setAction(xCoreAction* pStart , xCoreAction* pEnd , float _time)
{
    int nBone = pStart->nBone(); //
	if(pStart->skeletonID() != pEnd->skeletonID())
		return false;

	m_BoneFrame.free();

	m_Info.m_lTime = _time * 1000;
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
	for(int iBone = 0 ; iBone < m_nBone ; iBone++)
	{
        xCoreActionAttr* pStartAttri = pStart->attribute();
		m_ActionAttr[iBone] = pStartAttri ? pStartAttri->boneAttribute(iBone) : xCoreActionAttr::eBoneInclude;
		m_BoneFrame[iBone][0] = *pStart->getBoneData(iBone , nLastFrame ) ; //第一帧就是前一个动作的最后一帧。
		m_BoneFrame[iBone][1] = *pEnd->getBoneData(iBone   ,  0         ) ; //第二帧就是后一个动作的第一帧
	}
	return true;
}

bool xTransitionAction::setAction(xCoreSkeleton* pSkeleton , xCoreActionFrame* pStart , xCoreAction* pEnd , float _time)
{
    int nBone = pEnd->nBone(); //
    if(pEnd->skeletonID() != pSkeleton->id())
        return false;

    m_BoneFrame.free();

    m_Info.m_lTime = _time * 1000;
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
    for(int iBone = 0 ; iBone < m_nBone ; iBone++)
    {
        xCoreActionAttr* pEndAttri = pEnd->attribute();
        m_ActionAttr[iBone] = pEndAttri ? pEndAttri->boneAttribute(iBone) : xCoreActionAttr::eBoneInclude;

        const xSkinBone& SkinBone = pSkeleton->getBone(iBone);
        xBoneTrans&      trans    = pStart->boneTrans( iBone );

        m_BoneFrame[iBone][0].fromBoneTrans(trans , pStart->wsMat(iBone) , SkinBone.m_InitMTInv );
        m_BoneFrame[iBone][1] = *pEnd->getBoneData(iBone   ,  0         ) ; //第二帧就是后一个动作的第一帧
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
