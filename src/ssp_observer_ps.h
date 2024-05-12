#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int ssp_obsps_init(observer settings);
void ssp_obsps_destruct(void);
int ssp_obsps_process(void);

#ifdef __cplusplus
}
#endif
