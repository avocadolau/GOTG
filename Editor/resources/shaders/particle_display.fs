#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;

// Gets the Texture Unit from the main function
uniform sampler2D u_Texture;


float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}
void main()
{
    vec4 texColor = texture(u_Texture, texCoord);
    float depth = LinearizeDepth(gl_FragCoord.z) / far; 

    // FragColor.rgb = vec3(depth);
    // FragColor.a = texColor.a;
     if(texColor.a < 0.004)
        discard;
     FragColor = texColor;
}