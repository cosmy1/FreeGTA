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

#include "ErrorHandler.h"
#include "Logger.h"
#include <Windows.h>
#include <DbgHelp.h>

enum MiniDumpErrors
{
    MDE_NOERROR = -1,
    MDE_STANDARD = 0,
    MDE_DBGHELP_NOTFOUND = 1,
    MDE_DBGHELP_OLDVERSION = 2,
    MDE_FOLDER_PERMISSIONS = 3
};

bool ErrorHandler::m_errorActive = false;
INT8 ErrorHandler::m_minidumpError = MDE_NOERROR;
const UINT32 kMaxLogBufferSize = 2048;

void ErrorHandler::DebuggerPrint(const char* str)
{
    OutputDebugString(str);
}

bool ErrorHandler::IsDebuggerAttached()
{
    return IsDebuggerPresent() != FALSE;
}

void ErrorHandler::AssertHandler(const char* expr, const char* fileName, INT32 line, const char* fmt, ...)
{
    if (m_errorActive)
        return;

    m_errorActive = true;

    char msg[kMaxLogBufferSize];

    if (fmt)
    {
        va_list args;
        char message[kMaxLogBufferSize / 2];

        va_start(args, fmt);
        vsnprintf(message, sizeof(message), fmt, args);
        va_end(args);

        snprintf(msg, sizeof(msg), "Assertion failed!\n\nFile: %s\nLine: %d\n\nExpression: %s\n\nDetails: %s\n", fileName, line, expr, message);
    }
    else
    {
        if (fileName)
            snprintf(msg, sizeof(msg), "Assertion failed!\n\nFile: %s\nLine: %d\n\nExpression: %s\n", fileName, line, expr);
        else
            snprintf(msg, sizeof(msg), "Assertion failed!\n\nExpression: %s\n", expr);
    }

    FREEGTA_LOGFATAL(msg);

    if (!IsDebuggerAttached())
    {
        MessageBoxA(GetActiveWindow(), msg, "Assert", MB_ICONERROR | MB_OK);
        m_errorActive = false;
        exit(-1);
    }

    m_errorActive = false;

    FREEGTA_BREAK();
}

void ErrorHandler::FatalErrorHandler(const char* fileName, INT32 line, const char* fmt, ...)
{
    if (m_errorActive)
        return;

    m_errorActive = true;

    char msg[kMaxLogBufferSize];

    va_list args;
    char message[kMaxLogBufferSize / 2];

    va_start(args, fmt);
    vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);

    snprintf(msg, sizeof(msg), "An error has occured!\n\nFile: %s\nLine: %d\n\nDetails: %s\n", fileName, line, message);

    FREEGTA_LOGFATAL(msg);

    if (!IsDebuggerAttached())
    {
        MessageBoxA(GetActiveWindow(), msg, "Error", MB_ICONERROR | MB_OK);
        m_errorActive = false;
        exit(-1);
    }

    m_errorActive = false;

    FREEGTA_BREAK();
}

const char* ErrorHandler::GetMinidumpErrorDescription()
{
    switch (m_minidumpError)
    {
    case MDE_DBGHELP_NOTFOUND:
        return "DbgHelp.dll not found.";
    case MDE_DBGHELP_OLDVERSION:
        return "DbgHelp.dll have no MiniDumpWriteDump() function. Probably the DLL is too old or invalid.";
    case MDE_FOLDER_PERMISSIONS:
        return "Cannot create minidump file. Check if the folder have the right permissions.";
    }

    return "An unhandled exception has occured and the program will be terminated.";
}

INT32 ErrorHandler::WriteMiniDump(const char* appName, void* exceptionPointers)
{
    HMODULE dllModule = NULL;

    if (m_errorActive)
        return EXCEPTION_EXECUTE_HANDLER;

    m_errorActive = true;

    using MINIDUMPWRITEDUMP = BOOL(WINAPI*)(
        HANDLE hProcess,
        DWORD ProcessId,
        HANDLE hFile,
        MINIDUMP_TYPE DumpType,
        PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
        PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
        PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

    dllModule = LoadLibraryW(L"DbgHelp.dll");

    m_minidumpError = MDE_STANDARD;

    if (dllModule)
    {
        MINIDUMPWRITEDUMP miniDumpWriteDumpPtr = (MINIDUMPWRITEDUMP)GetProcAddress(dllModule, "MiniDumpWriteDump");
        if (miniDumpWriteDumpPtr)
        {
            MINIDUMP_EXCEPTION_INFORMATION info;
            info.ThreadId = GetCurrentThreadId();
            info.ExceptionPointers = (EXCEPTION_POINTERS*)exceptionPointers;
            info.ClientPointers = TRUE;

            time_t rawTime;
            struct tm* timeInfo;
            wchar_t fileName[MAX_PATH];

            time(&rawTime);
            timeInfo = localtime(&rawTime);

#ifdef _DEBUG
            swprintf(fileName, sizeof(fileName), L"%S_d-%04u%02u%02u%02u%02u%02u%010u.dmp", appName, timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday,
                timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec, GetTickCount());
#else
            swprintf(fileName, sizeof(fileName), L"%S-%04u%02u%02u%02u%02u%02u%010u.dmp", appName, timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday,
                timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec, GetTickCount());
#endif

            HANDLE fileHandle = CreateFileW(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

            if (fileHandle != INVALID_HANDLE_VALUE)
            {
                miniDumpWriteDumpPtr(GetCurrentProcess(), GetCurrentProcessId(), fileHandle, MiniDumpWithDataSegs, exceptionPointers ? &info : NULL, NULL, NULL);
                CloseHandle(fileHandle);
            }
            else
                m_minidumpError = MDE_FOLDER_PERMISSIONS;
        }
        else
            m_minidumpError = MDE_DBGHELP_OLDVERSION;

        FreeLibrary(dllModule);
    }
    else
        m_minidumpError = MDE_DBGHELP_NOTFOUND;

    m_errorActive = false;

    return EXCEPTION_EXECUTE_HANDLER;
}

void ErrorHandler::CrashHandler()
{
    if (m_errorActive)
        return;

    m_errorActive = true;

    FREEGTA_LOGFATAL(GetMinidumpErrorDescription());
    MessageBoxA(GetActiveWindow(), GetMinidumpErrorDescription(), "Crash", MB_ICONERROR | MB_OK);
    m_errorActive = false;
}
