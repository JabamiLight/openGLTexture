varying highp vec2 v_texcoord;
uniform sampler2D yuvTexSampler;

void main(){
    vec4 color=texture2D(yuvTexSampler,v_texcoord);
    float rgb = color.g;
    vec4 c =vec4(rgb,rgb,rgb,color.a);
    gl_FragColor=c;

}