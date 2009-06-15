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

#ifndef __xXmlParser_H__
#define __xXmlParser_H__

#include "xEvol3DBaseInc.h"
#include "stl_dllalloc.h"
#include <string>
#include <vector>
#include <istream>
#include <ostream>
#include <sstream>
#include <map>
BEGIN_NAMESPACE_XEVOL3D



class  _XEVOL_BASE_API_   xXmlDocument;
class  _XEVOL_BASE_API_   xXmlNode;
class  _XEVOL_BASE_API_   xXmlValue;

//============================================================
class  _XEVOL_BASE_API_  xXmlAttribute
{
public:
	const wchar_t* value() const;
	void           setValue(const wchar_t* newValue);
	void           setValue(int    newValue);
	void           setValue(size_t newValue);
	void           setValue(float newValue);
	void           setValue(bool newValue);
	void           setHex(unsigned int newValue);
	unsigned int   hex_value() const ;
	int            int_value() const;
	bool           bool_value() const;
	float          float_value() const;
protected:
	std::ds_wstring  m_Value;
};

class  _XEVOL_BASE_API_  xXmlValue : public xXmlAttribute
{
protected:
    std::ds_wstring  m_Name;
	short            m_Index;
public:
	xXmlValue(short i);
	void           setIndex(short i){m_Index = i;}
	const wchar_t* name() const;
	void           setName(const wchar_t* newName);
	friend bool operator < (const xXmlValue& lhv , const xXmlValue& rhv);
};

inline bool operator < (const xXmlValue& lhv , const xXmlValue& rhv)
{
	return lhv.m_Index < rhv.m_Index;
}

class _XEVOL_BASE_API_ xXmlCData : public xXmlAttribute
{
public:
	bool            m_rawData;
public:
	xXmlCData(bool bRawData = false);
	bool bRawData() const { return m_rawData ; }
};

class _XEVOL_BASE_API_  xXmlNode
{
public:
    typedef std::ds_wstring HASHVALUE;
public:
	typedef ds_map(HASHVALUE, xXmlValue*)         XmlValueMap;
	typedef ds_vector(xXmlNode*)                  XmlNodes   ;
	typedef ds_vector(xXmlCData*)                 xXmlCDatas   ;
	typedef ds_vector(xXmlValue*)                 XmlValues  ;

protected:
	XmlValueMap                        m_Values;
	XmlNodes                           m_ChildrenNodes;
	xXmlCDatas                         m_CDatas;
	xXmlNode*                          m_pParant;
	std::ds_wstring                    m_Name;
protected:
	xXmlNode(const xXmlNode& );
	xXmlNode* operator = (const xXmlNode& );
public:
	~xXmlNode();
	xXmlNode();
public:
	void           setName(const wchar_t* newName);
	const wchar_t* name();
	xXmlNode*      parent();
	xXmlNode*      insertNode(const wchar_t* nodeName);
	xXmlNode*      insertNodeAfter(const wchar_t* nodeName,size_t index);
	xXmlNode*      insertNodeBefore(const wchar_t* nodeName,size_t index);
	xXmlNode*      findNode(const wchar_t* nodeName, int index = 0 )  const ;
	int            findNode(const wchar_t* nodeName,XmlNodes& nodes)  const ;
	int            findNode(XmlNodes& nodes) const ;
	xXmlNode*      findNode(int index)  const ;
	int            countNode(const wchar_t* nodeName)  const ;
	int            countNode()  const ;
    int            countCData() const ;
	xXmlCData*     findCData(int index)  const ;

	xXmlValue*     findValue(const wchar_t* valueName)  const ;
	int            findValue(XmlValues& values)  const ;
	xXmlValue*     insertValue(const wchar_t* valueName) ;
	xXmlCData*     insertCData(bool bRawData = false) ;


