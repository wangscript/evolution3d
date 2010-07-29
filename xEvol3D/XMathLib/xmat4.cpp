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
/***
Matrix transform 3x3 for Matrix4x4
***/
void xmat4::conveter(xmat3& _m)
{
	_m.m[0][0] =  m[0][0]; _m.m[0][0] =  m[0][0]; _m.m[0][0] =  m[0][0];
	_m.m[1][1] =  m[1][1]; _m.m[1][1] =  m[1][1]; _m.m[1][1] =  m[1][1];
	_m.m[2][2] =  m[2][2]; _m.m[2][2] =  m[2][2]; _m.m[2][2] =  m[2][2];
}
void  xmat4::toQuat(xquat& _quat)
{
	float tr, s, q[4];
	int i, j, k;

	int nxt[3] = {1, 2, 0 };
	//计算矩阵轨迹
	tr = m[0][0] + m[1][1] + m[2][2];

	//检查矩阵轨迹是正还是负
	if(tr>0.0)
	{
		s = xsqrt(tr + 1.0f);
		_quat.w = s / 2.0f;
		s = 0.5f / s;
		_quat.x = (m[1][2] - m[2][1]) * s;
		_quat.y = (m[2][0] - m[0][2]) * s;
		_quat.z = (m[0][1] - m[1][0]) * s;
	}
	else
	{
		//轨迹是负
		i = 0;
		if(m[1][1]>m[0][0]) i = 1;
		if(m[2][2]>m[i][i]) i = 2;
		j = nxt[i];
		k = nxt[j];

		s = xsqrt((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);
		q[i] = s * 0.5f;
		if( s!= 0.0f) s = 0.5f / s;
		q[3] = (m[j][k] - m[k][j]) * s;
		q[j] = (m[i][j] - m[j][i]) * s;
		q[k] = (m[i][k] - m[k][i]) * s;
		_quat.x = q[0];
		_quat.y = q[1];
		_quat.z = q[2];
		_quat.w = q[3];
	}
}
void  xmat4::transform_l_3x3(const xvec3& v,xvec3& vOut)
{
	vOut.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
	vOut.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
	vOut.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];
}

void  xmat4::transform_l_3x3(const xvec4& v,xvec3& vOut)
{
	vOut.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
	vOut.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
	vOut.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];
}

void  xmat4::transform_l_3x3(const xvec3& v,xvec4& vOut)
{
	vOut.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
	vOut.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
	vOut.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];
}

void  xmat4::transform_l_3x3(const xvec4& v,xvec4& vOut)
{
	vOut.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0];
	vOut.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1];
	vOut.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2];
}


void  xmat4::transform_r_3x3(const xvec3& v,xvec3& vOut)
{
	vOut.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z ;
	vOut.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z ;
	vOut.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z ;

}

void  xmat4::transform_r_3x3(const xvec4& v,xvec3& vOut)
{
	vOut.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z ;
	vOut.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z ;
	vOut.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z ;
}

void  xmat4::transform_r_3x3(const xvec3& v,xvec4& vOut)
{
	vOut.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z ;
	vOut.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z ;
	vOut.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z ;

}

void  xmat4::transform_r_3x3(const xvec4& v,xvec4& vOut)
{
	vOut.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z ;
	vOut.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z ;
	vOut.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z ;

}
END_NAMESPACE_XMATHLIB


//不可以用汇编优化
BEGIN_NAMESPACE_XMATHLIB
/*
Matrix converter
*/
void  xmat4::from(const xmat3& _m)
{
	identity();
	m[0][0]= _m.m[0][0];    m[0][1]=_m.m[0][1];    m[0][2]=_m.m[0][2];
	m[1][0]= _m.m[1][0];    m[1][1]=_m.m[1][1];    m[1][2]=_m.m[1][2];
	m[2][0]= _m.m[2][0];    m[2][1]=_m.m[2][1];    m[2][2]=_m.m[2][2];
}

void  xmat4::from(const xmat4x3& _m)
{
	identity();
	m[0][0]= _m.m[0][0];    m[0][1]=_m.m[0][1];    m[0][2]=_m.m[0][2];
	m[1][0]= _m.m[1][0];    m[1][1]=_m.m[1][1];    m[1][2]=_m.m[1][2];
	m[2][0]= _m.m[2][0];    m[2][1]=_m.m[2][1];    m[2][2]=_m.m[2][2];
	m[3][0]= _m.m[3][0];    m[3][1]=_m.m[3][1];    m[3][2]=_m.m[3][2];
}

bool  xmat4::inverse()
{
	xmat4 mat = *this;
	return XM_Inv(mat,*this);
}


//逆矩阵，采用高斯消去法．
/*******************************************************************************
This function is tested by Stanly Lee at 2003-11
*******************************************************************************/
bool _XEVOL_BASE_API_ XM_Inv(const xmat4&m1 , xmat4& mOut)
{
	xmat4 m = m1;
	int max_r;
	//int max_c;
	float pivot = 1.0;
	mOut.identity();
	for(int row = 0 ; row < 4 ; row++ )
	{
		//选主元．
		pivot = m.m[row][row];

		max_r = row;
		for( int i = row ; i < 4 ; i++ )
		{
			float t = (float)fabs(m.m[i][row]);

			if( t > fabs(pivot))
			{
				pivot = m.m[i][row];
				max_r = i;
			}
		}
		//主元选定．
		if(numeric_equal(pivot , 0) )
			return false;
		//交换行
		if(max_r != row)
		{
			XM_Swap(m.m[max_r][0],m.m[row][0]); 	XM_Swap(mOut.m[max_r][0],mOut.m[row][0]);
			XM_Swap(m.m[max_r][1],m.m[row][1]); 	XM_Swap(mOut.m[max_r][1],mOut.m[row][1]);
			XM_Swap(m.m[max_r][2],m.m[row][2]); 	XM_Swap(mOut.m[max_r][2],mOut.m[row][2]);
			XM_Swap(m.m[max_r][3],m.m[row][3]);  	XM_Swap(mOut.m[max_r][3],mOut.m[row][3]);
		}

		//归一化到1

		for( int x = 0 ; x < 4 ; x ++)
		{
			mOut.m[row][x] /= pivot;
			m.m[row][x] /= pivot;
		}

		//消去当前列，除了当前行上的以外。该列全部成为0
		for(int y = 0 ; y < 4 ; y ++)
		{
			if(y == row )
				continue;
			float times = m.m[y][row];
			//消去。
			for (int x = 0 ; x < 4 ; x ++)
			{
				m.m[y][x] -= m.m[row][x] * times;
				mOut.m[y][x] -= mOut.m[row][x] * times;
			}
		}

	}

	return true;
}
END_NAMESPACE_XMATHLIB
