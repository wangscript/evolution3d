#include "../xStdPch.h"
#include "xStringTable.h"
#include "xI18N.h"
BEGIN_NAMESPACE_XEVOL3D

void xStringTable::addPath(const wchar_t* _path)
{
	size_t nPath = m_paths.size();
	for(size_t i = 0 ; i <  nPath; i ++)
	{
		if(m_paths[i] == _path)
			return ;
	}
    m_paths.push_back(_path);
}

void xStringTable::clearPath()
{
	m_paths.clear();
}

bool xStringTable::setLanguage(const wchar_t* lang)
{
	std::ds_wstring oldlang = m_strLang;
	if(lang == NULL)
	{
		m_strLang = L"";
	}
	else
	{
		m_strLang = lang;
	}

   if(oldlang == m_strLang && m_strMap.size() != 0)
	   return false ;

    xXmlDocument doc;

	std::ds_wstring langFile = m_strLang;
	if(m_strLang.size() <= 2)
	{
		xI18N i18N;
		langFile = i18N.getCurrentLocaleInfor()->_locale;
	}
    size_t nPaths = m_paths.size();
	for(size_t i = 0 ; i < nPaths ; i ++)
	{
		std::ds_wstring fullFileName = m_paths[i] +  langFile + L".xml";
		if(doc.load(fullFileName.c_str()) == true)
		{
			if( doc.root() )
			{
				load(doc.root()->findNode( L"strings") );
			}
		}
		doc.unload();
	}

	return false;
}

xStringTable*  xStringTable::singleton()
{
	static xStringTable g_table;
	return &g_table;
}

xStringTable::xStringTable()
{
	m_strLang = L"";
}
xStringTable::~xStringTable()
{
	clear();

}

bool xStringTable::load(xXmlNode* pTable)
{
	xXmlNode::XmlNodes nodes ;
	pTable->findNode(L"string" , nodes);
	size_t nNodes = nodes.size();
	for(size_t i = 0 ; i <  nNodes; i ++)
	{
		xXmlNode* pStringNode = nodes[i];
		const wchar_t* key = pStringNode->value(L"key");
		const wchar_t* val = pStringNode->value(L"value");
		addString(key,val);
	}
	return m_strMap.size() > 0 ;
}
bool xStringTable::addString(const wchar_t* _key , const wchar_t* _val)
{
	if(_key == NULL || _val == NULL)
		return false;
	vStringMaps::iterator pos = m_strMap.find(_key);
	if( pos != m_strMap.end() )
		return true;
	m_strMap[_key ] = _val;
	return true;
}
const wchar_t*  xStringTable::trans(const wchar_t* _key)
{
	if(_key == NULL)
		return L"string not found";
	vStringMaps::iterator pos = m_strMap.find(_key);
	if( pos == m_strMap.end() )
		return _key;
	return pos->second.c_str();
}

void  xStringTable::clear()
{
	m_strMap.clear();
}

END_NAMESPACE_XEVOL3D