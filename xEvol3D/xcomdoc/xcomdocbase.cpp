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
#pragma warning (disable:4273)

#ifdef _USE_XEVOL3D_NEW_
#include "../mem/mem_operator.h"
#endif

#include "xcomdoc_common.h"
#include <assert.h>
#include "xcomdocImp.h"
#include "xcomdocbase.h"
#include "xcomdir.h"
#include "xcdStringHash.h"
#include "xcdUtil.h"

using namespace XEvol3D;

BEGIN_NAMESPACE_XEVOL3D
static xComDocBaseCreatorMgr* g_Singleton = NULL;
IComDocBase* CreateComDoc(const wchar_t* _type)
{
	if(_type == NULL)
		return new xComDocument();
	IComDocBase* pDoc = xComDocBaseCreatorMgr::singleton()->createInstance(_type);
	if(pDoc == NULL) pDoc = new xComDocument();
	return pDoc;
}

xComDocBaseCreatorMgr* xComDocBaseCreatorMgr::singleton()
{
	if(g_Singleton == NULL)
	{
		g_Singleton = new xComDocBaseCreatorMgr();
	}
	return g_Singleton;
}

bool         xComDocBaseCreatorMgr::registCreator(IComDocBaseCreator* pCreator)
{
     m_vComDocCreators.push_back( pCreator );
	 return true;
}

bool         xComDocBaseCreatorMgr::unregistCreator(IComDocBaseCreator* pCreator)
{
	vComDocBaseCreators::iterator pos = m_vComDocCreators.begin();
	for( ; pos != m_vComDocCreators.end() ; pos ++)
	{
		if(*pos == pCreator)
		{
			m_vComDocCreators.erase(pos);
			return true ;
		}
	}
	return false;    
}

IComDocBase* xComDocBaseCreatorMgr::createInstance(const wchar_t* _ext)
{
	size_t iCreator = m_vComDocCreators.size() ;
      for(size_t i = 0 ; i < iCreator ; i ++)
	  {
		  IComDocBaseCreator* pCreator = m_vComDocCreators[i];
		  if(pCreator->support(_ext))
			  return pCreator->createInstace();
	  }
	  return NULL;
}


END_NAMESPACE_XEVOL3D



