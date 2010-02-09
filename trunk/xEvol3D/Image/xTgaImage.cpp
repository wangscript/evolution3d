// ==========================================================
// TARGA Loader and Writer
//
// Design and implementation by
// - Floris van den Berg (flvdberg@wxs.nl)
// - Jani Kajala (janik@remedy.fi)
// - Martin Weber (martweb@gmx.net)
// - Machiel ten Brinke (brinkem@uni-one.nl)
// - Peter Lemmens (peter.lemmens@planetinternet.be)
// - Herv?Drolon (drolon@infonie.fr)
//
// This file is part of FreeImage 3
//
// COVERED CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY
// OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES
// THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE
// OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED
// CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT
// THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY
// SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL
// PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER
// THIS DISCLAIMER.
//
// Use at your own risk!
// ==========================================================
#include "../xStdPch.h"
#include "Color.h"
#include "xImage.h"
#include "xTgaImage.h"
#include "endian.h" 
#include <assert.h>
// ----------------------------------------------------------
//   Constants + headers
// ----------------------------------------------------------
#ifndef _MSC_VER
// define portable types for 32-bit / 64-bit OS
#include <inttypes.h>
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;
#else
// MS is not C99 ISO compliant
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
#endif // _MSC_VER

#ifdef _WIN32
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif


BEGIN_NAMESPACE_XEVOL3D
struct  TGAHEADER
{
	BYTE id_length;				// ID length
	BYTE color_map_type;		// color map type
	BYTE image_type;			// image type

	WORD cm_first_entry;		// first entry index
	WORD cm_length;				// color map length
	BYTE cm_size;				// color map entry size, in bits

	WORD is_xorigin;			// X-origin of image
	WORD is_yorigin;			// Y-origin of image
	WORD is_width;				// image width
	WORD is_height;				// image height
	BYTE is_pixel_depth;		// pixel depth
	BYTE is_image_descriptor;	// image descriptor
} ;

typedef struct tagTGAFOOTER {
	DWORD extension_offset;	// extension area offset
	DWORD developer_offset;	// developer directory offset
	char signature[18];		// signature string
} TGAFOOTER;

struct FILE_BGRA
{
    unsigned char b,g,r,a;
};

struct FILE_BGR 
{
    unsigned char b,g,r;
} ;

#ifdef _WIN32
#pragma pack(pop)
#else
#pragma pack()
#endif

// ----------------------------------------------------------
// Image type
//
#define TGA_NULL		0	// no image data included
#define TGA_CMAP		1	// uncompressed, color-mapped image
#define TGA_RGB			2	// uncompressed, true-color image
#define TGA_MONO		3	// uncompressed, black-and-white image
#define TGA_RLECMAP		9	// run-length encoded, color-mapped image
#define TGA_RLERGB		10	// run-length encoded, true-color image
#define TGA_RLEMONO		11	// run-length encoded, black-and-white image
#define TGA_CMPCMAP		32	// compressed (Huffman/Delta/RLE) color-mapped image (e.g., VDA/D) - Obsolete
#define TGA_CMPCMAP4	33	// compressed (Huffman/Delta/RLE) color-mapped four pass image (e.g., VDA/D) - Obsolete


#define TGABITMAP_ALIGNMENT 4

// ==========================================================
// Internal functions
// ==========================================================
inline int CalculateLine(int width, int bitdepth)
{
    return ((width * bitdepth) + 7) / 8;
}

inline int CalculatePitch(int line) 
{
    return line + 3 & ~3;
}

inline unsigned char * CalculateScanLine(unsigned char *bits, unsigned pitch, int scanline)
{
    return (bits + (pitch * scanline));
}


unsigned TgaImage_GetLine(TgaImageData *dib) 
{
    return dib ? (( dib->biWidth * dib->biBitCount) + 7) / 8 : 0;
}

unsigned TgaImage_GetImageSize(int width, int height, int bpp) 
{
    unsigned dib_size =  CalculatePitch(CalculateLine(width, bpp)) * height; 
    return dib_size;
}
unsigned TgaImage_GetPitch(TgaImageData *dib) 
{
    return dib ? TgaImage_GetLine(dib) + 3 & ~3 : 0;
}

