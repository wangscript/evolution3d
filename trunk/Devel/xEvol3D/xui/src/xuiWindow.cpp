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

#include "../xuiWindow.h"
#include "../xuiWindowManager.h"
#include "../xuiStringParser.h"
#include <assert.h>
#include "../../xcomdoc/xcomdoc.h"
#include <map>
using namespace std;


BEGIN_NAMESPACE_XUI



bool operator >(const xuiWindowZValue& lhv , const xuiWindowZValue& rhv)
{
    if(lhv.m_zOrder > rhv.m_zOrder)
        return true;
    else if(lhv.m_zOrder < rhv.m_zOrder)
        return false;
    else 
        return lhv.m_zValue > rhv.m_zValue;
}

bool operator == (const xuiWindowZValue& lhv , const xuiWindowZValue& rhv)
{
    if(lhv.m_zOrder == rhv.m_zOrder && lhv.m_zOrder == rhv.m_zOrder)
        return true;
    return false;
}

bool operator <(const xuiWindowZValue& lhv , const xuiWindowZValue& rhv)
{
    if(lhv.m_zOrder < rhv.m_zOrder)
        return true;
    else if(lhv.m_zOrder > rhv.m_zOrder)
        return false;
    return lhv.m_zValue < rhv.m_zValue;
}



typedef map<int , xuiWindowCreator*> xWindowCreatorMap;
xWindowCreatorMap* gs_WindowCreatorMap = NULL;
class xWinCreatorMapIniter
{
public:
    xWinCreatorMapIniter()
    {
        if(gs_WindowCreatorMap == NULL)
            gs_WindowCreatorMap = new xWindowCreatorMap;
    }
    ~xWinCreatorMapIniter()
    {
        gs_WindowCreatorMap->clear();
    }
};

bool xuiWindowCreator::registeWindowCreate(xuiWindowCreator* pXUIWindowCreator)
{
    static xWinCreatorMapIniter __initer;
    int hashValue = xStringHash(pXUIWindowCreator->type());
    if(findWindowCreater(pXUIWindowCreator->type() ) )
    {
        return false;
    }
    gs_WindowCreatorMap->insert(xWindowCreatorMap::value_type(hashValue , pXUIWindowCreator) );
    return true;
}

xuiWindowCreator* xuiWindowCreator::findWindowCreater(const wchar_t* _windowName)
{
    static xWinCreatorMapIniter __initer;
    int hashValue = xStringHash( _windowName);
    xWindowCreatorMap::iterator pos = gs_WindowCreatorMap->find(hashValue);
    if(pos != gs_WindowCreatorMap->end() ) 
        return pos->second;
    return NULL;
}

const xuiWindowZValue&  xuiWindow::zValue() const
{
    return m_zValue;
}

void xuiWindow::setZValue(unsigned int zValue)
{
    m_zValue.m_zValue = zValue;
}


bool xuiWindow::hasMouseFocus()
{
    return m_pWindowMgr->getMouseFocus() == this;
}

bool xuiWindow::hasKbInputFocus()
{
    if(this->m_wndParent == NULL)
        return false;//默认的是没有键盘输入焦点的
    return m_wndParent->hasKbInputFocus();
}

bool xuiWindow::hasSeperatedKBFocus()
{
    if(this->m_wndParent == NULL)
        return false;//Default , the Keyboard and mouse has same focus. 
    return m_wndParent->hasSeperatedKBFocus();
}



void xuiWindow::startHideBlend()
{
    xuiWindowState_t* pHideState = findState(L"hide");
    if(pHideState)
    {
        m_stateBlend->setState(pHideState, 200);
    }
    else
    {
        xuiWindowState_t _startState = xuiWindowState_t::XUISTATE_HIDE;
        _startState.m_Region = m_state.m_Region;
        m_stateBlend->setState(&_startState, 200);
    }
    m_state.setName(L"normal");
}
xuiWindowStateBlender*    xuiWindow::getStateBlender()
{
    return m_stateBlend;
}

xuiWindowTransBlender*xuiWindow::getTransBlender()
{
    return m_transBlend;
}

