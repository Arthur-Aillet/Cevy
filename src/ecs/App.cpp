/*
** Agartha-Software, 2023
** C++evy
** File description:
** App
*/

#include "App.hpp"

void cevy::ecs::App::run() { _scheduler.run(*this); }

#include <windows.h>

extern "C" BOOL WINAPI DllMain(HINSTANCE const instance, // handle to DLL module
                               DWORD const reason,       // reason for calling function
                               LPVOID const reserved)    // reserved
{
  return TRUE; // Successful DLL_PROCESS_ATTACH.
}