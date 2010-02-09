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

#ifndef _X_MATH_FRUSTUM_H_
#define _X_MATH_FRUSTUM_H_

#include "xGeomLib.h"
#include "xvolume.h"
BEGIN_NAMESPACE_XGEOMLIB
class _XEVOL_BASE_API_  xCamera;
class _XEVOL_BASE_API_  xFrustum
{
public:
	enum 
	{
		PL_RIGHT = 0,
		PL_LEFT ,
		PL_BOTTOM,
		PL_TOP,
		PL_FAR,
		PL_NEAR,
		PL_MAX,
	};
public:
	xFrustum(){};
	~xFrustum(){};
	bool from(const xMathLib::xmat4& _viewMat , const xMathLib::xmat4& _projMat);
	bool from(xCamera* pCamera);
	bool visible(const xMathLib::xvec3& pt);
	bool visible(const xaabb& aabb);
	bool visible(const xaabb& aabb  , const xMathLib::xmat4& aabbMat);
	bool visible(const xMathLib::xvec3& _cent , float radius);
public:
	xplane m_plane[PL_MAX];
};
END_NAMESPACE_XGEOMLIB
#endif
