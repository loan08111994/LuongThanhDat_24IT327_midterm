// main.c - ham main, chia file / thu muc roi in ra
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "options.h"
#include "fileinfo.h"
#include "sort.h"
#include "display.h"

// kiem tra duong dan co phai thu muc can mo ra khong
static int is_dir_arg(const char *path, struct stat *lst, struct Options *op)
{
    struct stat st;

    if (op->d) return 0; // -d thi coi nhu file het
    if (S_ISDIR(lst->st_mode)) return 1;
    if (S_ISLNK(lst->st_mode)) {
        // symlink tro toi thu muc thi mo ra
        if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
            return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    struct Options op;
    struct FileList files, dirs;
    int first, i, n;
    int exit_code = 0;
    int is_tty = isatty(1);
    struct stat st;

    init_options(&op);
    first = parse_options(argc, argv, &op);
    if (first < 0) {
        fprintf(stderr, "dung: %s [-AacdFfhiklnqRrSstuw] [file ...]\n", argv[0]);
        return 1;
    }

    list_init(&files);
    list_init(&dirs);

    n = argc - first;
    if (n == 0) {
        // khong co doi so thi liet ke thu muc hien tai
        show_dir(".", &op, is_tty, 0, &exit_code);
        return exit_code;
    }

    // tach doi so ra 2 nhom: file va thu muc
    for (i = first; i < argc; i++) {
        if (lstat(argv[i], &st) < 0) {
            fprintf(stderr, "myls: %s: %s\n", argv[i], strerror(errno));
            exit_code = 1;
            continue;
        }
        if (is_dir_arg(argv[i], &st, &op)) {
            // lay stat that de sap xep cho dung (khong quan trong lam)
            list_add(&dirs, argv[i], argv[i], &st);
        } else {
            list_add(&files, argv[i], argv[i], &st);
        }
    }

    // sap xep rieng tung nhom, file truoc thu muc sau
    sort_files(&files, &op);
    sort_files(&dirs, &op);

    for (i = 0; i < files.count; i++)
        show_file(&files.items[i], &op, is_tty);

    for (i = 0; i < dirs.count; i++) {
        // nhieu doi so thi in ten thu muc
        int need_header = (files.count + dirs.count > 1) ? 1 : 0;
        if (files.count > 0 && i == 0 && dirs.count > 0)
            printf("\n");
        else if (i > 0)
            printf("\n");
        // neu -d thi da coi nhu file roi, khong toi day
        show_dir(dirs.items[i].path, &op, is_tty, need_header, &exit_code);
    }

    list_free(&files);
    list_free(&dirs);
    return exit_code;
}
