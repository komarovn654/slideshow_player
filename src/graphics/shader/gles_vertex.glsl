#version 100
attribute vec3 position;
attribute vec2 textCoord;

varying vec2 TextCoord;

void main()
{
    gl_Position = vec4(position, 1.0f);
    TextCoord = -textCoord;
}
