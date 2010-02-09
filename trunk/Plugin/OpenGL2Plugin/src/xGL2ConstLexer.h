#ifndef _XEVOL_OPENGL20_CONSTLEXER_H_
#define _XEVOL_OPENGL20_CONSTLEXER_H_
#include <RenderApi/xRenderApi.h>
#include "../GL2.h"
#include <string>
#include <vector>
#include <map>
BEGIN_NAMESPACE_XEVOL3D

struct xGL2GIFormatInfo
{
	ds_wstring    m_desc;
	ePIXEL_FORMAT m_fmt;

    GLenum    m_GLDataType;
    GLenum    m_GLDataFormat;
    int       m_nComponent;
    int       m_InternalFmt;
    GLenum    m_compressInFmt;
    int       m_bytePerComponent;

    xGL2GIFormatInfo()
    {
        m_GLDataType = 0;
        m_GLDataFormat = 0;
        m_nComponent = 0;
        m_InternalFmt = 0;
        m_bytePerComponent = 0;
        m_compressInFmt    = 0;
        m_fmt              = PIXELFORMAT_R8G8B8A8;
        m_desc             = L"";
    }

};

typedef std::map<int           , int> MapsDXGIToIndex;
typedef std::map<ePIXEL_FORMAT , int> MapsXEvolToIndex;
class xGL2ConstLexer
{
	std::vector<xGL2GIFormatInfo> m_vForamts;
	MapsDXGIToIndex               m_dxindex;
	MapsXEvolToIndex              m_fmtIndex;
protected:
	void __addFormat(ePIXEL_FORMAT fmt , GLenum GLDataType,GLenum GLDataFormat,int nComponent,int InternalFmt,int bytePerComponent  , const wchar_t* desc , GLenum compressedFmt = 0);
public:
	static xGL2ConstLexer* singleton();
    xGL2GIFormatInfo*      GetPixelFormat( GLenum innerfmt );
	xGL2GIFormatInfo*      GetPixelFormat( ePIXEL_FORMAT fmt );

	xGL2ConstLexer();
	~xGL2ConstLexer();
	
	static GLenum           GetMinFilter(const wchar_t* _filter_val , const wchar_t* _mip_val , bool bMagFilter);
	static GLuint           GetAdress(const wchar_t* _addressMode);
	static GLuint           GetCompareFunc(const wchar_t* _func);
	static int              GetTextureSlotIdx(const wchar_t* _texName);
	static GLuint           GetBlendFactor(const wchar_t* _blen);
	static GLuint           GetBlendOp(const wchar_t* _blen);
	static GLuint           GetFillMode(const wchar_t* _blen);
	static GLuint           GetCullMode(const wchar_t* _blen);
	static GLuint           GetStencilOp(const wchar_t* _op);
};
END_NAMESPACE_XEVOL3D

#endif

