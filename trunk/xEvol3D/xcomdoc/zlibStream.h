/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) .
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

#ifndef _ZLIB_STREAM_H_
#define _ZLIB_STREAM_H_
#include <ios>
#include "xcomdoc_common.h"

BEGIN_NAMESPACE_XEVOL3D

struct sCDDataCompressPack
{
	int m_DataSize;
	int m_CompreessedSize;
	int m_CompreessedRate;
};


template <typename T> T* t_malloc(int n)
{
	return (T*)malloc(sizeof(T) * n);

}

class CZLibReadBuffer;
struct CCDCompressedDataPackBuffer
{
private:
	sCDDataCompressPack m_Info;
	_xcd_int8*          buffer;
	int                 m_OffsetInFile;
	int                 m_dataBegPos;
public:
	friend class CZLibReadBuffer;
	CCDCompressedDataPackBuffer()	
	{
		init();
	}

	void  init()
	{
		m_Info.m_CompreessedSize = 0;
		m_Info.m_DataSize = 0;
		m_OffsetInFile = 0;
		buffer = NULL;
	}
	void  alloc()
	{
		buffer = (_xcd_int8*)malloc(m_Info.m_DataSize);
	}
	void  Release()
	{
		if(buffer != NULL)
			::free(buffer);
		buffer = NULL;
	}
};

struct sCDCompressedFile
{
	int                          m_nBlock;
	CCDCompressedDataPackBuffer* m_pEntris;
};

class CZLibReadBuffer
{

	IStreamIO* m_DiskFile;
	int          m_FileBegPos;
	int          m_ComprressedLen;

	sCDCompressedFile m_FileHeader;

	int    m_curBlockIndex;   // = 0;
	int    m_curBlockReadPos; // = 0;
	int    m_DataNowPos;
	int    m_DataLen;

protected:
	void   init_file_header();
	void   cacheBlock(int index);
	void   seek2CurBlock();
public:
	CZLibReadBuffer()
	{
		m_DiskFile = NULL;
		m_FileBegPos = 0;
		m_ComprressedLen = 0;
		m_curBlockIndex = 0;   // = 0;
		m_curBlockReadPos = 0; // = 0;
		m_DataNowPos = 0;
		m_DataLen = 0;

	}
	int    get_file_read_pos()
	{
		return (int)m_DiskFile->tell();
	}
	void   set_file_read_pos(int pos)
	{
        m_DiskFile->seek(pos,std::ios_base::beg);
	}
	void   set_stream(_xcd_int8* /*read_buf*/,int /*beginPos*/, int /*len*/,int /*compressedLen*/)
	{
	}
	void   set_stream(IStreamIO* DiskFile,int beginPos, int len,int compressedLen)
	{
		m_DiskFile   =  DiskFile;
		m_FileBegPos = beginPos;
		m_DataLen = len;
		m_ComprressedLen = compressedLen;
		init_file_header();
	}

	IStreamIO* get_stream()
	{
		return m_DiskFile;
	}

	void   rewind();
	int    read(_xcd_int8* buf, int byte_read);
    void   seekg(int _offset, std::ios_base::seekdir dir);
	int    tellg();
	int    data_len();
	void   close();
};

class CZlibWriteBuffer : public IWriteBuffer
{

};

int   XComDoc_Compress(_xcd_int8* dest_buf,_xcd_int8* src_buf, int src_len,int compressRate);
int   XComDoc_Uncompress(_xcd_int8* dest_buf,_xcd_int8* src_buf, int dest_buf_len , int src_len,int compressRate);
int   XComDoc_Encrypot(_xcd_int8* dest_buf,_xcd_int8* src_buf, int src_len);
int   XComDoc_DeEncrypot(_xcd_int8* dest_buf,_xcd_int8* src_buf, int src_len);

END_NAMESPACE_XEVOL3D
#endif
