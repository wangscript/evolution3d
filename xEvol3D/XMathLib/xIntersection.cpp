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
#include "xGeomLib.h"
#include "xvolume.h"
#include "xfrustum.h"
using namespace XEvol3D::XMathLib;


BEGIN_NAMESPACE_XGEOMLIB



bool InterSection(const xMathLib::xvec2* line1 , xMathLib::xvec2* line2)
{
	{
		xvec2 v1 = line2[0] - line1[0];
		xvec2 v2 = line2[1] - line1[0];
		xvec2 v  = line1[1] - line1[0];
		float _v1 = v.cp(v1);
		float _v2 = v.cp(v2);

		if( _v2 * _v1 > 0 ) //同侧
			return false;
	}

	//换一个位置        
	{
		xvec2 v1 = line1[0] - line2[0];
		xvec2 v2 = line1[1] - line2[0];
		xvec2 v  = line2[1] - line2[0];
		float _v1 = v.cp(v1);
		float _v2 = v.cp(v2);

		if( _v2 * _v1 > 0 ) //同侧
			return false;
	}
	return true;

}

bool InterSection(const xMathLib::xvec2* line1 , const xMathLib::xvec2* line2 , xMathLib::xvec2& _t)
{
	//求交点
	/*
	p = p1 + t1 * d1;
	p = p2 + t2 * d2;

	====>
	p1 + t1 * d1 = p2 + t2 * d2;

	p2-p1 = [t1 , t2] * [d1 , -d2];

	p2-p1 = [t1 , t2] *| d1.x ,  d1.y|
	|-d2.x , -d2.y|

	===>

	t1 , t2  = [p21] * inv( M)
	*/
	xvec2 _d1 = line1[1] - line1[0];
	xvec2 _d2 = line2[1] - line2[0];

	xvec2 p21 = line2[0]  - line1[0];
	xMathLib::xmat2 _M;
	_M.m00 =   _d1.x;  _M.m01 =   _d1.y;
	_M.m10 = - _d2.x;  _M.m11 = - _d2.y;
	_M.inverse();

	XM_Mul(p21 , _M , _t);

	if( _t.x > 1.0f ) return false;
	if( _t.x < 0.0f ) return false;

	if( _t.y > 1.0f ) return false;
	if( _t.y < 0.0f ) return false;

	return true;

}

float InterSection(xtriangle& tri,xtriangle& tri2,xlineseg& lineSeg)
{

	return 1;;
}
float InterSection(xtriangle& tri,xplane& plan,xlineseg& lineSeg)
{

	return 1;;
}


float InterSection(xtriangle& tri,xtriangle& tri2g)
{

	return 1;;
}

float InterSection(xtriangle& tri,xplane& plan)
{

	return 1;;
}

float  InterSection(xcylinder& cylinder, xFrustum& frustum)
{

	return 1;;
}


float  InterSection(xaabb& aabb, xFrustum& frustum)
{

	return 1;;
}

float  InterSection(xobb& obb,xFrustum& frutum)
{

	return 1;;
}

float  InterSection(xshpere& shpere,xFrustum& frustum)
{

	return 1;;
}



float  InterSection(xobb& boxA,xobb& boxB)
{

	return 1;;
}

float  InterSection(xaabb& aabbA,xaabb& aabbB)
{

	return 1;;
}



float  InterSection(xray& ray, xaabb& box,xvec3& pIn,xvec3& pOut)
{

	return 1;;
}

float  InterSection(xray& ray, xFrustum& frustum ,xvec3& pIn,xvec3& pOut)
{

	return 1;;
}

float  InterSection(xray& ray, xshpere& shpere,xvec3& pIn,xvec3& pOut)
{

	return 1;;
}

float  InterSection(xray& ray, xcylinder& cylinder,xvec3& pIn,xvec3& pOut)
{

	return 1;;
}


float  InterSection(xlineseg& lineSeg, xaabb& box,xvec3& pIn,xvec3& pOut)
{

	return 1;;
}

float  InterSection(xlineseg& lineSeg, xFrustum& frustum ,xvec3& pIn,xvec3& pOut)
{

	return 1;;
}

float  InterSection(xlineseg& lineSeg, xshpere& shpere,xvec3& pIn,xvec3& pOut)
{

	return 1;;
}

float  InterSection(xlineseg& lineSeg, xcylinder& cylinder,xvec3& pIn,xvec3& pOut)
{

	return 1;;
}



float  InterSection(xtriangle& tri, xaabb& box,xvec3& pStart,xvec3& pEnd)
{

	return 1;;
}

float  InterSection(xtriangle& tri, xFrustum& frustum ,xvec3& pStart,xvec3& pEnd)
{

	return 1;;
}

float  InterSection(xtriangle& tri, xshpere& shpere,xvec3& pStart,xvec3& pEnd)
{

	return 1;;
}

float  InterSection(xtriangle& tri, xcylinder& cylinder,xvec3& pStart,xvec3& pEnd)
{

	return 1;;
}


float  InterSection(xplane& plan, xaabb& box,xvec3& pStart,xvec3& pEnd)
{

	return 1;;
}

float  InterSection(xplane& plan, xFrustum& frustum ,xvec3& pStart,xvec3& pEnd)
{

	return 1;;
}

float  InterSection(xplane& plan, xshpere& shpere,xvec3& pStart,xvec3& pEnd)
{

	return 1;;
}

float  InterSection(xplane& plan, xcylinder& cylinder,xvec3& pStart,xvec3& pEnd)
{

	return 1;;
}


