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

#ifndef __XEVOL_PLUGIN_MGR_H__
#define __XEVOL_PLUGIN_MGR_H__
#include "../BaseLib/xEvol3DBaseInc.h"
#include "../fs/xFileSystem.h"
#include "../OperationSys/xOperationSys.h"
#include <vector>
#include <map>

#ifdef _DEBUG
#define PLUGIN_ENTRYPOINT        GetXEvol3DPluginObjectD
#define PLUGIN_ENTRYPOINT_NAME L"GetXEvol3DPluginObjectD"
#else
#define PLUGIN_ENTRYPOINT        GetXEvol3DPluginObject
#define PLUGIN_ENTRYPOINT_NAME L"GetXEvol3DPluginObject"
#endif


BEGIN_NAMESPACE_XEVOL3D

enum  ePluginType
{
    ePT_Renderer,
	ePT_RenderSystem,
	ePT_Platform,
	ePT_Script,
	ePT_Subsystem,
	ePT_ObjectCreator,
};

struct xPluginInfo 
{
	wchar_t     m_Name[32];
	wchar_t     m_Description[256];
	int         m_ID;
	ePluginType m_Type;

};

class IPluginObject
{
public:
	virtual	bool            start(const wchar_t* pluginName , const wchar_t* pluginPath) = 0;
	virtual	void            info(xPluginInfo* info) = 0;
	virtual	bool            stop() = 0;
	virtual unsigned int    nObject() = 0;
	virtual	void*           createObject(const wchar_t* objName, const void * arg) = 0;
	virtual	const wchar_t** objectList() = 0;
};
typedef IPluginObject* (*GETPLUGIN_OBJET_FUNC)();

class _XEVOL_BASE_API_  xPlugin
{
public:
   xPlugin();
   virtual ~xPlugin();
   bool               startPlugin();
   bool               stopPlugin();
   void*              createObject(const wchar_t* objectName,const wchar_t * _arg);
   bool               unload();
   const wchar_t*     pathName();
   const wchar_t*     fileName();
   bool               load( xDLLModule* pDLL , const wchar_t* pathName);
   const xPluginInfo& info() { return m_pluginInfo; };

protected:
   void               _reset();
protected:
   xPluginInfo       m_pluginInfo;
   wchar_t           m_PathName[512];
   wchar_t           m_FileName[256];

   xDLLModule*       m_hDLL;
   IPluginObject*    m_pluginObject;
};

class _XEVOL_BASE_API_  xPluginMgr
{
	typedef ds_vector(xPlugin*)            xPlugins;
	typedef ds_map(unsigned int, xPlugin*) xPluginMap;

	xPlugins    m_Plugins;
	xPluginMap  m_objCreateMap;
	xPluginMgr();
	~xPluginMgr();
public:
	static xPluginMgr* singleton();
	xPlugin*           loadPlugin(const wchar_t* pluginFile);
	int                loadPluginDir(const wchar_t* pluginDir);
    int                loadPluginDir(const wchar_t* pluginDir, const wchar_t* _ext);
	void               clear();
	xPlugin*           find(const wchar_t* name);
	void*              createObject(const wchar_t* _name,const wchar_t * _arg);
	void               registeObject(const wchar_t* _name , xPlugin* plugin);
	void               unregObject(const wchar_t* _name);
	const wchar_t*     compilerID();
protected:
	bool              __addPlugin(xPlugin* plugin) ;

	int               __loadPluginDirWithExt(const wchar_t* pluginDir , const wchar_t* _ext);
};

END_NAMESPACE_XEVOL3D
#endif
