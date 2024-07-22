#include "ssp_window.h"
#include "ssp_graphics_mock.h"

int ssp_glfw_init_mock_error(ssp_display_platform platform) { return 1; }
int ssp_glfw_init_mock_pass(ssp_display_platform platform) { return 0; }

void ssp_glfw_set_time_mock(double time) { return; }