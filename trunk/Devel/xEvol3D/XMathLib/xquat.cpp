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


#include "XMathLib.h"

BEGIN_NAMESPACE_XMATHLIB

void xeuler::toMatrix(xmat4& mOut)
{
#define M(row,col)  mOut.m [row ] [ col ]
	float &h = y;//heading
	float &p = x;//pitch.
	float &b = z;//bunk

	float sp = -M(2 , 1);
	if(sp <= -1.0f )
	{
		p = -1.570796f ; // -Pi/2
	}
	else
	{
		p = asin(sp);
	}

	if( sp > 0.9999999f)
	{
		b = 0.0f;
		h = atan2(-M(0,2) , M(0,0) );
	}
	else
	{
		 b = atan2(M(0,1) , M(1,1) );
		 h = atan2(M(2,0) , M(2,2) );
	}
#undef M
}

void xeuler::fromMatrix(xmat4& mIn)
{
#define M(row,col)  mIn.m [row ] [ col ]
	float &h = y;//heading
	float &p = x;//pitch.
	float &b = z;//bunk
	float sin_h = sin(h) ;
	float sin_p = sin(p) ;
	float sin_b = sin(b) ;
	float cos_h = cos(h) ; 
	float cos_p = cos(p) ;
	float cos_b = cos(b) ;

	M(0,0) =  cos_h * cos_b + sin_h * sin_p * sin_b;
	M(1,0) =  sin_b * cos_p;
	M(2,0) = -sin_h * cos_b + cos_h * sin_p * sin_b;
	M(3,0) = 0;

	M(0,1) = -cos_h * sin_b + sin_h * sin_p * cos_b;
	M(1,1) =  cos_b * cos_p;
	M(2,1) =  sin_h * sin_b + cos_h * sin_p * cos_b;
	M(3,1) = 0;

	M(0,2) = sin_h * cos_p;
	M(1,2) = -sin_p;
	M(2,2) = cos_h * cos_p;
	M(3,2) = 0;

	M(0,3) = 0;
	M(1,3) = 0;
	M(2,3) = 0;
	M(3,3) = 1;
#undef M
}

void xquat::fromEuler(xeuler& _euler)
{
	float h = _euler.y;//heading
	float p = _euler.x;//pitch.
	float b = _euler.z;//bunk
	w = cos(h/2)*cos(p/2)*cos(b/2) + sin(h/2)*sin(p/2)*sin(b/2);

	x = cos(h/2)*sin(p/2)*cos(b/2) + sin(h/2)*cos(p/2)*sin(b/2);
	y = sin(h/2)*cos(p/2)*cos(b/2) - cos(h/2)*sin(p/2)*sin(b/2);
	z = cos(h/2)*cos(p/2)*sin(b/2) - sin(h/2)*sin(p/2)*cos(b/2);
}

void xquat::toEuler(xeuler& _euler)
{
	float &h = _euler.y;//heading
	float &p = _euler.x;//pitch.
	float &b = _euler.z;//bunk
	h = 0.0f;
	p = 0.0f; 
	b = 0.0f; 
	float sp = -2.0f *(y*z + w*x);
	if(fabs(sp) > 0.99999f)
	{
		p = 1.570796f * sp;
		h = atan2(-x * z + w * y , 0.5f - y * y - z * z);
		b = 0.0f;
	}
	else
	{
		p = asin( sp);
		h = atan2(x * z + w * y , 0.5f - x * x - y * y);
		b = atan2(x * y + w * z , 0.5f - x * x - z * z);
	}
}

xquat& xquat::operator *= (const xquat& q2)
{
	xquat ret ;
	ret.x = w*q2.x + x*q2.w + y*q2.z - z*q2.y;
	ret.y = w*q2.y + y*q2.w + z*q2.x - x*q2.z;
	ret.z = w*q2.z + z*q2.w + x*q2.y - y*q2.x;
	ret.w = w*q2.w - x*q2.x - y*q2.y - z*q2.z;
	x = ret.x;
	y = ret.y;
	z = ret.z;
	w = ret.w;
	return *this;
}

xquat xquat::operator * (const xquat& q2)
{
	xquat ret ;
	ret.x = w*q2.x + x*q2.w + y*q2.z - z*q2.y;
	ret.y = w*q2.y + y*q2.w + z*q2.x - x*q2.z;
	ret.z = w*q2.z + z*q2.w + x*q2.y - y*q2.x;
	ret.w = w*q2.w - x*q2.x - y*q2.y - z*q2.z;
	return ret;
}


xvec4    xquat::rotate(const xvec4& vIn)
{
	xvec4 vTemp;
	XM_Rotate(*this,vIn,vTemp);
	return vTemp;
}





