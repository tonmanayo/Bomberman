#version 400 core

in  vec3    position;
in  vec3    normal;
in  vec2    uv;
in  float   matIndex;
in  int     node;

out vec3    fposition;
out vec3    fnormal;
out vec2    fuv;
out float   fmatIndex;

uniform mat4    model_matrix = mat4(1.0);
uniform mat4    view_matrix = mat4(1.0);
uniform mat4    proj_matrix = mat4(1.0);

uniform bool    hasAnime = false;
uniform mat4    animeMat;
uniform vec2    weights;

void main() {
    fposition = position;
    fnormal = normal;
    fuv = uv;
    fmatIndex = matIndex;

    if (hasAnime)
    {
        mat4 mvp = proj_matrix * view_matrix *  model_matrix;

        vec3    newPosition = weights.y * position + position;
        gl_Position = mvp * animeMat * vec4(newPosition, 1.0);
    }
    else
        gl_Position = proj_matrix * view_matrix *  model_matrix * vec4(position, 1.0);
}