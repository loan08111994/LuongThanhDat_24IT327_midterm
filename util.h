// util.h - cac ham phu tro
#ifndef UTIL_H
#define UTIL_H

#include <sys/stat.h>
#include "options.h"

void mode_to_str(mode_t mode, char *buf);
void human_size(long long bytes, char *buf, int bufsz);
long long display_blocks(struct stat *st, struct Options *op);
int get_blocksize(void);
void print_name(const char *name, struct Options *op);
char file_suffix(struct stat *st);
void format_time(struct stat *st, struct Options *op, char *buf, int bufsz);
int is_executable(struct stat *st);

#endif
