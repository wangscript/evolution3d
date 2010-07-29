#include "stdafx.h"
#include "xD10FileTexture.h"
#include "xDirect3D10API.h"
#include "xD10ConstLexer.h"
#include <BaseLib/xI18N.h>
#include <BaseLib/xBaseMath.h>
#include <Image/xImage.h>
#include <fs/xFileSystem.h>
#include <D3Dx9.h>
BEGIN_NAMESPACE_XEVOL3D;

IMPL_BASE_OBJECT_CLASSID(xD10FileTexture  , IBaseTexture);

xD10FileTexture::xD10FileTexture(xD3D10RenderApi* pAPI )
    :xD10UnkwonTexture(pAPI)
{

}

xD10FileTexture::~xD10FileTexture()
{
    unload();
}




bool fillLoadInfo(D3DX10_IMAGE_LOAD_INFO& loadInfo , bool lockAble , bool bAsRenderTarget)
{
    ZeroMemory( &loadInfo, sizeof(D3DX10_IMAGE_LOAD_INFO) );
    loadInfo.BindFlags      = D3D10_BIND_SHADER_RESOURCE ;//D3D10_BIND_SHADER_RESOURCE;
    if(bAsRenderTarget)
    {
        loadInfo.BindFlags = loadInfo.BindFlags | D3D10_BIND_RENDER_TARGET ;
    }
    loadInfo.CpuAccessFlags = lockAble? D3D10_CPU_ACCESS_WRITE : 0 ;
    loadInfo.FirstMipLevel  = 0;
    loadInfo.Format         = DXGI_FORMAT(-1) ;//DXGI_FORMAT_R8G8B8A8_UNORM;
    loadInfo.Width          = D3DX_FROM_FILE;
    loadInfo.Height         = D3DX_FROM_FILE;
    loadInfo.Depth          = D3DX_FROM_FILE;
    loadInfo.MipLevels      = D3DX10_DEFAULT;
    loadInfo.Usage          = D3D10_USAGE_DEFAULT;
    loadInfo.MipFilter      = D3DX10_FILTER_TRIANGLE;
    loadInfo.MiscFlags      = D3D10_RESOURCE_MISC_GENERATE_MIPS;// | D3D10_RESOURCE_MISC_TEXTURECUBE;
    return true;
}

bool xD10FileTexture::__loadImageFile(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
    xImage* pImg = xImageLib::load(IMAGE_UNKNOWN,fileName , (const int8*)buf , (int)bufLen , PIXELFORMAT_R8G8B8A8 );
    if( pImg == NULL )
        return false;

    pImg->flipY();
    xImageSize _size = pImg->getSize();
    int pitch = _size.pitch;

    if(pImg->nImage() == 1)
    {
        D3D10_TEXTURE2D_DESC desc;
        ZeroMemory( &desc, sizeof(desc) );
        desc.Width            = (UINT)_size.w;
        desc.Height           = (UINT)_size.h;
        desc.MipLevels        = (UINT)pImg->nMipmapLevel();
        desc.ArraySize        = 1;
        desc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM ; //DXGI_FORMAT_R32G32B32A32_FLOAT;
        if(_size.fmt == PIXELFORMAT_R32F)
        {
            desc.Format       = DXGI_FORMAT_R32_FLOAT ; //DXGI_FORMAT_R32G32B32A32_FLOAT;
        }
        desc.SampleDesc.Count = 1;
        desc.BindFlags        = D3D10_BIND_SHADER_RESOURCE  ;//| D3D10_BIND_RENDER_TARGET; 
        desc.Usage            = D3D10_USAGE_DEFAULT;
        desc.CPUAccessFlags   = 0;
        m_TexInfo.m_Usage     = desc.Usage;

        ID3D10Texture2D * pTexture = NULL;
        D3D10_SUBRESOURCE_DATA* pInitData = new D3D10_SUBRESOURCE_DATA [ pImg->nMipmapLevel() ] ;
        for(int i = 0 ; i < (int)pImg->nMipmapLevel() ; i ++)
        {
            pInitData[i].pSysMem = pImg->data(i);
            pInitData[i].SysMemPitch = pitch;
            pInitData[i].SysMemSlicePitch = 0;
            pitch/=2;
        }   

        m_pD10Api->d10Device()->CreateTexture2D( &desc, pInitData, &pTexture );
        xImageLib::kill(pImg);
        delete [] pInitData;
        if(pTexture == NULL)
            return false;
        return _load(pTexture);
    }
    else if(pImg->nImage() == 6)
    {
        XEVOL_WARNNING_NOT_IMPLEMENT_INFO("CubeMap目前还不支持dds以外格式\n");
    }
    return true;
}

