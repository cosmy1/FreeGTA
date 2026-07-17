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

// This file is an attempt to reconstruct the original MGraph library used in GTA.

#pragma once

#ifdef FREEGTA_HOOKS

#include "Hooks.h"

#endif

#define MGLAPI __cdecl

// MemSet.cpp
//======================================================
void* MGLAPI MGL_malloc(long size);
void* MGLAPI MGL_calloc(long size, long n);
void MGLAPI MGL_free(void* p);

void MGLAPI MGL_memset(void* s, int c, long n);
void MGLAPI MGL_memcpy(void* dst, void* src, long n);
void MGLAPI MGL_memcpyVIRTSRC(void* dst, void* src, long n);
void MGLAPI MGL_memcpyVIRTDST(void* dst, void* src, long n);
