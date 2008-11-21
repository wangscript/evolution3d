#include "xBaseModel.h"
#include "../xcomdoc/xdocfstream.h"
#include "../fs/xFileSystem.h"
BEGIN_NAMESPACE_XEVOL3D

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
{
	m_ext = L".xrm";
	
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

bool xBaseModelLoader:: _loadFromFile(const std::ds_wstring& strResName , xBaseModel* pRes, unsigned int arg)
{
	//支持目录里散开的xrm
	//支持目录里打包的xrm

	if(pRes->load(strResName.c_str(),arg) )
		return true;

	//加一个扩展名
	std::ds_wstring _ext = xFileSystem::singleton()->getFileExtName(strResName.c_str() );
	if(_ext.length() == 0)
	{
        std::ds_wstring nameWithExt = strResName + m_ext;
		if(pRes->load(nameWithExt.c_str(),arg) )
			return true;
	}
	return false;
}

bool xBaseModelLoader::_loadFromPackage(xcomdoc& doc , const std::ds_wstring& _name , xBaseModel* pRes, unsigned int arg)
{
	 //在包里散落的模型文件，这个是打包器推荐的方式
     if(pRes->load(doc,_name.c_str() , arg) )
		 return true;

	 //从包里的xrm里加载
	 xcomdocstream* pstream = doc.create_stream(_name.c_str());
	 if(pstream == NULL)
	 {
		 //加一个扩展名
		 std::ds_wstring nameWithExt = _name + m_ext;
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
	 _docInPkg.close();
	 doc.close_stream(pstream);
	 return ret; 
}

bool xBaseModelLoader::_loadResource  (const std::ds_wstring& strResName , xBaseModel* & pRes , int& ResSize, unsigned int arg)
{
	ResSize = 0;
	bool needDeletePRes = false;
	if(pRes == NULL)
	{
		needDeletePRes = true;
		pRes = new xBaseModel(m_pRenderApi , m_pTexMgr);
	}

	//绝对路径
	if(XEvol_IsAbsPath(strResName))
	{
		if( _loadFromFile(strResName , pRes , arg) )
		{
			ResSize = pRes->memUsage();
			return true;
		}
		pRes->unload();
	}
	else
	{
		ds_wstring full_name;
		int nResItem = (int)this->m_Packages.size();
		for(int i = 0 ; i < nResItem ; i++)
		{
			xResPathManager::ResPackageItem& item = this->m_Packages[i];
			if(item.m_Package.failed())
			{
				full_name = ds_wstring(item.m_ResDir) + strResName;
				if( _loadFromFile(full_name , pRes , arg) )
				{
					ResSize = pRes->memUsage();
					return true;
				}
				pRes->unload();
			}
			else
			{
				full_name = item.m_ResDir + strResName;
				if(_loadFromPackage(item.m_Package , full_name , pRes , arg))
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


END_NAMESPACE_XEVOL3D

