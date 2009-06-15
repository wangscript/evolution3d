#include "vrcModelNode.h"
#include "vrcSceneManager.h"
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


bool xVR_ModelNode::render(unsigned long passedTime)
{
	IRenderApi* pRenderApi = renderApi();
	xPlacement* plcment    = &m_Placement;
	xMathLib::xmat4 transMat;
	bool inuse = plcment->isInUsed();
	getTrans( transMat );	
	pRenderApi->pushMatrix(MATRIXMODE_World);
	pRenderApi->setMatrix(transMat.data , MATRIXMODE_World);
	xVR_SceneNode::render(passedTime);
	m_pRenderApi->colorSelector()->setRenderObjMasterID((int)this);
	m_pRenderApi->pushGpuProgram(m_hGpuShader.getResource() );
	m_hModel->draw();
	m_pRenderApi->popGpuProgram();
	pRenderApi->popMatrix(MATRIXMODE_World);
	return true;
}
bool xVR_ModelNode::load(xXmlNode * pNode)
{
	xVR_SceneNode::load(pNode);
    m_modelFile = pNode->value(L"file");
	m_hModel = m_pSceneManager->addModel(m_modelFile.c_str() );

	xGpuProgNameParser _parser;
	_parser.setShaderName( eShader_VertexShader , L"simpleMesh.vertex");
	if(m_hModel->skeleton())
	{
		_parser.addShaderNode( eShader_VertexShader , L"SkinAni" , (size_t)0);
	}
	_parser.setShaderName( eShader_PixelShader , L"simpleMesh.pixel<0:simple.texture;>" );
	xGpuProgramName _modelShaderName;
	_parser.toName(_modelShaderName);
	m_hGpuShader = m_pRenderApi->gpuProgramManager()->load(_modelShaderName);

	return true;
}
bool xVR_ModelNode::save(xXmlNode* pXMLNode)
{
	pXMLNode->setValue(L"file",m_modelFile.c_str() );
	return true;
}