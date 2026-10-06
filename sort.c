// sort.c - sap xep danh sach file
#include <string.h>
#include <stdlib.h>
#include "sort.h"

static struct Options *g_op;

// lay moc thoi gian de so sanh
static long get_time(struct stat *st, struct Options *op)
{
    if (op->time_type == TIME_CTIME) return (long)st->st_ctime;
    if (op->time_type == TIME_ATIME) return (long)st->st_atime;
    return (long)st->st_mtime;
}

static int cmp(const void *pa, const void *pb)
{
    const struct FileInfo *a = pa;
    const struct FileInfo *b = pb;
    int r = 0;

    // uu tien -S truoc, roi toi -t, cuoi cung la ten
    if (g_op->S) {
        if (a->st.st_size < b->st.st_size) r = 1;
        else if (a->st.st_size > b->st.st_size) r = -1;
        else r = strcmp(a->name, b->name);
    } else if (g_op->t) {
        long ta = get_time((struct stat *)&a->st, g_op);
        long tb = get_time((struct stat *)&b->st, g_op);
        if (ta < tb) r = 1;
        else if (ta > tb) r = -1;
        else r = strcmp(a->name, b->name);
    } else {
        r = strcmp(a->name, b->name);
    }

    if (g_op->r) r = -r;
    return r;
}

void sort_files(struct FileList *l, struct Options *op)
{
    if (op->f) return; // -f thi giu nguyen thu tu
    if (l->count <= 1) return;
    g_op = op;
    qsort(l->items, l->count, sizeof(struct FileInfo), cmp);
}
