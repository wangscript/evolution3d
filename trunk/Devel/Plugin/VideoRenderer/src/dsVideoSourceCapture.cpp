#pragma warning (disable : 4995)
#pragma warning (disable : 4311)

#include <assert.h>
#include "dsVideoSourceCapture.h"
#include "../include/xVideoRenderTarget.h"

#define DEF_WIDTH 320
#define DEF_HEITH 240

CVideoSourceCapature::CVideoControlInterface::CVideoControlInterface()
{
	m_pGraphBuilder = NULL;          // GraphBuilder
	m_pMediaControl = NULL;          // Media Control
	m_pMediaPosition = NULL;         // Media Position
	m_pMediaEvent = NULL;            // Media Event
	m_pCaptureGraphBuilder = NULL;

}

CVideoSourceCapature::CVideoControlInterface::~CVideoControlInterface()
{
	release();
}

bool CVideoSourceCapature::CVideoControlInterface::build()
{
	m_pGraphBuilder->QueryInterface(IID_IMediaControl,(void**)&m_pMediaControl);
	m_pGraphBuilder->QueryInterface(IID_IMediaPosition,(void**)&m_pMediaPosition);
	m_pGraphBuilder->QueryInterface(IID_IMediaEvent,(void**)&m_pMediaEvent);
	return (m_pGraphBuilder&&m_pMediaControl&&m_pMediaEvent&&m_pMediaPosition);
}
bool CVideoSourceCapature::CVideoControlInterface::release_ctrl()
{
	if(m_pMediaControl)
	{
		m_pMediaControl->Release();
		m_pMediaControl = NULL;
	}
	if(m_pMediaEvent)
	{
		m_pMediaEvent->Release();
		m_pMediaEvent = NULL;
	}
	if(m_pMediaPosition)
	{
		m_pMediaPosition->Release();
		m_pMediaPosition = NULL;
	}
	return true;
}

bool CVideoSourceCapature::CVideoControlInterface::release()
{

	if(m_pMediaControl)
	{
		m_pMediaControl->Release();
		m_pMediaControl = NULL;
	}
	if(m_pMediaEvent)
	{
		m_pMediaEvent->Release();
		m_pMediaEvent = NULL;
	}
	if(m_pMediaPosition)
	{
		m_pMediaPosition->Release();
		m_pMediaPosition = NULL;
	}

	if(m_pGraphBuilder != NULL)
	{
		m_pGraphBuilder->Release();
		m_pGraphBuilder = NULL;
	}

	if(m_pCaptureGraphBuilder != NULL)
	{
         m_pCaptureGraphBuilder->Release();
		 m_pCaptureGraphBuilder = NULL;
	}
	return true; 
}

void CVideoSourceCapature::CVideoControlInterface::confige()
{
     IAMVfwCaptureDialogs *pVfw = 0;
	 if(m_pCaptureGraphBuilder == NULL)
		 return ;
	 if(m_pMediaControl == NULL)
		 return ;

	 ISpecifyPropertyPages *pSpec;
	 CAUUID cauuid;

	 HRESULT hr = m_pCaptureSrcFilter->QueryInterface(IID_ISpecifyPropertyPages,
		 (void **)&pSpec);
	 if(hr == S_OK)
	 {
		 hr = pSpec->GetPages(&cauuid);

		 try 
		 {
			 hr = OleCreatePropertyFrame(GetActiveWindow(), 30, 30, NULL, 1,
				 (IUnknown **)&m_pCaptureSrcFilter, cauuid.cElems,
				 (GUID *)cauuid.pElems, 0, 0, NULL);
		 }
		 catch(...)
		 {

		 }


		 CoTaskMemFree(cauuid.pElems);
		 pSpec->Release();
	 }
}

//-----------------------------------------------------------------------------
// CVideoSourceCapature constructor
//-----------------------------------------------------------------------------
CVideoSourceCapature::CVideoSourceCapature()
{
	m_CreateHResult = S_OK;
	m_VCtrller.m_pCaptureSrcFilter = NULL;
	m_pRenderTarget = NULL;
	m_CreateHResult = S_OK; 
}


