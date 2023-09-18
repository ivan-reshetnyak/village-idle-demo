#include "pch.h"

#include <windowsx.h>

#include "window.h"


LRESULT CALLBACK window::WinFunc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CREATE) {
        SetWindowLongPtr(hWnd, 0, (LONG_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);  // Attach 'this' pointer to window class
        ((window*)((CREATESTRUCT*)lParam)->lpCreateParams)->hWnd = hWnd;
    }
    window* win = (window*)GetWindowLongPtr(hWnd, 0);

    switch (msg) {
    case WM_CREATE:
        return win->OnCreate((CREATESTRUCT*)lParam) ? 0 : -1;
    case WM_CLOSE:
    case WM_DESTROY:
        win->OnDestroy();
        return 0;
    case WM_SIZE:
        win->OnSize((UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
        return 0;
    case WM_ACTIVATE:
        win->OnActivate((UINT)LOWORD(wParam), (HWND)(lParam), (BOOL)HIWORD(wParam));
        return 0;
    case WM_ERASEBKGND:
        return (LRESULT)win->OnEraseBackground((HDC)wParam);
    case WM_PAINT:
    case WM_TIMER:
        win->OnPaint();
        win->OnTimer((UINT)wParam);
        return 0;
    case WM_KEYDOWN:
        win->OnKeyDown((UINT)wParam);
        return 0;
    case WM_KEYUP:
        win->OnKeyUp((UINT)wParam);
        return 0;
    case WM_MOUSEMOVE:
        win->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    case WM_LBUTTONDOWN:
        win->OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    case WM_MBUTTONDOWN:
        win->OnMButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    case WM_RBUTTONDOWN:
        win->OnRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    case WM_LBUTTONUP:
        win->OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    case WM_MBUTTONUP:
        win->OnMButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    case WM_RBUTTONUP:
        win->OnRButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;
    case WM_MOUSEWHEEL:
        win->OnMouseWheel(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), GET_WHEEL_DELTA_WPARAM(wParam), GET_KEYSTATE_WPARAM(wParam));
        return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}


bool window::Init(LPCSTR className, LPCSTR caption, int startWidth, int startHeight, ptr parent)
{
    width = startWidth;
    height = startHeight;

    HINSTANCE hInst = GetModuleHandle(NULL);

    /* Register window class */
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(VOID *);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIconSm = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON,
        GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 0);
    wc.hIcon = (HICON)LoadImage(NULL, IDI_APPLICATION, IMAGE_ICON,
        GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), 0);
    wc.lpszMenuName = NULL;
    wc.hInstance = hInst;
    wc.lpfnWndProc = WinFunc;
    wc.lpszClassName = className;
    if (!RegisterClassEx(&wc)) {
        return false;
    }

    /* Create window */
    hWnd = CreateWindow(className, caption, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        parent ? parent->hWnd : NULL, NULL, hInst, (LPVOID)this);
    if (hWnd == NULL) {
      return false;
    }

    ShowWindow(hWnd, SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    return true;
}


int window::Run()
{
    MSG msg;
    /* Message loop */
    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else if (isInitialized) {
            Idle();
        }
    }
    return (int)msg.wParam;
}


void window::OnDestroy() {
    isInitialized = false;
    PostQuitMessage(0);
}


void window::OnSize(UINT newWidth, UINT newHeight) {
  width = newWidth;
  height = newHeight;
}


bool window::OnEraseBackground(HDC hDC)
{
  HPEN hOldPen = (HPEN)SelectObject(hDC, GetStockObject(NULL_PEN));
  HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(WHITE_BRUSH));

  /* Default action - draw white rectangle */
  Rectangle(hDC, 0, 0, width, height);
  SelectObject(hDC, hOldPen);
  SelectObject(hDC, hOldBrush);
  return true;
}


window::info::info(HWND hWnd)
    : hWnd(hWnd)
{
}


point window::info::GetClientSize() const
{
    RECT winRect;
    GetClientRect(hWnd, &winRect);
    return point(winRect.right, winRect.bottom);
}


coord_winapi window::info::ScreenToRelative(const point& screen) const
{
    POINT winPt{screen.x, screen.y};
    ::ScreenToClient(hWnd, &winPt);

    RECT winRect;
    GetClientRect(hWnd, &winRect);

    return coord_winapi((float)winPt.x / winRect.right, (float)winPt.y / winRect.bottom);
}


coord_winapi window::info::ClientToRelative(const point& client) const
{
    RECT winRect;
    GetClientRect(hWnd, &winRect);

    return coord_winapi((float)client.x / winRect.right, (float)client.y / winRect.bottom);
}


point window::info::RelativeToClient(const coord_winapi& rel) const
{
    RECT winRect;
    GetClientRect(hWnd, &winRect);

    return point((int)(rel.x * winRect.right), (int)(rel.y * winRect.bottom));
}


point window::info::ScreenToClient(const point& screen) const
{
    POINT winPt{screen.x, screen.y};
    ::ScreenToClient(hWnd, &winPt);

    return point(winPt.x, winPt.y);
}
