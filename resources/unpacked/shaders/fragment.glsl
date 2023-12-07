uniform sampler2D texture;

varying vec2 v_uv;
varying vec4 v_color;

void main()
{
    gl_FragColor = texture2D(texture, v_uv.st);
}
