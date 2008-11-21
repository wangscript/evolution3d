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

#include "XMathLib.h"
#include "XGeomLib.h"
#include "XGVolume.h"
using namespace XEvol3D::XMathLib;


BEGIN_NAMESPACE_XGEOMLIB
namespace XInterSection
{

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

	float  InterSection(xcylinder& cylinder, xfrustum& frustum)
	{

		return 1;;
	}


	float  InterSection(xaabb& aabb, xfrustum& frustum)
	{

		return 1;;
	}

	float  InterSection(xobb& obb,xfrustum& frutum)
	{

		return 1;;
	}

	float  InterSection(xshpere& shpere,xfrustum& frustum)
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

	float  InterSection(xray& ray, xfrustum& frustum ,xvec3& pIn,xvec3& pOut)
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

	float  InterSection(xlineseg& lineSeg, xfrustum& frustum ,xvec3& pIn,xvec3& pOut)
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

	float  InterSection(xtriangle& tri, xfrustum& frustum ,xvec3& pStart,xvec3& pEnd)
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

	float  InterSection(xplane& plan, xfrustum& frustum ,xvec3& pStart,xvec3& pEnd)
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

}

END_NAMESPACE_XGEOMLIB

