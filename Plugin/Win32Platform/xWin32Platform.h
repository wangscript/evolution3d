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

#ifndef __WIN32__PLATFORM_H__
#define __WIN32__PLATFORM_H__
#include <windows.h>
#include <map>
#include <vector>
using namespace std;

#include <Application/xPlatform.h>
#include <Application/xApplication.h>
#include "xWindowImp.h"
BEGIN_NAMESPACE_XEVOL3D

#define MAPVKEY(x)   (x)
#define UNMAPVKEY(x) (x)


//Platform 里保存了所有为该Platform创建的窗口类．

class CKeyBoardInput  : public IKeyboard 
{
public:
	CKeyBoardInput()
	{
		memset(m_key, 0, sizeof(m_key));
	}

	~CKeyBoardInput()
	{

	}

	bool          isKeyPressed(VIRTUAL_KEY vKey) 
	{
		::GetKeyboardState(m_key);	
		int sysVKey = UNMAPVKEY(vKey);
		return (m_key[sysVKey]&0x80) != 0;
	}

	bool          isCharKey(VIRTUAL_KEY vKey)
	{
		::GetKeyboardState(m_key);	
		unsigned short charCode;
		int nSysVKey = UNMAPVKEY(vKey);
		UINT nScanCode = MapVirtualKey(nSysVKey , 0);
		int nChar = ToAscii(nSysVKey , nScanCode, m_key ,&charCode,0);
		return nChar > 0;

	}

	bool          isKeyToggle(VIRTUAL_KEY vKey)
	{
		::GetKeyboardState(m_key);	
		int sysVKey = UNMAPVKEY(vKey);
		return m_key[sysVKey]&0x01;
	}

	unsigned short convert2Char(VIRTUAL_KEY vKey)
	{
		::GetKeyboardState(m_key);	
		unsigned short charCode;
		int nSysVKey = UNMAPVKEY(vKey);
		UINT nScanCode = MapVirtualKey(nSysVKey , 0);
		int nChar = ToAscii(nSysVKey , nScanCode, m_key ,&charCode,0);
		if(nChar == 0) charCode = 0;
		return charCode;
	}

	unsigned char getSysVirtualKey(VIRTUAL_KEY vKey)
	{
		return UNMAPVKEY(vKey);
	}
private:
	BYTE   m_key[256];
	WPARAM m_mouse_Key;
};

class PlatformWin32 : public  IPlatform
{
	IMPL_BASE_OBJECT_INTERFACE(PlatformWin32);
protected:
	int                       m_FrameTime;
	int                       m_startTickt;
public:
	IWindow*                  m_pWindowInCreating;
    std::vector<std::wstring> m_vRegistedClass;
public:
	PlatformWin32();
	~PlatformWin32();
	const wchar_t*  name();
	//初始化平台
	bool            open(int argc , wchar_t** argv);

	//关闭平台
	bool            close() ;
	//应用程序退出
	bool            quit() ;

	void            getMousePosition(x2DPoint& point);

	bool            setDisplayMode(int w, int h, int cl_bits,int display_fre);
	//创建一个窗口

	IWindow*        createWindow(WIN_CREATE_STRUCT& createStruct , const xXmlNode* params = NULL) ;

	IRenderApi*     createRenderApi(int iWnd , const wchar_t* RenderApi , const xXmlNode* params);
	IWindow*        createRenderWindow(IRenderApi** pOutRenderApi ,  WIN_CREATE_STRUCT& createStruct, const wchar_t* RenderApi , const xXmlNode* params = NULL);

	//关闭一个窗口
	bool            closeWindow(IWindow* pWnd) ;
	//抓取窗口消息
	bool            grabWindowEvent(IWindow* pWnd);
	bool            releaseWindowEvent(IWindow* pWnd);


	//窗口管理
	void            managerWindow(IWindow* pWindow);
	void            unmanagerWindow(IWindow* pWindow);


	//得到普通精度的时间脉搏
	int             getTicktCount() ;
	int             getFrameTime();
	int             getElapsTime();
	//进入消息循环
	bool            messageLoop() ;

	bool            dealMessage();

	//处理消息
	bool            dispatchMsg(xWindowMsg & msg);
	bool            setMessageTranslator(ISysEventTranslator* pTranslator);
	IKeyboard*      getKeyboard(){return &m_Keyboard ; }
private:
	bool            insertWindow(IWindow*);
	bool            insertWindow(IWindow* , HWND hWnd);
public:
	HINSTANCE       getInstance()
	{
		return m_hInstance;
	}


	bool      _registerWindow(WIN_CREATE_STRUCT& createStruct);
public:
	static    LONG CALLBACK windowProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam); 
private:
    CKeyBoardInput  m_Keyboard;
	static bool     m_bKeyPressed[256];
	typedef         map<HWND,IWindow*> WindowMap;
	WindowMap              m_Windows;
	HINSTANCE              m_hInstance;
	static Win32WindowImp* m_inCreatingWnd;
};

class PlatformWin32Factory: public IPlatformFactory
{
public:	
    const wchar_t* name();

    IPlatform* createInstance();
    PlatformWin32Factory();
    static PlatformWin32Factory* g_pInstance;
};

END_NAMESPACE_XEVOL3D


#endif
