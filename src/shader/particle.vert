#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aOffset;

uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
    float angle = float(gl_InstanceID) * 0.743 + (time * 3.0);
    mat2 rot = mat2(cos(angle), -sin(angle),
            sin(angle), cos(angle));
    vec2 rotatedPos = rot * aPos;

    vec3 cameraRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 cameraUp = vec3(view[0][1], view[1][1], view[2][1]);

    vec3 pos = aOffset
            + cameraRight * rotatedPos.x * 0.05
            + cameraUp * rotatedPos.y * 0.05;

    gl_Position = projection * view * vec4(pos, 1.0);
}
