#type fragment
#version 330 core


layout (location = 0)out vec4 FinalColor;


in vec2 v_TexCoords;


// Structs

// Lights
struct DirrectionalLight {
    vec3 direction;
    float intensity;
}; 



struct PointLight {    
    vec3 position;
    float intensity ;
};  

struct SpotLight {

    vec3 position;
    vec3 direction;
    float intensity;
    float Cutoff_In;
    float Cutoff_Out;
};


//Utility
uniform vec3 CameraPosition;
uniform float AmbientLight;
// Shadows
uniform sampler2D DirectionalShadowMap;
uniform float ShadowBias;


vec2 TEXELSIZE = 1.0 / textureSize(DirectionalShadowMap, 0);

//Light Uniforms
uniform DirrectionalLight DirLight;
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];
#define NR_SPOT_LIGHTS 4
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;

 const float constant =  1.0f;
 const float linear =     0.0f;
 const float quadratic = 0.032f;

 const float AlphaClip = 0.33;

 const int PCFSamples = 4;
 const float SpecularExponent = 32.0;

 float SampleShadow(sampler2D ShadowMap,vec2 Coords, float Compare);
 float SampleShadowLinear(sampler2D ShadowMap,vec2 Coords,float Compare);
 float SampleShadowPCF(sampler2D ShadowMap,vec2 Coords,float Compare);
 vec3 CalcDirLight(DirrectionalLight light, vec3 normal, vec3 viewDirection);
 vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection,vec3 PositionWS);
 vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection,vec3 PositionWS);

 
void main()
{

    // DIFFUSE //
    vec3 Normal = texture(gNormal, v_TexCoords).rgb;
    vec3 Albedo = texture(gColor, v_TexCoords).rgb;
    float Specular = texture(gColor, v_TexCoords).a;
    vec3 PositionWS = texture(gPosition, v_TexCoords).rgb;

    

    vec3 ViewDirection = normalize(CameraPosition - PositionWS);

    vec3 Result = vec3(0.0);

    // phase 1: Directional lighting
    Result = CalcDirLight(DirLight, Normal, ViewDirection);
    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        Result += CalcPointLight(pointLights[i], Normal, PositionWS, ViewDirection,PositionWS);    
    }
    // phase 3: Spot light
    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
        Result += CalcSpotLight(spotLights[i], Normal, PositionWS, ViewDirection,PositionWS);    
    }

    //FinalColor = vec4(vec3(shdow),1.0f);
    float gamma = 2.2;
    
    FinalColor = vec4(Result, 1.0);

}

 float SampleShadow(sampler2D ShadowMap,vec2 Coords, float Compare)
 {
    //return Compare > texture(ShadowMap,Coords).r  ? 1.0 : 0.0;
    return step(  texture(ShadowMap,Coords).r,Compare);
 }

 float SampleShadowLinear(sampler2D ShadowMap,vec2 Coords,float Compare)
 {
     vec2 pixelpos = Coords/TEXELSIZE + vec2(0.5);
     vec2 fracPart = fract(pixelpos);
     vec2 StartTexel = (pixelpos - fracPart)*TEXELSIZE ;

     float texe1 = SampleShadow(ShadowMap,StartTexel,Compare);
     float texe2 = SampleShadow(ShadowMap,StartTexel + vec2(TEXELSIZE.x,0.0),Compare);
     float texe3 = SampleShadow(ShadowMap,StartTexel + vec2(0.0,TEXELSIZE.y),Compare);
     float texe4 = SampleShadow(ShadowMap,StartTexel + TEXELSIZE,Compare);

     float mix1 = mix(texe1,texe3,fracPart.y);
     float mix2 = mix(texe2,texe4,fracPart.y);

     return mix(mix1 , mix2,fracPart.x);
 }

  float SampleShadowPCF(sampler2D ShadowMap,vec2 Coords,float Compare)
 {
    const float SAMPLE_START = (PCFSamples - 1.0)/2.0;
    const float NUM_SAMPLE_SQUARED = PCFSamples * PCFSamples;

    float result = 0.f;
    for(float x = -SAMPLE_START; x <= SAMPLE_START; x += 1.0f)
    {
        for(float y = -SAMPLE_START; y <= SAMPLE_START; y+= 1.0f)
        {
        vec2 coordOffset = vec2(x,y)*TEXELSIZE;
        result += SampleShadowLinear(ShadowMap, Coords + coordOffset , Compare);
        }    
    }
    return result/(NUM_SAMPLE_SQUARED);
 }

 float ShadowCalculation(vec4 fragPosLightSpace,float Bias)
{
    float shadow = 0.0;
        // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    if(projCoords.z > 1.0)
    {
        return shadow;
    }
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
        // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // check whether current frag pos is in shadow
     
    //shadow = currentDepth - Bias > closestDepth  ? 1.0 : 0.0;  

    return SampleShadowPCF(DirectionalShadowMap, projCoords.xy,currentDepth - Bias);
}

vec3 CalcDirLight(DirrectionalLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize(-light.direction);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 HalfDir = normalize(lightDir + viewDirection);
    float spec = pow(max(dot(normal, HalfDir), 0.0), SpecularExponent);
    // combine results
        ////Diffuse////
    vec3 Diffuse = texture(gColor, v_TexCoords).rgb;

    vec3 Ambient = Diffuse * AmbientLight;
    
    float Specular = texture(gColor, v_TexCoords).a;
    
    float bias = max(ShadowBias * (1.0 - dot(normal, lightDir)), 0.005); 
    //float shadow =  ShadowCalculation(FragPosLightSpace,bias);(1.0 - shadow )

    return ((diff * Diffuse + spec * Specular ) * light.intensity )+ Ambient ;

} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection,vec3 PositionWS)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 HalfDir = normalize(lightDir + viewDirection);
    float spec = pow(max(dot(normal, HalfDir), 0.0), SpecularExponent);
    // attenuation
    float Distance = length(light.position - PositionWS);
    float attenuation =  1.0 / (Distance * Distance);
    // combine results
    vec3 Diffuse = texture(gColor, v_TexCoords).rgb;
    vec3 Ambient = Diffuse * AmbientLight;
    
    float Specular = texture(gColor, v_TexCoords).a;
    return (((diff * Diffuse + spec * Specular ) * light.intensity+ Ambient )* attenuation ) ;

} 
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection,vec3 PositionWS)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 HalfDir = normalize(lightDir + viewDirection);
    float spec = pow(max(dot(normal, HalfDir), 0.0), SpecularExponent);
    // attenuation
    float Distance = length(light.position - PositionWS);
    float attenuation =  1.0 /  (Distance * Distance);

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.Cutoff_In - light.Cutoff_Out;

    float intensity = clamp((theta - light.Cutoff_Out) / epsilon, 0.0, 1.0);

    vec3 Diffuse = texture(gColor, v_TexCoords).rgb;
    vec3 Ambient = Diffuse * AmbientLight;
    
    float Specular = texture(gColor, v_TexCoords).a;

    return ((diff * Diffuse + spec * Specular ) * light.intensity + Ambient)* intensity *attenuation;
}
  




#type vertex
#version 330 core

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoords;   // the position variable has attribute position 0

out vec2 v_TexCoords;
void main()
{
    v_TexCoords = a_TexCoords;
    gl_Position = vec4(a_Position.xy,0.0, 1.0);
}  