	void           setValue(const wchar_t* nodeName , const wchar_t* valueName, size_t  newValue        , int index = 0 );
	void           setValue(const wchar_t* nodeName , const wchar_t* valueName, int     newValue        , int index = 0 );
	void           setHex(const wchar_t* nodeName   , const wchar_t* valueName, unsigned int newValue   , int index = 0 );
	void           setValue(const wchar_t* nodeName , const wchar_t* valueName, float   newValue        , int index = 0 );
	void           setValue(const wchar_t* nodeName , const wchar_t* valueName, bool    newValue        , int index = 0 );
	void           setValue(const wchar_t* nodeName , const wchar_t* valueName, const wchar_t* value     , int index = 0);

	unsigned int   hex_value(const wchar_t* nodeName, const wchar_t* valueName, int index = 0 ) const ;
	int            int_value(const wchar_t* nodeName, const wchar_t* valueName, int index = 0 ) const ;
	bool           bool_value(const wchar_t* nodeName , const wchar_t* valueName, int index = 0 ) const ;
	float          float_value(const wchar_t* nodeName, const wchar_t* valueName, int index = 0 ) const ;
	const wchar_t* value(const wchar_t* nodeName,const wchar_t* valueName, int index = 0) const;

    unsigned int   value(const wchar_t* nodeName , const wchar_t* valueName, const unsigned int defValue ,int index = 0 );
    int            value(const wchar_t* nodeName , const wchar_t* valueName, const int          defValue ,int index = 0 );
    bool           value(const wchar_t* nodeName , const wchar_t* valueName, const bool         defValue ,int index = 0 );
    float          value(const wchar_t* nodeName , const wchar_t* valueName, const float        defValue ,int index = 0 );
    const wchar_t* value(const wchar_t* nodeName , const wchar_t* valueName, const wchar_t*     defValue ,int index = 0 );

	void           setValue(const wchar_t* valueName, size_t newValue);
	void           setValue(const wchar_t* valueName, int    newValue);
	void           setHex(const wchar_t* valueName  , unsigned int newValue);
	void           setValue(const wchar_t* valueName, float  newValue);
	void           setValue(const wchar_t* valueName, bool   newValue);
	void           setValue(const wchar_t* valueName, const wchar_t* newValue);

	int            int_value(const wchar_t* valueName) const ;
	unsigned int   hex_value(const wchar_t* valueName) const ;
	bool           bool_value(const wchar_t* valueName ) const ;
	float          float_value(const wchar_t* valueName) const ;
	const wchar_t* value(const wchar_t* valueName) const ;
	void           clear();
};

class xXmlDocument : public xXmlNode
{
	xXmlDocument(const xXmlDocument& ){};
	xXmlDocument* operator =(const xXmlDocument& ){return NULL;};
	bool       m_saveAsXML; 
public:
	xXmlDocument();
	//bool       load(const char* fileName , bool loadHeader = true);
	bool       load(const wchar_t* fileName , bool loadHeader = true);
	bool       load(const wchar_t* buf , int len , bool loadHeader = true);
    bool       load(const int8*    buf , int len , bool loadHeader = true);
    bool       load(const wchar_t* buf , size_t len , bool loadHeader = true);
    bool       load(const int8*    buf , size_t len , bool loadHeader = true);
	bool       load(std::istream& stream , bool loadHeader = true);
	//bool       save(const char* fileName , bool saveHeader = true);
	bool       save(const wchar_t* fileName , bool saveHeader = true);
	bool       save(std::ostream& stream , bool saveHeader = true);
	bool       save(std::wostringstream& stream , bool saveHeader = true);
	xXmlNode*  root();
	xXmlNode*  root(const wchar_t* name );
	void       setXMLStyle(bool bXMLStyle);
	bool       isXMLStyle();
	void       unload();

private:
	void __save(std::ostream& stream , xXmlNode* pNode, int level);
	void __save(std::wostringstream& stream , xXmlNode* pNode, int level);
};
END_NAMESPACE_XEVOL3D
#endif

