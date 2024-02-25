#include <assert.h>
#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <logger.h>
#include <string.h>

#include "shader.h"
#include "image_loader.h"

GLfloat vertices[] = {
    -1.0f,  1.0f, 0.0f,     0.0f, 1.0f, 
    -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,     1.0f, 0.0f,

     1.0f, -1.0f, 0.0f,     1.0f, 0.0f, 
     1.0f,  1.0f, 0.0f,     1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f,     0.0f, 1.0f,
};  

static shader_meta shaders[2] = {
    {
        .type = GL_VERTEX_SHADER,
        .path = "vertex.glsl",
    },
    {
        .type = GL_FRAGMENT_SHADER,
        .path = "fragment.glsl",
    }
};

static struct {
    GLuint vao_id; // vertex array object
    GLuint ebo_id; // element buffer object
    GLuint vbo_id; // vertex buffer object
} buffers;

static struct t_egl_context {
    struct wl_egl_window *egl_window;
    EGLDisplay egl_display;
    EGLConfig egl_conf;
    EGLSurface egl_surface;
    EGLContext egl_context;

    GLuint texture;
    GLuint texture2;
} egl_context;

static void setup_buffers(void)
{
    glGenBuffers(1, &buffers.vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ssp_init_egl(struct wl_display *src_display, struct wl_surface *src_surface) {
    egl_context.egl_window = wl_egl_window_create(src_surface, 960, 1280);
    if (egl_context.egl_window == EGL_NO_SURFACE) {
        log_error("Can't create egl window\n");
    }
    log_debug("egl window created");

    EGLint major, minor, count, n, size;
    EGLConfig *configs;
    int i;
    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    static const EGLint context_attribs[] = {
	    EGL_CONTEXT_CLIENT_VERSION, 2,
	    EGL_NONE
    };

    
    egl_context.egl_display = eglGetDisplay((EGLNativeDisplayType) src_display);
    if (egl_context.egl_display == EGL_NO_DISPLAY) {
	    log_error("Can't create egl display\n");
    }
    log_info("init egl display");

    if (eglInitialize(egl_context.egl_display, &major, &minor) != EGL_TRUE) {
        log_error("egl init error");
    }
    printf("EGL major: %d, minor %d\n", major, minor);

    eglGetConfigs(egl_context.egl_display, NULL, 0, &count);
    printf("EGL has %d configs\n", count);

    configs = calloc(count, sizeof *configs);
    
    eglChooseConfig(egl_context.egl_display, config_attribs, configs, count, &n);
    
    for (i = 0; i < n; i++) {
	    eglGetConfigAttrib(egl_context.egl_display, configs[i], EGL_BUFFER_SIZE, &size);
	    printf("Buffer size for config %d is %d\n", i, size);
	    eglGetConfigAttrib(egl_context.egl_display, configs[i], EGL_RED_SIZE, &size);
	    printf("Red size for config %d is %d\n", i, size);
	
	    // TODO: just choose the first one
	    egl_context.egl_conf = configs[i];
	    break;
    }

    egl_context.egl_context = eglCreateContext(egl_context.egl_display, egl_context.egl_conf, 
        EGL_NO_CONTEXT, context_attribs);
    egl_context.egl_surface = eglCreateWindowSurface(egl_context.egl_display, configs[0], 
        egl_context.egl_window, NULL);

    EGLBoolean makeCurrent = eglMakeCurrent(egl_context.egl_display, egl_context.egl_surface, 
        egl_context.egl_surface, egl_context.egl_context);
}

void ssp_create_window(struct wl_surface *src_surface) 
{
    setup_buffers();
    if (shader_create_program(shaders, 2) == 0) {
        log_error("Shader error");
    }

    glGenTextures(1, &egl_context.texture);
    glBindTexture(GL_TEXTURE_2D, egl_context.texture);   

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // NOTE the GL_NEAREST Here!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // NOTE the GL_NEAREST Here!    

    unsigned char* buf = (unsigned char*)malloc(720*1280*3);
    read_jpeg("photo_2021-08-13_16-05-04.jpg", &buf, 0);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 720, 1280, 0, GL_RGB, GL_UNSIGNED_BYTE, buf);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);



    glGenTextures(1, &egl_context.texture2);
    glBindTexture(GL_TEXTURE_2D, egl_context.texture2);   

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // NOTE the GL_NEAREST Here!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);  // NOTE the GL_NEAREST Here!    

    unsigned char* buf2 = (unsigned char*)malloc(960*1280*3);
    read_jpeg("photo_2023-08-20_15-13-23.jpg", &buf2, 0);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 960, 1280, 0, GL_RGB, GL_UNSIGNED_BYTE, buf2);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);       
}

void draw_1()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //glFlush();


    //glUniform1i(glGetUniformLocation(shader_get_program(), "ourTexture1"), 0);    

    shader_use_program();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, egl_context.texture);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (eglSwapBuffers(egl_context.egl_display, egl_context.egl_surface)) {
	    fprintf(stderr, "Swapped buffers\n");
    } else {
	    fprintf(stderr, "Swapped buffers failed\n");
    }
}

void draw_2()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //glFlush();


    //glUniform1i(glGetUniformLocation(shader_get_program(), "ourTexture1"), 0);    

    shader_use_program();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, egl_context.texture2);

    glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (eglSwapBuffers(egl_context.egl_display, egl_context.egl_surface)) {
	    fprintf(stderr, "Swapped buffers\n");
    } else {
	    fprintf(stderr, "Swapped buffers failed\n");
    }
}