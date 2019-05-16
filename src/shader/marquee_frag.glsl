
//#version 100

varying vec2 mask_uv;
varying vec2 previous_uv;
varying vec2 current_uv;

uniform float progress;

uniform sampler2D previous_image;
uniform sampler2D current_image;
uniform sampler2D mask_image;

void main() {
	vec4 mask = texture2D(mask_image, mask_uv);

	gl_FragColor = vec4(0);

	if (progress >= mask.r) {
		gl_FragColor = texture2D(current_image, current_uv);
	}
	else if (progress >= mask.g) {
	 	gl_FragColor = texture2D(previous_image, previous_uv);
	}

	gl_FragColor.a *= mask.b;
}
