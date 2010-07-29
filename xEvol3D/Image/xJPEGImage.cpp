/*
XReal3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xreal3d/) 
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
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string>
#include "xJPEGImage.h"
#include "Color.h"
#include "../BaseLib/xI18N.h"
#include "../OperationSys/xOperationSys.h"
#include "PixelFormatAssit.h"
#include "xImage.h"
extern "C" 
{
#include "jpeglib.h"
}
BEGIN_NAMESPACE_XEVOL3D

xThreadLocker* g_ILLibLocker = NULL;
class xLibJpgAutoLoker
{
public:
	xLibJpgAutoLoker();
	~xLibJpgAutoLoker();
};

xLibJpgAutoLoker::xLibJpgAutoLoker()
{
	if(g_ILLibLocker == NULL)
		g_ILLibLocker = xOperationSys::singleton()->createThreadLocker();
	if(g_ILLibLocker) g_ILLibLocker->lock();
}

xLibJpgAutoLoker::~xLibJpgAutoLoker()
{
	if(g_ILLibLocker) g_ILLibLocker->unlock();
}

//======================
xJpegImage::xJpegImage()
{
	m_nByteAlign = 4;
	m_BytePerPixel = 3;
	setMaxDim(0,0);
	m_Data = NULL;
	free();
}

bool xJpegImage::free()
{
	if(m_Data)
	{
		delete [] m_Data;
		m_Data = NULL;
		m_DesireFmt = PIXELFORMAT_None;
		m_Size.fmt  = PIXELFORMAT_R8G8B8A8;
		m_Size.w = 0;
		m_Size.h = 0;
		m_Size.pitch  = 0;
	}
	return true;
}

xJpegImage::~xJpegImage()
{
	free();
}

void xJpegImage::setMaxDim(int maxWidth , int maxHeight)
{
	m_MaxWidth = maxWidth;
	m_MaxHeight = maxHeight;
}

xJpegImage* xJpegImage::createInstance()
{
	return new xJpegImage;
}

bool xJpegImage::load(xImageSize size)
{
	return load(size.w , size.h , size.fmt);
}

bool xJpegImage::load(int w , int h , ePIXEL_FORMAT fmt)
{
	data(NULL , w  , h , fmt);
	return false;
}



bool xJpegImage::load(const wchar_t* wcsFileName , IMAGE_ORIGIN img_origin , unsigned int _arg )
{
	free();
	return load(wcsFileName , NULL , 0 , img_origin);
}

bool xJpegImage::data(const int8 * image_buffer,int mWidth,int mHeight , ePIXEL_FORMAT _format)
{
	free();
	m_Size.fmt = _format;
	m_Size.h = mHeight;
	m_Size.w  = mWidth;
    int BytePerPixel = xPixelFormatAssit::singleton()->GetPixelFormatBBP(m_Size.fmt)/8;
	m_Size.pitch  = m_Size.RoundnByte( BytePerPixel * mWidth , 4);
	m_Data = new unsigned char[ m_Size.pitch * mHeight] ;
	if(image_buffer)
	{
		memcpy(m_Data , image_buffer ,  m_Size.pitch * mHeight );
	}
	return false;
}

void JpegInitSource(j_decompress_ptr cinfo)
{

}

boolean JpegFillInputBuffer(j_decompress_ptr cinfo)
{
	/*
	jpegError = true;
	jpegSrcManager.bytes_in_buffer = jpegBufferLen;
	jpegSrcManager.next_input_byte = (JOCTET *)jpegBufferPtr;    */
	return TRUE;
}

void JpegSkipInputData(j_decompress_ptr cinfo, long num_bytes)
{/*
 if (num_bytes < 0 || (size_t)num_bytes > jpegSrcManager.bytes_in_buffer) {
 jpegError = true;
 jpegSrcManager.bytes_in_buffer = jpegBufferLen;
 jpegSrcManager.next_input_byte = (JOCTET *)jpegBufferPtr;
 } else {
 jpegSrcManager.next_input_byte += (size_t) num_bytes;
 jpegSrcManager.bytes_in_buffer -= (size_t) num_bytes;
 }*/
}

