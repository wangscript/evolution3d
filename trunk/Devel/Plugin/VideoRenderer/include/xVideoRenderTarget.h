#ifndef __VIDEO_IMAGE_RENDER_TARGET_H__
#define __VIDEO_IMAGE_RENDER_TARGET_H__
/*
DSHOW播放视频库

版权声明
本库为开放源代码。在遵循以下条款的前提下，你可以免费下载，修改，并传播这些代码。
该代码为参考DShow SDK的代码写成。在保留作者名字的前提下大家可以用于任何用途。
但是本人不承担任何责任和潜在责任。
功能    : 本程序为DSHOW播放视频代码。

潘李亮 2006/10/10
 */
enum VideoImageFormat
{
	VIF_RGB,
	VIF_RGBA,
	VIF_YV12,
	VIF_YUY2,
};

class IVideoRenderTarget
{
public:


	struct VRT_LockRect
	{
		unsigned char  mBytePerPixel;
		unsigned char* mPixel;
		int            mPicth;

		int            mWidth;
		int            mHeight;

		int            mBufferWidth;
		int            mBufferHeight;
	};


	virtual ~IVideoRenderTarget(){}
	virtual void           release() = 0;
	virtual void           thread_lock() = 0;
	virtual void           thread_unlock() = 0;

	
	//设备被删除的时候，收到这个通知
	virtual bool             flipY(){return false; }
	virtual bool             flipRGBA(){return false; }
	virtual bool             onDelete() = 0;
	virtual VideoImageFormat fmt() = 0;
	virtual bool             direct_update() = 0;
	virtual bool             update(void* data , int w , int h , int picth , VideoImageFormat fmt) = 0;
	virtual bool             check_fmt(VideoImageFormat vfmt) = 0;
	//设备被创建的时候，也就是和摄像头或者视频解码器连接的时候，会收到这个消息
	virtual bool             onCreate(int Width,int Height, VideoImageFormat vifmt) = 0;
	//当播放器或者摄像头停止的时候，收到这个消息
	virtual void             onStop() = 0;

	//缓冲区管理
	virtual void*          lock(VRT_LockRect& lockRect) = 0;
	virtual VRT_LockRect*  lock() = 0;
	virtual void*          unlock() = 0;

	//开始绘制一帧图象的时候收到这个消息
	virtual void           onStartRender() = 0;
	//绘制完图象的时候，收到这个消息， isRendered为true的时候，表示绘制成功，否则失败
	virtual void           onEndRender(bool isRendered) = 0;


};


#endif
