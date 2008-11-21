#include "xShaderName.h"
#include "../BaseLib/xStringHash.h"
#include "../BaseLib/xBaseLexer.h"
#include "../BaseLib/xI18N.h"
#include "../OperationSys/xOperationSys.h"
#include <sstream>
#include <set>
BEGIN_NAMESPACE_XEVOL3D

void  xGpuProgramName::clear()
{
	m_bLoadFromFile = false;
	m_Shaders.clear();
	m_hash1 = 0;
}
bool xGpuProgramName::hasShader() const
{
	for(size_t  i =0 ; i < m_Shaders.size() ; i ++)
	{
		eShaderType _st = m_Shaders[i].second;
		if(_st != eShader_None)
			return true;
	}
	return false;
}
bool xGpuProgramName::toStdName(ds_wstring& vs , ds_wstring& ps , ds_wstring& gs) const
{
	for(size_t i = 0 ; i < m_Shaders.size() ; i ++)
	{
		eShaderType _shaderType = m_Shaders[i].second;
		switch(_shaderType)
		{
		case eShader_VertexShader:
			{
				vs = m_Shaders[i].first.c_str();
				break;
			}
		case eShader_PixelShader:
			{
				ps = m_Shaders[i].first.c_str();
				break;
			}
		case eShader_GeometryShader:
			{
				gs = m_Shaders[i].first.c_str();
				break;
			}
		case eShader_None:
			return false;
		}
	}
	return true;
}

bool xGpuProgramName::toName(ds_wstring& name) const
{
	for(size_t i = 0 ; i < m_Shaders.size() ; i ++)
	{
		eShaderType _shaderType = m_Shaders[i].second;
		if(_shaderType == eShader_None)
		{
			name = m_Shaders[i].first.c_str();
			return true;
		}
	}
	return false;
}

void xGpuProgramName::set(const wchar_t* vs , const wchar_t* ps  , const wchar_t* gs )
{
	m_bLoadFromFile = false;
	m_Shaders.reserve(3);
	m_hash1 = 0;
	if(vs != NULL && wcslen(vs) > 0 ) 
	{
		m_hash1 +=  xStringHash(vs) ;
		m_Shaders.push_back( ShaderNameValue(vs , eShader_VertexShader) );
		m_bLoadFromFile = true;
	}
	if(ps != NULL && wcslen(ps) > 0) 
	{
		m_hash1 +=  xStringHash(ps) ;
		m_Shaders.push_back( ShaderNameValue(ps , eShader_PixelShader) );
		m_bLoadFromFile = true;

	}
	if(gs != NULL && wcslen(gs) > 0) 
	{
		m_hash1 += xStringHash(gs) ;
		m_Shaders.push_back( ShaderNameValue(gs , eShader_GeometryShader) );
		m_bLoadFromFile = true;
	}
}

void xGpuProgramName::set(const wchar_t* _shaderName, bool bLoadFromeFile)
{
	m_Shaders.reserve(5);
	if(_shaderName != NULL && wcslen(_shaderName) > 0) 
	{
		m_hash1 = 0 +  xStringHash(_shaderName) ;
		m_Shaders.push_back( ShaderNameValue(_shaderName , eShader_None) );
	}
	m_bLoadFromFile = bLoadFromeFile;
}

xGpuProgramName::xGpuProgramName(const wchar_t* vs , const wchar_t* ps , const wchar_t* gs )
{
	set(vs,ps,gs);
}

xGpuProgramName::xGpuProgramName(const wchar_t* _shaderName, bool bLoadFromeFile)
{
	set(_shaderName,bLoadFromeFile);
}


#ifdef _MSC_VER
size_t xGpuProgramName::hash_value() const
{
	size_t hv = 0;
	for(size_t i = 0 ; i < m_Shaders.size() ; i ++)
	{
		const std::ds_wstring& shaderName = m_Shaders[i].first;
		hv += xStringHash(shaderName.c_str());
	}
	return hv;
}

#endif


