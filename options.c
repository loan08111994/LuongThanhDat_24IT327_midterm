// options.c - doc tuy chon dong lenh
#include <unistd.h>
#include <sys/types.h>
#include "options.h"

void init_options(struct Options *op)
{
    op->a = 0;
    op->A = 0;
    op->time_type = TIME_MTIME;
    op->d = 0;
    op->F = 0;
    op->f = 0;
    op->size_mode = SIZE_DEFAULT;
    op->i = 0;
    op->long_mode = LONG_NONE;
    op->print_mode = PRINT_AUTO;
    op->R = 0;
    op->r = 0;
    op->S = 0;
    op->s = 0;
    op->t = 0;
}

int parse_options(int argc, char *argv[], struct Options *op)
{
    int c;

    // chuoi tuy chon theo manual
    while ((c = getopt(argc, argv, "AacdfFhiklnqRrSstuw")) != -1) {
        if (c == 'A') op->A = 1;
        else if (c == 'a') op->a = 1;
        else if (c == 'c') op->time_type = TIME_CTIME;
        else if (c == 'd') { op->d = 1; op->R = 0; }
        else if (c == 'F') op->F = 1;
        else if (c == 'f') op->f = 1;
        else if (c == 'h') op->size_mode = SIZE_HUMAN;
        else if (c == 'i') op->i = 1;
        else if (c == 'k') op->size_mode = SIZE_KB;
        else if (c == 'l') op->long_mode = LONG_L;
        else if (c == 'n') op->long_mode = LONG_N;
        else if (c == 'q') op->print_mode = PRINT_Q;
        else if (c == 'R') { op->R = 1; op->d = 0; }
        else if (c == 'r') op->r = 1;
        else if (c == 'S') op->S = 1;
        else if (c == 's') op->s = 1;
        else if (c == 't') op->t = 1;
        else if (c == 'u') op->time_type = TIME_ATIME;
        else if (c == 'w') op->print_mode = PRINT_W;
        else return -1;
    }

    // root luon thay file an nhu -A
    if (geteuid() == 0)
        op->A = 1;

    return optind;
}
