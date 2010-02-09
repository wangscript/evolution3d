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
#ifndef _XGEOM_VOLUME_H_
#define _XGEOM_VOLUME_H_

#include  "xMathLib.h"
#include  "xGeomLib.h"

BEGIN_NAMESPACE_XGEOMLIB

class _XEVOL_BASE_API_ xrect;
class _XEVOL_BASE_API_ xshpere;
class _XEVOL_BASE_API_ xobb;
class _XEVOL_BASE_API_ xcylinder;
class _XEVOL_BASE_API_ xaabb;
/*
*/
class  _XEVOL_BASE_API_ xrect
{
	float x;
	float y;
	float h;
	float w;
};

class _XEVOL_BASE_API_  xaabb
{
public:
	xaabb(){};
	xaabb(xMathLib::xvec3& Min,xMathLib::xvec3& Max){ Build(Min,Max); }
	void Build(xMathLib::xvec3& Min,xMathLib::xvec3& Max){  AddPoint(Min) ;  AddPoint(Max) ;};
	void Build(xMathLib::xvec3* points,int n) { for(int  i = 0 ; i < n ; i ++) { AddPoint(points[i]) ; } };
	void AddPoint(xMathLib::xvec3& v)
	{
		if(v.x > m_Max.x) m_Max.x = v.x;
		if(v.y > m_Max.y) m_Max.y = v.y;
		if(v.z > m_Max.z) m_Max.z = v.z;

		if(v.x < m_Min.x) m_Min.x = v.x;
		if(v.y < m_Min.y) m_Min.y = v.y;
		if(v.z < m_Min.z) m_Min.z = v.z;
	}
	void Init(float len = 1000000.0f)
	{
		m_Min.x = len;	m_Max.x = -len;
		m_Min.y = len;	m_Max.y = -len;
		m_Min.z = len;	m_Max.z = -len;
	}

public:
	xMathLib::xvec3 m_Min;
	xMathLib::xvec3 m_Max;
};

/*
Oreint-Bounding Box;
*/
class _XEVOL_BASE_API_  xobb
{
public:
	xMathLib::xvec3    m_Point;
	xMathLib::xvec3    m_Orient;
};




class  _XEVOL_BASE_API_ xshpere
{
public:
	xMathLib::xvec3  m_Orign;
	float            m_R;
};

class _XEVOL_BASE_API_ xcylinder
{
public:
	xMathLib::xvec3 m_StartPoint;
	xMathLib::xvec3 m_EndPoint;
	float           m_R;
};



END_NAMESPACE_XGEOMLIB

#endif