void JpegTermSource(j_decompress_ptr cinfo)
{
	/* No work necessary here. */
}


bool xJpegImage::load(const wchar_t* wcsFileName,const char* mem_buf , int buf_len, IMAGE_ORIGIN img_origin, unsigned int _arg )
{
	return load(wcsFileName , mem_buf , buf_len , PIXELFORMAT_R8G8B8A8 , img_origin , _arg);
}

int  xJpegImage::__calScaleNum(int w , int h)
{
	if(m_MaxWidth == 0 || m_MaxHeight == 0)
		return 8;
	for(int i = 7 ; i >= 1 ; i --)
	{
		float sc = i/8.0f;
		int nW = (int)(sc * w);
		int nH = (int)(sc * h);
		if(nW <= m_MaxWidth && nH <= m_MaxHeight)
		{
			return i;
		}
	}
	return 1;
}

static ePIXEL_FORMAT GetAvilableFmt(ePIXEL_FORMAT fmt)
{
	if(PIXELFORMAT_R8G8B8 == fmt || PIXELFORMAT_R8G8B8A8 == fmt || 	PIXELFORMAT_R8G8B8X8 == fmt || 
		PIXELFORMAT_B8G8R8 == fmt || PIXELFORMAT_B8G8R8A8 == fmt || 	PIXELFORMAT_B8G8R8X8 == fmt   )
		return fmt;
	return PIXELFORMAT_B8G8R8; 
}

bool xJpegImage::load(const wchar_t* wcsFileName, const int8* mem_buf,int buf_len, ePIXEL_FORMAT format,IMAGE_ORIGIN img_origin, unsigned int _arg)
{
	free();

	jpeg_decompress_struct cinfo;
	struct jpeg_source_mgr jpegSrcManager;
	jpeg_error_mgr jerr;


	// Initialize the JPEG decompression object with default error handling. 


	FILE *input_file = NULL;

	if(mem_buf == NULL)
	{
		input_file = XEvol_OpenFileRB(wcsFileName); //fopen(file_name, "rb");
		if (input_file == NULL)
		{
			return false;
		}
		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);
		jpeg_stdio_src(&cinfo, input_file);

	}
	else
	{

		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);

		jpegSrcManager.init_source = JpegInitSource;
		jpegSrcManager.fill_input_buffer = JpegFillInputBuffer;
		jpegSrcManager.skip_input_data = JpegSkipInputData;
		jpegSrcManager.resync_to_restart = jpeg_resync_to_restart;
		jpegSrcManager.term_source = JpegTermSource;
		jpegSrcManager.next_input_byte = (unsigned char*)mem_buf ;
		jpegSrcManager.bytes_in_buffer = buf_len;
		cinfo.src = &jpegSrcManager;

	}

	if (!jpeg_read_header(&cinfo, TRUE))
	{
		jpeg_destroy_decompress(&cinfo);
		if(input_file ){	fclose(input_file);	}
		return false;
	}

	m_Size.fmt     = GetAvilableFmt(format);
	m_BytePerPixel = xPixelFormatAssit::singleton()->GetPixelFormatBBP(m_Size.fmt)/8;

	cinfo.scale_num = __calScaleNum(cinfo.image_width , cinfo.image_height);
	jpeg_start_decompress(&cinfo);

	m_Size.h  =  cinfo.output_height;
	m_Size.w  =  cinfo.output_width;
	m_Size.pitch = m_Size.RoundnByte(m_Size.w * m_BytePerPixel , 4);
	int _DataLen = m_Size.h * m_Size.pitch;

	//分配图象存储数据空间
	m_Data = new unsigned char[_DataLen];
	unsigned char* pLine = NULL;
	if(m_BytePerPixel == 4)
	{
		pLine = new unsigned char[m_Size.w * 4 ];
	}

	bool bFlip = false;
	if(m_Size.fmt == PIXELFORMAT_B8G8R8A8 ) bFlip = true;
	//一次一行的读出 Jpeg中的图象数据。注意扫描行的顺序和BMP文件相反
	while (cinfo.output_scanline < cinfo.output_height)
	{
		unsigned char *scanline = (unsigned char *)m_Data + m_Size.pitch * ( m_Size.h-cinfo.output_scanline-1);

		int num_scanlines = 0;
		if(m_BytePerPixel == 3)
			num_scanlines = jpeg_read_scanlines(&cinfo, &scanline, 1);
		else
		{
			num_scanlines = jpeg_read_scanlines(&cinfo, &pLine, 1);
			__converTo32Bit(pLine , scanline , m_Size.w , bFlip);
		}

		if(num_scanlines!=1)
		{
			delete [] m_Data;
			m_Data = NULL;
			jpeg_finish_decompress(&cinfo);
			jpeg_destroy_decompress(&cinfo);
			if(input_file ){	fclose(input_file);	}
			if(pLine) delete [] pLine;
			pLine = NULL;
			return false;
		}
	}

	if(m_Size.fmt == PIXELFORMAT_B8G8R8 ) 
	{
		xImage::__flipRGB();
	}

	if(pLine) delete [] pLine; pLine = NULL;
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	if(input_file ){	fclose(input_file);	}
	return true;

}