void xuiWindow::startShowBlend()
{
    xuiWindowState_t* pHideState = findState(L"hide");
    if(pHideState)
    {
        m_stateBlend->setState(pHideState , &m_vStates[0] , 200);
    }
    else
    {
        xuiWindowState_t _startState = xuiWindowState_t::XUISTATE_HIDE;
        _startState.m_Region = m_state.m_Region;
        m_stateBlend->setState(&_startState , &m_vStates[0] , 200);
    }
    m_state.setName(L"normal");
}

bool xuiWindow::hide()
{
    if(!isVisable())return false;
    onHide();
    __notifyChildren(eXUIWN_Hide , this , 0, 0);
    m_bVisable = false;
    startHideBlend();
    return m_bVisable;
}


bool xuiWindow::show()
{
    if(isVisable())return false;
    onShow();
    __notifyChildren(eXUIWN_Show , this , 0, 0);
    m_bVisable = true;
    startShowBlend();
    setState(eXUIWState_Normal);
    return m_bVisable;
}

bool xuiWindow::isVisable()
{
    return m_bVisable;
}

bool  xuiWindow::__notifyParent(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2)
{
    if(this->m_wndParent) return m_wndParent->notify(_event , pWndSender , param1 , param2);
    return false;
}

bool  xuiWindow::__fireCommand(eXUIWinCommand  _command, xuiWindow* pCtrl, int param1 , int param2)
{
    if(this->m_wndParent) return m_wndParent->command(_command , pCtrl , param1 , param2);
    return false;
}

bool  xuiWindow::__notifyChildren(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2)
{
    for(size_t i = 0 ; i < m_Children.size() ; i ++)
    {
        m_Children[i]->notify(_event , pWndSender , param1 , param2);
    }
    return true;
}

bool xuiWindow::command(eXUIWinCommand  _command, xuiWindow* pCtrl, int param1 , int param2)
{
    return true;
}

bool xuiWindow::notify(eXUIWinNotifyEvent _event, xuiWindow* pWndSender , int param1 , int param2)
{
    switch(_event)
    {
    case eXUIWN_Hide:
        if(m_pWindowMgr->getMouseFocus() == this)
        {
            this->setMouseFocus(false);
        }
        startHideBlend();
        return true;
        break;
    case eXUIWN_Show:
        startShowBlend();
        return true;
        break;
    }
    __notifyChildren(_event, pWndSender , param1 , param2);
    return false;
}

xuiWindow::~xuiWindow()
{
    if(m_transBlend) delete m_transBlend;
    if(m_stateBlend) delete m_stateBlend;

    m_stateBlend = NULL;
    m_transBlend = NULL;
    if(m_wndParent) m_wndParent->removeChild(this);

    for(size_t i = 0 ; i < m_Children.size() ; i ++)
    {
        m_Children[i]->ReleaseObject();
    }
}

xuiWindow::xuiWindow(xuiWindow* parent , xuiWindowManager* pMgr)
:m_wndParent(parent)
{
    m_RefCount = 1;
    m_pWindowMgr = pMgr;
    m_bVisable = false;
    m_transBlend = new xuiWindowTransBlender(this);
    m_stateBlend = new xuiWindowStateBlender(this);
    m_zValue.m_zValue = m_pWindowMgr->getTime();

    m_state.m_color     = xColor_4f(0.6f,0.6f,0.6f,1.0f);
    m_state.m_textcolor = xColor_4f(0.0f,0.0f,0.0f,1.0f);
    m_state.m_border.m_color = xColor_4f(0.0f,0.0f,0.0f,1.0f);
    m_state.m_border.m_size  = 0;//has no border


    if(m_wndParent) m_wndParent->addChild(this);
}

bool xuiWindow::addChild(xuiWindow* pWindow)
{
    m_Children.push_back(pWindow);
    return true;
}

bool xuiWindow::removeChild(xuiWindow* pWindow)
{
    xuiWindows::iterator i = m_Children.begin();
    while( i != m_Children.end() )
    {
        if( *i == pWindow)
        {
            m_Children.erase(i);
            return true;
        }
    }
    return false;
}

