#pragma warning (disable : 4995)
#pragma warning (disable : 4311)

#include <assert.h>
#include <string>
using namespace std;
#include "dsVideoSourceFile.h"
#include "../include/xVideoRenderTarget.h"

#define DECL_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	EXTERN_C const GUID DECLSPEC_SELECTANY name \
	= { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

#define REGISTER_FILTERGRAPH
struct __declspec(uuid("{71791540-2017-11cf-ae26-0020afd79767}")) CLSID_TextureRenderer;

#pragma comment(lib ,"winmm.lib")

DWORD dwROTReg = 0xfedcba98;

static HRESULT AddToROT(IUnknown *pUnkGraph)
{
	IMoniker * pmk;
	IRunningObjectTable *pROT;
	if (FAILED(GetRunningObjectTable(0, &pROT))) {
		return E_FAIL;
	}

	WCHAR wsz[256];
	(void)StringCchPrintfW(wsz, NUMELMS(wsz),L"FilterGraph %08x  pid %08x\0", (DWORD_PTR) 0, GetCurrentProcessId());

	HRESULT hr = CreateItemMoniker(L"!", wsz, &pmk);
	if (SUCCEEDED(hr))
	{
		// Use the ROTFLAGS_REGISTRATIONKEEPSALIVE to ensure a strong reference
		// to the object.  Using this flag will cause the object to remain
		// registered until it is explicitly revoked with the Revoke() method.
		//
		// Not using this flag means that if GraphEdit remotely connects
		// to this graph and then GraphEdit exits, this object registration
		// will be deleted, causing future attempts by GraphEdit to fail until
		// this application is restarted or until the graph is registered again.
		hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph,
			pmk, &dwROTReg);
		pmk->Release();
	}

	pROT->Release();
	return hr;
}


static void RemoveFromROT(void)
{
	IRunningObjectTable *pirot=0;

	if (SUCCEEDED(GetRunningObjectTable(0, &pirot)))
	{
		pirot->Revoke(dwROTReg);
		pirot->Release();
	}
}


CVideoSourceFile::CVideoControlInterface::CVideoControlInterface()
{
	m_pGraphBuilder = NULL;          // GraphBuilder
	m_pMediaControl = NULL;          // Media Control
	m_pMediaPosition = NULL;         // Media Position
	m_pMediaEvent = NULL;            // Media Event

}

CVideoSourceFile::CVideoControlInterface::~CVideoControlInterface()
{
    release();
}

bool CVideoSourceFile::CVideoControlInterface::build()
{
	m_pGraphBuilder->QueryInterface(IID_IMediaControl,(void**)&m_pMediaControl);
	m_pGraphBuilder->QueryInterface(IID_IMediaPosition,(void**)&m_pMediaPosition);
	m_pGraphBuilder->QueryInterface(IID_IMediaEvent,(void**)&m_pMediaEvent);
	return (m_pGraphBuilder&&m_pMediaControl&&m_pMediaEvent&&m_pMediaPosition);
}
bool CVideoSourceFile::CVideoControlInterface::release()
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
   return TRUE; 
}

//-----------------------------------------------------------------------------
// CVideoSourceFile constructor
//-----------------------------------------------------------------------------
CVideoSourceFile::CVideoSourceFile() 
{
    // Store and AddRef the texture for our use.
    //ASSERT(m_CreateHResult);
    //if (m_CreateHResult)
    m_CreateHResult = S_OK;
	m_isNoAudio = FALSE;
	m_SrcFilter = NULL;
	m_pColorSpaceFilter = NULL;
	m_pVideoRenderer = new xDSVideoRenderer();
	m_CreateHResult = S_OK; 
	m_isNoAudio = false;
	m_FileName = "";
	m_SrcFilter = NULL;

}


//-----------------------------------------------------------------------------
// CVideoSourceFile destructor
//-----------------------------------------------------------------------------
CVideoSourceFile::~CVideoSourceFile()
{
	delete m_pVideoRenderer;
	release();
}

/*
//-----------------------------------------------------------------------------
// Msg: Display an error message box if needed
//-----------------------------------------------------------------------------
void Msg(TCHAR *szFormat, ...)
{
    TCHAR szBuffer[1024];  // Large buffer for long filenames or URLs
    const size_t NUMCHARS = sizeof(szBuffer) / sizeof(szBuffer[0]);
    const int LASTCHAR = NUMCHARS - 1;

    // Format the input string
    va_list pArgs;
    va_start(pArgs, szFormat);

    // Use a bounded buffer size to prevent buffer overruns.  Limit count to
    // character size minus one to allow for a NULL terminating character.
    _vsntprintf(szBuffer, NUMCHARS - 1, szFormat, pArgs);
    va_end(pArgs);

    // Ensure that the formatted string is NULL-terminated
    szBuffer[LASTCHAR] = TEXT('\0');

    MessageBox(NULL, szBuffer, TEXT("视频纹理加载器"), 
               MB_OK | MB_ICONERROR);
}
*/

