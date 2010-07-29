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


#ifndef _XMATH_PLANE_H_
#define _XMATH_PLANE_H_
#include "xmathbasedef.h"
#include <vector>

#include "xMathLib.h"


BEGIN_NAMESPACE_XGEOMLIB

class  _XEVOL_BASE_API_ xplane
{
public:
	xplane(){};
	xplane(const xMathLib::xvec3 a, const xMathLib::xvec3 b, const xMathLib::xvec3 c);
	xplane(const xMathLib::xvec3& point, const xMathLib::xvec3& normal);
	xplane(const xMathLib::xvec4& point, const xMathLib::xvec4& normal);
	xplane(const xMathLib::xvec4& v){ A = v.x;B = v.y ;C = v.z;D = v.w;};
	xplane(float _A,float _B,float _C,float _D){A = _A;B = _B ;C = _C;D = _D;}
	void normalize();
    void getPt0(xMathLib::xvec3& _pt);
    void getNormal(xMathLib::xvec3& _nor);
	operator  xMathLib::xvec4(){ return xMathLib::xvec4(A,B,C,D); }
	float operator*(const xMathLib::xvec3& p);

	friend float operator*(const xMathLib::xvec3& v ,const xplane& p);
public:
	float A;
	float B;
	float C;
	float D;
};

float operator*(const xMathLib::xvec3& v ,const xplane& p);

END_NAMESPACE_XGEOMLIB
#endif

