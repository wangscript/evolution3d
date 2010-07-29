#include "stdafx.h"
#include "objPlacement.h"
#include "../resource.h"
#include "../xEvolEnviroment.h"
#include "BaseLib/xBaseMath.h"
#include "xSceneSelector.h"
USING_NS_MDED;
xInsertNodeCommand::xInsertNodeCommand(ISceneNode* pParent , ISceneNode* pThis , bool bInsert)
{
	m_pParent = pParent;
	m_pThis   = pThis;
	m_pThis->AddRef();
	m_pParent->AddRef();
	m_bInsert = bInsert;
}

xInsertNodeCommand::~xInsertNodeCommand()
{
	XSAFE_RELEASEOBJECT(m_pThis);
	XSAFE_RELEASEOBJECT(m_pParent);
}
bool xInsertNodeCommand::Redo()
{
	if(m_bInsert == true)
	{
        m_pParent->insertChild(m_pThis, m_pThis->name());
	}
	else
	{
		m_pParent->removeChild(m_pThis);
	}
	return true;
}

bool xInsertNodeCommand::Undo()
{
	if(m_bInsert == true)
	{
		m_pParent->removeChild(m_pThis   );
	}
	else
	{
		m_pParent->insertChild(m_pThis, m_pThis->name());
	}
	
	return true;
}

xInsertNodeCommand* xInsertNodeCommand::newInstacne(ISceneNode* pParent , ISceneNode* pThis , bool bInsert)
{
	return new xInsertNodeCommand(pParent , pThis , bInsert);
}


bool xPlacementCommand::Redo()
{
    *m_pSceneNode->placement() = m_NewPlacement;
	m_pSceneNode->updateChildrenTrans();
	return true;
}

bool xPlacementCommand::Undo()
{
	*m_pSceneNode->placement() = m_OldPlacement;
	m_pSceneNode->updateChildrenTrans();
	return true;
}

//===========
xObjPlacement::xObjPlacement()
{
    m_placementType = eOPT_None;
    m_pCommandInDoing = NULL;

    m_SceneToolbar.m_hDll = AfxGetResourceHandle();
    wcscpy(m_SceneToolbar.m_name , L"SceneOperator");
    wcscpy(m_SceneToolbar.m_title , L"场景操作");
    m_SceneToolbar.m_ResID = IDR_SCENEPLACEMENT;
    m_SceneToolbar.SetType(CMEdUiToolBarInfo::eToolBar);

    m_SceneToolbar.m_funcCallback = this;
    m_SceneToolbar.ProcessToolbarID(-1);
    GetMedusaEditor()->GetUI()->RegisteToolbar(&m_SceneToolbar);

}

xObjPlacement::~xObjPlacement() 
{
	GetMedusaEditor()->GetUI()->UnregisteToolbar(m_SceneToolbar.m_name );
}

void xObjPlacement::init(CEvolEnviroment* pEnv) 
{
	m_pEvolEnv = pEnv;
	m_SceneSelection = pEnv->GetSelection();

}

bool xObjPlacement::DeActive()
{
    IMEdUIMessageListenner* pOldListener = GetMedusaEditor()->GetUI()->GetMessageListenner();
    IMEdUIMessageListenner* pThisLister = dynamic_cast<IMEdUIMessageListenner*>(this);
	if(pOldListener == pThisLister)
	{
		GetMedusaEditor()->GetUI()->PopMessageListenner();
	}
    return true;
}

bool xObjPlacement::onActive(IMEdUIMessageListenner::eActiveReason _Reason)
{
    if(_Reason == IMEdUIMessageListenner::eAR_Active || _Reason == IMEdUIMessageListenner::eAR_Attach )
    {
        m_SceneSelection->showGizimo();     
		return true;
    }
    else if(_Reason == IMEdUIMessageListenner::eAR_DeActive)
	{
		DeActive();
	}
	else 
	{
        //取消了。不绘制
		m_placementType = eOPT_None;
		m_SceneSelection->setGizmoNode(NULL);
		setCurSelNode(NULL);
	}
    return true;
}

xvec4  xObjPlacement::getMouseMoveDir(int x , int y , float& movPercent)
{
	float dx = x - m_lastDragPos.x;
	float dy = y - m_lastDragPos.y;
	xTextureDesc _desc;
	IRenderApi* pRenderApi = m_pEvolEnv->renderApi();
	IRenderCamera* pCamera = m_pEvolEnv->camera();

	pRenderApi->getRenderView()->desc(_desc);

	movPercent = sqrt((float)dx * dx  + dy * dy)/sqrt((float)_desc.m_width * _desc.m_width + _desc.m_height * _desc.m_height );
	xvec4 up , left , dir;
	pCamera->getLUD(left , up , dir);
	xvec3 ModelPos ;
	if(GetCurSelNode() == NULL || GetCurSelNode()->placement() == NULL)
	{
		xvec4 movDir2(0.0f,0.0f);
		return movDir2;
	}
	GetCurSelNode()->placement()->getPosition( ModelPos);
	xvec4 _distDir = - pCamera->m_Eye.m_EyePos;
	_distDir.w = 0.0f;
	float _dist = _distDir.len();
	float tanFov2 = XM_SinD(pCamera->getFOV()/2.0f ) / XM_CosD( pCamera->getFOV()/2.0f );

	float _screen_dist = _desc.m_height /2.0f / tanFov2; 
	float _maxH = tanFov2 * _dist * 2.0f;
	float _maxW = pCamera->getAspect() * _maxH ;


	float facX =  dx / float(_desc.m_width)   * _maxW;
	float facY = -dy / float(_desc.m_height)  * _maxH;

	xvec4 movDir = facY * up + facX * left;

	float s = _dist / _screen_dist;
	xvec4 movDir2 = dx * s * left  - dy  * s * up;
	return movDir2;
}

