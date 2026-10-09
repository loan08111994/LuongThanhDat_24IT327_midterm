// display.h - in thong tin file / thu muc
#ifndef DISPLAY_H
#define DISPLAY_H

#include "fileinfo.h"
#include "options.h"

void show_file(struct FileInfo *fi, struct Options *op, int is_tty);
void show_dir(const char *dirpath, struct Options *op, int is_tty, int show_header, int *exit_code);

#endif
