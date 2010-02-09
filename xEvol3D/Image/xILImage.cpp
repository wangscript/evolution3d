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
#ifdef _DEVIL_
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <ctype.h>

#undef _UNICODE
#undef UNICODE
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/devil_internal_exports.h>
#include <math.h>
#include "xILImage.h"
#include "xImage.h"
#include "../BaseLib/xI18N.h"
#ifdef _WIN32
#pragma  comment(lib,"DevIL.lib")
#pragma  comment(lib,"ilu.lib")
#endif
#include <iostream>
using namespace std;

#ifndef _UNICODE
#define _UNICODE
#endif

BEGIN_NAMESPACE_XEVOL3D
xILImage::IMAGEFORMAT_MAP xILImage::gs_PIXEL_FORMAT;
bool operator< (const xILImage::sILImageType& lhv , const  xILImage::sILImageType& rhv)
{
	return (lhv.bpp + 2 * lhv.fmt + 3 * lhv.type) < ( rhv.bpp + 2 * rhv.fmt + 3  * rhv.type);
}
enum IL_IMAGE_FILTER
{
	IL_FILTER_NEAREST	         = 0x2601,
	IL_FILTER_LINEAR		     = 0x2602,
	IL_FILTER_BILINEAR	         = 0x2603,
	IL_FILTER_SCALE_BOX	     = 0x2604,
	IL_FILTER_SCALE_TRIANGLE	 = 0x2605,
	IL_FILTER_SCALE_BELL	     = 0x2606,
	IL_FILTER_SCALE_BSPLINE     = 0x2607,
	IL_FILTER_SCALE_LANCZOS3	 = 0x2608,
	IL_FILTER_SCALE_MITCHELL	 = 0x2609,
};

IL_IMAGE_FILTER GetILFilter(IMAGE_FILTER filter)
{
	switch(filter)
	{
	case eFILTER_BOX:
		return IL_FILTER_SCALE_BOX;
		break;
	case eFILTER_BICUBIC:
		return IL_FILTER_SCALE_TRIANGLE;
		break;	  
	case eFILTER_BILINEAR:
		return IL_FILTER_BILINEAR;
		break;   
	case eFILTER_BSPLINE	:
		return IL_FILTER_SCALE_BSPLINE;
		break;  
	case eFILTER_CATMULLROM :
		return IL_FILTER_SCALE_MITCHELL;
		break;
	case eFILTER_LANCZOS3:
		return IL_FILTER_SCALE_LANCZOS3;
		break;
	default:
		return IL_FILTER_NEAREST;

	};
}

namespace X2DImageHelper
{
	IMAGE_ORIGIN       g_LibImageOrigin;


	int getILImageFormat(ePIXEL_FORMAT format)
	{
		switch(format)
		{
		case PIXELFORMAT_R8G8B8 :
			return IL_RGB;
		case PIXELFORMAT_R8G8B8A8 :
			return IL_RGBA;
		case PIXELFORMAT_B8G8R8 :
			return IL_BGR;
		case PIXELFORMAT_B8G8R8A8:
			return IL_BGRA;
		case PIXELFORMAT_DXT1:
			return IL_DXT1;
		case PIXELFORMAT_DXT2:
			return IL_DXT2;
		case PIXELFORMAT_DXT3:
			return IL_DXT3;
		case PIXELFORMAT_DXT4:
			return IL_DXT4;
		case PIXELFORMAT_DXT5:
			return IL_DXT5;
		default:
			return -1;
		}

	}



	ILenum  GetILImageType(const wchar_t* filename)
	{
		wstring nameExt =  xImage::GetFileExt(filename);
		const wchar_t* name_ext = nameExt.c_str();


		if(wcscmp(name_ext, L"JPEG") == 0)
			return IL_JPG;
		if(wcscmp(name_ext, L"JPG") == 0)
			return IL_JPG;
		if(wcscmp(name_ext, L"BMP") == 0)
			return IL_BMP;
		if(wcscmp(name_ext, L"PNG") == 0)
			return IL_PNG;
		if(wcscmp(name_ext, L"GIF") == 0)
			return IL_GIF;
		if(wcscmp(name_ext, L"TGA") == 0)
			return IL_TGA;
		if(wcscmp(name_ext, L"TIF") == 0 || wcscmp(name_ext, L"TIFF") == 0)
			return IL_TIF;
		if(wcscmp(name_ext, L"DDS") == 0)
			return IL_DDS ;
		if(wcscmp(name_ext, L"PCX") == 0)
			return IL_PCX;
		if(wcscmp(name_ext, L"PCD") == 0)
			return IL_PCD;
		if(wcscmp(name_ext, L"MNG") == 0)
			return IL_MNG;
		if(wcscmp(name_ext, L"PSD") == 0)
			return IL_PSD;
		if(wcscmp(name_ext, L"PSP") == 0)
			return IL_PSP;
		if(wcscmp(name_ext, L"ICO") == 0)
			return IL_ICO;
		if(wcscmp(name_ext, L"HDR") == 0)
			return IL_HDR;
		return IL_TYPE_UNKNOWN;


	}

}

