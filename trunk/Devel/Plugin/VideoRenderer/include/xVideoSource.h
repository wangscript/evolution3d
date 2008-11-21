#ifndef __VIDEO_RENDER_TARGET_H__
#define __VIDEO_RENDER_TARGET_H__
#define DEFAULT_VW (320)
#define DEFAULT_VH (240)
/*
DSHOW播放视频库

版权声明
本库为开放源代码。在遵循以下条款的前提下，你可以免费下载，修改，并传播这些代码。
该代码为参考DShow SDK的代码写成。在保留作者名字的前提下大家可以用于任何用途。
但是本人不承担任何责任和潜在责任。
功能    : 本程序为DSHOW播放视频代码。

潘李亮 2006/10/10
 */

enum VIDEOIMAGE_TYPE
{
	VIT_REALSIZE,
	VIT_SCALESIZE,
	VIT_TRIMSIZE
};

class IVideoRenderTarget;

class IVideoSource
{
	//播放接口
public:
	virtual bool                 load_a(const char* media,bool isNoAudio = FALSE) = 0;
	virtual bool                 load(const wchar_t* media,bool isNoAudio = FALSE) = 0;
	virtual bool                 unload() = 0;
	virtual bool                 play() = 0;
	virtual bool                 stop() = 0;
	virtual bool                 pause() = 0;
	virtual bool                 resume() = 0;
	virtual bool                 is_playing() = 0;
	virtual bool                 set_pos(double second) = 0;
	virtual double               get_pos() = 0;
	virtual bool                 set_rate(double rate) = 0;
	virtual double               get_rate() = 0;
	virtual double               get_len() = 0;
	virtual bool                 is_over() = 0;

	virtual bool                 create() = 0;
	virtual bool                 release() = 0;

	virtual void*                get_interface(const char* interfaceString) = 0;
	virtual void                 set_alpha(unsigned char alpha) = 0;
	virtual unsigned char        get_alpha() = 0;
	virtual bool                 check_state(bool needloop) = 0;
	virtual long                 get_state() = 0;
	virtual const char*          media_name() = 0;

	//RenderTarget
	virtual void                 set_renderTarget(IVideoRenderTarget* pRenderTarget) = 0;
	virtual IVideoRenderTarget*  get_renderTarget() = 0;
	virtual ~IVideoSource(){} ;

	
};

class IFrameListener
{
public:
	virtual bool beforUpdateVideoFrame() = 0;
    virtual bool afterUpdateVideoFrame() = 0 ;
};

struct sCaptureDevice
{
    wchar_t m_Description[256];
	wchar_t m_IDString[256];
};

struct sCaptureDeviceDim
{
	int m_Width;
	int m_Height;
	int m_Rate;
};

class ICaptureDeviceSelect
{
public:
	virtual bool            find_allDevice() = 0;
    virtual void*           set_curDevice(int index) = 0;
	virtual void*           set_curDevice(const wchar_t* IDString) = 0;
	virtual void            configue() = 0;
	virtual sCaptureDevice* get_curDevice() = 0;
	virtual sCaptureDevice* get_AllDevice() = 0;
	virtual int             get_DevieCount() = 0;
};

class ICaptureDeviceState
{
public:
	virtual int                get_DeviceDimsNumber() = 0;
    virtual sCaptureDeviceDim* get_DeviceDims() = 0;
	virtual void               releaseDeviceDims(sCaptureDeviceDim* Dims) = 0;
	virtual sCaptureDeviceDim  get_CurDeviceDim() = 0;
	virtual bool               set_CurDeviceDim(int w,int h,int rate) = 0;
	virtual bool               set_CurDeviceDim(sCaptureDeviceDim) = 0;
};

class ICaptureDeviceConfig
{
public:

	virtual void confige() = 0;
};

enum eVideoRendererType
{
    eVRT_CAPTURE,
	eVRT_FILE,
};


#endif
