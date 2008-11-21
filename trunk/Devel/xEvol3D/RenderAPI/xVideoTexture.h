#ifndef __XEVOL_VIDEO_TEXTURE_H__
#define __XEVOL_VIDEO_TEXTURE_H__
#include "xRenderObject.h"
BEGIN_NAMESPACE_XEVOL3D
class _XEVOL_BASE_API_ IVideoTextureCtrl : public IBaseObject
{
	IMPL_BASE_OBJECT_CLASSID_INTERFACE(IVideoTextureCtrl);
public:
	IVideoTextureCtrl():IBaseObject(){}
	virtual bool load(const wchar_t* videFile) = 0;
	virtual bool play() = 0;
	virtual bool stop() = 0;
	virtual bool pause() = 0;
	virtual bool resume() = 0;
	virtual bool seekable() = 0;
	virtual bool seek(int timeToSeek) = 0;
	virtual int  totalTime() = 0;
	
	//合并YUV Plane
	virtual IBaseTexture* mergePlane() = 0;
	//将Plane安装到默认的插槽里
    virtual bool          installPlane() = 0;
	//获得指定的Plane
	virtual IBaseTexture* plane(size_t i) = 0;
	//获得Plane的个数
	virtual size_t        nPlane() = 0;
};

END_NAMESPACE_XEVOL3D
#endif


