#ifndef _NV_API_RENDERER_H_
#define _NV_API_RENDERER_H_
#include "Application/xPluginMgr.h"
#include "../NvApi/nvapi.h"
BEGIN_NAMESPACE_XEVOL3D

class xNvRenderer
{
	NvDisplayHandle           m_hDisplay[NVAPI_MAX_PHYSICAL_GPUS * 2];
	int                       m_nvDisplayCount;
	NV_DISPLAY_DRIVER_VERSION m_DrvVersion;
public:
	xNvRenderer();
	~xNvRenderer();	
};

END_NAMESPACE_XEVOL3D

#endif