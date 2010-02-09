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
#ifdef _WIN32
#include <windows.h>
#endif

#include "../RenderAPI/xRenderAPI.h"
#include "../RenderAPI/xBaseTexture.h"
#include "xFontManager.h"
#include "../OperationSys/xOperationSys.h"
#include "../BaseLib/xStringHash.h"
#include <vector>
#include <string>
using namespace std;


//系统路径
//$FontDir$
BEGIN_NAMESPACE_XEVOL3D

xFontLoader::xFontLoader()
{
	m_pRendeDevice = NULL;
	m_FontXML.insertNode(L"fontconfig");
	m_FontXML.root()->insertNode(L"fonts");
	m_FontXML.root()->insertNode(L"familly");
}

void xFontLoader::setFontRenderDevice(IFontRenderDevice* pDevice)
{
	m_pRendeDevice = pDevice;
}
IFontRenderDevice* xFontLoader::fontRenderDevice()
{
	return m_pRendeDevice ;
}

xFontLoader::~xFontLoader()
{
     clearFonts();
}

bool xFontLoader::load(const wchar_t* cfgFile)
{
	xXmlDocument fontCfgDoc;
	if( false == fontCfgDoc.load( cfgFile ) )
		return false;
	
	xXmlNode* font_cfg = fontCfgDoc.root();
	load(font_cfg);
	fontCfgDoc.unload();
	return true;
}

void xFontLoader::extractFonts( xXmlNode* _pNode,  xXmlNode* pDestRootNode ) 
{
	xXmlNode::XmlNodes  fontNodes;
	std::ds_wstring fullFontName;
	_pNode->findNode(L"font",fontNodes);
	int _nFontNodes = (int)fontNodes.size();
	for(int k = 0 ; k <  _nFontNodes; k ++)
	{
		xXmlNode* pFontNode = fontNodes[k];
		const wchar_t* fontName    = pFontNode->value(L"name");
		const wchar_t* famillyName = _pNode->value(L"name");
		std::ds_wstring fullFontName ;
		makeFontName(famillyName , fontName , fullFontName);
		xXmlNode* pDestNode = pDestRootNode->insertNode( pFontNode->name() );
		pFontNode->cloneTo(pDestNode);
		pDestNode->setValue(L"fullname" , fullFontName.c_str() );
        pDestNode->setValue(L"familly" , _pNode->value(L"name") );

        xXmlNode::XmlValues values;
        _pNode->findValue(values);
        size_t nValue = values.size();
        for(size_t iValue = 0 ; iValue < nValue ; iValue ++)
        {
            xXmlValue* pValue = values[iValue];
            const wchar_t* _valueName = pValue->name();
            const wchar_t* _value     = pValue->value();
            if(_valueName == L"name")
                continue;
            pDestNode->setValue(_valueName , _value);
        }


		int ftId = xStringHash(fullFontName.c_str() );
		if(m_FontNodeMap.find(ftId) != m_FontNodeMap.end() )
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"Font exist:  %s\n" , fullFontName);
		}
		m_FontNodeMap[ftId] = pDestNode;
	}

	fontNodes.clear();
	//Aias
	_pNode->findNode(L"alias",fontNodes);
	_nFontNodes = (int)fontNodes.size();
	for(int k = 0 ; k <  _nFontNodes; k ++)
	{
		xXmlNode* pFontNode = fontNodes[k];
		const wchar_t* fontName  = pFontNode->value(L"name");
		const wchar_t* famillyName = _pNode->value(L"name");
		std::ds_wstring fullFontName ;
		makeFontName(famillyName , fontName , fullFontName);

		xXmlNode* pDestNode = pDestRootNode->insertNode( pFontNode->name() );
		pFontNode->cloneTo(pDestNode);
		pDestNode->setValue(L"fullname" , fullFontName.c_str() );
		pDestNode->setValue(L"type"    , _pNode->value(L"type") );

		int ftId = xStringHash(fullFontName.c_str() );
		if(m_FontNodeMap.find(ftId) != m_FontNodeMap.end() )
		{
			XEVOL_LOG(eXL_DEBUG_HIGH , L"Font exist:  %s\n" , fullFontName);
		}
		m_FontNodeMap[ftId] = pDestNode;
	}
}

bool xFontLoader::load(xXmlNode* pXMLNode)
{
	//提取成程序更容易读懂的XML格式
    xXmlNode* pRootNode    =  m_FontXML.root()->findNode(L"fonts");
	xXmlNode* pFamRootNode =  m_FontXML.root()->findNode(L"familly"); 
	xXmlNode::XmlNodes fontTypes;
	pXMLNode->findNode(fontTypes );
	size_t nFontTypes = fontTypes.size();
	for(size_t i = 0 ; i <  nFontTypes; i ++)
	{
		xXmlNode* pFontTypeNode = fontTypes[i];

		//找出这个类型下的所有的Familly;
		xXmlNode::XmlNodes  familyNodes;
		pFontTypeNode->findNode(L"family",familyNodes);

		size_t nFamNodes =  familyNodes.size();
		for(size_t j = 0 ; j < nFamNodes; j ++)
		{
			extractFonts(familyNodes[j], pRootNode);

			xXmlNode* pDestFamillyNode = pFamRootNode->insertNode(familyNodes[j]->value(L"name"));
			familyNodes[j]->cloneTo(pDestFamillyNode , false);
		}
		//--------------------------------------------------
		//找出这个类型下的所有font;
		extractFonts(pFontTypeNode , pRootNode);
	}
	m_FontXML.save(L"C:\\Users\\stanlylee\\dump.xml" , true);
	return true;
}


