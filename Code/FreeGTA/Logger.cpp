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

static Logger* g_logger = NULL;

Logger::Logger()
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