using namespace X2DImageHelper;

ePIXEL_FORMAT xILImage::__getImageFormatFromIL(int format , int imageType ,int bpp)
{
	sILImageType imgt(format,imageType,bpp);
	IMAGEFORMAT_MAP::iterator pos = gs_PIXEL_FORMAT.find(imgt);
	if(pos != gs_PIXEL_FORMAT.end() )
		return pos->second;

	switch(format)
	{
		case IL_DXT1:
			return PIXELFORMAT_DXT1;
		case  IL_DXT2:
			return PIXELFORMAT_DXT2;
		case  IL_DXT3:
			return PIXELFORMAT_DXT3;
		case IL_DXT4 :
			return PIXELFORMAT_DXT4;
		case IL_DXT5:
			return PIXELFORMAT_DXT5 ;
		default:
			return PIXELFORMAT_None;
	}

}

const wchar_t*   xILImage::supportExt(bool bSave)
{
     if(bSave)
	 {
		 static wchar_t* saveExt =  L"JPEG JPG BMP PNG GIF TGA DDS PCX";
		 return saveExt;
	 }
	 else
	 {
		 static wchar_t* loadExt =  L"JPEG JPG BMP PNG GIF TGA DDS PCX TIF TIFF HDR";
		 return loadExt;
	 }
}



IMAGE_ENCODE_FORMAT   xILImage::GetImageType(wchar_t* filename)
{
	wstring nameExt =  xImage::GetFileExt(filename);
	const  wchar_t* name_ext = nameExt.c_str();

	if(wcscmp(name_ext, L"JPEG") == 0)
		return IMAGEEMC_JPEG;
	if(wcscmp(name_ext, L"JPG") == 0)
		return IMAGEEMC_JPEG;
	if(wcscmp(name_ext, L"BMP") == 0)
		return IMAGEEMC_BMP;
	if(wcscmp(name_ext, L"PNG") == 0)
		return IMAGEEMC_PNG;
	if(wcscmp(name_ext, L"GIF") == 0)
		return IMAGEEMC_GIF;
	if(wcscmp(name_ext, L"TGA") == 0)
		return IMAGEEMC_TGA;
	if(wcscmp(name_ext, L"DDS") == 0)
		return IMAGEEMC_DDS;
	if(wcscmp(name_ext, L"PCX") == 0)
		return IMAGEEMC_PCX;
	if(wcscmp(name_ext, L"HDR") == 0)
		return IMAGEEMC_HDR;
	if(wcscmp(name_ext, L"TIF") == 0)
		return IMAGEEMC_TIF;
	if(wcscmp(name_ext, L"TIFF") == 0)
		return IMAGEEMC_TIF;
	return IMAGEEMC_UNK;


}

xILImage::~xILImage()
{
   free();
}

xILImage::xILImage()
{  
	static bool bInited = false;
	if(bInited == false)
	{
		init_lib(IO_LEFT_LOWER);
		bInited = true;
	}
	m_ImgSize.fmt = PIXELFORMAT_B8G8R8;
	m_ImgSize.h  = 1;
	m_ImgSize.w  = 1;
	m_Data = NULL;
	m_ImageID = -1;
}


