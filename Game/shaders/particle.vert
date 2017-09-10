#version 400

in vec3 position;

uniform mat4 proj_matrix;
uniform mat4 model_view_matrix;

void main(void){

	gl_Position = proj_matrix * model_view_matrix * vec4(position, 1.0);

}