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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "xDXTImage.h"
#include "endian.h"
#include <math.h>
#include <assert.h>
#include "xImage.h"
#include "../io/mstream.h"
BEGIN_NAMESPACE_XEVOL3D
static int max(int a , int b )
{
	return a > b ? a : b;
}
#define MAKEFOURCC(ch0, ch1, ch2, ch3) \
	((unsigned int)(unsigned int8)(ch0) | ((unsigned int)(unsigned int8)(ch1) << 8) |   \
	((unsigned int)(unsigned int8)(ch2) << 16) | ((unsigned int)(unsigned int8)(ch3) << 24 ))

#define FOURCC_DXT1	MAKEFOURCC('D','X','T','1')
#define FOURCC_DXT2	MAKEFOURCC('D','X','T','2')
#define FOURCC_DXT3	MAKEFOURCC('D','X','T','3')
#define FOURCC_DXT4	MAKEFOURCC('D','X','T','4')
#define FOURCC_DXT5	MAKEFOURCC('D','X','T','5')


typedef struct
{
	unsigned short col0, col1;
	unsigned int8 row[4];
} DXTColorBlock_t;

typedef struct
{
	unsigned short row[4];
} DXT3AlphaBlock_t;

typedef struct
{
	unsigned int8 alpha0, alpha1;
	unsigned int8 row[6];
} DXT5AlphaBlock_t;



static bool swapMem(int8* Mem1 , int8* Mem2 , int8* tempMem , int n)
{
	if(Mem2 == Mem1)
		return false;
    memcpy(tempMem , Mem1 , n);
	memcpy(Mem1 , Mem2 , n);  //Mem2 --> Mem1;
	memcpy(Mem2 , tempMem , n);
	return true;
}

static bool swapMem(int8* Mem1 , int8* Mem2 , int n)
{
	if(Mem2 == Mem1)
		return false;
	for(int i = 0 ; i < n ; i ++)
	{
         int8 t = Mem1[i];
		 Mem1[i] = Mem2[i];
		 Mem2[i]  = t;
	}
	return true;
}
void flipDXT1Blocks(DXTColorBlock_t *Block, int NumBlocks)
{
	int i;
	DXTColorBlock_t *ColorBlock=Block;

	for(i=0;i<NumBlocks;i++)
	{
		swapMem( (int8*)&ColorBlock->row[0],  (int8*)&ColorBlock->row[3], sizeof(unsigned int8));
		swapMem( (int8*)&ColorBlock->row[1],  (int8*)&ColorBlock->row[2], sizeof(unsigned int8));
		ColorBlock++;
	}
}

void flipDXT3Blocks(DXTColorBlock_t *Block, int NumBlocks)
{
	int i;
	DXTColorBlock_t *ColorBlock=Block;
	DXT3AlphaBlock_t *AlphaBlock;

	for(i=0;i<NumBlocks;i++)
	{
		AlphaBlock=(DXT3AlphaBlock_t *)ColorBlock;

		swapMem( (int8*)&AlphaBlock->row[0],  (int8*)&AlphaBlock->row[3], sizeof(unsigned short));
		swapMem( (int8*)&AlphaBlock->row[1],  (int8*)&AlphaBlock->row[2], sizeof(unsigned short));
		ColorBlock++;

		swapMem( (int8*)&ColorBlock->row[0],  (int8*)&ColorBlock->row[3], sizeof(unsigned int8));
		swapMem( (int8*)&ColorBlock->row[1],  (int8*)&ColorBlock->row[2], sizeof(unsigned int8));
		ColorBlock++;
	}
}

void flipDXT5Alpha(DXT5AlphaBlock_t *Block)
{
	unsigned long *Bits, Bits0=0, Bits1=0;

	memcpy(&Bits0, &Block->row[0], sizeof(unsigned int8)*3);
	memcpy(&Bits1, &Block->row[3], sizeof(unsigned int8)*3);

	Bits=((unsigned long *)&(Block->row[0]));
	*Bits&=0xff000000;
	*Bits|=(unsigned int8)(Bits1>>12)&0x00000007;
	*Bits|=(unsigned int8)((Bits1>>15)&0x00000007)<<3;
	*Bits|=(unsigned int8)((Bits1>>18)&0x00000007)<<6;
	*Bits|=(unsigned int8)((Bits1>>21)&0x00000007)<<9;
	*Bits|=(unsigned int8)(Bits1&0x00000007)<<12;
	*Bits|=(unsigned int8)((Bits1>>3)&0x00000007)<<15;
	*Bits|=(unsigned int8)((Bits1>>6)&0x00000007)<<18;
	*Bits|=(unsigned int8)((Bits1>>9)&0x00000007)<<21;

	Bits=((unsigned long *)&(Block->row[3]));
	*Bits&=0xff000000;
	*Bits|=(unsigned int8)(Bits0>>12)&0x00000007;
	*Bits|=(unsigned int8)((Bits0>>15)&0x00000007)<<3;
	*Bits|=(unsigned int8)((Bits0>>18)&0x00000007)<<6;
	*Bits|=(unsigned int8)((Bits0>>21)&0x00000007)<<9;
	*Bits|=(unsigned int8)(Bits0&0x00000007)<<12;
	*Bits|=(unsigned int8)((Bits0>>3)&0x00000007)<<15;
	*Bits|=(unsigned int8)((Bits0>>6)&0x00000007)<<18;
	*Bits|=(unsigned int8)((Bits0>>9)&0x00000007)<<21;
}

void flipDXT5Blocks(DXTColorBlock_t *Block, int NumBlocks)
{
	DXTColorBlock_t *ColorBlock=Block;
	DXT5AlphaBlock_t *AlphaBlock;
	int i;

	for(i=0;i<NumBlocks;i++)
	{
		AlphaBlock=(DXT5AlphaBlock_t *)ColorBlock;

		flipDXT5Alpha(AlphaBlock);
		ColorBlock++;

		swapMem( (int8*)&ColorBlock->row[0],  (int8*)&ColorBlock->row[3], sizeof(unsigned int8));
		swapMem( (int8*)&ColorBlock->row[1],  (int8*)&ColorBlock->row[2], sizeof(unsigned int8));
		ColorBlock++;
	}
}


