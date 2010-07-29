#include "xGL2FileTexture.h"
#include "xOpenGL2API.h"
#include "xGL2ConstLexer.h"
#include <BaseLib/xI18N.h>
#include <BaseLib/xBaseMath.h>
#include <Image/xImage.h>
#include <fs/xFileSystem.h>
#include <D3Dx9.h>
BEGIN_NAMESPACE_XEVOL3D

IMPL_BASE_OBJECT_CLASSID(xGL2FileTexture  , IBaseTexture);

xGL2FileTexture::xGL2FileTexture(xGL2RenderApi* pAPI )
:xGL2UnkwonTexture(pAPI)
{

}

xGL2FileTexture::~xGL2FileTexture()
{
   unload();
}


bool xGL2FileTexture::__loadImageFile(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	xImage* pImg = xImageLib::load(IMAGE_UNKNOWN,fileName , (const int8*)buf , (int)bufLen , PIXELFORMAT_R8G8B8A8 );
	if( pImg == NULL )
		return false;

	xImageSize _size = pImg->getSize();
	//GL2_TEXTURE2D_DESC desc;
	//ZeroMemory( &desc, sizeof(desc) );
	//desc.Width            = (UINT)_size.w;
	//desc.Height           = (UINT)_size.h;
	//desc.MipLevels        = (UINT)pImg->nMipmapLevel();
	//desc.ArraySize        = 1;
	//desc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM ; //DXGI_FORMAT_R32G32B32A32_FLOAT;
	//desc.SampleDesc.Count = 1;
	//desc.BindFlags        = GL2_BIND_SHADER_RESOURCE  ;//| GL2_BIND_RENDER_TARGET; 
	//desc.Usage            = GL2_USAGE_DEFAULT;
	//desc.CPUAccessFlags   = 0;
 //   
	int pitch = _size.pitch;

	if(pImg->nImage() == 1)
	{
		/// create GL texture  ID
		glGenTextures(1, &m_TexInfo.m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TexInfo.m_TextureID );
		for ( size_t mip = 0; mip < pImg->nMipmapLevel(); i++ )
		{
			glTexImage2D(GL_TEXTURE_2D, mip, GL_RGB8, _size.w, _size.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImg->data(mip));

		}

	}
	else if(pImg->nImage() == 6)
	{
        XEVOL_WARNNING_NOT_IMPLEMENT_INFO("CubeMap目前还不支持dds以外格式\n");
	}
	return true;
}

bool xGL2FileTexture::load(const wchar_t* fileName , unsigned long  arg)
{
	return __loadImageFile(fileName , NULL , 0 ,arg);
}

bool xGL2FileTexture::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	if(bufLen == 0 || buf == NULL)
		return load(fileName , arg);

	return __loadImageFile(fileName , buf , bufLen ,arg);
}




END_NAMESPACE_XEVOL3D