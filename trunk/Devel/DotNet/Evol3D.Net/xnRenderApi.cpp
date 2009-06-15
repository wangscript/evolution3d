// This is the main DLL file.

#include "stdafx.h"
#include <vcclr.h>
#include "Evol3D.Net.h"
#include "xnRenderApi.h"
#include "xnMath.h"
namespace xEvol3DNet 
{
	//=========Font======================================
	xFont::xFont(HFontRender pFontRender)
	{
		m_pRendeDevice = new HFontRender;
		*m_pRendeDevice = pFontRender;
	}
	xFont::~xFont()
	{
		delete m_pRendeDevice;
	}

	void  xFont::releaseChache()
	{
		m_pRendeDevice->getResource()->releaseChache();
	}

	bool  xFont::getCharDim(wchar_t _chr , int^ w , int^ h)
	{
		int _w  = 0;
		int _h  = 0;
		bool ret = m_pRendeDevice->getResource()->getCharDim(_chr , _w , _h);
		w = _w ; h = _h ;
		return ret ;
	}

	bool  xFont::getTextDim(String^ text , int^ w , int^ h)
	{
		pin_ptr<const wchar_t> _text   = PtrToStringChars(text);
		int _w  = 0;
		int _h  = 0;
		bool ret = m_pRendeDevice->getResource()->getTextDim(_text , _w , _h);
		w = _w ; h = _h ;
		return ret ;
	}

	bool  xFont::getTextDim(String^ strText, int^ w , int^ h , int maxWidth)
	{
		pin_ptr<const wchar_t> _text   = PtrToStringChars(strText);
		int _w  = 0;
		int _h  = 0;
		bool ret = m_pRendeDevice->getResource()->getTextDim(_text , _w , _h , maxWidth);
		w = _w ; h = _h ;
		return ret ;
	}
	int	  xFont::getTextWidth(String^ strText, int iBegin, int^ w)
	{
		pin_ptr<const wchar_t> _text   = PtrToStringChars(strText);
		int _w  = 0;
		int ret = m_pRendeDevice->getResource()->getTextWidth(_text , iBegin , _w);
		w = _w ; 
		return ret ;
	}

	int	  xFont::getCharCount(String^ strText, int iBegin, int^ nMaxWidth, bool bRoundVal)
	{
		pin_ptr<const wchar_t> _text   = PtrToStringChars(strText);
		int _w  = 0;
		int ret = m_pRendeDevice->getResource()->getCharCount(_text , iBegin, _w, bRoundVal);
		nMaxWidth = _w ; 
		return ret ;
	}

	//公共函数
	bool  xFont::drawText(String^strText, float x , float y , int maxWidth , xColorF^ cl )
	{
		pin_ptr<const wchar_t> _text   = PtrToStringChars(strText);
		return m_pRendeDevice->getResource()->drawText(_text , x , y , maxWidth , *cl->m_cl);
	}

	bool xFont::drawText(String^ strText, float x , float y ,xColorF^ cl )
	{
		pin_ptr<const wchar_t> _text   = PtrToStringChars(strText);
		return m_pRendeDevice->getResource()->drawText(_text , x , y , *cl->m_cl);
	}

	bool xFont::drawText(String^ strText, float x , float y , int iBgnIdx, int nCount, xColorF^ cl, int nMaxW )
	{
		pin_ptr<const wchar_t> _text   = PtrToStringChars(strText);
		return m_pRendeDevice->getResource()->drawText(_text , x , y , iBgnIdx, nCount,   *cl->m_cl, nMaxW);
	}

	bool xFont::drawTextOneLine(String^ strText, xVector^ rect, xColorF^ cl, eTextAlign hAlignMode , eTextAlign vAlignMode)
	{
		pin_ptr<const wchar_t> _text   = PtrToStringChars(strText);
		return m_pRendeDevice->getResource()->drawTextOneLine(_text , *rect->m_vec,   *cl->m_cl, (xFontRender::eAlignMode)hAlignMode , (xFontRender::eAlignMode)vAlignMode);
	}


	//shader名字

	xGpuProgramName& xShaderProgramName::name()
	{
		return * m_pName;
	}

	xShaderProgramName::xShaderProgramName()
	{
		m_pName   = new xGpuProgramName;
		m_pParser = new xGpuProgNameParser;
	}
	xShaderProgramName::~xShaderProgramName()
	{
		delete m_pName;
		delete m_pParser;
	}

	void  xShaderProgramName::set(String^ vs , String^ ps , String^ gs)
	{
		m_pName->set( ToPtr(vs) ,  ToPtr(ps) , ToPtr(gs) );
	}

	void  xShaderProgramName::set(String^ _shaderName, bool bLoadFromeFile)
	{
		m_pName->set( ToPtr(_shaderName) , bLoadFromeFile );
	}

	bool  xShaderProgramName::setShaderName(eShaderType _shaderType , String^ _shaderName)
	{
		bool bRet = m_pParser->setShaderName( XEvol3D::eShaderType(_shaderType) , ToPtr(_shaderName) );
		m_pParser->toName(*m_pName);
		return bRet;
	}

	void  xShaderProgramName::addShaderNode(eShaderType _shaderType , String^ _nodeName, String^_socketName)
	{
		m_pParser->addShaderNode( XEvol3D::eShaderType(_shaderType) , ToPtr(_nodeName) , ToPtr(_socketName) );
		m_pParser->toName(*m_pName);
	}

	void  xShaderProgramName::addShaderNode(eShaderType _shaderType , String^ _nodeName, size_t _socketIdx)
	{
		m_pParser->addShaderNode( XEvol3D::eShaderType(_shaderType) , ToPtr(_nodeName) , _socketIdx );
		m_pParser->toName(*m_pName);
	}



	//渲染器
	xRenderApi::xRenderApi(IRenderApi*  pRenderApi)
	{
		m_pRenderApi = pRenderApi;
	}

	xRenderApi::xRenderApi(Int32  Handle)
	{
		m_pRenderApi = (IRenderApi*)Handle;
	}

	bool	xRenderApi::begin(float r , float g , float b , float a , float z , unsigned int stencil )
	{
		xColor_4f cl;
		cl.r = r ; cl.g = g ; cl.b = b; cl.a = a;
		return m_pRenderApi->begin(cl,z , stencil);
	}


	xBaseTextureManager::~xBaseTextureManager()
	{
		m_pTexMgr->clear();            
	}		

	xBaseTextureManager::xBaseTextureManager(xRenderApi^ _pApi , String^ name , bool bNonRefKeep)
	{
		IRenderApi* pApi = _pApi->m_pRenderApi;
		m_pTexMgr = pApi->createTextureManager(ToPtr(name) , true );
		m_Api = pApi;
	}

	void xBaseTextureManager::addPackage(String^ pkgName , String^ _dir)
	{
		m_pTexMgr->addPackage(ToPtr(pkgName) , ToPtr(_dir));
	}

	void xBaseTextureManager::addPath(String^ pathName)
	{
		m_pTexMgr->addPath(ToPtr(pathName));
	}    
	
}


