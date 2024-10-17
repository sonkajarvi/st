#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <st/utils/stringbuilder.h>
#include <st/utils/vector.h>

void st_sb_add(StStringBuilder *sb, const char *add)
{
    assert(sb);

    _StSBEntry entry;
    entry.ptr = add;
    entry.len = strlen(add);

    vector_push(sb->entries, entry);
}

char *st_sb_concat(StStringBuilder *sb)
{
    assert(sb);

    if (vector_isempty(sb->entries))
        return NULL;

    size_t total_len = 0, prev_len = 0;
    vector_for(sb->entries, _StSBEntry, e)
        total_len += e->len;

    char *ret;
    if ((ret = malloc(total_len + 1)) == NULL)
        return NULL;

    vector_for(sb->entries, _StSBEntry, e) {
        memcpy(ret + prev_len, e->ptr, e->len);
        prev_len += e->len;
    }
    ret[total_len] = '\0';

    return ret;
}

void st_sb_free(StStringBuilder *sb)
{
    if (sb)
        vector_free(sb->entries);
}
