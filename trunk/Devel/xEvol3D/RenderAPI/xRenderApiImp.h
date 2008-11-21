#ifndef _XEVOL_RENDER_API_IMPL_H_
#define _XEVOL_RENDER_API_IMPL_H_
#include "xRenderAPI.h"
BEGIN_NAMESPACE_XEVOL3D

class _XEVOL_BASE_API_  xRenderApiImp : public IRenderApi
{

public:
	xRenderApiImp();
	virtual ~xRenderApiImp();

	xFontManager*                 fontManager() ;
	HFontRender                   findFont(const wchar_t* family , const wchar_t* fontName);
	HFontRender                   findFont(const wchar_t* fontName);
	IFontRenderDevice*            fontRenderDevice();
protected:
	void                          __updateTextureLoaders();
	xFontManager                  m_FontManager;
	IFontRenderDevice*            m_FontRenderDevice;

};


class xTextureLdrMgrProxy
{
public:
	typedef std::vector<ITextureLoader*>               vTextureLoaders;
	typedef std::map<std::ds_wstring, ITextureLoader*> TextureLoaderMap;
public:
	ITextureLoader*               findTextureLoader(const wchar_t* texExt);
	void                          addTextureLoader(ITextureLoader* pTextureLoader);
	void                          removeTextureLoader(ITextureLoader* pTextureLoader);
protected:
	void                          __updateTextureLoaders();
	vTextureLoaders               m_TextureLoaders;
	TextureLoaderMap              m_TextureLoaderMap;
};
END_NAMESPACE_XEVOL3D
#endif