#version 400

in  vec3    position;

uniform mat4    model_matrix = mat4(1.0);
uniform mat4    view_matrix = mat4(1.0);
uniform mat4    proj_matrix = mat4(1.0);

out vec2    textureCoords1;
out vec2    textureCoords2;
out float   blend;
out vec3    fposition;

uniform vec2    texOffset1;
uniform vec2    texOffset2;
uniform vec2    texCoordInfo;

void main(void){

    vec2 textureCoords = position.xy + vec2(0.5, 0.5);
    textureCoords.y = 1.0 - textureCoords.y;
    textureCoords /= texCoordInfo.x;
    textureCoords1 = textureCoords + texOffset1;
    textureCoords2 = textureCoords + texOffset2;
    blend = texCoordInfo.y;
    fposition = position;

    gl_Position = proj_matrix * view_matrix *  model_matrix * vec4(position.xy, 0.0, 1.0);
}