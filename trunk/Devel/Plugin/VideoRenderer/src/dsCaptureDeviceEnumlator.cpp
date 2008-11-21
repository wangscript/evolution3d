#pragma warning (disable : 4995)
#pragma warning (disable : 4311)
#include "windows.h"
#include <assert.h>
#include "dsVideoSourceCapture.h"
#include "../include/xVideoRenderTarget.h"

CVideoSourceCapature::CCaptureDeviceEnumlator::CCaptureDeviceEnumlator()
{
    m_nDevices = 0;
	m_pDevices = NULL;
	m_CurDevice = -1;
	m_IsAllDeviceInfoLoaded = FALSE;
	m_pCaptureSrcFilter = NULL;
}


CVideoSourceCapature::CCaptureDeviceEnumlator::~CCaptureDeviceEnumlator()
{
    m_IsAllDeviceInfoLoaded = FALSE;
}

bool CVideoSourceCapature::CCaptureDeviceEnumlator::find_allDevice()
{
	releaseAllDevices();

	ICreateDevEnum *pDevEnum = NULL;
	IEnumMoniker *pEnum = NULL;

	// Create the System Device Enumerator.
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
		CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, 
		reinterpret_cast<void**>(&pDevEnum));
	if (SUCCEEDED(hr))
	{
		// Create an enumerator for the video capture category.
		hr = pDevEnum->CreateClassEnumerator(
			CLSID_VideoInputDeviceCategory,
			&pEnum, 0);

		if(FAILED(hr) || pEnum == NULL)
			return FALSE;
	}
	else
	{
		return false;
	}

	std::vector<IMoniker*> tempMonikers;

	IMoniker *pMoniker = NULL;
	while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
	{
		tempMonikers.push_back(pMoniker);
	}

	int nDevice =  (int)tempMonikers.size();
	m_pDevices = new sCaptureDevice[nDevice];


	int iDevice  = 0;
	for(int i = 0 ; i < nDevice ; ++i)
	{
		pMoniker = tempMonikers[i];

		IPropertyBag *pPropBag;
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, 
			(void**)(&pPropBag));

		if (FAILED(hr))
		{
			pMoniker->Release();
			continue;  // Skip this one, maybe the next one will work.
		} 
		// Find the description or friendly name.
		VARIANT varName;
		VariantInit(&varName);
		hr = pPropBag->Read(L"Description", &varName, 0);
		if (FAILED(hr))
		{
			hr = pPropBag->Read(L"FriendlyName", &varName, 0);
		}
		if (SUCCEEDED(hr))
		{
			int nLen = (int)wcslen(varName.bstrVal);
			wcsncpy(m_pDevices[iDevice].m_Description,varName.bstrVal,nLen);
			m_pDevices[iDevice].m_Description[nLen] = 0;
			VariantClear(&varName); 
		}
		else
		{
			continue;
		}
 
		VARIANT varID;
		VariantInit(&varID);
		hr = pPropBag->Read(L"DevicePath", &varID, 0);
		if ( ! FAILED(hr))
		{
			int len = (int)wcslen(varID.bstrVal);
			wcsncpy(m_pDevices[iDevice].m_IDString,varID.bstrVal,len);
			m_pDevices[iDevice].m_IDString[len] = 0;
			VariantClear(&varID);
		}
		pPropBag->Release();

		iDevice ++;
		m_pMonikers.push_back(pMoniker);
	}

	m_nDevices = (int)m_pMonikers.size();

	tempMonikers.clear();
	m_IsAllDeviceInfoLoaded = (m_nDevices>0);

	if(m_nDevices == 0)
	{
		releaseAllDevices();
		return false;
	}
	return true;

}

IBaseFilter* CVideoSourceCapature::CCaptureDeviceEnumlator::get_curDeviceFilter()
{
	  if(m_pCaptureSrcFilter == NULL)
		  configue();
	  return m_pCaptureSrcFilter;
}

