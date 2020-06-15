#type fragment
#version 330 core


layout (location = 0) out vec4 gAlbedoSpec;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gPosition;

in vec3 v_Color;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_PositionWS;

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


  
void main()
{
     // store the fragment position vector in the first gbuffer texture
    gPosition = v_PositionWS;
    // also store the per-fragment normals into the gbuffer
    gNormal = normalize(v_Normal);
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = bHasAlbedoTexture ? texture(t_BaseColor, v_TexCoord).rgb  : BasecolorTint;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = bHasSpeclarTexture ? texture(t_Specular, v_TexCoord).r : SpecularStrenght;
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
    
    v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;                      // set ourColor to the input color we got from the vertex data
    v_TexCoord = a_TexCoord;
    v_PositionWS = vec3(u_Transform * vec4(a_Position, 1.0));
    //FragPosLightSpace = lightSpaceMatrix * vec4(v_PositionWS, 1.0);
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}    
