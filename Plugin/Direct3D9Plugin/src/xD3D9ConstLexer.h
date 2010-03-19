#ifndef _XEVOL_DIRECT3D9_CONSTLEXER_H_
#define _XEVOL_DIRECT3D9_CONSTLEXER_H_
#include <RenderApi/xRenderApi.h>
#include <string>
#include <vector>
#include <map>
BEGIN_NAMESPACE_XEVOL3D

struct xD3D9GIFormatInfo
{
	D3DFORMAT     m_dxfmt;
	ds_wstring    m_desc;
	int           m_compont;
	int           m_bytePerComponent;
	int           m_CompressRate;
	ePIXEL_FORMAT m_fmt;
};
typedef std::map<D3DFORMAT     , int> MapsDXGIToIndex;
typedef std::map<ePIXEL_FORMAT , int> MapsXEvolToIndex;

class xD3D9ConstLexer
{
	std::vector<xD3D9GIFormatInfo> m_vForamts;
	MapsDXGIToIndex               m_dxindex;
	MapsXEvolToIndex              m_fmtIndex;
protected:
	void __addDXGIFormat(ePIXEL_FORMAT fmt , D3DFORMAT dxfmt , const wchar_t* descStr , int nCompont , int _byte , int cpRate=1);
public:
	static xD3D9ConstLexer* singleton();
    xD3D9GIFormatInfo*      GetPixelFormat( D3DFORMAT  dxfmt );
	xD3D9GIFormatInfo*      GetPixelFormat( ePIXEL_FORMAT fmt );

	xD3D9ConstLexer();
	~xD3D9ConstLexer();
	
	static D3DTEXTUREFILTERTYPE   GetFilter(const wchar_t* _val);
	static D3DTEXTUREADDRESS      GetAdress(const wchar_t* _addressMode);
	static D3DCMPFUNC             GetCompareFunc(const wchar_t* _func);
	static int                    GetTextureSlotIdx(const wchar_t* _texName);
	static D3DBLEND               GetBlendFactor(const wchar_t* _blen);
	static D3DBLENDOP             GetBlendOp(const wchar_t* _blen);
	static D3DFILLMODE            GetFillMode(const wchar_t* _blen);
	static D3DCULL                GetCullMode(const wchar_t* _cull);
	static D3DSTENCILOP           GetStencilOp(const wchar_t* _op);
	static D3DXIMAGE_FILEFORMAT   GetDXImageFileFormat(const wchar_t* fileName);

    static D3DPOOL GetD3DPool(eResourceUsage usage)
    {
        if(usage == RESOURCE_USAGE_DEFAULT    ) return D3DPOOL_DEFAULT  ;
        if(usage == RESOURCE_USAGE_IMMUTABLE  ) return D3DPOOL_DEFAULT;
        if(usage == RESOURCE_USAGE_DYNAMIC    ) return D3DPOOL_MANAGED  ;
        if(usage == RESOURCE_USAGE_STAGING    ) return D3DPOOL_MANAGED  ;
        return D3DPOOL_MANAGED;
    }

    static DWORD  GetD3DUsage(eResourceAccessFlage flag , eResourceBindType bindType , eResourceUsage _usage)
    {
        DWORD usage = 0;
        if(bindType & BIND_AS_RENDER_TARGET ) usage |= D3DUSAGE_RENDERTARGET;
        return usage;
    }

};
END_NAMESPACE_XEVOL3D

#endif

