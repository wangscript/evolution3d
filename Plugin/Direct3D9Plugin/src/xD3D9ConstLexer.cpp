#include "stdafx.h"
#include "xD3D9ConstLexer.h"
#include "fs/xFileSystem.h"
BEGIN_NAMESPACE_XEVOL3D

void xD3D9ConstLexer::__addDXGIFormat(ePIXEL_FORMAT fmt , D3DFORMAT dxfmt , const wchar_t* descStr , int nCompont , int _byte , int cpRate)
{
    xD3D9GIFormatInfo fmtInfo;
	fmtInfo.m_compont = nCompont;
	fmtInfo.m_desc = descStr;
	fmtInfo.m_dxfmt = dxfmt;
	fmtInfo.m_fmt   = fmt;
	fmtInfo.m_bytePerComponent  = _byte;
	fmtInfo.m_CompressRate = cpRate;
	m_vForamts.push_back(fmtInfo);
	int nIdx = (int)m_vForamts.size() - 1;
	m_dxindex.insert( MapsDXGIToIndex::value_type(dxfmt , nIdx) );
	m_fmtIndex.insert(MapsXEvolToIndex::value_type(fmt  , nIdx) );
	return ;
}

xD3D9GIFormatInfo* xD3D9ConstLexer::GetPixelFormat( D3DFORMAT dxfmt )
{
	MapsDXGIToIndex::iterator pos = m_dxindex.find(dxfmt);
	if(pos == m_dxindex.end() )
		return NULL;
	int idx = pos->second;
	return &m_vForamts[ idx ];
}

xD3D9GIFormatInfo* xD3D9ConstLexer::GetPixelFormat( ePIXEL_FORMAT fmt )
{
	MapsXEvolToIndex::iterator pos = m_fmtIndex.find(fmt);
	if(pos == m_fmtIndex.end() )
		return NULL;
	int idx = pos->second;
	return &m_vForamts[ idx ];
}

