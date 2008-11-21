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
#define FREEIMAGE_BIGENDIAN
#include <FreeImage.h>
#include "xFreeImage.h"
#include "Color.h"
#include "../BaseLib/xI18N.h"
BEGIN_NAMESPACE_XEVOL3D
static bool GetFreeImageType(ePIXEL_FORMAT fmt, FREE_IMAGE_TYPE& type , int& bpp)
{
	switch(fmt)
	{
		//Classic Texture format
	case PIXELFORMAT_R8G8B8:
		type = FIT_BITMAP;
		bpp  = 24;
		return true;
		break;

	case PIXELFORMAT_R8G8B8A8:
		type = FIT_BITMAP;
		bpp  = 32;
		return true;
		break;
	case PIXELFORMAT_R8G8B8X8:
		type = FIT_BITMAP;
		bpp  = 32;
		return true;
		break;

	case PIXELFORMAT_B8G8R8:
		type = FIT_BITMAP;
		bpp  = 24;
		return true;
		break;

	case PIXELFORMAT_B8G8R8A8:
		type = FIT_BITMAP;
		bpp  = 32;
		return true;
		break;

	case PIXELFORMAT_B8G8R8X8:
		type = FIT_BITMAP;
		bpp  = 32;
		return true;
		break;

		//16bit interger texture format
	case PIXELFORMAT_R16G16B16U:
    case PIXELFORMAT_R16G16B16S:
case PIXELFORMAT_R16G16B16F:
		type = FIT_RGB16;
		bpp  = 48;
		return true;
		break;
	case PIXELFORMAT_R16G16B16A16F:
	case PIXELFORMAT_R16G16B16A16U:
	case PIXELFORMAT_R16G16B16A16S:
		type = FIT_RGBA16;
		bpp  = 64;
		return true;
		break;


		//32bit float texture format
	case PIXELFORMAT_R32G32B32F:
		type = FIT_RGBF;
		bpp  = 96;
		return true;
		break;
	case PIXELFORMAT_R32G32B32A32F:
		type = FIT_RGBAF;
		bpp  = 128;
		return true;
		break;

	//Single
	case PIXELFORMAT_ALPHA8:
		bpp =  8;
		return false;
		break;

	case PIXELFORMAT_LUMINANCE8:
		bpp =  8;
		return false;
		break;

	case PIXELFORMAT_LUMINANCE8ALPHA8:
		bpp =  16;
		return false;
		break;

	case PIXELFORMAT_LUMINANCE16:
		bpp =  16;
		return false;
		break;

	case PIXELFORMAT_LUMINANCE16F:
		bpp =  16;
		return false;
		break;

	case PIXELFORMAT_LUMINANCE32F:
		bpp =  32;
		return false;
		break;


	case PIXELFORMAT_R16F:
		bpp =  16;
		return false;
		break;
	case PIXELFORMAT_R16G16F:
		bpp =  32;
		return false;
		break;

	case PIXELFORMAT_R32F:
		bpp =  32;
		return false;
		break;
	case PIXELFORMAT_R32G32F:
		bpp =  64;
		return false;
		break;

	case PIXELFORMAT_None:
		break;
	}
	return false;
}




//======================
xFreeImage::xFreeImage()
{
	m_nByteAlign = 4;
	m_pFreeImage = NULL;
}

xFreeImage::~xFreeImage()
{
	free();
}

bool xFreeImage::load(xImageSize size)
{
    return load(size.w , size.h , size.fmt);
}

bool xFreeImage::load(int w , int h , ePIXEL_FORMAT fmt)
{
	FREE_IMAGE_TYPE type;
	int bpp = 32;
	if(false == GetFreeImageType(fmt,type,bpp))
		return false;
	m_pFreeImage = FreeImage_AllocateT(type,w,h,bpp);
	m_Fmt = __GetImagePixelFmt();
	return m_pFreeImage != NULL;
}

bool xFreeImage::free()
{
	if(m_pFreeImage == NULL) return false;
	FreeImage_Unload(m_pFreeImage);
	m_pFreeImage = NULL;
	return true;
}

bool xFreeImage::load(const wchar_t* wcsFileName,IMAGE_ORIGIN img_origin)
{
	int8 fileName[512]={0};
	XEvol_UnicodeToLocale(wcsFileName,fileName,512);

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	// check the file signature and get its format
	// (the second argument is currently not used by FreeImage)
	fif = FreeImage_GetFileType(fileName, 0);
	if(fif == FIF_UNKNOWN) 
	{
		// no signature ?
		// try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(fileName);
	}
	// check that the plugin has reading capabilities ...
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) 
	{
		m_pFreeImage = FreeImage_Load(fif, fileName, 0);
		m_Fmt = __GetImagePixelFmt();
		return m_pFreeImage != NULL;
	}
	return false;
}

bool xFreeImage::data(const int8 * image_buffer,int mWidth,int mHeight , ePIXEL_FORMAT format)
{
     free();
     load(mWidth, mHeight,format);
	 m_Fmt = format;
	 char* _data = (char*)data();
	 memcpy(_data , image_buffer , mHeight * getSize().pitch );
	 return true;
}

