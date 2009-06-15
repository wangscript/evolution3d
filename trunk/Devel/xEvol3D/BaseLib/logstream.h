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

#ifndef __LOGSYTEM_H__
#define __LOGSYTEM_H__
/*
stanlylee's log system
module name : logstream
*/
#ifdef _WIN32
#include <Windows.h>
#endif
#include <fstream>
#include <iostream>
#include <stdarg.h>
#include "xI18N.h"
#include "stl_dllalloc.h"
#include <vector>
namespace std
{

    template <typename Type,typename _Traits> class basic_logstream
    {

		std::ds_string_vector m_Buffers;

		void __flushBuffer()
		{
             for(size_t i = 0 ; i < m_Buffers.size() ; i ++)
			 {
				 m_filestream<<m_Buffers[i].c_str();
			 }
			 m_Buffers.clear();
		}

		void __insert2Buffer(const char* str)
		{
			if(m_Buffers.size() > 256) return;
			m_Buffers.push_back(str);
		}

        void init()
        {
            m_curLevel = 1;
            m_forbiddenLevel = 1;
			m_bConsoleOutput  = true;
			m_bAutoFlush = false;
			m_bOpened    = false;
        }
    public:
        typedef  basic_ofstream<Type,_Traits>     _MyFileStrem;
        typedef  basic_ios<Type, _Traits>        _MyBaseios;
        typedef  basic_logstream<Type,_Traits>    _Myos;
        typedef  basic_ostream<Type,_Traits>      _MyBasOs;


        enum logmode
        {
           log_common = 0x00000000,
           log_html   = 0x00000001,
        };


		void enableConsoleOutput(bool bConsole)
		{
			m_bConsoleOutput = bConsole;
		}
        basic_logstream()
        {
            init();
        }
		~basic_logstream()
		{
			close();
		}

        basic_logstream(const char* _filename,bool append = false,logmode lmode= log_common)
        {
            init();
            open(_filename,append,lmode);
        }

        //basic operation like fstream;
        void open(const char* _filename,bool append = false,logmode lmode= log_common)
        {
			m_bOpened = true;
            m_filestream.open(_filename);
			trace(1024,"Open log file:");
			trace(1024,_filename);
			trace(1024,"\n");
			__flushBuffer();
			
        }

		void open(const wchar_t* _filename,bool append = false,logmode lmode= log_common)
		{
			m_bOpened = true;
#ifndef _WIN32
			char ansiFileName[512] = {0};
			XEvol_UnicodeToLocale(_filename,ansiFileName,512);
			m_filestream.open(ansiFileName);
#else
			m_filestream.open(_filename);
#endif
			trace(1024,"Open log file:");
			trace(1024,_filename);
			trace(1024,"\n");
			__flushBuffer();

		}

        void close()
        {
			m_Buffers.clear();
			if(m_bOpened)
			{
				trace(1024,"Close Logger!\n");
				m_filestream.close();
				m_bOpened = false;
			}
            return;
        }

        bool fail()
        {
            return m_filestream.fail();
        }

        bool is_open()
        {
            return m_filestream.is_open();
        }

        bool bad()
        {
            return m_filestream.bad();
        }

        void setstate(ios_base::iostate _state,bool _Expcept = false)
        {
            m_filestream.setstate(_state,_Expcept);
        }
        void setstate(ios_base::iostate _state)
        {
            m_filestream.setstate(_state);
        }
        void clear(ios_base::iostate _state = ios_base::goodbit,bool _Expcept = false)
        {
            m_filestream.clear(_state,_Expcept);
        }

        void clear(ios_base::iostate _state)
        {
            m_filestream.clear(_state);
        }

        Type fill(Type _Newfill)
        {
            return m_filestream.fill(_Newfill);
        }

        Type fill()
        {
            return m_filestream.fill();
        }

        ios::fmtflags flags()
        {
            return  m_filestream.flags();
        }

        ios::fmtflags flags(ios::fmtflags _Newflags)
        {
            m_filestream.flush();
            return  m_filestream.flags(_Newflags);
        }

		void  enableAutoFlush(bool bAutoFlush)
		{
			m_bAutoFlush = bAutoFlush;
		}

		bool autoFlush()
		{
			return m_bAutoFlush;
		}

        _Myos& flush()
        {
            m_filestream.flush();
            return *this;
        }

        bool operator !()
        {
              return !m_filestream;
        }
        operator void* ()
        {
            return (void*)m_filestream;
        }
        bool opfx()
        {
            return m_filestream.opfx();
        }
        void osfx()
        {
            m_filestream.osfx();
        }
        streamsize precision()
        {
            return m_filestream.precision();
        }
        streamsize precision(streamsize _NewPrecision)
        {
             return m_filestream.precision(_NewPrecision);
        }

        streamsize width()
        {
            return m_filestream.width();
        }

        streamsize width(streamsize _NewPrecision)
        {
            return m_filestream.width(_NewPrecision);
        }

        //operator <<
        template <typename DataType>_Myos& operator<<(DataType data)
        {
               if(m_curLevel >= m_forbiddenLevel)
               {
                   m_filestream<<data;
				   if(m_bAutoFlush) flush();
			       if(m_bConsoleOutput)
				   {
					   cout<<data;
				   }
               }
               return *this;
        }

		template <const wchar_t*> _Myos& operator<<(const wchar_t* data)
		{
			trace(m_curLevel,data);
			return *this;
		}

		template <wchar_t*>_Myos& operator<<(wchar_t* data)
		{
			trace(m_curLevel,data);
			return *this;
		}

		template <wstring&>_Myos& operator<<(wstring& data)
		{
			trace(m_curLevel,data.c_str());
			return *this;
		}

		template <const wstring&>_Myos& operator<<(const wstring& data)
		{
			trace(m_curLevel,data.c_str());
			return *this;
		}

		typedef _MyBasOs& ( * PFBASEOSFUNC)(_MyBasOs&) ;
		typedef ios_base& ( * PFIOBASEOSFUNC)(ios_base&) ;
		typedef _MyBaseios& ( * PFMYBASEOSFUNC)(_MyBaseios&) ;
        //manipulators
        _Myos& operator<<(PFBASEOSFUNC _Pfn)
        {	// call basic_ostream manipulator
            ((*_Pfn)(m_filestream));
            return *this;
        }

        _Myos& operator<<(PFMYBASEOSFUNC _Pfn)
        {	// call basic_ios manipulator
            (*_Pfn)(m_filestream);
            return (*this);
        }

        _Myos& operator<<(PFIOBASEOSFUNC _Pfn)
        {	// call ios_base manipulator
            (*_Pfn)(*(ios_base *)this);
            return (*this);
        }


        void set_forbidden_level(int level)
        {
            m_forbiddenLevel = level;
        }

        void set_level(int level)
        {
            m_curLevel = level;
        }


		void traceLine(int level, const char* _text)
		{
			if(level >= m_forbiddenLevel)
			{
#ifdef _WIN32
#ifndef _WIN32_WCE
				OutputDebugStringA(_text);
#endif
#endif
				printf(_text);
				if(m_bOpened)
				{
					m_filestream<<_text;
					if(m_bAutoFlush) 
						flush();
				}
				else
				{
					__insert2Buffer(_text);
				}
				
				
			}
		}


        /**
        Here is the format;
        */
        void trace(const char* fmt, ...)
        {
			if(m_curLevel >= m_forbiddenLevel)
            {
				char buffer[1024] = {0}; 
                va_list args;
                va_start(args, fmt);
#ifdef _WIN32
				_vsnprintf(buffer, 1024, fmt, args);
#else
                vsnprintf(buffer, 1024, fmt, args);
#endif
                va_end(args);
                traceLine(m_curLevel , buffer);
            }
        }

        void trace(int level, const char* fmt, ...)
        {
			if(level >= m_forbiddenLevel)
            {
				char buffer[1024] = {0}; 
                va_list args;
                va_start(args, fmt);
#ifdef _WIN32
				_vsnprintf(buffer, 1024, fmt, args);
#else
				vsnprintf(buffer, 1024, fmt, args);
#endif
                va_end(args);
                traceLine(m_curLevel , buffer);
            }
        }

		//============Unicode formating log=================
		void trace(const wchar_t* fmt, ...)
		{
			if(m_curLevel >= m_forbiddenLevel)
			{
				wchar_t buffer[1024] = {0}; 
				va_list args;
				va_start(args, fmt);
#ifdef _WIN32
				_vsnwprintf(buffer, 1024, fmt, args);
#else
				vswprintf(buffer, 1024, fmt, args);
#endif
				va_end(args);
				traceLine(m_curLevel , buffer);
			}
		}

		void trace(int level , const wchar_t* fmt, ...)
		{
			if(m_curLevel >= m_forbiddenLevel)
			{
				wchar_t buffer[1024] = {0}; 
				va_list args;
				va_start(args, fmt);
#ifdef _WIN32
				_vsnwprintf(buffer, 1024, fmt, args);
#else
				vswprintf(buffer, 1024, fmt, args);
#endif
				va_end(args);
				traceLine(level , buffer);
			}
		}


		void traceLine(int level , const wchar_t* _text)
		{
			 char buf[1024]={0};
			 NS_XEVOL3D::XEvol_UnicodeToLocale(_text,buf,1024);
			 traceLine(level,buf);
		}
		//===================================================

    private:
        _MyFileStrem       m_filestream;
        bool               m_bConsoleOutput;
        int                m_curLevel;
        int                m_forbiddenLevel;
		bool               m_bAutoFlush;
		bool               m_bOpened;

    };

    typedef basic_logstream<char    , char_traits<char>    > logstream;
    typedef basic_logstream<wchar_t , char_traits<wchar_t> > wlogstream;

}
#endif

