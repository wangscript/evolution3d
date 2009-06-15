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

#ifndef __XML_MANAGER_H__
#define __XML_MANAGER_H__
#include "xResPackageMgr.h"
#include "xXmlDocument.h"

BEGIN_NAMESPACE_XEVOL3D
class  _XEVOL_BASE_API_  xXMLLoader : public xResPathManager
{
protected:
	unsigned int _getResSize(xXmlDocument* pRes);
	virtual bool _isResLoaded(xXmlDocument* pRes);
	virtual bool _loadResource(const std::ds_wstring& strResName   , XEvol3D::xXmlDocument* & pRes , int& ResSize, unsigned int arg);
	virtual bool _unloadResource(const std::ds_wstring& strResName , XEvol3D::xXmlDocument* & pRes , unsigned int& TotalResSize);
	virtual void _deleteResource(const std::ds_wstring& strResName , XEvol3D::xXmlDocument* pRes);
};

typedef TResHandle <xXmlDocument , std::ds_wstring , xXMLLoader     > HXMLDoc;
typedef TLRUPolicyResMgr<xXmlDocument , std::ds_wstring , xXMLLoader> xXMLDocMgr;

END_NAMESPACE_XEVOL3D

#endif

