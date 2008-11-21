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

#include "xcamera.h"
BEGIN_NAMESPACE_XGEOMLIB

void xCamera::XEye::toMatirx(xmat4& matView)
{
	XM_LookAt(matView,m_EyePos,m_EyeTarget,m_Up);
	return ;
}

xCamera::XEye::XEye():
m_EyePos(0,0,0),m_EyeTarget(0,0,-1),m_Up(0,1,0)
{
}

void xCamera::toMatrix(xmat4& matView,xmat4& matProject)
{

	m_Eye.toMatirx(matView);

	if(m_ProjectType == PT_PROJECT)
	{
		XM_Perspective(matProject,m_fFov,m_fAspect,m_fNear,m_fFar);
	}
	else if(m_ProjectType == PT_ORTHO)
	{
		XM_Ortho3D(matProject,m_fNear,m_fFar,m_ViewRect.m_top,m_ViewRect.m_bottom,m_ViewRect.m_left,m_ViewRect.m_right);
	}
	else
	{
		matProject.identity();
	}
	return ;

}

void xCamera::__calcMatrix()
{
	if(m_bUpdate == true)
	{
		m_Eye.toMatirx(m_ViewMat);

		if(m_ProjectType == PT_PROJECT)
		{
			XM_Perspective(m_ProjMat,m_fFov,m_fAspect,m_fNear,m_fFar);
		}
		else if(m_ProjectType == PT_ORTHO)
		{
			XM_Ortho3D(m_ProjMat,m_fNear,m_fFar,m_ViewRect.m_top,m_ViewRect.m_bottom,m_ViewRect.m_left,m_ViewRect.m_right);
		}
		else
		{
			m_ProjMat.identity();
		}
		m_bUpdate = false;
	}
}
void xCamera::toViewMatrix(xmat4& matView)
{
	__calcMatrix();
	matView = m_ViewMat;
}

void xCamera::toProjectMatrix(xmat4& matProject)
{
	__calcMatrix();
	matProject = m_ProjMat;
	return ;
}

xCamera::xCamera()
{
	m_bUpdate = true;
}

void xCamera::setViewRect(XViewRect& rect)
{
	__setdirty(); 
	m_ViewRect=rect;        
}


/*****************************************************************
//set the projectiv mode¡£
*****************************************************************/
void xCamera::getLeft(xvec4& _vleft)
{
	xvec4 _vdir = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	_vdir.normalize();
	_vleft = _vdir.cp(m_Eye.m_Up );
	_vleft.normalize();
}

void xCamera::getUp(xvec4& _vup)
{
	xvec4 _vdir = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	_vdir.normalize();
	xvec4 _vleft;
	_vleft = _vdir.cp(m_Eye.m_Up );
	_vleft.normalize();
	_vup = _vleft.cp(_vdir);

}
void xCamera::getDir(xvec4& _vdir)
{
	_vdir = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	_vdir.normalize();
}
void xCamera::getLUD(xvec4& _vleft , xvec4& _vup , xvec4& _vdir)
{
	_vdir = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	_vdir.normalize();
	_vleft = _vdir.cp(m_Eye.m_Up );
	_vleft.normalize();
	_vup = _vleft.cp(_vdir );
}

void xCamera::setProjectType(PROJECT_T type)       
{
	__setdirty(); 
	m_ProjectType = type  ; 
}
void   xCamera::setFOV(float fov)                   
{
	__setdirty();
	m_fFov   = fov;        
}
void   xCamera::setNearPlan(float near_plan)        
{
	__setdirty(); 
	m_fNear  = near_plan;   
}

void   xCamera::setFarPlan(float  far_plan)          
{
	__setdirty();
	m_fFar   = far_plan;    
}

void   xCamera::setAspect(float   aspect )
{
	__setdirty();
	m_fAspect= aspect;     
}

//get the projectiv mode¡£
xCamera::PROJECT_T xCamera::getProjectType()                 
{
	__setdirty();
	return m_ProjectType  ; 
}


void xCamera::setViewRect(float  l,float r,float t,float b)
{
	m_ViewRect.m_left = l;
	m_ViewRect.m_right = r;
	m_ViewRect.m_top = t;
	m_ViewRect.m_bottom =b;
}

void xCamera::setPerspective2D(int w , int h , float fov, bool bResetEye)
{
	__setdirty();

	float rfov = (fov/2.0f) * 3.1415926f/180.0f;
	float _zScreen  = float(h/2) / tan(rfov);
	float _near = 1.0f;
	float _far = _zScreen * 10.0f;

	if ( bResetEye )
	{
		m_Eye.m_EyePos    = xvec4(w/2.0f , h/2.0f , - _zScreen , 1.0f);
		m_Eye.m_EyeTarget = xvec4(w/2.0f , h/2.0f , 0.0f, 1.0f);
		m_Eye.m_Up        = xvec4(0.0f   , -1.0f   , 0.0f, 1.0f);
	}
	m_ViewRect.m_left = m_ViewRect.m_top = 0;
	m_ViewRect.m_right = (float)w;
	m_ViewRect.m_bottom = (float)h;
	m_ProjectType = PT_PROJECT;
	setNearPlan(_near);
	setFarPlan(_far);
	setFOV( fov );
	setAspect( float(w)/float(h) );
}

