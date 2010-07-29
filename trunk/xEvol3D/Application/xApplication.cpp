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
#include "xApplication.h"
#include "xListener.h"
#include "xPlatform.h"
#include "../OperationSys/xOperationSys.h"
BEGIN_NAMESPACE_XEVOL3D


xFPSCaculator::xFPSCaculator()
{
	m_PassedTime = 0;
	m_TotalFrame = 0;
	m_FPS = 0;
	m_LastTime = xGetTickCount();     
	m_bForceFPS = false;
}
void xFPSCaculator::reset()
{
	m_PassedTime = 0;
	m_TotalFrame = 0;
	m_FPS = 0;
	m_LastTime = xGetTickCount();          
}
void  xFPSCaculator::forceFPS(long fps)
{
	if(fps == 0) 
	{
		m_bForceFPS = false;
		return ;
	}
	m_frameTime = 1000/fps;
	m_bForceFPS = true;
}
int   xFPSCaculator::getForceFPS()
{
	if(m_frameTime == 0)
		return 0;
	return 1000/m_frameTime;
}
float xFPSCaculator::caculateFPS(int incFrame)
{
	m_TotalFrame += incFrame;
	long thisTime=xGetTickCount();
	if(m_bForceFPS)//强制帧率的帧率计算
	{
		while( ((int)thisTime-(int)m_LastTime) < m_frameTime)
		{
			int sTime = m_frameTime - ((int)thisTime-(int)m_LastTime) - 10 ;
			if(sTime > 0) xSleep(sTime);
			thisTime=xGetTickCount();
		}
		m_FPS =  1000.0f/(thisTime-m_LastTime);
		m_LastTime=thisTime;
	}//非强制帧率的帧率计算
	else
	{
		if( (thisTime-m_LastTime)>1000)
		{
			m_FPS= m_TotalFrame*1000.0f/(thisTime-m_LastTime);
			m_TotalFrame=0;
			m_LastTime=thisTime;
		}
	}
	return m_FPS;
}

long xFPSCaculator::getCurrentTick()
{
	return xGetTickCount();
}

float xFPSCaculator::getFPS()
{
	return m_FPS;
}


IApplication::IApplication()
{
	m_pMainWindow = NULL;
	m_FPSCounter.reset();
	m_State       = AS_PAUSE;
	m_pCurAppLayer = NULL;
}
IApplication::~IApplication()
{

}
bool     IApplication::init(int argc , wchar_t** argv)
{
	return true;
}

bool     IApplication::uninit()
{
	return true;
}

bool IApplication::onMessage(xWindowMsg& msg)
{
    if(m_pCurAppLayer) return m_pCurAppLayer->onMessage(msg);
	return false;
}

bool IApplication::dispatchMsg(xWindowMsg& msg)
{
	int num = (int)m_MsgListeners.size();

	for(int i = 0 ; i < (int)num ; i ++)
	{
		IMsgListener* pListener = m_MsgListeners[i];
		//检测Linstener是不是可以处理消息．
		if(pListener->dispatchMsg(msg) == false)
			continue;
		return true;
	}

	if(onMessage(msg) == true)
		return true;

	return false;
}


//?加入一个Listener．
void   IApplication::addMsgListener(IMsgListener* msgListenter)
{
	m_MsgListeners.push_back(msgListenter);
}

//删除一个Listener.
bool   IApplication::removeMsgListener(IMsgListener* msgListenter)
{
	vector<IMsgListener*> ::iterator i  = m_MsgListeners.begin();
	for(; i!= m_MsgListeners.end(); i ++)
	{
		if( *(i) == msgListenter)
		{
			m_MsgListeners.erase(i);
			return true;
		}
	}
	return false;
}



void IApplication::addFrameUpdateObj(IFrameUpdateObject* pObj)
{
	m_FrameUpdateObjs.push_back(pObj);
}

bool IApplication::removeFrameUpdateObj(IFrameUpdateObject* pObj)
{
	vector<IFrameUpdateObject*> ::iterator i  = m_FrameUpdateObjs.begin();
	for(; i!= m_FrameUpdateObjs.end(); i ++)
	{
		if( *(i) == pObj)
		{
			m_FrameUpdateObjs.erase(i);
			return true;
		}
	}
	return false;
}


