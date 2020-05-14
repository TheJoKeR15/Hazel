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

#type fragment
#version 330 core


layout (location = 0)out vec4 FinalColor;  

in vec3 v_Color;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_PositionWS;

//Utility
uniform vec3 CameraPosition;

//Light Uniforms
uniform float u_AmbiantLight;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform float u_LightIntensity;
uniform float u_LightRadius;

// Material Uniforms
uniform sampler2D u_Texture;
uniform float SpecularStrenght;
uniform float SpecularExponent;

//float SpecularExponent = 128;
//float SpecularStrenght = 1;
  
void main()
{

    // DIFFUSE //
    // normalizing the normals
    vec3 Normal = normalize(v_Normal);

    // calculating light direction
    vec3 LightDir = normalize(u_LightPosition - v_PositionWS);

    // calculating light Contribution
    float l = max(dot(Normal,LightDir),0.f);

    // Claculating the diffuse Term
    vec3 Diffuse = l * u_LightColor * u_LightIntensity;

    // Claculating the Ambient Term
    vec3 AmbiantLight = vec3(u_AmbiantLight);

    // SPECULAR // 
    // Camera direction 
    vec3 ViewDirection = normalize(CameraPosition - v_PositionWS);

    // reflection Vector
    vec3 ReflectionDir = reflect(-LightDir,Normal);

    // Specular Contribution
    float spec = pow(max(dot(ViewDirection,ReflectionDir),0.0),SpecularExponent);

    vec3 Specular = SpecularStrenght * u_LightColor * spec;

    vec3 Result = ( Specular + Diffuse + AmbiantLight);
    
    FinalColor = texture(u_Texture, v_TexCoord) * vec4(Result,1.0f);
    //FinalColor = vec4(u_LightPosition,1.);

}