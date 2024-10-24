#include <string.h>

#include "ssp_render_mock.h"
#include "ssp_shader.h"


int ssp_render_init_buffers_mock_p(void* render)
{
    return 0;
}

int ssp_render_init_buffers_mock_f(void* render)
{
    return 1;
}

int ssp_render_set_shaders_mock_p(void* render)
{
    return 0;
}

int ssp_render_set_shaders_mock_f(void* render)
{
    return 1;
}

int ssp_render_set_shaders_mock_null(void* render)
{
    ssp_render_t* r = (ssp_render_t*)render;
    r->shaders = NULL;

    return 0;
}

int ssp_render_setup_texture_mock_f(void* render)
{
    return 0;
}

int ssp_render_setup_texture_mock_p(void* render)
{
    return 1;
}
