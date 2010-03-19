#include "stdafx.h"
#include "xD3D9FileTexture.h"
#include "xDirect3D9API.h"
#include "xD3D9ConstLexer.h"
#include <BaseLib/xI18N.h>
#include <BaseLib/xBaseMath.h>
#include <Image/xImage.h>
#include <fs/xFileSystem.h>
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xD3D9FileTexture  , IBaseTexture);

xD3D9FileTexture::xD3D9FileTexture(xD3D9RenderApi* pAPI , const xTextureInitDesc* pDesc )
:xD3D9BaseTexture(pAPI)
{
    m_InitDesc.m_bReadable = false;
    m_InitDesc.m_bindType  = BIND_AS_SHADER_RESOURCE;
    m_InitDesc.m_access    = 0 ;
    m_InitDesc.m_usage     = RESOURCE_USAGE_DEFAULT;
    if(pDesc)
    {
        m_InitDesc = *pDesc;
    }

}

xD3D9FileTexture::~xD3D9FileTexture()
{
   unload();
}

void  xD3D9FileTexture::onDeviceReset(bool bNewDevice , ID3D9DeviceLostListener::eResetAction _Action)
{
    //if(_Action == ID3D9DeviceLostListener::eReset_Begin)
    //     XSAFE_RELEASE(m_pTexture);
}

bool xD3D9FileTexture::fillLoadInfo(D3DX_IMAGE_LOAD_INFO& loadInfo )
{
	ZeroMemory( &loadInfo, sizeof(D3DX_IMAGE_LOAD_INFO) );
    loadInfo.Width     = D3DX_DEFAULT;
    loadInfo.Height    = D3DX_DEFAULT;
    loadInfo.Depth     = D3DX_DEFAULT;
    loadInfo.MipLevels = D3DX_DEFAULT;
    loadInfo.Usage     = xD3D9ConstLexer::singleton()->GetD3DUsage(m_InitDesc.m_access , m_InitDesc.m_bindType , m_InitDesc.m_usage );
    loadInfo.Format    = D3DFMT_FROM_FILE;;
    loadInfo.Pool      = xD3D9ConstLexer::singleton()->GetD3DPool( m_InitDesc.m_usage );
    RESET_D3DPOOL(loadInfo.Pool);
    loadInfo.Filter    = D3DX_DEFAULT ;
    loadInfo.MipFilter = D3DX_DEFAULT;
    loadInfo.ColorKey  = 0xFF000000;
	return true;
}

bool xD3D9FileTexture::__loadImageFile(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	xImage* pImg = xImageLib::load(IMAGE_UNKNOWN,fileName , (const int8*)buf , (int)bufLen , PIXELFORMAT_R8G8B8A8 );
	if( pImg == NULL )
		return false;

	pImg->flipY();
	xImageSize _size = pImg->getSize();
	int pitch = _size.pitch;

	//1个图像.那么就是2D纹理
	if(pImg->nImage() == 1)
	{
        DWORD usage = xD3D9ConstLexer::singleton()->GetD3DUsage(m_InitDesc.m_access , m_InitDesc.m_bindType , m_InitDesc.m_usage );
        D3DPOOL pool = xD3D9ConstLexer::singleton()->GetD3DPool( m_InitDesc.m_usage ); 
        RESET_D3DPOOL(pool);
        IDirect3DTexture9* p2DTexture = NULL;
        if(FAILED(m_pD3D9RenderApi->getDevice()->CreateTexture((UINT)_size.w, (UINT)_size.h,1, usage , D3DFMT_A8B8G8R8, pool , &p2DTexture,0)) )
            return false;
        m_pTexture = p2DTexture;
        const char* pData[1] = { (const char*)pImg->data(0)};
        uploadTextureData(pData , _size.pitch , _size.pitch *  _size.h , 0 , false);
		if(m_pTexture == NULL)
			return false;
		return _load(m_pTexture);
	}
	else if(pImg->nImage() == 6)
	{
        XEVOL_WARNNING_NOT_IMPLEMENT_INFO("CubeMap目前还不支持dds以外格式\n");
	}
	return true;
}



