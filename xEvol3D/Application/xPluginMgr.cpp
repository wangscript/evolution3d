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
#include "xPluginMgr.h"
#include "../BaseLib/xStringHash.h"
#include "../fs/xFileSystem.h"
#include <string>
using namespace std;

BEGIN_NAMESPACE_XEVOL3D

xPluginMgr::xPluginMgr(){}
xPluginMgr::~xPluginMgr(){}
xPluginMgr* xPluginMgr::singleton()
{
	static xPluginMgr pm;
	return &pm;
}

xPlugin::xPlugin()
{
    _reset();
}

xPlugin::~xPlugin()
{
    unload();
}

bool xPlugin::startPlugin()
{
   return  m_pluginObject->start(m_FileName,m_PathName);
}

bool xPlugin::stopPlugin()
{
   return m_pluginObject->stop();
}

void* xPlugin::createObject(const wchar_t* objectName,const wchar_t * _arg)
{
	if(m_pluginObject == NULL)  return NULL;
    return m_pluginObject->createObject(objectName,_arg);
}

void xPlugin::_reset()
{
    m_hDLL = NULL;
    m_pluginObject = NULL;
    memset(m_FileName,0,512);
    memset(m_PathName,0,256);
    memset(&m_pluginInfo , 0 , sizeof(m_pluginInfo) );
}

bool xPlugin::unload()
{
	if(m_hDLL && m_pluginObject && m_pluginObject->nObject() > 0 )
	{
		const wchar_t** objectList = m_pluginObject->objectList();
		while( *objectList != NULL)
		{
			xPluginMgr::singleton()->unregObject(*objectList);
			objectList ++ ;
		}
		stopPlugin();
		m_hDLL->unload();
		m_hDLL->ReleaseObject();
		_reset();
	}
    return true;
}



bool xPlugin::load(xDLLModule* pDLL , const wchar_t* _pathName)
{
	if(pDLL == NULL)
		return false;

    wcsncpy(m_PathName , _pathName , 512);
    size_t i = wcslen(m_PathName);
    for(;i >= 0 ; i --)
    {
        if(m_PathName[i] == '\\' || m_PathName[i] =='/')
        {
            m_PathName[i] = 0;
            break;
        }
    }
    wcsncpy(m_FileName,&m_PathName[i+1],256);

	//Get the plugin object
	GETPLUGIN_OBJET_FUNC GetPluginObject    = (GETPLUGIN_OBJET_FUNC)pDLL->getProcAddress( PLUGIN_ENTRYPOINT_NAME );
	if(GetPluginObject == NULL)
	{
		return false;
	}

	//Build the plugin
	m_pluginObject = GetPluginObject();
	if(m_pluginObject == NULL)
	{
		return false;
	}

	if(startPlugin() == false)
		return false;
	m_pluginObject->info(&m_pluginInfo);
	m_hDLL = pDLL;

	//Register All support Object;	
	if(m_pluginObject && m_pluginObject->nObject() > 0 )
	{
		const wchar_t** objectList = m_pluginObject->objectList();
		while( *objectList != NULL)
		{
			xPluginMgr::singleton()->registeObject(*objectList,this);
			objectList ++ ;
		}
	}

	return true;
}	

const wchar_t* xPlugin::pathName()
{
    return m_PathName;
}

const wchar_t*  xPlugin::fileName()
{
    return m_FileName;
}


//===============================
void* xPluginMgr::createObject(const wchar_t* _name,const wchar_t * _arg)
{
	int hsID = xStringHash(_name);
	xPluginMap::iterator pos = m_objCreateMap.find(hsID);
	if(pos != m_objCreateMap.end() )
	{
		xPlugin* plugin = pos->second;
		return plugin->createObject(_name,_arg);
	}
	return NULL;
}

void xPluginMgr::registeObject(const wchar_t* _name , xPlugin* plugin)
{
	unsigned int hsID = xStringHash(_name);
	xPluginMap::iterator pos = m_objCreateMap.find(hsID);
	if(pos != m_objCreateMap.end() )
	{
		XEVOL_LOG(eXL_DEBUG_HIGH,L"Object name in plugin %s conflict\n",pos->second->fileName() );
		XEVOL_LOG(eXL_DEBUG_HIGH,L"    Object name: %s\n",_name);
		XEVOL_LOG(eXL_DEBUG_HIGH,L"    Plugin Name: %s\n",plugin->fileName() );
		return ;
	}
	m_objCreateMap.insert(map<int, xPlugin*>::value_type(hsID,plugin) );
	return ;
}

void xPluginMgr::unregObject(const wchar_t* _name)
{
	unsigned int hsID = xStringHash(_name);
	xPluginMap::iterator pos = m_objCreateMap.find(hsID);
	if(pos != m_objCreateMap.end() )
	{
		m_objCreateMap.erase(pos);
	}
	return ;
}

