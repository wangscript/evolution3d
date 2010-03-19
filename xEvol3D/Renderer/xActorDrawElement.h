#ifndef __XEVOL3D_ACTOR_MODELELEMENT_H__
#define __XEVOL3D_ACTOR_MODELELEMENT_H__
#include "xDrawElement.h"
#include "xModel/xBaseModelMgr.h"
#include "xModel/xBaseModel.h"
#include "xModel/xCoreAction.h"
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

class _XEVOL_BASE_API_ xActionTrack 
{

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
    xActionTrack(xCoreActionMgr* pActMgr , HCoreAction hMixedAction , HBaseModel  hModel , const wchar_t* _name , int iSlot);
    
protected:
    int               m_actTime;
    int               m_iTransitionTime;
    float             m_fTimeScale;
    xCoreActionMgr*   m_pActionMgr;
    vActionQueue      m_vActions;
    xTrackActionInfo  m_CurrentAction;
    HCoreAction       m_hStandbyAction;
    HCoreAction       m_hMixAction;
    HBaseModel        m_hModel;
    int               m_iSlot;
    std::wstring      m_name;
    int               m_HashName;
};


class _XEVOL_BASE_API_ xActorElement : public IDrawElement
{
public:
    typedef std::vector<xActionTrack*> vActionTracks;
	void                   setActionMgr(xCoreActionMgr* pActMgr);
    void                   setTimeScale(float _scale);
public:
	xActorElement(IBaseRenderer* pRenderer , int v);
	virtual ~xActorElement();    
	virtual void           center(XMathLib::xvec3&   _center);
	virtual void           aabb(xGeomLib::xaabb&     _aabb);
	virtual void           shpere(xGeomLib::xshpere& _shpere);
	virtual bool           render(unsigned long passedTime);
	virtual bool           update(unsigned long passedTime);
    xActionTrack*          findActionTrack(const wchar_t* _trackName);
    bool                   unloadActionTracks();
    void                   setMixAction(const wchar_t* actionXml);
    bool                   isVisible(xGeomLib::xCamera* pCamera);
    HBaseModel             hModel(){return m_Model ; }
    xCoreActionMgr*        actionMgr(){ return m_pActionMgr ; }
public:
	
public:
	bool    setModel(HBaseModel hModel);
public:
	DECL_BASE_OBJECT_DLLSAFE(xActorElement*);
protected:
	HBaseModel               m_Model;
	xCoreActionFrame*        m_pActionFrame;
	xCoreActionMgr*          m_pActionMgr;
	HCoreAction              m_hRestMotion;
	HCoreAction              m_hMixAction;
    vActionTracks            m_vActionTracks;
    xBaseActionState*        m_BaseActionStates;
};


END_NAMESPACE_XEVOL3D
#endif