bool xD10FileTexture::_prepareLoadInfo(D3DX10_IMAGE_INFO& imgInfo , D3DX10_IMAGE_LOAD_INFO& loadInfo)
{
    xD10GIFormatInfo* fmtInfo = xD10ConstLexer::singleton()->GetPixelFormat(imgInfo.Format);
    fillLoadInfo(loadInfo, false,true);
    loadInfo.MiscFlags      = loadInfo.MiscFlags &(~D3D10_RESOURCE_MISC_GENERATE_MIPS);//不要自动生成Mipmap
    if(fmtInfo->m_CompressRate != 1)
    {
        loadInfo.BindFlags = loadInfo.BindFlags  &(~ D3D10_BIND_RENDER_TARGET );
    }
    loadInfo.Width = imgInfo.Width;
    loadInfo.Height = imgInfo.Height;
    loadInfo.Depth = imgInfo.Depth;
    loadInfo.MipLevels = imgInfo.MipLevels;
    loadInfo.Format = imgInfo.Format;
    return true;
}

bool xD10FileTexture::load(const wchar_t* fileName , unsigned long  arg)
{
    if( xFileSystem::singleton()->fileExist(fileName) == false )
        return false;

    ID3D10Resource* pTexture = NULL;

    D3DX10_IMAGE_INFO      imgInfo;
    HRESULT hr = E_FAIL;
    std::wstring nameExt = xFileSystem::singleton()->getFileExtName(fileName);
    //if(nameExt != L"jpeg" && nameExt != L"jpg")
    {
        hr = D3DX10GetImageInfoFromFileW(fileName , NULL , &imgInfo , NULL);
    }

    if( ! FAILED(hr) )
    {
        D3DX10_IMAGE_LOAD_INFO loadInfo;
        if(_prepareLoadInfo(imgInfo , loadInfo) == false)
            return false;
        D3DX10CreateTextureFromFileW( m_pD10Api->d10Device() , fileName ,  &loadInfo, NULL, &pTexture, NULL );
    }
    if(pTexture)
    {
        m_TexInfo.m_ShaderViewFmt = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_RTViewFmt  = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_ResFmt     = DXGI_FORMAT_UNKNOWN;
        return _load(pTexture);
    }
    return __loadImageFile(fileName , NULL , 0 ,arg);
}

bool xD10FileTexture::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
    if(bufLen == 0 || buf == NULL)
        return load(fileName , arg);


    HRESULT hr = E_FAIL;
    ID3D10Resource* pTexture = NULL;
    D3DX10_IMAGE_INFO      imgInfo;
    std::wstring nameExt = xFileSystem::singleton()->getFileExtName(fileName);
    //if(nameExt != L"jpeg" && nameExt != L"jpg")
    {
        hr = D3DX10GetImageInfoFromMemory(buf , bufLen ,NULL, &imgInfo , NULL);
    }
    if(! FAILED(hr) )
    {
        D3DX10_IMAGE_LOAD_INFO loadInfo;
        if(_prepareLoadInfo(imgInfo , loadInfo) == false)
            return false;
        D3DX10CreateTextureFromMemory(m_pD10Api->d10Device() , buf , bufLen , &loadInfo, NULL, &pTexture, NULL );
    }

    if(pTexture)
    {
        m_TexInfo.m_ShaderViewFmt = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_RTViewFmt  = DXGI_FORMAT_UNKNOWN;
        m_TexInfo.m_ResFmt     = DXGI_FORMAT_UNKNOWN;
        return _load(pTexture);
    }
    return __loadImageFile(fileName , buf , bufLen ,arg);
}




END_NAMESPACE_XEVOL3D