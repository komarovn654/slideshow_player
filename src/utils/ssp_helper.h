// #pragma "once"
#ifndef helper_h
#define helper_h

#ifdef __cplusplus
extern "C" {
#endif

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <syslog.h>

#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
    #define _WIN32
#endif /* _WIN32 */

#if (defined(UTEST_BUILD) && UTEST_BUILD == 1)
    #define ssp_static
#else
    #define ssp_static static
#endif

#if (defined(EXC_PATH))
    #define SSP_EXC_PATH EXC_PATH
#else
    #define SSP_EXC_PATH "undef"
#endif

#define SSP_PATH_MAX_LEN      (PATH_MAX)
#define SSP_FILE_NAME_MAX_LEN (32)
#define SSP_FULL_NAME_MAX_LEN (SSP_PATH_MAX_LEN + SSP_FILE_NAME_MAX_LEN)

typedef struct ssp_image_storage_t {
    void* storage;

    char* (*image_name)(void* storage);
    void* (*move_to_next)(void* storage);
    void* (*insert)(void*  storage, const char* item_name);
    void  (*remove)(void** storage, const char* item_name);
} ssp_image_storage;

bool ssp_is_file_image(const char *file_name);

int ssp_dir_traversal(const char* dir_path, void* (*store_files)(void *storage, const char *file_name), void *storage, 
	bool (*filter)(const char *file_name));
int ssp_dir_create(const char* dir_path);
int ssp_dir_absolute_path(const char* dir_path, char* abs_path);
int ssp_mkdir(const char* dir_path);

void ssp_syslog(int type, const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif /* helper_h */
