#ifndef __XEVOL3D_KIDS_RENDERER_API_H__
#define __XEVOL3D_KIDS_RENDERER_API_H__


#ifdef _WIN32
  #ifdef   _XEVOL_BUILD_PLUGIN_ 
     #define  _XEVOL_RENDERER_API_   __declspec( dllexport )
  #else
     #define  _XEVOL_RENDERER_API_   __declspec( dllimport )
  #endif

#else
  #undef    _XEVOL_RENDERER_API_  
  #define   _XEVOL_RENDERER_API_ 
#endif

#ifdef _XEVOL_BUILD_STATIC_PLUGIN_
#undef   _XEVOL_RENDERER_API_   
#define  _XEVOL_RENDERER_API_ 
#endif

#ifdef _XEVOL_KIDS_RENDERER_EXPORT_
#undef   _XEVOL_RENDERER_API_   
#define  _XEVOL_RENDERER_API_   __declspec( dllexport )
#endif

#ifdef _XEVOL_KIDS_RENDERER_STATIC_
#undef   _XEVOL_RENDERER_API_   
#define  _XEVOL_RENDERER_API_  
#endif

#endif