#include "../xStdPch.h"
#include "xCoreMesh.h"
#include "xCoreSkeleton.h"
#include "xCoreMeshMgr.h"
#include "../BaseLib/xStringHash.h"
#include "xBaseModelMgr.h"
BEGIN_NAMESPACE_XEVOL3D
bool xCoreMeshName::operator <  (const xCoreMeshName& rhv) const
{
	return (m_strHash < rhv.m_strHash);
}

#ifdef _MSC_VER
size_t  xCoreMeshName::hash_value() const
{
	return m_strHash;
}
#endif

xCoreMeshName::~xCoreMeshName()
{

}

bool xCoreMeshName::operator == (const xCoreMeshName& rhv) const
{
	return (m_MeshName == rhv.m_MeshName && m_ModelName == rhv.m_ModelName);
	//不是骨骼动画.
	//只要模型名字相同，Mesh名字相同。他们就应该去加载相同的Mesh
	//if(m_pModel->getSkeleton() == NULL || 
	//    rhv.m_pModel->getSkeleton() == NULL)
	//    return (lhv.m_pModel->name() == rhv.m_pModel->name() ) ;

	//是骨骼动画。只要骨架名字相同，Mesh名字相同就可以了。
	//return (m_pModel->getSkeleton()->getID() == rhv.m_pModel->getSkeleton()->getID() );          
}

xCoreMeshName::xCoreMeshName(const wchar_t* meshName , const wchar_t* modelName , xCoreSkeleton* pSkeleton)
{
    m_pSkeleton = pSkeleton;
	if(meshName)
	{
		m_MeshName = meshName;
		m_strHash = xStringHash(meshName );
	}
	else
	{
		m_MeshName = L"";
		m_strHash  = 0; 
	}

	if(modelName)
	{
		m_ModelName = modelName;
		m_strHash += xStringHash(modelName  );
	}
	else
	{
		m_ModelName = L"";
	}
}

xCoreMeshLoader::xCoreMeshLoader()
{
	m_MeshExtName      = L".mesh";
}


xCoreMeshLoader::~xCoreMeshLoader()
{

}

xCoreMesh* xCoreMeshLoader::newInstance(const xCoreMeshName& name)
{
	return new xCoreMesh(m_pOwner->getTextureMgr() , name.m_pSkeleton);
}


bool xCoreMeshLoader::_isResLoaded(xCoreMesh* pCoreMesh)
{
	return pCoreMesh != NULL;
}

unsigned int   xCoreMeshLoader::_getResSize(xCoreMesh* pRes)
{
	return pRes->memUsage();
}

bool xCoreMeshLoader::__loadFromPackage(const xCoreMeshName& strResName,xCoreMesh* & pCoreMesh , xResPkgPathItem& item , int i)
{

	//++++++++++++++++++++Load function 01 ++++++++++++++++++++++++++++++++++++++++++++++++++
	//First we check the Model File is in the package
	//STYLE = PACKAGE.xcd[/resdir/somemodel.xrm [meshName] ]
	if(strResName.m_ModelName.length() > 0)
	{
		ds_wstring full_model_name = item.m_ResDir + strResName.m_ModelName + m_pOwner->ext();
		xcomdoc model_doc ;
		xcomdocstream* pmodelstream = item.m_Package.open_stream(full_model_name.c_str());
		model_doc.open(pmodelstream , xcdm_read);
		if(! model_doc.failed() && pCoreMesh->load(model_doc ,  strResName.m_MeshName.c_str() , 0 ) )
		{
			XEVOL_LOG(eXL_DEBUG_TIPS, L"Load from path, search index = %d , name = %s \n",i,strResName.m_MeshName.c_str() );
			model_doc.close_stream(pmodelstream);
			model_doc.close();
			return true;
		}

		model_doc.close_stream(pmodelstream);
		model_doc.close();
	}


	//+++++++++++++++++++++++++++++++Load function 02 +++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//Next We check the dir in the package
	ds_wstring full_mesh_name ;
	if(strResName.m_ModelName.length() > 0) 
	{
		//STYLE = PACKAGE.xcd[/resdir/somemodel/meshName.mesh] ]
		full_mesh_name = item.m_ResDir + strResName.m_ModelName + L"/" + strResName.m_MeshName + m_MeshExtName ; 
	}
	else
	{
		//STYLE = PACKAGE.xcd[/resdir/meshName.mesh] ]
		full_mesh_name = item.m_ResDir +  strResName.m_MeshName + m_MeshExtName ;
	}
	//Read from Stream;
	xcomdoc mesh_doc ;
	xcomdocstream* pmeshstream = item.m_Package.open_stream(full_mesh_name.c_str());
	mesh_doc.open(pmeshstream , xcdm_read);
	if(! mesh_doc.failed() && pCoreMesh->load( mesh_doc , L""  , 0) )
	{
	    XEVOL_LOG(eXL_DEBUG_TIPS ,L"Load from path, search index = %d , name = %s \n",i,strResName.m_MeshName.c_str() );
		mesh_doc.close_stream(pmeshstream);
		mesh_doc.close();
		return true;
	}
	mesh_doc.close_stream(pmeshstream);
	mesh_doc.close();

	//+++++++++++++++++++++++++++++++Load function 03 +++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//===========================================================================
	//At last , the stream is unpackaged in the package
	ds_wstring full_dir_name = item.m_ResDir + strResName.m_ModelName + L"/" + strResName.m_MeshName;
	if(strResName.m_ModelName.length() > 0) 
	{
		//STYLE = PACKAGE.xcd[/resdir/somemodel/meshName] ]
		full_dir_name = item.m_ResDir + strResName.m_ModelName + L"/" + strResName.m_MeshName;
	}
	else
	{
		//STYLE = PACKAGE.xcd[/resdir/meshName] ]
		full_dir_name = item.m_ResDir +  strResName.m_MeshName ;
	}
	if(pCoreMesh->load( item.m_Package  , full_dir_name.c_str()   , 0) )
	{
		XEVOL_LOG(eXL_DEBUG_TIPS,L"Load from path, search index = %d , name = %s \n",i,strResName.m_MeshName.c_str() );
		return true;
	}

	return false;
}

