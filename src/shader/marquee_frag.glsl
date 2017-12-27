//#version 100

varying vec2 mask_uv;
varying vec2 current_uv;
varying vec2 next_uv;

uniform float progress;

uniform sampler2D current_image;
uniform sampler2D next_image;
uniform sampler2D mask_image;

void main() {
	vec4 mask = texture2D(mask_image, mask_uv);

	/*if (mask.a == 0.0) {
		discard;
	}*/

	if (progress > mask.r) {
		gl_FragColor = texture2D(next_image, next_uv);
	}
	else {
		gl_FragColor = texture2D(current_image, current_uv);
	}

	gl_FragColor.a = mask.a;
}
