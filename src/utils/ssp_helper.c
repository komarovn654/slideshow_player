#include <dirent.h> 
#include <errno.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "logman/logman.h"

#include "ssp_helper.h"

typedef struct ssp_image_header_t {
	unsigned char header[4];
	char *name;
	int header_size;
} ssp_image_header;

static const ssp_image_header headers[] = {
	{ 
		.header = { 0xff, 0xd8 }, 
		.name = "jpeg/jpg",
		.header_size = 2,
	},
};

bool ssp_is_file_image(const char *file_name)
{
	if (!file_name || !*file_name) {
		log_error("Invalid file name");
		return false;
    }
	
	FILE *fp = fopen(file_name, "rb");
	if (!fp) {
		log_error("File opening error <%s>: %s", file_name, strerror(errno));
		return false;
	}

	unsigned char header[4];
	if (fread(header, sizeof(header), 1, fp) != 1) {
		log_error("File reading error <%s>", file_name);
		return false;
	}
	fclose(fp);
	
	for (size_t i = 0; i < sizeof(headers) / sizeof(ssp_image_header); i++) {
		if (memcmp(header, headers[i].header, headers[i].header_size) == 0) {
			log_debug("File <%s> is a <%s> image", file_name, headers[i].name);
			return true;
		}
	}

    log_debug("File <%s> is not an image", file_name);
    return false;
}

int ssp_dir_absolute_path(const char* dir_path, char* abs_path)
{
    if (realpath(dir_path, abs_path) == NULL) {
        return 1;
    }

	return 0;
}

int ssp_dir_traversal(const char* dir_path, void* (*store_files)(void *storage, const char *file_name), void *storage, 
	bool (*filter)(const char *file_name))
{
	if (dir_path == NULL || store_files == NULL) {
		return 1;
	}

	char abs_dir_path[SSP_PATH_MAX_LEN];
	if (ssp_dir_absolute_path(dir_path, abs_dir_path) != 0) {
		return 1;
	}

	DIR *d = opendir(abs_dir_path);
	if (d == NULL) {
		return 1;
	}

	char file_name[SSP_FULL_NAME_MAX_LEN] = { 0 };
  	struct dirent *dir;
  	while ((dir = readdir(d)) != NULL) {
		if ((snprintf(file_name, SSP_FULL_NAME_MAX_LEN, "%s/%s", abs_dir_path, dir->d_name)) >=
			(SSP_FULL_NAME_MAX_LEN)) {
			log_warning("<%s> is too long and was truncated", dir->d_name);
			continue;
		};
		if (filter(file_name)) {
			store_files(storage, file_name);
		}
    }
    closedir(d);

	return 0;
}

int ssp_mkdir(const char* dir_path)
{
#ifdef _WIN32
    return mkdir(dir_path);
#else
    return mkdir(dir_path, 0700);
#endif
}

int ssp_dir_create(const char* dir_path)
{
    struct stat st = {0};
    if (stat(dir_path, &st) == -1) {
        if (ssp_mkdir(dir_path) != 0) {
            log_error("Can't create a directory <%s>", dir_path);
            return 1;
        }
        log_info("Directory <%s> was created", dir_path);
    }

    return 0;
}
