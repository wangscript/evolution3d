/*
XReal3D Rendering Engine . (http://gforge.osdn.net.cn/projects/xreal3d/) 
Stanly.Lee 2006


This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "xRE_VideoTexture.h"
#include "xVideoRenderer.h"
#include "RenderAPI/xRenderView.h"
#include "RenderAPI/xShaderManager.h"
IMPL_BASE_OBJECT_CLASSID(xR_VideoTexture, IBaseTexture)
IMPL_BASE_OBJECT_CLASSID(xR_VideoControl, IVideoTextureCtrl)


xVideoCanvas::xVideoCanvas()
{
	m_pTexture = NULL;
}

xVideoCanvas::~xVideoCanvas()
{

}

void xVideoCanvas::Release()
{
	delete this;
}

void xVideoCanvas::thread_lock()
{
	m_pTexture->thread_lock();
}

void xVideoCanvas::thread_unlock()
{
	m_pTexture->thread_unlock();
}

bool xVideoCanvas::onDelete()
{
	return m_pTexture->onDelete();
}

VideoImageFormat  xVideoCanvas::fmt()
{
	return m_pTexture->fmt();
}

bool xVideoCanvas::flipY()
{
	VideoImageFormat _fmt = fmt();
	if(_fmt == VIF_RGBA)
	{
		return true;
	}
	return true;
}

bool xVideoCanvas::flipRGBA()
{
	VideoImageFormat _fmt = fmt();
	if(_fmt == VIF_RGBA)
	{
		return true;
	}
	return false;
}
bool  xVideoCanvas::direct_update()
{
	return m_pTexture->direct_update();
}

bool  xVideoCanvas::update(void* data , int w , int h , int picth , VideoImageFormat _fmt)
{
    return m_pTexture->OnUpdate(data , w , h , picth , _fmt);
}

bool  xVideoCanvas::check_fmt(VideoImageFormat vfmt)
{
	return m_pTexture->check_fmt(vfmt);
}
//设备被创建的时候，也就是和摄像头或者视频解码器连接的时候，会收到这个消息
bool xVideoCanvas::onCreate(int Width,int Height, VideoImageFormat vifmt)
{
	return m_pTexture->onCreate(Width , Height , vifmt);

}

void xVideoCanvas::onStop()
{
	return m_pTexture->onStop();
}

void* xVideoCanvas::lock(IVideoRenderTarget::VRT_LockRect& lockRect)
{
	return m_pTexture->lock(lockRect);
}

IVideoRenderTarget::VRT_LockRect* xVideoCanvas::lock()
{
	return m_pTexture->lock();
}
void* xVideoCanvas::unlock()
{
	return m_pTexture->unlock();
}

void xVideoCanvas::onStartRender()
{
	return m_pTexture->onStartRender();
}

void xVideoCanvas::onEndRender(bool isRendered)
{
	return m_pTexture->onEndRender(isRendered);
}


const    xObjectClassID&   xR_VideoControl::classID()
{ 
	return this->ms_CLASSID; 
} 

xR_VideoControl::xR_VideoControl()
{
	m_RefCount = 11111111;
}

void* xR_VideoControl::queryObject(const xObjectClassID& _id)
{
	if( this->ms_CLASSID.isType(_id))
	{
		this->AddRef(); 
		return this;
	} 
	if(xR_VideoTexture::ms_CLASSID.isType(_id) )
	{
		m_pVideoTexture->AddRef();
		return m_pVideoTexture;
	}
	return 0;
} 

bool xR_VideoControl::isType(const    xObjectClassID& _id)
{ 
	if( this->ms_CLASSID.isType(_id))
	{
		return true;
	}
	return false;
}

bool xR_VideoControl::load(const wchar_t* videFile)
{
     if(m_pVideoRender)
		 return m_pVideoRender->load(videFile) != FALSE;
	 return false;
}

bool xR_VideoControl::play()
{
	if(m_pVideoRender)
		return m_pVideoRender->play() != FALSE;
	return false;
}

bool xR_VideoControl::stop()
{
	if(m_pVideoRender)
		return m_pVideoRender->stop() != FALSE;
	return false;
}

bool xR_VideoControl::pause()
{
	if(m_pVideoRender)
		return m_pVideoRender->pause() != FALSE;
	return false;
}

bool xR_VideoControl::resume()
{
	if(m_pVideoRender)
		return m_pVideoRender->resume() != FALSE;
	return false;
}

bool xR_VideoControl::seekable()
{
	if(m_pVideoRender)
		return true;
	return false;
}

bool xR_VideoControl::seek(int timeToSeek)
{
	if(m_pVideoRender)
		return m_pVideoRender->set_pos(timeToSeek/1000.0f) != FALSE;
	return false;
}

int  xR_VideoControl::totalTime() 
{
	if(m_pVideoRender)
		return (int)(m_pVideoRender->get_len() * 1000);
	return 0;
}
bool  xR_VideoControl::isPlaying()
{
	if(m_pVideoRender)
	{
		return m_pVideoRender->is_playing();
	}
	return false;
}

int xR_VideoControl::get_pos()
{
	if(m_pVideoRender)
	{
		return (int)(m_pVideoRender->get_pos() * 1000);
	}
	return 0;
}

IBaseTexture*   xR_VideoControl::mergePlane()
{
	if(m_pVideoTexture)
	{
		return m_pVideoTexture->mergePlane();
	}
	return NULL;
}

bool   xR_VideoControl::installPlane()
{
	if(m_pVideoTexture)
	{
		return m_pVideoTexture->installPlane();
	}
	return false;
}

IBaseTexture*   xR_VideoControl::plane(size_t i)
{
	if(m_pVideoTexture)
	{
		return m_pVideoTexture->plane(i);
	}
	return NULL;
}

size_t  xR_VideoControl::nPlane()
{
	if(m_pVideoTexture)
	{
		m_pVideoTexture->nPlane();
	}
	return 0;
}
//===============================================
unsigned long  xR_VideoTexture::memUsage()
{
	unsigned int _usg = 0;
	if(m_p2DTexture) _usg += m_p2DTexture->memUsage() ; 
	if(m_pYUVTexture[0]) _usg += m_pYUVTexture[0]->memUsage() ; 
	if(m_pYUVTexture[1]) _usg += m_pYUVTexture[1]->memUsage() ; 
	if(m_pYUVTexture[2]) _usg += m_pYUVTexture[2]->memUsage() ; 
	return _usg;
}


const    xObjectClassID&   xR_VideoTexture::classID()
{ 
	return this->ms_CLASSID; 
} 

void* xR_VideoTexture::queryObject(const xObjectClassID& _id)
{
	if( xR_VideoControl::ms_CLASSID.isType(_id))
	{
		m_VideoCtrl.AddRef();
		return &m_VideoCtrl;
	} 
	if(xR_VideoTexture::ms_CLASSID.isType(_id) )
	{

		this->AddRef(); 
		return this;
	}
	return 0;
} 

bool xR_VideoTexture::isType(const    xObjectClassID& _id)
{ 
	if( this->ms_CLASSID.isType(_id))
	{
		return true;
	} 
    return false;
}


ePIXEL_FORMAT  xR_VideoTexture::format()
{
	if(m_fmt == VIF_RGBA)
		return PIXELFORMAT_R8G8B8A8;
	if(m_fmt == VIF_YV12)
		return PIXELFORMAT_YV12;
	return PIXELFORMAT_None;
}

bool xR_VideoTexture::validate()
{
	if(m_bDirty)
	{
		thread_lock();
		mergePlane();
		thread_unlock();
        m_bDirty = false;
	}
	return m_p2DTexture != NULL;
}

bool xR_VideoTexture::desc(xTextureDesc& desc)
{
	return m_p2DTexture->desc(desc);
}

bool xR_VideoTexture::load(const wchar_t* fileName , unsigned long  arg)
{
   return m_VideoCtrl.m_pVideoRender->load(fileName) != FALSE;
}


bool xR_VideoTexture::unload()
{
	m_VideoCtrl.m_pVideoRender->stop();
	m_VideoCtrl.m_pVideoRender->unload();
	__freeData();
	return true;
}

bool xR_VideoTexture::load(const wchar_t* fileName , const unsigned int8* buf , size_t bufLen, unsigned long arg)
{
	return m_VideoCtrl.m_pVideoRender->load(fileName) != FALSE;
}
bool  xR_VideoTexture::check_fmt(VideoImageFormat vfmt)
{
	if(vfmt == VIF_RGBA)
		return true;
	if(vfmt == VIF_YV12)
		return false ;//m_bEnableYV12;
	if(vfmt == VIF_YUY2)
		return false ;//m_bEnableYUY2;
	return false;
}

xR_VideoTexture::xR_VideoTexture(IRenderApi*pRenderApi , bool bFileRenderer , bool bEnableYV12, bool bEnableYUV2) : IBaseTexture(pRenderApi )
{
	m_bDirty      = false;
	m_bEnableYV12 = bEnableYV12;
	m_bEnableYUY2 = bEnableYUV2;
	m_RefCount = 1;
	m_pRenderApi = pRenderApi;
	if(bFileRenderer)
	{
		m_VideoCtrl.m_pVideoRender = createVideoSource(eVRT_FILE);
	}
	else
	{
		m_VideoCtrl.m_pVideoRender = createVideoSource(eVRT_CAPTURE);
	}
	m_VideoCtrl.m_pVideoRender->set_renderTarget(&m_Canvas);
	
	m_VideoCtrl.m_pVideoTexture = this;
	m_Canvas.m_pTexture = this;
    
	m_pYUVMergeShader = NULL;
	m_pMergeRenderView = NULL;
	m_p2DTexture = NULL;
	m_pYUVTexture[0] = NULL;
	m_pYUVTexture[1] = NULL;
	m_pYUVTexture[2] = NULL;

	InitializeCriticalSection(&m_Locker);

	m_Width = 0;
	m_Height = 0;
}



xR_VideoTexture::~xR_VideoTexture()
{
	m_VideoCtrl.m_pVideoRender->stop();
	m_VideoCtrl.m_pVideoRender->unload();
	m_VideoCtrl.m_pVideoRender->Release();
	releaseVideoSource(m_VideoCtrl.m_pVideoRender);
	m_VideoCtrl.m_pVideoRender = NULL;
	__freeData();	
}

void  xR_VideoTexture::__freeData()
{
    XSAFE_RELEASEOBJECT(m_pMergeRenderView);
	XSAFE_RELEASEOBJECT(m_p2DTexture);
	XSAFE_RELEASEOBJECT(m_pYUVTexture[0]);
	XSAFE_RELEASEOBJECT(m_pYUVTexture[1]);
	XSAFE_RELEASEOBJECT(m_pYUVTexture[2]);
	m_Width = 0;
	m_Height = 0;
}

void* xR_VideoTexture::handle()
{ 
	if(m_fmt == VIF_RGBA)
	{
		thread_lock();
		void* ret = m_p2DTexture->handle();
		thread_unlock();
		return ret;
	}
	else if(m_fmt == VIF_YV12)
	{
		thread_lock();
		IRenderTarget* pRT = (IRenderTarget*)m_pMergeRenderView->renderTarget(0);
		IBaseTexture* pTexture = pRT->toTexture();
		thread_unlock();
		return pTexture->handle();
	}
	else if(m_fmt == VIF_YUY2)
	{
		return NULL;
	}
	return NULL;
}

void xR_VideoTexture::__InstallYV12Env()
{
	if(m_pMergeRenderView == NULL)
	{
		m_pMergeRenderView = m_pRenderApi->createRenderView(m_Width , m_Height , true);
		m_pMergeRenderView->createRenderTarget(1,PIXELFORMAT_R8G8B8A8,false,true);
		m_pYUVMergeShader = m_pRenderApi->gpuProgramManager()->load(L"FSQuad",L"YV12ToRGB",NULL).getResource();
	}
	m_pRenderApi->pushGpuProgram(m_pYUVMergeShader);
	m_pRenderApi->pushRenderView(m_pMergeRenderView);
}
void  xR_VideoTexture::__MergeYV12Texture()
{
	__InstallYV12Env();
	xColor_4f cl(1.0f,1.0f,1.0,1.0f);
	float dest_Rect[4] = {-1.0f,-1.0f,(float)m_Width,(float)m_Height};
	IBlenderState* pBlendState = m_pRenderApi->getBlendState();
	IDepthStencilState* pZState = m_pRenderApi->getDepthStencilState();
	m_pRenderApi->setTexture(L"YVideoTexture" , m_pYUVTexture[0]);
	m_pRenderApi->setTexture(L"UVideoTexture" , m_pYUVTexture[1]);
	m_pRenderApi->setTexture(L"VVideoTexture" , m_pYUVTexture[2]);
	m_pRenderApi->setDepthStencilState(m_pRenderApi->createDepthStencilState(L"Overlay"));
	m_pRenderApi->setBlendState(NULL);
	m_pRenderApi->drawRectf(m_pYUVTexture[0],dest_Rect , cl);
	m_pRenderApi->setBlendState(pBlendState);
	m_pRenderApi->setDepthStencilState(pZState);
	m_pRenderApi->setTexture(L"YVideoTexture" , NULL);
	m_pRenderApi->setTexture(L"UVideoTexture" , NULL);
	m_pRenderApi->setTexture(L"VVideoTexture" , NULL);
	m_pRenderApi->popGpuProgram();
	m_pRenderApi->popRenderView();
}

IBaseTexture*   xR_VideoTexture::mergePlane()
{
	if(m_fmt == VIF_RGBA)
	{
		return m_p2DTexture;
	}
	else if(m_fmt == VIF_YV12)
	{
		thread_lock();
		__MergeYV12Texture();
		thread_unlock();
	}
	else if(m_fmt == VIF_YUY2)
	{
		//Merge YUY2
		return NULL;
	}
	return NULL;
}

bool   xR_VideoTexture::installPlane()
{
	if(m_fmt == VIF_RGBA)
	{
		m_pRenderApi->setTexture(L"RGBVideoTexture" , m_p2DTexture);
		return true;
	}
	else if(m_fmt == VIF_YV12)
	{
		m_pRenderApi->setTexture(L"YVideoTexture" , m_pYUVTexture[0]);
		m_pRenderApi->setTexture(L"UVideoTexture" , m_pYUVTexture[1]);
		m_pRenderApi->setTexture(L"VVideoTexture" , m_pYUVTexture[2]);
		return true;
	}
	else if(m_fmt == VIF_YUY2)
	{
		m_pRenderApi->setTexture(L"YUY2VideoTexture" , m_pYUVTexture[0]);
		return true;
	}
	return false;
}

IBaseTexture*   xR_VideoTexture::plane(size_t i)
{
	if(m_fmt == VIF_RGBA)
	{
		return m_p2DTexture;
	}
	else if(m_fmt == VIF_YV12)
	{
		return m_pYUVTexture[i];
	}
	else if(m_fmt == VIF_YUY2)
	{
		return m_pYUVTexture[0];
	}
	return NULL;
}

size_t  xR_VideoTexture::nPlane()
{
	if(m_fmt == VIF_YV12)	return 3;
	return 1;
}

//Texture For 2D Texture
bool xR_VideoTexture::lock(eLockPolicy lockPolicy, xTextureLockArea& lockInfo, int mipmapLevel , int arraySlice)
{
	if(m_p2DTexture)
	{
		return m_p2DTexture->lock(lockPolicy , lockInfo , mipmapLevel , arraySlice);
	}
	return false;
}
bool xR_VideoTexture::saveToFile(const wchar_t* fileName)
{
	if(m_p2DTexture)
	{
		return m_p2DTexture->saveToFile(fileName);
	}
	return true;
}
bool xR_VideoTexture::unlock(xTextureLockArea& lockInfo) 
{
	if(m_p2DTexture)
	{
		return m_p2DTexture->unlock(lockInfo);
	}
	return false;
}


void xR_VideoTexture::thread_lock()
{
    EnterCriticalSection(&m_Locker);
}

void xR_VideoTexture::thread_unlock()
{
	LeaveCriticalSection(&m_Locker);
}


//设备被删除的时候，收到这个通知
bool xR_VideoTexture::onDelete()
{
	 XSAFE_RELEASEOBJECT(m_pMergeRenderView);
	 m_pYUVMergeShader = NULL;
	 XSAFE_RELEASEOBJECT(m_p2DTexture);
	 XSAFE_RELEASEOBJECT(m_pYUVTexture[0]);
	 XSAFE_RELEASEOBJECT(m_pYUVTexture[1]);
	 XSAFE_RELEASEOBJECT(m_pYUVTexture[2]);
	 return true;
}

bool xR_VideoTexture::direct_update()
{
	if(m_fmt == VIF_YV12)
	    return true;
	return false;
}

bool xR_VideoTexture::OnUpdate(void* data , int w , int h , int picth , VideoImageFormat fmt)
{
	if(m_fmt == VIF_YV12)
	{
		picth = w;
		m_pRenderApi->lock();
		thread_lock();
		xTextureLockArea _lockArea;
		//Fill Y Texture;
		m_pYUVTexture[0]->lock(eLock_WriteDiscard , _lockArea );
		const char* srcLine = (const char*)data;
		char* dstLine       = (char*)_lockArea.m_pixels;
        for(int y = 0 ; y < h ; y ++)
		{
			memcpy(dstLine , srcLine , picth);
			dstLine += _lockArea.m_picth;
			srcLine += picth;
		}
		m_pYUVTexture[0]->unlock(_lockArea);


		//Fill U Texture;
		m_pYUVTexture[1]->lock(eLock_WriteDiscard , _lockArea );
		srcLine = (const char*)data + w * h;
		dstLine       = (char*)_lockArea.m_pixels;
		for(int y = 0 ; y < h/2 ; y ++)
		{
			memcpy(dstLine , srcLine , picth/2);
			dstLine += _lockArea.m_picth;
			srcLine += picth/2;
		}
		m_pYUVTexture[1]->unlock(_lockArea);


		//Fill V Texture;
		m_pYUVTexture[2]->lock(eLock_WriteDiscard , _lockArea );
		srcLine = (const char*)data + w * h + w/2 * h/2;
		dstLine       = (char*)_lockArea.m_pixels;
		for(int y = 0 ; y < h/2 ; y ++)
		{
			memcpy(dstLine , srcLine , picth/2);
			dstLine += _lockArea.m_picth;
			srcLine += picth/2;
		}
		m_pYUVTexture[2]->unlock(_lockArea);
		thread_unlock();
		m_pRenderApi->unlock();
		m_bDirty = true;
		return true;
	}
	return false;
}

//设备被创建的时候，也就是和摄像头或者视频解码器连接的时候，会收到这个消息
bool xR_VideoTexture::onCreate(int Width,int Height, VideoImageFormat vifmt)
{
	if(vifmt == VIF_YUY2) 
		vifmt = m_bEnableYUY2 ? VIF_YUY2 : VIF_RGBA;

	if(vifmt == VIF_YV12) 
		vifmt = m_bEnableYV12 ? VIF_YV12 : VIF_RGBA;

	m_fmt = vifmt;
    onDelete();
	m_LockInfo.mBufferHeight = Height;
	m_LockInfo.mBufferWidth  = Width;
	m_LockInfo.mBytePerPixel = 4;
    m_LockInfo.mHeight       = Height;
	m_LockInfo.mWidth        = Width;
	m_LockInfo.mPicth        = 4 *  Width;
	m_LockInfo.mPixel        = NULL;
	m_Width  = Width;
	m_Height = Height;
	if(m_fmt == VIF_RGBA)
	{
		m_p2DTexture = m_pRenderApi->createTexture(Width , Height , PIXELFORMAT_R8G8B8A8,false);
	}
	else if(m_fmt == VIF_YV12)
	{
		m_pYUVTexture[0] = m_pRenderApi->createTexture(Width   , Height   , PIXELFORMAT_LUMINANCE8,false);
		m_pYUVTexture[1] = m_pRenderApi->createTexture(Width/2 , Height/2 , PIXELFORMAT_LUMINANCE8,false);
		m_pYUVTexture[2] = m_pRenderApi->createTexture(Width/2 , Height/2 , PIXELFORMAT_LUMINANCE8,false);
	}
	else if(m_fmt == VIF_YUY2)
	{
		m_pYUVTexture[0] = m_pRenderApi->createTexture(Width , Height , PIXELFORMAT_R8G8B8A8,false);
	}
	return TRUE;
}

//当播放器或者摄像头停止的时候，收到这个消息
void xR_VideoTexture::onStop()
{

}

//缓冲区管理
void* xR_VideoTexture::lock(IVideoRenderTarget::VRT_LockRect& lockRect)
{
	m_pRenderApi->lock();
	thread_lock();	
	m_p2DTexture->lock(eLock_WriteDiscard , m_LockArea);
	lockRect = m_LockInfo;
	lockRect.mPicth = m_LockArea.m_picth;
	lockRect.mPixel = (unsigned char *)m_LockArea.m_pixels;
    return NULL;
}

IVideoRenderTarget::VRT_LockRect* xR_VideoTexture::lock()
{
	thread_lock();
	m_pRenderApi->lock();
	m_p2DTexture->lock(eLock_WriteDiscard , m_LockArea);
	m_LockInfo.mPicth = m_LockArea.m_picth;
	m_LockInfo.mPixel = (unsigned char *)m_LockArea.m_pixels;
	return NULL;
}

void* xR_VideoTexture::unlock()
{
	m_p2DTexture->unlock(m_LockArea);
	m_pRenderApi->unlock();
	thread_unlock();
	return NULL;
}

//开始绘制一帧图象的时候收到这个消息
void xR_VideoTexture::onStartRender()
{

}
//绘制完图象的时候，收到这个消息， isRendered为true的时候，表示绘制成功，否则失败
void xR_VideoTexture::onEndRender(bool isRendered)
{

}