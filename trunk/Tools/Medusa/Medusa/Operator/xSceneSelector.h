#pragma once
#include "Application/xWindow.h"
#include "xScene/xSceneHelperDrawable.h"
#include "Renderer/xDrawableHelper.h"
#include "../xMedusaEditor.h"

using namespace xMathLib;
class CEvolEnviroment;
USING_NS_XEVOL3D;

class MEDUSA_API xSceneSelection
{
public:
    ISceneNode*                    m_pSelSceneNode;
    xSceneHelperDrawableNode*      m_pGizmo;
    IDrawableHelper*               m_ArcBall;
    IDrawableHelper*               m_Axis;
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
    ISceneNode*      GetRotGizmo(){ return m_pGizmo ; }
    ISceneNode*      GetGizmo(){return m_pCurGizmo ; }
    bool             IsGizmoNode(ISceneNode* pNode);
    xSceneSelection();
    ~xSceneSelection();

};

class MEDUSA_API xSceneSelector
{
public:
    ISceneNode*      getSelect(int x , int y , unsigned int& v);
    void             begin(int x , int y , int w , int h);
	bool             checkPoint(ISceneNode* pNode , int x , int y , int BoundSize , unsigned int& v);
	void             end();
	~xSceneSelector();
	xSceneSelector(CEvolEnviroment* pEnv) ;
protected:
	xvec2i*           m_pSelectData;
	xvec2i            m_lastDragPos;
	int               m_RotateAxis;
	CEvolEnviroment*  m_pEvolEnv;
	xTextureDesc      m_TextDesc;
	int               m_x ;
	int               m_y ;
	int               m_w ;
	int               m_h ;
};