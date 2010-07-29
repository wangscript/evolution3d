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
#include "xcamera.h"
#include "xfrustum.h"
#include "xline.h"
using namespace XEvol3D::XMathLib;

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
xMathLib::xvec4 xCamera::XEye::getDir(bool bNormalized ) 
{ 
    xMathLib::xvec4 ret = m_EyeTarget - m_EyePos;
    if(bNormalized)
        ret.normalize();
    return ret;
}
xMathLib::xvec4 xCamera::XEye::getRight()
{
    xMathLib::xvec4 dir = getDir();
    xMathLib::xvec4 ret = dir.cp(m_Up);
    ret.normalize();
    return ret;
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

xFrustum* xCamera::frustum()
{
	__calcMatrix();
	return &m_frustum;
}

void xCamera::copyFrom(xCamera* pCamera)
{
      m_Eye         = pCamera->m_Eye;
	  m_fFov        = pCamera->m_fFov       ;
	  m_fNear       = pCamera->m_fNear      ;
	  m_fFar        = pCamera->m_fFar       ;
	  m_fAspect     = pCamera->m_fAspect    ;
	  m_ViewRect    = pCamera->m_ViewRect   ;
	  m_ProjectType = pCamera->m_ProjectType;
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
		m_frustum.from(m_ViewMat , m_ProjMat);
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
	m_fAspect = 1.0f;
	m_fFar = 0.0f;
	m_fFov = 45.0f;
	m_fNear = 0.01f;
	m_ProjectType = PT_ORTHO;
	m_ViewRect.m_left = 0.0f;
	m_ViewRect.m_right = 400.0f;
	m_ViewRect.m_top   = 0.0f;
	m_ViewRect.m_bottom = 300.0f;
	m_pEventHanlder = NULL;
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

void xCamera::cloneTo(xCamera* pCamera , bool fireEvent)
{
    pCamera->m_fFov           = m_fFov;
    pCamera->m_fNear          = m_fNear;
    pCamera->m_fFar           = m_fFar;
    pCamera->m_fAspect        = m_fAspect;
    pCamera->m_ViewRect       = m_ViewRect;
    pCamera->m_ProjectType    = m_ProjectType;
    pCamera->m_ViewMat        = m_ViewMat;
    pCamera->m_ProjMat        = m_ProjMat;
    pCamera->m_frustum        = m_frustum;
    pCamera->m_pEventHanlder  = m_pEventHanlder;
    pCamera->m_bUpdate = true;

    pCamera->m_Eye            = m_Eye;
    if( fireEvent ) 
    {
        pCamera->__setdirty();
    }
}

void xCamera::__setdirty()
{
	m_bUpdate = true;
	if(m_pEventHanlder)
		m_pEventHanlder->onChange();
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

void xCamera::distance( float fDistance )
{
	xvec4 dir = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	dir.normalize();
	dir = dir * fDistance; //×î½ü50m£¬×îÔ¶ 50000m
	m_Eye.m_EyeTarget = dir + m_Eye.m_EyePos;
	dirty();
}

float xCamera::distance()
{
	xvec4 dir = m_Eye.m_EyeTarget - m_Eye.m_EyePos;
	return dir.len();
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

void xCamera::getPickRay( xvec3& pvPickRay,int x,int y,int sw,int sh )
{
    __calcMatrix();
    xvec4 v;
    if( sw==-1 )
        sw = (int)m_ViewRect.width();
    if( sh==-1 )
        sh = (int)m_ViewRect.height();
    v.x =  ( 2.f * x / (float)sw - 1 ) / m_ProjMat.m[0][0];
    v.y = -( 2.f * y / (float)sh - 1 ) / m_ProjMat.m[1][1];
    v.z =  -1.0f;
    v.w =  1.0f;
    xmat4 matViewI = m_ViewMat;
    matViewI.inverse();

    XM_Mul(v, matViewI, pvPickRay);
}

void xCamera::getPickRay(xline& pvPickLine, int x, int y, int sw/*=-1*/, int sh/*=-1*/)
{
    xvec3 vDir;
    getPickRay(vDir, x, y, sw, sh);
    vDir = vDir - m_Eye.m_EyePos;
    vDir.normalize();
    pvPickLine.m_Dir = vDir;
    pvPickLine.m_Point = m_Eye.m_EyePos;
}

xvec3 xCamera::trans2Screen( xvec3& vScr , const xvec3& pvPos )
{
    xvec3 vDir = pvPos - m_Eye.m_EyePos;
    xvec3 vEyeDir = m_Eye.getDir();
    if( vDir.dp(vEyeDir) > 0.f )
    {
        xmat4 matProjView;
        xmat4 matView;
        xmat4 matProj;
        toMatrix(matView , matProj );
        XM_Mul(matView , matProj , matProjView);
        vScr = matProjView * pvPos;
        vScr.x = pvPos.x * matProjView.m[0][0] + pvPos.y * matProjView.m[1][0] + pvPos.z * matProjView.m[2][0] + matProjView.m[3][0];
        vScr.y = pvPos.x * matProjView.m[0][1] + pvPos.y * matProjView.m[1][1] + pvPos.z * matProjView.m[2][1] + matProjView.m[3][1];
        vScr.z = pvPos.x * matProjView.m[0][2] + pvPos.y * matProjView.m[1][2] + pvPos.z * matProjView.m[2][2] + matProjView.m[3][2];
        float w = matProjView.m[0][3] * pvPos.x + matProjView.m[1][3] * pvPos.y + matProjView.m[2][3] * pvPos.z + matProjView.m[3][3];
        vScr.x /= w;
        vScr.y /= w;
        vScr.z /= w;
        vScr.x *= m_ViewRect.width();
        vScr.y *= m_ViewRect.height();
    }
    else
    {
        vScr.x = -FLT_MAX;
        vScr.y = -FLT_MAX;
        vScr.z = -FLT_MAX;
    }
    return vScr;
}

void  xCamera::calcBillboardMatrix(const xMathLib::xmat4& worldMatrix , xMathLib::xmat4& _bbMatrix)
{
    __calcMatrix();
    XM_Mul(worldMatrix , m_ViewMat , _bbMatrix);
    _bbMatrix.noTrans();
    _bbMatrix.inverse();
}

END_NAMESPACE_XGEOMLIB

