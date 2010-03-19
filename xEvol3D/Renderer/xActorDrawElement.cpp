#include "../xStdPch.h"
#include "xActorDrawElement.h"
#include "xRenderer.h"
#include "xMaterial.h"
#include "../xModel/xCoreMesh.h"
#include "../xModel/xCoreActionMgr.h"
#include "../xModel/xMixAction.h"
#include "../xModel/xTransitionAction.h"

BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_DLLSAFE(xActorElement ,  IDrawElement);
IMPL_OBJECT_FACTORY(xActorElement , IDrawElement , xActorElementFactory , IDrawElementCreator , xDrawElementFactoryMgr , L"ActorElement", L"Actor Element" , IBaseRenderer* , int);

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
        xCoreActionName _name(m_hModel , actName , pModel->name() );
        m_hStandbyAction = m_pActionMgr->add(_name , 0 , true);
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
    xCoreActionName _TransActName(m_hModel , transitName.c_str() , pModel->name() );
    HCoreAction hTransAction = m_pActionMgr->add(_TransActName , 0 , true);
    m_vActions.push_back(hTransAction);
    hTransAction->setTime(m_iTransitionTime/1000.0f);

    m_vActions.push_back(m_hStandbyAction);
    xMixedAction* pMixAction = resource_cast<xMixedAction*>(m_hMixAction);
    pMixAction->setAction(m_iSlot , resource_cast<xCoreAction*>(hTransAction ) );
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
        xCoreActionName _TransActName(m_hModel , transitName.c_str() , pModel->name() );
        HCoreAction hTransAction = m_pActionMgr->add(_TransActName , 0 , true);
        if(hTransAction.getResource() == NULL)
        {
            continue ;
        }

        hTransAction->setTime(m_iTransitionTime/1000.0f);
        m_vActions.push_back(hTransAction);

        xCoreActionName _name(m_hModel , actName , pModel->name() );
        HCoreAction hAction = m_pActionMgr->add(_name , 0 , true);
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
        xCoreActionName _TransActName(m_hModel , transitName.c_str() , pModel->name() );
        HCoreAction hTransAction = m_pActionMgr->add(_TransActName , 0 , true);
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
    xMixedAction* pMixedAction = resource_cast<xMixedAction*>(m_hMixAction); 
    pMixedAction->setAction(m_iSlot, resource_cast<xCoreAction*>(trackActionInfo.m_hCoreAction) );
    int timeRemoved = m_actTime - _time;
    m_actTime = _time;
    m_vActions.pop_front();
    m_CurrentAction.setDesireTime();
    return true;

}


xActionTrack::xActionTrack(xCoreActionMgr* pActMgr , HCoreAction hMixedAction , HBaseModel  hModel , const wchar_t* _name , int iSlot)
{
    m_hModel     = hModel;
    m_hMixAction = hMixedAction;
    m_name       = _name;
    m_pActionMgr = pActMgr;
    m_HashName = xStringHash(_name);
    m_actTime = 0;
    m_CurrentAction.setNULL();;
    m_iTransitionTime = 400; //过渡时间为400毫秒
    m_fTimeScale      = 1.0f;
    m_iSlot = iSlot;
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
                enterStandby();
            }
        }
    }
    return true;
}


xActorElement::xActorElement(IBaseRenderer* pRenderer , int v):IDrawElement(pRenderer)
{
	m_pActionFrame      = NULL;
    m_pActionMgr        = NULL;
    m_BaseActionStates  = NULL;
}

xActorElement::~xActorElement()
{
    m_Model.unload();
}

void xActorElement::center(XMathLib::xvec3&   _center)
{

}

void xActorElement::aabb(xGeomLib::xaabb&     _aabb)
{
	xBaseModel* pModel = m_Model.getResource();
	if(pModel)
	{
		pModel->aabb(_aabb);
	}
}

void xActorElement::shpere(xGeomLib::xshpere& _shpere)
{

}


bool xActorElement::render(unsigned long passedTime)
{
	xBaseModel* pModel = m_Model.getResource();
	if(pModel)
	{
		pModel->setupAnimation(m_pActionFrame);
		size_t nMesh = pModel->nMesh();
		for(size_t i = 0 ; i < nMesh ; i ++)
		{
			xCoreMesh* pMesh = pModel->getMesh(i);
			pMesh->draw(pModel->skeleton() , m_pActionFrame);
		}
	}
	return true;
}

