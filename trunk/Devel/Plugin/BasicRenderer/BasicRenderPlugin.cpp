#include <BaseLib/xEvol3DBaseInc.h>
#include <Application/xPluginMgr.h>
#include <BaseLib/xStringHash.h>

#include "./include/BasicRenderer.h"
using namespace XEvol3D;



class xBasicRendererPlugin : public IPluginObject
{

    class xBasicRendererCreator : public IRendererCreator 
    {
    public:
        virtual const wchar_t* name() const 
        {
            return _BASICRENDER_NAME_;
        }
        virtual IBaseRenderer*     createInstance(IRenderApi* pRenderApi) const
        {
            return new xBasicRenderer(pRenderApi);
        }
    };

    xBasicRendererCreator* m_RenderCreator;
public:
    xBasicRendererPlugin()
    {
        m_RenderCreator = NULL;
    }

    bool start(const wchar_t* pluginName , const wchar_t* pluginPath)
    {
        if(m_RenderCreator == NULL)
        {
            m_RenderCreator = new xBasicRendererCreator;
            xRendererManager::singleton()->registeRenderer(m_RenderCreator);
        }
        return true;
    }
    void info(xPluginInfo* info)
    {
        wcsncpy(info->m_Description ,  L"Basic Renderer Plugin name=[BasicRenderer]" , 256);
        info->m_ID = xStringHash(L"Basic Renderer Plugin");
        wcsncpy(info->m_Name , L"Basic Renderer Plugin" , 32);
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
    void* createObject(const wchar_t* objName, const wchar_t * arg)
    {
        return NULL;
    }
    const wchar_t** objectList()
    {
        static wchar_t* objectList[1] = {NULL};
        return (const wchar_t**)objectList;
    }
};

extern "C" _declspec(dllexport) IPluginObject* PLUGIN_ENTRYPOINT()
{
    static xBasicRendererPlugin gPluginObject;
    return &gPluginObject;
}