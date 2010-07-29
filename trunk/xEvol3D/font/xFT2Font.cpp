/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "../xStdPch.h"
#include "../RenderAPI/xRenderAPI.h"
#include "../RenderAPI/xBaseTexture.h"
#include "../OperationSys/xOperationSys.h"
#include "xFT2Font.h"
#include "xFontRender.h"
#include "../BaseLib/xI18N.h"

#ifndef FT_PIX_FLOOR
#define FT_PIX_FLOOR( x )     ( (x) & ~63 )
#define FT_PIX_ROUND( x )     FT_PIX_FLOOR( (x) + 32 )
#define FT_PIX_CEIL( x )      FT_PIX_FLOOR( (x) + 63 )
#endif

#ifdef _WIN32
#ifndef _WIN32_WCE
#pragma comment(lib , "freetype.lib")
#endif
#endif

//#define  new XNEW_OP

BEGIN_NAMESPACE_XEVOL3D


static FT_Library g_FT2Lib;

int GetBoldenSize(bool bBold , int w )
{
	if(bBold == false ) return w ;
	return (int)(w * 1.2f);
}

int GetBoldenStrength(bool bBold , int w )
{
	if(bBold == false ) return 0 ;
	return (int)(w * 0.5f);
}

void xFT2IndexManager::setMaxIndex(int maxIndex)
{
	m_maxIndex = maxIndex;
	m_vFreeIndex.resize(m_maxIndex);
	for(int i = 0  ; i < m_maxIndex ; i ++)
	{
		m_vFreeIndex.push_back(i);
	}
}
void xFT2IndexManager::freeAllIndex()
{
	setMaxIndex(m_maxIndex );
}

int  xFT2IndexManager::useIndex()
{
	size_t idxSize = m_vFreeIndex.size(); 
	if( idxSize == 0)
		return -1;
	int idx = m_vFreeIndex[ idxSize - 1];
	m_vFreeIndex.pop_back();
	return idx;
}

void xFT2IndexManager::freeIndex(int index)
{
	m_vFreeIndex.push_back(index);
}



xFT2FontChar::xFT2FontChar(IFontRenderDevice* pRenderer  , xFT2FontCharLoader* pFT2FontLoader)
{
	m_adv_x = 0.0f;
	m_adv_y = 0.0f;
	m_left = 0.0f;
	m_top = 0.0f;
#ifndef _FONT_FULL_TEXTURE_
	m_pTexture = NULL;
	//m_pTexture->setFlipY( !m_pTexture->isFlipY() );
#else
	m_FT2FontLoader = pFT2FontLoader;
#endif
}

xFT2FontChar::~xFT2FontChar()
{
#ifndef _FONT_FULL_TEXTURE_
	m_pTexture->KillObject();
#endif
}
void xFT2FontChar::unload()
{
#ifndef _FONT_FULL_TEXTURE_
	m_pTexture->unload();
#else
	m_FT2FontLoader->m_idxManager.freeIndex(m_tex_idx);
#endif
}

static vector<xFT2FontCharLoader*> gs_AllManager;
void xFT2FontCharLoader::releaseAll()
{
	vector<xFT2FontCharLoader*>::iterator pos ;
	while(gs_AllManager.size() > 0)
	{
		pos = gs_AllManager.begin();
		xFT2FontCharLoader* pLoader = *pos;
		delete pLoader;
	}
	gs_AllManager.clear();
}

xFT2FontCharLoader::xFT2FontCharLoader()
{
	gs_AllManager.push_back(this);
	static bool sb_isInited = false;
	if(sb_isInited == false) 
	{
		if (FT_Init_FreeType( &g_FT2Lib )) 
		{
			XEVOL_LOG(eXL_DEBUG_HIGH,"FT_Init_FreeType failed\n");
			assert(0);
		}
	}
	sb_isInited = true;
#ifdef _DEBUG
	m_hFontList = new HXFT2FontChar[65536];
#endif
	m_w = 32;
	m_h = 32;
	m_FT_Face = NULL;
#ifdef _FONT_FULL_TEXTURE_
	m_pTexture = NULL;
#endif
}

