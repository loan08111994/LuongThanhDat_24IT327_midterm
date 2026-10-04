// fileinfo.c - quan ly danh sach file
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "fileinfo.h"

void list_init(struct FileList *l)
{
    l->items = NULL;
    l->count = 0;
    l->cap = 0;
}

int list_add(struct FileList *l, const char *path, const char *name, struct stat *st)
{
    if (l->count >= l->cap) {
        int ncap = l->cap == 0 ? 16 : l->cap * 2;
        struct FileInfo *p = realloc(l->items, ncap * sizeof(*p));
        if (!p) return -1;
        l->items = p;
        l->cap = ncap;
    }
    l->items[l->count].path = strdup(path);
    l->items[l->count].name = strdup(name);
    if (!l->items[l->count].path || !l->items[l->count].name) {
        free(l->items[l->count].path);
        free(l->items[l->count].name);
        return -1;
    }
    l->items[l->count].st = *st;
    l->count++;
    return 0;
}

void list_free(struct FileList *l)
{
    int i;
    for (i = 0; i < l->count; i++) {
        free(l->items[i].path);
        free(l->items[i].name);
    }
    free(l->items);
    l->items = NULL;
    l->count = 0;
    l->cap = 0;
}

// lay stat bang lstat, de giu symlink
int get_file_stat(const char *path, struct stat *st)
{
    return lstat(path, st);
}
