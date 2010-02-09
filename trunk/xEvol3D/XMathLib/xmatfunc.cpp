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


#include "../xStdPch.h"

#include "xMathLib.h"
BEGIN_NAMESPACE_XMATHLIB
//	经过该投影矩阵投影后，投影体为一个范体，
//
//	       x( -1 , 1 )
//		   y( -1 , 1 )
//		   z( -1 , 1 )
//


void	_XEVOL_BASE_API_ XM_Perspective(xmat4& mOut,float fov_angle,float aspect,float Znear,float Zfar)
{
	float dz = Zfar - Znear;

	float yp = XM_CosD(fov_angle / 2 ) / XM_SinD(fov_angle / 2 ) ;
	float xp = yp / aspect;

	float z1 = - (Zfar + Znear) / dz;
	float z2 = - 2*Zfar *  Znear / dz ;

	mOut.m[0][0] = xp ; mOut.m[0][1] = 0  ; mOut.m[0][2] = 0  ; mOut.m[0][3] =  0;
	mOut.m[1][0] = 0  ; mOut.m[1][1] = yp ; mOut.m[1][2] = 0  ; mOut.m[1][3] =  0;
	mOut.m[2][0] = 0  ; mOut.m[2][1] = 0  ; mOut.m[2][2] = z1 ; mOut.m[2][3] = -1;
	mOut.m[3][0] = 0  ; mOut.m[3][1] = 0  ; mOut.m[3][2] = z2 ; mOut.m[3][3] =  0;

	return ;

}

//
//	正交投影
// 　　　经过该投影矩阵后，物体大小不会变形．
//
//	x  = x/w + dx;
//	y  = y/h + dy;
//
//	注意投影后的范体是(-1,1);


void	_XEVOL_BASE_API_ XM_Ortho3D(xmat4& mOut,float Znear ,float Zfar ,float top,float bottom ,float left ,float right)
{

	float cx = (right + left) / 2.0f ;
	float cy = (bottom + top ) / 2.0f ;
	float w = (float)(right - left) / 2.0f ; //float w = (float)fabs(right - left) / 2.0f ;
	float h = (float)(bottom - top) / 2.0f ; //float h = (float)fabs(top - bottom) / 2.0f ;

	float x = 1.0f/w;
	float dx = -cx/w;

	float y = 1.0f/h;
	float dy = cy/h;


	float z = -2.0f/(Zfar - Znear);
	float dz = -((Zfar + Znear)) / (Zfar - Znear);

	mOut.m[0][0] = x  ; mOut.m[0][1] = 0  ; mOut.m[0][2] = 0  ; mOut.m[0][3] = 0;
	mOut.m[1][0] = 0  ; mOut.m[1][1] = y  ; mOut.m[1][2] = 0  ; mOut.m[1][3] = 0;
	mOut.m[2][0] = 0  ; mOut.m[2][1] = 0  ; mOut.m[2][2] = z  ; mOut.m[2][3] = 0;
	mOut.m[3][0] = dx ; mOut.m[3][1] = dy ; mOut.m[3][2] = dz ; mOut.m[3][3] = 1;


	return ;
}

void	_XEVOL_BASE_API_ XM_Transform(xmat4& mOut,float tx,float ty,float tz)
{
	mOut.m[0][0] = 1  ; mOut.m[0][1] = 0  ; mOut.m[0][2] = 0  ; mOut.m[0][3] = 0;
	mOut.m[1][0] = 0  ; mOut.m[1][1] = 1  ; mOut.m[1][2] = 0  ; mOut.m[1][3] = 0;
	mOut.m[2][0] = 0  ; mOut.m[2][1] = 0  ; mOut.m[2][2] = 1  ; mOut.m[2][3] = 0;
	mOut.m[3][0] = tx ; mOut.m[3][1] = ty ; mOut.m[3][2] = tz ; mOut.m[3][3] = 1;
	return ;
}

