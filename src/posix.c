#ifndef _WIN32

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "cloner.h"

bool run_git_clone(const char *url) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork failed"); return false; }

    if (pid == 0) {
        execvp("git", (char *[]){"git", "clone", (char *)url, NULL});
        perror("execvp failed");
        exit(1);
    }

    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) && WEXITSTATUS(status) == 0;
}

#endif