int xJpegImage::__CompressJPEG(char * filename, const unsigned char *image_buffer,int image_width,int image_height,ePIXEL_FORMAT fmt , int quality)
{
	xLibJpgAutoLoker _aLocker;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * outfile;		/* target file */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	if ((outfile = fopen(filename, "wb")) == NULL) 
	{
		fprintf(stderr, "can't open %s\n", filename);
		return 0;
	}
	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = image_width; 	/* image width and height, in pixels */
	cinfo.image_height = image_height;
	cinfo.input_components = 3;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

	int BPP = xPixelFormatAssit::singleton()->GetPixelFormatBBP(fmt)/8;
	jpeg_set_defaults(&cinfo);

	jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

	jpeg_start_compress(&cinfo, TRUE);

	row_stride = m_Size.RoundnByte(image_width * 3 , 4 ) ;	/* JSAMPLEs per row in image_buffer */

	bool bFlip = false;
	if(fmt == PIXELFORMAT_B8G8R8A8)
	{
		bFlip = true;
	}

	unsigned char* pLine = NULL;
	if(fmt != PIXELFORMAT_R8G8B8 )
	{
		pLine = new unsigned char[image_width * 4 ];
	}

	while (cinfo.next_scanline < cinfo.image_height) 
	{
		if(BPP == 4)
		{
			unsigned char* pSrcLine = (unsigned char*)&image_buffer[(image_height-cinfo.next_scanline-1) * row_stride];
			__converTo24Bit(pSrcLine , pLine , image_width , bFlip);
			row_pointer[0] = pLine;
		}
		else
		{
			unsigned char* pSrcLine = (unsigned char*)&image_buffer[(image_height-cinfo.next_scanline-1) * row_stride];
			if(fmt == PIXELFORMAT_B8G8R8) 
			{ 
				__flipRGB(pSrcLine , pLine , image_width);
				row_pointer[0] = pLine;
			}
			else
			{
				row_pointer[0] = pSrcLine;
			}			
		}
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	if(pLine) { delete [] pLine ; pLine = NULL ; }
	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);
	return 1;
}

bool xJpegImage::save(const wchar_t* wcsFileName , const int8* image_buf  , int w , int h  , ePIXEL_FORMAT format)
{
	int8 file_name[512]={0};
	XEvol_UnicodeToFsEnc(wcsFileName,file_name,512);
	return __CompressJPEG(file_name , (const unsigned char *)image_buf , w , h , format , 100) !=0 ;
}

bool xJpegImage::save(const wchar_t* wcsFileName)
{
	return save(wcsFileName , (const char*)m_Data , m_Size.w , m_Size.h , m_Size.fmt);
}

