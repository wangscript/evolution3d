
typedef unsigned long DWORD_PTR, *PDWORD_PTR;

#include <Atlbase.h>
#include <windows.h>

#include "streams.h"
#include "../include/xVideoSource.h"
#include "xVideoRenderer.h"
#include <dshow.h>
#include <string>
#include <vector>



class CVideoSourceCapature : public  IVideoSource , public ICaptureDeviceState
{
public:
	CVideoSourceCapature();
	~CVideoSourceCapature();

	class CVideoControlInterface : public ICaptureDeviceConfig
	{
	public:
		IGraphBuilder*  m_pGraphBuilder;          // GraphBuilder
		IMediaControl*  m_pMediaControl;          // Media Control
		IMediaPosition* m_pMediaPosition;         // Media Position
		IMediaEvent*    m_pMediaEvent;            // Media Event
		IBaseFilter*    m_pCaptureSrcFilter;

		ICaptureGraphBuilder2*  m_pCaptureGraphBuilder;

		CVideoControlInterface();
		~CVideoControlInterface();
		bool build();
		bool Release();
		bool release_ctrl();

		//以下为ICaptureDeviceConfig接口
		void confige();
	};
    CVideoControlInterface m_VCtrller;

	class CCaptureDeviceEnumlator : public ICaptureDeviceSelect
	{
		sCaptureDevice*        m_pDevices;
		std::vector<IMoniker*> m_pMonikers;  
		int             m_nDevices;

		int             m_CurDevice;
		bool            m_IsAllDeviceInfoLoaded;
		IBaseFilter*    m_pCaptureSrcFilter;
	public:
		CCaptureDeviceEnumlator();
		~CCaptureDeviceEnumlator();
		bool            find_allDevice();
		void*           set_curDevice(int index);
		void*           set_curDevice(const wchar_t* IDString);
		void            configue();
		sCaptureDevice* get_curDevice();
		sCaptureDevice* get_AllDevice();
		int             get_DevieCount();

		void            releaseAllDevices();
		IBaseFilter*    get_curDeviceFilter();
	};

	void           tearDown();
	void           NukeDownstream(IBaseFilter *pf);
	bool           buildPreview();
public:


	bool           load_a(const char* media,bool isNoAudio = FALSE);
	bool           load(const wchar_t* media,bool isNoAudio = FALSE);
	bool           unload();
	bool           play();
	bool           stop();
	bool           pause();
	bool           resume();
	bool           is_playing();
	bool           set_pos(double second);
	double         get_pos();
	bool           set_rate(double rate);
	double         get_rate();
	double         get_len();
	bool           is_over();

	bool           create();
	bool           Release();

	void*          get_interface(const char* interfaceString);
	void           set_alpha(unsigned char alpha);
	unsigned char  get_alpha();
	bool           check_state(bool needloop);
	long           get_state();
	const char*    media_name(){return NULL;}


	void                 set_renderTarget(IVideoRenderTarget* pRenderTarget);
	IVideoRenderTarget*  get_renderTarget();


	int                get_DeviceDimsNumber();
	sCaptureDeviceDim* get_DeviceDims();
	void               releaseDeviceDims(sCaptureDeviceDim* Dims);
	sCaptureDeviceDim  get_CurDeviceDim();
	bool               set_CurDeviceDim(int w,int h,int rate);
	bool               set_CurDeviceDim(sCaptureDeviceDim);


protected:
	//DirectShow专用的媒体播放接口
	HRESULT AddVideoCapture(const wchar_t* devIDString);
    HRESULT FindCaptureDevice(const wchar_t* devIDString = NULL);
	HRESULT SetDefVideoFormat();
	bool    BuildDeviceDimInfos(bool forceRebuild);
protected:
    CCaptureDeviceEnumlator m_DeviceEnumlator;
    xDSVideoRenderer        m_VideoRenderer;
	IVideoRenderTarget*     m_pRenderTarget;


	HRESULT m_CreateHResult; 
	std::vector<AM_MEDIA_TYPE> m_AllTypes;

};
