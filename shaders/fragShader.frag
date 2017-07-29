#version 410

in vec2 fragPosition;
in vec4 fragColor;
in vec2 fragUV;

out vec4 color;

uniform float time;
uniform sampler2D mySampler;
void main(){

    vec4 textureColor  = texture(mySampler, fragUV);
    //color = textureColor * fragColor;
color = vec4((fragColor.r * cos(fragPosition.x * 4 + time) + 1.0) * 0.5,
              (fragColor.g * cos(fragPosition.y * 4 + time) + 1.0) * 0.5,
              (fragColor.b * cos(fragPosition.x * 2 * 0.5 + time) + 1.0) * 0.5,
              fragColor.a)* textureColor;
}