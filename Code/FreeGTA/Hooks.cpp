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

#include "Hooks.h"
#include "ErrorHandler.h"
#include "Logger.h"
#include <MinHook.h>

void Hooks::Create()
{
    FREEGTA_LOGINFO("Installing hooks...");

    FREEGTA_ASSERT(MH_Initialize() == MH_OK);
}

void Hooks::Destroy()
{
    FREEGTA_LOGINFO("Uninstalling hooks...");

    FREEGTA_ASSERT(MH_Uninitialize() == MH_OK);
}
