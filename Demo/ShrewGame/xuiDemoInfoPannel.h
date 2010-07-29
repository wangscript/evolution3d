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

#ifndef __DEMO_UI_INFOPANNEL_H__
#define __DEMO_UI_INFOPANNEL_H__

#include <xui/xuiDialog.h>
BEGIN_NAMESPACE_XUI

class  xuiDemoInfoPannel : public xuiDialog
{
	wstring m_text;
public:
	xuiDemoInfoPannel(xuiWindow* parent , xuiWindowManager* pMgr);
	~xuiDemoInfoPannel();
	virtual bool   onNotify(eXUIWinNotifyEvent _event, xuiWindow* pCtrl , int param1 , int param2);
	virtual bool   onLoad(xXmlNode* pCfgNode);
	virtual bool   onDraw();
	void           setText(const wchar_t* _text);

public:
	IBaseTexture*  m_pTextCacheTexture;
};
END_NAMESPACE_XUI

#endif
