#include "../xStdPch.h"
#ifdef _LINUX
#include <dirent.h>
#endif

#ifdef _MSC_VER
#pragma warning(disable:4996)
#include <Windows.h>
#endif


#ifdef _USE_XEVOL3D_NEW_
#include "../mem/mem_operator.h"
#endif

#include "xFileSystem.h"
#include "../BaseLib/xI18N.h"
BEGIN_NAMESPACE_XEVOL3D


IMPL_REFCOUNT_OBJECT_FUNCTION(xFileStream);

static FILE* xOpenFileRWB(const wchar_t* wcsFileName)
{
#ifdef _WIN32
	FILE* fp = _wfopen(wcsFileName , L"wb+");
#else
	int8 mbsFileName[512]={0};
	XEvol_UnicodeToFsEnc(wcsFileName,mbsFileName,512);
	FILE* fp = fopen(mbsFileName , "wb+");
#endif
	return fp;
}

static FILE* OpenFileRB(const wchar_t* wcsFileName)
{
#ifdef _WIN32
	FILE* fp = _wfopen(wcsFileName , L"rb");
#else
	int8 mbsFileName[512]={0};
	XEvol_UnicodeToFsEnc(wcsFileName,mbsFileName,512);
	FILE* fp = fopen(mbsFileName , "rb");
#endif
	return fp;
}


xFileStream::xFileStream(bool bWriteable)
{
	m_fStream = NULL;
    m_bWriteable = bWriteable;
}

xFileStream::~xFileStream()
{
	m_fStream = NULL;
}

bool xFileStream::open_stream(const wchar_t* _fileName)
{
    if(m_bWriteable)
        m_fStream = xOpenFileRWB(_fileName);
    else
        m_fStream = OpenFileRB(_fileName);

	return m_fStream != NULL;
}

int xFileStream::close()
{
	return fclose(m_fStream);
}

size_t xFileStream::read(char* buf, size_t byte_read)
{
	return fread(buf,1 , byte_read,m_fStream);
}

size_t xFileStream::write(char* buf,size_t byte_read)
{
	return fwrite(buf,1 , byte_read,m_fStream);
}

size_t xFileStream::seek(long _offset, std::ios_base::seekdir _dir)
{
	switch(_dir)
	{
	case std::ios_base::beg:
		fseek(m_fStream,(long)_offset,SEEK_SET);
		break;
	case std::ios_base::end:
		fseek(m_fStream,(long)_offset,SEEK_END);
		break;
	case std::ios_base::cur:
		fseek(m_fStream,(long)_offset,SEEK_CUR);
		break;
	}
	return tell();
}

size_t xFileStream::tell()
{
	return ftell(m_fStream);
}

bool xFileStream::eof()
{
	return  feof(m_fStream) != 0;
}



xFileSystem* xFileSystem::singleton()
{
	static xFileSystem sys;
	return &sys;
}



std::ds_wstring xFileSystem::getFileName(const wchar_t* fullName)
{
	if(fullName == NULL)
		return L"";
	int idx = (int)wcslen(fullName);
	for(; idx >= 0 ; idx --)
	{
		if(fullName[idx] == '\\' || fullName[idx]=='/')
		{
			break;
		}
	}
	std::ds_wstring _ret = fullName + (idx + 1);   
	return _ret;
}

std::ds_wstring xFileSystem::getPathName(const wchar_t* fullName)
{
	if(fullName == NULL)
		return L"";
	std::ds_wstring _ret = fullName;   
	std::ds_wstring::size_type pos = _ret.rfind('\\');
	if(pos == std::ds_wstring::npos) 
	{
		pos = _ret.rfind('/');
	}
	if(pos == std::ds_wstring::npos )
	{
		return L"";
	}

	_ret.replace(pos ,wcslen(fullName), L"");
	return _ret + L"/";
}

bool xFileSystem::fileExist(const wchar_t* wcsFileName)
{
	FILE* fp = OpenFileRB(wcsFileName);
	bool bRet = fp != NULL;
	if(fp) fclose(fp);
	return bRet;
}

int xFileSystem::loadFile( unsigned int8*& fileBufer , const wchar_t* wcsFileName)
{
	FILE* fp = OpenFileRB(wcsFileName);
	if(fp == NULL)
	{
		fileBufer = NULL;
		return 0;
	}

	fseek(fp,0,SEEK_END);
	int fLen = ftell(fp);
	fileBufer = new  unsigned int8[fLen + 2];
    fseek(fp,0,SEEK_SET);
	fread(fileBufer,1,fLen,fp);
	fileBufer[fLen] = 0;
	fileBufer[fLen + 1] = 0;
	fclose(fp);
	return fLen;
}

