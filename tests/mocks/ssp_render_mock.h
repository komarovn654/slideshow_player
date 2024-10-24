#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ssp_render.h"

int ssp_render_init_buffers_mock_p(void* render);
int ssp_render_init_buffers_mock_f(void* render);
int ssp_render_set_shaders_mock_p(void* render);
int ssp_render_set_shaders_mock_f(void* render);
int ssp_render_set_shaders_mock_null(void* render);
int ssp_render_setup_texture_mock_f(void* render);
int ssp_render_setup_texture_mock_p(void* render);

#ifdef __cplusplus
}
#endif
