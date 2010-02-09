// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

using namespace ATL;

#include "xModel/xCoreActionMgr.h"
#include "XMathLib/xspline.h"
#include "xEvol3D.h"
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

#pragma warning(disable : 4251)
#pragma warning(disable : 4799)
#pragma warning(disable : 4201)
#pragma warning(disable : 6011)
#pragma warning(disable : 6031)
#pragma warning(disable : 6385)
#pragma warning(disable : 4819)

#ifdef _WIN32
extern HINSTANCE Global_hModuleHandle ;
#endif
