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

#ifndef __GDI_FONT_H__
#define __GDI_FONT_H__
#include "xFontRender.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#define _FT2_FULL_TEXTURE_

BEGIN_NAMESPACE_XEVOL3D

class   xGdiFontCharLoader;
class   xGdiFontChar;
typedef TResHandle <xGdiFontChar, wchar_t, xGdiFontCharLoader >  HXGdiFontChar;
typedef TLRUPolicyResMgr<xGdiFontChar , wchar_t , xGdiFontCharLoader >  xGdiFontCharMgr;

class xGdiFTIdxMgr
{
	vector<int> m_vFreeIndex;
	int         m_maxIndex;
public:
	void setMaxIndex(int maxIndex);
	void freeAllIndex();
	int  useIndex();
	void freeIndex(int index);
};
class xGdiFont;
class xGdiFontChar
{
public:
	int           m_tex_w ;
	int           m_tex_h ;

#ifndef _FT2_FULL_TEXTURE_
	IBaseTexture* m_pTexture;
#else
	int   m_tex_x ;
	int   m_tex_y ;
	int   m_tex_idx;
	xGdiFontCharLoader* m_FT2FontLoader;
#endif

	float         m_adv_x;
	float         m_adv_y;
	float         m_left;
	float         m_top;

public:

	xGdiFontChar(IFontRenderDevice* pRenderer , xGdiFontCharLoader* pFT2FontLoader);
	~xGdiFontChar();
	void unload();
};

class xGdiFontCharLoader : public IRefCountObject
{

public:
	FT_Face             m_FT_Face;
	int                 m_w , m_h;
	bool                m_bBold;
	bool                m_Italic;
	bool                m_UnderLine;
	bool                m_bAntilias;
	IFontRenderDevice*  m_pRenderer;

#ifdef _FT2_FULL_TEXTURE_
	IBaseTexture*     m_pTexture;
	xGdiFTIdxMgr  m_idxManager;
	int               m_nCharOfRow;
	int               m_tex_w;
	int               m_tex_h;
#endif

#ifdef _DEBUG
	HXGdiFontChar*      m_hFontList;
#endif
	IMPL_REFCOUNT_OBJECT_INTERFACE(xGdiFontCharLoader);
public:
	static void releaseAll();
	xGdiFontCharLoader();
	virtual     ~xGdiFontCharLoader();
	void        setCacheSize(int maxSize, int maxFontW , int maxFontH);
	void        setRenderer(IFontRenderDevice*    pRenderer) { m_pRenderer = pRenderer ; };
	bool        load_font(const wchar_t* font_file , int _w , int _h);

	unsigned int _getResSize(xGdiFontChar*& pRes);
	bool         _isResLoaded(xGdiFontChar* pRes);
	bool         _loadResource(wchar_t& _char , xGdiFontChar*& pRes, int& ResSize,unsigned int arg);
	bool         _unloadResource(wchar_t& _char , xGdiFontChar*& pRes, unsigned int& TotalResSize);
	void         _deleteResource(wchar_t& _char , xGdiFontChar* pRes);
};



class xGdiFont : public xFontRender
{
	xGdiFontCharMgr*  m_pFontCharMgr;
public:
    xFontInfo         m_Info;
	int               m_LinePitch;
    bool              m_bLinearFilter;
	eFontFilter       m_Filter;
public:
    IMPL_BASE_OBJECT_INTERFACE(xGdiFont);
	HXGdiFontChar    GetFontCharHandle(wchar_t _chr);
public:
	void             createFontCharManager(const wchar_t* managerName);
	void             setFontChareManager(xGdiFontCharMgr* pMgr);
	xGdiFontCharMgr* getFontCharManager();
	xGdiFont(IFontRenderDevice* pRenderer);
	~xGdiFont();
	void  releaseChache();
    const xFontInfo& getInfo(){ return m_Info ; }
	void  setDrawFilter(eFontFilter filter){m_Filter = filter ; }
	bool  drawChar(wchar_t _chr , float x , float y, int& dx , int& dy, xColor_4f cl) ;
	void  enableAntialias(bool bAnti);
	bool  isAntialias();
    bool  getCharDim(wchar_t _chr , int& dx , int& dy);
	void  setCacheSize(int maxSize);
	int   getCacheSize();
	int   getLinePitch(){return 0; m_LinePitch ; };
};

END_NAMESPACE_XEVOL3D
#endif