//=============GpuProgramNameParser===================
class xGpuProgNameParserLexer : public xBaseLexer<wchar_t , ds_wstring>
{
public:
	int getFileName(const wchar_t * p, ds_wstring& text)
	{
		const wchar_t * pBase = p;
		text.clear();
		assert( p );
		while( p && *p &&	( *p != '<' && *p !=' ' && *p !='\t' && *p != 0x0a  && *p != 0x0d)  )
		{
			p += GetCStyleChar(p , text);
		}
		return (int)(p - pBase);
	}

	int   getNodeSocketName(	const wchar_t* p, ds_wstring& text)
	{
		const wchar_t* pBase = p;
		text.clear();
		assert( p );
		//if (    p && *p && ( IsAlpha( (unsigned char) *p, encoding ) || *p == '_' ) )
		{
			while( p && *p &&	( IsAlphaNum( *p) || *p == '_' || *p == '.' ) )
			{
				text += *p;
				++p;
			}
		}
		return (int)(p - pBase);
	}
};

static xGpuProgNameParserLexer gs_Lexer;

xGpuProgNameParser::xShaderSocketInfo* xGpuProgNameParser::xShaderName::findSocket(const wchar_t* socketName)
{
	for(size_t i = 0 ; i < m_Sokects.size() ; i ++)
	{
		if(m_Sokects[i].m_name == socketName)
			return &m_Sokects[i];
	}
	return NULL;
}

xGpuProgNameParser::xShaderSocketInfo* xGpuProgNameParser::xShaderName::findSocket(const size_t   socketIdx)
{
	for(size_t i = 0 ; i < m_Sokects.size() ; i ++)
	{
		if(m_Sokects[i].m_idx == socketIdx)
			return &m_Sokects[i];
	}
	return NULL;
}

void xGpuProgNameParser::xShaderName::toName(ds_wstring& strName)
{
	strName = m_strName;
	if(m_Sokects.size() == 0)
		return ;
	strName += '<';
	for(size_t i = 0 ; i < m_Sokects.size() ; i ++)
	{
		xGpuProgNameParser::xShaderSocketInfo& Socket = m_Sokects[i];
		if(Socket.m_attachNodes.size() == 0)
			continue;
		if(Socket.m_idx == -1)
			strName += Socket.m_name;
		else
		{
			std::wostringstream oss;
			oss<<Socket.m_idx;
			strName += oss.str().c_str();
		}
		strName += ':';
		for(size_t j = 0 ; j < Socket.m_attachNodes.size() ; j ++)
		{
			strName += Socket.m_attachNodes[j];
			if(j != Socket.m_attachNodes.size() - 1)
				strName += ',';
		}
		strName += ';';
	}
	strName += '>';
	return ;
}

static int StringToIdx(const wchar_t* idx)
{
	for(size_t i = 0 ; i < wcslen(idx) ; i ++)
	{
		if( (idx[i]<'0')||(idx[i]>'9') )
			return -1;
	}
	return _wtoi(idx);
}


xGpuProgNameParser::xGpuProgNameParser(xGpuProgramName& _name)
{
	parse(_name);
}

xGpuProgNameParser::xGpuProgNameParser()
{

}

bool xGpuProgNameParser::parse(const xGpuProgramName& _name)
{
	for(size_t i = 0 ; i < _name.m_Shaders.size() ; i ++)
	{
		eShaderType _shaderType = _name.m_Shaders[i].second;
		if(_shaderType != eShader_None)
		{
			setShaderName(_shaderType , _name.m_Shaders[i].first.c_str() );
		}
	}
	return true;
}

bool xGpuProgNameParser::parse(const char* _shaderNameString)
{
	return false;
}

void xGpuProgNameParser::toName(xGpuProgramName& _name)
{
	_name.clear();
	_name.m_bLoadFromFile = false;
	ds_wstring _strVS = L"";
	ds_wstring _strPS = L"";
	ds_wstring _strGS = L"";
	if(m_VertexShader.m_strName.length() > 0)
	{
		m_VertexShader.toName(_strVS);		
	}
	if(m_PixelShader.m_strName.length() > 0)
	{
		m_PixelShader.toName(_strPS);		
	}
	if(m_GeomShader.m_strName.length() > 0)
	{
		m_GeomShader.toName(_strGS);		
	}
	_name.set(_strVS.c_str() , _strPS.c_str() , _strGS.c_str() );
	return ;
}

