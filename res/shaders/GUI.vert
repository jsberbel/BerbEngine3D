//The fragment shader operates on each pixel in a given polygon
#version 330 core
 
layout (location = 0) in vec4 vertex;
out vec2 fragUV;

uniform mat4 projection;
 
void main()
{
	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    fragUV = vec2(vertex.z, 1-vertex.w);
}