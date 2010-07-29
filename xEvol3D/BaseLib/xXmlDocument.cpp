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

#include "../xStdPch.h"
#include "xXmlDocument.h"
#include "xXmlDocBuilder.h"
#include "xXmlFileStream.h"
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

static xXmlLexer  gs_Lexer;
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><|
//=================================================================|
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><|
//                   xCfg Document                                 |
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><|
//=================================================================|
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><|


const wchar_t* xXmlAttribute::value()  const 
{
	return m_Value.c_str();
}

void  xXmlAttribute::setValue(const wchar_t* newValue)
{
	m_Value = newValue;
}


void  xXmlAttribute::setValue(int newValue)
{
	wchar_t buf[32];
	swprintf_x(buf,32,L"%d",newValue);
	setValue(buf);
}

void  xXmlAttribute::setValue(size_t newValue)
{
	wchar_t buf[32];
	swprintf_x(buf,32,L"%d",newValue);
	setValue(buf);
}

void   xXmlAttribute::setHex(unsigned int newValue)
{
	wchar_t buf[32];
	swprintf_x(buf,32,L"0x%x",newValue);
	setValue(buf);
}

void  xXmlAttribute::setValue(float newValue)
{
	wchar_t buf[32];
	swprintf_x(buf,32,L"%f",newValue);
	setValue(buf);
}

void  xXmlAttribute::setValue(bool newValue)
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

unsigned int xXmlAttribute::hex_value() const 
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

int   xXmlAttribute::int_value()  const 
{
	int v ;
	swscanf(m_Value.c_str(), L"%d",&v);
	return v;
}

bool  xXmlAttribute::bool_value()  const 
{
	if( m_Value    == L"true" || m_Value == L"True"  
		|| m_Value == L"yes"  || m_Value == L"Yes" 
		|| m_Value == L"on"   || m_Value == L"On"  )
		return true;
	return false;
}

float xXmlAttribute::float_value()  const 
{
	float v ;
	swscanf(m_Value.c_str(), L"%f",&v);
	return v;
}

//==========XML====Value
xXmlValue::xXmlValue(short i)
{
    m_Index = i;
}

const wchar_t* xXmlValue::name()  const
{
	return m_Name.c_str();
}

void        xXmlValue::setName(const wchar_t* newName)
{
	m_Name = newName;
}

//=======CDATA
xXmlCData::xXmlCData(bool bRawData)
{
	m_rawData = bRawData;
}
//====================================================
//++++++++++++++++++++++++++++++++++++++++++++++++++++
xXmlNode*   xXmlNode::parent()
{
	return m_pParant;
}

void         xXmlNode::setName(const wchar_t* newName)
{
	m_Name = newName;
}

const wchar_t*  xXmlNode::name()
{
	return m_Name.c_str();
}

xXmlNode::~xXmlNode()
{
	clear();
}

xXmlNode::xXmlNode()
{

}

xXmlNode::xXmlNode(const xXmlNode& )
{
};


xXmlNode* xXmlNode::operator = (const xXmlNode& )
{
		return NULL;
};

void xXmlNode::cloneTo(xXmlNode* pDestNode, bool cloneChild)
{
    //首先Clone数值
	XmlValues values;
	findValue(values);
	size_t nValue = values.size() ;
	for(size_t i = 0 ; i <  nValue ; i ++)
	{
		pDestNode->setValue(values[i]->name() , values[i]->value() );
	}

	//CData;
	size_t nCDatas = m_CDatas.size() ;
	for(size_t i = 0 ; i < nCDatas ;  i ++)
	{
		xXmlCData* pCData = m_CDatas[i];
		xXmlCData* pDestCData  = pDestNode->insertCData( pCData->bRawData() );
		pDestCData->setValue(pCData->value() );
	}

	if(cloneChild)
	{
		//所有的子节点
		size_t nChildrens = m_ChildrenNodes.size() ;
		for(size_t i = 0 ; i < nChildrens ; i ++)
		{
			xXmlNode* pChildNode = m_ChildrenNodes[i];
			xXmlNode* pNode = pDestNode->insertNode(pChildNode->name() );
			pChildNode->cloneTo(pNode , cloneChild);
		}
	}
}

