#include <stdio.h>
#include "cloner.h"

#define VERSION "1.1"

int main(int argc, char *argv[]) {
    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        printf("cloner version %s\nby zizies <3\n");
        return 0;
    }
    if (argc != 2) {
        fprintf(stderr, "usage: %s <repo-url>\n", argv[0]);
        return 1;
    }

    const char *url = argv[1];

    if (!run_git_clone(url)) {
        fprintf(stderr, "git clone failed\n");
        return 1;
    }

    char repo_name[256];
    get_repo_name(url, repo_name, sizeof(repo_name));

    if (!flatten_dir(repo_name)) {
        fprintf(stderr, "failed to flatten %s\n", repo_name);
        return 1;
    }

    printf("done - contents of %s moved into current dir\n", repo_name);
    return 0;
}