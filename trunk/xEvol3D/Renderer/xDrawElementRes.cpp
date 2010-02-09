#include "../xStdPch.h"
#include "xDrawElementRes.h"
#include "../fs/xFileSystem.h"
#include "xMaterial.h"
BEGIN_NAMESPACE_XEVOL3D
IMPL_BASE_OBJECT_DLLSAFE(IDrawElementResource ,  IDrawElement);
IDrawElementResource::IDrawElementResource(IBaseRenderer* pRenderer) 
:IDrawElement(pRenderer)
{
}

IDrawElementResource::~IDrawElementResource()
{

}
size_t xDrawElementResName::hash_value() const
{
	return m_hashValue;
}
 xDrawElementResName::xDrawElementResName(const wchar_t* typeName , const wchar_t* _resName)
 {
	 _Type = typeName;
	 m_hashValue = xStringHash(_Type.c_str()) + xStringHash(_ResName.c_str());
 }


const wchar_t*   xDrawElementResLoader::ext()
{
	return L"";
}

xBaseTextureMgr*   xDrawElementResLoader::getTextureMgr()
{
	return m_pTexMgr;
}


xDrawElementResLoader::xDrawElementResLoader()
{
	m_ext = L"";
	m_pPathMgr = NULL;	
	m_pTexMgr = NULL;	
	m_pRenderer = NULL;	

}

xDrawElementResLoader::~xDrawElementResLoader()
{

}

unsigned  int  xDrawElementResLoader::_getResSize(IDrawElementResource* pRes)
{
	if(pRes == NULL) return 0;

	return pRes->memUsage();
}

bool xDrawElementResLoader::_isResLoaded(IDrawElementResource* pRes)
{
	return pRes && pRes->isLoaded();
}

bool xDrawElementResLoader:: _loadFromFile(const std::ds_wstring& strResName , IDrawElementResource* pRes, unsigned int arg)
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

bool xDrawElementResLoader::_loadFromPackage(xcomdoc& doc , const std::ds_wstring& _name , IDrawElementResource* pRes, unsigned int arg)
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

bool xDrawElementResLoader::_loadResource  (const xDrawElementResName& resName , IDrawElementResource* & pRes , int& ResSize, unsigned int arg)
{
	ResSize = 0;
	bool needDeletePRes = false;
	if(pRes == NULL)
	{
		needDeletePRes = true;
		pRes = (IDrawElementResource*)xDrawElementFactoryMgr::singleton()->createInstance(resName.typeName().c_str() , m_pRenderer , arg);
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

bool xDrawElementResLoader::_unloadResource(const xDrawElementResName& resName , IDrawElementResource* & pRes , unsigned int& TotalResSize)
{
	TotalResSize -= pRes->memUsage();
	pRes->unload();
	return true;
}

void xDrawElementResLoader::_deleteResource(const xDrawElementResName& resName , IDrawElementResource* pRes)
{
	pRes->ReleaseObject();
	pRes = NULL;
	return ;
}
void xDrawElementResLoader::setRenderer(IBaseRenderer* pRenderer)
{
	m_pRenderer = pRenderer;
}
IBaseRenderer* xDrawElementResLoader::getRenderer()
{
	return m_pRenderer;
}
END_NAMESPACE_XEVOL3D
