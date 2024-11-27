#version 450 core
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;
in vec3 Normal[];

uniform mat4 projection;


vec3 normal = vec3()

void GenerateLine(int index)
{
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position + 
                                vec4(Normal[0], 0.0) * 0.8);
    EmitVertex();
    EndPrimitive();
     
    
}


void main()
{
    GenerateLine(0);
}