bool xActorElement::update(unsigned long passedTime)
{
	xBaseModel* pModel = m_Model.getResource();
	xCoreSkeleton* pSkeleton = (pModel == NULL ? NULL : pModel->skeleton());
	if(pModel && pSkeleton)
	{
		int nSlot = (int)m_vActionTracks.size();
        for(int i = 0 ; i <nSlot ; i ++)
        {
            xActionTrack* pTrack = m_vActionTracks[i];
            pTrack->update(passedTime , pSkeleton);
            m_BaseActionStates[i].m_lTime = pTrack->actionTime();
        }
		
		//用混合的动作去混合;
        xMixedAction* pMixAction = resource_cast<xMixedAction*>(m_hMixAction);
        long time_is_ms[12] = {0};
        for(int i = 0 ; i < (int)m_vActionTracks.size() ; i ++)
        {
            time_is_ms[i] = m_BaseActionStates[i].m_lTime;
        }
        //pMixAction->blend(time_is_ms , m_pActionFrame , pSkeleton);
		pMixAction->blend(m_BaseActionStates , m_pActionFrame , pSkeleton);


	}
	return true;
}
bool xActorElement::unloadActionTracks()
{
    int nSlot = (int)m_vActionTracks.size();
    for(int i = 0 ; i < nSlot ; i ++)
    {            
        xActionTrack* pTrack = m_vActionTracks[i];
        delete pTrack;
    }
    m_vActionTracks.clear();
    XSAFE_DELETE_ARRAY(m_BaseActionStates);
    return true;
}
bool xActorElement::setModel(HBaseModel hModel)
{
    unloadActionTracks();
	xBaseModel* pModel = hModel.getResource();
	if(pModel)
	{	
		m_pActionFrame = NULL;
		if(m_pActionFrame && m_Model.getResource() )
		{
			m_Model->destoryActionFrame(m_pActionFrame);
		}
		this->m_Model = hModel;
		m_pActionFrame = m_Model->createActionFrame();

		return true;
	}
	return false;
}

void xActorElement::setMixAction(const wchar_t* actionXml)
{
    xBaseModel* pModel = m_Model.getResource();
    if(pModel == NULL)
    {	
        return ;
    }
    unloadActionTracks();
    //创建动作混合器
    //测试代码
    xCoreActionName _MixActionName(m_Model , L"表情混合测试" , pModel->name() , actionXml );
    _MixActionName.m_Type = L"Mix";
    m_hMixAction = m_pActionMgr->add(_MixActionName , 0 , true);
    m_hMixAction.confirm();

    xMixedAction* pMixAction = resource_cast<xMixedAction*>(m_hMixAction);

    int nSlot = pMixAction->nActionSlot();

    for(int i = 0 ; i < nSlot ; i ++)
    {            
        xMixedAction::xActionSlot&     actSlot = pMixAction->actionSlot( i );
        xActionTrack* pTrack = new xActionTrack(m_pActionMgr , m_hMixAction , m_Model , actSlot.m_strName.c_str() , i );
        m_vActionTracks.push_back(pTrack);
    }
    m_BaseActionStates = new xBaseActionState[nSlot];

}
void xActorElement::setActionMgr(xCoreActionMgr* pActMgr)
{
    m_pActionMgr = pActMgr ;
    int _nSlot = (int)m_vActionTracks.size();
    for(int i = 0  ; i < _nSlot ; i ++)
    {
        xActionTrack* pTrack = m_vActionTracks[i];
        pTrack->setActionMgr(pActMgr);
    }
}
void xActorElement::setTimeScale(float _scale)
{
    int _nSlot = (int)m_vActionTracks.size();
    for(int i = 0  ; i < _nSlot ; i ++)
    {
        xActionTrack* pTrack = m_vActionTracks[i];
        pTrack->setTimeScale(_scale);
    }
}
xActionTrack* xActorElement::findActionTrack(const wchar_t* _trackName)
{
    int hashName = xStringHash(_trackName);
    int _nSlot = (int)m_vActionTracks.size();
    for(int i = 0  ; i < _nSlot ; i ++)
    {
        if(m_vActionTracks[i]->hashName() == hashName)
            return m_vActionTracks[i];
    }
    return NULL;
}

bool xActorElement::isVisible(xGeomLib::xCamera* pCamera)
{
    return true;
}

END_NAMESPACE_XEVOL3D
