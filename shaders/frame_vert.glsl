//#version 100

attribute vec4 position;
attribute vec2 uv;

varying vec2 vertex_uv;

uniform vec2 plane_size;
uniform mat4 world_matrix;
uniform mat4 model_matrix;

void main() {
	gl_Position = position;
	gl_Position.xy *= plane_size;
	gl_Position = world_matrix * model_matrix * gl_Position;

	vertex_uv = uv;
}
