#ifdef _WIN32
#include <Windows.h>
#else
typedef unsigned long HWND;
#endif

#define MAX_RENDER_TARGET 4

#include <GL/glew.h>