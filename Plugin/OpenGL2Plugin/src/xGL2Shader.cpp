#include "xGL2Shader.h"
#include "xOpenGL2API.h"
#include "xGL2ParamBuffer.h"
#include "xGL2Sampler.h"
#include "xGL2ConstLexer.h"
#include <fstream>
#include <BaseLib/xI18N.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xGL2Shader , IBaseShader)
IMPL_BASE_OBJECT_CLASSID(xGL2VertexShader , xGL2Shader)
IMPL_BASE_OBJECT_CLASSID(xGL2PixelShader  , IBaseShader)
IMPL_BASE_OBJECT_CLASSID(xGL2GeomShader   , IBaseShader)


IShaderParamTable* xGL2Shader::createShaderParamTable()
{
    xGL2ShaderParamTable* pShaderCT =  new xGL2ShaderParamTable(this);
	//-----¼ÓÔØÕâ¸öShaderConstTable;
	pShaderCT->load(m_pShaderReflect);
	return pShaderCT;
}

IShaderParamTable* xGL2Shader::getShaderParamTable()
{
	if(m_pConstTable == NULL)
	{
		m_pConstTable = new xGL2ShaderParamTable(this);
		m_pConstTable->load(m_pShaderReflect);
	}
	return m_pConstTable ;
}

void xGL2Shader::commitParamTable()
{
	getShaderParamTable();
	if(m_pConstTable)
	{
		m_pConstTable->updateRenderContext();
		m_pConstTable->commit();
	}
}

xGL2Shader::xGL2Shader(IRenderApi* pOwner):IBaseShader(pOwner)
{
	m_pShaderReflect = NULL;
	m_pConstTable    = NULL;
	m_RefCount       = 1;
}

xGL2Shader::~xGL2Shader()
{
    unload();
}

unsigned long xGL2Shader::memUsage()
{
	return 1;
}

bool xGL2Shader::afterLoad(ID3D10Blob* pShaderCode)
{
   HRESULT hr = GL2ReflectShader(pShaderCode->GetBufferPointer() , pShaderCode->GetBufferSize() , &m_pShaderReflect);
   if(FAILED(hr))
   {
	   return false;
   }
   return true;
}

bool  xGL2Shader::unload()
{
	XSAFE_RELEASE(m_pShaderReflect);
	return true;
}
bool xGL2Shader::load(const wchar_t* fileName , unsigned long  arg)
{
	m_Name = fileName;
	WIN32_FILE_ATTRIBUTE_DATA fad;
	memset(&fad,0,sizeof(fad));
	GetFileAttributesEx(fileName,GetFileExInfoStandard,&fad);
	int fLen = fad.nFileSizeLow;
	std::ifstream in;
	in.open(fileName);
	if(in.fail())
		return false;
	unsigned char* fBuf = new unsigned char[fLen];
	in.read((char*)fBuf,fLen);
	bool ret = load(fileName , fBuf, fLen , arg);
	delete [] fBuf;
	in.close();
	return ret;
}


/***
Vertex Shader
***/
xGL2VertexShader::xGL2VertexShader(IRenderApi* pOwner):xGL2Shader(pOwner)
{
     m_pVertexShader = NULL;
}

xGL2VertexShader::~xGL2VertexShader()
{
    unload();
}

bool  xGL2VertexShader::installShaderConstTable(IShaderParamTable* pShaderParamBuffer)
{
	xGL2RenderApi* pD10Api = (xGL2RenderApi*)m_pRenderApi;
	//pD10Api->d10Device()->VSSetConstantBuffers()
	return true;
}


bool xGL2VertexShader::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	unload();

	ID3D10Blob* pShaderCode = NULL;
	ID3D10Blob* pErroMsg    = NULL;
	GL2_SHADER_MACRO* pMacroDef = NULL;
	ID3D10Include*      pInclude  = NULL;
	const char*         func_name = "main";

	DWORD dwShaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= GL2_SHADER_DEBUG;
