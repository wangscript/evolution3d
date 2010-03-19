#ifndef _XEVOL_DIRECT3D10_SHADER_H__
#define _XEVOL_DIRECT3D10_SHADER_H__
#include <RenderAPI/xBaseShader.h>
#include "xD3D9ParamBuffer.h"
BEGIN_NAMESPACE_XEVOL3D

class xD3D9Shader ;
class xD3D9VertexShader;
class xD3D9PixelShader;
class xD3D9ShaderParamTable;



//class xD3D9ShaderInputPin : public IShaderNodePin
//{
//public:
//	vector<xShaderVariable> m_Args;
//public:
//	bool     __load(ID3D10ShaderReflection* pShaderReflection);
//public:
//	size_t   nArg();
//	bool     getArg(size_t idx , xShaderVariable& arg) ;
//	bool     connect(IShaderNodePin* pin);
//};

#define MAX_DYNAMICLINK_CLASS 32
class xD3D9Shader : public IBaseShader
{
    IMPL_BASE_OBJECT_INTERFACE(xD3D9Shader)
protected:
	xD3D9ShaderParamTable*           m_pParamTable;
    ID3DXConstantTable*              m_pShaderReflect;
	std::wstring                     m_Name;
	xD3D9RenderApi*                  m_pD3D9RenderApi;
public:
    xD3D9Shader(IRenderApi* pOwner);
    virtual ~xD3D9Shader();
    virtual IShaderParamTable*     createShaderParamTable();
    virtual bool                   load(const wchar_t* fileName , unsigned long  arg);
    virtual bool                   load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
    virtual void                   commitParamTable();
    virtual bool                   isLoaded(){ return m_pShaderReflect != NULL; }
    virtual bool                   unload() ;
    virtual unsigned long          memUsage() ;
    virtual eResourceType          res_type(){ return RESOURCE_SHADER ; }
    virtual void                   setName(const wchar_t* name) { m_Name = name ; }
    const wchar_t*                 name() { return m_Name.c_str() ; }
public:
    virtual eShaderType            type() = 0;
    IShaderParamTable*             getShaderParamTable();
protected:
	virtual bool           createShader(const DWORD* pBuffer) = 0;
};

class xD3D9VertexShader : public xD3D9Shader
{
public:
	IDirect3DVertexShader9* m_pVertexShader;

	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD3D9VertexShader)
public:
	xD3D9VertexShader(IRenderApi* pOwner);
	~xD3D9VertexShader();

	bool          installShaderConstTable(IShaderParamTable* pShaderParamBuffer);
	bool          createShader(const DWORD* pBuffer);
	bool          isLoaded();
	bool          unload();
public:
	eShaderType   type(){return eShader_VertexShader ; }
};

class xD3D9PixelShader : public xD3D9Shader
{
public:
	IDirect3DPixelShader9* m_pPixelShader;
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD3D9PixelShader)
public:
	xD3D9PixelShader(IRenderApi* pOwner);
	~xD3D9PixelShader();

	bool          installShaderConstTable(IShaderParamTable* pShaderParamBuffer);
	bool          createShader(const DWORD* pBuffer);
	bool          isLoaded();
	bool          unload();
public:
	eShaderType   type(){return eShader_PixelShader ; }
};


END_NAMESPACE_XEVOL3D

#endif

