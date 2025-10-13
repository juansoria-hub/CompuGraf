#version 330 core
struct Material {
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;

// === CAMBIO: ahora usamos varias luces ===
uniform int   numLights;        // usa 2
uniform Light lights[2];

uniform sampler2D texture_diffuse;

vec3 shade(Light light, vec3 norm, vec3 viewDir)
{
    // Ambient (mantengo tu elección de usar material.diffuse)
    vec3 ambient = light.ambient * material.diffuse;

    // Diffuse
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 lighting = vec3(0.0);
    for (int i = 0; i < numLights; ++i) {
        lighting += shade(lights[i], norm, viewDir);
    }

    color = vec4(lighting, 1.0) * texture(texture_diffuse, TexCoords);
}


