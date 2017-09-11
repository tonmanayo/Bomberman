#version 400

layout (location = 0) in  vec3    position;
layout (location = 1) in  mat4    model_matrix;
layout (location = 5) in  vec4    texOffsets;
layout (location = 6) in  float   blendFactor;

uniform mat4    view_matrix;
uniform mat4    proj_matrix;
uniform float   numberOfRows;

out vec2    textureCoords1;
out vec2    textureCoords2;
out float   blend;
out vec3    fposition;

void main(void){

    vec2 textureCoords = position.xy + vec2(0.5, 0.5);
    textureCoords.y = 1.0 - textureCoords.y;
    textureCoords /= numberOfRows;
    textureCoords1 = textureCoords + texOffsets.xy;
    textureCoords2 = textureCoords + texOffsets.zw;
    blend = blendFactor;
    fposition = position;

    gl_Position = proj_matrix * view_matrix *  model_matrix * vec4(position.xy, 0.0, 1.0);
}