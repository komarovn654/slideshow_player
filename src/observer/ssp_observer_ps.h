#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ssp_observer.h"

int ssp_obsps_init(ssp_observer settings);
void ssp_obsps_destruct(void);
int ssp_obsps_process(void);

#ifdef __cplusplus
}
#endif
