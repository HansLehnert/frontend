//#version 100

varying vec3 position;

uniform vec3 background_color;
uniform vec3 accent_color;

void main() {
	gl_FragColor.rgb = mix(
		accent_color, background_color, clamp(position.z, 0., 1.));
	gl_FragColor.a = 1.0;
}
