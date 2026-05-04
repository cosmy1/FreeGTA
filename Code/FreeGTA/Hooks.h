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

enum GTAVersion
{
    GTA_UNKNOWN,
    GTA_1997_EU,
    GTA_1998_US,
    GTA_1998_US_REV1,
    GTA_ROCKSTAR_FREE,
    GTA_COLLECTION
};

class Hooks
{
public:
    Hooks();
    ~Hooks();

    void Create();
    void Destroy();

private:
    static GTAVersion   GetGTAVersion(size_t baseAddress);
    static const char*  GetGTAVersionString(const GTAVersion& gtaVersion);
};
