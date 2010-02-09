#include "nvRenderer.h"
#include "BaseLib/xLogger.h"
#pragma comment(lib,"NvApi\\nvapi.lib")

BEGIN_NAMESPACE_XEVOL3D

xNvRenderer::xNvRenderer()
{
    NvAPI_Status ret = NvAPI_Initialize();
	if(NVAPI_OK != ret)
	{
         XEVOL_LOG(eXL_ERROR_NORMAL , L"Initialize NvApi Failed\n");
		 return ;
	}

	NvAPI_Status nvapiStatus;
	m_nvDisplayCount = 0;
	// Enumerate all display handles
	for(int i=0,nvapiStatus=NVAPI_OK; nvapiStatus == NVAPI_OK; i++)
	{
		nvapiStatus = NvAPI_EnumNvidiaDisplayHandle(i, &m_hDisplay[i]);

		if (nvapiStatus == NVAPI_OK) m_nvDisplayCount++;
	}
	for(int i=0; i< m_nvDisplayCount; i++)
	{
		NvAPI_GetDisplayDriverVersion(m_hDisplay[i] , &m_DrvVersion);
		
	}

	
}

xNvRenderer::~xNvRenderer()
{

}


END_NAMESPACE_XEVOL3D