//-----------------------------------------------------------------------------
// CVideoSourceCapature destructor
//-----------------------------------------------------------------------------
CVideoSourceCapature::~CVideoSourceCapature()
{
}

HRESULT CVideoSourceCapature::SetDefVideoFormat()
{
	if( TRUE == set_CurDeviceDim(DEF_WIDTH,DEF_HEITH,0) )
		return S_OK;
	return E_FAIL;
}
HRESULT CVideoSourceCapature::FindCaptureDevice(const wchar_t* devIDString)
{
 
	IBaseFilter* pDevSrcFilter = NULL;

	if(devIDString != NULL)
	pDevSrcFilter = (IBaseFilter*)m_DeviceEnumlator.set_curDevice(devIDString);

	if(pDevSrcFilter==NULL)
	{
          pDevSrcFilter = (IBaseFilter*)m_DeviceEnumlator.set_curDevice(0);
	}

	if(NULL == pDevSrcFilter)
		return E_FAIL;
	m_VCtrller.m_pCaptureSrcFilter = pDevSrcFilter;

	return S_OK;
}


HRESULT CVideoSourceCapature::AddVideoCapture(const wchar_t* devIDString)
{
	HRESULT hr = S_OK;

	try
	{
		// 创建CaptureCraphBuilder2.
		hr = CoCreateInstance (CLSID_CaptureGraphBuilder2 , NULL, CLSCTX_INPROC,
			IID_ICaptureGraphBuilder2, (void **) &(m_VCtrller.m_pCaptureGraphBuilder));
		if (FAILED(hr))
		{
			//Msg(TEXT("Could not create the capture graph builder!  hr=0x%x\0"), hr);
			return hr;
		}

		//把已经创建好的FilterGraph Attach到刚创建的m_pCaptureGraphBuilder上
		hr = m_VCtrller.m_pCaptureGraphBuilder->SetFiltergraph(m_VCtrller.m_pGraphBuilder);
		if (FAILED(hr))
		{
			return hr;
		}

		// Use the system device enumerator and class enumerator to find
		// a video capture/preview device, such as a desktop USB video camera.
		hr = FindCaptureDevice(devIDString);
		if (FAILED(hr))
		{
			// Don't display a message because FindCaptureDevice will handle it
			return hr;
		}

		// Add the returned capture filter to our graph.
		hr = m_VCtrller.m_pGraphBuilder->AddFilter(m_VCtrller.m_pCaptureSrcFilter, L"Video Capture");
		if (FAILED(hr))
		{
			//Msg(TEXT("Could not add the capture filter to the graph.  hr=0x%x\r\n\r\n")
			//	TEXT("Make sure that a video capture device is connected and functional\n")
			//	TEXT("and is not being used by another capture application.\0"), hr);
			return hr;
		}

		if( FAILED(SetDefVideoFormat()) )
			return E_FAIL;
		return S_OK;
		// Render the preview pin on the video capture filter.
		// This will create and connect any necessary transform filters.
		// We pass a pointer to the IBaseFilter interface of our CTextureRenderer
		// video renderer, which will draw the incoming video onto a D3D surface.&PIN_CATEGORY_PREVIEW
		//hr = m_VCtrller.m_pCaptureGraphBuilder->RenderStream (NULL, &MEDIATYPE_Video,m_VCtrller.m_pCaptureSrcFilter, NULL, this);
		//if (FAILED(hr))
		//{
		//	return hr;
		//}
	}// try
	catch(...)
	{
		hr = E_UNEXPECTED;
	}
	return S_OK;
}
//播放接口
bool   CVideoSourceCapature::unload()
{
	stop();
	IBaseFilter*    pVideoRenderer = &m_VideoRenderer; 
	if(m_VCtrller.m_pCaptureSrcFilter)
	{
		if(m_VCtrller.m_pGraphBuilder)
		{
			m_VCtrller.m_pGraphBuilder->RemoveFilter(m_VCtrller.m_pCaptureSrcFilter);
			pVideoRenderer->AddRef();
			m_VCtrller.m_pGraphBuilder->RemoveFilter(pVideoRenderer);
			m_VCtrller.m_pCaptureSrcFilter->Release();
			m_VCtrller.m_pCaptureSrcFilter = NULL;
		}
		if(m_pRenderTarget)
		{
			m_pRenderTarget->onStop();
		    m_pRenderTarget->onDelete();
		}
	}
	return true;
}

