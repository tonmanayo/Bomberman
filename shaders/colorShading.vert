#version 410

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec2 fragPosition;
out vec4 fragColor;
out vec2 fragUV;

uniform mat4 P;

void main(){
    gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

    fragPosition = vertexPosition;

    fragColor = vertexColor;

    fragUV = vec2(vertexUV.x, 1.0 - vertexUV.y);
}