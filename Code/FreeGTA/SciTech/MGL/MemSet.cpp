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

#include "MGL.h"

void* (*_MGL_malloc)(long size) = NULL;
void  (*_MGL_free)(void* p)     = NULL;

void _MGL_memset(void* p, int c, long n); // Internal
void _MGL_memsetw(void* p, int c, long n); // Internal
void _MGL_memsetl(void* p, long c, long n); // Internal

// Implementation
//////////////////////////////////////////////////////////////////////////////////////////////////

static void* WrapMalloc(long size)
{
    return malloc(static_cast<size_t>(size));
}

static void WrapFree(void* p)
{
    free(p);
}

void _MGL_initMalloc()
{
    _MGL_malloc = WrapMalloc;
    _MGL_free = WrapFree;
}

void MGLAPI MGL_memset(void* p, int c, long n)
{
    _MGL_memset(p, c, n);
}

void* MGLAPI MGL_malloc(long size)
{
    return _MGL_malloc(size);
}

void* MGLAPI MGL_calloc(long s, long n)
{
    void* p = MGL_malloc(s * n);
    MGL_memset(p, 0, s * n);
    return p;
}

void MGLAPI MGL_free(void* p)
{
    _MGL_free(p);
}

// Originally assembly routines
void _MGL_memset(void* p, int c, long n)
{
    memset(p, c, n);
}

void _MGL_memsetw(void* p, int c, long n)
{
    memset(p, c, n);
}

void _MGL_memsetl(void* p, long c, long n)
{
    memset(p, c, n);
}

void MGLAPI MGL_memcpy(void* p, void* s, long n)
{
    memcpy(p, s, n);
}

void MGLAPI MGL_memcpyVIRTSRC(void* p, void* s, long n)
{
    memcpy(p, s, n);
}

void MGLAPI MGL_memcpyVIRTDST(void* p, void* s, long n)
{
    memcpy(p, s, n);
}

// Hooking
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef FREEGTA_HOOKS

void __gta_MemSetHooks()
{
    //_MGL_malloc   0x00787160
    //_MGL_free     0x00787164
    Hooks::InstallHook(reinterpret_cast<void*>(0x004910D0), _MGL_initMalloc, NULL);
    Hooks::InstallHook(reinterpret_cast<void*>(0x004910F0), MGL_memset, NULL);
    Hooks::InstallHook(reinterpret_cast<void*>(0x00491110), MGL_malloc, NULL);
    Hooks::InstallHook(reinterpret_cast<void*>(0x00491130), MGL_calloc, NULL);
    Hooks::InstallHook(reinterpret_cast<void*>(0x00491160), MGL_free, NULL);

    Hooks::InstallHook(reinterpret_cast<void*>(0x00494954), _MGL_memset, NULL);
    Hooks::InstallHook(reinterpret_cast<void*>(0x0049498E), _MGL_memsetw, NULL);
    Hooks::InstallHook(reinterpret_cast<void*>(0x004949C4), _MGL_memsetl, NULL);
    Hooks::InstallHook(reinterpret_cast<void*>(0x004949DC), MGL_memcpy, NULL);
    Hooks::InstallHook(reinterpret_cast<void*>(0x00494A00), MGL_memcpyVIRTSRC, NULL);
    Hooks::InstallHook(reinterpret_cast<void*>(0x00494A30), MGL_memcpyVIRTDST, NULL);
}

#endif
