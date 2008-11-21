#ifndef __XVIDEO_RENDERER_H__
#define __XVIDEO_RENDERER_H__
typedef unsigned long DWORD_PTR, *PDWORD_PTR;

#include <Atlbase.h>

#include <windows.h>
#include <streams.h>
#include <dshow.h>
#include "../include/xVideoSource.h"

class xDSVideoRenderer;
enum VideoImageFormat;
typedef HRESULT (xDSVideoRenderer::*RENDER_SAMPLE)(IMediaSample *pMediaSample);

class xDSVideoRenderer : public CBaseVideoRenderer
{
	VideoImageFormat     m_fmt;
public:
	xDSVideoRenderer();
	~xDSVideoRenderer();
public:
	void                 set_alpha(unsigned char alpha){m_Alpha = alpha;}
	unsigned char        get_alpha(){return m_Alpha;};
	void                 set_renderTarget(IVideoRenderTarget* pRenderTarget){m_pRenderTarget = pRenderTarget;}
	IVideoRenderTarget*  get_renderTarget(){return m_pRenderTarget ; }
protected:
	//DirectShow专用的媒体播放接口
	HRESULT             CheckMediaType(const CMediaType *pmt );     // Format acceptable?
	HRESULT             SetMediaType(const CMediaType *pmt );       // Video format notification
	HRESULT             DoRenderSample(IMediaSample *pMediaSample); // New video sample
	HRESULT             EndOfStream();

	HRESULT             RGBRenderSample(IMediaSample * pSample);
	HRESULT             YV12RenderSample(IMediaSample * pSample);
	HRESULT             YUY2RenderSample(IMediaSample * pSample);
	HRESULT             FillRGBWithYV12(IMediaSample * pSample);
	HRESULT             FillRGBWithYUY2(IMediaSample * pSample);
protected:
	IVideoRenderTarget* m_pRenderTarget;
	unsigned char       m_Alpha;
	RENDER_SAMPLE       m_pRenderer;

	LONG                m_lVidWidth;	// Video width
	LONG                m_lVidHeight;	// Video Height
	LONG                m_lVidPitch;	// Video Pitch
	HRESULT             m_CreateHResult; 
};


#endif