xFT2FontCharLoader::~xFT2FontCharLoader()
{
	vector<xFT2FontCharLoader*>::iterator pos = gs_AllManager.begin();
	for(; pos != gs_AllManager.end() ; ++pos)
	{
		xFT2FontCharLoader* pLoader = *pos;
		if(pLoader == this)
		{
		    gs_AllManager.erase(pos);
			break;
		}
	}

	if(m_FT_Face)
	{
		FT_Done_Face(m_FT_Face);
	}
	xFT2FontCharMgr* pResMgr = m_pThis;
	if(pResMgr) pResMgr->clear();
#ifdef _DEBUG
	delete [] m_hFontList;
#endif

#ifdef _FONT_FULL_TEXTURE_
	if ( m_pTexture )
	{
		m_pTexture->KillObject();
		m_pTexture = NULL;
	}
#endif

}

/*
// 就是FT_Outline_Embolden
FT_Error Old_FT_Outline_Embolden( FT_Outline*  outline, FT_Pos strength )
{
	FT_Vector*    points;
	FT_Vector     v_prev, v_first, v_next, v_cur;
	FT_Angle      rotate, angle_in, angle_out;
	FT_Int        c, n, first;
	FT_Int        orientation;

	if ( !outline )
		return FT_Err_Invalid_Argument;

	strength /= 2;
	if ( strength == 0 )
		return FT_Err_Ok;

	orientation = FT_Outline_Get_Orientation( outline );
	if ( orientation == FT_ORIENTATION_NONE )
	{
		if ( outline->n_contours )
			return FT_Err_Invalid_Argument;
		else
			return FT_Err_Ok;
	}

	if ( orientation == FT_ORIENTATION_TRUETYPE )
		rotate = -FT_ANGLE_PI2;
	else
		rotate = FT_ANGLE_PI2;

	points = outline->points;

	first = 0;
	for ( c = 0; c < outline->n_contours; c++ )
	{
		int  last = outline->contours[c];

		v_first = points[first];
		v_prev  = points[last];
		v_cur   = v_first;

		for ( n = first; n <= last; n++ )
		{
			FT_Vector    in, out;
			FT_Angle    angle_diff;
			FT_Pos        d;
			FT_Fixed    scale;

			if ( n < last )
				v_next = points[n + 1];
			else
				v_next = v_first;

			// compute the in and out vectors 
			in.x = v_cur.x - v_prev.x;
			in.y = v_cur.y - v_prev.y;

			out.x = v_next.x - v_cur.x;
			out.y = v_next.y - v_cur.y;

			angle_in   = FT_Atan2( in.x, in.y );
			angle_out  = FT_Atan2( out.x, out.y );
			angle_diff = FT_Angle_Diff( angle_in, angle_out );
			scale      = FT_Cos( angle_diff / 2 );

			if ( scale < 0x4000L && scale > -0x4000L )
				in.x = in.y = 0;
			else
			{
				d = FT_DivFix( strength, scale );

				FT_Vector_From_Polar( &in, d, angle_in + angle_diff / 2 - rotate );
			}

			outline->points[n].x = v_cur.x + strength + in.x;
			//伀偙傟傪僐儊儞僩傾僂僩偟偨偩偗
			//outline->points[n].y = v_cur.y + strength + in.y;

			v_prev = v_cur;
			v_cur  = v_next;
		}

		first = last + 1;
	}

	return FT_Err_Ok;
}
*/

