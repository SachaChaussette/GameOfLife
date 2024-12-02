#include "pch.h"
#include "Macro.h"

GAMEOFLIFELIB__API void Macro::Config()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	SetConsoleOutputCP(CP_UTF8);
	locale::global(locale("fr_FR"));
}