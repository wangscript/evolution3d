#ifndef __XCFG_UNICODE_BUILDER_H__
#define __XCFG_UNICODE_BUILDER_H__
#include "xEvol3DBaseInc.h"
#include "xBaseLexer.h"
BEGIN_NAMESPACE_XEVOL3D
class  _XEVOL_BASE_API_   xXmlDocument;
class  _XEVOL_BASE_API_   xXmlNode;
class  _XEVOL_BASE_API_   xXmlValue;


enum 
{
	UTF8_LEAD_0   = 0xefU,
    UTF8_LEAD_1   = 0xbbU,
    UTF8_LEAD_2   = 0xbfU,
    UTF16_LEAD_0 = 0xFF,
    UTF16_LEAD_1 = 0xFE
};

//==============================================================
//xCfg Lexer
//==============================================================
class xXmlFileSource
{
	bool           m_useMicrosoftBOM;
	wchar_t*       m_Buffer;
	wchar_t*       m_pData;
	bool           m_bNeedDelete;
	
    wchar_t*       loadUCS2FileBuffer(const char* ucs16Buff , int len, bool& bConverted);
public :
	~xXmlFileSource();
	xXmlFileSource();
	bool            setSrc(const wchar_t* pTex);
	bool            loadStream(std::istream& stream);
	wchar_t*        getStr()   { return m_pData;   }
	bool            loadFile(const wchar_t* file);
	bool            loadMem(void* buf, int len);
	
};

class  xXmlLexer : public xBaseLexer<wchar_t, std::wstring>
{
	int  __skipComment(const wchar_t* p);
public:
	int  skipComment(const wchar_t* p);
	int  GetXMLStyleChar(const wchar_t* p, std::wstring& text );
	int  getString(const wchar_t* p, std::wstring& text, const wchar_t* endTag , FUNC_IS_ACCEPT func);
	int  getString(	const wchar_t* p, std::wstring& text , FUNC_IS_ACCEPT func);
};

class xXmlBuilder
{
public:
	static bool loadFile(xXmlDocument* pDocument , const wchar_t* _file,bool bLoadHeader);
	static bool loadMem(xXmlDocument* pDocument , const wchar_t* _pSrc ,bool bLoadHeader);
	static bool loadMem(xXmlDocument* pDocument , void* _pSrc ,unsigned int iDataLen ,bool bLoadHeader);
	static bool loadStream(xXmlDocument* pDocument ,std::istream& stream ,bool bLoadHeader);
};

END_NAMESPACE_XEVOL3D
#endif

