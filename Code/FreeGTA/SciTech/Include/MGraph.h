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

#pragma once

#ifdef FREEGTA_HOOKS
#include "Hooks.h"
#include "Logger.h"
#endif

#define MGLAPI __cdecl

enum MGL_errorType
{
    grOK = 0,
    grNoInit = -1,
    grNotDetected = -2,
    grDriverNotFound = -3,
    grBadDriver = -4,
    grLoadMem = -5,
    grInvalidMode = -6,
    grError = -8,
    grInvalidName = -9,
    grNoMem = -10,
    grNoModeSupport = -11,
    grInvalidFont = -12,
    grBadFontFile = -13,
    grFontNotFound = -14,
    grOldDriver = -15,
    grInvalidDevice = -16,
    grInvalidDC = -17,
    grInvalidCursor = -18,
    grCursorNotFound = -19,
    grInvalidIcon = -20,
    grIconNotFound = -21,
    grInvalidBitmap = -22,
    grBitmapNotFound = -23,
    grZbufferTooBig = -24,
    grNewFontFile = -25,
    grNoDoubleBuff = -26,
    grNoHardwareBlt = -28,
    grNoOffscreenMem = -29,
    grInvalidPF = -30,
    grLastError = -31,
};

// MGraph.cpp
//======================================================
int MGLAPI MGL_result();
void MGLAPI MGL_setResult(int result);
const char* MGLAPI MGL_errorMsg(int err);

// MemSet.cpp
//======================================================
void* MGLAPI MGL_malloc(long size);
void* MGLAPI MGL_calloc(long size, long n);
void MGLAPI MGL_free(void* p);

void MGLAPI MGL_memset(void* s, int c, long n);
void MGLAPI MGL_memcpy(void* dst, void* src, long n);
void MGLAPI MGL_memcpyVIRTSRC(void* dst, void* src, long n);
void MGLAPI MGL_memcpyVIRTDST(void* dst, void* src, long n);
