#ifndef __XCFG_UNICODE_BUILDER_H__
#define __XCFG_UNICODE_BUILDER_H__
#include "xEvol3DBaseInc.h"
#include "xBaseLexer.h"
BEGIN_NAMESPACE_XEVOL3D
class  _XEVOL_BASE_API_   xCfgDocument;
class  _XEVOL_BASE_API_   xCfgNode;
class  _XEVOL_BASE_API_   xCfgValue;

//==============================================================
//xCfg Lexer
//==============================================================
class xCfgFileSource
{
	bool           m_useMicrosoftBOM;
	wchar_t*       m_Buffer;
	wchar_t*       m_pData;
	bool           m_bNeedDelete;
	eEncFormat     m_encfmt;
	
    wchar_t*       loadUCS2FileBuffer(const char* ucs16Buff , int len, bool& bNeedDeleteUCS16Buffer);
public :
	~xCfgFileSource();
	xCfgFileSource();
	bool            setSrc(const wchar_t* pTex);
	bool            loadStream(std::istream& stream);
	eEncFormat      encoding(){ return m_encfmt;}
	wchar_t*        getStr()   { return m_pData;   }
	bool            loadFile(const wchar_t* file);
	bool            loadMem(void* buf, int len);
	
};

class  xCfgLexer : public xBaseLexer<wchar_t, std::wstring>
{
	int  __skipComment(const wchar_t* p);
public:
	int  skipComment(const wchar_t* p);
};

class xCfgBuilder
{
public:
	static bool loadFile(xCfgDocument* pDocument , const wchar_t* _file,bool bLoadHeader);
	static bool loadMem(xCfgDocument* pDocument , const wchar_t* _pSrc ,bool bLoadHeader);
	static bool loadMem(xCfgDocument* pDocument , void* _pSrc ,unsigned int iDataLen ,bool bLoadHeader);
	static bool loadStream(xCfgDocument* pDocument ,std::istream& stream ,bool bLoadHeader);
};

END_NAMESPACE_XEVOL3D
#endif

