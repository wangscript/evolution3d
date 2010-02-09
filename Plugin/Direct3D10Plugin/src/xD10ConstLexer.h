#ifndef _XEVOL_DIRECT3D10_CONSTLEXER_H_
#define _XEVOL_DIRECT3D10_CONSTLEXER_H_
#include <RenderApi/xRenderApi.h>
#include <d3d10.h>
#include <D3DX10.h>
#include <D3DX10tex.h>
#include <string>
#include <vector>
#include <map>
BEGIN_NAMESPACE_XEVOL3D

struct xD10GIFormatInfo
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
class xD10ConstLexer
{
	std::vector<xD10GIFormatInfo> m_vForamts;
	MapsDXGIToIndex               m_dxindex;
	MapsXEvolToIndex              m_fmtIndex;
protected:
	void __addDXGIFormat(ePIXEL_FORMAT fmt , DXGI_FORMAT dxfmt , const wchar_t* descStr , int nCompont , int _byte , int cpRate=1);
public:
	static xD10ConstLexer* singleton();
    xD10GIFormatInfo*      GetPixelFormat( DXGI_FORMAT dxfmt );
	xD10GIFormatInfo*      GetPixelFormat( ePIXEL_FORMAT fmt );

	xD10ConstLexer();
	~xD10ConstLexer();
	
	static D3D10_FILTER               GetFilter(const wchar_t* _min_val , const wchar_t* _mag_val , const wchar_t* _mip_val ,  bool bCamp);
	static D3D10_TEXTURE_ADDRESS_MODE GetAdress(const wchar_t* _addressMode);
	static D3D10_COMPARISON_FUNC      GetCompareFunc(const wchar_t* _func);
	static int                        GetTextureSlotIdx(const wchar_t* _texName);
	static D3D10_BLEND                GetBlendFactor(const wchar_t* _blen);
	static D3D10_BLEND_OP             GetBlendOp(const wchar_t* _blen);
	static D3D10_FILL_MODE            GetFillMode(const wchar_t* _blen);
	static D3D10_CULL_MODE            GetCullMode(const wchar_t* _blen);
	static D3D10_STENCIL_OP           GetStencilOp(const wchar_t* _op);
	static D3DX10_IMAGE_FILE_FORMAT   GetDX10ImageFileFormat(const wchar_t* fileName);
};
END_NAMESPACE_XEVOL3D

#endif

