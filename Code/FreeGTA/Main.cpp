/*
*   FreeGTA
*   Copyright (C) 2021-2026 cosmy1
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "Logger.h"
#include "ErrorHandler.h"
#include "Hooks.h"
#include <Windows.h>

Logger  g_loggerInstance;
Hooks   g_hooksInstance;

// Implementation
//////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CustomDllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        g_loggerInstance.Open("FreeGTA.log");

        FREEGTA_LOGINFO("FreeGTA starting...");

        DisableThreadLibraryCalls(hinstDLL);

        g_hooksInstance.Create();

        FREEGTA_LOGINFO("FreeGTA started!");
    }
    else if (fdwReason == DLL_PROCESS_DETACH)
    {
        g_hooksInstance.Destroy();

        FREEGTA_LOGINFO("FreeGTA shut-down!");
    }

    return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    __try
    {
        return CustomDllMain(hinstDLL, fdwReason, lpReserved);
    }
    __except (ErrorHandler::WriteMiniDump("FreeGTA", GetExceptionInformation()))
    {
        ErrorHandler::CrashHandler();
    }
}
