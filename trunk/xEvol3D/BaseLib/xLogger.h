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

#ifndef __XEVOL3D_LOGGER_H__
#define __XEVOL3D_LOGGER_H__
#include "xEvol3DAPI.h"

BEGIN_NAMESPACE_XEVOL3D

enum eXR_LOG_LEVEL
{
    eXL_DEBUG_TIPS   = 10,
	eXL_DEBUG_NORMAL = 100,
	eXL_DEBUG_HIGH   = 1000,

	eXL_ERROR_NORMAL = 10000,
	eXL_ERROR_FALT   = 100000,
};

void _XEVOL_BASE_API_       XEVOL_LOG_SET_LEVEL(eXR_LOG_LEVEL lv);
void _XEVOL_BASE_API_       XEVOL_LOG_REDIR(const wchar_t* fileName);
void _XEVOL_BASE_API_       XEVOL_LOG_REDIR(const char* fileName);
void _XEVOL_BASE_API_       XEVOL_LOG_CLOSE();

void _XEVOL_BASE_API_       XEVOL_LOG(eXR_LOG_LEVEL logLevel , const char * fmt , ...);
void _XEVOL_BASE_API_       XEVOL_PRINTF_LOG(const char * fmt , ...);
void _XEVOL_BASE_API_       XEVOL_LOGTOFILE(eXR_LOG_LEVEL logLevel , const char * fmt , ...);

void _XEVOL_BASE_API_       XEVOL_LOG(eXR_LOG_LEVEL logLevel , const wchar_t * fmt , ...);
void _XEVOL_BASE_API_       XEVOL_PRINTF_LOG(const wchar_t * fmt , ...);
void _XEVOL_BASE_API_       XEVOL_LOGTOFILE(eXR_LOG_LEVEL logLevel , const wchar_t * fmt , ...);


void _XEVOL_BASE_API_       XEVOL_LOG_FLUSH();
void _XEVOL_BASE_API_       XEVOL_LOG_AUTOFLUSH(bool bAutoFlush);
bool _XEVOL_BASE_API_       XEVOL_LOG_ISATUOFLUSH();
bool _XEVOL_BASE_API_       XEVOL_SVAE_LOGINFO_FILE(const wchar_t* fileName , const char* data );
bool _XEVOL_BASE_API_       XEVOL_SVAE_LOGINFO_FILE(const wchar_t* fileName , const wchar_t* data );
#ifdef _DEBUG
#define XEVOL_ASSERT(val , strLog)  do{ if( val == false) XEVOL_LOG(eXL_DEBUG_HIGH,strLog) ; }while(0);
#else
#define XEVOL_ASSERT(val , strLog) 
#endif

#define XEVOL_ASSERT_POINTER(p , strLog) XEVOL_ASSERT(p != NULL , strLog)

#define XEVOL_WARNNING_NOT_IMPLEMENT           XEVOL_LOG(eXL_DEBUG_HIGH,"%s(%d): very important function is not implemented: %s   \n",__FILE__ , __LINE__ ,__FUNCTION__);
#define XEVOL_WARNNING_NOT_IMPLEMENT_INFO(s)   XEVOL_LOG(eXL_DEBUG_HIGH,"%s(%d): function=[%s]  %s  \n",__FILE__ , __LINE__ ,__FUNCTION__ , s);
END_NAMESPACE_XEVOL3D

#endif

