/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
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

#ifndef __APPLICATION_LAYER_H__
#define __APPLICATION_LAYER_H__
#include "../BaseLib/xEvol3DBaseInc.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_  IAppLayer
{
public:
	virtual bool onHandle() = 0;
	virtual bool onUnhandle() = 0;
	virtual bool updateFrame(long timePassed ) = 0;

	virtual bool preHandle();	
	virtual bool preUnhandle() ;
	virtual bool postHandle();	
	virtual bool postUnhandle() ;
	
	virtual bool prevUpdateFrame(long timePassed = 0) ;
	virtual bool postUpdateFrame(long timePassed = 0)  ;

	virtual const wchar_t* name() = 0;
};

END_NAMESPACE_XEVOL3D
#endif