xD3D9ConstLexer::xD3D9ConstLexer()
{
	    //__addDXGIFormat(PIXELFORMAT_ ,  DXGI_FORMAT_R32G32B32A32_TYPELESS	= 1,
		__addDXGIFormat(PIXELFORMAT_R32G32B32A32F , D3DFMT_A32B32G32R32F      , L"FORMAT_R32G32B32A32_FLOAT" , 4 , 4);
		//__addDXGIFormat(PIXELFORMAT_R32G32B32A32U , DXGI_FORMAT_R32G32B32A32_UINT       , L"FORMAT_R32G32B32A32_UINT"  , 4 , 4);
		//__addDXGIFormat(PIXELFORMAT_R32G32B32A32S , DXGI_FORMAT_R32G32B32A32_SINT       , L"FORMAT_R32G32B32A32_SINT"  , 4 , 4);
		//__addDXGIFormat(PIXELFORMAT_            , DXGI_FORMAT_R32G32B32_TYPELESS        	 
		//__addDXGIFormat(PIXELFORMAT_R32G32B32F    , DXGI_FORMAT_R32G32B32_FLOAT	        , L"FORMAT_R32G32B32_FLOAT" , 3 , 4);
		//__addDXGIFormat(PIXELFORMAT_R32G32B32U    , DXGI_FORMAT_R32G32B32_UINT	        , L"FORMAT_R32G32B32_UINT"  , 3 , 4);
		//__addDXGIFormat(PIXELFORMAT_R32G32B32S    , DXGI_FORMAT_R32G32B32_SINT	        , L"FORMAT_R32G32B32_SINT"  , 3 , 4);
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R16G16B16A16_TYPELESS 
		__addDXGIFormat(PIXELFORMAT_R16G16B16A16F    , D3DFMT_A16B16G16R16F	    , L"FORMAT_R16G16B16A16_FLOAT" , 4 , 2);
		//__addDXGIFormat(PIXELFORMAT_R16G16B16A16U    , D3DFMT_A16B16G16R16	    , L"FORMAT_R16G16B16A16_UNORM" , 4 , 2);
		__addDXGIFormat(PIXELFORMAT_R16G16B16A16UINT , D3DFMT_A16B16G16R16	    , L"FORMAT_R16G16B16A16_UINT"  , 4 , 2);
		//__addDXGIFormat(PIXELFORMAT_R16G16B16A16S    , D3DFMT_A16B16G16R16	    , L"FORMAT_R16G16B16A16_SNORM" , 4 , 2);
		//__addDXGIFormat(PIXELFORMAT_R16G16B16A16SINT , D3DFMT_A16B16G16R16	    , L"FORMAT_R16G16B16A16_SINT"  , 4 , 2);
																					    
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R32G32_TYPELESS	= 15    ,
		__addDXGIFormat(PIXELFORMAT_R32G32F       , D3DFMT_G32R32F           	        , L"FORMAT_R32G32_FLOAT" , 2 , 4);
		//__addDXGIFormat(PIXELFORMAT_R32G32U       , DXGI_FORMAT_R32G32_UINT	            , L"FORMAT_R32G32_UINT"  , 2 , 4);
		//__addDXGIFormat(PIXELFORMAT_R32G32S       , DXGI_FORMAT_R32G32_SINT	            , L"FORMAT_R32G32_SINT"  , 2 , 4);
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R32G8X24_TYPELESS	 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_D32_FLOAT_S8X24_UINT	 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_X32_TYPELESS_G8X24_UINT 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R10G10B10A2_TYPELESS 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R10G10B10A2_UNORM 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R10G10B10A2_UINT 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R11G11B10_FLOAT 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8B8A8_TYPELESS 
		__addDXGIFormat(PIXELFORMAT_R8G8B8A8      ,D3DFMT_A8R8G8B8           	        , L"FORMAT_R8G8B8A8_UNORM"       , 4 , 1);
		__addDXGIFormat(PIXELFORMAT_R8G8B8A8      ,D3DFMT_A8R8G8B8                      , L"FORMAT_R8G8B8A8_UNORM_SRGB"  , 4 , 1);
		__addDXGIFormat(PIXELFORMAT_R8G8B8A8UINT  ,D3DFMT_A8R8G8B8	                    , L"FORMAT_R8G8B8A8_UINT"        , 4 , 1);
		__addDXGIFormat(PIXELFORMAT_R8G8B8A8S     ,D3DFMT_A8R8G8B8	                    , L"FORMAT_R8G8B8A8_SNORM"       , 4 , 1);
		__addDXGIFormat(PIXELFORMAT_R8G8B8A8SINT  ,D3DFMT_A8R8G8B8                      , L"FORMAT_R8G8B8A8_SINT"        , 4 , 1);

		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R16G16_TYPELESS	= 33,
		__addDXGIFormat(PIXELFORMAT_R16G16F       ,D3DFMT_G16R16F             , L"FORMAT_R16G16_FLOAT"         , 2  , 2);
		__addDXGIFormat(PIXELFORMAT_R16G16U       ,D3DFMT_G16R16			  , L"FORMAT_R16G16_UNORM"         , 2  , 2);
		__addDXGIFormat(PIXELFORMAT_R16G16UINT    ,D3DFMT_G16R16              , L"FORMAT_R16G16_UINT"          , 2  , 2);
		__addDXGIFormat(PIXELFORMAT_R16G16S       ,D3DFMT_G16R16			  , L"FORMAT_R16G16_SNORM"         , 2  , 2);
		__addDXGIFormat(PIXELFORMAT_R16G16SINT    ,D3DFMT_G16R16              , L"FORMAT_R16G16_SINT"          , 2  , 2);
		__addDXGIFormat(PIXELFORMAT_DEPTH32       ,D3DFMT_D32                 , L"FORMAT_D32_FLOAT"            , 1  , 4);
		__addDXGIFormat(PIXELFORMAT_R32F          ,D3DFMT_R32F                          , L"FORMAT_R32_FLOAT"            , 1  , 4);
		//__addDXGIFormat(PIXELFORMAT_R32U          ,DXGI_FORMAT_R32_UINT	                , L"FORMAT_R32_UINT"             , 1  , 4);
		//__addDXGIFormat(PIXELFORMAT_R32S          ,DXGI_FORMAT_R32_SINT	                , L"FORMAT_R32_SINT"             , 1  , 4);

        //__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS	= 21,
        //__addDXGIFormat(PIXELFORMAT_DEPTH32S8UX24 ,D3DFMT_D     , L"FORMAT_D32_FLOAT_S8X24_UINT" , 2  , 4);
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R24G8_TYPELESS 
		__addDXGIFormat(PIXELFORMAT_DEPTH24       ,D3DFMT_D24S8 	    , L"FORMAT_D24_UNORM_S8_UINT"         , 1 , 3);
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R24_UNORM_X8_TYPELESS 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_X24_TYPELESS_G8_UINT 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8_TYPELESS 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8_UNORM 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8_UINT 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8_SNORM 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8_SINT 
		//__addDXGIFormat(PIXELFORMAT_R16F        ,DXGI_FORMAT_R16_TYPELESS 
        __addDXGIFormat(PIXELFORMAT_R16F          ,D3DFMT_R16F	            , L"FORMAT_R16_FLOAT"   , 1 , 2);
        __addDXGIFormat(PIXELFORMAT_DEPTH16       ,D3DFMT_D16            , L"FORMAT_D16_UNORM"   , 1 , 2);
        //__addDXGIFormat(PIXELFORMAT_R16U          ,D3DFMT_R16            , L"FORMAT_R16_UNORM"   , 1 , 2);
        //__addDXGIFormat(PIXELFORMAT_R16U          ,D3DFMT_R16                , L"FORMAT_R16_UINT"    , 1 , 2);
        //__addDXGIFormat(PIXELFORMAT_R16S          ,D3DFMT_R16            , L"FORMAT_R16_SNORM"   , 1 , 2);
        //__addDXGIFormat(PIXELFORMAT_R16S          ,D3DFMT_R16                , L"FORMAT_R16_SINT"    , 1 , 2);
		//__addDXGIFormat(PIXELFORMAT_            ,D3DFMT_R8_TYPELESS	= 60,
		__addDXGIFormat(PIXELFORMAT_LUMINANCE8    ,D3DFMT_L8	                , L"FORMAT_R8_UNORM"    , 1 , 1);
		__addDXGIFormat(PIXELFORMAT_LUMINANCE8    ,D3DFMT_L8	                , L"FORMAT_R8_UINT"     , 1 , 1);
		__addDXGIFormat(PIXELFORMAT_LUMINANCE8    ,D3DFMT_L8	                , L"FORMAT_R8_SNORM"    , 1 , 1);
		__addDXGIFormat(PIXELFORMAT_LUMINANCE8    ,D3DFMT_L8	                , L"FORMAT_R8_SINT"     , 1 , 1);
		__addDXGIFormat(PIXELFORMAT_ALPHA8        ,D3DFMT_A8	                , L"FORMAT_A8_UNORM"    , 1 , 1);
		//__addDXGIFormat(PIXELFORMAT_ALPHA8      ,DXGI_FORMAT_R1_UNORM	 , L"FORMAT_R1_UNORM"    , 1 , 2);
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_R9G9B9E5_SHAREDEXP	= 67,
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_R8G8_B8G8_UNORM	= 68,
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_G8R8_G8B8_UNORM	= 69,
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC1_TYPELESS	= 70,
		__addDXGIFormat(PIXELFORMAT_DXT1         ,D3DFMT_DXT1                 , L"FORMAT_DX1_UNORM"   , 4 , 1 , 2);
		__addDXGIFormat(PIXELFORMAT_DXT1         ,D3DFMT_DXT1            , L"FORMAT_DX1_SRGB"    , 4 , 1 , 2);
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC2_TYPELESS	= 73,
		__addDXGIFormat(PIXELFORMAT_DXT2         ,D3DFMT_DXT2	                , L"FORMAT_DX2_UNORM"   , 4 , 1 , 2);
		__addDXGIFormat(PIXELFORMAT_DXT2         ,D3DFMT_DXT2	        , L"FORMAT_DX2_SRGB"    , 4 , 1 , 2);
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC3_TYPELESS	= 76,
		__addDXGIFormat(PIXELFORMAT_DXT3         ,D3DFMT_DXT3	                , L"FORMAT_DX3_UNORM"   , 4 , 1 , 2);
		__addDXGIFormat(PIXELFORMAT_DXT3         ,D3DFMT_DXT3            , L"FORMAT_DX3_SRGB"    , 4 , 1 , 2);
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC4_TYPELESS	= 79,
		__addDXGIFormat(PIXELFORMAT_DXT4         ,D3DFMT_DXT4	                , L"FORMAT_DX4_UNORM"   , 4 , 1 , 4);
		__addDXGIFormat(PIXELFORMAT_DXT4         ,D3DFMT_DXT4	                , L"FORMAT_DX4_SNORM"   , 4 , 1 , 4);
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC5_TYPELESS	= 82,
		__addDXGIFormat(PIXELFORMAT_DXT5         ,D3DFMT_DXT4	                , L"FORMAT_DX5_UNORM"   , 4 , 1 , 4);
		__addDXGIFormat(PIXELFORMAT_DXT5         ,D3DFMT_DXT4	                , L"FORMAT_DX5_SNORM"   , 4 , 1 , 4);
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_B5G6R5_UNORM	= 85,
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_B5G5R5A1_UNORM	= 86,
		__addDXGIFormat(PIXELFORMAT_B8G8R8A8 ,D3DFMT_A8B8G8R8	            , L"FORMAT_B8G8R8A8_UNORM"    , 4 , 1);
		__addDXGIFormat(PIXELFORMAT_B8G8R8X8 ,D3DFMT_X8B8G8R8	            , L"FORMAT_B8G8R8X8_UNORM"     , 4 , 1);
}

