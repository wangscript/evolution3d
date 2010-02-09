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

#include "../xuiTextLabel.h"
#include "../xuiStringParser.h"
BEGIN_NAMESPACE_XUI
xuiTextLabel::xuiTextLabel(xuiWindow* parent,xuiWindowManager* pMgr):xuiControl(parent , pMgr)
{

}

xuiTextLabel::~xuiTextLabel()
{

}

bool xuiTextLabel::load(xXmlNode* pCfgNode)
{
    xuiWindow::load(pCfgNode);
    xXmlNode* pAttrNode = pCfgNode->findNode(L"attribute");
    if(pAttrNode == NULL) pAttrNode = pCfgNode;

    const wchar_t * pTextValue = pAttrNode->value(L"text");
    if(pTextValue)
    {
        setText(pTextValue);
    }

    return true;
}

bool xuiTextLabel::draw()
{
    return false;
}

void xuiTextLabel::setText(const wchar_t* _text)
{
    m_text = _text;
}
END_NAMESPACE_XUI
