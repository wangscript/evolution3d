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

#include "XMTrace.h"

ostream& operator <<(ostream& out,xmat4& m)
{
	for(int r = 0 ; r < 4 ; r++)
	{
		for(int c = 0 ; c < 4 ; c++)
		{
			out.width(14);
			out<<m.m[r][c];
		}
		out<<endl;
	}
	return out;
}
ostream& operator <<(ostream& out,xvec4& v)
{
	out<<" x: "<<v.x<<"  y: "<<v.y<<"  z: "<<v.z<<"  w: "<<v.w<<endl;
	return out;
}

