//#version 100

attribute vec4 position;
attribute vec2 uv;

varying vec2 mask_uv;
varying vec2 previous_uv;
varying vec2 current_uv;

uniform mat4 world_matrix;
uniform mat4 model_matrix;

uniform float progress;

uniform float previous_aspect_ratio;
uniform float current_aspect_ratio;

void main() {
	gl_Position = world_matrix * model_matrix * position;

	mask_uv = uv;

	previous_uv = uv;
	previous_uv.y = (previous_uv.y - 0.5) / previous_aspect_ratio + 0.5;

	current_uv = uv;
	current_uv.y = (current_uv.y - 0.5) / current_aspect_ratio + 0.5;
}
