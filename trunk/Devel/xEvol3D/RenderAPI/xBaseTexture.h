#ifndef __XEVOL_BASE_TEXTURE_H__
#define __XEVOL_BASE_TEXTURE_H__
#include "xRenderObject.h"
#include "../BaseLib/stl_dllalloc.h"
#include "../BaseLib/xStringHash.h"
#include <vector>
BEGIN_NAMESPACE_XEVOL3D
/*
纹理和Sampler的使用规则：
1. 对于知道使用啥Shader的对象，直接使用setSampler(L"samplerName")和setTexture(L"TextureName")来设置。
   可以使用预处理的iSamplerSlot和iTexSlot来优化

2. 对于不知道使用啥Shader的对象，采用绑定器的机制，将Shader绑定到纹理上去.因为Shader知道自己需要什么。
   Shader里的Texture/Sampler能手动指定使用哪个slot。在shader的bind的时候，自动将RenderApi对应slot中的对象
   应用到Shader里。
   RenderApi会定义一群预制的Sampler，这些Sampler的参数写在脚本里可以自动加载，Shader的Sampler可以指定使用这些Sampler.
   RenderApi会定义一群预制的Texture Slot. 该Slot使用hash ID作为标记。Shader中会使用该hashID来指定使用哪个Slot。
  
绑定方法1. DrawalbeObject->m_pGpuProgram->bindTexture(L"DiffuseTexture" , pTexture);
绑定方法2. RenderApi->setTexture(L"TextureSlot",pTexture);会绑定纹理。
  

*/

struct xTextureLockArea
{
     char*  m_pixels;
	 int    m_width;
	 int    m_height;
	 int    m_depth;
	 int    m_picth;
	 int    m_slice_pitch;
	 long   m_lockResource;
};

struct xTextureDesc
{
	int           m_width;
	int           m_height;
	int           m_depth;
	ePIXEL_FORMAT m_fmt;
	int           m_nArraySize;
};



class _XEVOL_BASE_API_   IBaseTexture : public IRenderResource
{
public:
	IBaseTexture(IRenderApi* pRenderApi);
	virtual ~IBaseTexture();
public:
	virtual ePIXEL_FORMAT  format() = 0;
	virtual bool           validate() = 0;
	virtual bool           desc(xTextureDesc& desc) = 0;
	virtual bool           update(void* data  , int dateLen ,int mipmapLevel = 0 , int arraySlice = 0) = 0 ;
	virtual bool           lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0) = 0 ;
	virtual bool           unlock(xTextureLockArea& lockInfo) = 0 ;
     
	virtual bool           create(int w , int h , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1) = 0;
	virtual bool           create(int w , int h , int depth , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1) = 0;
	virtual IRenderTarget* toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0) = 0;
};


class _XEVOL_BASE_API_  ITextureLoader : public IBaseObject
{
public:
	virtual IBaseTexture*  createTexture(IRenderApi* pApi) = 0;
	virtual IBaseTexture*  loadTexture(IRenderApi* pApi , const wchar_t* textFile, void* _buf , int bufLen , const xTextureName& _texName ) = 0;
	virtual size_t         nExt() = 0;
	virtual const wchar_t* ext(size_t idx) = 0;
};

class _XEVOL_BASE_API_ xTextureLdrMgr
{
	xTextureLdrMgr();
	~xTextureLdrMgr();
public:
	//自定义纹理加载器
	ITextureLoader*        findTextureLoader(const wchar_t* texExt);
	void                   addTextureLoader(ITextureLoader* pTextureLoader);
	void                   removeTextureLoader(ITextureLoader* pTextureLoader);
	static xTextureLdrMgr* singleton();
};


END_NAMESPACE_XEVOL3D
#endif
