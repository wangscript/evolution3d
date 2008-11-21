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
#ifdef _WIN32
#include <windows.h>
#endif

#include "../RenderAPI/xRenderAPI.h"
#include "../RenderAPI/xBaseTexture.h"
#include "xFontManager.h"
#include "xFT2Font.h"
#include "xMultiFtRender.h"
#include "../OperationSys/xOperationSys.h"
#include "../BaseLib/xStringHash.h"
#include <vector>
#include <string>
using namespace std;


//系统路径
//$FontDir$
BEGIN_NAMESPACE_XEVOL3D

xFontLoader::xFontLoader()
{
	m_pRendeDevice = NULL;
}

void xFontLoader::setFontRenderDevice(IFontRenderDevice* pDevice)
{
	m_pRendeDevice = pDevice;
}

xFontLoader::~xFontLoader()
{
    unload();
}

void   xFontLoader::unload()
{
	xFontManager* pFontMgr  = dynamic_cast<xFontManager*>(this);
	if(pFontMgr == NULL)
		return ;
	pFontMgr->unload();
	
	{
		FONTYTPEMAP::iterator pos = m_FontTypes.begin();
		for(; pos != m_FontTypes.end() ; pos ++)
		{
			delete pos->second;
		}
		m_FontTypes.clear();
	}

}

bool xFontLoader::load(const wchar_t* cfgFile)
{
	xCfgDocument fontCfgDoc;
	if( false == fontCfgDoc.load( cfgFile ) )
		return false;
	
	xCfgNode* font_cfg = fontCfgDoc.root();
	load(font_cfg);
	fontCfgDoc.unload();
	return true;
}

bool xFontLoader::load(xCfgNode* pXMLNode)
{
	xCfgNode*	freeTypeNode = pXMLNode->findNode(L"freetype",0);
	if(freeTypeNode)
	{
		xCfgNode::CfgNodes  familyNodes;
		freeTypeNode->findNode(L"family",familyNodes);
		for(size_t i = 0 ; i < familyNodes.size() ; i ++)
		{
			xCfgNode* pFamillyNode = familyNodes[i];
			load_familly(pFamillyNode);
		}
	}

	xCfgNode::CfgNodes  mixedNodes;
	pXMLNode->findNode(L"mixed",mixedNodes);
	for(size_t i = 0 ; i < mixedNodes.size() ; i ++)
	{
		xCfgNode* pMixedNode = mixedNodes[i];
		load_mixed(pMixedNode);
	}

	return true;
}


HFontRender xFontLoader::findFont(const wchar_t* familly, const wchar_t* name)
{
	std::ds_wstring fontFullName = std::ds_wstring(familly) + L":" + name;
	xFontManager* pFontMgr  = dynamic_cast<xFontManager*>(this);
	return pFontMgr->find( fontFullName.c_str() );
}

HFontRender xFontLoader::findFont(const wchar_t* name)
{
	xFontManager* pFontMgr  = dynamic_cast<xFontManager*>(this);
	return pFontMgr->find( name );
}

void xFontLoader::beginFontRender()
{

}
void xFontLoader::endFontRender()
{

}
/*
从一个配置文件的节点开始加载一组字体
配置文件结构为

<Root/or xCfgDocument>
    <family>
	   <font> </font>
    </family>
</Root>

*/
static eFontFilter __get_sampler_filter(const wchar_t* filter_name)
{
	 if(filter_name == NULL)
		 return eFontFilter_Point;

	 wstring strFilter = filter_name;
     if(wstring(L"nearest") == strFilter || wstring(L"point") == strFilter)
	 {
		 return eFontFilter_Point;
	 }
	 else      if(wstring(L"linear") == strFilter)
	 {
		 return eFontFilter_Linear;
	 }
	 return eFontFilter_Linear;
}

static void __split_font_name(const wchar_t* fullName , wchar_t* _font_familly , wchar_t* _font_name)
{
	wcsncpy(_font_familly,fullName,32);
	int iSplit = 0;
	for(iSplit  = 0 ; iSplit < (int)wcslen(fullName) ; ++iSplit)
	{
		if(_font_familly[iSplit] == ':')
		{
			_font_familly[iSplit] = 0;
			break;
		}
	}
	wcsncpy(_font_name,fullName + iSplit + 1,32);
}


/*
*/
xFontRender* xFontLoader::insertMixedAlias(const wchar_t* pFontName , const wchar_t* pLinkFontName)
{
	HFontRender hMixedFont = findFont(pFontName);
	if( hMixedFont.getResource() )
		return hMixedFont.getResource();

	xFontManager* pFontMgr  = dynamic_cast<xFontManager*>(this);
	HFontRender   hLinkFont = pFontMgr->find(pLinkFontName);
	xFontRender*  pLinkFont = hLinkFont.getResource() ;
	pFontMgr->add(pFontName , pLinkFont , 0 );
	return pLinkFont;
}

