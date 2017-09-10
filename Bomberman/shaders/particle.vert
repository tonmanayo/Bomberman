#version 400

in vec3 position;

uniform mat4    model_matrix = mat4(1.0);
uniform mat4    view_matrix = mat4(1.0);
uniform mat4    proj_matrix = mat4(1.0);

void main(void){
    gl_Position = proj_matrix * view_matrix *  model_matrix * vec4(position, 1.0);
}