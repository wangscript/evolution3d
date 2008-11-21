// Evol3D.Net.h

#pragma once

#include "Evol3D.Net.h"
#include "xnRenderApi.h"
#include "xDotNetUlti.h"
#include "xnMath.h"
#include "xModel/xBaseModel.h"

using namespace XEvol3D;
using namespace System;

namespace xEvol3D 
{
	ref class   xColorF;

#undef  ENUMCLASS
#define ENUMCLASS( v ) public enum class

	public ref   class xBaseModel
	{
	internal:
		XEvol3D::HBaseModel*  m_obj;
		xBaseModel(XEvol3D::HBaseModel hModl) { m_obj = new XEvol3D::HBaseModel; *m_obj = hModl ; }
		~xBaseModel(){delete m_obj ; }
	public:		
		void Release() {m_obj->release() ;}
		void AddRef()  {m_obj->addRef() ; }
		void Lock()    {m_obj->lock();    }
		void Draw()    {(*m_obj)->draw();   }
		void update(int passedTime) 
		{ 
			(*m_obj)->updateFrame(passedTime);
		}
		void BoundBox(xVector3^ min , xVector3^ max)
		{
			xGeomLib::xaabb aabb = (*m_obj)->boundBox();
			*min->m_vec = aabb.m_Min;
			*max->m_vec = aabb.m_Max;
		}
	};

	public ref   class xBaseModelManager
	{
	internal:
		xBaseModelMgr* m_mgr;
	public:
		xBaseModelManager(xRenderApi^ _pApi , xBaseTextureManager^  _texMgr , String^ name)
		{
			IRenderApi* pApi      = _pApi->m_pRenderApi;
			xBaseTextureMgr* pMgr = _texMgr->m_pTexMgr;
			m_mgr = xBaseModelMgr::createInstance(pApi , pMgr , ToPtr(name));
		}

		xBaseModel^ loadModel(String^ model)
		{
			HBaseModel hModel = m_mgr->add(ToPtr(model) , 0 , true);
			return gcnew xBaseModel(hModel);
		}

		void addPackage(String^ pkgName , String^ _dir)
		{
			m_mgr->addPackage(ToPtr(pkgName) , ToPtr(_dir));
		}

		void addPath(String^ pathName)
		{
			m_mgr->addPath(ToPtr(pathName));
		}    
	};
}
