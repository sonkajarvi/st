#ifndef ST_EVENT_H
#define ST_EVENT_H

// Used as index in StEventHandler
#define ST_EVENT_WINDOW_RESIZE 0

#define ST_EVENT_FIRST ST_EVENT_WINDOW_RESIZE
#define ST_EVENT_LAST ST_EVENT_WINDOW_RESIZE
#define ST_EVENT_LENGTH (ST_EVENT_WINDOW_RESIZE + 1)

typedef struct StEventDataWindowResize
{
    int width;
    int height;
} StEventDataWindowResize;

typedef union StEventData
{
    StEventDataWindowResize window_resize;
} StEventData;

// Event list node
typedef struct StEvent
{
    struct StEvent *next;
    void (*callback)(StEventData *);
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
void st_event_trigger(int type, StEventData data);

StEventList *_st_event_list_from_type(int type);

#endif // ST_EVENT_H
