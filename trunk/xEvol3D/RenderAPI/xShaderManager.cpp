#include "../xStdPch.h"
#include "xShaderManager.h"
#include "xShaderName.h"
#include "../BaseLib/xStringHash.h"
#include "../BaseLib/xBaseLexer.h"
#include "../BaseLib/xI18N.h"
#include "../OperationSys/xOperationSys.h"
#include <sstream>
#include <set>
BEGIN_NAMESPACE_XEVOL3D
IMPL_REFCOUNT_OBJECT_FUNCTION(IShaderCodeGenerator);
//=============GPUProgramManager======================
void xGpuProgramManager::setRenderApi(IRenderApi* pRenderApi , const wchar_t* _defExt)
{
	m_pRenderApi = pRenderApi;
	m_defExt = _defExt;
    m_CodeMgr.setDefExt(m_defExt.c_str() );
}

HGpuProgram xGpuProgramManager::loadEffect(const wchar_t* _effName)
{
	HGpuProgram hProgram ;
	return hProgram; 
}

HGpuProgram xGpuProgramManager::load(const wchar_t* _programName)
{
	HGpuProgram hProgram ;
    xGpuProgramName _name(_programName,true);
	hProgram = m_pThis->add(_name,0,true);
    return hProgram;
}

HGpuProgram xGpuProgramManager::load(const wchar_t* _vsName , const wchar_t* _psName , const wchar_t* _gsName)
{
	HGpuProgram hProgram ;
	xGpuProgramName _name(_vsName , _psName , _gsName);
	hProgram = m_pThis->add(_name,0,true);
	return hProgram;
}

HGpuProgram xGpuProgramManager::load(const wchar_t* _shaderNames[eShader_Max])
{
	HGpuProgram hProgram ;
	xGpuProgramName _name(_shaderNames);
	hProgram = m_pThis->add(_name,0,true);
	return hProgram;
}

HGpuProgram xGpuProgramManager::load(const xGpuProgramName& _programName)
{
	HGpuProgram hProgram ;
	hProgram = m_pThis->add(_programName,0,true);
	return hProgram;
}

void xGpuProgramManager::clear()
{
	  m_pThis->clear();
}

void xGpuProgramManager::unload()
{
	m_pThis->unload();
}

xGpuProgramManager::~xGpuProgramManager()
{
	clearAllSearchPath();
}

xGpuProgramManager::xGpuProgramManager()
:m_CodeMgr(L"ShaderCodeMgr" , 1)
{
     m_defExt = L".hlsl";
}


void  xGpuProgramManager::clearAllSearchPath()
{
	m_CodeMgr.clearAllSearchPath();
	xResPathManager::clearAllSearchPath();
}

bool  xGpuProgramManager::addPackage(xcomdoc& package,const wchar_t* ResDir)
{
	m_CodeMgr.addPackage(package,ResDir);
	return xResPathManager::addPackage(package,ResDir);
}

bool  xGpuProgramManager::addPackage(const wchar_t* package_file,const wchar_t* ResDir)
{
	m_CodeMgr.addPackage(package_file , ResDir);
	return xResPathManager::addPackage(package_file , ResDir);
}

bool  xGpuProgramManager::addPath(const wchar_t* ResDir)
{
	m_CodeMgr.addPath(ResDir);
	return xResPathManager::addPath(ResDir);
}

unsigned int xGpuProgramManager::_getResSize(IGpuProgram* pRes)
{
	return 1;
}

bool xGpuProgramManager::_isResLoaded(IGpuProgram* pRes)
{
	return pRes && pRes->isLoaded();
}

bool xGpuProgramManager::_loadResource(const xGpuProgramName&  strResName  , IGpuProgram* & pRes , int& ResSize, unsigned int arg)
{
	ds_wstring _vsName;
	ds_wstring _psName;
	ds_wstring _gsName;
	if(strResName.hasShader() == false)
	{
		ds_wstring _name;
		strResName.toName(_name);
		return _loadResource( _name.c_str() , pRes , ResSize, arg);
	}
	else
	{
		bool needDeletePRes = false;
		if(pRes == NULL)
		{
			needDeletePRes = true;
			pRes = m_pRenderApi->createGpuProgram( );
		}

		const wchar_t* shaderNames[eShader_Max]; 
		for(int i = 0 ; i < eShader_Max ; i ++)
			shaderNames[i] = NULL;
		strResName.toStdName(shaderNames);

		
		if( pRes->load( shaderNames , arg ) )
		{
			ResSize = 1;
			XEVOL_LOG(eXL_DEBUG_TIPS, L"Load a gpu program package\n" );
			return true;
		}

		//如果是创建出来的就删除掉
		if(needDeletePRes)
		{
			pRes->KillObject();
			pRes = NULL;
		}
		return false;


	}
	return true;
}

