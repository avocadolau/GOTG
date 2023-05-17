#version 330 core
const int MAX_POINT_LIGHTS = 32;
const int MAX_SPOT_LIGHTS = 32;

struct BaseLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

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
};

struct PointLight
{
    BaseLight Base;
    vec3 LocalPos;
    Attenuation Atten;
};

struct SpotLight
{
    PointLight Base;
    vec3 Direction;
    float Cutoff;
};

in vec3 v_world_normal;
in vec4 v_vertex_color;
in vec2 v_vertex_uv;

in vec3 Normal;
in vec3 LocalPos;
in vec4 LightSpacePos;

// Outline percentage
uniform vec2 u_OutlineSmoothRange = vec2(0.4,0.4);
// Outline color
uniform vec4 u_OutlineColor = vec4(0,0,0,1);

uniform vec4 u_MatAmbientColor;
uniform vec4 u_MatDiffuseColor;
uniform vec4 u_MatSpecularColor;
uniform int u_ToonLevels = 4;
uniform float u_RimLightPower = 4.0;

float ToonScaleFactor = 1.0f / u_ToonLevels;

uniform DirectionalLight u_DirectionalLight;
uniform int u_NumPointLights;
uniform PointLight[MAX_POINT_LIGHTS] u_PointLights;
uniform int u_NumSpotLights;
uniform SpotLight[MAX_SPOT_LIGHTS] u_SpotLights;

uniform sampler2D u_ShadowMap;
uniform sampler2D u_Texture;
uniform float u_SpecularValue;
uniform vec3 u_CameraPosition;

out vec4 out_color;

vec4 GetAmbient(vec3 _world_normal) {
  vec3 normal = normalize(_world_normal);
  vec3 alpha = (normal + 1.) * .5;
  vec2 bt = mix(vec2(.3, .7), vec2(.4, .8), alpha.xz);
  vec3 ambient = mix(vec3(bt.x, .3, bt.x), vec3(bt.y, .8, bt.y), alpha.y);
  return vec4(ambient, 1.);
}

float CalcShadowFactor()
{
    vec3 projCoords = LightSpacePos.xyz / LightSpacePos.w;
    vec2 UVCoords;
    UVCoords.x = 0.5 * projCoords.x + 0.5;
    UVCoords.y = 0.5 * projCoords.y + 0.5;
    float z = 0.5 * projCoords.z + 0.5;
    float depth = texture(u_ShadowMap, UVCoords).x;

    float bias = 0.0025;

    if(depth + bias < z)
        return 0.5;
    else
        return 1.0;
}

float CalcRimLightFactor(vec3 pixelToCamera, vec3 normal)
{
    float rimFactor = dot(pixelToCamera, normal);
    rimFactor = 1.0 - rimFactor;
    rimFactor = max(0.0, rimFactor);
    rimFactor = pow(rimFactor, u_RimLightPower);
    return rimFactor;
}

vec4 CalcLightInternal(BaseLight light, vec3 direction, vec3 normal, float shadowFactor)
{
    vec4 ambientColor = vec4(light.Color, 1.0f) * light.AmbientIntensity * u_MatAmbientColor;

    float diffuseFactor = dot(normal, -direction);

    vec4 diffuseColor = vec4(0, 0, 0, 0);
    vec4 specularColor = vec4(0, 0, 0, 0);
    vec4 rimColor = vec4(0, 0, 0, 0);

    if(diffuseFactor > 0) 
    {
        diffuseFactor = floor(diffuseFactor * u_ToonLevels) * ToonScaleFactor;

        diffuseColor = vec4(light.Color, 1.0f) * light.DiffuseIntensity * u_MatDiffuseColor * diffuseFactor;

        vec3 pixelToCamera = normalize(u_CameraPosition - LocalPos);

        float rimFactor = CalcRimLightFactor(pixelToCamera, normal);
        rimColor = diffuseColor * rimFactor;
    }

    return (ambientColor + shadowFactor * (diffuseColor + specularColor + rimColor));
}

vec4 CalcDirectionalLight(vec3 normal)
{
    float shadowFactor = CalcShadowFactor();
    return CalcLightInternal(u_DirectionalLight.Base, u_DirectionalLight.Direction, normal, shadowFactor);
}

vec4 CalcPointLight(PointLight light, vec3 normal)
{
    float shadowFactor = 1.0;

    vec3 lightDirection = LocalPos - light.LocalPos;
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    vec4 color = CalcLightInternal(light.Base, lightDirection, normal, shadowFactor);

    float attenuation = light.Atten.Constant +
                        light.Atten.Linear * distance *
                        light.Atten.Exp * distance * distance;

    return color / attenuation;
}
vec4 CalcSpotLight(SpotLight light, vec3 normal)
{
    vec3 lightToPixel = normalize(LocalPos - light.Base.LocalPos);
    float spotFactor = dot(lightToPixel, light.Direction);

    if(spotFactor > light.Cutoff)
    {
        vec4 color = CalcPointLight(light.Base, normal);
        float spotLightIntensity = (1.0 - (1.0 - spotFactor) / (1.0 - light.Cutoff));
        return color * spotLightIntensity;
    }
}

void main() {
	vec4 ambient = GetAmbient(v_world_normal);
  
	vec3 normal = normalize(Normal);

	vec4 totalLight = CalcDirectionalLight(normal);
	
	for(int i = 0; i < u_NumPointLights; i++)
    {
        totalLight += CalcPointLight(u_PointLights[i], normal);
    }

    for(int i = 0; i < u_NumSpotLights; i++)
    {
        totalLight += CalcSpotLight(u_SpotLights[i], normal);
    }
	
	vec3 dir = u_CameraPosition - LocalPos;
	
	float dotEyeNormal = dot(normalize(dir),normalize(Normal));
    dotEyeNormal =abs(dotEyeNormal);
	
	vec2 flipuv = vec2(v_vertex_uv.s, 1. - v_vertex_uv.t);
	
	vec4 color = ambient * v_vertex_color * texture(u_Texture, flipuv);
	
	vec4 colorLight  = color * totalLight;
	
	out_color = mix(u_OutlineColor, colorLight, smoothstep(u_OutlineSmoothRange.x,u_OutlineSmoothRange.y,dotEyeNormal)) ;
}