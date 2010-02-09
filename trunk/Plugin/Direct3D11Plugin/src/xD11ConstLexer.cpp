#include "stdafx.h"
#include "xD11Sampler.h"
#include "xD11ConstLexer.h"
#include "fs/xFileSystem.h"
BEGIN_NAMESPACE_XEVOL3D

void xD11ConstLexer::__addDXGIFormat(ePIXEL_FORMAT fmt , DXGI_FORMAT dxfmt , const wchar_t* descStr , int nCompont , int _byte , int cpRate)
{
    xD11GIFormatInfo fmtInfo;
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

xD11GIFormatInfo* xD11ConstLexer::GetPixelFormat( DXGI_FORMAT dxfmt )
{
	MapsDXGIToIndex::iterator pos = m_dxindex.find(dxfmt);
	if(pos == m_dxindex.end() )
		return NULL;
	int idx = pos->second;
	return &m_vForamts[ idx ];
}

xD11GIFormatInfo* xD11ConstLexer::GetPixelFormat( ePIXEL_FORMAT fmt )
{
	MapsXEvolToIndex::iterator pos = m_fmtIndex.find(fmt);
	if(pos == m_fmtIndex.end() )
		return NULL;
	int idx = pos->second;
	return &m_vForamts[ idx ];
}

xD11ConstLexer::xD11ConstLexer()
{
	    //__addDXGIFormat(PIXELFORMAT_ ,  DXGI_FORMAT_R32G32B32A32_TYPELESS	= 1,
		__addDXGIFormat(PIXELFORMAT_R32G32B32A32F , DXGI_FORMAT_R32G32B32A32_FLOAT      , L"FORMAT_R32G32B32A32_FLOAT" , 4 , 4);
		__addDXGIFormat(PIXELFORMAT_R32G32B32A32U , DXGI_FORMAT_R32G32B32A32_UINT       , L"FORMAT_R32G32B32A32_UINT"  , 4 , 4);
		__addDXGIFormat(PIXELFORMAT_R32G32B32A32S , DXGI_FORMAT_R32G32B32A32_SINT       , L"FORMAT_R32G32B32A32_SINT"  , 4 , 4);
		//__addDXGIFormat(PIXELFORMAT_            , DXGI_FORMAT_R32G32B32_TYPELESS        	 
		__addDXGIFormat(PIXELFORMAT_R32G32B32F    , DXGI_FORMAT_R32G32B32_FLOAT	        , L"FORMAT_R32G32B32_FLOAT" , 3 , 4);
		__addDXGIFormat(PIXELFORMAT_R32G32B32U    , DXGI_FORMAT_R32G32B32_UINT	        , L"FORMAT_R32G32B32_UINT"  , 3 , 4);
		__addDXGIFormat(PIXELFORMAT_R32G32B32S    , DXGI_FORMAT_R32G32B32_SINT	        , L"FORMAT_R32G32B32_SINT"  , 3 , 4);
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R16G16B16A16_TYPELESS 
		__addDXGIFormat(PIXELFORMAT_R16G16B16A16F    , DXGI_FORMAT_R16G16B16A16_FLOAT	    , L"FORMAT_R16G16B16A16_FLOAT" , 4 , 2);
		__addDXGIFormat(PIXELFORMAT_R16G16B16A16U    , DXGI_FORMAT_R16G16B16A16_UNORM	    , L"FORMAT_R16G16B16A16_UNORM" , 4 , 2);
		__addDXGIFormat(PIXELFORMAT_R16G16B16A16UINT , DXGI_FORMAT_R16G16B16A16_UINT	    , L"FORMAT_R16G16B16A16_UINT"  , 4 , 2);
		__addDXGIFormat(PIXELFORMAT_R16G16B16A16S    , DXGI_FORMAT_R16G16B16A16_SNORM	    , L"FORMAT_R16G16B16A16_SNORM" , 4 , 2);
		__addDXGIFormat(PIXELFORMAT_R16G16B16A16SINT , DXGI_FORMAT_R16G16B16A16_SINT	    , L"FORMAT_R16G16B16A16_SINT"  , 4 , 2);
																					    
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R32G32_TYPELESS	= 15    ,
		__addDXGIFormat(PIXELFORMAT_R32G32F       , DXGI_FORMAT_R32G32_FLOAT	        , L"FORMAT_R32G32_FLOAT" , 2 , 4);
		__addDXGIFormat(PIXELFORMAT_R32G32U       , DXGI_FORMAT_R32G32_UINT	            , L"FORMAT_R32G32_UINT"  , 2 , 4);
		__addDXGIFormat(PIXELFORMAT_R32G32S       , DXGI_FORMAT_R32G32_SINT	            , L"FORMAT_R32G32_SINT"  , 2 , 4);
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R32G8X24_TYPELESS	 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_D32_FLOAT_S8X24_UINT	 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_X32_TYPELESS_G8X24_UINT 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R10G10B10A2_TYPELESS 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R10G10B10A2_UNORM 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R10G10B10A2_UINT 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R11G11B10_FLOAT 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8B8A8_TYPELESS 
		__addDXGIFormat(PIXELFORMAT_R8G8B8A8      ,DXGI_FORMAT_R8G8B8A8_UNORM	        , L"FORMAT_R8G8B8A8_UNORM"       , 4 , 1);
		__addDXGIFormat(PIXELFORMAT_R8G8B8A8      ,DXGI_FORMAT_R8G8B8A8_UNORM_SRGB      , L"FORMAT_R8G8B8A8_UNORM_SRGB"  , 4 , 1);
		__addDXGIFormat(PIXELFORMAT_R8G8B8A8UINT  ,DXGI_FORMAT_R8G8B8A8_UINT	        , L"FORMAT_R8G8B8A8_UINT"        , 4 , 1);
		__addDXGIFormat(PIXELFORMAT_R8G8B8A8S     ,DXGI_FORMAT_R8G8B8A8_SNORM	        , L"FORMAT_R8G8B8A8_SNORM"       , 4 , 1);
		__addDXGIFormat(PIXELFORMAT_R8G8B8A8SINT  ,DXGI_FORMAT_R8G8B8A8_SINT            , L"FORMAT_R8G8B8A8_SINT"        , 4 , 1);
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R16G16_TYPELESS	= 33,
		__addDXGIFormat(PIXELFORMAT_R16G16F       ,DXGI_FORMAT_R16G16_FLOAT             , L"FORMAT_R16G16_FLOAT"         , 2  , 2);
		__addDXGIFormat(PIXELFORMAT_R16G16U       ,DXGI_FORMAT_R16G16_UNORM				, L"FORMAT_R16G16_UNORM"         , 2  , 2);
		__addDXGIFormat(PIXELFORMAT_R16G16UINT    ,DXGI_FORMAT_R16G16_UINT              , L"FORMAT_R16G16_UINT"          , 2  , 2);
		__addDXGIFormat(PIXELFORMAT_R16G16S       ,DXGI_FORMAT_R16G16_SNORM				, L"FORMAT_R16G16_SNORM"         , 2  , 2);
		__addDXGIFormat(PIXELFORMAT_R16G16SINT    ,DXGI_FORMAT_R16G16_SINT              , L"FORMAT_R16G16_SINT"          , 2  , 2);
		__addDXGIFormat(PIXELFORMAT_DEPTH32       ,DXGI_FORMAT_D32_FLOAT                , L"FORMAT_D32_FLOAT"            , 1  , 4);
		__addDXGIFormat(PIXELFORMAT_R32F          ,DXGI_FORMAT_R32_FLOAT                , L"FORMAT_R32_FLOAT"            , 1  , 4);
		__addDXGIFormat(PIXELFORMAT_R32U          ,DXGI_FORMAT_R32_UINT	                , L"FORMAT_R32_UINT"             , 1  , 4);
		__addDXGIFormat(PIXELFORMAT_R32S          ,DXGI_FORMAT_R32_SINT	                , L"FORMAT_R32_SINT"             , 1  , 4);

        //__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS	= 21,
        __addDXGIFormat(PIXELFORMAT_DEPTH32S8UX24 ,DXGI_FORMAT_D32_FLOAT_S8X24_UINT     , L"FORMAT_D32_FLOAT_S8X24_UINT" , 2  , 4);
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R24G8_TYPELESS 
		__addDXGIFormat(PIXELFORMAT_DEPTH24       ,DXGI_FORMAT_D24_UNORM_S8_UINT	    , L"FORMAT_D24_UNORM_S8_UINT"         , 1 , 3);
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R24_UNORM_X8_TYPELESS 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_X24_TYPELESS_G8_UINT 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8_TYPELESS 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8_UNORM 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8_UINT 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8_SNORM 
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8_SINT 
		//__addDXGIFormat(PIXELFORMAT_R16F        ,DXGI_FORMAT_R16_TYPELESS 
		__addDXGIFormat(PIXELFORMAT_R16F          ,DXGI_FORMAT_R16_FLOAT	            , L"FORMAT_R16_FLOAT"   , 1 , 2);
		__addDXGIFormat(PIXELFORMAT_DEPTH16       ,DXGI_FORMAT_D16_UNORM	            , L"FORMAT_D16_UNORM"   , 1 , 2);
		__addDXGIFormat(PIXELFORMAT_R16U          ,DXGI_FORMAT_R16_UNORM	            , L"FORMAT_R16_UNORM"   , 1 , 2);
		__addDXGIFormat(PIXELFORMAT_R16U          ,DXGI_FORMAT_R16_UINT	                , L"FORMAT_R16_UINT"    , 1 , 2);
		__addDXGIFormat(PIXELFORMAT_R16S          ,DXGI_FORMAT_R16_SNORM	            , L"FORMAT_R16_SNORM"   , 1 , 2);
		__addDXGIFormat(PIXELFORMAT_R16S          ,DXGI_FORMAT_R16_SINT	                , L"FORMAT_R16_SINT"    , 1 , 2);
		//__addDXGIFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8_TYPELESS	= 60,
		__addDXGIFormat(PIXELFORMAT_LUMINANCE8    ,DXGI_FORMAT_R8_UNORM	                , L"FORMAT_R8_UNORM"    , 1 , 1);
		__addDXGIFormat(PIXELFORMAT_LUMINANCE8    ,DXGI_FORMAT_R8_UINT	                , L"FORMAT_R8_UINT"     , 1 , 1);
		__addDXGIFormat(PIXELFORMAT_LUMINANCE8    ,DXGI_FORMAT_R8_SNORM	                , L"FORMAT_R8_SNORM"    , 1 , 1);
		__addDXGIFormat(PIXELFORMAT_LUMINANCE8    ,DXGI_FORMAT_R8_SINT	                , L"FORMAT_R8_SINT"     , 1 , 1);
		__addDXGIFormat(PIXELFORMAT_ALPHA8        ,DXGI_FORMAT_A8_UNORM	                , L"FORMAT_A8_UNORM"    , 1 , 1);
		//__addDXGIFormat(PIXELFORMAT_ALPHA8      ,DXGI_FORMAT_R1_UNORM	 , L"FORMAT_R1_UNORM"    , 1 , 2);
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_R9G9B9E5_SHAREDEXP	= 67,
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_R8G8_B8G8_UNORM	= 68,
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_G8R8_G8B8_UNORM	= 69,
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC1_TYPELESS	= 70,
		__addDXGIFormat(PIXELFORMAT_DXT1         ,DXGI_FORMAT_BC1_UNORM                 , L"FORMAT_DX1_UNORM"   , 4 , 1 , 2);
		__addDXGIFormat(PIXELFORMAT_DXT1         ,DXGI_FORMAT_BC1_UNORM_SRGB            , L"FORMAT_DX1_SRGB"    , 4 , 1 , 2);
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC2_TYPELESS	= 73,
		__addDXGIFormat(PIXELFORMAT_DXT2         ,DXGI_FORMAT_BC2_UNORM	                , L"FORMAT_DX2_UNORM"   , 4 , 1 , 2);
		__addDXGIFormat(PIXELFORMAT_DXT2         ,DXGI_FORMAT_BC2_UNORM_SRGB	        , L"FORMAT_DX2_SRGB"    , 4 , 1 , 2);
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC3_TYPELESS	= 76,
		__addDXGIFormat(PIXELFORMAT_DXT3         ,DXGI_FORMAT_BC3_UNORM	                , L"FORMAT_DX3_UNORM"   , 4 , 1 , 2);
		__addDXGIFormat(PIXELFORMAT_DXT3         ,DXGI_FORMAT_BC3_UNORM_SRGB            , L"FORMAT_DX3_SRGB"    , 4 , 1 , 2);
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC4_TYPELESS	= 79,
		__addDXGIFormat(PIXELFORMAT_DXT4         ,DXGI_FORMAT_BC4_UNORM	                , L"FORMAT_DX4_UNORM"   , 4 , 1 , 4);
		__addDXGIFormat(PIXELFORMAT_DXT4         ,DXGI_FORMAT_BC4_SNORM	                , L"FORMAT_DX4_SNORM"   , 4 , 1 , 4);
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC5_TYPELESS	= 82,
		__addDXGIFormat(PIXELFORMAT_DXT5         ,DXGI_FORMAT_BC5_UNORM	                , L"FORMAT_DX5_UNORM"   , 4 , 1 , 4);
		__addDXGIFormat(PIXELFORMAT_DXT5         ,DXGI_FORMAT_BC5_SNORM	                , L"FORMAT_DX5_SNORM"   , 4 , 1 , 4);
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_B5G6R5_UNORM	= 85,
		//__addDXGIFormat(PIXELFORMAT_ ,DXGI_FORMAT_B5G5R5A1_UNORM	= 86,
		__addDXGIFormat(PIXELFORMAT_B8G8R8A8 ,DXGI_FORMAT_B8G8R8A8_UNORM	            , L"FORMAT_B8G8R8A8_UNORM"    , 4 , 1);
		__addDXGIFormat(PIXELFORMAT_B8G8R8X8 ,DXGI_FORMAT_B8G8R8X8_UNORM	            , L"FORMAT_B8G8R8X8_UNORM"     , 4 , 1);
}

xD11ConstLexer::~xD11ConstLexer()
{

}

xD11ConstLexer* xD11ConstLexer::singleton()
{
	static xD11ConstLexer g_s;
	return &g_s;
}

D3D11_FILL_MODE xD11ConstLexer::GetFillMode(const wchar_t* _fill)
{
	if(_fill == NULL) _fill = L"fill";
	std::wstring fillMode = _fill ;
	if(fillMode == L"line" || fillMode == L"wireframe") return D3D11_FILL_WIREFRAME	;
	if(fillMode == L"solid"|| fillMode == L"fill" )     return D3D11_FILL_SOLID	;
	 return D3D11_FILL_SOLID	;
}

D3D11_CULL_MODE xD11ConstLexer::GetCullMode(const wchar_t* _cull)
{	
	if(_cull == NULL) _cull = L"none";
    std::wstring cullMode = _cull ;
	if(cullMode == L"none")            return  D3D11_CULL_NONE;
	if(cullMode == L"front")            return  D3D11_CULL_FRONT;
	if(cullMode == L"back")            return  D3D11_CULL_BACK;
	return  D3D11_CULL_BACK;
}

D3D11_BLEND_OP  xD11ConstLexer::GetBlendOp(const wchar_t* _blenop)
{
	if(_blenop == NULL) _blenop = L"add";
	std::wstring blendop = _blenop ;
	if(blendop == L"add")            return  D3D11_BLEND_OP_ADD;
	if(blendop == L"subtract")       return  D3D11_BLEND_OP_SUBTRACT;
	if(blendop == L"rev_subtract")   return  D3D11_BLEND_OP_REV_SUBTRACT;
	if(blendop == L"min")            return  D3D11_BLEND_OP_MIN;
	if(blendop == L"max")            return  D3D11_BLEND_OP_MAX;
	return D3D11_BLEND_OP_ADD;
}

D3D11_BLEND  xD11ConstLexer::GetBlendFactor(const wchar_t* _blend)
{
	if(_blend == NULL) _blend = L"one";
	std::wstring blend = _blend ;
	if(blend == L"zero")            return  D3D11_BLEND_ZERO;
	if(blend == L"one")             return  D3D11_BLEND_ONE;
	if(blend == L"src")             return  D3D11_BLEND_SRC_COLOR;
	if(blend == L"inv_src")         return  D3D11_BLEND_INV_SRC_COLOR;
	if(blend == L"src_alpha")       return  D3D11_BLEND_SRC_ALPHA;
	if(blend == L"inv_src_alpha")   return 	D3D11_BLEND_INV_SRC_ALPHA;
	if(blend == L"dest_alpha")      return 	D3D11_BLEND_DEST_ALPHA;
	if(blend == L"inv_dest_alpha")  return 	D3D11_BLEND_INV_DEST_ALPHA;
	if(blend == L"dest")            return 	D3D11_BLEND_DEST_COLOR;
	if(blend == L"inv_dest")        return 	D3D11_BLEND_INV_DEST_COLOR;
	if(blend == L"src_alpha_sat")   return 	D3D11_BLEND_SRC_ALPHA_SAT;
	if(blend == L"factor")          return 	D3D11_BLEND_BLEND_FACTOR;
	if(blend == L"inv_factor")      return 	D3D11_BLEND_INV_BLEND_FACTOR;
	if(blend == L"src1")            return 	D3D11_BLEND_SRC1_COLOR;
	if(blend == L"inv_src1")        return 	D3D11_BLEND_INV_SRC1_COLOR;
	if(blend == L"src1_alpha")      return 	D3D11_BLEND_SRC1_ALPHA;
	if(blend == L"src1_alpha")      return 	D3D11_BLEND_INV_SRC1_ALPHA;
	return  D3D11_BLEND_ONE;
}

int xD11ConstLexer::GetTextureSlotIdx(const wchar_t* _texName)
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

D3D11_FILTER xD11ConstLexer::GetFilter(const wchar_t* _min_val , const wchar_t* _mag_val , const wchar_t* _mip_val ,  bool bCamp)
{
	if(_min_val == NULL) _min_val = L"linear";
	if(_mag_val == NULL) _mag_val = L"linear";
	if(_mip_val == NULL) _mip_val = L"linear";
	std::wstring min_val = _min_val ;
	std::wstring mag_val = _mag_val ;
	std::wstring mip_val = _mip_val ;
	D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	if(min_val == L"anisotropic" || min_val == L"aniso" || mag_val == L"anisotropic" || mag_val == L"aniso")
	{
		filter = D3D11_FILTER_ANISOTROPIC;
		if(bCamp)
			return D3D11_FILTER_COMPARISON_ANISOTROPIC ;
	}

	int iComp = bCamp ? 1: 0;
	int iMin  = (min_val == L"linear") ? 1: 0; 
	int iMag  = (mag_val == L"linear") ? 1: 0; 
	int iMip  = (mip_val == L"linear") ? 1: 0; 

	D3D11_FILTER a[2][2][2][2];
	a[0][0][0][0] = D3D11_FILTER_MIN_MAG_MIP_POINT;
	a[0][0][0][1] = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR ;
	a[0][0][1][0] = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
	a[0][0][1][1] = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
	a[0][1][0][0] = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	a[0][1][0][1] = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	a[0][1][1][0] = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	a[0][1][1][1] = D3D11_FILTER_MIN_MAG_MIP_LINEAR;


	a[1][0][0][0] = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	a[1][0][0][1] = D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
	a[1][0][1][0] = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
	a[1][0][1][1] = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
	a[1][1][0][0] = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
	a[1][1][0][1] = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	a[1][1][1][0] = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	a[1][1][1][1] = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;

	return a[iComp][iMin][iMag][iMip];
}


D3D11_TEXTURE_ADDRESS_MODE xD11ConstLexer::GetAdress(const wchar_t* _addressMode)
{
	if(_addressMode == NULL) _addressMode = L"clamp";
	std::wstring addressMode = _addressMode ;
	if(addressMode == L"repeat")     return D3D11_TEXTURE_ADDRESS_WRAP;
	if(addressMode == L"wrap")       return D3D11_TEXTURE_ADDRESS_WRAP;
	if(addressMode == L"mirror")     return D3D11_TEXTURE_ADDRESS_MIRROR ;
	if(addressMode == L"clamp")      return D3D11_TEXTURE_ADDRESS_CLAMP ;
	if(addressMode == L"border")     return D3D11_TEXTURE_ADDRESS_BORDER ;
	if(addressMode == L"mirroronce") return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE ;
	return D3D11_TEXTURE_ADDRESS_CLAMP;
}

D3D11_COMPARISON_FUNC xD11ConstLexer::GetCompareFunc(const wchar_t* _func)
{
	if(_func == NULL) _func = L"never";
	std::wstring func = _func ;
	if(func == L"never")   return D3D11_COMPARISON_NEVER;
	if(func == L"less")    return D3D11_COMPARISON_LESS;
	if(func == L"equal")   return D3D11_COMPARISON_EQUAL;
	if(func == L"lequal")  return D3D11_COMPARISON_LESS_EQUAL;
	if(func == L"greator") return D3D11_COMPARISON_GREATER;
	if(func == L"nequale") return D3D11_COMPARISON_NOT_EQUAL;
	if(func == L"gequale") return D3D11_COMPARISON_GREATER_EQUAL;
	if(func == L"always")  return D3D11_COMPARISON_ALWAYS;
	return D3D11_COMPARISON_NEVER;
}

D3DX11_IMAGE_FILE_FORMAT  xD11ConstLexer::GetDX11ImageFileFormat(const wchar_t* fileName)
{
	std::ds_wstring ext = xFileSystem::singleton()->getFileExtName(fileName);
    if(ext == L"bmp") return   D3DX11_IFF_BMP;
	if(ext == L"jpg") return   D3DX11_IFF_JPG;  if(ext == L"jpeg") return   D3DX11_IFF_JPG;
	if(ext == L"png") return   D3DX11_IFF_PNG;
	if(ext == L"dds") return   D3DX11_IFF_DDS;
	if(ext == L"tif") return   D3DX11_IFF_TIFF; if(ext == L"tiff") return   D3DX11_IFF_TIFF;
	if(ext == L"gif") return   D3DX11_IFF_GIF;
	return (D3DX11_IMAGE_FILE_FORMAT)-1;
}

D3D11_STENCIL_OP xD11ConstLexer::GetStencilOp(const wchar_t* _op)
{
	if(_op == NULL) _op = L"never";
	std::wstring func = _op ;
	if(func == L"keep")       return D3D11_STENCIL_OP_KEEP;
	if(func == L"zero")       return D3D11_STENCIL_OP_ZERO;
	if(func == L"replace")    return D3D11_STENCIL_OP_REPLACE;
	if(func == L"incr_sat")   return D3D11_STENCIL_OP_INCR_SAT;
	if(func == L"decr_sat")   return D3D11_STENCIL_OP_DECR_SAT;
	if(func == L"invert")     return D3D11_STENCIL_OP_INVERT;
	if(func == L"increase")   return D3D11_STENCIL_OP_INCR;
	if(func == L"decrease")   return D3D11_STENCIL_OP_DECR;

	if(func == L"incr")       return D3D11_STENCIL_OP_INCR;
	if(func == L"decr")       return D3D11_STENCIL_OP_DECR;
	return D3D11_STENCIL_OP_KEEP;
}


END_NAMESPACE_XEVOL3D
