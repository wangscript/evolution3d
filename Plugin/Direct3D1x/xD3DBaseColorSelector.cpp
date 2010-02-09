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
#include "xD3DBaseColorSelector.h"
#include "Image/PixelFormatAssit.h"
BEGIN_NAMESPACE_XEVOL3D

bool xPixelSelectIDConstBinderS::updateConstant(IShaderConstantReflection* pConst)
{
	int colorid[4];
	colorid[0] = m_id[0].sLoWord;
	colorid[1] = m_id[0].sHiWord;
	colorid[2] = m_id[1].sLoWord;
	colorid[3] = m_id[1].sHiWord;
	pConst->setData(colorid , 4 * sizeof(int) ); //2 * sizeof(xPixelSelID) );
	return true;
}

bool xPixelSelectIDConstBinderU::updateConstant(IShaderConstantReflection* pConst)
{
	int colorid[4];
	colorid[0] = m_id[0].uLoWord;
	colorid[1] = m_id[0].uHiWord;
	colorid[2] = m_id[1].uLoWord;
	colorid[3] = m_id[1].uHiWord;
	pConst->setData(colorid , 4 * sizeof(int) ); //2 * sizeof(xPixelSelID) );
	return true;
}


bool xD3DBaseColorSelector::setRenderObjectID(int id1 , int id2)
{
	m_SelectID[0]._id = id1;
	m_SelectID[1]._id = id2;
	return true;
}

bool xD3DBaseColorSelector::setRenderObjMasterID(int _id)
{
	m_SelectID[0]._id = _id;
	return true;
}

bool xD3DBaseColorSelector::setRenderObjSlaveID(int _id)
{
	m_SelectID[1]._id = _id;
	return true;
}

xD3DBaseColorSelector::xD3DBaseColorSelector(IRenderApi* pD10Api) : IColorSelector(pD10Api)
{
	m_SelectID[0]._id = 0; 
	m_SelectID[1]._id = 0;
	m_pRenderApi = pD10Api;
	m_ColorSelBinderS.setID(m_SelectID);
	m_ColorSelBinderU.setID(m_SelectID);
	m_pRenderApi->registeShaderConstBinder(L"colorID"   , &m_ColorSelBinderS);
	m_pRenderApi->registeShaderConstBinder(L"ColorID"   , &m_ColorSelBinderS);
	m_pRenderApi->registeShaderConstBinder(L"SelectID"  , &m_ColorSelBinderS);
	m_pRenderApi->registeShaderConstBinder(L"selectID"  , &m_ColorSelBinderS);
	m_pRenderApi->registeShaderConstBinder(L"color_id"  , &m_ColorSelBinderS);

	m_pRenderApi->registeShaderConstBinder(L"ucolorID"  , &m_ColorSelBinderU);
	m_pRenderApi->registeShaderConstBinder(L"uColorID"  , &m_ColorSelBinderU);
	m_pRenderApi->registeShaderConstBinder(L"uSelectID" , &m_ColorSelBinderU);
	m_pRenderApi->registeShaderConstBinder(L"uselectID" , &m_ColorSelBinderU);
	m_pRenderApi->registeShaderConstBinder(L"ucolor_id" , &m_ColorSelBinderU);

	m_SelectRenderView = NULL;
	m_ColorSelBlender = NULL;
	m_RefCount = 1;
}

xD3DBaseColorSelector::~xD3DBaseColorSelector()
{
	XSAFE_RELEASEOBJECT(m_SelectRenderView);
	//m_pRenderApi->unRegisteShaderConstBinder(L"colorID");
	//m_pRenderApi->unRegisteShaderConstBinder(L"ColorID");
	//m_pRenderApi->unRegisteShaderConstBinder(L"SelectID");
	//m_pRenderApi->unRegisteShaderConstBinder(L"selectID");
	//m_pRenderApi->unRegisteShaderConstBinder(L"color_id");

	//m_pRenderApi->unRegisteShaderConstBinder(L"ucolorID" );
	//m_pRenderApi->unRegisteShaderConstBinder(L"uColorID" );
	//m_pRenderApi->unRegisteShaderConstBinder(L"uSelectID");
	//m_pRenderApi->unRegisteShaderConstBinder(L"uselectID");
	//m_pRenderApi->unRegisteShaderConstBinder(L"ucolor_id");
}

bool xD3DBaseColorSelector::create(xXmlNode* pSelNode)
{
	ePIXEL_FORMAT fmt = xPixelFormatAssit::singleton()->GetPixelFormat( pSelNode->value(L"format") );
	const wchar_t* pColorShader = pSelNode->value(L"shader");
	if(pColorShader == NULL) pColorShader = L"ColorSel";
	m_SelectRenderView =   m_pRenderApi->createRenderView( 0 ,0 , true , xRTSampleDesc::None);
	m_SelectRenderView->createRenderTarget(1, fmt , true ,false);
	m_Format = fmt;
	m_hColorSelShader = m_pRenderApi->createShader(pColorShader, eShader_PixelShader);
	m_ColorSelBlender = m_pRenderApi->createBlendState(L"ColorSel");
	return true;
}

bool xD3DBaseColorSelector::begin()
{
	if(m_SelectRenderView == NULL)
		return false;
	m_pRenderApi->pushRenderView(m_SelectRenderView);
	m_pRenderApi->setBlendState(m_ColorSelBlender);
	return true;
}
bool xD3DBaseColorSelector::resize(int w , int h )
{
	if(m_SelectRenderView)
	{
		m_SelectRenderView->resize(w , h , true);
		m_SelectRenderView->createRenderTarget(1, m_Format , true ,false);
		return true;
	}
	return  false;
}
bool xD3DBaseColorSelector::end()
{
	if(m_SelectRenderView == NULL)
		return false;
	m_pRenderApi->popRenderView();
	IGpuProgram* pCurProgram = m_pRenderApi->getGpuProgram();
	m_pRenderApi->setGpuProgram( NULL );
	m_pRenderApi->setGpuProgram( pCurProgram );
	return true;
}

bool xD3DBaseColorSelector::getSelectID(int x , int y , int w , int h, void* _data)
{
	IRenderTarget* pRenderTarget = (IRenderTarget*)m_SelectRenderView->renderTarget(0);
	if(pRenderTarget == NULL)
		return false;
	pRenderTarget->grabRenderTagetData(x , y , w , h , _data);
	return true;
}

bool xD3DBaseColorSelector::beginPrimitive()
{
	m_pRenderApi->setShader( m_hColorSelShader.getResource() );
	m_hColorSelShader->commitParamTable();
	return true;
}

bool xD3DBaseColorSelector::endPrimitive()
{
	return true;
}

END_NAMESPACE_XEVOL3D