void CVideoSourceCapature::CCaptureDeviceEnumlator::releaseAllDevices()
{
	m_CurDevice = -1;
   for(int i = 0 ; i < m_nDevices ; ++i)
   {
	   m_pMonikers[i]->Release();
   }
   m_pMonikers.clear();
   delete [] m_pDevices;
   m_pDevices = NULL;
   m_nDevices = 0;
   m_IsAllDeviceInfoLoaded = FALSE;
}

void* CVideoSourceCapature::CCaptureDeviceEnumlator::set_curDevice(int index)
{
    if(m_IsAllDeviceInfoLoaded == FALSE)
		find_allDevice();

	if(index < 0 || index >= m_nDevices)
		return NULL;

	IMoniker* pMoniker = m_pMonikers[index];
	HRESULT hr = pMoniker->BindToObject(0,0,IID_IBaseFilter, (void**)&m_pCaptureSrcFilter);
	if(FAILED(hr))
	{
		m_pCaptureSrcFilter = NULL;
		return NULL;
	}

	m_CurDevice = index;
	return (void*)m_pCaptureSrcFilter;
		
}


void* CVideoSourceCapature::CCaptureDeviceEnumlator::set_curDevice(const wchar_t* IDString)
{
    for(int i = 0 ; i < m_nDevices ; i++)
	{
		if( wcscmp(m_pDevices[i].m_IDString,IDString) == 0 )
			return set_curDevice(i);
	}
	return NULL;
}


void CVideoSourceCapature::CCaptureDeviceEnumlator::configue()
{
	//ÔÝÊ±ÓÃµÄ¡£
      set_curDevice(0);
}


sCaptureDevice* CVideoSourceCapature::CCaptureDeviceEnumlator::get_curDevice()
{
	if(m_CurDevice == -1)
		return NULL;
	return m_pDevices + m_CurDevice;
}  


sCaptureDevice* CVideoSourceCapature::CCaptureDeviceEnumlator::get_AllDevice()
{
	return m_pDevices;
}


int             CVideoSourceCapature::CCaptureDeviceEnumlator::get_DevieCount()
{
    return m_nDevices;
}


int                 CVideoSourceCapature::get_DeviceDimsNumber()
{
   return 0;
}

sCaptureDeviceDim*  CVideoSourceCapature::get_DeviceDims()
{
    return NULL;
}

void                CVideoSourceCapature::releaseDeviceDims(sCaptureDeviceDim* Dims)
{

}

sCaptureDeviceDim   CVideoSourceCapature::get_CurDeviceDim()
{
	sCaptureDeviceDim dim;
	dim.m_Height = 0;
	dim.m_Rate = 0;
	dim.m_Width = 0;

	if(m_VCtrller.m_pCaptureGraphBuilder == NULL)
		return dim;


	IAMStreamConfig *pConfig = NULL;
	HRESULT hr = m_VCtrller.m_pCaptureGraphBuilder->FindInterface(
		&PIN_CATEGORY_CAPTURE, // Preview pin.
		0,    // Any media type.
		m_VCtrller.m_pCaptureSrcFilter, // Pointer to the capture filter.
		IID_IAMStreamConfig, (void**)&pConfig);
	AM_MEDIA_TYPE* type;
	pConfig->GetFormat(&type);

	if( (type->cbFormat < sizeof (VIDEOINFOHEADER)) ||
		(type->pbFormat == NULL))
	{
		DeleteMediaType(type);
		return dim;
	}

	VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)type->pbFormat;
	// pVih contains the detailed format information.
	LONG lWidth = pVih->bmiHeader.biWidth;
	LONG lHeight = pVih->bmiHeader.biHeight;

	dim.m_Width = lWidth; 
	dim.m_Height  = lHeight;
	dim.m_Rate = 30;

	DeleteMediaType(type);
	return dim;
}

