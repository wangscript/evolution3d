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

#include "xcdWriteBuffer.h"
#include "xcdReadBuffer.h"
#include <math.h>
#include <stdlib.h>
#include <vector>

#ifdef _USE_XEVOL3D_NEW_
#include "../mem/mem_operator.h"
#endif

#define max(a, b)  (((a) > (b)) ? (a) : (b)) 
BEGIN_NAMESPACE_XEVOL3D
void  CWriteBuffer::rewind()
{
	m_read_pos =0;
	m_write_pos=0;
	m_real_len = 0;
	m_need_free = 0;
}

bool  CWriteBuffer::duplicate_data()
{
	if(m_need_free == false)
	{
		_xcd_int8 * buf = new _xcd_int8[m_len];
		if(m_data)
		{
			memmove(buf,m_data,m_len);
			free();
		}
		m_data = buf;
		m_need_free = true;
		return m_data != NULL;
	}
	return false;
}

bool  CWriteBuffer::reserver(int len)
{
	if(len > m_len)
	{
		_xcd_int8 * buf = new _xcd_int8[len];
		if(m_data)
		{
			memmove(buf,m_data,m_len);
			if(m_data && m_need_free)
				delete [] m_data;
			m_data = 0;
		}
		m_len  = len;
		m_data = buf;
		m_need_free = true;
		return m_data != NULL;
	}
	return false;
}

void  CWriteBuffer::free()
{
	if(m_data && m_need_free)
		delete [] m_data;
	m_need_free = false;
	m_len =  0; 
	m_data = NULL;
}

CWriteBuffer::CWriteBuffer()
{
	m_data = NULL;
	m_read_pos =0;
	m_write_pos=0;
	m_len = 0;
	m_real_len = 0;
	m_need_free = 0;
}
CWriteBuffer::~CWriteBuffer()
{
	free();
}

int CWriteBuffer::write(const _xcd_int8* buf,int byte_write)
{
	int byte_need_write = byte_write;
	if( m_len - m_write_pos < byte_write )
	{
		int nReseved = max((int)byte_write,(int)m_len) * 2;
		if( false == reserver(nReseved) )
			return 0;
	}

	memcpy(&m_data[m_write_pos],buf,byte_need_write);
	m_write_pos += byte_need_write;

	if(m_real_len <  m_write_pos )
		m_real_len =  m_write_pos;
	return byte_need_write;   
}

int CWriteBuffer::capictiy()
{
	return m_len;
}

int CWriteBuffer::read(_xcd_int8* buf, int byte_read)
{
	int byte_need_read = byte_read;
	if( m_real_len - m_read_pos < byte_read )
	{
		byte_need_read = m_real_len- m_read_pos;
	}
	if(byte_need_read == 0)
		return 0;
	memcpy(buf,&m_data[m_read_pos],byte_need_read);
	m_read_pos += byte_need_read;
	return byte_need_read;
}

void CWriteBuffer::seekw(int _offset, std::ios_base::seekdir dir)
{
	if(dir == std::ios_base::cur)
	{
		if( m_write_pos + _offset >= m_real_len )
			m_write_pos = m_real_len-1;
		else
			m_write_pos += _offset;
	}
	else if(dir == std::ios_base::beg)
	{
		if( _offset > (int)m_real_len )
			m_write_pos = m_real_len-1;               
		else
			m_write_pos = _offset;                
	}
	else if(dir == std::ios_base::end)
	{
		if( _offset >= (int)m_real_len)
			m_write_pos = 0;  
		else
			m_write_pos = m_real_len -  _offset - 1;
	}
	else
	{
		return ;
	}
}

void CWriteBuffer::seekg(int _offset, std::ios_base::seekdir dir)
{
	if(dir == std::ios_base::cur)
	{
		if( m_read_pos + _offset >= m_real_len )
			m_read_pos = m_real_len-1;
		else
			m_read_pos += _offset;
	}
	else if(dir == std::ios_base::beg)
	{
		if( _offset > (int)m_real_len )
			m_read_pos = m_real_len-1;               
		else
			m_read_pos = _offset;                
	}
	else if(dir == std::ios_base::end)
	{
		if( _offset >= (int)m_real_len)
			m_read_pos = 0;  
		else
			m_read_pos = m_real_len -  _offset - 1;
	}
	else
	{
		return ;
	}
}

int  CWriteBuffer::tellw()
{ 
	return m_write_pos;
}

int CWriteBuffer::tellg()
{
	return m_read_pos;
}

_xcd_int8* CWriteBuffer::get_buf()
{
	return m_data;
}

void  CWriteBuffer::set_buf(const _xcd_int8* buf,int buf_len)
{
	free();
	m_data = (_xcd_int8*)buf;
	m_len = buf_len;
	m_real_len = buf_len;
	m_read_pos = m_write_pos = 0;
}

int CWriteBuffer::data_len()
{
	return m_real_len;
}

bool  CWriteBuffer::set_data_real_size(int real_size)
{
	if(m_len < real_size)
		return false;
	m_real_len = real_size;
	return true;
}


///=================================================================================================
int    CReadBuffer::read(_xcd_int8* buf, int byte_read)
{
	int byte_need_read = byte_read;
	if( size_t(m_FileNowPos + byte_read) > size_t(m_FileBegPos + m_DataInFileLen) )
	{
		byte_need_read =  (m_FileBegPos + m_DataInFileLen) - m_FileNowPos ;
	}
	if(byte_need_read == 0)
		return 0;
	//int disk_file_pos = m_DiskFile->tellg();
	m_DiskFile->seek(m_FileNowPos,std::ios_base::beg);
	m_DiskFile->read(buf,(int)byte_need_read);
	m_FileNowPos = (int)m_DiskFile->tell();
	//m_DiskFile->seekg(disk_file_pos,xcdsd_beg);
	return byte_need_read;
}

void   CReadBuffer::seekg(int _offset, std::ios_base::seekdir pos)
{
	if(pos == std::ios_base::cur)
	{
		if( (m_FileNowPos + _offset - m_FileBegPos) > m_DataInFileLen )
			m_FileNowPos = m_FileBegPos + m_DataInFileLen;
		else
			m_FileNowPos += _offset;
	}
	else if(pos == std::ios_base::beg)
	{
		if( _offset > (int)m_DataInFileLen )
			m_FileNowPos = m_FileBegPos + m_DataInFileLen;
		else
			m_FileNowPos = m_FileBegPos + _offset;
	}
	else if(pos == std::ios_base::end)
	{
		if( _offset > (int)m_DataInFileLen )
			m_FileNowPos = m_FileBegPos;
		else
			m_FileNowPos = m_FileBegPos + m_DataInFileLen - _offset;
	}
}

int    CReadBuffer::tellg()
{
	return (m_FileNowPos - m_FileBegPos);
}

int    CReadBuffer::data_len()
{
	return m_DataInFileLen;
}

END_NAMESPACE_XEVOL3D