BYTE *  TgaImage_GetScanLine(TgaImageData *dib, int scanline) 
{
    return (dib) ? CalculateScanLine(dib->data, TgaImage_GetPitch(dib), scanline) : NULL;
}


#ifdef XEVOL3D_BIGENDIAN
static void SwapHeader(TGAHEADER *header) 
{
	xSwapShort(&header->cm_first_entry);
	xSwapShort(&header->cm_length);
	xSwapShort(&header->is_xorigin);
	xSwapShort(&header->is_yorigin);
	xSwapShort(&header->is_width);
	xSwapShort(&header->is_height);
}

static void SwapFooter(TGAFOOTER *footer) 
{
	xSwapUInt(&footer->extension_offset);
	xSwapUInt(&footer->developer_offset);
}
#endif

// ==========================================================
// Plugin Interface
// ==========================================================

static int s_format_id;

#if (defined(_WIN32) || defined(__WIN32__))
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif // WIN32


static bool  Validate(IStreamIO *io) 
{
	TGAHEADER header;
	char extra[256]; // 256 temporary bytes (only 255 will be used though)

	// try to read the header in a whole
	if(io->read(header) != sizeof(TGAHEADER) )
		return false;

#ifdef XEVOL3D_BIGENDIAN
	SwapHeader(&header);
#endif

	// The Color Map Type should be a 0 or a 1...
	// NOTE: are other values possible?
	//   NO, according to http://cvs.sf.net/viewcvs.py/eifogl/eifogl/utility/tga/tga_file.e?rev=1.3
	//       and it seems that the current Targa implementation for FreeImage
	//       assumes that anything other than 0 means that there is a Color Map
	if(header.color_map_type != 0 && header.color_map_type != 1)
		return false;

	// If the Color Map Type is 1 then we validate the map entry information...
	// NOTA: let's stay compatible with the current FreeImage Targa implementation
	//       and check against Type 0 instead against Type 1
	if(header.color_map_type != 0) {
		// It doesn't make any sense if the first entry is larger than the Color Map Table
		if(header.cm_first_entry >= header.cm_length)
			return false;

		// NOTE : maybe we should check header.cm_size for known values?
		//        for now, don't allow 0 or anything bigger than 32
		if(header.cm_size <= 0 || header.cm_size > 32)
			return false;
	}
	else {
		// I have disabled this because I'm not really sure
		// but can one assume that if there is no Color Map
		// than all 3 related values are zeroed?
		// NOTE: atleast in my test files that seems to be the case...
		/*
		if(header.cm_first_entry != 0 || header.cm_length != 0 || header.cm_size != 0)
			return false;
		*/
	}

	// the width/height shouldn't be 0, right?
	if(header.is_width == 0 || header.is_height == 0)
		return false;

	// the extra data (following after the header) should be there
	if(io->read(extra, header.id_length) != header.id_length)
		return false;

	// let's now verify all the types that are supported by FreeImage
	// NOTE : this is our final verification
	switch(header.is_pixel_depth)
    {
		case 8: 
			switch(header.image_type) 
            {
			  case TGA_CMAP:
			  case TGA_MONO:
			  case TGA_RLECMAP:
			  case TGA_RLEMONO: return true;
			}
			break;
		case 15:
		case 16:
		case 24:
		case 32: 
			switch(header.image_type)
            {
				case TGA_RGB:
				case TGA_RLERGB: return true;
			}
	}

	return false;
}



void* TgaImage_Aligned_Malloc(size_t amount, size_t alignment) 
{
    assert(alignment == TGABITMAP_ALIGNMENT);
    return _aligned_malloc(amount, alignment);
}

bool xTgaImage::Unload()
{
    ::_aligned_free(m_TgaBitmap.data);
    memset(&m_TgaBitmap , 0 ,sizeof(m_TgaBitmap) );
    return true;
}

