#ifdef _WIN32

#include <stdio.h>
#include <windows.h>
#include "cloner.h"

bool run_git_clone(const char *url) {
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "git clone %s", url);

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "CreateProcess failed\n");
        return false;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return exitCode == 0;
}

#endif