bool xDXTImage::xDXTMipmap::flipDXT()
{
	int linesize, j;
    unsigned int8* image = this->m_pCompressedData;
    if(image == NULL)
	{
		return false;
	}

	DXTColorBlock_t *top;
	DXTColorBlock_t *bottom;
	int xblocks= this->w/4;
	int yblocks= this->h/4;

	switch(this->fmt)
	{
	case PIXELFORMAT_DXT1: 
		linesize=xblocks*8;

		for(j=0;j<(yblocks>>1);j++)
		{
			top=(DXTColorBlock_t *)(image+j*linesize);
			bottom=(DXTColorBlock_t *)(image+(((yblocks-j)-1)*linesize));

			flipDXT1Blocks(top, xblocks);
			flipDXT1Blocks(bottom, xblocks);
			swapMem( (int8*)bottom,  (int8*)top, linesize);
		}
		break;

	case PIXELFORMAT_DXT2:
		{
			XEVOL_LOG(eXL_ERROR_FALT,"DXT2 compressed data not supported FLIP\n");
			assert(0);
			return false;
		}
	case PIXELFORMAT_DXT3:
		linesize=xblocks*16;

		for(j=0;j<(yblocks>>1);j++)
		{
			top=(DXTColorBlock_t *)(image+j*linesize);
			bottom=(DXTColorBlock_t *)(image+(((yblocks-j)-1)*linesize));

			flipDXT3Blocks(top, xblocks);
			flipDXT3Blocks(bottom, xblocks);
			swapMem( (int8*)bottom,  (int8*)top, linesize);
		}
		break;

	case PIXELFORMAT_DXT4:
		{
			XEVOL_LOG(eXL_ERROR_FALT,"DXT4 compressed data not supported FLIP\n");
			assert(0);
			return false;
		}

	case PIXELFORMAT_DXT5:
		linesize=xblocks*16;

		for(j=0;j<(yblocks>>1);j++)
		{
			top=(DXTColorBlock_t *)(image+j*linesize);
			bottom=(DXTColorBlock_t *)(image+(((yblocks-j)-1)*linesize));

			flipDXT5Blocks(top, xblocks);
			flipDXT5Blocks(bottom, xblocks);
			swapMem( (int8*)bottom,  (int8*)top, linesize);
		}
		break;

	default:
		return false;
	}
	return true;

}

bool xDXTImage::xDXTMipmap::flipY()
{
	flipDXT();
	int Bpp = xImageSize::getBytePerPixel(fmt);
	int linePitch   = w *  Bpp;
	int sizeOfPlane = linePitch * h ;
	int8* linePitchMem = new int8[linePitch];
	for (int z = 0; z < depth; z++) 
	{
		for (int y = 0; y < h/2; y++ )
		{
			 int8* pLineUp   = (int8*)(m_Data + sizeOfPlane* z +  y * linePitch);
			 int8* pLineDown = (int8*)(m_Data + sizeOfPlane* z +  (h - y - 1) * linePitch);
			 swapMem(pLineDown , pLineUp , linePitchMem, linePitch);
		}
	}
	delete [] linePitchMem;
	return true;
}

xDXTImage::xDXTMipmap::xDXTMipmap(DDSHEAD& header,ePIXEL_FORMAT _fmt , int _w , int _h , int _depth)
: m_Header(header) , fmt(_fmt) , w(_w) , h(_h) , depth(_depth) 
{
	m_Data = NULL;
	m_pCompressedData = NULL;
	m_CompressedSize  = 0;
}

void xDXTImage::xDXTMipmap::free()
{
	delete [] m_Data;
	delete [] m_pCompressedData;
	m_Data = NULL;
}



xDXTImage::xDXTCubeImage::xDXTCubeImage(DDSHEAD& header) 
: m_Header(header) 
{
	m_nMipmap = 0;
}

void xDXTImage::xDXTCubeImage::free()
{
	size_t _nMipMaps = m_vMipmaps.size() ;
   for( size_t i  = 0 ; i < _nMipMaps ; i ++)
   {
	   m_vMipmaps[i]->free();
	   delete m_vMipmaps[i];
   }
   m_vMipmaps.clear();
   m_nMipmap = 0;
}


static void GetBitsFromMask(unsigned int Mask, unsigned int *ShiftLeft, unsigned int *ShiftRight)
{
	unsigned int Temp, i;

	if (Mask == 0) 
	{
		*ShiftLeft = *ShiftRight = 0;
		return;
	}

	Temp = Mask;
	for (i = 0; i < 32; i++, Temp >>= 1)
	{
		if (Temp & 1)
			break;
	}
	*ShiftRight = i;

	// Temp is preserved, so use it again:
	for (i = 0; i < 8; i++, Temp >>= 1) 
	{
		if (!(Temp & 1))
			break;
	}
	*ShiftLeft = 8 - i;

	return;
}

static void ReadColors(const unsigned int8* Data, xColor_4ub* Out)
{
	unsigned int8 r0, g0, b0, r1, g1, b1;

	b0 = Data[0] & 0x1F;
	g0 = ((Data[0] & 0xE0) >> 5) | ((Data[1] & 0x7) << 3);
	r0 = (Data[1] & 0xF8) >> 3;

	b1 = Data[2] & 0x1F;
	g1 = ((Data[2] & 0xE0) >> 5) | ((Data[3] & 0x7) << 3);
	r1 = (Data[3] & 0xF8) >> 3;

	Out[0].r = r0 << 3;
	Out[0].g = g0 << 2;
	Out[0].b = b0 << 3;

	Out[1].r = r1 << 3;
	Out[1].g = g1 << 2;
	Out[1].b = b1 << 3;
}

static void ReadColor(unsigned short Data, xColor_4ub* Out)
{
	unsigned int8 r, g, b;

	b = Data & 0x1f;
	g = (Data & 0x7E0) >> 5;
	r = (Data & 0xF800) >> 11;

	Out->r = r << 3;
	Out->g = g << 2;
	Out->b = b << 3;
}

