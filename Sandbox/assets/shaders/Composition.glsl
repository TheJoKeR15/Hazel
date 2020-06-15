#type fragment
#version 330 core

// Exposed Image
out vec4 FinalColor;

in vec2 TexCoords;

uniform sampler2D ExposedImage;
uniform sampler2D BloomImage1;
uniform sampler2D BloomImage2;
uniform sampler2D BloomImage3;
uniform sampler2D BloomImage4;
uniform sampler2D BloomImage5;

uniform float BloomIntensity;

// ACES Filmic tonemaping
vec3 ACESFilm(vec3 x);



void main()
{
    // Exposed Image
    vec3 col = texture(ExposedImage, TexCoords).rgb;
    //col = vec3(0.8863, 0.1569, 0.1569);
    vec3 Bloomed = texture(BloomImage1, TexCoords).rgb;
    Bloomed +=  texture(BloomImage2, TexCoords).rgb;
    Bloomed +=  texture(BloomImage3, TexCoords).rgb;
    Bloomed +=  texture(BloomImage4, TexCoords).rgb;
    Bloomed +=  texture(BloomImage5, TexCoords).rgb;
    // Tonmapping 
    vec3 Tonmapping = ACESFilm(col+Bloomed*BloomIntensity);
    // and gamma correction
    FinalColor.rgb = pow(Tonmapping,vec3(1/2.2));
    //FinalColor.rgb = Bloomed;
    FinalColor.a = 1.0;

}

vec3 ACESFilm(vec3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e),0.0,1.0);
}

#type vertex
#version 330 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoords;   // the position variable has attribute position 0

out vec2 TexCoords;
void main()
{
    TexCoords = a_TexCoords;
    gl_Position = vec4(a_Position.xy,0.0, 1.0);
}  
