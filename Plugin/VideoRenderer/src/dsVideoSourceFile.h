
typedef unsigned long DWORD_PTR, *PDWORD_PTR;

#include <Atlbase.h>

#include <windows.h>
#include <streams.h>
#include <dshow.h>
#include "../include/xVideoSource.h"
#include "xVideoRenderer.h"
#include <string>

class CVideoSourceFile : public  IVideoSource
{
public:
    CVideoSourceFile();
    ~CVideoSourceFile();

	class CVideoControlInterface
	{
	public:
		IGraphBuilder*  m_pGraphBuilder;          // GraphBuilder
		IMediaControl*  m_pMediaControl;          // Media Control
		IMediaPosition* m_pMediaPosition;         // Media Position
		IMediaEvent*    m_pMediaEvent;            // Media Event

		CVideoControlInterface();
		~CVideoControlInterface();
		bool build();
		bool Release();
	};

	CVideoControlInterface m_VCtrller;
public:

	//²¥·Å½Ó¿Ú

	 bool           load_a(const char* media,bool isNoAudio = FALSE);
	 bool           load(const wchar_t* media,bool isNoAudio = FALSE);
	 bool           unload();
	 bool           play();
	 bool           stop();
	 bool           pause();
	 bool           resume();
	 bool           is_playing();
	 bool           set_pos(double second);
	 double         get_pos();
	 bool           set_rate(double rate);
	 double         get_rate();
	 double         get_len();
	 bool           is_over();

	 bool           create(){return true;};
	 bool           Release();

	 void*               get_interface(const char* interfaceString);
	 void                set_alpha(unsigned char alpha);
	 unsigned char       get_alpha();
	 bool                check_state(bool needloop);
	 long                get_state();
	 const char*         media_name(){return m_FileName.c_str();}


	 void                set_auto_draw(bool flag = true){;};
	 bool                get_auto_draw(){return true;};

	void                 set_renderTarget(IVideoRenderTarget* pRenderTarget);
	IVideoRenderTarget*  get_renderTarget();
protected:

	IBaseFilter*        m_pColorSpaceFilter;
	IVideoRenderTarget* m_pRenderTarget ;
	xDSVideoRenderer*   m_pVideoRenderer;

    HRESULT m_CreateHResult; 

	bool    m_isNoAudio;

	std::string  m_FileName;

	IBaseFilter* m_SrcFilter;
   
};
