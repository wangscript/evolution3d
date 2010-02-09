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
#include "xBaseImage.h"
#include "xImage.h"
#include <string>
#include <assert.h>
BEGIN_NAMESPACE_XEVOL3D
xImageSize::xImageSize(int _w, int _h , ePIXEL_FORMAT _fmt)
:w(_w),h(_h),fmt(_fmt),pitch(0)
{


}
int xImageSize::getBitsPerPixel(ePIXEL_FORMAT fmt)
{
	return (fmt&0xff00) >> 8;
}

int xImageSize::getBytePerPixel(ePIXEL_FORMAT fmt)
{
	return getBitsPerPixel(fmt) / 8;
}


int xImageSize::getBytePerPixel()
{
	return getBytePerPixel(fmt);
}


int xImageSize::getBitsPerPixel()
{
	return getBitsPerPixel(fmt);
}

int xImageSize::caclTexPOTSize(int w)
{
	//return  1 << (int)floor((log((double)w)/log(2.0f)) + 0.5f); 
	int i = 1;
	while( i < w )
	{
		i <<= 1;
	}
	return i;
}



int xImageSize::RoundnByte(int len , int nByte)
{
	if ( ( len%nByte)==0 )
		return len;
	else
		return (len+ nByte-(len%nByte));
}

bool xImage::__flipRGBA()
{
	xImageSize _size = getSize();
	xColor_4ub* pixel = NULL;
	for(int y = 0 ; y < _size.h ; y ++)
	{
		for(int x = 0 ; x < _size.w ; x ++ )
		{
			pixel = (xColor_4ub*)(  (char*) data() + _size.pitch  * y  + 4*x );
			unsigned char t = pixel->r;
			pixel->r = pixel->b;
			pixel->b = t;
			pixel ++ ;
		}
	}
	return true;
}

bool xImage::__flipRGB()
{
	xImageSize _size = getSize();
	xColor_3ub* pixel = NULL;
	for(int y = 0 ; y < _size.h ; y ++)
	{

		for(int x = 0 ; x < _size.w ; x ++ )
		{
			pixel = (xColor_3ub*)(  (char*) data() + _size.pitch * y + 3*x );
			unsigned char t = pixel->r;
			pixel->r = pixel->b;
			pixel->b = t;
		}
	}
	return true;
}

bool xImage::flipY( )
{
	xImageSize _size = this->getSize();
	int h     = _size.h;
	int w     = _size.w;
	int pitch = _size.pitch;
	int8* pTemp   = new int8[ pitch ];
	int8* pTop    = (int8*)this->data() ;
	int8* pBottom = pTop + pitch * (h-1);
	for(int y = 0 ; y < h / 2 ; y ++)
	{
		memcpy(pTemp    , pTop    ,pitch);
		memcpy(pTop     , pBottom ,pitch);
		memcpy(pBottom  , pTemp   ,pitch);
		pTop    += pitch;
		pBottom -= pitch;	
	}
	delete [] pTemp ;
	return true;
}

xImage::xImage()
{
	m_RefCount = 1;
}

xImage::~xImage()
{
	assert(m_RefCount >= 0);
}

const wchar_t* xImage::GetFileExt(const wchar_t* fileName)
{
	static wchar_t name_ext[6]={0};
	size_t  len = wcslen(fileName);

	for( size_t i = len-1 ; i > 0 ; i -- )
	{
		if(fileName[i] == '.')
		{
			wcsncpy(name_ext,fileName + i + 1,6);

			for(size_t j=0;j< (len - i) ; j++)
			{
				if(name_ext[j]<127)
					name_ext[j] = tolower(name_ext[j]);
			}
			break;
		}
	}
	return name_ext;
}

bool xImage::save(const wchar_t* fileName , const int8* image_buf  , int w , int h  , ePIXEL_FORMAT format)
{
	xImage* pImage = xImageLib::create(fileName);
	pImage->data(image_buf , w , h , format);
	pImage->save(fileName);
	pImage->free();
	pImage->ReleaseObject();
	return true;
}

END_NAMESPACE_XEVOL3D