bool xGpuProgNameParser::setShaderName(eShaderType _shaderType , const wchar_t* _shaderName)
{
	xShaderName* pShaderName = _getShader(_shaderType);
	ds_wstring _shaderFile;
	const wchar_t* _code = _shaderName;

	//或者Shader的主文件名
	_code += gs_Lexer.getFileName(_code , _shaderFile);
	if(_shaderFile.length() == 0 )
		return false;

	pShaderName->m_strName = _shaderFile;
	//开始检测括号里的数据
	_code += gs_Lexer.skipWhiteSpace(_code);
	if(*_code != '<' )//没有加Node
		return true;

	_code += 1;

	while(*_code != 0 && *_code !='>')
	{
		//开始读取socket名字
		_code += gs_Lexer.skipWhiteSpace(_code);
		ds_wstring socketName;
		_code += gs_Lexer.getNodeSocketName(_code  , socketName);
		if(socketName.length() == 0 )
			return true;

		//Socket名字后面应该接:字符
		_code += gs_Lexer.skipWhiteSpace(_code);
		if(*_code != ':' )//没有加Node
		{
			XEVOL_LOG(eXL_DEBUG_NORMAL,"Shader标准Name中的socket=%s后未接:符号\n",socketName.c_str() );
			return true;
		}
		_code ++ ;
		_code += gs_Lexer.skipWhiteSpace(_code);

		//创建ShaderSokect;

		int idx = StringToIdx(socketName.c_str() );
		xShaderSocketInfo* pSocketInfo = NULL;
		if(idx == -1)
		{
			pSocketInfo = pShaderName->findSocket(socketName.c_str() );
		}
		else
		{
			pSocketInfo = pShaderName->findSocket( idx );
		}
		if(pSocketInfo == NULL)
		{
			xShaderSocketInfo info;
			info.m_name = idx >= 0 ? L"" : socketName;
			info.m_idx = idx;
			pShaderName->m_Sokects.push_back(info);
			pSocketInfo = & pShaderName->m_Sokects[ pShaderName->m_Sokects.size() - 1 ];
		}

		//Socket信息读取完了。加入到Socket列表里了。
		//现在来读取第一个Node名字
		while(*_code != 0 && *_code != ';' && *_code != '>')
		{
			ds_wstring _nodeName;
			_code += gs_Lexer.getNodeSocketName(_code  , _nodeName);
			if(socketName.length() == 0 )
				continue;
			_code += gs_Lexer.skipWhiteSpace(_code );
			pSocketInfo->m_attachNodes.push_back(_nodeName.c_str() );


			_code += gs_Lexer.skipWhiteSpace(_code);

			if(*_code == ',' )
			{
				_code ++ ;
				_code += gs_Lexer.skipWhiteSpace(_code);
				continue;
			}
			else if(*_code == '>')
			{
				return true;
			}
			else if(*_code == ';')
			{
				_code ++ ;
				_code += gs_Lexer.skipWhiteSpace(_code);
				break;
			}
			else
			{
				XEVOL_LOG(eXL_DEBUG_NORMAL,"Shader标准Name中的Node=%s后的字符位置\n",_nodeName.c_str() );
				return true;
			}
		}

	}


	return true;
}

void xGpuProgNameParser::addShaderNode(eShaderType _shaderType , const wchar_t* _nodeName, const wchar_t* _socketName )
{
	xShaderName* pShaderName = _getShader(_shaderType);
	int idx = StringToIdx( _socketName );
	xShaderSocketInfo* pSocketInfo = NULL;

	if(idx == -1)
	{
		pSocketInfo = pShaderName->findSocket( _socketName );
	}
	else
	{
		pSocketInfo = pShaderName->findSocket( idx );
	}
	if(pSocketInfo == NULL)
	{
		xShaderSocketInfo info;
		info.m_name = idx >= 0 ? L"" : _socketName;
		info.m_idx = idx;
		pShaderName->m_Sokects.push_back(info);
		pSocketInfo = & pShaderName->m_Sokects[ pShaderName->m_Sokects.size() - 1 ];
	}
	pSocketInfo->m_attachNodes.push_back( _nodeName );
	return ;
}