void  xILImage::init_lib(IMAGE_ORIGIN img_origin)
{
	static bool ilIsInit  = false;
	if(ilIsInit == false)
	{
		// Needed to initialize DevIL.
		ilInit();
		switch(img_origin)
		{
		case IO_LEFT_LOWER:
			ilEnable(IL_ORIGIN_SET);
			ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
			break;
		case IO_DEFAULT:
			ilEnable(IL_ORIGIN_SET);
			break;
		case IO_LEFT_UPPER:
			ilEnable(IL_ORIGIN_SET);
			ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
			break;
		case IO_NO_CARE:
		default:
			ilDisable(IL_ORIGIN_SET);
		}
		g_LibImageOrigin = img_origin;
		ilIsInit = true;
		ilEnable(IL_FILE_OVERWRITE);
#define ADD_FORMAT(fmt , type , bpp , pixelformat) gs_PIXEL_FORMAT.insert( IMAGEFORMAT_MAP::value_type(sILImageType(fmt , type , bpp) , pixelformat ) )

		ADD_FORMAT(IL_COLOR_INDEX , IL_UNSIGNED_BYTE , 8 , PIXELFORMAT_LUMINANCE8);
		ADD_FORMAT(IL_COLOR_INDEX , IL_BYTE          , 8 , PIXELFORMAT_LUMINANCE8);
		//Interger format
		ADD_FORMAT(IL_RGB  , IL_BYTE            , 24 , PIXELFORMAT_R8G8B8);
		ADD_FORMAT(IL_RGB  , IL_UNSIGNED_BYTE   , 24 , PIXELFORMAT_R8G8B8);
		ADD_FORMAT(IL_RGB  , IL_SHORT           , 48 , PIXELFORMAT_R16G16B16S);
		ADD_FORMAT(IL_RGB  , IL_UNSIGNED_SHORT  , 48 , PIXELFORMAT_R16G16B16U);
	
		ADD_FORMAT(IL_RGBA , IL_BYTE            , 32 , PIXELFORMAT_R8G8B8A8);
		ADD_FORMAT(IL_RGBA , IL_UNSIGNED_BYTE   , 32 , PIXELFORMAT_R8G8B8A8);
		ADD_FORMAT(IL_RGBA , IL_SHORT           , 64 , PIXELFORMAT_R16G16B16A16S);
		ADD_FORMAT(IL_RGBA , IL_UNSIGNED_SHORT  , 64 , PIXELFORMAT_R16G16B16A16U);

		ADD_FORMAT(IL_BGR  , IL_BYTE            , 24 , PIXELFORMAT_B8G8R8);
		ADD_FORMAT(IL_BGR  , IL_UNSIGNED_BYTE   , 24 , PIXELFORMAT_B8G8R8);



		ADD_FORMAT(IL_BGRA , IL_BYTE            , 32 , PIXELFORMAT_B8G8R8A8);
		ADD_FORMAT(IL_BGRA , IL_UNSIGNED_BYTE   , 32 , PIXELFORMAT_B8G8R8A8);

		//Float .
		ADD_FORMAT(IL_BGR  , IL_FLOAT            , 48 , PIXELFORMAT_R16G16B16F);
		ADD_FORMAT(IL_BGR  , IL_FLOAT            , 96 , PIXELFORMAT_R32G32B32F);

		ADD_FORMAT(IL_BGRA , IL_FLOAT            , 64 , PIXELFORMAT_R16G16B16A16F);
		ADD_FORMAT(IL_BGRA , IL_FLOAT            , 128, PIXELFORMAT_R32G32B32A32F);

#undef ADD_FORMAT
	}
}

void* xILImage::allocImageBuffer(size_t size)
{
	return (void*)(new int8[size]);
}

void  xILImage::freeImageBuffer(void* pBuffer)
{
	if(pBuffer != NULL)
		delete[] (int8*)pBuffer;
}

void* xILImage::data(size_t mipmapLevel, size_t cubeMapImageIndex, bool bCompressed)
{
	if(mipmapLevel == 0 && cubeMapImageIndex == 0)
	    return m_Data;
	if(ilIsImage(m_ImageID) == IL_FALSE)
	{
		//Not a devil image.
		return NULL;
	}
	ilBindImage(m_ImageID);
    size_t nLevel = ilGetInteger(IL_NUM_MIPMAPS) + 1;//IL得到的比实际少一
	size_t nCubeMap = ilGetInteger(IL_NUM_IMAGES) + 1;//IL得到的比实际少一
	if(nLevel <= mipmapLevel || nCubeMap <= cubeMapImageIndex)
		return NULL;
	ilActiveMipmap( (ILuint)mipmapLevel);
	ilActiveImage( (ILuint)cubeMapImageIndex);
	return ilGetData();
}

size_t xILImage::nMipmapLevel()
{
	if(ilIsImage(m_ImageID) == IL_FALSE)
		return 1;
	ilBindImage(m_ImageID);
	return ilGetInteger(IL_NUM_MIPMAPS) + 1;

}

size_t xILImage::nImage()
{
	if(ilIsImage(m_ImageID) == IL_FALSE)
		return 1;
	ilBindImage(m_ImageID);
	return ilGetInteger(IL_NUM_IMAGES) + 1;
}

size_t xILImage::nSlice()
{
	return 1;
}

xImageSize  xILImage::getSize()
{
	return m_ImgSize;
}

