#include <assert.h>
#include <stdlib.h>

#include <st/engine.h>
#include <st/event.h>
#include <st/utils/print.h>

StEvent *st_event_add_listener(int type, void (*callback)())
{
    if (!callback)
        return NULL;

    StEventList *list = _st_event_list_from_type(type);
    assert(list);

    StEvent *event = malloc(sizeof(*event));
    assert(event);

    event->callback = callback;
    event->hint = type;
    event->next = NULL;

    if (list->head)
        event->next = list->head;
    list->head = event;

    st_debug("Added event listener (event=%p, type=%d, callback=%p)\n", event, type, callback);

    return event;
}

void st_event_remove_listener(StEvent *event)
{
    if (!event)
        return;

    StEventList *list = _st_event_list_from_type(event->hint);
    assert(list);

    StEvent **tmp = &list->head;
    while ((*tmp) != event)
        tmp = &(*tmp)->next;
    *tmp = event->next;
}

void st_event_clear(int type)
{
    StEventList *list = _st_event_list_from_type(type);
    assert(list);

    StEvent *curr = list->head, *next = NULL;
    while (curr) {
        next = curr->next;
        free(curr);
        curr = next;
    }

    list->head = NULL;

    st_debug("Cleared event listeners (type=%d)\n", type);
}

void st_event_trigger(int type, StEventData data)
{
    StEventList *list = _st_event_list_from_type(type);
    assert(list);

    for (StEvent *event = list->head; event; event = event->next)
        event->callback(&data);
}

StEventList *_st_event_list_from_type(int type)
{
    St *st = st_instance();
    if (!st)
        return NULL;

    if (type < ST_EVENT_FIRST || type > ST_EVENT_LAST)
        return NULL;

    return &st->event_handler[type];
}
