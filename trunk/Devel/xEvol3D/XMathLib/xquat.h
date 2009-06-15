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

#ifndef __XQUATERNION_INLINE_INCLUDE__
#define __XQUATERNION_INLINE_INCLUDE__
#include "xmathbasedef.h"

BEGIN_NAMESPACE_XMATHLIB

class _XEVOL_BASE_API_ xeuler : public xvec3
{
public:
	float heading(){return  XM_Rad2Deg(y); }
	float pitch()  {return  XM_Rad2Deg(x); }
	float bank()   {return  XM_Rad2Deg(z); }
	float yAgnle() {return  XM_Rad2Deg(y); }
	float xAgnle() {return  XM_Rad2Deg(x); }
	float zAgnle() {return  XM_Rad2Deg(z); }

	void toMatrix(xmat4& mOut);
	void fromMatrix(xmat4& mIn);
};

class  _XEVOL_BASE_API_ xquat
{
public:
	xquat(float _x,float _y,float _z,float _w):x(_x),y(_y),z(_z),w(_w){};
	xquat(){x = 0.0f; y = 0.0f ; z = 0.0f;w = 1.0f;}
	float      magnitude(){return (float)sqrt(x*x + y*y + z*z + w*w);}
	void       conjugate(){x = -x ; y = -y ; z =-z;}
	void       conjugate(xquat& out){out.x =-x; out.y = -y ; out.z = -z;}

	xquat(const xvec4& axis,float angle = 0.0f)
	{
		set(axis,angle);
	}

	void set(float x , float y , float z,float angle = 0.0f)
	{
		xvec3 axis(x,y,z);
		set(axis,angle);
	}

	void toAxis(xvec3& axis,float& angle)
	{
		float angle2 = (float)acos(w);
		float sin2 = (float)sqrt(1.0f - w*w);
		if(sin2 == 0)
		{
			axis.x = 0;
			axis.y = 0;
			axis.z = 1;
		}
		else
		{
			axis.x = x / sin2;
			axis.y = y / sin2;
			axis.z = z / sin2;
		}
		angle = angle2 * 2.0f;
		angle = XM_Rad2Deg(angle);
	}

	void set(const xvec3& axis,float angle = 0.0f)
	{
		angle = XM_Deg2Rad(angle);
		//float len = 1.0f/axis.len();
		float angle2 = angle/2.0f;
		float sinangle2 = XM_SinR(angle2);
		float cosangle2 = XM_CosR(angle2);
		x = axis.x * sinangle2;
		y = axis.y * sinangle2;
		z = axis.z * sinangle2;
		w = cosangle2;
	}

	void set(const xvec4& axis,float angle = 0.0f)
	{
		angle = XM_Deg2Rad(angle);
		//float len = 1.0f/axis.len();
		float angle2 = angle/2.0f;
		float sinangle2 = XM_SinR(angle2);
		float cosangle2 = XM_CosR(angle2);
		x = axis.x * sinangle2;
		y = axis.y * sinangle2;
		z = axis.z * sinangle2;
		w = cosangle2;
	}

	xquat& normalize()
	{
		float len = magnitude();
		x /= len;
		y /= len;
		z /= len;
		w /= len;
		return *this;
	}

	void normalize(xquat& out)
	{
		float len = magnitude();
		out.x /= len;
		out.y /= len;
		out.z /= len;
		out.w /= len;
	}

	xquat& inverse()
	{
		float n = x * x + y * y + z * z + w * w;
		n = -1.0f / n;
		x *= n;
		y *= n;
		z *= n;
		w *= n;
		return *this;
	}

	void inverse(xquat& qOut)
	{
		float n = x * x + y * y + z * z + w * w;
		n = -1.0f / n;
		qOut.x = x*n;
		qOut.y = y*n;
		qOut.z = x*n;
		qOut.w = w*n;
		return ;
	}

public:
	void           fromEuler(xeuler& _euler);
	void           toEuler(xeuler& _euler);
	xmat4          toMatrix();
	void           toMatrix(xmat4& mOut);
	xquat          operator * (const xquat& q2);
	xquat&         operator *= (const xquat& q2);
	xquat          slerp(float t,const xquat& q2);
	xvec4          rotate(const xvec4& vIn);
	operator float* (){ return &x ; }

public:
	float x;
	float y;
	float z;
	float w;
};

END_NAMESPACE_XMATHLIB


#endif

