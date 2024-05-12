#pragma "once"

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(UTEST_BUILD) && UTEST_BUILD == 1)
    #define ssp_static
#else
    #define ssp_static static
#endif

#define SSP_FILE_NAME_MAX_LEN (200)

bool ssp_is_file_image(const char *file_name);

int ssp_dir_traversal(const char* dir_path, void (*store_files)(void *storage, const char *file_name), void *storage);
int ssp_dir_create(const char* dir_path);

#ifdef __cplusplus
}
#endif