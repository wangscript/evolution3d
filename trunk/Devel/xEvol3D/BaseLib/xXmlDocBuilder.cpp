#include "xXmlDocument.h"
#include "xXmlDocBuilder.h"
#include "xLogger.h"
#include <assert.h>
#include "xI18N.h"
using namespace std;

BEGIN_NAMESPACE_XEVOL3D

bool  XEvol_IsStringChar(unsigned int ch)
{
	if( ch != ' ' && ch !='=' && ch !=';' && ch !='#' && ch !='\t' && ch != 0x0a  && ch != 0x0d)
		return true;
	return false;
}

bool XEvol_IsCfgTokenChar(unsigned int ch)
{
	if( XEvol_IsAlphaNumChar(ch)  || ch == '_' 	|| ch == '-' 	|| ch == '.' 	|| ch == ':' )  
		return true;
	return false;
}

int xXmlLexer::__skipComment(const wchar_t* p)
{
	const wchar_t* pBase = p;
	//<!-- -->类型的文本。标准的xml注释
	if(xBaseLexer::stringEqual(p, L"<!--" , false))
	{
		p += wcslen(L"<!--");
		while(*p)
		{
			if( xBaseLexer::stringEqual(p, L"-->" , false) )
			{
				p += wcslen( L"-->");
				return int(p - pBase);
			}
			else
			{
				p++;
			}
		}
	}

	if(*p == 0)
		return int(p - pBase);

	// #/;我自己定义的注释
	if( stringEqual(p,L"#",false) ||
		stringEqual(p,L";",false))
	{
		p ++;
		p += xBaseLexer::skipLine(p);
		return int(p - pBase);
	}

	//我自己定义的注释 C++风格
	if( stringEqual(p,L"//",false) )
	{
		p +=2 ;
		p += xBaseLexer::skipLine(p);
		return int(p - pBase);
	}

	//我自己定义的注释 C 风格的
	if( stringEqual(p,L"/*",false) )
	{
		p +=2 ;
		while(*p && ! xBaseLexer::stringEqual(p,L"*/",false)) 
		{
			p++;
		}
		p+=2;
	}
	return int(p - pBase);
}

int  xXmlLexer::skipComment(const wchar_t* p)
{
		const wchar_t* pBase = p;
		p+= skipWhiteSpace(p);
		p+= __skipComment(p);
		p+= skipWhiteSpace(p);
		return int(p - pBase);
}

int  xXmlLexer::GetXMLStyleChar(const wchar_t* p, std::wstring& text )
{
	int i = 0;
	if(*p == '\\')
	{
		i ++ ;
		p ++;
		if( *p == 'n')
		{
			xBaseStrFunc<wchar_t,std::wstring>::appendNewLine(text);
			// text.append( L"\n");
		}
		else if( *p == 't')
		{
			xBaseStrFunc<wchar_t,std::wstring>::appendTab(text);
			// text.append( L"\t");
		}
		else if( *p == 'r')
		{
			xBaseStrFunc<wchar_t,std::wstring>::appendReturn(text);
			// text.append( L"\r");
		}
		else
		{
			text.append(p,1);
		}
		i ++;
		return 2;
	}
	else if(*p == '&')
	{
		int len = 1;
		if(stringEqual(p , L"&lt;",true) )
		{
			text.append(L"<",1);
			len = wcslen(L"&lt;");
		}
		else if(stringEqual(p , L"&gt; ",true) )
		{
			text.append(L">",1);
			len = wcslen(L"&gt;");
		}
		else if(stringEqual(p , L"&amp;",true) )
		{
			text.append(L"&",1);
			len = wcslen(L"&amp;");
		}
		else if(stringEqual(p , L"&quot;",true) )
		{
			text.append(L"\"",1);
			len = wcslen(L"&quot;");
		}
		else if(stringEqual(p , L"&apos;",true) )
		{
			text.append(L"\'",1);
			len = wcslen(L"&apos;");
		}
		else
		{
			text.push_back( *p );
			len = 1;
		}
		return len;
	}
	else
	{
		text.push_back( *p );
		return 1;
	}
}

int  xXmlLexer::getString(const wchar_t* p, std::wstring& text, const wchar_t* endTag , FUNC_IS_ACCEPT func)
{
	const wchar_t* pBase = p;
	text.clear();
	if ( xBaseStrFunc<wchar_t,std::wstring>::len(endTag) != 0	 )	
	{
		// Keep all the white space.
		while (	   p && *p  )
		{
			//to end;
			if( stringEqual( p, endTag , true ) )
				break;
			p += GetXMLStyleChar(p , text);

		}
		p += xBaseStrFunc<wchar_t,std::wstring>::len( endTag );
		return (int)(p - pBase);
	}
	else
	{
		return getToken(p,text , func);
	}
}

