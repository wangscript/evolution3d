#include "../xStdPch.h"
#include "xFileManager.h"
BEGIN_NAMESPACE_XEVOL3D

static FILE* OpenFile(const wchar_t* wcsFileName , int arg)
{
#ifdef _WIN32
    wchar_t _mode[4] = L"rt";
    if(arg & eOpen_Write                    )                       { _mode[0] = 'w';                   }
    if(arg & eOpen_Write && arg & eOpen_Read)                       { _mode[0] = 'w' ; _mode[2] = '+' ; }
    if(arg & eOpen_Write && arg & eOpen_Read && arg & eOpen_Exist ) { _mode[0] = 'r' ; _mode[2] = '+' ; }
    if(arg & eOpen_Binary                   )                       { _mode[1] = 'b' ;                  }

    FILE* fp = _wfopen(wcsFileName , _mode);
#else
    char _mode[4] = "rt";
    if(arg & eOpen_Write                    )                       { _mode[0] = 'w';                   }
    if(arg & eOpen_Write && arg & eOpen_Read)                       { _mode[0] = 'w' ; _mode[2] = '+' ; }
    if(arg & eOpen_Write && arg & eOpen_Read && arg & eOpen_Exist ) { _mode[0] = 'r' ; _mode[2] = '+' ; }
    if(arg & eOpen_Binary                   )                       { _mode[1] = 'b' ;                  }
    int8 mbsFileName[512]={0};
    XEvol_UnicodeToFsEnc(wcsFileName,mbsFileName,512);
    FILE* fp = fopen(mbsFileName , _mode);
#endif
    return fp;
}


bool xFILELoader::_isResLoaded(FILE* pRes)
{
    if( pRes == NULL )
        return false;
    return true;
}
unsigned int xFILELoader::_getResSize(FILE* pRes)
{
    return 1;
}

bool xFILELoader::_loadResource(const std::ds_wstring& strResName, FILE* & pRes , int& ResSize, unsigned int arg)
{
    if(pRes != NULL)
        return true;

    ResSize = 0;

    if(XEvol_IsAbsPath(strResName))
    {
        pRes = 	OpenFile(strResName.c_str() , arg);	
    }
    else
    {
        ds_wstring full_name;
        int nResItem = (int)this->m_Packages.size();
        for(int i = 0 ; i < nResItem ; i++)
        {
            xResPkgPathItem& item = this->m_Packages[i];
            if(item.m_Package.failed())
            {
                full_name = ds_wstring(item.m_ResDir) + strResName;
                pRes = 	OpenFile(full_name.c_str() , arg);	
                if(pRes) break;
            }
        }
    }

    return pRes != NULL;
}

bool xFILELoader::_unloadResource(const std::ds_wstring& strResName , FILE* & pRes , unsigned int& TotalResSize)
{
    fclose(pRes);
    pRes = NULL;
    return true;
}

void xFILELoader::_deleteResource(const std::ds_wstring& strResName , FILE* pRes)
{
    fclose(pRes);
    pRes = NULL;
} 


//==============
bool xStreamIOLoader::_isResLoaded(IStreamIO* pRes)
{
    if( pRes == NULL )
        return false;
    return true;
}
unsigned int xStreamIOLoader::_getResSize(IStreamIO* pRes)
{
    return 1;
}

bool xStreamIOLoader::_loadResource(const std::ds_wstring& strResName, IStreamIO* & pRes , int& ResSize, unsigned int arg)
{
    if(pRes != NULL)
        return true;
    ResSize = 0;
    if(XEvol_IsAbsPath(strResName))
    {
        pRes = 	xFileSystem::singleton()->loadFile(strResName.c_str() , (arg & eOpen_Write) != 0 );	
    }
    else
    {
        ds_wstring full_name;
        int nResItem = (int)this->m_Packages.size();
        for(int i = 0 ; i < nResItem ; i++)
        {
            xResPkgPathItem& item = this->m_Packages[i];
            if(item.m_Package.failed())
            {
                full_name = ds_wstring(item.m_ResDir) + strResName;
                pRes = 	xFileSystem::singleton()->loadFile(full_name.c_str() , (arg & eOpen_Write) != 0 );	
                if(pRes) break;
            }
        }
    }
    return pRes != NULL;
}

bool xStreamIOLoader::_unloadResource(const std::ds_wstring& strResName , IStreamIO* & pRes , unsigned int& TotalResSize)
{
    XSAFE_RELEASEOBJECT(pRes);
    return true;
}

void xStreamIOLoader::_deleteResource(const std::ds_wstring& strResName , IStreamIO* pRes)
{
    XSAFE_RELEASEOBJECT(pRes);
} 
END_NAMESPACE_XEVOL3D



