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

#ifndef __XEVOL3D_COLOR_H__
#define __XEVOL3D_COLOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma pack(push,1)

#define int8 char

#ifndef BEGIN_NAMESPACE_XEVOL3D
#define BEGIN_NAMESPACE_XEVOL3D namespace XEvol3D{
#define END_NAMESPACE_XEVOL3D  }
#endif

BEGIN_NAMESPACE_XEVOL3D

enum eDATA_TYPE
{
	DATATYPE_Float,
	DATATYPE_Short,
	DATATYPE_float16,
	DATATYPE_Int,
	DATATYPE_Byte,
	DATATYPE_UByte,
	DATATYPE_UShort,
};
#define MAKEPIXEL_FORMAT(bbp , index  )  ( (bbp<<8) +index)
enum ePIXEL_FORMAT
{
	//Classic Texture format
	PIXELFORMAT_R8G8B8             = MAKEPIXEL_FORMAT( 24, 0),
	PIXELFORMAT_R8G8B8A8           = MAKEPIXEL_FORMAT( 32, 1),
	PIXELFORMAT_R8G8B8X8           = MAKEPIXEL_FORMAT( 32, 2),
	PIXELFORMAT_B8G8R8             = MAKEPIXEL_FORMAT( 23, 3),
	PIXELFORMAT_B8G8R8A8           = MAKEPIXEL_FORMAT( 32, 4),
	PIXELFORMAT_B8G8R8X8           = MAKEPIXEL_FORMAT( 32, 5),

	//16bit interger texture format						   	
	PIXELFORMAT_R16G16B16          = MAKEPIXEL_FORMAT( 48, 6),
	PIXELFORMAT_R16G16B16A16       = MAKEPIXEL_FORMAT( 64, 7),

	PIXELFORMAT_B16G16R16          = MAKEPIXEL_FORMAT( 48, 8),
	PIXELFORMAT_B16G16R16A16       = MAKEPIXEL_FORMAT( 64, 9),

	//Signed
	PIXELFORMAT_R16G16B16S         = MAKEPIXEL_FORMAT( 48, 10),
	PIXELFORMAT_R16G16B16A16S      = MAKEPIXEL_FORMAT( 64, 11),

	PIXELFORMAT_B16G16R16S         = MAKEPIXEL_FORMAT( 48, 12),
	PIXELFORMAT_B16G16R16A16S      = MAKEPIXEL_FORMAT( 64, 13),

	//16bit float16 float texture format				   	
	PIXELFORMAT_R16G16B16F         = MAKEPIXEL_FORMAT( 48, 14),
	PIXELFORMAT_R16G16B16A16F      = MAKEPIXEL_FORMAT( 64, 15),

	PIXELFORMAT_B16G16R16F         = MAKEPIXEL_FORMAT( 48, 16),
	PIXELFORMAT_B16G16R16A16F      = MAKEPIXEL_FORMAT( 64, 17),

	//32bit float texture format						   	
	PIXELFORMAT_R32G32B32F         = MAKEPIXEL_FORMAT( 96, 18),
	PIXELFORMAT_R32G32B32A32F      = MAKEPIXEL_FORMAT(128, 19),

	PIXELFORMAT_B32G32R32F         = MAKEPIXEL_FORMAT( 96, 20),
	PIXELFORMAT_B32G32R32A32F      = MAKEPIXEL_FORMAT(128, 21),

	//None 3/4 channel									   	
	PIXELFORMAT_ALPHA8             = MAKEPIXEL_FORMAT(  8, 22),

	PIXELFORMAT_LUMINANCE8         = MAKEPIXEL_FORMAT(  8, 23),
	PIXELFORMAT_LUMINANCE8ALPHA8   = MAKEPIXEL_FORMAT( 16, 24),
	PIXELFORMAT_LUMINANCE16        = MAKEPIXEL_FORMAT( 16, 25),
	PIXELFORMAT_LUMINANCE16F       = MAKEPIXEL_FORMAT( 16, 26),
	PIXELFORMAT_LUMINANCE32F       = MAKEPIXEL_FORMAT( 32, 27),


	PIXELFORMAT_R16F               = MAKEPIXEL_FORMAT( 16, 28),
	PIXELFORMAT_R16G16F            = MAKEPIXEL_FORMAT( 32, 29),

	PIXELFORMAT_R32F               = MAKEPIXEL_FORMAT( 32, 30),
	PIXELFORMAT_R32G32F            = MAKEPIXEL_FORMAT( 64, 31),

