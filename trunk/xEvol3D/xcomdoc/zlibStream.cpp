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

#include "xcdWriteBuffer.h"
#include "xcdReadBuffer.h"
#include "zlibStream.h"
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#define max(a, b)  (((a) > (b)) ? (a) : (b)) 
#define _USE_ZLIB_STATIC_
#define  _USE_LZMA_

#ifdef _USE_ZLIB_STATIC_
#define _COMPRESSED_
#include "../zlib/zlib.h"
#endif

#ifdef _USE_ZLIB_DLL_
#define _COMPRESSED_
#include "zlib.h"
#pragma comment(lib,"zlib.lib")
#endif

#ifdef _USE_LZMA_
#define _COMPRESSED_
#include "../lzmza/LzmaLib.h"
int CompressLzma(char *dest, size_t *destLen, const char *src, size_t srcLen,
                       unsigned char *outProps, /* *outPropsSize must be = 5 */
                       int level = 9,      /* 0 <= level <= 9, default = 5 */
                       unsigned dictSize = (1 << 20),  /* default = (1 << 24) */
                       int lc = 3 ,        /* 0 <= lc <= 8, default = 3  */
                       int lp = 0 ,        /* 0 <= lp <= 4, default = 0  */
                       int pb = 2 ,        /* 0 <= pb <= 4, default = 2  */
                       int fb = 32,        /* 5 <= fb <= 273, default = 32 */
                       int numThreads = 1  /* 1 or 2, default = 2 */
                       )
{
    size_t outPropsSize = 5;
    return LzmaCompress((unsigned char*)dest , destLen , (unsigned char*)src ,srcLen , outProps , &outPropsSize , level , dictSize , lc , lp , pb ,fb ,numThreads);
}

int UncompressLzma(char *dest, size_t *destLen, const char *src, size_t *srcLen, const unsigned char *props, size_t propsSize)
{
    return LzmaUncompress((unsigned char*)dest , destLen , (unsigned char*)src , srcLen , props , propsSize);
}

#endif


//#ifndef _DEBUG
//ofstream out("C:\\xcomdoc.log.txt");
//#else
//ofstream out("C:\\xcomdoc.debug.txt");
//#endif

#ifdef _USE_XEVOL3D_NEW_
#include "../mem/mem_operator.h"
#endif

BEGIN_NAMESPACE_XEVOL3D

int XComDoc_Encrypot(_xcd_int8* dest_buf,_xcd_int8* src_buf, int src_len)
{
	for(int i = 0 ; i < src_len ; i++)
	{
		dest_buf[i] = ~src_buf[i];
	}
	return src_len;
}
int XComDoc_DeEncrypot(_xcd_int8* dest_buf,_xcd_int8* src_buf, int src_len)
{
	for(int i = 0 ; i < src_len ; i++)
	{
		dest_buf[i] = ~src_buf[i];
	}
	return src_len;
}

///=================================================================================================
#ifdef _COMPRESSED_
int   XComDoc_Compress(_xcd_int8* dest_buf,_xcd_int8* src_buf, int src_len,int compressRate)
{
	if(compressRate == XCOMDOC_COMPRESS_ENCRYPT)
		return XComDoc_Encrypot(dest_buf,src_buf,src_len);

    if(compressRate <= 10)
    {
#ifndef _WIN32_WCE
        uLong dest_len =  compressBound(src_len);
#else
        uLong dest_len =  2 * src_len + 12;
#endif
        compress2((Byte *)dest_buf,&dest_len,(Byte *)src_buf,src_len,compressRate - 1);
        return dest_len;
    }    
    else//大于10。用LZMA压缩
    {
        size_t dest_len = 2 * src_len + 12;
#ifdef _USE_LZMA_        
        struct _outProp
        {
            char _v;
            int  _disSize;
        }outProps;
        CompressLzma(dest_buf , &dest_len , src_buf ,src_len , (unsigned char*)&outProps , compressRate - 6 );
#else
        XEVOL_LOG(eXL_ERROR_FALT , "lzma not compiled, uncompressed failed\n");
        assert(0);
#endif
        return dest_len;
    }
	
}

