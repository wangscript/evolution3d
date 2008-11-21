#include "../../DotNet/Evol3D.Net/xDotNetUlti.h"
namespace VirtualCommunity
{
	using namespace xEvol3D;
	ref class xVirtualCmtyEnv
	{
		xEvol3DEnv^           m_Evol3D;
		xRenderApi^           m_RenderApi;
		xFont^                m_LogFont;
		xCamera^              m_2DCamera;
		xCamera^              m_3DCamera;
		bool                  m_bEditorMode;
		xHelperDrawable^      m_GridPlane;
		xBaseTextureManager^  m_TextureManager;
	public:
		xVirtualCmtyEnv();
		~xVirtualCmtyEnv();
		bool       bEditorMode();
		bool       Init(int hWnd);
		bool       Unit();
		bool       OnIdle();
		bool       OnResize(int w , int h);
	};
}