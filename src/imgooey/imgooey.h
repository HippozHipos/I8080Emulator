#include "imgui.h"
#include "../PlatformWin/PlatformWin32.h"


struct imgooey
{
	static void Init(HWND);
	static void CheckVer();
	static void SetFlags(int);
	static void Begin();
	static void End(); //we are only calling another function so best to inline (compiler will decide anyway)
	static void Destroy();
};