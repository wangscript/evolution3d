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

#include "xI18N.h"
#include <libcharset.h>
#include <string>
#include <vector>
#include <wchar.h>
using namespace std;
#ifdef _WIN32
#pragma  comment(lib , "libiconv.lib")
#endif
#include <iconv.h>
bool XEvol_Utf8ToUnicode(const char* _in, wchar_t* _out,int outlen)
{
    static xStringConverter g_strCovert;
	static bool g_bInited = false;
	if(g_bInited == false)
	{
		g_strCovert.setUTF82Unicode();
		g_bInited = true;
	}
	return g_strCovert.convert(_in, (char*)_out,outlen);
}

bool XEvol_UCS2ToUCS4(unsigned short* _in, unsigned int* _out,int inLen ,int outlen)
{
	static xStringConverter g_strCovert;
	static bool g_bInited = false;
	if(g_bInited == false)
	{
		g_strCovert.setUCS2ToUCS4();
		g_bInited = true;
	}
	return g_strCovert.convert((char*)_in, (char*)_out,inLen,outlen);
}


bool XEvol_UCS4ToUCS2(unsigned int* _in, unsigned short* _out,int inLen ,int outlen)
{
	static xStringConverter g_strCovert;
	static bool g_bInited = false;
	if(g_bInited == false)
	{
		g_strCovert.setUCS4ToUCS2();
		g_bInited = true;
	}
	return g_strCovert.convert((char*)_in, (char*)_out,inLen,outlen);
}

bool XEvol_UnicodeToUtf8(const wchar_t* _in, char* _out,int outlen)
{
	static xStringConverter g_strCovert;
	static bool g_bInited = false;
	if(g_bInited == false)
	{
		g_strCovert.setUnicode2UTF8();
		g_bInited = true;
	}
	return g_strCovert.convert(_in,_out,outlen);
}


bool XEvol_LocaleToUtf8(const char* _in, char* _out,int outlen)
{
	static xStringConverter g_strCovert;
	static bool g_bInited = false;
	if(g_bInited == false)
	{
		g_strCovert.setLocale2UTF8();
		g_bInited = true;
	}
	return g_strCovert.convert(_in,_out,outlen);

}


bool XEvol_Utf8ToLocale(const char* _in, char* _out,int outlen)
{
	static xStringConverter g_strCovert;
	static bool g_bInited = false;
	if(g_bInited == false)
	{
		g_strCovert.setUTF82Locale();
		g_bInited = true;
	}
	return g_strCovert.convert(_in,_out,outlen);
}

bool XEvol_UnicodeToLocale(const wchar_t* _in, char* _out,int outlen)
{
#ifdef _WIN32
	static xStringConverter g_strCovert;
	static bool g_bInited = false;
	if(g_bInited == false)
	{
		g_strCovert.setUnicode2Locale();
		g_bInited = true;
	}
	return g_strCovert.convert(_in,_out,outlen);
#endif
#ifdef _LINUX
    return 0 != wcstombs (_out, _in, wcslen(_in));
#endif
}

bool XEvol_LocaleToUnicode(const char* _in, wchar_t* _out,int outlen)
{
#ifdef _WIN32
	static xStringConverter g_strCovert;
	static bool g_bInited = false;
	if(g_bInited == false)
	{
		g_strCovert.setLocale2Unicode();
		g_bInited = true;
	}
	return g_strCovert.convert(_in,(char*)_out,outlen);
#endif
#ifdef _LINUX
	return 0 != mbstowcs(_out, _in, strlen(_in));
#endif
}


class xLang2LocaleMap
{
public:
    struct xLocaleInfo_t 
    {
		string _locale;
		string _lang;
		string _charset;
    };
private:
	typedef vector<xLocaleInfo_t> locals;
	locals m_locals;
	const char* m_localCharset;
public:
	xLang2LocaleMap()
	{
		add("en"   ,	"ASCII",		"ASCII");		
		add("zh_TW",	"BIG5",			"BIG5");		
		add("zh_CN",	"GBK" ,		"GB2312");		
		add("ja"   ,	"SHIFT-JIS",	"SHIFT-JIS");	
		add("ko",		"EUC-KR",		"EUC-KR");		

		add("ar",		"ISO-8859-6",	"ISO-8859-6");	
		add("da",		"ISO-8859-1", 	"ISO-8859-1");
		add("du",		"ISO-8859-1",	"ISO-8859-1");
		add("fi",		"ISO-8859-1",	"ISO-8859-1");
		add("fr",		"ISO-8859-1", 	"ISO-8859-1");

		add("de",		"ISO-8859-1",	"ISO-8859-1");
		add("el",		"ISO-8859-7",	"ISO-8859-7");	
		add("iw",		"ISO-8859-8", 	"ISO-8859-8");	
		add("it",		"ISO-8859-1", 	"ISO-8859-1");	
		add("no",		"ISO-8859-1",	"ISO-8859-1");	

		add("pt_BR",	"ISO-8859-1",	"ISO-8859-1");	
		add("es",		"ISO-8859-1",	"ISO-8859-1");	
		add("sv",		"ISO-8859-1", 	"ISO-8859-1");	
		add("pl",		"ISO-8859-2",	"CP1250");		
		add("ru",		"Cp1251", 		"CP1251");		

		add("tr",		"ISO-8859-9",	"ISO-8859-9");	
		add("sl",		"ISO-8859-5",	"CP1250");		
		add("cz",		"ISO-8859-5",	"CP1250");		
		add("frc",		"ISO-8859-5",	"CP1252");		
		add("hu",		"ISO-8859-5",	"CP1250");		
		add("pt",		"ISO-8859-1",	"ISO-8859-1");
		m_localCharset = locale_charset();
	}

