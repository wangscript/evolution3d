#include "xVideoRenderer.h"
#pragma warning (disable : 4995)
#pragma warning (disable : 4311)
#include <assert.h>
#include "../include/xVideoRenderTarget.h"

#define REGISTER_FILTERGRAPH
struct __declspec(uuid("{71791540-2017-11cf-ae26-0020afd79767}")) CLSID_TextureRenderer;

#ifdef  _DEBUG
#pragma comment(lib ,"strmbased.lib")
#else
#pragma comment(lib ,"strmbase.lib")
#endif

#pragma comment(lib ,"winmm.lib")

//-----------------------------------------------------------------------------
// xDSVideoRenderer constructor
//-----------------------------------------------------------------------------
xDSVideoRenderer::xDSVideoRenderer() : CBaseVideoRenderer(__uuidof(CLSID_TextureRenderer),NAME("xVideoRenderer"), NULL ,&m_CreateHResult)
{
	// Store and AddRef the texture for our use.
	//ASSERT(m_CreateHResult);
	//if (m_CreateHResult)
	m_CreateHResult = S_OK;
	m_pRenderTarget = NULL;
	m_pRenderTarget = NULL;
	m_Alpha = 255;
	m_lVidWidth = 0;	// Video width
	m_lVidHeight = 0;	// Video Height
	m_lVidPitch = 0;	// Video Pitch
	m_CreateHResult = S_OK; 
	m_pRenderer = NULL;

}


//-----------------------------------------------------------------------------
// xDSVideoRenderer destructor
//-----------------------------------------------------------------------------
xDSVideoRenderer::~xDSVideoRenderer()
{
}

//-----------------------------------------------------------------------------
// CheckMediaType: This method forces the graph to give us an R8G8B8 video
// type, making our copy to texture memory trivial.
//-----------------------------------------------------------------------------
HRESULT xDSVideoRenderer::CheckMediaType(const CMediaType *pmt)
{
	HRESULT   hr = E_FAIL;
	VIDEOINFO *pvi=0;

	CheckPointer(pmt,E_POINTER);

	//如果不是一个视频，我们就返回一个错误
	if( *pmt->FormatType() != FORMAT_VideoInfo && *pmt->FormatType() != FORMAT_VideoInfo2 ) 
	{
		return E_INVALIDARG;
	}

	//我们只接受RGB24视频
	pvi = (VIDEOINFO *)pmt->Format();

	pvi->AvgTimePerFrame;
	if( IsEqualGUID( *pmt->Type(),    MEDIATYPE_Video)  && 
		IsEqualGUID( *pmt->Subtype(), MEDIASUBTYPE_YV12 ) && 
		m_pRenderTarget && (m_pRenderTarget->check_fmt(VIF_YV12) ) )//MEDIASUBTYPE_YV12))
	{
		m_pRenderer = &xDSVideoRenderer::YV12RenderSample;
		hr = S_OK;
		m_fmt = VIF_YV12;
		return hr;
	}

	if(IsEqualGUID( *pmt->Type(),    MEDIATYPE_Video)  && 
		IsEqualGUID( *pmt->Subtype(), MEDIASUBTYPE_YUY2 ) &&
		m_pRenderTarget && (m_pRenderTarget->check_fmt(VIF_YUY2) ) )//MEDIASUBTYPE_RGB24))
	{
		m_pRenderer = &xDSVideoRenderer::YUY2RenderSample;
		hr = S_OK;
		m_fmt = VIF_YUY2;
		return hr;
	}
	

	//Check for RGB
	if(IsEqualGUID( *pmt->Type(),    MEDIATYPE_Video)  &&
	   IsEqualGUID( *pmt->Subtype(), MEDIASUBTYPE_RGB24) && 
	   m_pRenderTarget && (m_pRenderTarget->check_fmt(VIF_RGBA) || m_pRenderTarget->check_fmt(VIF_RGB) )   )
	{
		m_pRenderer = &xDSVideoRenderer::RGBRenderSample;
		hr = S_OK;
		m_fmt =  m_pRenderTarget->check_fmt(VIF_RGB) ? VIF_RGB : VIF_RGBA;
		return hr;
	}
	return hr;
}