void xFileSystem::unloadFile( unsigned int8*& fileBufer)
{
	delete [] fileBufer;
	fileBufer = NULL;
}
bool xFileSystem::loadFile(const wchar_t* fileName , std::ifstream& stream , std::ios::open_mode _mode)
{
#ifdef _WIN32
	stream.open(fileName , _mode);
#else
	int8 mbsFileName[512]={0};
	XEvol_UnicodeToFsEnc(fileName,mbsFileName,512);
	stream.open(mbsFileName , _mode);
#endif

	
	if(stream.fail())
		return false;
	return true;
}

bool xFileSystem::openFile(const wchar_t* fileName , std::fstream& stream , std::ios::open_mode _mode)
{
#ifdef _WIN32
	stream.open(fileName , _mode);
#else
	int8 mbsFileName[512]={0};
	XEvol_UnicodeToFsEnc(fileName,mbsFileName,512);
	stream.open(mbsFileName , _mode);
#endif
	if(stream.fail())
		return false;
	return true;
}

IStreamIO* xFileSystem::loadFile(const wchar_t* fileStream , bool bWrite )
{
    xFileStream* pFileStream = new xFileStream(bWrite);
	if(true == pFileStream->open_stream(fileStream) )
		return pFileStream;
	delete pFileStream;
    return NULL;
}

const wchar_t* xFileSystem::getFileExtName(const wchar_t* filename )
{
	static wchar_t name_ext[16]={0};
	name_ext[0] = 0;
	size_t  len = wcslen(filename);

	for( size_t i = len-1 ; i > 0 ; i -- )
	{
		if(filename[i] == '.')
		{
			wcsncpy(name_ext,filename + i + 1,16);
			{
				for(size_t j=0;j< (len - i) ; j++)
				{
					if(name_ext[j] < 127)
					{
				        name_ext[j] = tolower(name_ext[j]);
					}	
				}
			}
			break ;
		}
	}
	return name_ext;
}



xDirEnum::xDirEnum()
{

}

xDirEnum::~xDirEnum()
{ 
	m_vListners.clear(); 
}

void xDirEnum::addListenner(IDirEnumListenner* pListner)
{
	size_t nListeners = m_vListners.size()  ;
	for(size_t i = 0 ; i <  nListeners ; i++ )
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

bool xDirEnum::__findFile(xDirEnumItem& item)
{
	bool bRet = true;
	size_t _nListeners = m_vListners.size();
	for(size_t i = 0 ; i < _nListeners  ; i++ )
	{
		bool bRet2 = m_vListners[i]->onFindFile(item);
		bRet = bRet && bRet2;
	}
	return bRet;
}


static int ___fileSize(const wchar_t* wcsFileName)
{
	FILE* fp = OpenFileRB(wcsFileName);
	if(fp == NULL)
		return 0;
	fseek(fp,0,SEEK_END);
	int len = ftell(fp);
	fclose(fp);
	return len;
}

#ifdef _LINUX
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

		std::string newFileName = path + std::string("/") + std::string(dir_ent->d_name);

                
		xDirEnumItem item;
		item.m_isDir = ___isDir(newFileName.c_str());
		wchar_t wcsName[512]={0};
		XEvol_FsEncToUnicode(dir_ent->d_name,wcsName,512);
		item.m_Name = wcsName;

		std::string ansiPath = path + "/";
        wchar_t wcsPath[512]={0};
		XEvol_FsEncToUnicode(ansiPath.c_str() ,wcsPath,512);
		item.m_Path = wcsPath;
        //printf("ansiPath = %s\n unicode path=",ansiPath.c_str());
        //XEVOL_LOG(eXL_DEBUG_HIGH,wcsPath);
		printf("\n");
                 
		
		XEvol_FsEncToUnicode(newFileName.c_str() ,wcsName,512);
		item.m_FullName = wcsName;



		bool bRet = __findFile(item);
		if(recursive && item.m_isDir && bRet)
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
	std::ds_wstring path = dir_name;
	int count = 1;
	for(size_t i = 0 ; i < path.length() ; ++i)
		if(path[i] == '\\')  path[i] = '/';


	if(path[path.length()-1] != '/') 
		path += L"/";

	std::ds_wstring find_file = path + L"*.*";

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
			std::ds_wstring newPath =  path + fd.cFileName + L"/";
			item.m_FullName = newPath;
			item.m_isDir = true;
			bool bRet = __findFile(item);
			if(recursive && bRet)
				enumDir(newPath.c_str(),recursive);
		}
		else
		{
			std::ds_wstring file_name =  path + fd.cFileName;
			item.m_FullName = file_name;
			__findFile(item);			
		}

	}
	FindClose(hf);
}
#endif

END_NAMESPACE_XEVOL3D



