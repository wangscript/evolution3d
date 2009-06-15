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

#ifndef __PLATFORM_H__
#define __PLATFORM_H__
#include "xWindow.h"
#include "xApplication.h"
#include "xVirtualKey.h"
#include "../BaseLib/stl_dllalloc.h"
#include "../BaseLib/xXmlDocument.h"
#include "../BaseLib/logstream.h"
using namespace std;
BEGIN_NAMESPACE_XEVOL3D
//----------------------------------------------------------------

//----------------------------------------------------------------


template<typename T> class TScopeLocker
{
	T* m_pLocker;
public:
	TScopeLocker(T* pT)
	{
		m_pLocker=pT;
		if(m_pLocker) m_pLocker->lock();
	};
	~TScopeLocker()
	{
		if(m_pLocker)	m_pLocker->unlock();
	};
};

class _XEVOL_BASE_API_  IRenderApi;
class _XEVOL_BASE_API_  ISysEventTranslator
{
public:
	virtual bool translator(xWindowMsg& _winMsg) = 0;
};

class _XEVOL_BASE_API_  IPlatform : public IBaseObject
{
protected:
	ISysEventTranslator* m_pEventTranslator;
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IPlatform)
public:

	IPlatform();
    virtual ~IPlatform();
	static  IPlatform*      singleton();
	virtual const wchar_t*  name() = 0;
	//初始化平台
	virtual bool            open(int argc , wchar_t** argv) = 0;
	//关闭平台
	virtual bool            close() = 0;
	//应用程序退出
	virtual bool            quit() = 0;
	
	virtual void            getMousePosition(x2DPoint& point) = 0;

	virtual bool            setDisplayMode(int w, int h, int cl_bits,int display_fre) = 0;
	//创建一个窗口
	virtual IWindow*        createWindow(WIN_CREATE_STRUCT& createStruct,  const xXmlNode* params = NULL) = 0;

	//创建一个渲染窗口
	//Win32 + OpenGL < Win32Platform Window -> OpenGL Win32 RenderInitor ->
	//Win32 + D3D    < Win32Platform Window -> D3D Win32 RenderInitor ->
	//SDL + OpenGL   < SDL Platform (OpenGL) -> OpenGL Renderer
	//XLib + OpenGL  < Unix/Linux Platform  ->  GLX OpenGL RendererInitor->
	virtual IWindow*        createRenderWindow(IRenderApi** pOutRenderApi ,  WIN_CREATE_STRUCT& createStruct, const wchar_t* RenderApi , const xXmlNode* params = NULL) = 0;
    virtual IRenderApi*     createRenderApi(int iWnd , const wchar_t* RenderApi , const xXmlNode* params = NULL) = 0;

	//关闭一个窗口
	virtual bool            closeWindow(IWindow* pWnd) = 0;

	//抓住一个窗口的消息
	virtual bool            grabWindowEvent(IWindow* pWnd) = 0;
	virtual bool            releaseWindowEvent(IWindow* pWnd) = 0;

	//窗口管理
	virtual void            managerWindow(IWindow* pWindow) = 0;
	virtual void            unmanagerWindow(IWindow* pWindow) = 0;

	//得到普通精度的时间脉搏
    virtual int             getFrameTime()  = 0;
	virtual int             getElapsTime()  = 0;
	//进入消息循环
	virtual bool            messageLoop() = 0;

	//处理消息
	virtual bool            dispatchMsg(xWindowMsg & msg) = 0;
	virtual bool            setMessageTranslator(ISysEventTranslator* pTranslator) = 0;

	//设置当前有效果的应用．
	bool                    setApplication(IApplication* pApp);
	//得到当前有效的应用
	IApplication*           getApplication();
protected:
	IApplication*           m_pCurrentApplication;
};

class _XEVOL_BASE_API_  IPlatformFactory
{
public:	
	virtual const wchar_t*    name() = 0 ;
	virtual IPlatform*        createInstance() = 0;

public:
	static  IPlatform*        create(const wchar_t*  name);
	static  bool              registe(IPlatformFactory* creater);
	static  int               numOfPlatform();
	static  IPlatformFactory* getByIndex(int i) ;
};

inline bool xR_CreatePlatform(const wchar_t* name)
{
	return IPlatformFactory::create(name) != NULL;
}
END_NAMESPACE_XEVOL3D
#endif

