#include "pch.h"

#ifndef GLEW_STATIC
#  define GLEW_STATIC
#endif
#include "glew/glew.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glew32s")


#include "render.h"


bool render::Init(HWND hWindow)
{
    hWnd = hWindow;
    hDC = GetDC(hWnd);

    /* Describe pixel format */
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    int i = ChoosePixelFormat(hDC, &pfd);
    DescribePixelFormat(hDC, i, sizeof(pfd), &pfd);
    SetPixelFormat(hDC, i, &pfd);

    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
    
    if (glewInit() != GLEW_OK ||
        !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)) {
      // Failed to initialize GLEW
      wglMakeCurrent(NULL, NULL);
      wglDeleteContext(hRC);
      ReleaseDC(hWnd, hDC);
      return false;
    }
    
    /* Setup default settings */
    glDisable(GL_DEPTH_TEST);  // Disabled as app is entirely 2D for now but needs blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.05f, 0.15f, 0.25f, 1.f);

    return true;
}


void render::Destroy()
{
   wglMakeCurrent(NULL, NULL);
   wglDeleteContext(hRC);
   ReleaseDC(hWnd, hDC);
}


void render::Resize(int w, int h)
{
  glViewport(0, 0, w, h);
}


void render::StartFrame()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void render::EndFrame()
{
  glFinish();
}


void render::CopyFrame()
{
  SwapBuffers(hDC);
}
