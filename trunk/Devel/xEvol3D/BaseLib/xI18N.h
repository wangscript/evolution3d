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


#ifdef _WIN32
#define CHARSET_UNICODE16   "UCS-2-INTERNAL"//"UNICODE"
#define CHARSET_UNICODE32   "UCS-4-INTERNAL"//"UNICODE"
#else
#define CHARSET_UNICODE16   "UCS-2-INTERNAL"
#define CHARSET_UNICODE32   "UCS-4-INTERNAL"
#endif

#define CHARSET_UTF8      "UTF-8"
#define CHARSET_UTF16     "UTF-16"
#define CHARSET_UTF32     "UTF-32"


#define CHARSET_GB2312    "GB2312"
#define CHARSET_GBK       "GBK"
#define CHARSET_GB18030   "GB18030"

class _XEVOL_BASE_API_  xStringConverter
{
	void*       m_handle;
public:
	xStringConverter();
	~xStringConverter();
	bool        setLanguge(const char* _to , const char* _from);
	bool        setUTF82Locale();
	bool        setLocale2UTF8();

	bool        setUTF82Unicode();
	bool        setUnicode2UTF8();

	bool        setUnicode2Locale();
	bool        setLocale2Unicode();
	bool        setUCS2ToUCS4();
	bool        setUCS4ToUCS2();

	bool        convert(const char* _in, char* _out,int inLen,int outlen);
	bool        convert(const char* _in, char* _out,int outlen);
	bool        convert(const wchar_t* _in, char* _out,int outlen);
	const char* getLocaleLang();
	const char* getLocaleCharSet();
};

bool _XEVOL_BASE_API_  XEvol_Utf8ToUnicode(const char* _in, wchar_t* _out,int outlen);
bool _XEVOL_BASE_API_  XEvol_UnicodeToUtf8(const wchar_t* _in, char* _out,int outlen);
bool _XEVOL_BASE_API_  XEvol_LocaleToUtf8(const char* _in, char* _out,int outlen);
bool _XEVOL_BASE_API_  XEvol_Utf8ToLocale(const char* _in, char* _out,int outlen);
bool _XEVOL_BASE_API_  XEvol_UnicodeToLocale(const wchar_t* _in, char* _out,int outlen);
bool _XEVOL_BASE_API_  XEvol_LocaleToUnicode(const char* _in, wchar_t* _out,int outlen);
bool _XEVOL_BASE_API_  XEvol_UCS4ToUCS2(unsigned int* _in, unsigned short* _out, int inLen , int outlen);
bool _XEVOL_BASE_API_  XEvol_UCS2ToUCS4(unsigned short* _in, unsigned int* _out, int inLen , int outlen);


#endif

