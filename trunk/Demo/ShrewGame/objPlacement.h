#pragma once
#include "Application/xWindow.h"
#include "xScene/xSceneHelperDrawable.h"
#include "Renderer/xDrawableHelper.h"
class CEvolEnviroment;
using namespace xMathLib;
USING_NS_XEVOL3D;

enum eObjPlacementType
{
	eOPT_None,
	eOPT_Move,
	eOPT_Scale,
	eOPT_Rotate,
};

class xSceneSelection
{
public:
	ISceneNode*                    m_pSelSceneNode;
	xSceneHelperDrawableNode*      m_pRotGizimo;
	IDrawableHelper*               m_ArcBall;
	CEvolEnviroment*               m_pEvolEnv;
	xSceneHelperDrawableNode*      m_pCurGizmo;
public:
	bool             init(CEvolEnviroment*     pEvolEnv);
	void             showGizimo();
	void             setRotGizmo();
	void             setMoveGizmo();
	void             setNullGizmo();
	void             setGizmoNode(ISceneNode* pNode);
	void             setCurSelNode(ISceneNode* pNode);
	ISceneNode*      GetCurSelNode();
	ISceneNode*      GetRotGizmo(){ return m_pRotGizimo ; }
	ISceneNode*      GetGizmo(){return m_pCurGizmo ; }
	bool             IsGizmoNode(ISceneNode* pNode);
	xSceneSelection();
	~xSceneSelection();

};

class xObjPlacement
{
public:
	bool             onMessage(NS_XEVOL3D::xWindowMsg& msg);
	bool             onActive(bool bActive);
	xvec4            getMouseMoveDir(int x , int y , float& movPercent);
	void             setCurSelNode(ISceneNode* pNode);
	ISceneNode*      GetCurSelNode();
	bool             SetMoveOperator();
	bool             SetRotateOperator();
    xObjPlacement(xSceneSelection*  pSceneSelection);

	void init(CEvolEnviroment* pEnv) ;
protected:
	bool                           m_bInDrag;
	xvec2i                         m_lastDragPos;
	int                            m_RotateAxis;
	CEvolEnviroment*               m_pEvolEnv;	
	eObjPlacementType              m_placementType;
	xSceneSelection*               m_SceneSelection;
};