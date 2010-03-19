#ifndef _XEVOL_DIRECT3D9_RESOURCE_API_
#define _XEVOL_DIRECT3D9_RESOURCE_API_
#include <map>
#include <stack>
#include <RenderApi/xRenderApiImp.h>
#include <BaseLib/xStringHash.h>
#include <RenderApi/xShaderManager.h>
#include <RenderAPI/xRenderStateObjManager.h>
#include "RenderAPI/xMatrixContext.h"

BEGIN_NAMESPACE_XEVOL3D
class xD3D9RenderApi;
class ID3D9DeviceLostListener : public IRenderApiDeviceLostListener
{
public:
    virtual void onDeviceReset(bool bNewDevice , eResetAction _Action) = 0;
};

class xDeviceLostResManager
{
    IDirect3DDevice9* m_pDevice;
    xD3D9RenderApi*   m_pD3D9RenderApi;
public:
    void AddResource(IRenderApiDeviceLostListener* pResource);
    void RemoveResource(IRenderApiDeviceLostListener* pResource);
    void SetDevice(xD3D9RenderApi* pApi , IDirect3DDevice9* pDevice);
    void BeginDeviceLost();
    void EndDeviceLost();
protected:
    typedef std::map<IRenderApiDeviceLostListener* , IRenderApiDeviceLostListener*> mapDeviceLostListenners;
    mapDeviceLostListenners m_vDeviceLostListenners;
};

template <typename TResource> class TD3D9DeviceLostListener : public ID3D9DeviceLostListener
{
    TResource* m_pResource;
public:
    TD3D9DeviceLostListener(){ m_pResource = NULL ; }
    void SetResource(TResource* pResource){ m_pResource = pResource ; }
    void onDeviceReset(bool bNewDevice , eResetAction _Action)
    {
        return m_pResource->onDeviceReset(bNewDevice , _Action);
    }
};

#define DECLARE_DEVICE_LOST(_Type) \
    TD3D9DeviceLostListener<_Type>    m_DeviceLostHandler; \

#define REGISTER_DEVICE_LOST_RESOURCE(pD9Api)                     \
{                                                                 \
    m_DeviceLostHandler.SetResource(this) ;                       \
    xDeviceLostResManager*  pMgr = pD9Api->GetDeviceLostResMgr(); \
    pMgr->AddResource(&m_DeviceLostHandler);                      \
}                                                                 \

#define REMOVE_DEVICE_LOST_RESOURCE(pD9Api)                     \
{                                                                 \
    m_DeviceLostHandler.SetResource(this) ;                       \
    xDeviceLostResManager*  pMgr = pD9Api->GetDeviceLostResMgr(); \
    pMgr->RemoveResource(&m_DeviceLostHandler);                   \
}                                                                 \


END_NAMESPACE_XEVOL3D

#endif