xFontRender*  xFontLoader::insertMixed(const wchar_t* pFontName ,  const wchar_t* pUSCFontName,const wchar_t* pAsciiFontName, eFontFilter filter, int line_pitch, int maxCache)
{
	HFontRender hMixedFont = findFont(pFontName);
	if( hMixedFont.getResource() )
		return hMixedFont.getResource();

	//没有ucs字体。错误
	if(pUSCFontName == NULL)
		return NULL;

	wchar_t _font_family[32];
	wchar_t _font_name[32];
	__split_font_name(pUSCFontName , _font_family , _font_name);
	xFontRender* pUCSFont = findFont(_font_family , _font_name).getResource();

	if(pAsciiFontName == NULL) pAsciiFontName = pUSCFontName;

	__split_font_name(pAsciiFontName , _font_family , _font_name);
	xFontRender* pAsciiFont = findFont(_font_family , _font_name).getResource();

	if(pUCSFont == NULL || pAsciiFont == NULL)
	{
		return NULL;
	}

	xMixedFont* pMixedFont = new xMixedFont(m_pRendeDevice);
	pMixedFont->m_pAsciiFontRender = pAsciiFont;
	pMixedFont->m_pUCSFontRender   = pUCSFont;

	if(maxCache > 0)
	{
		pMixedFont->setCacheSize(maxCache );
	}
	pMixedFont->m_Filter = filter;
	pMixedFont->m_LinePitch = line_pitch;


	xFontManager* pFontMgr  = dynamic_cast<xFontManager*>(this);
	pFontMgr->add(pFontName , pMixedFont , 0 ).addRef();
	return pMixedFont;
}

bool   xFontLoader::load_mixed(xCfgNode* pNode)
{
	xCfgNode::CfgNodes  fontNodes;
	pNode->findNode(L"font",fontNodes);
	for(size_t i = 0  ; i < fontNodes.size() ; i++)
	{
		xCfgNode* pFontNode = fontNodes[i];

		const  wchar_t*  pFontName = pFontNode->value(L"name");
		const  wchar_t*  pUSCFontName  = pFontNode->value(L"unicode");
		const  wchar_t*  pAsciiFontName = pFontNode->value(L"ascii");
		const  wchar_t*  pFiltertName   = pFontNode->value(L"filter");
		eFontFilter      filter = __get_sampler_filter(pFiltertName);
		int              line_pitch = 0;
		if(pFontNode->value(L"line_pitch"))
		{
			line_pitch = pFontNode->int_value(L"line_pitch");
		}

		int nMaxCache = pFontNode->int_value(L"maxcache");
		insertMixed(pFontName , pUSCFontName , pAsciiFontName , filter , line_pitch ,nMaxCache);
	}

	xCfgNode::CfgNodes  aliasNodes;
	pNode->findNode(L"alias",aliasNodes);
	//处理link
	for(size_t i = 0  ; i < aliasNodes.size() ; i++)
	{
		xCfgNode* pFontNode = aliasNodes[i];
		const  wchar_t*  pFontName  =  pFontNode->value(L"name");
		const  wchar_t*  pLinkFontName  = pFontNode->value(L"link");
		//没有link字体。错误
		if(pLinkFontName == NULL || pFontName == NULL)
			continue;

	
		insertMixedAlias(pFontName , pLinkFontName);


	}

    return true;
}

/*
加载一种family的字体族
*/
bool   xFontLoader::load_familly(xCfgNode* pFamillyNode)
{
	if(pFamillyNode->findValue(L"type") == NULL)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH,L" Missing font type in xCfg document\n");
		return false;
	}
	if(wstring(L"truetype") == pFamillyNode->value(L"type"))
	{
		return __load_truetype_familly(pFamillyNode);
	}

	return false;
}

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
#ifdef _LINUX
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

xFontFamilly* xFontLoader::insertFamilly(const wchar_t* name , const wchar_t* fontFileName, FontType fontType)
{
	xFontFamilly* pFontFamlly = findFamilly(name);
	if(pFontFamlly) return pFontFamlly;

	pFontFamlly = new xFontFamilly;
	wcsncpy(pFontFamlly->name,name , 32);
	wchar_t fileName[512]={0};
	GetFontFileName(fontFileName,fileName);
	wcsncpy(pFontFamlly->fontFile , fileName , 256);
	pFontFamlly->fonttype = fontType;

	int hashName = xStringHash(name);
	m_FontTypes.insert( FONTYTPEMAP::value_type(hashName , pFontFamlly) );
	return pFontFamlly;
}

