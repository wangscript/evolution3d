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
      _Type x ;
      _Type y ;
      _Type w ;
      _Type h ;
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
	//Constructor build a vector
	TVec4(){}
	TVec4(_Type _x,_Type _y = 0,_Type _z = 0,_Type _w = 1):x(_x),y(_y),z(_z),w(_w){};
	TVec4(const TVec2<_Type>& v, float _z = 0.0f , float _w = 1.0f){x = v.x ; y = v.y ;	z = _z   ; 	w = _w ; };
	TVec4(const TVec3<_Type>& v, float _w = 1.0f){x = v.x ; y = v.y ; z = v.z ; 	w = _w ;	}
	~TVec4(){};//Force a Null destorier
public:
      void setRect(_Type _x , _Type _y , _Type _w , _Type _h)
      {
            rect.x = _x ; rect.y = _y ; rect.w = _w ; rect.h = _h;
      }
	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	float fabs()  const          
	{
		return (float) sqrt(x*x + y*y + z*z); 
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	float dp3(const TVec4<_Type>& v1)   const     
	{
		return v1.x * x + v1.y * y + v1.z * z;
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	float dp4(const TVec4<_Type>& v1)   const     
	{
		return v1.x * x + v1.y * y + v1.z * z + v1.w * w;
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
	float  len()  const     
	{ 
		return (float)sqrt(x * x + y * y + z * z) ;
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	float      squardlen()  const     
	{
		return (x * x + y * y + z * z) ;
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	TVec4<_Type> cp(const TVec4<_Type>& v1)  const      
	{
		float nx = y * v1.z - v1.y * z;
		float ny = z * v1.x - v1.z * x;
		float nz = x * v1.y - v1.x * y;
		return TVec4<_Type>(nx,ny,nz,1);
	}


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void normalize() 
	{
		float len = (float) sqrt(x*x + y*y + z*z);

		if(len == 0)
			return ;
		x/=len;
		y/=len;
		z/=len;
		w = 1;
	};


	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	void normalize(TVec4<_Type>& out) const
	{
		float len = (float) sqrt(x*x + y*y + z*z);
		if(len == 0)
			return ;
		out.x=x/len;
		out.y=y/len;
		out.z=z/len;
		out.w = 1;
	};

	TVec4<_Type> getNormal() const
	{
		TVec4<_Type> out(0,0,0,1);
		float len = (float) sqrt(x*x + y*y + z*z);
		if(len != 0)
		{
			out.x=x/len;
			out.y=y/len;
			out.z=z/len;
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
		x*=s;
		y*=s;
		z*=s;
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
	TVec4<_Type> operator *(const float s)  const     
	{
		return TVec4(x * s,y * s,z * s , 1);
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	float   operator *(const TVec4<_Type>& v1)  const     
	{
		return (v1.x * x + v1.y * y + v1.z * z) ;
	}

	//-------------------------------------------------------
	//Cross product
	//-------------------------------------------------------
	TVec4<_Type> operator ^(const TVec4<_Type>& v1)  const     
	{
		float nx = y * v1.z - v1.y * z;
		float ny = z * v1.x - v1.z * x;
		float nz = x * v1.y - v1.x * y;
		return TVec4<_Type>(nx,ny,nz,1);
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	float operator |(const TVec4<_Type>& v1)  const     
	{
		float t = dp3(v1)/(fabs() * v1.fabs());
		return (float)XM_ACosD(t);
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	bool operator||(const TVec4<_Type>& v1)  const     
	{
		float nx = y * v1.z - v1.y * z;
		float ny = z * v1.x - v1.z * x;
		float nz = x * v1.y - v1.x * y;

		if(nx == 0 && ny == 0 && nz == 0)
			return true;
		else
			return false;
	}

	//-----------------------------------------------------
	//
	//-----------------------------------------------------
	float clamp(TVec4<_Type>& v1)  const     
	{
		float t = dp3(v1)/(fabs() * v1.fabs());
		return (float)XM_ACosD(t);
	}


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

//typedef TVec4<float> xvec4;
//typedef TVec4<unsigned int> xvec4ui;
//typedef TVec4<int> xvec4i;

END_NAMESPACE_XMATHLIB

#endif