bool  xDXTImage::xDXTMipmap::DecompressARGB() 
{
	unsigned int ReadI = 0, TempBpp, i;
	unsigned int RedL, RedR;
	unsigned int GreenL, GreenR;
	unsigned int BlueL, BlueR;
	unsigned int AlphaL, AlphaR;
	unsigned int8	*Temp;

	if (m_pCompressedData == NULL)
		return false;


	if(fmt == PIXELFORMAT_LUMINANCE16 && m_Header.RGBBitCount == 16 && m_Header.RBitMask == 0xFFFF) 
	{ //HACK
	    m_Data = m_pCompressedData;
		m_pCompressedData = NULL;
		m_CompressedSize  = 0;
		return true;
	}

	GetBitsFromMask(m_Header.RBitMask, &RedL, &RedR);
	GetBitsFromMask(m_Header.GBitMask, &GreenL, &GreenR);
	GetBitsFromMask(m_Header.BBitMask, &BlueL, &BlueR);
	GetBitsFromMask(m_Header.RGBAlphaBitMask, &AlphaL, &AlphaR);

	Temp = m_pCompressedData;
	TempBpp = m_Header.RGBBitCount / 8;

	int Bpp = xImageSize::getBytePerPixel(fmt);
	int linePitch   = w *  Bpp;
	int sizeOfPlane = linePitch * h ;
	int sizeOfData  = sizeOfPlane * depth ;
	m_Data = new unsigned int8[ depth * sizeOfPlane ];

	for (i = 0; i < (unsigned int)sizeOfData ; i += Bpp) 
	{

		//TODO: This is SLOOOW...
		//but the old version crashed in release build under
		//winxp (and xp is right to stop this code - I always
		//wondered that it worked the old way at all)
		if (sizeOfData - i < 4) 
		{ //less than 4 int8 to write?
			if (TempBpp == 3) 
			{ //this branch is extra-SLOOOW
				ReadI =		*Temp	| ((*(Temp + 1)) << 8)	| ((*(Temp + 2)) << 16);
			}
			else if (TempBpp == 1)
			{
				ReadI = *((unsigned int8*)Temp);
			}
			else if (TempBpp == 2)
			{
				ReadI = Temp[0] | (Temp[1] << 8);
			}
		}
		else
		{
			ReadI = Temp[0] | (Temp[1] << 8) | (Temp[2] << 16) | (Temp[3] << 24);
		}

		Temp += TempBpp;

		m_Data[i] = ((ReadI & m_Header.RBitMask) >> RedR) << RedL;

		if( Bpp >= 3) 
		{
			m_Data[i+1] = ((ReadI & m_Header.GBitMask) >> GreenR) << GreenL;
			m_Data[i+2] = ((ReadI & m_Header.BBitMask) >> BlueR) << BlueL;

			if (Bpp == 4)
			{
				m_Data[i+3] = ((ReadI & m_Header.RGBAlphaBitMask) >> AlphaR) << AlphaL;
				if (AlphaL >= 7)
				{
					m_Data[i+3] = m_Data[i+3] ? 0xFF : 0x00;
				}
				else if (AlphaL >= 4) 
				{
					m_Data[i+3] = m_Data[i+3] | (m_Data[i+3] >> 4);
				}
			}
		}
		else if ( Bpp == 2)
		{
			m_Data[i+1] = ((ReadI & m_Header.RGBAlphaBitMask) >> AlphaR) << AlphaL;
			if (AlphaL >= 7) 
			{
				m_Data[i+1] = m_Data[i+1] ? 0xFF : 0x00;
			}
			else if (AlphaL >= 4) 
			{
				m_Data[i+1] = m_Data[i+1] | (m_Data[i+3] >> 4);
			}
		}
	}

	return true;
}


void CorrectPreMult(unsigned int8* Data , int sizeOfData)
{
	int  i;

	for (i = 0; i < sizeOfData ; i += 4) {
		if (Data[i+3] != 0) {  // Cannot divide by 0.
			Data[i]   = (unsigned int8)(((unsigned int)Data[i]   << 8) / Data[i+3]);
			Data[i+1] = (unsigned int8)(((unsigned int)Data[i+1] << 8) / Data[i+3]);
			Data[i+2] = (unsigned int8)(((unsigned int)Data[i+2] << 8) / Data[i+3]);
		}
	}

	return;
}

bool xDXTImage::xDXTMipmap::DecompressDXT1()
{
	int			     x, y, z, i, j, k, Select;
	unsigned int8*   Temp;
	xColor_4ub	     colours[4], *col;
	unsigned short   color_0, color_1;
	unsigned int     bitmask, Offset;

	int Bpp = xImageSize::getBytePerPixel(fmt);
	int linePitch   = w *  Bpp;
	int sizeOfPlane = linePitch * h ;
	
	
	if (!m_pCompressedData)
		return false;

	m_Data = new unsigned int8[ depth * sizeOfPlane ];

	Temp = m_pCompressedData;
	colours[0].a = 0xFF;
	colours[1].a = 0xFF;
	colours[2].a = 0xFF;
	//colours[3].a = 0xFF;
	for (z = 0; z < depth; z++) 
	{
		for (y = 0; y < h; y += 4) 
		{
			for (x = 0; x < w; x += 4) 
			{
				color_0 = *((unsigned short*)Temp);
				//UShort(&color_0);
				color_1 = *((unsigned short*)(Temp + 2));
				//UShort(&color_1);
				ReadColor(color_0, colours);
				ReadColor(color_1, colours + 1);
				bitmask = ((unsigned int*)Temp)[1];
				//UInt(&bitmask);
				Temp += 8;

				if (color_0 > color_1) 
				{
					// Four-color block: derive the other two colors.
					// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
					// These 2-bit codes correspond to the 2-bit fields 
					// stored in the 64-bit block.
					colours[2].b = (2 * colours[0].b + colours[1].b + 1) / 3;
					colours[2].g = (2 * colours[0].g + colours[1].g + 1) / 3;
					colours[2].r = (2 * colours[0].r + colours[1].r + 1) / 3;
					//colours[2].a = 0xFF;

					colours[3].b = (colours[0].b + 2 * colours[1].b + 1) / 3;
					colours[3].g = (colours[0].g + 2 * colours[1].g + 1) / 3;
					colours[3].r = (colours[0].r + 2 * colours[1].r + 1) / 3;
					colours[3].a = 0xFF;
				}
				else
				{ 
					// Three-color block: derive the other color.
					// 00 = color_0,  01 = color_1,  10 = color_2,
					// 11 = transparent.
					// These 2-bit codes correspond to the 2-bit fields 
					// stored in the 64-bit block. 
					colours[2].b = (colours[0].b + colours[1].b) / 2;
					colours[2].g = (colours[0].g + colours[1].g) / 2;
					colours[2].r = (colours[0].r + colours[1].r) / 2;
					//colours[2].a = 0xFF;

					colours[3].b = (colours[0].b + 2 * colours[1].b + 1) / 3;
					colours[3].g = (colours[0].g + 2 * colours[1].g + 1) / 3;
					colours[3].r = (colours[0].r + 2 * colours[1].r + 1) / 3;
					colours[3].a = 0x00;
				}

				for (j = 0, k = 0; j < 4; j++)
				{
					for (i = 0; i < 4; i++, k++) 
					{

						Select = (bitmask & (0x03 << k*2)) >> k*2;
						col = &colours[Select];

						if (((x + i) < w) && ((y + j) < h)) 
						{
							Offset = z * sizeOfPlane + (y + j) * linePitch + (x + i) * Bpp;
							m_Data[Offset + 0] = col->r;
							m_Data[Offset + 1] = col->g;
							m_Data[Offset + 2] = col->b;
							m_Data[Offset + 3] = col->a;
						}
					}
				}
			}
		}
	}

	return true;
}


