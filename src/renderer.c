#include <assert.h>

#include <st/engine.h>
#include <st/camera.h>
#include <st/renderer.h>

void renderer_init(StCamera *camera, StLight *light)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(renderer_init, camera, light);
}

void renderer_destroy(void)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(renderer_destroy);
}

void renderer_begin(void)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(renderer_begin);
}

void renderer_end(void)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(renderer_end);
}

void renderer_push_mesh(const StVertex *vertices,
    const size_t vertex_count, const unsigned int *indices, const size_t index_count)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(renderer_push_mesh, vertices, vertex_count, indices, index_count);
}

void renderer_draw_model(StModel *model, mat4 *model_matrix, vec4 color, StCamera *camera, StLight *light)
{
    StEngine *e = engine_context();
    assert(e);
    StWindow *window = e->window;
    assert(window);

    call_impl(renderer_push_model, model, model_matrix, color, camera, light);
}
