// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//



#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <stdio.h>
#include <tchar.h>


#include <BaseLib/xXmlDocument.h>
#include <BaseLib/xStringTable.h>
#include <BaseLib/xLogger.h>
#include "Image/xImage.h"
#include <OperationSys/xOperationSys.h>
#include <Application/xPluginMgr.h>
#include <Application/xPlatform.h>
#include <Application/xApplication.h>
#include <RenderApi/xRenderApi.h>
#include <RenderAPI/xBaseShader.h>
#include <RenderAPI/xShaderNode.h>
#include <RenderAPI/xShaderManager.h>
#include <RenderAPI/xRenderView.h>
#include <RenderAPI/xTLEnvioroment.h>
#include <Renderer/xRenderer.h>
#include <Renderer/xModelElement.h>
#include "xui/xuiWindowManager.h"
#include <tchar.h>
#include <XMathLib/xMathlib.h>
#include <Image/xILImage.h>
#include <RenderAPI/xBaseTexture.h>
#include "xModel/xBaseModel.h"
#include "Renderer/xDrawableHelper.h"
#include "Image/xFreeImage.h"
#include "xScene/xSceneGraph.h"
#include "xScene/xSceneSimpleVistor.h"
#include "objPlacement.h"
#include "xuiDemoInfoPannel.h"
#include "xEvol3D.h"
#include "xEffectSystem/xParticleSystem.h"
#include "xEffectSystem/xParticleEmitter.h"

// TODO: reference additional headers your program requires here
