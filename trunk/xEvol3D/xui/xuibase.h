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

#ifndef __XREAL3D_XUI_BASE_DEF_H__
#define __XREAL3D_XUI_BASE_DEF_H__
#include "../BaseLib/xEvol3DBaseInc.h"
#include "../BaseLib/xXmlDocument.h"
#include "../BaseLib/xXmlManager.h"
#include "../BaseLib/xI18N.h"
#include "../BaseLib/xRect.h"
#include "../image/Color.h"
#include "../Application/xApplication.h"
#include "../Application/xPlatform.h"
#include "../Application/xPluginMgr.h"
#include "../RenderAPI/xRenderAPI.h"
#ifndef BEGIN_NAMESPACE_XUI
#define BEGIN_NAMESPACE_XUI namespace XEvol3D{ namespace xui{
#define END_NAMESPACE_XUI  } }
#endif
BEGIN_NAMESPACE_XUI
END_NAMESPACE_XUI

namespace nsXui = XEvol3D::xui;
typedef xMathLib::xvec2          xuiPoint;
typedef xMathLib::xvec4          xuiRect;
typedef xMathLib::TRect<float>   xuiRect2D;

template<typename _Type > bool PtInRect(xuiRect& _rect , _Type _x , _Type _y)
{
    float l = _rect.rect.x;
    float r = _rect.rect.x + _rect.rect.w;
    float t = _rect.rect.y;
    float b = _rect.rect.y + _rect.rect.h;
    if( _x > l     &&	_x < r  &&   _y > t   &&	_y < b )
    {
        return true;
    }
    return false;
}

#pragma warning(disable : 4251)
#pragma warning(disable : 4799)
#pragma warning(disable : 4201)
#pragma warning(disable : 6011)
#pragma warning(disable : 6031)
#pragma warning(disable : 6385)

#endif