int  xXmlLexer::getString(	const wchar_t* p, std::wstring& text , FUNC_IS_ACCEPT func)
{
	const wchar_t* pBase = p;
	text.clear();
	assert( p );
	//if (    p && *p && ( IsAlpha( (unsigned char) *p, encoding ) || *p == '_' ) )
	{
		while(		p && *p &&	(  func(*p)  ) )
		{
			p += GetXMLStyleChar(p , text);
		}
	}
	return (int)(p - pBase);
}



xXmlLexer g_xCfgLexer;


//========XCFG Default Builder==============================
class xXmlBaseElementBuilder
{

public:
	int   readText(const wchar_t* pText,wstring& text);
};

class xXmlHeaderBuilder : public xXmlBaseElementBuilder
{
public:
	bool         m_xmlStyle;
	virtual int  parse(const wchar_t* pText);
	xXmlHeaderBuilder() {m_xmlStyle = false; }
	bool         isXMLStyle(){ return m_xmlStyle ; }
};

class xXmlElementBuilder : public xXmlBaseElementBuilder
{
public:
	xXmlNode* m_pNode;
	wstring    m_Name;
public:
	virtual  int   parseHeader(const wchar_t* pText,bool& bEndElement);
	virtual  int   parse(const wchar_t* pText);
};

class xXmlDefBuilder : public xXmlElementBuilder,public xXmlBuilder
{
public:
	xXmlHeaderBuilder   m_HeadDecl;
	bool         m_xmlStyle;
public:
	xXmlDefBuilder()
	{
		m_xmlStyle = false;
	}
	bool isXMLStyle(){return m_xmlStyle;}
	bool parseMIni(xXmlNode* pRootNode , const wchar_t* pText);
	bool parse(xXmlNode* pRootNode , const wchar_t* pText, bool bLoadHeader);
	bool loadFile(xXmlDocument* pDocument , const wchar_t* _file,bool bLoadHeader) ;
	bool loadMem(xXmlDocument* pDocument , const wchar_t* _pSrc ,bool bLoadHeader) ;
	bool loadMem(xXmlDocument* pDocument , void* _pSrc ,unsigned int iDataLen,bool bLoadHeader);
	bool loadStream(xXmlDocument* pDocument ,istream& stream ,bool bLoadHeader);
};

xXmlFileSource::xXmlFileSource()
{
	m_Buffer      = NULL;
	m_bNeedDelete = false;
	m_pData = NULL;
}

xXmlFileSource::~xXmlFileSource()
{
	if(m_Buffer && m_bNeedDelete) 
		delete[] m_Buffer;

	m_Buffer = NULL;
}
bool xXmlFileSource::setSrc(const wchar_t* pSrc)
{
	m_pData = (wchar_t*)pSrc;

	m_bNeedDelete = false;
	m_useMicrosoftBOM = true;

	//Head file;

	return true;
}

bool xXmlFileSource::loadMem(void* buf, int len)
{
	if(buf == NULL) return false;
	setSrc((wchar_t*)buf);
	return true;
}

wchar_t* xXmlFileSource::loadUCS2FileBuffer(const char* ucs16Buff , int len , bool& bNeedDeleteUCS16Buffer)
{
	bNeedDeleteUCS16Buffer = false;
	unsigned int8* cbuf = (unsigned int8*)ucs16Buff;
	if(  !(cbuf[0]==0xFF && cbuf[1]==0xFE) ) //不是UCS32
	{
		bNeedDeleteUCS16Buffer = true;
		return NULL;
	}

	if(sizeof(wchar_t) == 2 )
	{
		bNeedDeleteUCS16Buffer = false;
		return (wchar_t*)(ucs16Buff + 2);
	}
	else
	{
		bNeedDeleteUCS16Buffer = true;
		wchar_t* ucs32Buf = new wchar_t[len/2 + 1];
		int outBufLen = sizeof(wchar_t) * (len/2 + 1);
		memset(ucs32Buf , 0 ,  outBufLen);
		XEvol_UCS2ToUCS4( (unsigned short*)(ucs16Buff + 2) , (unsigned int*)ucs32Buf , len , outBufLen );
		return ucs32Buf;
	}
	return NULL;
}

