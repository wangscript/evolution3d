/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) .
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

#ifndef _MEM_DEFINE_H_
#define _MEM_DEFINE_H_

#define MAX_SMALLMEM_SIZE  256
#define DEFUALT_CHUNK_SIZE 8192

#define GUARD_BYTE 0XCCCC

#define BEGIN_XEVOL3D_MEM namespace XEvol3D{ namespace Mem{ 

#define END_XEVOL3D_MEM	}}

#include "../BaseLib/xEvol3DBaseInc.h"
#endif

