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

#ifndef __ALIAS_FONT_H__
#define __ALIAS_FONT_H__
#include "xFontRender.h"
#include "xFontManager.h"

BEGIN_NAMESPACE_XEVOL3D

class xAliasFont : public xFontRender
{
	HFontRender      m_FontRenderer;
public:
    IMPL_BASE_OBJECT_INTERFACE(xAliasFont);
	xAliasFont(xFontLoader* pFontLoader , int arg);	
	~xAliasFont();
	bool  init( xXmlNode* pInitXml);
	void  releaseChache();
    const xFontInfo& getInfo();
	bool  drawChar(xWCharType _chr , float x , float y, int& dx , int& dy, const xColor_4f& cl) ;
	void  enableAntialias(bool bAnti);
	bool  isAntialias();
    bool  getCharDim(xWCharType _chr , int& dx , int& dy);
	void  setCacheSize(int maxSize);
	int   getCacheSize();
	int   getLinePitch();
	int   getResSize();
};

END_NAMESPACE_XEVOL3D
#endif