bool xJpegImage::scale(int w , int h , IMAGE_FILTER filter)
{
	xImage* pImage = xImageLib::create(L"xxx.jpg");
	xImageSize OldSize = getSize();
	pImage->data((const char *)this->data() ,  OldSize.w , OldSize.h , OldSize.fmt);
	pImage->scale(w , h );
	xImageSize NewSize = pImage->getSize();

	if(NewSize.w%4 == 0)
	{
		data( (const char *)pImage->data() , NewSize.w , NewSize.h , GetAvilableFmt(NewSize.fmt)  );
	}
	else
	{
		data( NULL , NewSize.w , NewSize.h , NewSize.fmt  );
		unsigned char* _buf    = (unsigned char*)this->data();
		unsigned char* _srcBuf = (unsigned char*)pImage->data();
		int srcPith = NewSize.pitch;
		int dstPith = m_Size.pitch;
		int minLen = srcPith<dstPith?srcPith:dstPith;
		for(int i = 0 ;i < NewSize.h ; i ++)
		{
			memcpy(_buf + i * dstPith , _srcBuf + i * srcPith , minLen);
		}
	}

	pImage->free();
	pImage->ReleaseObject();
	return false;

	return false;
}

int xJpegImage::byteAlign()
{
	return m_nByteAlign;
}

bool xJpegImage::scalePOT(IMAGE_FILTER filter)
{
	xImageSize _siz = getSize();
	int h  = xImageSize::caclTexPOTSize(_siz.h);
	int w  = xImageSize::caclTexPOTSize(_siz.w);
	return scale(w, h ,filter);
}

void xJpegImage::__converTo32Bit(unsigned char* _pSrc , unsigned char* _pDest , int w , bool bFlip)
{
	unsigned char* pSrc  = _pSrc;
	unsigned char* pDest = _pDest;
	xColor_3ub* pSrcCl  = NULL;
	xColor_4ub* pDestCl = NULL;

	if(bFlip)
	{
		for(int i = 0 ; i < w ; i ++ )
		{
			pSrcCl  = (xColor_3ub*) pSrc;
			pDestCl = (xColor_4ub*) pDest;

			pDestCl->a = 255;
			pDestCl->r = pSrcCl->b;
			pDestCl->g = pSrcCl->g;
			pDestCl->b = pSrcCl->r;
			pSrc  +=3 ;
			pDest +=4 ;
		}
	}
	else
	{
		for(int i = 0 ; i < w ; i ++ )
		{
			pSrcCl  = (xColor_3ub*) pSrc;
			pDestCl = (xColor_4ub*) pDest;

			pDestCl->a = 255;
			pDestCl->r = pSrcCl->r;
			pDestCl->g = pSrcCl->g;
			pDestCl->b = pSrcCl->b;

			pSrc  +=3 ;
			pDest +=4 ;
		}
	}
}

void xJpegImage::__converTo24Bit(unsigned char* _pSrc , unsigned char* _pDest , int w , bool bFlip)
{
	unsigned char* pSrc  = (unsigned char*)_pSrc;
	unsigned char* pDest = (unsigned char*)_pDest;
	xColor_4ub* pSrcCl  = NULL;
	xColor_3ub* pDestCl = NULL;

	if(bFlip)
	{
		for(int i = 0 ; i < w ; i ++ )
		{
			pSrcCl  = (xColor_4ub*)pSrc;
			pDestCl = (xColor_3ub*)pDest;

			pDestCl->r = pSrcCl->b;
			pDestCl->g = pSrcCl->g;
			pDestCl->b = pSrcCl->r;
			pSrc  +=4 ;
			pDest +=3 ;
		}
	}
	else
	{
		for(int i = 0 ; i < w ; i ++ )
		{
			pSrcCl  = (xColor_4ub*)pSrc;
			pDestCl = (xColor_3ub*)pDest;
			pDestCl->r = pSrcCl->r;
			pDestCl->g = pSrcCl->g;
			pDestCl->b = pSrcCl->b;
			pSrc  +=4 ;
			pDest +=3 ;
		}
	}
}

