#include "../xStdPch.h"
#include "xSceneObjectRes.h"
#include "../fs/xFileSystem.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(ISceneObjectRes ,  ISceneObject);
ISceneObjectRes::ISceneObjectRes(ISceneGraph*  pScene , int arg) 
:ISceneObject(pScene , arg)
{
}

ISceneObjectRes::~ISceneObjectRes()
{

}
size_t xSceneObjectResName::hash_value() const
{
	return m_hashValue;
}
xSceneObjectResName::xSceneObjectResName(const wchar_t* typeName , const wchar_t* _resName)
{
	_Type = typeName;
	m_hashValue = xStringHash(_Type.c_str()) + xStringHash(_ResName.c_str());
}


const wchar_t*   xSceneObjectResLoader::ext()
{
	return L"";
}



xSceneObjectResLoader::xSceneObjectResLoader()
{
	m_ext = L"";
	m_pSceneGraph = NULL;
}

xSceneObjectResLoader::~xSceneObjectResLoader()
{

}

unsigned  int  xSceneObjectResLoader::_getResSize(ISceneObjectRes* pRes)
{
	if(pRes == NULL) return 0;

	return pRes->memUsage();
}
ISceneGraph* xSceneObjectResLoader::sceneGraph()
{
	return m_pSceneGraph ; 
}

void xSceneObjectResLoader::sceneGraph(ISceneGraph* pSceneGraph)
{ 
	m_pSceneGraph = pSceneGraph ; 
}

bool xSceneObjectResLoader::_isResLoaded(ISceneObjectRes* pRes)
{
	return pRes && pRes->isLoaded();
}

bool xSceneObjectResLoader:: _loadFromFile(const std::ds_wstring& strResName , ISceneObjectRes* pRes, unsigned int arg)
{
	//支持目录里散开的xrm
	//支持目录里打包的xrm

	if(pRes->load(strResName.c_str(), NULL , 0 , arg) )
		return true;

	//加一个扩展名
	std::ds_wstring _ext = xFileSystem::singleton()->getFileExtName(strResName.c_str() );
	if(_ext.length() == 0)
	{
		std::ds_wstring nameWithExt = strResName + m_ext;
		if(pRes->load(nameWithExt.c_str(),NULL , 0 , arg) )
			return true;
	}
	return false;
}

bool xSceneObjectResLoader::_loadFromPackage(xcomdoc& doc , const std::ds_wstring& _name , ISceneObjectRes* pRes, unsigned int arg)
{
	//在包里散落的模型文件，这个是打包器推荐的方式
	if(pRes->load(doc,_name.c_str(), arg) )
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

bool xSceneObjectResLoader::_loadResource  (const xSceneObjectResName& resName , ISceneObjectRes* & pRes , int& ResSize, unsigned int arg)
{
	ResSize = 0;
	bool needDeletePRes = false;
	if(pRes == NULL)
	{
		needDeletePRes = true;
		pRes = (ISceneObjectRes*)xSceneObjectFactoryMgr::singleton()->createInstance(resName.typeName().c_str() , m_pSceneGraph , arg);
		if(pRes == NULL)
			return false;
	}

	const wchar_t* strResName = resName.resName().c_str() ;
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
		int nResItem = (int)nPackageItem();
		for(int i = 0 ; i < nResItem ; i++)
		{
			xResPkgPathItem& item = this->packageItem(i);
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
		pRes->ReleaseObject();
		pRes = NULL;
	}
	return false;

}

bool xSceneObjectResLoader::_unloadResource(const xSceneObjectResName& resName , ISceneObjectRes* & pRes , unsigned int& TotalResSize)
{
	TotalResSize -= pRes->memUsage();
	pRes->unload();
	return true;
}

void xSceneObjectResLoader::_deleteResource(const xSceneObjectResName& resName , ISceneObjectRes* pRes)
{
	pRes->ReleaseObject();
	pRes = NULL;
	return ;
}


END_NAMESPACE_XEVOL3D