bool xILImage::free()
{
	if( m_ImageID != -1 && ilIsImage(m_ImageID) )
	{
		ilDeleteImages(1,(const ILuint*)&m_ImageID);
		m_ImageID = -1;
		m_Data = NULL;
		m_ImgSize.fmt = PIXELFORMAT_None;
		m_ImgSize.h  = 0;
		m_ImgSize.w   = 0;
		return true;
	}

	if(m_Data != NULL)
		freeImageBuffer((void*)m_Data);
	m_Data = NULL;

	m_ImgSize.fmt = PIXELFORMAT_None;
	m_ImgSize.h  = 0;
	m_ImgSize.w   = 0;
	return true;


}

void   xILImage::setColor(int x,int y, xColor_4ub cl)
{
	int bbp = m_ImgSize.getBytePerPixel();
	if(bbp == 1)
	{
		m_Data[y * m_ImgSize.w + x ] = cl.r;
		return ;
	}
	if(bbp == 3)
	{
		int pos = y * m_ImgSize.w * 3 + x*3; 
		m_Data[pos + 1] = cl.g;
		if(m_ImgSize.fmt == PIXELFORMAT_R8G8B8)
		{
			m_Data[pos    ] = cl.r;
			m_Data[pos + 2] = cl.b;
		}
		else
		{
			m_Data[pos    ] = cl.b;
			m_Data[pos + 2] = cl.r;
		}
	}

	if(bbp == 4)
	{
		int pos = y * m_ImgSize.w * 4 + x*4; 
		m_Data[pos + 1] = cl.g;
		m_Data[pos + 3] = cl.a;

		if(m_ImgSize.fmt == PIXELFORMAT_R8G8B8A8)
		{
			m_Data[pos    ] = cl.r;
			m_Data[pos + 2] = cl.b;
		}
		else
		{
			m_Data[pos    ] = cl.b;
			m_Data[pos + 2] = cl.r;
		}
	}

}

void   xILImage::getColor(int x,int y, xColor_4ub& cl)
{
	int bbp = m_ImgSize.getBytePerPixel();
	if(bbp == 1)
	{
		cl.r = m_Data[y * m_ImgSize.w + x ];
		return ;
	}
	if(bbp == 3)
	{
		int pos = y * m_ImgSize.w * 3 + x*3; 
		cl.g = m_Data[pos + 1];
		if(m_ImgSize.fmt == PIXELFORMAT_R8G8B8)
		{
			cl.r = m_Data[pos    ];
			cl.b = m_Data[pos + 2];
		}
		else
		{
			cl.b = m_Data[pos    ];
			cl.r = m_Data[pos + 2];
		}
	}

	if(bbp == 4)
	{
		int pos = y * m_ImgSize.w * 4 + x*4; 
		cl.g = m_Data[pos + 1] ;
		cl.a = m_Data[pos + 3] ;

		if(m_ImgSize.fmt == PIXELFORMAT_R8G8B8A8)
		{
			cl.r = m_Data[pos    ];
			cl.b = m_Data[pos + 2];
		}
		else
		{
			cl.b = m_Data[pos    ];
			cl.r = m_Data[pos + 2];
		}
	}
}

void   xILImage::setAlpha(int x,int y, unsigned int8 alpha)
{
	if(4 != m_ImgSize.getBytePerPixel())
		return ;
	if(m_ImgSize.fmt == PIXELFORMAT_B8G8R8A8 || m_ImgSize.fmt == PIXELFORMAT_R8G8B8A8)
	{
		int pos = y * m_ImgSize.w * 4 + x*4; 
		m_Data[pos+3] = alpha;
	}
}

void   xILImage::setAlpha(xColor_4ub& cl_key,unsigned int8 alpha)
{
	if(4 != m_ImgSize.getBytePerPixel())
		return ;

	int len = m_ImgSize.h * m_ImgSize.w * 4;

	if(m_ImgSize.fmt == PIXELFORMAT_B8G8R8A8 )
	{
		for(int l = 0 ; l < len ; l += 4)
		{
			if(m_Data[l] == cl_key.b && m_Data[l + 1] == cl_key.g && m_Data[l + 2] == cl_key.r)
			{
				m_Data[l + 3] = alpha;
			}
		}
	}  
	else if(m_ImgSize.fmt == PIXELFORMAT_R8G8B8A8)
	{
		for(int l = 0 ; l < len ; l += 4)
		{
			if(m_Data[l] == cl_key.r && m_Data[l + 1] == cl_key.g && m_Data[l + 2] == cl_key.b)
			{
				m_Data[l + 3] = alpha;
			}
		}           
	}
}