void xCamera::__setdirty()
{
	m_bUpdate = true;
}

void xCamera::yaw(float angle)
{
	xvec4 vDir  = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	xvec4 newDir;
	float len = vDir.len();
	vDir.normalize();
	xmat4 matRot ;
	XM_Rotate(matRot,m_Eye.m_Up,angle);
	XM_Mul(vDir,matRot,newDir);

	newDir = newDir * len;
	m_Eye.m_EyeTarget = m_Eye.m_EyePos + newDir;
	__setdirty();

}
void xCamera::pitch(float angle)
{
	xvec4 vDir  = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	float len = vDir.len();
	vDir.normalize();
	xvec4 vLeft = vDir.cp(m_Eye.m_Up);

	xvec4 newDir;
	xvec4 newUp;

	xmat4 matRot;
	XM_Rotate(matRot,vLeft,angle);

	XM_Mul(vDir,matRot,newDir);
	XM_Mul(m_Eye.m_Up,matRot,newUp);
	m_Eye.m_Up = newUp;
	newDir = newDir * len;
	m_Eye.m_EyeTarget = m_Eye.m_EyePos + newDir;
	__setdirty();
}
void xCamera::dirty()
{
	__setdirty();
}
void xCamera::roll(float angle)
{
	xvec4 vDir  = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	vDir.normalize();
	xvec4 vLeft = vDir.cp(m_Eye.m_Up);

	xvec4 newUp;

	xmat4 matRot;
	XM_Rotate(matRot,vDir,angle);

	XM_Mul(m_Eye.m_Up,matRot,newUp);
	m_Eye.m_Up = newUp;
	__setdirty();
}

void xCamera::circle(float angle)
{
	xvec4 vDir  = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	xvec4 newDir;
	float len = vDir.len();


	vDir.normalize();
	xmat4 matRot ;
	XM_Rotate(matRot,m_Eye.m_Up,angle);
	XM_Mul(vDir,matRot,newDir);

	newDir = newDir * len;
	m_Eye.m_EyePos = m_Eye.m_EyeTarget - newDir;
	__setdirty();
}


void xCamera::toward(float dist)
{
	xvec4 vDir = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	vDir.normalize();
	vDir *= dist;

	m_Eye.m_EyePos.x += vDir.x;
	m_Eye.m_EyePos.y += vDir.y;
	m_Eye.m_EyePos.z += vDir.z;

	m_Eye.m_EyeTarget.x += vDir.x;
	m_Eye.m_EyeTarget.y += vDir.y;
	m_Eye.m_EyeTarget.z += vDir.z;
	__setdirty();

}

void xCamera::upDown(float dist)
{
	xvec4 vDir = m_Eye.m_Up;
	vDir *= dist;

	m_Eye.m_EyePos.x += vDir.x;
	m_Eye.m_EyePos.y += vDir.y;
	m_Eye.m_EyePos.z += vDir.z;

	m_Eye.m_EyeTarget.x += vDir.x;
	m_Eye.m_EyeTarget.y += vDir.y;
	m_Eye.m_EyeTarget.z += vDir.z;
	__setdirty();
}

void xCamera::shift(float  dist)
{

	xvec4 vDir  = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	xvec4 vLeft = vDir.cp(m_Eye.m_Up);
	vLeft.normalize();
	vLeft *= dist;

	m_Eye.m_EyePos.x += vLeft.x;
	m_Eye.m_EyePos.y += vLeft.y;
	m_Eye.m_EyePos.z += vLeft.z;

	m_Eye.m_EyeTarget.x += vLeft.x;
	m_Eye.m_EyeTarget.y += vLeft.y;
	m_Eye.m_EyeTarget.z += vLeft.z;	
	__setdirty();
}

void xCamera::rotate(xvec4& axis,float angle)
{
	xvec4 vDir  = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	xvec4 vUp = m_Eye.m_Up;

	xmat4 rot_mat;
	XM_Rotate(rot_mat,axis,angle);

	xvec4 vDirNew;
	XM_Mul(vDir,rot_mat,vDirNew);
	XM_Mul(vUp,rot_mat,m_Eye.m_Up);
	m_Eye.m_EyeTarget = m_Eye.m_EyePos + vDirNew;
	__setdirty();
}

void xCamera::focus(float dist,float factor)
{
	xvec4 V = m_Eye.m_EyePos - m_Eye.m_EyeTarget;
	float len = V.len();

	if( (1  - dist/len) < factor) 
		return ;
	float f = (len - dist )/len;
	V = V * f;
	m_Eye.m_EyePos =  m_Eye.m_EyeTarget + V;
	__setdirty();
}
END_NAMESPACE_XGEOMLIB

