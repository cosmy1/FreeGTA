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

// This file is part of an effort to reconstruct the original MGL library used by GTA.
// Some portions of this file are adapted from the SciTech MGL GPL source: https://github.com/kendallb/scitech-mgl

#include "MGL.h"

#ifdef FREEGTA_HOOKS
int& _MGL_result = *reinterpret_cast<int*>(0x007873BC);
#else
int _MGL_result;
#endif

// Implementation
//////////////////////////////////////////////////////////////////////////////////////////////////

int MGLAPI MGL_result()
{
    int	result = _MGL_result;
#ifdef _DEBUG
    FREEGTA_LOGDEBUG("MGL_result() = %d", result);
#endif
    _MGL_result = grOK;
    return result;
}

// The linker removed this symbol from the GTA executable because it was never referenced
void MGLAPI MGL_setResult(int result)
{
    _MGL_result = result;
}

const char* MGLAPI MGL_errorMsg(int err)
{
    static char* messages[] =
    {
        "No error",
        "Graphics driver has not been installed",
        "Graphics hardware not detected",
        "Couldn't find graphics driver",
        "Graphics driver file corrupted",
        "Not enough memory to load graphics driver",
        "Invalid mode for selected driver",
        "Invalid driver number",
        "General graphics error",
        "Invalid graphics driver name",
        "Not enough memory to perform operation",
        "Selected graphics mode not supported",
        "Invalid font data",
        "Font file corrupted",
        "Couldn't find font file",
        "Incorrect version for driver file",
        "Invalid device type for operation",
        "Invalid device context",
        "Invalid cursor file",
        "Cursor file was not found",
        "Invalid icon file",
        "Icon file was not found",
        "Invalid bitmap file",
        "Bitmap file was not found",
        "Invalid error number",
        "Zbuffer allocation is too large",
        "Only Windows 2.x font files supported",
        "MGL_endRegion() was not called",
        "Double buffering is not available",
        "OffscreenDC's require hardware bitBlt support",
        "No available Offscreen display memory",
        "Invalid pixel format for memory DC",
    };

    if (err > 0 || err < grLastError)
        err = grLastError - 1;

    return messages[-err];
}

// Hooking
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef FREEGTA_HOOKS

void __gta_MGraphHooks()
{
    Hooks::InstallHook(reinterpret_cast<void*>(0x0048AE10), MGL_result, NULL);
    Hooks::InstallHook(reinterpret_cast<void*>(0x0048AE20), MGL_errorMsg, NULL);
}

#endif
