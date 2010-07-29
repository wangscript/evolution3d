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

#ifndef __XDXT_IMAGE_H__
#define __XDXT_IMAGE_H__
#include "xBaseImage.h"
#include <fstream>
#include <vector>
#include "../BaseLib/stl_dllalloc.h"
BEGIN_NAMESPACE_XEVOL3D

#ifdef _WIN32
#pragma pack(push, 1)
#endif
typedef struct DDSHEAD
{
	unsigned int8	Signature[4];

	unsigned int	Size1;				// size of the structure (minus MagicNum)
	unsigned int	Flags1; 			// determines what fields are valid
	unsigned int	Height; 			// height of surface to be created
	unsigned int	Width;				// width of input surface
	unsigned int	LinearSize; 		// Formless late-allocated optimized surface size
	unsigned int	Depth;				// Depth if a volume texture
	unsigned int	MipMapCount;		// number of mip-map levels requested
	unsigned int	AlphaBitDepth;		// depth of alpha buffer requested

	unsigned int	NotUsed[10];

	unsigned int	Size2;				// size of structure
	unsigned int	Flags2;				// pixel format flags
	unsigned int	FourCC;				// (FOURCC code)
	unsigned int	RGBBitCount;		// how many bits per pixel
	unsigned int	RBitMask;			// mask for red bit
	unsigned int	GBitMask;			// mask for green bits
	unsigned int	BBitMask;			// mask for blue bits
	unsigned int	RGBAlphaBitMask;	// mask for alpha channel

	unsigned int	ddsCaps1, ddsCaps2, ddsCaps3, ddsCaps4; // direct draw surface capabilities
	unsigned int	TextureStage;
} DDSHEAD;

#ifdef _WIN32
#pragma pack(pop)
#endif

#define DDS_CAPS				0x00000001L
#define DDS_HEIGHT				0x00000002L
#define DDS_WIDTH				0x00000004L
#define DDS_RGB					0x00000040L
#define DDS_PIXELFORMAT			0x00001000L
#define DDS_LUMINANCE			0x00020000L

#define DDS_ALPHAPIXELS			0x00000001L
#define DDS_ALPHA				0x00000002L
#define DDS_FOURCC				0x00000004L
#define DDS_PITCH				0x00000008L
#define DDS_COMPLEX				0x00000008L
#define DDS_TEXTURE				0x00001000L
#define DDS_MIPMAPCOUNT			0x00020000L
#define DDS_LINEARSIZE			0x00080000L
#define DDS_VOLUME				0x00200000L
#define DDS_MIPMAP				0x00400000L
#define DDS_DEPTH				0x00800000L

#define DDS_CUBEMAP				0x00000200L
#define DDS_CUBEMAP_POSITIVEX	0x00000400L
#define DDS_CUBEMAP_NEGATIVEX	0x00000800L
#define DDS_CUBEMAP_POSITIVEY	0x00001000L
#define DDS_CUBEMAP_NEGATIVEY	0x00002000L
#define DDS_CUBEMAP_POSITIVEZ	0x00004000L
#define DDS_CUBEMAP_NEGATIVEZ	0x00008000L

//================================================

class _XEVOL_BASE_API_  xDXTImage : public xImage
{
	struct xDXTMipmap
	{
		unsigned int8* m_Data;
		unsigned int8* m_pCompressedData;
		DDSHEAD&       m_Header;
		int            m_CompressedSize;
		ePIXEL_FORMAT  fmt;
		int            w;
		int            h;
		int            depth;
	public:
		xDXTMipmap(DDSHEAD& header,ePIXEL_FORMAT _fmt , int _w , int _h , int _depth);
		void         free();
		int          __getBlockSize(int LinearSize);
		bool         __ReadData(const wchar_t* __fileName , std::istream& _stream , int LinearSize);
		bool         __decompress();
	    bool         flipY();
		bool         flipDXT();
		bool	     DecompressARGB();
		bool	     DecompressDXT1();
		bool	     DecompressDXT2();
		bool	     DecompressDXT3();
		bool	     DecompressDXT4();
		bool	     DecompressDXT5();

	};

	struct xDXTCubeImage
	{
		int                    m_nMipmap;
		DDSHEAD&               m_Header;
		ds_vector(xDXTMipmap*) m_vMipmaps;

	public:
		xDXTCubeImage(DDSHEAD& header)  ;
        void free();
		
	};

public:
	xDXTImage();
	~xDXTImage();
	const wchar_t*  supportExt(bool bSave);
	int          byteAlign();
	bool         convert(ePIXEL_FORMAT newFmt){ return false ; }; 
	bool         scale(int w , int h , IMAGE_FILTER filter = eFILTER_BOX){ return false ; }; 
	bool         scalePOT(IMAGE_FILTER filter = eFILTER_BOX){ return false ; }; 
	bool         scaleFit(int w , int h , IMAGE_FILTER filter = eFILTER_BOX){ return false ; }; 

	bool         load(xImageSize size){ return false ; }; 
	bool         load(int w , int h , ePIXEL_FORMAT fmt){ return false ; }; 
	bool         load(const wchar_t* fileName, IMAGE_ORIGIN img_origin = IO_LEFT_LOWER, unsigned int _arg = 0);
	bool         load(const wchar_t* fileName, const int8* mem_buf , int buf_len, IMAGE_ORIGIN img_origin = IO_LEFT_LOWER, unsigned int _arg = 0);
	bool         load(const wchar_t* filename, const int8* mem_buf , int buf_len, ePIXEL_FORMAT format, IMAGE_ORIGIN img_origin = IO_LEFT_LOWER, unsigned int _arg = 0);
	bool         save(const wchar_t* fileName){ return false ; }; 
	bool         free();

	xImageSize   getSize();
	void*        data(size_t mipmapLevel = 0 , size_t cubeMapImageIndex = 0/*-1 means not a cubemap*/ , bool bCompressed = false);
	bool         data(const int8 * image_buffer,int mWidth,int mHeight , ePIXEL_FORMAT format);
	size_t       nMipmapLevel();
	size_t       nImage();
	size_t       nSlice();
protected:
	bool         __loadDDSFile(const wchar_t* fileName , std::istream& _stream);

	void         __AdjustVolumeTexture();

	bool         __readDdsHead(std::istream & _stream , DDSHEAD * ddsc);
	bool         __checkDds(DDSHEAD *Head);

	int          __checkPixelFormat(DDSHEAD& Head, ePIXEL_FORMAT& fmt);
protected:
	ds_vector(xDXTCubeImage*)   m_vCubeImages;
	xImageSize                  m_Size;
	int                         m_Depth;
	DDSHEAD                     m_Header;
	IMAGE_ORIGIN                m_ImgOrigin;

};

END_NAMESPACE_XEVOL3D

#endif
