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
#ifdef _LINUX
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


class _XEVOL_BASE_API_  xFileSystem
{
	xFileSystem(){};
public:

public:
	const wchar_t*      getFileExtName(const wchar_t* filename );
	int                 loadFile( unsigned int8*& fileBufer , const wchar_t* fileName);
	void                unloadFile( unsigned int8*& fileBufer);
	bool                fileExist(const wchar_t* filename);
	IRWStream*          loadFile(const wchar_t* fileStream);
	static xFileSystem* singleton();
};


END_NAMESPACE_XEVOL3D

#endif
