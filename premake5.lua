--[[
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
]]

function projectCommon(projName)
    targetname(projName)
    objdir("%{wks.location}/obj/%{prj.name}/%{cfg.buildcfg}/")

    targetdir("Bin")
    debugdir("Bin")

    architecture("x86")
    language("C++")
    characterset("MBCS")
    symbols("On")
    staticruntime("On")
    multiprocessorcompile("On")

    filter { "language:C++" }
        cppdialect("C++98")

    filter { "language:C" }
        cdialect("C99")

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        runtime("Release")
        editandcontinue("Off")
        optimize("Speed")
        linktimeoptimization("On")

    filter { "configurations:Debug" }
        defines { "_DEBUG" }
        runtime("Debug")
        editandcontinue("On")
        targetsuffix("_d")

    filter { "action:vs*", "system:windows" }
        defines {
            "WIN32",
            "_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS"
        }
        symbolspath("$(OutDir)$(TargetName).pdb")

    filter { }

    includedirs { "Code" }
end

workspace("FreeGTA")
    configurations { "Release", "Debug" }
    location(".Gen")

    startproject("Launcher")

    -- Launcher Project (Console)
    project("Launcher")
        kind("ConsoleApp")
        projectCommon("Launcher")

        files {
            "Code/Launcher/Resources/*.*",
            "Code/Launcher/*.*"
        }

    -- DPlay Wrapper Project (DLL)
    project("dplayx")
        kind("SharedLib")
        projectCommon("dplayx")

        files {
            "Code/DPlayX/Resources/*.*",
            "Code/DPlayX/*.*"
        }

    -- Game Project (DLL)
    project("FreeGTA")
        kind("SharedLib")
        projectCommon("FreeGTA")

        links { "MinHook" }

        defines {
            "FREEGTA_HOOKS"
        }

        files {
            "Code/FreeGTA/Resources/*.*",
            "Code/FreeGTA/SciTech/Include/*.*",
            "Code/FreeGTA/SciTech/MGL/*.*",
            "Code/FreeGTA/*.*"
        }

        includedirs {
            "Code/FreeGTA/SciTech/Include/",
            "Vendor/MinHook/include/"
        }

    -- MinHook Project (StaticLib)
    project("MinHook")
        kind("StaticLib")
        projectCommon("MinHook")

        files {
            "Vendor/MinHook/include/*.*",
            "Vendor/MinHook/src/hde/*.*",
            "Vendor/MinHook/src/*.*"
        }

        removefiles { "Vendor/MinHook/src/hde/hde64.c" } -- Only x86 is supported