bool xXmlFileSource::loadFile(const wchar_t* file)
{
	FILE* stream = NULL;
#ifdef _WIN32
	stream = _wfopen(file,L"rb");
	
#else
	char _mbsFileName[512]={0};
	XEvol_UnicodeToLocale(file,_mbsFileName,512);
	stream = fopen(_mbsFileName,"rb");
#endif

	if(stream == NULL)
		return false;

	long length = 0;
	fseek( stream, 0, SEEK_END );
	length = ftell( stream );
	fseek( stream, 0, SEEK_SET );

	char* ucs16Buff = NULL;
    ucs16Buff = new char[ length + 2] ;
	memset( ucs16Buff , 0 , length + 2);


	if ( fread( ucs16Buff,  1, length, stream ) != length ) 
	{
		delete [] ucs16Buff;
		ucs16Buff = NULL;
		return false;
	}

	bool bNeedDeleteUCS16Buffer = false;
    wchar_t*  pBuffer = loadUCS2FileBuffer(ucs16Buff,length , bNeedDeleteUCS16Buffer);
	if( bNeedDeleteUCS16Buffer )
	{
		delete [] ucs16Buff;
	}
	if(pBuffer == NULL)
	{
		return  false;
	}

	if(bNeedDeleteUCS16Buffer)
	{
		m_Buffer = pBuffer;
	}
	else
	{
		m_Buffer = (wchar_t*)ucs16Buff;
	}
	
	setSrc(pBuffer);
	m_bNeedDelete = true;
	fclose(stream);
	return true;
}

bool xXmlFileSource::loadStream(istream& stream)
{
	if(m_Buffer) delete[] m_Buffer;

	if(stream.fail())
		return false;

	long length = 0;
	int _begPos = (int)stream.tellg();
	stream.seekg(0,ios::end);
	int _endPos = (int)stream.tellg();
	length = _endPos - _begPos;
	stream.seekg(_begPos,ios::beg);

	char* ucs16Buff = NULL;
	ucs16Buff = new char[ length + 2] ;
	memset( ucs16Buff , 0 , length + 2);
	stream.read(ucs16Buff,length);

	bool bNeedDeleteUCS16Buffer = false;
	wchar_t*  pBuffer = loadUCS2FileBuffer(ucs16Buff,length , bNeedDeleteUCS16Buffer);
	if( bNeedDeleteUCS16Buffer )
	{
		delete [] ucs16Buff;
	}

	if(pBuffer == NULL)
	{
		return  false;
	}

	if(bNeedDeleteUCS16Buffer)
	{
		m_Buffer = pBuffer;
	}
	else
	{
		m_Buffer = (wchar_t*)ucs16Buff;
	}
	setSrc(pBuffer);
	m_bNeedDelete = true;
	return true;
}

int xXmlBaseElementBuilder::readText(const wchar_t* pText,wstring& text)
{
	int nWhite = g_xCfgLexer.skipWhiteSpace(pText);
	pText += nWhite;
	const wchar_t* endTag= L"";
	if(*pText == '\'')
	{
		endTag = L"\'";
	}
	if(*pText == '\"')
	{
		endTag = L"\"";
	}
	pText += wcslen(endTag);
	return nWhite +  (int)wcslen(endTag) + g_xCfgLexer.getString(pText,text,endTag , XEvol_IsCfgTokenChar);
}

