#include "../xStdPch.h"
#include "xActionTrack.h"
#include "xBaseModel.h"
#include "xTransitionAction.h"
#include "xCoreActionMgr.h"
#include "xBaseModelMgr.h"
BEGIN_NAMESPACE_XEVOL3D

int xActionTrack::actionTime()
{ 
    return m_actTime ;
}

int  xActionTrack::hashName()
{ 
    return m_HashName ; 
}

void  xActionTrack::setTimeScale(float _scale) 
{
    m_fTimeScale = _scale;
}

float xActionTrack::timeScale()
{ 
    return m_fTimeScale ;
}

void xActionTrack::setTransitionTime(int _time) 
{
    m_iTransitionTime = _time;
}

void xActionTrack::setActionMgr(xCoreActionMgr* pActMgr)
{
    m_pActionMgr = pActMgr ;
}

xBaseAction*  xActionTrack::getCurrentAction()
{
    if(m_CurrentAction.m_hCoreAction.isHandle() ) 
        return m_CurrentAction.m_hCoreAction.getResource();

    return m_hStandbyAction.getResource();
}

bool  xActionTrack::setStandbyAction(const wchar_t* actName)
{
    xBaseModel* pModel = m_hModel.getResource();
    if(pModel)
    {
        xCoreActionName _name(eActType_None , m_hModel , actName , pModel->name() );
        m_hStandbyAction = m_pActionMgr->loadAction(_name , 0 , true);
        return true;
    }	 
    return false;

}

bool   xActionTrack::enterStandby()
{
    xBaseModel* pModel = m_hModel.getResource();
    if(pModel == NULL)
        return false;

    if(m_hStandbyAction.getResource()  == NULL)
        return false;

    m_CurrentAction.setNULL();
    m_vActions.clear();

    //=============
    const wchar_t* lastAction = m_hStandbyAction->name();
    ds_wstring transitName = xTransitionAction::MakeName(lastAction , lastAction);
    xCoreActionName _TransActName(eActType_Transition , m_hModel , transitName.c_str() , pModel->name() );
    HCoreAction hTransAction = m_pActionMgr->loadAction(_TransActName , 0 , true);
    m_vActions.push_back(hTransAction);
    hTransAction->setTime(m_iTransitionTime/1000.0f);
    m_vActions.push_back(m_hStandbyAction);
    doNextAction(0);
    return true;
}

bool   xActionTrack::setActionList(vActionQueue& actionList)
{
    m_vActions.clear();
    m_vActions = actionList;
    return true;
}

bool   xActionTrack::setActionList(vActionNameList& actions)
{
    xBaseModel* pModel = m_hModel.getResource();
    if(pModel == NULL)
        return false;

    m_vActions.clear();
    m_vActions.push_back(m_hStandbyAction);
    const wchar_t* lastAction = m_hStandbyAction->name();
    const wchar_t* beginAction = lastAction;

    for(size_t i = 0 ; i < actions.size() ; i ++)
    {
        const wchar_t* actName = actions[i].c_str();
        ds_wstring transitName = xTransitionAction::MakeName(lastAction , actName);
        xCoreActionName _TransActName(eActType_Transition , m_hModel , transitName.c_str() , pModel->name() );
        HCoreAction hTransAction = m_pActionMgr->loadAction(_TransActName , 0 , true);
        if(hTransAction.getResource() == NULL)
        {
            continue ;
        }

        hTransAction->setTime(m_iTransitionTime/1000.0f);
        m_vActions.push_back(hTransAction);

        xCoreActionName _name(eActType_None , m_hModel , actName , pModel->name() );
        HCoreAction hAction = m_pActionMgr->loadAction(_name , 0 , true);
        if(hAction.getResource() == NULL)
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"Create transition action failed : %s action not exist\n" , actName );
            continue;
        }
        m_vActions.push_back(hAction);

        lastAction = actName;
    }

    {
        ds_wstring transitName = xTransitionAction::MakeName(lastAction , beginAction);
        xCoreActionName _TransActName(eActType_Transition , m_hModel , transitName.c_str() , pModel->name() );
        HCoreAction hTransAction = m_pActionMgr->loadAction(_TransActName , 0 , true);
        if(hTransAction.getResource() == NULL)
        {
            return true;
        }

        hTransAction->setTime(m_iTransitionTime/1000.0f);
        m_vActions.push_back(hTransAction);
    }

    return true;
}

