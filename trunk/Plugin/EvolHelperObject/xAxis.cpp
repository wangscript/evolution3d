#include "xStdPch.h"
#include "xAxis.h"
#include <OperationSys/xOperationSys.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xAxisDrawable ,  IDrawableHelper);
IMPL_OBJECT_FACTORY(xAxisDrawable , IDrawableHelper , xAxisFactory , IDrawElementCreator , xDrawElementFactoryMgr , L"Axis", L"Axis Element" , IBaseRenderer* , int);
xAxisDrawable::xAxisDrawable(IBaseRenderer* pRenderer, int arg)
:IDrawableHelper(pRenderer)
{
	m_pVertexStream = NULL;
	m_pIdxBuffer = NULL;
	m_pAss = NULL;
	m_pRenderApi = NULL;
	m_pTexture = NULL;

	m_pMaskTexture = NULL;
	m_pBlendState = NULL;
	m_RefCount = 1;
}

xAxisDrawable::~xAxisDrawable()
{

}

bool xAxisDrawable::init(xBaseTextureMgr* pTexMgr)
{
	IRenderApi* pRenderApi = m_pRenderer->renderApi();
	struct SimpleVertex
	{
		xvec4 pos;
        xvec4 cl;
	};
	m_Radios    = 1.0f;
    float ArrowL = 0.2f * m_Radios;
	m_InputDesc.addElement(SHADER_SEMANTIC_POSITION , SHADERVARTYPE_FLOAT4);
    m_InputDesc.addElement(SHADER_SEMANTIC_COLOR    , SHADERVARTYPE_FLOAT4);

	int nLineSeg = 6;
	int nPoint = 3 * nLineSeg;
	SimpleVertex*  vertices = new SimpleVertex[ nPoint ] ;

    for(int i = 0 ; i < 6 ; i ++)
    {
        vertices[X_AXIS * nLineSeg + i].cl = xvec4(1.0f , 0.0f , 0.0f , 1.0f);
        vertices[Y_AXIS * nLineSeg + i].cl = xvec4(0.0f , 1.0f , 0.0f , 1.0f);
        vertices[Z_AXIS * nLineSeg + i].cl = xvec4(0.0f , 0.0f , 1.0f , 1.0f);
    }
	for(int i = 0 ; i < 2 ; i ++)
	{
		vertices[X_AXIS * nLineSeg + i].pos = xvec4(m_Radios * i , 0.0f          , 0.0f         , 1.0f);
		vertices[Y_AXIS * nLineSeg + i].pos = xvec4(0.0f         , m_Radios  * i , 0.0f         , 1.0f);
		vertices[Z_AXIS * nLineSeg + i].pos = xvec4(0.0f         , 0.0f          , m_Radios * i , 1.0f);
	}

	//箭头第一个边
	vertices[X_AXIS * nLineSeg + 2].pos  = vertices[X_AXIS * nLineSeg + 1].pos; 
	vertices[Y_AXIS * nLineSeg + 2].pos  = vertices[Y_AXIS * nLineSeg + 1].pos; 
	vertices[Z_AXIS * nLineSeg + 2].pos  = vertices[Z_AXIS * nLineSeg + 1].pos; 

	vertices[X_AXIS * nLineSeg + 3].pos  = vertices[X_AXIS * nLineSeg + 1].pos + xvec4(-ArrowL       ,  0.2f * ArrowL  ,  0.2f * ArrowL); 
	vertices[Y_AXIS * nLineSeg + 3].pos  = vertices[Y_AXIS * nLineSeg + 1].pos + xvec4(0.2f * ArrowL , -ArrowL         ,  0.2f * ArrowL);  
	vertices[Z_AXIS * nLineSeg + 3].pos  = vertices[Z_AXIS * nLineSeg + 1].pos + xvec4(0.2f * ArrowL , 0.2f * ArrowL   , -ArrowL       ); 


	vertices[X_AXIS * nLineSeg + 4].pos  = vertices[X_AXIS * nLineSeg + 1].pos; 
	vertices[Y_AXIS * nLineSeg + 4].pos  = vertices[Y_AXIS * nLineSeg + 1].pos; 
	vertices[Z_AXIS * nLineSeg + 4].pos  = vertices[Z_AXIS * nLineSeg + 1].pos; 

	vertices[X_AXIS * nLineSeg + 5].pos  = vertices[X_AXIS * nLineSeg + 1].pos + xvec4(-ArrowL        , -0.2f * ArrowL  , -0.2f * ArrowL); 
	vertices[Y_AXIS * nLineSeg + 5].pos  = vertices[Y_AXIS * nLineSeg + 1].pos + xvec4(-0.2f * ArrowL , -ArrowL         , -0.2f * ArrowL); 
	vertices[Z_AXIS * nLineSeg + 5].pos  = vertices[Z_AXIS * nLineSeg + 1].pos + xvec4(-0.2f * ArrowL , -0.2f * ArrowL  , -ArrowL       ); 

	void* initData[] = { vertices };
	m_pRenderApi = pRenderApi;
	int nStride = sizeof(SimpleVertex);

	IInputAssembler* pAss = m_pRenderApi->createInputAssembler(L"AxisLineHelper",m_InputDesc);
	m_pVertexStream = pAss->createVertexStream();
	m_pVertexStream->createBuffers( nPoint , (void**)&initData, NULL );


	unsigned int* indices = new unsigned int[ nPoint ];//(m_nGridY + 1) * 2 +  (m_nGridX + 1) * 2];
	for(int y = 0 ; y < nPoint ; y ++)
	{
		indices[y] = y;
	}
	xInputBufferDesc idxBufDesc;
	idxBufDesc.m_usage = RESOURCE_USAGE_DEFAULT;
	idxBufDesc.m_accessFlage = RESOURCE_ACCESS_NONE;
	idxBufDesc.m_bindtype = BIND_AS_INDEX_BUFFER;
	m_pIdxBuffer = m_pRenderApi->createInputBuffer( nPoint  , 4 , &idxBufDesc,indices);

	m_pBlendState = m_pRenderApi->createBlendState(L"Default");

	m_pDepthState = m_pRenderApi->createDepthStencilState(L"Overlay");


	m_hProgram = m_pRenderApi->gpuProgramManager()->load(L"Axis.vertex",L"Axis.pixel",NULL);

	delete [] vertices;
	delete [] indices;
	return true;
}

