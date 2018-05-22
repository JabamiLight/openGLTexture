varying highp vec2 v_texcoord;
uniform sampler2D yuvTexSampler;

void main(){
    gl_FragColor=texture2D(yuvTexSampler,v_texcoord);
}