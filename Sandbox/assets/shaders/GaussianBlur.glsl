#type fragment
#version 330 core

// Exposed Image
out vec4 FinalColor;


in vec2 TexCoords;


uniform sampler2D BloomImage;
uniform bool bHorizontal;
uniform bool Enable;
//uniform float weight[6] = float[] (
//0.002322	0.00446	0.008051	0.013658	0.021773	0.032616	0.045914	0.060738	0.075504	0.088201	0.096823	0.09988);
uniform float weight[12] = float[] (0.09988,	0.096823,	0.088201,	0.075504,	0.060738,	0.045914,0.032616,0.021773,0.013658,0.008051,0.00446,0.002322);

void main()
{
    if (false)
    {
        FinalColor = vec4(texture(BloomImage, TexCoords).rgb, 1.0);
    }
    else
    { 
        vec2 tex_offset = 1.0 / textureSize(BloomImage, 0); // gets size of single texel
        vec3 result = texture(BloomImage, TexCoords).rgb * weight[0]; // current fragment's contribution
        

        if(bHorizontal)
        {
            for(int i = 1; i < 12; ++i)
            {
                result += texture(BloomImage, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
                result += texture(BloomImage, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            }
        }
        else
        {
            for(int i = 1; i < 12; ++i)
            {
                result += texture(BloomImage, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
                result += texture(BloomImage, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            }
        }

        FinalColor = vec4(result, 1.0);
        //FinalColor.rgb = vec3(0.1176, 0.6745, 0.1176);
        FinalColor.a = 1.0;
    }
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
