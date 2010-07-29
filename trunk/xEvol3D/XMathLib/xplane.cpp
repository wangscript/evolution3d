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
#include "xGeomLib.h"
using namespace XEvol3D::XMathLib;

BEGIN_NAMESPACE_XGEOMLIB
xplane::xplane(const xvec3 a, const xvec3 b, const xvec3 c)
{
	xvec3 v1 = c - a;
	xvec3 v2 = b - a;
	//Right hand; so v2 X v1
	xvec3 n = v2.cp(v1);
	if( !( numeric_equal(n.x , 0) && numeric_equal(n.y , 0) && numeric_equal(n.z , 0) ) )
	{
		n.normalize();
		A = n.x;
		B = n.y;
		C = n.z;
		D = -n.dp(a);
	}
	else
	{
		//Null plan
		A = 0;
		B = 0;
		C = 0;
		D = 0;
	}
}

xplane::xplane(const xvec3& point, const xvec3& _n)
{
    xvec3 n = _n;
	if( !(n.x == 0 && n.y == 0 && n.z == 0) )
	{
		n.normalize();
		A = n.x;
		B = n.y;
		C = n.z;
		D = -n.dp(point);
	}
	else
	{
		//Null plan
		A = 0;
		B = 0;
		C = 0;
		D = 0;
	}
}

xplane::xplane(const xvec4& point, const xvec4& _n)
{
    xvec4 n = _n;
    if( !(n.x == 0 && n.y == 0 && n.z == 0) )
    {
        n.normalize();
        A = n.x;
        B = n.y;
        C = n.z;
        D = -n.dp3(point);
    }
    else
    {
        //Null plan
        A = 0;
        B = 0;
        C = 0;
        D = 0;
    }
}

void xplane::normalize()
{
	xvec3* pNormal = (xvec3*)&A;
    float len = pNormal->len();
	if( xMathLib::numeric_equal(len , 0) )
		return ;
	//if(D > 0) len = - len;
	A /= len;
	B /= len;
	C /= len;
	D /= len;
}

void xplane::getPt0(xMathLib::xvec3& _pt)
{
    xvec3* pNormal = (xvec3*)&A;
    float len2 = A * A + B * B + C * C;
    if( xMathLib::numeric_equal(len2 , 0) )
        return ;

     float _e = -D / len2; 
     _pt.x = A * _e;
     _pt.y = B * _e;
     _pt.z = C * _e;
}

void  xplane::getNormal(xMathLib::xvec3& _nor)
{
     _nor.x = A;
     _nor.y = B;
     _nor.z = C;
     _nor.normalize();
}

float xplane::operator*(const xvec3& p)
{
	return (p.x * A + p.y * B + p.z * C + D);
}
float operator*(const xvec3& v ,const xplane& p)
{
	return v.x * p.A  + v.y * p.B + v.z * p.C + p.D;
}
END_NAMESPACE_XGEOMLIB