void xXmlNode::clear()
{
	size_t nChildrens = m_ChildrenNodes.size() ;
	for(size_t i = 0 ; i < nChildrens ; i ++)
	{
		m_ChildrenNodes[i]->clear();
		delete m_ChildrenNodes[i];
	}
	m_ChildrenNodes.clear();

	for(xXmlNode::XmlValueMap::iterator it = m_Values.begin(); it != m_Values.end() ; ++it)
	{
		xXmlValue* pValue = it->second;
		delete pValue;
	}
	m_Values.clear();

	for(xXmlNode::xXmlCDatas::iterator it = m_CDatas.begin() ; it != m_CDatas.end() ; ++it)
	{
		xXmlCData* pCData = *it;
		delete pCData;
	}
	m_CDatas.clear();

}

void  xXmlNode::removeNode(xXmlNode* pNode)
{
	XmlNodes::iterator pos = m_ChildrenNodes.begin();
	for(;pos != m_ChildrenNodes.end() ; pos ++)
	{
		if(*pos == pNode)
		{
			delete pNode;
			m_ChildrenNodes.erase(pos);
			return ;
		}
	}
}
xXmlNode*  xXmlNode::insertNode(const wchar_t* nodeName)
{
	xXmlNode* pNode = new xXmlNode;
	pNode->m_Name = nodeName;
	pNode->m_pParant = this;
	m_ChildrenNodes.push_back(pNode);
	return pNode;
}

xXmlCData*     xXmlNode::insertCData(bool bRawData)
{
      xXmlCData* cdata = new xXmlCData(bRawData);
	  m_CDatas.push_back(cdata);
	  return cdata;
}

xXmlNode*     xXmlNode::insertNodeAfter(const wchar_t* nodeName,size_t index)
{
	xXmlNode::XmlNodes::iterator pos = m_ChildrenNodes.begin() ;
	if(index >= m_ChildrenNodes.size())
		return NULL;
	for(size_t i = 0 ; i < index + 1 ; i ++ , pos ++);
	xXmlNode* pNode = new xXmlNode;
	pNode->m_Name = nodeName;
	pNode->m_pParant = this;
	m_ChildrenNodes.insert(pos,pNode);
	return pNode;
}

xXmlNode*     xXmlNode::insertNodeBefore(const wchar_t* nodeName,size_t index)
{
	xXmlNode::XmlNodes::iterator pos = m_ChildrenNodes.begin() ;
	if(index > m_ChildrenNodes.size())
		return NULL;
	for(size_t i = 0 ; i < index ; i ++ , pos ++);
	xXmlNode* pNode = new xXmlNode;
	pNode->m_Name = nodeName;
	pNode->m_pParant = this;
	m_ChildrenNodes.insert(pos,pNode);
	return pNode;
}

