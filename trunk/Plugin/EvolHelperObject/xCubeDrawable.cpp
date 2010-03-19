#include "xStdPch.h"
#include "xCubeDrawable.h"
#include <OperationSys/xOperationSys.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xCubeDrawable ,  IDrawableHelper);
IMPL_OBJECT_FACTORY(xCubeDrawable , IDrawableHelper , xCubeHelperFactory , IDrawElementCreator , xDrawElementFactoryMgr , L"CubeElement", L"Cube Element" , IBaseRenderer* , int);

xCubeDrawable::xCubeDrawable(IBaseRenderer* pRenderer, int arg)
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

xCubeDrawable::~xCubeDrawable()
{

}

bool xCubeDrawable::init(xBaseTextureMgr* pTexMgr)
{
	IRenderApi* pRenderApi = m_pRenderer->renderApi();
	struct SimpleVertex
	{
		xvec4 pos;
		xvec4 cl;
		xvec2 uv;
	};

	SimpleVertex vertices[] = 
	{
		//x    ,   y   ,  z                    diffusse                       u  , v 
		{xvec4(-1.0f , -1.0f ,  -1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(1.0f ,0.0f )  },
		{xvec4(-1.0f , -1.0f ,	 1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(1.0f ,1.0f )  },
		{xvec4( 1.0f , -1.0f ,	 1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(0.0f ,1.0f )  },
		{xvec4( 1.0f , -1.0f ,	-1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(0.0f ,0.0f )  },
		{xvec4(-1.0f ,  1.0f ,	-1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(0.0f ,0.0f )  },
		{xvec4( 1.0f ,  1.0f ,	-1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(1.0f ,0.0f )  },
		{xvec4( 1.0f ,  1.0f ,	 1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(1.0f ,1.0f )  },
		{xvec4(-1.0f ,  1.0f ,	 1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(0.0f ,1.0f )  },
		{xvec4(-1.0f , -1.0f ,	-1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(0.0f ,0.0f )  },
		{xvec4( 1.0f , -1.0f ,	-1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(1.0f ,0.0f )  },
		{xvec4( 1.0f ,  1.0f ,	-1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(1.0f ,1.0f )  },
		{xvec4(-1.0f ,  1.0f ,	-1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(0.0f ,1.0f )  },
		{xvec4( 1.0f , -1.0f ,	-1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(0.0f ,0.0f )  },
		{xvec4( 1.0f , -1.0f ,	 1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(1.0f ,0.0f )  },
		{xvec4( 1.0f ,  1.0f ,	 1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(1.0f ,1.0f )  },
		{xvec4( 1.0f ,  1.0f ,	-1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(0.0f ,1.0f )  },
		{xvec4( 1.0f , -1.0f ,	 1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(0.0f ,0.0f )  },
		{xvec4(-1.0f , -1.0f ,	 1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(1.0f ,0.0f )  },
		{xvec4(-1.0f ,  1.0f ,	 1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(1.0f ,1.0f )  },
		{xvec4( 1.0f ,  1.0f ,	 1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(0.0f ,1.0f )  },
		{xvec4(-1.0f , -1.0f ,	 1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(0.0f ,0.0f )  },
		{xvec4(-1.0f , -1.0f ,	-1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(1.0f ,0.0f )  },
		{xvec4(-1.0f ,  1.0f ,	-1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(1.0f ,1.0f )  },
		{xvec4(-1.0f ,  1.0f ,	 1.0f ),  xvec4(1.0f, 1.0f,1.0f, 1.0f)   ,   xvec2(0.0f ,1.0f )  },
	};

	void* initData[] = { vertices };
	m_pRenderApi = pRenderApi;
	int nStride = sizeof(SimpleVertex);
	m_InputDesc.addElement(SHADER_SEMANTIC_POSITION , SHADERVARTYPE_FLOAT4);
	m_InputDesc.addElement(SHADER_SEMANTIC_COLOR  , SHADERVARTYPE_FLOAT4);
	m_InputDesc.addElement(SHADER_SEMANTIC_TEXCOORD , SHADERVARTYPE_FLOAT2);
	m_InputDesc.addBufferDesc(RESOURCE_USAGE_DEFAULT,RESOURCE_ACCESS_NONE);
	IInputAssembler* pAss = m_pRenderApi->createInputAssembler(L"Simple",m_InputDesc);
	m_pVertexStream = pAss->createVertexStream();
	m_pVertexStream->createBuffers( 24 , (void**)&initData, NULL );

	unsigned int indices[36] =
	{
		0  ,2 ,1 , 
		2  ,0 ,3 , 
		4  ,6 ,5 , 
		6  ,4 ,7 , 
		8  ,10,9 , 
		10 ,8 ,11, 
		12 ,14,13, 
		14 ,12,15, 
		16 ,18,17, 
		18 ,16,19, 
		20 ,22,21, 
		22 ,20,23
	};

	xInputBufferDesc idxBufDesc;
	idxBufDesc.m_usage = RESOURCE_USAGE_DEFAULT;
	idxBufDesc.m_accessFlage = RESOURCE_ACCESS_NONE;
	idxBufDesc.m_bindtype = BIND_AS_INDEX_BUFFER;
	m_pIdxBuffer = m_pRenderApi->createInputBuffer(36  , 4 , &idxBufDesc,indices);

	m_pTexture = m_pRenderApi->createFileTexture(_XEVOL_ABSPATH_(L"texture/Cover.jpg") , NULL, 0);
	//m_pRenderApi->setShaderResource(eShader_PixelShader,0,m_pTexture);
	m_pRenderApi->setTexture(Texture_Diffuse , m_pTexture);

	m_pMaskTexture = m_pRenderApi->createFileTexture(_XEVOL_ABSPATH_(L"texture/Mask.dds") , NULL, 0);
	m_pRenderApi->setTexture(Texture_Mask , m_pMaskTexture);

	m_pBlendState = m_pRenderApi->createBlendState(L"Default");
	m_pRenderApi->setBlendState(m_pBlendState);


	//HGpuProgram hProgram = m_pRenderApi->gpuProgramManager()->load(L"simple.shader");
	//HGpuProgram hProgram = m_pRenderApi->gpuProgramManager()->load(L"simple.vertex" , L"simple.pixel(0:simple.mask,RGBAGray;)" , NULL);
	//HGpuProgram hProgram = m_pRenderApi->gpuProgramManager()->load(L"simple.vertex" , L"simple.pixel(0:simple.mask,RGBAGray;)" , NULL);;
	xGpuProgNameParser _shaderNameParser;
	xGpuProgramName    _shaderName;
	_shaderNameParser.setShaderName(eShader_VertexShader , L"simple.vertex" );
	_shaderNameParser.setShaderName(eShader_PixelShader  , L"simple.pixel"  );
	_shaderNameParser.addShaderNode(eShader_PixelShader  , L"simple.texture");
	//_shaderNameParser.addShaderNode(eShader_PixelShader  , L"simple.mask"   );
	//_shaderNameParser.addShaderNode(eShader_PixelShader  , L"simple.fakehdr" );
	//_shaderNameParser.addShaderNode(eShader_PixelShader  , L"RGBAGray"   );
	_shaderNameParser.toName(_shaderName);
	HGpuProgram hProgram = m_pRenderApi->gpuProgramManager()->load( _shaderName);
	m_pGpuProgram = hProgram.getResource() ;
	m_pRenderApi->setGpuProgram( m_pGpuProgram );

	m_pGpuProgram->setTexture(L"DiffuseTexture" , m_pTexture);
	m_pGpuProgram->setTexture(L"Texture1"       , m_pMaskTexture);

	return true;
}

bool xCubeDrawable::begin()
{
	m_pRenderApi->setTexture(Texture_Mask , m_pMaskTexture);
	m_pRenderApi->setBlendState(m_pBlendState);

	m_pRenderApi->setDepthStencilState(NULL);
	m_pRenderApi->setGpuProgram( m_pGpuProgram );
	return true;
}

bool xCubeDrawable::render(unsigned long passedTime)
{
	m_pRenderApi->setVertexStream( m_pVertexStream );
	m_pRenderApi->draw(m_pIdxBuffer , 36 );
	return true;
}

bool xCubeDrawable::end()
{
	return true;
}

void xCubeDrawable::center(XMathLib::xvec3&   _center)
{
}
void xCubeDrawable::aabb(xGeomLib::xaabb&     _aabb)
{
}

void xCubeDrawable::shpere(xGeomLib::xshpere& _shpere)
{
}

END_NAMESPACE_XEVOL3D