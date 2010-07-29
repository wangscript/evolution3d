#ifndef __XTRACK_ACTION_H__
#define __XTRACK_ACTION_H__

#include "xBaseAction.h"
#include <deque>
BEGIN_NAMESPACE_XEVOL3D
//--------------------------------------------------------
//一个动作序列
//一个角色动画包括好几个动作序列.
//每个序列的当前动作，参与角色的xMixAction的混合。
//--------------------------------------------------------
typedef std::vector<std::ds_wstring , dllsafe_alloc<std::ds_wstring> > vActionNameList;

struct xTrackActionInfo
{
    HCoreAction m_hCoreAction;
    float       m_DesireTime;
    float       m_OriginTime;

    xTrackActionInfo()
    {
        m_hCoreAction.setNULL();
    }

    void setNULL()
    {
        m_hCoreAction.setNULL();
        m_DesireTime = 0.0f;
        m_OriginTime = 0.0f;
    }
    xTrackActionInfo(HCoreAction hCoreAction)
    {
        m_hCoreAction = hCoreAction;
        m_DesireTime = hCoreAction->getDurTime();
        m_OriginTime = m_DesireTime;
    }

    xTrackActionInfo(HCoreAction hCoreAction , float desireTime)
    {
        m_hCoreAction = hCoreAction;
        m_DesireTime = desireTime;
        m_OriginTime = hCoreAction->getDurTime();
    }

    xTrackActionInfo(HCoreAction hCoreAction , HCoreAction hSyncAction)
    {
        m_hCoreAction = hCoreAction;
        m_DesireTime = hSyncAction->getDurTime();
        m_OriginTime = hCoreAction->getDurTime();
    }

    void setDesireTime()
    {
        if(m_hCoreAction.isHandle())
        {
            m_hCoreAction->setTime(m_DesireTime);
        }
    }

    void setOriginTime()
    {
        if(m_hCoreAction.isHandle())
        {
            m_hCoreAction->setTime(m_OriginTime);
        }
    }
};

typedef std::deque<xTrackActionInfo>                                        vActionQueue;

enum eActionTrackEvent
{
    eActionChange,
    eActionBegin,
    eActionFinish,
};

class IActionTrackListenner
{
public:
    virtual void onActionTrackEvent(eActionTrackEvent _event, int idx ,  xBaseAction* pAction) = 0;
    virtual void onActionTrackEvent(eActionTrackEvent _event, int idx ,  HCoreAction  hAction) = 0;
};

class _XEVOL_BASE_API_ xActionTrack 
{
public:
    typedef ds_vector(IActionTrackListenner*) vActionEventListenners;
public:
    void              setTimeScale(float _scale);
    float             timeScale();
    void              setActionMgr(xCoreActionMgr* pActMgr);
    void              setTransitionTime(int _time);
    bool              setStandbyAction(const wchar_t* actName);
    bool              setActionList(vActionNameList& actions);
    bool              setActionList(vActionQueue& actionList);
    xBaseAction*      getCurrentAction();
    bool              doNextAction(int _time);
    bool              update(unsigned long passedTime , xCoreSkeleton* pSkeleton);
    bool              enterStandby();
    int               actionTime();
    int               hashName();
    HCoreAction       standbyAction(){ return m_hStandbyAction ; }
    void              addListenner(IActionTrackListenner* pListenner);
    void              removeListenner(IActionTrackListenner* pListenner);
    void              fireEvent(eActionTrackEvent _event,  int iTrackIdx , xBaseAction* pAction);
    void              fireEvent(eActionTrackEvent _event,  int iTrackIdx , HCoreAction  pAction);
    xActionTrack(xCoreActionMgr* pActMgr , HBaseModel  hModel , const wchar_t* _name , int actionTrackId);

protected:
    int                    m_actTime;
    int                    m_iTransitionTime;
    float                  m_fTimeScale;
    xCoreActionMgr*        m_pActionMgr;
    vActionQueue           m_vActions;
    xTrackActionInfo       m_CurrentAction;
    HCoreAction            m_hStandbyAction;
    HBaseModel             m_hModel;
    int                    m_TrackId;
    std::wstring           m_name;
    int                    m_HashName;
    vActionEventListenners m_vListenners;
};

END_NAMESPACE_XEVOL3D
#endif
