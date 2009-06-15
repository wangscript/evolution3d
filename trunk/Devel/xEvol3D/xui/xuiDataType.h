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

#ifndef __XUI_DATA_TYPE_H__
#define __XUI_DATA_TYPE_H__
#include "xuibase.h"

BEGIN_NAMESPACE_XUI

struct xuiEllipse_t
{
    float x , y;
    float a , b;
};

struct xuiBevel
{
    float tl, tr,bl,br;
    xuiBevel(float _tl = 0 ,float _tr = 0,float _br = 0,float _bl = 0)
        :tl(_tl), tr(_tr),bl(_bl),br(_br)
    {
    }
    bool isBevel()
    {
        return (tl>0 || tr>0 || bl>0 || br>0);
    }
};

struct xuiRegion
{
    enum eRegionType
    {
        eRT_Rect,
        eRT_Ellipse
    };
    eRegionType        _type;
    xuiRect2D&          Rect2D(){ return _rect.rect ; }
    xuiRect&           Rect(){  return _rect ; };
protected:
    xMathLib::xvec4    _rect;
public:
    xuiRegion()
    {
        _type = eRT_Rect;
    }
};


END_NAMESPACE_XUI


#endif
