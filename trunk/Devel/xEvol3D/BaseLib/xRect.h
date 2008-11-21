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

#ifndef __XEVOL3D_RECT_H__
#define __XEVOL3D_RECT_H__
#include "xEvol3DBaseInc.h"
BEGIN_NAMESPACE_XEVOL3D

template <typename _Type> class X2DPoint
{
public:
	_Type x ;
	_Type y;
	X2DPoint(_Type _x = 0, _Type _y = 0) :x(_x) , y(_y){}
};

template <typename _Type> class X2DRect
{
public:
	_Type x ;
	_Type y ;
	_Type w ;
	_Type h ;
	X2DRect(_Type _x = 0 , _Type _y =0 , _Type _w = 0, _Type _h = 0)
		:x(_x),y(_y),w(_w),h(_h)
	{
	}
    
	_Type right()
	{
		return x + w;
	}

	_Type bottom()
	{
		return y + h;
	}

    _Type top()
	{
		return y;
	}

	_Type left()
	{
		return x;
	}

	bool PtInRect(_Type _x , _Type _y)
	{
		if( _x > left()  &&	_x < right() &&
			_y > top()   &&	_y < bottom() )
		{
			return true;
		}
		return false;
	}
};


template <typename _Type> class X3DBox
{
public:
	_Type x ;
	_Type y ;
	_Type z ;

	_Type w ;
	_Type h ;
	_Type d ;
	X3DBox(_Type _x = 0 , _Type _y =0 , _Type _z = 0 , _Type _w = 0, _Type _h = 0 ,_Type _d = 0)
		:x(_x),y(_y),z(_z) , w(_w),h(_h) , d(_d)
	{
	}
};

typedef X2DPoint<int>      x2DPoint;
typedef X2DPoint<float>    x2DPoint_f;

typedef X2DRect<float>   x2DRect_f;
typedef X2DRect<int>     x2DRect;

typedef X3DBox<float>   x3DBox_F;
typedef X3DBox<int>     x3DBox;

END_NAMESPACE_XEVOL3D

#endif

