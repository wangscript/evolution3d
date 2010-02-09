#ifndef _XHELPER_OBJECT_COMMON_INC_H_
#define _XHELPER_OBJECT_COMMON_INC_H_

#ifdef _WIN32
#ifdef   PLUGIN_EXPORTS 
#define  _XEVOL_PLUGIN_API_   __declspec( dllexport )
#else
#define  _XEVOL_PLUGIN_API_   __declspec( dllimport )
#endif
#else
#undef    _XEVOL_PLUGIN_API_  
#define   _XEVOL_PLUGIN_API_  
#endif


#ifdef _XEVOL_BUILD_STATIC_PLUGIN_
#undef    _XEVOL_PLUGIN_API_  
#define   _XEVOL_PLUGIN_API_  
#endif

#endif
