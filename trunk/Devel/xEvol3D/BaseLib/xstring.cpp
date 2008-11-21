#include <wchar.h>
#include <ctype.h>
#include <memory.h>
#include <stdlib.h>
#include <stdarg.h>
#include "xstring.h"

int  swprintf_x(wchar_t *buffer,size_t count, const wchar_t *fmt , ... )
{
	va_list args;
	va_start(args, fmt);
	int ret = 0;
#ifdef _WIN32
	ret = _vsnwprintf(buffer, count, fmt, args);
#else
	ret = vswprintf(buffer, count, fmt, args);
#endif
	va_end(args);
	return ret;
}

wchar_t * wcpncpy_x(wchar_t *dest, const wchar_t *src , size_t n)
{
	wint_t c;
	wchar_t *const s = dest;

	if (n >= 4)
	{
		size_t n4 = n >> 2;

		for (;;)
		{
			c = *src++;
			*dest++ = c;
			if (c == L'\0')
				break;
			c = *src++;
			*dest++ = c;
			if (c == L'\0')
				break;
			c = *src++;
			*dest++ = c;
			if (c == L'\0')
				break;
			c = *src++;
			*dest++ = c;
			if (c == L'\0')
				break;
			if (--n4 == 0)
				goto last_chars;
		}
		n -= dest - s;
		goto zero_fill;
	}

last_chars:
	n &= 3;
	if (n == 0)
		return dest;

	for (;;)
	{
		c = *src++;
		--n;
		*dest++ = c;
		if (c == L'\0')
			break;
		if (n == 0)
			return dest;
	}

zero_fill:
	while (n-- > 0)
		dest[n] = L'\0';

	return dest - 1;
}

wchar_t * wcpcpy_x (wchar_t *dest, const wchar_t *src)
{
	wchar_t *wcp = (wchar_t *) dest - 1;
	wint_t c;
	const int off = (int)(src - dest + 1);
	do
	{
		c = wcp[off];
		*++wcp = c;
	}
	while (c != L'\0');
	return wcp;
}


int wcscasecmp_x (const wchar_t *s1, const wchar_t *s2)// LOCALE_PARAM)
//LOCALE_PARAM_DECL
{
	wint_t c1, c2;

	if (s1 == s2)
		return 0;

	do
	{
		c1 = tolower(*s1++);
		c2 = tolower(*s2++);
		if (c1 == L'\0')
			break;
	}
	while (c1 == c2);
	return c1 - c2;
}

wchar_t * wcschrnul_x (const wchar_t *wcs, const wchar_t wc)
{
	while (*wcs != L'\0')
		if (*wcs == wc)
			break;
		else
			++wcs;
	return (wchar_t *) wcs;
}

int wcscmp_x(const wchar_t *s1, const wchar_t *s2)
{
	wint_t c1, c2;

	do
	{
		c1 = (wint_t) *s1++;
		c2 = (wint_t) *s2++;
		if (c1 == L'\0')
			return c1 - c2;
	}
	while (c1 == c2);

	return c1 - c2;
}



wchar_t *  wcschr_x (const wchar_t *wcs, const wchar_t wc)
{
	do
	if (*wcs == wc)
		return (wchar_t *) wcs;
	while (*wcs++ != L'\0');

	return NULL;
}

size_t  wcslen_x(const wchar_t *s)
{
	size_t len = 0;
	while (s[len] != L'\0')
	{
		if (s[++len] == L'\0')
			return len;
		if (s[++len] == L'\0')
			return len;
		if (s[++len] == L'\0')
			return len;
		++len;
	}
	return len;
}

wchar_t * wcscat_x (wchar_t *dest, const wchar_t *src)
{
	register wchar_t *s1 = dest;
	register const wchar_t *s2 = src;
	wchar_t c;

	/* Find the end of the string.  */
	do
	{
		c = *s1++;
	}
	while (c != L'\0');

	/* Make S1 point before the next character, so we can increment
	it while memory is read (wins on pipelined cpus).	*/
	s1 -= 2;

	do
	{
		c = *s2++;
		*++s1 = c;
	}
	while (c != L'\0');

	return dest;
}

wchar_t * wcsdup_x (const wchar_t *s)
{
	size_t len = (wcslen_x (s) + 1) * sizeof (wchar_t);
	void * newStr = malloc (len);

	if (newStr == NULL)
		return NULL;
	return (wchar_t *) memcpy (newStr, (void *) s, len);
}

