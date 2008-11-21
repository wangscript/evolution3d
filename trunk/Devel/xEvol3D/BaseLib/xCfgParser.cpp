/*
XEvol3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xevol3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "xCfgParser.h"
#include "xCfgBuilder.h"
#include "xI18N.h"
#include <fstream>
#include <iostream>
#include <assert.h>
#include <algorithm>
#ifdef _USE_XEVOL3D_NEW_
#include "../mem/mem_operator.h"
#endif
using namespace std;
BEGIN_NAMESPACE_XEVOL3D
#ifdef _WIN32
#pragma warning (disable : 4996)
#endif
//========================================================
//xCfg Lexer
#ifdef _WIN32
#pragma warning (disable : 4996)
#endif

static xCfgLexer  gs_Lexer;
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><|
//=================================================================|
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><|
//                   xCfg Document                                 |
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><|
//=================================================================|
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><|
xCfgValue::xCfgValue(short i)
{
    m_Index = i;
}

const wchar_t* xCfgValue::name()  const
{
	return m_Name.c_str();
}

void        xCfgValue::setName(const wchar_t* newName)
{
	m_Name = newName;
}

const wchar_t* xCfgValue::value()  const 
{
	return m_Value.c_str();
}

void  xCfgValue::setValue(const wchar_t* newValue)
{
	m_Value = newValue;
}


void  xCfgValue::setValue(int newValue)
{
	wchar_t buf[32];
    swprintf_x(buf,32,L"%d",newValue);
	setValue(buf);
}

void  xCfgValue::setValue(size_t newValue)
{
	wchar_t buf[32];
	swprintf_x(buf,32,L"%d",newValue);
	setValue(buf);
}

void   xCfgValue::setHex(unsigned int newValue)
{
	wchar_t buf[32];
	swprintf_x(buf,32,L"0x%x",newValue);
	setValue(buf);
}

void  xCfgValue::setValue(float newValue)
{
	wchar_t buf[32];
	swprintf_x(buf,32,L"%f",newValue);
	setValue(buf);
}

void  xCfgValue::setValue(bool newValue)
{
	if(newValue == true)
	{
		setValue( L"true" );
	}
	else
	{
		setValue( L"false");
	}
}

unsigned int xCfgValue::hex_value() const 
{
	unsigned int v ;
	if(m_Value.size() < 2)
	{
		swscanf(m_Value.c_str(), L"%d",&v);
		return v;
	}
	if(m_Value[0] =='0' && (m_Value[1] == 'X' || m_Value[1] == 'x' ) )
	{
		swscanf(m_Value.c_str() + 2, L"%X",&v);
	    return v;
	}
	swscanf(m_Value.c_str(), L"%d",&v);
	return v;
}

int   xCfgValue::int_value()  const 
{
	int v ;
	swscanf(m_Value.c_str(), L"%d",&v);
	return v;
}

bool  xCfgValue::bool_value()  const 
{
	if( m_Value    == L"true" || m_Value == L"True"  
		|| m_Value == L"yes"  || m_Value == L"Yes" 
		|| m_Value == L"on"   || m_Value == L"On"  )
		return true;
	return false;
}

float xCfgValue::float_value()  const 
{
	float v ;
	swscanf(m_Value.c_str(), L"%f",&v);
	return v;
}

//====================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++
xCfgNode*   xCfgNode::parent()
{
	return m_pParant;
}

void         xCfgNode::setName(const wchar_t* newName)
{
	m_Name = newName;
}

const wchar_t*  xCfgNode::name()
{
	return m_Name.c_str();
}

xCfgNode::~xCfgNode()
{
	clear();
}

xCfgNode::xCfgNode()
{

}

xCfgNode::xCfgNode(const xCfgNode& )
{
};


xCfgNode* xCfgNode::operator = (const xCfgNode& )
{
		return NULL;
};

void xCfgNode::clear()
{
	for(size_t i = 0 ; i <  m_ChildrenNodes.size() ; i ++)
	{
		m_ChildrenNodes[i]->clear();
		delete m_ChildrenNodes[i];
	}
	m_ChildrenNodes.clear();

	xCfgNode::CfgValueMap::iterator it = m_Values.begin();
	for(; it != m_Values.end() ; ++it)
	{
		xCfgValue* pValue = it->second;
		delete pValue;
	}
	m_Values.clear();
}

xCfgNode*  xCfgNode::insertNode(const wchar_t* nodeName)
{
	xCfgNode* pNode = new xCfgNode;
	pNode->m_Name = nodeName;
	pNode->m_pParant = this;
	m_ChildrenNodes.push_back(pNode);
	return pNode;
}

xCfgNode*     xCfgNode::insertNodeAfter(const wchar_t* nodeName,size_t index)
{
	xCfgNode::CfgNodes::iterator pos = m_ChildrenNodes.begin() ;
	if(index >= m_ChildrenNodes.size())
		return NULL;
	for(size_t i = 0 ; i < index + 1 ; i ++ , pos ++);
	xCfgNode* pNode = new xCfgNode;
	pNode->m_Name = nodeName;
	pNode->m_pParant = this;
	m_ChildrenNodes.insert(pos,pNode);
	return pNode;
}

xCfgNode*     xCfgNode::insertNodeBefore(const wchar_t* nodeName,size_t index)
{
	xCfgNode::CfgNodes::iterator pos = m_ChildrenNodes.begin() ;
	if(index > m_ChildrenNodes.size())
		return NULL;
	for(size_t i = 0 ; i < index ; i ++ , pos ++);
	xCfgNode* pNode = new xCfgNode;
	pNode->m_Name = nodeName;
	pNode->m_pParant = this;
	m_ChildrenNodes.insert(pos,pNode);
	return pNode;
}

xCfgNode*  xCfgNode::findNode(const wchar_t* nodeName, int index)  const 
{
	int iFind = 0;
	for(int i = 0 ; i < (int)m_ChildrenNodes.size() ; i ++)
	{
		if(m_ChildrenNodes[i]->m_Name == nodeName)
		{
			if(iFind == index)
				return m_ChildrenNodes[i];
			iFind ++ ;			
		}
	}
	return NULL;
}

int   xCfgNode::findNode(const wchar_t* nodeName,xCfgNode::CfgNodes& nodes)  const 
{
	for(int i = 0 ; i < (int)m_ChildrenNodes.size() ; i ++)
	{
		if(m_ChildrenNodes[i]->m_Name == nodeName)
		{
			nodes.push_back(m_ChildrenNodes[i]);
		}
	}
	return (int)nodes.size();
}

int xCfgNode::findNode(CfgNodes& nodes)  const 
{
	for(int i = 0 ; i < (int)m_ChildrenNodes.size() ; i ++)
	{
		nodes.push_back(m_ChildrenNodes[i]);
	}
	return (int)nodes.size();
}

xCfgNode*  xCfgNode::findNode(int index)  const 
{
	if(index >= (int)m_ChildrenNodes.size() || index < 0)
		return NULL;
	return m_ChildrenNodes[index];
}
int        xCfgNode::countNode()  const
{
	return (int)m_ChildrenNodes.size();
}
int        xCfgNode::countNode(const wchar_t* nodeName)  const 
{
	int iFind = 0;
	for(int i = 0 ; i < (int)m_ChildrenNodes.size() ; i ++)
	{
		if(m_ChildrenNodes[i]->m_Name == nodeName)
		{
			iFind ++ ;			
		}
	}
	return iFind;
}

xCfgValue* xCfgNode::findValue(const wchar_t* valueName)  const 
{
	xCfgNode::CfgValueMap::const_iterator i = m_Values.find(valueName);
	if( i != m_Values.end())
	{
		return i->second;
	}
	return NULL;

}

int  xCfgNode::findValue(CfgValues& values)  const 
{
	xCfgNode::CfgValueMap::const_iterator i = m_Values.begin();
	for(; i != m_Values.end() ; ++i)
	{
		values.push_back(i->second);
	}
	return (int)values.size();
}

xCfgValue* xCfgNode::insertValue(const wchar_t* valueName)
{
	if(findValue(valueName))
		return NULL;

	xCfgValue* pValue = new xCfgValue( (short)m_Values.size() + 1 );
	pValue->setName(valueName);
	m_Values.insert(CfgValueMap::value_type(valueName , pValue) );
	return pValue;
}

void xCfgNode::setValue(const wchar_t* valueName, size_t newValue)
{
	xCfgValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		pValue = insertValue(valueName);
	}
	pValue->setValue(newValue);
}

void xCfgNode::setValue(const wchar_t* valueName, int newValue)
{
	xCfgValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		pValue = insertValue(valueName);
	}
	pValue->setValue(newValue);
}

void xCfgNode::setHex(const wchar_t* valueName, unsigned int newValue)
{
	xCfgValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		pValue = insertValue(valueName);
	}
	pValue->setHex(newValue);
}

void xCfgNode::setValue(const wchar_t* valueName, float newValue)
{
	xCfgValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		pValue = insertValue(valueName);
	}
	pValue->setValue(newValue);
}

void xCfgNode::setValue(const wchar_t* valueName, bool newValue)
{
	if(valueName == NULL)
		return ;

	xCfgValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		pValue = insertValue(valueName);
	}
	pValue->setValue(newValue);
}

void xCfgNode::setValue(const wchar_t* valueName, const wchar_t* newValue)
{
	if(valueName == NULL || newValue == NULL)
		return ;

	xCfgValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		pValue = insertValue(valueName);
	}
	pValue->setValue(newValue);
}

int xCfgNode::int_value(const wchar_t* valueName)  const 
{
	if(valueName == NULL)
		return 0;

	xCfgValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		return 0;
	}
	return pValue->int_value();
}

unsigned int xCfgNode::hex_value(const wchar_t* valueName)  const 
{
	if(valueName == NULL)
		return 0;

	xCfgValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		return 0;
	}
	return pValue->hex_value();
}

bool xCfgNode::bool_value(const wchar_t* valueName )  const 
{
	if(valueName == NULL)
		return false;

	xCfgValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		return false;
	}
	return pValue->bool_value();
}

float xCfgNode::float_value(const wchar_t* valueName)  const 
{
	if(valueName == NULL)
		return 1.0f;

	xCfgValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		return 0.0f;
	}
	return pValue->float_value();
}

const wchar_t* xCfgNode::value(const wchar_t* valueName)  const 
{
	xCfgValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		return NULL;
	}
	return pValue->value();
}

void xCfgNode::setValue(const wchar_t* nodeName , const wchar_t* valueName, int newValue   , int index)
{
	if(nodeName == NULL || valueName == NULL)
		return ;
	xCfgNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		pNode = insertNode(nodeName);
	}
	pNode->setValue(valueName,newValue);
}

void xCfgNode::setHex(const wchar_t* nodeName , const wchar_t* valueName, unsigned int newValue   , int index)
{
	if(nodeName == NULL || valueName == NULL)
		return ;
	xCfgNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		pNode = insertNode(nodeName);
	}
	pNode->setHex(valueName,newValue);
}

void xCfgNode::setValue(const wchar_t* nodeName , const wchar_t* valueName, size_t newValue   , int index)
{
	if(nodeName == NULL || valueName == NULL)
		return ;
	xCfgNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		pNode = insertNode(nodeName);
	}
	pNode->setValue(valueName,newValue);
}

void xCfgNode::setValue(const wchar_t* nodeName , const wchar_t* valueName, float newValue , int index)
{
	if(nodeName == NULL || valueName == NULL)
		return ;

	xCfgNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		pNode = insertNode(nodeName);
	}
	pNode->setValue(valueName,newValue);
}

void xCfgNode::setValue(const wchar_t* nodeName , const wchar_t* valueName, bool newValue  , int index)
{
	if(nodeName == NULL || valueName == NULL)
		return ;

	xCfgNode* pNode = findNode(nodeName,index);
	if(pNode = NULL)
	{
		pNode = insertNode(nodeName);
	}
	pNode->setValue(valueName,newValue);
}

void xCfgNode::setValue(const wchar_t* nodeName , const wchar_t* valueName, const wchar_t* newValue , int index) 
{
	if(nodeName == NULL || valueName == NULL)
		return ;

	xCfgNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		pNode = insertNode(nodeName);
	}
	pNode->setValue(valueName,newValue);
}

unsigned int   xCfgNode::hex_value(const wchar_t* nodeName, const wchar_t* valueName, int index)  const 
{

	if(nodeName == NULL || valueName == NULL)
		return 0;

	xCfgNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		return 0;
	}
	return pNode->hex_value(valueName);
}

int   xCfgNode::int_value(const wchar_t* nodeName, const wchar_t* valueName, int index)  const 
{

	if(nodeName == NULL || valueName == NULL)
		return 0;

	xCfgNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		return 0;
	}
	return pNode->int_value(valueName);
}

bool  xCfgNode::bool_value(const wchar_t* nodeName , const wchar_t* valueName, int index)  const 
{
	if(nodeName == NULL || valueName == NULL)
		return false;

	xCfgNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		return false;
	}
	return pNode->bool_value(valueName);
}

float xCfgNode::float_value(const wchar_t* nodeName, const wchar_t* valueName, int index)  const 
{
	if(nodeName == NULL || valueName == NULL)
		return 1.0f;

	xCfgNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		return 0.0f;
	}
	return pNode->float_value(valueName);
}

const wchar_t*  xCfgNode::value(const wchar_t* nodeName,const wchar_t* valueName, int index)  const 
{
	if(nodeName == NULL || valueName == NULL)
		return NULL;

	xCfgNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		return NULL;
	}
	return pNode->value(valueName);
}


//============================================================================
void xCfgDocument::setXMLStyle(bool bXMLStyle)
{
	m_saveAsXML = bXMLStyle;
}

bool xCfgDocument::isXMLStyle()
{
	return m_saveAsXML;
}

bool xCfgDocument::load(const wchar_t* fileName, bool loadHeader )
{
	return xCfgBuilder::loadFile((xCfgDocument*) this, fileName,loadHeader );
}

bool xCfgDocument::load(const int8*    buf , int len , bool loadHeader)
{
    return xCfgBuilder::loadMem((xCfgDocument*) this, (void*)buf , len , loadHeader );
}

bool xCfgDocument::load(const wchar_t* buf , int len , bool loadHeader)
{
	return xCfgBuilder::loadMem((xCfgDocument*) this, buf , loadHeader );
}

bool xCfgDocument::load(std::istream& stream , bool loadHeader )
{
	return xCfgBuilder::loadStream((xCfgDocument*) this, stream,loadHeader );
}


#ifdef _WIN32
bool xCfgDocument::save(const wchar_t* fileName , bool saveHeader )
{
	ofstream file;
	file.open(fileName,ios::binary);
	if(file.fail())
		return false;
	bool ret = save(file , saveHeader);
	file.close();
	return ret;	
}
#else
bool xCfgDocument::save(const wchar_t* fileName , bool saveHeader )
{
	ofstream file;
	char fileNameANSII[1024]={0};
	XEvol_UnicodeToLocale(fileName,fileNameANSII,1024);
	file.open(fileNameANSII,ios::binary);
	if(file.fail())
		return false;
	bool ret = save(file , saveHeader);
	file.close();
	return ret;	
}

#endif

//bool xCfgDocument::save(const char* fileName , bool saveHeader )
//{
//	ofstream file;
//	file.open(fileName,ios::binary);
//	if(file.fail())
//		return false;
//	bool ret = save(file , saveHeader);
//	file.close();
//	return ret;	
//}


class _UNICODESTREAM
{
	ostream&    _stream;
	eEncFormat _encoding;
public:

	_UNICODESTREAM(ostream& os,eEncFormat encoding , bool bAddHeader = false):_stream(os)
	{
		_encoding = encoding;
		if(bAddHeader)
		{
			switch (encoding)  
			{
			case ENC_UNICODE:
				{
					unsigned char buf[2];
					buf[0] = 0xFF;
					buf[1] = 0xFE;
					os.write((const char*)buf,2);
				}
				break;
			case ENC_UTF8:
				{

				}
				break;
			case ENC_ASCII:
				{

				}
				break;
			}
		}


	}

	_UNICODESTREAM&  endl()
	{
		switch (_encoding)  
		{
		case ENC_UNICODE:
			{
				wchar_t c[20]={0};
				swprintf_x(c,20,L"\r\n");
				_stream.write((const char*)c, sizeof(wchar_t) * 2 );
			}
			break;
		case ENC_UTF8:
		case ENC_ASCII:
			{
				char c[20]={0};
				sprintf(c,"\r\n");
				_stream.write((const char*)c, sizeof(char) * 2 );
			}
			break;
		} 
		return *this;
	}

	_UNICODESTREAM& operator<< (const wchar_t* str)
	{
		for(size_t i = 0 ; i < wcslen(str) ; i ++)
		{
			wchar_t wChar = str[i];

			if(wChar == '\n' )
			{
				wChar = '\\';
		       _stream.write( (const int8*)&wChar, sizeof(wchar_t) );
			   wChar = 'n';
			   _stream.write( (const int8*)&wChar, sizeof(wchar_t) );

			}
			else if(wChar == '\t')
			{
				wChar = '\\';
				_stream.write( (const int8*)&wChar, sizeof(wchar_t) );
				wChar = 't';
				_stream.write( (const int8*)&wChar, sizeof(wchar_t) );
			}
			else if(wChar == '\r')
			{
				wChar = '\\';
				_stream.write( (const int8*)&wChar, sizeof(wchar_t) );
				wChar = 'r';
				_stream.write( (const int8*)&wChar, sizeof(wchar_t) );
			}
			else
			{
				_stream.write( (const int8*)&wChar, sizeof(wchar_t) );
			}
		}
		return *this;
	}
};

const wchar_t* _getEncodingString(eEncFormat fmt)
{
   switch (fmt)
   {
   case ENC_ASCII:
	   return L"";
   case ENC_UNICODE:
	   return L"unicode";
   case ENC_UTF8:
	   return L"utf-8";
   }
   return L"unicode";
}


bool xCfgDocument::save(std::ostream& _stream, bool saveHeader)
{	
	_UNICODESTREAM stream(_stream,ENC_UNICODE,true);

	if(saveHeader && !m_saveAsXML)
	{
		stream<< L"<?xCfgDocument version=\"1.0\" encoding=\""<<_getEncodingString(m_encoding)<<L"\"?>";
		stream.endl();
	}
	else if( m_saveAsXML)
	{
		if(m_encoding == ENC_ASCII)
		{
			stream<< L"<?xml version=\"1.0\" ?>";
			stream.endl().endl();
		}
		else
		{
			stream<< L"<?xml version=\"1.0\" encoding=\""<<_getEncodingString(m_encoding)<<L"\"?>";
			stream.endl().endl();
		}
	}


	for(int i = 0 ; i < (int)m_ChildrenNodes.size() ; i ++)
	{
		__save(_stream,m_ChildrenNodes[i] , 0);
	}
	return true;
}

xCfgDocument::xCfgDocument()
{
	m_pParant = NULL;
	m_saveAsXML = false;
	m_encoding = ENC_UNICODE;
	m_Name = L"DocumentRoot";
}

void xCfgDocument::unload()
{
	clear();
}
static ds_wstring    makeEscapeString(ds_wstring _in)
{
	ds_wstring ret;
	for(size_t i = 0 ; i < _in.length() ; i ++)
	{
		if(_in[i] == '\\' || _in[i] == '\"' )
			ret.append(1,'\\');
		ret.append(1,_in[i]);
	}
	return ret;
}

xCfgNode*  xCfgDocument::root()
{
    if(m_ChildrenNodes.size() == 1)
		return m_ChildrenNodes[0];
	else if(m_ChildrenNodes.size() != 0)
	{
		XEVOL_LOG(eXL_DEBUG_NORMAL,"Multiple Root Node in the xCfgDocument\n");
		return NULL;
	}
	return NULL;
}

xCfgNode*  xCfgDocument::root(const wchar_t* name )
{
	if( m_ChildrenNodes.size() == 0)
		return this->insertNode(name);
	else
	{
		XEVOL_LOG(eXL_DEBUG_NORMAL,"Root Node already exist\n");
		xCfgNode* pRoot = root();
		if(pRoot)
		{
			pRoot->setName(name);
		}
		return pRoot;
	}
	
}

void xCfgDocument::__save(ostream& _stream , xCfgNode* pNode,int level)
{
	xCfgNode::CfgValues values;

	_UNICODESTREAM stream(_stream,m_encoding);
	for(int l  = 0 ; l < (level) * 4 ; l++)
		stream<< L" ";

	if(m_saveAsXML == false)
	{
		stream<< L"<"<<pNode->name()<< L">";
		stream.endl();

		pNode->findValue(values);
		std::sort(values.begin(), values.end());
		for(int i = 0 ; i < (int)values.size() ;  i ++)
		{
			
			bool bHasWhiteSpace = false;
			for(int l  = 0 ; l < (level + 1) * 4 ; l++)
				stream<< L" ";

			const wchar_t*  _valName = values[i]->name();
			for(size_t k = 0 ; k < wcslen(_valName) ; k ++ )
			{
				if( gs_Lexer.IsWhiteSpace(_valName[k]) )
				{
					bHasWhiteSpace = true;
					break;
				}
			}
			if(bHasWhiteSpace)
			{
				stream<< L"\""<<values[i]->name()<< L"\"";
			}
			else
			{
				stream<<values[i]->name();
			}
			stream<< L" = \""<<makeEscapeString(values[i]->value()).c_str()<< L"\"";
			stream.endl();
		}

		xCfgNode::CfgNodes nodes;
		pNode->findNode(nodes);
		for(int i = 0 ; i < (int)nodes.size() ; i ++)
		{
			__save(_stream,nodes[i] , level + 1);
		}

		for(int l  = 0 ; l < (level) * 4 ; l++)
			stream<<L" ";
		stream<< L"</"<<pNode->name()<< L">";
		stream.endl();
	}

	else//save as xml
	{
		stream<<L"<"<<pNode->name()<<L" ";

		pNode->findValue(values);
		std::sort(values.begin(), values.end());
		for(int i = 0 ; i < (int)values.size() ;  i ++)
		{


			stream<< L" ";
			bool bHasWhiteSpace = false;
            const wchar_t* _valName = values[i]->name();
			for(size_t k = 0 ; k < wcslen(_valName) ; k ++ )
			{
				if( gs_Lexer.IsWhiteSpace(_valName[k]) )
				{
					bHasWhiteSpace = true;
					break;
				}
			}
			if(bHasWhiteSpace)
			{
				stream<< L"\""<<values[i]->name()<< L"\"";
			}
			else
			{
				stream<<values[i]->name();
			}
			stream<< L"=\"" <<makeEscapeString(values[i]->value()).c_str()<<L"\" ";
		}

		xCfgNode::CfgNodes nodes;
		pNode->findNode(nodes);

		if(nodes.size() == 0)
		{
            stream<< L"/>";
			stream.endl();
		}
		else
		{
			stream<< L">";
			stream.endl();
			for(int i = 0 ; i < (int)nodes.size() ; i ++)
			{
				__save( _stream , nodes[i] , level + 1);
			}

			for(int l  = 0 ; l < (level) * 4 ; l++)
				stream<< L" ";
			stream<< L"</" <<pNode->name()<< L">";
			stream.endl();
		}
	}
}
END_NAMESPACE_XEVOL3D