bool CVideoSourceCapature::set_CurDeviceDim(int w,int h,int rate)
{
	IAMStreamConfig *pConfig = NULL;
	if(m_VCtrller.m_pCaptureGraphBuilder == NULL)
		return FALSE;

	HRESULT hr = m_VCtrller.m_pCaptureGraphBuilder->FindInterface(
		&PIN_CATEGORY_CAPTURE, // Preview pin.
		&MEDIATYPE_Interleaved,    // Any media type.
		m_VCtrller.m_pCaptureSrcFilter, // Pointer to the capture filter.
		IID_IAMStreamConfig, (void**)&pConfig);

	if(FAILED(hr))
	{
		
		{
			char buf[256];
			sprintf(buf,"ÅäÖÃ³ß´çµÄÊ±ºòÊ§°Ü[MEDIATYPE_Interleaved]: %d = %d  HRESULT = %d\n",w , h , hr);
			OutputDebugStringA(buf);
		}

		hr = m_VCtrller.m_pCaptureGraphBuilder->FindInterface(
			&PIN_CATEGORY_CAPTURE, // Preview pin.
			&MEDIATYPE_Video,    // Any media type.
			m_VCtrller.m_pCaptureSrcFilter, // Pointer to the capture filter.
			IID_IAMStreamConfig, (void**)&pConfig);
		if(FAILED(hr) || pConfig == 0)
		{
			char buf[256];
			sprintf(buf,"ÅäÖÃ³ß´çµÄÊ±ºòÊ§°Ü[MEDIATYPE_Video]: %d = %d  HRESULT = %d\n",w , h , hr);
			OutputDebugStringA(buf);
	
		}

	}

	if(pConfig == NULL)
	{
		hr = m_VCtrller.m_pCaptureGraphBuilder->FindInterface(
			&PIN_CATEGORY_CAPTURE, // Preview pin.
			0,    // Any media type.
			m_VCtrller.m_pCaptureSrcFilter, // Pointer to the capture filter.
			IID_IAMStreamConfig, (void**)&pConfig);
	}

	int iCount = 0, iSize = 0;

	if(FAILED(hr) || pConfig == 0)
	{
		char buf[256];
		sprintf(buf,"ÅäÖÃ³ß´çµÄÊ±ºò³¹µ×Ê§°Ü: %d = %d  HRESULT = %d\n",w , h , hr);
		OutputDebugStringA(buf);
		return false;
	}

	hr = pConfig->GetNumberOfCapabilities(&iCount, &iSize);

	if (iSize == sizeof(VIDEO_STREAM_CONFIG_CAPS))
	{
		// Use the video capabilities structure.

		for (int iFormat = 0; iFormat < iCount; iFormat++)
		{
			VIDEO_STREAM_CONFIG_CAPS scc;
			AM_MEDIA_TYPE *pmtConfig;
			hr = pConfig->GetStreamCaps(iFormat, &pmtConfig, (BYTE*)&scc);
			if (SUCCEEDED(hr))
			{
				if( (pmtConfig->cbFormat < sizeof (VIDEOINFOHEADER)) ||
					(pmtConfig->pbFormat == NULL))
					continue;

				VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)pmtConfig->pbFormat;
				// pVih contains the detailed format information.
				LONG lWidth = pVih->bmiHeader.biWidth;
				LONG lHeight = pVih->bmiHeader.biHeight;

				/* Examine the format, and possibly use it. */
				if ((pmtConfig->majortype == MEDIATYPE_Video) &&
					//(pmtConfig->subtype == MEDIASUBTYPE_RGB24) &&
					(pmtConfig->formattype == FORMAT_VideoInfo) &&
					lWidth==w && lHeight == h)					 
				{
					stop();
					tearDown();
					hr = pConfig->SetFormat(pmtConfig);
					if(FALSE == buildPreview() )
					{
						DeleteMediaType(pmtConfig);
						pConfig->Release();
						return false;
					}

					if(FALSE == play() )
					{
						DeleteMediaType(pmtConfig);
						pConfig->Release();
						return false;
					}

					
					if(!FAILED(hr))
					{
						DeleteMediaType(pmtConfig);
						pConfig->Release();
						return true;
					}
				
				}
				// Delete the media type when you are done.
				DeleteMediaType(pmtConfig);
			}
		}
	}
	pConfig->Release();
	return false;
}