xD3D9ConstLexer::~xD3D9ConstLexer()
{

}

xD3D9ConstLexer* xD3D9ConstLexer::singleton()
{
	static xD3D9ConstLexer g_s;
	return &g_s;
}

D3DFILLMODE xD3D9ConstLexer::GetFillMode(const wchar_t* _fill)
{
	if(_fill == NULL) _fill = L"fill";
	std::wstring fillMode = _fill ;
	if(fillMode == L"line" || fillMode == L"wireframe") return D3DFILL_WIREFRAME	;
	if(fillMode == L"solid"|| fillMode == L"fill" )     return D3DFILL_SOLID	;
	 return D3DFILL_SOLID	;
}

D3DCULL xD3D9ConstLexer::GetCullMode(const wchar_t* _cull)
{	
	if(_cull == NULL) _cull = L"none";
    std::wstring cullMode = _cull ;
	if(cullMode == L"none")            return  D3DCULL_NONE;
	if(cullMode == L"front")           return  D3DCULL_CCW;
	if(cullMode == L"back")            return  D3DCULL_CW;
    if(cullMode == L"ccw")             return  D3DCULL_CCW;
    if(cullMode == L"cw")              return  D3DCULL_CW;
	return  D3DCULL_NONE;
}

D3DBLENDOP  xD3D9ConstLexer::GetBlendOp(const wchar_t* _blenop)
{
	if(_blenop == NULL) _blenop = L"add";
	std::wstring blendop = _blenop ;
	if(blendop == L"add")            return  D3DBLENDOP_ADD;
	if(blendop == L"subtract")       return  D3DBLENDOP_SUBTRACT;
	if(blendop == L"rev_subtract")   return  D3DBLENDOP_REVSUBTRACT;
	if(blendop == L"min")            return  D3DBLENDOP_MIN;
	if(blendop == L"max")            return  D3DBLENDOP_MAX;
	return D3DBLENDOP_ADD;
}

