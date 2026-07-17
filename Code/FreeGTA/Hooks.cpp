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

#include "Hooks.h"
#include "ErrorHandler.h"
#include "Logger.h"
#include <MinHook.h>

// Implementation
//////////////////////////////////////////////////////////////////////////////////////////////////

HMODULE(WINAPI* OriginalLoadLibraryA)(LPCSTR lpLibFileName);
FARPROC(WINAPI* OriginalGetProcAddress)(HMODULE hModule, LPCSTR lpProcName);
int(WINAPI* OriginalMessageBoxA)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);

HMODULE WINAPI DetourLoadLibraryA(LPCSTR lpLibFileName)
{
    HMODULE module = OriginalLoadLibraryA(lpLibFileName);

#ifdef _DEBUG
    FREEGTA_LOGDEBUG("GTA: LoadLibraryA(%s) = 0x%08x", lpLibFileName, module);
#endif

    return module;
}

FARPROC WINAPI DetourGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
    FARPROC address = OriginalGetProcAddress(hModule, lpProcName);

#ifdef _DEBUG
    FREEGTA_LOGDEBUG("GTA: GetProcAddress(0x%08x, %s) = 0x%08x", hModule, lpProcName, address);
#endif

    return address;
}

int WINAPI DetourMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
    int retCode = OriginalMessageBoxA(hWnd, lpText, lpCaption, uType);

#ifdef _DEBUG
    FREEGTA_LOGDEBUG("GTA: MessageBoxA(0x%08x, %s, %s, %d) = %d", hWnd, lpText, lpCaption, uType, retCode);
#endif

    return retCode;
}

Hooks::Hooks()
{
}

Hooks::~Hooks()
{
}

void Hooks::Create()
{
    GTAVersion gtaVersion = GetGTAVersion(reinterpret_cast<size_t>(GetModuleHandle(NULL)));

    FREEGTA_LOGINFO("Detected '%s'!", GetGTAVersionString(gtaVersion));

    if (gtaVersion != GTA_ROCKSTAR_FREE)
        FREEGTA_FATAL_ERROR("At the moment only '%s' release is supported!", GetGTAVersionString(GTA_ROCKSTAR_FREE)); // Only Rockstar Free Release is supported at the moment

    FREEGTA_LOGINFO("Installing hooks...");

    FREEGTA_ASSERT(MH_Initialize() == MH_OK);

    // Install custom hooks
    InstallHook(LoadLibraryA, DetourLoadLibraryA, &reinterpret_cast<PVOID&>(OriginalLoadLibraryA));
    InstallHook(GetProcAddress, DetourGetProcAddress, &reinterpret_cast<PVOID&>(OriginalGetProcAddress));
    InstallHook(MessageBoxA, DetourMessageBoxA, &reinterpret_cast<PVOID&>(OriginalMessageBoxA));
}

void Hooks::Destroy()
{
    FREEGTA_LOGINFO("Uninstalling hooks...");

    // Uninstall custom hooks
    UninstallHook(LoadLibraryA);
    UninstallHook(GetProcAddress);
    UninstallHook(MessageBoxA);

    FREEGTA_ASSERT(MH_Uninitialize() == MH_OK);
}

void Hooks::InstallHook(void* targetPtr, void* detourPtr, void** trampolinePtr)
{
    FREEGTA_ASSERT(MH_CreateHook(targetPtr, detourPtr, trampolinePtr) == MH_OK);
    FREEGTA_ASSERT(MH_EnableHook(targetPtr) == MH_OK);
}

void Hooks::UninstallHook(void* targetPtr)
{
    FREEGTA_ASSERT(MH_DisableHook(targetPtr) == MH_OK);
    FREEGTA_ASSERT(MH_RemoveHook(targetPtr) == MH_OK);
}

GTAVersion Hooks::GetGTAVersion(size_t baseAddress)
{
    IMAGE_DOS_HEADER* dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(baseAddress);
    IMAGE_NT_HEADERS* ntHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<BYTE*>(dosHeader) + dosHeader->e_lfanew);

    const DWORD kGTA_1997_EU = 0x343dff8b;          // 1997-10-10 12:12:27
    const DWORD kGTA_1998_US = 0x34db2bb5;          // 1998-02-06 16:26:45
    const DWORD kGTA_1998_US_Rev1 = 0x351fbb59;     // 1998-03-30 17:33:45
    const DWORD kGTA_Rockstar_Free = 0x3dcfd617;    // 2002-11-11 17:08:55
    const DWORD kGTA_Collection = 0x40902623;       // 2004-04-28 23:46:11

    switch (ntHeaders->FileHeader.TimeDateStamp)
    {
    case kGTA_1997_EU:
        return GTA_1997_EU;
    case kGTA_1998_US:
        return GTA_1998_US;
    case kGTA_1998_US_Rev1:
        return GTA_1998_US_REV1;
    case kGTA_Rockstar_Free:
        return GTA_ROCKSTAR_FREE;
    case kGTA_Collection:
        return GTA_COLLECTION;
    }

    return GTA_UNKNOWN;
}

const char* Hooks::GetGTAVersionString(const GTAVersion& gtaVersion)
{
    switch (gtaVersion)
    {
    case GTA_1997_EU:
        return "Grand Theft Auto (EUR)";
    case GTA_1998_US:
        return "Grand Theft Auto (USA)";
    case GTA_1998_US_REV1:
        return "Grand Theft Auto (Rev1) (USA)";
    case GTA_ROCKSTAR_FREE:
        return "Grand Theft Auto (Rockstar Free Release)";
    case GTA_COLLECTION:
        return "Grand Theft Auto (Collection)";
    }

    return "Grand Theft Auto (Unknown Release)";
}