/**************************************************************
2010-7-18
test data :
xMathLib::xvec3 _v0(1 , 0 , 0 );
xMathLib::xvec3 _v1(0 , 1 , 0 );
xMathLib::xvec3 _v2(0 , 0 , 1 );
xtriangle _tri(_v0 , _v1 , _v2);
xMathLib::xvec3 pt(100.0f , 30.0f , 10.0f);
xMathLib::xvec3 _ret ;
float t1 = 1.0f , t2 = -1.0f;
XM_Project(pt , _tri , _ret , t1 , t2);

===> Result is oK. stanly.lee


***************************************************************/

bool    XM_Project(const xMathLib::xvec3& pt , const xtriangle& tri , xMathLib::xvec3& _out , float& t1 , float t2)
{
    xMathLib::xvec3 _e0 = tri.m_points[1] - tri.m_points[0];
    xMathLib::xvec3 _e1 = tri.m_points[2] - tri.m_points[0];

    /*
     v*e0 - p0 * e0 = t1 * |e0|     + t2 * e0 * e1
     v*e1 - p0 * e1 = t1 * e0 * e1  + t2 * |e1|
    */

    xvec2 _v;
    _v.x = _e0.dp(pt) - _e0.dp( tri.m_points[0] );
    _v.y = _e1.dp(pt) - _e1.dp( tri.m_points[0] );

    float _le0 = _e0.dp(_e0);    float _le0_2 = sqrt( _le0 );
    float _le1 = _e1.dp(_e1);    float _le1_2 = sqrt( _le1 );
    float _le01 = _e0.dp(_e1);   float _le01_2  = sqrt( _le01 );

#ifdef _DEBUG
    //for test
    xMathLib::xmat2 _mat;
    _mat.m[0][0] = _le0 ; _mat.m[0][1] = _le01;
    _mat.m[1][0] = _le01; _mat.m[1][1] = _le1 ;
    _mat.inverse();

    xvec2 _t ;
    XM_Mul(_v , _mat , _t);
#endif

    //////////////////////////////////////////////////////////////////////////
    //[ (e1*v0)/(e0*e1 - e01^2) - (e01*v1)/(e0*e1 - e01^2), (e0*v1)/(e0*e1 - e01^2) - (e01*v0)/(e0*e1 - e01^2)]
    float v0 = _v.x;
    float v1 = _v.y;

    t1 = (_le1*v0)/(_le0*_le1 - _le01_2) - (_le01*v1)/(_le0*_le1 - _le01_2);
    t2 = (_le0*v1)/(_le0*_le1 - _le01_2) - (_le01*v0)/(_le0*_le1 - _le01_2);

    _out = tri.m_points[0] + t1 * _e0 + t2 * _e1;

    return (t1  + t2) < 1.0f && t1 >= 0.0f && t2 >= 0.0f;
}

void    XM_Project(const xMathLib::xvec3& pt , xplane& plan , xMathLib::xvec3& _ret )
{
    xMathLib::xvec3 nor , p0;
    plan.getNormal(nor);
    plan.getPt0(p0);


    return XM_Project(pt , p0 , nor , _ret);

}

/**********************************************************
2010-7-18
test data :
xMathLib::xvec3 p0(1.0f , 1.0f , 1.0f);
xMathLib::xvec3 n(1.0f , 1.0f , 1.0f);
xGeomLib::xplane pl(p0 , n );
xMathLib::xvec3 pt(100.0f , 30.0f , 10.0f);

===> Result is oK. stanly.lee

***********************************************************/
void    XM_Project(const xMathLib::xvec3& pt , const xMathLib::xvec3& p0  , const xMathLib::xvec3& nor ,  xMathLib::xvec3& _ret )
{
    xMathLib::xvec3 n1 = nor;
    n1.x += nor.y;
    n1.y += nor.z;
    n1.z += nor.x;
    n1.normalize();

    //-------e0--------
    xvec3 _e0 ;
    n1.cp(nor , _e0);
    if( numeric_equal(_e0.len() , 0.0f) )
    {
        n1.x += nor.y;
        n1.y += nor.z;
        n1.normalize();
        n1.cp(nor , _e0);
    }
    _e0.normalize();

    xvec3 _e1;
    _e0.cp(nor , _e1);
    _e1.normalize();


    /*
     v*e0 - p0 * e0 = t0 * |e0|
     v*e1 - p0 * e1 = t1 * |e1|
    */

    xvec3 v = pt - p0;
    float t0 = v.dp(_e0);
    float t1 = v.dp(_e1);

    _ret = p0 + t0 * _e0 + t1 * _e1;
    return ;
}


void    XM_Project(const xMathLib::xvec3& pt , const xMathLib::xvec3 vert[3] ,  xMathLib::xvec3& _ret )
{
    xvec3 p0 = vert[0];
    xvec3 v1 = vert[1] - vert[0];
    xvec3 v2 = vert[2] - vert[0];

    xvec3 nor = v1.cp(v2);
    nor.normalize();
    v1.normalize();

    //跟上面那个函数一样
    xvec3& _e0 = v1;
    xvec3 _e1;
    _e0.cp(nor , _e1);
    _e1.normalize();


    /*
     v*e0 - p0 * e0 = t0 * |e0|
     v*e1 - p0 * e1 = t1 * |e1|
    */
    xvec3 v = pt - p0;
    float t0 = v.dp(_e0);
    float t1 = v.dp(_e1);

    _ret = p0 + t0 * _e0 + t1 * _e1;
    return ;
}


END_NAMESPACE_XGEOMLIB

