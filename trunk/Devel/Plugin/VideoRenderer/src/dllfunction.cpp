#include "dsVideoSourceCapture.h"
#include "dsVideoSourceFile.h"

extern "C"
{
	__declspec(dllexport) IVideoSource* createVideoSource(eVideoRendererType eType)
	{
		if(eType == eVRT_CAPTURE)
			return new CVideoSourceCapature;
		return new CVideoSourceFile;
	}
	__declspec(dllexport) void releaseVideoSource(IVideoSource* pRenderer)
	{
		pRenderer->stop();
		pRenderer->unload();
		pRenderer->release();
		CVideoSourceCapature* pCapRender;
		pCapRender = dynamic_cast<CVideoSourceCapature*>(pRenderer);
		CVideoSourceFile* pFileRender;
		pFileRender = dynamic_cast<CVideoSourceFile*>(pRenderer);

		if(pFileRender != NULL)
		{
			delete pFileRender;
		}
		else if( pCapRender != NULL )
		{
			delete pCapRender;
		}
	}

};
