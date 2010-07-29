#ifndef __XEVOL3D_ACTOR_MODELELEMENT_H__
#define __XEVOL3D_ACTOR_MODELELEMENT_H__
#include "xDrawElement.h"
#include "xModel/xBaseModelMgr.h"
#include "xModel/xBaseModel.h"
#include "xModel/xBaseAction.h"
#include "xModel/xActionTrack.h"
#include <deque>
BEGIN_NAMESPACE_XEVOL3D

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
    bool                   isVisible(xGeomLib::xCamera* pCamera, const xMathLib::xmat4& _mat);
    HBaseModel             hModel(){return m_Model ; }
    xCoreActionMgr*        actionMgr(){ return m_pActionMgr ; }
    void                   onActionTrackEvent(eActionTrackEvent _event, int idx ,  xBaseAction* pAction);
    void                   onActionTrackEvent(eActionTrackEvent _event, int idx ,  HCoreAction  hAction);
public:
	
public:
	bool    setModel(HBaseModel hModel);
public:
	DECL_BASE_OBJECT_DLLSAFE(xActorElement*);
protected:
    class xActionStateListenner : public IActionTrackListenner
    {
    public:
        virtual void onActionTrackEvent(eActionTrackEvent _event, int idx ,  xBaseAction* pAction);
        virtual void onActionTrackEvent(eActionTrackEvent _event, int idx ,  HCoreAction  hAction);
    public:
        xActorElement* m_pElement;
    };
protected:
	HBaseModel               m_Model;
	xCoreActionFrame*        m_pActionFrame;
	xCoreActionMgr*          m_pActionMgr;
	HCoreAction              m_hRestMotion;
	HCoreAction              m_hMixAction;
    vActionTracks            m_vActionTracks;
    xBaseActionState*        m_BaseActionStates;
    xActionStateListenner    m_Listenner;
};


END_NAMESPACE_XEVOL3D
#endif
