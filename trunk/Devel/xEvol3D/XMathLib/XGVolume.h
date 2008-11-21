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

#include  "XMathLib.h"
#include  "XGeomLib.h"
#include  "xcamera.h"

using namespace XEvol3D::XMathLib;


BEGIN_NAMESPACE_XGEOMLIB

class _XEVOL_BASE_API_ xrect;
class _XEVOL_BASE_API_ xshpere;
class _XEVOL_BASE_API_ xobb;
class _XEVOL_BASE_API_ xfrustum;
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
	xaabb(xvec3& Min,xvec3& Max){ Build(Min,Max); }
	void Build(xvec3& Min,xvec3& Max){};
	void Build(xvec3* points,int n){};
	void AddPoint(xvec3& point){};

public:
	xvec3 m_Min;
	xvec3 m_Max;
};

/*
Oreint-Bounding Box;
*/
class _XEVOL_BASE_API_  xobb
{
public:
	xvec3    m_Point;
	xvec3    m_Orient;
};


class _XEVOL_BASE_API_  xfrustum
{
	xplane* m_ClipPlanes;
	int    m_nClipPlane;
public:
	void BuildFromMatrix(xmat4& mViewTrans,xmat4& mProjectTrans);
	void BuildFromCamera(xCamera& Camera);
	void BuildFromCameraAndPortal(xCamera& Camera, xpolygon& polygon);
	void BuildFromPortal(xvec3& viewPos, xpolygon& polygon);

	bool visiable(xvec3& point);
	bool visiable(xtriangle& tri);
	bool visiable(xobb& obb);
	bool visiable(xshpere& shpere);
	bool visiable(xaabb&  aabb);
	bool visiable(xcylinder& cylinder);

	~xfrustum();
	xfrustum(){m_ClipPlanes = NULL;m_nClipPlane=0;};

};

class  _XEVOL_BASE_API_ xshpere
{
public:
	xvec3  m_Orign;
	float  m_R;
};

class _XEVOL_BASE_API_ xcylinder
{
public:
	xvec3 m_StartPoint;
	xvec3 m_EndPoint;
	float  m_R;
};



END_NAMESPACE_XGEOMLIB

#endif
