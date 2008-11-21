#include "xD10Shader.h"
#include "xDirect3D10API.h"
#include "xD10ParamBuffer.h"
#include "xD10Sampler.h"
#include "xD10ConstLexer.h"
#include <fstream>
#include <BaseLib/xI18N.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD10Shader , IBaseShader)
IMPL_BASE_OBJECT_CLASSID(xD10VertexShader , xD10Shader)
IMPL_BASE_OBJECT_CLASSID(xD10PixelShader  , IBaseShader)
IMPL_BASE_OBJECT_CLASSID(xD10GeomShader   , IBaseShader)


IShaderParamTable* xD10Shader::createShaderParamTable()
{
    xD10ShaderParamTable* pShaderCT =  new xD10ShaderParamTable(this);
	//-----¼ÓÔØÕâ¸öShaderConstTable;
	pShaderCT->load(m_pShaderReflect);
	return pShaderCT;
}

IShaderParamTable* xD10Shader::getShaderParamTable()
{
	if(m_pConstTable == NULL)
	{
		m_pConstTable = new xD10ShaderParamTable(this);
		m_pConstTable->load(m_pShaderReflect);
	}
	return m_pConstTable ;
}

void xD10Shader::commitParamTable()
{
	getShaderParamTable();
	if(m_pConstTable)
	{
		m_pConstTable->updateRenderContext();
		m_pConstTable->commit();
	}
}

xD10Shader::xD10Shader(IRenderApi* pOwner):IBaseShader(pOwner)
{
	m_pShaderReflect = NULL;
	m_pConstTable    = NULL;
	m_RefCount       = 1;
}

xD10Shader::~xD10Shader()
{
    unload();
}

unsigned long xD10Shader::memUsage()
{
	return 1;
}

bool xD10Shader::afterLoad(ID3D10Blob* pShaderCode)
{
   HRESULT hr = D3D10ReflectShader(pShaderCode->GetBufferPointer() , pShaderCode->GetBufferSize() , &m_pShaderReflect);
   if(FAILED(hr))
   {
	   return false;
   }
   return true;
}

bool  xD10Shader::unload()
{
	XSAFE_RELEASE(m_pShaderReflect);
	return true;
}
bool xD10Shader::load(const wchar_t* fileName , unsigned long  arg)
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
xD10VertexShader::xD10VertexShader(IRenderApi* pOwner):xD10Shader(pOwner)
{
     m_pVertexShader = NULL;
}

xD10VertexShader::~xD10VertexShader()
{
    unload();
}

bool  xD10VertexShader::installShaderConstTable(IShaderParamTable* pShaderParamBuffer)
{
	xD3D10RenderApi* pD10Api = (xD3D10RenderApi*)m_pRenderApi;
	//pD10Api->d10Device()->VSSetConstantBuffers()
	return true;
}


bool xD10VertexShader::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	unload();

	ID3D10Blob* pShaderCode = NULL;
	ID3D10Blob* pErroMsg    = NULL;
	D3D10_SHADER_MACRO* pMacroDef = NULL;
	ID3D10Include*      pInclude  = NULL;
	const char*         func_name = "main";

	DWORD dwShaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

    HRESULT  hr = D3D10CompileShader((LPCSTR)buf,bufLen,NULL,pMacroDef,pInclude,func_name,"vs_4_0",dwShaderFlags,&pShaderCode,&pErroMsg );

	if( FAILED(hr) )
	{
		const char* strErroMsg = (const char*)pErroMsg->GetBufferPointer();
		XEVOL_LOG(eXL_ERROR_FALT,strErroMsg);
		XSAFE_RELEASE(pShaderCode);
		XSAFE_RELEASE(pErroMsg);
		return false;
	}

	 xD3D10RenderApi* pD10Api = (xD3D10RenderApi*)m_pRenderApi;
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

bool xD10VertexShader::isLoaded()
{
    return m_pVertexShader != NULL;
}

bool xD10VertexShader::unload()
{
	xD10Shader::unload();
    XSAFE_RELEASE(m_pVertexShader);
	return true;
}

/***
Pixel Shader
***/
xD10PixelShader::xD10PixelShader(IRenderApi* pOwner):xD10Shader(pOwner)
{
	m_pPixelShader = NULL;
}

xD10PixelShader::~xD10PixelShader()
{
	unload();
}



bool  xD10PixelShader::installShaderConstTable(IShaderParamTable* pShaderParamBuffer)
{
	xD3D10RenderApi* pD10Api = (xD3D10RenderApi*)m_pRenderApi;
	//pD10Api->d10Device()->VSSetConstantBuffers()

	return true;
}