xPlugin* xPluginMgr::loadPlugin(const wchar_t* pluginFile)
{
	xDLLModule* pDLLModule = xOperationSys::singleton()->createDLL(pluginFile);
	 if(pDLLModule == NULL)
		 return NULL;
	 xPlugin* pPlugin = new xPlugin;
	 if( pPlugin->load(pDLLModule, pluginFile) == false)
	 {
		 pPlugin->unload();
		 delete pPlugin;
		 pDLLModule->ReleaseObject();
		 return NULL;
	 }
     
	 if( __addPlugin(pPlugin) )
	 {
		 XEVOL_LOG(eXL_DEBUG_NORMAL,L"Find a plugin file = %s \n"  , pluginFile);
		 XEVOL_LOG(eXL_DEBUG_NORMAL,L"              name = %s \n"  , pPlugin->info().m_Name);
		 XEVOL_LOG(eXL_DEBUG_NORMAL,L"              ID   = 0X%X \n", pPlugin->info().m_ID);
		 XEVOL_LOG(eXL_DEBUG_NORMAL,L"              desc = %s \n"  , pPlugin->info().m_Description);
		 return pPlugin;
	 }
	 xPlugin* retPlugin = find(pPlugin->info().m_Name);
	 delete pPlugin;
	 return retPlugin;
}

xPlugin*  xPluginMgr::find(const wchar_t* name)
{
	size_t nPlugin = m_Plugins.size();
	for(size_t i = 0 ; i <  nPlugin; i ++)
	{
		xPlugin* plugin = m_Plugins[i];
		if(plugin->info().m_Name == wstring(name) )
			return plugin;
	}
	return NULL;
}

bool xPluginMgr::__addPlugin(xPlugin* plugin)
{
   if(find(plugin->info().m_Name) == NULL)
   {
	   m_Plugins.push_back(plugin);
	   return true;
   }
   return false;
}
void xPluginMgr::clear()
{
	 size_t nPlugin = m_Plugins.size();
     for(size_t i = 0 ; i < nPlugin ; i ++)
	 {
		 xPlugin* plugin = m_Plugins[i];
		 plugin->unload();
		 delete plugin;
	 }
	 m_Plugins.clear();
}
class xPluginFileListenner : public IDirEnumListenner
{
public:
	ds_wstring_vector m_Files;
private:
	bool onFindFile(xDirEnumItem& item)
	{
        if(item.m_isDir == false)
		{
            m_Files.push_back(item.m_FullName);
		}
		return true;
	}
};

int  xPluginMgr::__loadPluginDirWithExt(const wchar_t* pluginDir , const wchar_t* _ext)
{
	int nLoading = 0;
	xDirEnum dirEnum;
    xPluginFileListenner listener;
	dirEnum.addListenner(&listener);
	dirEnum.enumDir(pluginDir,false);
	size_t nFiles = listener.m_Files.size() ;
	for(size_t i = 0 ; i <  nFiles ; i ++)
	{
        const wchar_t*  strFindFile = listener.m_Files[i].c_str();
		wstring _file_ext = xFileSystem::singleton()->getFileExtName( strFindFile );
		if(_file_ext == _ext)
		{
			if(loadPlugin(strFindFile) != NULL)
				nLoading ++;
		}
	}
	return nLoading;
}

int  xPluginMgr::loadPluginDir(const wchar_t* pluginDir)
{
	int nLoading = 0;
	XEVOL_LOG(eXL_DEBUG_NORMAL,L"Search Plugin in directory : %s \n" , pluginDir);
#ifdef _DEBUG
    nLoading += __loadPluginDirWithExt(pluginDir, L"xpd");
#else
	nLoading += __loadPluginDirWithExt(pluginDir, L"xpi");
#endif
	XEVOL_LOG(eXL_DEBUG_NORMAL,L"Find %d plugin \n" , nLoading);
	return nLoading;
}

int  xPluginMgr::loadPluginDir(const wchar_t* pluginDir, const wchar_t* _ext)
{
    int nLoading = 0;
    XEVOL_LOG(eXL_DEBUG_NORMAL,L"Search Plugin in directory : %s \n" , pluginDir);
    nLoading += __loadPluginDirWithExt(pluginDir, _ext);
    XEVOL_LOG(eXL_DEBUG_NORMAL,L"Find %d plugin \n" , nLoading);
    return nLoading;
}

const wchar_t*  xPluginMgr::compilerID()
{
#ifdef _DEBUG 
	return L"Debug";
#else
	return L"Release";
#endif
}

END_NAMESPACE_XEVOL3D



