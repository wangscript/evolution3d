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

#include "xMultiFtRender.h"
//#define  new XNEW_OP

BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_CLASSID(xMixedFont  , xFontRender);
//==============================
IMPL_OBJECT_FACTORY(xMixedFont , xFontRender , xMixedFontCreator , xFontRenderCreator , xFontRenderCreatorMgr , L"mixed", L"mixed font" ,xFontLoader* , int );

xMixedFont::xMixedFont(xFontLoader* pFontLoader , int arg):xFontRender(pFontLoader ,arg)
{
	m_RefCount = 1 ;
	m_LinePitch = 2;


}

xMixedFont::~xMixedFont()
{
	m_pAsciiFontRender.Release();
	m_pUCSFontRender.Release();
}

bool xMixedFont::init( xXmlNode* pFontNode)
{
	xFontLoader* pFontLoader = m_pFontLoader;
	//不是重复的字体
	if(pFontNode->value(L"link") == NULL &&  pFontNode->value(L"alias") == NULL )
	{
		const  wchar_t*  pFontName      = pFontNode->value(L"name");
		const  wchar_t*  pUSCFontName   = pFontNode->value(L"unicode");
		const  wchar_t*  pAsciiFontName = pFontNode->value(L"ascii");
		const  wchar_t*  pFiltertName   = pFontNode->value(L"filter");
		eFontFilter      filter = xFontLoader::getFontSamplerFilter(pFiltertName);
		int              line_pitch = 0;
		if(pFontNode->value(L"line_pitch"))
		{
			line_pitch = pFontNode->int_value(L"line_pitch");
		}
		int nMaxCache = pFontNode->int_value(L"maxcache");


		//没有ucs字体。错误
		if(pUSCFontName == NULL)
			return NULL;
    	m_pUCSFontRender = pFontLoader->findFont(pUSCFontName , true);
		if(pAsciiFontName == NULL) pAsciiFontName = pUSCFontName;
		m_pAsciiFontRender = pFontLoader->findFont(pAsciiFontName , true);
		if(m_pUCSFontRender.getResource() == NULL || m_pAsciiFontRender.getResource() == NULL)
		{
			return NULL;
		}

		m_pAsciiFontRender.AddRef();
		m_pUCSFontRender.AddRef();

		if(nMaxCache > 0)
		{
			setCacheSize(nMaxCache );
		}
		m_Filter = filter;
		m_LinePitch = line_pitch;
	}
	else
	{
		//-------------------------------------
	}
	return true;
}
const   xFontInfo& xMixedFont::getInfo()
{ 
	return m_pUCSFontRender->getInfo() ; 
}

void  xMixedFont::setDrawFilter(eFontFilter filter)
{
	m_Filter = filter ; 
}

bool  xMixedFont::drawText(const wchar_t* strText, float x , float y , int maxWidth , const xColor_4f& cl )
{
	if(m_pUCSFontRender.getResource() == NULL && m_pAsciiFontRender.getResource() == NULL)
		return false;	
	return xFontRender::drawText(strText,x,y,maxWidth,cl);

}

bool  xMixedFont::drawText(const wchar_t* strText, float x , float y ,const xColor_4f& cl )
{
	if(m_pUCSFontRender.getResource() == NULL && m_pAsciiFontRender.getResource() == NULL)
		return false;	
	return xFontRender::drawText(strText,x,y,cl);
}

xFontRender* xMixedFont::chooseRenderer(wchar_t _chr)
{
	xFontRender* pFont = NULL;

	if(_chr < 127 && m_pAsciiFontRender.getResource() ) 
	{
		pFont = m_pAsciiFontRender.getResource();
	}
	else
	{
		pFont = m_pUCSFontRender.getResource();
	}
	return pFont;
}
bool  xMixedFont::getCharDim(wchar_t _chr , int& dx , int& dy)
{
	xFontRender* pFont = chooseRenderer(_chr);
	return pFont->getCharDim(_chr , dx,dy );
}

bool  xMixedFont::getTextDim(const wchar_t*  strText , int& dx , int& dy , int maxWidth)
{
	dx = 0 ; dy = 0;
	if(m_pUCSFontRender == NULL && m_pAsciiFontRender == NULL)
		return false;	
	return xFontRender::getTextDim(strText,dx,dy,maxWidth);
}

bool xMixedFont::drawChar(wchar_t _chr , float x , float y, int& dx , int& dy, const xColor_4f& cl)
{
	xFontRender* pFont = chooseRenderer(_chr);
	return pFont->drawChar(_chr,x,y,dx,dy,cl);
}

bool  xMixedFont::drawTextToDynTexture(const wchar_t* strText, IBaseTexture* pRenderTarget)
{
	return false;
}

void  xMixedFont::releaseChache()
{
	m_pUCSFontRender->releaseChache();
	m_pAsciiFontRender->releaseChache();
}
void  xMixedFont::setCacheSize(int maxSize)
{
	m_pUCSFontRender->setCacheSize(maxSize);
	m_pAsciiFontRender->setCacheSize(maxSize);
}

int   xMixedFont::getCacheSize()
{
	return m_pAsciiFontRender->getCacheSize() + m_pUCSFontRender->getCacheSize();
}
void xMixedFont::enableAntialias(bool bAnti)
{
	m_pAsciiFontRender->enableAntialias(bAnti);
	m_pUCSFontRender->enableAntialias(bAnti);
}
bool xMixedFont::isAntialias()
{
	return m_pAsciiFontRender->isAntialias() && m_pUCSFontRender->isAntialias();
}

END_NAMESPACE_XEVOL3D
