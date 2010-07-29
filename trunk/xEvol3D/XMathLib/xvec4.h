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

#ifndef __XVECTOR_INLINE_INCLUDE__
#define __XVECTOR_INLINE_INCLUDE__

BEGIN_NAMESPACE_XMATHLIB

template <typename _Type> class TRect
{
public:
    union
    {
        struct { 
            _Type x ;
            _Type y ;
            _Type w ;
            _Type h ;
        };
        _Type v[4];
    };

public:
      _Type right() {     return x + w;   }
      _Type bottom(){     return y + h;   }
      _Type top()   {     return y;       }
      _Type left()  {     return x;       }
      bool  PtInRect(_Type _x , _Type _y)
      {
            if( _x > left()  &&	_x < right() &&
                  _y > top()   &&	_y < bottom() )
            {
                  return true;
            }
            return false;
      }
};

template <typename _Type> class  _XEVOL_BASE_API_ TVec4
{
public:
	typedef _Type value_type ;
public:
	//Constructor build a vector
	TVec4(){}
	TVec4(_Type _x,_Type _y = 0,_Type _z = 0,_Type _w = 1):x(_x),y(_y),z(_z),w(_w){};
	TVec4(const TVec2<_Type>& v, _Type _z = 0 , _Type _w = 1){x = v.x ; y = v.y ;	z = _z   ; 	w = _w ; };
	TVec4(const TVec3<_Type>& v, _Type _w = 1){x = v.x ; y = v.y ; z = v.z ; 	w = _w ;	}
	~TVec4(){};//Force a Null destorier
public:
      void setRect(_Type _x , _Type _y , _Type _w , _Type _h)
      {
            rect.x = _x ; rect.y = _y ; rect.w = _w ; rect.h = _h;
      }
	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	value_type fabs()  const          
	{
		return (value_type) xsqrt( value_type(x*x + y*y + z*z) ); 
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	value_type dp3(const TVec4<_Type>& v1)   const     
	{
		return value_type(v1.x * x + v1.y * y + v1.z * z);
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	value_type dp4(const TVec4<_Type>& v1)   const     
	{
		return value_type(v1.x * x + v1.y * y + v1.z * z + v1.w * w);
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void  add(const TVec4<_Type>& v1) 
	{ 
		x += v1.x; y += v1.y; z += v1.z; 
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void  sub(const TVec4<_Type>& v1) 
	{ 
		x += v1.x; y += v1.y; z += v1.z; 
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void  rhw()
	{ 
		x/=w ; 
		y/=w ; 
		z/=w ; 
		w=1; 
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	value_type  len()  const     
	{ 
		return (value_type)xsqrt( value_type(x * x + y * y + z * z) ) ;
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	value_type      squardlen()  const     
	{
		return value_type(x * x + y * y + z * z) ;
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec4<_Type> cp(const TVec4<_Type>& v1)  const      
	{
		_Type nx = y * v1.z - v1.y * z;
		_Type ny = z * v1.x - v1.z * x;
		_Type nz = x * v1.y - v1.x * y;
		return TVec4<_Type>(nx,ny,nz,1);
	}

	void cp(const TVec4<_Type>& v1 , TVec4<_Type>& _out)  const      
	{
		_out.x = y * v1.z - v1.y * z;
		_out.y = z * v1.x - v1.z * x;
		_out.z = x * v1.y - v1.x * y;
		_out.w = 1.0f;
	}

	void cp(const TVec4<_Type>& v1 , TVec3<_Type>& _out)  const      
	{
		_out.x = y * v1.z - v1.y * z;
		_out.y = z * v1.x - v1.z * x;
		_out.z = x * v1.y - v1.x * y;
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void normalize() 
	{
		value_type len = (value_type) xsqrt( float(x*x + y*y + z*z) );

		if(len == 0)
			return ;
		x= _Type(x/len);
		y= _Type(y/len);
		z= _Type(z/len);
		w = 1;
	};


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void normalize(TVec4<_Type>& out) const
	{
		value_type len = (value_type) xsqrt( float(x*x + y*y + z*z) );
		if(numeric_equal(len,(value_type)0))
			return ;
		out.x= _Type(x/len);
		out.y= _Type(y/len);
		out.z= _Type(z/len);
		out.w = 1;
	};

	TVec4<_Type> getNormal() const
	{
		TVec4<_Type> out(0,0,0,1);
		value_type len = (value_type) xsqrt( float(x*x + y*y + z*z) );
		if(len != 0)
		{
			out.x= _Type(x/len);
			out.y= _Type(y/len);
			out.z= _Type(z/len);
			out.w = 1;
		}
		return out;
	};


public:

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void    operator *=(const float s)
	{
		x= _Type(x*s);
		y= _Type(y*s);
		z= _Type(z*s);
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	bool    operator ==(const TVec4<_Type>& v)   const     
	{
		return (v.x == x && v.y == y && v.z == z && v.w == w) ;
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec4 operator -(const TVec4<_Type>& v1)  const     
	{
		return TVec4<_Type>(x - v1.x , y-v1.y ,z - v1.z, 1  );
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec4 operator -()  const     
	{
		return TVec4<_Type>(-x , -y ,- z, w );
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec4<_Type> operator +(const TVec4<_Type>& v1)  const     
	{
		return TVec4<_Type>(x + v1.x , y + v1.y ,z + v1.z, 1  );
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec4<_Type> operator *(const value_type s)  const     
	{
		return TVec4(  _Type(x * s) ,  _Type(y * s),  _Type(z * s) , 1);
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	float   operator *(const TVec4<_Type>& v1)  const     
	{
		return ( (float)(v1.x * x + v1.y * y + v1.z * z) );
	}

	//-------------------------------------------------------
	//Cross product
	//-------------------------------------------------------
	TVec4<_Type> operator ^(const TVec4<_Type>& v1)  const     
	{
		_Type nx = y * v1.z - v1.y * z;
		_Type ny = z * v1.x - v1.z * x;
		_Type nz = x * v1.y - v1.x * y;
		return TVec4<_Type>(nx,ny,nz,1);
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	value_type operator |(const TVec4<_Type>& v1)  const     
	{
		value_type t = dp3(v1)/(fabs() * v1.fabs());
		return (value_type)XM_ACosD((float)t);
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	bool operator||(const TVec4<_Type>& v1)  const     
	{
		_Type nx = y * v1.z - v1.y * z;
		_Type ny = z * v1.x - v1.z * x;
		_Type nz = x * v1.y - v1.x * y;

		if( numeric_equal(nx , (_Type)0) && numeric_equal(ny ,  (_Type)0) && numeric_equal(nz ,  (_Type)0) )
			return true;
		else
			return false;
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	value_type clamp(TVec4<_Type>& v1)  const     
	{
		value_type t = dp3(v1)/(fabs() * v1.fabs());
		return (value_type)XM_ACosD((float)t);
	}

#define MK_SWIZLLE3( a , b , c) TVec3<_Type>  a##b##c() { return TVec3<_Type>(a , b , c); }
    //=======x========
    MK_SWIZLLE3(x,x,x);
    MK_SWIZLLE3(x,x,y);
    MK_SWIZLLE3(x,x,z);
    MK_SWIZLLE3(x,x,w);

    MK_SWIZLLE3(x,y,x);
    MK_SWIZLLE3(x,y,y);
    MK_SWIZLLE3(x,y,z);
    MK_SWIZLLE3(x,y,w);

    MK_SWIZLLE3(x,z,x);
    MK_SWIZLLE3(x,z,y);
    MK_SWIZLLE3(x,z,z);
    MK_SWIZLLE3(x,z,w);

    MK_SWIZLLE3(x,w,x);
    MK_SWIZLLE3(x,w,y);
    MK_SWIZLLE3(x,w,z);
    MK_SWIZLLE3(x,w,w);

    //======y==========
    MK_SWIZLLE3(y,x,x);
    MK_SWIZLLE3(y,x,y);
    MK_SWIZLLE3(y,x,z);
    MK_SWIZLLE3(y,x,w);

    MK_SWIZLLE3(y,y,x);
    MK_SWIZLLE3(y,y,y);
    MK_SWIZLLE3(y,y,z);
    MK_SWIZLLE3(y,y,w);

    MK_SWIZLLE3(y,z,x);
    MK_SWIZLLE3(y,z,y);
    MK_SWIZLLE3(y,z,z);
    MK_SWIZLLE3(y,z,w);

    MK_SWIZLLE3(y,w,x);
    MK_SWIZLLE3(y,w,y);
    MK_SWIZLLE3(y,w,z);
    MK_SWIZLLE3(y,w,w);

    //======z==========
    MK_SWIZLLE3(z,x,x);
    MK_SWIZLLE3(z,x,y);
    MK_SWIZLLE3(z,x,z);
    MK_SWIZLLE3(z,x,w);

    MK_SWIZLLE3(z,y,x);
    MK_SWIZLLE3(z,y,y);
    MK_SWIZLLE3(z,y,z);
    MK_SWIZLLE3(z,y,w);

    MK_SWIZLLE3(z,z,x);
    MK_SWIZLLE3(z,z,y);
    MK_SWIZLLE3(z,z,z);
    MK_SWIZLLE3(z,z,w);

    MK_SWIZLLE3(z,w,x);
    MK_SWIZLLE3(z,w,y);
    MK_SWIZLLE3(z,w,z);
    MK_SWIZLLE3(z,w,w);

    //======w==========
    MK_SWIZLLE3(w,x,x);
    MK_SWIZLLE3(w,x,y);
    MK_SWIZLLE3(w,x,z);
    MK_SWIZLLE3(w,x,w);

    MK_SWIZLLE3(w,y,x);
    MK_SWIZLLE3(w,y,y);
    MK_SWIZLLE3(w,y,z);
    MK_SWIZLLE3(w,y,w);

    MK_SWIZLLE3(w,z,x);
    MK_SWIZLLE3(w,z,y);
    MK_SWIZLLE3(w,z,z);
    MK_SWIZLLE3(w,z,w);

    MK_SWIZLLE3(w,w,x);
    MK_SWIZLLE3(w,w,y);
    MK_SWIZLLE3(w,w,z);
    MK_SWIZLLE3(w,w,w);
#undef MK_SWIZLLE3

#define MK_SWIZLLE2( a , b ) TVec3<_Type>  a##b() { return TVec2<_Type>(a , b); }

    MK_SWIZLLE2(x,x);
    MK_SWIZLLE2(x,y);
    MK_SWIZLLE2(x,z);
    MK_SWIZLLE2(x,w);

    MK_SWIZLLE2(y,x);
    MK_SWIZLLE2(y,y);
    MK_SWIZLLE2(y,z);
    MK_SWIZLLE2(y,w);

    MK_SWIZLLE2(z,x);
    MK_SWIZLLE2(z,y);
    MK_SWIZLLE2(z,z);
    MK_SWIZLLE2(z,w);

    MK_SWIZLLE2(w,x);
    MK_SWIZLLE2(w,y);
    MK_SWIZLLE2(w,z);
    MK_SWIZLLE2(w,w);
#undef MK_SWIZLLE2

	operator _Type* (){ return v ; }

public://We set this data public is for easy access
	union
	{
		struct
		{
			_Type x;
			_Type y;
			_Type z;
			_Type w;//Four component the w is useful ......
		};
		//for texture coordinate
		struct
		{
			_Type s,t,r,q;
		};
		TRect<_Type> rect;
		_Type        v[4];
	};
};
template <typename T> TVec4<T> operator * (const T& v1 , const TVec4<T>& v2)
{
    TVec4<T> _ret;
    _ret.x = v1 * v2.x;
    _ret.y = v1 * v2.y;
    _ret.z = v1 * v2.z;
    _ret.w = v1 * v2.w;
    return _ret;
}

template <typename T> TVec4<T> operator * (const TVec4<T>& v1 , const TVec4<T>& v2)
{
    TVec4<T> _ret;
    _ret.x = v1.x * v2.x;
    _ret.y = v1.y * v2.y;
    _ret.z = v1.z * v2.z;
    _ret.w = v1.w * v2.w;
    return _ret;
}

//typedef TVec4<float> xvec4;
//typedef TVec4<unsigned int> xvec4ui;
//typedef TVec4<int> xvec4i;

END_NAMESPACE_XMATHLIB

#endif

