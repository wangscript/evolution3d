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

#include "../../xStdPch.h"
#include "../xuiCaption.h"
#include "../xuiStringParser.h"
#include "../xuiWindowManager.h"
BEGIN_NAMESPACE_XUI
DECL_XUI_CONTROLCREATOR(xuiCaption)
REG_XUI_CONTROLCREATOR(xuiCaption, true)

xuiCaption::xuiCaption(xuiWindow* parent,xuiWindowManager* pMgr)
:xuiControl(parent , pMgr)
{
    m_CurState.m_Region.Rect().setRect(0.0f,0.0f,0.0f,32.0f);
}

xuiCaption::~xuiCaption()
{
}

bool xuiCaption::onLMouseRelease(xMouseMsg& mEvent)
{
    static int lastTickt = IPlatform::singleton()->getElapsTime();
    int thisTickt = IPlatform::singleton()->getElapsTime();
    xuiRect _closeBox = getAbsRect(this->m_CloseBox.Rect() );
    if(PtInRect(_closeBox ,  (float)mEvent.x , (float)mEvent.y))
    {
        __fireCommand(eXUIWCMD_Close,this,0,0);
    }

    if(thisTickt - lastTickt < 500)
    {
        thisTickt -= 500;
        __fireCommand(eXUIWCMD_Icon,this,0,0);
    }
    lastTickt = thisTickt;
    return true;
}

bool xuiCaption::load(xXmlNode* pCfgNode)
{
    xuiWindow::load(pCfgNode);
    xXmlNode* attribeNode = pCfgNode->findNode(L"attribte");
    if(attribeNode == NULL) attribeNode = pCfgNode;

    const wchar_t* pText = attribeNode->value(L"text");
    if(pText)
    {
        setText(pText);
    }

    //得到默认的状态的Region
    m_CurState.m_Region.Rect2D().w = m_wndParent->getRegion().Rect2D().w;

    if(attribeNode->findValue(L"size"))
    {
        xuiRect2D _rect = m_CurState.m_Region.Rect2D();
        _rect.h = pCfgNode->float_value(L"size");
    }


    //所有的Region和普通窗口都是不一样
    {
        xXmlNode::XmlNodes stateNodes;
        pCfgNode->findNode(L"state",stateNodes);

        m_vStates[0].m_Region = m_CurState.m_Region;

		size_t _nStateNodes = stateNodes.size();
        for(size_t i = 0; i <  _nStateNodes; i ++)
        {
            xuiWindowState& _state = m_vStates[i+1];
            xXmlNode* pStateNode = stateNodes[i]; 
            _state.m_Region = m_CurState.m_Region;
            if(pStateNode->findValue(L"size"))
            {
                _state.m_Region.Rect2D().h = (float)pStateNode->float_value(L"size");
            }
        }
    }



    float box_w = m_CurState.m_Region.Rect2D().h - 6.0f;
    m_CloseBox.Rect2D().x = m_CurState.m_Region.Rect2D().w - m_CurState.m_Region.Rect2D().h - 1.0f;
    m_CloseBox.Rect2D().y = 3;
    m_CloseBox.Rect2D().h = m_CloseBox.Rect2D().w = box_w;
    return true;
}

bool xuiCaption::draw()
{
    xuiWindow::draw();

    //绘制自己特有的东西;
    xuiRect _rect = m_wndParent->getAbsRect( m_CurState.m_Region.Rect() ) ;// getWndAbsRect();
    m_hFont->drawTextOneLine(m_text.c_str(),_rect, m_CurState.m_textcolor,xFontRender::eAlign_Center,xFontRender::eAlign_Center);


    //xColor_4f _closeBoxCl = xColor_4f(1.0,0.0f,0.0f, m_CurState.m_color.a);
    //xuiRegion _region = m_CloseBox;
    //_region.Rect() = this->getAbsRect(m_CloseBox.Rect() );
    //this->_drawRect(_region.Rect(),_closeBoxCl,xuiBevel(2,2,2,2));

    //Draw Cross
    //xLineHelper _line[2];
    //xHelperRenderer* hlpRenderer = IRenderSystem::singleton()->getHelperRender();

    //_line[0].m_color = _closeBoxCl;

    //_line[0].m_Point[0] = xvec3(_region._rect.left()  + 3 , _region._rect.top()    + 3, 0.0f);
    //_line[0].m_Point[1] = xvec3(_region._rect.right() - 3 , _region._rect.bottom() - 3, 0.0f );

    //_line[1].m_color = _closeBoxCl;
    //_line[1].m_Point[0] = xvec3(_region._rect.right()  - 3 , _region._rect.top()    + 3, 0.0f);
    //_line[1].m_Point[1] = xvec3(_region._rect.left()   + 3 , _region._rect.bottom() - 3, 0.0f );

    //hlpRenderer->BeginRender();
    //hlpRenderer->Draw(2 , _line);
    //hlpRenderer->EndRender();

   
    //IRenderSystem::singleton()->drawRect(_rect,xColor_4f(1.0,0.0f,0.0f,1.0f));
    return true;
}


void xuiCaption::setText(const wchar_t* _text)
{
    m_text = _text;
}

void xuiCaption::onLostActive()
{
    setState(eXUIWState_LostActive);
}

void xuiCaption::onActive()
{
    setState(eXUIWState_Normal);
}
END_NAMESPACE_XUI