xGpuProgNameParser::xShaderName* xGpuProgNameParser::_getShader(eShaderType _shaderType)
{
	xShaderName* pShaderName = NULL;
	switch(_shaderType)
	{
	case eShader_PixelShader:
		pShaderName = &m_PixelShader;
		break;
	case eShader_VertexShader:
		pShaderName = &m_VertexShader;
		break;
	case eShader_GeometryShader:
		pShaderName = &m_GeomShader;
		break;
	}
	return pShaderName;
}
void xGpuProgNameParser::addShaderNode(eShaderType _shaderType , const wchar_t* _nodeName , size_t _socketIdx )
{
	xShaderName* pShaderName = _getShader(_shaderType);
	int idx = (int)_socketIdx;
	xShaderSocketInfo* pSocketInfo = pShaderName->findSocket( idx );
	if(pSocketInfo == NULL)
	{
		xShaderSocketInfo info;
		info.m_name =  L"" ;
		info.m_idx  = idx;
		pShaderName->m_Sokects.push_back(info);
		pSocketInfo = & pShaderName->m_Sokects[ pShaderName->m_Sokects.size() - 1 ];
	}
	pSocketInfo->m_attachNodes.push_back( _nodeName );
	return ;
}

void InsertNodes(vHShaderCodeNodes& _nodes , HShaderCodeNode hCode)
{
	for(size_t i = 0 ; i < _nodes.size() ; i ++)
	{
		if(_nodes[i] == hCode)
			return ;
	}
	_nodes.push_back(hCode);
}

void printNodeName( HShaderCodeNode hCodeNode , ds_string& _code)
{
	char node_name[1024] = {0};
	XEvol_UnicodeToLocale(hCodeNode->name() , node_name , 1024 );
	_code += node_name ; 
}