xXmlNode*  xXmlNode::findNode(const wchar_t* nodeName, int index)  const 
{
	int iFind = 0;
	int nChildrenNode = (int)m_ChildrenNodes.size();
	for(int i = 0 ; i <  nChildrenNode; i ++)
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

int   xXmlNode::findNode(const wchar_t* nodeName,xXmlNode::XmlNodes& nodes)  const 
{
	int nChildrenNode = (int)m_ChildrenNodes.size();
	for(int i = 0 ; i <  nChildrenNode; i ++)
	{
		if(m_ChildrenNodes[i]->m_Name == nodeName)
		{
			nodes.push_back(m_ChildrenNodes[i]);
		}
	}
	return (int)nodes.size();
}

int xXmlNode::findNode(XmlNodes& nodes)  const 
{
	int nChildrenNode = (int)m_ChildrenNodes.size();
	for(int i = 0 ; i <  nChildrenNode; i ++)
	{
		nodes.push_back(m_ChildrenNodes[i]);
	}
	return (int)nodes.size();
}

xXmlNode*  xXmlNode::findNode(int index)  const 
{
	if(index >= (int)m_ChildrenNodes.size() || index < 0)
		return NULL;
	return m_ChildrenNodes[index];
}

xXmlCData* xXmlNode::findCData(int index)  const
{
	if(index >= (int)m_CDatas.size() || index < 0)
		return NULL;
	return m_CDatas[index];
}

int        xXmlNode::countNode()  const
{
	return (int)m_ChildrenNodes.size();
}

int xXmlNode::countCData() const
{
	return (int)m_CDatas.size();
}


int        xXmlNode::countNode(const wchar_t* nodeName)  const 
{
	int iFind = 0;
	int nChildrenNode = (int)m_ChildrenNodes.size();
	for(int i = 0 ; i <  nChildrenNode; i ++)
	{
		if(m_ChildrenNodes[i]->m_Name == nodeName)
		{
			iFind ++ ;			
		}
	}
	return iFind;
}

xXmlValue* xXmlNode::findValue(const wchar_t* valueName)  const 
{
	xXmlNode::XmlValueMap::const_iterator i = m_Values.find(valueName);
	if( i != m_Values.end())
	{
		return i->second;
	}
	return NULL;

}

int  xXmlNode::findValue(XmlValues& values)  const 
{
	xXmlNode::XmlValueMap::const_iterator i = m_Values.begin();
	for(; i != m_Values.end() ; ++i)
	{
		values.push_back(i->second);
	}
	return (int)values.size();
}

xXmlValue* xXmlNode::insertValue(const wchar_t* valueName)
{
	if(findValue(valueName))
		return NULL;

	xXmlValue* pValue = new xXmlValue( (short)m_Values.size() + 1 );
	pValue->setName(valueName);
	m_Values.insert(XmlValueMap::value_type(valueName , pValue) );
	return pValue;
}

void xXmlNode::setValue(const wchar_t* valueName, size_t newValue)
{
	xXmlValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		pValue = insertValue(valueName);
	}
	pValue->setValue(newValue);
}

void xXmlNode::setValue(const wchar_t* valueName, int newValue)
{
	xXmlValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		pValue = insertValue(valueName);
	}
	pValue->setValue(newValue);
}

void xXmlNode::setHex(const wchar_t* valueName, unsigned int newValue)
{
	xXmlValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		pValue = insertValue(valueName);
	}
	pValue->setHex(newValue);
}

void xXmlNode::setValue(const wchar_t* valueName, float newValue)
{
	xXmlValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		pValue = insertValue(valueName);
	}
	pValue->setValue(newValue);
}

void xXmlNode::setValue(const wchar_t* valueName, bool newValue)
{
	if(valueName == NULL)
		return ;

	xXmlValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		pValue = insertValue(valueName);
	}
	pValue->setValue(newValue);
}

void xXmlNode::setValue(const wchar_t* valueName, const wchar_t* newValue)
{
	if(valueName == NULL || newValue == NULL)
		return ;

	xXmlValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		pValue = insertValue(valueName);
	}
	pValue->setValue(newValue);
}

int xXmlNode::int_value(const wchar_t* valueName)  const 
{
	if(valueName == NULL)
		return 0;

	xXmlValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		return 0;
	}
	return pValue->int_value();
}

unsigned int xXmlNode::hex_value(const wchar_t* valueName)  const 
{
	if(valueName == NULL)
		return 0;

	xXmlValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		return 0;
	}
	return pValue->hex_value();
}

bool xXmlNode::bool_value(const wchar_t* valueName )  const 
{
	if(valueName == NULL)
		return false;

	xXmlValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		return false;
	}
	return pValue->bool_value();
}

float xXmlNode::float_value(const wchar_t* valueName)  const 
{
	if(valueName == NULL)
		return 1.0f;

	xXmlValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		return 0.0f;
	}
	return pValue->float_value();
}

const wchar_t* xXmlNode::value(const wchar_t* valueName)  const 
{
	xXmlValue* pValue = findValue(valueName);
	if(pValue == NULL)
	{
		return NULL;
	}
	return pValue->value();
}

void xXmlNode::setValue(const wchar_t* nodeName , const wchar_t* valueName, int newValue   , int index)
{
	if(nodeName == NULL || valueName == NULL)
		return ;
	xXmlNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		pNode = insertNode(nodeName);
	}
	pNode->setValue(valueName,newValue);
}