void xObjPlacement::setCurSelNode(ISceneNode* pNode)
{
	return m_SceneSelection->setCurSelNode(pNode);
}

ISceneNode* xObjPlacement::GetCurSelNode()
{
	return m_SceneSelection->GetCurSelNode();
}




bool xObjPlacement::onMessage(NS_XEVOL3D::xWindowMsg& msg)
{
	if(msg.MsgID == NS_XEVOL3D::WIN_KEYUP)
	{
		if(msg.Keyboard.nSysVKey == VK_ESCAPE)
		{
			IMEdUIMessageListenner* pOldListener = GetMedusaEditor()->GetUI()->GetMessageListenner();
			IMEdUIMessageListenner* pThisLister = dynamic_cast<IMEdUIMessageListenner*>(this);
            if(pOldListener == pThisLister)
            {
                GetMedusaEditor()->GetUI()->PopMessageListenner();
            }
            return true;
		}
	}
	if(msg.MsgID == NS_XEVOL3D::WIN_LMOUSE_DOWN)
	{
		xTextureDesc _desc;
		int x = msg.Mouse.x;
		int y = msg.Mouse.y;
        unsigned int v = 0;
		xSceneSelector _selector(m_pEvolEnv);
		_selector.begin(x - 4 ,  y - 4 , 8 ,  8);
		ISceneNode* pSelNode = _selector.getSelect(x , y , v);
		m_RotateAxis = -1;
		bool hasHitGizmo = _selector.checkPoint(m_SceneSelection->GetRotGizmo() , x , y , 4 , v);
        
		if(hasHitGizmo)
		{
			m_lastDragPos = xvec2i(x , y);
			m_RotateAxis = v ;
			_selector.end();
			return true;
		}

		//如果是Node不属于场景。就啥也不做。
		if(pSelNode && pSelNode->bInScene() == false )
		{
			return true;
		}

		
		m_SceneSelection->setGizmoNode(NULL);
        setCurSelNode(NULL);
        if(pSelNode)
		{
			setCurSelNode(pSelNode);
		}
		if( GetCurSelNode() )
		{
			m_bInDrag     = true;
			m_lastDragPos = xvec2i(x , y);
			if(m_placementType == eOPT_Rotate)
			{
				m_SceneSelection->setRotGizmo();
			}
			else if(m_placementType == eOPT_Move)
			{
				m_SceneSelection->setMoveGizmo();
			}

			//开始拖动了，记录一个Command;
			m_pCommandInDoing= new xPlacementCommand;
			m_pCommandInDoing->m_pSceneNode = GetCurSelNode();
			m_pCommandInDoing->m_OldPlacement = *( GetCurSelNode()->placement() );
		}
		return true;
	}

	if(msg.MsgID == NS_XEVOL3D::WIN_MOUSE_MOVE)
	{
		if(GetCurSelNode() == NULL)
			return true;

		if(GetCurSelNode() == NULL || GetCurSelNode()->placement() == NULL)
			return true;

		int x = msg.Mouse.x;
		int y = msg.Mouse.y;
		if(m_bInDrag)
		{
			float movPercent;
			if(m_SceneSelection->GetCurSelNode() == NULL)
				return false;
			xvec4 movDir = getMouseMoveDir(x , y , movPercent);
			xvec3 ModelPos ;

			GetCurSelNode()->placement()->getPosition( ModelPos);
			ModelPos = ModelPos + movDir;
			GetCurSelNode()->placement()->setPosition(ModelPos);
			GetCurSelNode()->invalidatePlacement();
		}
		else if(m_RotateAxis >= 0 )
		{
			float movPercent;
			xvec4 xAxis = xvec4(1.0f,0.0f,0.0f,1.0f);
			xvec4 yAxis = xvec4(0.0f,1.0f,0.0f,1.0f);
			xvec4 zAxis = xvec4(0.0f,0.0f,1.0f,1.0f);
			xvec4 movDir = getMouseMoveDir(x , y , movPercent);
			movDir.normalize();

			if(m_RotateAxis == 0)//XY
			{
				xvec4 RotAxis = zAxis;
				xvec4 rotateDir = movDir.cp(RotAxis);
				xvec4 cpAxis = yAxis + xAxis;
				float factor = rotateDir.dp3(cpAxis);
				float angle = factor * 90.0f * movPercent * 24.0f;
				xquat q1 , q2 , q;
				q1.set( xvec3( 0.0f , 0.0f , 1.0f ) , angle);
				GetCurSelNode()->placement()->getRotate( q2 );
				XM_Mul(q2,q1,q);
				GetCurSelNode()->placement()->setRotate(q);
				GetCurSelNode()->invalidatePlacement();
				//xmat4 mat = m_ModelRotate * rotMat;
				//m_ModelRotate = mat;
			}
			if(m_RotateAxis == 1)//YZ
			{
				xvec4 RotAxis = xAxis;
				xvec4 rotateDir = movDir.cp(RotAxis);
				xvec4 cpAxis = yAxis + zAxis;
				float factor = rotateDir.dp3(cpAxis);
				float angle = factor * 90.0f * movPercent * 24.0f;
				xquat q1 , q2 , q;
				q1.set( xvec3(1.0f , 0.0f , 0.0f ) , angle);

				GetCurSelNode()->placement()->getRotate( q2 );
				XM_Mul(q2,q1,q);
				GetCurSelNode()->placement()->setRotate(q);
				GetCurSelNode()->invalidatePlacement();
				//xmat4 mat = m_ModelRotate * rotMat;
				//m_ModelRotate = mat;
			}
			if(m_RotateAxis == 2)//XZ
			{
				xvec4 RotAxis = yAxis;
				xvec4 rotateDir = movDir.cp(RotAxis);
				xvec4 cpAxis = xAxis + zAxis;
				float factor = rotateDir.dp3(cpAxis);
				float angle = factor * 90.0f * movPercent * 24.0f;
				xquat q1 , q2 , q;
				q1.set( xvec3( 0.0f  , 1.0f , 0.0f ) , angle);
				GetCurSelNode()->placement()->getRotate( q2 );
				XM_Mul(q2,q1,q);
				GetCurSelNode()->placement()->setRotate(q);
				GetCurSelNode()->invalidatePlacement();
			}

		}
		m_lastDragPos = xvec2i(x , y);
		return true;
	}
	if(msg.MsgID == NS_XEVOL3D::WIN_LMOUSE_RELEASE)
	{
		int x = msg.Mouse.x;
		int y = msg.Mouse.y;
		m_bInDrag = false;
		m_RotateAxis = -1;
		m_lastDragPos = xvec2i(x , y);

		//结束拖动了。放一个Undo到里面去
		if(m_pCommandInDoing)
		{
			m_pCommandInDoing->m_NewPlacement = *(m_pCommandInDoing->m_pSceneNode->placement() );
			GetMedusaEditor()->GetUI()->InsertCommand(m_pCommandInDoing);
			m_pCommandInDoing = NULL;
		}
		return true;
	}
	return false;
}