int   XComDoc_Uncompress(_xcd_int8* dest_buf,_xcd_int8* src_buf, int dest_buf_len , int src_len,int compressRate)
{
	if(compressRate == XCOMDOC_COMPRESS_ENCRYPT)
		return XComDoc_DeEncrypot(dest_buf,src_buf,src_len);
	

    if(compressRate <= 10)
    {
        uLong dest_len = dest_buf_len;
        uncompress((Byte *)dest_buf,&dest_len,(Byte *)src_buf,src_len);
        return dest_len;
    }
    else
    {
         size_t dest_len = dest_buf_len;
#ifdef _USE_LZMA_
        struct _outProp
        {
            char _v;
            int  _disSize;
        }outProps;
        int lc  = 3  ;
        int lp  = 0  ;
        int pb  = 2  ;
        outProps._v = (5 * pb + lp ) * 9 + lc;
        outProps._disSize = 1 << 20;
       
        size_t srcLen = src_len;
        UncompressLzma(dest_buf , &dest_len , src_buf , &srcLen , (unsigned char*)&outProps, 5);
#else
        XEVOL_LOG(eXL_ERROR_FALT , "lzma not compiled, uncompressed failed\n");
        assert(0);
#endif
        return (int)dest_len;
    }
	
}
#else
int   XComDoc_Compress(_xcd_int8* dest_buf,_xcd_int8* src_buf, int src_len,int compressRate)
{
	return XComDoc_Encrypot(dest_buf,src_buf,src_len);
}
int   XComDoc_Uncompress(_xcd_int8* dest_buf,_xcd_int8* src_buf, int dest_buf_len , int src_len,int compressRate)
{
	return XComDoc_DeEncrypot(dest_buf,src_buf,src_len);
}
#endif

sCDCompressedFile m_FileHeader;
void  CZLibReadBuffer::init_file_header()
{
	int old_pos = (int)m_DiskFile->tell();
	m_DiskFile->seek(m_FileBegPos,std::ios_base::beg);
	m_DiskFile->read((_xcd_int8*)&m_FileHeader.m_nBlock,4);
	m_FileHeader.m_pEntris = t_malloc<CCDCompressedDataPackBuffer>(m_FileHeader.m_nBlock);

	int _offset = 4 + sizeof(sCDDataCompressPack) * m_FileHeader.m_nBlock;   
	
	int dataBegPos = 0;

	for(int i = 0 ; i < m_FileHeader.m_nBlock ; i++)
	{
		m_FileHeader.m_pEntris[i].init();
		m_DiskFile->read((_xcd_int8*)&m_FileHeader.m_pEntris[i].m_Info , sizeof(sCDDataCompressPack));
		//计算这个块在文件里的开始位置
		m_FileHeader.m_pEntris[i].m_OffsetInFile = _offset + m_FileBegPos;
		m_FileHeader.m_pEntris[i].m_dataBegPos = dataBegPos;
		dataBegPos += m_FileHeader.m_pEntris[i].m_Info.m_DataSize;
		_offset     += m_FileHeader.m_pEntris[i].m_Info.m_CompreessedSize ;
	}

	m_curBlockReadPos = 0;
	m_curBlockIndex   = 0;
	m_DataNowPos = 0;

	m_DiskFile->seek(old_pos,std::ios_base::beg);
}


void  CZLibReadBuffer::cacheBlock(int index)
{
	CCDCompressedDataPackBuffer& readBlock = m_FileHeader.m_pEntris[index];
	//already readed into buffer
	if(readBlock.buffer)
		return ;

	m_DiskFile->seek(readBlock.m_OffsetInFile, std::ios_base::beg);
	readBlock.alloc();

	_xcd_int8* compressedbuf = t_malloc<_xcd_int8>(readBlock.m_Info.m_CompreessedSize);
	m_DiskFile->read(compressedbuf,readBlock.m_Info.m_CompreessedSize);
	/*int  ucSize = */XComDoc_Uncompress(readBlock.buffer,compressedbuf,readBlock.m_Info.m_DataSize,readBlock.m_Info.m_CompreessedSize,readBlock.m_Info.m_CompreessedRate);
	::free(compressedbuf);
}


