uniform sampler2D texture;
uniform mat4 color_matrix;
uniform vec4 color_offset;

varying vec2 v_uv;
varying vec4 v_color;

void main()
{
    vec4 col = texture2D(texture, v_uv.st);
    gl_FragColor = color_matrix * col + color_offset * col;
}
