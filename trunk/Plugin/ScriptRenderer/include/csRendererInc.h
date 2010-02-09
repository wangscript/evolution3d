#ifndef  _BASIC_RENDERER_INCLUDE_
#define _BASIC_RENDERER_INCLUDE_


#ifdef _WIN32
#ifdef   BASRENDERER_EXPORTS 
#define  _XEVOL_BASRD_API_   __declspec( dllexport )
#else
#define  _XEVOL_BASRD_API_   __declspec( dllimport )

#ifdef   _DEBUG 
//#pragma  comment(lib,"xEvol3D_Debug.lib")
#else
//#pragma  comment(lib,"xEvol3D_Release.lib")
#endif

#endif
#else
#undef    _XEVOL_BASRD_API_  
#define   _XEVOL_BASRD_API_  
#endif

#ifdef _LINUX
#undef    _XEVOL_BASRD_API_  
#define   _XEVOL_BASRD_API_  
#endif

#ifdef BASRENDERER_STATIC
#undef   _XEVOL_BASRD_API_   
#define  _XEVOL_BASRD_API_ 
#endif


#endif