bool xCoreMeshLoader::__loadFromDirectory(const xCoreMeshName& strResName,xCoreMesh* & pCoreMesh , xResPkgPathItem& resPkgItem, int i)
{
	//First search Model Named Package. STYLE  = DIR/somemodel.xrm[/meshName]
	if(strResName.m_ModelName.length() > 0)
	{
		ds_wstring full_model_name = resPkgItem.m_ResDir + strResName.m_ModelName + m_pOwner->ext();
		xcomdoc model_doc ;
		model_doc.open(full_model_name.c_str() , xcdm_read);
		if(! model_doc.failed() && pCoreMesh->load(model_doc  , strResName.m_MeshName.c_str() ,  0) )
		{
			XEVOL_LOG(eXL_DEBUG_TIPS, L"Load from path, search index = %d , name = %s \n",i,strResName.m_MeshName.c_str() );
			model_doc.close();
			return true;
		}
		model_doc.close();
	}
	//========================================================================

	//===========================================================================
	//Then We search the Mesh file in directory 
	ds_wstring full_mesh_name;
	if(strResName.m_ModelName.length() > 0 )
	{
		//STYLE =  DIR/somemodel/m_MeshName.mesh
		full_mesh_name = resPkgItem.m_ResDir + strResName.m_ModelName + L"/" + strResName.m_MeshName + m_MeshExtName;
	}
	else
	{
		//STYLE =  DIR/m_MeshName.mesh
		full_mesh_name = resPkgItem.m_ResDir + strResName.m_MeshName + m_MeshExtName;
	}
	xcomdoc mesh_doc;
	mesh_doc.open(full_mesh_name.c_str() , xcdm_read);
	if(! mesh_doc.failed() && pCoreMesh->load(  mesh_doc ,L"" , 0) )
	{
		XEVOL_LOG(eXL_DEBUG_TIPS,L"Load from path, search index = %d , name = %s \n",i,strResName.m_MeshName.c_str() );
		mesh_doc.close();
		return true;
	}
	mesh_doc.close();
	//===========================================================================

	return false;
}


bool xCoreMeshLoader::_loadResource(const xCoreMeshName& strResName , xCoreMesh* & pCoreMesh , int& ResSize, unsigned int arg)
{
	ds_wstring full_name;
	ResSize = 0;
	bool needDeletepCoreMesh = false;
	if(pCoreMesh == NULL)
	{
		needDeletepCoreMesh = true;
		pCoreMesh = newInstance(strResName);
		if(pCoreMesh == NULL)
			return false;
	}
	int nResItem = (int)m_pOwner->nPackageItem();
	for(int i = 0 ; i < nResItem ; i++)
	{
		xResPkgPathItem& item = m_pOwner->packageItem(i);
		bool _ret = false;
		if(item.m_Package.failed())
		{
			//We are load a File . add ext to the end;
			_ret = __loadFromDirectory(strResName , pCoreMesh,item , i);
		}
		else
		{   
			_ret = __loadFromPackage(strResName , pCoreMesh,item , i);
		}
		ResSize = pCoreMesh->memUsage();
		if(_ret) return true;

	}

	//如果是创建出来的就删除掉
	if(needDeletepCoreMesh)
	{
		delete pCoreMesh;
		pCoreMesh = NULL;
	}

	return false;
}

bool xCoreMeshLoader::_unloadResource(const xCoreMeshName& strResName,xCoreMesh* & pCoreMesh , unsigned int& TotalResSize)
{
	TotalResSize -= pCoreMesh->memUsage();
	pCoreMesh->unload();
	return true;
}

void xCoreMeshLoader::_deleteResource(const xCoreMeshName& strResName,xCoreMesh* pCoreMesh)
{
	if(pCoreMesh == NULL)
		return ;
	pCoreMesh->unload();
	pCoreMesh->KillObject();
	pCoreMesh = NULL;
}


END_NAMESPACE_XEVOL3D
