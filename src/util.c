#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

    // collect names first - renaming entries out from under readdir is
    // undefined and can silently skip files
    char **names = NULL;
    size_t count = 0, cap = 0;
    struct dirent *entry;

    while ((entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (count == cap) {
            size_t new_cap = cap ? cap * 2 : 32;
            char **grown = realloc(names, new_cap * sizeof(*names));
            if (!grown) {
                perror("realloc failed");
                goto fail;
            }
            names = grown;
            cap = new_cap;
        }

        names[count] = strdup(entry->d_name);
        if (!names[count]) {
            perror("strdup failed");
            goto fail;
        }
        count++;
    }

    closedir(d);
    d = NULL;

    char src[1024], dst[1024];
    for (size_t i = 0; i < count; i++) {
        snprintf(src, sizeof(src), "%s/%s", subdir, names[i]);
        snprintf(dst, sizeof(dst), "./%s", names[i]);

        if (rename(src, dst) != 0) {
            perror("rename failed");
            goto fail;
        }
    }

    for (size_t i = 0; i < count; i++) free(names[i]);
    free(names);

    if (rmdir(subdir) != 0) {
        perror("rmdir failed");
        return false;
    }

    return true;

fail:
    for (size_t i = 0; i < count; i++) free(names[i]);
    free(names);
    if (d) closedir(d);
    return false;
}
