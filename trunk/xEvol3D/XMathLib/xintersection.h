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


#ifndef _XMATH_INTERSECTION_H_
#define _XMATH_INTERSECTION_H_
#include "xmathbasedef.h"
#include <vector>

#include "xMathLib.h"
BEGIN_NAMESPACE_XGEOMLIB
class _XEVOL_BASE_API_ xray;
class _XEVOL_BASE_API_ xplane;
class _XEVOL_BASE_API_ xlineseg;
class _XEVOL_BASE_API_ xtriangle;
class _XEVOL_BASE_API_ xplane;
class _XEVOL_BASE_API_ xpolygon;
class _XEVOL_BASE_API_ xCylinder;

_XEVOL_BASE_API_ bool    InterSection(const xMathLib::xvec2* line1 , xMathLib::xvec2* line2);
_XEVOL_BASE_API_ bool    InterSection(xray& ray,xplane& plan, xMathLib::xvec3& point,float & t);
_XEVOL_BASE_API_ bool    InterSection(xray& ray,xtriangle& tri,xMathLib::xvec3& point,float & t);
_XEVOL_BASE_API_ bool    InterSection(xray& ray,xtriangle& tri,xMathLib::xvec3& point,float& t, float& u, float& v );

_XEVOL_BASE_API_ bool    InterSection(xlineseg& lineSeg,xplane& plan,xMathLib::xvec3& point,float & t);
_XEVOL_BASE_API_ bool    InterSection(xlineseg& lineSeg,xtriangle& tri,xMathLib::xvec3& point,float & t);

_XEVOL_BASE_API_ float   InterSection(xray& ray1,xray& ray2,xMathLib::xvec3& point);
_XEVOL_BASE_API_ float   InterSection(xray& ray,xlineseg& lineSeg,xMathLib::xvec3& point);
_XEVOL_BASE_API_ float   InterSection(xlineseg& lineSeg1,xlineseg& lineSeg2,xMathLib::xvec3& point);

_XEVOL_BASE_API_ float   InterSection(xMathLib::xvec3& point ,xtriangle& tri);
_XEVOL_BASE_API_ float   InterSection(xMathLib::xvec3& point ,xpolygon& polygon);

_XEVOL_BASE_API_ float   InterSection(xtriangle& tri,xtriangle& tri2,xlineseg& lineSeg);
_XEVOL_BASE_API_ float   InterSection(xtriangle& tri,xplane& plan,xlineseg& lineSeg);

_XEVOL_BASE_API_ float   InterSection(xtriangle& tri,xtriangle& tri2g);
_XEVOL_BASE_API_ float   InterSection(xtriangle& tri,xplane& plan);

//将一个点投影到三角形
//tri为三角形[v0 , v1 , v2] , _out为投影点，t1为[v1-v0]的系数，t2为[v2-v0]的系数
_XEVOL_BASE_API_ bool    XM_Project(const xMathLib::xvec3& pt , const xtriangle& tri , xMathLib::xvec3& _out , float& t1 , float t2);


//将一个点投影到平面
_XEVOL_BASE_API_ void    XM_Project(const xMathLib::xvec3& pt , xplane& plan , xMathLib::xvec3& _out );


//将一个点投影到平面, 平面由点法向量来确定，
_XEVOL_BASE_API_ void    XM_Project(const xMathLib::xvec3& pt , const xMathLib::xvec3& p0  , const xMathLib::xvec3& n ,  xMathLib::xvec3& _out );

//将一个点投影到平面, 平面由三个点来确定，
_XEVOL_BASE_API_ void    XM_Project(const xMathLib::xvec3& pt , const xMathLib::xvec3 v[3] ,  xMathLib::xvec3& _ret );
END_NAMESPACE_XGEOMLIB
#endif