bool xuiWindow::nameEq(const wchar_t* _name)
{
    if(_name == NULL)
        return false;

    if( std::wstring(_name) == m_name)
    {
        return true;
    }
    return false;
}
const wchar_t* xuiWindow::name()
{
    return m_name;
}

xuiRegion& xuiWindow::getRegion()
{
    return m_state.m_Region;
}

void xuiWindow::setPosition(xuiPoint& pos)
{
    //Move All State;
    float dx = (float)pos.x - m_vStates[0].m_Region.Rect2D().x;
    float dy = (float)pos.y - m_vStates[0].m_Region.Rect2D().y;

    for(size_t i = 0 ; i < m_vStates.size() ; ++i)
    {
        m_vStates[0].m_Region.Rect2D().x += dx;
        m_vStates[0].m_Region.Rect2D().y += dy;
    }

    m_state.m_Region.Rect2D().x = (float)pos.x ;
    m_state.m_Region.Rect2D().y = (float)pos.y ;
}

xuiPoint   xuiWindow::getAbsPosition(xuiPoint& pos)
{
    xuiPoint _wndPos = getWndAbsPos();

    pos.x += _wndPos.x;
    pos.y += _wndPos.y;
    return pos;
}

xuiRect    xuiWindow::getAbsRect(xuiRect& _rect)
{
    //求出窗口的实际区域
    xuiRect _baseRect = getWndAbsRect();
    xuiRect _ret = m_pWindowMgr->clipRect(_baseRect, _rect);
    return _ret;
}

xuiPoint xuiWindow::getWndAbsPos()
{
    xuiPoint pos ;
    pos.x = m_state.m_Region.Rect2D().x;
    pos.y = m_state.m_Region.Rect2D().y;
    if(m_wndParent == NULL)
        return pos;
    else
    {
        xuiPoint _parentpos = m_wndParent->getWndAbsPos();
        pos.x += _parentpos.x;
        pos.y += _parentpos.y;
        return pos;
    }
}

xuiRect xuiWindow::getWndAbsRect()
{
    if(m_wndParent == NULL) 
    {
        return m_state.m_Region.Rect();
    }
    else
    {
        //求出父窗口的实际区域
        xuiRect _baseRect = m_wndParent->getWndAbsRect();
        xuiRect _ret = m_pWindowMgr->clipRect(_baseRect, m_state.m_Region.Rect() );
        return _ret;
    }
}

//===============================
bool xuiWindow::__loadBorder(xuiWindowBorder_t& border ,xXmlNode* pNode)
{
    border.m_size =  0;
    border.m_color = xColor_4f(1.0f,1.0f,1.0f,1.0f);
    if(pNode == NULL)
    {
        XEVOL_LOG(eXL_DEBUG_TIPS,"not find border node\n");
        return false;
    }
    if(pNode->findValue(L"color"))
    {
        string2Color(pNode->findValue(L"color")->value() , border.m_color);
    }
    if(pNode->findValue(L"size"))
    {
        border.m_size = pNode->findValue(L"size")->int_value();
    }

    if(pNode->findValue(L"bevel"))
    {
        string2Bevel(pNode->findValue(L"bevel")->value(), border.m_bevel);
    }
    return true;
}

