#ifndef _XEVOL_OPENGL20_SHADER_H__
#define _XEVOL_OPENGL20_SHADER_H__
#include <RenderAPI/xBaseShader.h>
#include <GL2.h>
#include <d3d10shader.h>
#include "xGL2ParamBuffer.h"
BEGIN_NAMESPACE_XEVOL3D

class xGL2Shader ;
class xGL2VertexShader;
class xGL2PixelShader;
class xGL2GeomShader;
class xGL2ShaderParamTable;



//class xGL2ShaderInputPin : public IShaderNodePin
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


class xGL2Shader : public IBaseShader
{
    IMPL_BASE_OBJECT_INTERFACE(xGL2Shader)
protected:
	ID3D10ShaderReflection*          m_pShaderReflect;
	ID3D10Blob*                      m_pShaderCode;
	xGL2ShaderParamTable*            m_pConstTable;
	std::wstring                     m_Name;

public:
    xGL2Shader(IRenderApi* pOwner);
	~xGL2Shader();
	virtual bool           afterLoad(ID3D10Blob* pSrcCode);
	IShaderParamTable*     createShaderParamTable();
	virtual bool           installShaderConstTable(IShaderParamTable* pShaderParamBuffer) = 0;
    IShaderParamTable*     getShaderParamTable();
	void                   commitParamTable();

public:
	virtual bool           load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)=0;
	virtual bool           unload() ;
	bool                   load(const wchar_t* fileName , unsigned long  arg);
	unsigned long          memUsage();
};

class xGL2VertexShader : public xGL2Shader
{
public:
	ID3D10VertexShader* m_pVertexShader;

	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xGL2VertexShader)
public:
	xGL2VertexShader(IRenderApi* pOwner);
	~xGL2VertexShader();

	bool          installShaderConstTable(IShaderParamTable* pShaderParamBuffer);
	bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
public:
	eShaderType   type(){return eShader_VertexShader ; }
};

class xGL2PixelShader : public xGL2Shader
{
public:
	ID3D10PixelShader* m_pPixelShader;
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xGL2PixelShader)
public:
	xGL2PixelShader(IRenderApi* pOwner);
	~xGL2PixelShader();

	bool          installShaderConstTable(IShaderParamTable* pShaderParamBuffer);
	bool          load(const wchar_t* fileName ,const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
public:
	eShaderType   type(){return eShader_PixelShader ; }
};


class xGL2GeomShader : public xGL2Shader
{
public:
	ID3D10GeometryShader* m_pGeomShader;
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xGL2GeomShader)
public:
	xGL2GeomShader(IRenderApi* pOwner);
	~xGL2GeomShader();

	bool          installShaderConstTable(IShaderParamTable* pShaderParamBuffer);
	bool          load(const wchar_t* fileName ,const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool          isLoaded();
	bool          unload();
public:
	eShaderType   type(){return eShader_GeometryShader ; }
};

END_NAMESPACE_XEVOL3D

#endif