xColor_4ub* xTgaImage::GetScanLine(int scanline, int flipvert) 
{
    //assert ((scanline >= 0) && (scanline < (int)FreeImage_GetHeight(dib)));

    if (flipvert)
        return (xColor_4ub*)TgaImage_GetScanLine(&m_TgaBitmap, scanline);
    else
        return (xColor_4ub*)TgaImage_GetScanLine(&m_TgaBitmap, m_TgaBitmap.biHeight - scanline - 1);
}

bool xTgaImage::Allocate(int width, int height, int bpp) 
{
    height = abs(height);
    // check pixel bit depth
    switch(bpp) 
    {
    case 1:
    case 4:
    case 8:
    case 16:
    case 24:
    case 32:
        break;
    default:
        bpp = 8;
        break;
    }



    // calculate the size of a FreeImage image
    // align the palette and the pixels on a TGABITMAP_ALIGNMENT bytes alignment boundary
    // palette is aligned on a 16 bytes boundary
    // pixels are aligned on a 16 bytes boundary

    unsigned dib_size   = TgaImage_GetImageSize(width, height, bpp);
    m_TgaBitmap.data = (BYTE *)TgaImage_Aligned_Malloc(dib_size * sizeof(BYTE), TGABITMAP_ALIGNMENT);

    if (m_TgaBitmap.data != NULL)
    {
        memset(m_TgaBitmap.data, 0, dib_size);
        m_TgaBitmap.biWidth            = width;
        m_TgaBitmap.biHeight           = height;
        m_TgaBitmap.biBitCount         = (WORD)bpp;
        return true;
    }


    return false;
}


