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


#ifndef _XMATH_LINE_
#define _XMATH_LINE_
#include "xmathbasedef.h"
#include <vector>
#include "xMathLib.h"


BEGIN_NAMESPACE_XGEOMLIB

class _XEVOL_BASE_API_ xline
{
public:	
	xline(xMathLib::xvec3 point,xMathLib::xvec3 dir){m_Point = point;m_Dir = dir;}

public:
	xMathLib::xvec3     m_Point;
	xMathLib::xvec3     m_Dir;
};


class  _XEVOL_BASE_API_ xlineseg
{
public:
	xlineseg(xMathLib::xvec3 start,xMathLib::xvec3 end){m_Start = start;m_End = end;};
	xlineseg(xMathLib::xvec3 start,xMathLib::xvec3 dir,float len);

public:
	xMathLib::xvec3 m_Start;
	xMathLib::xvec3 m_End;
};

class  _XEVOL_BASE_API_ xray
{
public:
	xMathLib::xvec3     m_Point;
	xMathLib::xvec3     m_Dir;      
};


END_NAMESPACE_XGEOMLIB
#endif

