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

#pragma once

#include "Utils.h"

#ifdef _DEBUG
#   define FREEGTA_BREAK() __debugbreak()
#else
#   define FREEGTA_BREAK()
#endif

class ErrorHandler
{
public:
    static void DebuggerPrint(const char* str);
    static bool IsDebuggerAttached();

    static void AssertHandler(const char* expr, const char* fileName, INT32 line, const char* fmt, ...);
    static void FatalErrorHandler(const char* fileName, INT32 line, const char* fmt, ...);

    static const char* GetMinidumpErrorDescription();
    static INT32 WriteMiniDump(const char* appName, void* exceptionPointers);
    static void CrashHandler();

private:
    static bool m_errorActive;
    static INT8 m_minidumpError;
};

#define FREEGTA_ASSERT(EXP)         { if(!(EXP)) ErrorHandler::AssertHandler(#EXP, __FILE__, __LINE__, NULL); }
#define FREEGTA_ASSERT_F(EXP, ...)  { if(!(EXP)) ErrorHandler::AssertHandler(#EXP, __FILE__, __LINE__, __VA_ARGS__); }
#define FREEGTA_FATAL_ERROR(...)    { ErrorHandler::FatalErrorHandler(__FILE__, __LINE__, __VA_ARGS__); }
