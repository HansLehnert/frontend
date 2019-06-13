//#version 100

varying vec2 vertex_uv;
//out vec4 out_color;

uniform sampler2D image;
uniform vec4 tint;
uniform float opacity;

void main() {
	gl_FragColor = texture2D(image, vertex_uv);
	gl_FragColor.rgb = mix(gl_FragColor.rgb, tint.rgb, tint.a);
	gl_FragColor.a *= opacity;
}