void  xJpegImage::__flipRGB(unsigned char* pSrc , unsigned char* pDest , int w)
{
	unsigned char* pSrcCl  = pSrc;
	unsigned char* pDestCl = pDest;
	for(int i = 0 ; i < w ; i ++ )
	{
		xColor_3ub* pSrcClPixel  = (xColor_3ub*)pSrcCl;
		xColor_3ub* pDestClPixel = (xColor_3ub*)pSrcCl;

		pDestClPixel->r = pSrcClPixel->b;
		pDestClPixel->g = pSrcClPixel->g;
		pDestClPixel->b = pSrcClPixel->r;

		pSrcCl  +=3 ;
		pDestCl +=3 ;
	}
}

bool xJpegImage::scaleFit(int maxW , int maxH , IMAGE_FILTER filter)
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

void* xJpegImage::data(size_t mipmapLevel, size_t cubeMapImageIndex,  bool bCompressed)
{
	return m_Data;
}


bool xJpegImage::convert(ePIXEL_FORMAT newFmt)
{
	if(m_Data == NULL)
		return false;

	ePIXEL_FORMAT old_fmt = m_Size.fmt;

	if(newFmt == old_fmt)
		return true;

	return false;
}

xImageSize xJpegImage::getSize()
{
	xImageSize _siz ;

	if(m_Data == NULL)
	{
		_siz.fmt = PIXELFORMAT_None;
		_siz.h = 0 ;
		_siz.w = 0;
		_siz.pitch = 0;
		return _siz;
	}

	return m_Size;
}


size_t xJpegImage::nMipmapLevel()
{
	return 1;
}

size_t xJpegImage::nImage()
{
	return 1;
}
size_t xJpegImage::nSlice()
{
	return 1;
}

const wchar_t*   xJpegImage::supportExt(bool bSave)
{
	if(bSave)
	{
		static wchar_t* saveExt = L"JPEG JPG";
		return saveExt;
	}
	else
	{
		static wchar_t* loadExt = L"JPEG JPG";
		return loadExt;
	}
}

bool  xJpegImage::getSize(const wchar_t* wcsFileName , xImageSize& _size)
{

	jpeg_decompress_struct cinfo;
	jpeg_error_mgr jerr;
	FILE *input_file;
	input_file = XEvol_OpenFileRB(wcsFileName);//, "rb");
	if (input_file == NULL)
	{
		return false;
	}
	// Initialize the JPEG decompression object with default error handling. 
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	jpeg_stdio_src(&cinfo, input_file);
	if (!jpeg_read_header(&cinfo, TRUE))
	{
		fclose(input_file);
		//jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		return false;
	}

	_size.h  =  cinfo.image_width;
	_size.w  =  cinfo.image_height;
	_size.pitch = _size.RoundnByte(_size.w * 3 , 4);
	//jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(input_file);
	return true;
}

//增加加载器
class xJpegImageLoader : public xImageLib::xImageLoader
{
public:
    xImage* create(const wchar_t* pImageName)
    {
        return new xJpegImage;
    }
};

class xJpegImageInfoLoader : public xImageLib::xImageInfoLoader
{
    bool getSize(const wchar_t* wcsFileName , xImageSize& _size)
    {
        return xJpegImage::getSize(wcsFileName , _size);
    }
};


static xJpegImageLoader loader;
static xJpegImageInfoLoader infoLoader;

const bool bJpegLoader     = xImageLib::registeImageLoader(L"jpeg" , &loader);
const bool bJpgLoader      = xImageLib::registeImageLoader(L"jpg" , &loader);
const bool bJpegSpecLoader = xImageLib::registeSpecialImageLoader(L"jpeg" , &loader);
const bool bJpgSpecLoader  = xImageLib::registeSpecialImageLoader(L"jpg" , &loader);
const bool bJpegInfoLoader = xImageLib::registeImageInfoLoader(L"jpeg" , &infoLoader);
const bool bJpgInfoLoader  = xImageLib::registeImageInfoLoader(L"jpg"  , &infoLoader);


END_NAMESPACE_XEVOL3D

#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib , "libjpeg_debug.lib")
#else
#pragma comment(lib , "libjpeg.lib")
#endif
#endif
