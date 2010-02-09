#include "xGL2BaseColorSelector.h"
#include "xOpenGL2API.h"
#include "Image/PixelFormatAssit.h"
#include "xGL2Shader.h"
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


bool xGL2BaseColorSelector::setRenderObjectID(int id1 , int id2)
{
	m_SelectID[0]._id = id1;
	m_SelectID[1]._id = id2;
	return true;
}

bool xGL2BaseColorSelector::setRenderObjMasterID(int _id)
{
	m_SelectID[0]._id = _id;
	return true;
}

bool xGL2BaseColorSelector::setRenderObjSlaveID(int _id)
{
	m_SelectID[1]._id = _id;
	return true;
}

xGL2BaseColorSelector::xGL2BaseColorSelector(xGL2RenderApi* pD10Api) : IColorSelector(pD10Api)
{
	m_pD10Api = pD10Api;
	m_SelectID[0]._id = 0; 
	m_SelectID[1]._id = 0;
	m_ColorSelBinderS.setID(m_SelectID);
	m_ColorSelBinderU.setID(m_SelectID);
	m_pD10Api->registeShaderConstBinder(L"colorID"   , &m_ColorSelBinderS);
	m_pD10Api->registeShaderConstBinder(L"ColorID"   , &m_ColorSelBinderS);
	m_pD10Api->registeShaderConstBinder(L"SelectID"  , &m_ColorSelBinderS);
	m_pD10Api->registeShaderConstBinder(L"selectID"  , &m_ColorSelBinderS);
	m_pD10Api->registeShaderConstBinder(L"color_id"  , &m_ColorSelBinderS);

	m_pD10Api->registeShaderConstBinder(L"ucolorID"  , &m_ColorSelBinderU);
	m_pD10Api->registeShaderConstBinder(L"uColorID"  , &m_ColorSelBinderU);
	m_pD10Api->registeShaderConstBinder(L"uSelectID" , &m_ColorSelBinderU);
	m_pD10Api->registeShaderConstBinder(L"uselectID" , &m_ColorSelBinderU);
	m_pD10Api->registeShaderConstBinder(L"ucolor_id" , &m_ColorSelBinderU);

	m_SelectRenderView = NULL;
	m_SelectRenderView = NULL;
	m_pColorSelShader = NULL;
	m_ColorSelBlender = NULL;
	m_RefCount = 1;
}

xGL2BaseColorSelector::~xGL2BaseColorSelector()
{
	XSAFE_DELETE(m_SelectRenderView);
	//m_pD10Api->unRegisteShaderConstBinder(L"colorID");
	//m_pD10Api->unRegisteShaderConstBinder(L"ColorID");
	//m_pD10Api->unRegisteShaderConstBinder(L"SelectID");
	//m_pD10Api->unRegisteShaderConstBinder(L"selectID");
	//m_pD10Api->unRegisteShaderConstBinder(L"color_id");

	//m_pD10Api->unRegisteShaderConstBinder(L"ucolorID" );
	//m_pD10Api->unRegisteShaderConstBinder(L"uColorID" );
	//m_pD10Api->unRegisteShaderConstBinder(L"uSelectID");
	//m_pD10Api->unRegisteShaderConstBinder(L"uselectID");
	//m_pD10Api->unRegisteShaderConstBinder(L"ucolor_id");
}

bool xGL2BaseColorSelector::create(xXmlNode* pSelNode)
{
	ePIXEL_FORMAT fmt = xPixelFormatAssit::singleton()->GetPixelFormat( pSelNode->value(L"format") );
	const wchar_t* pColorShader = pSelNode->value(L"shader");
	if(pColorShader == NULL) pColorShader = L"ColorSel";
	m_SelectRenderView = (xGL2RenderView*) m_pD10Api->createRenderView( 0 ,0 , true);
	m_SelectRenderView->createRenderTarget(1, fmt , true ,false);
	m_Format = fmt;
	m_pColorSelShader = (xGL2Shader*)m_pD10Api->createShader(pColorShader,eShader_PixelShader);
	m_ColorSelBlender = m_pD10Api->createBlendState(L"ColorSel");
	return true;
}

bool xGL2BaseColorSelector::begin()
{
	if(m_SelectRenderView == NULL)
		return false;
	m_pD10Api->pushRenderView(m_SelectRenderView);
	m_pD10Api->setBlendState(m_ColorSelBlender);
	return true;
}
bool xGL2BaseColorSelector::resize(int w , int h )
{
	if(m_SelectRenderView)
	{
		m_SelectRenderView->resize(w , h , true);
		m_SelectRenderView->createRenderTarget(1, m_Format , true ,false);
		return true;
	}
	return  false;
}
bool xGL2BaseColorSelector::end()
{
	if(m_SelectRenderView == NULL)
		return false;
	m_pD10Api->popRenderView();
	IGpuProgram* pCurProgram = m_pD10Api->getGpuProgram();
	m_pD10Api->setGpuProgram( NULL );
	m_pD10Api->setGpuProgram( pCurProgram );
	return true;
}

bool xGL2BaseColorSelector::getSelectID(int x , int y , int w , int h, void* _data)
{
	IRenderTarget* pRenderTarget = (IRenderTarget*)m_SelectRenderView->renderTarget(0);
	if(pRenderTarget == NULL)
		return false;
	pRenderTarget->grabRenderTagetData(x , y , w , h , _data);
	return true;
}

bool xGL2BaseColorSelector::beginPrimitive()
{
	m_pD10Api->setPixelShader(m_pColorSelShader);
	xGL2PixelShader* pColorSelShader = (xGL2PixelShader*)m_pColorSelShader;
	pColorSelShader->commitParamTable();
	return true;
}

bool xGL2BaseColorSelector::endPrimitive()
{
	return true;
}

END_NAMESPACE_XEVOL3D
