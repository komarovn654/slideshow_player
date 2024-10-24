#include "ssp_gl.h"
#include "ssp_gl_set.h"

typedef struct ssp_gl_t {
    ssp_glad_load_gl_loader_t load_gl_loader;
    ssp_gl_gen_vertex_arrays_t gen_vertex_arrays;
    ssp_gl_bind_vertex_array_t bind_vertex_array;
    ssp_gl_gen_buffers_t gen_buffers;
    ssp_gl_bind_buffer_t bind_buffer;
    ssp_gl_buffer_data_t buffer_data;
    ssp_gl_vertex_attrib_pointer_t vertex_attrib_pointer;
    ssp_gl_enable_vertex_attrib_array_t enable_vertex_attrib_array;
    ssp_gl_active_texture_t active_texture;
    ssp_gl_bind_texture_t bind_texture;
    ssp_gl_tex_image2D_t tex_image2D;
    ssp_gl_generate_mipmap_t generate_mipmap;
    ssp_gl_gen_textures_t gen_textures;
    ssp_gl_tex_parammetri_t tex_parammetri;
    ssp_gl_clear_color_t clear_color;
    ssp_gl_clear_t clear;
    ssp_gl_draw_arrays_t draw_arrays;
    ssp_gl_create_shader_t create_shader;
    ssp_gl_shader_source_t shader_source;
    ssp_gl_compile_shader_t compile_shader;
    ssp_gl_get_shaderiv_t get_shaderiv;
    ssp_gl_get_shader_info_log_t get_shader_info_log;
    ssp_gl_use_program_t use_program;
    ssp_gl_create_program_t create_program;
    ssp_gl_attach_shader_t attach_shader;
    ssp_gl_link_program_t link_program;
    ssp_gl_get_programiv_t get_programiv;
    ssp_gl_get_program_info_log_t get_program_info_log;
    ssp_gl_delete_shader_t delete_shader;
    ssp_gl_viewport_t viewport;
    ssp_gl_get_string_t get_string;
} ssp_gl_t;

static ssp_gl_t ssp_gl = {
    .load_gl_loader = (ssp_glad_load_gl_loader_t)gladLoadGLLoader,
    .gen_vertex_arrays = ssp_gl_gen_vertex_arrays_wrap,
    .bind_vertex_array = ssp_gl_bind_vertex_array_wrap,
    .gen_buffers = ssp_gl_gen_buffers_wrap,
    .bind_buffer = ssp_gl_bind_buffer_wrap,
    .buffer_data = ssp_gl_buffer_data_wrap,
    .vertex_attrib_pointer = ssp_gl_vertex_attrib_pointer_wrap,
    .enable_vertex_attrib_array = ssp_gl_enable_vertex_attrib_array_wrap,
    .active_texture = ssp_gl_active_texture_wrap,
    .bind_texture = ssp_gl_bind_texture_wrap,
    .tex_image2D = ssp_gl_tex_image2D_wrap,
    .generate_mipmap = ssp_gl_generate_mipmap_wrap,
    .gen_textures = ssp_gl_gen_textures_wrap,
    .tex_parammetri = ssp_gl_tex_parammetri_wrap,
    .clear_color = ssp_gl_clear_color_wrap,
    .clear = ssp_gl_clear_wrap,
    .draw_arrays = ssp_gl_draw_arrays_wrap,
    .create_shader = ssp_gl_create_shader_wrap,
    .shader_source = ssp_gl_shader_source_wrap,
    .compile_shader = ssp_gl_compile_shader_wrap,
    .get_shaderiv = ssp_gl_get_shaderiv_wrap,
    .get_shader_info_log = ssp_gl_get_shader_info_log_wrap,
    .use_program = ssp_gl_use_program_wrap,
    .create_program = ssp_gl_create_program_wrap,
    .attach_shader = ssp_gl_attach_shader_wrap,
    .link_program = ssp_gl_link_program_wrap,
    .get_programiv = ssp_gl_get_programiv_wrap,
    .get_program_info_log = ssp_gl_get_program_info_log_wrap,
    .delete_shader = ssp_gl_delete_shader_wrap,
    .viewport = ssp_gl_viewport_wrap,
    .get_string = ssp_gl_get_string_wrap,
};