D3DBLEND  xD3D9ConstLexer::GetBlendFactor(const wchar_t* _blend)
{
	if(_blend == NULL) _blend = L"one";
	std::wstring blend = _blend ;
	if(blend == L"zero")            return  D3DBLEND_ZERO;
	if(blend == L"one")             return  D3DBLEND_ONE;
	if(blend == L"src")             return  D3DBLEND_SRCCOLOR;
	if(blend == L"inv_src")         return  D3DBLEND_INVSRCCOLOR;
	if(blend == L"src_alpha")       return  D3DBLEND_SRCALPHA;
	if(blend == L"inv_src_alpha")   return 	D3DBLEND_INVSRCALPHA;
	if(blend == L"dest_alpha")      return 	D3DBLEND_DESTALPHA;
	if(blend == L"inv_dest_alpha")  return 	D3DBLEND_INVDESTALPHA;
	if(blend == L"dest")            return 	D3DBLEND_DESTCOLOR;
	if(blend == L"inv_dest")        return 	D3DBLEND_INVDESTCOLOR;
	if(blend == L"src_alpha_sat")   return 	D3DBLEND_SRCALPHASAT;
	if(blend == L"factor")          return 	D3DBLEND_BLENDFACTOR;
	if(blend == L"inv_factor")      return 	D3DBLEND_INVBLENDFACTOR;
	if(blend == L"src1")            return 	D3DBLEND_SRCCOLOR;
	if(blend == L"inv_src1")        return 	D3DBLEND_INVSRCCOLOR;
	if(blend == L"src1_alpha")      return 	D3DBLEND_SRCALPHA;
	if(blend == L"src1_alpha")      return 	D3DBLEND_INVSRCALPHA;
	return  D3DBLEND_ONE;
}