typedef struct tagVIDEOINFOHEADER2 {
	RECT                rcSource;
	RECT                rcTarget;
	DWORD               dwBitRate;
	DWORD               dwBitErrorRate;
	REFERENCE_TIME      AvgTimePerFrame;
	DWORD               dwInterlaceFlags;
	DWORD               dwCopyProtectFlags;
	DWORD               dwPictAspectRatioX; 
	DWORD               dwPictAspectRatioY; 
	union {
		DWORD           dwControlFlags;
		DWORD           dwReserved1;
	};
	DWORD               dwReserved2;
	BITMAPINFOHEADER    bmiHeader;
} VIDEOINFOHEADER2;

HRESULT xDSVideoRenderer::SetMediaType(const CMediaType *pmt)
{

	//重新找回媒体的信息
	// Bitmap info header
	if(*pmt->FormatType() == FORMAT_VideoInfo)
	{
		VIDEOINFO *pviBmp;    
		pviBmp = (VIDEOINFO *)pmt->Format();

		m_lVidWidth  = pviBmp->bmiHeader.biWidth;
		m_lVidHeight = abs(pviBmp->bmiHeader.biHeight);
		m_lVidPitch  = (m_lVidWidth * 3 + 3) & ~(3); // We are forcing RGB24

		assert(m_pRenderTarget);
		m_pRenderTarget->onDelete();
		if( m_pRenderTarget->onCreate(m_lVidWidth,m_lVidHeight,m_fmt) == TRUE)
			return S_OK;

		return E_FAIL;
	}
	else if(*pmt->FormatType() == FORMAT_VideoInfo2)
	{
          VIDEOINFOHEADER2* pVinfo = (VIDEOINFOHEADER2*)pmt->pbFormat;
		  m_lVidWidth  = pVinfo->bmiHeader.biWidth;
		  m_lVidHeight = abs(pVinfo->bmiHeader.biHeight);
		  m_lVidPitch  = (m_lVidWidth * 3 + 3) & ~(3); // We are forcing RGB24

		  assert(m_pRenderTarget);
		  m_pRenderTarget->onDelete();
		  if( m_pRenderTarget->onCreate(m_lVidWidth,m_lVidHeight,m_fmt) == TRUE)
			  return S_OK;

		  return E_FAIL;
		  
	}

}


//-----------------------------------------------------------------------------
// DoRenderSample: A sample has been delivered. Copy it to the texture.
//-----------------------------------------------------------------------------
HRESULT xDSVideoRenderer::EndOfStream()
{
	if(m_pRenderTarget)
		m_pRenderTarget->onStop();
	return CBaseRenderer::EndOfStream();
}


