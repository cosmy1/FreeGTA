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

#include "Logger.h"
#include "ErrorHandler.h"

const UINT32    kMaxLogBufferSize = 2048;
static Logger*  g_logger = NULL;

Logger::Logger() :
    m_logLevelType(LogLevelType::LLT_DEBUG)
{
    m_file = NULL;
    g_logger = this;
}

Logger::~Logger()
{
    Close();
    g_logger = NULL;
}

void Logger::Open(const char* fileName)
{
    Close();

    if (fileName != NULL)
        fopen_s(&m_file, fileName, "w");
}

void Logger::Close()
{
    if (IsFileOpen())
        fclose(m_file);
}

bool Logger::IsFileOpen() const
{
    return m_file != NULL;
}

FILE* Logger::GetFilePointer() const
{
    return m_file;
}

void Logger::SetLogLevelType(const LogLevelType& logLevelType)
{
    m_logLevelType = logLevelType;
}

LogLevelType Logger::GetLogLevelType() const
{
    return m_logLevelType;
}

void Logger::Log(const LogLevelType& logLevelType, const char* fmt, ...)
{
    if (!g_logger || g_logger->GetLogLevelType() > logLevelType)
        return;

    const char* tag = NULL;
    const char* nl = "";

    switch (logLevelType)
    {
    case LogLevelType::LLT_DEBUG:
        tag = "[DEBUG]";
        break;
    case LogLevelType::LLT_INFO:
        tag = "[INFO]";
        break;
    case LogLevelType::LLT_WARNING:
        tag = "[WARNING]";
        break;
    case LogLevelType::LLT_ERROR:
        tag = "[ERROR]";
        break;
    case LogLevelType::LLT_FATAL:
        tag = "[FATAL]";
        nl = "\n\n";
        break;
    default:
        tag = "";
    }

    va_list args;
    char buffer[kMaxLogBufferSize - 256];
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    time_t rawTime;
    struct tm* timeInfo;

    time(&rawTime);
    timeInfo = localtime(&rawTime);

    char msg[kMaxLogBufferSize];

    snprintf(msg, sizeof(msg), "[%04u-%02u-%02u %02u:%02u:%02u]%s : %s%s\n",
        timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday,
        timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec, tag, nl, buffer);

    ErrorHandler::DebuggerPrint(msg);

    if (g_logger->IsFileOpen())
        fprintf(g_logger->GetFilePointer(), msg);
}
