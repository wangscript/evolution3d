#ifndef _XEVOL_LEXER_H_
#define _XEVOL_LEXER_H_
#include "xEvol3DBaseInc.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>
BEGIN_NAMESPACE_XEVOL3D
enum eTextEncFmt
{
    TXTENC_ASCII,
    TXTENC_UTF8,
    TXTENC_UNICODE
};

template <typename _CharT , typename _StrT> class xBaseStrFunc
{
public:
	static size_t   len(const _CharT* ) { return 0 ; }
	static _CharT   copy(_CharT* _dest ,const _CharT* _src){return NULL;}
	static _CharT   ncopy(_CharT* _dest,const _CharT* _src , size_t n){return NULL;}
	static void     appendReturn(_StrT& _str) {};    
	static void     appendTab(_StrT& _str)    {};  
	static void     appendNewLine(_StrT& _str){};  
};

template <typename _StrT> class xBaseStrFunc<wchar_t , typename _StrT>
{
public:
	static size_t     len(const wchar_t* _str)                            { return wcslen(_str) ;               }
	static wchar_t*   copy(wchar_t* _dest,const wchar_t* _src)            { return wcscpy(_dest , _src ) ;      }
	static wchar_t*   ncopy(wchar_t* _dest,const wchar_t* _src , size_t n){ return wcsncpy(_dest , _src , n) ;  }
	static void       appendReturn(_StrT& _str) {_str.append(L"\r"); };    
	static void       appendTab(_StrT& _str)    {_str.append(L"\t"); };  
	static void       appendNewLine(_StrT& _str){_str.append(L"\n"); };  
};

template <typename _StrT> class xBaseStrFunc<char, typename _StrT>
{
public:
	static size_t  len(const char* _str)                         { return strlen(_str) ;             }
	static char*   copy(char* _dest,const char* _src)            { return strcpy(_dest , _src ) ;    }
	static char*   ncopy(char* _dest,const char* _src , size_t n){ return strncpy(_dest , _src , n) ;}
	static void    appendReturn(_StrT& _str) { _str.append("\r"); }    
	static void    appendTab(_StrT& _str)    { _str.append("\t"); };  
	static void    appendNewLine(_StrT& _str){ _str.append("\n"); };  
};

typedef bool (*FUNC_IS_ACCEPT)(unsigned int ch);

_XEVOL_BASE_API_  bool  XEvol_IsReadalbeChar(unsigned int ch);


inline bool XEvol_IsAlphaNumChar(unsigned int ch) 
{
	if ( ch < 128 )
		return isalnum( ch ) != 0;
	else
		return true;	
}

inline bool XEvol_IsAlphaChar(unsigned int anyByte) 
{
	if ( anyByte < 128 )
		return isalpha( anyByte ) != 0;
	else
		return true;	
}

template <typename _CharT , typename _StrT> class  xBaseLexer
{
public:
	int skipLine(const _CharT* p)
	{
		const _CharT* pBase = p;
		while(*p && *p != 0x0a &&  *p != 0x0d ) 
		{
			p++;
		}
		p ++ ;
		return int(p - pBase);
	}

	int IsAlpha( _CharT anyByte)
	{
		if ( anyByte < 128 )
			return isalpha( anyByte );
		else
			return 1;	
	}

	int IsAlphaNum(_CharT anyByte )
	{
		if ( anyByte < 128 )
			return isalnum( anyByte );
		else
			return 1;	
	}


	bool IsWhiteSpace( _CharT c )		
	{ 
		if ( c < 128 )
			return ( c == '\t' || c == ' '  || c == 0x0A || c == 0x0D ); 
		return false;
	}

	bool IsWhiteSpace( int c )
	{
		if ( c < 128 )
			return IsWhiteSpace( (char) c );
		return false;	// Again, only truly correct for English/Latin...but usually works.
	}

	int ToLower( int v)
	{
		if ( v < 128 ) 
			return tolower( v );
		return v;
	}

	bool stringEqual(const _CharT* p,const _CharT* tag,bool ignoreCase)
	{
		assert( p );
		assert( tag );
		if ( !p || !*p )
		{
			return false;
		}

		const _CharT* q = p;

		if ( ignoreCase )
		{
			while ( *q && *tag && ToLower( *q) == ToLower( *tag) )
			{
				++q;
				++tag;
			}

			if ( *tag == 0 )
				return true;
		}
		else
		{
			while ( *q && *tag && *q == *tag )
			{
				++q;
				++tag;
			}

			if ( *tag == 0 )		// Have we found the end of the tag, and everything equal?
				return true;
		}
		return false;
	}

	int  skipWhiteSpaceOneLine(const _CharT* p )
	{
		const _CharT* pBase = p;
		if ( !p || !*p )
		{
			return 0;
		}

		//Skip all white space;

		while ( *p && ( *p == '\t' || *p == ' ' ) )
			++p;

		return int(p - pBase);
	}

	int  skipWhiteSpace(const _CharT* p )
	{
		const _CharT* pBase = p;
		if ( !p || !*p )
		{
			return 0;
		}

		//Skip all white space;

		while ( *p && (IsWhiteSpace( *p ) || *p == '\n' || *p =='\r') )
			++p;

		return int(p - pBase);
	}

	int  GetCStyleChar(const _CharT* p, _StrT& text )
	{
		int i = 0;
		if(*p == '\\')
		{
			i ++ ;
			p ++;
			if( *p == 'n')
			{
				xBaseStrFunc<_CharT,_StrT>::appendNewLine(text);
				// text.append( L"\n");
			}
			else if( *p == 't')
			{
				xBaseStrFunc<_CharT,_StrT>::appendTab(text);
				// text.append( L"\t");
			}
			else if( *p == 'r')
			{
				xBaseStrFunc<_CharT,_StrT>::appendReturn(text);
				// text.append( L"\r");
			}
			else
			{
				text.append(p,1);
			}
			i ++;
			return 2;
		}
		else
		{

			text.push_back( *p );
			return 1;
		}
	}

	int getString(const _CharT* p, _StrT& text, const _CharT* endTag , FUNC_IS_ACCEPT func)
	{
		const _CharT* pBase = p;
		text.clear();
		if ( xBaseStrFunc<_CharT,_StrT>::len(endTag) != 0	 )	
		{
			// Keep all the white space.
			while (	   p && *p  )
			{
				//to end;
				if( stringEqual( p, endTag , true ) )
					break;
				p += GetCStyleChar(p , text);

			}
			p += xBaseStrFunc<_CharT,_StrT>::len( endTag );
			return (int)(p - pBase);
		}
		else
		{
			return getToken(p,text , func);
		}
	}

	int   getToken(	const _CharT* p, _StrT& text , FUNC_IS_ACCEPT func)
	{
		const _CharT* pBase = p;
		text.clear();
		assert( p );
		//if (    p && *p && ( IsAlpha( (unsigned char) *p, encoding ) || *p == '_' ) )
		{
			while( p && *p &&	(  func(*p) ) )
			{
				text += *p;
				++p;
			}
		}
		return (int)(p - pBase);
	}

	int   getString(	const _CharT* p, _StrT& text , FUNC_IS_ACCEPT func)
	{
		const _CharT* pBase = p;
		text.clear();
		assert( p );
		//if (    p && *p && ( IsAlpha( (unsigned char) *p, encoding ) || *p == '_' ) )
		{
			while(		p && *p &&	(  func(*p)  ) )
			{
				p += GetCStyleChar(p , text);
			}
		}
		return (int)(p - pBase);
	}
};

END_NAMESPACE_XEVOL3D

#endif
