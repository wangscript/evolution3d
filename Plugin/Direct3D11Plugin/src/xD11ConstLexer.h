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
};
END_NAMESPACE_XEVOL3D

#endif

