#include "xGL2Sampler.h"
#include "xGL2ConstLexer.h"
BEGIN_NAMESPACE_XEVOL3D
void xGL2ConstLexer::__addFormat(ePIXEL_FORMAT fmt , GLenum GLDataType,GLenum GLDataFormat,int nComponent,int InternalFmt,int bytePerComponent  , const wchar_t* desc, GLenum compressedFmt)
{
    xGL2GIFormatInfo fmtInfo;
    fmtInfo.m_bytePerComponent = bytePerComponent;
    fmtInfo.m_GLDataFormat     = GLDataFormat;
    fmtInfo.m_GLDataType       = GLDataType;
    fmtInfo.m_InternalFmt      = InternalFmt;
    fmtInfo.m_nComponent       = nComponent ;
    fmtInfo.m_compressInFmt    = compressedFmt;
    fmtInfo.m_fmt   = fmt;
    fmtInfo.m_desc = desc;
	m_vForamts.push_back(fmtInfo);
	int nIdx = (int)m_vForamts.size() - 1;
	m_dxindex.insert( MapsDXGIToIndex::value_type(InternalFmt , nIdx) );
	m_fmtIndex.insert(MapsXEvolToIndex::value_type(fmt  , nIdx) );
	return ;
}

xGL2GIFormatInfo* xGL2ConstLexer::GetPixelFormat(GLenum innerfmt )
{
	MapsDXGIToIndex::iterator pos = m_dxindex.find(innerfmt);
	if(pos == m_dxindex.end() )
		return NULL;
	int idx = pos->second;
	return &m_vForamts[ idx ];
}

xGL2GIFormatInfo* xGL2ConstLexer::GetPixelFormat( ePIXEL_FORMAT fmt )
{
	MapsXEvolToIndex::iterator pos = m_fmtIndex.find(fmt);
	if(pos == m_fmtIndex.end() )
		return NULL;
	int idx = pos->second;
	return &m_vForamts[ idx ];
}

