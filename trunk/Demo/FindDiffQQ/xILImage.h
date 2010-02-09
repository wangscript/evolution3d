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

/***************************************************************************

***************************************************************************/

#ifndef _IMAGE_LOADER_
#define _IMAGE_LOADER_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <map>
#include "Color.h"
#include "xBaseImage.h"
#pragma pack(push,1)
#pragma warning(disable : 4996)

BEGIN_NAMESPACE_XEVOL3D


class   xILImage : public xImage
{
public:
	struct sILImageType
	{
		int fmt;
		int type;
		int bpp;
		sILImageType(int _fmt,int _type, int _bpp)
			:fmt(_fmt) , type(_type) , bpp(_bpp){}
	};


	typedef std::map<sILImageType, ePIXEL_FORMAT> IMAGEFORMAT_MAP;
	static IMAGEFORMAT_MAP gs_PIXEL_FORMAT;
public:
	xILImage();
	~xILImage();
//basic image function
	const wchar_t*  supportExt(bool bSave);
	int          byteAlign();
	bool         convert(ePIXEL_FORMAT newFmt);
	bool         scale(int w , int h , IMAGE_FILTER filter = eFILTER_BOX);
	bool         scalePOT(IMAGE_FILTER filter = eFILTER_BOX) ; 
	bool         scaleFit(int w , int h , IMAGE_FILTER filter = eFILTER_BOX) ;

	bool         load(xImageSize size);
	bool         load(int w , int h , ePIXEL_FORMAT fmt);
	bool         load(const wchar_t* fileName, IMAGE_ORIGIN img_origin = IO_LEFT_LOWER);
	bool         load(const wchar_t* fileName,const int8* mem_buf , int buf_len, IMAGE_ORIGIN img_origin = IO_LEFT_LOWER);
	bool         load(const wchar_t* filename,const int8* mem_buf , int buf_len, ePIXEL_FORMAT format,IMAGE_ORIGIN img_origin = IO_LEFT_LOWER);
	bool         save(const wchar_t* fileName);
	bool         free();

	xImageSize   getSize();
	void*        data(size_t mipmapLevel = 0 , size_t cubeMapImageIndex = 0/*-1 means not a cubemap*/, bool bCompressed = false);
	bool         data(const int8 * image_buffer,int mWidth,int mHeight , ePIXEL_FORMAT format);
	size_t       nMipmapLevel();
	size_t       nImage();
	size_t       nSlice();
//============ÌØÊâµÄº¯Êý==================
	void         setColor(int x,int y, xColor_4ub cl);
	void         getColor(int x,int y, xColor_4ub& cl);
	void         setAlpha(int x,int y, unsigned char alpha);
	void         setAlpha(xColor_4ub& cl_key,unsigned int8 alpha);
	void         setAlpha(xColor_4ub& cl_key);


	static bool  save(const wchar_t* filename,const int8 * image_buffer,int mWidth,int mHeight , ePIXEL_FORMAT format);
	//--------------------------------------------------------------
	//
	//  alloc & free ImageBuffer.
	//---------------------------------------------------------------
	static             void* allocImageBuffer(size_t size);
	static             void  freeImageBuffer(void* pBuffer);
	static             void  init_lib(IMAGE_ORIGIN img_origin = IO_LEFT_LOWER);

	static IMAGE_ENCODE_FORMAT GetImageType(wchar_t* filename);


protected:

	void               __GetImageInfo();
	ePIXEL_FORMAT      __getImageFormatFromIL(int format , int imageType ,int bpp);
	bool               __convertILFormat(ePIXEL_FORMAT format);
	bool               __checkData();
public:
	unsigned int8*     m_Data;
	xImageSize         m_ImgSize;
	int                m_ImageID; //for DevIL if this ID is -1 , mean's not loaded by devIL?
	int                m_nByteAlign;
};


END_NAMESPACE_XEVOL3D
#pragma  pack(pop)
#endif