bool xDXTImage::xDXTMipmap::DecompressDXT2()
{
	// Can do color & alpha same as dxt3, but color is pre-multiplied 
	//   so the result will be wrong unless corrected. 
	if (!DecompressDXT3())
		return false;

	int sizeOfData = depth *  w  * h * xImageSize::getBytePerPixel(fmt);
	CorrectPreMult(m_Data , sizeOfData);

	return true;
}


bool xDXTImage::xDXTMipmap::DecompressDXT3()
{
	int			x, y, z, i, j, k, Select;
	unsigned int8		*Temp;
	//Color565	*color_0, *color_1;
	xColor_4ub	colours[4], *col;
	unsigned int		bitmask, Offset;
	unsigned short	word;
	unsigned int8 *alpha;

	int Bpp = xImageSize::getBytePerPixel(fmt);
	int linePitch   = w *  Bpp;
	int sizeOfPlane = linePitch * h ;


	if (!m_pCompressedData)
		return false;

	m_Data = new unsigned int8[ depth * sizeOfPlane ];

	Temp = m_pCompressedData;

	for (z = 0; z < depth ; z++) 
	{
		for (y = 0; y < h ; y += 4) 
		{
			for (x = 0; x < w ; x += 4)
			{
				alpha = Temp;
				Temp += 8;
				ReadColors(Temp, colours);
				bitmask = ((unsigned int*)Temp)[1];
				//UInt(&bitmask);
				Temp += 8;

				// Four-color block: derive the other two colors.    
				// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
				// These 2-bit codes correspond to the 2-bit fields 
				// stored in the 64-bit block.
				colours[2].b = (2 * colours[0].b + colours[1].b + 1) / 3;
				colours[2].g = (2 * colours[0].g + colours[1].g + 1) / 3;
				colours[2].r = (2 * colours[0].r + colours[1].r + 1) / 3;
				//colours[2].a = 0xFF;

				colours[3].b = (colours[0].b + 2 * colours[1].b + 1) / 3;
				colours[3].g = (colours[0].g + 2 * colours[1].g + 1) / 3;
				colours[3].r = (colours[0].r + 2 * colours[1].r + 1) / 3;
				//colours[3].a = 0xFF;

				k = 0;
				for (j = 0; j < 4; j++) 
				{
					for (i = 0; i < 4; i++, k++) 
					{

						Select = (bitmask & (0x03 << k*2)) >> k*2;
						col = &colours[Select];

						if (((x + i) < w) && ((y + j) < h)) 
						{
							Offset = z * sizeOfPlane + (y + j) * linePitch + (x + i) * Bpp;
							m_Data[Offset + 0] = col->r;
							m_Data[Offset + 1] = col->g;
							m_Data[Offset + 2] = col->b;
						}
					}
				}

				for (j = 0; j < 4; j++) 
				{
					word = alpha[2*j] + 256*alpha[2*j+1];
					for (i = 0; i < 4; i++) 
					{
						if (((x + i) < w) && ((y + j) < h)) 
						{
							Offset = z * sizeOfPlane + (y + j) * linePitch + (x + i) * Bpp + 3;
							m_Data[Offset] = word & 0x0F;
							m_Data[Offset] = m_Data[Offset] | (m_Data[Offset] << 4);
						}
						word >>= 4;
					}
				}

			}
		}
	}

	return true;
}


bool  xDXTImage::xDXTMipmap::DecompressDXT4()
{
	// Can do color & alpha same as dxt5, but color is pre-multiplied 
	//   so the result will be wrong unless corrected. 
	if (!DecompressDXT5())
		return false;


	int sizeOfData = depth *  w  * h * xImageSize::getBytePerPixel(fmt);
	CorrectPreMult(m_Data , sizeOfData);

	return false;
}


