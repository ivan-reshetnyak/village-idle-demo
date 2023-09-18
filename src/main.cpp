#include "pch.h"

#include <Windows.h>
#undef min
#undef max

#include "game/game.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
    return game().Run();
}