bool xGpuProgNameParser::generateCode(eShaderType _shaderType  , xShaderCodeNodeMgr* pNodeMgr , ds_string& _code)
{
	xShaderName* pShaderName = _getShader(_shaderType);

	//如果Main Code 没有找到，则返回错误
	HShaderCodeNode hMainCode = pNodeMgr->add(pShaderName->m_strName.c_str() , 0 , true);
	if(hMainCode.getResource() == NULL)
		return false;

	//如果没有附加任何Node，则调用主Node本身生成默认的 code。
	if(pShaderName->m_Sokects.size() == 0)
	{
		hMainCode->generateCode(_code);
		return true;
	}
	//如果这个主Node不能作为骨架Node，则调用默认的code，并发出警告
	if(hMainCode->isSkeletonNode() == false)
	{
		XEVOL_LOG(eXL_DEBUG_HIGH,"Shader Node 不能作为主节点使用node name=");
		XEVOL_LOG(eXL_DEBUG_HIGH,pShaderName->m_strName.c_str() );
		XEVOL_LOG(eXL_DEBUG_HIGH,"\n");
		hMainCode->generateCode(_code);
		return true;
	}

	//现在来为每个Socket找到对应的xShaderNodeSocket,每一个xShaderNodeSocket*都应该存在于hMainCode里
	//对于那些不存在的Socket，我们要去掉，并给出提示

	xShaderName shaderName ;
	shaderName.m_strName = pShaderName->m_strName;
	xShaderSocketInfos& sockets = shaderName.m_Sokects;
	for(size_t i = 0 ; i < pShaderName->m_Sokects.size() ; i ++)
	{
		ds_wstring& socketName = pShaderName->m_Sokects[i].m_name;
		int         socketIdx  = pShaderName->m_Sokects[i].m_idx;
		xShaderNodeSocket* pSocket = hMainCode->findSocket(socketIdx , socketName.c_str() );
		if(pSocket == NULL)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH,"socket name 在主节点里使用不存在 socket name=");
			XEVOL_LOG(eXL_DEBUG_HIGH,socketName.c_str() );
			XEVOL_LOG(eXL_DEBUG_HIGH,"\n");
		}
		else
		{
			sockets.push_back( pShaderName->m_Sokects[i]) ;
		}

	}

	//如果附加了Code，则先要先找出所有的Node, 并且，这个Node一定要是Slot类型的
	vHShaderCodeNodes _nodes;
	for(size_t i = 0 ; i < sockets.size() ; i ++)
	{
		for(size_t j = 0 ; j < sockets[i].m_attachNodes.size() ; j ++ )
		{
			ds_wstring& nodeName = sockets[i].m_attachNodes[j];
			HShaderCodeNode hCodeNode = pNodeMgr->add(nodeName.c_str() , 0 , true);
			if(hCodeNode.getResource() == NULL)
			{
				XEVOL_LOG(eXL_DEBUG_HIGH,"Shader Node not found name=");
				XEVOL_LOG(eXL_DEBUG_HIGH,nodeName.c_str());
				XEVOL_LOG(eXL_DEBUG_HIGH,"\n");
			}
			else if(!hCodeNode->isSlotNode() )
			{
				XEVOL_LOG(eXL_DEBUG_HIGH,"Shader Node is not a slot node name=");
				XEVOL_LOG(eXL_DEBUG_HIGH,nodeName.c_str());
				XEVOL_LOG(eXL_DEBUG_HIGH,"\n");	
			}
			else
			{
				InsertNodes(_nodes , hCodeNode);
			}
		}
	}

	//所有的Node都已经加载，并且可以使用的Socket也都已经准备完毕.

	//生成所有引入库
	std::set<ds_wstring> vLibImportSet;
	//主Node的引入库
	ds_wstring_vector&  MainImpLibs  = hMainCode->importLibs() ;
	for(size_t j = 0 ; j < MainImpLibs.size() ; j ++)
	{
		vLibImportSet.insert( MainImpLibs[j] );
	}

	for(size_t i = 0 ; i < _nodes.size() ; i ++)
	{
		HShaderCodeNode     hCodeNode   = _nodes[i];
		ds_wstring_vector&  importLibs  = hCodeNode->importLibs() ;
		for(size_t j = 0 ; j < importLibs.size() ; j ++)
		{
			vLibImportSet.insert( importLibs[j] );
		}
	}

	_code += "//===============================================\n";
	_code += "//Begin import lib\n";
	for(std::set<ds_wstring>::iterator pos = vLibImportSet.begin() ; pos != vLibImportSet.end() ; pos ++ )
	{
		ds_wstring libName = *pos ;
		HShaderCodeNode hLibNode = pNodeMgr->add(libName.c_str() , 0 , true);
		if(hLibNode.getResource())
		{
			std::ds_string libCode;
			_code += "//Import lib name=";
			char _ansiLibName[256] = {0};
			XEvol_UnicodeToLocale(libName.c_str() ,_ansiLibName , 256 );
			_code += _ansiLibName;
			_code += "\n";
			hLibNode->generateCode(libCode);
			_code += libCode;
		}
	}
	_code += "//Finish import lib\n";
	_code += "//============================================\n\n";
	//所有的slot Node 应该都要放进来


	//现在先来生成默认shader code 的declaration.
	_generateDecl(hMainCode , _code);
	for(size_t i = 0 ; i < _nodes.size() ; i ++)
	{
		HShaderCodeNode hCodeNode = _nodes[i];
		_generateDecl(hCodeNode , _code);
	}
	_code += "//==================================================\n\n";




	_code += "//============================================\n";
	for(size_t i = 0 ; i < _nodes.size() ; i ++)
	{
		HShaderCodeNode hCodeNode = _nodes[i];
		_generateSlotCode(hCodeNode , _code);
	}
	_code += "//============================================\n";


	_code += "\n\n";
	_code += "//============================================\n";
	//现在该是遍历整个hMainCode的section，并判断是不是Sokect.
	_code += "//Begin Main Node'code \n";
	xShaderCodeSections& sections = hMainCode->sections();
	int idxSocket = 0 ; //当前的Socket idx
	for(size_t i = 0 ; i < sections.size() ; i ++)
	{
		xShaderCodeSection* pSection = sections[i];
		//如果是Source Code，则直接添加进来
		if(pSection->typeName() == std::ds_wstring(CODE_SECTION) )
		{
			_code += pSection->code();
		}
		else if(pSection->typeName() == std::ds_wstring(DEFAULT_SECTION) )
		{
			//如果是默认代码，则要判断这个section指定的sockt上有没有绑定Node
			const wchar_t* socketName = pSection->name();
			const int      socketIdx  = hMainCode->findSocketIdx( socketName );
			xShaderSocketInfo* pSocketInfo = shaderName.findSocket(socketName);
			if(pSocketInfo == NULL) pSocketInfo = shaderName.findSocket( socketIdx );
			//所在的Socket上没有绑定任何Node
			if(pSocketInfo == NULL || pSocketInfo->m_attachNodes.size() == 0)
			{
				_code += pSection->code();
			}
		}
		else if( pSection->typeName() == std::ds_wstring(SOCKET_SECTION) )
		{
			//如果是默认代码，则要判断这个section指定的sockt上有没有绑定Node
			const wchar_t* socketName = pSection->name();
			const int      socketIdx  = hMainCode->findSocketIdx( socketName );
			xShaderSocketInfo* pSocketInfo = shaderName.findSocket(socketName);
			if(pSocketInfo == NULL) pSocketInfo = shaderName.findSocket( socketIdx );
			//所在的Socket上绑定了Node
			if(pSocketInfo && pSocketInfo->m_attachNodes.size() != 0)
			{
				xShaderNodeSocket* pSocket = hMainCode->findSocket( socketName );
				_generateSocketCode(pNodeMgr, pSocket,pSocketInfo , _code);
			}
		}
		//至此，代码生成完毕
	}
	return false;
}