void xXmlNode::setHex(const wchar_t* nodeName , const wchar_t* valueName, unsigned int newValue   , int index)
{
	if(nodeName == NULL || valueName == NULL)
		return ;
	xXmlNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		pNode = insertNode(nodeName);
	}
	pNode->setHex(valueName,newValue);
}

void xXmlNode::setValue(const wchar_t* nodeName , const wchar_t* valueName, size_t newValue   , int index)
{
	if(nodeName == NULL || valueName == NULL)
		return ;
	xXmlNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		pNode = insertNode(nodeName);
	}
	pNode->setValue(valueName,newValue);
}

void xXmlNode::setValue(const wchar_t* nodeName , const wchar_t* valueName, float newValue , int index)
{
	if(nodeName == NULL || valueName == NULL)
		return ;

	xXmlNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		pNode = insertNode(nodeName);
	}
	pNode->setValue(valueName,newValue);
}

void xXmlNode::setValue(const wchar_t* nodeName , const wchar_t* valueName, bool newValue  , int index)
{
	if(nodeName == NULL || valueName == NULL)
		return ;

	xXmlNode* pNode = findNode(nodeName,index);
	if(pNode = NULL)
	{
		pNode = insertNode(nodeName);
	}
	pNode->setValue(valueName,newValue);
}

void xXmlNode::setValue(const wchar_t* nodeName , const wchar_t* valueName, const wchar_t* newValue , int index) 
{
	if(nodeName == NULL || valueName == NULL)
		return ;

	xXmlNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		pNode = insertNode(nodeName);
	}
	pNode->setValue(valueName,newValue);
}

unsigned int   xXmlNode::hex_value(const wchar_t* nodeName, const wchar_t* valueName, int index)  const 
{

	if(nodeName == NULL || valueName == NULL)
		return 0;

	xXmlNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		return 0;
	}
	return pNode->hex_value(valueName);
}

int   xXmlNode::int_value(const wchar_t* nodeName, const wchar_t* valueName, int index)  const 
{

	if(nodeName == NULL || valueName == NULL)
		return 0;

	xXmlNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		return 0;
	}
	return pNode->int_value(valueName);
}

bool  xXmlNode::bool_value(const wchar_t* nodeName , const wchar_t* valueName, int index)  const 
{
	if(nodeName == NULL || valueName == NULL)
		return false;

	xXmlNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		return false;
	}
	return pNode->bool_value(valueName);
}

float xXmlNode::float_value(const wchar_t* nodeName, const wchar_t* valueName, int index)  const 
{
	if(nodeName == NULL || valueName == NULL)
		return 1.0f;

	xXmlNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		return 0.0f;
	}
	return pNode->float_value(valueName);
}

const wchar_t*  xXmlNode::value(const wchar_t* nodeName,const wchar_t* valueName, int index)  const 
{
	if(nodeName == NULL || valueName == NULL)
		return NULL;

	xXmlNode* pNode = findNode(nodeName,index);
	if(pNode == NULL)
	{
		return NULL;
	}
	return pNode->value(valueName);
}

//取值并设置默认值
unsigned int   xXmlNode::value(const wchar_t* nodeName , const wchar_t* valueName, const unsigned int defValue ,int index )
{
    if(nodeName == NULL || valueName == NULL) return NULL;
    xXmlNode* pNode = findNode(nodeName,index);
    if(pNode == NULL) { pNode = insertNode(nodeName) ; }
    xXmlValue* value = pNode->findValue(valueName);
    if(value == NULL) 
    {
        pNode->setHex(valueName , defValue);
    }
    return pNode->hex_value(valueName);
}

int            xXmlNode::value(const wchar_t* nodeName , const wchar_t* valueName, const int defValue ,int index )
{
    if(nodeName == NULL || valueName == NULL) return NULL;
    xXmlNode* pNode = findNode(nodeName,index);
    if(pNode == NULL) { pNode = insertNode(nodeName) ; }
    xXmlValue* value = pNode->findValue(valueName);
    if(value == NULL) 
    {
        pNode->setValue(valueName , defValue);
    }
    return pNode->int_value(valueName);
}

