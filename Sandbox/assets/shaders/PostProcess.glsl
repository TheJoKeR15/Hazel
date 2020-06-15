#type fragment
#version 330 core

// Exposed Image
layout (location = 0) out vec4 FinalColor;
// Bloom
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D HDRImage;
uniform sampler2D BloomImage;
uniform float ExposureCompensation;
uniform float Aperture;
uniform float ShutterSpeed;
uniform float Iso;

uniform float BloomTreshold;



// Get an exposure using the Saturation-based Speed method.
float getSaturationBasedExposure(float aperture,float shutterSpeed,float iso);

// ACES Filmic tonemaping
vec3 ACESFilm(vec3 x);


float computeEV100FromAvgLuminance( float avgLuminance );
float computeEV100( float aperture , float shutterTime , float ISO );
float convertEV100(float EV100);

//vec3 accurateLinearToSRGB(vec3 linearCol );

void main()
{
    // HDR
    vec3 col = texture(HDRImage, TexCoords).rgb;
    // manual settings
    float EV100 = computeEV100( Aperture , ShutterSpeed , Iso );

    float exposure = convertEV100(EV100-ExposureCompensation);
   
    // Tonmapping and gamma correction
    //vec3 Tonmapping = ACESFilm(col * exposure);
    FinalColor.rgb = col * exposure;
    FinalColor.a = 1.0;
    
        float brightness = dot(FinalColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > BloomTreshold)
        BrightColor = vec4(FinalColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}

/*
* Get an exposure using the Saturation-based Speed method.
*/
float getSaturationBasedExposure(float aperture,
                                 float shutterSpeed,
                                 float iso)
{
    float l_max = (7800.0f / 65.0f) * (aperture*aperture) / (iso * shutterSpeed);
    return 1.0f / l_max;
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

float computeEV100( float aperture, float shutterTime, float ISO )
{
// EV number is defined as:
// 2^ EV_s = N^2 / t and EV_s = EV_100 + log2 (S /100)
// This gives
// EV_s = log2 (N^2 / t)
// EV_100 + log2 (S /100) = log2 (N^2 / t)
// EV_100 = log2 (N^2 / t) - log2 (S /100)
// EV_100 = log2 (N^2 / t . 100 / S)
 return log2((aperture * aperture) / shutterTime * 100 / ISO );
 }

 float computeEV100FromAvgLuminance( float avgLuminance )
 {
 // We later use the middle gray at 12.7% in order to have
 // a middle gray at 18% with a sqrt (2) room for specular highlights
 // But here we deal with the spot meter measuring the middle gray
 // which is fixed at 12.5 for matching standard camera
 // constructor settings (i.e. calibration constant K = 12.5)
 // Reference : http :// en. wikipedia . org / wiki / Film_speed
 return log2(avgLuminance * 100.0 / 12.5) ;
 }

float convertEV100(float EV100)
 {
    // Compute the maximum luminance possible with H_sbs sensitivity
    // maxLum = 78 / ( S * q ) * N^2 / t
    // = 78 / ( S * q ) * 2^ EV_100
    // = 78 / (100 * 0.65) * 2^ EV_100
    // = 1.2 * 2^ EV
    // Reference : http :// en. wikipedia . org / wiki / Film_speed
    float maxLuminance = 1.2f * pow(2.0f , EV100 );
    float result =(1.0f / maxLuminance);
    return result;
 }

/*  vec3 accurateLinearToSRGB (vec3 linearCol )
{
    vec3 sRGBLo = linearCol * 12.92;
    //vec3 sRGBHi = ( pow(abs(linearCol), 1.0/2.4) * 1.055) - 0.055;
    vec3 sRGB = ( linearCol <= 0.0031308) ? sRGBLo : sRGBHi ;
    return sRGB ;
} */




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
