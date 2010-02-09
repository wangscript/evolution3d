#pragma once 
#include "xGL2RenderView.h"
#include <GL/glew.h>

BEGIN_NAMESPACE_XEVOL3D
xGL2RenderWindow::xGL2RenderWindow(HWND hWnd , IRenderApi* pRenderApi)
:xGL2RenderView(pRenderApi)
,m_WindowRT(pRenderApi , m_TexDesc)
{
    m_hWnd = hWnd;
    m_hDC = GetDC(m_hWnd);
    PIXELFORMATDESCRIPTOR pfd=
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,
        32,
        0,0,0,0,0,0,0
    };
    int n=ChoosePixelFormat(m_hDC,&pfd); 
    SetPixelFormat(m_hDC,n,&pfd);
    m_hRC =wglCreateContext(m_hDC); 
    wglMakeCurrent(m_hDC,m_hRC); 
    glColor4f(1.0,1.0,1.0,1.0f);
    glClearColor(0.0,0.0,0.0,.50);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glewInit();


    

    RECT _rect ;
    GetClientRect(m_hWnd , &_rect);
    int w = _rect.right - _rect.left;
    int h = _rect.bottom - _rect.top;

    //=============================
    m_TexDesc.m_depth = 1;
    m_TexDesc.m_fmt = PIXELFORMAT_R8G8B8A8;
    m_TexDesc.m_height = h;
    m_TexDesc.m_nArraySize = 1;
    m_TexDesc.m_width = w;

    XEVOL_LOG(eXL_DEBUG_TIPS , "OpenGL RenderWindow inited [w = %d h = %d]\n" , w , h );
}

xGL2RenderWindow::~xGL2RenderWindow()
{
    destory();
}

void xGL2RenderWindow::makeCurrent()
{
    wglMakeCurrent(m_hDC,m_hRC); 
}

void xGL2RenderWindow::swapBuffer()
{
    HDC hdc = wglGetCurrentDC();
    SwapBuffers(hdc); 
    glFlush();
}

bool xGL2RenderWindow::desc(xTextureDesc& _desc)
{
    _desc = m_TexDesc;
    return true;
}

bool xGL2RenderWindow::destory()
{
    if(m_hRC)
    {
        wglMakeCurrent(m_hDC,NULL);
        wglDeleteContext(m_hRC); 
        ReleaseDC(m_hWnd,m_hDC);
    }
    m_hRC = NULL;
    m_hDC = NULL;
    return m_hRC != NULL;
}


bool xGL2RenderWindow::resize(int w , int h)
{
    if( w == 0 || h == 0)
    {
        RECT _rect ;
        GetClientRect(m_hWnd , &_rect);
        w = _rect.right - _rect.left;
        h = _rect.bottom - _rect.top;
    }
    m_TexDesc.m_height = h;
    m_TexDesc.m_width  = w;
    return true;
}

int xGL2RenderWindow::startIdx()
{ 
    return 1;
}

END_NAMESPACE_XEVOL3D
