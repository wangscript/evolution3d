#include "stdafx.h"
#include "xD11Shader.h"
#include "xDirect3D11API.h"
#include "xD11ParamBuffer.h"
#include "xD11Sampler.h"
#include "xD11ConstLexer.h"
#include <fstream>
#include <BaseLib/xI18N.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD11Shader , IBaseShader)
IMPL_BASE_OBJECT_CLASSID(xD11VertexShader , xD11Shader)
IMPL_BASE_OBJECT_CLASSID(xD11PixelShader  , xD11Shader)
IMPL_BASE_OBJECT_CLASSID(xD11GeomShader   , xD11Shader)
IMPL_BASE_OBJECT_CLASSID(xD11HullShader   , xD11Shader)
IMPL_BASE_OBJECT_CLASSID(xD11ComputeShader, xD11Shader)
IMPL_BASE_OBJECT_CLASSID(xD11DomainShader , xD11Shader)

IShaderParamTable* xD11Shader::createShaderParamTable()
{
    char* pData = new char[1024];

    xD11ShaderParamTable* pShaderCT =  new xD11ShaderParamTable(this);
	//-----¼ÓÔØÕâ¸öShaderConstTable;
	pShaderCT->load(m_pShaderReflect);
	return pShaderCT;
}

IShaderParamTable* xD11Shader::getShaderParamTable()
{
	if(m_pConstTable == NULL)
	{
		m_pConstTable = new xD11ShaderParamTable(this);
		m_pConstTable->load(m_pShaderReflect);
	}
	return m_pConstTable ;
}

void xD11Shader::commitParamTable()
{
	getShaderParamTable();
	if(m_pConstTable)
	{
		m_pConstTable->updateRenderContext();
		m_pConstTable->commit();
	}
}

xD11Shader::xD11Shader(IRenderApi* pOwner):IBaseShader(pOwner)
{
	m_pD11RenderApi = dynamic_cast<xD3D11RenderApi*>(pOwner);
	m_pShaderReflect = NULL;
	m_pConstTable    = NULL;
	m_RefCount       = 1;
	m_nDynamicLinkClass = 0;
}

xD11Shader::~xD11Shader()
{
    unload();
}

unsigned long xD11Shader::memUsage()
{
	return 1;
}

bool xD11Shader::afterLoad(ID3D10Blob* pShaderCode)
{
   HRESULT hr = D3DReflect(pShaderCode->GetBufferPointer() , pShaderCode->GetBufferSize() ,IID_ID3D11ShaderReflection , (void**)&m_pShaderReflect);
   if(FAILED(hr))
   {
	   return false;
   }
   return true;
}

bool  xD11Shader::unload()
{
	XSAFE_RELEASE(m_pShaderReflect);
	return true;
}
bool xD11Shader::load(const wchar_t* fileName , unsigned long  arg)
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
bool xD11Shader::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
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

	ID3DX11ThreadPump* pThreadPump = NULL;
	const char* strProfileName = m_pD11RenderApi->getShaderProfile( type() );
	DWORD dwEffectFlags = 0;
	HRESULT  hr = D3DX11CompileFromMemory((LPCSTR)buf,bufLen,NULL,pMacroDef,pInclude,func_name,strProfileName ,dwShaderFlags, dwEffectFlags, pThreadPump , &pShaderCode,&pErroMsg  , NULL);

	if( FAILED(hr) )
	{
		const char* strErroMsg = (const char*)pErroMsg->GetBufferPointer();
		XEVOL_LOG(eXL_ERROR_FALT,strErroMsg);
		XSAFE_RELEASE(pShaderCode);
		XSAFE_RELEASE(pErroMsg);
		return false;
	}

	xD3D11RenderApi* pD11Api = (xD3D11RenderApi*)m_pRenderApi;
	ID3D11ClassLinkage * pClassLinkage = NULL;
	if(false == this->createShader(pShaderCode , pClassLinkage) )
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


/***
Vertex Shader
***/
xD11VertexShader::xD11VertexShader(IRenderApi* pOwner):xD11Shader(pOwner)
{
     m_pVertexShader = NULL;
}

xD11VertexShader::~xD11VertexShader()
{
    unload();
}

bool  xD11VertexShader::installShaderConstTable(IShaderParamTable* pShaderParamBuffer)
{
	xD3D11RenderApi* pD11Api = (xD3D11RenderApi*)m_pRenderApi;
	//pD11Api->d10Device()->VSSetConstantBuffers()
	return true;
}

bool  xD11VertexShader::createShader(ID3D10Blob* pShaderCode , ID3D11ClassLinkage * pClassLinkage)
{

	 HRESULT hr = m_pD11RenderApi->d11Device()->CreateVertexShader(pShaderCode->GetBufferPointer() , pShaderCode->GetBufferSize() , NULL, &m_pVertexShader);
	 return SUCCEEDED(hr);
}

bool xD11VertexShader::isLoaded()
{
    return m_pVertexShader != NULL;
}

bool xD11VertexShader::unload()
{
	xD11Shader::unload();
    XSAFE_RELEASE(m_pVertexShader);
	return true;
}

/***
Pixel Shader
***/
xD11PixelShader::xD11PixelShader(IRenderApi* pOwner):xD11Shader(pOwner)
{
	m_pPixelShader = NULL;
}

xD11PixelShader::~xD11PixelShader()
{
	unload();
}



bool  xD11PixelShader::installShaderConstTable(IShaderParamTable* pShaderParamBuffer)
{
	xD3D11RenderApi* pD11Api = (xD3D11RenderApi*)m_pRenderApi;
	//pD11Api->d10Device()->VSSetConstantBuffers()

	return true;
}


