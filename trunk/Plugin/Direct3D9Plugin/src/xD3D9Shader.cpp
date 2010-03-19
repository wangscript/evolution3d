#include "stdafx.h"
#include "xD3D9Shader.h"
#include "xDIRECT3D9API.h"
#include "xD3D9ParamBuffer.h"
#include "xD3D9Sampler.h"
#include "xD3D9ConstLexer.h"
#include <fstream>
#include <BaseLib/xI18N.h>
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xD3D9Shader , IBaseShader)
IMPL_BASE_OBJECT_CLASSID(xD3D9VertexShader , xD3D9Shader)
IMPL_BASE_OBJECT_CLASSID(xD3D9PixelShader  , xD3D9Shader)

IShaderParamTable* xD3D9Shader::createShaderParamTable()
{
    char* pData = new char[1024];

    xD3D9ShaderParamTable* pShaderCT =  new xD3D9ShaderParamTable(this);
	//-----¼ÓÔØÕâ¸öShaderConstTable;
	pShaderCT->load(m_pShaderReflect);
	return pShaderCT;
}

IShaderParamTable* xD3D9Shader::getShaderParamTable()
{
	if(m_pParamTable == NULL)
	{
		m_pParamTable = new xD3D9ShaderParamTable(this);
		m_pParamTable->load(m_pShaderReflect);
	}
	return m_pParamTable ;
}

void xD3D9Shader::commitParamTable()
{
	getShaderParamTable();
	if(m_pParamTable)
	{
		m_pParamTable->updateRenderContext();
		m_pParamTable->commit();
	}
}

xD3D9Shader::xD3D9Shader(IRenderApi* pOwner):IBaseShader(pOwner)
{
	m_pD3D9RenderApi   = dynamic_cast<xD3D9RenderApi*>(pOwner);
	m_pShaderReflect = NULL;
	m_pParamTable    = NULL;
	m_RefCount       = 1;
}

xD3D9Shader::~xD3D9Shader()
{
    unload();
}

unsigned long xD3D9Shader::memUsage()
{
	return 1;
}

bool  xD3D9Shader::unload()
{
	XSAFE_RELEASE(m_pShaderReflect);
    XSAFE_DELETE(m_pParamTable);
	return true;
}
bool xD3D9Shader::load(const wchar_t* fileName , unsigned long  arg)
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
bool xD3D9Shader::load(const wchar_t* fileName , const unsigned int8* srcCode , size_t strLen, unsigned long arg)
{
	unload();

    const char*         func_name = "main";

    DWORD dwShaderFlags = 0;//D3DXSHADER_USE_LEGACY_D3DX9_31_DLL;

    LPD3DXBUFFER pCode     = NULL;                  // Buffer with the assembled shader code
    LPD3DXBUFFER pErrorMsgs= NULL; // Buffer with error messages

	const char* strProfileName = m_pD3D9RenderApi->getShaderProfile( type() );
    HRESULT hr = D3DXCompileShader((LPCSTR)srcCode,strLen,NULL,NULL, func_name ,strProfileName, dwShaderFlags ,&pCode,&pErrorMsgs,&m_pShaderReflect);
    if(pCode != 0)
    {
        long* pCodeBuffer = NULL;
        pCodeBuffer = (long*)pCode->GetBufferPointer();
        if(pCodeBuffer == NULL)
        {
            pCode->Release();
            return false;
        }
        this->createShader((const DWORD*)pCodeBuffer);
     }
    else
    {
        if(pErrorMsgs)
        {
            char* erromesg = (char*)pErrorMsgs->GetBufferPointer();
            XEVOL_LOG(eXL_ERROR_FALT , erromesg );
        }
        return false;
    }

	XSAFE_RELEASE(pCode);
	XSAFE_RELEASE(pErrorMsgs);
	m_Name = fileName;
	return true;
}


/***
Vertex Shader
***/
xD3D9VertexShader::xD3D9VertexShader(IRenderApi* pOwner):xD3D9Shader(pOwner)
{
     m_pVertexShader = NULL;
}

xD3D9VertexShader::~xD3D9VertexShader()
{
    unload();
}

bool  xD3D9VertexShader::installShaderConstTable(IShaderParamTable* pShaderParamBuffer)
{
	xD3D9RenderApi* pD11Api = (xD3D9RenderApi*)m_pRenderApi;
	//pD11Api->d10Device()->VSSetConstantBuffers()
	return true;
}

bool  xD3D9VertexShader::createShader(const DWORD* pCodeBuffer)
{
     IDirect3DDevice9* pD3DDevice = m_pD3D9RenderApi->getDevice();
     HRESULT hr =  pD3DDevice->CreateVertexShader(pCodeBuffer,&m_pVertexShader);
	 return SUCCEEDED(hr);
}

bool xD3D9VertexShader::isLoaded()
{
    return m_pVertexShader != NULL;
}

bool xD3D9VertexShader::unload()
{
	xD3D9Shader::unload();
    XSAFE_RELEASE(m_pVertexShader);
	return true;
}

/***
Pixel Shader
***/
xD3D9PixelShader::xD3D9PixelShader(IRenderApi* pOwner):xD3D9Shader(pOwner)
{
	m_pPixelShader = NULL;
}

xD3D9PixelShader::~xD3D9PixelShader()
{
	unload();
}



bool  xD3D9PixelShader::installShaderConstTable(IShaderParamTable* pShaderParamBuffer)
{
	xD3D9RenderApi* pD11Api = (xD3D9RenderApi*)m_pRenderApi;
	//pD11Api->d10Device()->VSSetConstantBuffers()

	return true;
}


bool  xD3D9PixelShader::createShader(const DWORD* pCodeBuffer)
{
    IDirect3DDevice9* pD3DDevice = m_pD3D9RenderApi->getDevice();
    HRESULT hr =  pD3DDevice->CreatePixelShader(pCodeBuffer,&m_pPixelShader);
    return SUCCEEDED(hr);
}

bool xD3D9PixelShader::isLoaded()
{
	return m_pPixelShader != NULL;
}

bool xD3D9PixelShader::unload()
{
	xD3D9Shader::unload();
	XSAFE_RELEASE(m_pPixelShader);
	return true;
}

END_NAMESPACE_XEVOL3D
