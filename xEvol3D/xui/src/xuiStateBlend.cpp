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
BEGIN_NAMESPACE_XUI



xuiWindowStateBlender::xuiWindowStateBlender(xuiWindow* pWindow)
{
    m_time = 0;
    m_totalTime = 0;
    m_pWindow   = pWindow;
}

xuiWindowStateBlender::~xuiWindowStateBlender()
{

}
static void __lerp(xuiBevel& out, xuiBevel& v1 , xuiBevel& v2, float _f)
{
    out.bl =   v1.bl *  (1.0f - _f) + v2.bl * _f;
    out.br =   v1.br *  (1.0f - _f) + v2.br * _f;
    out.tl =   v1.tl *  (1.0f - _f) + v2.tl * _f;
    out.tr =   v1.tr *  (1.0f - _f) + v2.tr * _f;
}

static void __lerp(xuiRegion& out, xuiRegion& v1 , xuiRegion& v2, float _f)
{
    if(v1._type == xuiRegion::eRT_Rect)
    {
        out.Rect2D().x =   v1.Rect2D().x *  (1.0f - _f) + v2.Rect2D().x * _f;
        out.Rect2D().y =   v1.Rect2D().y *  (1.0f - _f) + v2.Rect2D().y * _f;
        out.Rect2D().w =   v1.Rect2D().w *  (1.0f - _f) + v2.Rect2D().w * _f;
        out.Rect2D().h =   v1.Rect2D().h *  (1.0f - _f) + v2.Rect2D().h * _f;
    }

}

static void __lerp(xColor_4f& out, xColor_4f& v1 , xColor_4f& v2, float _f)
{
    out.r = v1.r *  (1.0f - _f) + v2.r * _f;
    out.g = v1.g *  (1.0f - _f) + v2.g * _f;
    out.b = v1.b *  (1.0f - _f) + v2.b * _f;
    out.a = v1.a *  (1.0f - _f) + v2.a * _f;
}

static void __lerp(float& out, float& v1 , float& v2, float _f)
{
    out = v1 *  (1.0f - _f) + v2 * _f;
}

static void __lerp(int& out, int& v1 , int& v2, float _f)
{
    out = (int)(v1 *  (1.0f - _f) + v2 * _f);
}


bool xuiWindowStateBlender::update(long passedTime)
{
    if(m_time > 0) m_time -= passedTime;
    else return true;


    float sPercent = m_time/float(m_totalTime);
    if(sPercent<0.0f)sPercent = 0.0f;
    float ePercent = 1.0f - sPercent;

    xuiWindowState _state ;
    __lerp(_state.m_color          , m_Start.m_color          , m_End.m_color          , ePercent);
    __lerp(_state.m_textcolor      , m_Start.m_textcolor      , m_End.m_textcolor      , ePercent);
    __lerp(_state.m_border.m_bevel , m_Start.m_border.m_bevel , m_End.m_border.m_bevel , ePercent);
    __lerp(_state.m_border.m_color , m_Start.m_border.m_color , m_End.m_border.m_color , ePercent);
    __lerp(_state.m_border.m_size  , m_Start.m_border.m_size  , m_End.m_border.m_size  , ePercent);
    __lerp(_state.m_Region         , m_Start.m_Region         , m_End.m_Region         , ePercent);
    m_pWindow->__setState(&_state);
    return true;
}

bool xuiWindowStateBlender::isInBlending() const
{
    return m_time  >  0;
}
bool xuiWindowStateBlender::stop()
{
    m_time = 0;
    m_totalTime = 0;
    return true;
}
bool xuiWindowStateBlender::setState(xuiWindowState* pStartState , xuiWindowState* pEndState , long _time)
{
    m_End = *pEndState;
    m_Start = *pStartState;
    m_totalTime = m_time = _time;
    return true;
}

bool xuiWindowStateBlender::setState(xuiWindowState* pEndState , long _time)
{
    m_End = *pEndState;
    m_Start = m_pWindow->m_CurState;
    m_totalTime = m_time  = _time; 
    return true;
}


END_NAMESPACE_XUI