#include "../xStdPch.h"
#include "xBaseModel.h"
#include "xCoreMesh.h"
#include "xBaseModelMgr.h"
#include "../xcomdoc/xdocfstream.h"
#include "../fs/xFileSystem.h"
BEGIN_NAMESPACE_XEVOL3D

void SetModelNameWithExt(xBaseModel* pModel , const wchar_t* _name)
{
	wchar_t name[256] = {0};
	wcsncpy(name , _name , 256);
	for(int i = (int)wcslen(name) - 1 ; i >= 0 ; i -- )
	{
		if(name[i] == '.')
		{
			name[i] = 0;
			pModel->setName(name);
		}
	}
	pModel->setName(name);
}

const wchar_t*   xBaseModelLoader::ext()
{
	return m_ext.c_str();
}



const wchar_t*     xBaseModelLoader::getModelExtName()
{
	return ext();
}

xBaseTextureMgr*   xBaseModelLoader::getTextureMgr()
{
	return m_pTexMgr;
}

void   xBaseModelLoader::KillObject()
{
	xBaseModelMgr* pMgr = dynamic_cast<xBaseModelMgr*>(this);
	delete pMgr;

}

xBaseModelMgr* xBaseModelLoader::createInstance(IRenderApi* pRenderApi , xBaseTextureMgr* pTexMgr , const wchar_t* name)
{
	xBaseModelMgr* pMgr = new xBaseModelMgr(name , 0 );
	pMgr->m_pRenderApi = pRenderApi;
	pMgr->m_pTexMgr    = pTexMgr;
	return pMgr;
}

xBaseModelLoader::xBaseModelLoader()
:m_MeshMgr(L"MeshManager" , 0)
{
	m_ext = L".xrm";
	m_pPathMgr = NULL;	
}

xBaseModelLoader::~xBaseModelLoader()
{

}
unsigned  int  xBaseModelLoader::_getResSize(xBaseModel* pRes)
{
	if(pRes == NULL) return 0;

    return pRes->memUsage();
}

bool xBaseModelLoader::_isResLoaded(xBaseModel* pRes)
{
	return pRes && pRes->isLoaded();
}

bool xBaseModelLoader:: _loadFromFile(const std::ds_wstring& strResDir , const std::ds_wstring& strResName , xBaseModel* pRes, unsigned int arg)
{
	//支持目录里散开的xrm
	//支持目录里打包的xrm
    //c:\strResName.xrm
	//C:\strResName\desc.xml
	ds_wstring fullName = strResDir + strResName;
	if(pRes->load(fullName.c_str(),arg) )
	{
		SetModelNameWithExt(pRes , strResName.c_str() );
		return true;
	}

	//加一个扩展名
	std::ds_wstring _ext = xFileSystem::singleton()->getFileExtName(strResName.c_str() );
	if(_ext.length() == 0)
	{
        std::ds_wstring nameWithExt = fullName + m_ext;
		if(pRes->load(nameWithExt.c_str(),arg) )
		{
			pRes->setName( strResName.c_str() );
			return true;
		}
	}
	return false;
}

bool xBaseModelLoader::_loadFromPackage(xResPkgPathItem& item , const std::ds_wstring& strResName , xBaseModel* pRes, unsigned int arg)
{
	 xcomdoc& doc = item.m_Package;
	 ds_wstring fullName = item.m_ResDir + strResName;
	 //在包里散落的模型文件，这个是打包器推荐的方式
	 //c:\MyPkg.xcd[_name/desc.xml]
     if(pRes->load(doc, fullName.c_str() , arg) )
	 {
		 SetModelNameWithExt(pRes , strResName.c_str() );
		 return true;
	 }

	 //从包里的xrm里加载
	 //c:\MyPkg.xcd[_name.xrm]
	 std::ds_wstring modelName = strResName;
	 xcomdocstream* pstream = doc.create_stream(fullName.c_str());
	 if(pstream == NULL)
	 {
		 //加一个扩展名
		 std::ds_wstring nameWithExt = fullName + m_ext;
		 modelName = strResName + m_ext;
		 pstream = doc.create_stream(nameWithExt.c_str());
	 }

	 //加了扩展名，依然找不到那个stream,表示失败了。
	 if(pstream == NULL)
	 {
		 return false;
	 }

	 xcomdoc _docInPkg ;
	 if(_docInPkg.open(pstream,xcdm_read) == false)
	 {
		 doc.close_stream(pstream);
		 return false;
	 }

	 bool ret = pRes->load(_docInPkg , L"" , arg);
	 SetModelNameWithExt(pRes , modelName.c_str() );
	 _docInPkg.close();
	 doc.close_stream(pstream);
	 return ret; 
}

bool xBaseModelLoader::_loadResource  (const std::ds_wstring& strResName , xBaseModel* & pRes , int& ResSize, unsigned int arg)
{
	std::ds_wstring _ext = xFileSystem::singleton()->getFileExtName(strResName.c_str() );
	if(_ext.length() == 0)
	{
		_ext = m_ext;
	}

	ResSize = 0;
	bool needDeletePRes = false;
	if(pRes == NULL)
	{
		needDeletePRes = true;
		pRes = xBaseModelCreatorMgr::singleton()->createInstance( _ext.c_str() , m_pTexMgr , false);
		if(pRes == NULL)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"File  "); XEVOL_LOG(eXL_DEBUG_HIGH , strResName.c_str() );	 XEVOL_LOG(eXL_DEBUG_HIGH , L" not registe a loader for it\n");
			return false;
		}
	}

	//绝对路径
	if(XEvol_IsAbsPath(strResName))
	{
		std::ds_wstring pathName = xFileSystem::singleton()->getPathName( strResName.c_str() );
		std::ds_wstring fileName = xFileSystem::singleton()->getFileName( strResName.c_str() );
		if( _loadFromFile(pathName.c_str() , fileName.c_str() , pRes , arg) )
		{
			ResSize = pRes->memUsage();
			return true;
		}
		pRes->unload();
	}
	else
	{
		int nResItem = (int)nPackageItem();
		for(int i = 0 ; i < nResItem ; i++)
		{
			xResPkgPathItem& item = this->packageItem(i);
			//路径是个目录
			if(item.m_Package.failed())
			{
				if( _loadFromFile(item.m_ResDir , strResName , pRes , arg) )
				{
					ResSize = pRes->memUsage();
					return true;
				}
				pRes->unload();
			}
			//路径是个包文件
			else
			{
				if(_loadFromPackage(item , strResName , pRes , arg))
				{
					ResSize = pRes->memUsage();
					return true;
				}
				pRes->unload();
			}

		}
	}

	//如果是创建出来的就删除掉
	if(needDeletePRes)
	{
		delete pRes;
		pRes = NULL;
	}
	return false;

}

bool xBaseModelLoader::_unloadResource(const std::ds_wstring& strResName , xBaseModel* & pRes , unsigned int& TotalResSize)
{
	TotalResSize -= pRes->memUsage();
	pRes->unload();
	return true;
}

void xBaseModelLoader::_deleteResource(const std::ds_wstring& strResName , xBaseModel* pRes)
{
    delete pRes;
	pRes = NULL;
	return ;
}

HCoreMesh  xBaseModelLoader::loadMesh(const wchar_t*  _meshName  , const wchar_t* _ownModelName , bool bLoadImm)
{
     xCoreMeshName _name(_meshName , _ownModelName);
	 return m_MeshMgr.add(_name , 0 , bLoadImm);
}

END_NAMESPACE_XEVOL3D

