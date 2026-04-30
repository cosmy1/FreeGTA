/*********************************************************************
This file is part of FreeGTA.
Copyright (C) 2021-2026 cosmy1

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
**********************************************************************/

#include <Windows.h>

typedef LPVOID LPDIRECTPLAY;
typedef LPVOID LPDPENUMDPCALLBACK;
typedef LPVOID LPDPENUMDPCALLBACKA;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    return TRUE;
}

HRESULT WINAPI FakeDirectPlayCreate(LPGUID lpGUID, LPDIRECTPLAY* lplpDP, IUnknown* pUnk)
{
    return E_FAIL;
}

HRESULT WINAPI FakeDirectPlayEnumerateA(LPDPENUMDPCALLBACKA, LPVOID)
{
    return E_FAIL;
}

HRESULT WINAPI FakeDirectPlayEnumerateW(LPDPENUMDPCALLBACK, LPVOID)
{
    return E_FAIL;
}
