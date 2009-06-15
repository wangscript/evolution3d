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

#ifndef __XUI_TEXT_LABEL_H__
#define __XUI_TEXT_LABEL_H__
#include "xuiControl.h"
BEGIN_NAMESPACE_XUI
//TextLabel is a single line text. 
//TextArea is a multi-line text .
class _XEVOL_BASE_API_ xuiTextLabel : public xuiControl
{
protected:
    wstring   m_text;
public:
    xuiTextLabel(xuiWindow* parent, xuiWindowManager* pMgr);
    ~xuiTextLabel();
    virtual bool load(xXmlNode* pCfgNode); // virtual ;
    virtual bool draw();
    virtual void setText(const wchar_t* _text);
};
END_NAMESPACE_XUI

#endif

