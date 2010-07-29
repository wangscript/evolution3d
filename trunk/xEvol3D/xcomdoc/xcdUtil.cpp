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
#define _USED_IN_EDITOR

#include "xcomdocImp.h"
#include "xcdUtil.h"
#include <fstream>
#include <string>
#include "xcdDirEnum.h"
using namespace std;
static int FileLen();

#ifdef _USE_XEVOL3D_NEW_
#include "../mem/mem_operator.h"
#endif

#if (defined(_WIN32) || defined(_WIN64) )
#include <windows.h>
# ifndef _WIN32_WCE
#include <direct.h>
# endif
#endif

#ifdef _UNIX
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

BEGIN_NAMESPACE_XEVOL3D
using namespace xComdoc;
class CDirComDocBuilder : public IDirEnumListenner
{
public:

	xComDocument* m_pDoc;
	int      m_Rate;
	wstring   m_BaseDir;
public:
	bool onFindFile(xDirEnumItem& item)
	{
		wstring name = L"";
		if( item.m_FullName[m_BaseDir.length()] == '/')
			name = (item.m_FullName.c_str() + m_BaseDir.length() + 1);
		else
			name = (item.m_FullName.c_str() + m_BaseDir.length() );

		if(item.m_isDir)
		{
			m_pDoc->create_stream(name.c_str(),m_Rate,xcddt_dir_flag);
		}
		else
		{
			xStreamContext* pContext =  (xStreamContext* )m_pDoc->create_stream_with_dir(name.c_str(),m_Rate);
			pContext->m_pStream->_set_src_file(item.m_FullName.c_str(),true);
			m_pDoc->m_TotalSrcSize += item.m_FileSizeLow;
			delete pContext;
		}
		return true;
	}
};

void AddDirectoryRecurse(IComDocBase* _doc,const wchar_t* dir_name,int compressed_rate)
{
	xComDocument * pDoc = type_cast<xComDocument*>(_doc); 
	if(pDoc == NULL)
		return ;

	pDoc->m_TotalSrcSize = 0;
	pDoc->m_bInProccesPackage = true;
	CDirComDocBuilder builder;
	builder.m_BaseDir = dir_name;
	builder.m_pDoc = pDoc;
	builder.m_Rate = compressed_rate;
	xDirEnum enumDir;
	enumDir.addListenner(&builder);
	enumDir.enumDir(dir_name,true);
	//parseDir(pDoc,dir_name,NULL,0,compressed_rate);
}

void create_all_dir(const wchar_t* bas_dir,const wchar_t* file)
{
	wchar_t dir_name[128];
	for(int i = 0 ; i < (int) wcslen(file) ; ++i)
	{
		if(file[i] == '/' || file[i] == '\\')
		{
			wcsncpy(dir_name,file,128);
			dir_name[i] = 0;
			wstring dir_full_name = std::wstring(bas_dir) + dir_name;

			char mbsDirName[512]={0};
			_xcd_wcsTombs(dir_full_name.c_str() , mbsDirName , 512);
#ifdef _WIN32
			_mkdir(mbsDirName);
#else

			mkdir(mbsDirName,0777);
#endif

		}
	}
}


void ExtractToDirectory(IComDocBase* _doc,const wchar_t* bas_dir)
{
	xComDocument * pDoc = type_cast<xComDocument*>(_doc); 
	if(pDoc == NULL)
		return ;

	wchar_t _bas_dir[128];
	wcscpy(_bas_dir,bas_dir);
	int len = (int)wcslen(_bas_dir);
	for(int i =0 ; i < len ; i++)
	{
		if(_bas_dir[i] == '\\') _bas_dir[i] = '/';
	}
	if(_bas_dir[len-1] != '/')
		wcscat(_bas_dir,L"/");  


	create_all_dir(L"",_bas_dir);
	int nEntrys = (int)pDoc->m_DataEntrys.size();
	for(int i = 0 ; i < nEntrys; ++i)
	{
		xComDocument::DataEntry& entry = pDoc->m_DataEntrys[i];
		create_all_dir(_bas_dir,entry.m_DataEntryItem.getDataName() );
		if(! xcd_is_dir(entry.m_DataEntryItem.m_DataType))
		{
			wstring file_name = std::wstring(_bas_dir) + entry.m_DataEntryItem.getDataName();

#ifdef _WIN32
	        ofstream out(file_name.c_str(),ios::binary);
#else
			char mbcFileName[512] = {0};    
	        _xcd_wcsTombs(file_name.c_str(),mbcFileName,512);
	        ofstream out(mbcFileName,ios::binary);
#endif


			int len = (int)entry.m_pStream->data_len();
			_xcd_int8* buf = new _xcd_int8[len];
			entry.m_pStream->read(buf,len);
			out.write(buf,len);
			out.close();
		}

	}
}
END_NAMESPACE_XEVOL3D

