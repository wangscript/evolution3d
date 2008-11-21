#include "Stdafx.h"
#include "vrcModelNode.h"

IMPL_BASE_OBJECT_CLASSID(xVR_ModelNode , xVR_SceneNode);
xVR_ModelNode::xVR_ModelNode(XVR_SceneManager* pMgr)
:xVR_SceneNode(pMgr)
{
	m_RefCount  = 1;
}

xVR_ModelNode::~xVR_ModelNode()
{

}


bool xVR_ModelNode::update(unsigned long passedTime)
{
	m_hModel->updateFrame(passedTime);  
	return xVR_SceneNode::update(passedTime);
}


void xVR_ModelNode::render(unsigned long passedTime)
{
	IRenderApi* pRenderApi = renderApi();
	xPlacement* plcment    = &m_Placement;
	xMathLib::xmat4 transMat;
	bool inuse = plcment->isInUsed();
	getTrans( transMat );	
	pRenderApi->pushMatrix(MATRIXMODE_World);
	pRenderApi->setMatrix(transMat.data , MATRIXMODE_World);
	xVR_SceneNode::render(passedTime);
	pRenderApi->popMatrix(MATRIXMODE_World);
}
bool xVR_ModelNode::load(xCfgNode * pNode)
{
    XEVOL_WARNNING_NOT_IMPLEMENT;
	return true;
}