HRESULT xDSVideoRenderer::RGBRenderSample(IMediaSample * pSample)
{
	unsigned char* pDstData = NULL;
	unsigned char* pSrcData = NULL;

	IVideoRenderTarget::VRT_LockRect RTRect;

	int lDstPitch = 0;
	int lSrcPitch = 0;
	int Bbp = 0;


	CheckPointer(pSample,E_POINTER);
	pSample->GetPointer( &pSrcData);


	if(m_pRenderTarget == NULL)
		return S_OK;

	if(m_pRenderTarget->fmt() == VIF_RGB && m_pRenderTarget->direct_update() )
	{
          if( true == m_pRenderTarget->update(pSrcData , m_lVidWidth , m_lVidHeight, m_lVidPitch , VIF_RGB) );
		     return S_OK;
	}

	m_pRenderTarget->onStartRender();
	m_pRenderTarget->lock(RTRect);

	pDstData = RTRect.mPixel;




	lDstPitch = RTRect.mPicth ;
	lSrcPitch = m_lVidPitch;


	unsigned char* pSrcLine = pSrcData;
	unsigned char* pDstLine = pDstData;
	Bbp = RTRect.mBytePerPixel;

	if(m_pRenderTarget->flipY())
	{
		pDstLine += (RTRect.mHeight - 1) * lDstPitch;
		lDstPitch =- lDstPitch;
	}

	int _R = 0 , _G = 1 , _B = 2, _A = 3;
	if(m_pRenderTarget->flipRGBA() )
	{
		_B = 0;	_G = 1;	_R = 2;	_A = 3;		
	}

	if(pSrcData == NULL || pDstData == NULL)
	{
#ifdef _DEBUG
		OutputDebugString(L"CVideoRender DoSampler 的Buffer为空\n");
#endif
		m_pRenderTarget->unlock();
		return E_FAIL;
	}

	if(Bbp == 3)
	{
		for(int y = 0 ; y < RTRect.mHeight ; ++y)
		{
			for(int x = 0 ; x < RTRect.mWidth ; ++x)
			{
				pDstLine[3 * x + _R] = pSrcLine[3 * x + 0];
				pDstLine[3 * x + _G] = pSrcLine[3 * x + 1];
				pDstLine[3 * x + _B] = pSrcLine[3 * x + 2];
			}
			pSrcLine += lSrcPitch;
			pDstLine += lDstPitch;
		}

	}
	else if(Bbp == 4)
	{
		for(int y = 0 ; y < RTRect.mHeight ; ++y)
		{
			for(int x = 0 ; x < RTRect.mWidth ; ++x)
			{
				pDstLine[4 * x + _R] = pSrcLine[3 * x + 0];
				pDstLine[4 * x + _G] = pSrcLine[3 * x + 1];
				pDstLine[4 * x + _B] = pSrcLine[3 * x + 2];
				pDstLine[4 * x + _A] = m_Alpha;
			}
			pSrcLine += lSrcPitch;
			pDstLine += lDstPitch;
		}

	}
	else
	{
		m_pRenderTarget->unlock();
		m_pRenderTarget->onEndRender(false);
		return E_FAIL;
	}
	//解锁 Video的RenderTarget
	m_pRenderTarget->unlock();
	m_pRenderTarget->onEndRender(true);
	return S_OK;
}

HRESULT xDSVideoRenderer::FillRGBWithYUY2(IMediaSample * pSample)
{
	unsigned char* pDstData = NULL;
	unsigned char* pSrcData = NULL;

	IVideoRenderTarget::VRT_LockRect RTRect;

	int lDstPitch = 0;
	int lSrcPitch = 0;
	int Bbp = 0;


	if(m_pRenderTarget == NULL)
		return S_OK;

	m_pRenderTarget->onStartRender();
	m_pRenderTarget->lock(RTRect);

	pDstData = RTRect.mPixel;

	CheckPointer(pSample,E_POINTER);
	pSample->GetPointer( &pSrcData);

	//===============
    int dataLen = pSample->GetActualDataLength();
	memcpy(pDstData , pSrcData , dataLen);
	m_pRenderTarget->unlock();
	m_pRenderTarget->onEndRender(true);

	return S_OK;

	//==============
	lDstPitch = RTRect.mPicth ;
	lSrcPitch = m_lVidWidth * 2;//m_lVidPitch;


	Bbp = RTRect.mBytePerPixel;
	unsigned char* pSrcLine = pSrcData;
	unsigned char* pDstLine = pDstData;// + (RTRect.mHeight - 1) * RTRect.mWidth * Bbp;

	if(m_pRenderTarget->flipY())
	{
		pDstLine += (RTRect.mHeight - 1) * lDstPitch;
		lDstPitch =- lDstPitch;
	}

	int _R = 0 , _G = 1 , _B = 2, _A = 3;
	if(m_pRenderTarget->flipRGBA() )
	{
		_B = 0;	_G = 1;	_R = 2;	_A = 3;		
	}

	if(pSrcData == NULL || pDstData == NULL)
	{
#ifdef _DEBUG
		OutputDebugString(L"CVideoRender DoSampler 的Buffer为空\n");
#endif
		m_pRenderTarget->unlock();
		return E_FAIL;
	}

	if(Bbp == 3)
	{
		for(int y = 0 ; y < RTRect.mHeight ; ++y)
		{
			for(int x = 0 ; x < RTRect.mWidth ; ++x)
			{
				pDstLine[3 * x + _R] = pSrcLine[2 * x];//pSrcLine[3 * x + 0];
				pDstLine[3 * x + _G] = pSrcLine[2 * x];//pSrcLine[3 * x + 1];
				pDstLine[3 * x + _B] = pSrcLine[2 * x];//pSrcLine[3 * x + 2];
			}
			pSrcLine += lSrcPitch;
			pDstLine += lDstPitch;
		}

	}
	else if(Bbp == 4)
	{
		for(int y = 0 ; y < RTRect.mHeight ; ++y)
		{
			for(int x = 0 ; x < RTRect.mWidth ; ++x)
			{
				pDstLine[4 * x + _R] = pSrcLine[2 * x];//pSrcLine[3 * x + 0];
				pDstLine[4 * x + _G] = pSrcLine[2 * x];//pSrcLine[3 * x + 1];
				pDstLine[4 * x + _B] = pSrcLine[2 * x];//pSrcLine[3 * x + 2];
				pDstLine[4 * x + _A] = m_Alpha;
			}
			pSrcLine += lSrcPitch;
			pDstLine += lDstPitch;
		}

	}
	else
	{
		m_pRenderTarget->unlock();
		m_pRenderTarget->onEndRender(false);
		return E_FAIL;
	}
	//解锁 Video的RenderTarget
	m_pRenderTarget->unlock();
	m_pRenderTarget->onEndRender(true);
	return S_OK;
}

