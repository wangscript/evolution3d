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

#ifndef __XVECTOR_2D_INLINE_INCLUDE__
#define __XVECTOR_2D_INLINE_INCLUDE__

#include "xmathbasedef.h"
BEGIN_NAMESPACE_XMATHLIB

template <typename _Type> class _XEVOL_BASE_API_ TVec2
{
public:
	TVec2(){};
	~TVec2(){};
	TVec2(_Type _x,_Type _y){x = _x ; y = _y; }
	TVec2(TVec3<_Type>& v){	this->x = v.x;	this->y = v.y;}
	TVec2(TVec4<_Type>& v){	this->x = v.x;	this->y = v.y;}
	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void              add(const TVec2<_Type>& rv)
	{
		x += rv.x; 
		y += rv.y;
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void              sub(const TVec2<_Type>& rv)
	{
		x -= rv.x; 
		y -= rv.y;
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void              mul(float s)
	{
		x *= s; 
		y *= s;
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	float             fabs()  const     
	{ 
		return (float)sqrt(x * x + y * y) ;
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	float             len()  const     
	{ 
		return (float)sqrt(x * x + y * y) ;
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	float             squardlen()  const     
	{
		return (float)(x * x + y * y) ;
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void              normalize()
	{
		float len = (float)sqrt(x * x + y * y) ; 
		x/=len;
		y/=len; 
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void              normalize(TVec2<_Type> &vout)
	{
		float len = (float)sqrt(x * x + y * y) ; 
		vout.x = x/len; vout.y = y/len;
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec2<_Type>         operator +(const TVec2<_Type>& v)  const     
	{ 
		return TVec2<_Type>(v.x + x, v.y + y);
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec2<_Type>         operator -(const TVec2<_Type>& v)  const     
	{ 
		return TVec2<_Type>(x - v.x, y - v.y);
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec2<_Type>         operator *(const float s)  const     
	{ 
		return TVec2<_Type>(x * s, y * s);
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	bool             operator ==(const TVec2<_Type>& v)  const      
	{
		return (v.x == x && v.y == y); 
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	bool             isZero()  const     
	{
		return (x ==0 && y ==0);
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	float            dp(const TVec2<_Type>& v)  const     
	{
		return (v.x*x + v.y *y) ;
	}



	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec2<_Type>&        operator +=(const TVec2<_Type>& v)
	{ 
		x += v.x ; y += v.y ; 
		return *this; 
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec2<_Type>&        operator -=(const TVec2<_Type>& v)
	{ 
		x -= v.x ; y -= v.y ; 
		return *this; 
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec2<_Type>&        operator *=(float s)
	{ 
		x *= s ; y *= s ; 
		return *this; 
	}

	operator _Type* (){ return v ; }
public:
	union
	{
		struct
		{
			_Type x,y;
		};
		struct
		{
			_Type s,t;
		};
		_Type v[2];
	};
};

//typedef TVec2<float>         xvec2;
//typedef TVec2<unsigned int>  xvec2ui;
//typedef TVec2<int>           xvec2i;

END_NAMESPACE_XMATHLIB



#endif