xGL2ConstLexer::xGL2ConstLexer()
{
	    //__addFormat(PIXELFORMAT_ ,  DXGI_FORMAT_R32G32B32A32_TYPELESS	= 1,
		__addFormat(PIXELFORMAT_R32G32B32A32F , GL_FLOAT             , GL_RGBA , 4 , GL_RGBA32F  , 4 , L"FORMAT_R32G32B32A32_FLOAT");
		__addFormat(PIXELFORMAT_R32G32B32A32U , GL_UNSIGNED_INT      , GL_RGBA , 4 , GL_RGBA32UI , 4 , L"FORMAT_R32G32B32A32_UINT" );
		__addFormat(PIXELFORMAT_R32G32B32A32S , GL_INT               , GL_RGBA , 4 , GL_RGBA32I  , 4 , L"FORMAT_R32G32B32A32_SINT" );
		//__addFormat(PIXELFORMAT_            , DXGI_FORMAT_R32G32B32_TYPELESS        	 
		__addFormat(PIXELFORMAT_R32G32B32F    , GL_FLOAT             , GL_RGB  , 3,  GL_RGBA32F  , 4 , L"FORMAT_R32G32B32_FLOAT" );
		__addFormat(PIXELFORMAT_R32G32B32U    , GL_UNSIGNED_INT      , GL_RGB  , 3,  GL_RGBA32UI , 4 , L"FORMAT_R32G32B32_UINT"  );
		__addFormat(PIXELFORMAT_R32G32B32S    , GL_INT               , GL_RGB  , 3,  GL_RGBA32I  , 4 , L"FORMAT_R32G32B32_SINT"  );
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R16G16B16A16_TYPELESS 
		__addFormat(PIXELFORMAT_R16G16B16A16F    , GL_HALF_FLOAT     , GL_RGBA , 4 , GL_RGBA16F  , 2 , L"FORMAT_R16G16B16A16_FLOAT" );
		__addFormat(PIXELFORMAT_R16G16B16A16U    , GL_UNSIGNED_SHORT , GL_RGBA , 4 , GL_RGBA32UI , 2 , L"FORMAT_R16G16B16A16_UNORM" );
		__addFormat(PIXELFORMAT_R16G16B16A16UINT , GL_UNSIGNED_SHORT , GL_RGBA , 4 , GL_RGBA32UI , 2 , L"FORMAT_R16G16B16A16_UINT"  );
		__addFormat(PIXELFORMAT_R16G16B16A16S    , GL_SHORT          , GL_RGBA , 4 , GL_RGBA32I  , 2 , L"FORMAT_R16G16B16A16_SNORM" );
		__addFormat(PIXELFORMAT_R16G16B16A16SINT , GL_SHORT          , GL_RGBA , 4 , GL_RGBA32I  , 2 , L"FORMAT_R16G16B16A16_SINT"  );
																					    
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R32G32_TYPELESS	= 15    ,
		__addFormat(PIXELFORMAT_R32G32F         , GL_FLOAT          , GL_LUMINANCE_ALPHA   , 2 , GL_RG32F	  , 4  , L"FORMAT_R32G32_FLOAT");
		__addFormat(PIXELFORMAT_R32G32U         , GL_UNSIGNED_INT   , GL_LUMINANCE_ALPHA   , 2 , GL_RG32UI    , 4  , L"FORMAT_R32G32_UINT" );
		__addFormat(PIXELFORMAT_R32G32S         , GL_INT            , GL_LUMINANCE_ALPHA   , 2 , GL_RG32I	  , 4  , L"FORMAT_R32G32_SINT" );
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R32G8X24_TYPELESS	 
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_D32_FLOAT_S8X24_UINT	 
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS 
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_X32_TYPELESS_G8X24_UINT 
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R10G10B10A2_TYPELESS 
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R10G10B10A2_UNORM 
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R10G10B10A2_UINT 
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R11G11B10_FLOAT 
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8B8A8_TYPELESS 
		__addFormat(PIXELFORMAT_R8G8B8A8      , GL_UNSIGNED_BYTE , GL_BGRA_EXT , 4 , GL_RGBA8	,  1 , L"FORMAT_R8G8B8A8_UNORM" , GL_COMPRESSED_RGBA);
		__addFormat(PIXELFORMAT_R8G8B8A8UINT  , GL_UNSIGNED_BYTE , GL_BGRA_EXT , 4 , GL_RGBA8	,  1 , L"FORMAT_R8G8B8A8_UINT"  , GL_COMPRESSED_RGBA);
		__addFormat(PIXELFORMAT_R8G8B8A8S     , GL_BYTE          , GL_BGRA_EXT , 4 , GL_RGBA8	,  1 , L"FORMAT_R8G8B8A8_SNORM" , GL_COMPRESSED_RGBA);
		__addFormat(PIXELFORMAT_R8G8B8A8SINT  , GL_BYTE          , GL_BGRA_EXT , 4 , GL_RGBA8   ,  1 , L"FORMAT_R8G8B8A8_SINT"  , GL_COMPRESSED_RGBA);
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R16G16_TYPELESS	= 33,
		__addFormat(PIXELFORMAT_R16G16F       ,GL_HALF_FLOAT     , GL_LUMINANCE_ALPHA , 2 ,  GL_RG16F  , 2  , L"FORMAT_R16G16_FLOAT" );
		__addFormat(PIXELFORMAT_R16G16U       ,GL_UNSIGNED_SHORT , GL_LUMINANCE_ALPHA , 2 ,  GL_RG16UI , 2  , L"FORMAT_R16G16_UNORM" );
		__addFormat(PIXELFORMAT_R16G16UINT    ,GL_UNSIGNED_SHORT , GL_LUMINANCE_ALPHA , 2 ,  GL_RG16UI , 2  , L"FORMAT_R16G16_UINT"  );
		__addFormat(PIXELFORMAT_R16G16S       ,GL_SHORT          , GL_LUMINANCE_ALPHA , 2 ,  GL_RG16I  , 2 	, L"FORMAT_R16G16_SNORM" );
		__addFormat(PIXELFORMAT_R16G16SINT    ,GL_SHORT          , GL_LUMINANCE_ALPHA , 2 ,  GL_RG16I  , 2  , L"FORMAT_R16G16_SINT"  );

		__addFormat(PIXELFORMAT_R32F          ,GL_FLOAT          , GL_LUMINANCE       , 1 , GL_R32F               , 4  , L"FORMAT_R32_FLOAT");
		__addFormat(PIXELFORMAT_R32U          ,GL_UNSIGNED_INT   , GL_LUMINANCE       , 1 , GL_R32UI              , 4  , L"FORMAT_R32_UINT" );
		__addFormat(PIXELFORMAT_R32S          ,GL_INT            , GL_LUMINANCE       , 1 , GL_R32I               , 4  , L"FORMAT_R32_SINT" );

        //__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS	= 21,
        __addFormat(PIXELFORMAT_DEPTH32       ,GL_FLOAT          , GL_DEPTH_COMPONENT , 1 , GL_DEPTH_COMPONENT32F , 4 , L"FORMAT_D32_FLOAT"            );
        __addFormat(PIXELFORMAT_DEPTH32S8UX24 ,GL_FLOAT          , GL_DEPTH_STENCIL   , 1 , GL_DEPTH32F_STENCIL8  , 5 , L"FORMAT_D32_FLOAT_S8X24_UINT" );
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R24G8_TYPELESS 
		__addFormat(PIXELFORMAT_DEPTH24       ,GL_UNSIGNED_INT   , GL_DEPTH_COMPONENT , 1 , GL_DEPTH_COMPONENT24  , 4 , L"FORMAT_D24_UNORM_S8_UINT"    );
        __addFormat(PIXELFORMAT_DEPTH16       ,GL_UNSIGNED_SHORT , GL_DEPTH_COMPONENT , 1 , GL_DEPTH_COMPONENT16  , 2 , L"FORMAT_D16_UNORM"            );

		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R24_UNORM_X8_TYPELESS 
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_X24_TYPELESS_G8_UINT 
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8_TYPELESS 
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8_UNORM 
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8_UINT 
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8_SNORM 
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8G8_SINT 
		//__addFormat(PIXELFORMAT_R16F        ,DXGI_FORMAT_R16_TYPELESS 
		__addFormat(PIXELFORMAT_R16F          ,GL_HALF_FLOAT     , GL_LUMINANCE , 1 ,  GL_R16F	 , 2   , L"FORMAT_R16_FLOAT"  );
		__addFormat(PIXELFORMAT_R16U          ,GL_UNSIGNED_SHORT , GL_LUMINANCE , 1 ,  GL_R16UI	 , 2   , L"FORMAT_R16_UNORM"  );
		//__addFormat(PIXELFORMAT_R16U          ,GL_UNSIGNED_SHORT , GL_LUMINANCE , 1 ,  GL_R16UI	 , 2   , L"FORMAT_R16_UINT"   );
		__addFormat(PIXELFORMAT_R16S          ,GL_SHORT          , GL_LUMINANCE , 1 ,  GL_R16I	 , 2   , L"FORMAT_R16_SNORM"  );
		//__addFormat(PIXELFORMAT_R16S          ,GL_SHORT          , GL_LUMINANCE , 1 ,  GL_R16I	 , 2   , L"FORMAT_R16_SINT"   );
		//__addFormat(PIXELFORMAT_            ,DXGI_FORMAT_R8_TYPELESS	= 60,
		__addFormat(PIXELFORMAT_LUMINANCE8       ,GL_UNSIGNED_BYTE  , GL_LUMINANCE       , 1 , GL_LUMINANCE        ,   1 , L"FORMAT_R8_UNORM" , GL_COMPRESSED_LUMINANCE);
        __addFormat(PIXELFORMAT_LUMINANCE16      ,GL_UNSIGNED_SHORT , GL_LUMINANCE       , 1 , GL_LUMINANCE16      ,   2 , L"FORMAT_R16_UNORM");
        __addFormat(PIXELFORMAT_LUMINANCE16F     ,GL_UNSIGNED_BYTE  , GL_LUMINANCE       , 1 , GL_LUMINANCE16F_ARB ,   1 , L"FORMAT_R16_FLOAT");
        __addFormat(PIXELFORMAT_LUMINANCE32F     ,GL_UNSIGNED_BYTE  , GL_LUMINANCE       , 1 , GL_LUMINANCE32F_ARB ,   1 , L"FORMAT_R32_FLOAT");
        __addFormat(PIXELFORMAT_LUMINANCE8ALPHA8 ,GL_UNSIGNED_BYTE  ,  GL_LUMINANCE_ALPHA, 2 , GL_LUMINANCE8_ALPHA8,   1 , L"FORMAT_LUMINANCE8ALPHA8" , GL_COMPRESSED_LUMINANCE_ALPHA);

		__addFormat(PIXELFORMAT_ALPHA8           ,GL_UNSIGNED_BYTE  , GL_ALPHA           , 1 , GL_ALPHA8           ,   1 , L"FORMAT_A8_UNORM"  , GL_COMPRESSED_ALPHA);
		//__addFormat(PIXELFORMAT_ALPHA8      ,DXGI_FORMAT_R1_UNORM	 , L"FORMAT_R1_UNORM"    , 1 , 2);
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_R9G9B9E5_SHAREDEXP	= 67,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_R8G8_B8G8_UNORM	= 68,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_G8R8_G8B8_UNORM	= 69,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC1_TYPELESS	= 70,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC1_UNORM	= 71,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC1_UNORM_SRGB	= 72,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC2_TYPELESS	= 73,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC2_UNORM	= 74,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC2_UNORM_SRGB	= 75,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC3_TYPELESS	= 76,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC3_UNORM	= 77,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC3_UNORM_SRGB	= 78,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC4_TYPELESS	= 79,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC4_UNORM	= 80,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC4_SNORM	= 81,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC5_TYPELESS	= 82,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC5_UNORM	= 83,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_BC5_SNORM	= 84,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_B5G6R5_UNORM	= 85,
		//__addFormat(PIXELFORMAT_ ,DXGI_FORMAT_B5G5R5A1_UNORM	= 86,
		__addFormat(PIXELFORMAT_B8G8R8A8 ,GL_UNSIGNED_BYTE  , GL_BGRA_EXT , 4 , GL_RGBA8 , 1 , L"FORMAT_B8G8R8A8_UNORM");
		__addFormat(PIXELFORMAT_B8G8R8X8 ,GL_UNSIGNED_BYTE  , GL_BGRA_EXT , 4 , GL_RGBA8 , 1 , L"FORMAT_B8G8R8X8_UNORM");

        __addFormat(PIXELFORMAT_DXT1     , GL_UNSIGNED_BYTE , GL_RGBA     , 4, GL_RGB8 , sizeof(char) , L"FORMAT_DXT1" ,  GL_COMPRESSED_RGB  );
        __addFormat(PIXELFORMAT_DXT2     , GL_UNSIGNED_BYTE , GL_RGBA     , 4, GL_RGB8 , sizeof(char) , L"FORMAT_DXT2" ,  GL_COMPRESSED_RGB  );
        __addFormat(PIXELFORMAT_DXT3     , GL_UNSIGNED_BYTE , GL_RGBA     , 4, GL_RGB8 , sizeof(char) , L"FORMAT_DXT3" ,  GL_COMPRESSED_RGB  );
        __addFormat(PIXELFORMAT_DXT4     , GL_UNSIGNED_BYTE , GL_RGBA     , 4, GL_RGB8 , sizeof(char) , L"FORMAT_DXT4" ,  GL_COMPRESSED_RGB  );
        __addFormat(PIXELFORMAT_DXT5     , GL_UNSIGNED_BYTE , GL_RGBA     , 4, GL_RGB8 , sizeof(char) , L"FORMAT_DXT5" ,  GL_COMPRESSED_RGB  );

}
xGL2ConstLexer::~xGL2ConstLexer()
{

}

