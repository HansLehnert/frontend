//#version 100

attribute vec4 position;
attribute vec2 uv;

varying vec2 vertex_uv;

uniform mat4 world_matrix;
uniform mat4 model_matrix;

void main() {
	//gl_Position = position;
	gl_Position = world_matrix * model_matrix * position;

	vertex_uv = uv;
}