bool xuiWindow::load(xXmlNode* pCfgNode)
{
    //可以特意指定一个attribute的段。也可以不指定
    xXmlNode* pAttrNode = pCfgNode->findNode(L"attribute");
    if(pAttrNode == NULL) pAttrNode = pCfgNode;
    assert(pAttrNode);



    //Load from Common Control Lib.
    xXmlNode* pCommCtrlNode = m_pWindowMgr->findCommCtrl(pCfgNode->value(L"public"));
    if(pCommCtrlNode)
    {
        if(pAttrNode->findValue(L"region"))
        {
            string2Region(pAttrNode->findValue(L"region")->value() , m_state.m_Region);
        }
        load(pCommCtrlNode);
    }

    __loadState(m_state, pAttrNode);
    m_state.setName(L"init");
    //Read caption
    xXmlValue* pStyleValue = pAttrNode->findValue(L"style");
    if(pStyleValue) 
    {
        __parseStyle(pStyleValue->value() );
    }


    //Read the name;
    xXmlValue* pNameValue = pCfgNode->findValue(L"name");
    m_name[0] = 0;
    if(pNameValue) 
        wcsncpy(m_name,pNameValue->value(),32);

    //Read font.
    xXmlValue* pFontValue = pAttrNode->findValue(L"font");
    if(pFontValue)
    {
        IRenderApi* pRenderApi = m_pWindowMgr->renderApi();

        m_hFont =  pRenderApi->fontManager()->findFont(pFontValue->value());
    }

    //Read all font
    __loadAllStates(pAttrNode);
    return true;
}


//================================
bool xuiWindow::__parseStyle(const wchar_t* _style)
{
    wstring style = _style;
    //if(style.find("border") != string::npos)
    //	this->m_bBorder = true;
    //if(style.find("caption") != string::npos)
    //	this->m_bCaption = true;
    //if(style.find("resize") != string::npos)
    //	this->m_bResizeable = true;

    //if(style.find("max") != string::npos)
    //	this->m_bMaxBox = true;
    //if(style.find("min") != string::npos)
    //	this->m_bMinBox = true;
    //if(style.find("closebox") != string::npos)
    //	this->m_bCloseBox = true;
    return true;
}


bool xuiWindow::hitTest(xuiRect& rect , int x , int y)
{
    xuiRect _thisRect = getAbsRect(rect);
    return  PtInRect(_thisRect, (float)x ,  (float)y);
}

eXUIWinHitState xuiWindow::hitTest(int x , int y)
{
    xuiRect _thisRect = getWndAbsRect();
    if( PtInRect(_thisRect , (float)x ,  (float)y) )
    {
        return eXUIWHS_InWindow;
    }
    return eXUIWHS_OutWindow;
}
bool xuiWindow::drawWindow()
{
    xuiWindowTransform_t _winTrans;
    if(m_transBlend && m_transBlend->getTrans(_winTrans) )
    {
        _winTrans.begin();
        bool ret = draw();
        _winTrans.end();
        return ret;
    }
    else
    {
        return draw();
    }
}
bool xuiWindow::draw()
{
    return drawBorder();
}

bool xuiWindow::drawBorder()
{
    if(m_state.m_border.m_size)
    {
        xuiRegion region;
        region._type = m_state.m_Region._type;
        region.Rect() = getWndAbsRect();
        return _drawRect(region.Rect() , m_state.m_border.m_color, m_state.m_border.m_bevel);
    }
    return false;
}
bool xuiWindow::_drawRect(xMathLib::xvec4& _rect , xColor_4f& _cl)
{
    //xLineHelper _line[4];
    //xHelperRenderer* hlpRenderer = IRenderSystem::singleton()->getHelperRender();

    //_line[0].m_color = _cl;
    //_line[0].m_Point[0] = xvec3(_rect.x , _rect.y, 0.0f);
    //_line[0].m_Point[1] = xvec3(_rect.x , _rect.y + _rect.h, 0.0f );

    //_line[1].m_color = _cl;
    //_line[1].m_Point[0] = xvec3(_rect.x , _rect.y, 0.0f);
    //_line[1].m_Point[1] = xvec3(_rect.x + _rect.w, _rect.y , 0.0f );

    //_line[2].m_color = _cl;
    //_line[2].m_Point[0] = xvec3(_rect.x + _rect.w, _rect.y + _rect.h , 0.0);
    //_line[2].m_Point[1] = xvec3(_rect.x + _rect.w, _rect.y , 0.0f );

    //_line[3].m_color = _cl;
    //_line[3].m_Point[0] = xvec3(_rect.x + _rect.w , _rect.y + _rect.h , 0.0);
    //_line[3].m_Point[1] = xvec3(_rect.x , _rect.y + _rect.h, 0.0f );

    //hlpRenderer->BeginRender();
    //hlpRenderer->Draw(4 , _line);
    //hlpRenderer->EndRender();
    return true;
}

