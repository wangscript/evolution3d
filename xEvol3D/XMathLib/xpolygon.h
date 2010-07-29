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


#ifndef _XMATH_POLYGON_H_
#define _XMATH_POLYGON_H_
#include "xmathbasedef.h"
#include <vector>
#include "xMathLib.h"

BEGIN_NAMESPACE_XGEOMLIB


class  _XEVOL_BASE_API_ xtriangle
{
public:
	xtriangle()
	{

	}
	xtriangle(const xMathLib::xvec3& v1 , const xMathLib::xvec3& v2 , const xMathLib::xvec3& v3)
	{
		m_points[0] = v1;
		m_points[1] = v2;
		m_points[2] = v3;
	}
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

class  _XEVOL_BASE_API_ xpolygon2
{
public:
	std::vector<xMathLib::xvec2 , dllsafe_alloc<xMathLib::xvec2> > m_points;
};


_XEVOL_BASE_API_ bool   XM_ToTriangles(xMathLib::xvec2* ptList , int nPt , ds_vector(xMathLib::xvec3i)& _ret);
_XEVOL_BASE_API_ bool   XM_Tesselation(const XGeomLib::xtriangle* ptList , int nTriangle , int iLevel , ds_vector(XGeomLib::xtriangle)& _ret);
_XEVOL_BASE_API_ bool   XM_Tesselation(const XGeomLib::xtriangle& tri , int iLevel , ds_vector(XGeomLib::xtriangle)& _ret);

//检测顶点是否位于一个不规则多边形内部
_XEVOL_BASE_API_ bool   XM_Inside(const xMathLib::xvec2* ptList , int nPt , const xMathLib::xvec2& P);



END_NAMESPACE_XGEOMLIB
#endif

