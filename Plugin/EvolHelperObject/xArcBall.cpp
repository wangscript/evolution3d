#include "xStdPch.h"
#include "xArcBall.h"
#include <OperationSys/xOperationSys.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xArcBallDrawable ,  IDrawableHelper);
IMPL_OBJECT_FACTORY(xArcBallDrawable , IDrawableHelper , xxArcBallFactory , IDrawElementCreator , xDrawElementFactoryMgr , L"ArcBall", L"ArcBall Element" , IBaseRenderer* , int);
xArcBallDrawable::xArcBallDrawable(IBaseRenderer* pRenderer, int arg)
:IDrawableHelper(pRenderer)
{
	m_pVertexStream = NULL;
	m_pIdxBuffer = NULL;
	m_pAss = NULL;
	m_pRenderApi = NULL;
	m_pTexture = NULL;
	m_pGpuProgram = NULL;

	m_pMaskTexture = NULL;
	m_pBlendState = NULL;
    m_RefCount = 1;
}

xArcBallDrawable::~xArcBallDrawable()
{

}

bool xArcBallDrawable::init(xBaseTextureMgr* pTexMgr)
{
	IRenderApi* pRenderApi = m_pRenderer->renderApi();
	struct SimpleVertex
	{
		xvec4 pos;
	};
	m_Radios    = 1.0f;
	m_nSegement = 64;

	m_InputDesc.addElement(SHADER_SEMANTIC_POSITION , SHADERVARTYPE_FLOAT4);

	int nPoint = 3 * (m_nSegement + 1);
	SimpleVertex*  vertices = new SimpleVertex[ nPoint ] ;
	
	float argStep = 2 * 3.1415926f/m_nSegement;
	float arg = 0.0f;

	int nLineSeg = m_nSegement + 1;
	for(int i = 0 ; i <= m_nSegement ; i ++)
	{
        float _x = m_Radios * sin(arg);
		float _y = m_Radios * cos(arg);

		vertices[XY_PLANE * nLineSeg + i].pos.x = _x;
		vertices[XY_PLANE * nLineSeg + i].pos.y = _y;
		vertices[XY_PLANE * nLineSeg + i].pos.z = 0.0f;
		vertices[XY_PLANE * nLineSeg + i].pos.w = 1.0f;

		vertices[YZ_PLANE * nLineSeg + i].pos.x = 0.0f;
		vertices[YZ_PLANE * nLineSeg + i].pos.y = _y;
		vertices[YZ_PLANE * nLineSeg + i].pos.z = _x;
		vertices[YZ_PLANE * nLineSeg + i].pos.w = 1.0f;

		vertices[XZ_PLANE * nLineSeg + i].pos.x = _x;
		vertices[XZ_PLANE * nLineSeg + i].pos.y = 0.0f;
		vertices[XZ_PLANE * nLineSeg + i].pos.z = _y;
		vertices[XZ_PLANE * nLineSeg + i].pos.w = 1.0f;

		arg += argStep;
	}


	void* initData[] = { vertices };
	m_pRenderApi = pRenderApi;
	int nStride = sizeof(SimpleVertex);
	
	IInputAssembler* pAss = m_pRenderApi->createInputAssembler(L"LineArcBall",m_InputDesc);
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
	


	HGpuProgram hProgram = m_pRenderApi->gpuProgramManager()->load(L"ArcBall.vertex",L"ArcBall.pixel",NULL);
	m_pGpuProgram = hProgram.getResource() ;
	m_pRenderApi->setGpuProgram( m_pGpuProgram );

	delete [] vertices;
	delete [] indices;
	return true;
}

bool xArcBallDrawable::begin()
{
	m_pRenderApi->setBlendState(m_pBlendState);
	m_pRenderApi->setDepthStencilState(m_pDepthState);
	m_pRenderApi->setGpuProgram( m_pGpuProgram );
	return true;
}

bool xArcBallDrawable::render(unsigned long passedTime)
{
	begin();
	int nVertex =  (m_nSegement + 1);
	m_pRenderApi->setVertexStream( m_pVertexStream );
	for(int i = 0 ; i < 3 ; i ++ )
	{
		m_pRenderApi->colorSelector()->setRenderObjSlaveID(i);
	    m_pRenderApi->draw(m_pIdxBuffer , nVertex , i  * nVertex , ePrimtiveType_LineStrip );
	}
	end();
	return true;
}

bool xArcBallDrawable::end()
{
	return true;
}

void xArcBallDrawable::center(XMathLib::xvec3&   _center)
{
}
void xArcBallDrawable::aabb(xGeomLib::xaabb&     _aabb)
{
	_aabb.m_Max = xvec3(m_Radios  , m_Radios  , m_Radios);
	_aabb.m_Min = xvec3(-m_Radios , -m_Radios , -m_Radios);
	return ;
}

void xArcBallDrawable::shpere(xGeomLib::xshpere& _shpere)
{
	_shpere.m_Orign = xvec3(0.0f , 0.0f , 0.0f );
	_shpere.m_R     = m_Radios;
}

END_NAMESPACE_XEVOL3D