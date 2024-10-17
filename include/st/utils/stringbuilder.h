#ifndef ST_UTILS_STRINGBUILDER_H
#define ST_UTILS_STRINGBUILDER_H

#include <stddef.h>

typedef struct _StSBEntry
{
    const char *ptr;
    size_t len;
} _StSBEntry;

typedef struct StStringBuilder
{
    _StSBEntry *entries;
} StStringBuilder;

void st_sb_add(StStringBuilder *sb, const char *add);
char *st_sb_concat(StStringBuilder *sb);
void st_sb_free(StStringBuilder *sb);

#endif // ST_UTILS_STRINGBUILDER_H
