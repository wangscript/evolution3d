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

#ifndef __XEVOL_I18N_H__
#define __XEVOL_I18N_H__
#include "xEvol3DBaseInc.h"
#include "stl_dllalloc.h"
#include "xstring.h"
#ifdef _WIN32
#include <Windows.h>
#endif

BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_ xI18N
{
public:
    struct xLocaleInfo_t 
    {
#ifdef _WIN32
        LANGID           _langID;
#endif
        std::ds_wstring  _locale;
        std::ds_wstring  _langDispName;
        std::ds_wstring  _langName;
        int              _cpNumber;
    };
public:
    xI18N(); 
    ~xI18N();
    xLocaleInfo_t* getCurrentLocaleInfor();
#ifdef _WIN32      
    void           add(LANGID _id , const wchar_t* _langDispName, const wchar_t* _locale, const wchar_t* _langName , int cpNumber) ;
    xLocaleInfo_t* getByLangID(LANGID langID);
#else

#endif

    
private:
    typedef std::vector<xLocaleInfo_t> locals;
    locals         m_locals;
};   


//---------File Open Assit function----------------------
inline FILE* XEvol_OpenFileRB(const wchar_t* wcsFileName)
{
#ifdef _WIN32
	FILE* fp = _wfopen(wcsFileName , L"rb");
#else
	int8 mbsFileName[512]={0};
	XEvol_UnicodeToUtf8(wcsFileName,mbsFileName,512);
	FILE* fp = fopen(mbsFileName , "rb");
#endif
	return fp;
}

inline FILE* XEvol_OpenFileRB(const char  * ansiFileName)
{
	FILE* fp = fopen(ansiFileName , "rb");
	return fp;
}


inline FILE* XEvol_OpenFileWB(const wchar_t* wcsFileName)
{
#ifdef _WIN32
	FILE* fp = _wfopen(wcsFileName , L"wb");
#else
	int8 mbsFileName[512]={0};
	XEvol_UnicodeToUtf8(wcsFileName,mbsFileName,512);
	FILE* fp = fopen(mbsFileName , "wb");
#endif
	return fp;
}

inline FILE* XEvol_OpenFileWB(const char  * ansiFileName)
{
	FILE* fp = fopen(ansiFileName , "wb");
	return fp;
}
//-----------------------------------------------------
END_NAMESPACE_XEVOL3D

#endif