bool  xD11PixelShader::createShader(ID3D10Blob* pShaderCode , ID3D11ClassLinkage * pClassLinkage)
{

	HRESULT hr = m_pD11RenderApi->d11Device()->CreatePixelShader(pShaderCode->GetBufferPointer() , pShaderCode->GetBufferSize() , NULL, &m_pPixelShader);
	return SUCCEEDED(hr);
}

bool xD11PixelShader::isLoaded()
{
	return m_pPixelShader != NULL;
}

bool xD11PixelShader::unload()
{
	xD11Shader::unload();
	XSAFE_RELEASE(m_pPixelShader);
	return true;
}

/***
Geometry Shader
***/
xD11GeomShader::xD11GeomShader(IRenderApi* pOwner):xD11Shader(pOwner)
{
	m_pGeomShader = NULL;
}

xD11GeomShader::~xD11GeomShader()
{
	unload();
}

bool  xD11GeomShader::installShaderConstTable(IShaderParamTable* pShaderParamBuffer)
{
	xD3D11RenderApi* pD11Api = (xD3D11RenderApi*)m_pRenderApi;
	//pD11Api->d10Device()->VSSetConstantBuffers()

	return true;
}



bool  xD11GeomShader::createShader(ID3D10Blob* pShaderCode , ID3D11ClassLinkage * pClassLinkage)
{

	HRESULT hr = m_pD11RenderApi->d11Device()->CreateGeometryShader(pShaderCode->GetBufferPointer() , pShaderCode->GetBufferSize() , NULL, &m_pGeomShader);
	return SUCCEEDED(hr);
}


bool xD11GeomShader::isLoaded()
{
	return m_pGeomShader != NULL;
}

bool xD11GeomShader::unload()
{
	xD11Shader::unload();
	XSAFE_RELEASE(m_pGeomShader);
	return true;
}
//HullShader
xD11HullShader::xD11HullShader(IRenderApi* pOwner):xD11Shader(pOwner)
{
	m_pHullShader = NULL;
}

xD11HullShader::~xD11HullShader()
{
	unload();
}

bool  xD11HullShader::installShaderConstTable(IShaderParamTable* pShaderParamBuffer)
{
	xD3D11RenderApi* pD11Api = (xD3D11RenderApi*)m_pRenderApi;
	//pD11Api->d10Device()->VSSetConstantBuffers()

	return true;
}

bool  xD11HullShader::createShader(ID3D10Blob* pShaderCode , ID3D11ClassLinkage * pClassLinkage)
{

	HRESULT hr = m_pD11RenderApi->d11Device()->CreateHullShader(pShaderCode->GetBufferPointer() , pShaderCode->GetBufferSize() , NULL, &m_pHullShader);
	return SUCCEEDED(hr);
}


bool xD11HullShader::isLoaded()
{
	return m_pHullShader != NULL;
}

bool xD11HullShader::unload()
{
	xD11Shader::unload();
	XSAFE_RELEASE(m_pHullShader);
	return true;
}

//DomainShader
xD11DomainShader::xD11DomainShader(IRenderApi* pOwner):xD11Shader(pOwner)
{
	m_pDomainShader = NULL;
}

xD11DomainShader::~xD11DomainShader()
{
	unload();
}

bool  xD11DomainShader::installShaderConstTable(IShaderParamTable* pShaderParamBuffer)
{
	xD3D11RenderApi* pD11Api = (xD3D11RenderApi*)m_pRenderApi;
	//pD11Api->d10Device()->VSSetConstantBuffers()

	return true;
}

bool  xD11DomainShader::createShader(ID3D10Blob* pShaderCode , ID3D11ClassLinkage * pClassLinkage)
{

	HRESULT hr = m_pD11RenderApi->d11Device()->CreateDomainShader(pShaderCode->GetBufferPointer() , pShaderCode->GetBufferSize() , NULL, &m_pDomainShader);
	return SUCCEEDED(hr);
}


bool xD11DomainShader::isLoaded()
{
	return m_pDomainShader != NULL;
}

bool xD11DomainShader::unload()
{
	xD11Shader::unload();
	XSAFE_RELEASE(m_pDomainShader);
	return true;
}

//ComputeShader
xD11ComputeShader::xD11ComputeShader(IRenderApi* pOwner):xD11Shader(pOwner)
{
	m_pComputeShader = NULL;
}

xD11ComputeShader::~xD11ComputeShader()
{
	unload();
}

bool  xD11ComputeShader::installShaderConstTable(IShaderParamTable* pShaderParamBuffer)
{
	xD3D11RenderApi* pD11Api = (xD3D11RenderApi*)m_pRenderApi;
	//pD11Api->d10Device()->VSSetConstantBuffers()

	return true;
}

bool  xD11ComputeShader::createShader(ID3D10Blob* pShaderCode , ID3D11ClassLinkage * pClassLinkage)
{

	HRESULT hr = m_pD11RenderApi->d11Device()->CreateComputeShader(pShaderCode->GetBufferPointer() , pShaderCode->GetBufferSize() , NULL, &m_pComputeShader);
	return SUCCEEDED(hr);
}


bool xD11ComputeShader::isLoaded()
{
	return m_pComputeShader != NULL;
}

bool xD11ComputeShader::unload()
{
	xD11Shader::unload();
	XSAFE_RELEASE(m_pComputeShader);
	return true;
}

END_NAMESPACE_XEVOL3D
