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
#include "../xStdPch.h"
#include "xcdDirEnum.h"

#ifdef _UNIX
#include <dirent.h>
#endif

#ifdef _WIN32
#pragma warning(disable:4996)
#include <Windows.h>
#endif


#ifdef _USE_XEVOL3D_NEW_
#include "../mem/mem_operator.h"
#endif

BEGIN_NAMESPACE_XEVOL3D
namespace xComdoc 
{
	xDirEnum::xDirEnum()
	{

	}

	xDirEnum::~xDirEnum()
	{ 
		m_vListners.clear(); 
	}

	void xDirEnum::addListenner(IDirEnumListenner* pListner)
	{
		size_t nSize = m_vListners.size();
		for(size_t i = 0 ; i <   nSize; i++ )
		{
			if(m_vListners[i] == pListner)
				return ;
		}
		m_vListners.push_back(pListner);
	}

	void xDirEnum::removeListenner(IDirEnumListenner* pListner)
	{
		for(LISTENNERS::iterator i = m_vListners.begin() ; i != m_vListners.end() ; ++i)
		{
			if(*i == pListner)
			{
				m_vListners.erase(i);
				return ;
			}
		}
		return ;
	}

	void xDirEnum::__findFile(xDirEnumItem& item)
	{
		size_t nSize = m_vListners.size();
		for(size_t i = 0 ; i <  nSize ; i++ )
		{
			m_vListners[i]->onFindFile(item);
		}
	}


	static int ___fileSize(const wchar_t* wcsFileName)
	{
		FILE* file = _xcd_wfopen_rb(wcsFileName);
		if(file == NULL)
			return 0;
		fseek(file,0,SEEK_END);
		int len = ftell(file);
		fclose(file);
		return len;
	}

#ifdef _UNIX
static bool ___isDir(const char* dir_name)
{
	DIR* dirp = opendir(dir_name);
	bool bIsDir = dirp != NULL;
	closedir(dirp);
	return bIsDir;
}


void xDirEnum::_enumDir(const wchar_t* wcsDirName, bool recursive)
{
    int8 dir_name[512]={0};
	XEvol_UnicodeToFsEnc(wcsDirName,dir_name,512);

	char strPath[256];
	strcpy(strPath, dir_name);
	int count = 1;
	for(size_t i = 0 ; i < (int)strlen(strPath) ; ++i)
		if(strPath[i] == '\\')  strPath[i] = '/';

	if( strPath[ (int)strlen(strPath) - 1] == '/')
		strPath[ (int)strlen(strPath) - 1] = 0;

	std::string path = strPath;
	DIR* dirp =  opendir(path.c_str());
	if(dirp == NULL)
		return ;

	dirent* dir_ent;

	while(dir_ent = readdir(dirp))
	{

		if( std::string(".") == dir_ent->d_name )
			continue;
		if( std::string("..") == dir_ent->d_name )
			continue;

		std::string path1 = path + std::string("/");
		std::string newFileName = path1 + std::string(dir_ent->d_name);
		xDirEnumItem item;
		item.m_isDir = ___isDir(newFileName.c_str());
		wchar_t wcsName[512]={0};
		XEvol_FsEncToUnicode(dir_ent->d_name,wcsName,512);

		std::string ansiPath = path + "/";
		XEvol_FsEncToUnicode(ansiPath.c_str() ,wcsName,512);
		item.m_Path = wcsName;


		item.m_Name = wcsName;
		XEvol_FsEncToUnicode(newFileName.c_str() ,wcsName,512);
		item.m_FullName = wcsName;



		__findFile(item);
		if(recursive && item.m_isDir)
		{
			_enumDir(wcsName , recursive);
		}
	}
	closedir(dirp);
}

void xDirEnum::enumDir(const wchar_t* wcsDirName, bool recursive)
{
	int8 dir_name[512]={0};
	XEvol_UnicodeToFsEnc(wcsDirName,dir_name,512);
	_enumDir(wcsDirName, recursive );
}
#endif

#ifdef _MSC_VER
	void xDirEnum::enumDir(const wchar_t* dir_name, bool recursive)
	{
		WIN32_FIND_DATAW fd;
		std::wstring path = dir_name;
		int count = 1;
		for(size_t i = 0 ; i < path.length() ; ++i)
			if(path[i] == '\\')  path[i] = '/';


		if(path[path.length()-1] != '/') 
			path += L"/";

		std::wstring find_file = path + L"*.*";

		HANDLE hf = ::FindFirstFileW(find_file.c_str(),&fd); 

		while( FindNextFileW(hf,&fd)	)
		{

			if(wcscmp(fd.cFileName,L"..")==0 || wcscmp(fd.cFileName,L".")==0)
				continue;
			xDirEnumItem item;
			item.m_isDir = false;
			item.m_Name = fd.cFileName;
			item.m_Path = path;
			if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{   
				std::wstring newPath =  path + fd.cFileName + L"/";
				item.m_FullName = newPath;
				item.m_isDir = true;
				__findFile(item);
				if(recursive)
					enumDir(newPath.c_str(),recursive);
			}
			else
			{
				std::wstring file_name =  path + fd.cFileName;
				item.m_FullName = file_name;
				__findFile(item);			
			}

		}
		FindClose(hf);
	}
#endif
};


bool xcd_is_dir(const wchar_t* _dir)
{
#ifdef _WIN32
	DWORD   dw=GetFileAttributesW( _dir );   
	if   (dw!=0xffffffff   &&   dw   &   FILE_ATTRIBUTE_DIRECTORY)   
	{   
		return true;
	}   
	else   
	{   
		return false;   
	}  
#else
	int8 mbsFileName[512]={0};
	_xcd_wcsTombs(_dir,mbsFileName,512);
	return ___isDir(_dir);
#endif

}

END_NAMESPACE_XEVOL3D

