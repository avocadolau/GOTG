#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 LocalPos;

struct DirectionalLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
    vec3 Direction;
};

uniform vec4 u_MatAmbientColor;
uniform vec4 u_MatDiffuseColor;
uniform vec4 u_MatSpecularColor;

uniform DirectionalLight u_DirectionalLight;
uniform vec4 u_Color;
uniform float u_SpecularValue;
uniform vec3 u_CameraPosition;

void main()
{
     vec4 ambientColor = vec4(u_DirectionalLight.Color, 1.0f) *
                        u_DirectionalLight.AmbientIntensity *
                        u_MatAmbientColor;

    vec3 normal = normalize(Normal);

    float diffuseFactor = dot(normal, -u_DirectionalLight.Direction);

    vec4 diffuseColor = vec4(0, 0, 0, 0);
    vec4 specularColor = vec4(0, 0, 0, 0);

    if (diffuseFactor > 0) {
        diffuseColor = vec4(u_DirectionalLight.Color, 1.0f) *
                       u_DirectionalLight.DiffuseIntensity *
                       u_MatDiffuseColor *
                       diffuseFactor;

        vec3 pixelToCamera = normalize(u_CameraPosition - LocalPos);
        vec3 lightReflect = normalize(reflect(u_DirectionalLight.Direction, normal));

        float specularFactor = dot(pixelToCamera, lightReflect);
        if (specularFactor > 0) {
            specularFactor = pow(specularFactor, u_SpecularValue);
            specularColor = vec4(u_DirectionalLight.Color, 1.0f) *
                            u_MatSpecularColor *
                            specularFactor;
        }
    }

    FragColor = u_Color *
                (ambientColor + diffuseColor + specularColor);
}