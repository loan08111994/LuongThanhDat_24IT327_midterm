// options.h - dinh nghia struct chua cac tuy chon cua ls
#ifndef OPTIONS_H
#define OPTIONS_H

// loai thoi gian dung de sap xep / in ra
#define TIME_MTIME 0
#define TIME_CTIME 1
#define TIME_ATIME 2

// kieu in long
#define LONG_NONE 0
#define LONG_L 1
#define LONG_N 2

// kieu hien thi size
#define SIZE_DEFAULT 0
#define SIZE_KB 1
#define SIZE_HUMAN 2

// kieu in ky tu dac biet
#define PRINT_AUTO 0
#define PRINT_Q 1
#define PRINT_W 2

struct Options {
    int a;          // -a hien file an
    int A;          // -A hien file an tru . va ..
    int time_type;  // -c / -u
    int d;          // -d
    int F;          // -F
    int f;          // -f khong sap xep
    int size_mode;  // -h / -k
    int i;          // -i in inode
    int long_mode;  // -l / -n
    int print_mode; // -q / -w
    int R;          // -R de quy
    int r;          // -r dao nguoc
    int S;          // -S sap xep theo size
    int s;          // -s in so block
    int t;          // -t sap xep theo thoi gian
};

void init_options(struct Options *op);
int parse_options(int argc, char *argv[], struct Options *op);

#endif