void   xILImage::setAlpha(xColor_4ub& cl_key)
{
	if(4 != m_ImgSize.getBytePerPixel())
		return ;

	int len = m_ImgSize.h * m_ImgSize.w * 4;

	if(m_ImgSize.fmt == PIXELFORMAT_B8G8R8A8 )
	{
		for(int l = 0 ; l < len ; l += 4)
		{
			if(m_Data[l] == cl_key.b && m_Data[l + 1] == cl_key.g && m_Data[l + 2] == cl_key.r)
			{
				m_Data[l + 3] = cl_key.a;
			}
		}
	}  
	else if(m_ImgSize.fmt == PIXELFORMAT_R8G8B8A8)
	{
		for(int l = 0 ; l < len ; l += 4)
		{
			if(m_Data[l] == cl_key.r && m_Data[l + 1] == cl_key.g && m_Data[l + 2] == cl_key.b)
			{
				m_Data[l + 3] = cl_key.a;
			}
		}           
	}     
}

bool   xILImage::load(int Width, int Height , ePIXEL_FORMAT Format)
{
	free();
	m_ImgSize.fmt = Format;
	m_ImgSize.h = Height;
	m_ImgSize.w  = Width;
	int byte_per_pixel = m_ImgSize.getBytePerPixel();
	m_Data = (unsigned int8*)allocImageBuffer(byte_per_pixel *  Height *  Width);
	memset((void*)m_Data,0,byte_per_pixel * Height * Width);
	return m_Data != NULL;
}
bool   xILImage::load(xImageSize size)
{
	return load(size.w,size.h,size.fmt);
}