bool   CVideoSourceCapature::load_a(const char* media_file,bool isNoAudio)
{
	WCHAR wcFileName[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, media_file, -1, wcFileName, MAX_PATH);
	return load(wcFileName,isNoAudio);
	return true;
}

bool   CVideoSourceCapature::load(const wchar_t* media_file,bool isNoAudio)
{
	HRESULT hr = S_OK;
	IBaseFilter*            pVideoRenderer = &m_VideoRenderer;           // Texture Renderer Filter
	CComPtr<IPin>           pFSrcPinOut;    // Source Filter Output Pin   

	unload();

	//如果没有创建GraphBuilder。则创建它
	if(m_VCtrller.m_pGraphBuilder == NULL)
	{
		//创建FilterGraph;
		hr = CoCreateInstance(CLSID_FilterGraph, NULL,CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_VCtrller.m_pGraphBuilder);

		//创建失败
		if (FAILED(hr))
			return false;

		//Connecting the address
		if( FAILED(hr) )
		{
			return false;
		}

		pVideoRenderer->AddRef();
		if (FAILED(hr = m_VCtrller.m_pGraphBuilder->AddFilter(pVideoRenderer, L"XREALVIDEOCAPTURERENDERER")))
		{
			return false;
		}

		if(FAILED(AddVideoCapture(media_file)))
			return false;
	}
	else
	{
		hr = FindCaptureDevice(media_file);
		if (FAILED(hr))
		{
			return false;
		}

		pVideoRenderer->AddRef();
		if (FAILED(hr = m_VCtrller.m_pGraphBuilder->AddFilter(pVideoRenderer, L"XREALVIDEOCAPTURERENDERER")))
		{
			return false;
		}

		// Add the returned capture filter to our graph.
		hr = m_VCtrller.m_pGraphBuilder->AddFilter(m_VCtrller.m_pCaptureSrcFilter, L"Video Capture");
		if (FAILED(hr))
		{
			return false;
		}

		if( FAILED(SetDefVideoFormat()) )
			return false;
	}

	m_VCtrller.build();

	return true;
}

bool   CVideoSourceCapature::play()
{
	if(m_VCtrller.m_pMediaControl == NULL)
	{
		return false;
	}

	if (FAILED(m_VCtrller.m_pMediaControl->Run()))
	{ 
		return false;
	}
	return true;
}


bool   CVideoSourceCapature::stop()
{
	if(m_pRenderTarget)
	{
		m_pRenderTarget->onStop();
	}
	if(m_VCtrller.m_pMediaControl == NULL)
	{
		return false;
	}
	if( FAILED(m_VCtrller.m_pMediaControl->Stop()))
	{ 
		return false;
	}
	return true;
}

bool   CVideoSourceCapature::pause()
{
	if(m_VCtrller.m_pMediaControl == NULL)
	{
		return false;
	}
	if( FAILED(m_VCtrller.m_pMediaControl->Pause() ))
	{ 
		return false;
	}
	return true;
}

bool   CVideoSourceCapature::resume()
{
	return play();
}

bool   CVideoSourceCapature::is_playing()
{
	return true;
}


bool   CVideoSourceCapature::set_pos(double second)
{
	if(m_VCtrller.m_pMediaPosition)
	{
		if(SUCCEEDED (m_VCtrller.m_pMediaPosition->put_CurrentPosition(second) ) ) 
			return true;
		return false;
	}
	return false;
}