int    CZLibReadBuffer::read(_xcd_int8* buf, int byte_read)
{
	int byte_need_read = byte_read;
	if( size_t(m_DataNowPos + byte_read) > size_t(m_DataLen) )
	{
		byte_need_read =  m_DataLen - m_DataNowPos ;
	}
	if(byte_need_read == 0)
		return 0;

	CCDCompressedDataPackBuffer& readBlock = m_FileHeader.m_pEntris[m_curBlockIndex];

	int byteInThisBlock = readBlock.m_Info.m_DataSize - m_curBlockReadPos;

	if(readBlock.buffer == NULL) 
		cacheBlock(m_curBlockIndex);

	if(byteInThisBlock >= byte_need_read)
	{
		memcpy(buf,readBlock.buffer + m_curBlockReadPos  , byte_need_read) ;
		m_curBlockReadPos += byte_need_read;
		m_DataNowPos      += byte_need_read;  
		return byte_need_read;        
	}
	else
	{
		memcpy(buf,readBlock.buffer + m_curBlockReadPos  , byteInThisBlock) ;
		m_DataNowPos      += byteInThisBlock;
		m_curBlockIndex ++;
		readBlock.Release();
		cacheBlock(m_curBlockIndex);
		m_curBlockReadPos = 0;
		return byteInThisBlock + read(buf + byteInThisBlock , byte_need_read - byteInThisBlock);
	}
}

void   CZLibReadBuffer::seekg(int _offset, std::ios_base::seekdir pos)
{
	if(pos == std::ios_base::cur)
	{
		if( (m_DataNowPos + _offset) > m_DataLen )
			m_DataNowPos = m_DataLen;
		else
			m_DataNowPos += _offset;
	}
	else if(pos == std::ios_base::beg)
	{
		if( _offset > (int)m_DataLen )
			m_DataNowPos = 0 + m_DataLen;
		else
			m_DataNowPos = 0 + _offset;
	}
	else if(pos == std::ios_base::end)
	{
		if( _offset > (int)m_DataLen )
			m_DataNowPos = 0;
		else
			m_DataNowPos = 0 + m_DataLen - _offset;
	}

	seek2CurBlock();
}

void   CZLibReadBuffer::seek2CurBlock()
{
	int curBlockIndex;
	for(int i = 0 ; i  < m_FileHeader.m_nBlock  ; i++ )
	{
		if( m_DataNowPos < m_FileHeader.m_pEntris[i].m_dataBegPos)
		{
			curBlockIndex = i - 1;
			m_curBlockReadPos = m_DataNowPos - m_FileHeader.m_pEntris[i - 1].m_dataBegPos;
			if(m_curBlockIndex != curBlockIndex)
			{
				m_FileHeader.m_pEntris[m_curBlockIndex].Release();
				cacheBlock(curBlockIndex);
			}
			m_curBlockIndex=curBlockIndex;
			return ;
		}
	}

	curBlockIndex   = m_FileHeader.m_nBlock - 1 ;
	m_curBlockReadPos = m_DataNowPos - m_FileHeader.m_pEntris[m_FileHeader.m_nBlock - 1].m_dataBegPos;
	if(m_curBlockIndex != curBlockIndex)
	{
		m_FileHeader.m_pEntris[m_curBlockIndex].Release();
		cacheBlock(curBlockIndex);
	}
	m_curBlockIndex=curBlockIndex;
}
int    CZLibReadBuffer::tellg()
{
	return m_DataNowPos;
}

int    CZLibReadBuffer::data_len()
{
	return m_DataLen;
}

END_NAMESPACE_XEVOL3D
