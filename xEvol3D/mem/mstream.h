/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) .
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

#ifndef __MEM_STREAM_BUFFER_H__
#define __MEM_STREAM_BUFFER_H__
/*
提供 C++ 标准库的扩展。支持从一段内存上建立一个istream类。

作者： 潘李亮 2004 - 10 - 25 
Xheartblue 2004-10-25

本代码为免费代码，可以只有使用和修改。
但是你得保留本声明和作者名字。

*/
#include <streambuf>

namespace std
{

	template <typename _CharT, typename _CharTrait = char_traits<_CharT> >	
	class mem_streambuf : public basic_streambuf<_CharT,_CharTrait>
	{
	public: //类型的定义 
		typedef _CharT                             streambuf_type;
		typedef _CharT                             char_type;
		typedef int                                int_type;
		typedef typename std::streampos            pos_type;
		typedef typename std::streamsize           off_type;

	protected:
		size_t       m_bufLen;
		char_type*   m_buffer;
		bool         m_bIsAllocInner;
	public:

		//构造函数
		mem_streambuf(char_type* buf,size_t len)
		{
			set_buf(buf,len);
		}
		mem_streambuf()
		{
			m_buffer = NULL;
			m_bufLen = 0;
			m_bIsAllocInner = false;	
		}
		mem_streambuf(size_t len)
		{
			m_buffer = NULL;
			m_bufLen = 0;
			m_bIsAllocInner = false;
			alloc_buf(len);
			_init();
		}
		~mem_streambuf()
		{
			dealloc_buf();
		}

		bool  alloc_buf(int len)
		{
			dealloc_buf();
			m_buffer = new char_type[len];
			m_bIsAllocInner = true;
			m_bufLen = len;
			return true;
		}
		bool  set_buf(char_type* buf,size_t len)
		{
			m_buffer = buf;
			m_bufLen = len;
			m_bIsAllocInner = false;
			_init();
			return true;
		}
		bool  dealloc_buf()
		{
			if(m_bIsAllocInner)
			{
				delete [] m_buffer;
			}
			m_buffer = NULL;
			m_bufLen = 0;
			m_bIsAllocInner = false;
			return true;
		}

		char_type* get_buf()
		{
			return m_buffer;
		}

	protected:
		void _init()
		{
			setg(m_buffer,m_buffer,m_buffer+ m_bufLen);
			setp(m_buffer,m_buffer,m_buffer+ m_bufLen);
		}

	};//endl class mem_streambuf;

	template <typename _CharT , typename _CharTrait = char_traits<_CharT> >
	class basic_imemstream : public basic_istream<_CharT,_CharTrait>
	{
	public:
		typedef mem_streambuf<_CharT,_CharTrait> stream_type;
		typedef _CharT                           char_type;
	protected:
		stream_type m_buffer;

	public:
		basic_imemstream(stream_type* buf): basic_istream<_CharT,_CharTrait>(buf)
		{
		}

		basic_imemstream(char_type* buf,size_t len)
			:m_buffer(buf,len),basic_istream<_CharT,_CharTrait>(&m_buffer)
		{
		}

		basic_imemstream():m_buffer(),basic_istream<_CharT,_CharTrait>(&m_buffer)
		{

		}

		basic_imemstream(int len)
			:m_buffer(len),basic_istream<_CharT,_CharTrait>(&m_buffer)
		{
		}

		void set_buf(char_type* buf,int len)
		{
			m_buffer.set_buf(buf,len);
		}

		char_type* get_buf()
		{
			return m_buffer.get_buf();
		}

		basic_streambuf<_CharT,_CharTrait>* rdbuf()
		{
			return &m_buffer;
		}

		void      rdbuf(basic_streambuf<_CharT,_CharTrait>* buf)
		{
			m_buffer = buf;
		}

	};


	template <typename _CharT , typename _CharTrait = char_traits<_CharT> >
	class basic_omemstream : public basic_ostream<_CharT,_CharTrait>
	{
	public:
		typedef mem_streambuf<_CharT,_CharTrait> stream_type;
		typedef _CharT                           char_type;
	protected:
		stream_type m_buffer;

	public:
		basic_omemstream(stream_type* buf): basic_ostream<_CharT,_CharTrait>(buf)
		{
		}

		basic_omemstream(char_type* buf,size_t len)
			:m_buffer(buf,len),basic_ostream<_CharT,_CharTrait>(&m_buffer)
		{
		}

		basic_omemstream():m_buffer(),basic_ostream<_CharT,_CharTrait>(&m_buffer)
		{

		}

		basic_omemstream(int len)
			:m_buffer(len),basic_ostream<_CharT,_CharTrait>(&m_buffer)
		{
		}

		void set_buf(char_type* buf,int len)
		{
			m_buffer.set_buf(buf,len);
		}

		char_type* get_buf()
		{
			return m_buffer.get_buf();
		}

		basic_streambuf<_CharT,_CharTrait>* rdbuf()
		{
			return &m_buffer;
		}

		void      rdbuf(basic_streambuf<_CharT,_CharTrait>* buf)
		{
			m_buffer = buf;
		}

	};

	template <typename _CharT , typename _CharTrait = char_traits<_CharT> >
	class basic_memstream : public basic_ostream<_CharT,_CharTrait>,public basic_istream<_CharT,_CharTrait>
	{
	public:
		typedef mem_streambuf<_CharT,_CharTrait> stream_type;
		typedef _CharT                           char_type;
	protected:
		stream_type m_buffer;

	public:
		basic_memstream(stream_type* buf)
			: basic_ostream<_CharT,_CharTrait>(buf),
			basic_istream<_CharT,_CharTrait>(buf)
		{
		}

		basic_memstream(char_type* buf,size_t len)
			:m_buffer(buf,len),
			basic_ostream<_CharT,_CharTrait>(&m_buffer),
			basic_istream<_CharT,_CharTrait>(&m_buffer)
		{
		}

		basic_memstream()
			:m_buffer(),
			basic_ostream<_CharT,_CharTrait>(&m_buffer),
			basic_istream<_CharT,_CharTrait>(&m_buffer)
		{
		}

		basic_memstream(int len)
			:m_buffer(len),
			basic_istream<_CharT,_CharTrait>(&m_buffer),
			basic_ostream<_CharT,_CharTrait>(&m_buffer)
		{
		}

		void set_buf(char_type* buf,int len)
		{
			m_buffer.set_buf(buf,len);
		}

		char_type* get_buf()
		{
			return m_buffer.get_buf();
		}

		basic_streambuf<_CharT,_CharTrait>* rdbuf()
		{
			return &m_buffer;
		}

		void      rdbuf(basic_streambuf<_CharT,_CharTrait>* buf)
		{
			m_buffer = buf;
		}

	};

	typedef basic_imemstream<char>    imemstream;
	typedef basic_imemstream<wchar_t> wimemstream;
	typedef basic_omemstream<char>    omemstream;
	typedef basic_omemstream<wchar_t> womemstream;
	typedef basic_memstream<char>     memstream;
	typedef basic_memstream<wchar_t>  wmemstream;

}
#endif