bool xDXTImage::xDXTMipmap::DecompressDXT5()
{
	int			x, y, z, i, j, k, Select;
	unsigned int8		*Temp; //, r0, g0, b0, r1, g1, b1;
	xColor_4ub	colours[4], *col;
	unsigned int		bitmask, Offset;
	unsigned int8		alphas[8], *alphamask;
	unsigned int		bits;

	int Bpp = xImageSize::getBytePerPixel(fmt);
	int linePitch   = w *  Bpp;
	int sizeOfPlane = linePitch * h ;


	if (!m_pCompressedData)
		return false;

	m_Data = new unsigned int8[ depth * sizeOfPlane ];

	Temp = m_pCompressedData;

	for (z = 0; z < depth; z++) 
	{
		for (y = 0; y < h; y += 4)
		{
			for (x = 0; x < w; x += 4)
			{
				if (y >= h || x >= w)
					break;
				alphas[0] = Temp[0];
				alphas[1] = Temp[1];
				alphamask = Temp + 2;
				Temp += 8;

				ReadColors(Temp, colours);
				bitmask = ((unsigned int*)Temp)[1];
				//UInt(&bitmask);
				Temp += 8;

				// Four-color block: derive the other two colors.    
				// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
				// These 2-bit codes correspond to the 2-bit fields 
				// stored in the 64-bit block.
				colours[2].b = (2 * colours[0].b + colours[1].b + 1) / 3;
				colours[2].g = (2 * colours[0].g + colours[1].g + 1) / 3;
				colours[2].r = (2 * colours[0].r + colours[1].r + 1) / 3;
				//colours[2].a = 0xFF;

				colours[3].b = (colours[0].b + 2 * colours[1].b + 1) / 3;
				colours[3].g = (colours[0].g + 2 * colours[1].g + 1) / 3;
				colours[3].r = (colours[0].r + 2 * colours[1].r + 1) / 3;
				//colours[3].a = 0xFF;

				k = 0;
				for (j = 0; j < 4; j++) 
				{
					for (i = 0; i < 4; i++, k++) 
					{

						Select = (bitmask & (0x03 << k*2)) >> k*2;
						col = &colours[Select];

						// only put pixels out < width or height
						if (((x + i) < w) && ((y + j) < h))
						{
							Offset = z * sizeOfPlane + (y + j) * linePitch + (x + i) * Bpp;
							m_Data[Offset + 0] = col->r;
							m_Data[Offset + 1] = col->g;
							m_Data[Offset + 2] = col->b;
						}
					}
				}

				// 8-alpha or 6-alpha block?    
				if (alphas[0] > alphas[1]) 
				{    
					// 8-alpha block:  derive the other six alphas.    
					// Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
					alphas[2] = (6 * alphas[0] + 1 * alphas[1] + 3) / 7;	// bit code 010
					alphas[3] = (5 * alphas[0] + 2 * alphas[1] + 3) / 7;	// bit code 011
					alphas[4] = (4 * alphas[0] + 3 * alphas[1] + 3) / 7;	// bit code 100
					alphas[5] = (3 * alphas[0] + 4 * alphas[1] + 3) / 7;	// bit code 101
					alphas[6] = (2 * alphas[0] + 5 * alphas[1] + 3) / 7;	// bit code 110
					alphas[7] = (1 * alphas[0] + 6 * alphas[1] + 3) / 7;	// bit code 111
				}
				else 
				{
					// 6-alpha block.
					// Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
					alphas[2] = (4 * alphas[0] + 1 * alphas[1] + 2) / 5;	// Bit code 010
					alphas[3] = (3 * alphas[0] + 2 * alphas[1] + 2) / 5;	// Bit code 011
					alphas[4] = (2 * alphas[0] + 3 * alphas[1] + 2) / 5;	// Bit code 100
					alphas[5] = (1 * alphas[0] + 4 * alphas[1] + 2) / 5;	// Bit code 101
					alphas[6] = 0x00;										// Bit code 110
					alphas[7] = 0xFF;										// Bit code 111
				}

				// Note: Have to separate the next two loops,
				//	it operates on a 6-int8 system.

				// First three bytes
				//bits = *((ILint*)alphamask);
				bits = (alphamask[0]) | (alphamask[1] << 8) | (alphamask[2] << 16);
				for (j = 0; j < 2; j++)
				{
					for (i = 0; i < 4; i++) 
					{
						// only put pixels out < width or height
						if (((x + i) < w) && ((y + j) < h))
						{
							Offset = z * sizeOfPlane + (y + j) * linePitch + (x + i) *  Bpp + 3;
							m_Data[Offset] = alphas[bits & 0x07];
						}
						bits >>= 3;
					}
				}

				// Last three bytes
				//bits = *((ILint*)&alphamask[3]);
				bits = (alphamask[3]) | (alphamask[4] << 8) | (alphamask[5] << 16);
				for (j = 2; j < 4; j++) 
				{
					for (i = 0; i < 4; i++)
					{
						// only put pixels out < width or height
						if (((x + i) < w) && ((y + j) < h)) 
						{
							Offset = z * sizeOfPlane + (y + j) * linePitch + (x + i) *  Bpp + 3;
							m_Data[Offset] = alphas[bits & 0x07];
						}
						bits >>= 3;
					}
				}
			}
		}
	}

	return true;
}

bool xDXTImage::xDXTMipmap::__decompress()
{
	switch (fmt)
	{
	case PIXELFORMAT_R8G8B8:
	case PIXELFORMAT_R8G8B8A8:
	case PIXELFORMAT_B8G8R8:
	case PIXELFORMAT_B8G8R8A8:
	case PIXELFORMAT_LUMINANCE8:
	case PIXELFORMAT_LUMINANCE8ALPHA8:
	case PIXELFORMAT_ALPHA8:
		return DecompressARGB();

	case PIXELFORMAT_DXT1:
		return DecompressDXT1();

	case PIXELFORMAT_DXT2:
		return DecompressDXT2();

	case PIXELFORMAT_DXT3:
		return DecompressDXT3();

	case PIXELFORMAT_DXT4:
		return DecompressDXT4();

	case PIXELFORMAT_DXT5:
		return DecompressDXT5();

	//case PF_ATI1N:
	//	return DecompressAti1n();
	//case PF_3DC:
	//	return Decompress3Dc();
	//case PF_RXGB:
	//	return DecompressRXGB();

	case PIXELFORMAT_R16G16B16A16U:
	case PIXELFORMAT_R16G16B16A16S:
	case PIXELFORMAT_R16G16B16A16F:
	case PIXELFORMAT_R32G32B32A32F:
	case PIXELFORMAT_R32G32B32A32U:
	case PIXELFORMAT_R32G32B32A32S:

	case PIXELFORMAT_R16F:
	case PIXELFORMAT_R16G16F:
	case PIXELFORMAT_R16G16S:
	
	case PIXELFORMAT_R16G16U:
	
	case PIXELFORMAT_R32F:
	case PIXELFORMAT_R32G32F:
	case PIXELFORMAT_R32U:
	case PIXELFORMAT_R32G32U:
	case PIXELFORMAT_R32S:
	case PIXELFORMAT_R32G32S:
	
	case PIXELFORMAT_LUMINANCE16:
		{
			m_Data = m_pCompressedData;
			m_pCompressedData = NULL;
			m_CompressedSize  = 0;
		}
		break;
	}
	return true;
}