int xXmlHeaderBuilder::parse(const wchar_t* pText)
{
	m_xmlStyle = false;
	const wchar_t* cfgHeadDecl = L"<?xXmlDocument";
	const wchar_t* xmlHeadDecl = L"<?xml";
	const wchar_t* pBase = pText;
	if(true == g_xCfgLexer.stringEqual(pText,cfgHeadDecl,true) )
	{
		pText += wcslen(cfgHeadDecl);	
		m_xmlStyle = false;
	}
	else if(true == g_xCfgLexer.stringEqual(pText,xmlHeadDecl,true))
	{
		pText += wcslen(xmlHeadDecl);	
		m_xmlStyle = true;
	}
	else
	{
		XEVOL_LOG(eXL_DEBUG_NORMAL,"Error Parsing xXmlDocument Header : <?xml or <?xXmlDocument not found\n");
		return 0;
	}


	while(*pText)
	{
		pText += g_xCfgLexer.skipWhiteSpace(pText);
		if(g_xCfgLexer.stringEqual(pText, L"?>",false) )
		{
			pText+= wcslen(L"?>");
			break;
		}
		wstring _name , _value;
		pText += readText(pText,_name);
		if(_name.length() == 0)
		{
			XEVOL_LOG(eXL_DEBUG_NORMAL," Error Parsing xXmlDocument Header : version not found\n");
			return int(pText - pBase);
		}
		pText+= g_xCfgLexer.skipWhiteSpace(pText);
		if(*pText != '=')
		{
			XEVOL_LOG(eXL_DEBUG_NORMAL," Error Parsing xXmlDocument Header : version's value not found miss char(=) \n");
			return int(pText - pBase);
		}
		pText ++;
		pText += readText(pText,_value);

		if(_name == L"encoding")
		{
			if(_value == L"utf-16" || _value == L"UTF-16" || _value == L"unicode" || _value == L"UNICODE") 
            {
            }
			else 
            {
				XEVOL_LOG(eXL_DEBUG_NORMAL, L" warnnig unkwon encoding =  %s \n" , _value.c_str());
            }
		}
	}

	//注释可以加在>后面
	pText += g_xCfgLexer.skipComment(pText);
	return int(pText - pBase);
}

int xXmlElementBuilder::parseHeader(const wchar_t* pText,bool& bEndElement)
{
	const wchar_t* pBase = pText;
	bEndElement = false;
	pText += g_xCfgLexer.skipWhiteSpace(pText);
	if(*pText != '<')
	{
		XEVOL_LOG(eXL_DEBUG_NORMAL," Error Parsing xXmlDocument Element : Missing char(<)\n" );
		return int(pText - pBase);
	}
	pText ++;
	pText += g_xCfgLexer.skipWhiteSpace(pText);
	wstring _elemntName;
	pText += readText(pText,_elemntName);
	if(*pText != ' '&& *pText != '>' && _elemntName.length() == 0)
	{
		XEVOL_LOG(eXL_DEBUG_NORMAL," Missing Element Name\n");
		return int(pText - pBase);
	}
	m_Name = _elemntName;
	m_pNode = m_pNode->insertNode(_elemntName.c_str());
	while(*pText)
	{
		pText += g_xCfgLexer.skipWhiteSpace(pText);
		if(g_xCfgLexer.stringEqual(pText,L">",false) )
		{
			pText+= wcslen(L">");
			break;
		}
		if(g_xCfgLexer.stringEqual(pText,L"/>",false) )
		{
			pText+= wcslen(L"/>");
			bEndElement = true;
			break;
		}

		wstring _name , _value;
		pText += readText(pText,_name);
		if(_name.length() == 0)
		{
			XEVOL_LOG(eXL_DEBUG_NORMAL," Error Parsing Value's Name length = 0\n");
			return int(pText - pBase);
		}
		pText+= g_xCfgLexer.skipWhiteSpace(pText);
		if(*pText != '=')
		{
			XEVOL_LOG(eXL_DEBUG_NORMAL," Error Parsing Value's length = 0\n");
			return int(pText - pBase);
		}
		pText ++;
		pText += readText(pText,_value);
		xXmlValue* pValue = m_pNode->insertValue(_name.c_str());
		if( pValue  == NULL)
		{
			XEVOL_LOG(eXL_DEBUG_HIGH,"error : a value named %s is exist\n",_name.c_str() );
			continue;
		}
		pValue->setValue(_value.c_str());


	}
	//注释可以加在>后面
	pText += g_xCfgLexer.skipComment(pText);
	return int(pText - pBase);
}