bool xGpuProgNameParser::_generateDecl(HShaderCodeNode hCodeNode , ds_string& _code)
{
	xShaderCodeSections& sections = hCodeNode->sections();

	int nDecl = 0;
	for(size_t i = 0 ; i < sections.size() ; i ++)
	{
		xShaderCodeSection* pSection = sections[i];
		if(pSection->typeName() == std::ds_wstring(DECL_SECTION) )
			nDecl ++ ;
	}
	if(nDecl == 0)
		return true;

	_code += "\n//Insert Node Declaration Node= ";
	printNodeName(hCodeNode,_code);
	_code += "\n";
	for(size_t i = 0 ; i < sections.size() ; i ++)
	{
		xShaderCodeSection* pSection = sections[i];
		if(pSection->typeName() == std::ds_wstring(DECL_SECTION) )
		{
			_code += pSection->code();
		}
	}

	return true;
}

bool xGpuProgNameParser::_generateSlotCode(HShaderCodeNode hCodeNode , ds_string& _code)
{
	_code += "\n//Insert Node Code Node= ";
	printNodeName(hCodeNode,_code);
	_code += "\n";
	xShaderCodeSections& sections = hCodeNode->sections();
	for(size_t i = 0 ; i < sections.size() ; i ++)
	{
		xShaderCodeSection* pSection = sections[i];
		if(pSection->typeName() == std::ds_wstring(CODE_SECTION ) )
		{
			_code += pSection->code();
		}
	}
	return true;
}

struct xRetVal
{
	ds_string               m_RetVal;
	std::ds_string          m_TypeName;//类型名字
	xShaderVarInfo          m_info;
	size_t                  m_nElement;
};

