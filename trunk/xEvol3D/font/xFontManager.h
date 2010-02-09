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

#ifndef __FONT_MANAGE_H__
#define __FONT_MANAGE_H__
#include "xFontRender.h"
#include "../BaseLib/xXmlDocument.h"
#include <map>

BEGIN_NAMESPACE_XEVOL3D
enum FontType
{
	eFT_Bitmap,
	eFT_Truetype,
	eFT_DistFieldFont,
};


struct xFontInitor
{
   int         height ;//="36"
   int         width ;//="36" 
   bool        bold;//"false" 
   bool        italic;//"false" 
   bool        underline;//"false" 
   eFontFilter filter;//"nearest" 
   bool        antialias;//"false" 
   int         maxcache;//"1024"/
   int         linepitch;
};

class   _XEVOL_BASE_API_ xFontLoader;
typedef TResHandle<xFontRender    , std::ds_wstring , xFontLoader> HFontRender;
typedef TSimpleResMgr<xFontRender , std::ds_wstring , xFontLoader> xFontManager;

class   _XEVOL_BASE_API_ xFontLoader
{
	IFontRenderDevice*                    m_pRendeDevice;
public:
	virtual ~xFontLoader();
	xFontLoader();
	IFontRenderDevice* fontRenderDevice();
    void               setFontRenderDevice(IFontRenderDevice* pDevice);

	HFontRender        findFont(const wchar_t* familly, const wchar_t* name, bool loadImm);
    HFontRender        findFont(const wchar_t* name, bool loadImm);

	bool               load(const wchar_t* cfgFile);
	bool               load(xXmlNode* pXMLNode);

	bool               clearFonts();

	void               extractFonts( xXmlNode* pNode,  xXmlNode* pDestRootNode );
	void               beginFontRender();
	void               endFontRender();

protected:
	unsigned int       _getResSize(xFontRender* pRes);
	bool               _isResLoaded(xFontRender* pRes);
	bool               _preLoadResource(const std::ds_wstring& strResName   , xFontRender* & pRes , int& ResSize, unsigned int arg)   { return true ; };
	bool               _postLoadResource(const std::ds_wstring& strResName   , xFontRender* & pRes , int& ResSize, unsigned int arg)  { return true ; };

	bool               _loadResource(const std::ds_wstring& strResName   , xFontRender* & pRes , int& ResSize, unsigned int arg);
	bool               _unloadResource(const std::ds_wstring& strResName , xFontRender* & pRes , unsigned int& TotalResSize);
	void               _deleteResource(const std::ds_wstring& strResName , xFontRender* pRes);
public:
	static eFontFilter getFontSamplerFilter(const wchar_t* filter_name);
	static void        splitFontName(const wchar_t* fullName , wchar_t* _font_familly , wchar_t* _font_name);
    static void        makeFontName(const wchar_t* famillyName , const std::ds_wstring& fontName , std::ds_wstring& _outName);
protected:
	xXmlDocument            m_FontXML;
	xXmlDocument            m_FontFamillyXML;
	typedef ds_map(int , xXmlNode*) FontNodeMap;
	FontNodeMap             m_FontNodeMap;
};

END_NAMESPACE_XEVOL3D

#endif