// 垂直加粗
//FT_Error FT_Outline_Embolden_Vert( FT_Outline*  outline, FT_Pos strength )
//{
//	FT_Vector*    points;
//	FT_Vector    v_prev, v_first, v_next, v_cur;
//	FT_Angle    rotate, angle_in, angle_out;
//	FT_Int        c, n, first;
//	FT_Int        orientation;
//
//	if ( !outline )
//		return FT_Err_Invalid_Argument;
//
//	strength /= 2;
//	if ( strength == 0 )
//		return FT_Err_Ok;
//
//	orientation = FT_Outline_Get_Orientation( outline );
//	if ( orientation == FT_ORIENTATION_NONE )
//	{
//		if ( outline->n_contours )
//			return FT_Err_Invalid_Argument;
//		else
//			return FT_Err_Ok;
//	}
//
//	if ( orientation == FT_ORIENTATION_TRUETYPE )
//		rotate = -FT_ANGLE_PI2;
//	else
//		rotate = FT_ANGLE_PI2;
//
//	points = outline->points;
//
//	first = 0;
//	for ( c = 0; c < outline->n_contours; c++ )
//	{
//		int  last = outline->contours[c];
//
//		v_first = points[first];
//		v_prev  = points[last];
//		v_cur   = v_first;
//
//		for ( n = first; n <= last; n++ )
//		{
//			FT_Vector    in, out;
//			FT_Angle    angle_diff;
//			FT_Pos        d;
//			FT_Fixed    scale;
//
//			if ( n < last )
//				v_next = points[n + 1];
//			else
//				v_next = v_first;
//
//			/**//* compute the in and out vectors */
//			in.x = v_cur.x - v_prev.x;
//			in.y = v_cur.y - v_prev.y;
//
//			out.x = v_next.x - v_cur.x;
//			out.y = v_next.y - v_cur.y;
//
//			angle_in   = FT_Atan2( in.x, in.y );
//			angle_out  = FT_Atan2( out.x, out.y );
//			angle_diff = FT_Angle_Diff( angle_in, angle_out );
//			scale      = FT_Cos( angle_diff / 2 );
//
//			if ( scale < 0x4000L && scale > -0x4000L )
//				in.x = in.y = 0;
//			else
//			{
//				d = FT_DivFix( strength, scale );
//
//				FT_Vector_From_Polar( &in, d, angle_in + angle_diff / 2 - rotate );
//			}
//
//			//outline->points[n].x = v_cur.x + strength + in.x;
//			//仾偙傟傪僐儊儞僩傾僂僩偟偨偩偗
//			outline->points[n].y = v_cur.y + strength + in.y;
//
//			v_prev = v_cur;
//			v_cur  = v_next;
//		}
//
//		first = last + 1;
//	}
//
//	return FT_Err_Ok;
//}
//
//// 新的加粗函数
//FT_Error FT2_Outline_Embolden( FT_Outline*  outline, FT_Pos str_h, FT_Pos str_v )
//{
//	if ( !outline ) return FT_Err_Invalid_Argument;
//	int orientation = FT_Outline_Get_Orientation( outline );
//	if ( orientation == FT_ORIENTATION_NONE )
//		if ( outline->n_contours ) return FT_Err_Invalid_Argument;
//	FT_Outline_Embolden_Vert( outline, str_v );
//	FT_Outline_Embolden( outline, str_h );
//	return FT_Err_Ok;
//}
//
//// 让一个字体槽加粗，并且填充其他的大小属性
//void FT2_GlyphSlot_Embolden( FT_GlyphSlot  slot , FT_Pos xEmbolden , FT_Pos yEmbolden)
//{
//	if(xEmbolden == 0 && yEmbolden == 0)
//		return;
//	FT_Library  library = slot->library;
//	FT_Face     face    = slot->face;
//	FT_Error    error;
//	FT_Pos      xstr = xEmbolden; 
//	FT_Pos      ystr = yEmbolden;
//
//
//	if ( slot->format != FT_GLYPH_FORMAT_OUTLINE &&
//		slot->format != FT_GLYPH_FORMAT_BITMAP )
//		return;
//
//	if ( slot->format == FT_GLYPH_FORMAT_OUTLINE )
//	{
//		FT_BBox oldBox;
//		FT_Outline_Get_CBox(&slot->outline , &oldBox);
//		error = FT2_Outline_Embolden( &slot->outline, xstr , ystr);
//		if ( error )
//			return;
//
//		FT_BBox newBox;
//		FT_Outline_Get_CBox(&slot->outline , &newBox);
//		xstr = (newBox.xMax - newBox.xMin) - (oldBox.xMax - oldBox.xMin);
//		ystr = (newBox.yMax - newBox.yMin) - (oldBox.yMax - oldBox.yMin);
//	}
//	else if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
//	{
//		xstr = FT_PIX_FLOOR( xstr );
//		if ( xstr == 0 )
//			xstr = 1 << 6;
//		ystr = FT_PIX_FLOOR( ystr );
//
//		error = FT_Bitmap_Embolden( library, &slot->bitmap, xstr, ystr );
//		if ( error )
//			return;
//	}
//
//	if ( slot->advance.x )
//		slot->advance.x += xstr;
//
//	if ( slot->advance.y )
//		slot->advance.y += ystr;
//
//	slot->metrics.width        += xstr;
//	slot->metrics.height       += ystr;
//	slot->metrics.horiBearingY += ystr;
//	slot->metrics.horiAdvance  += xstr;
//	slot->metrics.vertBearingX -= xstr / 2;
//	slot->metrics.vertBearingY += ystr;
//	slot->metrics.vertAdvance  += ystr;
//
//	if ( slot->format == FT_GLYPH_FORMAT_BITMAP )
//		slot->bitmap_top += ystr >> 6;
//}