class xRetValManager
{
	typedef std::map<ds_string , xRetVal> vRetVals;
	vRetVals m_vRetVals;
public:
	bool find(const char* _semantic , ds_string& strRetVal , xShaderVariable& arg)
	{
		vRetVals::iterator pos  = m_vRetVals.find(_semantic);
		if(pos == m_vRetVals.end() )
			return false;

		xRetVal retVal = pos->second;
		if(retVal.m_TypeName == arg.m_TypeName )
		{
			strRetVal = retVal.m_RetVal;
			return true;
		}
		else if(retVal.m_info.nComponent() > arg.m_info.nComponent() )
		{
			//类型转换，目前只支持floatn之间的类型转换
			switch(arg.m_info.nComponent())
			{
			case 1:
				strRetVal =  arg.m_TypeName + "(" + retVal.m_RetVal + ".x)"; 
				return true;
			case 2:
				strRetVal =  arg.m_TypeName + "(" + retVal.m_RetVal + ".xy)"; 
				return true;
			case 3:
				strRetVal =  arg.m_TypeName + "(" + retVal.m_RetVal + ".xyz)"; 
				return true;
			}

		}

		XEVOL_LOG(eXL_DEBUG_HIGH,"输入参数不匹配 申请参数类型为%s,实际参数类型为%s\n",arg.m_TypeName.c_str() , retVal.m_TypeName.c_str());

		return false;
	}

	bool insert(const char* _semantic , const char* strRetVal , xShaderVariable& arg)
	{
		xRetVal retVal ;
		retVal.m_TypeName   =arg.m_TypeName   ;//类型名字
		retVal.m_info       =arg.m_info       ;//是float4还是matrix4x4?
		retVal.m_RetVal     =strRetVal;
		retVal.m_nElement   =arg.m_nElement;
		m_vRetVals[_semantic] = retVal;
		return true;
	}
};


