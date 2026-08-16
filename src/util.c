#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include "cloner.h"

void get_repo_name(const char *url, char *out, size_t out_size) {
    // find last '/'
    const char *last_slash = strrchr(url, '/');
    const char *name_start = last_slash ? last_slash + 1 : url;

    strncpy(out, name_start, out_size - 1);
    out[out_size - 1] = '\0';

    // strip trailing ".git" if present
    size_t len = strlen(out);
    if (len > 4 && strcmp(out + len - 4, ".git") == 0) {
        out[len - 4] = '\0';
    }
}

bool flatten_dir(const char *subdir) {
    DIR *d = opendir(subdir);
    if (!d) {
        perror("opendir failed");
        return false;
    }

    struct dirent *entry;
    char src[1024], dst[1024];

    while ((entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(src, sizeof(src), "%s/%s", subdir, entry->d_name);
        snprintf(dst, sizeof(dst), "./%s", entry->d_name);

        if (rename(src, dst) != 0) {
            perror("rename failed");
            closedir(d);
            return false;
        }
    }

    closedir(d);

    if (rmdir(subdir) != 0) {
        perror("rmdir failed");
        return false;
    }

    return true;
}