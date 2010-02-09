#ifndef _XEVOLUTION3D_H_
#define _XEVOLUTION3D_H_
#include "BaseLib/xEvol3DBaseInc.h"
#include "xModel/xModelFWDecl.h"
#include "RenderAPI/xTextureManager.h"
#include "XMathLib/xGeomLib.h"
#include "XMathLib/xcamera.h"

BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ IPlatform;
class _XEVOL_BASE_API_ IWindow;
class _XEVOL_BASE_API_ IRenderApi;
class _XEVOL_BASE_API_ xXmlDocument;
class _XEVOL_BASE_API_ xXmlNode;
class _XEVOL_BASE_API_ IApplication;
class _XEVOL_BASE_API_ ISceneGraph;
class _XEVOL_BASE_API_ IBaseRenderer;
class _XEVOL_BASE_API_ xEvol3DEngine
{
protected:
	xXmlDocument*             m_cfgDocument;
	IPlatform*                m_platform;
	IWindow*                  m_pWindow ;
	IRenderApi*               m_pRenderApi;
	std::ds_wstring           m_cfgFile;

	xBaseTextureMgr*          m_pTexMgr;
	xBaseModelMgr*            m_pModelMgr;
	xResPathManager           m_ResPathMgr;
	typedef ds_map(std::ds_wstring , xResPathManager*) mapResPathManagers;
	mapResPathManagers        m_ResPathManagers;
public:
	xEvol3DEngine();
	~xEvol3DEngine();
	IRenderApi*      renderApi();
	bool             exit();
	bool             initCfg(const wchar_t* cfgFile);
	bool             initPlatform(IApplication* pApp , const wchar_t* cfgFile, const wchar_t* pluginDir);
    bool             init(IApplication* pApp , void* hWnd  , const wchar_t* cfgFile, const wchar_t* projectName ,  const wchar_t* argv , int argc, xXmlNode* pRenderParams = NULL);
	bool             initRenderer( const wchar_t* projectName, void* hWnd , xXmlNode* pCreateParams = NULL);
	IPlatform*       platform(){ return m_platform ; }
	xResPathManager* getResPathMgr();
	xResPathManager* getResPathMgr(const wchar_t* pName);
	xBaseModelMgr*   modelMgr();
	xBaseTextureMgr* textureManager();
	bool             loadCamera(xGeomLib::xCamera* pCamera , xXmlNode* pCameraNode);
	bool             saveCamera(xGeomLib::xCamera* pCamera , xXmlNode* pCameraNode);

	bool             messageLoop();
	//ISceneGraph*    createScene(const wchar_t* _sceneFile , IBaseRenderer* pRenderer);
	xXmlNode*        sysConfig();
	int              sysValue(const wchar_t* sector , const wchar_t* _value, int defaultValue);
	const wchar_t*   sysValue(const wchar_t* sector , const wchar_t* _value, const wchar_t* defValue);
	float            sysValue(const wchar_t* sector , const wchar_t* _value, float defValue);     
	bool             sysValue(const wchar_t* sector , const wchar_t* _value, bool defValue);
protected:
	xXmlNode*        sysCfgNode(const wchar_t* sector , bool bCreated = false);
}; 
END_NAMESPACE_XEVOL3D

#endif
