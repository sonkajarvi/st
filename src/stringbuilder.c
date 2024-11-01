#include <stdlib.h>
#include <string.h>

#include <st/utility/assert.h>
#include <st/utility/stringbuilder.h>
#include <st/utility/util.h>

struct sb_node
{
    struct st_lsnode node;
    const char *str;
    size_t len;
};

void st_strbuilder_free(struct st_strbuilder *sb)
{
    st_assert(sb);

    if (!sb->head)
        return;

    struct st_lsnode *node = sb->head;
    while (node) {
        struct sb_node *cont = st_container_of(node, struct sb_node, node);
        node = node->next;

        free(cont);
    }

    sb->head = NULL;
}

// todo: Return status code?
void st_strbuilder_append(struct st_strbuilder *sb, const char *str)
{
    st_assert(sb);

    // Do nothing, if string is NULL or empty
    if (!str || !str[0])
        return;

    struct sb_node *node = malloc(sizeof(*node));
    st_assert(node);

    node->str = str;
    node->len = strlen(str);
    node->node.next = sb->head ? : NULL;

    sb->head = &node->node;
}

char *st_strbuilder_concat(struct st_strbuilder *sb)
{
    st_assert(sb);

    if (!sb->head)
        return NULL;

    const size_t len = st_strbuilder_length(sb);
    char *ret = malloc(len + 1);

    // The linked list is in reverse order,
    // so the strings have to be copied in reverse order
    size_t stride = len;
    for (struct st_lsnode *node = sb->head; node; node = node->next) {
        struct sb_node *cont = st_container_of(node, struct sb_node, node);

        stride -= cont->len;
        memcpy(ret + stride, cont->str, cont->len);
    }
    ret[len] = '\0';

    return ret;
}

size_t st_strbuilder_length(struct st_strbuilder *sb)
{
    st_assert(sb);

    if (!sb->head)
        return 0;

    size_t len = 0;
    for (struct st_lsnode *node = sb->head; node; node = node->next) {
        struct sb_node *cont = st_container_of(node, struct sb_node, node);
        len += cont->len;
    }
    return len;
}
