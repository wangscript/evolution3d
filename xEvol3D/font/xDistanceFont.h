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

#ifndef __DISTANCE_FONT_H__
#define __DISTANCE_FONT_H__
#include "xFontRender.h"

BEGIN_NAMESPACE_XEVOL3D

struct xDFFCharDesc
{
     unsigned short m_w;
	 unsigned short m_h;
	 unsigned short m_adv_x;
	 unsigned short m_adv_y;
	 unsigned short m_left;
	 unsigned short m_top;
	 int            m_offset;
};

class _XEVOL_BASE_API_ xDistanceFontGenerator
{
	int    m_fontSize;
	int    m_ttfSize;
	unsigned char*  m_ttfBuffer;
	void*  m_FT_Face;

public:
	wchar_t m_Begin;
	wchar_t m_Last;
public:
	xDistanceFontGenerator(int fontSize , int ttfSize);
	xDistanceFontGenerator(int fontSize , int ttfSize , wchar_t begin , wchar_t last);
	bool generate(const wchar_t* ttfFile , const wchar_t* distFileName);
	int  gen(wchar_t ch , FILE* file , int Offset , xDFFCharDesc& desc);
private:
	float          GetNearest(int x , int y , unsigned char v , int w , int h , unsigned char* _data);
	unsigned char* getPixel(int x , int y  , int w , int h , unsigned char* _data);
};


class  xDFFFamily
{
    xDFFCharDesc* m_pCharDesc;
};


END_NAMESPACE_XEVOL3D
#endif