bool CVideoSourceCapature::set_CurDeviceDim(sCaptureDeviceDim dim)
{
   return set_CurDeviceDim(dim.m_Width,dim.m_Height,dim.m_Rate);
}

bool CVideoSourceCapature::buildPreview()
{
	HRESULT hr ;
	IBaseFilter* pVideoRenderer = &m_VideoRenderer;
	pVideoRenderer->AddRef();
	if (FAILED(hr = m_VCtrller.m_pGraphBuilder->AddFilter(pVideoRenderer, L"XREALVIDEOCAPTURERENDERER")))
	{
		return false;
	}

	hr = m_VCtrller.m_pCaptureGraphBuilder->RenderStream (&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video,m_VCtrller.m_pCaptureSrcFilter, NULL, pVideoRenderer);
	if (FAILED(hr))
	{
		return false;
	}

	m_VCtrller.build();
	return true;
}

void CVideoSourceCapature::NukeDownstream(IBaseFilter *pf)
{
	IPin *pP=0, *pTo=0;
	ULONG u;
	IEnumPins *pins = NULL;
	PIN_INFO pininfo;

	if (!pf)
		return;

	HRESULT hr = pf->EnumPins(&pins);
	pins->Reset();

	while(hr == NOERROR)
	{
		hr = pins->Next(1, &pP, &u);
		if(hr == S_OK && pP)
		{
			pP->ConnectedTo(&pTo);
			if(pTo)
			{
				hr = pTo->QueryPinInfo(&pininfo);
				if(hr == NOERROR)
				{
					if(pininfo.dir == PINDIR_INPUT)
					{
						NukeDownstream(pininfo.pFilter);
						m_VCtrller.m_pGraphBuilder->Disconnect(pTo);
						m_VCtrller.m_pGraphBuilder->Disconnect(pP);
						m_VCtrller.m_pGraphBuilder->RemoveFilter(pininfo.pFilter);
					}
					pininfo.pFilter->Release();
				}
				pTo->Release();
			}
			pP->Release();
		}
	}

	if(pins)
		pins->Release();
}

void CVideoSourceCapature::tearDown()
{
	m_VCtrller.release_ctrl();
	NukeDownstream(m_VCtrller.m_pCaptureSrcFilter);
}

bool    CVideoSourceCapature::BuildDeviceDimInfos(bool forceRebuild)
{
	if(m_VCtrller.m_pCaptureGraphBuilder == NULL)
		return false;

	if(m_AllTypes.size() > 0)
	{
		if(!forceRebuild)
			return true;
	}
	m_AllTypes.clear();

	IAMStreamConfig *pConfig = NULL;
	HRESULT hr = m_VCtrller.m_pCaptureGraphBuilder->FindInterface(
		&PIN_CATEGORY_CAPTURE, // Preview pin.
		0,    // Any media type.
		m_VCtrller.m_pCaptureSrcFilter, // Pointer to the capture filter.
		IID_IAMStreamConfig, (void**)&pConfig);

	int iCount = 0, iSize = 0;
	hr = pConfig->GetNumberOfCapabilities(&iCount, &iSize);

	if (iSize == sizeof(VIDEO_STREAM_CONFIG_CAPS))
	{
		// Use the video capabilities structure.

		for (int iFormat = 0; iFormat < iCount; iFormat++)
		{
			VIDEO_STREAM_CONFIG_CAPS scc;
			AM_MEDIA_TYPE *pmtConfig;
			hr = pConfig->GetStreamCaps(iFormat, &pmtConfig, (BYTE*)&scc);
			if (SUCCEEDED(hr))
			{
				if( (pmtConfig->cbFormat >= sizeof (VIDEOINFOHEADER)) ||
					(pmtConfig->pbFormat != NULL))
				{
					m_AllTypes.push_back(*pmtConfig);
				}
				DeleteMediaType(pmtConfig);
			}
		}
	}
	pConfig->Release();
	return true;
}