bool xFreeImage::load(const wchar_t* fileName,const char* mem_buf , int buf_len, IMAGE_ORIGIN img_origin)
{
	if(mem_buf == NULL)
	{
		return load(fileName,img_origin);
	}


	FIMEMORY* pMem =  FreeImage_OpenMemory((BYTE*)mem_buf,buf_len);
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileTypeFromMemory(pMem,buf_len);
	if(fif == FIF_UNKNOWN)
	{
		FreeImage_CloseMemory(pMem);
		return false;
	}

	m_pFreeImage = FreeImage_LoadFromMemory(fif , pMem , 0);
	m_Fmt = __GetImagePixelFmt();
	return m_pFreeImage != NULL;
}

bool xFreeImage::load(const wchar_t* filename, const int8* buf,int buf_len, ePIXEL_FORMAT format,IMAGE_ORIGIN img_origin)
{
   if(load(filename, buf , buf_len , img_origin) )
   {
        return convert(format);
   }
   return false;
}

bool xFreeImage::save(const wchar_t* wcsFileName)
{
	int8 fileName[512]={0};
	XEvol_UnicodeToLocale(wcsFileName,fileName,512);

	if(m_pFreeImage == NULL) return false;
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	bool bSuccess = FALSE;

	// Try to guess the file format from the file extension
	fif = FreeImage_GetFIFFromFilename(fileName);
	if(fif != FIF_UNKNOWN ) 
	{
		// Check that the dib can be saved in this format
		bool bCanSave;

		FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(m_pFreeImage);
		if(image_type == FIT_BITMAP) 
		{
			// standard bitmap type
			int bpp = FreeImage_GetBPP(m_pFreeImage);
			bCanSave = (FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp));
		}
		else 
		{
			// special bitmap type
			bCanSave = FreeImage_FIFSupportsExportType(fif, image_type) != FALSE;
		}

		if(bCanSave) 
		{
			bSuccess = FreeImage_Save(fif, m_pFreeImage, fileName, 0) != FALSE;
			return bSuccess;
		}
	}
	return bSuccess;
}

bool xFreeImage::scale(int w , int h , IMAGE_FILTER filter)
{
	if(m_pFreeImage == NULL) return false;

	FIBITMAP* pNewBitMap =  FreeImage_Rescale(m_pFreeImage , w, h , (FREE_IMAGE_FILTER)filter);
	return __replace(pNewBitMap);
}
int xFreeImage::byteAlign()
{
	return m_nByteAlign;
}
bool xFreeImage::scalePOT(IMAGE_FILTER filter)
{
	xImageSize _siz = getSize();
	int h  = xImageSize::caclTexPOTSize(_siz.h);
	int w  = xImageSize::caclTexPOTSize(_siz.w);
	return scale(w, h ,filter);
}

bool xFreeImage::scaleFit(int maxW , int maxH , IMAGE_FILTER filter)
{
	xImageSize _siz = getSize();
	if( maxW <= _siz.w  || maxH <= _siz.h )
	{
		if(_siz.w / float(_siz.h) > maxW / float(maxH) )
		{
			int w = maxW;
			int h = (int) (float(_siz.h) / _siz.w * w);
			return scale(w , h ,filter);
		}
		else
		{
			int h = maxH;
			int w = (int) (float(_siz.w ) / _siz.h * h);
			return scale(w , h ,filter);
		}
	}
	return true;
}

void* xFreeImage::data(size_t mipmapLevel, size_t cubeMapImageIndex,  bool bCompressed)
{
	if(m_pFreeImage == NULL)
		return NULL;
	return FreeImage_GetBits(m_pFreeImage);
}

bool xFreeImage::flipRGBA(int Bpp)
{
	BYTE* pData = FreeImage_GetBits(m_pFreeImage);
	int   pitch = FreeImage_GetWidth(m_pFreeImage) * Bpp;
	int   h     = FreeImage_GetHeight(m_pFreeImage);
	for(int y = 0 ;  y < h ; y ++)
	{
		for(int i = 0  ; i < pitch ; i +=Bpp )
		{
			char r = pData[i + pitch *y     ];
			char b = pData[i + pitch *y  + 2];
			pData[i + pitch *y  + 2] = r;
			pData[i + pitch *y     ] = b;
		}
	}
	return true;
}
bool xFreeImage::convert(ePIXEL_FORMAT newFmt)
{
	if(m_pFreeImage == NULL)
		return false;
	ePIXEL_FORMAT old_fmt = m_Fmt;

	if(newFmt == old_fmt)
		return true;

	if(old_fmt == PIXELFORMAT_B8G8R8A8 && newFmt == PIXELFORMAT_R8G8B8A8)
	{
         flipRGBA(4);
		 m_Fmt = newFmt;
		 return true;
	}

	if(old_fmt == PIXELFORMAT_B8G8R8 && newFmt == PIXELFORMAT_R8G8B8)
	{
        flipRGBA(3);	
		m_Fmt = newFmt;
		return true;
	}

	if(old_fmt == PIXELFORMAT_B8G8R8 && newFmt == PIXELFORMAT_B8G8R8A8)
	{
		FIBITMAP* pBitMap = FreeImage_ConvertTo32Bits(m_pFreeImage);
		m_Fmt = newFmt;
		return __replace(pBitMap);
	}

	if(newFmt == PIXELFORMAT_B8G8R8 && old_fmt == PIXELFORMAT_B8G8R8A8)
	{
		FIBITMAP* pBitMap = FreeImage_ConvertTo24Bits(m_pFreeImage);
		m_Fmt = newFmt;
		return __replace(pBitMap);
	}

	if(newFmt == PIXELFORMAT_R32G32B32F )
	{

		FIBITMAP* pBitMap = FreeImage_ConvertToRGBF(m_pFreeImage);//
		m_Fmt = newFmt;
		return __replace(pBitMap);
	}
	return false;
}

