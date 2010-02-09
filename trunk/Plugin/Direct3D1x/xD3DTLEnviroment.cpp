#include "stdafx.h"
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <stack>
#include <deque>
#include <new>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <iostream>
#include <math.h>
#include <assert.h>
#include "RenderApi/xRenderApi.h"
#include "RenderApi/xRenderApiImp.h"
#include "BaseLib/xStringHash.h"
#include "RenderApi/xShaderManager.h"
#include "RenderApi/xBaseShader.h"
#include "RenderApi/xRenderStateObjManager.h"
#include "RenderApi/xRenderApiManager.h"
#include "RenderApi/xRenderView.h"
#include "RenderApi/xBaseTexture.h"
#include "BaseLib/xI18N.h"
#include "BaseLib/xBaseMath.h"
#include "OperationSys/xOperationSys.h"
#include "RenderAPI/xShaderDefine.h"
#include "RenderAPI/xBaseTexture.h"
#include "xD3DTLEnviroment.h"
#include "RenderAPI/xMemBuffer.h"
#include "RenderApi/xRenderApi.h"
#include "RenderApi/xRenderApiImp.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD1xLightingEnv  , ILightingEnv);
xD1xLightingEnv::~xD1xLightingEnv()
{
	xRenderApiBase* pRenderApiBase = dynamic_cast<xRenderApiBase*>(m_pRenderApi);
	pRenderApiBase->removeLightingState(m_name.c_str() );
	m_pConstBuffer->ReleaseObject();
	m_pRenderApi->removeInputBuffer(m_pConstBuffer);
}

xD1xLightingEnv::xD1xLightingEnv(IRenderApi* pRenderApi , const wchar_t* _name)
:ILightingEnv(pRenderApi)
{
    m_RefCount = 1;
	m_name = _name;
	int iBufLen = sizeof(xD1xLightEnvBuffer);
	m_pConstBuffer = pRenderApi->createConstBuffer( iBufLen );
	IInputBuffer* pInputBuffer = (IInputBuffer*)m_pConstBuffer;
	pRenderApi->setInputBuffer(_name , m_pConstBuffer);
	m_pReflection = (IInputBufferReflection *)pInputBuffer->createReflection(true);
	m_pEnvBuffer = (xD1xLightEnvBuffer*)m_pReflection->systemBuffer();
	m_pReflection->setDirty(true);
}

xLightDesc* xD1xLightingEnv::lightDesc(size_t idx)
{
	return &m_pEnvBuffer->m_Light[idx];
}

void xD1xLightingEnv::enableLight(size_t _nLight)
{
    m_pEnvBuffer->nLight[0] = (int)_nLight;
}
size_t xD1xLightingEnv::nLightEnabled()
{
    return m_pEnvBuffer->nLight[0];
}
const wchar_t* xD1xLightingEnv::name()
{
	return m_name.c_str() ; 
}
void xD1xLightingEnv::applyChange()
{
	m_pReflection->setDirty(true);
}


END_NAMESPACE_XEVOL3D