bool xAxisDrawable::begin()
{
	m_pRenderApi->setBlendState(m_pBlendState);
	m_pRenderApi->setDepthStencilState(m_pDepthState);
	m_pRenderApi->setGpuProgram( m_hProgram.getResource() );
	return true;
}

bool xAxisDrawable::render(unsigned long passedTime)
{
	begin();
	int nVertex =  3 * 2;
	m_pRenderApi->setVertexStream( m_pVertexStream );
	for(int i = 0 ; i < 3 ; i ++ )
	{
		m_pRenderApi->colorSelector()->setRenderObjSlaveID(i);
		m_pRenderApi->draw(m_pIdxBuffer , nVertex , i  * nVertex , ePrimtiveType_Lines );
	}
	end();
	return true;
}

bool xAxisDrawable::end()
{
	return true;
}

void xAxisDrawable::center(XMathLib::xvec3&   _center)
{
}
void xAxisDrawable::aabb(xGeomLib::xaabb&     _aabb)
{
	_aabb.m_Max = xvec3(m_Radios  , m_Radios  , m_Radios);
	_aabb.m_Min = xvec3(-m_Radios , -m_Radios , -m_Radios);
	return ;
}

void xAxisDrawable::shpere(xGeomLib::xshpere& _shpere)
{
	_shpere.m_Orign = xvec3(0.0f , 0.0f , 0.0f );
	_shpere.m_R     = m_Radios;
}

END_NAMESPACE_XEVOL3D