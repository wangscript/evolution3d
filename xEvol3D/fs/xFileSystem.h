#ifndef __XBASE_FILESYSTEM_H__
#define __XBASE_FILESYSTEM_H__

#include "../BaseLib/xEvol3DBaseInc.h"
#include "../BaseLib/stl_dllalloc.h"
#include <vector>
BEGIN_NAMESPACE_XEVOL3D

struct _XEVOL_BASE_API_  xDirEnumItem
{
	std::ds_wstring m_Name;
	std::ds_wstring m_Path;
	std::ds_wstring m_FullName;
	bool            m_isDir;
	int             m_FileSizeLow;
	int             m_FileSizeHi;
};

class _XEVOL_BASE_API_  IDirEnumListenner
{
public:
	virtual bool onFindFile(xDirEnumItem& item) = 0;
};

class _XEVOL_BASE_API_  xDirEnum
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
	bool       __findFile(xDirEnumItem& item);

};


class xFileStream : public IStreamIO
{
	FILE* m_fStream;
	DECL_REFCOUNT_OBJECT_INTERFACE(xFileStream)
public:	
	xFileStream(bool bWriteable = false);
	~xFileStream();
	size_t   read(char* buf, size_t byte_read);
	size_t   write(char* buf,size_t len);
	size_t   seek(long _offset, std::ios_base::seekdir _dir);
	size_t   tell();
	int      close();	
	bool     eof();
public:
	bool     open_stream(const wchar_t* _fileName);
    bool     m_bWriteable;
};

class _XEVOL_BASE_API_  xFileSystem
{
	xFileSystem(){};
public:

public:
	std::ds_wstring        getFileName(const wchar_t* fullName);
	std::ds_wstring        getPathName(const wchar_t* fullName);
	const wchar_t*         getFileExtName(const wchar_t* filename );
	int                    loadFile( unsigned int8*& fileBufer , const wchar_t* fileName);
	void                   unloadFile( unsigned int8*& fileBufer);
	bool                   fileExist(const wchar_t* filename);
	IStreamIO*             loadFile(const wchar_t* fileStream , bool bWrite = false);
	bool                   loadFile(const wchar_t* fileName , std::ifstream& stream , std::ios::open_mode _mode  = std::ios::in | std::ios::binary );
	bool                   openFile(const wchar_t* fileName , std::fstream& stream , std::ios::open_mode _mode  =  std::ios::binary);
    static xFileSystem*    singleton();
    template <typename T>void
    fileNameWithoutExt(const wchar_t* name , T& out)
    {
        for(size_t i = 0 ; i < wcslen(name) ; i ++)
        {
            if(name[i] == '.') break;
            out.push_back( name[i] );
        }
    }

    template <typename T>void
    fileNameWithoutExt(const char* name , T& out)
    {
        for(size_t i = 0 ; i < strlen(name) ; i ++)
        {
            if(name[i] == '.') break;
            out.push_back( name[i] );
        }
    }

};

END_NAMESPACE_XEVOL3D

#endif
