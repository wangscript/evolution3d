#include "xShaderLexer.h"
#include "../BaseLib/xBaseLexer.h"
#include "../BaseLib/xI18N.h"
#include "../BaseLib/xStringHash.h"
#include "../fs/xFileSystem.h"
BEGIN_NAMESPACE_XEVOL3D

static xBaseLexer<char,std::ds_string> gs_Lexer;

bool XEvol_IsShaderVarTypeChar(unsigned int ch )
{
	if(XEvol_IsAlphaNumChar(ch ) || ch == '_' || ch == '.')
		return true;
	return false;
}

bool XEvol_IsShaderVarNameChar(unsigned int ch)
{
	if(XEvol_IsAlphaNumChar(ch ) || ch == '_' || ch == '.')
		return true;
	return false;
}

xShaderVarParser::xShaderVarParser()
{
	//D3D
	_insert("float" ,SHADERVARTYPE_FLOAT  );
	_insert("float2",SHADERVARTYPE_FLOAT2 );
	_insert("float3",SHADERVARTYPE_FLOAT3 );
	_insert("float4",SHADERVARTYPE_FLOAT4 );

	_insert("half",SHADERVARTYPE_HALF     );
	_insert("half2",SHADERVARTYPE_HALF2   );
	_insert("half3",SHADERVARTYPE_HALF3   );
	_insert("half4",SHADERVARTYPE_HALF4   );

	_insert("int",SHADERVARTYPE_INT       );
	_insert("int2",SHADERVARTYPE_INT2     );
	_insert("int3",SHADERVARTYPE_INT3     );
	_insert("int4",SHADERVARTYPE_INT4     );

	//_insert("",SHADERVARTYPE_UINT       );
	//_insert("",SHADERVARTYPE_UINT2      );
	//_insert("",SHADERVARTYPE_UINT3      );
	//_insert("",SHADERVARTYPE_UINT4      );

	_insert("short",SHADERVARTYPE_SHORT   );
	_insert("short2",SHADERVARTYPE_SHORT2 );
	_insert("short3",SHADERVARTYPE_SHORT3 );
	_insert("short4",SHADERVARTYPE_SHORT4 );

	//_insert("",SHADERVARTYPE_USHORT     );
	//_insert("",SHADERVARTYPE_USHORT2    );
	//_insert("",SHADERVARTYPE_USHORT3    );
	//_insert("",SHADERVARTYPE_USHORT4    );

	//_insert("",SHADERVARTYPE_BYTE       );
	//_insert("",SHADERVARTYPE_BYTE2      );
	//_insert("",SHADERVARTYPE_BYTE3      );
	//_insert("",SHADERVARTYPE_BYTE4      );

	//_insert("",SHADERVARTYPE_UBYTE      );
	//_insert("",SHADERVARTYPE_UBYTE2     );
	//_insert("",SHADERVARTYPE_UBYTE3     );
	//_insert("",SHADERVARTYPE_UBYTE4     );

	//Matrix															
	_insert("float4x4",SHADERVARTYPE_Matrix          );
	_insert("float3x3",SHADERVARTYPE_Matrix3x3       );
	_insert("float4x3",SHADERVARTYPE_Matrix4x3       );
	_insert("float3x4",SHADERVARTYPE_Matrix3x4       );

	//Object
	_insert("Sampler"    ,SHADERVARTYPE_Sampler    );
	_insert("Texture"    ,SHADERVARTYPE_Texture    );
	_insert("Texture1D"  ,SHADERVARTYPE_Texture1D  );
	_insert("Texture2D"  ,SHADERVARTYPE_Texture2D  );
	_insert("Texture3D"  ,SHADERVARTYPE_Texture3D  );
	_insert("TextureCube",SHADERVARTYPE_TextureCube);
}
xShaderVarParser::~xShaderVarParser()
{

}
xShaderVarParser* xShaderVarParser::singleton()
{
    static xShaderVarParser gs_parser;
	return &gs_parser;
}
void xShaderVarParser::_insert(const char* typeName , eShaderVarType _et)
{
	int id = xStringHash(typeName);
	m_mapTypes.insert(mapShaderVarTypes::value_type(id , _et) );
}

bool xShaderVarParser::shaderVarInfo(const char* varType , xShaderVarInfo& info)
{
	int id = xStringHash(varType);
	mapShaderVarTypes::iterator pos = m_mapTypes.find(id);
	if(pos == m_mapTypes.end() )
		return false;
	return shaderVarInfo(pos->second , info);
}

bool xShaderVarParser::shaderVarInfo(eShaderVarType varType , xShaderVarInfo& info)
{
	int it = (int)varType;
	info.m_Type = varType;
	info.m_dataType = (eShaderVarDataType)(it >> 16);
	short ut = it & 0x0000ffff;
	info.m_nCol = ut>>8;
	info.m_nRow = ut & 0x00ff;
	info.m_bytePerComponent = info.m_dataType & 0x00ff;
	return true;
}

