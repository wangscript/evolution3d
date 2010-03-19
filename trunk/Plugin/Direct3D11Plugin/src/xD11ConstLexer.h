#ifndef _XEVOL_DIRECT3D11_CONSTLEXER_H_
#define _XEVOL_DIRECT3D11_CONSTLEXER_H_
#include <RenderApi/xRenderApi.h>
#include <string>
#include <vector>
#include <map>
BEGIN_NAMESPACE_XEVOL3D

struct xD11GIFormatInfo
{
	DXGI_FORMAT   m_dxfmt;
	ds_wstring    m_desc;
	int           m_compont;
	int           m_bytePerComponent;
	int           m_CompressRate;
	ePIXEL_FORMAT m_fmt;
};
typedef std::map<DXGI_FORMAT   , int> MapsDXGIToIndex;
typedef std::map<ePIXEL_FORMAT , int> MapsXEvolToIndex;
class xD11ConstLexer
{
	std::vector<xD11GIFormatInfo> m_vForamts;
	MapsDXGIToIndex               m_dxindex;
	MapsXEvolToIndex              m_fmtIndex;
protected:
	void __addDXGIFormat(ePIXEL_FORMAT fmt , DXGI_FORMAT dxfmt , const wchar_t* descStr , int nCompont , int _byte , int cpRate=1);
public:
	static xD11ConstLexer* singleton();
    xD11GIFormatInfo*      GetPixelFormat( DXGI_FORMAT dxfmt );
	xD11GIFormatInfo*      GetPixelFormat( ePIXEL_FORMAT fmt );

	xD11ConstLexer();
	~xD11ConstLexer();
	
	static D3D11_FILTER               GetFilter(const wchar_t* _min_val , const wchar_t* _mag_val , const wchar_t* _mip_val ,  bool bCamp);
	static D3D11_TEXTURE_ADDRESS_MODE GetAdress(const wchar_t* _addressMode);
	static D3D11_COMPARISON_FUNC      GetCompareFunc(const wchar_t* _func);
	static int                        GetTextureSlotIdx(const wchar_t* _texName);
	static D3D11_BLEND                GetBlendFactor(const wchar_t* _blen);
	static D3D11_BLEND_OP             GetBlendOp(const wchar_t* _blen);
	static D3D11_FILL_MODE            GetFillMode(const wchar_t* _blen);
	static D3D11_CULL_MODE            GetCullMode(const wchar_t* _blen);
	static D3D11_STENCIL_OP           GetStencilOp(const wchar_t* _op);
	static D3DX11_IMAGE_FILE_FORMAT   GetDX11ImageFileFormat(const wchar_t* fileName);

    static D3D11_USAGE                Usage(eResourceUsage usage)
    {
        if(usage == RESOURCE_USAGE_DEFAULT    ) return D3D11_USAGE_DEFAULT  ;
        if(usage == RESOURCE_USAGE_IMMUTABLE  ) return D3D11_USAGE_IMMUTABLE;
        if(usage == RESOURCE_USAGE_DYNAMIC    ) return D3D11_USAGE_DYNAMIC  ;
        if(usage == RESOURCE_USAGE_STAGING    ) return D3D11_USAGE_STAGING  ;
        return D3D11_USAGE_DYNAMIC;
    }

    static D3D11_CPU_ACCESS_FLAG     AccessFlag(eResourceAccessFlage flag)
    {
        if(flag == RESOURCE_ACCESS_NONE      )  return D3D11_CPU_ACCESS_FLAG(0);
        if(flag == RESOURCE_ACCESS_READ      )  return D3D11_CPU_ACCESS_READ;
        if(flag == RESOURCE_ACCESS_WRITE     )  return D3D11_CPU_ACCESS_WRITE;
        if(flag == RESOURCE_ACCESS_READWRITE )  return D3D11_CPU_ACCESS_FLAG(D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE);
        return D3D11_CPU_ACCESS_WRITE;
    }

    static D3D11_BIND_FLAG          BindFlag (eResourceBindType bindType)
    {
        D3D11_BIND_FLAG outFlag = D3D11_BIND_FLAG(0);
        if(bindType & BIND_AS_CONSTANT_BUFFER       )  outFlag =  (D3D11_BIND_FLAG)(outFlag |D3D11_BIND_CONSTANT_BUFFER);
        if(bindType & BIND_AS_INDEX_BUFFER          )  outFlag =  (D3D11_BIND_FLAG)(outFlag |D3D11_BIND_INDEX_BUFFER   );
        if(bindType & BIND_AS_CONSTANT_BUFFER       )  outFlag =  (D3D11_BIND_FLAG)(outFlag |D3D11_BIND_CONSTANT_BUFFER);
        if(bindType & BIND_AS_SHADER_RESOURCE       )  outFlag =  (D3D11_BIND_FLAG)(outFlag |D3D11_BIND_SHADER_RESOURCE);
        if(bindType & BIND_AS_STREAM_OUTPUT         )  outFlag =  (D3D11_BIND_FLAG)(outFlag |D3D11_BIND_STREAM_OUTPUT  );
        if(bindType & BIND_AS_RENDER_TARGET         )  outFlag =  (D3D11_BIND_FLAG)(outFlag |D3D11_BIND_RENDER_TARGET  );
        return outFlag;

    }

};
END_NAMESPACE_XEVOL3D

#endif