	//Depth												   	
	PIXELFORMAT_DEPTH16            = MAKEPIXEL_FORMAT( 16, 32),
	PIXELFORMAT_DEPTH24            = MAKEPIXEL_FORMAT( 24, 33),
	PIXELFORMAT_DEPTH32            = MAKEPIXEL_FORMAT( 32, 34), 


	//不可以操作的
	PIXELFORMAT_DXT1               = MAKEPIXEL_FORMAT( 32,  35), 
	PIXELFORMAT_DXT2               = MAKEPIXEL_FORMAT( 32,  36), 
	PIXELFORMAT_DXT3               = MAKEPIXEL_FORMAT( 32,  37), 
	PIXELFORMAT_DXT4               = MAKEPIXEL_FORMAT( 32,  38), 
	PIXELFORMAT_DXT5               = MAKEPIXEL_FORMAT( 32,  39), 


	//视频纹理的常见的三种格式。
	//其他的YUV格式目前不支持
	PIXELFORMAT_YV12               = MAKEPIXEL_FORMAT(12 , 40),
	PIXELFORMAT_YVY2               = MAKEPIXEL_FORMAT(16 , 41),
	PIXELFORMAT_AYUV               = MAKEPIXEL_FORMAT(32 , 42),

	//================
	PIXELFORMAT_None,

};

class  xColor_4ub;
class  xColor_3ub;

template <typename T> T _CLAM_TO_(T v , T a , T b)
{
	if( v < a ) return a ;
	if( v > b ) return b;
	return v;
}

template <typename T> T TMin(const T& v1 , const T& v2)
{
	return v1 < v2?v1:v2;
}

template <typename T> T TMax(const T& v1 , const T& v2)
{
	return v1 > v2?v1:v2;
}

class  xColor_4f
{
public:
	float r,g,b,a;

	xColor_4f(){}
	xColor_4f(float _r, float _g,float _b,float _a) {
		r = _r; g = _g ; b = _b; a = _a;
	}
	xColor_4f operator = (const xColor_4ub& rhv);
	xColor_4f(const xColor_4ub& rhv);
	xColor_4f operator = (const xColor_3ub& rhv);
	xColor_4f(const xColor_3ub& rhv);
	static xColor_4f fromByte(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255){
		return xColor_4f(r/255.f,g/255.f,b/255.f,a/255.f);
	}

	xColor_4f  operator +(const xColor_4f& rhv)
	{
		xColor_4f outcl;
		outcl.r = TMin(1.0f,r + rhv.r);
		outcl.g = TMin(1.0f,g + rhv.g);
		outcl.b = TMin(1.0f,b + rhv.b);
		outcl.a = TMin(1.0f,a + rhv.a);
		return outcl;
	}

	xColor_4f  operator -(const xColor_4f& rhv)
	{
		xColor_4f outcl;
		outcl.r = TMax(0.0f,r - rhv.r);
		outcl.g = TMax(0.0f,g - rhv.g);
		outcl.b = TMax(0.0f,b - rhv.b);
		outcl.a = TMax(0.0f,a - rhv.a);
		return outcl;
	}

	xColor_4f& operator +=(const xColor_4f& rhv)
	{
		r = TMin(1.0f,r + rhv.r);
		g = TMin(1.0f,g + rhv.g);
		b = TMin(1.0f,b + rhv.b);
		a = TMin(1.0f,a + rhv.a);
		return *this;
	}

	xColor_4f& operator -=(const xColor_4f& rhv)
	{
		r = TMax(0.0f,r - rhv.r);
		g = TMax(0.0f,g - rhv.g);
		b = TMax(0.0f,b - rhv.b);
		a = TMax(0.0f,a - rhv.a);
		return *this;
	}
	xColor_4f& operator *=(float scale)
	{
		r = TMax(0.0f,scale * r);
		g = TMax(0.0f,scale * g);
		b = TMax(0.0f,scale * b);
		a = TMax(0.0f,scale * a);
		return *this;
	}
	xColor_4f operator *(float scale)
	{
		xColor_4f _ret;
		_ret.r = TMax(0.0f,scale * r);
		_ret.g = TMax(0.0f,scale * g);
		_ret.b = TMax(0.0f,scale * b);
		_ret.a = TMax(0.0f,scale * a);
		return _ret;
	}
	inline friend bool operator ==(const xColor_4f& c1, const xColor_4f& c2){
		return c1.a == c2.a && c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
	}
	inline friend bool operator !=(const xColor_4f& c1, const xColor_4f& c2){
		return c1.a != c2.a || c1.r != c2.r || c1.g != c2.g || c1.b != c2.b;
	}

