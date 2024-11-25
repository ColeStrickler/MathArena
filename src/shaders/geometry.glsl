#version 450 core
layout (points) in;
layout (line_strip, max_vertices = 4) out;

uniform mat4 view;
uniform mat4 projection;
void main()
{
    vec4 point = gl_in[0].gl_Position;

    gl_Position =  projection * view * point;
    EmitVertex();

    gl_Position = projection * view * (vec4(1.0, 0.0, 0.0, 0.0) + point);
    EmitVertex();
    EndPrimitive();


    gl_Position =  projection * view * point;
    EmitVertex();

    gl_Position =  projection * view * (vec4(0.0, 0.0, 1.0, 0.0) + point);
    EmitVertex();

    EndPrimitive();
}