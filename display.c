// display.c - in file va thu muc ra man hinh
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysmacros.h>
#include "display.h"
#include "sort.h"
#include "util.h"

// in 1 file tren 1 dong, kem -i -s -l -F
void show_file(struct FileInfo *fi, struct Options *op, int is_tty)
{
    (void)is_tty;

    if (op->i)
        printf("%lu ", (unsigned long)fi->st.st_ino);

    if (op->s) {
        if (op->size_mode == SIZE_HUMAN) {
            char hb[32];
            human_size((long long)fi->st.st_blocks * 512, hb, sizeof(hb));
            printf("%s ", hb);
        } else {
            printf("%lld ", display_blocks(&fi->st, op));
        }
    }

    if (op->long_mode != LONG_NONE) {
        char mode[16], tbuf[64];
        mode_to_str(fi->st.st_mode, mode);
        format_time(&fi->st, op, tbuf, sizeof(tbuf));

        printf("%s %lu ", mode, (unsigned long)fi->st.st_nlink);

        // ten owner / group, -n thi in so
        if (op->long_mode == LONG_N) {
            printf("%d %d ", (int)fi->st.st_uid, (int)fi->st.st_gid);
        } else {
            struct passwd *pw = getpwuid(fi->st.st_uid);
            struct group *gr = getgrgid(fi->st.st_gid);
            if (pw) printf("%s ", pw->pw_name);
            else printf("%d ", (int)fi->st.st_uid);
            if (gr) printf("%s ", gr->gr_name);
            else printf("%d ", (int)fi->st.st_gid);
        }

        // size hoac major, minor
        if (S_ISCHR(fi->st.st_mode) || S_ISBLK(fi->st.st_mode)) {
            printf("%d, %d ", (int)major(fi->st.st_rdev),
                (int)minor(fi->st.st_rdev));
        } else if (op->size_mode == SIZE_HUMAN) {
            char hb[32];
            human_size((long long)fi->st.st_size, hb, sizeof(hb));
            printf("%s ", hb);
        } else {
            printf("%lld ", (long long)fi->st.st_size);
        }

        printf("%s ", tbuf);
    }

    print_name(fi->name, op);

    if (op->F) {
        char s = file_suffix(&fi->st);
        if (s) putchar(s);
    }

    // symlink thi in them " -> dich"
    if (op->long_mode != LONG_NONE && S_ISLNK(fi->st.st_mode)) {
        char linkto[1024];
        ssize_t n = readlink(fi->path, linkto, sizeof(linkto) - 1);
        if (n >= 0) {
            linkto[n] = '\0';
            printf(" -> %s", linkto);
        }
    }

    putchar('\n');
}

// co bo qua file an khong
static int skip_dot(const char *name, struct Options *op)
{
    if (name[0] != '.') return 0;
    if (op->a) return 0; // -a giu het
    if (op->A) {
        // -A bo . va .. thoi
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) return 1;
        return 0;
    }
    return 1; // mac dinh an het
}

// in noi dung 1 thu muc, co -R thi goi de quy
void show_dir(const char *dirpath, struct Options *op, int is_tty, int show_header, int *exit_code)
{
    DIR *dp;
    struct dirent *e;
    struct FileList list;
    int i;
    long long total = 0;

    dp = opendir(dirpath);
    if (!dp) {
        fprintf(stderr, "myls: %s: %s\n", dirpath, strerror(errno));
        *exit_code = 1;
        return;
    }

    list_init(&list);

    while ((e = readdir(dp)) != NULL) {
        char full[4096];
        struct stat st;
        if (skip_dot(e->d_name, op)) continue;
        snprintf(full, sizeof(full), "%s/%s", dirpath, e->d_name);
        if (lstat(full, &st) < 0) {
            fprintf(stderr, "myls: %s: %s\n", full, strerror(errno));
            *exit_code = 1;
            continue;
        }
        if (list_add(&list, full, e->d_name, &st) < 0) {
            fprintf(stderr, "myls: het bo nho\n");
            *exit_code = 1;
            break;
        }
    }
    closedir(dp);

    sort_files(&list, op);

    if (show_header)
        printf("%s:\n", dirpath);

    // dong total cho -l, con -s chi in khi ra terminal
    if (op->long_mode != LONG_NONE || (op->s && is_tty)) {
        for (i = 0; i < list.count; i++)
            total += list.items[i].st.st_blocks;
        // total tinh theo don vi 512, doi ra kieu hien thi
        if (op->size_mode == SIZE_HUMAN) {
            char hb[32];
            human_size(total * 512, hb, sizeof(hb));
            // giu don gian: in so block kieu cu neu khong phai -h ?
            // o day in tong so block 512 cho giong ls that thi hon,
            // nhung voi -h ta in dang doc duoc
            printf("total %s\n", hb);
        } else if (op->size_mode == SIZE_KB) {
            printf("total %lld\n", (total * 512 + 1023) / 1024);
        } else {
            int bs = get_blocksize();
            printf("total %lld\n", (total * 512 + bs - 1) / bs);
        }
    }

    for (i = 0; i < list.count; i++)
        show_file(&list.items[i], op, is_tty);

    // de quy vao thu muc con
    if (op->R) {
        for (i = 0; i < list.count; i++) {
            // bo qua . va ..
            if (strcmp(list.items[i].name, ".") == 0 ||
                strcmp(list.items[i].name, "..") == 0)
                continue;
            // chi xuong thu muc that, khong theo symlink
            if (S_ISDIR(list.items[i].st.st_mode)) {
                printf("\n");
                show_dir(list.items[i].path, op, is_tty, 1, exit_code);
            }
        }
    }

    list_free(&list);
}
