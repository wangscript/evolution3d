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

#ifndef __TEMPLATE_PKG_RESOURCE_PATH_MGR_H__
#define __TEMPLATE_PKG_RESOURCE_PATH_MGR_H__
#include "xEvol3DBaseInc.h"
#include "stl_dllalloc.h"
#include <iostream>
#include "../xcomdoc/xcomdoc.h"
BEGIN_NAMESPACE_XEVOL3D
inline bool XEvol_ConvertToSystemFile(std::ds_wstring& _filename)
{
	size_t len = _filename.size() ;
	for(size_t i = 0 ; i < len ; i ++)
	{
		if(_filename[i] == PATH_SPLITTER1 || _filename[i] == PATH_SPLITTER2) _filename[i] = PATH_SPLITTER;
	}
	return true;
}
inline std::ds_wstring XEvol_RelativePath(const wchar_t* _filename , const wchar_t* _basePath)
{
	std::ds_wstring fileName = _filename;
	std::ds_wstring basePath = _basePath;
    XEvol_ConvertToSystemFile(fileName);
	XEvol_ConvertToSystemFile(basePath);
	std::ds_wstring::size_type pos = fileName.find(basePath);
	if(pos != 0)
		return L"";

	std::ds_wstring ret = fileName.c_str() + basePath.length();
	return ret;
}

//-----------------------------------------------------------------------------------
//一个通用的ResMgr，
//只要求ResType实现了XNullRes的接口
//-----------------------------------------------------------------------------------
struct xResPkgPathItem
{
	std::ds_wstring   m_ResDir;
	xcomdoc           m_Package;
};

class _XEVOL_BASE_API_  xResPathManager
{
public:
	void clearAllSearchPath()
	{
		m_Packages.clear();
	}

	xResPathManager(const wchar_t* _name = NULL)
	{
		m_strPathMgrName[0]=0;
		setPathMgrName(_name);		
	}

	void setPathMgrName(const wchar_t* _name)
	{
		if(_name)
		{
			wcscpy(m_strPathMgrName , _name);
		}
	}
	const wchar_t* pathMgrName()
	{
		return m_strPathMgrName;
	}
	~xResPathManager()
	{
		clearAllSearchPath();
	}

	bool addPackage(xcomdoc& package,const wchar_t* ResDir)
	{
		size_t index = this->m_Packages.size();
		this->m_Packages.push_back(xResPkgPathItem());
		xResPkgPathItem& item = m_Packages[index];
		item.m_Package = package;
		item.m_ResDir  = ResDir;
		return true;
	}

	bool addPackage(const wchar_t* package_file,const wchar_t* ResDir)
	{
		xResPkgPathItem item;
		item.m_Package.open(package_file,xcdm_read,0,false,true);
		if(item.m_Package.failed())
			return false;
		item.m_ResDir  = ResDir;
		this->m_Packages.push_back(item);
		return true;
	}

	bool addPath(const wchar_t* ResDir)
	{
		size_t nPackages = m_Packages.size() ;
		for(size_t i = 0 ; i < nPackages;  i ++)
		{
			if(m_Packages[i].m_Package.failed() == false)
				continue;

			if(m_Packages[i].m_ResDir == ResDir)
				return false;
		}

		xResPkgPathItem item;
		item.m_ResDir  = ResDir;
		this->m_Packages.push_back(item);
		return true;
	}

	xResPkgPathItem& packageItem(int index)
	{
		return m_Packages[index];
	}

	std::ds_wstring relativePath( const wchar_t* _path )
	{
		std::ds_wstring _ret = L"";
		for(size_t i = 0  ; i < nPackageItem() ; i ++)
		{
			if(m_Packages[i].m_Package.failed())
			{
				_ret = XEvol_RelativePath(_path , m_Packages[i].m_ResDir.c_str() );
				if(_ret.length() > 0)
					return _ret;
			}
		}
		return _ret;
	}

	size_t nPackageItem()
	{
		return m_Packages.size();
	}
protected:
	wchar_t      m_strPathMgrName[32];
	typedef std::vector<xResPkgPathItem , dllsafe_alloc<xResPkgPathItem> > ResPkgItems;
	ResPkgItems  m_Packages;

};


class _XEVOL_BASE_API_ xResPathMgrProxy
{
public:
	bool               addPackage(xcomdoc& package,const wchar_t* ResDir);
	bool               addPackage(const wchar_t* package_file,const wchar_t* ResDir);
	bool               addPath(const wchar_t* ResDir);
	xResPkgPathItem&   packageItem(int index);
	std::ds_wstring    relativePath( const wchar_t* _path );
	void               setPathMgr(xResPathManager* pPathMgr);
	xResPathManager*   getPathMgr();
	size_t             nPackageItem();

	xResPathMgrProxy()
	{
		m_pPathMgr = NULL;
	}
protected:
	xResPathManager*   m_pPathMgr;
};

inline bool   xResPathMgrProxy::addPackage(xcomdoc& package,const wchar_t* ResDir)
{
	if(m_pPathMgr == NULL) return false;
	return m_pPathMgr->addPackage(package , ResDir);
}

inline bool   xResPathMgrProxy::addPackage(const wchar_t* package_file,const wchar_t* ResDir)
{
	if(m_pPathMgr == NULL) return false;
	return m_pPathMgr->addPackage(package_file , ResDir);
}

inline bool   xResPathMgrProxy::addPath(const wchar_t* ResDir)
{
	if(m_pPathMgr == NULL) return false;
	return m_pPathMgr->addPath(ResDir);
}
inline xResPkgPathItem&   xResPathMgrProxy::packageItem(int index)
{
	return m_pPathMgr->packageItem(index);
}

inline size_t             xResPathMgrProxy::nPackageItem()
{
	if(m_pPathMgr == NULL) return 0;
	return m_pPathMgr->nPackageItem();
}
inline std::ds_wstring   xResPathMgrProxy::relativePath( const wchar_t* _path )
{
	if(m_pPathMgr == NULL) return L"";
	return m_pPathMgr->relativePath(_path);
}

inline void  xResPathMgrProxy::setPathMgr(xResPathManager* pPathMgr)
{
	m_pPathMgr = pPathMgr;
}

inline xResPathManager*  xResPathMgrProxy::getPathMgr()
{
	return m_pPathMgr;
}

//*******************************************************

END_NAMESPACE_XEVOL3D

#endif