void ssp_set_gl_fptr_default(void)
{
    ssp_gl.load_gl_loader = (ssp_glad_load_gl_loader_t)gladLoadGLLoader;
    ssp_gl.gen_vertex_arrays = ssp_gl_gen_vertex_arrays_wrap;
    ssp_gl.bind_vertex_array = ssp_gl_bind_vertex_array_wrap;
    ssp_gl.gen_buffers = ssp_gl_gen_buffers_wrap;
    ssp_gl.bind_buffer = ssp_gl_bind_buffer_wrap;
    ssp_gl.buffer_data = ssp_gl_buffer_data_wrap;
    ssp_gl.vertex_attrib_pointer = ssp_gl_vertex_attrib_pointer_wrap;
    ssp_gl.enable_vertex_attrib_array = ssp_gl_enable_vertex_attrib_array_wrap;
    ssp_gl.active_texture = ssp_gl_active_texture_wrap;
    ssp_gl.bind_texture = ssp_gl_bind_texture_wrap;
    ssp_gl.tex_image2D = ssp_gl_tex_image2D_wrap;
    ssp_gl.generate_mipmap = ssp_gl_generate_mipmap_wrap;
    ssp_gl.gen_textures = ssp_gl_gen_textures_wrap;
    ssp_gl.tex_parammetri = ssp_gl_tex_parammetri_wrap;
    ssp_gl.clear_color = ssp_gl_clear_color_wrap;
    ssp_gl.clear = ssp_gl_clear_wrap;
    ssp_gl.draw_arrays = ssp_gl_draw_arrays_wrap;
    ssp_gl.create_shader = ssp_gl_create_shader_wrap;
    ssp_gl.shader_source = ssp_gl_shader_source_wrap;
    ssp_gl.compile_shader = ssp_gl_compile_shader_wrap;
    ssp_gl.get_shaderiv = ssp_gl_get_shaderiv_wrap;
    ssp_gl.get_shader_info_log = ssp_gl_get_shader_info_log_wrap;
    ssp_gl.use_program = ssp_gl_use_program_wrap;
    ssp_gl.create_program = ssp_gl_create_program_wrap;
    ssp_gl.attach_shader = ssp_gl_attach_shader_wrap;
    ssp_gl.link_program = ssp_gl_link_program_wrap;
    ssp_gl.get_programiv = ssp_gl_get_programiv_wrap;
    ssp_gl.get_program_info_log = ssp_gl_get_program_info_log_wrap;
    ssp_gl.delete_shader = ssp_gl_delete_shader_wrap;
    ssp_gl.viewport = ssp_gl_viewport_wrap;
    ssp_gl.get_string = ssp_gl_get_string_wrap;
}

