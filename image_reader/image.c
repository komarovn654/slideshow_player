#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <jpeglib.h>
#include <setjmp.h>
#include "pixman-1/pixman.h"

static void swizzle_row(JSAMPLE *row, JDIMENSION width)
{
	JSAMPLE *s;
	uint32_t *d;

	s = row + (width - 1) * 3;
	d = (uint32_t *) (row + (width - 1) * 4);
	while (s >= row) {
		*d = 0xff000000 | (s[0] << 16) | (s[1] << 8) | (s[2] << 0);
		s -= 3;
		d--;
	}
}

struct my_error_mgr {
  struct jpeg_error_mgr pub;

  jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

static void pixman_image_destroy_func(pixman_image_t *image, void *data)
{
	free(data);
}

static void my_error_exit (j_common_ptr cinfo)
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  (*cinfo->err->output_message) (cinfo);
  longjmp(myerr->setjmp_buffer, 1);
}

static pixman_image_t *load_jpeg(FILE *fp)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	pixman_image_t *pixman_image = NULL;
	unsigned int i;
	int stride, first;
	JSAMPLE *data, *rows[4];
	jmp_buf env;

	cinfo.err = jpeg_std_error(&jerr);
	jerr.error_exit = my_error_exit;
	cinfo.client_data = env;
	if (setjmp(env))
		return NULL;

	jpeg_create_decompress(&cinfo);

	jpeg_stdio_src(&cinfo, fp);

	jpeg_read_header(&cinfo, TRUE);

	cinfo.out_color_space = JCS_RGB;
	jpeg_start_decompress(&cinfo);

	stride = cinfo.output_width * 4;
	data = malloc(stride * cinfo.output_height);
	if (data == NULL) {
		fprintf(stderr, "couldn't allocate image data\n");
		return NULL;
	}

	while (cinfo.output_scanline < cinfo.output_height) {
		first = cinfo.output_scanline;
		for (i = 0; i < 4; i++)
			rows[i] = data + (first + i) * stride;

		jpeg_read_scanlines(&cinfo, rows, 4);
		for (i = 0; first + i < cinfo.output_scanline; i++)
			swizzle_row(rows[i], cinfo.output_width);
	}

	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);

	pixman_image = pixman_image_create_bits(PIXMAN_a8r8g8b8,
					cinfo.output_width,
					cinfo.output_height,
					(uint32_t *) data, stride);

	pixman_image_set_destroy_function(pixman_image,
				pixman_image_destroy_func, data);

	return pixman_image;
}

typedef struct image_loader {
	unsigned char header[4];
	char *name;
	int header_size;
	pixman_image_t *(*load)(FILE *fp);
} image_loader;

static const image_loader loaders[] = {
	{ 
		.header = { 0xff, 0xd8 }, 
		.name = "jpeg/jpg",
		.header_size = 2,
		.load = load_jpeg,
	},
};

pixman_image_t *load_image(const char *filename)
{
	if (!filename || !*filename)
		return NULL;
	
	FILE *fp = fopen(filename, "rb");
	if (!fp) {
		fprintf(stderr, "%s: %s\n", filename, strerror(errno));
		return NULL;
	}

	unsigned char header[4];
	if (fread(header, sizeof header, 1, fp) != 1) {
		fprintf(stderr, "%s: unable to read file header\n", filename);
		goto err_close;
	}
	rewind(fp);

	pixman_image_t *image;
	for (uint32_t i = 0; i < sizeof(loaders) / sizeof(image_loader); i++) {
		if (memcmp(header, loaders[i].header, loaders[i].header_size) == 0) {
			image = loaders[i].load(fp);
			fclose(fp);
			return image;
		}
	}
	fprintf(stderr, "%s: unsupported image type\nSupported only:", filename);
	for (uint32_t i = 0; i < sizeof(loaders) / sizeof(image_loader); i++) {
		fprintf(stderr, " %s;", loaders[i].name);
	}
	fprintf(stderr, "\n");
	
	err_close:
		fclose(fp);
		return NULL;
}
