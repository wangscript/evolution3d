/*
XReal3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xreal3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/


#ifndef _WIN32
#define BOOL int
#define TRUE  1
#define FALSE 0
#else
#include <windows.h>
#endif
#include "xVideoRenderer.h"
#include "xVideoRenderTarget.h"
#include "RenderAPI/xRenderAPI.h"
#include "RenderAPI/xBaseTexture.h"
#include "RenderAPI/xVideoTexture.h"
USING_NS_XEVOL3D;

class xR_VideoTexture;
class xR_VideoControl;

class xVideoCanvas :  public IVideoRenderTarget
{
	friend class xR_VideoControl;
	friend class xR_VideoTexture;
protected:
	xR_VideoTexture*                 m_pTexture;
public:
	xVideoCanvas();
	~xVideoCanvas();
	virtual void             release();
	virtual void             thread_lock();
	virtual void             thread_unlock();


	//设备被删除的时候，收到这个通知
	virtual bool             onDelete();
	virtual VideoImageFormat fmt() ;
	virtual bool             direct_update();
	virtual bool             update(void* data , int w , int h , int picth , VideoImageFormat fmt);
	virtual bool             check_fmt(VideoImageFormat vfmt);
	//设备被创建的时候，也就是和摄像头或者视频解码器连接的时候，会收到这个消息
	virtual bool             onCreate(int Width,int Height, VideoImageFormat vifmt);
	//当播放器或者摄像头停止的时候，收到这个消息
	virtual void             onStop();
	virtual bool             flipY();
	virtual bool             flipRGBA();

	//缓冲区管理
	virtual void*            lock(VRT_LockRect& lockRect);
	virtual VRT_LockRect*    lock();
	virtual void*            unlock();

	//开始绘制一帧图象的时候收到这个消息
	virtual void             onStartRender();
	//绘制完图象的时候，收到这个消息， isRendered为true的时候，表示绘制成功，否则失败
	virtual void             onEndRender(bool isRendered);
   
};

class xR_VideoControl : public IVideoTextureCtrl
{
public:
	IVideoSource*    m_pVideoRender;
	xR_VideoTexture* m_pVideoTexture;
	IMPL_REFCOUNT_OBJECT_INTERFACE(xR_VideoControl);

	static   xObjectClassID ms_CLASSID;                              
	virtual const    xObjectClassID&  classID();
    virtual void*    queryObject(const xObjectClassID& _id);
    virtual bool     isType(const    xObjectClassID& _id);
public:
	xR_VideoControl();
	virtual bool load(const wchar_t* videFile);
	virtual bool play() ;
	virtual bool stop() ;
	virtual bool pause() ;
	virtual bool resume() ;
	virtual bool seekable() ;
	virtual bool seek(int timeToSeek);
	virtual int  totalTime() ;

	virtual IBaseTexture* mergePlane();
	//将Plane安装到默认的插槽里
	virtual bool          installPlane();
	//获得指定的Plane
	virtual IBaseTexture* plane(size_t i);
	//获得Plane的个数
	virtual size_t        nPlane();
};

//class _XREAL_API_  IFrameUpdateObject
//{
//public:
//	virtual const char* name() = 0;
//	virtual bool  updateFrame(int passedTime , unsigned int thisTime) = 0;
//	virtual void  turnOn(bool bTurnOn) = 0;
//	virtual bool  isTurnOn() = 0;
//};

class xR_VideoTexture : public IBaseTexture //,  public IVideoRenderTarget  public IFrameUpdateObject
{
protected:
	xR_VideoControl  m_VideoCtrl;
	xVideoCanvas     m_Canvas;
	IBaseTexture*    m_p2DTexture;
	VideoImageFormat m_fmt;
	IRenderApi*      m_pRenderApi;
	bool             m_bEnableYV12;
	bool             m_bEnableYUY2;
public:
	IMPL_REFCOUNT_OBJECT_INTERFACE(xR_VideoControl);
	static   xObjectClassID ms_CLASSID;                              
	virtual const    xObjectClassID&  classID();
	virtual void*    queryObject(const xObjectClassID& _id);
	virtual bool     isType(const    xObjectClassID& _id);



public:
	xR_VideoTexture(IRenderApi*pRenderApi , bool bFileRenderer , bool bEnableYV12, bool bEnableYUV2);
	~xR_VideoTexture();

	ePIXEL_FORMAT  format();
	bool           validate();
	bool           desc(xTextureDesc& desc);
	bool           load(const wchar_t* fileName , unsigned long  arg);
	bool           load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg);
	bool           unload();

	bool           update(void* data  , int dateLen ,int mipmapLevel = 0 , int arraySlice = 0) {return false; }
	bool           lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel = 0 , int arraySlice = 0);
	bool           unlock(xTextureLockArea& lockInfo);

	bool           create(int w , int h , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1) {return false; }
	bool           create(int w , int h , int depth , ePIXEL_FORMAT fmt, int mipMapLevels = 1 , int arraySize = 1) {return false; }
	IRenderTarget* toRenderTarget(size_t iSlice = 0 , size_t iMipMapLevel = 0) { return NULL; }


	void*          handle();
	eResourceType  res_type(){return RESOURCE_TEXTURE2D ; }

	bool           isLoaded(){return m_VideoCtrl.m_pVideoRender != NULL ; }
	
	unsigned long  memUsage();

	//=================================
	void           thread_lock();
	void           thread_unlock();
    bool           check_fmt(VideoImageFormat vfmt);

	//设备被删除的时候，收到这个通知
	VideoImageFormat fmt(){return m_fmt ; }
	bool             onDelete();
	bool             direct_update();
	bool             OnUpdate(void* data , int w , int h , int picth , VideoImageFormat fmt);
	bool             onCreate(int Width,int Height, VideoImageFormat vifmt);
	//当播放器或者摄像头停止的时候，收到这个消息
	void             onStop();

	//缓冲区管理
	void*            lock(IVideoRenderTarget::VRT_LockRect& lockRect);
	IVideoRenderTarget::VRT_LockRect*  lock();
	void*            unlock();

	//开始绘制一帧图象的时候收到这个消息
	void             onStartRender();
	//绘制完图象的时候，收到这个消息， isRendered为true的时候，表示绘制成功，否则失败
	void             onEndRender(bool isRendered);

	IBaseTexture*    mergePlane();
	//将Plane安装到默认的插槽里
	bool             installPlane();
	//获得指定的Plane
	IBaseTexture*    plane(size_t i);
	//获得Plane的个数
	size_t           nPlane();
	void             __freeData();

	void             __InstallYV12Env();
	void             __MergeYV12Texture();
protected:
	IVideoRenderTarget::VRT_LockRect m_LockInfo;
    xTextureLockArea                 m_LockArea;
	IGpuProgram*                     m_pYUVMergeShader;
	IRenderView*                     m_pMergeRenderView;
	IBaseTexture*                    m_pYUVTexture[3];
	CRITICAL_SECTION                 m_Locker;
	int                              m_Width;
	int                              m_Height;
	bool                             m_bDirty;
};