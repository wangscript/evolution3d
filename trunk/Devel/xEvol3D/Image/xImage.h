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

#ifndef __XIMAGELIB_H__
#define __XIMAGELIB_H__
#include "xBaseImage.h"
#include "../BaseLib/xRefCountObjPtr.h"
BEGIN_NAMESPACE_XEVOL3D
namespace xImageLib
{
_XEVOL_BASE_API_  xImage*      create(IMAGE_TYPE type = IMAGE_NORMAL);
_XEVOL_BASE_API_  xImage*      load(IMAGE_TYPE type ,xImageSize size);
_XEVOL_BASE_API_  xImage*      load(IMAGE_TYPE type ,int w , int h , ePIXEL_FORMAT fmt);
_XEVOL_BASE_API_  xImage*      load(IMAGE_TYPE type ,const wchar_t* fileName, IMAGE_ORIGIN img_origin = IO_LEFT_LOWER);
_XEVOL_BASE_API_  xImage*      load(IMAGE_TYPE type ,const wchar_t* fileName, const int8* mem_buf , int buf_len, IMAGE_ORIGIN img_origin = IO_LEFT_LOWER);
_XEVOL_BASE_API_  xImage*      load(IMAGE_TYPE type ,const wchar_t* filename, const int8* mem_buf , int buf_len, ePIXEL_FORMAT format,IMAGE_ORIGIN img_origin = IO_LEFT_LOWER);
_XEVOL_BASE_API_  void         kill(xImage* pImage);

}
typedef xAutoReleaser<xImage> xImagePtr;
END_NAMESPACE_XEVOL3D


#endif