bool xFT2FontCharLoader::load_font(const wchar_t* font_file , int _w , int _h)
{
	char strFontFile[1024] = {0};
	XEvol_UnicodeToFsEnc(font_file,strFontFile,1024);
	//加载一个字体,取默认的Face,一般为Regualer
	if (FT_New_Face( g_FT2Lib, strFontFile, 0, &m_FT_Face )) 
	{
		XEVOL_LOG(eXL_DEBUG_HIGH,"FT_New_Face failed (there is probably a problem with your font file\n   <font = %s> \n" , strFontFile);
		return false;
	}
	FT_Select_Charmap(m_FT_Face, FT_ENCODING_UNICODE);
	m_w = _w ; m_h = _h;
	m_FT_Face->num_fixed_sizes;
	//大小要乘64.这是规定。照做就可以了。
	//FT_Set_Char_Size( m_FT_Face , 0 , m_w << 6, 96, 96);
	FT_Set_Pixel_Sizes(m_FT_Face,m_w, m_h);
	return true;
}

unsigned int xFT2FontCharLoader::_getResSize(xFT2FontChar*& pRes) 
{
	return 1;
}

bool xFT2FontCharLoader::_isResLoaded(xFT2FontChar* pRes)
{
	return pRes != NULL;
}
#define FONTSIZE_SCALE 1
bool xFT2FontCharLoader::_loadResource(xWCharType& _char , xFT2FontChar*& pRes, int& ResSize,unsigned int arg)
{
	//if(FT_Load_Glyph( m_FT_Face, FT_Get_Char_Index( m_FT_Face, _char ), FT_LOAD_DEFAULT ))
	//	throw std::runtime_error("FT_Load_Glyph failed");

	FT_Int32 load_flags = FT_LOAD_RENDER|FT_LOAD_FORCE_AUTOHINT ;

	if(m_bAntilias) 
		load_flags |= ( FT_LOAD_TARGET_NORMAL| FT_LOAD_TARGET_LIGHT);
	else
		load_flags |= ( FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO );



	if(pRes == NULL)
	{
		pRes = new xFT2FontChar(m_pRenderer ,  this );
		if(pRes == NULL)
			return false;
	}
	//得到字模
	FT_UInt  glyph_index;
	/* retrieve glyph index from character code */
	glyph_index = FT_Get_Char_Index( m_FT_Face, _char );
	/* load glyph image into the slot (erase previous one) */
	int error = FT_Load_Glyph( m_FT_Face, glyph_index, FT_LOAD_DEFAULT );
    
	//if(m_bBold)
	//{
	//	int xStrenth = GetBoldenStrength(m_bBold , m_w);
	//	int yStrenth = GetBoldenStrength(m_bBold , m_h);
	//	FT2_GlyphSlot_Embolden(m_FT_Face->glyph , xStrenth , yStrenth );
	//}
	if(m_bAntilias)
	{
		error = FT_Render_Glyph( m_FT_Face->glyph, FT_RENDER_MODE_NORMAL );
	}
	else
	{
		error = FT_Render_Glyph( m_FT_Face->glyph, FT_RENDER_MODE_NORMAL );
	}


	FT_Glyph glyph;
	if(FT_Get_Glyph( m_FT_Face->glyph, &glyph ))
	{
		XEVOL_LOG(eXL_DEBUG_HIGH,"FT_Load_Glyph failed\n");
		return false;
	}
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	//取道位图数据
	FT_Bitmap& bitmap = bitmap_glyph->bitmap;

	//把位图数据拷贝自己定义的数据区里.这样旧可以画到需要的东西上面了。
	int width  =  bitmap.width;
	int height =  bitmap.rows;

	m_FT_Face->size->metrics.y_ppem;
	m_FT_Face->glyph->metrics.horiAdvance;
	pRes->m_adv_x = m_FT_Face->glyph->advance.x / 64.0f;
	pRes->m_adv_y = m_FT_Face->size->metrics.y_ppem; //m_FT_Face->glyph->metrics.horiBearingY / 64.0f;
	pRes->m_left = (float)bitmap_glyph->left;
	pRes->m_top  = (float)bitmap_glyph->top;

	int   tex_pitch =  0;
	unsigned char* tex_pixel = NULL;
    IBaseTexture* pTexture = NULL;
#ifndef _FONT_FULL_TEXTURE_
	if(pRes->m_pTexture != NULL)
	{
		pRes->m_pTexture->KillObject();
	}

	if(width == 0 || height == 0)
	{
		FT_Done_Glyph(glyph);
		return true;
	}

	if(m_pRenderer->isTextureSupport( PIXELFORMAT_ALPHA8 ) )
	{
		pRes->m_pTexture = m_pRenderer->createTexture(  width,height, PIXELFORMAT_ALPHA8   , false);//Alpha8);
	}																		 						
	else																	 						
	{																		 						
		pRes->m_pTexture = m_pRenderer->createTexture(  width,height, PIXELFORMAT_B8G8R8A8 , false);//RGBA);
	}
	
	if(pRes->m_pTexture == NULL)
		return width == 0 && height == 0;

    pTexture = pRes->m_pTexture;
	xTextureLockArea lockInfo;
	pTexture->lock(eLock_WriteDiscard , lockInfo);
	tex_pixel = (unsigned char*)lockInfo.m_pixels ;
	tex_pitch = lockInfo.m_picth ;

	pRes->m_tex_w = width ;
	pRes->m_tex_h = height;

#else
    pTexture = m_pTexture;
	pRes->m_tex_idx = m_idxManager.useIndex();
	pRes->m_tex_y   = pRes->m_tex_idx / m_nCharOfRow;
	int nFontWidth  = m_tex_w / m_nCharOfRow;
	int nFontHeight = m_tex_h / m_nCharOfRow;


	pRes->m_tex_y   = nFontHeight * pRes->m_tex_y;
    pRes->m_tex_x   = (pRes->m_tex_idx % m_nCharOfRow ) * nFontWidth;

	pRes->m_tex_w = width ;
	pRes->m_tex_h = height;

	xTextureLockArea lockInfo;
	pTexture->lock(eLock_WriteDiscard , lockInfo);
	tex_pixel = (unsigned char*)lockInfo.m_pixels ;
	tex_pitch = lockInfo.m_picth ;
	if(pTexture->format() == PIXELFORMAT_B8G8R8A8)
	{
		tex_pixel += lockInfo.m_picth * pRes->m_tex_y + pRes->m_tex_x * 4;
	}
	else
	{
		tex_pixel += lockInfo.m_picth * pRes->m_tex_y + pRes->m_tex_x;
	}
	
#endif



	float grayScale = 1.0f;

	if( m_bAntilias == false ) grayScale = 1.3f;
	//灰度图
	if(bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
	{
		for(int y=0; y  < height ; y++)
		{
			for(int x=0; x < width; x++)
			{
				int _y = y;
				unsigned char _vl =  0;
				if(x < bitmap.width && y < bitmap.rows) 
				{
					_vl =  (x>=bitmap.width || y>=bitmap.rows) ? 0 : bitmap.buffer[x + bitmap.width*y];
				}
				//if(_vl > 25 && _vl < 220) _vl = 220;

				unsigned int iVal = _vl; iVal = (int)(grayScale * iVal);
				if(iVal > 255) iVal = 255;
				if(pTexture->format() == PIXELFORMAT_B8G8R8A8)
				{

					tex_pixel[(4*x + _y * tex_pitch)+0] = 0xff;
					tex_pixel[(4*x + _y * tex_pitch)+1] = 0xff;
					tex_pixel[(4*x + _y * tex_pitch)+2] = 0xff;
					tex_pixel[(4*x + _y * tex_pitch)+3] = (unsigned char)iVal;
				}
				else
				{
					tex_pixel[(1*x + _y * tex_pitch)+0] = (unsigned char)iVal;
				}				
			}
		}
	}
	else if(bitmap.pixel_mode == FT_PIXEL_MODE_MONO) //单色图
	{
		for(int y=0; y  < height ; y++)
		{
			for(int x=0; x < width; x++)
			{
				int _y = y;
				unsigned char _vl =  0;
				if(x < bitmap.width && y < bitmap.rows) 
				{
					_vl =  ((bitmap.buffer[(y * bitmap.pitch) + x / 8] << (x % 8)) & 0x80) ? 0xFFFFFFFF : 0x00000000;
				}
				unsigned int iVal = _vl; iVal = (int)(grayScale * iVal);
				if(iVal > 255) iVal = 255;

				if(pTexture->format() == PIXELFORMAT_B8G8R8A8)
				{
					tex_pixel[(4*x + _y * tex_pitch)+0] = 0xff;
					tex_pixel[(4*x + _y * tex_pitch)+1] = 0xff;
					tex_pixel[(4*x + _y * tex_pitch)+2] = 0xff;
					tex_pixel[(4*x + _y * tex_pitch)+3] = (unsigned char)iVal;
				}
				else
				{
					tex_pixel[(1*x + _y * tex_pitch)+0] = (unsigned char)iVal;
				}

			}
		}
	}
	pTexture->unlock(lockInfo);
	pTexture->validate();
	FT_Done_Glyph(glyph);
	ResSize = 1;
	return true;
}

bool xFT2FontCharLoader::_unloadResource(xWCharType& _char , xFT2FontChar*& pRes, unsigned int& TotalResSize)
{
	pRes->unload();
	TotalResSize -= 1;
	return true;
}

void xFT2FontCharLoader::_deleteResource(xWCharType& _char , xFT2FontChar* pRes)
{
	delete pRes;
	pRes = NULL;
}

void xFT2FontCharLoader::setCacheSize(int maxSize , int maxFontW , int maxFontH)
{

#ifdef _FONT_FULL_TEXTURE_
	m_nCharOfRow = (int) ( sqrt( (float)maxSize )  + 0.5f );
	
	int xStrenth = GetBoldenStrength(m_bBold , m_w);
	int yStrenth = GetBoldenStrength(m_bBold , m_h);

	int tex_w = m_nCharOfRow * ( GetBoldenSize(m_bBold , m_w) * FONTSIZE_SCALE);
	int tex_h = m_nCharOfRow * ( GetBoldenSize(m_bBold , m_h) * FONTSIZE_SCALE);
	m_tex_w = tex_w;
	m_tex_h = tex_h;
	if( m_pTexture ) m_pTexture->KillObject();
	m_idxManager.setMaxIndex(  m_nCharOfRow * m_nCharOfRow );
	if( m_pRenderer->isTextureSupport(PIXELFORMAT_ALPHA8) )
	{
		m_pTexture = m_pRenderer->createTexture( tex_w , tex_h , PIXELFORMAT_ALPHA8   , true);
	}																	 					
	else																 					
	{																	 					
		m_pTexture = m_pRenderer->createTexture(  tex_w , tex_h ,PIXELFORMAT_B8G8R8A8 ,  true);
	}
	//m_pTexture->setFlipY( !m_pTexture->isFlipY() );
#endif
}
//88888888888888888888888888888888888888888

IMPL_OBJECT_FACTORY(xFT2Font , xFontRender , xFT2FontCreator , xFontRenderCreator , xFontRenderCreatorMgr , L"freetype", L"freetype font" , xFontLoader* , int );

void  xFT2Font::releaseChache()
{
	m_pFontCharMgr->unload();
}

void xFT2Font::setCacheSize(int maxSize)
{
	m_pFontCharMgr->setCacheSize(maxSize , m_Info.nMaxWidth , m_Info.nMaxHeight);
#ifdef _FONT_FULL_TEXTURE_
	 m_pFontCharMgr->unload();
#endif

     m_pFontCharMgr->setMaxSize(maxSize);
}

int  xFT2Font::getCacheSize()
{
	return m_pFontCharMgr->getMaxSize();
}
IMPL_BASE_OBJECT_CLASSID(xFT2Font  , xFontRender);

void GetFontFileName(const wchar_t* _in, wchar_t* _out)
{
	if( wcslen(_in) < 2 || _in[0] !='$')
	{
		wcscpy(_out , _in);
		return ;
	}
	if(_in[0]=='$' && _in[1] == '$' )
	{
		wcscpy(_out , &_in[1]);
		return ;
	}

	size_t i = 1;
	for(i = 1 ; i < wcslen(_in) ; i ++ )
	{
		if(_in[i] == '$')
			break;
	}

	//没找到匹配的
	if( i == wcslen(_in) )
	{
		wcscpy(_out , &_in[1]);
		return ;
	}

	wchar_t systemRoot[256];
	wcsncpy(systemRoot , &_in[1] , 256);
	systemRoot[i-1] = 0;
#ifdef _UNIX
	if(std::wstring(systemRoot) == L"FontDir")
	{
		wcsncpy(systemRoot,L"./font",257);
	}
	else
	{
		wcsncpy(systemRoot,L"./font",257);
	}
#endif

#ifdef _WIN32

#ifdef _WIN32_WCE
	//WINCE 目前只支持当前字体
	wcsncpy(systemRoot,L".\font",257);
#else
	if(std::wstring(systemRoot) == L"FontDir")
	{
		GetWindowsDirectoryW(systemRoot,256);
		wcscat(systemRoot,L"\\Fonts");
	}
	else
	{
		wcsncpy(systemRoot,L".\font",257);
	}
#endif

#endif

	wcsncpy(_out , systemRoot , 256);
	wcsncpy(&_out[wcslen(systemRoot)] , &_in[i+1] , 256);
	return ;
}



xFT2Font::xFT2Font(xFontLoader* pFontLoader , int arg):xFontRender(pFontLoader , arg)
{
	m_RefCount = 1 ;
	m_LinePitch = 5;
	m_Filter = eFontFilter_Point;

}

xFT2Font::~xFT2Font()
{
    
}
bool xFT2Font::init( xXmlNode* pFontNode)
{
	xFontLoader* pFontLoader = m_pFontLoader;

	const wchar_t*  pFontName    = pFontNode->value(L"name");
	const wchar_t*  famillyName  = pFontNode->value(L"familly" ) ;
	wstring         fullFontName = wstring( famillyName ) + pFontName; 
	const  wchar_t* pFiltertName   = pFontNode->value(L"filter");

	//从配置文件取到数据
	xFontInitor fi;
	fi.height     = pFontNode->int_value(L"height");
	fi.width      = pFontNode->int_value(L"width");
	fi.antialias  = pFontNode->bool_value(L"antialias");
	fi.bold       = pFontNode->bool_value(L"bold");
	fi.italic     = pFontNode->bool_value(L"italic");
	fi.underline  = pFontNode->bool_value(L"underline");
	fi.maxcache   = pFontNode->int_value(L"maxcache");
	fi.filter =     pFontLoader->getFontSamplerFilter(pFiltertName);
	fi.linepitch  = 0;
	if(pFontNode->value(L"line_pitch"))
	{
		fi.linepitch = pFontNode->int_value(L"line_pitch");
	}


	wchar_t fileName[512]={0};
	GetFontFileName(pFontNode->value(L"file") , fileName);
	wstring AbsFontFile = _XEVOL_ABSPATH_(fileName);


	//因为是个新字体，所以需要做一个FontCharMgr来管理它
	createFontCharManager( fullFontName.c_str() ) ;
	xFT2FontCharMgr* pFontCharMgr = getFontCharManager();

	pFontCharMgr->setRenderer(m_pRendeDevice);
	pFontCharMgr->m_UnderLine = fi.underline;
	pFontCharMgr->m_bBold     = fi.bold;
	pFontCharMgr->m_Italic    = fi.italic;
	pFontCharMgr->m_bAntilias = fi.antialias;

	int maxCache = fi.maxcache;
	if(maxCache == 0) maxCache = 1024;

	if( pFontCharMgr->load_font(AbsFontFile.c_str(), fi.width , fi.height) ==false)
	{
		delete pFontCharMgr;
		return NULL; 
	}

	
	//pFontCharMgr->setMaxSize(maxCache);
	//g_FT2FontCharMgr.push_back(pFontCharMgr);

	//新建一个FreeType2的字体绘制器。并把它和Font


	m_Filter          = fi.filter;
	m_Info.bAntialias = fi.antialias;
	m_Info.bBold      = fi.bold;
	m_Info.bItalic    = fi.italic;
	m_Info.bUnderLine = fi.underline;

	m_Info.nMaxWidth  = GetBoldenSize(fi.bold,fi.width );
	m_Info.nMaxHeight = GetBoldenSize(fi.bold,fi.height);
	wcsncpy(m_Info.strFamilly , famillyName , 32);
	wcsncpy(m_Info.strName    , pFontName,         16);
	m_Info.nHeight =  fi.height;
	m_Info.nWidth  =  fi.width;
	m_LinePitch = fi.linepitch;

    setCacheSize(maxCache );
	return true;
}

void   xFT2Font::createFontCharManager(const wchar_t* managerName)
{
	m_pFontCharMgr = new xFT2FontCharMgr(managerName , 1);
    m_pFontCharMgr->setThis(m_pFontCharMgr);
}
void   xFT2Font::setFontChareManager(xFT2FontCharMgr* pMgr)
{
	m_pFontCharMgr = pMgr;
}
xFT2FontCharMgr*  xFT2Font::getFontCharManager()
{
	return m_pFontCharMgr;
}

bool  xFT2Font::getCharDim(xWCharType _chr , int& dx , int& dy)
{
    HXFT2FontChar hFontChar = GetFontCharHandle(_chr) ;
	dx = dy = 0;
	dx = int(hFontChar->m_adv_x ) + 2; 
	dy = int(hFontChar->m_adv_y ) ;
	return true;
}

void xFT2Font::enableAntialias(bool bAnti)
{
	if(m_pFontCharMgr->m_bAntilias == bAnti)
		return ;
	m_pFontCharMgr->unload();
	m_pFontCharMgr->m_bAntilias = true;
}
bool xFT2Font::isAntialias()
{
	return m_pFontCharMgr->m_bAntilias;
}

HXFT2FontChar xFT2Font::GetFontCharHandle(xWCharType _chr)
{
	xFT2FontCharMgr* FONTCHARMGR = (xFT2FontCharMgr*)(m_pFontCharMgr);
#ifdef _DEBUG
    if(_chr < 65536)
    {
        HXFT2FontChar hFontChar = FONTCHARMGR->m_hFontList[_chr];
        if(hFontChar.isHandle() == false)
        {
            hFontChar = FONTCHARMGR->add(_chr , (unsigned int) this,  true);
            FONTCHARMGR->m_hFontList[_chr] = hFontChar;
        } 
        return hFontChar;
    }
    else
    {
        HXFT2FontChar hFontChar = FONTCHARMGR->add(_chr , (unsigned int) this , true);
        if( hFontChar.RefCount() == 1)
        {
            hFontChar.AddRef();
        }
        return hFontChar;
    }
#else
	//Debug已经吧句柄保存起来，不存在RefCount为0的情况。
	HXFT2FontChar hFontChar = FONTCHARMGR->add(_chr , (unsigned int) this , true);
	if( hFontChar.RefCount() == 1)
	{
		hFontChar.AddRef();
	}
    return hFontChar;
#endif
	
}

bool xFT2Font::drawChar(xWCharType _chr , float x , float y, int& dx , int& dy, const xColor_4f& cl)
{
	HXFT2FontChar hFontChar = GetFontCharHandle(_chr) ;
	IBaseTexture* pTexture = NULL;

    dx = dy = 0;
	if(_chr != 32)
	{
		int char_w = 0;
		int char_h = 0;

#ifndef _FONT_FULL_TEXTURE_
		pTexture  = hFontChar->m_pTexture;

		if(pTexture == NULL)
			return false;

		char_w = hFontChar->m_tex_w;
		char_h = hFontChar->m_tex_h;
#else
		pTexture  = m_pFontCharMgr->m_pTexture;

		if(pTexture == NULL)
			return false;

		

		char_w = hFontChar->m_tex_w;
		char_h = hFontChar->m_tex_h;

#endif

		pTexture->validate();
		m_pRendeDevice->setFontFilter(m_Filter);
		float f_x = x + hFontChar->m_left ; 
		float f_y = y - hFontChar->m_top  ; //y + (hFontChar->m_top  - dim.m_ImageHeight) * m_scale_h ;

		float destRect[4] = {f_x, f_y , (float)char_w , (float)char_h} ;
#ifndef _FONT_FULL_TEXTURE_
		m_pRendeDevice->drawRectf( pTexture,destRect , cl);
#else
		float srcRect[4] = { (float)hFontChar->m_tex_x , (float)hFontChar->m_tex_y , (float)hFontChar->m_tex_w , (float)hFontChar->m_tex_h } ;
		m_pRendeDevice->drawRectf( pTexture,destRect , srcRect, cl);
#endif
	}

	dx = int(hFontChar->m_adv_x ) + 2; 
	dy = int(hFontChar->m_adv_y ) ;
	return true;
}



END_NAMESPACE_XEVOL3D
