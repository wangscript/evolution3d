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
using namespace XEvol3D::XMathLib;

BEGIN_NAMESPACE_XGEOMLIB
namespace XInterSection
{
	bool InterSection(xray& ray,xplane& plan,xvec3& point,float & t)
	{
		t = 0;

		/***

		-D - P0 * N(A,B,C)
		t = --------------------
		d * N(A,B,C)

		**/

		//其中D 为平面方程中的D,
		//P0为射线的起点。�?d 为射线的方向


		t= -(plan.D  + ray.m_Point.x * plan.A + ray.m_Point.y * plan.B + ray.m_Point.z * plan.C);
		float t2 = (ray.m_Dir.x * plan.A + ray.m_Dir.y * plan.B + ray.m_Dir.z * plan.C) ;

		if( numeric_equal(t2 ,  (float)0.00) )
			return false;

		t /= t2;


		//求出交点
		point  = ray.m_Point + ray.m_Dir * t;

		if( t < 0)
			return false;

		return true;
	}

	///**************************************************************************
	//求三角形和射线的交点�?

	//2004-  6 -27 日建�?
	//2004 - 6 -27 日初步测试�?
	//***************************************************************************/
	bool    InterSection(xray& ray,xtriangle& tri,xvec3& point,float & t)
	{

		xvec3 v1 = tri.m_points[1] - tri.m_points[0];
		xvec3 v2 = tri.m_points[2] - tri.m_points[0];

		xvec3 n = v1.cp(v2);
		/**********************************************************************
		ray = p0 + D * t
		Tri : p1 - p3

		(P2-P1) X (P3-P1) = N

		N * (P - P1) = 0  ===>
		N * (P0 + D * t - P1)  = 0

		==>

		N * (P0 - P1) + N * D * t = 0;
		(P1-P0) * N
		t = ----------------------
		D * N
		******************************************************************/
		t = n.dp(tri.m_points[0] - ray.m_Point);
		t /= (ray.m_Dir.dp(n));
		if( t < 0)
			return false;

		//求出交点
		point  = ray.m_Point + ray.m_Dir * t;

		//判断点是不是在三角内�?
		v1 =  tri.m_points[0] - point ;
		v2 =  tri.m_points[1] - point ;

		xvec3 v3 = tri.m_points[2] - point ;

		v1.cp(v2,n);
		xvec3 n1,n2;
		v2.cp(v3,n1);
		v3.cp(v1,n2);

		if(n.isZero())
			return true;
		if(n1.isZero())
			return true;
		if(n2.isZero())
			return true;

		if(false == Helper::IsSameDir(n2,n))
			return false;

		return Helper::IsSameDir(n1,n2);
	}

	///**************************************************************************
	//求三角形和射线的交点。能求出三个参数�?

	//2004-  6 -27 日建�?
	//2004 - 6 -27 日初步测试�?
	//***************************************************************************/
	bool InterSection(xray& ray,xtriangle& tri,xvec3& point,float& t, float& u, float& v )
	{
		// Find vectors for two edges sharing vert0


		//三角形为 v1,v2,v3
		//两条边为  e1 = v2-v1   e2 = v3 - v1
		//射线�?   ray = p0 + d * t
		//三角形内部的一�?p = v1 + u * e1 + v * e2  (  u+v<1)

		//所以：

		/***
		v1 + u * e1 + v * e2 = p0 + d * t              ===>
		u * e1 + v * e2 - t * d = p0 - v1              ===>
		- t * d  + v * e2 +  u * e1  = p0 - v1         ===>

		|  d.x   d.y    d.z  |
		[-t,v,u]  |  e2.x  e2.y   e2.z |   = p0 - p1  ===>
		|  e1.x  e1.y   e1.z |


		[-t,v,u] * M  = p0 - p1 ;


		[-t,v,u] = (p0 - p1) * Inv(M);


		t  = (p0 - p1) * e1 X e2 / Det(M) =  (p0 - p1) X e1 * e2 / Det(M)

		v  = (p0 - p1) * e1 X d  / Det(M) =  (p0 - p1) X e1 * d  / Det(M)

		u  = (p0 - p1) * d X e2  / Det(M)

		**/
		xvec3 e1  = tri.m_points[1] - tri.m_points[0];
		xvec3 e2  = tri.m_points[2] - tri.m_points[0];


		//求出矩阵 M �?det(M)。并记录 d x e2;
		xvec3 vCP_dir_e2;
		ray.m_Dir.cp(e2,vCP_dir_e2);

		//得到矩阵的行列式的�?
		float det = e1.dp(vCP_dir_e2);

		//保存 (p0 - p1)
		xvec3 v_p0_p1;

		//为了判断方便。det = abs(det)
		if( det > 0 )
		{
			v_p0_p1 = ray.m_Point - tri.m_points[0];
		}
		else
		{
			v_p0_p1 = tri.m_points[0] - ray.m_Point  ;
			det = -det;
		}

		if( det < 0.0000001f )
			return false;

		// u  = (p0 - p1) * d X e2  / Det(M)  Det(M)以后再除
		u  = v_p0_p1.dp(vCP_dir_e2);
		if( u < 0.0f || u > det )
			return false;


		// 保存 (p0 - p1) X e1
		xvec3 vCP_p0p1_e1;
		v_p0_p1.cp(e1,vCP_p0p1_e1);

		// v  = (p0 - p1) * e1 X d  / Det(M) =  (p0 - p1) X e1 * d  / Det(M)
		// Det(M)以后再除
		v = ray.m_Dir.dp(vCP_p0p1_e1);
		if( v < 0.0f || u + v > det )
			return false;

		// Calculate t, scale parameters, ray intersects triangle
		t =  e2.dp(vCP_p0p1_e1);
		float fInvDet = 1.0f / det;
		t *= fInvDet;
		u *= fInvDet;
		v *= fInvDet;
		point = ray.m_Point + (ray.m_Dir*t);
		return true;
	}

	//线段和面的相�?
	bool  InterSection(xlineseg& lineSeg,xplane& plan,xvec3& point,float & t)
	{
		return 1;;
	}
	bool  InterSection(xlineseg& lineSeg,xtriangle& tri,xvec3& point,float & t)
	{
		return 1;;
	}

	//线和线的相交
	float InterSection(xray& ray1,xray& ray2,xvec3& point)
	{
		return 1;;
	}
	float InterSection(xray& ray,xlineseg& lineSeg,xvec3& point)
	{

		return 1;;
	}
	float InterSection(xlineseg& lineSeg1,xlineseg& lineSeg2,xvec3& point)
	{

		return 1;;
	}

	//点是不是在三角形和面�?
	float InterSection(xvec3& point ,xtriangle& tri)
	{

		return 1;;
	}
	float InterSection(xvec3& point ,xpolygon& polygon)
	{

		return 1;;
	}
}
END_NAMESPACE_XGEOMLIB