eShaderVarSemantic xShaderVarParser::shaderVarSemantic(const wchar_t* _semanticName)
{
	std::wstring semanticName = _semanticName;

	if( semanticName == SHADER_SEMANTIC_POSITION ) 
		return eSES_POSITION  ; 
	if( semanticName == SHADER_SEMANTIC_NORMAL ) 
		return eSES_NORMAL   ;  
	if( semanticName == SHADER_SEMANTIC_TANGENT ) 
		return eSES_TANGENT;    
	if( semanticName == SHADER_SEMANTIC_BINORMAL ) 
		return eSES_BINORMAL;   
	if( semanticName == SHADER_SEMANTIC_TEXCOORD ) 
		return eSES_TEXCOORD ;  
	if( semanticName == SHADER_SEMANTIC_COLOR ) 
		return eSES_DIFFUSE ;   
	if( semanticName == SHADER_SEMANTIC_ANY  ) 
		return eSES_ANY  ;      
	return eSES_ANY  ; 
}

const wchar_t* xShaderVarParser::shaderVarSemantic(eShaderVarSemantic semantic)
{
	if(semantic == eSES_POSITION )
		return    SHADER_SEMANTIC_POSITION  ; 
	if(semantic == eSES_NORMAL   )
		return    SHADER_SEMANTIC_NORMAL    ; 
	if(semantic == eSES_TANGENT  ) 
		return    SHADER_SEMANTIC_TANGENT   ; 
	if(semantic == eSES_BINORMAL ) 
		return    SHADER_SEMANTIC_BINORMAL  ; 
	if(semantic == eSES_TEXCOORD ) 
		return    SHADER_SEMANTIC_TEXCOORD  ; 
	if(semantic == eSES_DIFFUSE  ) 
		return    SHADER_SEMANTIC_COLOR   ; 
	if(semantic == eSES_ANY      ) 
		return    SHADER_SEMANTIC_ANY       ; 
	return    SHADER_SEMANTIC_ANY       ; 

}

xShaderVariable::xShaderVariable()
{

}

xShaderVariable::~xShaderVariable()
{

}

//Shader Pin
xShaderPin::xShaderPin()
{
	m_name = L"";
}

void xShaderPin::set_name(const wchar_t* _name)
{
	m_name = _name;
}

bool xShaderPin::insertArg(const char* _datatype , const char* _argName , const char* _argSemantic)
{
	wchar_t wcsDataType[32]={0};
	wchar_t wcsArgName[32]={0};
	wchar_t wcsArgSemantic[32]={0};
	XEvol_LocaleToUnicode(_datatype     , wcsDataType    , 32);
	XEvol_LocaleToUnicode(_argName      , wcsArgName     , 32);
	XEvol_LocaleToUnicode(_argSemantic  , wcsArgSemantic , 32);
	xShaderVariable _arg;
	_arg.m_TypeName = _datatype;
	_arg.m_Name     = _argName;
	_arg.m_Semantic = _argSemantic;
	xShaderVarParser::singleton()->shaderVarInfo(_arg.m_TypeName.c_str() , _arg.m_info );
	this->m_Args.push_back(_arg);
	return true;
}



