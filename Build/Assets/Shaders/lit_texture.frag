#version 430

in layout(location = 0) vec3 fposition;
in layout(location = 1) vec3 fnormal;
in layout(location = 2) vec2 ftexcoord;

out layout(location = 0) vec4 ocolor;

layout(binding = 0) uniform sampler2D tex;

uniform struct Material
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
} material;

uniform vec3 ambientLight;

uniform struct Light
{
    vec3 lightPosition;
    vec3 diffuseLight;
} light;

vec3 ads(in vec3 position, in vec3 normal)
{
    // AMBIENT
    vec3 ambient = ambientLight;

    // DIFFUSE
    vec3 lightDir = normalize(light.lightPosition - position);
    // calculates the cosine of the angle between the light direction and the normal vector
    float intensity = max(dot(lightDir, normal), 0);
    // calculates the diffuse reflection color by scaling the lights diffuse color with the intensity
    vec3 diffuse = (light.diffuseLight * intensity) * material.diffuse;

    // SPECULAR
    vec3 specular = vec3(0);
    if (intensity > 0)
    {
        // calculates reflection dir using law of reflection
        vec3 reflection = reflect(-lightDir, normal);
        // calculates view dir by normalizing the vector pointing from the frag to the 'camera'
        vec3 viewDir = normalize(-position);
        // calculates intensity of specular reflection based on the angle between reflection and view directions
        intensity = max(dot(reflection, viewDir), 0);
        // apply shininess factor to the intensity
        intensity = pow(intensity, material.shininess);
        // calculates the specular reflection color by scaling the material's specular color with the intensity
        specular = material.specular * intensity;
    }

    return ambient + diffuse + specular;
}

void main()
{
    vec4 texcolor = texture(tex, ftexcoord);
    // ocolor calculated using the ads function
    ocolor = texcolor * vec4(ads(fposition, fnormal), 1);
}