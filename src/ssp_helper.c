#include <errno.h>
#include <logman/logman.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef struct image {
	unsigned char header[4];
	char *name;
	int header_size;
} image;

static const image loaders[] = {
	{ 
		.header = { 0xff, 0xd8 }, 
		.name = "jpeg/jpg",
		.header_size = 2,
	},
};

bool ssp_is_file_image(const char *file_name)
{
	if (!file_name || !*file_name) {
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

	for (uint32_t i = 0; i < sizeof(loaders) / sizeof(image); i++) {
		if (memcmp(header, loaders[i].header, loaders[i].header_size) == 0) {
			log_debug("File <%s> is a <%s> image", file_name, loaders[i].name);
			return true;
		}
	}

    log_debug("File <%s> has been ignored", file_name);
    return false;
}