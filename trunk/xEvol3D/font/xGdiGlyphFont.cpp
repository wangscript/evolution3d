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
#include "xGdiGlyphFont.h"
#include "xFontRender.h"
#include "../BaseLib/xI18N.h"

//#define  new XNEW_OP

BEGIN_NAMESPACE_XEVOL3D

void xGdiGlyphIndexManager::setMaxIndex(int maxIndex)
{
	m_maxIndex = maxIndex;
	m_vFreeIndex.resize(m_maxIndex);
	for(int i = 0  ; i < m_maxIndex ; i ++)
	{
		m_vFreeIndex.push_back(i);
	}
}
void xGdiGlyphIndexManager::freeAllIndex()
{
	setMaxIndex(m_maxIndex );
}

int  xGdiGlyphIndexManager::useIndex()
{
	size_t idxSize = m_vFreeIndex.size(); 
	if( idxSize == 0)
		return -1;
	int idx = m_vFreeIndex[ idxSize - 1];
	m_vFreeIndex.pop_back();
	return idx;
}

void xGdiGlyphIndexManager::freeIndex(int index)
{
	m_vFreeIndex.push_back(index);
}



xGdiGlyphFontChar::xGdiGlyphFontChar(IFontRenderDevice* pRenderer  , xGdiGlyphFontCharLoader* pFT2FontLoader)
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

xGdiGlyphFontChar::~xGdiGlyphFontChar()
{
#ifndef _FONT_FULL_TEXTURE_
	m_pTexture->KillObject();
#endif
}
void xGdiGlyphFontChar::unload()
{
#ifndef _FONT_FULL_TEXTURE_
	m_pTexture->unload();
#else
	m_FT2FontLoader->m_idxManager.freeIndex(m_tex_idx);
#endif
}

static vector<xGdiGlyphFontCharLoader*> gs_AllManager;
void xGdiGlyphFontCharLoader::releaseAll()
{
	vector<xGdiGlyphFontCharLoader*>::iterator pos ;
	while(gs_AllManager.size() > 0)
	{
		pos = gs_AllManager.begin();
		xGdiGlyphFontCharLoader* pLoader = *pos;
		delete pLoader;
	}
	gs_AllManager.clear();
}

xGdiGlyphFontCharLoader::xGdiGlyphFontCharLoader()
{
	gs_AllManager.push_back(this);
#ifdef _DEBUG
	m_hFontList = new HXGdiGlyphFontChar[65536];
#endif
	m_w = 32;
	m_h = 32;
#ifdef _FONT_FULL_TEXTURE_
	m_pTexture = NULL;
#endif
}

xGdiGlyphFontCharLoader::~xGdiGlyphFontCharLoader()
{
	vector<xGdiGlyphFontCharLoader*>::iterator pos = gs_AllManager.begin();
	for(; pos != gs_AllManager.end() ; ++pos)
	{
		xGdiGlyphFontCharLoader* pLoader = *pos;
		if(pLoader == this)
		{
		    gs_AllManager.erase(pos);
			break;
		}
	}
	xGdiGlyphFontCharMgr* pResMgr = dynamic_cast<xGdiGlyphFontCharMgr*>(this);
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


    DeleteObject(m_hGdiFont);
    DeleteDC(m_hFontDC);
}

bool xGdiGlyphFontCharLoader::load_font(const wchar_t* pFont , xFontInfo& info)
{

	//加载一个字体,取默认的Face,一般为Regualer

    HWND hDesktop = ::GetActiveWindow();
    HDC hDesktopDC = GetDC(hDesktop);
    m_hFontDC = ::CreateCompatibleDC( hDesktopDC );
    ReleaseDC(hDesktop , hDesktopDC);
	m_hGdiFont = NULL;
	if(pFont == NULL)// && wcslen(pFont) > 0)
	{
		pFont = L"";
	}
	LOGFONT lf;
	memset(&lf,0,sizeof(LOGFONT));
	lf.lfHeight = -MulDiv(info.nHeight, GetDeviceCaps(m_hFontDC, LOGPIXELSY), 96);
	lf.lfItalic=info.bItalic;
	if(info.bBold) lf.lfWeight = FW_BOLD;
	lf.lfUnderline = info.bUnderLine;
	lf.lfCharSet = DEFAULT_CHARSET;

	wcscpy(lf.lfFaceName, pFont );

	m_hGdiFont = ::CreateFontIndirect(&lf);
	::SelectObject(m_hFontDC , m_hGdiFont);
	
	return true;
}

unsigned int xGdiGlyphFontCharLoader::_getResSize(xGdiGlyphFontChar*& pRes) 
{
	return 1;
}

