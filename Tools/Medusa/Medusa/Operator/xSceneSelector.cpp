#include "stdafx.h"
#include "xSceneSelector.h"
#include "../resource.h"
#include "../xEvolEnviroment.h"
#include "BaseLib/xBaseMath.h"
USING_NS_MDED;

xSceneSelection::xSceneSelection()
{
    m_pCurGizmo     = NULL;
    m_pSelSceneNode = NULL;
    m_pGizmo    = NULL;
    m_ArcBall       = NULL;
    m_Axis = NULL;

}

bool xSceneSelection::init(CEvolEnviroment*     pEvolEnv)
{
    m_pEvolEnv  = pEvolEnv;
    m_ArcBall = (IDrawableHelper *)xDrawElementFactoryMgr::singleton()->createInstance(L"ArcBall" , m_pEvolEnv->renderer() , 0) ;
    m_ArcBall->init(pEvolEnv->texMgr() );

    m_Axis    = (IDrawableHelper *)xDrawElementFactoryMgr::singleton()->createInstance(L"Axis"    , m_pEvolEnv->renderer() , 0) ;
    m_Axis->init(pEvolEnv->texMgr() );

    return true;
}

xSceneSelection::~xSceneSelection()
{

}

void xSceneSelection::showGizimo()
{
    if(m_pCurGizmo == NULL)
        return ;
    m_pCurGizmo->attachToScene( m_pEvolEnv->scene() );

    //根据父节点,设置Gizmo的大小和位置
    ISceneNode* pParent = m_pCurGizmo->getParent();
    if(pParent)
    {
        xGeomLib::xaabb aabb;
        aabb.Init(); 
        size_t nObj = pParent->nObjects();
        for(size_t i = 0 ; i < nObj ; i ++ )
        {
            ISceneDrawable* pDrawable = dynamic_cast<ISceneDrawable*>(pParent->getObject(i));
            size_t nEl = pDrawable->nDrawElement();
            for(size_t iEl = 0 ; iEl < nEl  ; iEl ++)
            {
                xGeomLib::xaabb _aabb;
                if(pDrawable && pDrawable->drawElement(iEl) )
                {
                    pDrawable->drawElement(iEl)->aabb(_aabb);	
                    aabb.AddPoint(_aabb.m_Min);
                    aabb.AddPoint(_aabb.m_Max);
                }
            }

        }
        xvec3 cen = (aabb.m_Max + aabb.m_Min) * 0.5f;
        xvec3 len = aabb.m_Max - aabb.m_Min;
        float _len = len.len() * 0.5f;
        m_pCurGizmo->placement()->setPosition(cen);
        m_pCurGizmo->placement()->setScale(xvec3(_len , _len , _len ));
        m_pCurGizmo->invalidatePlacement();
    }
}


void xSceneSelection::setNullGizmo()
{
    setGizmoNode(NULL);
}

void xSceneSelection::setCurSelNode(ISceneNode* pNode)
{
    //当前的Gizmo Node是不能参与选择的。
    if(IsGizmoNode(pNode))
        return ;
    GetMedusaEditor()->GetUI()->fireMEdUIEvent(eMEUI_SceneSelChanged , (int)pNode , 0);
    setGizmoNode(NULL);
    m_pSelSceneNode = pNode;
}

ISceneNode* xSceneSelection::GetCurSelNode()
{
    return m_pSelSceneNode;
}
bool xSceneSelection::IsGizmoNode(ISceneNode* pNode)
{
    if(dynamic_cast<ISceneNode*>(m_pGizmo) == pNode)
        return true;
    if(dynamic_cast<ISceneNode*>(m_pCurGizmo)  == pNode)
        return true;
    return false;
}

