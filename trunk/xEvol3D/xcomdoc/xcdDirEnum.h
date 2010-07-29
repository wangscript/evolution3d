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
#ifndef __XEVOL3D_DIR_ENUM_H__
#define __XEVOL3D_DIR_ENUM_H__
#include <vector>
#include <string>
#include "xcomdoc_common.h"
BEGIN_NAMESPACE_XEVOL3D
namespace xComdoc 
{
	struct xDirEnumItem
	{
		std::wstring m_Name;
		std::wstring m_Path;
		std::wstring m_FullName;
		bool         m_isDir;
		int          m_FileSizeLow;
		int          m_FileSizeHi;
	};

	class IDirEnumListenner
	{
	public:
		virtual bool onFindFile(xDirEnumItem& item) = 0;
	};

	class xDirEnum
	{
#ifdef _UNIX
	void _enumDir(const wchar_t* wcsDirName, bool recursive);
#endif
	public:
		typedef std::vector<IDirEnumListenner*> LISTENNERS;
	public:
		xDirEnum();
		~xDirEnum();

		void       enumDir(const wchar_t* dir_name, bool recursive);
		void       addListenner(IDirEnumListenner* pListner);
		void       removeListenner(IDirEnumListenner* pListner);
	protected:
		LISTENNERS m_vListners;
		void       __findFile(xDirEnumItem& item);

	};
};
END_NAMESPACE_XEVOL3D

#endif
