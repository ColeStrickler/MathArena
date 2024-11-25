#version 450 core




// Input: each vertex has a position (vec4)
layout(location = 0) in vec3 aPos;  // The position of the point

uniform mat4 model;

// Output: passing the position to the next stage (for line rendering)
void main()
{

    vec3 FragPos = vec3(model * vec4(aPos, 1.0)); // world space * model transform gives us position of fragment
    gl_Position = vec4(FragPos, 1.0);  // Simply pass the position to the fragment shader
}