xGL2ConstLexer* xGL2ConstLexer::singleton()
{
	static xGL2ConstLexer g_s;
	return &g_s;
}

GLuint xGL2ConstLexer::GetFillMode(const wchar_t* _fill)
{
	if(_fill == NULL) _fill = L"fill";
	std::wstring fillMode = _fill ;
	if(fillMode == L"line" || fillMode == L"wireframe") return GL_LINE	;
	if(fillMode == L"solid"|| fillMode == L"fill" )     return GL_FILL	;
	 return GL_FILL	;
}

GLuint xGL2ConstLexer::GetCullMode(const wchar_t* _cull)
{	
	if(_cull == NULL) _cull = L"none";
    std::wstring cullMode = _cull ;
	if(cullMode == L"none")            return  GL_NONE;
	if(cullMode == L"front")           return  GL_FRONT;
	if(cullMode == L"back")            return  GL_BACK;
	return  GL_BACK;
}

GLuint  xGL2ConstLexer::GetBlendOp(const wchar_t* _blenop)
{
	if(_blenop == NULL) _blenop = L"add";
	std::wstring blendop = _blenop ;
	if(blendop == L"add")            return  GL_ADD;
	if(blendop == L"subtract")       return  GL_FUNC_SUBTRACT;
	if(blendop == L"rev_subtract")   return  GL_FUNC_REVERSE_SUBTRACT;
	if(blendop == L"min")            return  GL_MIN;
	if(blendop == L"max")            return  GL_MAX;
	return GL_ADD;
}