bool xGpuProgNameParser::_generateSocketCode(xShaderCodeNodeMgr* pNodeMgr ,xShaderNodeSocket* pSocket , xShaderSocketInfo* info , ds_string& _code)
{
	xRetValManager retValMgr;
	for(size_t i = 0 ; i < pSocket->m_InputPin.m_Args.size() ; i ++)
	{
		xShaderVariable& arg = pSocket->m_InputPin.m_Args[i];
		ds_string retArgName = arg.m_Name;
		if(pSocket->m_InputPin.m_structName.length() != 0 )
		{
			retArgName = pSocket->m_InputPin.m_structName + "." + arg.m_Name;
		}
		retValMgr.insert(arg.m_Semantic.c_str() , retArgName.c_str() , arg);
	}

	//对每一个Node,
	//1. 首先构造输入参数 ---(1). 结构体输入 (2). 非结构体输入
	//2. 生成返回值的名字，可能是结构体，也可能是基本数据结构，如果只是基本数据类型，则返回值应该只有一个。
	//3. 插入函数调用，函数的调用的参数表，由第1步生成，返回值由第二步生成。
	//4. 分析返回值，并把生成的返回值压入到返回值列表里。
	xShaderPin* pLastOutPin = NULL;
	ds_string   strLastVal = "";
	ds_string   strLastValType = "";
	for(  size_t i = 0 ; i <  info->m_attachNodes.size() ; i ++ )
	{
		char preInfo [256];
		char nodeName[64]={0};
		XEvol_UnicodeToLocale(info->m_attachNodes[i].c_str() , nodeName , 64);
		sprintf(preInfo,"\n   //应用修改器 name=%s \n" , nodeName );
		_code += preInfo;

		HShaderCodeNode hNode = pNodeMgr->add(info->m_attachNodes[i].c_str() , 0 , true);
		
		if(hNode.getResource() == NULL)
			continue;

		xShaderNodeSlot* pSlot = hNode->getSlotInfo();
		xShaderPin& InputPin = pSlot->m_InputPin;
		xShaderPin& OutPin   = pSlot->m_OutPin;
		pLastOutPin = &OutPin;
		//构造输入参数表。
		ds_string inputArgList = "";
		for(size_t iArg = 0 ; iArg < InputPin.m_Args.size() ; iArg ++)
		{
			xShaderVariable& arg = InputPin.m_Args[iArg];
			ds_string retVal = "";
			//找到对应的参数
			if(false == retValMgr.find(arg.m_Semantic.c_str() , retVal , arg) )
			{
				XEVOL_LOG(eXL_DEBUG_HIGH,"输入参数不匹配\n");
				return false;
			}
			inputArgList += retVal;
			if(iArg != InputPin.m_Args.size() - 1) inputArgList += " , ";
		}
		//输入参数表构造完毕

		//构造返回值
		ds_string outRetClass = "";
		if( OutPin.m_structName.length() != 0 )
		{

			outRetClass = OutPin.m_structName ;
		}
		else
		{
			outRetClass = OutPin.m_Args[0].m_TypeName ;
		}

		char nodeRetArgName[125];
		sprintf(nodeRetArgName , "Node_%d_Ret", i);
		strLastVal = nodeRetArgName;
		strLastValType = outRetClass;
		outRetClass += " ";
		outRetClass += nodeRetArgName ;
		outRetClass += " ";

		//生成函数调用
		ds_string funcall = "   " + outRetClass + "= " + pSlot->m_funcName + "(" + inputArgList + ");\n";
		_code += funcall ;

		//函数调用完毕. 把结果压入到返回值列表里
		for(size_t iArg = 0 ; iArg < OutPin.m_Args.size() ; iArg ++)
		{
			xShaderVariable& arg = OutPin.m_Args[iArg];
			ds_string outRetArg = "";
			if( OutPin.m_structName.length() != 0 )
			{
				outRetArg = ds_string(nodeRetArgName) + "." + arg.m_Name;
			}
			else
			{
				outRetArg = ds_string(nodeRetArgName);
			}

			retValMgr.insert(arg.m_Semantic.c_str() , outRetArg.c_str() , arg);
			continue;
		}		
	}

	//所有的函数调用都已经生成完毕，现在根据Socket的OutputPin开始整理Socket的结果
	xShaderPin& socketOutpin = pSocket->m_OutPin;
	ds_string   retVarPre = "";
	ds_string   retDecl = "";
	if(socketOutpin.m_varName.length() != 0)
	{
		retVarPre = socketOutpin.m_varName + "." ;
	}
	else if(socketOutpin.m_structName.length() != 0)
	{
		retDecl += "\n";;
		retDecl += "//声明返回值;\n";
		retDecl += "   " + socketOutpin.m_structName + "  RetVal_Finall;\n";
		retVarPre = "RetVal_Finall." ;
	}

	_code += "\n";
	if(socketOutpin.m_Args.size() > 0 )//如果输出参数列表有。就根据输出参数列表来
	{
		_code += retDecl;
		for(size_t i = 0 ; i < socketOutpin.m_Args.size() ; i ++ )
		{
			xShaderVariable& arg = socketOutpin.m_Args[i];
			ds_string assSent =  "   " + retVarPre + arg.m_Name + " = ";
			ds_string retVal = "";
			//找到对应的参数
			if(false == retValMgr.find(arg.m_Semantic.c_str() , retVal , arg) )
			{
				XEVOL_LOG(eXL_DEBUG_HIGH,"输入参数不匹配: \n");
				return false;
			}
			assSent = assSent + retVal + "; \n";
			_code += assSent;			
		}
	}
	else if(socketOutpin.m_structName.length() > 0 ) 
	{
		//如果输出Pin参数表为空，那么表示需要自己构造Shader的返回值，
		//返回多少参数可以由你自己定义的最后一个Node来决定
		//OpenGL里更简单，只要最后一个Node里声明varying就可以了。不需要用到此功能。
		//声明一个返回值
		ds_string def_ = "#define " + socketOutpin.m_structName + " " + strLastValType + " \n";
		_code = def_ + _code;
		_code += "\n";
		_code += "   return " + strLastVal  + "; \n";
		_code += "}\n";
	}

	return true;
}

END_NAMESPACE_XEVOL3D
