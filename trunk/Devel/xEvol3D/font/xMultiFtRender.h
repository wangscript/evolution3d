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

#ifndef __MIXED_FONTRENDER_H__
#define __MIXED_FONTRENDER_H__
#include "xFontRender.h"

BEGIN_NAMESPACE_XEVOL3D
class xMixedFont : public xFontRender
{
public:
	xFontRender*      m_pUCSFontRender;
	xFontRender*      m_pAsciiFontRender;
	eFontFilter       m_Filter;
	int               m_LinePitch;
	
    IMPL_BASE_OBJECT_INTERFACE(xMixedFont);
public:
	xMixedFont(IFontRenderDevice* pRenderer);
	const   xFontInfo& getInfo();
	void    setDrawFilter(eFontFilter filter);
	bool    drawChar(wchar_t _chr , float x , float y, int& dx , int& dy, xColor_4f cl);
	void    enableAntialias(bool bAnti);
	bool    isAntialias();
	bool    drawText(const wchar_t* strText, float x , float y ,xColor_4f cl );
	bool    drawText(const wchar_t* strText, float x , float y , int maxWidth , xColor_4f cl );
	bool    drawTextToDynTexture(const wchar_t* strText, IBaseTexture* pDy);

	bool    getCharDim(wchar_t _chr , int& dx , int& dy);
	bool    getTextDim(const wchar_t*  strText , int& dx , int& dy , int maxWidth = 0);

	void    releaseChache();
	void    setCacheSize(int maxSize);
	int     getCacheSize();
	int     getLinePitch(){return m_LinePitch ; };
};

END_NAMESPACE_XEVOL3D
#endif