bool xuiWindow::_drawRect(xMathLib::xvec4& _rect , xColor_4f& _cl , xuiBevel& bevel)
{
    if(bevel.isBevel() == false)
        return _drawRect(_rect,_cl);

    //xLineHelper _line[8];
    //xHelperRenderer* hlpRenderer = IRenderSystem::singleton()->getHelperRender();

    //_line[0].m_color = _cl;
    //_line[0].m_Point[0] = xvec3(_rect.left()           , _rect.top() + bevel.tl , 0.0f);
    //_line[0].m_Point[1] = xvec3(_rect.left() + bevel.tl, _rect.top()            , 0.0f);

    //_line[1].m_color = _cl;
    //_line[1].m_Point[0] = xvec3(_rect.left()  + bevel.tl, _rect.top()           , 0.0f);
    //_line[1].m_Point[1] = xvec3(_rect.right() - bevel.tr, _rect.top()           , 0.0f);

    //_line[2].m_color = _cl;
    //_line[2].m_Point[0] = xvec3(_rect.right() - bevel.tr, _rect.top()           , 0.0f);
    //_line[2].m_Point[1] = xvec3(_rect.right()           , _rect.top() + bevel.tr, 0.0f);

    //_line[3].m_color = _cl;
    //_line[3].m_Point[0] = xvec3(_rect.right()           , _rect.top()    + bevel.tr, 0.0f);
    //_line[3].m_Point[1] = xvec3(_rect.right()           , _rect.bottom() - bevel.br, 0.0f);


    //_line[4].m_color = _cl;
    //_line[4].m_Point[0] = xvec3(_rect.right()           , _rect.bottom() - bevel.br, 0.0f);
    //_line[4].m_Point[1] = xvec3(_rect.right() - bevel.br, _rect.bottom()           , 0.0f);


    //_line[5].m_color = _cl;
    //_line[5].m_Point[0] = xvec3(_rect.right() - bevel.br, _rect.bottom()           , 0.0f);
    //_line[5].m_Point[1] = xvec3(_rect.left()  + bevel.bl, _rect.bottom()           , 0.0f);

    //_line[6].m_color = _cl;
    //_line[6].m_Point[0] = xvec3(_rect.left()  + bevel.bl, _rect.bottom()           , 0.0f);
    //_line[6].m_Point[1] = xvec3(_rect.left()            , _rect.bottom() - bevel.bl, 0.0f);

    //_line[7].m_color = _cl;
    //_line[7].m_Point[0] = xvec3(_rect.left()            , _rect.bottom() - bevel.bl, 0.0f);
    //_line[7].m_Point[1] = xvec3(_rect.left()            , _rect.top()    + bevel.tl, 0.0f);

    //hlpRenderer->BeginRender();
    //hlpRenderer->Draw(8 , _line);
    //hlpRenderer->EndRender();
    return true;
}

bool xuiWindow::_drawRegion(xuiRegion& _region, xColor_4f& _cl)
{
    _drawRect(_region.Rect(),_cl);
    return true;

}

bool xuiWindow::__loadState(xuiWindowState_t& _state, xXmlNode* pAttrNode)
{
    if(pAttrNode->findValue(L"region"))
    {
        string2Region(pAttrNode->findValue(L"region")->value() , _state.m_Region);
    }
    if(pAttrNode->findValue(L"textcolor"))
    {
        string2Color(pAttrNode->findValue(L"textcolor")->value() , _state.m_textcolor);
    }
    if(pAttrNode->findValue(L"color"))
    {
        string2Color(pAttrNode->findValue(L"color")->value() , _state.m_color);
    }

    if(&_state != &m_state)
    {
        if(pAttrNode->findValue(L"name") == NULL)
        {
            XEVOL_LOG(eXL_DEBUG_TIPS,"xuiButton Control's state must has a name\n");
            return false;
        }
        _state.setName(pAttrNode->findValue(L"name")->value());
    }
    __loadBorder(_state.m_border,pAttrNode->findNode(L"border"));

    return true;
}
bool xuiWindow::__loadAllStates(xXmlNode* pCfgNode)
{
    xXmlNode::XmlNodes stateNodes;
    pCfgNode->findNode(L"state",stateNodes);
    if(m_vStates.size( ) == 0)
    {
        m_vStates.push_back(m_state);
        m_vStates[0].setName( L"normal" );
    }

    for(size_t i = 0; i < stateNodes.size() ; i ++)
    {
        xXmlNode* pAttrNode = stateNodes[i]; 

        const wchar_t* _name = pAttrNode->value(L"name");
        xuiWindowState_t* pState = findState(_name);
        if(pState)//If we have same state in the state list , modify it
        {
            __loadState(*pState, pAttrNode);
        }
        else
        {
            xuiWindowState_t _state = m_state ;
            __loadState(_state, pAttrNode);
            m_vStates.push_back(_state);
        }
    }
    return true;
}

