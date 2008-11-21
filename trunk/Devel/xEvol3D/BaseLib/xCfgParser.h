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

#ifndef __xCfgParser_H__
#define __xCfgParser_H__

#include "xEvol3DBaseInc.h"
#include "stl_dllalloc.h"
#include <string>
#include <vector>
#include <istream>
#include <ostream>
#include <map>
BEGIN_NAMESPACE_XEVOL3D

typedef std::ds_wstring HASHVALUE;
enum eEncFormat
{
	ENC_ASCII,
	ENC_UTF8,
	ENC_UNICODE
};

class  _XEVOL_BASE_API_   xCfgDocument;
class  _XEVOL_BASE_API_   xCfgNode;
class  _XEVOL_BASE_API_   xCfgValue;

//============================================================
class  _XEVOL_BASE_API_  xCfgValue
{
protected:

	std::ds_wstring  m_Value;
	std::ds_wstring  m_Name;
	short            m_Index;
public:
	xCfgValue(short i);
	void           setIndex(short i){m_Index = i;}
	const wchar_t* name() const;
	void           setName(const wchar_t* newName);

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
	friend bool operator < (const xCfgValue& lhv , const xCfgValue& rhv);
};

inline bool operator < (const xCfgValue& lhv , const xCfgValue& rhv)
{
	return lhv.m_Index < rhv.m_Index;
}

class _XEVOL_BASE_API_  xCfgNode
{
public:
	typedef ds_map(HASHVALUE, xCfgValue*)         CfgValueMap;
	typedef ds_vector(xCfgNode*)                  CfgNodes   ;
	typedef ds_vector(xCfgValue*)                 CfgValues  ;

protected:
	CfgValueMap                        m_Values;
	CfgNodes                           m_ChildrenNodes;
	xCfgNode*                          m_pParant;
	std::ds_wstring                    m_Name;
protected:
	xCfgNode(const xCfgNode& );
	xCfgNode* operator = (const xCfgNode& );
public:
	~xCfgNode();
	xCfgNode();
public:
	void           setName(const wchar_t* newName);
	const wchar_t* name();
	xCfgNode*      parent();
	xCfgNode*      insertNode(const wchar_t* nodeName);
	xCfgNode*      insertNodeAfter(const wchar_t* nodeName,size_t index);
	xCfgNode*      insertNodeBefore(const wchar_t* nodeName,size_t index);
	xCfgNode*      findNode(const wchar_t* nodeName, int index = 0 )  const ;
	int            findNode(const wchar_t* nodeName,CfgNodes& nodes)  const ;
	int            findNode(CfgNodes& nodes) const ;
	xCfgNode*      findNode(int index)  const ;
	int            countNode(const wchar_t* nodeName)  const ;
	int            countNode()  const ;

	xCfgValue*     findValue(const wchar_t* valueName)  const ;
	int            findValue(CfgValues& values)  const ;
	xCfgValue*     insertValue(const wchar_t* valueName) ;


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

class xCfgDocument : public xCfgNode
{
	eEncFormat m_encoding;
	xCfgDocument(const xCfgDocument& ){};
	xCfgDocument* operator =(const xCfgDocument& ){return NULL;};
	bool       m_saveAsXML; 
public:
	xCfgDocument();
	//bool       load(const char* fileName , bool loadHeader = true);
	bool       load(const wchar_t* fileName , bool loadHeader = true);
	bool       load(const wchar_t* buf , int len , bool loadHeader = true);
	bool       load(const int8*    buf , int len , bool loadHeader = true);
	bool       load(std::istream& stream , bool loadHeader = true);
	//bool       save(const char* fileName , bool saveHeader = true);
	bool       save(const wchar_t* fileName , bool saveHeader = true);
	bool       save(std::ostream& stream , bool saveHeader = true);
	xCfgNode*  root();
	xCfgNode*  root(const wchar_t* name );
	void       setEncoding(eEncFormat encoding){m_encoding = encoding;}
	eEncFormat getEncoding(){return m_encoding;}
	void       setXMLStyle(bool bXMLStyle);
	bool       isXMLStyle();
	void       unload();

public:
	void __save(std::ostream& stream , xCfgNode* pNode, int level);
};
END_NAMESPACE_XEVOL3D
#endif

