/*
XReal3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xreal3d/) 
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
#include "stdafx.h"
#include  "xuiDemoInfoPannel.h"
#include  "xui/xuiWindowManager.h"
#include  "Application/xPlatform.h"
using namespace XEvol3D;

BEGIN_NAMESPACE_XUI
DECL_XUI_DIALOGCREATOR(xuiDemoInfoPannel , L"infopanel.xml");
REG_XUI_DIALOGCREATOR(xuiDemoInfoPannel,true);
xuiDemoInfoPannel::xuiDemoInfoPannel(xuiWindow* parent , xuiWindowManager* pMgr)
:xuiDialog(parent , pMgr)
{

}
xuiDemoInfoPannel::~xuiDemoInfoPannel()
{

}
bool xuiDemoInfoPannel::onNotify(eXUIWinNotifyEvent _event, xuiWindow* pCtrl , int param1 , int param2)
{
	if(_event == eXUIWN_Sys_Resize)
	{
		float w = (float)param1 ;
		float h = (float)param2;
		float x = w - m_state.m_Region.Rect2D().w - 10.0f;
		float y = h - m_state.m_Region.Rect2D().h - 10.0f;
		setPosition(xuiPoint(x,y));
	}
	return false;
}

bool xuiDemoInfoPannel::onLoad(xXmlNode* pCfgNode)
{
	int w = (int)m_state.m_Region.Rect2D().w;
	int h = (int)m_state.m_Region.Rect2D().h;

	xuiRect rect = m_state.m_Region.Rect();
	rect.x = rect.y = 0;
    IRenderApi* pRenderApi = m_pWindowMgr->renderApi();
	m_pTextCacheTexture = pRenderApi->createRenderableTexture(w , h , 1 ,PIXELFORMAT_R8G8B8A8 , false);// createRenderToTexture(w,h,PIXELFORMAT_R8G8B8A8) ;

    int wnd_w = 0;
    int wnd_h = 0;
    pRenderApi->getWindowSize(wnd_w , wnd_h );
	float x = (float)wnd_w - m_state.m_Region.Rect2D().w - 10.0f;
	float y = (float)wnd_h - m_state.m_Region.Rect2D().h - 10.0f;
	setPosition(xuiPoint(x,y));

	xuiWindow::load(pCfgNode);
	return true;
}

void xuiDemoInfoPannel::setText(const wchar_t* _text)
{
	m_text = _text;
	//x2DRect_f rect = m_state.m_Region._rect;
	//rect.x = rect.y = 0;
    //IRenderApi* pRenderApi = m_pWindowMgr->renderApi();
	//IRenderSystem::singleton()->setZEnable(false);
	//IRenderSystem::singleton()->enableBlend(false);
	//m_pTextCacheTexture->bind(0);
	//IRenderSystem::singleton()->getRenderView()->pushState();
	//IRenderSystem::singleton()->getRenderView()->bindRenderTarget(m_pTextCacheTexture->getMipmapLevel(0),0);
	//IRenderSystem::singleton()->enter2DMode(true);
	//IRenderSystem::singleton()->clear(0.0f,0.0f,0.0f,0.0f);
	//m_pFont->drawText(m_text.c_str(),5,5, (int)rect.w - 10, m_state.m_textcolor);
	//IRenderSystem::singleton()->leave2DMode();
	//IRenderSystem::singleton()->getRenderView()->popState();
	//m_pTextCacheTexture->unbind(0);
}

bool xuiDemoInfoPannel::onDraw()
{
	xuiRect2D rect = m_state.m_Region.Rect2D();
    m_hFont->drawText(m_text.c_str(),m_state.m_Region.Rect2D().x + 5,m_state.m_Region.Rect2D().y + 5 , (int)rect.w - 10, m_state.m_textcolor);
	//IRenderSystem::singleton()->enableBlend(true);
	//IRenderSystem::singleton()->drawRect(m_pTextCacheTexture, rect, m_state.m_textcolor);
	return true;
}
END_NAMESPACE_XUI