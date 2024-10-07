#ifndef ST_EVENT_H
#define ST_EVENT_H

// Used as index in StEventHandler
#define ST_EVENT_WINDOW_RESIZE 0

#define ST_EVENT_FIRST ST_EVENT_WINDOW_RESIZE
#define ST_EVENT_LAST ST_EVENT_WINDOW_RESIZE
#define ST_EVENT_LENGTH (ST_EVENT_WINDOW_RESIZE + 1)

#define st_event_trigger(___type, ...) ({ \
    StEngine *___engine = st_engine_context(); \
    assert(___engine); \
    StEventList *___list = _st_event_list_from_type((___type)); \
    assert(___list); \
    for (StEvent *___event = ___list->head; ___event; ___event = ___event->next) \
        ___event->callback(__VA_ARGS__); \
})

// Event list node
typedef struct StEvent
{
    struct StEvent *next;
    void (*callback)();
    unsigned char hint; // Type hint for removing nodes
} StEvent;

typedef struct StEventList
{
    StEvent *head;
} StEventList;

typedef StEventList StEventHandler[ST_EVENT_LENGTH];

StEvent *st_event_add_listener(int type, void (*callback)());
void st_event_remove_listener(StEvent *event);
void st_event_clear(int type);

StEventList *_st_event_list_from_type(int type);

#endif // ST_EVENT_H
