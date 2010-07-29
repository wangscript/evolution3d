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

#ifndef __READ_BUFFER_H__
#define __READ_BUFFER_H__

#include "xcomdoc_common.h"
#include <fstream>
BEGIN_NAMESPACE_XEVOL3D

class CCPPIFStream : public IStreamIO
{
	FILE* m_fStream;
    IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(CCPPIFStream)
public:
	bool open_stream(const wchar_t* _fileName)
	{
		m_fStream = _xcd_wfopen_rb(_fileName);
		return m_fStream != NULL;
	}

	int   close()
	{
		return fclose(m_fStream);
	}

	size_t    read(_xcd_int8* buf, size_t byte_read)
	{
		fread(buf,1, byte_read,m_fStream);
		return byte_read;
	}

    size_t   seek(long _offset, std::ios_base::seekdir _dir)
	{
		switch(_dir)
		{
        case std::ios_base::beg:
			fseek(m_fStream,(long)_offset,SEEK_SET);
			break;
		case std::ios_base::end:
			fseek(m_fStream,(long)_offset,SEEK_END);
			break;
		case std::ios_base::cur:
			fseek(m_fStream,(long)_offset,SEEK_CUR);
			break;
		}
        return tell();
	}

	size_t    tell()
	{
		return ftell(m_fStream);
	}

	bool eof()
	{
		return  feof(m_fStream) != 0;
	}
};

class CCPPIStream : public IStreamIO
{
	std::istream* m_pStream;
    IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(CCPPIStream)
public:
	int   close()
	{
        return 0;
	}

	size_t    read(_xcd_int8* buf, size_t byte_read)
	{
        m_pStream->read(buf , (std::streamsize)byte_read);
		return byte_read;
	}

    size_t   seek(long _offset, std::ios_base::seekdir dir)
	{
		m_pStream->seekg(_offset,dir);
        return (size_t)m_pStream->tellg();
	}

	size_t    tell()
	{
		return (size_t)m_pStream->tellg();
	}

	void set_stream(std::istream*  pStream)
	{
		m_pStream = pStream;
	}

	bool eof()
	{
		return m_pStream->eof();
	}
};

class CXComDocReadStream : public IStreamIO
{
    IMPL_NONE_REFCOUNT_OBJECT_INTERFACE(CXComDocReadStream);
	xcomdocstream* m_pStream;
public:
	CXComDocReadStream()
	{
		m_pStream = NULL;
	}
	~CXComDocReadStream()
	{
		m_pStream->Release();
	}

	int   close()
	{
        return 0;
	}

	size_t    read(_xcd_int8* buf, size_t byte_read)
	{
		m_pStream->read(buf,byte_read);
		return byte_read;
	}

	size_t   seek(long _offset, std::ios_base::seekdir dir)
	{
		xcd_seek_dir pos = xcdsd_cur;
		if(dir == std::ios_base::cur) pos = xcdsd_cur;
		else if(dir == std::ios_base::end) pos = xcdsd_end;
		else if(dir == std::ios_base::beg) pos = xcdsd_beg;
		else _offset = 0;

		m_pStream->stream_seekr(pos,_offset);
        return m_pStream->stream_tellr();
	}

	size_t    tell()
	{
		return m_pStream->stream_tellr();
	}

	void set_stream(xcomdocstream*  pStream)
	{
		m_pStream = pStream;
	}

	bool eof()
	{
		if( tell() >= (int)m_pStream->data_len() )
			return true;
		return false;
	}
};


class CReadBuffer
{
	IStreamIO* m_DiskFile;
	int          m_FileBegPos;
	int          m_FileNowPos;
	int          m_DataInFileLen;
public:
	int    get_file_read_pos()
	{
		return (int)m_DiskFile->tell();
	}
	void   set_file_read_pos(int pos)
	{
		m_DiskFile->seek(pos,std::ios_base::beg);
	}
	void   set_stream(IStreamIO* DiskFile,int beginPos, int len)
	{
		m_DiskFile = DiskFile;
		m_FileBegPos = m_FileNowPos = beginPos;
		m_DataInFileLen = len;
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

END_NAMESPACE_XEVOL3D

#endif
