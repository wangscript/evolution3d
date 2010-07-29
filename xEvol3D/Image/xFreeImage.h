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

#ifndef __XFREE_IMAGE_H__
#define __XFREE_IMAGE_H__




#include "Color.h"
#include "xBaseImage.h"
#include "../BaseLib/xEvol3DBaseInc.h"
struct  FIBITMAP;
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_  xFreeImage : public xImage
{
public:
	FIBITMAP*     m_pFreeImage;
	ePIXEL_FORMAT m_Fmt;
	int           m_nByteAlign;
public:
	xFreeImage();
    ~xFreeImage();
    const wchar_t*  supportExt(bool bSave);
	int          byteAlign();
    bool         convert(ePIXEL_FORMAT newFmt);
	bool         scale(int w , int h , IMAGE_FILTER filter = eFILTER_BOX);
	bool         scalePOT(IMAGE_FILTER filter = eFILTER_BOX) ; 
	bool         scaleFit(int w , int h , IMAGE_FILTER filter = eFILTER_BOX) ;

	bool         load(xImageSize size);
	bool         load(int w , int h , ePIXEL_FORMAT fmt);
	bool         load(const wchar_t* fileName, IMAGE_ORIGIN img_origin = IO_LEFT_LOWER, unsigned int _arg = 0);
	bool         load(const wchar_t* fileName,const int8* mem_buf , int buf_len, IMAGE_ORIGIN img_origin = IO_LEFT_LOWER, unsigned int _arg = 0);
    bool         load(const wchar_t* filename,const int8* mem_buf , int buf_len, ePIXEL_FORMAT format,IMAGE_ORIGIN img_origin = IO_LEFT_LOWER, unsigned int _arg = 0);
	bool         save(const wchar_t* fileName);
	bool         free();
	bool         flipRGBA(int Bpp);
	bool         flipY( );

	xImageSize   getSize();
	void*        data(size_t mipmapLevel = 0 , size_t cubeMapImageIndex = 0/*-1 means not a cubemap*/,  bool bCompressed = false);
	bool         data(const int8 * image_buffer,int mWidth,int mHeight , ePIXEL_FORMAT format);
	
	size_t       nMipmapLevel();
	size_t       nImage();
	size_t       nSlice();
	
protected:
	bool          __replace(FIBITMAP *new_dib);
	ePIXEL_FORMAT __GetImagePixelFmt();
};

END_NAMESPACE_XEVOL3D




#endif
