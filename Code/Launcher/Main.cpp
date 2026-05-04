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

#include <stdio.h>
#include <Windows.h>

#define MAXWAIT 10000

#define __EXE "GTA\\WINO\\Grand Theft Auto.exe"

#ifdef _DEBUG
#   define __DLL "FreeGTA_d.dll"
#else
#   define __DLL "FreeGTA.dll"
#endif

// Implementation
//////////////////////////////////////////////////////////////////////////////////////////////////

static bool FileExists(const char* path)
{
	DWORD attrib = GetFileAttributes(path);
	return (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}

static bool InjectDll(const char* dll, const char* targetPath, DWORD* lastErrorCode)
{
	if (!FileExists(dll) || !FileExists(targetPath))
	{
		if (lastErrorCode)
			*lastErrorCode = ERROR_FILE_NOT_FOUND;
		return false;
	}

	HMODULE moduleHandle = GetModuleHandle("kernel32.dll");

	if (!moduleHandle)
	{
		if (lastErrorCode)
			*lastErrorCode = GetLastError();
		return false;
	}

	LPVOID loadLibraryPtr = GetProcAddress(moduleHandle, "LoadLibraryA");

	if (!loadLibraryPtr)
	{
		if (lastErrorCode)
			*lastErrorCode = GetLastError();
		return false;
	}

	STARTUPINFO startupInfo;
	PROCESS_INFORMATION processInformation;

	memset(&startupInfo, 0, sizeof(startupInfo));
	startupInfo.cb = sizeof(STARTUPINFO);

	if (!CreateProcess(targetPath, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &startupInfo, &processInformation))
	{
		if (lastErrorCode)
			*lastErrorCode = GetLastError();
		return false;
	}

	size_t length = strlen(dll) * sizeof(char);
	LPVOID remoteString = VirtualAllocEx(processInformation.hProcess, NULL, length + 1, MEM_COMMIT, PAGE_READWRITE);
	
	if (!remoteString)
	{
		CloseHandle(processInformation.hProcess);
		CloseHandle(processInformation.hThread);

		if (lastErrorCode)
			*lastErrorCode = GetLastError();
		return false;
	}

	if (!WriteProcessMemory(processInformation.hProcess, remoteString, dll, length, NULL))
	{
		VirtualFreeEx(processInformation.hProcess, remoteString, 0, MEM_RELEASE);
		CloseHandle(processInformation.hProcess);
		CloseHandle(processInformation.hThread);

		if (lastErrorCode)
			*lastErrorCode = GetLastError();
		return false;
	}

	HANDLE remoteThread = CreateRemoteThread(processInformation.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryPtr, remoteString, 0, NULL);

	if (!remoteThread)
	{
		VirtualFreeEx(processInformation.hProcess, remoteString, 0, MEM_RELEASE);
		CloseHandle(processInformation.hProcess);
		CloseHandle(processInformation.hThread);

		if (lastErrorCode)
			*lastErrorCode = GetLastError();
		return false;
	}

	if (WaitForSingleObject(remoteThread, MAXWAIT) != WAIT_TIMEOUT)
	{
		ResumeThread(processInformation.hThread);
		VirtualFreeEx(processInformation.hProcess, remoteString, 0, MEM_RELEASE);
		CloseHandle(processInformation.hProcess);
		CloseHandle(processInformation.hThread);

		return true;
	}

	VirtualFreeEx(processInformation.hProcess, remoteString, 0, MEM_RELEASE);
	CloseHandle(processInformation.hProcess);
	CloseHandle(processInformation.hThread);

	if (lastErrorCode)
		*lastErrorCode = ERROR_TIMEOUT;
	return false;
}

int main(int argc, char* argv[])
{
	printf("FreeGTA Launcher\n");
	printf("Injecting '%s' in '%s' process...\n", __DLL, __EXE);
	
	DWORD lastErrorCode = 0;
	bool success = InjectDll(__DLL, __EXE, &lastErrorCode);
	
	if (!success)
	{
		printf("Failed to inject the DLL (LastError: 0x%08x).\n", lastErrorCode);
		return 1;
	}

	printf("DLL injected successfully.\n");

	return 0;
}
