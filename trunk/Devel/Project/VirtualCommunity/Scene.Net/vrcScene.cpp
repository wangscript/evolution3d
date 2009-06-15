#include "stdafx.h"
#include "vrcScene.h"
namespace VirtualCommunity
{
	using namespace xEvol3DNet;
	xVRSceneManager::xVRSceneManager(xRenderApi^ renderApi):xVRSceneNode(NULL)
	{
		IRenderApi* pRenderApi = (IRenderApi*)renderApi->Handle;
		XVR_SceneManager* pMgr = XVR_SceneManager::createSceneManager(L"" , pRenderApi);
		m_pMgr = pMgr;
		m_pSceneNode = m_pMgr;
	}

	void xVRSceneManager::SelectNode(int x ,int y)
	{
		m_SelectedNode = getSelected(x , y);

	}
	bool xVRSceneManager::addModelNode(String^ modelFile , xVRSceneNode^ sceneNode)
	{
         const wchar_t* _modelFile = ToPtr(modelFile);
		 std::ds_wstring strFile = m_pMgr->modelManager()->relativePath(_modelFile);
		 if(strFile.length() == 0)
			 return false;
		 XEvol3D::xXmlNode xmlNode;
		 xmlNode.setValue(L"file",strFile.c_str());
		 xVR_SceneNode* pModel = m_pMgr->createNode(L"ModelNode",strFile.c_str(),&xmlNode , sceneNode->m_pSceneNode);
		 if(pModel == NULL)
			 return false;
		 return true;
	}

	bool xVRSceneManager::addModelPath(String^ modelPath)
	{
		std::wstring _modelPath = ToPtr(modelPath);
		_modelPath+=L"\\";
		m_pMgr->modelManager()->addPath(_modelPath.c_str());
		return true;
	}

	bool xVRSceneManager::addTexturePath(String^ texturePath)
	{
		std::wstring _modelPath = ToPtr(texturePath);
		_modelPath+=L"\\";
		m_pMgr->textureManager()->addPath(_modelPath.c_str());
		return true;
	}
}