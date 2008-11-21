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

#include "xILImage.h"
#include "xFreeImage.h"
#include "xImage.h"
#include "xDXTImage.h"
#include "xHDRImage.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <ctype.h>

BEGIN_NAMESPACE_XEVOL3D
namespace xImageLib
{
	xImage*         create(IMAGE_TYPE type)
	{
#ifdef _DEVIL_
		return new xILImage;
#else
		return new xFreeImage;
#endif
	}

	static xImage* __createImageByeExtname(const wchar_t* filename )
	{

		std::wstring nameExt = xImage::GetFileExt(filename);
		const wchar_t* name_ext = nameExt.c_str();
		if(wcscmp(name_ext, L"DDS") == 0)
			return new xDXTImage;
		if(wcscmp(name_ext, L"HDR") == 0)
			return new xFreeImage;
		if(wcscmp(name_ext, L"JPEG") == 0)
			return new xFreeImage;
		if(wcscmp(name_ext, L"JPG") == 0)
			return new xFreeImage;
		if(wcscmp(name_ext, L"TGA") == 0)
			return new xFreeImage;
		//if(wcscmp(name_ext, L"PNG") == 0)
		//	return new xFreeImage;

		else 
#ifdef _DEVIL_
			return new xILImage;
#else
			return new xFreeImage;
#endif

		return NULL;

	}
	xImage* load(IMAGE_TYPE type ,xImageSize size)
	{
        xImage* pImage = create(type);
		if(false == pImage->load(size) )
		{
			kill(pImage);
			return NULL;
		}
		return pImage;
	}

	xImage* load(IMAGE_TYPE type , int w , int h , ePIXEL_FORMAT fmt)
	{
		xImage* pImage = create(type);
		if(false == pImage->load(w,h,fmt) )
		{
			kill(pImage);
			return NULL;
		}
		return pImage;
	}

	xImage* load(IMAGE_TYPE type ,const wchar_t* fileName, IMAGE_ORIGIN img_origin)
	{
			xImage* pImage = __createImageByeExtname(fileName);
			if(false == pImage->load(fileName , img_origin) )
			{
				kill(pImage);
				return NULL;
			}
			return pImage;
	}

	xImage* load(IMAGE_TYPE type ,const wchar_t* fileName, const int8* mem_buf , int buf_len, IMAGE_ORIGIN img_origin)
	{
		xImage* pImage = __createImageByeExtname(fileName);
		if(false == pImage->load(fileName , mem_buf , buf_len , img_origin) )
		{
			kill(pImage);
			return NULL;
		}
		return pImage;
	}

	xImage* load(IMAGE_TYPE type ,const wchar_t* fileName, const int8* mem_buf , int buf_len, ePIXEL_FORMAT format,IMAGE_ORIGIN img_origin )
	{
		xImage* pImage = __createImageByeExtname(fileName);
		if(false == pImage->load(fileName , mem_buf , buf_len , format, img_origin) )
		{
			kill(pImage);
			return NULL;
		}
		return pImage;
	}

	void kill(xImage* pImage)
	{
		pImage->free();
		delete pImage;
	}
}

END_NAMESPACE_XEVOL3D
