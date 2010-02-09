#ifndef __xXmlFileStream_H__
#define __xXmlFileStream_H__

#include "xEvol3DBaseInc.h"
#include "stl_dllalloc.h"
#include <string>
#include <vector>
#include <istream>
#include <ostream>
#include <sstream>
#include <map>

BEGIN_NAMESPACE_XEVOL3D

//====================================================
template<typename TStreamType> class xXmlUnicodeStream
{
	TStreamType&    _stream;
public:

	xXmlUnicodeStream(TStreamType& os,bool bAddHeader = false):_stream(os)
	{
		if(bAddHeader)
		{
			unsigned char buf[2];
			buf[0] = 0xFF;
			buf[1] = 0xFE;
			os.write((const char*)buf,2);
		}


	}

	xXmlUnicodeStream&  endl()
	{
		wchar_t c[20]={0};
		swprintf(c,20,L"\r\n");
		_stream.write((const char*)c, sizeof(wchar_t) * 2 );
		return *this;
	}

	xXmlUnicodeStream& operator<< (const wchar_t* str)
	{
		for(size_t i = 0 ; i < wcslen(str) ; i ++)
		{
			wchar_t wChar = str[i];

			if(wChar == '\n' )
			{
				wChar = '\\';
				_stream.write( (const char*)&wChar, sizeof(wchar_t) );
				wChar = 'n';
				_stream.write( (const char*)&wChar, sizeof(wchar_t) );

			}
			else if(wChar == '\t')
			{
				wChar = '\\';
				_stream.write( (const char*)&wChar, sizeof(wchar_t) );
				wChar = 't';
				_stream.write( (const char*)&wChar, sizeof(wchar_t) );
			}
			else if(wChar == '\r')
			{
				wChar = '\\';
				_stream.write( (const char*)&wChar, sizeof(wchar_t) );
				wChar = 'r';
				_stream.write( (const char*)&wChar, sizeof(wchar_t) );
			}
			else
			{
				_stream.write( (const char*)&wChar, sizeof(wchar_t) );
			}
		}
		return *this;
	}
};

template<> class xXmlUnicodeStream<std::wostringstream>
{
	std::wostringstream&    _stream;
public:

	xXmlUnicodeStream(std::wostringstream& os,bool bAddHeader = false):_stream(os)
	{
	}

	xXmlUnicodeStream&  endl()
	{
		_stream<<L"\n";
		return *this;
	}

	xXmlUnicodeStream& operator<< (const wchar_t* str)
	{
		_stream<<str;
		return *this;
	}
};


//====================================================
template<typename TStreamType> class xXmlUtf8Stream
{
protected:
	TStreamType&    _stream;
public:
	xXmlUtf8Stream(TStreamType& os,bool bAddHeader = false) : _stream(os)
	{
		if(bAddHeader)
		{
			unsigned char buf[3];
			buf[0] = UTF8_LEAD_0;
			buf[1] = UTF8_LEAD_1;
			buf[2] = UTF8_LEAD_2;
			os.write((const char*)buf,3);
		}
	}

    xXmlUtf8Stream&  endl()
	{
		char c[20]={0};
		sprintf(c,"\r\n");
		_stream.write((const char*)c, sizeof(char) * 2 );
		return *this;
	}

	xXmlUtf8Stream& operator<< (const wchar_t* _str)
	{
		char* str = new char[ wcslen(_str) * 3 ];
		memset(str , 0 , wcslen(_str) * 3 );
		XEvol_UnicodeToUtf8(_str , str , wcslen(_str) * 3  );

		for(size_t i = 0 ; i < strlen(str) ; i ++)
		{
			char wChar = str[i];

			if(wChar == '\n' )
			{
				wChar = '\\';
				_stream.write( (const char*)&wChar, sizeof(char) );
				wChar = 'n';
				_stream.write( (const char*)&wChar, sizeof(char) );

			}
			else if(wChar == '\t')
			{
				wChar = '\\';
				_stream.write( (const char*)&wChar, sizeof(char) );
				wChar = 't';
				_stream.write( (const char*)&wChar, sizeof(char) );
			}
			else if(wChar == '\r')
			{
				wChar = '\\';
				_stream.write( (const char*)&wChar, sizeof(char) );
				wChar = 'r';
				_stream.write( (const char*)&wChar, sizeof(char) );
			}
			else
			{
				_stream.write( (const char*)&wChar, sizeof(char) );
			}
		}
		return *this;
	}
};



template<typename TStreamType> class xXmlAnsiStream
{
protected:
	TStreamType&    _stream;
public:
	xXmlAnsiStream(TStreamType& os,bool bAddHeader = false) : _stream(os)
	{

	}

	xXmlAnsiStream&  endl()
	{
		char c[20]={0};
		sprintf(c,"\r\n");
		_stream.write((const char*)c, sizeof(char) * 2 );
		return *this;
	}

	xXmlAnsiStream& operator<< (const wchar_t* _str)
	{
		char* str = new char[ wcslen(_str) * sizeof(wchar_t) ];
		memset(str , 0 , wcslen(_str) * sizeof(wchar_t) );
		XEvol_UnicodeToLocale(_str , str , wcslen(_str) * sizeof(wchar_t)  );

		for(size_t i = 0 ; i < strlen(str) ; i ++)
		{
			char wChar = str[i];

			if(wChar == '\n' )
			{
				wChar = '\\';
				_stream.write( (const char*)&wChar, sizeof(char) );
				wChar = 'n';
				_stream.write( (const char*)&wChar, sizeof(char) );

			}
			else if(wChar == '\t')
			{
				wChar = '\\';
				_stream.write( (const char*)&wChar, sizeof(char) );
				wChar = 't';
				_stream.write( (const char*)&wChar, sizeof(char) );
			}
			else if(wChar == '\r')
			{
				wChar = '\\';
				_stream.write( (const char*)&wChar, sizeof(char) );
				wChar = 'r';
				_stream.write( (const char*)&wChar, sizeof(char) );
			}
			else
			{
				_stream.write( (const char*)&wChar, sizeof(char) );
			}
		}
		return *this;
	}
};

END_NAMESPACE_XEVOL3D

#endif