bool xTgaImage::LoadTga(IStreamIO *io) 
{
	if (io) 
    {
		try
        {
			// remember the start offset

			long start_offset = io->tell();

			// read and process the bitmap's header
			TGAHEADER header;

			io->read(header);

#ifdef XEVOL3D_BIGENDIAN
			SwapHeader(&header);
#endif

			int line = CalculateLine(header.is_width, header.is_pixel_depth);
			int pitch = CalculatePitch(line);
			int alphabits = header.is_image_descriptor & 0x0f;
			int fliphoriz = (header.is_image_descriptor & 0x10) ? 0 : 1; //currently FreeImage improperly treats this as flipvert also
			int flipvert = (header.is_image_descriptor & 0x20) ? 1 : 0;

			// skip comment
            io->seek(header.id_length, std::ios::cur);

			switch (header.is_pixel_depth) 
            {
				case 8 :
                case 15 :
                case 16 :
				{
                    //不支持
                    XEVOL_LOG(eXL_DEBUG_HIGH , ">>>Warning: ");
                    XEVOL_LOG(eXL_DEBUG_HIGH , L"Tga file format not support pixel depth=%d\n" , header.is_pixel_depth);
                }

                case 24 :
				{
					xTgaImage::Allocate(header.is_width, header.is_height, 32);
					// read in the bitmap bits
					switch (header.image_type) 
                    {
						case TGA_RGB:
						{
							FILE_BGR bgr;
							if (fliphoriz)
                            {
								for (unsigned count = header.is_height; count > 0; count--) 
                                {
									xColor_4ub* bits = GetScanLine( count-1, flipvert);

									for (int x = 0; x < m_TgaBitmap.biWidth ; x ++) 
                                    {
										io->read(bgr);

										bits[x ].b	= bgr.b;
										bits[x ].g  = bgr.g;
										bits[x ].r	= bgr.r;
                                        bits[x ].a  = 255;
									}
								}
                            }
							else
                            {
								for (unsigned count = 0; count < header.is_height; count++) 
                                {
									xColor_4ub *bits = GetScanLine( count, flipvert);
									for (int x = 0; x < m_TgaBitmap.biWidth ; x ++) 
                                    {
										io->read(bgr);
										bits[x ].b = bgr.b;
										bits[x ].g = bgr.g;
										bits[x ].r = bgr.r;
                                        bits[x ].a = 255;
									}
								}
                            }
							break;
						}
						case TGA_RLERGB:
						{
							int x = 0;
							int y = 0;
							BYTE rle;
							xColor_4ub*  bits;
							
							if (fliphoriz)
								bits = GetScanLine( header.is_height - y - 1, flipvert);
							else
								bits = GetScanLine( y, flipvert);
							
							if (alphabits)
                            {
								while(1) 
                                {
									io->read(&rle,1);
									
									if (rle>127) 
                                    {
										rle -= 127;

										FILE_BGRA bgra;

										io->read(bgra);

										for (int ix = 0; ix < rle; ix++) 
                                        {
											bits[x ].b = bgra.b;
											bits[x ].g = bgra.g;
											bits[x ].r = bgra.r;
											bits[x ].a = bgra.a;
											x ++;

											if (x >= line) 
                                            {
												x = 0;
												y++;

												if (y >= header.is_height)
													goto done243;

												if (fliphoriz)
													bits = GetScanLine( header.is_height - y - 1, flipvert);
												else
													bits = GetScanLine( y, flipvert);
											}
										}
									} 
                                    else 
                                    {
										rle++;

										for (int ix = 0; ix < rle; ix++) 
                                        {
											FILE_BGRA bgra;

											io->read(bgra);

											bits[x].b = bgra.b;
											bits[x].g = bgra.g;
											bits[x].r = bgra.r;
											bits[x].a = bgra.a;
											x ++;
											
											if (x >= line) 
                                            {
												x = 0;
												y++;

												if (y >= header.is_height)
													goto done243;											

												if (fliphoriz)
													bits = GetScanLine( header.is_height - y - 1, flipvert);
												else
													bits = GetScanLine( y, flipvert);
											}
										}
									}
								}
							} 
                            else 
                            {
								while (1)
                                {
									io->read(&rle,1);
									
									if (rle>127)
                                    {
										rle -= 127;

										FILE_BGR bgr;

										io->read(bgr);

										for (int ix = 0; ix < rle; ix++)
                                        {
											bits[x ].b = bgr.b;
											bits[x ].g = bgr.g;
											bits[x ].r = bgr.r;
											x ++;

											if (x >= line) 
                                            {
												x = 0;
												y++;

												if (y >= header.is_height)
													goto done243;											
												
												if (fliphoriz)
													bits = GetScanLine( header.is_height - y - 1, flipvert);
												else
													bits = GetScanLine( y, flipvert);
											}
										}
									} 
                                    else 
                                    {
										rle++;

										for (int ix = 0; ix < rle; ix++)
                                        {
											FILE_BGR bgr;		

											io->read(bgr);

											bits[x ].b	= bgr.b;
											bits[x ].g	= bgr.g;
											bits[x ].r	= bgr.r;
                                            bits[x ].a  = 255;
											x ++;
											
											if (x >= line) 
                                            {
												x = 0;
												y++;

												if (y >= header.is_height)
													goto done243;											

												if (fliphoriz)
													bits = GetScanLine( header.is_height - y - 1, flipvert);
												else
													bits = GetScanLine( y, flipvert);
											}
										}
									}
								}
							}
					done243 :
							break;
						}

						default :
							xTgaImage::Unload();
							return NULL;
					}

					break;
				}
				
				case 32 :
				{
					xTgaImage::Allocate(header.is_width, header.is_height, 32);
					// read in the bitmap bits

					switch (header.image_type)
                    {
						case TGA_RGB:
						{
							// uncompressed

							if (alphabits) 
                            {
								if (fliphoriz)
									for (unsigned count = header.is_height; count > 0; count--) 
                                    {
										xColor_4ub *bits = bits = GetScanLine( count-1, flipvert);

										for (unsigned cols = 0; cols < header.is_width; cols++) 
                                        {
											FILE_BGRA bgra;

											io->read(bgra);
											bits->b	= bgra.b;
											bits->g = bgra.g;
											bits->r	= bgra.r;
											bits->a = bgra.a;
											bits ++;
										}
									}
								else
									for (unsigned count = 0; count < header.is_height; count++)
                                    {
										xColor_4ub *bits = GetScanLine( count, flipvert);

										for (unsigned cols = 0; cols < header.is_width; cols++)
                                        {
											FILE_BGRA bgra;
											io->read(bgra);

                                            bits->b	= bgra.b;
                                            bits->g = bgra.g;
                                            bits->r	= bgra.r;
                                            bits->a = bgra.a;

											bits ++;
										}
									}

							} 
                            else 
                            {

								for (unsigned count = 0; count < header.is_height; count++) 
                                {
									xColor_4ub *bits;

									if (fliphoriz)
										bits = GetScanLine( header.is_height - count - 1, flipvert);
									else
										bits = GetScanLine( count, flipvert);

									for (unsigned cols = 0; cols < header.is_width; cols++) 
                                    {
										FILE_BGRA bgra;

										io->read(bgra);

                                        bits->b	= bgra.b;
                                        bits->g = bgra.g;
                                        bits->r	= bgra.r;
                                        bits->a = bgra.a;

										bits ++;
									}
								}
							}

							break;
						}
						case TGA_RLERGB:
						{
							int x = 0;
							int y = 0;
							BYTE rle;
                            xColor_4ub *bits = NULL;
							
							if (fliphoriz)
								bits = GetScanLine( header.is_height - y - 1, flipvert);
							else
								bits = GetScanLine( y, flipvert);
							
							while(1) 
                            {
								io->read(&rle,1);
									
								if (rle>127) 
                                {
									rle -= 127;

									FILE_BGRA bgra;

									io->read(bgra);

									for (int ix = 0; ix < rle; ix++) 
                                    {
										bits[x ].b = bgra.b;
										bits[x ].g = bgra.g;
										bits[x ].r = bgra.r;
										bits[x ].a = bgra.a;
										x ++;

										if (x >= line)
                                        {
											x = 0;
											y++;

											if (y >= header.is_height)
												goto done3210;

											if (fliphoriz)
												bits = GetScanLine( header.is_height - y - 1, flipvert);
											else
												bits = GetScanLine( y, flipvert);
										}
									}
								} 
                                else 
                                {
									rle++;

									for (int ix = 0; ix < rle; ix++) 
                                    {
										FILE_BGRA bgra;

										io->read(bgra);

										bits[x ].b = bgra.b;
										bits[x ].g = bgra.g;
										bits[x ].r = bgra.r;
										bits[x ].a = bgra.a;
										x ++;
											
										if (x >= line)
                                        {
											x = 0;
											y++;

											if (y >= header.is_height)
												goto done3210;

											if (fliphoriz)
												bits = GetScanLine( header.is_height - y - 1, flipvert);
											else
												bits = GetScanLine( y, flipvert);
										}
									}
								}
							}

					done3210 :
							break;
						}

						default :
							xTgaImage::Unload();
							return NULL;
					}

					break;
				}
			}

			return true;

		} 
        catch(const char *message) 
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , message);
			return false;
		}
	}

	return false;
}