int xXmlElementBuilder::parse(const wchar_t* pText)
{
	const wchar_t* pBase = pText;
	bool bEndElement;
	pText += parseHeader(pText,bEndElement);
	if(bEndElement)
		return int(pText - pBase);

	while(*pText)
	{
		pText += g_xCfgLexer.skipComment(pText);
		if(g_xCfgLexer.stringEqual(pText, L"</",false) )
		{
			wstring _elementName;
			pText += wcslen(L"</");
			bool _ret = g_xCfgLexer.stringEqual(pText,m_pNode->name(),true );
			if(false ==  _ret)
			{
				XEVOL_LOG(eXL_DEBUG_NORMAL," Error Parsing xXmlDocument error : node name and end name mismatch \n");
				break;
			}
			pText += wcslen(m_pNode->name());
			if(*pText != '>')
			{
				XEVOL_LOG(eXL_DEBUG_NORMAL," Error Parsing xXmlDocument error : miss char(>) \n");
				break;
			}
			pText ++ ;
			//注释可以加在(>)后面
			pText += g_xCfgLexer.skipComment(pText);
			break;

		}

		//干掉xml风格的注释
		if(g_xCfgLexer.stringEqual(pText,L"<!--",false) )
		{
			pText += g_xCfgLexer.skipComment(pText);
			continue;
		}

		//干掉xml风格的注释
		if(g_xCfgLexer.stringEqual(pText,L"<?",false) )
		{
			pText += g_xCfgLexer.skipComment(pText);
			continue;
		}

		//干掉C风格的注释
		if(g_xCfgLexer.stringEqual(pText,L"/*",false) )
		{
			pText += g_xCfgLexer.skipComment(pText);
			continue;
		}

		//干掉C++风格的注释
		if(g_xCfgLexer.stringEqual(pText,L"//",false) )
		{
			pText += g_xCfgLexer.skipComment(pText);
			continue;
		}

		//干掉自定义风格的注释
		if(g_xCfgLexer.stringEqual(pText,L"#",false) || g_xCfgLexer.stringEqual(pText,L";",false ))
		{
			pText += g_xCfgLexer.skipComment(pText);
			continue;
		}

		if(g_xCfgLexer.stringEqual(pText,L"<",false) )
		{
			//===============================================
			//解析有没有CDATA数据
			//===============================================
			if(g_xCfgLexer.stringEqual(pText, L"<![CDATA[",false))
			{
				pText += wcslen(L"<![CDATA[");
				wstring _value;
				pText += g_xCfgLexer.getString(pText , _value , L"]]>" , XEvol_IsCfgTokenChar);
				xXmlCData* pCData = m_pNode->insertCData(false);
				if(pCData != NULL)
				{
					pCData->setValue(_value.c_str());
				}
				continue;
			}
			else///另外一个节点
			{
				xXmlElementBuilder _element;
				_element.m_pNode = this->m_pNode;
				pText += _element.parse(pText);			
				continue;
			}
		}




		//===============================================
		//解析数据------
		//<Node> NAME = "value" </Node>  类型的非XML节点
		//===============================================

		wstring _name , _value;
		pText += readText(pText,_name);
		if(_name.length() == 0)
		{
			XEVOL_LOG(eXL_DEBUG_NORMAL," Error Parsing Value's Name length = 0\n");
			return int(pText - pBase);
		}
		pText+= g_xCfgLexer.skipWhiteSpace(pText);
		if(*pText != '=')
		{
			//=========CDATA数据============================================
			XEVOL_LOG(eXL_DEBUG_NORMAL," Parsing Value's length = 0 , Treat as CDATA\n");
			xXmlCData* pCData = m_pNode->insertCData(true);
			if(pCData != NULL)
			{
				pCData->setValue(_name.c_str());
			}
		}
		else //是一个Value节点
		{
			pText ++;
			pText += readText(pText,_value);
			//注释可以加在value后面
			pText += g_xCfgLexer.skipComment(pText);
			xXmlValue* pValue = m_pNode->insertValue(_name.c_str());
			pValue->setValue(_value.c_str());
		}



	}
	return int(pText - pBase);

}

bool xXmlDefBuilder::loadFile(xXmlDocument* pDocument , const wchar_t* _file,bool bLoadHeader)
{
	xXmlFileSource src;
	if(false == src.loadFile(_file) )
		return false;
	parse(pDocument,src.getStr(),bLoadHeader) ;
	return pDocument->countNode() > 0;

}

bool xXmlDefBuilder::loadMem(xXmlDocument* pDocument , void* _pSrc ,unsigned int iDataLen,bool bLoadHeader)
{
	xXmlFileSource src;
	if(false == src.loadMem( (void *)_pSrc,iDataLen) )
		return false;
	parse(pDocument,src.getStr(),bLoadHeader) ;
	return pDocument->countNode() > 0;
}

bool xXmlDefBuilder::loadMem(xXmlDocument* pDocument , const wchar_t* _pSrc ,bool bLoadHeader)
{
	xXmlFileSource src;
	if(false == src.setSrc(_pSrc) )
		return false;
	parse(pDocument,src.getStr(),bLoadHeader) ;
	return pDocument->countNode() > 0;
}

bool xXmlDefBuilder::loadStream(xXmlDocument* pDocument ,istream& stream ,bool bLoadHeader)
{
	xXmlFileSource src;
	if(false == src.loadStream(stream) )
		return false;
	parse(pDocument,src.getStr(),bLoadHeader ) ;
	return pDocument->countNode() > 0;
}

