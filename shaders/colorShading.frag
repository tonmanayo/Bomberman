#version 410

in vec2 fragPosition;
in vec4 fragColor;
in vec2 fragUV;

out vec4 color;

uniform sampler2D mySampler;
void main(){

    vec4 textureColor  = texture(mySampler, fragUV);
    color = textureColor * fragColor;
color = vec4(fragColor.r * textureColor);
}