HFontRender xFontLoader::findFont(const wchar_t* familly, const wchar_t* name, bool loadImm)
{
	std::ds_wstring fontFullName = std::ds_wstring(familly) + L":" + name;
	xFontManager* pFontMgr  = dynamic_cast<xFontManager*>(this);
	return pFontMgr->findFont( fontFullName.c_str() , loadImm);
}

HFontRender xFontLoader::findFont(const wchar_t* name , bool loadImm)
{
	xFontManager* pFontMgr  = dynamic_cast<xFontManager*>(this);
	return pFontMgr->add( name , 0 , loadImm);
}

bool xFontLoader::clearFonts()
{
	  xFontManager* pFontMgr  = dynamic_cast<xFontManager*>(this);
	  if(pFontMgr) pFontMgr->unload(  ); 
      m_FontNodeMap.clear();
	  m_FontXML.unload();
	  return true;
}

void xFontLoader::beginFontRender()
{

}
void xFontLoader::endFontRender()
{

}
/*
从一个配置文件的节点开始加载一组字体
配置文件结构为

<Root/or xXmlDocument>
    <family>
	   <font> </font>
    </family>
</Root>

*/
eFontFilter xFontLoader::getFontSamplerFilter(const wchar_t* filter_name)
{
	 if(filter_name == NULL)
		 return eFontFilter_Point;

	 wstring strFilter = filter_name;
     if(wstring(L"nearest") == strFilter || wstring(L"point") == strFilter)
	 {
		 return eFontFilter_Point;
	 }
	 else      if(wstring(L"linear") == strFilter)
	 {
		 return eFontFilter_Linear;
	 }
	 return eFontFilter_Linear;
}

void xFontLoader::splitFontName(const wchar_t* fullName , wchar_t* _font_familly , wchar_t* _font_name)
{
	wcsncpy(_font_familly,fullName,32);
	int iSplit = 0;
	for(iSplit  = 0 ; iSplit < (int)wcslen(fullName) ; ++iSplit)
	{
		if(_font_familly[iSplit] == ':')
		{
			_font_familly[iSplit] = 0;
			break;
		}
	}
	wcsncpy(_font_name,fullName + iSplit + 1,32);
}


//=================================================
//Resource Mananger

unsigned int xFontLoader::_getResSize(xFontRender* pRes)
{
    return pRes->getResSize();
}

bool         xFontLoader::_isResLoaded(xFontRender* pRes)
{
    return pRes != NULL;
}
void xFontLoader::makeFontName(const wchar_t* famillyName , const std::ds_wstring& fontName , std::ds_wstring& _outName)
{
 	if(famillyName == NULL || fontName.find(':') != std::ds_wstring::npos )
	{
		_outName = fontName;		
	}
	else
	{
		_outName = std::ds_wstring(  famillyName ) + L":" + fontName;
	}
}
bool         xFontLoader::_loadResource(const std::ds_wstring& strResName   , xFontRender* & pRes , int& ResSize, unsigned int arg)
{
	int fontId = xStringHash( strResName.c_str() );
	FontNodeMap::iterator pos = m_FontNodeMap.find(fontId);
	if(pos == m_FontNodeMap.end())
		return false;

	if(pRes)
	{
		pRes->ReleaseObject();
		pRes = NULL;
	}

	xXmlNode* pNode = pos->second;
	if( pNode->name() == std::wstring(L"alias") )
	{
		pRes = xFontRenderCreatorMgr::singleton()->createInstance(L"alias" , this , (int)arg);
		if(pRes->init(pNode) == false)
		{
			pRes->ReleaseObject();
			pRes = NULL;
		}
	}
	else
	{
        //从Type开始找起，一直找到type100
        for(int i = 1 ; i < 100 ; i ++)
        {
            wchar_t buffer[32] = {0};
            swprintf_x(buffer , 32 , L"type%d" , i);
            const wchar_t* fontType = pNode->value(buffer);
            if(i == 1)
            {
                fontType  = pNode->value(L"type");           
            }
            if(fontType == NULL) break;
            pRes = xFontRenderCreatorMgr::singleton()->createInstance(fontType , this , (int)arg);
            if(pRes)
                break;

        }

        if(pRes == NULL || pRes->init(pNode) == false)
		{
			XSAFE_RELEASEOBJECT(pRes);
		}
	}
	
    return pRes!=NULL;
}

bool         xFontLoader::_unloadResource(const std::ds_wstring& strResName , xFontRender* & pRes , unsigned int& TotalResSize)
{
	if(pRes)
	{
		TotalResSize -= _getResSize(pRes);
		pRes->ReleaseObject();
		pRes = NULL;
	}

    return false;
}

void         xFontLoader::_deleteResource(const std::ds_wstring& strResName , xFontRender* pRes)
{
	if(pRes)
	{
		pRes->ReleaseObject();
		pRes = NULL;
	}
}

END_NAMESPACE_XEVOL3D
