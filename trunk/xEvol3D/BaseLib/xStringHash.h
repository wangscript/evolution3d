#ifndef __XSTRING_HASH_H__
#define __XSTRING_HASH_H__
namespace XEvol3D 
{
	template<typename _CharT> unsigned  int  RSHash( const _CharT   * str)
	{
		unsigned  int  b  =   378551 ;
		unsigned  int  a  =   63689 ;
		unsigned  int  hash  =   0 ;

		while  ( * str)
		{
			hash  =  hash  *  a  +  ( * str ++ );
			a  *=  b;
		} 

		return  (hash  &   0x7FFFFFFF );
	} 

	//  JS Hash Function 
	template<typename _CharT> unsigned  int  JSHash( const _CharT   * str)
	{
		unsigned  int  hash  =   1315423911 ;

		while  ( * str)
		{
			hash  ^=  ((hash  <<   5 )  +  ( * str ++ )  +  (hash  >>   2 ));
		} 

		return  (hash  &   0x7FFFFFFF );
	} 

	//  P. J. Weinberger Hash Function 
	template<typename _CharT>  unsigned  int  PJWHash( const _CharT   * str)
	{
		unsigned  int  BitsInUnignedInt  =  (unsigned  int )( sizeof (unsigned  int )  * 8 );
		unsigned  int  ThreeQuarters     =  (unsigned  int )((BitsInUnignedInt   *   3 ) /  4 );
		unsigned  int  OneEighth         =  (unsigned  int )(BitsInUnignedInt  /   8 );

		unsigned  int  HighBits          =  (unsigned  int )( 0xFFFFFFFF )  <<  (BitsInUnignedInt  -  OneEighth);
		unsigned  int  hash              =   0 ;
		unsigned  int  test              =   0 ;

		while  ( * str)
		{
			hash  =  (hash  <<  OneEighth)  +  ( * str ++ );
			if  ((test  =  hash  &  HighBits)  !=   0 )
			{
				hash  =  ((hash  ^  (test  >>  ThreeQuarters))  &  ( ~ HighBits));
			} 
		} 

		return  (hash  &   0x7FFFFFFF );
	} 

	//  ELF Hash Function 
	template<typename _CharT>  unsigned  int  ELFHash( const _CharT   * str)
	{
		unsigned  int  hash  =   0 ;
		unsigned  int  x     =   0 ;

		while  ( * str)
		{
			hash  =  (hash  <<   4 )  +  ( * str ++ );
			if  ((x  =  hash  &   0xF0000000L )  !=   0 )
			{
				hash  ^=  (x  >>   24 );
				hash  &=   ~ x;
			} 
		} 

		return  (hash  &   0x7FFFFFFF );
	} 

	//  BKDR Hash Function 
	template<typename _CharT>  unsigned  int  BKDRHash( const _CharT   * str)
	{
		unsigned  int  seed  =   131 ;  //  31 131 1313 13131 131313 etc.. 
		unsigned  int  hash  =   0 ;

		while  ( * str)
		{
			hash  =  hash  *  seed  +  ( * str ++ );
		} 

		return  (hash  &   0x7FFFFFFF );
	} 

	//  SDBM Hash Function 
	template<typename _CharT> unsigned  int  SDBMHash( const _CharT   * str)
	{
		unsigned  int  hash  =   0 ;

		while  ( * str)
		{
			hash  =  ( * str ++ )  +  (hash  <<   6 )  +  (hash  <<   16 )  -  hash;
		} 

		return  (hash  &   0x7FFFFFFF );
	} 

	//  DJB Hash Function 
	template<typename _CharT> unsigned  int  DJBHash( const _CharT   * str)
	{
		unsigned  int  hash  =   5381 ;

		while  ( * str)
		{
			hash  +=  (hash  <<   5 )  +  ( * str ++ );
		} 

		return  (hash  &   0x7FFFFFFF );
	} 

	//  AP Hash Function 
	template<typename _CharT> unsigned  int  APHash( char   * str)
	{
		unsigned  int  hash  =   0 ;
		int  i;

		for  (i = 0 ;  * str; i ++ )
		{
			if  ((i  &   1 )  ==   0 )
			{
				hash  ^=  ((hash  <<   7 )  ^  ( * str ++ )  ^  (hash  >>   3 ));
			} 
			else 
			{
				hash  ^=  ( ~ ((hash  <<   11 )  ^  ( * str ++ )  ^  (hash  >>   5 )));
			} 
		} 

		return  (hash  &   0x7FFFFFFF );
	} 

	//====================Memory Hash==========================================
	//  ELF Hash Function 
	inline unsigned  int  ELF_MemHash( const char  * str, int len)
	{
		unsigned  int  hash  =   0 ;
		unsigned  int  x     =   0 ;

		while  ( len > 0)
		{
			len -- ;
			hash  =  (hash  <<   4 )  +  ( * str ++ );
			if  ((x  =  hash  &   0xF0000000L )  !=   0 )
			{
				hash  ^=  (x  >>   24 );
				hash  &=   ~ x;
			} 
		} 

		return  (hash  &   0x7FFFFFFF );
	} 

	//  P. J. Weinberger Hash Function 
	inline unsigned  int  PJW_MemHash( const char  * str , int len)
	{
		unsigned  int  BitsInUnignedInt  =  (unsigned  int )( sizeof (unsigned  int )  * 8 );
		unsigned  int  ThreeQuarters     =  (unsigned  int )((BitsInUnignedInt   *   3 ) /  4 );
		unsigned  int  OneEighth         =  (unsigned  int )(BitsInUnignedInt  /   8 );

		unsigned  int  HighBits          =  (unsigned  int )( 0xFFFFFFFF )  <<  (BitsInUnignedInt  -  OneEighth);
		unsigned  int  hash              =   0 ;
		unsigned  int  test              =   0 ;

		while  ( len > 0)
		{
			len --;
			hash  =  (hash  <<  OneEighth)  +  ( * str ++ );
			if  ((test  =  hash  &  HighBits)  !=   0 )
			{
				hash  =  ((hash  ^  (test  >>  ThreeQuarters))  &  ( ~ HighBits));
			} 
		} 

		return  (hash  &   0x7FFFFFFF );
	} 


	//===========================================================================
	inline unsigned int xStringHash(const char* str)
	{
		return ELFHash(str);
	}

	inline unsigned int xStringHash(const wchar_t* str)
	{
		return ELFHash(str);
	}


	inline unsigned int xMemHash(const char* str, int len)
	{
		return ELF_MemHash(str,len);
	}

	template <typename T>  unsigned int  xMemHash(const T& _value)
	{
		return xMemHash( (const char*)&_value , sizeof(T) );
	}

	template <typename T>  unsigned int  xMemHash(const T* _value , int nElement)
	{
		return xMemHash( (const char*)_value , sizeof(T) * nElement);
	}


	//================第二套哈希方案===========================================================
	inline unsigned int xStringHash2(const char* str)
	{
		return PJWHash(str);
	}

	inline unsigned int xStringHash2(const wchar_t* str)
	{
		return PJWHash(str);
	}

	inline unsigned int xMemHash2(const char* str, int len)
	{
		return PJW_MemHash(str,len);
	}

	template <typename T>  unsigned int  xMemHash2(const T& _value)
	{
		return xMemHash2( (const char*)&_value , sizeof(T) );
	}

	template <typename T>  unsigned int  xMemHash2(const T* _value , int nElement)
	{
		return xMemHash2( (const char*)_value , sizeof(T) * nElement);
	}

}
#endif
