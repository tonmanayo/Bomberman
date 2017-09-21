#version 400 core

in  vec3    fposition;
in  vec3    fnormal;
in  vec2    fuv;
in  float   fmatIndex;

struct  Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D   textures[10];
uniform bool        isTexture[10];
uniform vec4        texColor[10];
uniform vec3        viewPos;
uniform bool        dead = false;
uniform vec2        offSet1;
uniform vec2        offSet2;
uniform vec2        rowsBlend;

out vec4 out_color;

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, sampler2D tex,
    vec4 texColor)
{
    vec3 lightDir = normalize(light.position - fragPos);
    lightDir = normalize(-lightDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    // combine results
    vec3 ambient  = light.ambient  * texColor.xyz;
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(tex, fuv));
    vec3 specular = light.specular * spec * vec3(texture(tex, fuv));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight1(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 texColor)
{
    vec3 lightDir = normalize(light.position - fragPos);
    lightDir = normalize(-lightDir);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    // combine results
    float   distToLit = length(light.position - fragPos);
    float att = 1.0 / distToLit;

    vec3 ambient  = light.ambient  * texColor.xyz;
    vec3 diffuse  = light.diffuse  * diff * texColor.xyz;
    vec3 specular = light.specular * spec * texColor.xyz;
    return (ambient * att + diffuse * att + specular * att);
}

void main() {

    Light light;

    light.position = vec3(-0.2f, -1.0f, 3.3f);
    light.ambient = vec3(0.3f, 0.24f, 0.14f);
    light.diffuse = vec3(0.7f, 0.42f, 0.26f);
    light.specular = vec3(0.5f, 0.5f, 0.5f);

    vec4    color;
    if (!dead){
        color = texture(textures[0], fuv);
    }else{
        vec2  textureCoords = fuv;
        textureCoords.y = 1.0 - textureCoords.y;
        textureCoords /= rowsBlend.x;
        vec2    textureCoords1 = textureCoords + offSet1;
        vec2    textureCoords2 = textureCoords + offSet2;
        vec4    color1 = texture(textures[0], textureCoords1);
        vec4    color2 = texture(textures[0], textureCoords2);

        color = mix(color1, color2, rowsBlend.y);
    }

    vec3 viewDir = normalize(viewPos - fposition);
    //color = (light, normalize(fnormal), fposition, viewDir, color);
    //float gamma = 2.0;
    //color.rgb = pow(color.rgb, vec3(1.0 / gamma));
	out_color = color;
}