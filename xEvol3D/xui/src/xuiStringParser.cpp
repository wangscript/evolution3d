/*
XReal3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xreal3d/) 
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

#include "../../xStdPch.h"
#include <assert.h>
#include "../xuiStringParser.h"

BEGIN_NAMESPACE_XUI
bool strUtf8ToUnicode(const char* _utfString , wchar_t* out_buf , int len)
{
    return XEvol_Utf8ToUnicode(_utfString , out_buf , len) == conversionOK;

}

bool string2Region(const wchar_t* _str , xuiRegion& region)
{
    static std::wstring str;
    str=L"";
    static std::wstring typeStr;
    typeStr = L"";
    int i = 0;
    for(i = 0 ; i < (int)wcslen(_str) ; i ++)
    {
        if(_str[i] == '[')
        {
            break;
        }
        if( _str[i] != ' ' && _str[i] != '\t')
        {
            typeStr.push_back(_str[i]);              
        }
    }

    for(; i < (int)wcslen(_str) ; i ++) 
    {
        if(_str[i] != ' ' && _str[i] != '\t') 
        {
            str.push_back(_str[i]);
        }
    }

    if(typeStr == L"rect" || typeStr == L"RECT" || typeStr == L"Rect")
    {
        stringToRect(str.c_str() , region.Rect2D() );
        region._type = xuiRegion::eRT_Rect;
    }
    else if(typeStr == L"DELTA" || typeStr == L"delta" || typeStr == L"Delta")
    {
        float x , y , w , h;
        swscanf(str.c_str(),L"[%f,%f,%f,%f]",&x,&y,&w,&h);
        region.Rect2D().x += x;
        region.Rect2D().y += y;
        region.Rect2D().w += (w - x) ;
        region.Rect2D().h += (h - y) ;
    }
    return true;
}

bool stringToRect(const wchar_t* _str , xuiRect2D& rect)
{
    swscanf(_str,L"[%f,%f,%f,%f]",&rect.x,&rect.y,&rect.w,&rect.h);
    return true;
}

bool string2Bevel(const wchar_t* _str , xuiBevel& bevel)
{
    static std::wstring str;
    str=L"";
    for(int i = 0 ; i < (int)wcslen(_str) ; i ++) 
    {
        if(_str[i] != ' ' && _str[i] != '\t') 
        {
            str.push_back(_str[i]);
        }
    }
    swscanf(str.c_str(),L"%f:%f:%f:%f",&bevel.tl,&bevel.tr,&bevel.br,&bevel.bl);
    return true;
}


bool string2Color(const wchar_t* _str , xColor_4f& color)
{
    static std::wstring str;
    str=L"";
    for(int i = 0 ; i < (int)wcslen(_str) ; i ++) 
    {
        if(_str[i] != ' ' && _str[i] != '\t') 
        {
            str.push_back(_str[i]);
        }
    }
    assert(str.length() > 0);
    if(str[0] == '[')
    {
        swscanf(str.c_str(),L"[%f,%f,%f,%f]",&color.r,&color.g,&color.b,&color.a);
    }
    else
    {
        xColor_4ub _cl;
        swscanf(str.c_str() ,L"%x", &_cl.color);
        color = _cl;		
    }
    return true;
}

/*****************************************************
感谢天下有雪，石勇慧提供这个函数
******************************************************/
long strToInteger(wchar_t *szString, char cRadix)
{
    assert(szString);
    assert(cRadix>=2 && cRadix<=36);

    long    lResult = 0;
    wchar_t cDigital = 0;
    bool    bMinus = false;

    while (*szString==' ' || *szString=='\t')
        ++szString;

    if (*szString == '+')
    {
        bMinus = false;
        ++szString;
    }
    else if (*szString == '-')
    {
        bMinus = true;
        ++szString;
    }

    for (; *szString; ++szString)
    {
        if  (*szString>='0' && *szString<='9')
        {
            cDigital = *szString - '0';
        }
        else if  (*szString>='A' && *szString<='Z')
        {
            cDigital = *szString - 'A' + 10;
        }
        else if  (*szString>='a' && *szString<='z')
        {
            cDigital = *szString - 'a' + 10;
        }
        else 
        {
            break;
        }
        lResult = lResult * cRadix + cDigital;
    }

    if (bMinus)
    {
        lResult = -lResult;
    }

    return lResult;
} 

END_NAMESPACE_XUI

