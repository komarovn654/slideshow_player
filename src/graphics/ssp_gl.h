#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "glad/glad.h"

void ssp_gl_gen_vertex_arrays_wrap(GLsizei n, GLuint *arrays);
void ssp_gl_bind_vertex_array_wrap(GLuint array);
void ssp_gl_gen_buffers_wrap(GLsizei n, GLuint *buffers);
void ssp_gl_bind_buffer_wrap(GLenum target, GLuint buffer);
void ssp_gl_buffer_data_wrap(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
void ssp_gl_vertex_attrib_pointer_wrap(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
void ssp_gl_enable_vertex_attrib_array_wrap(GLuint index);
void ssp_gl_active_texture_wrap(GLenum texture);
void ssp_gl_bind_texture_wrap(GLenum target, GLuint texture);
void ssp_gl_tex_image2D_wrap(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
void ssp_gl_generate_mipmap_wrap(GLenum target);
void ssp_gl_gen_textures_wrap(GLsizei n, GLuint *textures);
void ssp_gl_tex_parammetri_wrap(GLenum target, GLenum pname, GLint param);
void ssp_gl_clear_color_wrap(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void ssp_gl_clear_wrap(GLbitfield mask);
void ssp_gl_draw_arrays_wrap(GLenum mode, GLint first, GLsizei count);
GLuint ssp_gl_create_shader_wrap(GLenum type);
void ssp_gl_shader_source_wrap(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length);
void ssp_gl_compile_shader_wrap(GLuint shader);
void ssp_gl_get_shaderiv_wrap(GLuint shader, GLenum pname, GLint *params);
void ssp_gl_get_shader_info_log_wrap(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void ssp_gl_use_program_wrap(GLuint program);
GLuint ssp_gl_create_program_wrap(void);
void ssp_gl_attach_shader_wrap(GLuint program, GLuint shader);
void ssp_gl_link_program_wrap(GLuint program);
void ssp_gl_get_programiv_wrap(GLuint program, GLenum pname, GLint *params);
void ssp_gl_get_program_info_log_wrap(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void ssp_gl_delete_shader_wrap(GLuint shader);
void ssp_gl_viewport_wrap(GLint x, GLint y, GLsizei width, GLsizei height);
const GLubyte* ssp_gl_get_string_wrap(GLenum name);

int ssp_glad_load_gl_loader(GLADloadproc load);
void ssp_gl_gen_vertex_arrays(GLsizei n, GLuint *arrays);
void ssp_gl_bind_vertex_array(GLuint array);
void ssp_gl_gen_buffers(GLsizei n, GLuint *buffers);
void ssp_gl_bind_buffer(GLenum target, GLuint buffer);
void ssp_gl_buffer_data(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
void ssp_gl_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
void ssp_gl_enable_vertex_attrib_array(GLuint index);
void ssp_gl_active_texture(GLenum texture);
void ssp_gl_bind_texture(GLenum target, GLuint texture);
void ssp_gl_tex_image2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
void ssp_gl_generate_mipmap(GLenum target);
void ssp_gl_gen_textures(GLsizei n, GLuint *textures);
void ssp_gl_tex_parammetri(GLenum target, GLenum pname, GLint param);
void ssp_gl_clear_color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void ssp_gl_clear(GLbitfield mask);
void ssp_gl_draw_arrays(GLenum mode, GLint first, GLsizei count);
GLuint ssp_gl_create_shader(GLenum type);
void ssp_gl_shader_source(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length);
void ssp_gl_compile_shader(GLuint shader);
void ssp_gl_get_shaderiv(GLuint shader, GLenum pname, GLint *params);
void ssp_gl_get_shader_info_log(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void ssp_gl_use_program(GLuint program);
GLuint ssp_gl_create_program(void);
void ssp_gl_attach_shader(GLuint program, GLuint shader);
void ssp_gl_link_program(GLuint program);
void ssp_gl_get_programiv(GLuint program, GLenum pname, GLint *params);
void ssp_gl_get_program_info_log(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void ssp_gl_delete_shader(GLuint shader);
void ssp_gl_viewport(GLint x, GLint y, GLsizei width, GLsizei height);
const GLubyte* ssp_gl_get_string(GLenum name);

#ifdef __cplusplus
}
#endif