bool           xXmlNode::value(const wchar_t* nodeName , const wchar_t* valueName, const bool         defValue ,int index )
{
    if(nodeName == NULL || valueName == NULL) return NULL;
    xXmlNode* pNode = findNode(nodeName,index);
    if(pNode == NULL) { pNode = insertNode(nodeName) ; }
    xXmlValue* value = pNode->findValue(valueName);
    if(value == NULL) 
    {
        pNode->setValue(valueName , defValue);
    }
    return pNode->bool_value(valueName);
}

float          xXmlNode::value(const wchar_t* nodeName , const wchar_t* valueName, const float        defValue ,int index )
{
    if(nodeName == NULL || valueName == NULL) return NULL;
    xXmlNode* pNode = findNode(nodeName,index);
    if(pNode == NULL) { pNode = insertNode(nodeName) ; }
    xXmlValue* value = pNode->findValue(valueName);
    if(value == NULL) 
    {
        pNode->setValue(valueName , defValue);
    }
    return pNode->float_value(valueName);
}

const wchar_t* xXmlNode::value(const wchar_t* nodeName , const wchar_t* valueName, const wchar_t*     defValue ,int index )
{
    if(nodeName == NULL || valueName == NULL) return NULL;
    xXmlNode* pNode = findNode(nodeName,index);
    if(pNode == NULL) { pNode = insertNode(nodeName) ; }
    xXmlValue* value = pNode->findValue(valueName);
    if(value == NULL) 
    {
        pNode->setValue(valueName , defValue);
    }
    return pNode->value(valueName);
}

//============================================================================
void xXmlDocument::setXMLStyle(bool bXMLStyle)
{
	m_saveAsXML = bXMLStyle;
}

bool xXmlDocument::isXMLStyle()
{
	return m_saveAsXML;
}

bool xXmlDocument::load(const wchar_t* fileName, bool loadHeader )
{
	return xXmlBuilder::loadFile((xXmlDocument*) this, fileName,loadHeader );
}

bool xXmlDocument::load(const int8*    buf , int len , bool loadHeader)
{
    return xXmlBuilder::loadMem((xXmlDocument*) this, (void*)buf , len , loadHeader );
}

bool xXmlDocument::load(const wchar_t* buf , int len , bool loadHeader)
{
	return xXmlBuilder::loadMem((xXmlDocument*) this, buf , loadHeader );
}

bool xXmlDocument::load(const int8*    buf , size_t len , bool loadHeader)
{
    return xXmlBuilder::loadMem((xXmlDocument*) this, (void*)buf , (int)len , loadHeader );
}

bool xXmlDocument::load(const wchar_t* buf , size_t len , bool loadHeader)
{
    return xXmlBuilder::loadMem((xXmlDocument*) this, buf , loadHeader );
}

bool xXmlDocument::load(IStreamIO* stream , bool loadHeader )
{
    int pos = (int)stream->tell();
    stream->seek(0 , ios::end);
    int len = (int)stream->tell() - pos;
    stream->seek(pos , ios::beg);

    int8 * buf = new int8[len];
    stream->read(buf , len);

    bool bRet = load( buf , len , loadHeader );
    delete [] buf ;
    return bRet;
}

bool xXmlDocument::load(std::istream& stream , bool loadHeader )
{
	return xXmlBuilder::loadStream((xXmlDocument*) this, stream,loadHeader );
}


#ifdef _WIN32
bool xXmlDocument::save(const wchar_t* fileName , bool saveHeader , eTextEncode _encode)
{
	ofstream file;
	file.open(fileName,ios::binary);
	if(file.fail())
		return false;
	bool ret = save(file , saveHeader ,_encode);
	file.close();
	return ret;	
}
#else
bool xXmlDocument::save(const wchar_t* fileName , bool saveHeader , eTextEncode _encode )
{
	ofstream file;
	char fileNameANSII[1024]={0};
	XEvol_UnicodeToFsEnc(fileName,fileNameANSII,1024);
	file.open(fileNameANSII,ios::binary);
	if(file.fail())
		return false;
	bool ret = save(file , saveHeader , _encode);
	file.close();
	return ret;	
}