void	_XEVOL_BASE_API_ XM_Transform(xmat3& mOut,float tx,float ty)
{
	mOut.m[0][0] = 1  ; mOut.m[0][1] = 0  ; mOut.m[0][2] = 0  ; 
	mOut.m[1][0] = 0  ; mOut.m[1][1] = 1  ; mOut.m[1][2] = 0  ; 
	mOut.m[2][0] = tx ; mOut.m[2][1] = ty ; mOut.m[2][2] = 1  ; 
	return ;
}

void	_XEVOL_BASE_API_ XM_Scale(xmat4& mOut,float sx,float sy,float sz)
{
	mOut.m[0][0] = sx  ; mOut.m[0][1] = 0  ; mOut.m[0][2] = 0  ; mOut.m[0][3] = 0;
	mOut.m[1][0] = 0  ; mOut.m[1][1] = sy  ; mOut.m[1][2] = 0  ; mOut.m[1][3] = 0;
	mOut.m[2][0] = 0  ; mOut.m[2][1] = 0   ; mOut.m[2][2] = sz ; mOut.m[2][3] = 0;
	mOut.m[3][0] = 0  ; mOut.m[3][1] = 0   ; mOut.m[3][2] = 0  ; mOut.m[3][3] = 1;
	return ;
}

void   _XEVOL_BASE_API_ XM_XForm_Trans(xmat4& mOut,xmat4& mRot, xvec3& vCenter)
{
     xmat4 xTrans , xTransInv;
	 XM_Transform(xTrans    , vCenter.x , vCenter.y ,vCenter.z);
	 XM_Transform(xTransInv , -vCenter.x , -vCenter.y ,-vCenter.z);
	 mOut = xTransInv * mRot * xTrans;
}

void	_XEVOL_BASE_API_ XM_RotateX(xmat4& mOut,float angle)
{
	float c = XM_CosD(angle);
	float s = XM_SinD(angle);
	mOut.m[0][0] = 1  ; mOut.m[0][1] = 0  ; mOut.m[0][2] = 0  ; mOut.m[0][3] = 0;
	mOut.m[1][0] = 0  ; mOut.m[1][1] = c  ; mOut.m[1][2] = s  ; mOut.m[1][3] = 0;
	mOut.m[2][0] = 0  ; mOut.m[2][1] = -s ; mOut.m[2][2] = c  ; mOut.m[2][3] = 0;
	mOut.m[3][0] = 0  ; mOut.m[3][1] = 0  ; mOut.m[3][2] = 0  ; mOut.m[3][3] = 1;
	return ;
}


void	_XEVOL_BASE_API_ XM_RotateY(xmat4& mOut,float angle)
{
	float c = XM_CosD(angle);
	float s = XM_SinD(angle);
	mOut.m[0][0] = c  ; mOut.m[0][1] = 0  ; mOut.m[0][2] = -s  ; mOut.m[0][3] = 0;
	mOut.m[1][0] = 0  ; mOut.m[1][1] = 1  ; mOut.m[1][2] =  0  ; mOut.m[1][3] = 0;
	mOut.m[2][0] = s  ; mOut.m[2][1] = 0  ; mOut.m[2][2] =  c  ; mOut.m[2][3] = 0;
	mOut.m[3][0] = 0  ; mOut.m[3][1] = 0  ; mOut.m[3][2] =  0  ; mOut.m[3][3] = 1;
	return ;
}

void	_XEVOL_BASE_API_ XM_RotateZ(xmat4& mOut,float angle)
{
	float c = XM_CosD(angle);
	float s = XM_SinD(angle);
	mOut.m[0][0] = c  ; mOut.m[0][1] = s  ; mOut.m[0][2] = 0  ; mOut.m[0][3] = 0;
	mOut.m[1][0] = -s ; mOut.m[1][1] = c  ; mOut.m[1][2] = 0  ; mOut.m[1][3] = 0;
	mOut.m[2][0] = 0  ; mOut.m[2][1] = 0  ; mOut.m[2][2] = 1  ; mOut.m[2][3] = 0;
	mOut.m[3][0] = 0  ; mOut.m[3][1] = 0  ; mOut.m[3][2] = 0  ; mOut.m[3][3] = 1;
	return ;

}