bool xGpuProgramManager::_loadResource(const wchar_t* strResName, IGpuProgram* & pRes , int& ResSize, unsigned int arg)
{
	ds_wstring full_name;
	ResSize = 0;
	bool needDeletePRes = false;
	if(pRes == NULL)
	{
		needDeletePRes = true;
		pRes = m_pRenderApi->createGpuProgram( );
	}
	int nResItem = (int)this->m_Packages.size();
	for(int i = 0 ; i < nResItem ; i++)
	{
		xResPkgPathItem& item = this->m_Packages[i];
		if(item.m_Package.failed())
		{
			full_name = ds_wstring(item.m_ResDir) + strResName;
			if(pRes->load(full_name.c_str() , 0) )
			{
				ResSize = pRes->memUsage();
				return true;
			}
			else
			{
				pRes->unload();
			}
		}
		else
		{
			full_name = item.m_ResDir + strResName;
			xcomdocstream* pstream = item.m_Package.open_stream(full_name.c_str());
			if(pstream)
			{
				size_t dLen = pstream->data_len();
				unsigned int8* buf = new unsigned int8[dLen];
				bool ret = false;
				if(pRes->load(full_name.c_str() , buf , dLen , 0 ) )
				{
					ResSize = pRes->memUsage();
					XEVOL_LOG(eXL_DEBUG_TIPS, L"Load from package, search index = %d name = %s\n",i,strResName );
					ret = true;
				}
				else
				{
					pRes->unload();
				}
				pstream->Release();
				delete [] buf;
				return ret ;
			}

		}

	}

	//如果是创建出来的就删除掉
	if(needDeletePRes)
	{
		pRes->KillObject();
		pRes = NULL;
	}

	return false;
}

bool xGpuProgramManager::_unloadResource(const xGpuProgramName& strResName , IGpuProgram* & pRes , unsigned int& TotalResSize)
{
	pRes->unload();
	TotalResSize -= 1;
	return true;
}

void xGpuProgramManager::_deleteResource(const xGpuProgramName& strResName , IGpuProgram*& pRes)
{
	pRes->KillObject();
	pRes = NULL;
	return ;
}

//==========Shader Code Manager============
xBaseShaderLoader::xBaseShaderLoader()
{
	m_pCodeMgr = NULL;
	m_bPreGen  = true;
	m_defExt   = L".hlsl";
#ifdef _DEBUG
	m_bPreGen = false;
#endif
	m_preGenFolder = _XEVOL_ABSPATH_(L"./Profile/");
}

void xBaseShaderLoader::setOutputForlder(const wchar_t* _path)
{
    m_pCodeMgr->addPath(_path);
	m_preGenFolder = _path;
}

void xBaseShaderLoader::enablePreGenerator(bool bEnable)
{
	m_bPreGen = bEnable;
}

void xBaseShaderLoader::setRenderApi(IRenderApi* pRenderApi , xShaderCodeNodeMgr*  pCodeMgr , const wchar_t* defExt)
{
	m_pCodeMgr   = pCodeMgr;
	m_pRenderApi = pRenderApi;
	m_defExt     = defExt;
	m_preGenFolder = _XEVOL_ABSPATH_(L"./Profile/");
}

bool xBaseShaderLoader::_isResLoaded(IBaseShader* pRes)
{
	if( pRes == NULL )
		return false;
	return pRes->isLoaded();
}

bool ToSystemFileName(const std::ds_wstring& _in , std::ds_wstring& _out)
{
	_out = _in;
	for(size_t i = 3 ;i < _out.length() ; i ++)
	{
		if(_out[i] == ':') _out[i] = ' ';
		if(_out[i] == '<') _out[i] = '(';
		if(_out[i] == '>') _out[i] = ')';
		if(_out[i] == ';') _out[i] = ' ';
		//if(ansiFileName[i] == '.') ansiFileName[i] = '_';
	}
	return true;
}

