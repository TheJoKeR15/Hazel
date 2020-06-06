#type fragment
#version 330 core


layout (location = 0)out vec4 FinalColor;

in vec3 v_Color;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_PositionWS;
in vec4 FragPosLightSpace;

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

// Material Uniforms
uniform sampler2D t_BaseColor;
uniform sampler2D t_Specular;


uniform float SpecularStrenght;
uniform float SpecularExponent;


uniform float AmbientLight;

uniform vec3 BasecolorTint;

uniform bool bMasked = true;

uniform bool bHasAlbedoTexture = false;
uniform bool bHasSpeclarTexture = false;



 const float constant =  1.0f;
 const float linear =     0.0f;
 const float quadratic = 0.032f;

 const float AlphaClip = 0.33;

 const int PCFSamples = 4;
 


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
    vec3 Diffuse = vec3(0.f);
    if (bHasAlbedoTexture)
    {
        
        Diffuse = texture(t_BaseColor, v_TexCoord).rgb;
    }
    else
    {
        Diffuse = BasecolorTint;
    }
    vec3 Ambient = Diffuse * AmbientLight;
    
    float Specular = 0.f;
    if (bHasSpeclarTexture)
    {
        Specular = SpecularStrenght * texture(t_Specular, v_TexCoord).r;
    }
    else
    {
        Specular = SpecularStrenght;
    }
    
    float bias = max(ShadowBias * (1.0 - dot(normal, lightDir)), 0.005); 
    float shadow =  ShadowCalculation(FragPosLightSpace,bias);

    return ((diff * Diffuse + spec * Specular ) * light.intensity * (1.0 - shadow ))+ Ambient ;

} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 HalfDir = normalize(lightDir + viewDirection);
    float spec = pow(max(dot(normal, HalfDir), 0.0), SpecularExponent);
    // attenuation
    float Distance = length(light.position - v_PositionWS);
    float attenuation =  1.0 / (Distance * Distance);
    // combine results
    vec3 Diffuse = vec3(0.f);
    if (bHasAlbedoTexture)
    {
        Diffuse = texture(t_BaseColor, v_TexCoord).rgb;
    }
    else
    {
        Diffuse = BasecolorTint;
    }
    vec3 Ambient = Diffuse * AmbientLight;
    
    float Specular = 0.f;
    if (bHasSpeclarTexture)
    {
        Specular = SpecularStrenght * texture(t_Specular, v_TexCoord).r;
    }
    else
    {
        Specular = SpecularStrenght;
    }

    return (((diff * Diffuse + spec * Specular ) * light.intensity+ Ambient )* attenuation ) ;

} 
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 HalfDir = normalize(lightDir + viewDirection);
    float spec = pow(max(dot(normal, HalfDir), 0.0), SpecularExponent);
    // attenuation
    float Distance = length(light.position - v_PositionWS);
    float attenuation =  1.0 /  (Distance * Distance);

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.Cutoff_In - light.Cutoff_Out;

    float intensity = clamp((theta - light.Cutoff_Out) / epsilon, 0.0, 1.0);

       vec3 Diffuse = vec3(0.f);
    if (bHasAlbedoTexture)
    {
        Diffuse = texture(t_BaseColor, v_TexCoord).rgb;
    }
    else
    {
        Diffuse = BasecolorTint;
    }
    vec3 Ambient = Diffuse * AmbientLight;
    
    float Specular = 0.f;
    if (bHasSpeclarTexture)
    {
        Specular = SpecularStrenght * texture(t_Specular, v_TexCoord).r;
    }
    else
    {
        Specular = SpecularStrenght;
    }

    return ((diff * Diffuse + spec * Specular ) * light.intensity + Ambient)* intensity *attenuation;
}
  
void main()
{
    if (bMasked &&texture(t_BaseColor, v_TexCoord).a < AlphaClip)
    {
        discard;
    }
    // DIFFUSE //
    // normalizing the normals
    vec3 Normal = normalize(v_Normal);

    vec3 ViewDirection = normalize(CameraPosition - v_PositionWS);

    vec3 Result = vec3(0.0);

    // phase 1: Directional lighting
    Result = CalcDirLight(DirLight, Normal, ViewDirection);
    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        Result += CalcPointLight(pointLights[i], Normal, v_PositionWS, ViewDirection);    
    }
    // phase 3: Spot light
    for(int i = 0; i < NR_SPOT_LIGHTS; i++)
    {
        Result += CalcSpotLight(spotLights[i], Normal, v_PositionWS, ViewDirection);    
    }

    //FinalColor = vec4(vec3(shdow),1.0f);
    float gamma = 2.2;
    
    FinalColor = vec4(Result, 1.0);




    //FinalColor = FragPosLightSpace;

    //FinalColor.rgb = pow(FinalColor.rgb, vec3(1.0/gamma));

    /*
    // SPECULAR // 
    // Camera direction 

    // Specular Contribution
    float spec = pow(max(dot(ViewDirection,ReflectionDir),0.0),SpecularExponent);

    vec3 Specular =vec3(1.f);
    if (bHasSpeclarTexture)
    {
        Specular = SpecularStrenght * u_LightColor * u_LightIntensity * spec * texture(t_Specular, v_TexCoord).r;
    }
    else
    {
        Specular = SpecularStrenght * u_LightColor * spec ;
    }

    Specular *= intensity;

      /*
    // spotlight (soft edges)
    float theta = dot(LightDir, normalize(-u_LightDirection)); 
    float epsilon = u_LightCutoff - u_LightCutoffOut;
    float intensity = clamp((theta - u_LightCutoffOut) / epsilon, 0.0, 1.0);
    

    // reflection Vector
    vec3 ReflectionDir = reflect(-LightDir,Normal);

    
    vec3 Result = (( Specular* attenuation  + Diffuse* attenuation  ))+ AmbiantLight;
    
    FinalColor =  vec4(Result,1.0f);
    */
    //FinalColor  = texture(t_Specular, v_TexCoord);
    //FinalColor = vec4(vec3(SpecularStrenght),1.);

}

#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;   // the position variable has attribute position 0
layout (location = 1) in vec3 a_Normal;      
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform mat4 lightSpaceMatrix;
  
out vec3 v_Color;                           // output a color to the fragment shader
out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_PositionWS;
out vec4 FragPosLightSpace;

void main()
{
    
    v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;                      // set ourColor to the input color we got from the vertex data
    v_TexCoord = a_TexCoord;
    v_PositionWS = vec3(u_Transform * vec4(a_Position, 1.0));
    FragPosLightSpace = lightSpaceMatrix * vec4(v_PositionWS, 1.0);
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}    
