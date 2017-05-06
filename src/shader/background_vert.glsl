//#version 100

attribute vec4 vertex_position;

varying vec3 position;

uniform mat4 world_matrix;
uniform vec3 offset;

void main() {
	gl_Position = world_matrix * (vertex_position + vec4(offset, 1));
	gl_Position.w = 1.0 + gl_Position.z;
	position = gl_Position.xyz;
}