bool xActionTrack::doNextAction(int _time)
{
    xBaseModel* pModel = m_hModel.getResource();
    if(pModel == NULL)
        return false;
    if(m_vActions.size() == 0)
    {
        m_CurrentAction.m_hCoreAction.setNULL();
        return false;
    }
    m_CurrentAction.setOriginTime();
    xTrackActionInfo trackActionInfo = *m_vActions.begin();
    m_CurrentAction = trackActionInfo;

    fireEvent(eActionChange , m_TrackId , trackActionInfo.m_hCoreAction );
    int timeRemoved = m_actTime - _time;
    m_actTime = _time;
    m_vActions.pop_front();
    m_CurrentAction.setDesireTime();
    return true;

}

xActionTrack::xActionTrack(xCoreActionMgr* pActMgr , HBaseModel  hModel , const wchar_t* _name , int trackId)
{
    m_hModel     = hModel;
    m_name       = _name;
    m_pActionMgr = pActMgr;
    m_HashName = xStringHash(_name);
    m_actTime = 0;
    m_CurrentAction.setNULL();;
    m_iTransitionTime = 400; //过渡时间为400毫秒
    m_fTimeScale      = 1.0f;
    m_TrackId = trackId;
    m_hStandbyAction.setNULL();
}

bool xActionTrack::update(unsigned long passedTime , xCoreSkeleton* pSkeleton)
{
    passedTime = (unsigned long)(m_fTimeScale * passedTime);
    xBaseAction* pAction = getCurrentAction();
    if(pAction && pSkeleton)
    {
        m_actTime += passedTime;
        int lefTime = 0;
        const xActionInfo* pActInfo = pAction->info();
        int actionTime = pActInfo->m_lTime;
        if(m_actTime > actionTime)
        {
            lefTime = m_actTime - actionTime;
        }

        if(lefTime > 0 )
        {
            if( false == doNextAction(0) )
            {
                fireEvent(eActionFinish , m_TrackId , m_CurrentAction.m_hCoreAction.getResource() );
                enterStandby();
            }
        }
    }
    return true;
}

void xActionTrack::addListenner(IActionTrackListenner* pListenner)
{
    size_t nL = m_vListenners.size();
    for(size_t i = 0 ; i < nL ; i ++)
    {
        IActionTrackListenner* pL = m_vListenners[i];
        if(pL == pListenner) return ;
    }
    m_vListenners.push_back( pListenner );
}

void xActionTrack::removeListenner(IActionTrackListenner* pListenner)
{
    vActionEventListenners::iterator pos = m_vListenners.begin();
    for( ; pos != m_vListenners.end() ; pos ++)
    {
        if(*pos == pListenner)
        {
            m_vListenners.erase(pos);
            return ;
        }
    }
    return ;
}

void xActionTrack::fireEvent(eActionTrackEvent _event, int iTrackIdx , xBaseAction* pAction)
{
    size_t nL = m_vListenners.size();
    for(size_t i = 0 ; i < nL ; i ++)
    {
        IActionTrackListenner* pL = m_vListenners[i];
        pL->onActionTrackEvent(_event , iTrackIdx , pAction);
    }
}
void xActionTrack::fireEvent(eActionTrackEvent _event, int iTrackIdx , HCoreAction pAction)
{
    size_t nL = m_vListenners.size();
    for(size_t i = 0 ; i < nL ; i ++)
    {
        IActionTrackListenner* pL = m_vListenners[i];
        pL->onActionTrackEvent(_event , iTrackIdx , pAction);
    }
}
END_NAMESPACE_XEVOL3D
