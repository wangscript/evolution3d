#ifndef __XVRC_MODEL_SCENE_NODE_H__
#define __XVRC_MODEL_SCENE_NODE_H__

#include "vrcSceneNode.h"
#include "RenderAPI/xShaderManager.h"
USING_NS_XEVOL3D 
class _XEVOL_PLUGIN_API_ xVR_ModelNode : public xVR_SceneNode
{
	friend class           XVR_SceneManager;
	HBaseModel             m_hModel;
	std::wstring           m_modelFile;
	HGpuProgram            m_hGpuShader;
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xVR_ModelNode);
public:
	xVR_ModelNode(XVR_SceneManager* pSceneManager);
	~xVR_ModelNode();
	void                   setModel(HBaseModel hModel) { m_hModel = hModel ; }
	HBaseModel             getModel()                  { return m_hModel ;}
	bool                   render(unsigned long passedTime);
	bool                   update(unsigned long passedTime);
	const wchar_t*         type(){return L"ModelNode" ; }
	bool                   load(xXmlNode *);
	bool                   save(xXmlNode* pXMLNode);
};
#endif