bool xD3D9FileTexture::_prepareLoadInfo(D3DXIMAGE_INFO& imgInfo , D3DX_IMAGE_LOAD_INFO& loadInfo)
{
	xD3D9GIFormatInfo* fmtInfo = xD3D9ConstLexer::singleton()->GetPixelFormat(imgInfo.Format);
	fillLoadInfo(loadInfo);
	if(fmtInfo && fmtInfo->m_CompressRate != 1)
	{
		loadInfo.Usage = loadInfo.Usage  &(~ D3DUSAGE_RENDERTARGET );
	}
	loadInfo.Width     = imgInfo.Width;
	loadInfo.Height    = imgInfo.Height;
    loadInfo.Depth     = imgInfo.Depth;
	loadInfo.MipLevels = imgInfo.MipLevels;
	loadInfo.Format    = imgInfo.Format;
	return true;
}

bool xD3D9FileTexture::load(const wchar_t* fileName , unsigned long  arg)
{
	FILE* fp = _wfopen(fileName , L"rb");
    if(fp == NULL)
        return false;

    fseek(fp , 0 , SEEK_END);
    int len = ftell(fp);
    fseek(fp , 0 , SEEK_SET);
    unsigned char* fBuf = new unsigned char[len];
    fread(fBuf , 1 , len , fp);
    fclose(fp);
    return load(fileName , fBuf , len , arg);
}

bool xD3D9FileTexture::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(bufLen == 0 || buf == NULL)
		return load(fileName , arg);

	HRESULT hr = E_FAIL;
	D3DXIMAGE_INFO      imgInfo;
	std::wstring nameExt = xFileSystem::singleton()->getFileExtName(fileName);
	//if(nameExt != L"jpeg" && nameExt != L"jpg")
	{
		hr = D3DXGetImageInfoFromFileInMemory(buf , (UINT)bufLen ,&imgInfo);
	}
	if(! FAILED(hr) )
	{
		D3DX_IMAGE_LOAD_INFO loadInfo;
		if(_prepareLoadInfo(imgInfo , loadInfo) == false)
			return false;
        if(imgInfo.ResourceType == D3DRTYPE_TEXTURE)
        {
            IDirect3DTexture9* p2DTexture = NULL;
            D3DXCreateTextureFromFileInMemoryEx(m_pD3D9RenderApi->getDevice() , buf , bufLen , loadInfo.Width , loadInfo.Height , loadInfo.MipLevels , 
                loadInfo.Usage , loadInfo.Format , loadInfo.Pool , loadInfo.Filter , loadInfo.MipFilter , loadInfo.ColorKey ,  &imgInfo , NULL , &p2DTexture );
            m_pTexture = p2DTexture;
        }
        else if(imgInfo.ResourceType == D3DRTYPE_CUBETEXTURE)
        {
            IDirect3DCubeTexture9* pCubeTexture = NULL;
            D3DXCreateCubeTextureFromFileInMemoryEx(m_pD3D9RenderApi->getDevice() , buf , bufLen , loadInfo.Width , loadInfo.MipLevels , 
                loadInfo.Usage , loadInfo.Format , loadInfo.Pool , loadInfo.Filter , loadInfo.MipFilter , loadInfo.ColorKey ,  &imgInfo , NULL , &pCubeTexture);
            m_pTexture = pCubeTexture;
        }
        else if(imgInfo.ResourceType == D3DRTYPE_VOLUMETEXTURE)
        {
            IDirect3DVolumeTexture9* pVolumeTexture = NULL;
            D3DXCreateVolumeTextureFromFileInMemoryEx(m_pD3D9RenderApi->getDevice() , buf , bufLen , loadInfo.Width , loadInfo.Height , loadInfo.Depth , loadInfo.MipLevels , 
                loadInfo.Usage , loadInfo.Format , loadInfo.Pool , loadInfo.Filter , loadInfo.MipFilter , loadInfo.ColorKey ,  &imgInfo , NULL , &pVolumeTexture );
            m_pTexture = pVolumeTexture;
        }

	}

	if(m_pTexture)
    {
        return _load(m_pTexture);
    }
	return __loadImageFile(fileName , buf , bufLen ,arg);
}




END_NAMESPACE_XEVOL3D