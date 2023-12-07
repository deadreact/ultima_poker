uniform sampler2D texture;

varying vec2 v_uv;
varying vec4 v_color;

void main()
{
    vec4 colorRGBA = texture2D(texture, v_uv.st);
    gl_FragColor.r = colorRGBA.b * v_color.r;
    gl_FragColor.g = colorRGBA.r * v_color.g;
    gl_FragColor.b = colorRGBA.g * v_color.b;
    gl_FragColor.a = colorRGBA.a * v_color.a;
}
