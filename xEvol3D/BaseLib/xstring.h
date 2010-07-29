#ifndef __XEVOL_STRING_H__
#define __XEVOL_STRING_H__
#include <wchar.h>
#include "xEvol3DAPI.h"
//#define _USE_ICONV_
//#ifdef _UNIX
//#define wcscspn     wcscspn_x   
//#define wcpncpy		wcpncpy_x
//#define wcpcpy		wcpcpy_x
//#define wcscasecmp	wcscasecmp_x
//#define wcschrnul	wcschrnul_x
//#define wcscmp		wcscmp_x
//#define wcschr		wcschr_x
//#define wcslen		wcslen_x
//#define wcscat		wcscat_x
//#define wcsdup		wcsdup_x
//#define wcsncasecmp	wcsncasecmp_x
//#define wcsncat		wcsncat_x
//#define wcsncpy		wcsncpy_x
//#define wcsnlen		wcsnlen_x
//#define wcsstr		wcsstr_x
//#endif

//////////////////////////////////////////////////////////////////////////
_XEVOL_BASE_API_  size_t     wcscspn_x    (const wchar_t *wcs, const wchar_t *reject);
_XEVOL_BASE_API_  wchar_t *  wcpncpy_x    (wchar_t *dest, const wchar_t *src , size_t n);
_XEVOL_BASE_API_  wchar_t *  wcpcpy_x     (wchar_t *dest, const wchar_t *src);
_XEVOL_BASE_API_  int        wcscasecmp_x (const wchar_t *s1, const wchar_t *s2);
_XEVOL_BASE_API_  wchar_t *  wcschrnul_x  (const wchar_t *wcs, const wchar_t wc);
_XEVOL_BASE_API_  int        wcscmp_x     (const wchar_t *s1, const wchar_t *s2);
_XEVOL_BASE_API_  wchar_t *  wcschr_x     (const wchar_t *wcs, const wchar_t wc);
_XEVOL_BASE_API_  size_t     wcslen_x     (const wchar_t *s);
_XEVOL_BASE_API_  wchar_t *  wcscat_x     (wchar_t *dest, const wchar_t *src);
_XEVOL_BASE_API_  wchar_t *  wcsdup_x     (const wchar_t *s);
_XEVOL_BASE_API_  int        wcsncasecmp_x(const wchar_t *s1, const wchar_t *s2, size_t n );
_XEVOL_BASE_API_  wchar_t *  wcsncat_x    (wchar_t *dest, const wchar_t *src, size_t n);
_XEVOL_BASE_API_  wchar_t *  wcsncpy_x    (wchar_t *dest, const wchar_t *src, size_t n);
_XEVOL_BASE_API_  size_t     wcsnlen_x    (const wchar_t *s, size_t maxlen);
_XEVOL_BASE_API_  wchar_t *  wcsstr_x     (const wchar_t *haystack, const wchar_t *needle);
_XEVOL_BASE_API_  int        swprintf_x   (wchar_t *buffer,size_t count, const wchar_t *fmt , ... );
_XEVOL_BASE_API_  void       tolower_x    (wchar_t * buffer);
_XEVOL_BASE_API_  void       toupper_x    (wchar_t * buffer);
//////////////////////////////////////////////////////////////////////////


BEGIN_NAMESPACE_XEVOL3D

//iconv's wrapper
#ifdef _USE_ICONV_
#define CHARSET_UNICODE16   "UCS-2-INTERNAL"
#define CHARSET_UNICODE32   "UCS-4-INTERNAL"
#define CHARSET_UTF8        "UTF-8"
#define CHARSET_UTF16       "UTF-16"
#define CHARSET_UTF32       "UTF-32"
class _XEVOL_BASE_API_  xStringConverter
{
    void*       m_handle;
public:
    xStringConverter();
    ~xStringConverter();
    bool           setLanguge(const char* _to , const char* _from);

    bool           setUTF82Locale();
    bool           setLocale2UTF8();

    bool           setUTF82Unicode();
    bool           setUnicode2UTF8();

    bool           setUnicode2Locale();
    bool           setLocale2Unicode();

    bool           setUCS2ToUCS4();
    bool           setUCS4ToUCS2();


    bool           convert(const char* _in, char* _out,int inLen,int outlen);
    bool           convert(const char* _in, char* _out,int outlen);
    bool           convert(const wchar_t* _in, char* _out,int outlen);
};
#endif


template<int _WCharWidth> class TWCharTrait
{
public:
    typedef wchar_t        CharType ;
    typedef wchar_t        UTF32;
    typedef unsigned short UTF16;
    typedef unsigned char  UTF8;
    enum { WCharWidth = _WCharWidth , }; 

public:
    wchar_t  GetCodePoint(const wchar_t* _text , size_t& index , wchar_t* _out);
    wchar_t  GetCodePoint(const wchar_t* _text , size_t& index );
    size_t   strlen(const wchar_t* _text);
};




template<> class TWCharTrait<2>
{
public:
    typedef unsigned int   CharType ;
    typedef wchar_t        UTF16;
    typedef uint32         UTF32;
    typedef unsigned char  UTF8;

    enum { WCharWidth = 2 , } ; 
public:
    UTF32  GetCodePoint(const wchar_t* _text , size_t& index , wchar_t* _out);
    UTF32  GetCodePoint(const wchar_t* _text , size_t& index );
    size_t strlen(const wchar_t* _text);
};




