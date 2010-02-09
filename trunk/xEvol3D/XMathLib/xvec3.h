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

#ifndef __XVECTOR_3D_INLINE_INCLUDE__
#define __XVECTOR_3D_INLINE_INCLUDE__

#include "xmathbasedef.h"
BEGIN_NAMESPACE_XMATHLIB

template <typename _Type> class  TVec3
{
public:
typedef _Type value_type ;
public:
	TVec3(){};
	TVec3(_Type _v){	x = _v ; y = _v; z = _v; }
	TVec3(_Type _x,_Type _y ,_Type _z){	x = _x ; y = _y; z = _z;}
	TVec3(TVec2<_Type>& v){	x = v.x ; y = v.y ;z = 0 ;}
	TVec3(TVec4<_Type> & v){	x = v.x ; y = v.y ;z = v.z;}
	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void        add(const TVec3<_Type>& rv)
	{
		x += rv.x; y += rv.y; z += rv.z;
	}


	void        sub(const TVec3<_Type>& rv)
	{
		x -= rv.x; y -= rv.y; z -= rv.z;
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void        mul(float s)
	{
		x = _Type(x *s); 
		y = _Type(y *s); 
		z = _Type(z *s);
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void        normalize()
	{
		float len = (float)xsqrt( float(x * x + y * y + z * z) ) ; 
		x=_Type(x/len);
		y=_Type(y/len); 
		z=_Type(z/len);
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void        normalize(TVec3<_Type> &vout)
	{
		float len = (float)xsqrt( float(x * x + y * y + z * z) ) ; 
		vout.x = _Type(x/len); 
		vout.y = _Type(y/len); 
		vout.z = _Type(z/len);
	};

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec3<_Type>    operator +(const TVec3<_Type>& v)  const     
	{ 
		return TVec3<_Type>(v.x + x, v.y + y,v.z + z);
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec3<_Type>    operator -(const TVec3<_Type>& v)  const     
	{ 
		return TVec3<_Type>(x - v.x, y - v.y,z - v.z);
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec3<_Type>    operator *(const float s)  const     
	{ 
		return TVec3<_Type>( _Type(x * s), _Type(y * s) , _Type(z * s) );
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	bool         operator ==(const TVec3<_Type>& v)   const     
	{
		return (v.x == x && v.y == y && v.z == z) ; 
	}

	//-----------------------------------------------------
	//the lenght of a vector
	//-----------------------------------------------------
	value_type        fabs()  const     
	{ 
		return (value_type)xsqrt( float(x * x + y * y + z * z) ) ;
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	value_type        len()  const     
	{ 
		return (value_type)xsqrt( float(x * x + y * y + z * z) );
	}



	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	value_type        squardlen()  const     
	{
		return (value_type)(x * x + y * y + z * z) ;
	}

	//-----------------------------------------------------
	//dot product
	//-----------------------------------------------------
	value_type        dp(const TVec3<_Type>& v)  const     
	{
		return value_type(v.x*x + v.y *y + v.z * z ); 
	}


	//-----------------------------------------------------
	//cross product
	//-----------------------------------------------------
	void         cp(const TVec3<_Type>& v,TVec3<_Type>& vOut)  const     
	{
		vOut.x = y * v.z - v.y * z;
		vOut.y = z * v.x - v.z * x;
		vOut.z = x * v.y - v.x * y;
	}

	//-----------------------------------------------------
	//cross product
	//-----------------------------------------------------
	TVec3<_Type>      cp(const TVec3<_Type>& v)  const     
	{
		_Type nx = y * v.z - v.y * z;
		_Type ny = z * v.x - v.z * x;
		_Type nz = x * v.y - v.x * y;
		return TVec3<_Type>(nx,ny,nz);
	}

	TVec3<_Type>&     operator +=(const TVec3<_Type>& v)
	{ 
		x += v.x ; y += v.y ; 
		return *this; 
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec3<_Type>&     operator -=(const TVec3<_Type>& v)
	{ 
		x -= v.x ; y -= v.y ; 
		return *this; 
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec3<_Type>&     operator *=(value_type s)
	{ 
		x = _Type(x *s); 
		y = _Type(y *s); 
		z = _Type(z *s); 
		return *this; 
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec3<_Type>      operator ^(const TVec3<_Type>& v)  const     
	{ 
		return cp(v); 
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	value_type operator |(TVec3<_Type>& v1)  const     
	{
		value_type t = dp(v1)/(fabs() * v1.fabs());
		return (value_type)XM_ACosD((float)t);
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	bool operator||(const TVec3<_Type>& v1)  const     
	{
		_Type x = y * v1.z - v1.y * z;
		_Type y = z * v1.x - v1.z * x;
		_Type z = x * v1.y - v1.x * y;

		if(numeric_equal(x, (_Type)0) && numeric_equal(y,(_Type)0) && numeric_equal(z,(_Type)0))
			return true;
		else
			return false;
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	value_type clamp(TVec3<_Type>& v1)  const     
	{
		value_type t = dp(v1)/(fabs() * v1.fabs());

		return (value_type)XM_ACosD((float)t);
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	bool isZero()  const     
	{
		return (fabs()<0.000000001f);
	}


	operator _Type* (){ return v ; }

public:
	union
	{
		struct
		{
			_Type x,y,z;
		};
		_Type   v[3];
	};

};

//typedef TVec3<float> xvec3;
//typedef TVec3<int  > xvec3i;
//typedef TVec3<unsigned int  > xvec3ui;

END_NAMESPACE_XMATHLIB

#endif

