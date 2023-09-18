#pragma once

#include <map>
#include <memory>

#include <Windows.h>
#undef min
#undef max

#include "math/common.h"


/// @brief WINAPI window base class
class window {
public:
    using ptr = std::shared_ptr<window>;


    class info {
    public:
        info(HWND hWnd = NULL);
        ~info() = default;

        point GetClientSize() const;
        coord_winapi ScreenToRelative(const point& screen) const;
        point ScreenToClient(const point& screen) const;
        coord_winapi ClientToRelative(const point& client) const;
        point RelativeToClient(const coord_winapi& rel) const;

    private:
        HWND hWnd;
    };


    /// @brief Main loop function
    /// @return Main loop return code
    int Run();

    HWND GetHandle() const { return hWnd; }
    info GetInfo() const { return info(hWnd); }

protected:
    bool isInitialized = false;
    unsigned width = 0, height = 0;


    window() = default;
    virtual ~window() = default;

    bool Init(LPCSTR className, LPCSTR caption, int startWidth, int startHeight, ptr parent = nullptr);

    virtual bool OnCreate(CREATESTRUCT*) { return true; }
    virtual void OnDestroy();
    virtual void OnSize(UINT width, UINT height);
    virtual bool OnEraseBackground(HDC);
    virtual void OnPaint() {}
    virtual void OnActivate(UINT reason, HWND hWndActDeact, BOOL isMinimized) {}
    virtual void OnTimer(INT id) {}

    virtual void OnMouseMove(INT x, INT y) {}
    virtual void OnLButtonDown(INT x, INT y) {}
    virtual void OnMButtonDown(INT x, INT y) {}
    virtual void OnRButtonDown(INT x, INT y) {}
    virtual void OnLButtonUp(INT x, INT y) {}
    virtual void OnMButtonUp(INT x, INT y) {}
    virtual void OnRButtonUp(INT x, INT y) {}

    virtual void OnKeyDown(UINT key) {}
    virtual void OnKeyUp(UINT key) {}

    virtual void OnMouseWheel(INT x, INT y, INT z, UINT keys) {}
    virtual void Idle() {}

private:
    HWND hWnd = NULL;


    /// @brief WINAPI window message handler
    static LRESULT CALLBACK WinFunc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};
