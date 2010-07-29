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

void xActorElement::xActionStateListenner::onActionTrackEvent(eActionTrackEvent _event, int idx ,  xBaseAction* pAction)
{
    m_pElement->onActionTrackEvent(_event , idx , pAction);
}

void xActorElement::xActionStateListenner::onActionTrackEvent(eActionTrackEvent _event, int idx ,  HCoreAction  hAction)
{
    m_pElement->onActionTrackEvent(_event , idx , hAction);
}

xActorElement::xActorElement(IBaseRenderer* pRenderer , int v):IDrawElement(pRenderer)
{
	m_pActionFrame      = NULL;
    m_pActionMgr        = NULL;
    m_BaseActionStates  = NULL;
    m_Listenner.m_pElement = this;
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
void xActorElement::onActionTrackEvent(eActionTrackEvent _event, int idx ,  xBaseAction* pAction)
{
    if(_event == eActionChange)
    {
        xMixedAction* pMixedAction = resource_cast<xMixedAction*>(m_hMixAction); 
        if(pMixedAction)
        {
            pMixedAction->setAction(idx, pAction );
        }
    }
}

void xActorElement::onActionTrackEvent(eActionTrackEvent _event, int idx ,  HCoreAction  hAction)
{
    if(_event == eActionChange)
    {
        xMixedAction* pMixedAction = resource_cast<xMixedAction*>(m_hMixAction); 
        if(pMixedAction)
        {
            pMixedAction->setAction(idx, hAction );
        }
    }

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
    xCoreActionName _MixActionName(eActType_Mix , m_Model , L"表情混合测试" , pModel->name() , actionXml );
    _MixActionName.m_Ext = L"Mix";
    m_hMixAction = m_pActionMgr->loadAction(_MixActionName , 0 , true);
    m_hMixAction.confirm();

    xMixedAction* pMixAction = resource_cast<xMixedAction*>(m_hMixAction);

    int nSlot = pMixAction->nActionSlot();

    for(int i = 0 ; i < nSlot ; i ++)
    {            
        xMixedAction::xActionSlot&     actSlot = pMixAction->actionSlot( i );
        xActionTrack* pTrack = new xActionTrack(m_pActionMgr , m_Model , actSlot.m_strName.c_str() , i );
        pTrack->addListenner( &m_Listenner );
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

bool xActorElement::isVisible(xGeomLib::xCamera* pCamera, const xMathLib::xmat4& _mat)
{
    return true;
}

END_NAMESPACE_XEVOL3D