bool   xILImage::convert(ePIXEL_FORMAT newFormat)
{
	if( ilIsImage(m_ImageID) )
		return __convertILFormat(newFormat);

	if(m_ImgSize.getBytePerPixel() != 3 && m_ImgSize.getBytePerPixel() != 4)
		return false;

	xImageSize newSize;
	newSize.fmt = newFormat;
	if(newSize.getBytePerPixel() != 3 && newSize.getBytePerPixel() != 4)
		return false;

	int byte_per_color;
	int x ; 
	int y ;
	bool needSwap;
	unsigned int8* pDstLine;
	unsigned int8* pSrcLine;


	if(newFormat == m_ImgSize.fmt)
		return true;
	//---------------------------------------------
	//以下处理RGB(A) <-> BRG(A)
	//---------------------------------------------
	byte_per_color = 3;
	if(newFormat == PIXELFORMAT_R8G8B8 && m_ImgSize.fmt == PIXELFORMAT_B8G8R8)
		goto SWAP_BR;

	if(newFormat == PIXELFORMAT_R8G8B8 && m_ImgSize.fmt == PIXELFORMAT_B8G8R8)
		goto SWAP_BR;

	byte_per_color = 4;
	if(newFormat == PIXELFORMAT_R8G8B8A8 && m_ImgSize.fmt == PIXELFORMAT_B8G8R8A8)
		goto SWAP_BR;
	if(newFormat == PIXELFORMAT_B8G8R8A8 && m_ImgSize.fmt == PIXELFORMAT_R8G8B8A8)
	{
SWAP_BR:
		x = 0 ; 
		y = 0 ;
		unsigned int8 temp;
		unsigned int8* pixel = m_Data;

		for(y = 0;y < m_ImgSize.h ; y ++)
		{
			for(x = 0 ; x < m_ImgSize.w ; x++)
			{
				temp = *pixel;
				*pixel = *(pixel + 2);
				*(pixel+2) = temp;
				pixel += byte_per_color; 
			}
		}
		m_ImgSize.fmt = newFormat;
		return true;
	}


	//--------------------------------------------------------------------
	//下面处理２４bit 到　３２　bit的转换
	needSwap = false;
	if(newFormat == PIXELFORMAT_R8G8B8A8 && m_ImgSize.fmt == PIXELFORMAT_R8G8B8)
		goto C24TO_32BIT;
	if(newFormat == PIXELFORMAT_B8G8R8A8 && m_ImgSize.fmt == PIXELFORMAT_B8G8R8)
		goto C24TO_32BIT;

	needSwap = true;
	if(newFormat == PIXELFORMAT_R8G8B8A8 && m_ImgSize.fmt == PIXELFORMAT_B8G8R8)
		goto C24TO_32BIT;
	if(newFormat == PIXELFORMAT_B8G8R8A8 && m_ImgSize.fmt == PIXELFORMAT_R8G8B8)
	{
C24TO_32BIT:
		unsigned char* pNewData = (unsigned char*)allocImageBuffer(m_ImgSize.h * m_ImgSize.w * 4);

		pSrcLine = m_Data;
		pDstLine = pNewData;
		for(y = 0 ;  y < m_ImgSize.h ; y ++)
		{
			for( x = 0 ; x < m_ImgSize.w ; x++)
			{
				if(needSwap)
				{
					*pDstLine = *(pSrcLine + 2);
					pDstLine++;
					*pDstLine = *(pSrcLine + 1);
					pDstLine++;
					*pDstLine = *pSrcLine;
					pDstLine++;
					*pDstLine = 255;
					pDstLine++;
					pSrcLine += 3;
				}
				else
				{
					*pDstLine = *pSrcLine;

					pDstLine++; 
					pSrcLine++;
					*pDstLine = *pSrcLine;

					pDstLine++; 
					pSrcLine++;

					*pDstLine = *pSrcLine;
					pDstLine++; 
					pSrcLine++;

					*pDstLine = 255;
					pDstLine++; 
				}

			}
		}
		freeImageBuffer((void*)m_Data);
		m_Data = pNewData;
		m_ImgSize.fmt = newFormat;
		return true;
	}

	//-----------------------------------------
	//下面处理32bit 到24bit的转换
	needSwap = false;
	if(newFormat == PIXELFORMAT_R8G8B8 && m_ImgSize.fmt == PIXELFORMAT_R8G8B8A8)
		goto C32TO_24BI;
	if(newFormat == PIXELFORMAT_B8G8R8 && m_ImgSize.fmt == PIXELFORMAT_B8G8R8A8)
		goto C32TO_24BI;

	needSwap = true;
	if(newFormat == PIXELFORMAT_R8G8B8 && m_ImgSize.fmt == PIXELFORMAT_B8G8R8A8)
		goto C32TO_24BI;
	if(newFormat == PIXELFORMAT_B8G8R8 && m_ImgSize.fmt == PIXELFORMAT_R8G8B8A8)
	{
C32TO_24BI:
		unsigned int8* pNewData = (unsigned int8*)allocImageBuffer(m_ImgSize.h * m_ImgSize.w * 3);

		pSrcLine = m_Data;
		pDstLine = pNewData;

		for(y = 0 ;  y < m_ImgSize.h ; y ++)
		{
			for( x = 0 ; x < m_ImgSize.w ; x++)
			{
				if(needSwap)
				{
					*pDstLine = *(pSrcLine + 2);
					pDstLine++;
					*pDstLine = *(pSrcLine + 1);
					pDstLine++;
					*pDstLine = *pSrcLine;
					pDstLine++;

					pSrcLine += 4;
				}
				else
				{
					*pDstLine = *pSrcLine;
					pDstLine++; pSrcLine++;

					*pDstLine = *pSrcLine;
					pDstLine++; pSrcLine++;

					*pDstLine = *pSrcLine;
					pDstLine++; pSrcLine++;

					pSrcLine++;

				}

			}
		}

		freeImageBuffer((void*)m_Data);
		m_Data = pNewData;
		m_ImgSize.fmt = newFormat;
		return true;
	}

	return false;
}//convertToFormat函数到这里为止．


void set_image_origin(IMAGE_ORIGIN img_origin)
{
	switch(img_origin)
	{
	case IO_LEFT_LOWER:
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		break;
	case IO_DEFAULT:
		ilEnable(IL_ORIGIN_SET);
		break;
	case IO_LEFT_UPPER:
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
		break;
	case IO_NO_CARE:
	default:
		ilDisable(IL_ORIGIN_SET);
	}
}

void reset_image_origin()
{
	switch(g_LibImageOrigin)
	{
	case IO_LEFT_LOWER:
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		break;
	case IO_DEFAULT:
		ilEnable(IL_ORIGIN_SET);
		break;
	case IO_LEFT_UPPER:
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_UPPER_LEFT);
		break;
	case IO_NO_CARE:
	default:
		ilDisable(IL_ORIGIN_SET);
	}
}


bool  xILImage::scale(int w , int h,IMAGE_FILTER filter)
{
	if( w != m_ImgSize.w  || h != m_ImgSize.h )
	{
		ilBindImage(m_ImageID);
		int imgDepth  = ilGetInteger(IL_IMAGE_DEPTH);
		iluImageParameter(ILU_FILTER, (ILenum)GetILFilter(filter) );
		iluScale(w,h,imgDepth);
		__GetImageInfo();
		return true;
	}
	return false;
}

