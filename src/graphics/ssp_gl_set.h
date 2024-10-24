#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void ssp_set_gl_fptr_default(void);

typedef int  (*ssp_glad_load_gl_loader_t)(void *load);
typedef void (*ssp_gl_gen_vertex_arrays_t)(int n, unsigned int *arrays);
typedef void (*ssp_gl_bind_vertex_array_t)(unsigned int array);
typedef void (*ssp_gl_gen_buffers_t)(int n, unsigned int *buffers);
typedef void (*ssp_gl_bind_buffer_t)(unsigned int target, unsigned int buffer);
typedef void (*ssp_gl_buffer_data_t)(unsigned int target, signed long size, const void *data, unsigned int usage);
typedef void (*ssp_gl_vertex_attrib_pointer_t)(unsigned int index, int size, unsigned int type, unsigned char normalized, int stride, const void *pointer);
typedef void (*ssp_gl_enable_vertex_attrib_array_t)(unsigned int index);
typedef void (*ssp_gl_active_texture_t)(unsigned int texture);
typedef void (*ssp_gl_bind_texture_t)(unsigned int target, unsigned int texture);
typedef void (*ssp_gl_tex_image2D_t)(unsigned int target, int level, int internalformat, int width, int height, int border, unsigned int format, unsigned int type, const void *pixels);
typedef void (*ssp_gl_generate_mipmap_t)(unsigned int target);
typedef void (*ssp_gl_gen_textures_t)(int n, unsigned int *textures);
typedef void (*ssp_gl_tex_parammetri_t)(unsigned int target, unsigned int pname, int param);
typedef void (*ssp_gl_clear_color_t)(float red, float green, float blue, float alpha);
typedef void (*ssp_gl_clear_t)(unsigned int mask);
typedef void (*ssp_gl_draw_arrays_t)(unsigned int mode, int first, int count);
typedef unsigned int (*ssp_gl_create_shader_t)(unsigned int type);
typedef void (*ssp_gl_shader_source_t)(unsigned int shader, int count, const char *const *string, const int *length);
typedef void (*ssp_gl_compile_shader_t)(unsigned int shader);
typedef void (*ssp_gl_get_shaderiv_t)(unsigned int shader, unsigned int pname, int *params);
typedef void (*ssp_gl_get_shader_info_log_t)(unsigned int shader, int bufSize, int *length, char *infoLog);
typedef void (*ssp_gl_use_program_t)(unsigned int program);
typedef unsigned int (*ssp_gl_create_program_t)(void);
typedef void (*ssp_gl_attach_shader_t)(unsigned int program, unsigned int shader);
typedef void (*ssp_gl_link_program_t)(unsigned int program);
typedef void (*ssp_gl_get_programiv_t)(unsigned int program, unsigned int pname, int *params);
typedef void (*ssp_gl_get_program_info_log_t)(unsigned int program, int bufSize, int *length, char *infoLog);
typedef void (*ssp_gl_delete_shader_t)(unsigned int shader);
typedef void (*ssp_gl_viewport_t)(int x, int y, int width, int height);
typedef const unsigned char* (*ssp_gl_get_string_t)(unsigned int name);

void ssp_glad_set_load_gl_loader(ssp_glad_load_gl_loader_t fptr);
void ssp_gl_set_gen_vertex_arrays(ssp_gl_gen_vertex_arrays_t fptr);
void ssp_gl_set_bind_vertex_array(ssp_gl_bind_vertex_array_t fptr);
void ssp_gl_set_gen_buffers(ssp_gl_gen_buffers_t fptr);
void ssp_gl_set_bind_buffer(ssp_gl_bind_buffer_t fptr);
void ssp_gl_set_buffer_data(ssp_gl_buffer_data_t fptr);
void ssp_gl_set_vertex_attrib_pointer(ssp_gl_vertex_attrib_pointer_t fptr);
void ssp_gl_set_enable_vertex_attrib_array(ssp_gl_enable_vertex_attrib_array_t fptr);
void ssp_gl_set_active_texture(ssp_gl_active_texture_t fptr);
void ssp_gl_set_bind_texture(ssp_gl_bind_texture_t fptr);
void ssp_gl_set_tex_image2D(ssp_gl_tex_image2D_t fptr);
void ssp_gl_set_generate_mipmap(ssp_gl_generate_mipmap_t fptr);
void ssp_gl_set_gen_textures(ssp_gl_gen_textures_t fptr);
void ssp_gl_set_tex_parammetri(ssp_gl_tex_parammetri_t fptr);
void ssp_gl_set_clear_color(ssp_gl_clear_color_t fptr);
void ssp_gl_set_clear(ssp_gl_clear_t fptr);
void ssp_gl_set_draw_arrays(ssp_gl_draw_arrays_t fptr);
void ssp_gl_set_create_shader(ssp_gl_create_shader_t fptr);
void ssp_gl_set_shader_source(ssp_gl_shader_source_t fptr);
void ssp_gl_set_compile_shader(ssp_gl_compile_shader_t fptr);
void ssp_gl_set_get_shaderiv(ssp_gl_get_shaderiv_t fptr);
void ssp_gl_set_get_shader_info_log(ssp_gl_get_shader_info_log_t fptr);
void ssp_gl_set_use_program(ssp_gl_use_program_t fptr);
void ssp_gl_set_create_program(ssp_gl_create_program_t fptr);
void ssp_gl_set_attach_shader(ssp_gl_attach_shader_t fptr);
void ssp_gl_set_link_program(ssp_gl_link_program_t fptr);
void ssp_gl_set_get_programiv(ssp_gl_get_programiv_t fptr);
void ssp_gl_set_get_program_info_log(ssp_gl_get_program_info_log_t fptr);
void ssp_gl_set_delete_shader(ssp_gl_delete_shader_t fptr);
void ssp_gl_set_viewport(ssp_gl_viewport_t fptr);
void ssp_gl_set_get_string(ssp_gl_get_string_t fptr);

#ifdef __cplusplus
}
#endif
