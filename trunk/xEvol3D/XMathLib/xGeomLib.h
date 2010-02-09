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


#ifndef _XGEOM_LIB_H_
#define _XGEOM_LIB_H_
#include "xmathbasedef.h"
#include <vector>

#include "xMathLib.h"


BEGIN_NAMESPACE_XGEOMLIB

class xline;
class xlineseg;
class xplane;
class xtriangle;
class xpolygon;
class xCylinder;


class _XEVOL_BASE_API_ xline
{
public:	
	xline(xMathLib::xvec3 point,xMathLib::xvec3 dir){m_Point = point;m_Dir = dir;}

public:
	xMathLib::xvec3     m_Point;
	xMathLib::xvec3     m_Dir;
};


class  _XEVOL_BASE_API_ xlineseg
{
public:
	xlineseg(xMathLib::xvec3 start,xMathLib::xvec3 end){m_Start = start;m_End = end;};
	xlineseg(xMathLib::xvec3 start,xMathLib::xvec3 dir,float len);

public:
	xMathLib::xvec3 m_Start;
	xMathLib::xvec3 m_End;
};


class  _XEVOL_BASE_API_ xplane
{
public:
	xplane(){};
	xplane(xMathLib::xvec3 a,xMathLib::xvec3 b,xMathLib::xvec3 c);
	xplane(xMathLib::xvec3 point,xMathLib::xvec3 normal);
	xplane(xMathLib::xvec4 v){ A = v.x;B = v.y ;C = v.z;D = v.w;};
	xplane(float _A,float _B,float _C,float _D){A = _A;B = _B ;C = _C;D = _D;}
    void normalize();
	operator  xMathLib::xvec4(){ return xMathLib::xvec4(A,B,C,D); }
	float operator*(const xMathLib::xvec3& p);

	friend float operator*(const xMathLib::xvec3& v ,const xplane& p);
public:
	float A;
	float B;
	float C;
	float D;
};


class  _XEVOL_BASE_API_ xtriangle
{
public:
	void normal(xMathLib::xvec3& n)
	{
		xMathLib::xvec3 a = m_points[2] - m_points[0];
		xMathLib::xvec3 b = m_points[1] - m_points[0];
		n =  b.cp(a);
	}
public:
	xMathLib::xvec3 m_points[3];
};

class  _XEVOL_BASE_API_ xpolygon
{
public:
	std::vector<xMathLib::xvec3 , dllsafe_alloc<xMathLib::xvec3> > m_points;
};

class  _XEVOL_BASE_API_ xray
{
public:
	xMathLib::xvec3     m_Point;
	xMathLib::xvec3     m_Dir;      
};



float operator*(const xMathLib::xvec3& v ,const xplane& p);

namespace XInterSection
{

	bool    InterSection(xray& ray,xplane& plan, xMathLib::xvec3& point,float & t);
	bool    InterSection(xray& ray,xtriangle& tri,xMathLib::xvec3& point,float & t);
	bool    InterSection(xray& ray,xtriangle& tri,xMathLib::xvec3& point,float& t, float& u, float& v );

	bool    InterSection(xlineseg& lineSeg,xplane& plan,xMathLib::xvec3& point,float & t);
	bool    InterSection(xlineseg& lineSeg,xtriangle& tri,xMathLib::xvec3& point,float & t);

	float   InterSection(xray& ray1,xray& ray2,xMathLib::xvec3& point);
	float   InterSection(xray& ray,xlineseg& lineSeg,xMathLib::xvec3& point);
	float   InterSection(xlineseg& lineSeg1,xlineseg& lineSeg2,xMathLib::xvec3& point);

	float   InterSection(xMathLib::xvec3& point ,xtriangle& tri);
	float   InterSection(xMathLib::xvec3& point ,xpolygon& polygon);

	float   InterSection(xtriangle& tri,xtriangle& tri2,xlineseg& lineSeg);
	float   InterSection(xtriangle& tri,xplane& plan,xlineseg& lineSeg);

	float   InterSection(xtriangle& tri,xtriangle& tri2g);
	float   InterSection(xtriangle& tri,xplane& plan);
};

namespace Helper
{
	__inline bool IsSameDir(xMathLib::xvec3& v1,xMathLib::xvec3& v2)
	{
		float t = v1.x * v2.x;
		if( xMathLib::numeric_equal(t , 0) )
		{
			if(v1.x !=0 || v2.x !=0)
				return false;
		}
		if( t < 0)
			return false;

		t = v1.y * v2.y;
		if( xMathLib::numeric_equal( t ,  0) )
		{
			if(v1.y !=0 || v2.y !=0)
				return false;
		}
		if( t < 0)
			return false;

		t = v1.z * v2.z;
		if( xMathLib::numeric_equal( t , 0) )
		{
			if(v1.z !=0 || v2.z !=0)
				return false;
		}
		if( t < 0)
			return false;

		return true;
	}
}

END_NAMESPACE_XGEOMLIB
namespace xGeomLib = XEvol3D::XGeomLib ;
#include "xvolume.h"
#endif