/*******************************************************************************
This function is tested by Stanly Lee at 2003-11

*******************************************************************************/
void _XEVOL_BASE_API_ XM_RotatePV(xmat4& mOut,const xvec4& v , float angle)
{
	float vsin = XM_SinD(angle);
	float vcos = XM_CosD(angle);

	float a = v.x * vsin;
	float b = v.y * vsin;
	float c = v.z * vsin;

	mOut.m[0][0] = vcos  ; mOut.m[0][1] = c     ; mOut.m[0][2] = -b    ; mOut.m[0][3] = 0;
	mOut.m[1][0] = -c    ; mOut.m[1][1] = vcos  ; mOut.m[1][2] = a     ; mOut.m[1][3] = 0;
	mOut.m[2][0] = b     ; mOut.m[2][1] = -a    ; mOut.m[2][2] = vcos  ; mOut.m[2][3] = 0;
	mOut.m[3][0] = 0     ; mOut.m[3][1] = 0     ; mOut.m[3][2] = 0     ; mOut.m[3][3] = 1;
	return ;

}


/*******************************************************************************
This function is tested by Stanly Lee at 2003-11
*******************************************************************************/
void _XEVOL_BASE_API_ XM_Rotate(xmat4& mOut,const xvec4& v , float angle)
{
	//This code and algrithm is copy from the mesaLib4.0
	float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c, s, c;


	float x = v.x;
	float y = v.y;
	float z = v.z;

	s = (float) XM_SinD( angle  );
	c = (float) XM_CosD( angle  );

	mOut.identity();

#define M(row,col)  mOut.m [row ] [ col ]

	float mag = (float) xsqrt(x * x + y * y + z * z);

	if (mag <= 1.0e-4)
	{
		/* no rotation, leave mat as-is */
		return;
	}

	x /= mag;
	y /= mag;
	z /= mag;


	xx = x * x;		yy = y * y;
	zz = z * z;		xy = x * y;
	yz = y * z;		zx = z * x;
	xs = x * s;		ys = y * s;
	zs = z * s;		one_c = 1.0F - c;

	/* We already hold the identity-matrix so we can skip some statements */
	M(0,0) = (one_c * xx) + c;
	M(1,0) = (one_c * xy) - zs;
	M(2,0) = (one_c * zx) + ys;
	/*  M(3,0) = 0.0F; */

	M(0,1) = (one_c * xy) + zs;
	M(1,1) = (one_c * yy) + c;
	M(2,1) = (one_c * yz) - xs;
	/*  M(3,1) = 0.0F; */

	M(0,2) = (one_c * zx) - ys;
	M(1,2) = (one_c * yz) + xs;
	M(2,2) = (one_c * zz) + c;
	/*  M(3,2) = 0.0F; */

	/*
	M(0,3) = 0.0F;
	M(1,3) = 0.0F;
	M(2,3) = 0.0F;
	M(3,3) = 1.0F;
	*/
#undef M

}

/*******************************************************************************
This function is tested by Stanly Lee at 2003-11
*******************************************************************************/
void	_XEVOL_BASE_API_ XM_LookAt(xmat4& mOut,const xvec4& eyeAt,const xvec4& eyeTarget,const xvec4& upDir)
{
	xvec4 vZ =   eyeAt - eyeTarget;//注意这里是因为OpenGL用的you手系．默认的方向是向着Z的负方向．
	xvec4 vX =   upDir.cp(vZ);

	vX.normalize();
	vZ.normalize();
	xvec4 vY =   vZ.cp(vX);//重新计算向上的方向．我们可以保证vY也是个单位向量，就不需要再做单位化了。

#define M(row,col)  mOut.m[row ] [ col ]

	M(0,0) = vX.x;
	M(1,0) = vX.y;
	M(2,0) = vX.z;
	M(3,0) = - vX.dp3(eyeAt) ;


	M(0,1) =  vY.x;
	M(1,1) =  vY.y;
	M(2,1) =  vY.z;
	M(3,1) =  -vY.dp3(eyeAt) ;

	M(0,2) =  vZ.x;
	M(1,2) =  vZ.y;
	M(2,2) =  vZ.z;
	M(3,2) =  -vZ.dp3(eyeAt) ;

	M(0,3) = 0;
	M(1,3) = 0;
	M(2,3) = 0;
	M(3,3) = 1;
#undef M
}
void    _XEVOL_BASE_API_ XM_Decomp_NoTrans(xmat4& mrt)
{
     mrt.m[3][0] = mrt.m[3][1] = mrt.m[3][2] = 0.0f;
}