bool xXmlDefBuilder::parse(xXmlNode* pRootNode , const wchar_t* pText , bool bLoadHeader)
{
	const wchar_t* pINIText  = pText;
	while(*pINIText)
	{
		int nComment = g_xCfgLexer.skipComment(pINIText);
		pINIText += nComment;
		if(nComment == 0) break;
	}
	pINIText += g_xCfgLexer.skipWhiteSpace(pText);
	if(*pINIText == L'[') 
	{
		return xXmlDefBuilder::parseMIni(pRootNode , pINIText);
	}

	if(bLoadHeader)
	{
		pText += m_HeadDecl.parse(pText);
	}

	xXmlDocument* pDocument = (xXmlDocument*)pRootNode;
	pDocument->setXMLStyle(m_HeadDecl.isXMLStyle() );
	m_pNode = pRootNode;
	pText += g_xCfgLexer.skipComment(pText);
	while(*pText && *pText == '<')
	{
		xXmlElementBuilder _element;
		_element.m_pNode = this->m_pNode;
		pText += _element.parse(pText);		
		pText += g_xCfgLexer.skipWhiteSpace(pText);
	}
	return true;

}

bool xXmlDefBuilder::parseMIni(xXmlNode* pDocment , const wchar_t* pText)
{
	const wchar_t* p = pText;
	xXmlNode* pRootNode = pDocment->insertNode(L"xXmlDocument");
	xXmlNode* pNode = NULL;
	while(*p)
	{
		//Skip all comment and white space
		p += g_xCfgLexer.skipWhiteSpace(p);
		p += g_xCfgLexer.skipComment(p);
		p += g_xCfgLexer.skipWhiteSpace(p);
		if(*p!='[')
		{
			XEVOL_LOG(eXL_DEBUG_NORMAL," Error reading section header missing [\n");
			return false;
		}
		p ++;
		wstring _sectionName;
		p += g_xCfgLexer.getToken(p,_sectionName, XEvol_IsCfgTokenChar );
		if(*p!=']')
		{
			XEVOL_LOG(eXL_DEBUG_NORMAL," Error reading section header missing ]\n");
			return false;
		}
		p ++;
		pNode = pRootNode->insertNode(_sectionName.c_str() );

		while(*p)
		{
			p += g_xCfgLexer.skipComment(p);
			if(*p == '[')
				break;
			p+= g_xCfgLexer.skipWhiteSpace(p);
			wstring _name,_value;
			p+= g_xCfgLexer.getToken(p,_name, XEvol_IsCfgTokenChar );
			p+= g_xCfgLexer.skipWhiteSpace(p);
			if(*p == 0)
				break;
			if(*p != '=')
			{
				XEVOL_LOG(eXL_DEBUG_NORMAL," Error reading section item miss '=' [\n");
				continue;
			}

			p++;
			p+= g_xCfgLexer.skipWhiteSpace(p);
			if(*p == '\"')
			{
				p+= 1;
				p+= g_xCfgLexer.getString(p,_value,L"\"" , XEvol_IsCfgTokenChar );
				p+= 1;
			}
			else
			{
				p+= g_xCfgLexer.getToken(p,_value        , XEvol_IsStringChar);
			}

			xXmlValue* pValue = pNode->insertValue(_name.c_str() );
			pValue->setValue(_value.c_str());
		}

	}
	return true;
}
bool xXmlBuilder::loadFile(xXmlDocument* pDocument , const wchar_t* _file,bool bLoadHeader)
{
	xXmlDefBuilder builder;
	return builder.loadFile( pDocument , _file , bLoadHeader );
}

bool xXmlBuilder::loadMem(xXmlDocument* pDocument , const wchar_t* _pSrc ,bool bLoadHeader)
{
	xXmlDefBuilder builder;
	return builder.loadMem( pDocument , _pSrc , bLoadHeader );
}

bool xXmlBuilder::loadMem(xXmlDocument* pDocument , void* _pSrc , unsigned int iDataLen , bool bLoadHeader)
{
	xXmlDefBuilder builder;
	return builder.loadMem( pDocument , _pSrc , iDataLen , bLoadHeader );
}

bool xXmlBuilder::loadStream(xXmlDocument* pDocument ,std::istream& stream ,bool bLoadHeader)
{
	xXmlDefBuilder builder;
	return builder.loadStream( pDocument, stream, bLoadHeader );
}

END_NAMESPACE_XEVOL3D
