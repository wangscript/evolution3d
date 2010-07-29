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

class _XEVOL_BASE_API_ xray;
class _XEVOL_BASE_API_ xplane;
class _XEVOL_BASE_API_ xlineseg;
class _XEVOL_BASE_API_ xtriangle;
class _XEVOL_BASE_API_ xplane;
class _XEVOL_BASE_API_ xpolygon;
class _XEVOL_BASE_API_ xCylinder;

__inline bool XM_IsSameDir(xMathLib::xvec3& v1,xMathLib::xvec3& v2)
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

END_NAMESPACE_XGEOMLIB
namespace xGeomLib = XEvol3D::XGeomLib ;
#include "xline.h"
#include "xplane.h"
#include "xpolygon.h"
#include "xintersection.h"
#include "xvolume.h"
#include "xfrustum.h"
#include "xcamera.h"

#endif