//播放接口
bool   CVideoSourceFile::unload()
{
	stop();
	if(m_SrcFilter)
	{
		if(m_VCtrller.m_pGraphBuilder)
		{
			RemoveFromROT();
			m_VCtrller.m_pGraphBuilder->RemoveFilter(m_SrcFilter);
            m_VCtrller.m_pGraphBuilder->RemoveFilter(m_pVideoRenderer);
			m_SrcFilter->Release();
			m_SrcFilter = NULL;

			if(m_pColorSpaceFilter)
			{
				m_VCtrller.m_pGraphBuilder->RemoveFilter(m_pColorSpaceFilter);
				m_pColorSpaceFilter->Release();
				m_pColorSpaceFilter= NULL;
			}
		}

		if(m_pRenderTarget)
		{
			m_pRenderTarget->onStop();
		    m_pRenderTarget->onDelete();
		}
	}
    return TRUE;
}
bool   CVideoSourceFile::load_a(const char* media_file,bool isNoAudio)
{
	WCHAR wcFileName[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, media_file, -1, wcFileName, MAX_PATH);
	return load(wcFileName,isNoAudio);
    return TRUE;
}


DECL_GUID(CLSID_ColorSpaceFilter,
			//0x083863F1, 0x70DE, 0x11D0, 0xBD, 0x40, 0x00, 0xA0, 0xC9, 0x11, 0xCE, 0x86);
			  0x1643E180, 0x90F5, 0x11CE, 0x97, 0xD5, 0x00, 0xAA, 0x00, 0x55, 0x59, 0x5A);
//0x083863F1, 0x56AE, 0x11D6, 0x8C, 0x1F, 0x00, 0x10, 0x4B, 0x49, 0xAA, 0x66);

//{083863F1-70DE-11D0-BD40-00A0C911CE86}\{1643E180-90F5-11CE-97D5-00AA0055595A}

static IPin* FindPin(IBaseFilter* pFilter,const wchar_t* name)
{
	IEnumPins* pEnumPin = NULL;
	pFilter->EnumPins(&pEnumPin);
	IPin* pin = NULL;
	IPin* pinLast = NULL;
	while(SUCCEEDED(pEnumPin->Next(1,&pin,NULL) ) )
	{
		PIN_INFO info;
		if(pinLast == pin) break;
		pin->QueryPinInfo(&info);
		wstring pinName = info.achName;
		if(pinName.find(name) != wstring::npos )
		{
			pEnumPin->Release();
			return pin;
		}
		pin->Release();
		pinLast = pin;
		continue;
	}
	pEnumPin->Release();
	return NULL;
}

bool   CVideoSourceFile::load(const wchar_t* media_file,bool isNoAudio)
{
	m_isNoAudio = isNoAudio;
	HRESULT hr = S_OK;
	CComPtr<IBaseFilter>    pVideoRenderer;           // Texture Renderer Filter
	CComPtr<IPin>           pFSrcPinOut;    // Source Filter Output Pin   

	unload();
	//如果没有创建GraphBuilder。则创建它
	if(m_VCtrller.m_pGraphBuilder == NULL)
	{
		//创建FilterGraph;
		hr = CoCreateInstance(CLSID_FilterGraph, NULL,CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void **)&m_VCtrller.m_pGraphBuilder);

		//创建失败
		if (FAILED(hr))
			return FALSE;

		//Connecting the address
		if( FAILED(hr) )
		{
			return FALSE;
		}


	}

	//hr = CoCreateInstance(CLSID_ColorSpaceFilter, NULL , CLSCTX_INPROC_SERVER ,IID_IBaseFilter  , (void**)&m_pColorSpaceFilter);
	//if(SUCCEEDED(hr))
	//{
	//	m_VCtrller.m_pGraphBuilder->AddFilter(m_pColorSpaceFilter, L"Color Space converter");
	//}


	//得到VideoRenderer里的IBaseFilter的指针，并加到graph里
	pVideoRenderer   = m_pVideoRenderer;
	m_pVideoRenderer ->AddRef();
	if (FAILED(hr = m_VCtrller.m_pGraphBuilder->AddFilter(m_pVideoRenderer, L"XREALVIDEORENDERER")))
	{
		return FALSE;
	}

	if(m_isNoAudio)
	{
		//如果不需要音频分量。则直接连接两个Pin.而不需要让Filter Graph渲染所有的pins(因为只要把源输出和输入连接起来)
		//加一个文件源Filter到Graph里。
		if (FAILED(m_VCtrller.m_pGraphBuilder->AddSourceFilter (media_file, L"SOURCE", &m_SrcFilter) ))
		{
			return FALSE;
		}
		//找不到输出连接点
		pFSrcPinOut = FindPin(m_SrcFilter,L"Output");
		if ( pFSrcPinOut == NULL && (pFSrcPinOut = FindPin(m_SrcFilter,L"Video")) == NULL )
		{
			return FALSE;
		}

		CComPtr<IPin> pFTRPinIn = FindPin(m_pVideoRenderer,L"In");      // Texture Renderer Input Pin
		//查找源的输出pin，和渲染器的输入pin。
		if (pFTRPinIn == NULL)
		{
			return FALSE;
		}
		//连接两个pin
		if (FAILED(hr = m_VCtrller.m_pGraphBuilder->Connect(pFSrcPinOut, pFTRPinIn)))
		{
			return FALSE;
		}
	}
	else
	{
		//渲染源filter的输出pin，Filter Graph Manager将连接视频流，
		//CVideoSourceFile将会加载和连接到audio Renderer(如果需要)。
		if (FAILED(hr =m_VCtrller.m_pGraphBuilder->RenderFile(media_file,NULL)))
		{
			return FALSE;
		}
		//if (FAILED(hr =m_VCtrller.m_pGraphBuilder->Render(pFSrcPinOut)))
		//{
		//	return FALSE;
		//}
	}

	//取得Graph Media 的Control event position 控制接口
	// Get the graph's media control, event & position interfaces
	m_VCtrller.build();

	AddToROT(m_VCtrller.m_pGraphBuilder);
	return TRUE;
}

