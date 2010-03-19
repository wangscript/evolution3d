#include "stdafx.h"
#include "xTerrainHeightQuery.h"
BEGIN_NAMESPACE_XEVOL3D

xTerrainHeightSelector::xTerrainHeightSelector(IRenderApi* pApi , IColorSelector* pBasicSelect)
:IColorSelector(pApi)
{
	m_SelectRenderView = NULL;
	m_ColorSelBlender = NULL;
	m_RefCount = 1;
	m_pRenderApi = pApi;
	m_pBasicSelector  = pBasicSelect;
}

xTerrainHeightSelector::~xTerrainHeightSelector()
{
	XSAFE_RELEASEOBJECT(m_SelectRenderView);
}

bool xTerrainHeightSelector::create(xXmlNode* pNode)
{
	m_fmt = PIXELFORMAT_R32G32B32A32F;
	m_SelectRenderView =   m_pRenderApi->createRenderView( 0 ,0 , true);
	m_SelectRenderView->createRenderTarget(1, m_fmt , true ,false);
	m_ColorSelBlender = m_pRenderApi->createBlendState(L"ColorSel");
	m_hColorSelShader = m_pRenderApi->createShader(L"PositionSel", eShader_PixelShader);
	return true;
}

bool xTerrainHeightSelector::setRenderObjectID(int id1 , int id2)
{
	return m_pBasicSelector->setRenderObjectID(id1 , id2);
}

bool xTerrainHeightSelector::setRenderObjMasterID(int _id)
{
	return m_pBasicSelector->setRenderObjMasterID(_id);
}

bool xTerrainHeightSelector::setRenderObjSlaveID(int _id)
{
	return m_pBasicSelector->setRenderObjSlaveID(_id);
}

bool xTerrainHeightSelector::begin()
{
	if(m_SelectRenderView == NULL)
		return false;
	m_pRenderApi->pushRenderView(m_SelectRenderView);
	m_pRenderApi->setBlendState(m_ColorSelBlender);
	return true;
}
bool xTerrainHeightSelector::resize(int w , int h )
{
	if(m_SelectRenderView)
	{
		m_SelectRenderView->resize(w , h , true);
		m_SelectRenderView->createRenderTarget(1, m_fmt , true ,false);
		return true;
	}
	return  false;
}
bool xTerrainHeightSelector::end()
{
	if(m_SelectRenderView == NULL)
		return false;
	m_pRenderApi->popRenderView();
	IGpuProgram* pCurProgram = m_pRenderApi->getGpuProgram();
	m_pRenderApi->setGpuProgram( NULL );
	m_pRenderApi->setGpuProgram( pCurProgram );
	return true;
}

bool xTerrainHeightSelector::getSelectID(int x , int y , int w , int h, void* _data)
{
	IRenderTarget* pRenderTarget = (IRenderTarget*)m_SelectRenderView->renderTarget(0);
	if(pRenderTarget == NULL)
		return false;
	pRenderTarget->grabRenderTagetData(_data , x , y , w , h);
	return true;
}

bool xTerrainHeightSelector::beginPrimitive()
{
	m_pRenderApi->setShader(m_hColorSelShader);
	m_hColorSelShader->commitParamTable();
	return true;
}

bool xTerrainHeightSelector::endPrimitive()
{
	return true;
}





END_NAMESPACE_XEVOL3D
