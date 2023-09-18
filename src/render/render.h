#pragma once

#include <windows.h>


/// @brief OpenGL handler class
class render {
public:
    /// @brief Initialize OpenGL
    /// @return true if successful
    bool Init(HWND);
    void Destroy();

    void Resize(int w, int h);
    void StartFrame();
    void EndFrame();
    void CopyFrame();

private:
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;
};
