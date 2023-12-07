uniform sampler2D texture;

varying vec2 v_uv;
varying vec4 v_color;

void main()
{
    vec4 colorRGBA = texture2D(texture, v_uv.st);
    float grayscale = (colorRGBA.r + colorRGBA.g + colorRGBA.b)/3;
    gl_FragColor.r = grayscale * v_color.r;
    gl_FragColor.g = grayscale * v_color.g;
    gl_FragColor.b = grayscale * v_color.b;
    gl_FragColor.a = colorRGBA.a * v_color.a;
}
