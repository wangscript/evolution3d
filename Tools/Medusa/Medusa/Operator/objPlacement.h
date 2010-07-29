#pragma once

#include "Application/xWindow.h"
#include "xScene/xSceneHelperDrawable.h"
#include "Renderer/xDrawableHelper.h"
#include "../xMedusaEditor.h"
#include "xSceneSelector.h"
using namespace xMathLib;
class CEvolEnviroment;
USING_NS_XEVOL3D;

enum eObjPlacementType
{
	eOPT_None,
	eOPT_Move,
	eOPT_Scale,
	eOPT_Rotate,
};

class MEDUSA_API xPlacementCommand : public nsMedusaEditor::IMEdUICommand
{
	IMPL_REFCOUNT_OBJECT_INTERFACE(xPlacementCommand);
public:
	bool Redo();
	bool Undo();
public:
	ISceneNode* m_pSceneNode;
	xPlacement  m_OldPlacement;
	xPlacement  m_NewPlacement;
};

class MEDUSA_API xInsertNodeCommand : public nsMedusaEditor::IMEdUICommand
{
	IMPL_REFCOUNT_OBJECT_INTERFACE(xInsertNodeCommand);
public:
	xInsertNodeCommand(ISceneNode* pParent , ISceneNode* pThis , bool bInsert = true);
	~xInsertNodeCommand();
	bool Redo();
	bool Undo();
	static xInsertNodeCommand* newInstacne(ISceneNode* pParent , ISceneNode* pThis , bool bInsert = true);
public:
	ISceneNode* m_pParent;
	ISceneNode* m_pThis;
	bool        m_bInsert;
};


class MEDUSA_API xObjPlacement : public NS_MDED::IMEdUIMessageListenner , public NS_MDED::CMEdUiToolBarInfo::MEdUIToolbarCallback
{
public:
    enum eCommand
    {
        eCmd_Move,
        eCmd_Rotate,
        eCmd_Scale,
    };
public:
    bool              DeActive();
	bool              onMessage(NS_XEVOL3D::xWindowMsg& msg);
	bool              onActive(IMEdUIMessageListenner::eActiveReason _Reason);
	virtual bool      OnUpdateCommandUI(int ctrlID , int ctrlIdx, NS_MDED::CMEdUiToolBarInfo::CMEdUiCmdUi* pUi);
	virtual bool      OnCommand(int ctrlID , int ctrlIdx);
    virtual bool      SendCommand(eCommand _cmd);
	xvec4             getMouseMoveDir(int x , int y , float& movPercent);
	void              setCurSelNode(ISceneNode* pNode);
	ISceneNode*       GetCurSelNode();
    eObjPlacementType placementType() { return m_placementType ; }
    xObjPlacement();
    ~xObjPlacement();
	void init(CEvolEnviroment* pEnv) ;
protected:
	bool                           m_bInDrag;
	xvec2i                         m_lastDragPos;
	int                            m_RotateAxis;
	CEvolEnviroment*               m_pEvolEnv;	
	eObjPlacementType              m_placementType;
	xSceneSelection*               m_SceneSelection;
	xPlacementCommand*             m_pCommandInDoing;

	NS_MDED::CMEdUiToolBarInfo     m_SceneToolbar;
};