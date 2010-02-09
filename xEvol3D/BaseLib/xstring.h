#ifndef __XEVOL_STRING_H__
#define __XEVOL_STRING_H__
#include <wchar.h>
#include "xEvol3DAPI.h"

//#ifdef _LINUX
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

#endif