xFontFamilly* xFontLoader::findFamilly(const wchar_t* name)
{
	int hashName = xStringHash(name);
	FONTYTPEMAP::iterator pos = m_FontTypes.find(hashName );
	if(pos != m_FontTypes.end() )
		return pos->second;
	return NULL;
}
/*
加载truetype类的字体族
*/
xFontRender* xFontLoader::insertTrueType(xFontFamilly* familly, const wchar_t* pFontName , xFontInitor* pFontInitor)
{

	//因为是个新字体，所以需要做一个FontCharMgr来管理它
	wstring AbsFontFile = _ABSPATH(familly->fontFile);

	xFT2Font* pFont = new xFT2Font(m_pRendeDevice);
	pFont->createFontCharManager((wstring(familly->name) +L":" + pFontName).c_str() ) ;

	xFT2FontCharMgr* pFontCharMgr = pFont->getFontCharManager();

	pFontCharMgr->setRenderer(m_pRendeDevice);
	pFontCharMgr->m_UnderLine = pFontInitor->underline;
	pFontCharMgr->m_bBold     = pFontInitor->bold;
	pFontCharMgr->m_Italic    = pFontInitor->italic;
	pFontCharMgr->m_bAntilias = pFontInitor->antialias;

	int maxCache = pFontInitor->maxcache;

	if(maxCache == 0) maxCache = 1024;

	if( pFontCharMgr->load_font(AbsFontFile.c_str(),pFontInitor->width , pFontInitor->height) ==false)
	{
		delete pFontCharMgr;
		return NULL; 
	}

	pFont->setCacheSize(maxCache );
	//pFontCharMgr->setMaxSize(maxCache);
	//g_FT2FontCharMgr.push_back(pFontCharMgr);

	//新建一个FreeType2的字体绘制器。并把它和Font


	pFont->m_Filter = pFontInitor->filter;
	pFont->m_Info.bAntialias = pFontInitor->antialias;
	pFont->m_Info.bBold = pFontInitor->bold;
	pFont->m_Info.bItalic = pFontInitor->italic;
	pFont->m_Info.bUnderLine = pFontInitor->underline;

	pFont->m_Info.nMaxWidth  = pFontInitor->width;
	pFont->m_Info.nMaxHeight = pFontInitor->height;
	wcsncpy(pFont->m_Info.strFamilly , familly->name , 32);
	wcsncpy(pFont->m_Info.strName    , pFontName,         16);
	pFont->m_Info.nHeight =  pFontInitor->height;
	pFont->m_Info.nWidth  =  pFontInitor->width;

    pFont->m_LinePitch = pFontInitor->linepitch;

	std::ds_wstring fontFullName = std::ds_wstring(familly->name) + L":" + pFontName;
	xFontManager* pFontMgr  = dynamic_cast<xFontManager*>(this);
	pFontMgr->add(fontFullName , pFont  , 0 ).addRef();
	return false;
}

xFontRender* xFontLoader::insertTrueType(const wchar_t* familly, const wchar_t* name , xFontInitor* pFontInitor)
{
    xFontFamilly* pFontFamilly = findFamilly(familly);
    if(pFontFamilly->fonttype == eFT_Truetype)
		return insertTrueType(pFontFamilly , name , pFontInitor);
    return NULL;
}

bool   xFontLoader::__load_truetype_familly(xCfgNode* pFamillyNode)
{
	xCfgNode::CfgNodes  fontNodes;
	pFamillyNode->findNode(L"font",fontNodes);
	const wchar_t* pFontFile = pFamillyNode->value(L"file");
	
	
	const wchar_t* pFontFamillyName = pFamillyNode->value(L"name");
	xFontFamilly* pFontFamilly = insertFamilly(pFontFamillyName , pFontFile ,eFT_Truetype );
	
	//处理非缩放过的字体
	for(size_t i = 0  ; i < fontNodes.size() ; i++)
	{
		xCfgNode* pFontNode = fontNodes[i];

		//字体为缩放的字体，
		const  wchar_t*  pFontName = pFontNode->value(L"name");
		if(findFont(pFontFamillyName , pFontName).getResource() != NULL)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH,L" Font(%s) Familly(%s) font allready exist\n",pFontName , pFontFamillyName);
			continue;
		}

		//从配置文件取到数据
		xFontInitor fi;
		fi.height     = pFontNode->int_value(L"height");
		fi.width      = pFontNode->int_value(L"width");
		fi.antialias  = pFontNode->bool_value(L"antialias");
		fi.bold       = pFontNode->bool_value(L"bold");
		fi.italic     = pFontNode->bool_value(L"italic");
		fi.underline  = pFontNode->bool_value(L"underline");
		fi.maxcache   = pFontNode->int_value(L"maxcache");
		fi.linepitch  = 0;

		const  wchar_t*  pFiltertName   = pFontNode->value(L"filter");
		fi.filter =     __get_sampler_filter(pFiltertName);

		if(pFontNode->value(L"line_pitch"))
		{
			fi.linepitch = pFontNode->int_value(L"line_pitch");
		}

		//插入一个新的font;
		insertTrueType(pFontFamilly,pFontName , &fi);
	}
	return true;
}


//=================================================
//Resource Mananger

unsigned int xFontLoader::_getResSize(xFontRender* pRes)
{
    return 1;
}

bool         xFontLoader::_isResLoaded(xFontRender* pRes)
{
    return false;
}

bool         xFontLoader::_loadResource(const std::ds_wstring& strResName   , xFontRender* & pRes , int& ResSize, unsigned int arg)
{
    return false;
}

bool         xFontLoader::_unloadResource(const std::ds_wstring& strResName , xFontRender* & pRes , unsigned int& TotalResSize)
{
    return false;
}

void         xFontLoader::_deleteResource(const std::ds_wstring& strResName , xFontRender* pRes)
{

}

END_NAMESPACE_XEVOL3D
