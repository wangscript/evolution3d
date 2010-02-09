// Evol3D.Net.h

#pragma once

#include "BaseLib/xXmlDocument.h"
#include "BaseLib/xLogger.h"
#include "OperationSys/xOperationSys.h"
#include "Application/xPluginMgr.h"
#include "Application/xPlatform.h"
#include "Application/xApplication.h"
#include "RenderApi/xRenderApi.h"
#include "RenderAPI/xBaseShader.h"
#include "RenderAPI/xShaderNode.h"
#include "RenderAPI/xShaderManager.h"
#include <tchar.h>
#include "XMathLib/xMathlib.h"
#include "Image/xILImage.h"
#include "RenderAPI/xBaseTexture.h"
#include "RenderAPI/xTextureManager.h"
#include "RenderAPI/xRenderAPI.h"

#include "Renderer/xDrawableHelper.h"

#include "Evol3D.Net.h"
#include "xDotNetUlti.h"
#include "xnMath.h"
#include "xnRenderApi.h"
using namespace XEvol3D;
using namespace System;

namespace xEvol3DNet 
{
	public ref   class xDrawable 
	{
	internal:
		IDrawElement* m_pObject;
		xDrawable(IDrawElement*  pObject): m_pObject(pObject) { m_pObject->AddRef() ; };
		~xDrawable(){m_pObject->ReleaseObject() ; };
		xAABB^    _aabb   (  )  {xAABB^    _ret = gcnew xAABB()     ; m_pObject->aabb(_ret->aabb()    ); return _ret ; }
		xSphere^  _sphere (  )  {xSphere^  _ret = gcnew xSphere()   ; m_pObject->shpere(_ret->shpere()); return _ret ; }
		xVector3^ _center (  )  {xVector3^ _ret = gcnew xVector3()  ; m_pObject->center(_ret->xvec()  ); return _ret ; } 
	public:
		bool render(int passedTime) {return m_pObject->render(passedTime) ; }
		PF_ReadOnly(xAABB^    , aabb   , this , _aabb   , , );
		PF_ReadOnly(xSphere^  , sphere , this , _sphere , , );
		PF_ReadOnly(xVector3^ , center , this , _center , , );
	};

	public ref   class xHelperDrawable : public xDrawable
	{
	internal:
		IDrawableHelper* m_pObject;
		xHelperDrawable(IDrawableHelper*  pObject): m_pObject(pObject) , xDrawable(pObject) { m_pObject->AddRef() ; };
		~xHelperDrawable(){m_pObject->ReleaseObject() ; };
	public:
		bool  init(xBaseTextureManager^ pTexMgr){ return m_pObject->init(pTexMgr->m_Api , pTexMgr->m_pTexMgr) ; }
		bool  begin(){return m_pObject->begin() ; }
		bool  end(){return m_pObject->end() ; }
	};
}
