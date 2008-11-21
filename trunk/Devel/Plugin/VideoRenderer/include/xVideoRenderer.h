#ifndef __XREAL_CAMERA_H__
#define __XREAL_CAMERA_H__

#include "xVideoSource.h"
#include "xVideoRenderTarget.h"

extern "C"
{
	IVideoSource*   createVideoSource(eVideoRendererType eType);
	void            releaseVideoSource(IVideoSource* pRenderer);
};


#endif
