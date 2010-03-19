#include "stdafx.h"
#include <BaseLib/xEvol3DAPI.h>
#include <BaseLib/xXmlDocument.h>
#include <OperationSys/xOperationSys.h>
#include "xDIRECT3D9API.h"
#include "Image/PixelFormatAssit.h"
#include "fs/xFileSystem.h"
#include "xD3DTLEnviroment.h"
//#include "../xDIRECT3D9DllApi.h"

#include "xD3DInputBuffer.h"
#include "xD3D9Sampler.h"
#include "xD3D9RazState.h"
#include "xD3D9DepthStencilState.h"
#include "xD3D9BlendState.h"
#include "xD3D9FileTexture.h"
#include "xD3D9RenderTarget.h"
#include "xD3D9Texture2D.h"
#include "xD3D9TextureCube.h"
#include "xD3D9InputBuffer.h"
#include "xD3D9InputAssembler.h"

#include "xD3D9RenderView.h"
#include "xD3D9Shader.h"
#include "xD3D9GPUProgram.h"


BEGIN_NAMESPACE_XEVOL3D

#define GUID_SELECT_MODE(_value)  if( m_RenderMode == eRenderMode_Select ) return _value;

void xDeviceLostResManager::AddResource(IRenderApiDeviceLostListener* pResource)
{
    m_vDeviceLostListenners[pResource] = pResource;

}

void xDeviceLostResManager::RemoveResource(IRenderApiDeviceLostListener* pResource)
{
    mapDeviceLostListenners::iterator pos = m_vDeviceLostListenners.find(pResource);
    if(pos == m_vDeviceLostListenners.end() )
        return ;
    m_vDeviceLostListenners.erase(pos);
    return ;
}

void  xDeviceLostResManager::SetDevice(xD3D9RenderApi* pApi , IDirect3DDevice9* pDevice)
{
    m_pD3D9RenderApi = pApi;
    m_pDevice        = pDevice;
}

void  xDeviceLostResManager::BeginDeviceLost()
{
    mapDeviceLostListenners::iterator pos = m_vDeviceLostListenners.begin();
    for(; pos != m_vDeviceLostListenners.end() ; pos ++)
    {
        IRenderApiDeviceLostListener* pListener = pos->second;
        pListener->onDeviceReset(false , IRenderApiDeviceLostListener::eReset_Begin);
    }
    
}

void  xDeviceLostResManager::EndDeviceLost()
{
    mapDeviceLostListenners::iterator pos = m_vDeviceLostListenners.begin();
    for(; pos != m_vDeviceLostListenners.end() ; pos ++)
    {
        IRenderApiDeviceLostListener* pListener = pos->second;
        pListener->onDeviceReset(false , IRenderApiDeviceLostListener::eReset_Finish);
    }
}
//=================================================
//
//=================================================
void xD3D9RenderApi::AddDeviceLostListener(IRenderApiDeviceLostListener* pResource)
{
    m_DeviceLostResManager.AddResource(pResource);
}

void xD3D9RenderApi::RemoveDeviceLostListener(IRenderApiDeviceLostListener* pResource)
{
    m_DeviceLostResManager.RemoveResource(pResource);

}


xDeviceLostResManager* xD3D9RenderApi::GetDeviceLostResMgr()
{
     return &m_DeviceLostResManager;
}

bool xD3D9RenderApi::__beforeResetDevice()
{
    m_DeviceLostResManager.BeginDeviceLost();
    m_FontManager.unload();
    //m_GpuProgramMgr.unload();
    for(size_t i = 0  ; i < m_TextureMagrs.size() ; i ++)
    {
        m_TextureMagrs[i]->unload();
    }
    return true;
}

bool xD3D9RenderApi::__afterDeviceRest()
{
    m_DeviceLostResManager.EndDeviceLost();
    return true;
}

bool xD3D9RenderApi::__resetDevice(int w, int h)
{
    return true;
}



bool xD3D9RenderApi::__unloadAllResource()
{
    return true;
}

bool xD3D9RenderApi::__destory()
{
    XSAFE_RELEASEOBJECT(m_RenderWindow);
    XSAFE_RELEASEOBJECT(m_pDefZStencilState);
    XSAFE_RELEASEOBJECT(m_pDefBlender);
    XSAFE_RELEASEOBJECT(m_pDefRazState);
    XSAFE_RELEASEOBJECT(m_pDefSampler);

    XSAFE_RELEASE(m_pD3DDevice);
    return true;

}

bool xD3D9RenderApi::__afterCreateDevice()
{
    m_pDefBlender = new xD3D9BlendState(this);
    m_pDefRazState = new xD3D9RazState(this);
    m_pDefZStencilState = new xD3D9ZStencilState(this);
    m_pDefSampler   = new xD3D9Sampler(this);
    for(int i = 0 ; i < Texture_StageMax ; i ++)
    {
        m_Samplers[i] = NULL;
    }

    m_2DZValue = 0.0f;
    m_pDef2DRectEnv = new xD3D2DRectEnv(this);
    m_pDef2DRect    = new xD3D2DRect(m_pDef2DRectEnv);

    m_DeviceLostResManager.SetDevice(this , getDevice() );
    return true;   
}


END_NAMESPACE_XEVOL3D