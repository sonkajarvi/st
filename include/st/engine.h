#ifndef ST_ENGINE_H
#define ST_ENGINE_H

#include <st/camera.h>
#include <st/model.h>
#include <st/renderer.h>
#include <st/window.h>

typedef struct StEngine
{
    StWindow *window;
    struct {
        double (*engine_time)(StWindow *);
        void (*window_create)(StWindow *, const char *, int, int);
        void (*window_destroy)(StWindow *);
        void (*window_show)(StWindow *);
        void (*window_get_size)(StWindow *, int *, int *);
        void (*window_get_pos)(StWindow *, int *, int *);

        void (*poll_events)(StWindow *);
        void (*swap_buffers)(StWindow *);
        
        void (*context_create)(StWindow *);
        void (*context_destroy)(StWindow *);
        void (*window_vsync)(StWindow *, bool);
    
        void (*renderer_init)(StCamera *, StLight *);
        void (*renderer_destroy)(void);
        void (*renderer_begin)(void);
        void (*renderer_end)(void);
        void (*renderer_push_mesh)(const StVertex *, const size_t, const unsigned int *, const size_t);
    } impl;
} StEngine;

extern StEngine *__global_engine_context;

void engine_init(void);
void engine_destroy(void);
StEngine *engine_context(void);

#endif // ST_ENGINE_H
