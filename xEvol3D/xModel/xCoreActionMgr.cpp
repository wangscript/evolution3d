#include "../xStdPch.h"
#include "xCoreAction.h"
#include "xCoreSkeleton.h"
#include "xCoreActionMgr.h"
#include "../BaseLib/xStringHash.h"
#include "../fs/xFileSystem.h"
#include "xBaseModel.h"
#include "xBaseModelMgr.h"
BEGIN_NAMESPACE_XEVOL3D
bool xCoreActionName::operator <  (const xCoreActionName& rhv) const
{
	return (m_strHash < rhv.m_strHash);
}

#ifdef _MSC_VER
size_t  xCoreActionName::hash_value() const
{
	return m_strHash;
}
#endif

xCoreActionName::~xCoreActionName()
{

}

bool xCoreActionName::operator == (const xCoreActionName& rhv) const
{
	return (m_strHash == rhv.m_strHash);
       
}

xCoreActionName::xCoreActionName()
{
	m_hModel.setNULL();
	m_Name = L"";
	m_strHash = 0;
}

xCoreActionName::xCoreActionName(HBaseModel hBaseModel , const wchar_t* actionName , const wchar_t* skelName)
{
	m_hModel = hBaseModel;
	if(actionName)
	{
		m_Name = actionName;
		m_strHash = xStringHash(actionName );
	}
	else
	{
		m_Name = L"";
		m_strHash  = 0; 
	}

	if(skelName)
	{
		m_strHash += xStringHash(skelName  );
	}

}

xCoreActionLoader::xCoreActionLoader()
{
    m_bActionLoop = true;
}


xCoreActionLoader::~xCoreActionLoader()
{

}

bool xCoreActionLoader::newInstance(const xCoreActionName& name , xCoreAction* & pCoreAction)
{
	std::ds_wstring _name1;
	std::ds_wstring _name2;
	xCoreActionMgr* pMgr = dynamic_cast<xCoreActionMgr*>(this);
	if( xTransitionAction::IsTransitionName(name.m_Name , _name1 , _name2) )
	{
		xBaseModel* pModel = name.m_hModel.getResource();
		if(pModel == NULL)
			return false;

		const wchar_t* strSkelName = pModel->name();

		xTransitionAction* _action = dynamic_cast<xTransitionAction*>(pCoreAction);
		if(_action == NULL)
		{
			_action = new xTransitionAction();
		}
	    
		xCoreActionName _ActName1(name.m_hModel , _name1.c_str() , strSkelName );
		xCoreActionName _ActName2(name.m_hModel , _name2.c_str() , strSkelName );
		HCoreAction hAction1 = pMgr->add(_ActName1 , 0 , true);
		HCoreAction hAction2 = pMgr->add(_ActName2 , 0 , true);
		if(hAction1.getResource() == NULL || hAction2.getResource() == NULL)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"Create transition action failed : %S action not exist\n" , _name1.c_str() );
			_action->ReleaseObject();
			return false;
		}

		_action->setAction(hAction1.getResource() , hAction2.getResource() , 1.0f );
		pCoreAction = _action;
        pCoreAction->setLoop(m_bActionLoop);
		return true;
	}

	
	pCoreAction = new xCoreAction();
    pCoreAction->setLoop(m_bActionLoop);
	return false;
}


bool xCoreActionLoader::_isResLoaded(xCoreAction* pCoreAction)
{
	return pCoreAction != NULL;
}

unsigned int   xCoreActionLoader::_getResSize(xCoreAction* pRes)
{
	return pRes->memUsage();
}

bool xCoreActionLoader::__loadFromPackage(const xCoreActionName& strResName,xCoreAction* & pCoreAction , xResPkgPathItem& item , int i)
{
	xBaseModel* pModel = strResName.m_hModel.getResource();
	if(pModel == NULL)
		return false;
	const wchar_t* strSkelName = pModel->name();

	//++++++++++++++++++++Load function 01 ++++++++++++++++++++++++++++++++++++++++++++++++++
	//【PAKAGE.xrm】【PATH】/SkeletonName/skeleton/[ACTIONNAME]
	{
		ds_wstring full_action_name = item.m_ResDir + strSkelName + L"/skeleton/" + strResName.m_Name.c_str() + L".xra";
		xcomdoc model_doc ;
		xcomdocstream* pActionStream = item.m_Package.open_stream(full_action_name.c_str());

        //加载没有扩展名的        
		if(pActionStream == NULL)
		{
			full_action_name = item.m_ResDir + strSkelName + L"/skeleton/" + strResName.m_Name.c_str();
            pActionStream = item.m_Package.open_stream(full_action_name.c_str());
		} 

		if(pCoreAction->load(strResName.m_Name.c_str() , pActionStream) == true)
		{
			item.m_Package.close_stream(pActionStream);
			return true;
		}
		item.m_Package.close_stream(pActionStream);
	}

	//【PAKAGE.xrm】【PATH】/SkeletonName/[ACTIONNAME]
	{
		ds_wstring full_action_name = item.m_ResDir + strSkelName + L"/" + strResName.m_Name.c_str() + L".xra";;
		xcomdoc model_doc ;
		xcomdocstream* pActionStream = item.m_Package.open_stream(full_action_name.c_str());
		
		//加载没有扩展名的
		if(pActionStream == NULL)
		{
			full_action_name = item.m_ResDir + strSkelName + L"/" + strResName.m_Name.c_str();
            pActionStream = item.m_Package.open_stream(full_action_name.c_str());
		}
		if(pCoreAction->load(strResName.m_Name.c_str() ,pActionStream) == true)
		{
			item.m_Package.close_stream(pActionStream);
			return true;
		}
		item.m_Package.close_stream(pActionStream);
	}
	return false;
}

