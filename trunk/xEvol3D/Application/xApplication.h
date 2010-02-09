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

#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#include <vector>
#include <map>
#include <string>
using namespace std;

#include "xWindow.h"
#include "xListener.h"
#include "xApplicationLayer.h"

BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_   IMsgListener;
class _XEVOL_BASE_API_   IFrameUpdateObject
{
public:
	virtual const wchar_t* name() = 0;
	virtual bool           updateFrame(int passedTime , unsigned int thisTime) = 0;
	virtual void           turnOn(bool bTurnOn) = 0;
	virtual bool           isTurnOn() = 0;
};


class _XEVOL_BASE_API_  xFPSCaculator
{
public:
	xFPSCaculator();
	void    reset();
	float   caculateFPS(int incFrame = 1);
	void    forceFPS(long fps);
	int     getForceFPS();
	long    getCurrentTick();
	float   getFPS();
private:

	bool    m_bForceFPS;
	long    m_PassedTime;
	long    m_TotalFrame;
	float   m_FPS;
	long    m_LastTime;

	long    m_frameTime;
};

//Application为一个应用．
//一个程序可以有很多的Application
class _XEVOL_BASE_API_  IApplication
{
public:
	enum ApplicationState
	{
		AS_RUN  = 1,
		AS_STOP = 2,
		AS_DIE  = 3,
		AS_PAUSE = 4
	};
	//-----------------------------
	//帧计数器
	//-----------------------------

public:

	//------------------------------------------------
	//构造函数。
	//------------------------------------------------
	IApplication();
	virtual ~IApplication();

	//----------------------------------------------
	//窗口消息处理函数。
	//----------------------------------------------
	virtual  bool   dispatchMsg(xWindowMsg& msg);
	virtual  bool   onMessage(xWindowMsg& msg);
	void            addMsgListener(IMsgListener* msgListenter);
	bool            removeMsgListener(IMsgListener* msgListenter);

	//-----------------------------------------------
	//事件处理函数。窗口类不需要事件处理。
	//事件全部由应用处理
	//-----------------------------------------------
	void             addFrameUpdateObj(IFrameUpdateObject* pObj);
	bool             removeFrameUpdateObj(IFrameUpdateObject* pObj);

	virtual bool     init(int argc , wchar_t** argv);
	virtual bool     uninit();
	virtual bool     prevUpdateFrame(long passedTime);
	virtual bool     postUpdateFrame(long passedTime);
	virtual void     updateFrame(long passedTime);
	//------------------------------------------------
	//计算帧率
	//------------------------------------------------
	xFPSCaculator*   getFPSCounter();
	void             resetFPSCounter();
	void             forceFPS(long fps){m_FPSCounter.forceFPS(fps);}
	int              getForceFPS(){ return m_FPSCounter.getForceFPS();}
	long             getCurrentTick(){return m_FPSCounter.getCurrentTick();}
	float            getFPS(){return (float)m_FPSCounter.getForceFPS();}
	//------------------------------------------------
	//应用和窗口之间的关系。
	//------------------------------------------------
	void             setMainWindow(IWindow* pMainWnd);
	IWindow*         getMainWindow();
	//-------------------------------------------------
	//应用的状态
	//-------------------------------------------------
	ApplicationState getApplicationState();
	void             setApplicationState(ApplicationState state);
	bool             isRun();

	//应用程序的真正逻辑
	bool                     registeAppLayer(IAppLayer* appLayer);
	bool                     unregisteAppLayer(IAppLayer* appLayer);
	bool                     unregisteAppLayer(const wchar_t* AppLayerName);
	IAppLayer*               findAppLayer(const wchar_t* appLayerName);
	IAppLayer*               getCurAppLayer();

	virtual bool             exitAppliction();
	virtual IAppLayer*       handleAppLayer(const wchar_t* AppLayerName);
	virtual IAppLayer*       handleAppLayer(IAppLayer* appLayer);

private:
	//所有的Listenr.
	vector<IMsgListener*>       m_MsgListeners;
	vector<IFrameUpdateObject*> m_FrameUpdateObjs;
protected:
	ApplicationState m_State;
	xFPSCaculator    m_FPSCounter;
	IWindow*         m_pMainWindow;
	IAppLayer*       m_pCurAppLayer;


	typedef vector<IAppLayer*> AppLayers;
	AppLayers                  m_AppLayers;

};


template<typename _Type> class TFrameUpdateObjHolder : public IFrameUpdateObject
{
	_Type*    m_pObject;
	bool      m_bTurnOn;
public:
	TFrameUpdateObjHolder(_Type* pObj , bool turnOn = false)
	{
		m_bTurnOn = turnOn;
		m_pObject = pObj;
	};
	~TFrameUpdateObjHolder()
	{
		if(IPlatform::singleton() && IPlatform::singleton()->IPlatform::singleton()->getApplication()  )
			IPlatform::singleton()->getApplication()->removeFrameUpdateObj(this);
	};
	const wchar_t* name()
	{
		if(m_pObject)return m_pObject->name();
		return NULL;
	}
	bool  updateFrame(int passedTime , unsigned int thisTime)
	{
		if(m_pObject){return	m_pObject->updateFrame(passedTime , thisTime);	}
		return false;
	}
	void  turnOn(bool bTurnOn)	{	m_bTurnOn = bTurnOn;}
	bool  isTurnOn(){	return m_bTurnOn;}
};

END_NAMESPACE_XEVOL3D

#endif