	void   add(const char* _lang, const char* _charset, const char* _locale)
	{
		xLocaleInfo_t info;
		info._charset = _charset;
		info._lang    = _lang;
		info._locale  = _locale;
		m_locals.push_back(info);      
	}

	xLocaleInfo_t* getByLang(const char* _lang)
	{
		for(size_t i = 0 ; i < m_locals.size() ; ++i)
		{
			if(m_locals[i]._lang == _lang)
				return &m_locals[i];
		}
		return NULL;
	}
};

xLang2LocaleMap gs_Lang2LocaleMap;
xStringConverter::xStringConverter()
{
    m_handle = (iconv_t)-1;
}

xStringConverter::~xStringConverter()
{
    if(m_handle != (iconv_t)-1)
		iconv_close(m_handle);
}

bool  xStringConverter::setUTF82Locale()
{
   return setLanguge(getLocaleCharSet(),"UTF-8");
}

bool  xStringConverter::setLocale2UTF8()
{
   return setLanguge("UTF-8",getLocaleCharSet());
}

bool xStringConverter::setUCS2ToUCS4()
{
	return setLanguge(CHARSET_UNICODE32 , CHARSET_UNICODE16);
}

bool xStringConverter::setUCS4ToUCS2()
{
	return setLanguge(CHARSET_UNICODE16 , CHARSET_UNICODE32);
}

bool xStringConverter::setUTF82Unicode()
{
   if(sizeof(wchar_t) == 2)  return setLanguge(CHARSET_UNICODE16 , "UTF-8");
   if(sizeof(wchar_t) == 4)  return setLanguge(CHARSET_UNICODE32 , "UTF-8");
   return false;
}

bool xStringConverter::setUnicode2UTF8()
{
   if(sizeof(wchar_t) == 2)  return setLanguge("UTF-8" , CHARSET_UNICODE16);
   if(sizeof(wchar_t) == 4)  return setLanguge("UTF-8" , CHARSET_UNICODE32);
   return false;
}

bool  xStringConverter::setUnicode2Locale()
{
	if(sizeof(wchar_t) == 2)  return setLanguge(getLocaleCharSet(),CHARSET_UNICODE16);
	if(sizeof(wchar_t) == 4)  return setLanguge(getLocaleCharSet(),CHARSET_UNICODE32);
	return false;
}

bool  xStringConverter::setLocale2Unicode()
{
	if(sizeof(wchar_t) == 2)  return setLanguge(CHARSET_UNICODE16,getLocaleCharSet());
	if(sizeof(wchar_t) == 4)  return setLanguge(CHARSET_UNICODE32,getLocaleCharSet());
	return false;
}

bool  xStringConverter::setLanguge(const char* _to , const char* _from)
{
    if(m_handle != (iconv_t)-1 )
	{
		iconv_close(m_handle);
		m_handle = (iconv_t)-1;
	}
	m_handle = iconv_open(_to,_from);
	return m_handle != (iconv_t)-1;
}

bool xStringConverter::convert(const char* _in, char* _out,int inLen,int outlen)
{
	if(m_handle == (iconv_t)-1 )
		return false;
	size_t _bufInLen = inLen;
	size_t _bufOutLen = outlen;
#ifdef _WIN32
	bool ret =  iconv(m_handle , &_in,&_bufInLen,&_out,&_bufOutLen) == 0;
#else
	bool ret =  iconv(m_handle , (char**)&_in,&_bufInLen,(char**)&_out,&_bufOutLen) == 0;
#endif
	return ret;
}
bool xStringConverter::convert(const char* _in, char* _out,int outlen)
{
    if(m_handle == (iconv_t)-1 )
		return false;
	size_t _bufInLen = strlen(_in);
	return convert(_in , _out , (int)_bufInLen , (int)outlen);
}

bool xStringConverter::convert(const wchar_t* _in, char* _out,int outlen)
{
	if(m_handle == (iconv_t)-1 )
		return false;
	size_t _bufInLen = wcslen(_in) * 2;
	return convert((const char*)_in , _out , (int)_bufInLen , (int)outlen);
}

const char* xStringConverter::getLocaleLang()
{
#ifdef _WIN32_WCE
	return "GBK";
#else
    return getenv("LANG");
#endif
}

const char* xStringConverter::getLocaleCharSet()
{
	return locale_charset();

#ifdef _WIN32_WCE
	char* lang =  "GBK";
#else
	char* lang =  getenv("LANG");
#endif
	xLang2LocaleMap::xLocaleInfo_t* pInfo = gs_Lang2LocaleMap.getByLang(lang);
	if(pInfo == NULL)
		return NULL;
	return pInfo->_charset.c_str();
}


