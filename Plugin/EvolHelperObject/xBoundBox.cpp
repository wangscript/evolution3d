#include "xStdPch.h"
#include "xBoundBox.h"
#include <OperationSys/xOperationSys.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xBoundBoxHelper ,  IDrawableHelper);
IMPL_OBJECT_FACTORY(xBoundBoxHelper , IDrawableHelper , xBoundBoxHelperFactory , IDrawElementCreator , xDrawElementFactoryMgr , L"BoundBox", L"BoundBox Element" , IBaseRenderer* , int);

xBoundBoxHelper::xBoundBoxHelper(IBaseRenderer* pRenderer, int arg)
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

xBoundBoxHelper::~xBoundBoxHelper()
{

}

bool xBoundBoxHelper::init(xBaseTextureMgr* pTexMgr)
{
	IRenderApi* pRenderApi = m_pRenderer->renderApi();
	struct SimpleVertex
	{
		xvec4 pos;
	};
	m_nGridX = 32;
	m_nGridY = 32;
	m_lenX   = 1.0f;
	m_lenY   = 1.0f;
	float step_x = m_lenX / m_nGridX;
	float step_y = m_lenY / m_nGridY;

	int nVertex =  (m_nGridY + 1 + m_nGridX + 1) * 2 ;
	SimpleVertex*  vertices = new SimpleVertex[ nVertex ] ;
	int idx = 0;
	int nDim[2] = {m_nGridY + 5, m_nGridX + 5};
	for(int i = 0 ; i <= m_nGridX ; i ++)
	{
		vertices[idx].pos = xvec4(i * step_x - m_lenX / 2.0f , 0 - m_lenY / 2.0f , 0.0f , 1.0f );
		idx ++ ; 
		vertices[idx].pos = xvec4(i * step_x - m_lenX / 2.0f , 0 + m_lenY / 2.0f , 0.0f , 1.0f );
		idx ++ ; 
	}

	for(int i = 0 ; i <= m_nGridY ; i ++)
	{
		vertices[idx].pos = xvec4(0 - m_lenX / 2.0f , i * step_y - m_lenY / 2.0f , 0.0f , 1.0f );
		idx ++ ; 
		vertices[idx].pos = xvec4(0 + m_lenX / 2.0f , i * step_y - m_lenY / 2.0f , 0.0f , 1.0f );
		idx ++ ; 
	}

	void* initData[] = { vertices };
	m_pRenderApi = pRenderApi;
	int nStride = sizeof(SimpleVertex);
	m_InputDesc.addElement(SHADER_SEMANTIC_POSITION , SHADERVARTYPE_FLOAT4);
	IInputAssembler* pAss = m_pRenderApi->createInputAssembler(L"LineGrid",m_InputDesc);
	m_pVertexStream = pAss->createVertexStream();
	m_pVertexStream->createBuffers( nVertex , (void**)&initData, NULL );



	
	unsigned int* indices = new unsigned int[ nVertex ];//(m_nGridY + 1) * 2 +  (m_nGridX + 1) * 2];
	for(int y = 0 ; y < nVertex ; y ++)
	{
		indices[y] = y;
	}
	xInputBufferDesc idxBufDesc;
	idxBufDesc.m_usage = RESOURCE_USAGE_DEFAULT;
	idxBufDesc.m_accessFlage = RESOURCE_ACCESS_NONE;
	idxBufDesc.m_bindtype = BIND_AS_INDEX_BUFFER;
	m_pIdxBuffer = m_pRenderApi->createInputBuffer( nVertex  , 4 , &idxBufDesc,indices);

	m_pBlendState = m_pRenderApi->createBlendState(L"Default");
	m_pRenderApi->setBlendState(m_pBlendState);


	HGpuProgram hProgram = m_pRenderApi->gpuProgramManager()->load(L"PlaneGrid.vertex",L"PlaneGrid.pixel",NULL);
	m_pGpuProgram = hProgram.getResource() ;
	m_pRenderApi->setGpuProgram( m_pGpuProgram );

	delete [] vertices;
	delete [] indices;
	return true;
}

bool xBoundBoxHelper::begin()
{
	m_pRenderApi->setGpuProgram( m_pGpuProgram );
	return true;
}

bool xBoundBoxHelper::render(unsigned long passedTime)
{
	int nVertex =  (m_nGridY + 1 + m_nGridX + 1) * 2 ;
	m_pRenderApi->setVertexStream( m_pVertexStream );
	m_pRenderApi->draw(m_pIdxBuffer , nVertex , 0 , ePrimtiveType_Lines );
	return true;
}

bool xBoundBoxHelper::end()
{
	return true;
}

void xBoundBoxHelper::center(XMathLib::xvec3&   _center)
{
}
void xBoundBoxHelper::aabb(xGeomLib::xaabb&     _aabb)
{
}

void xBoundBoxHelper::shpere(xGeomLib::xshpere& _shpere)
{
}

END_NAMESPACE_XEVOL3D