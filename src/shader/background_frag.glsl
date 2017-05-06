//#version 100

varying vec3 position;

uniform vec3 bg_color;

void main() {
	vec3 back_color = bg_color * 0.1;
	vec3 front_color = pow(bg_color, vec3(0.1));//vec3(0.3) + bg_color * 0.5;
	gl_FragColor.xyz = mix(front_color, back_color, sqrt(position.z));
	gl_FragColor.w = 1.0;
}