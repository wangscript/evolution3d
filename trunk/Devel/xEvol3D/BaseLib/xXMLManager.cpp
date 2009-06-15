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

#include "xXmlManager.h"

#include "../xcomdoc/xdocfstream.h"
#include "../io/mstream.h"


BEGIN_NAMESPACE_XEVOL3D
bool xXMLLoader::_isResLoaded(xXmlDocument* pRes)
{
	if( pRes == NULL )
		return false;
	return pRes->countNode() > 0;
}
unsigned int xXMLLoader::_getResSize(xXmlDocument* pRes)
{
	return 1;
}

bool xXMLLoader::_loadResource(const std::ds_wstring& strResName,xXmlDocument* & pRes , int& ResSize, unsigned int arg)
{
	
	ResSize = 0;
	bool needDeletePRes = false;
	if(pRes == NULL)
	{
		needDeletePRes = true;
		pRes = new xXmlDocument;
	}

	if(XEvol_IsAbsPath(strResName))
	{
		if(pRes->load(strResName.c_str() , arg != 0 ) )
		{
			ResSize = 1;
			return true;
		}
		else
		{
			pRes->unload();
		}			
	}
	else
	{
		ds_wstring full_name;
		int nResItem = (int)this->m_Packages.size();
		for(int i = 0 ; i < nResItem ; i++)
		{
			xResPathManager::ResPackageItem& item = this->m_Packages[i];
			if(item.m_Package.failed())
			{
				full_name = ds_wstring(item.m_ResDir) + strResName;
				if(pRes->load(full_name.c_str() , arg != 0) )
				{
					ResSize = 1;
					return true;
				}
			}
			else
			{
				full_name = item.m_ResDir + strResName;
				xcomdocstream* pstream = item.m_Package.open_stream(full_name.c_str());
				if(pstream)
				{
					xcdstream _stream(pstream);
					if(pRes->load(_stream , arg != 0 ) )
					{
						ResSize = 1;
						//XEVOL_LOG(eXL_DEBUG_TIPS, L"Load from package, search index = %d name = %s\n",i,strResName.c_str() );
						pstream->release();
						return true;
					}
				}

			}

		}
	}


	//如果是创建出来的就删除掉
	if(needDeletePRes)
	{
		delete pRes;
		pRes = NULL;
	}

	return false;
}

bool xXMLLoader::_unloadResource(const std::ds_wstring& strResName , xXmlDocument* & pRes , unsigned int& TotalResSize)
{
	TotalResSize -= 1;
	pRes->unload();
	delete pRes;
	pRes = NULL;
	return true;
}

void xXMLLoader::_deleteResource(const std::ds_wstring& strResName , xXmlDocument* pRes)
{
	if(pRes == NULL)
		return ;
	pRes->unload();
	delete pRes;
	pRes = NULL;
}  

END_NAMESPACE_XEVOL3D

