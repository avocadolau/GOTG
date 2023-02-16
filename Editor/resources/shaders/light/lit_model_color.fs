#version 330 core

#define MAX_POINT_LIGHTS 128

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 LocalPos;

struct BaseLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
}

struct DirectionalLight
{
    BaseLight Base;
    vec3 Direction;
};

struct Attenuation
{
    float Constant;
    float Linear;
    float Exp;
}

struct PointLight
{
    BaseLight Base;
    vec3 LocalPos;
    Attenuation Atten;
}

uniform vec4 u_MatAmbientColor;
uniform vec4 u_MatDiffuseColor;
uniform vec4 u_MatSpecularColor;

uniform DirectionalLight u_DirectionalLight;
uniform PointLight[MAX_POINT_LIGHTS] u_PointLights;
uniform int u_NumPointLights;

uniform vec4 u_Color;
uniform float u_SpecularValue;
uniform vec3 u_CameraPosition;


vec4 CalcLightInternal(BaseLight light, vec3 direction, vec3 normal)
{
    vec4 ambientColor = vec3(light.Color, 1.0f) * light.AmbientIntensity * u_MatAmbientColor;

    float diffuseFactor = dot(normal, -direction);

    vec4 diffuseColor = vec4(0, 0, 0, 0);
    vec4 specularColor = vec4(0, 0, 0, 0);

    if(diffuseFactor > 0) 
    {
        diffuseColor = vec4(light.Color, 1.0f) * light.DiffuseIntensity * u_MatDiffuseColor * diffuseFactor;

        vec3 pixelToCamera = normalize(u_CameraPosition - LocalPos);
        vec3 lightReflect = normalize(reflect(direction, normal));
        float specularFactor = dot(pixelToCamera, lightReflect);

        if(specularFactor > 0)
        {
            specularFactor = pow(specularFactor, u_SpecularValue);
            specularColor = vec4(light.Color, 1.0f) * light.DiffuseIntensity * u_MatSpecularColor * specularFactor;
        }
    }

    return ambientColor + diffuseColor + specularColor;
}

vec4 CalcDirectionalLight(vec3 normal)
{
    return CalcLightInternal(u_DirectionalLight.Base, u_DirectionalLight.Direction, normal);
}

vec4 CalcPointLight(int index, vec3 normal)
{
    vec3 lightDirection = LocalPos - u_PointLights[index].LocalPos;
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    vec4 color = CalcLightInternal(u_PointLights[index].Base, lightDirection, normal);

    float attenuation = u_DirectionalLight[index].Atten.Constant +
                        u_DirectionalLight[index].Atten.Linear * distance *
                        u_DirectionalLight[index].Atten.Exp * distance * distance;

    return color / attenuation;
}


void main()
{
    vec3 normal = normalize(Normal);
    vec4 totalLight = CalcDirectionalLight(normal);

    for(int i = 0; i < u_NumPointLights; i++)
    {
        totalLight += CalcPointLight(i, normal);
    }

    FragColor = u_Color * totalLight;
}