#ifndef __XFILE_MANAGER_H__
#define __XFILE_MANAGER_H__

#include "../BaseLib/xResPackageMgr.h"
#include "xFileSystem.h"
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
enum 
{
    eOpen_Binary = 0x0001,
    eOpen_Write  = 0x0002,
    eOpen_Read   = 0x0004,
    eOpen_Exist  = 0x0008,
    eOpen_Append = 0x0010,
};

class  _XEVOL_BASE_API_  xFILELoader : public xResPathManager
{
protected:
    unsigned int _getResSize(FILE* pRes);
    virtual bool _isResLoaded(FILE* pRes);
    virtual bool _preLoadResource(const std::ds_wstring& strResName    , FILE* & pRes , int& ResSize, unsigned int arg) { return true ; }
    virtual bool _postLoadResource(const std::ds_wstring& strResName   , FILE* & pRes , int& ResSize, unsigned int arg){ return true ; }
    virtual bool _loadResource(const std::ds_wstring& strResName       , FILE* & pRes , int& ResSize, unsigned int arg);
    virtual bool _unloadResource(const std::ds_wstring& strResName     , FILE* & pRes , unsigned int& TotalResSize);
    virtual void _deleteResource(const std::ds_wstring& strResName     , FILE* pRes);
};

typedef TResHandle <FILE      , std::ds_wstring , xFILELoader> HFILE;
typedef TLRUPolicyResMgr<FILE , std::ds_wstring , xFILELoader> xFILEMgr;


class  _XEVOL_BASE_API_  xStreamIOLoader : public xResPathManager
{
protected:
    unsigned int _getResSize(IStreamIO* pRes);
    virtual bool _isResLoaded(IStreamIO* pRes);
    virtual bool _preLoadResource(const std::ds_wstring& strResName    , IStreamIO* & pRes , int& ResSize, unsigned int arg) { return true ; }
    virtual bool _postLoadResource(const std::ds_wstring& strResName   , IStreamIO* & pRes , int& ResSize, unsigned int arg){ return true ; }
    virtual bool _loadResource(const std::ds_wstring& strResName       , IStreamIO* & pRes , int& ResSize, unsigned int arg);
    virtual bool _unloadResource(const std::ds_wstring& strResName     , IStreamIO* & pRes , unsigned int& TotalResSize);
    virtual void _deleteResource(const std::ds_wstring& strResName     , IStreamIO* pRes);
};

typedef TResHandle <IStreamIO     , std::ds_wstring , xStreamIOLoader> HStreamIO;
typedef TLRUPolicyResMgr<IStreamIO , std::ds_wstring , xStreamIOLoader> xStreamIOMgr;

END_NAMESPACE_XEVOL3D

#endif
