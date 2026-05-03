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

enum LogLevelType
{
    LLT_DEBUG = 0,
    LLT_INFO,
    LLT_WARNING,
    LLT_ERROR,
    LLT_FATAL,
    LLT_NONE
};

class Logger
{
public:
    Logger();
    ~Logger();

    void Open(const char* fileName);
    void Close();

    bool IsFileOpen() const;
    FILE* GetFilePointer() const;

    void SetLogLevelType(const LogLevelType& logLevelType);
    LogLevelType GetLogLevelType() const;

    static void Log(const LogLevelType& logLevelType, const char* fmt, ...);

private:
    FILE* m_file;
    LogLevelType m_logLevelType;
};

#define FREEGTA_LOGDEBUG(...)   Logger::Log(LogLevelType::LLT_DEBUG,    __VA_ARGS__)
#define FREEGTA_LOGINFO(...)    Logger::Log(LogLevelType::LLT_INFO,     __VA_ARGS__)
#define FREEGTA_LOGWARNING(...) Logger::Log(LogLevelType::LLT_WARNING,  __VA_ARGS__)
#define FREEGTA_LOGERROR(...)   Logger::Log(LogLevelType::LLT_ERROR,    __VA_ARGS__)
#define FREEGTA_LOGFATAL(...)   Logger::Log(LogLevelType::LLT_FATAL,    __VA_ARGS__)
