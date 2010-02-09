#include <BaseLib/xEvol3DBaseInc.h>
#include <Application/xPluginMgr.h>
#include <BaseLib/xStringHash.h>

#include "./include/csRenderer.h"
using namespace XEvol3D;



class xScriptRendererPlugin : public IPluginObject
{

    class csRendererCreator : public IRendererCreator 
    {
    public:
        virtual const wchar_t* name() const 
        {
            return _CSCRIPTRENDER_NAME_;
        }
        virtual IBaseRenderer*     createInstance(IRenderApi* pRenderApi) const
        {
            return new csRenderer(pRenderApi);
        }
    };

    csRendererCreator* m_RenderCreator;
public:
    xScriptRendererPlugin()
    {
        m_RenderCreator = NULL;
    }

    bool start(const wchar_t* pluginName , const wchar_t* pluginPath)
    {
        if(m_RenderCreator == NULL)
        {
            m_RenderCreator = new csRendererCreator;
            xRendererManager::singleton()->registeRenderer(m_RenderCreator);
        }
        return true;
    }
    void info(xPluginInfo* info)
    {
        wcsncpy(info->m_Description ,  L"Script Renderer Plugin name=[csRenderer]" , 256);
        info->m_ID = xStringHash(L"Script Renderer Plugin");
        wcsncpy(info->m_Name , L"Script Renderer Plugin" , 32);
        info->m_Type =ePT_Renderer;
    }

    bool stop()
    {
        if(m_RenderCreator)
        {
            xRendererManager::singleton()->unregisteRenderer(m_RenderCreator);
            delete m_RenderCreator;
            m_RenderCreator = NULL;
        }

        return true;
    }

    unsigned int nObject()
    {
        return 0;
    }
    void* createObject(const wchar_t* objName, const void * arg)
    {
        return NULL;
    }
    const wchar_t** objectList()
    {
        static wchar_t* objectList[1] = {NULL};
        return (const wchar_t**)objectList;
    }
};


#ifndef _XEVOL_BUILD_STATIC_PLUGIN_
extern "C" _declspec(dllexport) IPluginObject* PLUGIN_ENTRYPOINT()
{
    static xScriptRendererPlugin gPluginObject;
    return &gPluginObject;
}
#else

#endif
