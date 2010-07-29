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

#include<stdio.h>
#include<math.h>
#include<ctype.h>
#include<string.h>
#include "../io/mstream.h"
#include "xImage.h"
#include "xHDRImage.h"
BEGIN_NAMESPACE_XEVOL3D

#define RGBE_VALID_PROGRAMTYPE   0x01
#define RGBE_VALID_GAMMA         0x02
#define RGBE_VALID_EXPOSURE      0x04


typedef struct
{
	int   valid;
	int8  programType[16];
	float gamma;
	float exposure;
} rgbeHeader;

static void DecodeRGBE(unsigned int8 *src, float *dst)
{
	if(src[3])
	{
		float exp = (float)ldexp(1.0, src[3] - (128 + 8));
		dst[0] = src[0] * exp;
		dst[1] = src[1] * exp;
		dst[2] = src[2] * exp;
	}
	else
	{
		dst[0] = 0; dst[1] = 0; dst[2] = 0;
	}
}




static bool ReadRGBEHeader(const wchar_t* fileName , std::istream& _stream , int *width, int *height, rgbeHeader *header)
{
	int8 buffer[128] = {0};
	float temp;

	if(header)
	{
		header->valid = 0;
		header->programType[0] = 0;
		header->gamma = 1.0f;
		header->exposure = 1.0f;
	}

	_stream.getline(buffer, sizeof(buffer) );

	if(header)
	{
		header->valid |= RGBE_VALID_PROGRAMTYPE;

		int i = 0;
		for( i = 0; i < sizeof(header->programType) - 1; i++)
		{
			if(isspace(buffer[i + 2]) || (buffer[i + 2] == 0)) break;
			header->programType[i] = buffer[i + 2];
		}

		header->programType[i] = 0;

		_stream.getline(buffer, sizeof(buffer) );
	}

	while(1)
	{
		if((buffer[0] == 0)) break;

		else if(strcmp(buffer, "\n") == 0) break;
		else if(strcmp(buffer, "FORMAT=32-bit_rle_rgbe\n") == 0) 
		{ 
		}
		else if(header && (sscanf(buffer, "GAMMA=%g", &temp) == 1))
		{
			header->valid |= RGBE_VALID_GAMMA;
			header->gamma = temp;
		}
		else if(header && (sscanf(buffer, "EXPOSURE=%g", &temp) == 1))
		{
			header->valid |= RGBE_VALID_EXPOSURE;
			header->exposure = temp;
		}

		_stream.getline(buffer, sizeof(buffer) );
	}

	_stream.getline(buffer, sizeof(buffer) );
	if(sscanf(buffer, "-Y %d +X %d", height, width) < 2)
		return false;

	return true;
}


bool ReadRGBEUncompressed(const wchar_t* fileName , std::istream& _stream, float *img, int size)
{
	unsigned int8 rgbe[4];

	while(size > 0)
	{
		_stream.read( (int8*)rgbe , sizeof(rgbe));
		DecodeRGBE(&rgbe[0], img);
		img += 3;
		size--;
	}

	return true;
}


bool ReadRGBERLE(const wchar_t* fileName , std::istream& _stream, float *image, int width, int height, bool decode)
{
	unsigned int8 rgbe[4], *buffer = NULL, *ptr, *ptr_end;
	int count;
	unsigned int8 buf[2];

	if(width < 8 || width > 0x7fff)
		return ReadRGBEUncompressed(fileName ,_stream, image, width * height);

	while(height > 0)
	{
		_stream.read( (int8*)rgbe , sizeof(rgbe));

		if(rgbe[0] != 2 || rgbe[1] != 2 || rgbe[2] & 0x80)
		{
			DecodeRGBE(&rgbe[0], image);
			image += 3;

			if(buffer) delete[] buffer;
			return ReadRGBEUncompressed(fileName ,_stream, image, width * height - 1);
		}

		if(((int)rgbe[2] << 8 | rgbe[3]) != width)
		{
			if(buffer) delete[] buffer;
			return false;
		}

		if(!buffer)
			buffer = new unsigned int8[sizeof(unsigned int8) * width * 4];

		if(!buffer) return false;
		ptr = &buffer[0];

		int i = 0;
		for( i = 0; i < 4; i++)
		{
			ptr_end = &buffer[(i + 1) * width];

			while(ptr < ptr_end)
			{
				_stream.read( (int8*)buf, sizeof(buf[0]) * 2);

				if(buf[0] > 128)
				{
					count = buf[0] - 128;

					if(count == 0 || count > ptr_end - ptr)
					{
						delete[] buffer;
						return false;
					}

					while(count > 0)
					{
						*ptr++ = buf[1];
						count--;
					}
				}
				else
				{
					count = buf[0];

					if(count == 0 || count > ptr_end - ptr)
					{
						delete[] buffer;
						return false;
					}

					*ptr++ = buf[1];
					count--;

					if(count > 0)
					{
						_stream.read( (int8*)ptr, sizeof(*ptr) * count );
						ptr += count;
					}
				}
			}
		}

		for(i = 0; i < width; i++)
		{
			rgbe[0] = buffer[i];
			rgbe[1] = buffer[i + width];
			rgbe[2] = buffer[i + 2 * width];
			rgbe[3] = buffer[i + 3 * width];

			if(decode) DecodeRGBE(rgbe, image);
			else
			{
				image[0] = rgbe[0];
				image[1] = rgbe[1];
				image[2] = rgbe[2];
				image[3] = rgbe[3];
			}
			image += 3;
		}

		height--;
	}

	delete[] buffer;
	return true;
}