bool xGdiGlyphFontCharLoader::_isResLoaded(xGdiGlyphFontChar* pRes)
{
	return pRes != NULL;
}
#define FONTSIZE_SCALE 1

/*
以下演示如何获取线条数据
DWORD returnBytes = GetGlyphOutlineW(dc.GetSafeHdc(), thisChar, GGO_BEZIER, &gm, sizeof(buffer), buffer, &mat2);
if(returnBytes != GDI_ERROR)
{
    TTPOLYGONHEADER* pTTPH = (TTPOLYGONHEADER*)buffer;
    _ASSERTE(pTTPH->dwType == TT_POLYGON_TYPE);
    //对闭合路径进行循环
    while(returnBytes > 0)
    {
        dc.MoveTo(pTTPH->pfxStart.x.value, pTTPH->pfxStart.y.value);
        TTPOLYCURVE* pCurrentCurve = (TTPOLYCURVE*)(pTTPH+1);
        int remainBytes = pTTPH->cb - sizeof(TTPOLYGONHEADER);
        while(remainBytes > 0)
        {
            switch(pCurrentCurve->wType)
            {
            case TT_PRIM_LINE:
            case TT_PRIM_QSPLINE:
            case TT_PRIM_CSPLINE:
                {
                    for(int i=0; i<pCurrentCurve->cpfx; ++i)
                    {
                        dc.LineTo(pCurrentCurve->apfx[i].x.value, pCurrentCurve->apfx[i].y.value);
                    }
                }
                break;
            }
            int count = sizeof(TTPOLYCURVE) + (pCurrentCurve->cpfx-1)*sizeof(POINTFX);
            pCurrentCurve = (TTPOLYCURVE*)((char*)pCurrentCurve + count);
            remainBytes -= count;
        }
        dc.LineTo(pTTPH->pfxStart.x.value, pTTPH->pfxStart.y.value);

        returnBytes -= pTTPH->cb;
        pTTPH = (TTPOLYGONHEADER*)((char*)pTTPH + pTTPH->cb);
    }
}
*/

