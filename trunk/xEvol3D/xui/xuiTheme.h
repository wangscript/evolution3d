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

#ifndef __XUI_WINDOW_THEME_H__
#define __XUI_WINDOW_THEME_H__
#include "xuibase.h"
#include "xuiDataType.h"
#include "../font/xFontManager.h"
BEGIN_NAMESPACE_XUI
class _XEVOL_BASE_API_ xuiWindowBorder;
class _XEVOL_BASE_API_ xuiWindowTransform;
class _XEVOL_BASE_API_ xuiTextureLayer;

class _XEVOL_BASE_API_ xuiWindowState;
class _XEVOL_BASE_API_ xuiWindowStateBlender;
class _XEVOL_BASE_API_ xuiWindowTransBlender;
class _XEVOL_BASE_API_ xuiWindowState;

typedef std::vector<xuiWindowState*, dllsafe_alloc<xuiWindowState*>> vXuiWindowState;
typedef ds_vector(xuiTextureLayer) xuiTextureLayers;

class _XEVOL_BASE_API_ xuiTheme
{
public:
    virtual xuiWindowStateBlender* createStateBlender(const wchar_t* stateBlenderType) = 0;
    virtual xuiWindowState*        createState(const wchar_t* _name , xuiWindowState* pTemplate = NULL) = 0;
    virtual xuiWindowTransBlender* createTransBlender(const wchar_t* transBlenderType) = 0;
    virtual xuiWindowBorder*       createBorder(const wchar_t* borderType) = 0; 
};


END_NAMESPACE_XUI

#endif