int wcsncasecmp_x (const wchar_t *s1, const wchar_t *s2, size_t n )//LOCALE_PARAM)
//LOCALE_PARAM_DECL
{
	wint_t c1, c2;

	if (s1 == s2 || n == 0)
		return 0;

	do
	{
		c1 = (wint_t) tolower (*s1++);
		c2 = (wint_t) tolower (*s2++);
		if (c1 == L'\0' || c1 != c2)
			return c1 - c2;
	} while (--n > 0);

	return c1 - c2;
}

wchar_t * wcsncat_x (wchar_t *dest, const wchar_t *src, size_t n)
{
	wchar_t c;
	wchar_t * const s = dest;

	/* Find the end of DEST.  */
	do
	c = *dest++;
	while (c != L'\0');

	/* Make DEST point before next character, so we can increment
	it while memory is read (wins on pipelined cpus).	*/
	dest -= 2;

	if (n >= 4)
	{
		size_t n4 = n >> 2;
		do
		{
			c = *src++;
			*++dest = c;
			if (c == L'\0')
				return s;
			c = *src++;
			*++dest = c;
			if (c == L'\0')
				return s;
			c = *src++;
			*++dest = c;
			if (c == L'\0')
				return s;
			c = *src++;
			*++dest = c;
			if (c == L'\0')
				return s;
		} while (--n4 > 0);
		n &= 3;
	}

	while (n > 0)
	{
		c = *src++;
		*++dest = c;
		if (c == L'\0')
			return s;
		n--;
	}

	if (c != L'\0')
		*++dest = L'\0';

	return s;
}


wchar_t * wcsncpy_x (wchar_t *dest, const wchar_t *src, size_t n)
{
	wint_t c;
	wchar_t *const s = dest;

	--dest;

	if (n >= 4)
	{
		size_t n4 = n >> 2;

		for (;;)
		{
			c = *src++;
			*++dest = c;
			if (c == L'\0')
				break;
			c = *src++;
			*++dest = c;
			if (c == L'\0')
				break;
			c = *src++;
			*++dest = c;
			if (c == L'\0')
				break;
			c = *src++;
			*++dest = c;
			if (c == L'\0')
				break;
			if (--n4 == 0)
				goto last_chars;
		}
		n = n - (dest - s) - 1;
		if (n == 0)
			return s;
		goto zero_fill;
	}

last_chars:
	n &= 3;
	if (n == 0)
		return s;

	do
	{
		c = *src++;
		*++dest = c;
		if (--n == 0)
			return s;
	}
	while (c != L'\0');

zero_fill:
	do
	*++dest = L'\0';
	while (--n > 0);

	return s;
}

size_t wcsnlen_x (const wchar_t *s, size_t maxlen)
{
	size_t len = 0;

	while (maxlen > 0 && s[len] != L'\0')
	{
		++len;
		if (--maxlen == 0 || s[len] == L'\0')
			return len;
		++len;
		if (--maxlen == 0 || s[len] == L'\0')
			return len;
		++len;
		if (--maxlen == 0 || s[len] == L'\0')
			return len;
		++len;
		--maxlen;
	}

	return len;
}

wchar_t * wcsstr_x (const wchar_t *haystack, const wchar_t *needle)
{
	wchar_t b, c;

	if ((b = *needle) != L'\0')
	{
		haystack--;				/* possible ANSI violation */
		do
		if ((c = *++haystack) == L'\0')
			goto ret0;
		while (c != b);

		if (!(c = *++needle))
			goto foundneedle;
		++needle;
		goto jin;

		for (;;)
		{
			wchar_t a;
			const wchar_t *rhaystack, *rneedle;

			do
			{
				if (!(a = *++haystack))
					goto ret0;
				if (a == b)
					break;
				if ((a = *++haystack) == L'\0')
					goto ret0;
shloop:	      ;
			}
			while (a != b);

jin:	  if (!(a = *++haystack))
			  goto ret0;

		  if (a != c)
			  goto shloop;

		  if (*(rhaystack = haystack-- + 1) == (a = *(rneedle = needle)))
			  do
			  {
				  if (a == L'\0')
					  goto foundneedle;
				  if (*++rhaystack != (a = *++needle))
					  break;
				  if (a == L'\0')
					  goto foundneedle;
			  }
			  while (*++rhaystack == (a = *++needle));

			  needle = rneedle;		  /* took the register-poor approach */

			  if (a == L'\0')
				  break;
		}
	}
foundneedle:
	return (wchar_t*) haystack;
ret0:
	return NULL;
}

size_t wcscspn_x(const wchar_t *wcs, const wchar_t *reject)
{
	register size_t count = 0;

	while (*wcs != L'\0')
		if (wcschr_x (reject, *wcs++) == NULL)
			++count;
		else
			return count;

	return count;
}

