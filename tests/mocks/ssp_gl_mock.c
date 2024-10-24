#include <stdlib.h>

#include "ssp_gl_mock.h"

int ssp_glad_load_gl_loader_mock_p(GLADloadproc load) {
    return 1;
}

int ssp_glad_load_gl_loader_mock_f(GLADloadproc load) {
    return 0;
}

void ssp_gl_gen_vertex_arrays_mock_p(GLsizei n, GLuint *arrays) {
    *arrays = 1;
    return;
}

void ssp_gl_bind_vertex_array_mock_p(GLuint array) {
    return;
}

void ssp_gl_gen_buffers_mock_p(GLsizei n, GLuint *buffers) {
    *buffers = 1;
    return;
}

void ssp_gl_bind_buffer_mock_p(GLenum target, GLuint buffer) {
    return;
}

void ssp_gl_buffer_data_mock_p(GLenum target, GLsizeiptr size, const void *data, GLenum usage) {
    return;
}

void ssp_gl_vertex_attrib_pointer_mock_p(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) {
    return;
}

void ssp_gl_enable_vertex_attrib_array_mock_p(GLuint index) {
    return;
}

void ssp_gl_active_texture_mock_p(GLenum texture) {
    return;
}

void ssp_gl_bind_texture_mock_p(GLenum target, GLuint texture) {
    return;
}

void ssp_gl_tex_image2D_mock_p(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) {
    return;
}

void ssp_gl_generate_mipmap_mock_p(GLenum target) {
    return;
}

void ssp_gl_gen_textures_mock_p(GLsizei n, GLuint *textures) {
    *textures = 1;
    return;
}

void ssp_gl_tex_parammetri_mock_p(GLenum target, GLenum pname, GLint param) {
    return;
}

void ssp_gl_clear_color_mock_p(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    return;
}

void ssp_gl_clear_mock_p(GLbitfield mask) {
    return;
}

void ssp_gl_draw_arrays_mock_p(GLenum mode, GLint first, GLsizei count) {
    return;
}

GLuint ssp_gl_create_shader_mock_p(GLenum type) {
    return 1;
}

GLuint ssp_gl_create_shader_mock_f(GLenum type) {
    return 0;
}

void ssp_gl_shader_source_mock_p(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length) {
    return;
}

void ssp_gl_compile_shader_mock_p(GLuint shader) {
    return;
}

void ssp_gl_get_shaderiv_mock_p(GLuint shader, GLenum pname, GLint *params) {
    return;
}

void ssp_gl_get_shader_info_log_mock_p(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
    return;
}

void ssp_gl_use_program_mock_p(GLuint program) {
    return;
}

GLuint ssp_gl_create_program_mock_p(void) {
    return 1;
}

GLuint ssp_gl_create_program_mock_f(void) {
    return 0;
}

void ssp_gl_attach_shader_mock_p(GLuint program, GLuint shader) {
    return;
}

void ssp_gl_link_program_mock_p(GLuint program) {
    return;
}

void ssp_gl_get_programiv_mock_p(GLuint program, GLenum pname, GLint *params) {
    return;
}

void ssp_gl_get_program_info_log_mock_p(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) {
    return;
}

void ssp_gl_delete_shader_mock_p(GLuint shader) {
    return;
}

void ssp_gl_viewport_mock_p(GLint x, GLint y, GLsizei width, GLsizei height) {
    return;
}

const GLubyte* ssp_gl_get_string_mock_p(GLenum name) {
    return (void*)1;
}

const GLubyte* ssp_gl_get_string_mock_f(GLenum name) {
    return NULL;
}

