#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "ssp_gl.h"

int ssp_glad_load_gl_loader_mock_p(GLADloadproc load);
int ssp_glad_load_gl_loader_mock_f(GLADloadproc load);
void ssp_gl_gen_vertex_arrays_mock_p(GLsizei n, GLuint *arrays);
void ssp_gl_bind_vertex_array_mock_p(GLuint array);
void ssp_gl_gen_buffers_mock_p(GLsizei n, GLuint *buffers);
void ssp_gl_bind_buffer_mock_p(GLenum target, GLuint buffer);
void ssp_gl_buffer_data_mock_p(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
void ssp_gl_vertex_attrib_pointer_mock_p(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
void ssp_gl_enable_vertex_attrib_array_mock_p(GLuint index);
void ssp_gl_active_texture_mock_p(GLenum texture);
void ssp_gl_bind_texture_mock_p(GLenum target, GLuint texture);
void ssp_gl_tex_image2D_mock_p(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
void ssp_gl_generate_mipmap_mock_p(GLenum target);
void ssp_gl_gen_textures_mock_p(GLsizei n, GLuint *textures);
void ssp_gl_tex_parammetri_mock_p(GLenum target, GLenum pname, GLint param);
void ssp_gl_clear_color_mock_p(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void ssp_gl_clear_mock_p(GLbitfield mask);
void ssp_gl_draw_arrays_mock_p(GLenum mode, GLint first, GLsizei count);
GLuint ssp_gl_create_shader_mock_p(GLenum type);
GLuint ssp_gl_create_shader_mock_f(GLenum type);
void ssp_gl_shader_source_mock_p(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length);
void ssp_gl_compile_shader_mock_p(GLuint shader);
void ssp_gl_get_shaderiv_mock_p(GLuint shader, GLenum pname, GLint *params);
void ssp_gl_get_shader_info_log_mock_p(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void ssp_gl_use_program_mock_p(GLuint program);
GLuint ssp_gl_create_program_mock_p(void);
GLuint ssp_gl_create_program_mock_f(void);
void ssp_gl_attach_shader_mock_p(GLuint program, GLuint shader);
void ssp_gl_link_program_mock_p(GLuint program);
void ssp_gl_get_programiv_mock_p(GLuint program, GLenum pname, GLint *params);
void ssp_gl_get_program_info_log_mock_p(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
void ssp_gl_delete_shader_mock_p(GLuint shader);
void ssp_gl_viewport_mock_p(GLint x, GLint y, GLsizei width, GLsizei height);
const GLubyte* ssp_gl_get_string_mock_p(GLenum name);
const GLubyte* ssp_gl_get_string_mock_f(GLenum name);

#ifdef __cplusplus
}
#endif
