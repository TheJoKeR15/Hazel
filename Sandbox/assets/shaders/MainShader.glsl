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
    v_Normal = a_Normal;                      // set ourColor to the input color we got from the vertex data
    v_TexCoord = a_TexCoord;
    v_PositionWS = vec3(u_Transform * vec4(a_Position, 1.0));
}    

#type fragment
#version 330 core


out vec4 FinalColor;  
in vec3 v_Color;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_PositionWS;

uniform vec3 u_Color;
uniform float u_AmbiantLight;
uniform vec3 u_LightColor;
uniform vec3 u_LightPosition;
uniform float u_LightIntensity;
uniform float u_LightRadius;
uniform sampler2D u_Texture;
  
void main()
{
    // normalizing the normals
    vec3 Normal = normalize(v_Normal);

    // calculating light direction
    vec3 ldir = normalize(u_LightPosition - v_PositionWS);

    // calculating light Contribution
    float l = max(dot(ldir,Normal),0.f);

    // Claculating the diffuse Term
    vec3 Diffuse = l * u_LightColor;

    // Claculating the Ambient Term
    Vec3 AmbiantLight = vec3(u_AmbiantLight);
    
    FinalColor = texture(u_Texture, v_TexCoord) * ( Diffuse + AmbiantLight) ;

}