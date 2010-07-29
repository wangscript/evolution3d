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

#ifndef __XMATH_FUNC_INLINE_INCLUDE__
#define __XMATH_FUNC_INLINE_INCLUDE__

#include "xmathbasedef.h"

#ifdef __XMATHLIB_ASM__

#ifdef _MSC_VER
#include "XMath_ASM_VC.h"
#endif

#ifdef _GNUC_
#include "XMath_ASM_GCC.h"
#endif

#endif

#ifndef __XMATHLIB_ASM__
BEGIN_NAMESPACE_XMATHLIB
__FORCE_INLINE__ void   XM_SinCosD(float angle,float& s,float& c)
{
	angle = (float)angle / 180 * (float)XM_PI;
	s = (float)sin(angle);
	c = (float)cos(angle);
}

__FORCE_INLINE__ void   XM_SinCosR(float angle,float& s,float& c)
{
	s = (float)sin(angle);
	c = (float)cos(angle);
}

__FORCE_INLINE__ float  XM_CosD(float angle)
{
	angle = (float)angle / 180 * (float)XM_PI;
	return (float)cos(angle);
}

__FORCE_INLINE__ float  XM_SinD(float angle)
{
	angle = (float)angle / 180 * (float)XM_PI;
	return (float)sin(angle);
}

__FORCE_INLINE__ float  XM_CosR(float angle)
{
	return (float)cos(angle);
}

__FORCE_INLINE__ float  XM_SinR(float angle)
{
	return (float)sin(angle);
}


END_NAMESPACE_XMATHLIB
#endif


//普通的数学查表函数。
BEGIN_NAMESPACE_XMATHLIB
namespace static_table
{
	static double s_sin_table [36000];
	static double s_cos_table [36000];
}

__FORCE_INLINE__ void   XM_InitTriTable()
{
	double d = 1/180.0 *XM_PI;
	for(int i = 0 ;i < 36000 ; i ++)
	{
		static_table::s_cos_table[i] = (float)cos(i/100. * d);
		static_table::s_sin_table[i] = (float)cos(i/100. * d);
	}
}
__FORCE_INLINE__ double XM_FastSinD(float angle)
{
	int a = static_cast<int>(angle * 100);
	if(a > 36000)
	{
		return static_table::s_sin_table[a%36000];
	}
	else if(a < 0)
	{
		a = -a;
		if(a > 36000)
		{
			return -static_table::s_sin_table[a%36000];
		}
		else
		{
			return -static_table::s_sin_table[a];
		}
	}
	else
	{
		return -static_table::s_sin_table[a];
	}
}

__FORCE_INLINE__ double XM_FastCosD(float angle)
{
	int a = static_cast<int>(fabs(angle) * 100);

	if(a > 36000)
	{
		return static_table::s_cos_table[a%36000];
	}
	else
	{
		return static_table::s_cos_table[a];
	}
}

__FORCE_INLINE__ double XM_FastSinR(float angle)
{
	int a = static_cast<int>(angle * 180.0/XM_PI * 100);
	if(a > 36000)
	{
		return static_table::s_sin_table[a%36000];
	}
	else if(a < 0)
	{
		a = -a;
		if(a > 36000)
		{
			return -static_table::s_sin_table[a%36000];
		}
		else
		{
			return -static_table::s_sin_table[a];
		}
	}
	else
	{
		return -static_table::s_sin_table[a];
	}
}

__FORCE_INLINE__ double XM_FastCosR(float angle)
{
	int a = static_cast<int>(fabs(angle) * 180.0/XM_PI* 100);

	if(a > 36000)
	{
		return static_table::s_cos_table[a%36000];
	}
	else
	{
		return static_table::s_cos_table[a];
	}
}
END_NAMESPACE_XMATHLIB

//常用的数学函数，肯定没有汇编版本的
BEGIN_NAMESPACE_XMATHLIB


template <typename T> void  XM_Swap(T& arg1,T& arg2)
{
	T temp= arg1;
	arg1 = arg2;
	arg2 = temp;
	return ;
}



__FORCE_INLINE__ float XM_InvSqrt (float x)
{
	float hf = 0.5f*x;
	int i = *(int*)&x;
	i = 0x5f3759df - (i >> 1);
	x = *(float*)&i;
	x = x*(1.5f - hf*x*x);
	return x;
}

__FORCE_INLINE__ float  XM_Rad2Deg(float rad)
{
	return (float)rad / (float)XM_PI * 180;
}

__FORCE_INLINE__ float  XM_Deg2Rad(float deg)
{
	return (float)deg / 180 * (float)XM_PI;
}


__FORCE_INLINE__ float  XM_ACosD(float c)
{
	float rt = (float)acos(c);
	return (float)(rt / XM_PI * 180);
}

__FORCE_INLINE__ float  XM_ASinD(float s)
{
	float rt = (float)asin(s);
	return (float)(rt / XM_PI * 180);
}
__FORCE_INLINE__ float  XM_ACosR(float c)
{
	return (float)acos(c);
}
__FORCE_INLINE__ float  XM_ASinR(float s)
{

	return (float)asin(s);
}

template <typename T>T  xsqrt(const T v)
{
	return sqrt( (float)v);
}

__FORCE_INLINE__ double  xsqrt(const double v)
{
    return sqrt(v);
}

__FORCE_INLINE__ float  xsqrt(const float v)
{
    return sqrt(v);
}

__FORCE_INLINE__ int    xsqrt(const int v)
{
	return (int)sqrt( (double) v);
}

__FORCE_INLINE__  unsigned int  xsqrt(const unsigned int v)
{
	return (unsigned int)sqrt( (double) v);
}

__FORCE_INLINE__ short    xsqrt(const short v)
{
	return (short)sqrt( (double) v);
}

__FORCE_INLINE__  unsigned short  xsqrt(const unsigned short v)
{
	return (unsigned short)sqrt( (double) v);
}

__FORCE_INLINE__  char    xsqrt(const char v)
{
	return (char)sqrt( (double) v);
}

__FORCE_INLINE__ unsigned char    xsqrt(const unsigned char v)
{
	return (unsigned char)xsqrt( (double) v);
}


template <typename T> bool numeric_equal(const T& v1 , const T& v2)
{
	return v1 == v2;
}


inline bool  numeric_equal(const float& v1 , const float& v2)
{
	 const float epsilon = 0.0000001f;
     return ( fabs(v1 - v2 ) < epsilon );
}

inline bool  numeric_equal(const double& v1 , const double& v2)
{
	const double epsilon = 0.0000001;
	return ( fabs(v1 - v2 ) < epsilon );
}

END_NAMESPACE_XMATHLIB

#endif