int xD3D9ConstLexer::GetTextureSlotIdx(const wchar_t* _texName)
{
	std::wstring  texName = _texName;
	if(texName == L"Diffuse" || texName == L"diffuse" )
		return Texture_Diffuse;
	if(texName == L"Mask"    || texName == L"mask" )
		return Texture_Mask;

	if(texName == L"TextureStage0" || texName == L"textureStage0" || texName == L"Texture0" || texName == L"texture0"  )
		return Texture_Stage0;																		   
																												   
	if(texName == L"TextureStage1" || texName == L"textureStage1" || texName == L"Texture1" || texName == L"texture1"  )
		return Texture_Stage1;																		   
																												   
	if(texName == L"TextureStage2" || texName == L"textureStage2" || texName == L"Texture2" || texName == L"texture2"  )
		return Texture_Stage2;																		   
																												   
	if(texName == L"TextureStage3" || texName == L"textureStage3" || texName == L"Texture3" || texName == L"texture3"  )
		return Texture_Stage3;																		   
																												   
	if(texName == L"TextureStage4" || texName == L"textureStage4" || texName == L"Texture4" || texName == L"texture4"  )
		return Texture_Stage4;																		   
																												   
	if(texName == L"TextureStage5" || texName == L"textureStage5" || texName == L"Texture5" || texName == L"texture5"  )
		return Texture_Stage5;																		   
																												   
	if(texName == L"TextureStage6" || texName == L"textureStage6" || texName == L"Texture6" || texName == L"texture6"  )
		return Texture_Stage6;																		   
																												   
	if(texName == L"TextureStage7" || texName == L"textureStage7" || texName == L"Texture7" || texName == L"texture7"  )
		return Texture_Stage7;

	return -1;
}