bool xILImage::scaleFit(int maxW, int maxH,IMAGE_FILTER filter)
{
	if( maxW <= m_ImgSize.w  || maxH <= m_ImgSize.h )
	{
		if(m_ImgSize.w / float(m_ImgSize.h) > maxW / float(maxH) )
		{
			int w = maxW;
			int h = (int) (float(m_ImgSize.h) / m_ImgSize.w * w);
			return scale(w , h  ,filter);
		}
		else
		{
			int h = maxH;
			int w = (int) (float(m_ImgSize.w ) / m_ImgSize.h * h);
			return scale(w , h ,filter);
		}
	}
	return true;
}




bool  xILImage::scalePOT(IMAGE_FILTER filter)
{
	int h  = xImageSize::caclTexPOTSize(m_ImgSize.h);
	int w  = xImageSize::caclTexPOTSize(m_ImgSize.w);
	return scale(w, h,filter);
}

bool  xILImage::load(const wchar_t* filename, IMAGE_ORIGIN img_origin)
{
	return load(filename,NULL,0,img_origin);
}


bool  xILImage::load(const wchar_t* wcsFileName, const int8* buf,int buf_len, IMAGE_ORIGIN img_origin )
{
	char ansiFileName[1024] = {0};
	XEvol_UnicodeToFsEnc(wcsFileName , ansiFileName , 1024);

	m_ImgSize.h = 0;
	m_ImgSize.w  = 0;
	m_ImgSize.fmt = PIXELFORMAT_None;

	ILuint   ImageID;

	ILenum   image_type = GetILImageType(wcsFileName);
	if(image_type == IL_TYPE_UNKNOWN)
		return false;

	set_image_origin(img_origin);

	ilGenImages(1,&ImageID);
	ilBindImage(ImageID);


	ILboolean IsLoaded  = false;

	if( buf != NULL && buf_len >0)
	{

		IsLoaded = ilLoadL(image_type,(ILvoid*)buf,buf_len);
	}
	else
	{
		IsLoaded = ilLoadImage( ansiFileName );
	}

	if(! IsLoaded)
	{
		m_ImgSize.h = 0;
		m_ImgSize.w = 0;
		m_ImgSize.fmt = PIXELFORMAT_None;
		m_Data = NULL;
		m_ImageID = -1;
		ilBindImage(0);
		ilDeleteImages(1,(const ILuint*)&ImageID);  
		reset_image_origin();
		return false;
	}

	int actImage = 0;
	//If file is a icon ... he he
	if(image_type == IL_ICO)
	{
		int nImage = ilGetInteger(IL_NUM_IMAGES);
		int nLayer = ilGetInteger(IL_NUM_LAYERS);
		int maxVImage = -1 , maxImageValue = 0;
		for(int i = 0 ; i < nImage ; i ++)
		{
			ilActiveImage(i);
			int w = ilGetInteger(IL_IMAGE_WIDTH);
			int h = ilGetInteger(IL_IMAGE_HEIGHT);
			int v  = w + h;
			v += ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);
			if(v > maxImageValue) 
			{
				maxImageValue = v;
				maxVImage = i;
			}
		}
		actImage= maxVImage;
	}
	ilActiveImage(actImage);

	m_ImgSize.fmt = __getImageFormatFromIL(ilGetInteger(IL_IMAGE_FORMAT) , ilGetInteger(IL_IMAGE_TYPE) , ilGetInteger(IL_IMAGE_BITS_PER_PIXEL));

	if(m_ImgSize.fmt == PIXELFORMAT_None)
	{
		m_Data = NULL;
		m_ImageID = -1;
		ilBindImage(0);
		ilDeleteImages(1,(const ILuint*)&ImageID);  
		reset_image_origin();
		return false;
	}


    //ilActiveImage(actImage);

	
    //ilActiveImage(actImage);
    __GetImageInfo();
	m_ImageID = ImageID;
	reset_image_origin();
	return true;

}

void xILImage::__GetImageInfo()
{
	int actImage = 0;
	ilActiveImage(actImage);
	ILimage* curImage = ilGetCurImage();

	m_ImgSize.w  = ilGetInteger(IL_IMAGE_WIDTH);
	m_ImgSize.h  = ilGetInteger(IL_IMAGE_HEIGHT);
	m_Data = ilGetData();
	m_Data = curImage->Data;
	m_ImgSize.pitch = curImage->Bps;
	m_nByteAlign = (curImage->Bps%4)?1:4;
}
int xILImage::byteAlign()
{
   return m_nByteAlign;
}