bool  IApplication::prevUpdateFrame(long passedTime)
{
	m_FPSCounter.caculateFPS();
	int thisTickt = xGetTickCount();
	size_t nObjs = m_FrameUpdateObjs.size();
	for(size_t i = 0 ; i <  nObjs; ++i)
	{
		m_FrameUpdateObjs[i]->updateFrame(passedTime , thisTickt);
	}

	if(m_pCurAppLayer)
		return m_pCurAppLayer->prevUpdateFrame(passedTime);


	return false;
}
bool  IApplication::postUpdateFrame(long passedTime)
{
	if(m_pCurAppLayer)
		return m_pCurAppLayer->postUpdateFrame(passedTime);
	return false;
}

void IApplication::updateFrame(long passedTime)
{
	if(m_pCurAppLayer)
		m_pCurAppLayer->updateFrame(passedTime);

    this->onLogicTick(passedTime);
    this->onRenderTick(passedTime);
}

bool IApplication::onLogicTick(long passedTime)
{
    if(m_pCurAppLayer)
        m_pCurAppLayer->onLogicTick(passedTime);
    return true;
}

bool IApplication::onRenderTick(long passedTime)
{
    if(m_pCurAppLayer)
        m_pCurAppLayer->onRenderTick(passedTime);
    return true;
}

//------------------------------------------------
//计算帧率
//------------------------------------------------
xFPSCaculator* IApplication::getFPSCounter()
{
	return &m_FPSCounter;
}

void IApplication::resetFPSCounter()
{
	m_FPSCounter.reset();
}

//------------------------------------------------
//应用和窗口之间的关系。
//------------------------------------------------
void IApplication::setMainWindow(IWindow* pMainWnd)
{
	m_pMainWindow = pMainWnd;
}

IWindow* IApplication::getMainWindow()
{
	return m_pMainWindow;
}

//-------------------------------------------------
//应用的状态
//-------------------------------------------------
IApplication::ApplicationState IApplication::getApplicationState()
{
	return m_State;
}
void IApplication::setApplicationState(ApplicationState state)
{
	m_State = state;
}

bool IApplication::isRun()
{
	return m_State==AS_RUN;
}



bool IApplication::registeAppLayer(IAppLayer* appLayer)
{
	if(findAppLayer(appLayer->name()) != NULL)
		return false;
	m_AppLayers.push_back( appLayer );
	return true;
}

bool IApplication::unregisteAppLayer(IAppLayer* appLayer)
{
	return unregisteAppLayer( appLayer->name() );
}

bool IApplication::unregisteAppLayer(const wchar_t* AppLayerName)
{
	AppLayers::iterator fi =     m_AppLayers.begin();
	std::wstring strLayerName = AppLayerName;
	for(; fi != m_AppLayers.end() ; fi ++)
	{
		IAppLayer* pLayer = *fi;
		if(strLayerName == pLayer->name()  )
		{
			m_AppLayers.erase( fi );
			return true;
		}
	}
	return false;
}

IAppLayer*  IApplication::findAppLayer(const wchar_t* appLayerName)
{
	AppLayers::iterator fi =     m_AppLayers.begin();
	std::wstring strLayerName = appLayerName;
	for(; fi != m_AppLayers.end() ; fi ++)
	{
		IAppLayer* pLayer = *fi;
		if(strLayerName == pLayer->name()  )
		{
			return pLayer;
		}
	}
	return NULL;
}

IAppLayer*  IApplication::getCurAppLayer()
{
	return m_pCurAppLayer;
} 

bool   IApplication::exitAppliction()
{
	if(m_pCurAppLayer)
	{
		m_pCurAppLayer->onUnhandle();
	}
	return true;
}

IAppLayer*  IApplication::handleAppLayer(const wchar_t* AppLayerName)
{
	return handleAppLayer(findAppLayer(AppLayerName));
}

IAppLayer*  IApplication::handleAppLayer(IAppLayer* appLayer)
{
	if( appLayer== NULL)
		return NULL;

	IAppLayer* pOldLayer = m_pCurAppLayer;
	if(pOldLayer)
		pOldLayer->onUnhandle();

	m_pCurAppLayer = appLayer;
	m_pCurAppLayer->onHandle();
	return pOldLayer;
}

IApplication* xEvol_GetApplication()
{
	if(IPlatform::singleton() == NULL )
		return NULL;
	return IPlatform::singleton()->getApplication();
}

END_NAMESPACE_XEVOL3D