typedef TWCharTrait<sizeof(wchar_t)>           xWCharTrait;
typedef xWCharTrait::CharType                  xWCharType; 

typedef xWCharTrait::UTF32  UTF32;	/* at least 32 bits */
typedef xWCharTrait::UTF16  UTF16;	/* at least 16 bits */
typedef xWCharTrait::UTF8   UTF8;	/* typically 8 bits */

/* Some fundamental constants */
#define UNI_REPLACEMENT_CHAR (UTF32)0x0000FFFD
#define UNI_MAX_BMP (UTF32)0x0000FFFF
#define UNI_MAX_UTF16 (UTF32)0x0010FFFF
#define UNI_MAX_UTF32 (UTF32)0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32 (UTF32)0x0010FFFF

typedef enum 
{
    conversionOK, 		/* conversion successful */
    sourceExhausted,	/* partial character in source, but hit end */
    targetExhausted,	/* insuff. room in target for conversion */
    sourceIllegal,		/* source sequence is illegal/malformed */
    conversionFailed,
} ConversionResult;

typedef enum 
{
    strictConversion = 0,
    lenientConversion
} ConversionFlags;


_XEVOL_BASE_API_ ConversionResult XEvol_Utf8ToUtf16 (const UTF8*  sourceStart,  UTF16* targetStart , size_t outlen , ConversionFlags flags = lenientConversion);
_XEVOL_BASE_API_ ConversionResult XEvol_Utf16ToUtf8 (const UTF16* sourceStart,  UTF8* targetStart  , size_t outlen , ConversionFlags flags = lenientConversion);
_XEVOL_BASE_API_ ConversionResult XEvol_Utf8ToUtf32 (const UTF8*  sourceStart,  UTF32* targetStart , size_t outlen , ConversionFlags flags = lenientConversion);
_XEVOL_BASE_API_ ConversionResult XEvol_Utf32ToUtf8 (const UTF32* sourceStart,  UTF8* targetStart  , size_t outlen , ConversionFlags flags = lenientConversion);
_XEVOL_BASE_API_ ConversionResult XEvol_Utf16ToUtf32(const UTF16* sourceStart,  UTF32* targetStart , size_t outlen , ConversionFlags flags = lenientConversion);
_XEVOL_BASE_API_ ConversionResult XEvol_Utf32ToUtf16(const UTF32* sourceStart,  UTF16* targetStart , size_t outlen , ConversionFlags flags = lenientConversion);
_XEVOL_BASE_API_ ConversionResult XEvol_UnicodeToFsEnc(const wchar_t* _in, char* _out,size_t outlen);
_XEVOL_BASE_API_ ConversionResult XEvol_FsEncToUnicode(const char* _in, wchar_t* _out,size_t outlen);
_XEVOL_BASE_API_ ConversionResult XEvol_Utf8ToUnicode(const char* _in, wchar_t* _out,size_t outlen);
_XEVOL_BASE_API_ ConversionResult XEvol_UnicodeToUtf8(const wchar_t* _in, char* _out,size_t outlen);
_XEVOL_BASE_API_ ConversionResult XEvol_LocaleToUtf8(const char* _in, char* _out,size_t outlen);
_XEVOL_BASE_API_ ConversionResult XEvol_Utf8ToLocale(const char* _in, char* _out,size_t outlen);
_XEVOL_BASE_API_ ConversionResult XEvol_UnicodeToLocale(const wchar_t* _in, char* _out,size_t outlen);
_XEVOL_BASE_API_ ConversionResult XEvol_LocaleToUnicode(const char* _in, wchar_t* _out,size_t outlen);
_XEVOL_BASE_API_ UTF32            XEvol_GetUtf32CodePoint(const UTF16 * text , size_t &c , UTF16 * strOut);
_XEVOL_BASE_API_ bool             XEvol_IsUtf8(const UTF8 * source, const UTF8 *sourceEnd);
_XEVOL_BASE_API_ size_t           XEvol_UTF16Len(const UTF16 * text);




template <int _WCharWidth> wchar_t  TWCharTrait<_WCharWidth>::GetCodePoint(const wchar_t* _text , size_t& index , wchar_t* _out)
{
    wchar_t _ret = _text[index];
    _out[0] = _ret;
    _out[1] = 0;
    index ++;
    return  _ret;
}

template <int _WCharWidth> wchar_t  TWCharTrait<_WCharWidth>::GetCodePoint(const wchar_t* _text , size_t& index )
{
    wchar_t _ret = _text[index];
    index ++;
    return  _ret;
}

template <int _WCharWidth> size_t TWCharTrait<_WCharWidth>::strlen(const wchar_t* _text)
{
    return wcslen_x(_text);
}


inline TWCharTrait<2>::UTF32  TWCharTrait<2>::GetCodePoint(const wchar_t* _text , size_t& index , wchar_t* _out)
{
    return XEvol_GetUtf32CodePoint((const xWCharTrait::UTF16 *)_text , index , (xWCharTrait::UTF16*)_out );
}

inline TWCharTrait<2>::UTF32  TWCharTrait<2>::GetCodePoint(const wchar_t* _text , size_t& index )
{
    return XEvol_GetUtf32CodePoint((const xWCharTrait::UTF16*)_text , index , NULL );
}

inline size_t TWCharTrait<2>::strlen(const wchar_t* _text)
{
    return wcslen_x(_text);
}


END_NAMESPACE_XEVOL3D



#endif