void ssp_gl_gen_vertex_arrays_wrap(GLsizei n, GLuint *arrays) { glGenVertexArrays(n, arrays); }
void ssp_gl_bind_vertex_array_wrap(GLuint array) { glBindVertexArray(array); }
void ssp_gl_gen_buffers_wrap(GLsizei n, GLuint *buffers) { glGenBuffers(n, buffers); }
void ssp_gl_bind_buffer_wrap(GLenum target, GLuint buffer) { glBindBuffer(target, buffer); }
void ssp_gl_buffer_data_wrap(GLenum target, GLsizeiptr size, const void *data, GLenum usage) { glBufferData(target, size, data, usage); }
void ssp_gl_vertex_attrib_pointer_wrap(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
{
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}
void ssp_gl_enable_vertex_attrib_array_wrap(GLuint index) { glEnableVertexAttribArray(index); }
void ssp_gl_active_texture_wrap(GLenum texture) { glActiveTexture(texture); }
void ssp_gl_bind_texture_wrap(GLenum target, GLuint texture) { glBindTexture(target, texture); }
void ssp_gl_tex_image2D_wrap(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
    glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}
void ssp_gl_generate_mipmap_wrap(GLenum target) { glGenerateMipmap(target); }
void ssp_gl_gen_textures_wrap(GLsizei n, GLuint *textures) { glGenTextures(n, textures); }
void ssp_gl_tex_parammetri_wrap(GLenum target, GLenum pname, GLint param) { glTexParameteri(target, pname, param); }
void ssp_gl_clear_color_wrap(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) { glClearColor(red, green, blue, alpha); }
void ssp_gl_clear_wrap(GLbitfield mask) { glClear(mask); }
void ssp_gl_draw_arrays_wrap(GLenum mode, GLint first, GLsizei count) { glDrawArrays(mode, first, count); }
GLuint ssp_gl_create_shader_wrap(GLenum type) { return glCreateShader(type); }
void ssp_gl_shader_source_wrap(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length)
{
    glShaderSource(shader, count, string, length);
}
void ssp_gl_compile_shader_wrap(GLuint shader) { glCompileShader(shader); }
void ssp_gl_get_shaderiv_wrap(GLuint shader, GLenum pname, GLint *params) { glGetShaderiv(shader, pname, params); }
void ssp_gl_get_shader_info_log_wrap(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    glGetShaderInfoLog(shader, bufSize, length, infoLog);
}
void ssp_gl_use_program_wrap(GLuint program) { glUseProgram(program); }
GLuint ssp_gl_create_program_wrap(void) { return glCreateProgram(); }
void ssp_gl_attach_shader_wrap(GLuint program, GLuint shader) { glAttachShader(program, shader); }
void ssp_gl_link_program_wrap(GLuint program) { glLinkProgram(program); }
void ssp_gl_get_programiv_wrap(GLuint program, GLenum pname, GLint *params) { glGetProgramiv(program, pname, params); }
void ssp_gl_get_program_info_log_wrap(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    glGetProgramInfoLog(program, bufSize, length, infoLog);
}
void ssp_gl_delete_shader_wrap(GLuint shader) { glDeleteShader(shader); }
void ssp_gl_viewport_wrap(GLint x, GLint y, GLsizei width, GLsizei height) { glViewport(x, y, width, height); }
const GLubyte* ssp_gl_get_string_wrap(GLenum name) { return glGetString(name); }


int ssp_glad_load_gl_loader(GLADloadproc load) { return ssp_gl.load_gl_loader(load); }
void ssp_gl_gen_vertex_arrays(GLsizei n, GLuint *arrays) { return ssp_gl.gen_vertex_arrays(n, arrays); }
void ssp_gl_bind_vertex_array(GLuint array) { return ssp_gl.bind_vertex_array(array); }
void ssp_gl_gen_buffers(GLsizei n, GLuint *buffers) { return ssp_gl.gen_buffers(n, buffers); }
void ssp_gl_bind_buffer(GLenum target, GLuint buffer) { return ssp_gl.bind_buffer(target, buffer); }
void ssp_gl_buffer_data(GLenum target, GLsizeiptr size, const void *data, GLenum usage) 
{ 
    return ssp_gl.buffer_data(target, size, data, usage);
}
void ssp_gl_vertex_attrib_pointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
{
    return ssp_gl.vertex_attrib_pointer(index, size, type, normalized, stride, pointer);
}
void ssp_gl_enable_vertex_attrib_array(GLuint index) { return ssp_gl.enable_vertex_attrib_array(index); }
void ssp_gl_active_texture(GLenum texture) { return ssp_gl.active_texture(texture); }
void ssp_gl_bind_texture(GLenum target, GLuint texture) { return ssp_gl.bind_texture(target, texture); }
void ssp_gl_tex_image2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels)
{
    return ssp_gl.tex_image2D(target, level, internalformat, width, height, border, format, type, pixels);
}
void ssp_gl_generate_mipmap(GLenum target) { return ssp_gl.generate_mipmap(target); }
void ssp_gl_gen_textures(GLsizei n, GLuint *textures) { return ssp_gl.gen_textures(n ,textures); }
void ssp_gl_tex_parammetri(GLenum target, GLenum pname, GLint param) { return ssp_gl.tex_parammetri(target, pname, param); }
void ssp_gl_clear_color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) { return ssp_gl.clear_color(red, green, blue, alpha); }
void ssp_gl_clear(GLbitfield mask) { return ssp_gl.clear(mask); }
void ssp_gl_draw_arrays(GLenum mode, GLint first, GLsizei count) { return ssp_gl.draw_arrays(mode, first, count); }
GLuint ssp_gl_create_shader(GLenum type) { return ssp_gl.create_shader(type); }
void ssp_gl_shader_source(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length)
{
    return ssp_gl.shader_source(shader, count, string, length);
}
void ssp_gl_compile_shader(GLuint shader) { return ssp_gl.compile_shader(shader); }
void ssp_gl_get_shaderiv(GLuint shader, GLenum pname, GLint *params) { return ssp_gl.get_shaderiv(shader, pname, params); }
void ssp_gl_get_shader_info_log(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    return ssp_gl.get_shader_info_log(shader, bufSize, length, infoLog);
}
void ssp_gl_use_program(GLuint program) { return ssp_gl.use_program(program); }
GLuint ssp_gl_create_program(void) { return ssp_gl.create_program(); }
void ssp_gl_attach_shader(GLuint program, GLuint shader) { return ssp_gl.attach_shader(program, shader); }
void ssp_gl_link_program(GLuint program) { return ssp_gl.link_program(program); }
void ssp_gl_get_programiv(GLuint program, GLenum pname, GLint *params) { return ssp_gl.get_programiv(program, pname, params); }
void ssp_gl_get_program_info_log(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)
{
    return ssp_gl.get_program_info_log(program, bufSize, length, infoLog);
}
void ssp_gl_delete_shader(GLuint shader) { return ssp_gl.delete_shader(shader); }
void ssp_gl_viewport(GLint x, GLint y, GLsizei width, GLsizei height) { return ssp_gl.viewport(x, y, width, height); }
const GLubyte* ssp_gl_get_string(GLenum name) { return ssp_gl.get_string(name); }


