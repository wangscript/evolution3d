#include "xVRCBaseInclude.h"
#include "./Scene.Net/vrcScene.h"
namespace VirtualCommunity
{
	using namespace xEvol3DNet;
	ref class xVirtualCmtyEnv
	{
	internal:
		xEvol3DEnv^           m_Evol3D;
		xRenderApi^           m_RenderApi;
		xFont^                m_LogFont;
		xEvol3DNet::xCamera^  m_2DCamera;
		xEvol3DNet::xCamera^  m_3DCamera;
		bool                  m_bEditorMode;
		xHelperDrawable^      m_GridPlane;
		xBaseTextureManager^  m_TextureManager;
		xVRSceneManager^      m_Scene;
		System::Threading::Mutex^ m_Locker;
	public:
		xVirtualCmtyEnv();
		~xVirtualCmtyEnv();
		bool       bEditorMode();
		bool       Init(int hWnd);
		bool       Unit();
		bool       OnIdle();
		bool       OnResize(int w , int h);
		bool       OnLeftMouseClick(int x , int y);
	};
}