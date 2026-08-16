#ifndef CLONER_H
#define CLONER_H

#include <stdbool.h>

bool run_git_clone(const char *url);
void get_repo_name(const char *url, char *out, size_t out_size);
bool flatten_dir(const char *subdir);

#endif