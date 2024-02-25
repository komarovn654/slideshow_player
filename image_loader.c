#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "image_loader.h"
#include <jpeglib.h>

struct my_error_mgr {
  struct jpeg_error_mgr pub;

  jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr * my_error_ptr;

static void my_error_exit (j_common_ptr cinfo)
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  (*cinfo->err->output_message) (cinfo);
  longjmp(myerr->setjmp_buffer, 1);
}

static int jpeg_setup_error_callback(struct jpeg_decompress_struct* cinfo, struct jpeg_error_mgr* jerr)
{
    jmp_buf env;
    cinfo->err = jpeg_std_error(jerr);
    jerr->error_exit = my_error_exit;
    cinfo->client_data = env;
    if (setjmp(env)) {
        return 1;
    }
    
    return 0;
}

int read_jpeg(const char* path, unsigned char** buf, int buf_size)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        printf("Can't open file: %s\n", path);
        return 1;
    }
    
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;
    JSAMPARRAY buffer;        /* Output row buffer */
    int row_stride;           /* physical row width in output buffer */
    int j = 0;
    
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    if (setjmp(jerr.setjmp_buffer)) {
      jpeg_destroy_decompress(&cinfo);
      fclose(fp);
      return 1;
    }

    jpeg_create_decompress(&cinfo);
    
    jpeg_stdio_src(&cinfo, fp);
    
    jpeg_read_header(&cinfo, TRUE);

    jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    if (buffer == NULL) {
        printf("Couldn't allocate image data\n");
        return 1;
    }

    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        
        for (int i = 0; i < row_stride; i++) {
            (*buf)[j++] = buffer[0][i];
        }
    }

    printf("%d\n", j);
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    
    return 0;
}