bool  Save(IStreamIO *io, TgaImageData *dib, int page, void *data) 
{
	if (dib != NULL && io != NULL)
    {
		int bpp          = dib->biBitCount;
		// write the file header
		TGAHEADER header;
		header.id_length = 0;
		header.cm_first_entry = 0;
		header.is_xorigin = 0;
		header.is_yorigin = 0;
		header.is_width  = (WORD)dib->biWidth;
		header.is_height = (WORD)dib->biHeight;
		header.is_pixel_depth = (BYTE)bpp;
		header.is_image_descriptor = 0;
        

        header.color_map_type = 0;
        header.image_type = TGA_RGB;
        header.cm_length = 0;
        header.cm_size = 0;
		

		// write the header

#ifdef XEVOL3D_BIGENDIAN
		SwapHeader(&header);
#endif
		io->write(&header, sizeof(header));
#ifdef XEVOL3D_BIGENDIAN
		SwapHeader(&header);
#endif

		// write the palette	

		// write the data bits

		for (unsigned y = 0; y < header.is_height; y++)
        {
			BYTE *bits = (BYTE*)TgaImage_GetScanLine(dib, y);

			switch(bpp) 
            {
				case 8:
				{
				}
				case 16:
				{
				}
				case 24:
				{
					FILE_BGR bgr;
					xColor_3ub *trip;
					for(unsigned x = 0; x < header.is_width; x++) 
                    {
						trip = ((xColor_3ub *)bits) + x;
						bgr.b = trip->b;
						bgr.g = trip->g;
						bgr.r = trip->r;
						io->write(&bgr, sizeof(FILE_BGR));
					}
					break;
				}
				case 32:
				{
					FILE_BGRA bgra;
					xColor_4ub *quad;
					for(unsigned x = 0; x < header.is_width; x++) 
                    {
						quad = ((xColor_4ub *)bits) + x;
						bgra.b = quad->b;
						bgra.g = quad->g;
						bgra.r = quad->r;
						bgra.a = quad->a;
						io->write(&bgra, sizeof(FILE_BGRA));
					}
					break;
				}
			}
		}

		// write the TARGA signature

		TGAFOOTER footer;
		footer.developer_offset = 0;
		footer.extension_offset = 0;
		strcpy(footer.signature, "TRUEVISION-XFILE.");

#ifdef XEVOL3D_BIGENDIAN
		SwapFooter(&footer);
#endif
		io->write(&footer, sizeof(footer));

		return true;
	}

	return false;
}