bool xGdiGlyphFontCharLoader::_loadResource(wchar_t& _char , xGdiGlyphFontChar*& pRes, int& ResSize,unsigned int arg)
{
	//if(FT_Load_Glyph( m_FT_Face, FT_Get_Char_Index( m_FT_Face, _char ), FT_LOAD_DEFAULT ))
	//	throw std::runtime_error("FT_Load_Glyph failed");

    UINT  uFormat = GGO_GRAY8_BITMAP;
    GLYPHMETRICS GlyphMetrics;
    MAT2 glyphMat = {{0,1},{0,0},{0,0},{0,1}};;

    if(GDI_ERROR == GetGlyphOutline(m_hFontDC , _char , GGO_METRICS  , &GlyphMetrics , 0 , NULL , &glyphMat ) )
        return false;

    int BufferLen = GetGlyphOutline(m_hFontDC , _char , uFormat , &GlyphMetrics , 0 , 0 , &glyphMat);
    char* imageBuffer = new char[BufferLen] ;
    
    GetGlyphOutline(m_hFontDC , _char , uFormat , &GlyphMetrics , BufferLen , imageBuffer , &glyphMat);

	if(pRes == NULL)
	{
		pRes = new xGdiGlyphFontChar(m_pRenderer ,  this );
		if(pRes == NULL)
			return false;
	}

	//取道位图数据
	//把位图数据拷贝自己定义的数据区里.这样旧可以画到需要的东西上面了。
	int width  =  GlyphMetrics.gmBlackBoxX;
	int height =  GlyphMetrics.gmBlackBoxY;
    int bitmap_width  = width;
    int bitmap_height = height;
    int bitmap_pitch  = (GlyphMetrics.gmBlackBoxX + 3) & ~3;

	pRes->m_adv_x = (float)GlyphMetrics.gmCellIncX;
	pRes->m_adv_y = (float)(GlyphMetrics.gmCellIncY + GlyphMetrics.gmBlackBoxY);
	pRes->m_left  = (float)GlyphMetrics.gmptGlyphOrigin.x;
	pRes->m_top   = (float)GlyphMetrics.gmptGlyphOrigin.y;

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
	pTexture->lock(eLock_ReadWrite , lockInfo);
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
	if( uFormat == GGO_GRAY8_BITMAP )
	{
        bitmap_pitch  = (GlyphMetrics.gmBlackBoxX + 3) & ~3;;
        BYTE * lineSrc = (BYTE *)imageBuffer;

		for(int y=0; y  < height ; y++)
		{
			for(int x=0; x < width; x++)
			{
				int _y = y;
				unsigned int _vl =  0;
				if(x < bitmap_width && y < bitmap_height) 
				{
					BYTE Value = lineSrc[x];
                    _vl = (int)(Value / 64.0f * 255);
                    if(Value > 0)
                    {
                        _vl = (int)(grayScale * _vl);
                        if(_vl > 255) 
                            _vl = 255;
                    }


				}
				//if(_vl > 25 && _vl < 220) _vl = 220;


				if(pTexture->format() == PIXELFORMAT_B8G8R8A8)
				{

					tex_pixel[(4*x + _y * tex_pitch)+0] = 0xff;
					tex_pixel[(4*x + _y * tex_pitch)+1] = 0xff;
					tex_pixel[(4*x + _y * tex_pitch)+2] = 0xff;
					tex_pixel[(4*x + _y * tex_pitch)+3] = (unsigned char)_vl;
				}
				else
				{
					tex_pixel[(1*x + _y * tex_pitch)+0] = (unsigned char)_vl;
				}				
			}

            lineSrc += bitmap_pitch;
		}
	}
	else if(uFormat == GGO_BITMAP) //单色图
	{
        int dwOneLine = (bitmap_width/8 + 3) / 4 * 4;
		for(int y=0; y  < height ; y++)
		{
			for(int x=0; x < width; x++)
			{
				int _y = y;
				unsigned char _vl =  0;
                BYTE * pData = (BYTE*)imageBuffer + dwOneLine*y + x/8;

                if(x < bitmap_width && y < bitmap_height) 
				{
                     BYTE BitMask = (1 << 7-(x%8)); 
                     BOOL bBit =  *pData & BitMask;
					_vl =   bBit ? 0xFFFFFFFF : 0x00000000;
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
	ResSize = 1;
	return true;
}

bool xGdiGlyphFontCharLoader::_unloadResource(wchar_t& _char , xGdiGlyphFontChar*& pRes, unsigned int& TotalResSize)
{
	pRes->unload();
	TotalResSize -= 1;
	return true;
}

void xGdiGlyphFontCharLoader::_deleteResource(wchar_t& _char , xGdiGlyphFontChar* pRes)
{
	delete pRes;
	pRes = NULL;
}

void xGdiGlyphFontCharLoader::setCacheSize(int maxSize , int maxFontW , int maxFontH)
{

#ifdef _FONT_FULL_TEXTURE_
	m_nCharOfRow = (int) ( sqrt( (float)maxSize ) );
	
	int tex_w = m_nCharOfRow * ( m_w * FONTSIZE_SCALE);
	int tex_h = m_nCharOfRow * ( m_h * FONTSIZE_SCALE);
	m_tex_w = tex_w;
	m_tex_h = tex_h;
	if( m_pTexture ) m_pTexture->KillObject();
	m_idxManager.setMaxIndex(  m_nCharOfRow * m_nCharOfRow );
	if( m_pRenderer->isTextureSupport(PIXELFORMAT_ALPHA8) )
	{
		m_pTexture = m_pRenderer->createTexture( tex_w , tex_h , PIXELFORMAT_ALPHA8   , true , RESOURCE_USAGE_DEFAULT);
	}																	 					
	else																 					
	{																	 					
		m_pTexture = m_pRenderer->createTexture(  tex_w , tex_h ,PIXELFORMAT_B8G8R8A8 ,  true , RESOURCE_USAGE_DEFAULT);
	}
	//m_pTexture->setFlipY( !m_pTexture->isFlipY() );
#endif
}
//88888888888888888888888888888888888888888

IMPL_OBJECT_FACTORY(xGdiGlyphFont , xFontRender , xGdiGlyphFontCreator , xFontRenderCreator , xFontRenderCreatorMgr , L"gdiglyph", L"gdi font" , xFontLoader* , int );

void  xGdiGlyphFont::releaseChache()
{
	m_pFontCharMgr->unload();
}

void xGdiGlyphFont::setCacheSize(int maxSize)
{
	m_pFontCharMgr->setCacheSize(maxSize , m_Info.nMaxWidth , m_Info.nMaxHeight);
#ifdef _FONT_FULL_TEXTURE_
	 m_pFontCharMgr->unload();
#endif

     m_pFontCharMgr->setMaxSize(maxSize);
}

int  xGdiGlyphFont::getCacheSize()
{
	return m_pFontCharMgr->getMaxSize();
}
IMPL_BASE_OBJECT_CLASSID(xGdiGlyphFont  , xFontRender);
xGdiGlyphFont::xGdiGlyphFont(xFontLoader* pFontLoader , int arg):xFontRender(pFontLoader , arg)
{
	m_RefCount = 1 ;
	m_LinePitch = 5;
	m_Filter = eFontFilter_Point;

}

xGdiGlyphFont::~xGdiGlyphFont()
{
    
}
bool xGdiGlyphFont::init( xXmlNode* pFontNode)
{
	xFontLoader* pFontLoader = m_pFontLoader;

	const wchar_t*  pFontName    = pFontNode->value(L"name");
    const wchar_t*  pFont        = pFontNode->value(L"font");
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


    std::wstring fileName = famillyName;//pFontNode->value(L"fontname") ;

	//因为是个新字体，所以需要做一个FontCharMgr来管理它
	createFontCharManager( fileName.c_str() ) ;
	xGdiGlyphFontCharMgr* pFontCharMgr = getFontCharManager();

	pFontCharMgr->setRenderer(m_pRendeDevice);
	pFontCharMgr->m_UnderLine = fi.underline;
	pFontCharMgr->m_bBold     = fi.bold;
	pFontCharMgr->m_Italic    = fi.italic;
	pFontCharMgr->m_bAntilias = fi.antialias;

	int maxCache = fi.maxcache;
	if(maxCache == 0) maxCache = 1024;


	
	//pFontCharMgr->setMaxSize(maxCache);
	//g_FT2FontCharMgr.push_back(pFontCharMgr);

	//新建一个FreeType2的字体绘制器。并把它和Font


	m_Filter          = fi.filter;
	m_Info.bAntialias = fi.antialias;
	m_Info.bBold      = fi.bold;
	m_Info.bItalic    = fi.italic;
	m_Info.bUnderLine = fi.underline;

	m_Info.nMaxWidth  = fi.width;
	m_Info.nMaxHeight = fi.height;
	wcsncpy(m_Info.strFamilly , famillyName , 32);
	wcsncpy(m_Info.strName    , pFontName,         16);
	m_Info.nHeight =  fi.height;
	m_Info.nWidth  =  fi.width;
	m_LinePitch = fi.linepitch;

	if(pFont == NULL) pFont = pFontName;

    if( pFontCharMgr->load_font(pFont , m_Info) ==false)
    {
        delete pFontCharMgr;
        return NULL; 
    }


    setCacheSize(maxCache );
	return true;
}

void   xGdiGlyphFont::createFontCharManager(const wchar_t* managerName)
{
	m_pFontCharMgr = new xGdiGlyphFontCharMgr(managerName , 1);
}
void   xGdiGlyphFont::setFontChareManager(xGdiGlyphFontCharMgr* pMgr)
{
	m_pFontCharMgr = pMgr;
}
xGdiGlyphFontCharMgr*  xGdiGlyphFont::getFontCharManager()
{
	return m_pFontCharMgr;
}

bool  xGdiGlyphFont::getCharDim(wchar_t _chr , int& dx , int& dy)
{
    HXGdiGlyphFontChar hFontChar = GetFontCharHandle(_chr) ;
	dx = dy = 0;
	dx = int(hFontChar->m_adv_x ) + 2; 
	dy = int(hFontChar->m_adv_y ) ;
	return true;
}

void xGdiGlyphFont::enableAntialias(bool bAnti)
{
	if(m_pFontCharMgr->m_bAntilias == bAnti)
		return ;
	m_pFontCharMgr->unload();
	m_pFontCharMgr->m_bAntilias = true;
}
bool xGdiGlyphFont::isAntialias()
{
	return m_pFontCharMgr->m_bAntilias;
}

HXGdiGlyphFontChar xGdiGlyphFont::GetFontCharHandle(wchar_t _chr)
{
	xGdiGlyphFontCharMgr* FONTCHARMGR = (xGdiGlyphFontCharMgr*)(m_pFontCharMgr);
#ifdef _DEBUG
	HXGdiGlyphFontChar hFontChar = FONTCHARMGR->m_hFontList[_chr];
	if(hFontChar.isHandle() == false)
	{
		hFontChar = FONTCHARMGR->add(_chr , (unsigned int) this,  true);
		FONTCHARMGR->m_hFontList[_chr] = hFontChar;
	}    
#else
	//Debug已经吧句柄保存起来，不存在RefCount为0的情况。
	HXGdiGlyphFontChar hFontChar = FONTCHARMGR->add(_chr , (unsigned int) this , true);
	if( hFontChar.RefCount() == 1)
	{
		hFontChar.AddRef();
	}
#endif
	return hFontChar;
}

bool xGdiGlyphFont::drawChar(wchar_t _chr , float x , float y, int& dx , int& dy, const xColor_4f& cl)
{
	HXGdiGlyphFontChar hFontChar = GetFontCharHandle(_chr) ;
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