	float abs()
	{
		return  (0.3f * r + 0.59f * g + 0.11f * b);
	}

};


#if !defined(_MSC_VER) || defined(_WIN32_WCE)



#endif

class  xColor_4ub
{
public:
	union
	{
		struct  {
			unsigned int8 r,g,b,a;
		};
		unsigned long color;
	};

	xColor_4ub operator = (const xColor_4f& rhv)
	{
		r = (unsigned int8)(rhv.r * 255);
		g = (unsigned int8)(rhv.g * 255);
		b = (unsigned int8)(rhv.b * 255);
		a = (unsigned int8)(rhv.a * 255);
		return * this;

	}
	xColor_4ub(const xColor_4f& rhv)
	{
		r = (unsigned int8)(rhv.r * 255);
		g = (unsigned int8)(rhv.g * 255);
		b = (unsigned int8)(rhv.b * 255);
		a = (unsigned int8)(rhv.a * 255);         
	}

	xColor_4ub operator = (const xColor_3ub& rhv);
	xColor_4ub(const xColor_3ub& rhv);



public:
	xColor_4ub(){}
	xColor_4ub(unsigned int8 _r,unsigned int8 _g , unsigned int8 _b,unsigned int8 _a = 255):r(_r),g(_g),b(_b),a(_a) {};
	void        swapEndian()
	{
		unsigned int8 t = r;
		r = b;
		b = t;
	}

	xColor_4ub(float _r,float _g , float _b , float _a  )
	{
		r = (unsigned int8) ( _CLAM_TO_(  _r , 0.0f, 1.0f) * 255);
		g = (unsigned int8) ( _CLAM_TO_(  _g , 0.0f, 1.0f) * 255);
		b = (unsigned int8) ( _CLAM_TO_(  _b , 0.0f, 1.0f) * 255);
		a = (unsigned int8) ( _CLAM_TO_(  _a , 0.0f, 1.0f) * 255);
	}

	xColor_4ub  operator +(const xColor_4ub& rhv)
	{
		xColor_4ub outcl;
#if defined(_MSC_VER) && !defined(_WIN32_WCE)
		__asm
		{
			MOVD mm0 , this;
			PADDUSB mm0,rhv;
			MOVD outcl,mm0;
		}
#else
		outcl.r = TMin(255,r + int(rhv.r));
		outcl.g = TMin(255,g + int(rhv.g));
		outcl.b = TMin(255,b + int(rhv.b));
		outcl.a = TMin(255,a + int(rhv.a));
#endif
		return outcl;
	}

	xColor_4ub  operator -(const xColor_4ub& rhv)
	{
		xColor_4ub outcl;
#if defined(_MSC_VER) && !defined(_WIN32_WCE)
		__asm
		{
			MOVD mm0 , this;
			PSUBUSB mm0,rhv;
			MOVD outcl,mm0;
		}
#else
		outcl.r = TMax(0,r - int(rhv.r));
		outcl.g = TMax(0,g - int(rhv.g));
		outcl.b = TMax(0,b - int(rhv.b));
		outcl.a = TMax(0,a - int(rhv.a));
#endif
		return outcl;
	}

	xColor_4ub& operator +=(const xColor_4ub& rhv)
	{
#if defined(_MSC_VER) && !defined(_WIN32_WCE)
		__asm
		{
			MOVD mm0 , this;
			PADDUSB mm0,rhv;
			MOVD this,mm0;
		}
#else
		r = TMin(255,r + int(rhv.r));
		g = TMin(255,g + int(rhv.g));
		b = TMin(255,b + int(rhv.b));
		a = TMin(255,a + int(rhv.a));
#endif
		return *this;
	}

	xColor_4ub& operator -=(const xColor_4ub& rhv)
	{
#if defined(_MSC_VER) && !defined(_WIN32_WCE)
		__asm
		{
			MOVD mm0 , this;
			PSUBUSB mm0,rhv;
			MOVD this,mm0;
		}
#else
		r = TMax(0,r - int(rhv.r));
		g = TMax(0,g - int(rhv.g));
		b = TMax(0,b - int(rhv.b));
		a = TMax(0,a - int(rhv.a));
#endif
		return *this;
	}

	unsigned int8 abs()
	{
		int n = int(0.3f * r + 0.59f * g + 0.11f * b);
		return (unsigned int8)n;
	}
};

