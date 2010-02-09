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

#include "xBaseImage.h"
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
					name_ext[j] = toupper(name_ext[j]);
			}
			break;
		}
	}
	return name_ext;
}

END_NAMESPACE_XEVOL3D

