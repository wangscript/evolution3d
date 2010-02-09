#ifndef _XEVOL_DIRECT3D10_SHADER_H__
#define _XEVOL_DIRECT3D10_SHADER_H__
#include <RenderAPI/xBaseShader.h>
#include "xD11ParamBuffer.h"
BEGIN_NAMESPACE_XEVOL3D

class xD11Shader ;
class xD11VertexShader;
class xD11PixelShader;
class xD11GeomShader;
class xD11ShaderParamTable;



//class xD11ShaderInputPin : public IShaderNodePin
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
class xD11Shader : public IBaseShader
{
    IMPL_BASE_OBJECT_INTERFACE(xD11Shader)
protected:
	ID3D11ShaderReflection*          m_pShaderReflect;
	xD11ShaderParamTable*            m_pConstTable;
	std::wstring                     m_Name;
	xD3D11RenderApi*                 m_pD11RenderApi;
	ID3D11ClassInstance *            m_DynamicLinkClass[MAX_DYNAMICLINK_CLASS];
	int                              m_nDynamicLinkClass;
public:
    xD11Shader(IRenderApi* pOwner);
	~xD11Shader();
	virtual bool           afterLoad(ID3D10Blob* pSrcCode);
	IShaderParamTable*     createShaderParamTable();
	virtual bool           installShaderConstTable(IShaderParamTable* pShaderParamBuffer) = 0;
    IShaderParamTable*     getShaderParamTable();
	void                   commitParamTable();
	int                    nDynamicLinkClas(){ return m_nDynamicLinkClass ; }
	ID3D11ClassInstance ** DynamicLinkClass(){ if(m_DynamicLinkClass == 0) return NULL ; return m_DynamicLinkClass; }
   
public:
	virtual bool           load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	virtual bool           unload() ;
	bool                   load(const wchar_t* fileName , unsigned long  arg);
	unsigned long          memUsage();

protected:
	virtual bool           createShader(ID3D10Blob* pShaderCode , ID3D11ClassLinkage * pClassLinkage) = 0;
};

class xD11VertexShader : public xD11Shader
{
public:
	ID3D11VertexShader* m_pVertexShader;

	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD11VertexShader)
public:
	xD11VertexShader(IRenderApi* pOwner);
	~xD11VertexShader();

	bool          installShaderConstTable(IShaderParamTable* pShaderParamBuffer);
	bool          createShader(ID3D10Blob* pShaderCode , ID3D11ClassLinkage * pClassLinkage);
	bool          isLoaded();
	bool          unload();
public:
	eShaderType   type(){return eShader_VertexShader ; }
};

class xD11PixelShader : public xD11Shader
{
public:
	ID3D11PixelShader* m_pPixelShader;
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD11PixelShader)
public:
	xD11PixelShader(IRenderApi* pOwner);
	~xD11PixelShader();

	bool          installShaderConstTable(IShaderParamTable* pShaderParamBuffer);
	bool          createShader(ID3D10Blob* pShaderCode , ID3D11ClassLinkage * pClassLinkage);
	bool          isLoaded();
	bool          unload();
public:
	eShaderType   type(){return eShader_PixelShader ; }
};


class xD11GeomShader : public xD11Shader
{
public:
	ID3D11GeometryShader* m_pGeomShader;
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD11GeomShader)
public:
	xD11GeomShader(IRenderApi* pOwner);
	~xD11GeomShader();

	bool          installShaderConstTable(IShaderParamTable* pShaderParamBuffer);
	bool          createShader(ID3D10Blob* pShaderCode , ID3D11ClassLinkage * pClassLinkage);
	bool          isLoaded();
	bool          unload();
public:
	eShaderType   type(){return eShader_GeometryShader ; }
};

class xD11HullShader : public xD11Shader
{
public:
	ID3D11HullShader*  m_pHullShader;
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD11HullShader)
public:
	xD11HullShader(IRenderApi* pOwner);
	~xD11HullShader();

	bool          installShaderConstTable(IShaderParamTable* pShaderParamBuffer);
	bool          createShader(ID3D10Blob* pShaderCode , ID3D11ClassLinkage * pClassLinkage);
	bool          isLoaded();
	bool          unload();
public:
	eShaderType   type(){return eShader_HullShader ; }
};


class xD11DomainShader : public xD11Shader
{
public:
	ID3D11DomainShader*  m_pDomainShader;
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD11DomainShader)
public:
	xD11DomainShader(IRenderApi* pOwner);
	~xD11DomainShader();

	bool          installShaderConstTable(IShaderParamTable* pShaderParamBuffer);
	bool          createShader(ID3D10Blob* pShaderCode , ID3D11ClassLinkage * pClassLinkage);
	bool          isLoaded();
	bool          unload();
public:
	eShaderType   type(){return eShader_DomainShader ; }
};


class xD11ComputeShader : public xD11Shader
{
public:
	ID3D11ComputeShader*  m_pComputeShader;
public:
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD11ComputeShader)
public:
	xD11ComputeShader(IRenderApi* pOwner);
	~xD11ComputeShader();

	bool          installShaderConstTable(IShaderParamTable* pShaderParamBuffer);
	bool          createShader(ID3D10Blob* pShaderCode , ID3D11ClassLinkage * pClassLinkage);
	bool          isLoaded();
	bool          unload();
public:
	eShaderType   type(){return eShader_ComputeShader ; }
};

END_NAMESPACE_XEVOL3D

#endif

