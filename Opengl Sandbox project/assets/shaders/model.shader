#shader vertex
#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec3 fragPos;

void main() {
	gl_Position = u_proj * u_view * u_model * vec4(pos, 1);

	fragPos = pos;
}



#shader fragment
#version 330 core


in vec3 fragPos;
out vec4 color;

void main() {
	color = vec4(fragPos, 1);
}