#shader vertex
#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;


void main() {
		
	vec4 tPos = u_proj * u_view * u_model * vec4(pos, 1);
	gl_Position = tPos;

}



#shader fragment
#version 330 core

uniform vec3 color;

out vec4 oColor;

void main() {
	oColor = vec4(color, 1);
}