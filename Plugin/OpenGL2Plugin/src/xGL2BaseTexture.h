#ifndef _XEVOL_OPENGL20_BASE_TEXTURE_H_
#define _XEVOL_OPENGL20_BASE_TEXTURE_H_
#include <RenderApi/xRenderApi.h>
#include <string>
#include <vector>
#include <RenderAPI/xBaseTexture.h>
#include <OperationSys/xOperationSys.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/glu.h>

BEGIN_NAMESPACE_XEVOL3D
class xGL2RenderApi;
class xGL2TextureBuffer
{
public:
	char* m_pixel;
	int   m_pitch;
	int   m_depth;
	int   m_dataLen;
	void* m_destBuffer;
public:
	xGL2TextureBuffer();
	~xGL2TextureBuffer();
	void  setDestBuffer(void* destBuffer);
	bool  create(int pitch , int h);
	bool  unload();
	bool  flush(int h ,  int dest_pitch , int src_pitch = 0);
};

struct xGLTextureInfo
{
	ePIXEL_FORMAT m_fmt;
	GLuint        m_TextureID;
	int           m_TexWidth;
	int           m_TexHeight;
	int           m_TexDepth;
	int           m_MipmapLevel;
	int           m_ArraySize;
	eResourceType m_Type;
	int           m_MemSize;
	int           m_Pitch;
	int           m_SlicePitch;
	int           m_nBytePerPixel;
	D3D10_USAGE   m_Usage;

};
class xGL2BaseTexture : public IBaseTexture
{
protected:
	xGL2RenderApi*            m_pGL2Api;
	xGLTextureInfo            m_TexInfo;
	IMPL_REFCOUNT_OBJECT_INTERFACE(xGL2BaseTexture);
public:
	xGL2BaseTexture(xGL2RenderApi* pGL2Api);
	virtual ~xGL2BaseTexture();
	virtual unsigned long  memUsage();
	virtual eResourceType  res_type();
	virtual IRenderTarget* toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0){return NULL ; }
	virtual ePIXEL_FORMAT  format(){ return m_TexInfo.m_fmt ; }
	virtual bool           validate(){return true; }
	virtual bool           desc(xTextureDesc& _desc);
	virtual bool           grabRenderTagetData(int x , int y , int w , int h , void* pData) { return false ; }
    virtual bool           unload();
    bool                   thread_lock();
    bool                   thread_unlock();
    xThreadLocker*         thread_locker();
	GLuint                 getTexID() { return m_TexInfo.m_TextureID; }
public:
    xThreadLocker*         m_pLocker;
    
};

class xGL2UnkwonTexture : public xGL2BaseTexture
{
public:
	void*                 handle();
	virtual bool          isLoaded();
	virtual bool          unload();
protected:
	virtual bool          load(const wchar_t* fileName , unsigned long  arg) = 0;
	virtual bool          load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg) = 0;

	virtual bool          update(void* data  , int dateLen ,int mipmapLevel = 0 , int arraySlice = 0){ return NULL ;}
	virtual bool          lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0){ return false ;}
	virtual bool          unlock(xTextureLockArea& lockInfo){return false;}
protected:
	xGL2UnkwonTexture(xGL2RenderApi* pD10Api);
	virtual ~xGL2UnkwonTexture();
};

enum GL_FEATURE 
{
    FEATURE_NPOT,
    FEATURE_PBO,
    FEATURE_TEXTRUE_COMPRESSION,
};
bool GL_Feature_Support(GL_FEATURE _feature);
END_NAMESPACE_XEVOL3D

#endif
