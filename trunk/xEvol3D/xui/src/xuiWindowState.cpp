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
#include "../xuiWindowState.h"
#include "../xuiStringParser.h"
#include "../xuiWindowManager.h"

BEGIN_NAMESPACE_XUI
xuiWindowState xuiWindowState::XUISTATE_HIDE = xuiWindowState(L"hide");
xuiWindowState::xuiWindowState(const wchar_t* name)
{
    memset(this,sizeof(xuiWindowState) , 0);

    setName(name);
    m_color          = xColor_4f(0.0f, 0.0f,0.0f,1.0f);
    m_textcolor      = xColor_4f(1.0f, 1.0f,1.0f,1.0f);
    m_border.m_color = xColor_4f(1.0f, 1.0f,1.0f,1.0f);
    m_color          = xColor_4f(0.6f,0.6f,0.6f,1.0f);
    m_textcolor      = xColor_4f(0.0f,0.0f,0.0f,1.0f);
    m_border.m_color = xColor_4f(0.0f,0.0f,0.0f,1.0f);
    m_border.m_size  = 0;//has no border


}
void xuiWindowState::operator = (const xuiWindowState& rhv)
{
    //²»¿½±´×´Ì¬Ãû×Ö
    m_color    = rhv.m_color    ;
    m_textcolor= rhv.m_textcolor;
    m_border   = rhv.m_border   ;
    m_Region   = rhv.m_Region   ;
}

void xuiWindowState::setName(const wchar_t* _name)
{
    wcsncpy(m_Name,_name,32);
}

bool xuiWindowState::load(xXmlNode* pAttrNode , xuiWindow* pWindow)
{
    if(pAttrNode->findValue(L"region"))
    {
        string2Region(pAttrNode->findValue(L"region")->value() , m_Region);
    }
    if(pAttrNode->findValue(L"textcolor"))
    {
        string2Color(pAttrNode->findValue(L"textcolor")->value() , m_textcolor);
    }
    if(pAttrNode->findValue(L"color"))
    {
        string2Color(pAttrNode->findValue(L"color")->value() , m_color);
    }

    xXmlNode::XmlNodes textureNodes;
    pAttrNode->findNode(L"image" , textureNodes);
    size_t nImages = textureNodes.size() ;
    for(size_t i = 0 ; i < nImages ; i ++)
    {
        xXmlNode* pImageNode = textureNodes[i];
        const wchar_t* image = pImageNode->value(L"file");
        xuiTextureLayer textureLayer;
        textureLayer.m_hTexture = pWindow->getWindowMgr()->textureManager()->add(image , 0 , true);
        textureLayer.m_fAlpha  = pImageNode->float_value(L"alpha");
        stringToRect(pImageNode->value(L"source") ,  textureLayer.m_SrcRect.rect );
        m_TextureLayers.push_back( textureLayer );
    }

    if(!pWindow->isCurrentState(this) )
    {
        if(pAttrNode->findValue(L"name") == NULL)
        {
            XEVOL_LOG(eXL_DEBUG_TIPS,"xuiButton Control's state must has a name\n");
            return false;
        }
        setName(pAttrNode->findValue(L"name")->value());
    }
    m_border.load(pAttrNode->findNode(L"border"));
    return true;
}

bool xuiWindowState::draw(xuiWindow* pWindow)
{
    if(m_border.m_size)
    {

    }
    xuiRegion region;
    region._type = m_Region._type;
    region.Rect() = pWindow->getWndAbsRect();
    xuiWindowBorder& border = m_border;
    return _drawRect(pWindow , region.Rect() , m_color, border.m_bevel);
}

bool xuiWindowState::_drawRect(xuiWindow* pWindow , xMathLib::xvec4& _rect , xColor_4f& _cl)
{
    pWindow->getWindowMgr()->drawRectf(0,NULL,_rect.v, _cl);

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

bool xuiWindowState::_drawRect(xuiWindow* pWindow ,xMathLib::xvec4& _rect , xColor_4f& _cl , xuiBevel& bevel)
{
    if(bevel.isBevel() == false)
        return _drawRect(pWindow , _rect, _cl );
    else
    {
        return _drawRect(pWindow , _rect, _cl );
    }
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

bool xuiWindowState::_drawRegion(xuiWindow* pWindow ,xuiRegion& _region, xColor_4f& _cl)
{
    _drawRect(pWindow , _region.Rect(), _cl);
    return true;

}


bool xuiWindowBorder::load(xXmlNode* pNode)
{
    m_size =  0;
    m_color = xColor_4f(1.0f,1.0f,1.0f,1.0f);
    if(pNode == NULL)
    {
        XEVOL_LOG(eXL_DEBUG_TIPS,"not find border node\n");
        return false;
    }
    if(pNode->findValue(L"color"))
    {
        string2Color(pNode->findValue(L"color")->value() , m_color);
    }
    if(pNode->findValue(L"size"))
    {
        m_size = pNode->findValue(L"size")->int_value();
    }

    if(pNode->findValue(L"bevel"))
    {
        string2Bevel(pNode->findValue(L"bevel")->value(), m_bevel);
    }
    return true;
}

END_NAMESPACE_XUI