double CVideoSourceCapature::get_pos()
{
	double time;
	if(m_VCtrller.m_pMediaPosition == NULL)
		return -1.0f;

	if( SUCCEEDED(m_VCtrller.m_pMediaPosition->get_CurrentPosition(&time) ) )
		return time;
	else
		return -1.0;
}


bool   CVideoSourceCapature::set_rate(double rate)
{
	if(m_VCtrller.m_pMediaPosition)
	{
		if( SUCCEEDED(m_VCtrller.m_pMediaPosition->put_Rate(rate)) )
			return true;
		return false;
	}
	return false;
}

double CVideoSourceCapature::get_rate()
{
	double rate;
	if(m_VCtrller.m_pMediaPosition)
	{
		if( SUCCEEDED(m_VCtrller.m_pMediaPosition->get_Rate(&rate)) )
			return rate;
		return -1.0;
	}
	return -1.0;
}

double CVideoSourceCapature::get_len()
{
	double len;
	if(m_VCtrller.m_pMediaPosition)
	{
		if( SUCCEEDED(m_VCtrller.m_pMediaPosition->get_Duration(&len)) )
			return len;
		return -1.0;
	}
	return -1.0;
}


bool   CVideoSourceCapature::is_over()
{
	long lEventCode = get_state();
	if(lEventCode == -1)
		return false;

	if (EC_COMPLETE == lEventCode) 
	{
		return true;
	}
	return false;
}

bool   CVideoSourceCapature::check_state(bool needloop)
{

	long lEventCode = get_state();
	if(lEventCode == -1)
		return false;

	if (EC_COMPLETE == lEventCode) 
	{
		if(needloop)
		{
			return SUCCEEDED(m_VCtrller.m_pMediaPosition->put_CurrentPosition(0));
		}
		return false;
	}
	return true;
}

long   CVideoSourceCapature::get_state()
{
	long lEventCode;
	long lParam1;
	long lParam2;
	HRESULT hr;

	if (!m_VCtrller.m_pMediaEvent)
		return -1;

	hr =m_VCtrller.m_pMediaEvent->GetEvent(&lEventCode, (LONG_PTR *) &lParam1, (LONG_PTR *) &lParam2, 0);
	if (SUCCEEDED(hr))
	{
		hr = m_VCtrller.m_pMediaEvent->FreeEventParams(lEventCode, lParam1, lParam2);
		return lEventCode;
	}
	return -1;
}

bool   CVideoSourceCapature::release()
{
	stop();
	unload();
	IBaseFilter*    pVideoRenderer = &m_VideoRenderer;
	if(m_VCtrller.m_pGraphBuilder)
	{
		m_VCtrller.m_pGraphBuilder->RemoveFilter(pVideoRenderer);
	}
	m_VCtrller.release();
	return true;
}

bool CVideoSourceCapature::create()
{
	return m_DeviceEnumlator.find_allDevice();
}

void* CVideoSourceCapature::get_interface(const char* interfaceString)
{
	if(strcmp("ICaptureDeviceSelect",interfaceString) == 0)
		return (ICaptureDeviceSelect*)&m_DeviceEnumlator;
	if(strcmp("ICaptureDeviceConfig",interfaceString) == 0)
		return (ICaptureDeviceConfig*)&m_VCtrller;

	if(strcmp("ICaptureDeviceState",interfaceString) == 0)
		return dynamic_cast<ICaptureDeviceState*>(this);

	return NULL;
}
void CVideoSourceCapature::set_renderTarget(IVideoRenderTarget* pRenderTarget)
{
	m_VideoRenderer.set_renderTarget( pRenderTarget);
	m_pRenderTarget = pRenderTarget;
}

IVideoRenderTarget* CVideoSourceCapature::get_renderTarget()
{
	return m_pRenderTarget ; 
}
void CVideoSourceCapature::set_alpha(unsigned char alpha)
{
	m_VideoRenderer.set_alpha(alpha);
}
unsigned char CVideoSourceCapature::get_alpha()
{
	return m_VideoRenderer.get_alpha();
}
