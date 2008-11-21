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

#include "xLogger.h"
//#include "../Application/Platform.h"
#include <stdio.h>
#include <stdarg.h>
#include "logstream.h"


BEGIN_NAMESPACE_XEVOL3D
static int g_forbiden_level = 0;

std::logstream& Get_Logger()
{
	static std::logstream logger;
	return logger;
}

void XEVOL_LOG_SET_LEVEL(eXR_LOG_LEVEL lv)
{
    g_forbiden_level = (int)lv;
	Get_Logger().set_forbidden_level( (int)lv );
}

void XEVOL_LOG_REDIR(const char* fileName)
{
	Get_Logger().open(fileName);
}

void XEVOL_LOG_REDIR(const wchar_t* fileName)
{
   Get_Logger().open(fileName);
}

void XEVOL_PRINTF_LOG(const wchar_t * fmt , ...)
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
	Get_Logger().traceLine((int)eXL_DEBUG_HIGH,buffer);
}

void XEVOL_LOG(eXR_LOG_LEVEL logLevel , const wchar_t * fmt , ...)
{
    if((int)logLevel < g_forbiden_level)
		return ;

	wchar_t buffer[1024] = {0}; 
	va_list args;
	va_start(args, fmt);
#ifdef _WIN32
	_vsnwprintf(buffer, 1024, fmt, args);
#else
	vswprintf(buffer, 1024, fmt, args);
#endif
	va_end(args);
	Get_Logger().traceLine((int)logLevel,buffer);
}

void XEVOL_LOGTOFILE(eXR_LOG_LEVEL logLevel , const wchar_t * fmt , ...)
{
	if((int)logLevel < g_forbiden_level)
		return ;
	wchar_t buffer[1024] = {0}; 
	va_list args;
	va_start(args, fmt);
#ifdef _WIN32
	_vsnwprintf(buffer, 1024, fmt, args);
#else
	vswprintf(buffer, 1024, fmt, args);
#endif
	va_end(args);
	Get_Logger().traceLine((int)logLevel,buffer);
	Get_Logger().flush();
}


//=================ANSI Log===========================

void XEVOL_PRINTF_LOG(const char * fmt , ...)
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
	Get_Logger().traceLine((int)eXL_DEBUG_HIGH,buffer);
}

void XEVOL_LOG(eXR_LOG_LEVEL logLevel , const char * fmt , ...)
{
	if((int)logLevel < g_forbiden_level)
		return ;
	char buffer[1024] = {0}; 
	va_list args;
	va_start(args, fmt);
#ifdef _WIN32
	_vsnprintf(buffer, 1024, fmt, args);
#else
	vsnprintf(buffer, 1024, fmt, args);
#endif
	va_end(args);
	Get_Logger().traceLine((int)logLevel,buffer);
}

void XEVOL_LOGTOFILE(eXR_LOG_LEVEL logLevel , const char * fmt , ...)
{
	if((int)logLevel < g_forbiden_level)
		return ;
	char buffer[1024] = {0}; 
	va_list args;
	va_start(args, fmt);
#ifdef _WIN32
	_vsnprintf(buffer, 1024, fmt, args);
#else
	vsnprintf(buffer, 1024, fmt, args);
#endif
	va_end(args);
	Get_Logger().traceLine((int)logLevel,buffer);
	Get_Logger().flush();
}

void XEVOL_LOG_FLUSH()
{
     Get_Logger().flush();
}

void  XEVOL_LOG_CLOSE()
{
	Get_Logger().close();
}
void  XEVOL_LOG_AUTOFLUSH(bool bAutoFlush)
{
	Get_Logger().enableAutoFlush(bAutoFlush);
}

bool  XEVOL_LOG_ISATUOFLUSH()
{
	return Get_Logger().autoFlush();
}


bool XEVOL_SVAE_LOGINFO_FILE(const wchar_t* fileName , const char* data )
{
	char ansiFileName[512] = {0};
	XEvol_UnicodeToLocale(fileName , ansiFileName , 512);
	for(size_t i = 3 ;i < strlen(ansiFileName) ; i ++)
	{
		if(ansiFileName[i] == ':') ansiFileName[i] = ' ';
		if(ansiFileName[i] == '<') ansiFileName[i] = '(';
		if(ansiFileName[i] == '>') ansiFileName[i] = ')';
		if(ansiFileName[i] == ';') ansiFileName[i] = ' ';
		//if(ansiFileName[i] == '.') ansiFileName[i] = '_';
	}
	std::ofstream out(ansiFileName);//, std::ios_base::binary);
	out.write(data , strlen(data) );
	out.close();
	return true;
}

bool XEVOL_SVAE_LOGINFO_FILE(const wchar_t* fileName , const wchar_t* data )
{
	char ansiFileName[512] = {0};
	XEvol_UnicodeToLocale(fileName , ansiFileName , 512);
	for(size_t i = 3 ;i < strlen(ansiFileName) ; i ++)
	{
		if(ansiFileName[i] == ':') ansiFileName[i] = ' ';
		if(ansiFileName[i] == '<') ansiFileName[i] = '(';
		if(ansiFileName[i] == '>') ansiFileName[i] = ')';
		if(ansiFileName[i] == ';') ansiFileName[i] = ' ';
		//if(ansiFileName[i] == '.') ansiFileName[i] = '_';
	}
	std::wofstream out(ansiFileName);
	out<<data<<std::endl;
	out.close();
	return true;
}

END_NAMESPACE_XEVOL3D