int  xDXTImage::xDXTMipmap::__getBlockSize(int LinearSize)
{
	int blockSize = 0;
	int Bpp = xImageSize::getBytePerPixel(fmt);

	int  CompFactor = 1;
	bool isCompressed = false;
	switch(fmt)
	{
	case PIXELFORMAT_DXT1:
		//This is officially 6, we have 8 here because DXT1 may contain alpha
		CompFactor = 8;
		break;
	case PIXELFORMAT_DXT2:
	case PIXELFORMAT_DXT3:
	case PIXELFORMAT_DXT4:
	case PIXELFORMAT_DXT5:
		CompFactor = 4;
		break;
	//case PIXELFORMAT_RXGB:
	//case PIXELFORMAT_3DC:
		//This is officially 4 for 3dc, but that's bullshit :) There's no
		//alpha data in 3dc images
		//CompFactor = 3;
		//break;
	//case PIXELFORMAT_ATI1N:
		//CompFactor = 2;
		//break;
	default:
		CompFactor = 1;
	}

	if (m_Header.Flags1 & DDS_LINEARSIZE) 
	{
		if(fmt >= PIXELFORMAT_DXT1 && fmt <= PIXELFORMAT_DXT5)
		{
			//compressed format
			int minW = (((w+3)/4))*4;
			int minH = (((h+3)/4))*4;
			blockSize = (minW * minH * depth * Bpp) / CompFactor;
			isCompressed = true;
		}
		else
		{
			blockSize = w * h * depth * Bpp;
			blockSize = LinearSize;
		}
		//else 
		//{
		//	//don't use Bpp to support argb images with less than 32 bits
		//	Head.LinearSize = Width * Height * Depth * (Head.RGBBitCount >> 3);
		//}
	}
	else 
	{
		blockSize = LinearSize;
	}
	return blockSize;
}
bool xDXTImage::xDXTMipmap::__ReadData(const wchar_t* __fileName , std::istream& _stream , int LinearSize)
{
	int blockSize = __getBlockSize(LinearSize);
	if ( m_pCompressedData) 
	{
		delete []  m_pCompressedData;
		m_pCompressedData = NULL;
	}

	if (m_Header.Flags1 & DDS_LINEARSIZE)
	{
		//Head.LinearSize = Head.LinearSize * Depth;

		m_pCompressedData = new unsigned int8[ blockSize ];
		if ( m_pCompressedData == NULL)
		{
			return false;
		}

		_stream.read( (int8*) m_pCompressedData , blockSize); // != (unsigned int)m_Header.LinearSize) 
		//{
		//	delete [] m_pCompressedData;
		//	m_pCompressedData = NULL;
		//	return false;
		//}
	}
	else 
	{
		unsigned int	Bps;
		unsigned int8	*Temp;

		Bps = w * m_Header.RGBBitCount / 8;
		m_CompressedSize = Bps * h * depth;

		m_pCompressedData = new unsigned int8[m_CompressedSize] ;
		if (m_pCompressedData == NULL) 
		{
			return false;
		}

		Temp = m_pCompressedData;
		for (int z = 0; z < depth; z++) 
		{
			for (int y = 0; y < h ; y++) 
			{
				//if (iread(Temp, 1, Bps) != Bps)
				//{
				//	ifree(CompData);
				//	CompData = NULL;
				//	return false;
				//}
				_stream.read( (int8*)Temp , Bps);
				Temp += Bps;
			}
		}
	}

	return true;
}




bool xDXTImage::__readDdsHead(std::istream & _stream , DDSHEAD * ddsc)
{

	_stream.read( (int8*)ddsc , sizeof(DDSHEAD) );
#ifdef __BIG_ENDIAN__
    unsigned int * pData = (unsigned int*) ddsc;
	for(int i = 0 ; i < sizeof(DDSHEAD)/(sizeof(unsigned int))  ; i++)
	{
		xSwapUInt(pData);
		pData ++;
	}
#endif
	if (ddsc->Depth == 0)
		ddsc->Depth = 1;
	if(ddsc->MipMapCount == 0)
        ddsc->MipMapCount = 1;
	return true;
}

bool xDXTImage::__checkDds(DDSHEAD *Head)
{

	if (strnicmp( (int8*)m_Header.Signature, "DDS ", 4))
		return false;
	//note that if Size1 is "DDS " this is not a valid dds file according
	//to the file spec. Some broken tool out there seems to produce files
	//with this value in the size field, so we support reading them...
	if (m_Header.Size1 != 124 && m_Header.Size1 != MAKEFOURCC('D', 'D', 'S', ' '))
		return false;
	if (m_Header.Size2 != 32)
		return false;
	if (m_Header.Width == 0 || m_Header.Height == 0)
		return false;
	return true;
}