HRESULT xDSVideoRenderer::FillRGBWithYV12(IMediaSample * pSample)
{
	unsigned char* pDstData = NULL;
	unsigned char* pSrcData = NULL;

	IVideoRenderTarget::VRT_LockRect RTRect;

	int lDstPitch = 0;
	int lSrcPitch = 0;
	int Bbp = 0;

	m_pRenderTarget->onStartRender();
	m_pRenderTarget->lock(RTRect);
	pDstData = RTRect.mPixel;
	CheckPointer(pSample,E_POINTER);
	pSample->GetPointer( &pSrcData);
	lDstPitch = RTRect.mPicth ;
	lSrcPitch = m_lVidWidth ;//m_lVidPitch;
	Bbp = RTRect.mBytePerPixel;
	unsigned char* pSrcLine = pSrcData;
	unsigned char* pDstLine = pDstData ;//+ (RTRect.mHeight - 1) * RTRect.mWidth * Bbp;


	if(m_pRenderTarget->flipY())
	{
		pDstLine += (RTRect.mHeight - 1) * lDstPitch;
		lDstPitch =- lDstPitch;
	}

	int _R = 0 , _G = 1 , _B = 2, _A = 3;
	if(m_pRenderTarget->flipRGBA() )
	{
		_B = 0;	_G = 1;	_R = 2;	_A = 3;		
	}
	if(pSrcData == NULL || pDstData == NULL)
	{
#ifdef _DEBUG
		OutputDebugString(L"CVideoRender DoSampler 的Buffer为空\n");
#endif
		m_pRenderTarget->unlock();
		return E_FAIL;
	}

	if(Bbp == 3)
	{
		for(int y = 0 ; y < RTRect.mHeight ; ++y)
		{
			for(int x = 0 ; x < RTRect.mWidth ; ++x)
			{
				pDstLine[3 * x + _R] = pSrcLine[x];//pSrcLine[3 * x + 0];
				pDstLine[3 * x + _G] = pSrcLine[x];//pSrcLine[3 * x + 1];
				pDstLine[3 * x + _B] = pSrcLine[x];//pSrcLine[3 * x + 2];
			}					 
			pSrcLine += lSrcPitch;
			pDstLine += lDstPitch;
		}

	}
	else if(Bbp == 4)
	{
		for(int y = 0 ; y < RTRect.mHeight ; ++y)
		{
			for(int x = 0 ; x < RTRect.mWidth ; ++x)
			{
				pDstLine[4 * x + _R] = pSrcLine[x];//pSrcLine[3 * x + 0];
				pDstLine[4 * x + _G] = pSrcLine[x];//pSrcLine[3 * x + 1];
				pDstLine[4 * x + _B] = pSrcLine[x];//pSrcLine[3 * x + 2];
				pDstLine[4 * x + _A] = m_Alpha;
			}
			pSrcLine += lSrcPitch;
			pDstLine += lDstPitch;
		}

	}
	else
	{
		m_pRenderTarget->unlock();
		m_pRenderTarget->onEndRender(false);
		return E_FAIL;
	}
	//解锁 Video的RenderTarget
	m_pRenderTarget->unlock();
	m_pRenderTarget->onEndRender(true);
	return S_OK;
}

