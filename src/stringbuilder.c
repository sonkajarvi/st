#include <stdlib.h>
#include <string.h>

#include <st/util/assert.h>
#include <st/util/stringbuilder.h>
#include <st/util/util.h>

struct sb_node
{
    struct st_lsnode node;
    const char *str;
    size_t len;
};

static inline struct sb_node *__add_string(struct st_lsnode **head, const char *str)
{
    struct sb_node *tmp = malloc(sizeof(*tmp));
    st_assert(tmp);

    tmp->str = str;
    tmp->len = strlen(str);
    tmp->node.next = *head;

    *head = &tmp->node;
    return tmp;
}

void st_strbuilder_free(struct st_strbuilder *sb)
{
    st_assert(sb);

    if (!sb->head)
        return;

    struct st_lsnode *tmp = sb->head;
    while (tmp) {
        struct sb_node *cont = st_container_of(tmp, struct sb_node, node);
        tmp = tmp->next;

        free(cont);
    }

    sb->head = NULL;
    sb->length = 0;
}

// todo: Return status code?
void st_strbuilder_append(struct st_strbuilder *sb, const char *str)
{
    st_assert(sb);

    // Do nothing if string is NULL or empty
    if (!str || !str[0])
        return;

    const struct sb_node *tmp = __add_string(&sb->head, str);
    sb->length += tmp->len;
}

// todo: Return status code?
void st_strbuilder_insert(struct st_strbuilder *sb, const size_t index, const char *str)
{
    st_assert(sb);

    // Do nothing if string is NULL or empty
    if (!str || !str[0])
        return;

    // If string builder is empty, we can just append
    if (!sb->head)
        goto append;

    // Out of bounds, just append
    if (index >= sb->length)
        goto append;

    // String builder uses a singly linked list to store the strings,
    // so they are in reverse order. Therefore indexing is a bit tricky
    //
    //  56789      01234
    //  v          v
    // "world" -> "hello" -> NULL

    struct st_lsnode *head = sb->head;
    size_t offset = sb->length;

    while (head) {
        struct sb_node *cont = st_container_of(head, struct sb_node, node);
        offset -= cont->len;

        if (index < offset)
            goto skip;

        // If 'index' equals to a node's starting index,
        // 'str' can be inserted right after, without splitting the node
        if (index == offset) {
            const struct sb_node *tmp = __add_string(&head->next, str);
            sb->length += tmp->len;
            return;
        }

        // 'index' is in the middle of a node, so it has to be split

        // Middle node
        struct sb_node *mid = __add_string(&head->next, str);
        sb->length += mid->len;

        // Right node
        struct sb_node *right = __add_string(&mid->node.next, cont->str);
        right->len = cont->len - (index - offset);

        // Left node
        cont->str += right->len;
        cont->len -= right->len;

        return;
skip:
        head = head->next;
    }

append:
    st_strbuilder_append(sb, str);
}

char *st_strbuilder_concat(struct st_strbuilder *sb)
{
    st_assert(sb);

    if (!sb->head)
        return NULL;

    char *ret = malloc(sb->length + 1);

    // The linked list is in reverse order,
    // so the strings have to be copied in reverse order
    size_t stride = sb->length;
    for (struct st_lsnode *node = sb->head; node; node = node->next) {
        struct sb_node *cont = st_container_of(node, struct sb_node, node);

        stride -= cont->len;
        memcpy(ret + stride, cont->str, cont->len);
    }
    ret[sb->length] = '\0';

    return ret;
}