#endif

    HRESULT  hr = GL2CompileShader((LPCSTR)buf,bufLen,NULL,pMacroDef,pInclude,func_name,"vs_4_0",dwShaderFlags,&pShaderCode,&pErroMsg );

	if( FAILED(hr) )
	{
		const char* strErroMsg = (const char*)pErroMsg->GetBufferPointer();
		XEVOL_LOG(eXL_ERROR_FALT,strErroMsg);
		XSAFE_RELEASE(pShaderCode);
		XSAFE_RELEASE(pErroMsg);
		return false;
	}

	 xGL2RenderApi* pD10Api = (xGL2RenderApi*)m_pRenderApi;
	 hr = pD10Api->d10Device()->CreateVertexShader(pShaderCode->GetBufferPointer() , pShaderCode->GetBufferSize() , &m_pVertexShader);
	 if( FAILED(hr) )
	 {
		 XSAFE_RELEASE(pShaderCode);
		 XSAFE_RELEASE(pErroMsg);
		 return false;
	 }

	afterLoad(pShaderCode);
	XSAFE_RELEASE(pShaderCode);
	XSAFE_RELEASE(pErroMsg);
	m_Name = fileName;
	return true;
}

bool xGL2VertexShader::isLoaded()
{
    return m_pVertexShader != NULL;
}

bool xGL2VertexShader::unload()
{
	xGL2Shader::unload();
    XSAFE_RELEASE(m_pVertexShader);
	return true;
}

/***
Pixel Shader
***/
xGL2PixelShader::xGL2PixelShader(IRenderApi* pOwner):xGL2Shader(pOwner)
{
	m_pPixelShader = NULL;
}

xGL2PixelShader::~xGL2PixelShader()
{
	unload();
}



bool  xGL2PixelShader::installShaderConstTable(IShaderParamTable* pShaderParamBuffer)
{
	xGL2RenderApi* pD10Api = (xGL2RenderApi*)m_pRenderApi;
	//pD10Api->d10Device()->VSSetConstantBuffers()

	return true;
}



bool xGL2PixelShader::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	unload();

	ID3D10Blob* pShaderCode = NULL;
	ID3D10Blob* pErroMsg    = NULL;
	GL2_SHADER_MACRO* pMacroDef = NULL;
	ID3D10Include*      pInclude  = NULL;
	const char*         func_name = "main";

	DWORD dwShaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= GL2_SHADER_DEBUG;
#endif

	HRESULT  hr = GL2CompileShader((LPCSTR)buf,bufLen,NULL,pMacroDef,pInclude,func_name,"ps_4_0",dwShaderFlags,&pShaderCode,&pErroMsg );

	if( FAILED(hr) )
	{
		const char* strErroMsg = (const char*)pErroMsg->GetBufferPointer();
		XEVOL_LOG(eXL_ERROR_FALT,strErroMsg);
		XSAFE_RELEASE(pShaderCode);
		XSAFE_RELEASE(pErroMsg);
		return false;
	}

	xGL2RenderApi* pD10Api = (xGL2RenderApi*)m_pRenderApi;
	hr = pD10Api->d10Device()->CreatePixelShader(pShaderCode->GetBufferPointer() , pShaderCode->GetBufferSize() , &m_pPixelShader);
	if( FAILED(hr) )
	{
		XSAFE_RELEASE(pShaderCode);
		XSAFE_RELEASE(pErroMsg);
		return false;
	}

	afterLoad(pShaderCode);

	XSAFE_RELEASE(pShaderCode);
	XSAFE_RELEASE(pErroMsg);
	m_Name = fileName;
	return true;
}

bool xGL2PixelShader::isLoaded()
{
	return m_pPixelShader != NULL;
}

bool xGL2PixelShader::unload()
{
	xGL2Shader::unload();
	XSAFE_RELEASE(m_pPixelShader);
	return true;
}

/***
Pixel Shader
***/
xGL2GeomShader::xGL2GeomShader(IRenderApi* pOwner):xGL2Shader(pOwner)
{
	m_pGeomShader = NULL;
}

xGL2GeomShader::~xGL2GeomShader()
{
	unload();
}

bool  xGL2GeomShader::installShaderConstTable(IShaderParamTable* pShaderParamBuffer)
{
	xGL2RenderApi* pD10Api = (xGL2RenderApi*)m_pRenderApi;
	//pD10Api->d10Device()->VSSetConstantBuffers()

	return true;
}



