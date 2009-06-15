// TestBase.cpp : Defines the entry point for the console application.
//

#include <BaseLib/xXmlDocument.h>
#include <BaseLib/xLogger.h>
#include <OperationSys/xOperationSys.h>
#include <Application/xPluginMgr.h>
#include <Application/xPlatform.h>
#include <Application/xApplication.h>
#include <RenderApi/xRenderApi.h>
#include <RenderAPI/xBaseShader.h>
#include <RenderAPI/xShaderNode.h>
#include <RenderAPI/xShaderManager.h>
#include <tchar.h>
#include <XMathLib/xMathlib.h>
#include <Image/xILImage.h>
#include <RenderAPI/xBaseTexture.h>
#include "xModel/xBaseModel.h"
#include "Renderer/xDrawableHelper.h"
#include "Image/xFreeImage.h"
#include "font/xDistanceFont.h"
using namespace XEvol3D;

class xTestFont
{
public:
    xDFFCharDesc* m_pCharDesc;
	IBaseTexture* m_pTexture;
	IRenderApi*   m_pRenderApi;
public:
	xTestFont();
	~xTestFont();
	void        unload();
	bool        load(IRenderApi* pRenderApi , const wchar_t * fileName , wchar_t ch);
	void        draw();
};