GLuint  xGL2ConstLexer::GetBlendFactor(const wchar_t* _blend)
{
	if(_blend == NULL) _blend = L"one";
	std::wstring blend = _blend ;
	if(blend == L"zero")            return  GL_ZERO;
	if(blend == L"one")             return  GL_ONE;
	if(blend == L"src")             return  GL_SRC_COLOR;
	if(blend == L"inv_src")         return  GL_ONE_MINUS_SRC_COLOR;
	if(blend == L"src_alpha")       return  GL_SRC_ALPHA;
	if(blend == L"inv_src_alpha")   return 	GL_ONE_MINUS_SRC_ALPHA;
	if(blend == L"dest_alpha")      return 	GL_DST_ALPHA;
	if(blend == L"inv_dest_alpha")  return 	GL_ONE_MINUS_DST_ALPHA;
	if(blend == L"dest")            return 	GL_DST_COLOR;
	if(blend == L"inv_dest")        return 	GL_ONE_MINUS_DST_COLOR;
	if(blend == L"factor")          return 	GL_CONSTANT_COLOR;
	if(blend == L"inv_factor")      return 	GL_ONE_MINUS_CONSTANT_COLOR;
	if(blend == L"src1")            return 	GL_SOURCE1_RGB;
	if(blend == L"src1_alpha")      return 	GL_SRC1_ALPHA;
	return  GL_ONE;
}

