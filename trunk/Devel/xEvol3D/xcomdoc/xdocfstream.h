/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) .
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

#ifndef __XCOMDOC_STREAM_BUFFER_H__
#define __XCOMDOC_STREAM_BUFFER_H__
/*
提供 C++ 标准库的扩展。
支持从一个xcomdocstream上建立一个c++ stream类。

作者： 潘李亮 2004 - 10 - 25 
xheartblue 2004-10-25

本代码为免费代码，可以只有使用和修改。
但是你得保留本声明和作者名字。

*/

#include "xcomdoc.h"

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file.hpp>



namespace std
{
   template <typename _Char > class xcomdocstream_device
   {
   public:
#ifdef _WIN32
	  typedef typename _Char char_type;
#else
	  typedef _Char char_type;
#endif
	   typedef boost::iostreams::seekable_device_tag             category;
       xcomdocstream_device(NS_XEVOL3D::xcomdocstream* pStream) 
	   { 
		   m_pStream = pStream;
	   }

	   std::streamsize read(char_type* s, std::streamsize n)
	   {
             m_pStream->read((_xcd_int8*)s , n * sizeof(char_type) );
			 return n;
	   }

	   std::streamsize write(const char_type* s, std::streamsize n)
	   {
           m_pStream->write((const _xcd_int8*)s, n * sizeof(char_type) );
		   return n;
	   }

	   boost::iostreams::stream_offset seek(boost::iostreams::stream_offset off, std::ios_base::seekdir way)
	   {
		   NS_XEVOL3D::xcd_seek_dir xcom_way = xcdsd_cur;
		   if(way == std::ios_base::cur) 
			   xcom_way = xcdsd_cur;
		   else if(way == std::ios_base::end) 
			   xcom_way = xcdsd_end;
		   else if(way == std::ios_base::beg) 
			   xcom_way = xcdsd_beg;
		   m_pStream->stream_seekr(xcom_way , off);
		   m_pStream->stream_seekw(xcom_way , off);
		   return m_pStream->stream_tellr();
	   }
   private:
	   //typedef typename Container::size_type   size_type;
	   typedef int                               size_type;
	   NS_XEVOL3D::xcomdocstream                 *m_pStream;
   };


   typedef boost::iostreams::stream< xcomdocstream_device<char>     > xcdstream ;
   typedef boost::iostreams::stream< xcomdocstream_device<wchar_t>  > wxcdstream;

}

#endif