xImageSize xFreeImage::getSize()
{
	xImageSize _siz ;

	if(m_pFreeImage == NULL)
	{
		_siz.fmt = PIXELFORMAT_None;
		_siz.h = 0 ;
		_siz.w = 0;
		_siz.pitch = 0;
		return _siz;
	}
	_siz.fmt   = m_Fmt;
	_siz.h     = FreeImage_GetHeight(m_pFreeImage);
	_siz.w     = FreeImage_GetWidth(m_pFreeImage);
	_siz.pitch = FreeImage_GetPitch(m_pFreeImage);
	return _siz;
}

bool xFreeImage::__replace(FIBITMAP *new_dib) 
{
	if(new_dib == NULL) 
		return false;
	if(m_pFreeImage)
		FreeImage_Unload(m_pFreeImage);
	m_pFreeImage = new_dib;
	return TRUE;
}

ePIXEL_FORMAT xFreeImage::__GetImagePixelFmt()
{
	if(m_pFreeImage == NULL)
		return PIXELFORMAT_None;


	FREE_IMAGE_TYPE type = FreeImage_GetImageType(m_pFreeImage);
	int bpp = FreeImage_GetBPP(m_pFreeImage);
	switch(type)
	{
	case FIT_BITMAP:	// standard image			: 1-, 4-, 8-, 16-, 24-, 32-bit
		switch(bpp)
		{
		case 8:
			return PIXELFORMAT_LUMINANCE8;
		case 24:
			return PIXELFORMAT_B8G8R8;
		case 32:
			return PIXELFORMAT_B8G8R8A8;
		default:
			return PIXELFORMAT_None;
		};
		break;

	case FIT_UINT16:	// array of unsigned short	: unsigned 16-bit
		return PIXELFORMAT_LUMINANCE16;
		break;

	case FIT_INT16:	// array of short			: signed 16-bit
		return PIXELFORMAT_LUMINANCE16;
		break;

	case FIT_UINT32:	// array of unsigned long	: unsigned 32-bit
		return PIXELFORMAT_LUMINANCE32F;
		break;

	case FIT_INT32:	// array of long			: signed 32-bit
		return PIXELFORMAT_LUMINANCE32F;
		break;

	case FIT_FLOAT:	// array of float			: 32-bit IEEE floating point
		return PIXELFORMAT_LUMINANCE32F;
		break;

	case FIT_DOUBLE:	// array of double			: 64-bit IEEE floating point
		return PIXELFORMAT_None;
		break;

	case FIT_COMPLEX:	// array of FICOMPLEX		: 2 x 64-bit IEEE floating point
		return PIXELFORMAT_None;
		break;

	case FIT_RGB16:	// 48-bit RGB image			: 3 x 16-bit
		return PIXELFORMAT_R16G16B16F;
		break;

	case FIT_RGBA16:	// 64-bit RGBA image		: 4 x 16-bit
		return PIXELFORMAT_R16G16B16A16F;
		break;

	case FIT_RGBF:	// 96-bit RGB float image	: 3 x 32-bit IEEE floating point
		return PIXELFORMAT_R32G32B32F;
		break;

	case FIT_RGBAF :	// 128-bit RGBA float image	: 4 x 32-bit IEEE floating point
		return PIXELFORMAT_R32G32B32A32F;
		break;
	default:
		return PIXELFORMAT_None;
	}
}
size_t xFreeImage::nMipmapLevel()
{
   return 1;
}

size_t xFreeImage::nImage()
{
   return 1;
}
size_t xFreeImage::nSlice()
{
	return 1;
}

const wchar_t*   xFreeImage::supportExt(bool bSave)
{
	if(bSave)
	{
		static wchar_t* saveExt = L"JPEG JPG BMP PNG GIF TGA DDS PCX ";
		return saveExt;
	}
	else
	{
		static wchar_t* loadExt = L"JPEG JPG BMP PNG GIF TGA DDS PCX TIF TIFF HDR";
		return loadExt;
	}
}

END_NAMESPACE_XEVOL3D
#ifdef _WIN32
#pragma comment(lib , "FreeImage.lib")
#endif