bool   CVideoSourceFile::play()
{
	if(m_VCtrller.m_pMediaControl == NULL)
	{
		return FALSE;
	}

	if (FAILED(m_VCtrller.m_pMediaControl->Run()))
	{ 
		return FALSE;
	}
    return TRUE;
}


bool   CVideoSourceFile::stop()
{
	if(m_pRenderTarget)
	{
		m_pRenderTarget->onStop();
	}
	if(m_VCtrller.m_pMediaControl == NULL)
	{
		return FALSE;
	}
	if( FAILED(m_VCtrller.m_pMediaControl->Stop()))
	{ 
		return FALSE;
	}
	return TRUE;
}

bool   CVideoSourceFile::pause()
{
	if(m_VCtrller.m_pMediaControl == NULL)
	{
		return FALSE;
	}
	if( FAILED(m_VCtrller.m_pMediaControl->Pause() ))
	{ 
		return FALSE;
	}
	return TRUE;
}

bool   CVideoSourceFile::resume()
{
     return play();
}

bool   CVideoSourceFile::is_playing()
{
     return TRUE;
}


bool   CVideoSourceFile::set_pos(double second)
{
	if(m_VCtrller.m_pMediaPosition)
	{
       if(SUCCEEDED (m_VCtrller.m_pMediaPosition->put_CurrentPosition(second) ) ) 
	      return TRUE;
	   return FALSE;
	}
	return FALSE;
}

double CVideoSourceFile::get_pos()
{
	 double time;
	 if(m_VCtrller.m_pMediaPosition == NULL)
		 return FALSE;

	 if( SUCCEEDED(m_VCtrller.m_pMediaPosition->get_CurrentPosition(&time) ) )
         return time;
	 else
		 return -1.0;
}


bool   CVideoSourceFile::set_rate(double rate)
{
	if(m_VCtrller.m_pMediaPosition)
	{
        if( SUCCEEDED(m_VCtrller.m_pMediaPosition->put_Rate(rate)) )
			return TRUE;
		return FALSE;
	}
	return FALSE;
}

double CVideoSourceFile::get_rate()
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

double CVideoSourceFile::get_len()
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


bool   CVideoSourceFile::is_over()
{
	long lEventCode = get_state();
	if(lEventCode == -1)
		return FALSE;

	if (EC_COMPLETE == lEventCode) 
	{
		return TRUE;
	}
	return FALSE;
}

bool   CVideoSourceFile::check_state(bool needloop)
{

	long lEventCode = get_state();
	if(lEventCode == -1)
		return FALSE;

	if (EC_COMPLETE == lEventCode) 
	{
		if(needloop)
		{
		    return SUCCEEDED(m_VCtrller.m_pMediaPosition->put_CurrentPosition(0));
		}
        return FALSE;
	}
	return FALSE;
}

long   CVideoSourceFile::get_state()
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

bool   CVideoSourceFile::release()
{
   stop();
   unload();
   if(m_VCtrller.m_pGraphBuilder)
   {
	   m_VCtrller.m_pGraphBuilder->RemoveFilter( m_pVideoRenderer );
   }
   m_VCtrller.release();
   return true;
}

void* CVideoSourceFile::get_interface(const char* interfaceString)
{
	return NULL;
}

void CVideoSourceFile::set_renderTarget(IVideoRenderTarget* pRenderTarget)
{
	m_pVideoRenderer->set_renderTarget( pRenderTarget);
	m_pRenderTarget = pRenderTarget;
}

IVideoRenderTarget* CVideoSourceFile::get_renderTarget()
{
	return m_pRenderTarget ; 
}
void                CVideoSourceFile::set_alpha(unsigned char alpha)
{
	m_pVideoRenderer->set_alpha(alpha);
}
unsigned char       CVideoSourceFile::get_alpha()
{
	return m_pVideoRenderer->get_alpha();
}
