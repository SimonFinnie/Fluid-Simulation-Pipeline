#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 view;
uniform mat4 projection;
uniform int freeCam;

void main()
{
    if(freeCam == 1){
        gl_Position = projection * view * vec4(aPos, 1.0f);
    }
    else{
        gl_Position = vec4(aPos, 1.0f);
    }
    TexCoord = aTexCoord;
}