xmat4 xquat::toMatrix()
{
	xmat4 mOut;
	toMatrix(mOut);
	return mOut;
}



void     xquat::toMatrix(xmat4& mOut)
{
#define M(row,col)  mOut.m [row ] [ col ]

	float x2 = x*x;
	float y2 = y*y;
	float z2 = z*z;
	float w2 = w*w;
	float xy = 2*x*y;
	float wz = 2*w*z;
	float wy = 2*w*y;
	float xz = 2*x*z;
	float yz = 2*y*z;
	float wx = 2*x*w;

	M(0,0) = 1.0f - 2.0f * (y2+z2);
	M(1,0) = (xy + wz);
	M(2,0) = (xz - wy);
	M(3,0) = 0;

	M(0,1) = (xy - wz);
	M(1,1) = 1.0f - 2.0f * (x2 + z2);
	M(2,1) = (yz + wx);
	M(3,1) = 0;

	M(0,2) = (xz + wy);
	M(1,2) = (yz - wx);
	M(2,2) = 1.0f - 2.0f * (x2 + y2);
	M(3,2) = 0;

	M(0,3) = 0;
	M(1,3) = 0;
	M(2,3) = 0;
	M(3,3) = 1;
#undef M
}

xquat xquat::slerp(float t,  const xquat& q2)
{
	xquat qOut;
	XM_Slerp(t,*this,q2,qOut);
	return qOut;
}


void    _XEVOL_BASE_API_ XM_Mul(const xquat& qA,const xquat& qB,xquat& qOut)
{
	qOut.x = qA.w*qB.x + qA.x*qB.w + qA.y*qB.z - qA.z*qB.y;
	qOut.y = qA.w*qB.y + qA.y*qB.w + qA.z*qB.x - qA.x*qB.z;
	qOut.z = qA.w*qB.z + qA.z*qB.w + qA.x*qB.y - qA.y*qB.x;
	qOut.w = qA.w*qB.w - qA.x*qB.x - qA.y*qB.y - qA.z*qB.z;
}

void    _XEVOL_BASE_API_ XM_Add(const xquat& qA,const xquat& qB,xquat& qOut)
{
	qOut.x = qA.x + qB.x  ;
	qOut.y = qA.y + qB.y  ;
	qOut.z = qA.z + qB.z  ;
	qOut.w = qA.w + qB.w  ;
}

void    _XEVOL_BASE_API_ XM_Inverse(const xquat& qA ,xquat& qOut)
{
	float n = qA.x * qA.x + qA.y * qA.y + qA.z * qA.z + qA.w * qA.w;
	n = -1.0f / n ;
	qOut = qA;
	qOut.x *= n;
	qOut.y *= n;
	qOut.z *= n;
	qOut.w *= n;
}

void    _XEVOL_BASE_API_ XM_Rotate(const xquat& q,const xvec4& v,xvec4& vOut)
{
	xquat qn = q;

	qn.normalize();
	xquat qc(-qn.x,-qn.y,-qn.z,qn.w);
	xquat qv(v.x,v.y,v.z,1);

	xquat qtemp;
	xquat qresult;
	//  [qn * V ] * qn(-1)
	XM_Mul(qn,qv,qtemp);
	XM_Mul(qtemp,qc,qresult);

	vOut.x = qresult.x;
	vOut.y = qresult.y;
	vOut.z = qresult.z;
	vOut.w = 1;
}

void    _XEVOL_BASE_API_ XM_Slerp(float t,const xquat& q0,const xquat& q1,xquat& qOut)
{
	float k0,k1,cosomega = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
	xquat q;
	if(cosomega < 0.0) 
	{
		cosomega = -cosomega;
		q.x = -q1.x;
		q.y = -q1.y;
		q.z = -q1.z;
		q.w = -q1.w;
	} 
	else 
	{
		q.x = q1.x;
		q.y = q1.y;
		q.z = q1.z;
		q.w = q1.w;
	}
	if(1.0 - cosomega > 1e-6) 
	{
		float omega = acos(cosomega);
		float sinomega = sin(omega);
		k0 = sin((1.0f - t) * omega) / sinomega;
		k1 = sin(t * omega) / sinomega;
	} 
	else 
	{
		k0 = 1.0f - t;
		k1 = t;
	}
	qOut.x = q0.x * k0 + q.x * k1;
	qOut.y = q0.y * k0 + q.y * k1;
	qOut.z = q0.z * k0 + q.z * k1;
	qOut.w = q0.w * k0 + q.w * k1;
}


END_NAMESPACE_XMATHLIB
