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
#include "../RenderAPI/xRenderAPI.h"
#include "../RenderAPI/xBaseTexture.h"

#include "xFT2Font.h"
#include "xFontRender.h"
#include "../BaseLib/xI18N.h"
#ifdef _WIN32
#ifndef _WIN32_WCE
#pragma comment(lib , "freetype.lib")
#endif
#endif

//#define  new XNEW_OP

BEGIN_NAMESPACE_XEVOL3D


static FT_Library g_FT2Lib;

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
#ifndef _FT2_FULL_TEXTURE_
	m_pTexture = NULL;
	//m_pTexture->setFlipY( !m_pTexture->isFlipY() );
#else
	m_FT2FontLoader = pFT2FontLoader;
#endif
}

xFT2FontChar::~xFT2FontChar()
{
#ifndef _FT2_FULL_TEXTURE_
	m_pTexture->KillObject();
#endif
}
void xFT2FontChar::unload()
{
#ifndef _FT2_FULL_TEXTURE_
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
#ifdef _FT2_FULL_TEXTURE_
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
	xFT2FontCharMgr* pResMgr = dynamic_cast<xFT2FontCharMgr*>(this);
	if(pResMgr) pResMgr->clear();
#ifdef _DEBUG
	delete [] m_hFontList;
#endif

#ifdef _FT2_FULL_TEXTURE_
	if ( m_pTexture )
	{
		m_pTexture->KillObject();
		m_pTexture = NULL;
	}
#endif

}

bool xFT2FontCharLoader::load_font(const wchar_t* font_file , int _w , int _h)
{
	char strFontFile[1024] = {0};
	XEvol_UnicodeToLocale(font_file,strFontFile,1024);
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
bool xFT2FontCharLoader::_loadResource(wchar_t& _char , xFT2FontChar*& pRes, int& ResSize,unsigned int arg)
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
#ifndef _FT2_FULL_TEXTURE_
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
		pRes->m_pTexture = m_pRenderer->createLockableTexture(  width,height, PIXELFORMAT_ALPHA8   , false);//Alpha8);
	}																		 						
	else																	 						
	{																		 						
		pRes->m_pTexture = m_pRenderer->createLockableTexture(  width,height, PIXELFORMAT_B8G8R8A8 , false);//RGBA);
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

bool xFT2FontCharLoader::_unloadResource(wchar_t& _char , xFT2FontChar*& pRes, unsigned int& TotalResSize)
{
	pRes->unload();
	TotalResSize -= 1;
	return true;
}

void xFT2FontCharLoader::_deleteResource(wchar_t& _char , xFT2FontChar* pRes)
{
	delete pRes;
	pRes = NULL;
}

void xFT2FontCharLoader::setCacheSize(int maxSize , int maxFontW , int maxFontH)
{

#ifdef _FT2_FULL_TEXTURE_
	m_nCharOfRow = (int) ( sqrt( (float)maxSize ) );
	
	int tex_w = m_nCharOfRow * ( m_w * FONTSIZE_SCALE);
	int tex_h = m_nCharOfRow * ( m_h * FONTSIZE_SCALE);
	m_tex_w = tex_w;
	m_tex_h = tex_h;
	if( m_pTexture ) m_pTexture->KillObject();
	m_idxManager.setMaxIndex(  m_nCharOfRow * m_nCharOfRow );
	if( m_pRenderer->isTextureSupport(PIXELFORMAT_ALPHA8) )
	{
		m_pTexture = m_pRenderer->createLockableTexture( tex_w , tex_h , PIXELFORMAT_ALPHA8   , true);
	}																	 					
	else																 					
	{																	 					
		m_pTexture = m_pRenderer->createLockableTexture(  tex_w , tex_h ,PIXELFORMAT_B8G8R8A8 ,  true);
	}
	//m_pTexture->setFlipY( !m_pTexture->isFlipY() );
#endif
}
//88888888888888888888888888888888888888888
void  xFT2Font::releaseChache()
{
	m_pFontCharMgr->unload();
}

void xFT2Font::setCacheSize(int maxSize)
{
	m_pFontCharMgr->setCacheSize(maxSize , m_Info.nMaxWidth , m_Info.nMaxHeight);
#ifdef _FT2_FULL_TEXTURE_
	 m_pFontCharMgr->unload();
#endif

     m_pFontCharMgr->setMaxSize(maxSize);
}

int  xFT2Font::getCacheSize()
{
	return m_pFontCharMgr->getMaxSize();
}

xFT2Font::xFT2Font(IFontRenderDevice* pRenderer):xFontRender(pRenderer)
{
	
	m_LinePitch = 5;
	m_Filter = eFontFilter_Point;
}

xFT2Font::~xFT2Font()
{
    
}

void   xFT2Font::createFontCharManager(const wchar_t* managerName)
{
	m_pFontCharMgr = new xFT2FontCharMgr(managerName , 1);
}
void   xFT2Font::setFontChareManager(xFT2FontCharMgr* pMgr)
{
	m_pFontCharMgr = pMgr;
}
xFT2FontCharMgr*  xFT2Font::getFontCharManager()
{
	return m_pFontCharMgr;
}

bool  xFT2Font::getCharDim(wchar_t _chr , int& dx , int& dy)
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

HXFT2FontChar xFT2Font::GetFontCharHandle(wchar_t _chr)
{
	xFT2FontCharMgr* FONTCHARMGR = (xFT2FontCharMgr*)(m_pFontCharMgr);
#ifdef _DEBUG
	HXFT2FontChar hFontChar = FONTCHARMGR->m_hFontList[_chr];
	if(hFontChar.isHandle() == false)
	{
		hFontChar = FONTCHARMGR->add(_chr , (unsigned int) this,  true);
		FONTCHARMGR->m_hFontList[_chr] = hFontChar;
	}    
#else
	//Debug已经吧句柄保存起来，不存在RefCount为0的情况。
	HXFT2FontChar hFontChar = FONTCHARMGR->add(_chr , (unsigned int) this , true);
	if( hFontChar.refCount() == 1)
	{
		hFontChar.addRef();
	}
#endif
	return hFontChar;
}

bool xFT2Font::drawChar(wchar_t _chr , float x , float y, int& dx , int& dy, xColor_4f cl)
{
	HXFT2FontChar hFontChar = GetFontCharHandle(_chr) ;
	IBaseTexture* pTexture = NULL;

    dx = dy = 0;
	if(_chr != 32)
	{
		int char_w = 0;
		int char_h = 0;

#ifndef _FT2_FULL_TEXTURE_
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
#ifndef _FT2_FULL_TEXTURE_
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
