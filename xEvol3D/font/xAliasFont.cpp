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

#include "xAliasFont.h"
BEGIN_NAMESPACE_XEVOL3D


IMPL_OBJECT_FACTORY(xAliasFont , xFontRender , xAliasFontCreator , xFontRenderCreator , xFontRenderCreatorMgr , L"alias", L"alias font" , xFontLoader* , int );


void  xAliasFont::releaseChache()
{
	return m_FontRenderer->releaseChache();
}

const xFontInfo& xAliasFont::getInfo()
{
	return m_FontRenderer->getInfo();
}


bool  xAliasFont::drawChar(wchar_t _chr , float x , float y, int& dx , int& dy, const xColor_4f& cl)
{
	return m_FontRenderer->drawChar(_chr ,  x ,  y,  dx , dy, cl);
}

void  xAliasFont::enableAntialias(bool bAnti)
{
	return m_FontRenderer->enableAntialias(bAnti);
}

bool  xAliasFont::isAntialias()
{
	return m_FontRenderer->isAntialias();
}

bool  xAliasFont::getCharDim(wchar_t _chr , int& dx , int& dy)
{
	return m_FontRenderer->getCharDim(_chr , dx , dy);
}

void  xAliasFont::setCacheSize(int maxSize)
{
	return m_FontRenderer->setCacheSize(maxSize);
}

int   xAliasFont::getCacheSize()
{
	return m_FontRenderer->getCacheSize();
}
int   xAliasFont::getLinePitch()
{
	return m_FontRenderer->getLinePitch();
}

IMPL_BASE_OBJECT_CLASSID(xAliasFont  , xFontRender);


xAliasFont::xAliasFont(xFontLoader* pFontLoader,   int arg):xFontRender(pFontLoader , arg )
{
	m_RefCount = 1 ;
}

int xAliasFont::getResSize()
{
	return 0;
}

bool xAliasFont::init(xXmlNode* pNode)
{
	std::ds_wstring fullName ;
	std::ds_wstring fontName = pNode->value(L"link");
	m_pFontLoader->makeFontName(pNode->value(L"familly"), fontName, fullName);
	m_FontRenderer = m_pFontLoader->findFont(fullName.c_str() , true);
	m_FontRenderer.AddRef();
	return m_FontRenderer.getResource() != NULL;
}
xAliasFont::~xAliasFont()
{
	m_FontRenderer.Release();
    
}

END_NAMESPACE_XEVOL3D
