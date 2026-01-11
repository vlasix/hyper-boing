#include "gamerunner.h"

#ifdef _WIN32
#include <windows.h>
#endif

/**
 * Main entry point for the application
 */
int main(int argc, char* argv[])
{
    GameRunner runner;
    return runner.run();
}

/**
 * Windows entry point wrapper
 */
#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return main(__argc, __argv);
}
#endif