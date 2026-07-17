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

typedef void* (*MGL_mallocFn)(long size);
typedef void (*MGL_freeFn)(void* p);

#ifdef FREEGTA_HOOKS
MGL_mallocFn& _MGL_malloc = *reinterpret_cast<MGL_mallocFn*>(0x00787160);
MGL_freeFn& _MGL_free = *reinterpret_cast<MGL_freeFn*>(0x00787164);
#else
MGL_mallocFn _MGL_malloc = NULL;
MGL_freeFn _MGL_free = NULL;
#endif

void _MGL_memset(void* p, int c, long n);   // Internal
void _MGL_memsetw(void* p, int c, long n);  // Internal
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
#ifdef _DEBUG
    FREEGTA_LOGDEBUG("_MGL_initMalloc()");
#endif
    _MGL_malloc = WrapMalloc;
    _MGL_free = WrapFree;
}

void MGLAPI MGL_memset(void* p, int c, long n)
{
#ifdef _DEBUG
    FREEGTA_LOGDEBUG("MGL_memset()");
#endif
    _MGL_memset(p, c, n);
}

void* MGLAPI MGL_malloc(long size)
{
#ifdef _DEBUG
    FREEGTA_LOGDEBUG("MGL_malloc()");
#endif
    return _MGL_malloc(size);
}

void* MGLAPI MGL_calloc(long s, long n)
{
#ifdef _DEBUG
    FREEGTA_LOGDEBUG("MGL_calloc()");
#endif
    void* p = MGL_malloc(s * n);
    MGL_memset(p, 0, s * n);
    return p;
}

void MGLAPI MGL_free(void* p)
{
#ifdef _DEBUG
    FREEGTA_LOGDEBUG("MGL_free()");
#endif
    _MGL_free(p);
}

// Originally assembly routines
void _MGL_memset(void* p, int c, long n)
{
#ifdef _DEBUG
    FREEGTA_LOGDEBUG("_MGL_memset()");
#endif
    memset(p, c, n);
}

void _MGL_memsetw(void* p, int c, long n)
{
#ifdef _DEBUG
    FREEGTA_LOGDEBUG("_MGL_memsetw()");
#endif
    memset(p, c, n);
}

void _MGL_memsetl(void* p, long c, long n)
{
#ifdef _DEBUG
    FREEGTA_LOGDEBUG("_MGL_memsetl()");
#endif
    memset(p, c, n);
}

void MGLAPI MGL_memcpy(void* p, void* s, long n)
{
#ifdef _DEBUG
    FREEGTA_LOGDEBUG("MGL_memcpy()");
#endif
    memcpy(p, s, n);
}

void MGLAPI MGL_memcpyVIRTSRC(void* p, void* s, long n)
{
#ifdef _DEBUG
    FREEGTA_LOGDEBUG("MGL_memcpyVIRTSRC()");
#endif
    memcpy(p, s, n);
}

void MGLAPI MGL_memcpyVIRTDST(void* p, void* s, long n)
{
#ifdef _DEBUG
    FREEGTA_LOGDEBUG("MGL_memcpyVIRTDST()");
#endif
    memcpy(p, s, n);
}

// Hooking
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef FREEGTA_HOOKS

void __gta_MemSetHooks()
{
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
