//#version 100

attribute vec4 position;

varying vec2 mask_uv;
varying vec2 current_uv;
varying vec2 next_uv;

uniform mat4 world_matrix;
uniform mat4 model_matrix;

uniform float current_aspect;
uniform float next_aspect;

void main() {
	//gl_Position = position;
	gl_Position = world_matrix * model_matrix * position;

	mask_uv.x = position.x <= 0.0 ? 0.0 : 1.0;
	mask_uv.y = position.y <= 0.0 ? 1.0 : 0.0;

	current_uv.x = mask_uv.x;
	current_uv.y = (mask_uv.y - 0.5) * current_aspect + 0.5;

	next_uv.x = mask_uv.x;
	next_uv.y = (mask_uv.y - 0.5) * next_aspect + 0.5;
}
