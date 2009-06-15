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

struct xFontFamilly
{
    wchar_t   name[32];
	wchar_t   fontFile[256];
	FontType  fonttype;
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
	typedef std::map<int , xFontFamilly*> FONTYTPEMAP;
	FONTYTPEMAP                           m_FontTypes;
	IFontRenderDevice*                    m_pRendeDevice;

public:
	virtual ~xFontLoader();
	xFontLoader();
    void               setFontRenderDevice(IFontRenderDevice* pDevice);
	xFontFamilly*      insertFamilly(const wchar_t* name , const wchar_t* fontFileName, FontType fontType);
    xFontFamilly*      findFamilly(const wchar_t* name);

	xFontRender*       insertTrueType(const wchar_t* familly, const wchar_t* name , xFontInitor* pFontInitor);
	xFontRender*       insertTrueType(xFontFamilly* familly , const wchar_t* name , xFontInitor* pFontInitor);
	xFontRender*       insertMixed(const wchar_t* name ,  const wchar_t* unicode,const wchar_t* ascii,eFontFilter filter= eFontFilter_Point,int line_pitch= 1 , int maxCache = 0);
	xFontRender*       insertMixedAlias(const wchar_t* name , const wchar_t* link);
	//xFontRender*       insert(const wchar_t* unicode, const wchar_t* ansi);

	HFontRender        findFont(const wchar_t* familly, const wchar_t* name);
    HFontRender        findFont(const wchar_t* name);
	void               unload();
	bool               load(const wchar_t* cfgFile);
	bool               load(xXmlNode* pXMLNode);
	void               beginFontRender();
	void               endFontRender();

protected:
    bool               load_familly(xXmlNode* pNode);
	bool               load_mixed(xXmlNode* pNode);
	
protected:
	unsigned int       _getResSize(xFontRender* pRes);
	bool               _isResLoaded(xFontRender* pRes);
	bool               _loadResource(const std::ds_wstring& strResName   , xFontRender* & pRes , int& ResSize, unsigned int arg);
	bool               _unloadResource(const std::ds_wstring& strResName , xFontRender* & pRes , unsigned int& TotalResSize);
	void               _deleteResource(const std::ds_wstring& strResName , xFontRender* pRes);
protected:
	bool         __load_truetype_familly(xXmlNode* pNode);
	bool         __load_truetype_familly_withascii(xXmlNode* pNode);
};

END_NAMESPACE_XEVOL3D

#endif