bool xuiWindow::updateFrame(int passedTime , unsigned int thisTime)
{
    for(size_t i = 0 ;i < m_Children.size() ; i ++)
    {
        m_Children[i]->updateFrame(passedTime , thisTime);
    }
    return m_stateBlend->update((long)passedTime);
}

bool xuiWindow::setMouseFocus(bool bFocus)
{
    m_pWindowMgr->setMouseFocus(this,bFocus);
    return m_pWindowMgr->getMouseFocus() == this;
}


void xuiWindow::onMouseFocus()
{
    setState(eXUIWState_Focus);
}

void xuiWindow::onLostMouseFocus()
{
    setState(eXUIWState_Normal);
}


void xuiWindow::onLostActive()
{
    for(size_t i = 0 ; i < m_Children.size() ; i ++)
    {
        m_Children[i]->onLostActive();
    }
}

void xuiWindow::onActive()
{
    for(size_t i = 0 ; i < m_Children.size() ; i ++)
    {
        m_Children[i]->onActive();
    }
    setZValue( m_pWindowMgr->getTime() ); 
}

bool xuiWindow::onShow()
{
    return false;
}

bool xuiWindow::onHide()
{
    return false;
}

void xuiWindow::setState(xuiWindowState_t* pState , bool bBlend)
{
    if(pState)
    {
        if(m_stateBlend && bBlend) 
            m_stateBlend->setState(pState,200);
        else
        {
            __setState(pState);
        }
    }
    else
    {
        //不存在这个state 则使用默认的state
        setState(&m_vStates[0] , bBlend);
    }
}

void xuiWindow::__setState(xuiWindowState_t* pState)
{
    if(pState)
    {
        m_state  =   *pState ;
    }
}

xuiWindowState_t* xuiWindow::findState(const wchar_t* _stateName)
{
    xuiWindowState_t* _defState = NULL;
    for(size_t i = 0 ; i < m_vStates.size() ; ++i)
    {
        if(m_vStates[i].m_Name == wstring(_stateName) )
        {
            _defState = &m_vStates[i];
            return _defState;
        }
    }
    return NULL;
}

void xuiWindow::setState(const wchar_t* _stateName, bool bBlend)
{
    xuiWindowState_t* _defState = findState(_stateName);
    m_state.setName(_stateName);
    setState(_defState,bBlend);

}
void xuiWindow::setState(eXUIWinState _state, bool bBlend)
{
    switch(_state)
    {
    case eXUIWState_Normal:
        setState(L"normal",bBlend);
        break;
    case eXUIWState_LostActive:
        setState(L"lostactive",bBlend);
        break;
    case eXUIWState_Gray:
        setState(L"gray",bBlend);
        break;
    case eXUIWState_Press:
        setState(L"press",bBlend);
        break;
    case eXUIWState_Focus:
        setState(L"focus",bBlend);
        break;
    case eXUIWState_KeyboardFocus:
        {
            if(findState(L"keyboardfocus") == NULL)
            {
                setState(L"focus",bBlend);
            }
            else
            {
                setState(L"keyboardfocus",bBlend);
            }
        }

        break;
    }
}
END_NAMESPACE_XUI
