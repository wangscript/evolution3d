// dllmain.h : Declaration of module class.

class CxEvol3DWebViewerModule : public CAtlDllModuleT< CxEvol3DWebViewerModule >
{
public :
	DECLARE_LIBID(LIBID_xEvol3DWebViewerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_XEVOL3DWEBVIEWER, "{A34DC166-3ECB-4910-B45E-69ADC5E9B533}")
};

extern class CxEvol3DWebViewerModule _AtlModule;
