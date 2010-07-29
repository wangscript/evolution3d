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

#ifndef __XMATH_LIB_DECLARTION_H__
#define __XMATH_LIB_DECLARTION_H__


#include "xmathbasedef.h"

#include <math.h>
#include <memory.h>
BEGIN_NAMESPACE_XMATHLIB


class xmat4;
class xmat4x3;
class xmat3;

EXPIMP_TEMPLATE template <typename _Type> class _XEVOL_BASE_API_ TVec2;
EXPIMP_TEMPLATE template <typename _Type> class _XEVOL_BASE_API_ TVec3;
EXPIMP_TEMPLATE template <typename _Type> class _XEVOL_BASE_API_ TVec4;

typedef _XEVOL_BASE_API_ TVec2<double>        xvec2r;
typedef _XEVOL_BASE_API_ TVec2<float>         xvec2;
typedef _XEVOL_BASE_API_ TVec2<unsigned int>  xvec2ui;
typedef _XEVOL_BASE_API_ TVec2<int>           xvec2i;

typedef _XEVOL_BASE_API_ TVec3<double>        xvec3r;
typedef _XEVOL_BASE_API_ TVec3<float>         xvec3;
typedef _XEVOL_BASE_API_ TVec3<unsigned int>  xvec3ui;
typedef _XEVOL_BASE_API_ TVec3<int>           xvec3i;
								 
typedef _XEVOL_BASE_API_ TVec4<double>        xvec4r;
typedef _XEVOL_BASE_API_ TVec4<float>         xvec4;
typedef _XEVOL_BASE_API_ TVec4<unsigned int>  xvec4ui;
typedef _XEVOL_BASE_API_ TVec4<int>           xvec4i;

typedef xvec4  float4;
typedef xvec4i int4;
typedef xmat4  float4x4;

typedef xvec3  float3;
typedef xvec3i int3;
typedef xmat3  float3x3;

typedef xvec2  float2;
typedef xvec2i int2;


class xquat;

const double XM_PI = 3.141592653589;

//xquat functions
void   _XEVOL_BASE_API_  XM_Mul(const xquat& qA,const xquat& qB,xquat& qOut);
void   _XEVOL_BASE_API_  XM_Rotate(const xquat& q,const xvec4& v,xvec4& vOut);
void   _XEVOL_BASE_API_  XM_Add(const xquat& qA,const xquat& qB,xquat& qOut);
void   _XEVOL_BASE_API_  XM_Inverse(const xquat& qA ,xquat& qOut);
void   _XEVOL_BASE_API_  XM_Slerp(float t,const xquat& q1,const xquat& q2,xquat& qOut);

//Matrix function
bool	_XEVOL_BASE_API_ XM_Inv(const xmat4x3& m1 , xmat4x3& mOut);
bool	_XEVOL_BASE_API_ XM_Inv(const xmat3& m1 , xmat3& mOut);
bool    _XEVOL_BASE_API_ XM_Inv(const xmat4&m1 , xmat4& mOut);
void    _XEVOL_BASE_API_ XM_Transpos(const xmat4&m , xmat4& mOut);
void	_XEVOL_BASE_API_ XM_Perspective(xmat4& mOut,float fov_angle,float aspect,float Znear,float Zfar);
void	_XEVOL_BASE_API_ XM_Ortho3D(xmat4& mOut,float Znear ,float Zfar ,float top,float bottom ,float left ,float right);

void	_XEVOL_BASE_API_ XM_Transform(xmat4& mOut,float tx,float ty,float tz);
void	_XEVOL_BASE_API_ XM_Transform(xmat3& mOut,float tx,float ty);

void	_XEVOL_BASE_API_ XM_Scale(xmat4& mOut,float sx,float sy,float sz);
void    _XEVOL_BASE_API_ XM_XForm_Trans(xmat4& mOut,xmat4& mRot, xvec3& vCenter);
void	_XEVOL_BASE_API_ XM_RotateX(xmat4& mOut,float angle);
void	_XEVOL_BASE_API_ XM_RotateY(xmat4& mOut,float angle);
void	_XEVOL_BASE_API_ XM_RotateZ(xmat4& mOut,float angle);
void    _XEVOL_BASE_API_ XM_RotatePV(xmat4& mOut,const xvec4& v , float angle);
void    _XEVOL_BASE_API_ XM_Rotate(xmat4& mOut,const xvec4& v , float angle);
void	_XEVOL_BASE_API_ XM_LookAt(xmat4& mOut,const xvec4& eyeAt,const xvec4& eyeTarget,const xvec4& upDir);
void    _XEVOL_BASE_API_ XM_Decomp_ScaleRotate(xmat4& mrt , xmat4& rot , xvec4& scale);
void    _XEVOL_BASE_API_ XM_Decomp_XForm_Center(xmat4& mrt , xvec4& center);
void    _XEVOL_BASE_API_ XM_Decomp_NoTrans(xmat4& mrt);
void    _XEVOL_BASE_API_ XM_Decomp_NoScale(xmat4& mrt);
END_NAMESPACE_XMATHLIB

#endif