const wchar_t*  xTgaImage::supportExt(bool bSave)
{
    return L"tga";
}

int  xTgaImage::byteAlign()
{
    return TGABITMAP_ALIGNMENT;
}

bool xTgaImage::load(const wchar_t* fileName, IMAGE_ORIGIN img_origin )
{
    m_ImgOrigin = img_origin;
    IStreamIO* pStream = xImageLib::createImageStream(fileName , NULL , 0);
    bool bRet = LoadTga(pStream);
    pStream->ReleaseObject();
    if(bRet && img_origin == IO_LEFT_UPPER)
    {
        flipY();
   }
    return bRet;
}

bool xTgaImage::load(const wchar_t* fileName, const int8* mem_buf , int buf_len, IMAGE_ORIGIN img_origin)
{
    m_ImgOrigin = img_origin;
    IStreamIO* pStream = xImageLib::createImageStream(fileName , mem_buf , buf_len);
    bool bRet = LoadTga(pStream);
    pStream->ReleaseObject();
    if(bRet && img_origin == IO_LEFT_UPPER)
    {
        flipY();
    }
    return bRet;
}

bool xTgaImage::load(const wchar_t* filename, const int8* mem_buf , int buf_len, ePIXEL_FORMAT format, IMAGE_ORIGIN img_origin)
{   
    m_ImgOrigin = img_origin;
    return load(filename , mem_buf , buf_len , img_origin);
}

bool xTgaImage::free()
{
    return Unload();
}

xImageSize xTgaImage::getSize()
{
    xImageSize _size;
    _size.w = m_TgaBitmap.biWidth;
    _size.h = m_TgaBitmap.biHeight;
    _size.fmt = PIXELFORMAT_B8G8R8A8;
    _size.pitch = TgaImage_GetPitch(&m_TgaBitmap);
    return _size;
}

void* xTgaImage::data(size_t mipmapLevel , size_t cubeMapImageIndex/*-1 means not a cubemap*/ , bool bCompressed )
{
    return m_TgaBitmap.data;
}

bool xTgaImage::data(const int8 * image_buffer,int mWidth,int mHeight , ePIXEL_FORMAT format)
{
    return false;
}

//增加加载器
class xTgaImageLoader : public xImageLib::xImageLoader
{
public:
    xImage* create(const wchar_t* pImageName)
    {
        return new xTgaImage;
    }
};

xTgaImage::xTgaImage()
{
    m_ImgOrigin = IO_LEFT_LOWER;
    memset(&m_TgaBitmap , 0 ,sizeof(m_TgaBitmap) );
}
xTgaImage::~xTgaImage()
{
   free();
}
static xTgaImageLoader loader;
const bool bTgaImageLoader     = xImageLib::registeImageLoader(L"tga" , &loader);

END_NAMESPACE_XEVOL3D
