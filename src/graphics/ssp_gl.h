#if (defined(SSP_GLAD) && SSP_GLAD == 1)
    #include "glad/glad.h"
#else
    #define GLFW_CLIENT_API (0)
    #define GLFW_OPENGL_API (0)
    #define GLFW_CONTEXT_VERSION_MAJOR (0)
    #define GLFW_CONTEXT_VERSION_MINOR (0)
    #define GLFW_OPENGL_FORWARD_COMPAT (0)
    #define GL_TRUE (0)
    #define GLFW_OPENGL_PROFILE (0)
    #define GLFW_OPENGL_CORE_PROFILE (0)
    #define GL_ARRAY_BUFFER (0)
    #define GL_STATIC_DRAW (0)
    #define GL_FLOAT (0)
    #define GL_FALSE (0)
    #define GL_VERTEX_SHADER (0)
    #define GL_FRAGMENT_SHADER (0)
    #define GLfloat float
    #define GLvoid void

    typedef void (*ssp_gl_gen_vertex_array_t)(int, unsigned int*);
    typedef void (*ssp_gl_bind_vertex_array_t)(int);
#endif