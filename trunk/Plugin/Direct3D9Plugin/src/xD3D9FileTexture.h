#ifndef _XEVOL_DIRECT3D9_FILE_2DTEXTURE_H_
#define _XEVOL_DIRECT3D9_FILE_2DTEXTURE_H_
#include "xD3D9BaseTexture.h"
BEGIN_NAMESPACE_XEVOL3D
struct D3DX_IMAGE_LOAD_INFO 
{  
    UINT Width;
    UINT Height;
    UINT Depth;
    UINT MipLevels;
    DWORD Usage;
    D3DFORMAT Format;
    D3DPOOL Pool;
    DWORD Filter;
    DWORD MipFilter;
    D3DCOLOR ColorKey;

};


class xD3D9FileTexture : public xD3D9BaseTexture
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(xD3D9FileTexture);
	bool                  __loadImageFile(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
public:
	xD3D9FileTexture(xD3D9RenderApi* pAPI , const xTextureInitDesc* pDesc);
	~xD3D9FileTexture();
	virtual bool          load(const wchar_t* fileName , unsigned long  arg);
	virtual bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
public:
	virtual bool          create(const  xTextureInitDesc& initDesc , xTextureInitData* pInitData = NULL, int nInitData = 0) { return false ;}
protected:
	bool _prepareLoadInfo(D3DXIMAGE_INFO& imgInfo , D3DX_IMAGE_LOAD_INFO& loadInfo);
    bool fillLoadInfo(D3DX_IMAGE_LOAD_INFO& loadInfo );
    void onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action);
private:
    xTextureInitDesc m_InitDesc;
};


END_NAMESPACE_XEVOL3D

#endif