bool xCoreActionLoader::__loadFromDirectory(const xCoreActionName& strResName,xCoreAction* & pCoreAction , xResPkgPathItem& item, int i)
{
	xBaseModel* pModel = strResName.m_hModel.getResource();
	if(pModel == NULL)
		return false;
	const wchar_t* strSkelName = pModel->name();

	//【PATH】/SkeletonName/skeleton/[ACTIONNAME]
	{
		ds_wstring full_action_name = item.m_ResDir + strSkelName + L"/skeleton/" + strResName.m_Name.c_str() + L".xra";
		xcomdoc model_doc ;
		std::ifstream _stream;
		bool bRet = xFileSystem::singleton()->loadFile(full_action_name.c_str() , _stream );
		//加载没有扩展名的
		if(bRet == false )
		{
			full_action_name = item.m_ResDir + strSkelName + L"/skeleton/" + strResName.m_Name.c_str();
			bRet = xFileSystem::singleton()->loadFile(full_action_name.c_str() , _stream );
		}
		if(bRet && pCoreAction->load(strResName.m_Name.c_str() , _stream))
		{
			_stream.close();
			return true;
		}
		_stream.close();
	}

	//【PATH】/SkeletonName/[ACTIONNAME]
	{
		ds_wstring full_action_name = item.m_ResDir + strSkelName + L"/" + strResName.m_Name.c_str() + L".xra";
		std::ifstream _stream;
		bool bRet = xFileSystem::singleton()->loadFile(full_action_name.c_str() , _stream );
		//加载没有扩展名的
		if(bRet == false )
		{
			full_action_name = item.m_ResDir + strSkelName + L"/" + strResName.m_Name.c_str();
			bRet = xFileSystem::singleton()->loadFile(full_action_name.c_str() , _stream );
		}

		if(bRet && pCoreAction->load(strResName.m_Name.c_str() , _stream))
		{
			_stream.close();
			return true;
		}
		_stream.close();
	}
	return false;
}

bool xCoreActionLoader::_loadResource(const xCoreActionName& strResName , xCoreAction* & pCoreAction , int& ResSize, unsigned int arg)
{
	ds_wstring full_name;
	ResSize = 0;
	bool needDeletepCoreMesh = false;
	if(pCoreAction == NULL)
	{
		needDeletepCoreMesh = true;
		if(true == newInstance(strResName , pCoreAction) )
			return true;

		if(pCoreAction == NULL)
			return false;
	}

	int nResItem = (int)nPackageItem();
	for(int i = 0 ; i < nResItem ; i++)
	{
		xResPkgPathItem& item = packageItem(i);
		bool _ret = false;
		if(item.m_Package.failed())
		{
			//We are load a File . add ext to the end;
			_ret = __loadFromDirectory(strResName , pCoreAction,item , i);
		}
		else
		{   
			_ret = __loadFromPackage(strResName , pCoreAction,item , i);
		}
		ResSize = pCoreAction->memUsage();
        pCoreAction->setLoop(m_bActionLoop);
		if(_ret) return true;

	}

	//如果是创建出来的就删除掉
	if(needDeletepCoreMesh)
	{
		delete pCoreAction;
		pCoreAction = NULL;
	}

	return false;
}

bool xCoreActionLoader::_unloadResource(const xCoreActionName& strResName,xCoreAction* & pCoreAction , unsigned int& TotalResSize)
{
	TotalResSize -= pCoreAction->memUsage();
	pCoreAction->unload();
	return true;
}

void xCoreActionLoader::_deleteResource(const xCoreActionName& strResName,xCoreAction* pCoreAction)
{
	if(pCoreAction == NULL)
		return ;
	pCoreAction->unload();
	pCoreAction->KillObject();
	pCoreAction = NULL;
}


END_NAMESPACE_XEVOL3D