int xDXTImage::__checkPixelFormat(DDSHEAD& Head, ePIXEL_FORMAT& fmt)
{
	int BlockSize;

	if (Head.Flags2 & DDS_FOURCC)
	{
		BlockSize = ((Head.Width + 3)/4) * ((Head.Height + 3)/4) * Head.Depth;
		switch (Head.FourCC)
		{
		case MAKEFOURCC('D','X','T','1'):
			fmt       = PIXELFORMAT_DXT1;
			BlockSize *= 8;
			break;

		case MAKEFOURCC('D','X','T','2'):
			fmt       = PIXELFORMAT_DXT2;
			BlockSize *= 16;
			break;

		case MAKEFOURCC('D','X','T','3'):
			fmt       = PIXELFORMAT_DXT3;
			BlockSize *= 16;
			break;

		case MAKEFOURCC('D','X','T','4'):
			fmt       = PIXELFORMAT_DXT4;
			BlockSize *= 16;
			break;

		case MAKEFOURCC('D','X','T','5'):
			fmt       = PIXELFORMAT_DXT5;
			BlockSize *= 16;
			break;

		case MAKEFOURCC('A', 'T', 'I', '1'):
			fmt       = PIXELFORMAT_None;
			BlockSize *= 8;
			break;

		case MAKEFOURCC('A', 'T', 'I', '2'):
			//CompFormat = PF_3DC;
			fmt       = PIXELFORMAT_None;
			BlockSize *= 16;
			break;

		case MAKEFOURCC('R', 'X', 'G', 'B'):
			fmt       = PIXELFORMAT_None;
			BlockSize *= 16;
			break;

		case MAKEFOURCC('$', '\0', '\0', '\0'):
			fmt       = PIXELFORMAT_R16G16B16A16F;
			BlockSize = Head.Width * Head.Height * Head.Depth * 8;
			break;

		case MAKEFOURCC('o', '\0', '\0', '\0'):
			fmt       = PIXELFORMAT_R16F;
			BlockSize = Head.Width * Head.Height * Head.Depth * 2;
			break;

		case MAKEFOURCC('p', '\0', '\0', '\0'):
			fmt       = PIXELFORMAT_R16G16F;
			BlockSize = Head.Width * Head.Height * Head.Depth * 4;
			break;

		case MAKEFOURCC('q', '\0', '\0', '\0'):
			fmt       = PIXELFORMAT_R16G16B16A16F;
			BlockSize = Head.Width * Head.Height * Head.Depth * 8;
			break;

		case MAKEFOURCC('r', '\0', '\0', '\0'):
			fmt       = PIXELFORMAT_R32F;
			BlockSize = Head.Width * Head.Height * Head.Depth * 4;
			break;

		case MAKEFOURCC('s', '\0', '\0', '\0'):
			fmt       = PIXELFORMAT_R32G32F;
			BlockSize = Head.Width * Head.Height * Head.Depth * 8;
			break;

		case MAKEFOURCC('t', '\0', '\0', '\0'):
			fmt       = PIXELFORMAT_R32G32B32A32F;
			BlockSize = Head.Width * Head.Height * Head.Depth * 16;
			break;

		default:
			fmt       = PIXELFORMAT_None;
			BlockSize *= 16;
			break;
		}
	} 
	else 
	{
		// This dds texture isn't compressed so write out ARGB or luminance format
		if (Head.Flags2 & DDS_ALPHA)//This dds texture only has Alpha
		{
			switch(Head.RGBBitCount )
			{
			case 8:
				fmt = PIXELFORMAT_ALPHA8;
				break;
			default:
				fmt = PIXELFORMAT_None;
				break;
			}
		}
		else if (Head.Flags2 & DDS_LUMINANCE)//DDS file has LUMINACE component.
		{
			if (Head.Flags2 & DDS_ALPHAPIXELS) 
			{
				switch(Head.RGBBitCount )
				{
				case 16:
					fmt = PIXELFORMAT_LUMINANCE8ALPHA8;
					break;
				default:
					fmt = PIXELFORMAT_None;
					break;
				}
				
			} 
			else 
			{
				switch(Head.RGBBitCount )
				{
				case 8:
					fmt = PIXELFORMAT_LUMINANCE8;
					break;
				case 16:
					fmt = PIXELFORMAT_LUMINANCE16;
					break;
				default:
					fmt = PIXELFORMAT_None;
					break;
				}
			}
		}
		else 
		{
			if (Head.Flags2 & DDS_ALPHAPIXELS) 
			{
				fmt = PIXELFORMAT_R8G8B8A8;
			} 
			else 
			{
				fmt = PIXELFORMAT_R8G8B8;
			}
		}
		BlockSize = (Head.Width * Head.Height * Head.Depth * (Head.RGBBitCount >> 3));
	}

	return BlockSize;
}


bool xDXTImage::__loadDDSFile(const wchar_t* fileName , std::istream& _stream)
{
	int  BlockSize = 0;

	if (!__readDdsHead(_stream , &m_Header)  || !__checkDds(&m_Header)) 
	{
		return false;
	}

	ePIXEL_FORMAT fmt;
	BlockSize = __checkPixelFormat(m_Header,fmt);
	if (fmt == PIXELFORMAT_None) 
	{
		XEVOL_LOG(eXL_ERROR_FALT,"DDS Pixel format not support\n");
		return false;
	}

	// Microsoft bug, they're not following their own documentation.
	if ( !(m_Header.Flags1 & (DDS_LINEARSIZE | DDS_PITCH)) || m_Header.LinearSize == 0) 
	{
		m_Header.Flags1 |= DDS_LINEARSIZE;
		m_Header.LinearSize = BlockSize;
	}

	m_Size.fmt = fmt;
	m_Size.w = m_Header.Width;
	m_Size.h = m_Header.Height;
	m_Size.pitch = m_Header.RGBBitCount/8 * m_Header.Width;
	m_Depth  = m_Header.Depth;
    if(m_Size.pitch == 0 )
	{
		int Bpp = xImageSize::getBytePerPixel(fmt);	
		m_Size.pitch =  m_Size.w * Bpp;
	}
	int nCubeImage = 1;
	if (m_Header.ddsCaps1 & DDS_COMPLEX) 
	{
		if (m_Header.ddsCaps2 & DDS_CUBEMAP) 
		{
			nCubeImage = 6;
		}
	}


   if(nCubeImage != 6 )
   {
	  __AdjustVolumeTexture();
   }

   for(int i =  0 ; i < nCubeImage ; i ++)
   {
	   xDXTCubeImage* pCubeImage = new xDXTCubeImage(m_Header);
	   pCubeImage->m_nMipmap = m_Header.MipMapCount;
	   m_vCubeImages.push_back(pCubeImage);

	   int w = m_Size.w;
	   int h = m_Size.h;
	   int d = m_Depth;
	   int linearSize = m_Header.LinearSize;
	   for(size_t iLevel = 0 ; iLevel < m_Header.MipMapCount ; iLevel ++)
	   {
		   xDXTMipmap* pMipMap = new xDXTMipmap(m_Header , m_Size.fmt , w , h , d);
		   pMipMap->__ReadData(fileName , _stream , linearSize);
		   if(nCubeImage == 1)
			   pMipMap->flipDXT();
		   pMipMap->__decompress();
		   pCubeImage->m_vMipmaps.push_back(pMipMap);

		   if(m_ImgOrigin == IO_LEFT_LOWER)
		   {
			   //if(nCubeImage == 1) //如果只有一个图。就要给它反过来
		       //   pMipMap->flipY();
			   //如果是Cube map。就不需要反过来。 因为其他的本来就是反的（在OpenGL里）
			   //if(nCubeImage == 6 &&(i ==2 || i ==3) ) 
			   //{//如果是Cube map。则只要把POS_Y NEG_Y 给反过来。 因为其他的本来就是反的（在OpenGL里）
				//   pMipMap->flipY();
			   //}
		   }

		   int bitShift = 0;
		   if(w >= 2) bitShift++;
		   if(h >= 2) bitShift++;
		   if(d >= 2) bitShift++;
		   linearSize >>= bitShift;
		   w /=2; if(w==0)w=1;
		   h /=2; if(h==0)h=1;
		   d /=2; if(d==0)d=1;
	   }

   }

	return true;
}