bool  xObjPlacement::OnUpdateCommandUI(int ctrlID , int ctrlIdx , NS_MDED::CMEdUiToolBarInfo::CMEdUiCmdUi* pUi)
{
	if(ctrlID == ID_BTN_SCENE_OP_MOVE)
	{
		if(m_placementType == eOPT_Move)
             pUi->SetCheck(TRUE);
		else 
            pUi->SetCheck(FALSE);
	}

	if(ctrlID == ID_BTN_SCENE_OP_ROTATE)
	{
		if(m_placementType == eOPT_Rotate)
            pUi->SetCheck(TRUE);
        else 
            pUi->SetCheck(FALSE);
	}
	return true;
}

bool  xObjPlacement::SendCommand(xObjPlacement::eCommand _cmd)
{
    if(_cmd == eCmd_Move  ) return OnCommand( ID_BTN_SCENE_OP_MOVE   , 0);
    if(_cmd == eCmd_Rotate) return OnCommand( ID_BTN_SCENE_OP_ROTATE , 0);
    return true;
}

bool  xObjPlacement::OnCommand(int ctrlID , int ctrlIdx)
{
	switch(ctrlID)
	{
	case ID_BTN_SCENE_OP_MOVE:
		{
			IMEdUIMessageListenner* pOldListener = GetMedusaEditor()->GetUI()->GetMessageListenner();
			IMEdUIMessageListenner* pThisLister = dynamic_cast<IMEdUIMessageListenner*>(this);
			if(pOldListener != pThisLister)
			{
				GetMedusaEditor()->GetUI()->PushMessageListenner(this);
			}
			this->m_placementType = eOPT_Move;
			m_SceneSelection->setMoveGizmo();;
		}
		break;
	case ID_BTN_SCENE_OP_ROTATE:
		{
			IMEdUIMessageListenner* pOldListener = GetMedusaEditor()->GetUI()->GetMessageListenner();
			IMEdUIMessageListenner* pThisLister = dynamic_cast<IMEdUIMessageListenner*>(this);
			if(pOldListener != pThisLister)
			{
				GetMedusaEditor()->GetUI()->PushMessageListenner(this);
			}
			this->m_placementType = eOPT_Rotate;
			m_SceneSelection->setRotGizmo();
		}
		break;
	}
	return true;
}