#type fragment
#version 330 core


layout (location = 0)out vec4 FinalColor;  

in vec3 v_Color;
in vec3 v_TexCoord;
//in vec3 v_Normal;
//in vec3 v_PositionWS;

// Structs

//Utility
//uniform vec3 CameraPosition;

// Material Uniforms
uniform samplerCube Cubemap;

  
void main()
{
    FinalColor = texture(Cubemap, v_TexCoord);
}

#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;   // the position variable has attribute position 0
layout (location = 1) in vec3 a_Normal;      
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
  
out vec3 v_Color;                           // output a color to the fragment shader
out vec3 v_TexCoord;
//out vec3 v_Normal;
//out vec3 v_PositionWS;

void main()
{
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
    //v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;                      // set ourColor to the input color we got from the vertex data
    v_TexCoord = a_Position;
    //v_PositionWS = vec3(u_Transform * vec4(a_Position, 1.0));
}    
