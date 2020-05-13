#type vertex
#version 330 core
layout (location = 0) in vec3 a_Position;   // the position variable has attribute position 0
layout (location = 1) in vec3 a_Color;      // the color variable has attribute position 1
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
  
out vec3 v_Color;                           // output a color to the fragment shader
out vec2 v_TexCoord;

void main()
{
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
    v_Color = a_Color;                      // set ourColor to the input color we got from the vertex data
    v_TexCoord = a_TexCoord;
}    

#type fragment
#version 330 core


out vec4 FinalColor;  
in vec3 v_Color;
in vec2 v_TexCoord;

uniform vec3 u_Color;
uniform sampler2D u_Texture;
  
void main()
{
    FinalColor = texture(u_Texture, v_TexCoord)  ;
}