/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) .
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

#ifndef __GDIGLYPH_FONT_H__
#define __GDIGLYPH_FONT_H__
#include <Windows.h>
#include "xFontRender.h"
#include "xFontManager.h"
#define _FONT_FULL_TEXTURE_

BEGIN_NAMESPACE_XEVOL3D

class   xGdiGlyphFontCharLoader;
class   xGdiGlyphFontChar;
typedef TResHandle <xGdiGlyphFontChar, wchar_t, xGdiGlyphFontCharLoader >  HXGdiGlyphFontChar;
typedef TLRUPolicyResMgr<xGdiGlyphFontChar , wchar_t , xGdiGlyphFontCharLoader >  xGdiGlyphFontCharMgr;

class xGdiGlyphIndexManager
{
	vector<int> m_vFreeIndex;
	int         m_maxIndex;
public:
	void setMaxIndex(int maxIndex);
	void freeAllIndex();
	int  useIndex();
	void freeIndex(int index);
};

class xGdiGlyphFont;
class xGdiGlyphFontChar
{
public:
	int           m_tex_w ;
	int           m_tex_h ;

#ifndef _FONT_FULL_TEXTURE_
	IBaseTexture* m_pTexture;
#else
	int   m_tex_x ;
	int   m_tex_y ;
	int   m_tex_idx;
	xGdiGlyphFontCharLoader* m_FT2FontLoader;
#endif

	float         m_adv_x;
	float         m_adv_y;
	float         m_left;
	float         m_top;

public:

	xGdiGlyphFontChar(IFontRenderDevice* pRenderer , xGdiGlyphFontCharLoader* pFT2FontLoader);
	~xGdiGlyphFontChar();
	void unload();
};

class xGdiGlyphFontCharLoader : public IRefCountObject
{

public:

	int                 m_w , m_h;
	bool                m_bBold;
	bool                m_Italic;
	bool                m_UnderLine;
	bool                m_bAntilias;
	IFontRenderDevice*  m_pRenderer;
    HDC                 m_hFontDC;
    HFONT               m_hGdiFont;

#ifdef _FONT_FULL_TEXTURE_
	IBaseTexture*          m_pTexture;
	xGdiGlyphIndexManager  m_idxManager;
	int                    m_nCharOfRow;
	int                    m_tex_w;
	int                    m_tex_h;
#endif

#ifdef _DEBUG
	HXGdiGlyphFontChar*      m_hFontList;
#endif
	IMPL_REFCOUNT_OBJECT_INTERFACE(xGdiGlyphFontCharLoader);
public:
	static void releaseAll();
	xGdiGlyphFontCharLoader();
	virtual     ~xGdiGlyphFontCharLoader();
	void        setCacheSize(int maxSize, int maxFontW , int maxFontH);
	void        setRenderer(IFontRenderDevice*    pRenderer) { m_pRenderer = pRenderer ; };
	bool        load_font(const wchar_t* pFont ,xFontInfo& info);

	unsigned int _getResSize(xGdiGlyphFontChar*& pRes);
	bool         _isResLoaded(xGdiGlyphFontChar* pRes);
	bool         _postLoadResource(wchar_t& _char , xGdiGlyphFontChar*& pRes, int& ResSize,unsigned int arg) { return true ; }
	bool         _preLoadResource(wchar_t& _char , xGdiGlyphFontChar*& pRes, int& ResSize,unsigned int arg)  { return true ; }
	bool         _loadResource(wchar_t& _char , xGdiGlyphFontChar*& pRes, int& ResSize,unsigned int arg);
	bool         _unloadResource(wchar_t& _char , xGdiGlyphFontChar*& pRes, unsigned int& TotalResSize);
	void         _deleteResource(wchar_t& _char , xGdiGlyphFontChar* pRes);
};



class xGdiGlyphFont : public xFontRender
{
	xGdiGlyphFontCharMgr*  m_pFontCharMgr;
public:
    xFontInfo         m_Info;
	int               m_LinePitch;
    bool              m_bLinearFilter;
	eFontFilter       m_Filter;
public:
    IMPL_BASE_OBJECT_INTERFACE(xGdiGlyphFont);
	HXGdiGlyphFontChar    GetFontCharHandle(wchar_t _chr);
public:
	void             createFontCharManager(const wchar_t* managerName);
	void             setFontChareManager(xGdiGlyphFontCharMgr* pMgr);
	xGdiGlyphFontCharMgr* getFontCharManager();
	xGdiGlyphFont(xFontLoader* pFontLoader , int arg);
	~xGdiGlyphFont();
	bool init( xXmlNode* pInitXml);
	void  releaseChache();
    const xFontInfo& getInfo(){ return m_Info ; }
	void  setDrawFilter(eFontFilter filter){m_Filter = filter ; }
	bool  drawChar(wchar_t _chr , float x , float y, int& dx , int& dy, const xColor_4f& cl) ;
	void  enableAntialias(bool bAnti);
	bool  isAntialias();
    bool  getCharDim(wchar_t _chr , int& dx , int& dy);
	void  setCacheSize(int maxSize);
	int   getCacheSize();
	int   getLinePitch(){return 0; m_LinePitch ; };
	int   getResSize(){ return 1 ; };
};

END_NAMESPACE_XEVOL3D
#endif
