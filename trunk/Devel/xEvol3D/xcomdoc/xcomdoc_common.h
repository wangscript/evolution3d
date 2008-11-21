#ifndef _xcomdoc_COMMON_H_
#define _xcomdoc_COMMON_H_
#include "xcomdoc.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include "../BaseLib/xI18N.h"
#endif
BEGIN_NAMESPACE_XEVOL3D
bool             xcd_is_dir(const wchar_t* _dir);
int              xcd_string_hash(const wchar_t* str , int* valueA , int* valueB);
inline bool      xcd_is_dir(xcd_data_type type){return (type & xcddt_dir_flag) != 0;}

class CMemIFStream : public IReadStream
{
	const _xcd_int8* m_Buffer;
	int              m_BufLen;
	int              m_read_pos;
public:
	CMemIFStream()
	{
		m_read_pos = 0;
		m_Buffer  = 0;
		m_BufLen = 0;
	}
	void   close()
	{
	}

	_xcd_int8*  get_buf()
	{
		return (_xcd_int8*)m_Buffer;
	}

	void        set_buf(const _xcd_int8* buf,int buf_len)
	{ 
		m_read_pos = 0 ;
		m_Buffer = buf;
		m_BufLen = buf_len;       
	}

	int    read(_xcd_int8* buf, int byte_read)
	{
		int byte_need_read = byte_read;
		if( m_BufLen - m_read_pos < byte_read )
		{
			byte_need_read = m_BufLen - m_read_pos;
		}
		if(byte_need_read == 0)
			return 0;
		memcpy(buf,&m_Buffer[m_read_pos],byte_need_read);
		m_read_pos += byte_need_read;
		return byte_need_read;
	}

	void   seekg(int _offset, ios::seekdir dir)
	{
		if(dir == std::ios::cur)
		{
			if( m_read_pos + _offset >= m_BufLen )
				m_read_pos = m_BufLen-1;
			else
				m_read_pos += _offset;
		}
		else if(dir == std::ios::beg)
		{
			if( _offset > (int)m_BufLen )
				m_read_pos = m_BufLen-1;               
			else
				m_read_pos = _offset;                
		}
		else if(dir == std::ios::end)
		{
			if( _offset >= (int)m_BufLen)
				m_read_pos = 0;  
			else
				m_read_pos = m_BufLen -  _offset - 1;
		}
		else
		{
			return ;
		}
	}

	int    tellg()
	{
		return m_read_pos;
	}

	bool eof()
	{
		return m_read_pos >= m_BufLen;
	}
};


inline void _xcd_wcsTombs(const wchar_t* ucsBuffer, char* mbsBuffer,int outBuffer)
{
#ifdef _WIN32
	WideCharToMultiByte(CP_ACP,0,ucsBuffer,(int)wcslen(ucsBuffer),mbsBuffer,outBuffer,NULL,NULL);
#else
	XEvol_UnicodeToLocale(ucsBuffer,mbsBuffer,outBuffer);
#endif
}

inline void _xcd_mbsTowcs(const char* mbsBuffer,  wchar_t* ucsBuffer,int outBuffer)
{
#ifdef _WIN32
	MultiByteToWideChar(CP_ACP , 0, mbsBuffer , (int)strlen(mbsBuffer) , ucsBuffer , outBuffer);
#else
	XEvol_LocaleToUnicode(mbsBuffer,ucsBuffer,outBuffer);
#endif
}

inline FILE* _xcd_wfopen_rb(const wchar_t* wcsFileName)
{
#ifdef _WIN32
	FILE* fp = _wfopen(wcsFileName , L"rb");
#else
	int8 mbsFileName[512]={0};
	_xcd_wcsTombs(wcsFileName,mbsFileName,512);
	FILE* fp = fopen(mbsFileName , "rb");
#endif
	return fp;
}

inline FILE* _xcd_wfopen_rwb(const wchar_t* wcsFileName)
{
#ifdef _WIN32
	FILE* fp = _wfopen(wcsFileName , L"wb+");
#else
	int8 mbsFileName[512]={0};
	_xcd_wcsTombs(wcsFileName,mbsFileName,512);
	FILE* fp = fopen(mbsFileName , "wb+");
#endif
	return fp;
}

inline FILE* _xcd_wfopen_rw_notCreate(const wchar_t* wcsFileName)
{
#ifdef _WIN32
	FILE* fp = _wfopen(wcsFileName , L"rb+");
#else
	int8 mbsFileName[512]={0};
	_xcd_wcsTombs(wcsFileName,mbsFileName,512);
	FILE* fp = fopen(mbsFileName , "rb+");
#endif
	return fp;
}

template <typename _tstream> void _xcd_wfstream_bin(const wchar_t* wcsFileName , _tstream& _stream )
{
#ifdef _WIN32
	_stream.open(wcsFileName , ios::binary);
#else
	int8 mbsFileName[512]={0};
	_xcd_wcsTombs(wcsFileName,mbsFileName,512);
	_stream.open(mbsFileName , ios::binary);
#endif

}




END_NAMESPACE_XEVOL3D

#endif