bool xHDRImage::__loadHDRFile(const wchar_t* fileName , std::istream& _stream , IMAGE_ORIGIN img_origin)
{
	rgbeHeader header;
	int width = 0, height = 0;



	ReadRGBEHeader(fileName , _stream , &width, &height, &header);

	m_Data = (int8*)(new float[width * height * 3]);
	ReadRGBERLE(fileName , _stream, (float*)m_Data, width, height, true);

	m_imgSize.w = width;
	m_imgSize.h = height;
	m_imgSize.pitch = 16 * m_imgSize.w;
	m_imgSize.fmt = PIXELFORMAT_R32G32B32A32F;
	return m_Data != NULL;
}

xHDRImage::xHDRImage()
{
    m_exposure = 1.0f;
	m_Data  = NULL;
}

xHDRImage::~xHDRImage()
{
    free();
}

int xHDRImage::byteAlign()
{
     return 4;
}

bool xHDRImage::load(const wchar_t* fileName, IMAGE_ORIGIN img_origin, unsigned int _arg)
{
	m_ImgOrigin = img_origin;
	std::ifstream _stream;

#ifdef _WIN32
	_stream.open(fileName , std::ios::binary);
#else
	int8 mbsFileName[512]={0};
	XEvol_UnicodeToFsEnc(fileName,mbsFileName,512);
	_stream.open(mbsFileName , std::ios::binary);
#endif

	if(_stream.fail())
	{
		return false;
	}
	return __loadHDRFile(fileName , _stream);
}

bool xHDRImage::load(const wchar_t* fileName, const int8* mem_buf , int buf_len, IMAGE_ORIGIN img_origin, unsigned int _arg)
{
	m_ImgOrigin = img_origin;
	if(mem_buf)
	{
		std::imemstream _stream;
		_stream.set_buf((int8*)mem_buf , buf_len);
		return __loadHDRFile(fileName , _stream);
	}
	return load(fileName , img_origin);
}

bool xHDRImage::load(const wchar_t* fileName, const int8* mem_buf , int buf_len, ePIXEL_FORMAT format, IMAGE_ORIGIN img_origin, unsigned int _arg)
{
    return load(fileName , mem_buf , buf_len , img_origin);
}


bool xHDRImage::free()
{
   delete [] m_Data;
   m_Data  = NULL;
   return true;
}



xImageSize xHDRImage::getSize()
{
    return m_imgSize;
}


void* xHDRImage::data(size_t mipmapLevel, size_t cubeMapImageIndex /*-1 means not a cubemap*/ , bool bCompressed) 
{
    if(mipmapLevel == 0 && cubeMapImageIndex == 0)
		return m_Data;
	return NULL;
}


bool xHDRImage::data(const int8 * image_buffer,int mWidth,int mHeight , ePIXEL_FORMAT format)
{
    return false;
}


size_t xHDRImage::nMipmapLevel()
{
    return 1;
}


size_t xHDRImage::nImage()
{
   return 1;
}


size_t xHDRImage::nSlice()
{
   return 1;
}

const wchar_t*   xHDRImage::supportExt(bool bSave)
{
	if(bSave)
	{
		return NULL;
	}
	else
	{
		static wchar_t* loadExt = L"hdr";
		return loadExt;
	}
}

//Ôö¼Ó¼ÓÔØÆ÷
class xHDRmageLoader : public xImageLib::xImageLoader
{
public:
    xImage* create(const wchar_t* pImageName)
    {
        return new xHDRImage;
    }
};

static xHDRmageLoader loader;
const bool bHDRLoader     = xImageLib::registeImageLoader(L"hdr" , &loader);

END_NAMESPACE_XEVOL3D
