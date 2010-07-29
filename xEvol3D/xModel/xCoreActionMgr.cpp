#include "../xStdPch.h"
#include "xCoreAction.h"
#include "xBaseAction.h"
#include "xTransitionAction.h"
#include "xMixAction.h"
#include "xKeyframeAction.h"

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

xCoreActionName::xCoreActionName(eActionType _Type)
{
	m_hModel.setNULL();
	m_ActionName = L"";
	m_ResFile    = L"";
	m_strHash = 0;
	m_Ext = L"xra";
    m_ActionType = _Type;
}

HCoreAction xCoreActionLoader::loadAction(const xCoreActionName& name, int arg , bool bLoadedImm )
{
    xBaseModel* pModel = name.m_hModel.getResource();
    if(pModel == NULL)
        return HCoreAction();

    std::ds_wstring _name1;
    std::ds_wstring _name2;
    xCoreActionMgr* pMgr = m_pThis;
    if( xTransitionAction::IsTransitionName(name.m_ActionName , _name1 , _name2) )
    {
        const wchar_t* strSkelName = pModel->name();
        xCoreActionName _ActName1(eActType_None , name.m_hModel , _name1.c_str() , strSkelName );
        xCoreActionName _ActName2(eActType_None , name.m_hModel , _name2.c_str() , strSkelName );
        HCoreAction hAction1 = pMgr->add(_ActName1 , 0 , true);
        HCoreAction hAction2 = pMgr->add(_ActName2 , 0 , true);
    }
    return pMgr->add(name , arg , bLoadedImm);
}