bool xD10PixelShader::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	unload();

	ID3D10Blob* pShaderCode = NULL;
	ID3D10Blob* pErroMsg    = NULL;
	D3D10_SHADER_MACRO* pMacroDef = NULL;
	ID3D10Include*      pInclude  = NULL;
	const char*         func_name = "main";

	DWORD dwShaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	HRESULT  hr = D3D10CompileShader((LPCSTR)buf,bufLen,NULL,pMacroDef,pInclude,func_name,"ps_4_0",dwShaderFlags,&pShaderCode,&pErroMsg );

	if( FAILED(hr) )
	{
		const char* strErroMsg = (const char*)pErroMsg->GetBufferPointer();
		XEVOL_LOG(eXL_ERROR_FALT,strErroMsg);
		XSAFE_RELEASE(pShaderCode);
		XSAFE_RELEASE(pErroMsg);
		return false;
	}

	xD3D10RenderApi* pD10Api = (xD3D10RenderApi*)m_pRenderApi;
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

bool xD10PixelShader::isLoaded()
{
	return m_pPixelShader != NULL;
}

bool xD10PixelShader::unload()
{
	xD10Shader::unload();
	XSAFE_RELEASE(m_pPixelShader);
	return true;
}

/***
Pixel Shader
***/
xD10GeomShader::xD10GeomShader(IRenderApi* pOwner):xD10Shader(pOwner)
{
	m_pGeomShader = NULL;
}

xD10GeomShader::~xD10GeomShader()
{
	unload();
}

bool  xD10GeomShader::installShaderConstTable(IShaderParamTable* pShaderParamBuffer)
{
	xD3D10RenderApi* pD10Api = (xD3D10RenderApi*)m_pRenderApi;
	//pD10Api->d10Device()->VSSetConstantBuffers()

	return true;
}



bool xD10GeomShader::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	unload();

	ID3D10Blob* pShaderCode = NULL;
	ID3D10Blob* pErroMsg    = NULL;
	D3D10_SHADER_MACRO* pMacroDef = NULL;
	ID3D10Include*      pInclude  = NULL;
	const char*         func_name = "main";

	DWORD dwShaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	HRESULT  hr = D3D10CompileShader((LPCSTR)buf,bufLen,NULL,pMacroDef,pInclude,func_name,"ps_4_0",dwShaderFlags,&pShaderCode,&pErroMsg );

	if( FAILED(hr) )
	{
		const char* strErroMsg = (const char*)pErroMsg->GetBufferPointer();
		XEVOL_LOG(eXL_ERROR_FALT,strErroMsg);
		XSAFE_RELEASE(pShaderCode);
		XSAFE_RELEASE(pErroMsg);
		return false;
	}

	xD3D10RenderApi* pD10Api = (xD3D10RenderApi*)m_pRenderApi;
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

bool xD10GeomShader::isLoaded()
{
	return m_pGeomShader != NULL;
}

bool xD10GeomShader::unload()
{
	xD10Shader::unload();
	XSAFE_RELEASE(m_pGeomShader);
	return true;
}


//===================...========================

//size_t   xD10ShaderInputPin::nArg()
//{
//	return m_Args.size();
//}
//
//bool     xD10ShaderInputPin::getArg(size_t idx , xShaderVariable& arg)
//{
//	if(m_Args.size() <= idx)
//		return false;
//	arg = m_Args[idx];
//	return true;
//}
//
//bool     xD10ShaderInputPin::connect(IShaderNodePin* pin)
//{
//	return false;
//}

//bool     xD10ShaderInputPin::__load(ID3D10ShaderReflection* pShaderReflection)
//{
//	m_Args.clear();
//	D3D10_SHADER_DESC desc;
//	pShaderReflection->GetDesc(&desc);
//	for(int i = 0 ; i < desc.InputParameters ; i ++)
//	{
//		xShaderVariable arg;
//		D3D10_SIGNATURE_PARAMETER_DESC ipdesc;
//		pShaderReflection->GetInputParameterDesc(i , &ipdesc);
//		wchar_t _semName[64] = {0};
//		XEvol_LocaleToUnicode( ipdesc.SemanticName , _semName , 64 );
//		arg.m_semantic = xRenderAPIDefInfo::shaderVarSemantic( _semName );
//		arg.m_strSemantic = _semName;
//		arg.m_semanticIdx = ipdesc.SemanticIndex;
//		switch(ipdesc.ComponentType)
//		{
//		case D3D10_REGISTER_COMPONENT_UNKNOWN :
//			arg.m_dataType = SHADERVARTYPE_FLOAT4;
//			break;
//		case D3D10_REGISTER_COMPONENT_UINT32:
//			arg.m_dataType = SHADERVARTYPE_UINT4;
//			break;
//		case D3D10_REGISTER_COMPONENT_SINT32:
//			arg.m_dataType = SHADERVARTYPE_INT4;
//			break;
//		case D3D10_REGISTER_COMPONENT_FLOAT32:
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
