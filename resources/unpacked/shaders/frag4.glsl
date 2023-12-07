uniform sampler2D texture;

varying vec2 v_uv;
varying vec4 v_color;

void main()
{
    vec4 col = v_color * texture2D(texture, v_uv.st);
    gl_FragColor.r = col.r/2 + col.g/4 + col.b/4;
    gl_FragColor.g = col.g/2 + col.r/4 + col.b/4;
    gl_FragColor.b = col.b/2 + col.g/4 + col.r/4;
    gl_FragColor.a = col.a;
}