void xDXTImage::__AdjustVolumeTexture()
{
	if (m_Header.Depth <= 1)
		return;

	// All volume textures I've seem so far didn't have the DDS_COMPLEX flag set,
	// even though this is normally required. But because noone does set it,
	// also read images without it (TODO: check file size for 3d texture?)
	if (/*!(m_Header.ddsCaps1 & DDS_COMPLEX) ||*/ !(m_Header.ddsCaps2 & DDS_VOLUME)) 
	{
		m_Header.Depth = 1;
		m_Depth = 1;
	}

	switch (m_Size.fmt)
	{
	case PIXELFORMAT_B8G8R8A8:
	case PIXELFORMAT_B8G8R8:
	case PIXELFORMAT_R8G8B8A8:
	case PIXELFORMAT_R8G8B8:
	case PIXELFORMAT_ALPHA8:
	case PIXELFORMAT_LUMINANCE8:
	case PIXELFORMAT_LUMINANCE8ALPHA8:
		//don't use the iCompFormatToBpp() function because this way
		//argb images with only 8 bits (eg. a1r2g3b2) work as well
		m_Header.LinearSize = max(1L,m_Header.Width) * max(1L,m_Header.Height) * (m_Header.RGBBitCount / 8);
		break;

	case PIXELFORMAT_DXT1:
	//case PF_ATI1N:
		m_Header.LinearSize = ((m_Header.Width+3)/4) * ((m_Header.Height+3)/4) * 8;
		break;

	case PIXELFORMAT_DXT2:
	case PIXELFORMAT_DXT3:
	case PIXELFORMAT_DXT4:
	case PIXELFORMAT_DXT5:
	//case PIXELFORMAT_3DC:
	//case PF_RXGB:
		m_Header.LinearSize = ((m_Header.Width+3)/4) * ((m_Header.Height+3)/4) * 16;
		break;

	case PIXELFORMAT_R16G16B16A16U:
	case PIXELFORMAT_R16G16B16A16S:
	case PIXELFORMAT_R16G16B16A16F:
	case PIXELFORMAT_R32G32B32A32F:
	case PIXELFORMAT_R32G32B32A32U:
	case PIXELFORMAT_R32G32B32A32S:
	case PIXELFORMAT_R16F:
	case PIXELFORMAT_R16G16F:
	case PIXELFORMAT_R16G16S:
	case PIXELFORMAT_R16G16U:
	case PIXELFORMAT_R32F:
	case PIXELFORMAT_R32G32F:
	case PIXELFORMAT_R32U:
	case PIXELFORMAT_R32G32U:
	case PIXELFORMAT_R32S:
	case PIXELFORMAT_R32G32S:

	case PIXELFORMAT_LUMINANCE16:
		m_Header.LinearSize = max(1L,m_Header.Width) * max(1L,m_Header.Height) * xImageSize::getBytePerPixel(m_Size.fmt);
		break;
	}

	m_Header.Flags1 |= DDS_LINEARSIZE;
	m_Header.LinearSize *= m_Header.Depth;

	return;
}


size_t xDXTImage::nMipmapLevel()
{
	if(nImage() ==0 )return 0;
    return m_vCubeImages[0]->m_vMipmaps.size();
}

size_t  xDXTImage::nImage()
{
    return m_vCubeImages.size();
}
size_t xDXTImage::nSlice()
{
    return m_Depth;
}

xDXTImage::xDXTImage()
{
	m_ImgOrigin = IO_LEFT_LOWER;
}

xDXTImage::~xDXTImage()
{  
	free();	
}

int  xDXTImage::byteAlign()
{
	return 4;
}


bool xDXTImage::load(const wchar_t* fileName, IMAGE_ORIGIN img_origin, unsigned int _arg)
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
	__loadDDSFile(fileName , _stream);
	return true;
}

bool xDXTImage::load(const wchar_t* fileName, const int8* mem_buf , int buf_len, IMAGE_ORIGIN img_origin, unsigned int _arg)
{
	m_ImgOrigin = img_origin;
	if(mem_buf)
	{
		std::imemstream _stream;
		_stream.set_buf((int8*)mem_buf , buf_len);
		return __loadDDSFile(fileName , _stream);
	}
	return load(fileName , img_origin , _arg);
}

bool xDXTImage::load(const wchar_t* fileName, const int8* mem_buf , int buf_len, ePIXEL_FORMAT format,IMAGE_ORIGIN img_origin, unsigned int _arg)
{
	return load(fileName , mem_buf , buf_len , img_origin , _arg);
}

bool xDXTImage::free()
{
	size_t _nCubeImage = m_vCubeImages.size() ;
	for(size_t i = 0 ; i <  _nCubeImage ; i ++)
	{
		m_vCubeImages[i]->free();
		delete m_vCubeImages[i];
	}
	m_vCubeImages.clear();
	return true;
}

xImageSize xDXTImage::getSize()
{
	return m_Size;
}

void* xDXTImage::data(size_t mipmapLevel  , size_t cubeMapImageIndex  /*-1 means not a cubemap*/ , bool bCompressed)
{
	size_t _nMipmapLevel = nMipmapLevel();
	size_t _nImage = nImage();
	if(mipmapLevel >= _nMipmapLevel || cubeMapImageIndex >= _nImage )
		return NULL;

	if(bCompressed && m_vCubeImages[cubeMapImageIndex]->m_vMipmaps[mipmapLevel]->m_pCompressedData)
	{
		return m_vCubeImages[cubeMapImageIndex]->m_vMipmaps[mipmapLevel]->m_pCompressedData;
	}
	return m_vCubeImages[cubeMapImageIndex]->m_vMipmaps[mipmapLevel]->m_Data;
}

bool xDXTImage::data(const int8 * image_buffer,int mWidth,int mHeight , ePIXEL_FORMAT format)
{
	return false;
}

const wchar_t*   xDXTImage::supportExt(bool bSave)
{
	if(bSave)
	{
		return NULL;
	}
	else
	{
		static wchar_t* loadExt = L"dds";
		return loadExt;
	}
}

//增加加载器
class xDXTImageLoader : public xImageLib::xImageLoader
{
public:
    xImage* create(const wchar_t* pImageName)
    {
        return new xDXTImage;
    }
};

static xDXTImageLoader loader;
const bool bDDSImageLoader     = xImageLib::registeImageLoader(L"dds" , &loader);

#define FOURCC_RGBA16   0x00000024
#define FOURCC_R16F     0x0000006f
#define FOURCC_RG16F    0x00000070
#define FOURCC_RGBA16F  0x00000071
#define FOURCC_R32F     0x00000072
#define FOURCC_RG32F    0x00000073
#define FOURCC_RGBA32F  0x00000074


END_NAMESPACE_XEVOL3D
