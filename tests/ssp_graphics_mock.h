#ifndef graphics_mock_h
#define graphics_mock_h

#include "ssp_window.h"

#ifdef __cplusplus
extern "C" {
#endif

int ssp_glfw_init_mock_error(ssp_display_platform platform);
int ssp_glfw_init_mock_pass(ssp_display_platform platform);

#ifdef __cplusplus
}
#endif

#endif