xCoreActionName::xCoreActionName(eActionType _Type , HBaseModel hBaseModel , const wchar_t* actionName , const wchar_t* skelName , const wchar_t* resFile )
{
	m_Ext = L"xra";
    m_ActionType = _Type;
	m_hModel = hBaseModel;
	if(actionName == NULL ) actionName = L"";
	m_ActionName = actionName;
	m_strHash = xStringHash( m_ActionName.c_str() );


    if(resFile == NULL) resFile = m_ActionName.c_str();	
	m_ResFile = resFile;
	m_strHash += xStringHash(m_ResFile.c_str() );

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

bool xCoreActionLoader::newInstance(const xCoreActionName& name , xBaseAction* & pCoreAction)
{

	xCoreActionMgr* pMgr = m_pThis;
    std::ds_wstring _name1;
    std::ds_wstring _name2;
    bool bTransition = xTransitionAction::IsTransitionName(name.m_ActionName , _name1 , _name2) ;

	if(name.m_ActionType == eActType_Transition ||  name.m_ActionType == eActType_None && bTransition )
	{
    
		xBaseModel* pModel = name.m_hModel.getResource();
		if(pModel == NULL)
			return false;

		const wchar_t* strSkelName = pModel->name();

		xTransitionAction* _action = type_cast<xTransitionAction*>(pCoreAction);
		if(_action == NULL)
		{
			_action = new xTransitionAction(pModel->skeleton() , 0 );
		}
	    
		xCoreActionName _ActName1(eActType_None , name.m_hModel , _name1.c_str() , strSkelName );
		xCoreActionName _ActName2(eActType_None , name.m_hModel , _name2.c_str() , strSkelName );
		HCoreAction hAction1 = pMgr->add(_ActName1 , 0 , true);
		HCoreAction hAction2 = pMgr->add(_ActName2 , 0 , true);
		if(hAction1.getResource() == NULL || hAction2.getResource() == NULL)
		{
            if( hAction1.getResource() == NULL) 
			    XEVOL_LOG(eXL_DEBUG_HIGH , L"Create transition action failed : %s action not exist\n" , _name1.c_str() );

            if( hAction2.getResource() == NULL) 
                XEVOL_LOG(eXL_DEBUG_HIGH , L"Create transition action failed : %s action not exist\n" , _name2.c_str() );
            
            _action->ReleaseObject();
			return false;
		}

        hAction1.lock();
        hAction2.lock();
        xCoreAction* pCoreAction1 = type_cast<xCoreAction*>( hAction1.getResource() );
        xCoreAction* pCoreAction2 = type_cast<xCoreAction*>( hAction2.getResource() );
		_action->setAction( pCoreAction1  , pCoreAction2 , 1.0f );
		pCoreAction = _action;
        pCoreAction->setLoop(m_bActionLoop);
        hAction1.unlock();
        hAction2.unlock();
		return true;
	}


	if(name.m_ActionType == eActType_Mix)
	{
        xBaseModel* pModel = name.m_hModel.getResource();
        pCoreAction = new xMixedAction( pModel? pModel->skeleton() : NULL , 0 );
        pCoreAction->setLoop(m_bActionLoop);
        return false;
    }

    if(name.m_ActionType == eActType_None )
    {
        pCoreAction = NULL;
        return false;
    }

    if(name.m_ActionType == eActType_Freeze )
    {
        xBaseModel* pModel = name.m_hModel.getResource();
        if(pModel == NULL)
            return false;

        const wchar_t* strSkelName = pModel->name();

        xPauseAction* _action = type_cast<xPauseAction*>(pCoreAction);
        if(_action == NULL)
        {
            _action = new xPauseAction(pModel? pModel->skeleton() : NULL , 0);
        }

        float _actionTime = 0.4f;
        std::ds_wstring _actionName = xPauseAction::GetActionName(name.m_ActionName.c_str() , _actionTime );
       xCoreActionName _ActName1(eActType_None , name.m_hModel , _actionName.c_str() , strSkelName );

        HCoreAction hAction1 = pMgr->add(_ActName1 , 0 , true);
        if(hAction1.getResource() == NULL)
        {
            XEVOL_LOG(eXL_DEBUG_HIGH , L"Create Pause action failed : %s action not exist\n" , _actionName.c_str() );
            return false;
        }

        xCoreAction* pCoreAction1 = type_cast<xCoreAction*>( hAction1.getResource() );
        _action->setAction( pCoreAction1 , _actionTime);
        pCoreAction = _action;
        pCoreAction->setLoop(false);
        return true;
    }

    return false;
}


bool xCoreActionLoader::_isResLoaded(xBaseAction* pCoreAction)
{
	return pCoreAction != NULL;
}

unsigned int   xCoreActionLoader::_getResSize(xBaseAction* pRes)
{
	return pRes->memUsage();
}

xBaseAction*   xCoreActionLoader::createAction(const xCoreActionName& strResName)
{
	xBaseAction* pAction = NULL;
	int resSize = 0;
	_loadResource(strResName , pAction , resSize , 0);
	return pAction;
}

xBaseAction* CreateActionByStream(xcomdocstream* pActionStream, bool bLoop)
{
    int32       nBone = 0;
    xSkeletonID SkeID;
    xActionInfo Info;

    pActionStream->read(SkeID );
    pActionStream->read(nBone );
    pActionStream->read(Info  );
    pActionStream->stream_seekr(xcdsd_beg , 0 );
    xBaseAction* pCoreAction =  xBaseAction::createInstance( Info.m_eActType , NULL , 0);   

    if(pCoreAction) pCoreAction->setLoop(bLoop);
    return pCoreAction;
}

xBaseAction* CreateActionByStream(std::ifstream& stream , bool bLoop)
{
    int32       nBone = 0;
    xSkeletonID SkeID;
    xActionInfo Info;

    stream.read((char*)&SkeID , sizeof(SkeID) );
    stream.read((char*)&nBone , sizeof(nBone) );
    stream.read((char*)&Info  , sizeof(Info) );
    stream.seekg(0 , ios::beg);
    xBaseAction* pCoreAction =  xBaseAction::createInstance( Info.m_eActType , NULL , 0);   
    if(pCoreAction) pCoreAction->setLoop(bLoop);
    return pCoreAction;
}

#define STR_SKELETON   PATH_SPLITTER_STRING + L"skeleton" + PATH_SPLITTER_STRING
bool xCoreActionLoader::__loadFromPackage(const xCoreActionName& strResName,xBaseAction* & pCoreAction , xResPkgPathItem& item , int i )
{
	xBaseModel* pModel = strResName.m_hModel.getResource();
	if(pModel == NULL)
		return false;
	const wchar_t* strSkelName = pModel->name();
    std::ds_wstring _FileExt = strResName.m_Ext;

	//++++++++++++++++++++Load function 01 ++++++++++++++++++++++++++++++++++++++++++++++++++
	//【PAKAGE.xrm】【PATH】/SkeletonName/skeleton/[ACTIONNAME]
	{
		ds_wstring full_action_name = item.m_ResDir + strSkelName + STR_SKELETON + strResName.m_ResFile.c_str() + L"."  + _FileExt;
		xcomdoc model_doc ;
		xcomdocstream* pActionStream = item.m_Package.open_stream(full_action_name.c_str());

        //加载没有扩展名的        
		if(pActionStream == NULL)
		{
			full_action_name = item.m_ResDir + strSkelName + STR_SKELETON + strResName.m_ResFile.c_str();
            pActionStream = item.m_Package.open_stream(full_action_name.c_str());
		} 

        if(pActionStream)
        {
            //创建Action
            if(pCoreAction == NULL)
            {
                pCoreAction = CreateActionByStream(pActionStream , m_bActionLoop);
            }

            if(pCoreAction->load(strResName.m_ActionName.c_str() , pActionStream) == true)
            {
                item.m_Package.close_stream(pActionStream);
                return true;
            }
            item.m_Package.close_stream(pActionStream);
        }
	}

	//【PAKAGE.xrm】【PATH】/SkeletonName/[ACTIONNAME]
	{
		ds_wstring full_action_name = item.m_ResDir + strSkelName + PATH_SPLITTER_STRING + strResName.m_ResFile.c_str() + L"."  + _FileExt;
		xcomdoc model_doc ;
		xcomdocstream* pActionStream = item.m_Package.open_stream(full_action_name.c_str());
		
		//加载没有扩展名的
		if(pActionStream == NULL)
		{
			full_action_name = item.m_ResDir + strSkelName + PATH_SPLITTER_STRING + strResName.m_ResFile.c_str();
            pActionStream = item.m_Package.open_stream(full_action_name.c_str());
		}

        if(pActionStream)
        {
            //创建Action
            if(pCoreAction == NULL)
            {
                pCoreAction = CreateActionByStream(pActionStream , m_bActionLoop);
            }

            if(pCoreAction->load(strResName.m_ActionName.c_str() ,pActionStream) == true)
            {
                item.m_Package.close_stream(pActionStream);
                return true;
            }
            item.m_Package.close_stream(pActionStream);
        }

	}
	return false;
}

bool xCoreActionLoader::__loadFromDirectory(const xCoreActionName& strResName,xBaseAction* & pCoreAction , xResPkgPathItem& item, int i  )
{
	xBaseModel* pModel = strResName.m_hModel.getResource();
	if(pModel == NULL)
		return false;
	const wchar_t* strSkelName = pModel->name();
    std::ds_wstring _FileExt = strResName.m_Ext;

	//【PATH】/SkeletonName/skeleton/[ACTIONNAME]
	{
		ds_wstring full_action_name = item.m_ResDir + strSkelName + PATH_SPLITTER_STRING + STR_SKELETON + PATH_SPLITTER_STRING + strResName.m_ResFile.c_str() + L"." + _FileExt;
		xcomdoc model_doc ;
		std::ifstream _stream;
		bool bRet = xFileSystem::singleton()->loadFile(full_action_name.c_str() , _stream );
		//加载没有扩展名的
		if(bRet == false )
		{
			full_action_name = item.m_ResDir + strSkelName + STR_SKELETON + strResName.m_ResFile.c_str();
			bRet = xFileSystem::singleton()->loadFile(full_action_name.c_str() , _stream );
		}

        if(bRet)
        {
            //创建Action
            if(pCoreAction == NULL)
            {
                pCoreAction = CreateActionByStream(_stream , m_bActionLoop);
            }
            if(pCoreAction->load(strResName.m_ActionName.c_str() , _stream))
            {
                _stream.close();
                return true;
            }
            _stream.close();
        }
		
	}

	//【PATH】/SkeletonName/[ACTIONNAME]
	{
		ds_wstring full_action_name = item.m_ResDir + strSkelName + PATH_SPLITTER_STRING + strResName.m_ResFile.c_str() + L"."  + _FileExt;
		std::ifstream _stream;
		bool bRet = xFileSystem::singleton()->loadFile(full_action_name.c_str() , _stream );
		//加载没有扩展名的
		if(bRet == false )
		{
			full_action_name = item.m_ResDir + strSkelName + PATH_SPLITTER_STRING + strResName.m_ResFile.c_str();
			bRet = xFileSystem::singleton()->loadFile(full_action_name.c_str() , _stream );
		}

        if(bRet)
        {
            //创建Action
            if(pCoreAction == NULL)
            {
                pCoreAction = CreateActionByStream(_stream , m_bActionLoop);
            }
            if(pCoreAction->load(strResName.m_ActionName.c_str() , _stream))
            {
                _stream.close();
                return true;
            }
            _stream.close();
        }
	}
	return false;
}

bool xCoreActionLoader::_loadResource(const xCoreActionName& strResName , xBaseAction* & pCoreAction , int& ResSize, unsigned int arg)
{
 	ds_wstring full_name;
	ResSize = 0;
	bool needDeletepCoreMesh = false;
	if(pCoreAction == NULL)
	{
		needDeletepCoreMesh = true;
		if(true == newInstance(strResName , pCoreAction) )
			return true;
	}

	int nResItem = (int)nPackageItem();
	for(int i = 0 ; i < nResItem ; i++)
	{
		xResPkgPathItem& item = packageItem(i);
		bool _ret = false;
		if(item.m_Package.failed())
		{
			//We are load a File . add ext to the end;
			_ret = __loadFromDirectory(strResName , pCoreAction,item , i );
		}
		else
		{   
			_ret = __loadFromPackage(strResName , pCoreAction,item , i );
		}
        if(pCoreAction && _ret )
        {
            ResSize = pCoreAction->memUsage();
            pCoreAction->setLoop(m_bActionLoop);
            return true;
        }
        else
        {
            delete pCoreAction;
        }
	}

	//如果是创建出来的就删除掉
	if(needDeletepCoreMesh)
	{
		delete pCoreAction;
		pCoreAction = NULL;
	}

	return false;
}

bool xCoreActionLoader::_unloadResource(const xCoreActionName& strResName,xBaseAction* & pCoreAction , unsigned int& TotalResSize)
{
	TotalResSize -= pCoreAction->memUsage();
    pCoreAction->unload();
    pCoreAction->KillObject();
    pCoreAction = NULL;
	return true;
}

void xCoreActionLoader::_deleteResource(const xCoreActionName& strResName,xBaseAction* pCoreAction)
{
	if(pCoreAction == NULL)
		return ;
	pCoreAction->unload();
	pCoreAction->KillObject();
	pCoreAction = NULL;
}


END_NAMESPACE_XEVOL3D