#endif

//bool xXmlDocument::save(const char* fileName , bool saveHeader )
//{
//	ofstream file;
//	file.open(fileName,ios::binary);
//	if(file.fail())
//		return false;
//	bool ret = save(file , saveHeader);
//	file.close();
//	return ret;	
//}



xXmlDocument::xXmlDocument()
{
	m_pParant = NULL;
	m_saveAsXML = true;
	m_Name = L"DocumentRoot";
}

xXmlDocument::~xXmlDocument()
{
    unload();
}

void xXmlDocument::unload()
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

xXmlNode*  xXmlDocument::root()
{
    if(m_ChildrenNodes.size() == 1)
		return m_ChildrenNodes[0];
	else if(m_ChildrenNodes.size() != 0)
	{
		XEVOL_LOG(eXL_DEBUG_NORMAL,"Multiple Root Node in the xXmlDocument\n");
		return NULL;
	}
	return NULL;
}

xXmlNode*  xXmlDocument::root(const wchar_t* name )
{
	if( m_ChildrenNodes.size() == 0)
		return this->insertNode(name);
	else
	{
		XEVOL_LOG(eXL_DEBUG_NORMAL,"Root Node already exist\n");
		xXmlNode* pRoot = root();
		if(pRoot)
		{
			pRoot->setName(name);
		}
		return pRoot;
	}
	
}





template <typename _TOutStream > void saveToStream(bool bSaveAsXML , _TOutStream& stream , xXmlNode* pNode,int level)
{
	xXmlNode::XmlValues values;

	for(int l  = 0 ; l < (level) * 4 ; l++)
		stream<< L" ";

	if(bSaveAsXML == false && pNode->countCData() == 0)
	{
		stream<< L"<"<<pNode->name()<< L">";
		stream.endl();

		pNode->findValue(values);
		std::sort(values.begin(), values.end());
		int nValues = (int)values.size();
		for(int i = 0 ; i <  nValues;  i ++)
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

		xXmlNode::XmlNodes nodes;
		pNode->findNode(nodes);
		int nNodes = (int)nodes.size() ;
		for(int i = 0 ; i < nNodes ; i ++)
		{
			saveToStream(bSaveAsXML, stream,nodes[i] , level + 1);
		}

		for(int l  = 0 ; l < (level) * 4 ; l++)
			stream<<L" ";
		stream<< L"</"<<pNode->name()<< L">";
		stream.endl();
	}

	else//save as xml
	{
		stream<<L"<"<<pNode->name()<<L"";

		pNode->findValue(values);
		std::sort(values.begin(), values.end());
		int nValues = (int)values.size();

		for(int i = 0 ; i <  nValues;  i ++)
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

		//保存子节点
		xXmlNode::XmlNodes nodes;
		pNode->findNode(nodes);

		if(nodes.size() == 0 && pNode->countCData() == 0)
		{
			stream<< L"/>";
			stream.endl();
		}
		else
		{
			stream<< L">";

			//保存CDATA数据
			for(int i = 0 ; i < pNode->countCData() ; i ++)
			{
				xXmlCData* pCData = pNode->findCData(i);
				if( !pCData->bRawData() )
				{
					stream<<L"<![CDATA[";
				}
				stream<<pCData->value();
				if( !pCData->bRawData() )
				{
					stream<<L"]]>";
				}
			}

			//如果没有子节点，就不用换行
			if(nodes.size() > 0) 
				stream.endl();

			int nNodes = (int)nodes.size();
			for(int i = 0 ; i < nNodes ; i ++)
			{
				saveToStream(bSaveAsXML, stream , nodes[i] , level + 1);
			}

			//如果没有子节点，就不用换行
			if(nodes.size() > 0) 
			{
				for(int l  = 0 ; l < (level) * 4 ; l++)
					stream<< L" ";
			}

			stream<< L"</" <<pNode->name()<< L">";
			stream.endl();
		}
	}
}


