#ifndef _XEVOL_DIRECT3D10_SHADER_H__
#define _XEVOL_DIRECT3D10_SHADER_H__
#include <RenderAPI/xBaseShader.h>
#include <D3D10.h>
#include <d3d10shader.h>
#include "xD10ParamBuffer.h"
BEGIN_NAMESPACE_XEVOL3D

class xD10Shader ;
class xD10VertexShader;
class xD10PixelShader;
class xD10GeomShader;
class xD10ShaderParamTable;



//class xD10ShaderInputPin : public IShaderNodePin
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


class xD10Shader : public IBaseShader
{
    IMPL_BASE_OBJECT_INTERFACE(xD10Shader)
protected:
	ID3D10ShaderReflection*          m_pShaderReflect;
	xD10ShaderParamTable*            m_pConstTable;
	std::wstring                     m_Name;
public:
    xD10Shader(IRenderApi* pOwner);
	~xD10Shader();
	virtual bool           afterLoad(ID3D10Blob* pSrcCode);
	IShaderParamTable*     createShaderParamTable();
	virtual bool           installShaderConstTable(IShaderParamTable* pShaderParamBuffer) = 0;
    IShaderParamTable*     getShaderParamTable();
	void                   commitParamTable();
    virtual void           setName(const wchar_t* name) { m_Name = name ; }
    const wchar_t*         name() { return m_Name.c_str() ; }
public:
	virtual bool           load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)=0;
	virtual bool           unload() ;
	bool                   load(const wchar_t* fileName , unsigned long  arg);
	unsigned long          memUsage();
};

class xD10VertexShader : public xD10Shader
{
public:
	ID3D10VertexShader* m_pVertexShader;

	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD10VertexShader)
public:
	xD10VertexShader(IRenderApi* pOwner);
	~xD10VertexShader();

	bool          installShaderConstTable(IShaderParamTable* pShaderParamBuffer);
	bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
public:
	eShaderType   type(){return eShader_VertexShader ; }
};

class xD10PixelShader : public xD10Shader
{
public:
	ID3D10PixelShader* m_pPixelShader;
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD10PixelShader)
public:
	xD10PixelShader(IRenderApi* pOwner);
	~xD10PixelShader();

	bool          installShaderConstTable(IShaderParamTable* pShaderParamBuffer);
	bool          load(const wchar_t* fileName ,const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
public:
	eShaderType   type(){return eShader_PixelShader ; }
};


class xD10GeomShader : public xD10Shader
{
public:
	ID3D10GeometryShader* m_pGeomShader;
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD10GeomShader)
public:
	xD10GeomShader(IRenderApi* pOwner);
	~xD10GeomShader();

	bool          installShaderConstTable(IShaderParamTable* pShaderParamBuffer);
	bool          load(const wchar_t* fileName ,const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
public:
	eShaderType   type(){return eShader_GeometryShader ; }
};

END_NAMESPACE_XEVOL3D

#endif

