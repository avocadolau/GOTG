#version 330
 #extension GL_ARB_explicit_uniform_location : enable

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 LocalPos;

//====================================
// all vfx must have this unifroms
//lifetime form 1 to 0,  100% normalized
uniform float u_LifeTime;
uniform vec4 u_Color;
//=====================
uniform vec3 u_CameraPosition;
uniform sampler2D u_Texture;
uniform sampler2D u_DiscardTex;

void main()
{
    //calculate dissolve

    if(u_LifeTime > 0.9)
    {
        float dissolveAmount = (u_LifeTime - 0.9) * 10.0;

        float dissolve = texture2D(u_DiscardTex, TexCoord).r;
        dissolve = dissolve *  0.999;

        float visible = dissolve - dissolveAmount;

        if(visible < 0)
            discard;
    }

    //calculate fresnel
    vec3 dir = u_CameraPosition - LocalPos;

    float dotEyeNormal = dot(normalize(dir),normalize(Normal));
    dotEyeNormal =abs(dotEyeNormal);


    vec4 color = texture2D(u_Texture, TexCoord) * u_Color;

     // Check if texture color is black
    bool isBlack = color.r == 0.0 && color.g == 0.0 && color.b == 0.0;

    if(isBlack) 
        discard;

    vec4 finalColor = mix( vec4(1,1,1,1) , color,smoothstep(0.2,0.5,dotEyeNormal));

    FragColor = finalColor;

    // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
	else
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}