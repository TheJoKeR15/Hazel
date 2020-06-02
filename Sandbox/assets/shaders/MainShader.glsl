#type fragment
#version 330 core


layout (location = 0)out vec4 FinalColor;  

in vec3 v_Color;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_PositionWS;

// Structs

// Lights
struct DirrectionalLight {
    vec3 direction;
    float intensity;
}; 



struct PointLight {    
    vec3 position;
    float intensity;
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

uniform bool bMaseked = true;

uniform bool bHasAlbedoTexture = false;
uniform bool bHasSpeclarTexture = false;



 const float constant =  1.0f;
 const float linear =     0.0f;
 const float quadratic = 0.032f;

 const float AlphaClip = 0.33;


vec3 CalcDirLight(DirrectionalLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), SpecularExponent);
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

    return ((diff * Diffuse + spec * Specular ) * light.intensity )+ Ambient ;
} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), SpecularExponent);
    // attenuation
    float Distance = length(light.position - v_PositionWS);
    float attenuation =  1.0 / (constant + linear * Distance + quadratic * (Distance * Distance));
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
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), SpecularExponent);
    // attenuation
    float Distance = length(light.position - v_PositionWS);
    float attenuation =  1.0 / (constant + linear * Distance + quadratic * (Distance * Distance));

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
    vec3 Ambient = Diffuse * 0.05f;
    
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
    if (texture(t_BaseColor, v_TexCoord).a < AlphaClip)
    {
        discard;
    }
    // DIFFUSE //
    // normalizing the normals
    vec3 Normal = normalize(v_Normal);

    vec3 ViewDirection = normalize(CameraPosition - v_PositionWS);

    vec3 Result = vec3(0.f);

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


    FinalColor = vec4(Result, 1.0);

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
  
out vec3 v_Color;                           // output a color to the fragment shader
out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_PositionWS;

void main()
{
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
    v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;                      // set ourColor to the input color we got from the vertex data
    v_TexCoord = a_TexCoord;
    v_PositionWS = vec3(u_Transform * vec4(a_Position, 1.0));
}    
