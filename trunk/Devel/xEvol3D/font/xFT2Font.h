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

#ifndef __FT2_FONT_H__
#define __FT2_FONT_H__
#include "xFontRender.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#define _FT2_FULL_TEXTURE_

BEGIN_NAMESPACE_XEVOL3D

class   xFT2FontCharLoader;
class   xFT2FontChar;
typedef TResHandle <xFT2FontChar, wchar_t, xFT2FontCharLoader >  HXFT2FontChar;
typedef TLRUPolicyResMgr<xFT2FontChar , wchar_t , xFT2FontCharLoader >  xFT2FontCharMgr;

class xFT2IndexManager
{
	vector<int> m_vFreeIndex;
	int         m_maxIndex;
public:
	void setMaxIndex(int maxIndex);
	void freeAllIndex();
	int  useIndex();
	void freeIndex(int index);
};
class xFT2Font;
class xFT2FontChar
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
	xFT2FontCharLoader* m_FT2FontLoader;
#endif

	float         m_adv_x;
	float         m_adv_y;
	float         m_left;
	float         m_top;

public:

	xFT2FontChar(IFontRenderDevice* pRenderer , xFT2FontCharLoader* pFT2FontLoader);
	~xFT2FontChar();
	void unload();
};

class xFT2FontCharLoader : public IRefCountObject
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
	xFT2IndexManager  m_idxManager;
	int               m_nCharOfRow;
	int               m_tex_w;
	int               m_tex_h;
#endif

#ifdef _DEBUG
	HXFT2FontChar*      m_hFontList;
#endif
	IMPL_REFCOUNT_OBJECT_INTERFACE(xFT2FontCharLoader);
public:
	static void releaseAll();
	xFT2FontCharLoader();
	virtual     ~xFT2FontCharLoader();
	void        setCacheSize(int maxSize, int maxFontW , int maxFontH);
	void        setRenderer(IFontRenderDevice*    pRenderer) { m_pRenderer = pRenderer ; };
	bool        load_font(const wchar_t* font_file , int _w , int _h);

	unsigned int _getResSize(xFT2FontChar*& pRes);
	bool         _isResLoaded(xFT2FontChar* pRes);
	bool         _loadResource(wchar_t& _char , xFT2FontChar*& pRes, int& ResSize,unsigned int arg);
	bool         _unloadResource(wchar_t& _char , xFT2FontChar*& pRes, unsigned int& TotalResSize);
	void         _deleteResource(wchar_t& _char , xFT2FontChar* pRes);
};



class xFT2Font : public xFontRender
{
	xFT2FontCharMgr*  m_pFontCharMgr;
public:
    xFontInfo         m_Info;
	int               m_LinePitch;
    bool              m_bLinearFilter;
	eFontFilter       m_Filter;
public:
	HXFT2FontChar GetFontCharHandle(wchar_t _chr);
public:
	void             createFontCharManager(const wchar_t* managerName);
	void             setFontChareManager(xFT2FontCharMgr* pMgr);
	xFT2FontCharMgr* getFontCharManager();
	xFT2Font(IFontRenderDevice* pRenderer);
	~xFT2Font();
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
