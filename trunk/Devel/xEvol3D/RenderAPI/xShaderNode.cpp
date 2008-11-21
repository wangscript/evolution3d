#include "xShaderNode.h"
#include "../BaseLib/xBaseLexer.h"
#include "../BaseLib/xI18N.h"
#include "../fs/xFileSystem.h"
BEGIN_NAMESPACE_XEVOL3D
//===============================================================================
//Resource interface
static xBaseLexer<char,std::string> gs_Lexer;
xShaderNodeSockets&  xShaderCodeNode::sockets()
{
	return m_vSockets;
}

xShaderNodeSlot*  xShaderCodeNode::slot()
{
	return m_pSlot;
}

bool  xShaderCodeNode::isSkeletonNode()
{
	return m_pSlot == NULL;
}
bool  xShaderCodeNode::isSlotNode()
{
	return m_pSlot != NULL;
}

bool xShaderCodeNode::load(const wchar_t* nodeName ,unsigned  int arg)
{
	 unsigned int8* buffer = NULL;
	int len = xFileSystem::singleton()->loadFile(buffer , nodeName);
	bool bRet = false;
	if(len > 0)
	{
		bRet = load(nodeName,buffer,len , arg);
	}	
	xFileSystem::singleton()->unloadFile(buffer);
	return bRet;
}

int xShaderCodeNode::load_Action(const char* _basecode)
{
	const char* _code = _basecode;

	std::string actionName = "";
	_code += gs_Lexer.skipWhiteSpace(_code);
	_code += gs_Lexer.getToken(_code , actionName , XEvol_IsShaderVarTypeChar );
	if(actionName.length() == 0)
	{
		XEVOL_LOG(eXL_DEBUG_NORMAL , L"Shader Node 的actionName没有名字\n");
		return (int)(_code - _basecode);
	}

	//读入Shader Node的名字
	std::string actionValue = "";
	_code += gs_Lexer.skipWhiteSpaceOneLine(_code);
	if(*_code == '=')
	{
		_code += 1;//跳过=
		_code +=  gs_Lexer.skipWhiteSpaceOneLine(_code);
		_code += gs_Lexer.getToken(_code , actionValue , XEvol_IsShaderVarNameChar);
	}

	_code += gs_Lexer.skipLine(_code);


	if(actionName == "import" || actionName == "Import")
	{
		wchar_t libName[256] = {0};
		XEvol_LocaleToUnicode(actionValue.c_str() , libName , 256);
		m_ImportNode.push_back(libName);
	}

	return (int) (_code - _basecode);
}

bool xShaderCodeNode::load(const wchar_t* nodeName , const  unsigned int8* buf , unsigned int len , unsigned int arg )
{
	m_nodeName = nodeName;
	const char* _code = (const char*) buf;
	//一共有三个Section要读入的
	while(1)
	{
		//从"//{{"开始一直到新一行的"//}}"结束
		while( *_code )
		{
			if(gs_Lexer.stringEqual(_code , SECTION_BEGIN , true) )
				break;
			if(gs_Lexer.stringEqual(_code , COMMENT_ACTION , true) )
			{
				_code += xBaseStrFunc<char,std::string>::len(COMMENT_ACTION);
				_code += load_Action(_code);
				continue;
			}
			else
			{
				_code += gs_Lexer.skipLine(_code);
				continue;
			}
			
		}
		if(*_code == 0 )
			break;

		//读入Node的类型.
		_code += xBaseStrFunc<char,std::string>::len(SECTION_BEGIN);
		_code += gs_Lexer.skipWhiteSpace(_code);
		std::string secTypeName = "";
		
		_code += gs_Lexer.getToken(_code , secTypeName , XEvol_IsShaderVarTypeChar );
		if(secTypeName.length() == 0)
		{
			XEVOL_LOG(eXL_DEBUG_NORMAL , L"Shader Node 的Section没有名字\n");
			break;
		}

		//读入Shader Node的名字
		std::string secName = "";
		_code += gs_Lexer.skipWhiteSpaceOneLine(_code);
		if(*_code == '=')
		{
			_code += 1;//跳过=
			_code +=  gs_Lexer.skipWhiteSpaceOneLine(_code);
			_code += gs_Lexer.getToken(_code , secName , XEvol_IsShaderVarNameChar);
		}

		_code += gs_Lexer.skipLine(_code);
		//根据名字选择一种Section类型
		xShaderCodeSection* pSection = createSection(secTypeName.c_str() , secName.c_str() );
		//读入这个section
		
		_code += pSection->load(_code);
		aferSectionLoad(pSection);
	}

	if(m_vSections.size() == 0)
	{
		m_code = (const char*)buf;
	}
	return true;
}


