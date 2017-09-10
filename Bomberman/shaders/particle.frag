#version 400

out vec4 out_colour;

in  vec3    fposition;
in  vec2    textureCoords1;
in  vec2    textureCoords2;
in  float   blend;

struct  Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3        viewPos;
uniform sampler2D   particleTexture;

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

void main(void){
    Light light;

    light.position = vec3(-0.2f, -1.0f, 3.3f);
    light.ambient = vec3(0.3f, 0.24f, 0.14f);
    light.diffuse = vec3(0.7f, 0.42f, 0.26f);
    light.specular = vec3(0.5f, 0.5f, 0.5f);

    vec4    color1 = texture(particleTexture, textureCoords1);
    vec4    color2 = texture(particleTexture, textureCoords2);

	vec4    colour = mix(color1, color2, blend);
	vec3 viewDir = normalize(viewPos - fposition);
	//colour = vec4(CalcPointLight1(light, normalize(vec3(1, 1, 1)), fposition, viewDir, colour), 1.0);
    //float gamma = 2.0;
    //colour.rgb = pow(colour.rgb, vec3(1.0 / gamma));
    //colour.g -= 0.4;
    //colour.b -= 0.2;
    out_colour = colour;
}