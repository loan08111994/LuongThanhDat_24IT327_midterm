// util.c - ham phu tro cho ls
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/stat.h>
#include "util.h"

// doi mode sang chuoi kieu "-rwxr-xr-x"
void mode_to_str(mode_t mode, char *buf)
{
    // ky tu dau : loai file
    if (S_ISDIR(mode)) buf[0] = 'd';
    else if (S_ISLNK(mode)) buf[0] = 'l';
    else if (S_ISCHR(mode)) buf[0] = 'c';
    else if (S_ISBLK(mode)) buf[0] = 'b';
    else if (S_ISFIFO(mode)) buf[0] = 'p';
    else if (S_ISSOCK(mode)) buf[0] = 's';
    else buf[0] = '-';

    // owner
    buf[1] = (mode & S_IRUSR) ? 'r' : '-';
    buf[2] = (mode & S_IWUSR) ? 'w' : '-';
    if (mode & S_ISUID) {
        buf[3] = (mode & S_IXUSR) ? 's' : 'S';
    } else {
        buf[3] = (mode & S_IXUSR) ? 'x' : '-';
    }
    // group
    buf[4] = (mode & S_IRGRP) ? 'r' : '-';
    buf[5] = (mode & S_IWGRP) ? 'w' : '-';
    if (mode & S_ISGID) {
        buf[6] = (mode & S_IXGRP) ? 's' : 'S';
    } else {
        buf[6] = (mode & S_IXGRP) ? 'x' : '-';
    }
    // other
    buf[7] = (mode & S_IROTH) ? 'r' : '-';
    buf[8] = (mode & S_IWOTH) ? 'w' : '-';
    if (mode & S_ISVTX) {
        buf[9] = (mode & S_IXOTH) ? 't' : 'T';
    } else {
        buf[9] = (mode & S_IXOTH) ? 'x' : '-';
    }
    buf[10] = '\0';
}

// doi so byte sang dang doc duoc: 1K, 2M ...
void human_size(long long bytes, char *buf, int bufsz)
{
    const char *u[] = {"B", "K", "M", "G", "T"};
    double v = (double)bytes;
    int i = 0;
    while (v >= 1024 && i < 4) {
        v = v / 1024;
        i++;
    }
    if (i == 0)
        snprintf(buf, bufsz, "%lldB", bytes);
    else if (v < 10)
        snprintf(buf, bufsz, "%.1f%s", v, u[i]);
    else
        snprintf(buf, bufsz, "%.0f%s", v, u[i]);
}

// doc BLOCKSIZE tu moi truong, mac dinh 512
int get_blocksize(void)
{
    char *p = getenv("BLOCKSIZE");
    long v;
    if (!p || !*p) return 512;
    v = atol(p);
    if (v <= 0) return 512;
    return (int)v;
}

// so block hien thi theo -h / -k / BLOCKSIZE
long long display_blocks(struct stat *st, struct Options *op)
{
    long long bytes = (long long)st->st_blocks * 512;
    if (op->size_mode == SIZE_HUMAN) {
        // truong hop -h thi ham in se tu doi sang chuoi,
        // o day tra ve so block 512 de tinh total, con khi in thi doi rieng
        // nhung de don gian ta tra ve bytes, nguoi goi tu xu ly
        return bytes;
    }
    if (op->size_mode == SIZE_KB) {
        return (bytes + 1023) / 1024;
    }
    {
        int bs = get_blocksize();
        return (bytes + bs - 1) / bs;
    }
}

int is_executable(struct stat *st)
{
    if (S_ISDIR(st->st_mode)) return 0;
    if (st->st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) return 1;
    return 0;
}

// hau to cho -F
char file_suffix(struct stat *st)
{
    if (S_ISDIR(st->st_mode)) return '/';
    if (S_ISLNK(st->st_mode)) return '@';
    if (S_ISFIFO(st->st_mode)) return '|';
    if (S_ISSOCK(st->st_mode)) return '=';
    if (is_executable(st)) return '*';
    return 0;
}

// in ten file, xu ly -q / -w
void print_name(const char *name, struct Options *op)
{
    int use_q = 0;
    int i;

    if (op->print_mode == PRINT_Q) use_q = 1;
    else if (op->print_mode == PRINT_W) use_q = 0;
    else {
        // mac dinh: ra terminal thi dung -q
        use_q = isatty(1) ? 1 : 0;
    }

    if (!use_q) {
        fputs(name, stdout);
        return;
    }
    for (i = 0; name[i]; i++) {
        unsigned char c = name[i];
        if (isprint(c))
            putchar(c);
        else
            putchar('?');
    }
}

// lay moc thoi gian theo -c / -u roi doi sang "Oct 13 10:30"
void format_time(struct stat *st, struct Options *op, char *buf, int bufsz)
{
    time_t t = st->st_mtime;
    struct tm *tm;
    static const char *mon[] = {"Jan","Feb","Mar","Apr","May","Jun",
        "Jul","Aug","Sep","Oct","Nov","Dec"};

    if (op->time_type == TIME_CTIME) t = st->st_ctime;
    else if (op->time_type == TIME_ATIME) t = st->st_atime;

    tm = localtime(&t);
    if (!tm) {
        snprintf(buf, bufsz, "??? ?? ??:??");
        return;
    }
    snprintf(buf, bufsz, "%s %2d %02d:%02d",
        mon[tm->tm_mon], tm->tm_mday, tm->tm_hour, tm->tm_min);
}
