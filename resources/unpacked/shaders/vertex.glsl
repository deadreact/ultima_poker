attribute vec3 a_position;
attribute vec2 a_texcoord;

uniform vec4 u_color;
uniform mat4 mvp_matrix;

varying vec2 v_uv;
varying vec4 v_color;

void main(void)
{
    gl_Position = mvp_matrix * vec4(a_position, 1.0);
    v_uv = a_texcoord;
    v_color = u_color;
}
