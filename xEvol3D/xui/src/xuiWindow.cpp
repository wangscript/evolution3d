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

IMPL_REFCOUNT_OBJECT_FUNCTION(xuiWindow)
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
    xuiWindowState* pHideState = findState(L"hide");
    if(pHideState)
    {
        m_pStateBlend->setState(pHideState, 200);
    }
    else
    {
        xuiWindowState _startState = xuiWindowState::XUISTATE_HIDE;
        _startState.setRegion(m_CurState.region());
        m_pStateBlend->setState(&_startState, 200);
    }
    m_CurState.setName(L"normal");
}
xuiWindowStateBlender*    xuiWindow::getStateBlender()
{
    return m_pStateBlend;
}

xuiWindowTransBlender*xuiWindow::getTransBlender()
{
    return m_transBlend;
}

bool xuiWindow::isCurrentState(xuiWindowState* pState)
{
    if(pState == &m_CurState)
        return true;
    return false;
}

void xuiWindow::startShowBlend()
{
    xuiWindowState* pHideState = findState(L"hide");
    if(pHideState)
    {
        m_pStateBlend->setState(pHideState , &m_vStates[0] , 200);
    }
    else
    {
        xuiWindowState _startState = xuiWindowState::XUISTATE_HIDE;
        _startState.setRegion( m_CurState.region() );
        m_pStateBlend->setState(&_startState , &m_vStates[0] , 200);
    }
    m_CurState.setName(L"normal");
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
	size_t _nChildren = m_Children.size();
    for(size_t i = 0 ; i <  _nChildren ; i ++)
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
    if(m_pStateBlend) delete m_pStateBlend;

    m_pStateBlend = NULL;
    m_transBlend = NULL;
    if(m_wndParent) m_wndParent->removeChild(this);

	size_t _nChildren = m_Children.size();
	for(size_t i = 0 ; i <  _nChildren ; i ++)
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
    m_pStateBlend = new xuiWindowStateBlender(this);
    m_zValue.m_zValue = m_pWindowMgr->getTime();

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
    return m_CurState.region();
}

void xuiWindow::setPosition(xuiPoint& pos)
{
    //Move All State;
    float dx = (float)pos.x - m_vStates[0].region().Rect2D().x;
    float dy = (float)pos.y - m_vStates[0].region().Rect2D().y;

	size_t _nStates = m_vStates.size();
    for(size_t i = 0 ; i <  _nStates; ++i)
    {
        m_vStates[0].region().Rect2D().x += dx;
        m_vStates[0].region().Rect2D().y += dy;
    }

    m_CurState.region().Rect2D().x = (float)pos.x ;
    m_CurState.region().Rect2D().y = (float)pos.y ;
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
    pos.x = m_CurState.region().Rect2D().x;
    pos.y = m_CurState.region().Rect2D().y;
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
        return m_CurState.region().Rect();
    }
    else
    {
        //求出父窗口的实际区域
        xuiRect _baseRect = m_wndParent->getWndAbsRect();
        xuiRect _ret = m_pWindowMgr->clipRect(_baseRect, m_CurState.region().Rect() );
        return _ret;
    }
}

//===============================


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
            string2Region(pAttrNode->findValue(L"region")->value() , m_CurState.region() );
        }
        load(pCommCtrlNode);
    }

    m_CurState.load(pAttrNode , this);
    m_CurState.setName(L"init");
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

        m_hFont =  pRenderApi->fontManager()->findFont(pFontValue->value() , true);
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
    xuiWindowTransform _winTrans;
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
    m_CurState.draw(this);
    size_t _nChildren  = m_Children.size();
    for(size_t i = 0 ; i < _nChildren ; i ++)
    {
        xuiWindow* pChild = m_Children[i];
        pChild->draw();
    }
    return true;
}


bool xuiWindow::__loadAllStates(xXmlNode* pCfgNode)
{
    xXmlNode::XmlNodes stateNodes;
    pCfgNode->findNode(L"state",stateNodes);
    if(m_vStates.size( ) == 0)
    {
        m_vStates.push_back(m_CurState);
        m_vStates[0].setName( L"normal" );
    }

	size_t _nStateNodes = stateNodes.size();
    for(size_t i = 0; i <  _nStateNodes; i ++)
    {
        xXmlNode* pAttrNode = stateNodes[i]; 

        const wchar_t* _name = pAttrNode->value(L"name");
        xuiWindowState* pState = findState(_name);
        if(pState)//If we have same state in the state list , modify it
        {
            pState->load(pAttrNode , this);
        }
        else
        {
            xuiWindowState _state = m_CurState ;
            _state.load(pAttrNode , this);
            m_vStates.push_back(_state );
        }
    }
    return true;
}

bool xuiWindow::updateFrame(int passedTime , unsigned int thisTime)
{
	size_t _nChildren = m_Children.size();
	for(size_t i = 0 ; i <  _nChildren ; i ++)
    {
        m_Children[i]->updateFrame(passedTime , thisTime);
    }
    return m_pStateBlend->update((long)passedTime);
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
	size_t _nChildren = m_Children.size();
	for(size_t i = 0 ; i <  _nChildren ; i ++)
    {
        m_Children[i]->onLostActive();
    }
}

void xuiWindow::onActive()
{
	size_t _nChildren = m_Children.size();
	for(size_t i = 0 ; i <  _nChildren ; i ++)
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

void xuiWindow::setState(xuiWindowState* pState , bool bBlend)
{
    if(pState)
    {
        if(m_pStateBlend && bBlend) 
            m_pStateBlend->setState(pState,200);
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

void xuiWindow::__setState(xuiWindowState* pState)
{
    if(pState)
    {
        m_CurState  =   *pState ;
    }
}

xuiWindowState* xuiWindow::findState(const wchar_t* _stateName)
{
    xuiWindowState* _defState = NULL;
	size_t _nStates = m_vStates.size();
	for(size_t i = 0 ; i <  _nStates; ++i)
    {
        if(m_vStates[i].name() == wstring(_stateName) )
        {
            _defState = &m_vStates[i];
            return _defState;
        }
    }
    return NULL;
}

void xuiWindow::setState(const wchar_t* _stateName, bool bBlend)
{
    xuiWindowState* _defState = findState(_stateName);
    m_CurState.setName(_stateName);
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
