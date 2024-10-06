#include <stdio.h>
#include <stdlib.h>

#include <st/utils/util.h>

char *read_file(const char *path)
{
    FILE *file = fopen(path, "r");
    char *s = NULL;
    size_t len = 0, ret = 0;

    if (file) {

        fseek(file, 0, SEEK_END);
        len = ftell(file);
        fseek(file, 0, SEEK_SET);

        s = malloc(len + 1);
        if (s)
            ret = fread(s, 1, len, file);
        s[len] = '\0';

        fclose(file);

        if (len != ret) {
            free(s);
            s = NULL;
        }
    }

    return s;
}
