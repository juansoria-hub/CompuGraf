#version 330 core

// Aumentado el número de luces puntuales soportadas
#define NUMBER_OF_POINT_LIGHTS 8

// Structs (sin cambios)
struct Material
{
    sampler2D diffuse;
    sampler2D specular; 
    float shininess;
};

struct DirLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform int transparency;

// Function prototypes
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir );
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir );
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir );

void main( )
{
    // Properties
    vec3 norm = normalize( Normal );
    vec3 viewDir = normalize( viewPos - FragPos );
    
    // --- PASO 1: Calcular la contribución total de todas las fuentes de luz ---
    // Se calcula la luz de forma aditiva, sin considerar aún el color del objeto.
    vec3 totalLighting = vec3(0.0, 0.0, 0.0);
    
    // Iluminación Direccional
    totalLighting += CalcDirLight( dirLight, norm, viewDir );
    
    // Point Lights (Vela y Farol, y las demás que se configuren)
    for ( int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++ )
    {
        totalLighting += CalcPointLight( pointLights[i], norm, FragPos, viewDir );
    }
    
    // Spot Light (Foco)
    totalLighting += CalcSpotLight( spotLight, norm, FragPos, viewDir );
    
    // --- PASO 2: Aplicar la iluminación al color de la textura del objeto ---
    vec4 texel = texture(material.diffuse, TexCoords);
    vec3 finalColor = totalLighting * texel.rgb;
    
    // Asignar el color final, manteniendo el canal alfa de la textura para la transparencia
    color = vec4(finalColor, texel.a);

    // Si el fragmento es muy transparente, lo descartamos para que no se dibuje
    if(transparency == 1 && color.a < 0.1)
        discard;
}

// --- MODIFICADO: Ahora calcula la contribución de luz pura, sin textura ---
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir )
{
    vec3 lightDir = normalize( -light.direction );
    
    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );
    
    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );
    
    // Combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec; 
    
    return ( ambient + diffuse + specular );
}

// --- MODIFICADO: Ahora calcula la contribución de luz pura, sin textura ---
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    // Si la luz no tiene componente difuso, la consideramos apagada para ahorrar cálculos.
    if (length(light.diffuse) < 0.01) {
        return vec3(0.0, 0.0, 0.0);
    }
    
    vec3 lightDir = normalize( light.position - fragPos );
    
    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );
    
    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );
    
    // Attenuation
    float distance = length( light.position - fragPos );
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );
    
    // Combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;
    
    return ( ambient + diffuse + specular ) * attenuation;
}

// --- MODIFICADO: Ahora calcula la contribución de luz pura, sin textura ---
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 lightDir = normalize( light.position - fragPos );
    
    // Spotlight intensity check
    float theta = dot( lightDir, normalize( -light.direction ) );
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp( ( theta - light.outerCutOff ) / epsilon, 0.0, 1.0 );
    
    // Si la intensidad es cero, no hay luz, así que no calculamos nada más.
    if (intensity <= 0.0) {
        return vec3(0.0, 0.0, 0.0);
    }

    // Diffuse shading
    float diff = max( dot( normal, lightDir ), 0.0 );
    
    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );
    
    // Attenuation
    float distance = length( light.position - fragPos );
    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );
    
    // Combine results
    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;
    
    return ( ambient + diffuse + specular ) * attenuation * intensity;
}