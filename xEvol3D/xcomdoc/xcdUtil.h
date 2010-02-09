/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) . (http://gforge.osdn.net.cn/projects/xevol3d/) .
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

#ifndef __CDAPI_UTIL_H__
#define __CDAPI_UTIL_H__
#include "xcomdoc_common.h"
BEGIN_NAMESPACE_XEVOL3D

class xComDocument;
class XComDocStream;
void AddDirectoryRecurse(IComDocBase*  pDoc,const wchar_t* dir_name,int compressed_rate);
void ExtractToDirectory(IComDocBase* pDoc,const wchar_t* bas_dir);
void create_all_dir(const wchar_t* bas_dir,const wchar_t* file);
END_NAMESPACE_XEVOL3D
#endif