bool xBaseShaderLoader::__useCodeGenerator(const std::ds_wstring& strResName , const unsigned char * _code,IBaseShader* & pRes , int& ResSize, unsigned int arg)
{

	IShaderCodeGenerator* pCodeGenerator = NULL;
	//不能直接加载。首先检查是不是一个需要采用编辑器编辑的ShaderNode连接器的方法。那么该Shader将是一个可能是完全有编辑器产生的Shader
	if( pCodeGenerator = m_pRenderApi->createShaderCodeGenerator( _code ) )
	{
		eShaderType _st = (eShaderType)arg;
		std::ds_wstring _sysFileName = pCodeGenerator->getShaderFileName();

		xShaderCodeNode* pShaderCode = NULL;
		if(m_bPreGen)
		{
			HShaderCodeNode hCodeNode = m_pCodeMgr->add(_sysFileName,0,false);
			pShaderCode = hCodeNode.getResource();
		}

		ds_string _code;
		unsigned char* _strcode = NULL;
		if( pShaderCode ) //如果直接可以加载，那么表示这是一个单一的Node
		{
			_strcode = (unsigned char *)pShaderCode->code();
		}
		else
		{
			XEVOL_LOG(eXL_DEBUG_TIPS, L"开始组合Shader=%s\n" , strResName.c_str() );
			pCodeGenerator->getCode(_code);
			_strcode = (unsigned char *)_code.c_str() ;
			std::ds_wstring _strProfileName = m_preGenFolder + strResName + m_defExt;
			XEVOL_SVAE_LOGINFO_FILE(_XEVOL_ABSPATH_(_strProfileName.c_str() ) , ( char*)_strcode );
		}

		if( pRes->load(strResName.c_str(), _strcode , strlen( (const char *)_strcode) , _st) )
		{
			ResSize = 1;
			XEVOL_LOG(eXL_DEBUG_TIPS, L"Load a shader name=%s\n" , strResName.c_str() );
			return true;
		}
		return false;

	}
	return false;
}
bool xBaseShaderLoader::_loadResource(const std::ds_wstring& strResName,IBaseShader* & pRes , int& ResSize, unsigned int arg)
{
	ds_wstring full_name;
	ResSize = 0;
	eShaderType _st = (eShaderType)arg;
	bool needDeletePRes = false;
	if(pRes == NULL)
	{
		needDeletePRes = true;
		pRes = m_pRenderApi->createShader( _st );
	}
	

	HShaderCodeNode hCodeNode = m_pCodeMgr->add(strResName,0,false);
	if(hCodeNode.getResource() ) //如果直接可以加载，那么表示这是一个单一的Node
	{
		unsigned char* _code = NULL;
		ds_string      _strCode;
		if(hCodeNode->sections().size() == 0 && hCodeNode->importLibs().size() == 0)//没有Section,没有因入库需要处理
		{
			_code = (unsigned char*)hCodeNode->code();
		}
		else
		{
			hCodeNode->generateCode(_strCode , m_pCodeMgr);
#ifdef _DEBUG
			std::ds_wstring _strProfileName = m_preGenFolder  + strResName + m_defExt;
			XEVOL_SVAE_LOGINFO_FILE(_strProfileName.c_str()  , _strCode.c_str() );
#endif
			_code = (unsigned char*)_strCode.c_str();


		}
		//直接加载上来的可能是个CodeGenerator的脚本
	    //所以先测试
		if(__useCodeGenerator( strResName , _code , pRes , ResSize , arg) )
		{
			return true;
		}
		if( pRes->load(strResName.c_str(), _code , strlen( (const char *)_code) , _st) )
		{
			ResSize = 1;
			XEVOL_LOG(eXL_DEBUG_TIPS, L"Load a shader name=%s\n" , strResName.c_str() );
			return true;
		}
	}
	else//最后采用自动连接ShaderNode的方法
	{
		std::ds_wstring _sysFileName;
		ToSystemFileName(strResName , _sysFileName);

		xShaderCodeNode* pShaderCode = NULL;
		if(m_bPreGen)
		{
			HShaderCodeNode hCodeNode = m_pCodeMgr->add(_sysFileName,0,false);
			pShaderCode = hCodeNode.getResource();
		}
		
		ds_string _code;
		unsigned char* _strcode = NULL;
		if( pShaderCode ) //如果直接可以加载，那么表示这是一个单一的Node
		{
			_strcode = (unsigned char *)pShaderCode->code();
		}
		else
		{
			XEVOL_LOG(eXL_DEBUG_TIPS, L"开始组合Shader=%s\n" , strResName.c_str() );
			xGpuProgNameParser _shaderNameParser;
			_shaderNameParser.setShaderName(_st,strResName.c_str() );
			_shaderNameParser.generateCode(_st , m_pCodeMgr ,_code);
			_strcode = (unsigned char *)_code.c_str() ;
			std::ds_wstring _strProfileName = m_preGenFolder + strResName + m_defExt;
			XEVOL_SVAE_LOGINFO_FILE(_XEVOL_ABSPATH_(_strProfileName.c_str() ) , ( char*)_strcode );
		}

		if( pRes->load(strResName.c_str(), _strcode , strlen( (const char *)_strcode) , _st) )
		{
			ResSize = 1;
			XEVOL_LOG(eXL_DEBUG_TIPS, L"Load a shader name=%s\n" , strResName.c_str() );
			return true;
		}
	}


	//如果是创建出来的就删除掉
	if(needDeletePRes)
	{
		pRes->KillObject();
		pRes = NULL;
	}

	return false;
}

bool xBaseShaderLoader::_unloadResource(const std::ds_wstring& strResName , IBaseShader* & pRes , unsigned int& TotalResSize)
{
	TotalResSize -= 1;
	pRes->unload();
	return true;
}

void xBaseShaderLoader::_deleteResource(const std::ds_wstring& strResName , IBaseShader* pRes)
{
	if(pRes == NULL)
		return ;
	pRes->unload();
	pRes->KillObject();
	pRes = NULL;
}  

END_NAMESPACE_XEVOL3D