void    _XEVOL_BASE_API_ XM_Decomp_NoScale(xmat4& mrt)
{
	xvec3 v1(mrt.m[0][0] , mrt.m[1][0] , mrt.m[2][0]);
	xvec3 v2(mrt.m[0][1] , mrt.m[1][1] , mrt.m[2][1]);
	xvec3 v3(mrt.m[0][2] , mrt.m[1][2] , mrt.m[2][2]);
	float sx = v1.len();
	float sy = v2.len();
	float sz = v3.len();
	mrt.m[0][0] /= sx ; mrt.m[1][0]  /= sy  ;   mrt.m[2][0] /= sz ;
	mrt.m[0][1] /= sx ; mrt.m[1][1]  /= sy  ;   mrt.m[2][1] /= sz ;
	mrt.m[0][2] /= sx ; mrt.m[1][2]  /= sy  ;   mrt.m[2][2] /= sz ;
}

//分解出旋转和缩放
void    _XEVOL_BASE_API_ XM_Decomp_ScaleRotate(xmat4& mrt , xmat4& rot , xvec4& scale)
{
    xvec3 v1(mrt.m[0][0] , mrt.m[1][0] , mrt.m[2][0]);
	xvec3 v2(mrt.m[0][1] , mrt.m[1][1] , mrt.m[2][1]);
	xvec3 v3(mrt.m[0][2] , mrt.m[1][2] , mrt.m[2][2]);

	float sx = scale.x = v1.len();
	float sy = scale.y = v2.len();
	float sz = scale.z = v3.len();
	scale.w = 1.0f;

	rot.identity();
    rot.m[0][0] = mrt.m[0][0] / sx ; rot.m[1][0] = mrt.m[1][0]  / sy  ,  rot.m[2][0]= mrt.m[2][0] / sz ;
    rot.m[0][1] = mrt.m[0][1] / sx ; rot.m[1][1] = mrt.m[1][1]  / sy  ,  rot.m[2][1]= mrt.m[2][1] / sz ;
    rot.m[0][2] = mrt.m[0][2] / sx ; rot.m[1][2] = mrt.m[1][2]  / sy  ,  rot.m[2][2]= mrt.m[2][2] / sz ;
}

void    _XEVOL_BASE_API_ XM_Decomp_XForm_Center(xmat4& mrt , xvec4& center)
{
	xmat4 rot ;
	rot.identity();
	rot.m[0][0] = 1.0f - mrt.m[0][0]  ; rot.m[1][0] = 0.0f - mrt.m[1][0] ,  rot.m[2][0]= 0.0f - mrt.m[2][0];
	rot.m[0][1] = 0.0f - mrt.m[0][1]  ; rot.m[1][1] = 1.0f - mrt.m[1][1] ,  rot.m[2][1]= 0.0f - mrt.m[2][1];
	rot.m[0][2] = 0.0f - mrt.m[0][2]  ; rot.m[1][2] = 0.0f - mrt.m[1][2] ,  rot.m[2][2]= 1.0f - mrt.m[2][2];

	xvec4 v(mrt.m[3][0] , mrt.m[3][1] , mrt.m[3][2] , 1.0f);
	xmat4 invRot;
	XM_Inv(rot , invRot);
	center = v * invRot;
}

END_NAMESPACE_XMATHLIB

