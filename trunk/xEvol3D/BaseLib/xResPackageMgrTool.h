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

#ifndef __TEMPLATE_PKG_RESOURCE_MGR_TOOL_H__
#define __TEMPLATE_PKG_RESOURCE_MGR_TOOL_H__
#include "xResPackageMgr.h"
#include "../OperationSys/xOperationSys.h"
BEGIN_NAMESPACE_XEVOL3D
template <typename _TMgr > bool XEvol_AddResManagerPath(_TMgr& _Mgr , xXmlNode* pRootNode)
{
	if(pRootNode==NULL)
		return false;

	xXmlNode::XmlNodes pathNodes ;
	pRootNode->findNode(L"path",pathNodes);
	size_t nPathNodes = pathNodes.size() ;
	for(size_t i = 0 ; i < nPathNodes ;  i ++)
	{
		xXmlNode* pPathNode = pathNodes[i];
		const wchar_t* strPath =  pPathNode->value(L"path");
		const wchar_t* strPkg  =  pPathNode->value(L"file");
        bool  bUseResPath = pPathNode->bool_value(L"useResPath");
        if(bUseResPath)
        {
            if(strPkg)
            {
                _Mgr.addPackage( _RES_ABSPATH_(strPkg) , strPath?strPath:L"");

            }
            else if(strPath)
            {
                _Mgr.addPath( _RES_ABSPATH_(strPath) );
            }
        }
        else
        {
            if(strPkg)
            {
                _Mgr.addPackage( _XEVOL_ABSPATH_(strPkg) , strPath?strPath:L"");

            }
            else if(strPath)
            {
                _Mgr.addPath( _XEVOL_ABSPATH_(strPath) );
            }
        }
	}
	return true;
}


END_NAMESPACE_XEVOL3D

#endif