int xGL2ConstLexer::GetTextureSlotIdx(const wchar_t* _texName)
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

GLenum xGL2ConstLexer::GetMinFilter(const wchar_t* _filter_val , const wchar_t* _mip_val , bool bMagFilter)
{
	if(_filter_val == NULL) _filter_val = L"linear";
	if(_mip_val == NULL) _mip_val = L"linear";
    return GL_LINEAR_MIPMAP_LINEAR;
	std::wstring filter_val = _filter_val ;
	std::wstring mip_val    = _mip_val ;
	GLenum filter = GL_LINEAR_MIPMAP_LINEAR;
	if(_filter_val == L"anisotropic" || _filter_val == L"aniso" || _filter_val == L"anisotropic" || _filter_val == L"aniso")
	{
		filter = GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT;
        return filter;
	}

	int iFilter  = (filter_val == L"linear") ? 1: 0; 
	int iMip  = (mip_val    == L"linear") ? 1: 0; 
    
    if(bMagFilter)
    {
        if(iFilter == 1) return GL_LINEAR;
        return GL_NEAREST;
    }

	GLenum a[2][2];
	a[0][0] = GL_NEAREST_MIPMAP_NEAREST;
	a[0][1] = GL_NEAREST_MIPMAP_LINEAR ;
	a[1][0] = GL_LINEAR_MIPMAP_NEAREST;
	a[1][1] = GL_LINEAR_MIPMAP_LINEAR;

	return a[iFilter][iMip];
}



GLenum xGL2ConstLexer::GetAdress(const wchar_t* _addressMode)
{
	if(_addressMode == NULL) _addressMode = L"clamp";
	std::wstring addressMode = _addressMode ;
	if(addressMode == L"wrap")       return GL_REPEAT;
	if(addressMode == L"mirror")     return GL_MIRROR_CLAMP_EXT ;
	if(addressMode == L"clamp")      return GL_CLAMP ;
	if(addressMode == L"border")     return GL_CLAMP_TO_BORDER ;
	return GL_CLAMP;
}

GLenum xGL2ConstLexer::GetCompareFunc(const wchar_t* _func)
{
	if(_func == NULL) _func = L"never";
	std::wstring func = _func ;
	if(func == L"never")   return GL_NEVER;
	if(func == L"less")    return GL_LESS;
	if(func == L"equal")   return GL_EQUAL;
	if(func == L"lequal")  return GL_LEQUAL;
	if(func == L"greator") return GL_GREATER;
	if(func == L"nequale") return GL_NOTEQUAL;
	if(func == L"gequale") return GL_GEQUAL;
	if(func == L"always")  return GL_ALWAYS;
	return GL_NEVER;
}

GLenum xGL2ConstLexer::GetStencilOp(const wchar_t* _op)
{
	if(_op == NULL) _op = L"never";
	std::wstring func = _op ;
	if(func == L"keep")       return GL_KEEP;
	if(func == L"zero")       return GL_ZERO;
	if(func == L"replace")    return GL_REPLACE;

	if(func == L"invert")     return GL_INVERT;
	if(func == L"increase")   return GL_INCR;
	if(func == L"decrease")   return GL_DECR;
	if(func == L"incr")       return GL_INCR;
	if(func == L"decr")       return GL_DECR;
	return GL_KEEP;
}


END_NAMESPACE_XEVOL3D
