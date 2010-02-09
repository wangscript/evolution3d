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

#ifndef __XBASE_IMAGE_H__
#define __XBASE_IMAGE_H__
#include "Color.h"
#include "../BaseLib/xEvol3DBaseInc.h"
BEGIN_NAMESPACE_XEVOL3D

enum IMAGE_FILTER
{
	eFILTER_BOX		  = 0,	// Box, pulse, Fourier window, 1st order (constant) b-spline
	eFILTER_BICUBIC	  = 1,	// Mitchell & Netravali's two-param cubic filter
	eFILTER_BILINEAR   = 2,	// Bilinear filter
	eFILTER_BSPLINE	  = 3,	// 4th order (cubic) b-spline
	eFILTER_CATMULLROM = 4,	// Catmull-Rom spline, Overhauser spline
	eFILTER_LANCZOS3	  = 5,	// Lanczos3 filter
	eFILTER_NONE,
};

//----------------------------------------------
// defien the Image codec Type
//     BITMAP
//     JPEG
//     TGA
//----------------------------------------------
enum IMAGE_ENCODE_FORMAT
{
	IMAGEEMC_BMP    = 0,
	IMAGEEMC_JPEG,
	IMAGEEMC_TGA ,
	IMAGEEMC_PCX ,
	IMAGEEMC_DDS ,
	IMAGEEMC_PNG ,
	IMAGEEMC_GIF ,
	IMAGEEMC_HDR,
	IMAGEEMC_TIF,
	IMAGEEMC_UNK    = 0xffffffff,
	
};

enum IMAGE_TYPE
{
    IMAGE_NORMAL,
	IMAGE_CUBEMAP,

	IMAGE_UNKNOWN,
};

enum IMAGE_ORIGIN
{
	IO_LEFT_LOWER,
	IO_LEFT_UPPER,
	IO_NO_CARE,
	IO_DEFAULT,
};

class  _XEVOL_BASE_API_   xImageSize
{
public:
	int           w ; 
	int           h ;
	int           pitch;
	ePIXEL_FORMAT fmt;

public:
	xImageSize(int w = 0, int h =  0 , ePIXEL_FORMAT fmt=PIXELFORMAT_R8G8B8A8);
	int        getBytePerPixel();
	int        getBitsPerPixel();

	static int getBytePerPixel(ePIXEL_FORMAT fmt);
	static int getBitsPerPixel(ePIXEL_FORMAT fmt);
	static int caclTexPOTSize(int w);
	static int RoundnByte(int len , int nByte);
};

class _XEVOL_BASE_API_  xImage : public IRefCountObject
{
public:
	IMPL_REFCOUNT_OBJECT_INTERFACE(xImage);
	virtual ~xImage();
	xImage();

	static  const wchar_t* GetFileExt(const wchar_t* fileName);
	virtual const wchar_t* supportExt(bool bSave) = 0;
	virtual int            byteAlign() = 0;
	virtual bool           convert(ePIXEL_FORMAT newFmt) = 0;
	virtual bool           scale(int w , int h , IMAGE_FILTER filter = eFILTER_BOX) = 0;
	virtual bool           scalePOT(IMAGE_FILTER filter = eFILTER_BOX) = 0 ; 
	virtual bool           scaleFit(int w , int h , IMAGE_FILTER filter = eFILTER_BOX)  = 0;

	virtual bool         load(xImageSize size) = 0;
	virtual bool         load(int w , int h , ePIXEL_FORMAT fmt) = 0;
	virtual bool         load(const wchar_t* fileName, IMAGE_ORIGIN img_origin = IO_LEFT_LOWER) = 0;
	virtual bool         load(const wchar_t* fileName, const int8* mem_buf , int buf_len, IMAGE_ORIGIN img_origin = IO_LEFT_LOWER) = 0;
	virtual bool         load(const wchar_t* filename, const int8* mem_buf , int buf_len, ePIXEL_FORMAT format,IMAGE_ORIGIN img_origin = IO_LEFT_LOWER) = 0;
	virtual bool         save(const wchar_t* fileName) = 0;
	virtual bool         save(const wchar_t* fileName , const int8* image_buf  , int w , int h  , ePIXEL_FORMAT format);
	virtual bool         free() = 0;

	virtual xImageSize   getSize() = 0;
	virtual void*        data(size_t mipmapLevel = 0 , size_t cubeMapImageIndex = 0/*-1 means not a cubemap*/, bool bCompressed = false) = 0;
	virtual bool         data(const int8 * image_buffer,int mWidth,int mHeight , ePIXEL_FORMAT format) = 0;
	virtual size_t       nMipmapLevel() = 0;
	virtual size_t       nImage() = 0;
	virtual size_t       nSlice() = 0;

	virtual bool         flipY( );
protected:
	bool          __flipRGB();
	bool          __flipRGBA();
};

END_NAMESPACE_XEVOL3D
#endif