int xShaderPin::load(const char* _codeBase)
{
	m_structName = "";
	
	const char* _code = _codeBase;
	_code += gs_Lexer.skipWhiteSpace(_code);

	//首先读入结构体的名字
	if(gs_Lexer.stringEqual(_code,"class",true) || gs_Lexer.stringEqual(_code,"struct",true) || gs_Lexer.stringEqual(_code,"var",true))
	{
		ds_string  _type = "";
		_code += gs_Lexer.getToken(_code , _type , XEvol_IsShaderVarTypeChar );
		_code += gs_Lexer.skipWhiteSpace(_code);
		if( *_code == '=')
		{
			_code ++ ;
			_code += gs_Lexer.skipWhiteSpace(_code);
			if(gs_Lexer.stringEqual(_type.c_str(),"class",true) || gs_Lexer.stringEqual(_type.c_str() ,"struct",true) )
			   _code += gs_Lexer.getToken(_code , m_structName , XEvol_IsShaderVarNameChar );
			else
			   _code += gs_Lexer.getToken(_code , m_varName    , XEvol_IsShaderVarNameChar );
		}
	}

	_code += gs_Lexer.skipWhiteSpace(_code);
	if(*_code != '{' )//以 '{'开头
	{
		XEVOL_LOG(eXL_DEBUG_HIGH,L"Pin的描述应该以{开头\n");
		return false;
	}
	_code ++ ;
	_code += gs_Lexer.skipWhiteSpace(_code);
	//读入属性
	while(1)
	{
		if(*_code == '}')
		{
			_code ++ ;
			break;
		}
		if(*_code == 0 )
			break;

		std::ds_string  _dataType    = "";
		std::ds_string  _argName     = "";
		std::ds_string  _argSemantic = "";
		//类型
		_code += gs_Lexer.skipWhiteSpace(_code);
		_code += gs_Lexer.getToken(_code , _dataType , XEvol_IsShaderVarTypeChar  );

		//名字
		_code += gs_Lexer.skipWhiteSpace(_code);
		_code += gs_Lexer.getToken(_code , _argName , XEvol_IsShaderVarNameChar );

		//接下来是个:
		_code += gs_Lexer.skipWhiteSpace(_code);
		if(*_code == ':')
		{
			_code ++ ;
			_code += gs_Lexer.skipWhiteSpace(_code);
			_code += gs_Lexer.getToken(_code , _argSemantic , XEvol_IsShaderVarNameChar );
			_code += gs_Lexer.skipWhiteSpace(_code);
		}

		if( *_code != ';') //应该以;来结尾
		{
			XEVOL_LOG(eXL_DEBUG_HIGH, L"Pin的参数描述应该以;来结尾\n");

		}

		XEVOL_LOG(eXL_DEBUG_TIPS, "找到一个变量,Type =%s , Name=%s , Semantic=%s\n" , _dataType.c_str() , _argName.c_str() , _argSemantic.c_str() );
		_code += gs_Lexer.skipLine(_code);
		_code += gs_Lexer.skipWhiteSpace(_code);
        insertArg( _dataType.c_str() , _argName.c_str() , _argSemantic.c_str());
		continue;
	}

	return (int)(_code - _codeBase);
}

bool xShaderPin::unload()
{
	m_Args.clear();
	return true;
}
const wchar_t*  xShaderPin::name()
{
	return m_name.c_str();
}

//============================================================================================
const wchar_t* xShaderNodeSocket::name()
{
	return m_name.c_str();
}

bool xShaderNodeSocket::load(const char* _codeBase)
{
	const char* _code = _codeBase;
	for(int i = 0 ; i < 2 ;  i ++)
	{
		_code += gs_Lexer.skipWhiteSpace(_code);
		std::ds_string  _pinType = "";
		std::ds_string  _pinName = "";
		_code += gs_Lexer.getToken(_code , _pinType , XEvol_IsShaderVarTypeChar );
		_code += gs_Lexer.skipWhiteSpaceOneLine(_code);
		if(*_code == '=')
		{
			_code += gs_Lexer.skipWhiteSpaceOneLine(_code);
			_code += gs_Lexer.getToken(_code , _pinName , XEvol_IsShaderVarNameChar );
		}
		_code += gs_Lexer.skipWhiteSpace(_code);//跳过一行


		xShaderPin* pShaderPin = NULL;
		//比较是啥Pin
		if(_pinType == "Input")
		{
			pShaderPin = &m_InputPin;     
		}
		else if(_pinType == "Output")
		{
			pShaderPin = &m_OutPin;      
		}
		else 
		{
			XEVOL_LOG(eXL_DEBUG_HIGH,"Pin的类型未知 Type=%s\n",_pinType.c_str() );
			continue;
		}


        _code += pShaderPin->load(_code);
	}
	return true;
}

bool xShaderNodeSocket::unload()
{
	m_OutPin.unload();
	m_InputPin.unload();
	return true;
}

xShaderNodeSocket::xShaderNodeSocket(const wchar_t* _name)
{
	m_name = _name;
}

//================
xShaderNodeSlot::xShaderNodeSlot(const wchar_t* _name) : xShaderNodeSocket(_name)
{
	char _ansiName[256]={0};
	XEvol_UnicodeToLocale(_name , _ansiName , 256);
	m_funcName = _ansiName;
}


//============================================================================================
xShaderCodeSection::xShaderCodeSection(const wchar_t* _typeName , const wchar_t* _name)
{
	m_typename = _typeName;
	m_name     = _name;
}
int xShaderCodeSection::load(const char* _codeBase)
{
	const char* _code = _codeBase;
	int _total_len = 0;
	//没到头，而且没结束
	while(*_code && gs_Lexer.stringEqual(_code,SECTION_END,true) == false )
	{
		int _len = gs_Lexer.skipLine(_code);
		m_code.append(_code , _len);
		_code += _len;
	}
	//跳过"//}}"这一行
	_code += gs_Lexer.skipLine(_code);
	m_code.append("\n");
	return (int)(_code - _codeBase);
}

bool xShaderCodeSection::unload()
{
	return true;
}

END_NAMESPACE_XEVOL3D
