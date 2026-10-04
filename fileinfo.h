// fileinfo.h - lay thong tin file
#ifndef FILEINFO_H
#define FILEINFO_H

#include <sys/stat.h>

struct FileInfo {
    char *path; // duong dan day du
    char *name; // ten de hien thi
    struct stat st;
};

struct FileList {
    struct FileInfo *items;
    int count;
    int cap;
};

void list_init(struct FileList *l);
int list_add(struct FileList *l, const char *path, const char *name, struct stat *st);
void list_free(struct FileList *l);
int get_file_stat(const char *path, struct stat *st);

#endif
