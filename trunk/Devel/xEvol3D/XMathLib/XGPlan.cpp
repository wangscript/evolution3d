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

#include "XGeomLib.h"
BEGIN_NAMESPACE_XGEOMLIB
xplane::xplane(xvec3 a, xvec3 b,xvec3 c)
{
	xvec3 v1 = c - a;
	xvec3 v2 = b - a;
	//Right hand; so v2 X v1
	xvec3 n = v2.cp(v1);
	if( !(n.x == 0 && n.y == 0 && n.z == 0) )
	{
		n.normalize();
		A = n.x;
		B = n.y;
		C = n.z;
		D = n.dp(a);
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

xplane::xplane(xvec3 point,xvec3 n)
{
	if( !(n.x == 0 && n.y == 0 && n.z == 0) )
	{
		n.normalize();
		A = n.x;
		B = n.y;
		C = n.z;
		D = n.dp(point);
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


float xplane::operator*(const xvec3& p)
{
	return (p.x * A + p.y * B + p.z * C +  D);
}
float operator*(const xvec3& v ,const xplane& p)
{
	return v.x * p.A  + v.y * p.B + v.z * p.C + p.D;
}
END_NAMESPACE_XGEOMLIB

