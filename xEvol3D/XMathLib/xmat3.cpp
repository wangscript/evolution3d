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

void xmat3::from(const xmat4& _m)
{
	for( int r = 0 ; r< 3; r++)
		for(int c = 0 ; c < 3 ; c++)
		{
			m[r][c] = _m.m[r][c];
		}
}

void  xmat3::from(const xmat4x3& _m)
{
	for( int r = 0 ; r< 3; r++)
		for(int c = 0 ; c < 3 ; c++)
		{
			m[r][c] = _m.m[r][c];
		}
}

bool  xmat3::inverse()
{
	xmat3 mat = *this;
	return XM_Inv(mat,*this); 
}


bool	_XEVOL_BASE_API_ XM_Inv(const xmat3& m1 , xmat3& mOut)
{
	xmat3 m = m1;
	int max_r;
	//int max_c;
	float pivot = 1.0;
	mOut.identity();
	for(int row = 0 ; row < 3 ; row++ )
	{
		//选主元．
		pivot = m.m[row][row];

		max_r = row;
		for( int i = row ; i < 3 ; i++ )
		{
			float t = (float)fabs(m.m[i][row]);

			if( t > fabs(pivot))
			{
				pivot = m.m[i][row];
				max_r = i;
			}
		}
		//主元选定．
		if( numeric_equal(pivot,0)  )
			return false;
		//交换行
		if(max_r != row)
		{
			XM_Swap(m.m[max_r][0],m.m[row][0]); 	XM_Swap(mOut.m[max_r][0],mOut.m[row][0]);
			XM_Swap(m.m[max_r][1],m.m[row][1]); 	XM_Swap(mOut.m[max_r][1],mOut.m[row][1]);
			XM_Swap(m.m[max_r][2],m.m[row][2]); 	XM_Swap(mOut.m[max_r][2],mOut.m[row][2]);
		}

		//归一化到1

		for( int x = 0 ; x < 3 ; x ++)
		{
			mOut.m[row][x] /= pivot;
			m.m[row][x] /= pivot;
		}

		//消去当前列，除了当前行上的以外。该列全部成为0
		for(int y = 0 ; y < 3 ; y ++)
		{
			if(y == row )
				continue;
			float times = m.m[y][row];
			//消去。
			for (int x = 0 ; x < 3 ; x ++)
			{
				m.m[y][x] -= m.m[row][x] * times;
				mOut.m[y][x] -= mOut.m[row][x] * times;
			}
		}

	}
	return true;
}
END_NAMESPACE_XMATHLIB