bool xXmlDocument::save(std::ostream& _stream, bool saveHeader , eTextEncode _encode)
{	
	if(_encode == eText_Unicode) 
	{
		xXmlUnicodeStream<std::ostream> stream(&_stream,true);

		if(saveHeader)
		{
			stream<< L"<?xml version=\"1.0\" encoding=\"unicode\"?>";
			stream.endl();
		}
		int nChildNodes = (int)m_ChildrenNodes.size();
		for(int i = 0 ; i <  nChildNodes; i ++)
		{
			saveToStream(m_saveAsXML , stream , m_ChildrenNodes[i] , 0);
		}
		return true;
	}
	
	if(_encode == eText_Utf8) 
	{
		xXmlUtf8Stream<std::ostream> stream(&_stream,true);

		if(saveHeader)
		{
			stream<< L"<?xml version=\"1.0\" encoding=\"utf-8\"?>";
			stream.endl();
		}
		int nChildNodes = (int)m_ChildrenNodes.size();
		for(int i = 0 ; i <  nChildNodes; i ++)
		{
			saveToStream(m_saveAsXML , stream , m_ChildrenNodes[i] , 0);
		}
		return true;
	}

	if(_encode == eText_Ansi) 
	{
		xXmlAnsiStream<std::ostream> stream(&_stream,true);

		if(saveHeader)
		{
			stream<< L"<?xml version=\"1.0\" encoding=\"ansi\"?>";
			stream.endl();
		}
		int nChildNodes = (int)m_ChildrenNodes.size();
		for(int i = 0 ; i <  nChildNodes; i ++)
		{
			saveToStream(m_saveAsXML , stream , m_ChildrenNodes[i] , 0);
		}
		return true;
	}
	return false;
}

bool xXmlDocument::save(IStreamIO* _stream, bool saveHeader , eTextEncode _encode)
{	
    if(_encode == eText_Unicode) 
    {
        xXmlUnicodeStream<IStreamIO> stream(_stream,true);

        if(saveHeader)
        {
            stream<< L"<?xml version=\"1.0\" encoding=\"unicode\"?>";
            stream.endl();
        }
        int nChildNodes = (int)m_ChildrenNodes.size();
        for(int i = 0 ; i <  nChildNodes; i ++)
        {
            saveToStream(m_saveAsXML , stream , m_ChildrenNodes[i] , 0);
        }
        return true;
    }

    if(_encode == eText_Utf8) 
    {
        xXmlUtf8Stream<IStreamIO> stream(_stream,true);

        if(saveHeader)
        {
            stream<< L"<?xml version=\"1.0\" encoding=\"utf-8\"?>";
            stream.endl();
        }
        int nChildNodes = (int)m_ChildrenNodes.size();
        for(int i = 0 ; i <  nChildNodes; i ++)
        {
            saveToStream(m_saveAsXML , stream , m_ChildrenNodes[i] , 0);
        }
        return true;
    }

    if(_encode == eText_Ansi) 
    {
        xXmlAnsiStream<IStreamIO> stream(_stream,true);

        if(saveHeader)
        {
            stream<< L"<?xml version=\"1.0\" encoding=\"ansi\"?>";
            stream.endl();
        }
        int nChildNodes = (int)m_ChildrenNodes.size();
        for(int i = 0 ; i <  nChildNodes; i ++)
        {
            saveToStream(m_saveAsXML , stream , m_ChildrenNodes[i] , 0);
        }
        return true;
    }
    return false;
}

bool xXmlDocument::save(std::wostringstream& _stream , bool saveHeader)
{
	xXmlUnicodeStream<std::wostringstream> stream(&_stream,true);

	if(saveHeader)
	{
		stream<< L"<?xml version=\"1.0\" encoding=\"unicode\"?>";
		stream.endl();
	}

	int nChildNodes = (int)m_ChildrenNodes.size();
	for(int i = 0 ; i <  nChildNodes; i ++)
	{
		saveToStream(m_saveAsXML , stream , m_ChildrenNodes[i] , 0);
	}
	return true;
}
END_NAMESPACE_XEVOL3D