void xSceneSelection::setRotGizmo()
{
    if(m_pGizmo == NULL)
    {
        m_pGizmo = dynamic_cast<xSceneHelperDrawableNode*>(XEvol_CreateSceneNode(L"xSceneHelperDrawableNode" , m_pEvolEnv->scene() , NULL) );
        if ( NULL == m_pGizmo )
            return;
    }
    m_pGizmo->detachDrawElement(m_Axis);
    m_pGizmo->attachDrawElement(m_ArcBall);
    xvec3 _scale(10.0f, 10.0f , 10.f);
    m_pGizmo->placement()->setScale( _scale );
    m_pGizmo->setInScene(false);
    setGizmoNode(m_pGizmo);
}
void xSceneSelection::setMoveGizmo()
{
    if(m_pGizmo == NULL)
    {
        m_pGizmo = dynamic_cast<xSceneHelperDrawableNode*>(XEvol_CreateSceneNode(L"xSceneHelperDrawableNode" , m_pEvolEnv->scene() , NULL) );
        if ( NULL == m_pGizmo )
            return;
    }
    m_pGizmo->detachDrawElement(m_ArcBall);
    m_pGizmo->attachDrawElement(m_Axis);
    xvec3 _scale(30.0f, 30.0f , 30.f);
    m_pGizmo->placement()->setScale( _scale );
    m_pGizmo->setInScene(false);
    setGizmoNode(m_pGizmo);
}
void xSceneSelection::setGizmoNode(ISceneNode* pNode)
{
    if(pNode == NULL)
    {
        if(m_pSelSceneNode) 
        {
            m_pSelSceneNode->removeChild(m_pGizmo);
        }
        return ;
    }
    if(m_pSelSceneNode )
    {
        m_pSelSceneNode->insertChild(pNode);
        pNode->updateChildrenTrans();
    }
    m_pCurGizmo = dynamic_cast<xSceneHelperDrawableNode*>(pNode);
    showGizimo();

}


//////////////////////////////////////////////////////////////////////////
ISceneNode* xSceneSelector::getSelect(int x , int y , unsigned int& v)
{
     if(m_pSelectData == NULL)
	 {
		 unsigned int v;
		 m_pSelectData = m_pEvolEnv->GetSelectData(m_TextDesc , x , y , 1 , 1 );
		 xvec2i& val = *m_pSelectData;
		 ISceneNode* pSelNode = NULL;
		 if(val.x > 10000)
		 {
			 //val.x = xRoundToByte(val.x, 4);
			 pSelNode = dynamic_cast<ISceneNode*> ( (IBaseObject*)val.x);
		 }
		 m_pEvolEnv->ReleaseSelectData(m_pSelectData);
		 m_pSelectData = NULL;
		 return pSelNode;
	 }

	 //选取数据已经准备好了
	 int dx = x - m_x;
	 int dy = y - m_y;

	 xvec2i& val = m_pSelectData[ dy * m_w + dx];

	 ISceneNode* pSelNode = NULL;
	 if(val.x > 10000)
	 {
		 //val.x = xRoundToByte(val.x, 4);
		 pSelNode = dynamic_cast<ISceneNode*> ( (IBaseObject*)val.x);
	 }
	 return pSelNode;

}

void xSceneSelector::begin(int x , int y , int w , int h)
{
	if(m_pSelectData != NULL)
	{
		m_pEvolEnv->ReleaseSelectData(m_pSelectData);
		m_pSelectData = NULL;
	}
	m_pSelectData = m_pEvolEnv->GetSelectData2(m_TextDesc , x , y , w , h);
	m_x = x ; 
	m_y = y ;
	m_w = w ;
	m_h = h ;
}

bool xSceneSelector::checkPoint(ISceneNode* pNode , int x , int y , int BoundSize , unsigned int& v)
{
	BoundSize = BoundSize/2;
	for(int _i = -BoundSize ; _i < BoundSize ; _i ++)
	{
		for(int _j = -BoundSize ; _j < BoundSize ; _j ++)
		{
			int _x = (x + _i) - m_x;
			int _y = (y + _j) - m_y;
			if(_x >=0 && _x < m_w && _y >= 0 && _y < m_h)
			{
				xvec2i& val = m_pSelectData[ _y * m_w + _x];
				if(val.x == (int)pNode )
				{ 
					v = val.y;
					return true;
				}
			}		
		}
	}
	return false;
}

void xSceneSelector::end()
{
	m_pEvolEnv->ReleaseSelectData(m_pSelectData);
	m_pSelectData = NULL;
}
xSceneSelector::~xSceneSelector()
{
	m_pEvolEnv->ReleaseSelectData(m_pSelectData);
}

xSceneSelector::xSceneSelector(CEvolEnviroment* pEnv)
{
	m_pSelectData = NULL;
	m_pEvolEnv = pEnv;
}