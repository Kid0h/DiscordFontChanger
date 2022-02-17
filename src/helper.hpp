#pragma once

#include <filesystem>

// #define WIN32_LEAN_AND_MEAN
// #include <Windows.h>
// #include <TlHelp32.h>
// #include <psapi.h>

class helper {
public:
    static bool file_exists(const std::filesystem::path& path) { std::error_code e;  return std::filesystem::exists(path, e); }
    static bool delete_file(const std::filesystem::path& path) { std::error_code e; return std::filesystem::remove(path, e); }
    static bool copy_file(const std::filesystem::path& from, const std::filesystem::path& to) { std::error_code e; return std::filesystem::copy_file(from, to); }
    static bool create_directory(const std::filesystem::path& path) { std::error_code e; return std::filesystem::create_directories(path); }

    
	// static bool kill_process_by_id(DWORD PID)
	// {
	// 	HANDLE tmpHandle = OpenProcess(PROCESS_TERMINATE, TRUE, PID);
	// 	if (tmpHandle != NULL)
	// 		return TerminateProcess(tmpHandle, 0);
	// 	return false;
	// }

	// static bool kill_process(const wchar_t* name) {
    //     bool status = false;

    //     HANDLE hProcessSnap;
	// 	PROCESSENTRY32W pe32; pe32.dwSize = sizeof(PROCESSENTRY32W);

	// 	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	// 	if (Process32FirstW(hProcessSnap, &pe32)) {
    //         do {
    //             if (wcscmp(pe32.szExeFile, name) == 0) {
    //                 status = kill_process_by_id(pe32.th32ProcessID);
    //                 if (!status) {
    //                     break;
    //                 }
    //             }
    //         } while (Process32NextW(hProcessSnap, &pe32));
	// 	}

	// 	CloseHandle(hProcessSnap);
	// 	return status;
    // }

    // static void launch(const wchar_t* executable_path) {
	// 	//Token handle
	// 	HANDLE tokenHandle;
	// 	OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY, &tokenHandle);

	// 	// additional information
	// 	STARTUPINFOW si;
	// 	PROCESS_INFORMATION pi;

	// 	// set the size of the structures
	// 	ZeroMemory(&si, sizeof(si));
	// 	si.cb = sizeof(si);
	// 	ZeroMemory(&pi, sizeof(pi));

	// 	// start the program up
	// 	CreateProcessAsUserW(tokenHandle,
	// 		executable_path,// the path
	// 		L"",           // Command line
	// 		NULL,           // Process handle not inheritable
	// 		NULL,           // Thread handle not inheritable
	// 		FALSE,          // Set handle inheritance to FALSE
	// 		NULL,           // No creation flags
	// 		NULL,           // Use parent's environment block
	// 		NULL,           // Use parent's starting directory 
	// 		&si,            // Pointer to STARTUPINFO structure
	// 		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	// 	);

	// 	// Close process and thread handles. 
	// 	CloseHandle(pi.hProcess);
	// 	CloseHandle(pi.hThread);
    // }
};