bool xShaderCodeNode::generateCode(std::ds_string& code)
{
	if(m_vSections.size() == 0)
	{
		code = m_code;
		return true;
	}

	for(size_t i = 0 ; i < m_vSections.size() ; i ++)
	{
		xShaderCodeSection* pSection = m_vSections[i];
		std::wstring secTypeName = pSection->typeName();
		if(secTypeName == DEFAULT_SECTION || secTypeName == CODE_SECTION || secTypeName == DECL_SECTION)
		{
			code += pSection->code();
		}
	}
	return true;
}

bool xShaderCodeNode::unload()
{
	m_vSections.clear();
	m_code.clear();
	m_vSockets.clear();
	if(m_pSlot)  
	{ 
		delete m_pSlot ;
		m_pSlot =  NULL;
	}
	m_ImportNode.clear();
	return true;
}

int xShaderCodeNode::memUsage()
{
	return 1;
}
//
xShaderCodeNode::xShaderCodeNode()
{
	m_pSlot = NULL;
	return ;
}

xShaderCodeNode::~xShaderCodeNode()
{
	for(size_t i = 0 ; i < m_vSockets.size() ; i ++)
	{
		m_vSockets[i]->unload();
		delete m_vSockets[i];
	}
	for(size_t i = 0 ; i < m_vSections.size() ;  i ++)
	{
		m_vSections[i]->unload();
		delete m_vSections[i];
	}
	if(m_pSlot)
	{
		m_pSlot->unload();
		delete m_pSlot;
		m_pSlot = NULL;
	}
	m_vSockets.clear();
	m_vSections.clear();
	return ;
}


xShaderCodeSection*  xShaderCodeNode::createSection(const char* _typeName , const char* _name)
{
	wchar_t ucsTypeName[32]={0};
	wchar_t ucsName[32]={0};
	XEvol_LocaleToUnicode(_typeName, ucsTypeName,32);
	XEvol_LocaleToUnicode(_name    , ucsName    ,32);
	xShaderCodeSection* pSection = new xShaderCodeSection(ucsTypeName , ucsName);
	m_vSections.push_back(pSection);
	return pSection;
}

bool xShaderCodeNode::aferSectionLoad(xShaderCodeSection* pSection)
{
	const wchar_t* _name = pSection->name();
	std::wstring _type = pSection->typeName();

	//载入Slot节点的对象 
	if(_type == SLOT_SECTION )
	{
		if(m_pSlot != NULL)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"一个Shader Node 不能有大于一个的Slot\n");
			return false;
		}
		m_pSlot = new xShaderNodeSlot(_name);
		return m_pSlot->load( pSection->code() );
	}

	//载入Socket节点的对象
	if(_type == SOCKET_SECTION)
	{
		xShaderNodeSocket* pSocket = new xShaderNodeSocket(_name);
		if( pSocket->load(pSection->code() ))
		{
			m_vSockets.push_back(pSocket);
			return true;
		}
		return false;
	}
	return true;
}

xShaderNodeSocket*  xShaderCodeNode::findSocket(const wchar_t* _name)
{
    for(size_t i = 0 ; i < m_vSockets.size() ; i ++)
	{
		xShaderNodeSocket* pSocket = m_vSockets[i];
		if(wstring(_name) == pSocket->name() )
			return pSocket;
	}
	return NULL;
}

xShaderNodeSlot*  xShaderCodeNode::getSlotInfo()
{
	return m_pSlot;
}

xShaderNodeSocket*   xShaderCodeNode::findSocket(int idx , const wchar_t* _name)
{
     if( wcslen(_name) != 0 )
		 return findSocket(_name);
	 if( idx >= 0 )
		 return findSocket(idx);
	 return NULL;
}
xShaderNodeSocket* xShaderCodeNode::findSocket(int idx)
{
	if(idx < 0 || idx >= (int)m_vSockets.size() )
		return NULL;
	xShaderNodeSocket* pSocket = m_vSockets[idx];
	return pSocket;
}

int xShaderCodeNode::findSocketIdx(const wchar_t* _name)
{
	for(size_t i = 0 ; i < m_vSockets.size() ; i ++)
	{
		xShaderNodeSocket* pSocket = m_vSockets[i];
		if(wstring(_name) == pSocket->name() )
			return (int)i;
	}
	return -1;
}
xShaderCodeSections& xShaderCodeNode::sections()
{
	return m_vSections;
}

xShaderCodeNode* xShaderCodeNode::newInstance(const wchar_t* strResName )
{
	return new xShaderCodeNode;
}

void xShaderCodeNode::deleteInstance(xShaderCodeNode* pRes)
{
	delete pRes;
}

END_NAMESPACE_XEVOL3D