class  xColor_3ub
{
public:
	union
	{
		struct  
		{
			unsigned int8 r,g,b;
		};

	};

	xColor_3ub operator = (const xColor_4f& rhv)
	{
		r = (unsigned int8)(rhv.r * 255);
		g = (unsigned int8)(rhv.g * 255);
		b = (unsigned int8)(rhv.b * 255);
		return * this;
	}

	xColor_3ub(const xColor_4f& rhv)
	{
		r = (unsigned int8)(rhv.r * 255);
		g = (unsigned int8)(rhv.g * 255);
		b = (unsigned int8)(rhv.b * 255);
	}

	xColor_3ub operator = (const xColor_4ub& rhv)
	{
		r = (rhv.r);
		g = (rhv.g);
		b = (rhv.b);
		return * this;
	}

	xColor_3ub(const xColor_4ub& rhv)
	{
		r = (rhv.r);
		g = (rhv.g);
		b = (rhv.b);
	}


public:
	xColor_3ub(){}
	xColor_3ub(unsigned int8 _r,unsigned int8 _g , unsigned int8 _b):r(_r),g(_g),b(_b){};
	void        swapEndian()
	{
		unsigned int8 t = r;
		r = b;
		b = t;
	}

	xColor_3ub(float _r, float _g , float _b , float _a)
	{
		r = (unsigned int8) ( _CLAM_TO_(  _r , 0.0f, 1.0f) * 255);
		g = (unsigned int8) ( _CLAM_TO_(  _g , 0.0f, 1.0f) * 255);
		b = (unsigned int8) ( _CLAM_TO_(  _b , 0.0f, 1.0f) * 255);
	}

	xColor_3ub  operator +(const xColor_3ub& rhv)
	{
		xColor_3ub outcl;
		outcl.r = TMin(255,r + int(rhv.r));
		outcl.g = TMin(255,g + int(rhv.g));
		outcl.b = TMin(255,b + int(rhv.b));
		return outcl;
	}

	xColor_3ub  operator -(const xColor_3ub& rhv)
	{
		xColor_3ub outcl;
		outcl.r = TMax(0,r - int(rhv.r));
		outcl.g = TMax(0,g - int(rhv.g));
		outcl.b = TMax(0,b - int(rhv.b));
		return outcl;
	}

	xColor_3ub& operator +=(const xColor_3ub& rhv)
	{
		r = TMin(255,r + int(rhv.r));
		g = TMin(255,g + int(rhv.g));
		b = TMin(255,b + int(rhv.b));
		return *this;
	}

	xColor_3ub& operator -=(const xColor_3ub& rhv)
	{
		r = TMax(0,r - int(rhv.r));
		g = TMax(0,g - int(rhv.g));
		b = TMax(0,b - int(rhv.b));
		return *this;
	}

	unsigned int8 abs()
	{
		int n = int(0.3f * r + 0.59f * g + 0.11f * b);
		return (unsigned int8)n;
	}
};

inline xColor_4f xColor_4f::operator = (const xColor_4ub& rhv)
{
	r = rhv.r /255.0f;
	g = rhv.g /255.0f;
	b = rhv.b /255.0f;
	a = rhv.a /255.0f;
	return *this;
}

inline xColor_4f xColor_4f::operator = (const xColor_3ub& rhv)
{
	r = rhv.r /255.0f;
	g = rhv.g /255.0f;
	b = rhv.b /255.0f;
	a = 1.0f;
	return *this;
}

inline xColor_4f::xColor_4f(const xColor_4ub& rhv)
{
	r = rhv.r /255.0f;
	g = rhv.g /255.0f;
	b = rhv.b /255.0f;
	a = rhv.a /255.0f;     
}

inline xColor_4f::xColor_4f(const xColor_3ub& rhv)
{
	r = rhv.r /255.0f;
	g = rhv.g /255.0f;
	b = rhv.b /255.0f;
	a = 1.0f;     
}


inline xColor_4ub xColor_4ub::operator = (const xColor_3ub& rhv)
{
	r = rhv.r;
	g = rhv.g;
	b = rhv.b;
	a = 255;
	return *this;
}


inline xColor_4ub::xColor_4ub(const xColor_3ub& rhv)
{
	r = rhv.r;
	g = rhv.g;
	b = rhv.b;
	a = 255;     
}

END_NAMESPACE_XEVOL3D
#pragma pack(pop)
#endif