bool xGL2GeomShader::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	unload();

	ID3D10Blob* pShaderCode = NULL;
	ID3D10Blob* pErroMsg    = NULL;
	GL2_SHADER_MACRO* pMacroDef = NULL;
	ID3D10Include*      pInclude  = NULL;
	const char*         func_name = "main";

	DWORD dwShaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= GL2_SHADER_DEBUG;
#endif

	HRESULT  hr = GL2CompileShader((LPCSTR)buf,bufLen,NULL,pMacroDef,pInclude,func_name,"ps_4_0",dwShaderFlags,&pShaderCode,&pErroMsg );

	if( FAILED(hr) )
	{
		const char* strErroMsg = (const char*)pErroMsg->GetBufferPointer();
		XEVOL_LOG(eXL_ERROR_FALT,strErroMsg);
		XSAFE_RELEASE(pShaderCode);
		XSAFE_RELEASE(pErroMsg);
		return false;
	}

	xGL2RenderApi* pD10Api = (xGL2RenderApi*)m_pRenderApi;
	hr = pD10Api->d10Device()->CreateGeometryShader(pShaderCode->GetBufferPointer() , pShaderCode->GetBufferSize() , &m_pGeomShader);
	if( FAILED(hr) )
	{
		XSAFE_RELEASE(pShaderCode);
		XSAFE_RELEASE(pErroMsg);
		return false;
	}

	afterLoad(pShaderCode);
	XSAFE_RELEASE(pShaderCode);
	XSAFE_RELEASE(pErroMsg);
	m_Name = fileName;
	return true;
}

bool xGL2GeomShader::isLoaded()
{
	return m_pGeomShader != NULL;
}

bool xGL2GeomShader::unload()
{
	xGL2Shader::unload();
	XSAFE_RELEASE(m_pGeomShader);
	return true;
}


//===================...========================

//size_t   xGL2ShaderInputPin::nArg()
//{
//	return m_Args.size();
//}
//
//bool     xGL2ShaderInputPin::getArg(size_t idx , xShaderVariable& arg)
//{
//	if(m_Args.size() <= idx)
//		return false;
//	arg = m_Args[idx];
//	return true;
//}
//
//bool     xGL2ShaderInputPin::connect(IShaderNodePin* pin)
//{
//	return false;
//}

//bool     xGL2ShaderInputPin::__load(ID3D10ShaderReflection* pShaderReflection)
//{
//	m_Args.clear();
//	GL2_SHADER_DESC desc;
//	pShaderReflection->GetDesc(&desc);
//	for(int i = 0 ; i < desc.InputParameters ; i ++)
//	{
//		xShaderVariable arg;
//		GL2_SIGNATURE_PARAMETER_DESC ipdesc;
//		pShaderReflection->GetInputParameterDesc(i , &ipdesc);
//		wchar_t _semName[64] = {0};
//		XEvol_LocaleToUnicode( ipdesc.SemanticName , _semName , 64 );
//		arg.m_semantic = xRenderAPIDefInfo::shaderVarSemantic( _semName );
//		arg.m_strSemantic = _semName;
//		arg.m_semanticIdx = ipdesc.SemanticIndex;
//		switch(ipdesc.ComponentType)
//		{
//		case GL2_REGISTER_COMPONENT_UNKNOWN :
//			arg.m_dataType = SHADERVARTYPE_FLOAT4;
//			break;
//		case GL2_REGISTER_COMPONENT_UINT32:
//			arg.m_dataType = SHADERVARTYPE_UINT4;
//			break;
//		case GL2_REGISTER_COMPONENT_SINT32:
//			arg.m_dataType = SHADERVARTYPE_INT4;
//			break;
//		case GL2_REGISTER_COMPONENT_FLOAT32:
//			arg.m_dataType = SHADERVARTYPE_FLOAT4;
//			break;
//		}
//		arg.m_nColum = 4;
//		arg.m_nRow   = 1;
//		m_Args.push_back(arg);
//	}
//	return true;
//}

END_NAMESPACE_XEVOL3D