HRESULT xDSVideoRenderer::YV12RenderSample(IMediaSample * pSample)
{
    if(m_pRenderTarget == NULL)
		return E_FAIL;

	if(m_pRenderTarget->fmt() == VIF_RGBA || m_pRenderTarget->fmt()  == VIF_RGB)
	{
		return FillRGBWithYV12(pSample);
	}
	else
	{
		if(m_pRenderTarget->direct_update())
		{
			unsigned char* pSrcData = NULL;
			CheckPointer(pSample,E_POINTER);
			pSample->GetPointer( &pSrcData);
			m_pRenderTarget->onStartRender();
            m_pRenderTarget->update( pSrcData , m_lVidWidth , m_lVidHeight , m_lVidPitch , VIF_YV12);
			m_pRenderTarget->onEndRender(true);
		}
		else
		{
			IVideoRenderTarget::VRT_LockRect RTRect;
			unsigned char* pSrcData = NULL;
			CheckPointer(pSample,E_POINTER);
			pSample->GetPointer( &pSrcData);
			m_pRenderTarget->onStartRender();
			m_pRenderTarget->lock(RTRect);
            int rtBufSize = RTRect.mHeight * RTRect.mPicth;
			int srcBufSize = m_lVidPitch * m_lVidHeight;

			int sizeToCp = rtBufSize > srcBufSize ? srcBufSize : rtBufSize;
			memcpy(RTRect.mPixel , pSrcData , sizeToCp );
			m_pRenderTarget->unlock();
			m_pRenderTarget->onEndRender(true);
		}
	}
	return S_OK;
}

HRESULT xDSVideoRenderer::YUY2RenderSample(IMediaSample * pSample)
{
	if(m_pRenderTarget == NULL)
		return E_FAIL;

	if(m_pRenderTarget->fmt() == VIF_RGBA || m_pRenderTarget->fmt()  == VIF_RGB)
	{
		return FillRGBWithYUY2(pSample);
	}
	else
	{
		if(m_pRenderTarget->direct_update())
		{
			unsigned char* pSrcData = NULL;
			CheckPointer(pSample,E_POINTER);
			pSample->GetPointer( &pSrcData);
			m_pRenderTarget->onStartRender();
			m_pRenderTarget->update( pSrcData , m_lVidWidth , m_lVidHeight , m_lVidPitch , VIF_YUY2);
			m_pRenderTarget->onEndRender(true);
		}
		else
		{
			IVideoRenderTarget::VRT_LockRect RTRect;
			unsigned char* pSrcData = NULL;
			CheckPointer(pSample,E_POINTER);
			pSample->GetPointer( &pSrcData);
			m_pRenderTarget->onStartRender();
			m_pRenderTarget->lock(RTRect);
			int rtBufSize = RTRect.mHeight * RTRect.mPicth;
			int srcBufSize = m_lVidPitch * m_lVidHeight;

			int sizeToCp = rtBufSize > srcBufSize ? srcBufSize : rtBufSize;
			memcpy(RTRect.mPixel , pSrcData , sizeToCp );
			m_pRenderTarget->unlock();
			m_pRenderTarget->onEndRender(true);
		}
	}
	return S_OK;
}


HRESULT xDSVideoRenderer::DoRenderSample( IMediaSample * pSample )
{
	if(m_pRenderer) return  (this->*m_pRenderer)(pSample);
	return E_FAIL;
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

