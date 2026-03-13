#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aOffset;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 pos = vec3(aPos.x * 0.05, aPos.y * 0.05, 0.0) + aOffset;
    gl_Position = projection * view * vec4(pos, 1.0);
}
