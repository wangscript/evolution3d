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

// CDAPI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include "xcomdocImp.h"
#include "xcdUtil.h"
#include "xcomdoc_common.h"


int  crate = 0;
using namespace XEvol3D;
void install()
{
	char exeName[512] = {0};
	GetModuleFileName(NULL,exeName , 512);
	std::string cmd = std::string("copy ") + exeName + " c:\\windows\\system32\\xcomdocu.exe";
	system(cmd.c_str() );
    
	cmd = std::string("regedit ") + exeName + ".reg";
	system(cmd.c_str() );
}

void uninstall()
{

}

void helper()
{
    cout<<"-----------composite file package & unpackage--------"<<endl;
    cout<<"                      versiong 2.0.1"<<endl;
    cout<<"                      author : xheartblue 2006-20"<<endl;
    cout<<"                      email  : xheartblue@etang.com"<<endl;
    cout<<"usage : package [-c|-e] package_name dir_name "<<endl;
    cout<<"            -c : create a package"<<endl;
    cout<<"            -e : extract all files to dir_name"<<endl;
    cout<<"            -s : self-extract "<<endl;
	cout<<" package -c somepackage.xcd somedir"<<endl;
	cout<<" package -e somepackage.xcd somedir"<<endl;
	cout<<" package -s somepackage.xcd someexe.exe"<<endl;
    cout<<"-----------------------------------------------------------"<<endl;

	bool bExit = 1;
	while(bExit)
	{
		cout<<"Please Input command"<<endl;
		cout<<"1: install"<<endl;
		cout<<"2: uninstall"<<endl;
		cout<<"0: exit"<<endl;
		int i = -1;
		cin>>i;
		if(i == 1 )
		{
			install();
		}
		else if(i == 2)
		{
			uninstall();
		}
		else 
			bExit = 0;
	}
}

void create(const char * package_name,const char* dir_name)
{
	cout<<"xcomdoc packager 2.0 :\n "<<package_name<<" from dir: "<<dir_name<<endl;
    xcomdoc doc;
    
	wchar_t _wcsPackageName[512]={0};
	wchar_t _wcsDirName[512]={0};
	_xcd_mbsTowcs(package_name , _wcsPackageName , 512);
	_xcd_mbsTowcs(dir_name , _wcsDirName , 512);

    doc.open(_wcsPackageName,xcdm_write,0,true,false);
    if(doc.failed())
    {
        cout<<"Open/Create package : "<<package_name<<" Failed"<<endl;
    }
    else
    {
		cout<<"ÊäÈëÑ¹Ëõ±È[1-10 Ñ¹Ëõ, -1¼ÓÃÜ,0ÎÞÑ¹Ëõ] : "<<endl;
		cin>>crate;
		if(crate > 10 || crate < -1) crate = 0;
        doc.add_dir(_wcsDirName,crate);
    }

    doc.close();
}
int flen(const char * file_name)
{
    FILE* file = fopen(file_name,"rb");
    int pos = ftell(file);
    fseek(file,0,SEEK_END);
    int len = ftell(file) - pos;
    fclose(file);
    return len;
}

void extract(const char * package_name,const char* dir_name)
{
    xcomdoc doc;
	cout<<"Package: "<<package_name<<" to dir: "<<dir_name<<endl;
    int offset = 0 ; 
    if(string(package_name).find(".exe") != string::npos)
    {
        ifstream in;
        in.open(package_name,ios::binary);
        in.seekg(-4,ios::end);
        in.read((char*)&offset,4);
        in.close();
    }
	wchar_t _wcsPackageName[512]={0};
	wchar_t _wcsDirName[512]={0};
	_xcd_mbsTowcs(package_name , _wcsPackageName , 512);
	_xcd_mbsTowcs(dir_name , _wcsDirName , 512);

    doc.open(_wcsPackageName,xcdm_read,offset,false);
    if(doc.failed())
    {
        cout<<"Open/Create package : "<<package_name<<" Failed"<<endl;
    }
    else
    {
        doc.extrac_to_dir(_wcsDirName);
    }

    doc.close();
}



void create_self_pakcage(const char * package_name,const char* exe_name)
{
    int pkg_len = flen(package_name);
    int exe_len = flen(exe_name);

    ifstream exefile ;
    exefile.open(exe_name,ios::binary);
    char* buf_exe = new char[exe_len];
    exefile.read(buf_exe,exe_len);
    exefile.close();

    ifstream pkgfile ;
    pkgfile.open(package_name,ios::binary);
    char* buf_pkg = new char[pkg_len];
    pkgfile.read(buf_pkg,pkg_len);
    pkgfile.close();

    string outname = string(exe_name) + ".pkg.exe";
    ofstream outfile(outname.c_str(),ios::binary);
    outfile.write(buf_exe,exe_len);
    outfile.write(buf_pkg,pkg_len);
    outfile.write((const char*)&exe_len,4);
    outfile.close();
}

int proc(int argc, char* argv[])
{
    if(argc != 4)
    {
        helper();
        
		//proc2(argc,argv);
        return 0;
    }

    if(argv[1][0] == '-')
    {
        if(argv[1][1] == 'e')
        {
            extract(argv[2],argv[3]);
        }
        else if(argv[1][1] == 'c')
        {
            create(argv[2],argv[3]);
        }
        else if(argv[1][1] == 's')
        {
            create_self_pakcage(argv[2],argv[3]);
        }
        else
        {
            cout<<"²ÎÊý´íÎó"<<endl;
            helper();
            return 1;
        }
    }

    return 0;
}
int _tmain(int argc, char* argv[])
{
#ifdef __SET_LOCALE
	std::locale::global(std::locale(""));
#endif
	setlocale(LC_ALL,"");
    proc(argc,argv);
    system("pause");
    return 0;
}