D3DTEXTUREFILTERTYPE xD3D9ConstLexer::GetFilter(const wchar_t* _val)
{
    std::wstring strFilterName = _val;
    if(strFilterName == L"point")        return D3DTEXF_POINT;
    if(strFilterName == L"linear")       return D3DTEXF_LINEAR ;
    if(strFilterName == L"anisotropic")  return D3DTEXF_ANISOTROPIC ;

    if(strFilterName == L"nearest")      return D3DTEXF_POINT;
    if(strFilterName == L"linear")       return D3DTEXF_LINEAR ;
    if(strFilterName == L"aniso")        return D3DTEXF_ANISOTROPIC ;

    return D3DTEXF_LINEAR;
}


D3DTEXTUREADDRESS xD3D9ConstLexer::GetAdress(const wchar_t* _addressMode)
{
	if(_addressMode == NULL) _addressMode = L"clamp";
	std::wstring addressMode = _addressMode ;
    if(addressMode == L"repeat")     return D3DTADDRESS_WRAP;
	if(addressMode == L"wrap")       return D3DTADDRESS_WRAP;
	if(addressMode == L"mirror")     return D3DTADDRESS_MIRROR ;
	if(addressMode == L"clamp")      return D3DTADDRESS_CLAMP ;
	if(addressMode == L"border")     return D3DTADDRESS_BORDER ;
	if(addressMode == L"mirroronce") return D3DTADDRESS_MIRRORONCE ;
	return D3DTADDRESS_CLAMP;
}

D3DCMPFUNC xD3D9ConstLexer::GetCompareFunc(const wchar_t* _func)
{
	if(_func == NULL) _func = L"always";
	std::wstring func = _func ;
    if(func == L"never")   return D3DCMP_NEVER;
	if(func == L"less")    return D3DCMP_LESS;
	if(func == L"equal")   return D3DCMP_EQUAL;
	if(func == L"lequal")  return D3DCMP_LESSEQUAL;
	if(func == L"greator") return D3DCMP_GREATER;
	if(func == L"nequale") return D3DCMP_NOTEQUAL;
	if(func == L"gequale") return D3DCMP_GREATEREQUAL;
	if(func == L"always")  return D3DCMP_ALWAYS;
	return D3DCMP_ALWAYS;
}

D3DXIMAGE_FILEFORMAT  xD3D9ConstLexer::GetDXImageFileFormat(const wchar_t* fileName)
{
	std::ds_wstring ext = xFileSystem::singleton()->getFileExtName(fileName);
    if(ext == L"bmp") return   D3DXIFF_BMP;
	if(ext == L"jpg") return   D3DXIFF_JPG;  if(ext == L"jpeg") return   D3DXIFF_JPG;
	if(ext == L"png") return   D3DXIFF_PNG;
	if(ext == L"dds") return   D3DXIFF_DDS;

	return (D3DXIMAGE_FILEFORMAT)-1;
}

D3DSTENCILOP xD3D9ConstLexer::GetStencilOp(const wchar_t* _op)
{
	if(_op == NULL) _op = L"never";
    std::wstring func = _op ; 
	if(func == L"keep")       return D3DSTENCILOP_KEEP;
	if(func == L"zero")       return D3DSTENCILOP_ZERO;
	if(func == L"replace")    return D3DSTENCILOP_REPLACE;
	if(func == L"incr_sat")   return D3DSTENCILOP_INCRSAT;
	if(func == L"decr_sat")   return D3DSTENCILOP_DECRSAT;
	if(func == L"invert")     return D3DSTENCILOP_INVERT;
	if(func == L"increase")   return D3DSTENCILOP_INCR;
	if(func == L"decrease")   return D3DSTENCILOP_DECR;

	if(func == L"incr")       return D3DSTENCILOP_INCR;
	if(func == L"decr")       return D3DSTENCILOP_DECR;
	return D3DSTENCILOP_KEEP;
}


END_NAMESPACE_XEVOL3D
