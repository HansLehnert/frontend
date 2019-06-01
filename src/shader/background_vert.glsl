//#version 100

attribute vec4 vertex_position;

varying vec3 position;

uniform mat4 model_matrix;
uniform mat4 world_matrix;
uniform vec3 offset;

void main() {
	gl_Position = vertex_position + vec4(offset, 0.);
	gl_Position = world_matrix * model_matrix * gl_Position;
	gl_Position.w = 1.0 + gl_Position.z;
	position = gl_Position.xyz;
}