bool  xILImage::load(const wchar_t* filename, const int8* buf,int buf_len, ePIXEL_FORMAT format,IMAGE_ORIGIN img_origin)
{
	if(false == load(filename,buf,buf_len,img_origin))
		return false;
    __convertILFormat(format);
	m_Data = ilGetData();
	return true;
}

bool xILImage::__convertILFormat(ePIXEL_FORMAT format)
{
	int imgFormat  = ilGetInteger(IL_IMAGE_FORMAT);
	int newILFormat = getILImageFormat(format);
	if(imgFormat !=  newILFormat)
	{
		m_ImgSize.fmt = format;
		ilBindImage(m_ImageID);
		ilConvertImage(newILFormat,ilGetInteger(IL_IMAGE_TYPE));
	}
	__GetImageInfo();
	return true;
}

bool xILImage::data(const int8 * image_buffer,int mWidth,int mHeight , ePIXEL_FORMAT format)
{
	if(!ilIsImage(m_ImageID))
	{
		ilGenImages(1,(ILuint*)&m_ImageID);
	}
	ilBindImage(m_ImageID);
	int type = IL_UNSIGNED_BYTE;
	if( format == PIXELFORMAT_DXT1   || 
		format == PIXELFORMAT_DXT2  ||
		format == PIXELFORMAT_DXT3  || 
		format == PIXELFORMAT_DXT4  || 
		format == PIXELFORMAT_DXT5)
		type = -1;

	m_ImgSize.fmt = format;
	m_ImgSize.h   = mHeight;
	m_ImgSize.w   = mWidth;

	ilTexImage(mWidth,mHeight,1,xImageSize::getBytePerPixel(format),getILImageFormat(format),type, (void*)image_buffer);
	m_Data = ilGetData();
	return true;
}
bool xILImage::__checkData()
{
	if(ilIsImage(m_ImageID) == IL_FALSE)
	{
		unsigned int8* pOldData = m_Data;
		data((const int8*)m_Data , m_ImgSize.w , m_ImgSize.h, m_ImgSize.fmt);
		freeImageBuffer(pOldData);		
	}
	return true;
};

bool xILImage::save(const wchar_t* filename)
{
	char ansiFileName[1024] = {0};
	XEvol_UnicodeToFsEnc(filename , ansiFileName , 1024);
	ILenum   image_type = GetILImageType(filename);
	if(image_type == IL_TYPE_UNKNOWN)
		return  false;

    __checkData();

	ilBindImage(m_ImageID);
	int type = IL_UNSIGNED_BYTE;
	if( m_ImgSize.fmt == PIXELFORMAT_DXT1  || 
		m_ImgSize.fmt == PIXELFORMAT_DXT2  ||
		m_ImgSize.fmt == PIXELFORMAT_DXT3  || 
		m_ImgSize.fmt == PIXELFORMAT_DXT4  || 
		m_ImgSize.fmt == PIXELFORMAT_DXT5)
		type = -1;
	ilSaveImage( ansiFileName );
	return true;
}


bool     xILImage::save(const wchar_t* filename,const int8 * image_buffer, int mWidth,int mHeight , ePIXEL_FORMAT format)
{
	char ansiFileName[1024] = {0};
	XEvol_UnicodeToFsEnc(filename , ansiFileName , 1024);
	ILuint   ImageID;
	ILenum   image_type = GetILImageType(filename);
	if(image_type == IL_TYPE_UNKNOWN)
		return false ;
	ilGenImages(1,&ImageID);
	ilBindImage(ImageID);
	int type = IL_UNSIGNED_BYTE;
	if(format == PIXELFORMAT_DXT1   || 
		format == PIXELFORMAT_DXT2  ||
		format == PIXELFORMAT_DXT3  || 
		format == PIXELFORMAT_DXT4  || 
		format == PIXELFORMAT_DXT5)
		type = -1;
	ilTexImage(mWidth,mHeight,1,xImageSize::getBytePerPixel(format),getILImageFormat(format),type, (void*)image_buffer);
	ilSaveImage( ansiFileName);
	ilDeleteImages(1,&ImageID);
	return true;
}

//增加加载器
class xILImageLoader : public xImageLib::xImageLoader
{
public:
    xImage* create(const wchar_t* pImageName)
    {
        return new xILImage;
    }
};

static xILImageLoader loader;
const bool bILLoader     = xImageLib::registeImageLoader(L"any" , &loader);

END_NAMESPACE_XEVOL3D

#endif