void ssp_glad_set_load_gl_loader(ssp_glad_load_gl_loader_t fptr) { ssp_gl.load_gl_loader = fptr; }
void ssp_gl_set_gen_vertex_arrays(ssp_gl_gen_vertex_arrays_t fptr) { ssp_gl.gen_vertex_arrays = fptr; }
void ssp_gl_set_bind_vertex_array(ssp_gl_bind_vertex_array_t fptr) { ssp_gl.bind_vertex_array = fptr; }
void ssp_gl_set_gen_buffers(ssp_gl_gen_buffers_t fptr) { ssp_gl.gen_buffers = fptr; }
void ssp_gl_set_bind_buffer(ssp_gl_bind_buffer_t fptr) { ssp_gl.bind_buffer = fptr; }
void ssp_gl_set_buffer_data(ssp_gl_buffer_data_t fptr) { ssp_gl.buffer_data = fptr; }
void ssp_gl_set_vertex_attrib_pointer(ssp_gl_vertex_attrib_pointer_t fptr) { ssp_gl.vertex_attrib_pointer = fptr; }
void ssp_gl_set_enable_vertex_attrib_array(ssp_gl_enable_vertex_attrib_array_t fptr) { ssp_gl.enable_vertex_attrib_array = fptr; }
void ssp_gl_set_active_texture(ssp_gl_active_texture_t fptr) { ssp_gl.active_texture = fptr; }
void ssp_gl_set_bind_texture(ssp_gl_bind_texture_t fptr) { ssp_gl.bind_texture = fptr; }
void ssp_gl_set_tex_image2D(ssp_gl_tex_image2D_t fptr) { ssp_gl.tex_image2D = fptr; }
void ssp_gl_set_generate_mipmap(ssp_gl_generate_mipmap_t fptr) { ssp_gl.generate_mipmap = fptr; }
void ssp_gl_set_gen_textures(ssp_gl_gen_textures_t fptr) { ssp_gl.gen_textures = fptr; }
void ssp_gl_set_tex_parammetri(ssp_gl_tex_parammetri_t fptr) { ssp_gl.tex_parammetri = fptr; }
void ssp_gl_set_clear_color(ssp_gl_clear_color_t fptr) { ssp_gl.clear_color = fptr; }
void ssp_gl_set_clear(ssp_gl_clear_t fptr) { ssp_gl.clear = fptr; }
void ssp_gl_set_draw_arrays(ssp_gl_draw_arrays_t fptr) { ssp_gl.draw_arrays = fptr; }
void ssp_gl_set_create_shader(ssp_gl_create_shader_t fptr) { ssp_gl.create_shader = fptr; }
void ssp_gl_set_shader_source(ssp_gl_shader_source_t fptr) { ssp_gl.shader_source = fptr; }
void ssp_gl_set_compile_shader(ssp_gl_compile_shader_t fptr) { ssp_gl.compile_shader = fptr; }
void ssp_gl_set_get_shaderiv(ssp_gl_get_shaderiv_t fptr) { ssp_gl.get_shaderiv = fptr; }
void ssp_gl_set_get_shader_info_log(ssp_gl_get_shader_info_log_t fptr) { ssp_gl.get_shader_info_log = fptr; }
void ssp_gl_set_use_program(ssp_gl_use_program_t fptr) { ssp_gl.use_program = fptr; }
void ssp_gl_set_create_program(ssp_gl_create_program_t fptr) { ssp_gl.create_program = fptr; }
void ssp_gl_set_attach_shader(ssp_gl_attach_shader_t fptr) { ssp_gl.attach_shader = fptr; }
void ssp_gl_set_link_program(ssp_gl_link_program_t fptr) { ssp_gl.link_program = fptr; }
void ssp_gl_set_get_programiv(ssp_gl_get_programiv_t fptr) { ssp_gl.get_programiv = fptr; }
void ssp_gl_set_get_program_info_log(ssp_gl_get_program_info_log_t fptr) { ssp_gl.get_program_info_log = fptr; }
void ssp_gl_set_delete_shader(ssp_gl_delete_shader_t fptr) { ssp_gl.delete_shader = fptr; }
void ssp_gl_set_viewport(ssp_gl_viewport_t fptr) { ssp_gl.viewport = fptr; }
void ssp_gl_set_get_string(ssp_gl_get_string_t fptr) { ssp_gl.get_string = fptr; }
