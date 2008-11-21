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

#ifndef _X_MATH_CAMERA_H_
#define _X_MATH_CAMERA_H_

#include "XMathLib.h"
using namespace XEvol3D::XMathLib;

BEGIN_NAMESPACE_XGEOMLIB
	class _XEVOL_BASE_API_  xCamera
	{
    public:
		//Projection Type
		enum PROJECT_T
		{
			PT_PROJECT = 0, 
			PT_ORTHO   = 1, 
			PT_FRUSTUM = 2  // Not implemented
		};

		//Define the view rect
		class XViewRect
		{
		public:
			float m_left;
			float m_right;
			float m_top;
			float m_bottom;
		};
public:
		//define the eye infomation
		class _XEVOL_BASE_API_ XEye
		{
			friend class xCamera;
		public:
			xvec4 m_Up;
			xvec4 m_EyePos;
			xvec4 m_EyeTarget;
		public:
			XEye(xvec4& EyeTarget,xvec4& EyePos,xvec4& Up)
				:m_Up(Up),m_EyePos(EyePos),m_EyeTarget(EyeTarget)
			{

			}
			XEye();
			/*no destructor*/
			~XEye(){};
			void toMatirx(xmat4& matView);
		};
	public:


		xCamera();
		/*****************************************************************
		Build the transform matrix from the camera
		include : project,lookat .
		*****************************************************************/
		void       toMatrix(xmat4& matView,xmat4& matProject);
        void       toProjectMatrix(xmat4& matProject);
		void       toViewMatrix(xmat4& matView);
		void       dirty();

		/*****************************************************************
		yaw.
		pitch.
		roll
		circle.
		*****************************************************************/
		void       yaw(float angle);
		void       pitch(float angle);
		void       roll(float angle);
		void       circle(float angle);


		/*****************************************************************
        change the camera's position
		*****************************************************************/
		void       toward(float dist);
		void       upDown(float dist);
		void       shift(float  dist);

		/*****************************************************************
        rotate the camera
        *****************************************************************/
        void       rotate(xvec4& axis,float angle);
		void       focus(float dist,float factor = 0.1);


		/*****************************************************************
		set / get the property
		*****************************************************************/
		void       setEye(xCamera::XEye& eye){__setdirty(); m_Eye = eye;};
		/*****************************************************************
		得到摄影机的参数
		*****************************************************************/
		XEye&      getEye()                         {return     m_Eye;       }
		float      getFOV()                         {return     m_fFov;      }
		float      getNearPlan()                    {return     m_fNear;     }
		float      getFarPlan()                     {return     m_fFar;      }
		float      getAspect()                      {return     m_fAspect;   }
		XViewRect* getViewRect()                    {return    &m_ViewRect;  }
        void       getLeft(xvec4& _vleft);
		void       getUp(xvec4& _vup);
		void       getDir(xvec4& _vdir);
		void       getLUD(xvec4& _vleft , xvec4& _vup , xvec4& _vdir);
		XEye m_Eye;
	protected:
		void        __setdirty();
		void        __calcMatrix();
		/*
        Project Matrix  information。
		*/
		float       m_fFov;
		float       m_fNear;
		float       m_fFar;
		float       m_fAspect;
		XViewRect   m_ViewRect;
		PROJECT_T   m_ProjectType;
		xmat4       m_ViewMat;
		xmat4       m_ProjMat;
		bool        m_bUpdate;
	
public:

		/*****************************************************************
		//set the projectino's informatino。
		*****************************************************************/
		void   setFOV(float fov) ;
		void   setNearPlan(float near_plan);
		void   setFarPlan(float  far_plan);
		void   setAspect(float   aspect );
		/*****************************************************************
		//set the viewport's size 。only effect the ortho .
		*****************************************************************/
		void   setViewRect(XViewRect& rect);
		void   setPerspective2D(int w , int h , float fov, bool bResetEye = true) ;
		/*****************************************************************
		//set the projectiv mode。
		*****************************************************************/
		void     setProjectType(PROJECT_T type);
		//get the projectiv mode。
		PROJECT_T getProjectType();
		void      setViewRect(float  l,float r,float t,float b);


	};
END_NAMESPACE_XGEOMLIB
#endif
